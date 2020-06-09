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
        for (int j = n - i - 1; j > 0; j--) //loop for printing spaces
        {
            printf(" ");
        }
        for (int k = 0; k < i + 1; k++) //loop for printing hashes
        {
            printf("#");
        }
        printf("\n");
    }
}