#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

void decrypt(string p, int k_int, char c);

int main(int argc, string argv[])
{
    if ((argc == 1) || (argc > 2)) //checks if there's two command line arguments or not
    {
        printf("Error: Please enter in one positive integer after the program execution command.\n");
        return 1;
    }

    for (int i = 0; i < strlen(argv[1]); i++)
    {
        string k = argv[1]; //implementation to pass the arg to a variable for ez iteration
        if ((isdigit(k[i])) == 0)
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }

    int k_int = atoi(argv[1]) % 26; //counters if k is greater than 26 (only 26 letters in alphabet)
    string p = get_string("plaintext: "); //plaintext
    char c = '\0'; //ciphertext

    printf("ciphertext: ");
    decrypt(p, k_int, c);

    return 0;
}

void decrypt(string p, int k_int, char c)
{
    for (int i = 0; i < strlen(p); i++)
    {
        if (isalpha(p[i]) != 0)
        {
            if (islower(p[i]) != 0) //checks if lowercase character since those ASCII values are different than uppercase
            {
                c = ((p[i] - 'a' + k_int) % 26) + 'a'; //subtract by lowercase a
            }
            else
            {
                c = ((p[i] - 'A' + k_int) % 26) + 'A'; //subtract by uppercase A
            }
        }
        else
        {
            c = p[i]; //keeps punctuation as is
        }
        printf("%c", c);
    }
    printf("\n");
}