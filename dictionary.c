/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 *
 */

#include <stdbool.h>

#include "dictionary.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// function for char indexing
int alphabet(int letter);

// function to unload recursive
void unloader(node* root);

// word counting global
int words = 0;

// function to create nodes, takes no value and returns "reseted NULL" node
void *new_node(void);

// global declaration of root structure to the node tree
node* root;


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    
    // "traveller" pointer that points to the root
    node* ptrav = root;
    
    // variable for looping
    int word_lenght = strlen(word);
    
    // iterate throught the word lenght
    for(int i = 0; i < word_lenght; i++)
    {
        // function call to return number for alphabet array
        int letter = alphabet(word[i]);

        // if letter is not on dictionary return false        
        if(ptrav->children[letter] == NULL)
            return false;
                
        // keep travelling til end of the word
        ptrav = ptrav->children[letter];
                
    }
    // return end of the word, true if is a word.
    return ptrav->is_word;
    
}


/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // Opens the file and associate with a stream
    FILE* fp = fopen(dictionary, "r");
    
    // Check if could open or file is valid
    if(!fp)
    {
        printf("Error\n");
        return false;
    }
    
        
    // variables declaration    
    int index = 0;
    char word[LENGTH+1];
    
    // create root node
    root = new_node();

    // create "traveller" pointer that points to the root
    node* ptrav = root;
    
    //interate throught every letter of the given file stream
    for (int c = fgetc(fp); c != EOF; c = fgetc(fp))
    {
        // allow only alphabetical characters and apostrophes
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            // append character to word
            word[index] = c;
            index++;
        }

        // found a whole word
        else if (index > 0)
        {
            // terminate current word
            word[index] = '\0';

            // update word counter declared globally
            words++;
            
            // pointing to the root
            ptrav = root;
            
            // variable for looping
            int word_length = strlen(word);
            
            //creating node tree
            for(int i = 0; i < word_length; i++)
            {
                // function call to return number for alphabet array
                int letter = alphabet(word[i]);
                
                // checks if trav.children[] points to a known letter in dictionary
                if(ptrav->children[letter] == NULL)
                {
                    // creates a new node for new letter in children array and points to it
                    ptrav->children[letter] = new_node();
                }
                
                // keep travelling til end of the word
                ptrav = ptrav->children[letter];
            }
            
            // reached end of the word
            ptrav->is_word = true;

            // prepare for next word
            index = 0;
        }
    }
    
    // close file stream
    fclose(fp);
    
    return true;
   
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // globaly declared and incresead during loading
    return words;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // call to unload function on root's dictionary
    unloader(root);
    
    return true;
}



// Function that checks if letter is from alphabet[a-z] plus apostrophe. If yes returns number to children array [0-26]
int alphabet(int letter)
{
    
    if(letter == 39)
        return 26;
    else if(letter > 96 && letter < 123)
        return tolower(letter) - (int)'a';
    else if(letter > 64 && letter < 91)
        return tolower(letter) - (int)'a';
    else
        return -1;
        
}

// Function to create a new node, allocates memory and set bool is_word value to false
void *new_node(void)
{
    // allocates memory for the new node (calloc didn't raised errors like malloc with valgrind)
    node* new_node = calloc(1,sizeof(node));
    
    // check if have enough memory, if not exit and raise standard error
    if(new_node)
    {
        new_node->is_word = false;
    }
    else
    {
        fprintf(stderr, "calloc failed\n");
        exit(EXIT_FAILURE);
    }
    
    return new_node;
    
}
// Recursive function to unload dictionary. Iterate on every node children/letter of root, if not null go deeper in another children/letter.
void unloader(node* root)
{
    // iterate throught children [27 letters] if we have a letter call again to check inside for another children[]
    for(int i = 0; i < 27; i++)
        if(root->children[i] != NULL)
            unloader(root->children[i]);
    
    // free your roots! 
    free(root);
    
    
}