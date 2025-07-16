// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Keeps track of the word count in in the dictionary
int total_word_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    node *current = table[hash(word)];
    while (current != NULL)
    {
        if (strcasecmp(current->word, word) == 0)
        {
            return true;
        }

        current = current->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned long hash = 0;
    int c;

    while ((c = *word++))
    {
        hash = (hash << 5) + toupper(c);
    }

    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    char word[LENGTH + 1];

    while (fgets(word, sizeof(word) + 1, file))
    {
        for (int i = 0; i < strlen(word); i++)
        {
            if (word[i] == '\n')
            {
                word[i] = '\0';
                break;
            }
        }

        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            return false;
        }

        strcpy(new_node->word, word);

        int hash_index = hash(word);

        new_node->next = table[hash_index];

        table[hash_index] = new_node;

        total_word_count++;
    }

    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return total_word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *current = table[i];
        while (current != NULL)
        {
            node *tmp = current;

            current = current->next;

            free(tmp);
        }
    }

    return true;
}
