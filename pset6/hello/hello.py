from cs50 import get_string


def hello():
    name = get_string("What is your name?\n") #asks user to input their name
    print(f"hello, {name}")


hello()