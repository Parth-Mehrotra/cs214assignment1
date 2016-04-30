
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

int parseCommand(char* command)
{
	if(strlen(command) < 4)
	{
		return -1;
	}
	if(memcmp(command, "exit", 5) == 0)
	{
		return EXIT;
	}
	if(strlen(command) < 5)
	{
		return -1;
	}
	if(memcmp(command, "open ", 5) == 0)
	{
		return OPEN;
	}
	if(strlen(command) < 6)
	{
		return -1;
	}
	if(memcmp(command, "start ", 6) == 0)
	{
		return START;
	}
	else if(memcmp(command, "debit ", 6) == 0)
	{
		return DEBIT;
	}
	else if(memcmp(command, "finish", 6) == 0)
	{
		return FINISH;
	}
	if(strlen(command) < 7)
	{
		return -1;
	}
	else if(memcmp(command, "balance", 7) == 0)
	{
		return BALANCE;
	}
	else if(memcmp(command, "credit ", 7) == 0)
	{
		return CREDIT;
	}
	else
	{
		return -1;
	}
}
AccountPtr openAccount(char* name);
AccountPtr startAccount(char* name);
void closeAccount(AccountPtr account);
int changeBalance(AccountPtr account, int amount);

int addPThread(ThreadListPtr threadList, pthread_t* pthreadPtr, int newsockfd)
{
	NodePtr newNode;
	if((newNode = (NodePtr)malloc(sizeof(struct Node)) < 0)
		return 0;
	newNode->pthreadPtr = pthreadPtr;
	newNode->newsockfd = newsockfd;
	newNode->inSession = 1;
	newNode->next = threadList->head;
	threadList->head = newNode;
	return 1;
}
int endPThread(ThreadListPtr threadList, int newsockfd)
{
	NodePtr temp = threadList->head;
	while(temp != NULL)
	{
		if(temp->newsockfd == newsockfd)
		{
			temp->inSession = 0;
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}

int destroyThreadList(ThreadListPtr threadList)
{
	if(threadList == NULL)
		return 1;
	NodePtr temp = threadList->head;
	while(temp != NULL)
	{
		threadList->head = temp->next;
		free(temp);
	}
	free(threadList);
	return 1;
}

void* threadGarbageCollector(void* listPtr)
{
	ThreadListPtr threadList = (ThreadListPtr)listPtr;
	NodePtr temp = threadList->head;
	NodePtr prev = NULL;
	while(1)
	{
		temp = threadList->head;
		prev = NULL;
		while(temp != NULL)
		{
			if(temp->inSession == 0)
			{
				if(prev != NULL)
					prev->next = temp->next;
				else
					threadList->head = temp->next;
				pthread_join(temp->pthreadPtr);
				free(temp->pthreadPtr);
				free(temp);
				break;
			}
			prev = temp;
			temp = temp->next;
		}
	}
}

