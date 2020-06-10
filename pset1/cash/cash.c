#include <stdio.h>
#include <cs50.h>
#include <math.h>
int main(void)
{
    float change;
    int quarters;
    int dimes;
    int nickels;
    int pennies;
    int i = 0; //counter variables
    int j = 0;
    int k = 0;
    int l = 0;
    do
    {
        change = get_float("How much change is owed? Note: Please enter in the dollar amount without the dollar sign ($). "); //inputs for change
    }
    while (change < 0); //re-prompt if value is negative
    int cents = round(change * 100); //converts to cents
    //printf("%d\n", cents);

    //int i = 0; //only need to do quarter fcn once
    do //quarter fcn
    {
        i++; //stops quarter fcn by incrementing it to 1.
        quarters = floor(cents / 25); //round down because we want the least whole integer
        //printf("%d\n", quarters); uncomment for debugging purposes. shows the total amount of respective coin
    }
    while (i < 1); //loop stops executing now that i = 1

    //int j = 0; //only need to do dime fcn once
    do //dime fcn
    {
        j++; //stops dime fcn
        dimes = floor(((cents - (quarters * 25)) / 10));
        //printf("%d\n", dimes);
    }
    while (j < 1);

    //int k = 0; //only need to do nickel fcn once
    do //nickel fcn
    {
        k++; //stops nickel fcn
        nickels = floor((cents - ((quarters * 25) + (dimes * 10))) / 5);
        //printf("%d\n", nickels);
    }
    while (k < 1);

    //int l = 0; //only need to do pennies fcn once
    do //pennies fcn
    {
        l++; //stops pennies fcn
        pennies = floor(((cents - ((quarters * 25) + (dimes * 10) + (nickels * 5))) / 1));
        //printf("%d\n", pennies);
    }
    while (l < 1);

    int coins = quarters + dimes + nickels + pennies;
    printf("%d\n", coins);
}