import os
import datetime
import itertools
from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():

    # this query will group all the common stocks together and automatically sums up the stocks, giving a final neat query result
    stocks = db.execute("SELECT stock, SUM(shares) FROM buy WHERE id=:id GROUP BY stock", id=session['user_id'])

    # initializing the total value sum
    total = 0

    # iterating through each stock, which is just a list of dictionaries
    for stock in stocks:
        symbol = stock["stock"]
        name = lookup(stock["stock"])['name']
        shares = stock['SUM(shares)']
        price = lookup(stock["stock"])['price']
        total_value = price * shares
        total += total_value

        # updating portfolio everytime index page is loaded due to fluctuating stock prices
        db.execute("UPDATE portfolio SET id=:id, symbol=:symbol, name=:name, shares=:shares, price=:price, total_value=:total_value WHERE symbol=:symbol2",
                 id=session['user_id'], symbol=symbol, name=name, shares=shares, price=usd(price), total_value=usd(total_value), symbol2=symbol)

        # delete stocks (rows) from portfolio db which contain zero shares. no point of wasting space in the db to store them, no?
        # originally I had thought of implementing an if statement (thx to jinja) within the index.html but thought that'd be overkill
        # why keep wasted space within the db amirite? this seemed like a more optimal solution
        for stock in stocks:
            if stock['SUM(shares)'] == 0:
                db.execute("DELETE FROM portfolio WHERE symbol=:symbol", symbol=stock['stock'])

    stocks_updated = db.execute("SELECT * FROM portfolio WHERE id=:id", id=session['user_id'])
    current_cash = db.execute("SELECT cash FROM users WHERE id=:id", id=session['user_id'])[0]['cash']
    cash_and_total_values = current_cash + total
    return render_template("index.html", stocks_updated=stocks_updated, current_cash=usd(current_cash), cash_and_total_values=usd(cash_and_total_values))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "POST":
        # if symbol field is blank
        # *** IMPORTANT NOTE *** WHY DOES IT NOT WORK WHEN I TYPE IF NOT 'variable name' where the variable is request.form.get
        if not request.form.get("symbol"):
            return apology("MUST NOT LEAVE SYMBOL FIELD BLANK")

        # checks if symbol is a valid stock symbol
        elif not lookup(request.form.get("symbol")):
            return apology("MUST SUBMIT VALID STOCK SYMBOL")

        # if shares field is blank
        elif not request.form.get("shares"):
            return apology("MUST NOT LEAVE SHARES FIELD BLANK")

        # checks if shares value is positive
        elif int(request.form.get("shares")) < 1:
            return apology("MUST SUBMIT POSITIVE NUMBER OF SHARES")

        # amount of cash from current user
        user_cash = db.execute("SELECT cash FROM users WHERE id=:id", id=session['user_id'])[0]['cash']

        # checks whether user has enough cash to purchase amount of stocks
        stock_price = lookup(request.form.get("symbol"))['price'] * int(request.form.get("shares"))
        if user_cash < stock_price:
            return apology("NOT ENOUGH FUNDS")

        # perform purchase
        elif user_cash > stock_price:
            cash_amount = user_cash - stock_price
            db.execute("UPDATE users SET cash=:updated_cash WHERE id=:id", updated_cash=cash_amount, id=session['user_id'])

            #EST for me
            time = datetime.datetime.now()-datetime.timedelta(hours=4)
            rounded_time_to_nearest_sec = str(time - datetime.timedelta(microseconds=time.microsecond))
            db.execute("INSERT INTO buy (id, stock, price, shares, dayandtime, action) VALUES (:id, :stock, :price, :shares, :dayandtime, :action)",
            id=session['user_id'], stock=lookup(request.form.get("symbol"))['symbol'], price=lookup(request.form.get("symbol"))['price'],
            shares=int(request.form.get("shares")), dayandtime=rounded_time_to_nearest_sec, action='BOUGHT')

            # inserting few values into portfolio db to set it up before the index function updates it
            db.execute("INSERT INTO portfolio (id, symbol, name) VALUES (:id, :symbol, :name)", id=session['user_id'],
                       symbol=lookup(request.form.get("symbol"))['symbol'], name=lookup(request.form.get("symbol"))['name'])

            flash('Bought!')
            return redirect("/buy")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    history = db.execute("SELECT * FROM buy WHERE id=:id", id=session['user_id'])
    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():

    #user submits whatever symbol via the form
    if request.method == "POST":
        stock_symbol = lookup(request.form.get("symbol"))

        # checks if valid stock symbol exists
        if not stock_symbol:
            return apology("invalid stock symbol", 403)

        stock_name = stock_symbol["name"]
        stock_price = stock_symbol["price"]
        stock_symbol_2 = stock_symbol["symbol"]
        return render_template("quoted.html", stock_name=stock_name, stock_price=stock_price, stock_symbol_2=stock_symbol_2)

    # user wants to GET to the quote page
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":

        # gets list of dictionaries of usernames from database
        username_list = db.execute("SELECT username FROM users")

        #create list of just solely usernames minus the dictionary aspect
        user_list = []
        for row in username_list:
            user_list.append(row["username"])

        # ensures username is submitted and whether username doesn't exist already in the database
        username = request.form.get("username")
        if not username:
            return apology("must provide username", 403)
        elif username in user_list:
            return apology("username already exists", 403)

        # ensures password is submitted (and is alphanumeric) and both password and confirm password fields match
        elif request.form.get("password").isalpha() == True:
            return apology("must input alphanumeric password")
        elif not request.form.get("password"):
            return apology("must provide password", 403)
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords don't match", 403)

        # creates hash for password
        password = request.form.get("password")
        hash_brown = generate_password_hash(password, method='pbkdf2:sha256', salt_length=8)

        # inputs username and hashed password into database
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hashh)", username=username, hashh=hash_brown)

        #redirects them to login page after successful registration
        return redirect("/login")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    stocks = db.execute("SELECT symbol FROM portfolio WHERE id=:id", id=session['user_id'])

    if request.method == "POST":

        shares = request.form.get("shares")
        symbol = request.form.get("stock")

        # if stock picked is not owned by the user for any reason whatsoever
        stocks_list = []
        for stock in stocks:
            stocks_list.append(stock['symbol'])
        if request.form.get("stock") not in stocks_list:
            return apology("MUST OWN SHARES OF THAT STOCK")

        # checking if valid stock input
        if not request.form.get("stock"):
            return apology("SELECT VALID STOCK")

        # if shares is not a positive int
        elif shares.isnumeric() == False:
            return apology("MUST ENTER POSITIVE INTEGER")

        #if selected shares amount is greater than what the user actually owns. the db.exec returns a dict within a list
        shares_owned = db.execute("SELECT shares FROM portfolio where id=:id AND symbol=:symbol", id=session['user_id'], symbol=symbol)
        num_of_shares = shares_owned[0]['shares']
        if int(shares) > num_of_shares:
            return apology("SHARES PICKED EXCEEDS THE AMOUNT OF SHARES YOU OWN")

        #sell the stock
        time = datetime.datetime.now()-datetime.timedelta(hours=4)
        rounded_time_to_nearest_sec = str(time - datetime.timedelta(microseconds=time.microsecond))
        db.execute("INSERT INTO buy (id, stock, price, shares, dayandtime, action) VALUES (:id, :stock, :price, :shares, :dayandtime, :action)",
        id=session['user_id'], stock=request.form.get("stock"), price=lookup(request.form.get("stock"))['price'],
        shares=int(request.form.get("shares")) * -1, dayandtime=rounded_time_to_nearest_sec, action='SOLD')

        # update cash amount (add to cash the amount sold)
        stock_price = lookup(request.form.get("stock"))['price'] * int(shares)
        user_cash = db.execute("SELECT cash FROM users WHERE id=:id", id=session['user_id'])[0]['cash']
        cash_amount = user_cash + stock_price
        db.execute("UPDATE users SET cash=:updated_cash WHERE id=:id", updated_cash=cash_amount, id=session['user_id'])
        flash('Sold!')
        return redirect("/sell")


    else:
        return render_template("sell.html", stocks=stocks)

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
