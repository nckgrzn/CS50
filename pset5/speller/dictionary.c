/**
 * Implements a dictionary's functionality.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "dictionary.h"

// create global node structure for a trie
typedef struct node
{
    bool is_word;
    struct node* children[27];
}
node;


// create root node
node* root;

// create node to traverse through the trie with
node* trav;

// create global integer for number of words in dictionary
int wordno = 0;

// declare prototype for function to recursively unload dictionary
void freenodes(node* ptr);


/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // dedicate memory for root node and set to NULL (0)
    root = malloc(sizeof(node));
    memset(root, 0, sizeof(node));

    // open dictionary
    FILE *dict = fopen(dictionary, "r");
    if(dict == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return 1;
    }

    // create an array used to index into words in dictionary
    char word[LENGTH + 1];

    // iterate through words in dictionary
    while(fscanf(dict, "%s\n", word) > 0)
    {
        // set traversal node equal to root node
        trav = root;

        // increase word count
        wordno++;

        // iterate through characters in dictionary words
        for(int i = 0; word[i] != '\0'; i++)
        {

            // turn character into standardized integer used to index into trie nodes
            if(isalpha(word[i]))
            {
                int a = tolower(word[i]) - 97;

                // iterate through trie to map dictionary words, creating new nodes where necessary
                if (trav->children[a] == NULL)
                {
                    trav->children[a] = malloc(sizeof(node));
                    memset(trav->children[a], 0, sizeof(node));
                    trav = trav->children[a];
                }
                else trav = trav->children[a];
            }
        }

        // set a marker to indicate nodes that represent the end of a word
        trav->is_word = true;
    }

    // close dictionary file after loading
    fclose(dict);
    return true;
}


/**
 * Returns true if word is in dictionary else false.
 */

bool check(const char *word)
{
    // set traversal node to root node
    trav = root;

    // iterate through characters in text file words
    for(int i = 0; word[i] != '\0'; i++)
    {
            // turn character into standardized integer used to index into trie nodes
            if (isalpha(word[i]))
            {
                int a = tolower(word[i]) - 97;

                // go to corresponding element in children; if NULL, word is misspelled
                if(trav->children[a] == NULL)
                {
                    return false;
                }

                // if not NULL, move to next letter
                else trav = trav->children[a];
            }
    }

    // return node marker that indicates the end of a word to display whether or not the word is misspelled/in the dictionary
    return trav->is_word;
}


/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // return numbers of dictionary words counted in the load function
    return wordno;
}

void freenodes(node* ptr)
{
    // iterate through children in a node
    for(int i = 0; i < 27; i++)
        {
            // if child is pointing to another node, go to it and reset loop
            if(ptr->children[i] != NULL)
            {
                freenodes(ptr->children[i]);
            }
        }

    // free node after iterating through all of its children
    free(ptr);
}


/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // call recursive node unloading function
    freenodes(root);
    return true;
}