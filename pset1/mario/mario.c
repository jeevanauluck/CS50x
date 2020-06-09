#include <stdio.h>
#include <cs50.h>
int main(void)
{
    int n;
    do
    {
        n = get_int("Height: ");
    }
    while ((n < 1) || (n > 8));
    for (int i = 0; i < n; i++) //loop for printing a new line
    {
        for (int j = n - i - 1; j > 0; j--) //loop for printing spaces left half
        {
            printf(" ");
        }
        for (int k = 0; k < i + 1; k++) //loop for printing hashes left half
        {
            printf("#");
        }
        for (int l = 0; l < 1; l++) //loop for printing spaces right half
        {
            printf("  ");
        }
        for (int m = 0; m < i + 1; m++) //loop for printing hashes right half
        {
            printf("#");
        }
        printf("\n");
    }
}