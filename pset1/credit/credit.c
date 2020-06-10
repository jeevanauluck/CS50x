#include <stdio.h>
#include <cs50.h>
#include <math.h>
int main(void)
{
    long number;
    do
    {
        number = get_long("Enter card number: ");
    }
    while ((floor(log10(number) + 1)) !=13 && (floor(log10(number) + 1)) !=15 && (floor(log10(number) + 1)) !=16); //(ceil((log10(number))) != 2);
    
    m
}