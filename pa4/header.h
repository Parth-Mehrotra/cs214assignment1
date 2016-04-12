#ifndef SORTED_LIST_H
#define SORTED_LIST_H
/*
 * sorted-list.h
 */

#include <stdlib.h>

/*
 * Sorted list type that will hold all the data to be sorted.
 */
struct Node {
	char* filePath;
	int frequency;
	struct Node* next;
};
typedef struct Node* NodePtr;

typedef struct hashNode{
	char* key;
	NodePtr ptr;
	struct hashNode *next;
} hashNodeT;
typedef hashNodeT* hashNodePtr;

typedef struct hashtable{
	int size;
	int numEntries;
	int numUniqueEntries;
	struct hashNode** table;	
} hashTableT;
typedef hashTableT* hashTablePtr;

struct Token_ {
    char* string;
    char* type;
    struct Token_* next;
};
typedef struct Token_ Token;

struct TokenizerT_ {
    char* inputString;
    Token* head;
    Token* current;
};
typedef struct TokenizerT_ TokenizerT;

/* Create a new hashtable. */
hashTablePtr createTable(int size);

unsigned int hash(hashTablePtr hashtable, char* key);

hashNodePtr createHashNode(char* key);

/* Insert a key-value pair into a hash table. */
void insert(hashTablePtr hashtable, char* key, char* filePath);

int SLInsert(hashNodePtr hashNode, char* filePath);

// Returns a sorted array of all the elements of a hashtable
char** getSortedList(hashTablePtr hashtable);

// Returns filePath and frequency data of a given string
NodePtr getListOfFiles(hashTablePtr hashtable, char* key);

TokenizerT* TKCreate(char* filePath);

void TKDestroy(TokenizerT*);

// Inserts all the strings in a file into a hashtable
void hashFile(hashTablePtr hashtable, char* filePath);

// Parses filePath and outputs fileName
char* parseFilePath(char* filePath);
#endif
