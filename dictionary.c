// Implements a dictionary's functionality
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = (LENGTH + 1) * 'z';

// Hash table
node *table[N];

// Define variable to keep track of size of dictionary
int total = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // case-insensitive
    // allow for apostrophes

    // 1. Hash word to obtain hash value
    int index = hash(word);
    // 2. Access linked list at that index in the hash table
    // Set up variable cursor which is a pointer to the first element in the list
    node *cursor = table[index];
    // 3. Traverse linked list, looking for the word using the function strcasecmp()
    while (cursor != NULL)
    {
        // if you find word that you're looking for
        if (strcasecmp(cursor -> word, word) == 0)
        {
            // return true for words actually in dictionary
            return true;
        }
        else {
            // Move the cursor to the next node in the linked list
            cursor = cursor -> next;
        }
    }

    // if you've reached the end of the linked list (cursor = NULL), then stop and return false
    return false;
}

// Hashes word to a number
// takes word as input and outputs a number according to which "bucket" to store the word in
unsigned int hash(const char *word)
{
    int sum = 0;
    // Use math to calculate sum of ASCII values
    for (int i = 0; i < strlen(word); i++)
    {
        sum += tolower(word[i]);
    }
    // return 0 through N-1 inclusive corresponding to that letter
    return (sum % N);
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // 1. Open dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Unable to open dictionary\n");
        return false;
    }

    // 2. Read strings from file one at a time
    // define word variable
    char word[LENGTH + 1];
    // Keep running as long as not end of file
    while (fscanf(file, "%s", word) != EOF)
    {
        // 3. Create a new node for each word
        // Allocate memory for new node
        node *n = malloc(sizeof(node));
        // Always check if n is NULL before proceeding. If it is NULL, stop, because something went wrong, or you're out of memory
        if (n == NULL)
        {
            printf("n was NULL. Something went wrong here\n");
            return false;
        }
        // Go into node n, and go into its word field, and assign it the value from the dictionary
        strcpy(n -> word, word);
        // Go into node n, and go into its next field, and assign the value NULL
        n -> next = NULL;

        // 4. Use Hash Function
        int index = hash(word);

        // 5. Insert node into hash table at location
        if (table[index] == NULL)
        {
            table[index] = n;
        }
        else
        {
            n -> next = table[index];
            table[index] = n;
        }
        // Increment word count after each successful load into dictionary
        total++;
    }
    // Close file to avoid leaky memory
    fclose(file);
    // return true if dictionary successfully loads into memory
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return total;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Frees memory by giving back the memory used for the first node and creates temporary variable to free memory for subsequent nodes
    for (int i = 0; i < N; i++)
    {
        node *head = table[i];
        node *cursor = head;
        node *tmp = head;
        while (cursor != NULL)
        {
            cursor = cursor -> next;
            free(tmp);
            tmp = cursor;
        }
    }
    return true;
}
