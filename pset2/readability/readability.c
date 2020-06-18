#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int letter_counter(string text);
int word_counter(string text);
int sentence_counter(string text);
void coleman_liau_formula(string text);

int main(void)
{
    string text = get_string("Text: ");
    //letter_counter(text); uncommented or commented seems to both work
    //word_counter(text);
    //sentence_counter(text);
    coleman_liau_formula(text);
    return 0;
}

// letter counter
int letter_counter(string text)
{
    int letter = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (isalpha(text[i]) != 0)
        {
            letter++;
        }
    }
    return letter;
}

// word counter
int word_counter(string text)
{
    int spaces = 1;
    for (int i = 0; i < strlen(text); i++)
    {
        if (isspace(text[i]) != 0)
        {
            spaces++;
        }
    }
    return spaces;
}

// sentence counter
int sentence_counter(string text)
{
    int sentences = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == '.' || text[i] == '!'
            || text[i] == '?') //one flaw is that an occurence such as ellipsis would increase the sentence counter
        {
            sentences++;
        }
    }
    return sentences;
}

void coleman_liau_formula(string text)
{
    float L = ((float) letter_counter(text) / (float) word_counter(text)) * 100;
    float S = ((float) sentence_counter(text) / (float) word_counter(text)) * 100;
    int index = round((0.0588 * L) - (0.296 * S) - 15.8);

    if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}