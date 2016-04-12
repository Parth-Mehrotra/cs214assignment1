/*
 * sorted-list.c
 */

#include <stdlib.h>
#include "header.h"

int SLInsert(hashNodePtr hashNode, char* filePath) {
	if (hashNode->ptr == NULL) {
		hashNode->ptr = (NodePtr) malloc(sizeof(struct Node));
		hashNode->ptr -> filePath = filePath;
		hashNode->ptr -> frequency = 1;
		hashNode->ptr -> next = NULL;
		return 1;
	}

	NodePtr temp = hashNode->ptr;
	NodePtr prev = NULL;

	while(temp != NULL && temp->filePath != filePath)
	{
		prev = temp;
		temp = temp->next;
	}

	if(temp == NULL)
	{
		prev->next = (NodePtr) malloc(sizeof(struct Node));
		prev->next->filePath = filePath;
		prev->next->frequency = 1;
		prev->next->next = NULL;
	}
	else
	{
		temp->frequency++;
		if(prev != NULL && prev->frequency < temp->frequency)
		{
			prev->next = temp->next;
			NodePtr trav = hashNode->ptr;
			if(trav->frequency < temp->frequency)
			{
				temp->next = trav;
				hashNode->ptr = temp;
				return 1;
			}
			while(trav->next != NULL && trav->next->frequency > temp->frequency)
			{
				trav = trav->next;
			}
			temp->next = trav->next;
			trav->next = temp;
			return 1;
		}
	}
	return 1;
}
