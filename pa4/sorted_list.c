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

	while(temp != NULL && strcmp(temp->filePath, filePath) != 0)
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
		temp = prev->next;
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
				prev = NULL;
			}
			else
			{
				while(trav->next != NULL && trav->next->frequency > temp->frequency)
				{
					trav = trav->next;
				}
				temp->next = trav->next;
				trav->next = temp;
				prev = trav;
			}
		}
	}

	// If elements of the same frequency are not sorted alphabetically, fix it.
	if(prev != NULL && strcmp(prev->filePath, temp->filePath) > 0)
	{
		int myFrequency = temp->frequency;
		prev->next = temp->next;
		NodePtr trav = hashNode->ptr;
		if(trav->frequency == myFrequency && strcmp(trav->filePath, temp->filePath) > 0)
		{
			temp->next = trav;
			hashNode->ptr = temp;
		}
		else
		{
			while(trav->next != NULL && trav->next->frequency != myFrequency)
				trav = trav->next;
			while(trav->next != NULL && strcmp(trav->next->filePath, temp->filePath) > 0)
				trav = trav->next;
			temp->next = trav->next;
			trav->next = temp;
			prev = trav;
		}
	}
	return 1;
}
