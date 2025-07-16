#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int calculate_score(string w);

int main(void)
{
    string player1Input = get_string("Player 1: ");
    string player2Input = get_string("Player 2: ");

    int player1Score = calculate_score(player1Input);
    int player2Score = calculate_score(player2Input);

    if (player1Score > player2Score)
    {
        printf("Player 1 wins!\n");
    }
    else if (player1Score < player2Score)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int calculate_score(string word)
{
    const int alphabetLength = 26;

    char letters[alphabetLength] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
                                    'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                                    's', 't', 'u', 'v', 'w', 'x', 'y', 'x'};

    int points[alphabetLength] = {1, 3, 3, 2,  1, 4, 2, 3, 1, 8, 5, 1, 3,
                                  1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

    int score = 0;

    for (int i = 0, l = strlen(word); i < l; i++)
    {
        for (int j = 0; j < alphabetLength; j++)
        {
            if (tolower(word[i]) == letters[j])
            {
                score += points[j];
            }
        }
    }

    return score;
}
