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
	struct hashNode** table;	
} hashTableT;
typedef hashTableT* hashTablePtr;

/* Create a new hashtable. */
hashtablePtr createTable(int size);

/* Hash a string for a particular hash table. */
int hash(hashTablePtr hashtable, char* key);

// Create new hashNode
hashNodePtr createHashNode(char* key);

/* Insert a key-value pair into a hash table. */
void insert(hashTablePtr hashtable, char* key, char* filePath);

// Insert an entry in the sorted list
int SLInsert(hashNodePtr hashNode, char* filePath);

char** getSortedList(hashTablePtr hashtable);

#endif
