#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

string encrypt(string v, string k);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Missing key argument!\n");
        return 1;
    }

    string key = argv[1];

    int keylength = strlen(key);

    if (keylength != 26)
    {
        printf("Key must be %i characters!\n", 26);
        return 1;
    }

    for (int i = 0; i < keylength; i++)
    {
        if (!isalpha(key[i]))
        {
            printf("Key must only consist of alpha numeric characters!\n");
            return 1;
        }

        for (int j = 0; j < keylength; j++)
        {
            if (i != j)
            {
                if (tolower(key[i]) == tolower(key[j]))
                {
                    printf("Key must only contain unique characters!\n");
                    return 1;
                }
            }
        }
    }

    string plaintext = get_string("plaintext: ");

    string encrypted = encrypt(plaintext, key);

    printf("ciphertext: %s\n", encrypted);
}

string encrypt(string value, string key)
{
    for (int i = 0, l = strlen(value); i < l; i++)
    {
        if (isalpha(value[i]))
        {
            bool isLower = islower(value[i]);

            int asciiIndex = value[i] - (isLower ? 'a' : 'A');

            value[i] = isLower ? tolower(key[asciiIndex]) : toupper(key[asciiIndex]);
        }
    }

    return value;
}
