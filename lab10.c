#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct Trie
{
    int isWord;
    struct Trie *next[26];
};

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    if (word[0] != '\0')
    {
        int index = word[0] - 'a';
        if (pTrie->next[index] == NULL)
            pTrie->next[index] = (struct Trie *)calloc(1, sizeof(struct Trie));
        word++;
        insert(pTrie->next[index], word);
    }
    else
        pTrie->isWord += 1;
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    int index = word[0] - 'a';

    if (word[0] != '\0' && pTrie->next[index] == NULL)
        return 0;
    else if (word[0] == '\0')
        return pTrie->isWord;
    else
    {
        word++;
        return numberOfOccurances(pTrie->next[index], word);
    }
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    for (int i = 0; i < 26; i++)
    {
        if (pTrie->next[i] != NULL)
            deallocateTrie(pTrie->next[i]);
    }

    free(pTrie);
    return NULL;
}

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie *new_trie = (struct Trie *)calloc(1, sizeof(struct Trie));
    // struct Trie *new_trie = (struct Trie *)malloc(sizeof(struct Trie));
    // for (int i = 0; i < 26; i++)
    //     new_trie->next[i] = NULL;
    return new_trie;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    int num;

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("FAILURE: \"%s\" did not open!\n", filename);
        exit(1);
    }
    else
        printf("SUCCESS: \"%s\" opened! MODE: \"%s\"\n", filename, "r");

    fscanf(file, "%d", &num);

    char word[120];

    for (int i = 0; i < num; i++)
    {
        fscanf(file, "%s", word);
        pInWords[i] = calloc(strlen(word) + 1, sizeof(char));
        strcpy(pInWords[i], word);
    }

    return num;
}

int main(void)
{
    char *inWords[256];

    // read the number of the words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    // parse lineby line, and insert each word to the trie data structure
    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }
    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
        printf("There is an error in this program\n");
    return 0;
}