
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#include "serverFunctions.h"

extern ThreadListPtr threadList;
extern AccountPtr* accountList;
extern int numAccounts;

extern pthread_mutex_t openAccMutex;
extern pthread_mutex_t startAccMutex;
extern pthread_mutex_t changeBalanceMutex;

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
int openAccount(char* name)
{
	// Lock Mutex
	pthread_mutex_lock(&openAccMutex);
	if(numAccounts == 20)
	{
		pthread_mutex_unlock(&openAccMutex);
		return -1;
	}
	int i;
	for(i = 0; i < numAccounts; i++)
	{
		if(strcmp((*(accountList + i))->name, name) == 0)
		{
			pthread_mutex_unlock(&openAccMutex);
			return -2;
		}
	}
	int newAccIndex = 0;
	AccountPtr newAcc = (AccountPtr)malloc(sizeof(struct Account));
	newAcc->name = name;
	newAcc->balance = 0.0;
	newAcc->isInSession = 1;
	*(accountList + numAccounts) = newAcc;
	newAccIndex = numAccounts;
	numAccounts++;
	// Unlock Mutex
	pthread_mutex_unlock(&openAccMutex);

	return newAccIndex;
}
int startAccount(char* name)
{
	// Lock Mutex
	pthread_mutex_lock(&startAccMutex);
	int accIndex = -1;
	int i;
	for(i = 0; i < numAccounts; i++)
	{
		if(strcmp((*(accountList + i))->name, name) == 0)
			accIndex = i;
	}
	if(accIndex == -1)
	{
		pthread_mutex_unlock(&startAccMutex);
		return -1;
	}
	AccountPtr thisAcc = *(accountList+accIndex);
	if(thisAcc->isInSession)
	{
		pthread_mutex_unlock(&startAccMutex);
		return -2;
	}
	thisAcc->isInSession = 1;
	// Unlock Mutex
	pthread_mutex_unlock(&startAccMutex);
	return accIndex;
}
int changeBalance(AccountPtr account, float amount)
{
	// Lock Mutex
	pthread_mutex_lock(&changeBalanceMutex);
	if((amount + account->balance) < 0)
	{
		pthread_mutex_unlock(&changeBalanceMutex);
		return 0;
	}
	account->balance += amount;
	// Unlock Mutex
	pthread_mutex_unlock(&changeBalanceMutex);
	return 1;
}

int addPThread(pthread_t* pthreadPtr, int newsockfd)
{
	NodePtr newNode;
	if((newNode = (NodePtr)malloc(sizeof(struct Node))) < 0)
		return 0;
	newNode->pthreadPtr = pthreadPtr;
	newNode->newsockfd = newsockfd;
	newNode->inSession = 1;
	newNode->next = threadList->head;
	threadList->head = newNode;
	return 1;
}
int endPThread(int newsockfd)
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

int destroyThreadList()
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
