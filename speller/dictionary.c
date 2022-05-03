// Implements a dictionary's functionality

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

bool traverse_nodes(node *n, const char *word);

// TODO: Choose number of buckets in hash table
const unsigned int N = 10000;

// Hash table
node *table[N];

int word_count = 0;
bool loaded;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int index = hash(&word[0]);

    if (table[index] == NULL)
    {
        return false;
    }

    node *checker = table[index];

    if (checker == NULL)
    {
        return false;
    }

    if (traverse_nodes(checker, word))
    {
        return true;
    }

    return false;
}

bool traverse_nodes(node *n, const char *word)
{
    if (strcasecmp(n->word, word) == 0)
    {
        return true;
    }

    if (n->next == NULL)
    {
        return false;
    }

    return traverse_nodes(n->next, word);
}
// Hashes word to a number
unsigned int hash(const char *word)
{
    // Calculating length once for speed
    int len = strlen(word);

    int index = 0;

    // If a word is longer than 5 letters, we take the first 3 and do random math to find index point
    // I just messed around with values until I got something faster
    if (len > 5)
    {
        for (int i = 0; i < 3; i++)
        {
            index += (toupper(word[i]) % 27 * 6 + 3);
        }

        return index;
    }

    return toupper(word[0]) % 27;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Load dictionary file
    FILE *in_dict = fopen(dictionary, "r");

    if (in_dict == NULL)
    {
        return false;
    }

    // Allocate char array for string with size of max length
    char buffer[LENGTH + 1];

    // Declare new char for reading char one at a time
    int j = 0;

    // int variable for accessing hash index
    int i = 0;

    // Read string from file 1 at a time
    while (fscanf(in_dict, "%s", buffer) != EOF)
    {
        // Create new node for each string
        node *n = malloc(sizeof(node));

        if (n == NULL)
        {
            return false;
        }

        // Set next and word from the string
        n->next = NULL;
        strcpy(n->word, buffer);

        // Use hash function to find index to place head of linked list
        i = hash(buffer);

        // If there is no head node, the new node becomes the head
        if (table[i] == NULL)
        {
            table[i] = n;
            word_count++;
        }

        // Otherwise set new node->next to be equal to the head node, then put new node as the head
        else
        {
            n->next = table[i];
            table[i] = n;
            word_count++;
        }

    }

    fclose(in_dict);

    loaded = true;
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if (loaded)
    {
        return word_count;
    }

    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Loop over the array of nodes
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            // If there is a head node, make a copy of that node, and another copy of the next node
            node *temp = table[i];

            while (temp->next != NULL)
            {
                // Iterate over all nodes in linked list and free them making sure not to orphan any nodes
                node *checker = temp->next;
                free(temp);
                temp = checker;
            }

            // Free the final node
            free(temp);
        }
    }

    return true;
}

