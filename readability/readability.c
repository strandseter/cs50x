#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

void print_result(int i);
int calculate_index(int l, int w, int s);
int count_letters(string s);
int count_words(string s);
int count_sentences(string s);

int main(void)
{
    string input = get_string("Text: ");

    int letters = count_letters(input);
    int words = count_words(input);
    int sentences = count_sentences(input);

    int index = calculate_index(letters, words, sentences);

    print_result(index);
}

void print_result(int index)
{
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

int calculate_index(int letters, int words, int sentences)
{
    float L = (float) letters / (float) words * 100;
    float S = (float) sentences / (float) words * 100;

    float index = (0.0588 * L) - (0.296 * S) - 15.8;

    int rounded = (int) round(index);

    return rounded;
}

int count_letters(string word)
{
    int count = 0;

    for (int i = 0, l = strlen(word); i < l; i++)
    {
        if (isalpha(word[i]))
        {
            count++;
        }
    }

    return count;
}

int count_words(string word)
{
    int count = 0;

    for (int i = 0, l = strlen(word); i < l; i++)
    {
        if (word[i] == ' ')
        {
            count++;
        }
    }

    return count + 1;
}

int count_sentences(string word)
{
    const int endings_length = 3;
    const char endings[endings_length] = {'.', '!', '?'};

    int count = 0;

    for (int i = 0, l = strlen(word); i < l; i++)
    {
        for (int j = 0; j < endings_length; j++)
        {
            if (word[i] == endings[j])
            {
                count++;
            }
        }
    }

    return count;
}
