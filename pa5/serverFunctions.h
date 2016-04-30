#ifndef SERVER_FUNCTIONS_H
#define SERVER_FUNCTIONS_H

#include <stdlib.h>
#include <pthread.h>

// Bank struct and function definitions
struct Account {
	char* name;
	float balance;
	int isInSession;
};
typedef struct Account* AccountPtr;

enum {OPEN=0, START=1, CREDIT=2, DEBIT=3, BALANCE=4, FINISH=5, EXIT=6};

int parseCommand(char* command);
AccountPtr openAccount(char* name);
AccountPtr startAccount(char* name);
void closeAccount(AccountPtr account);
int changeBalance(AccountPtr account, int amount);

struct Node{
	pthread_t* pthreadPtr;
	int newsockfd;
	int inSession;
	struct Node* next;
};
typedef struct Node* NodePtr;

struct ThreadList{
	struct Node* head;
};
typedef struct ThreadList* ThreadListPtr;

int addPThread(ThreadListPtr threadList, pthread_t* pthreadPtr, int newsockfd);
int endPThread(ThreadListPtr threadList, int newsockfd);
int destroyThreadList(ThreadListPtr threadList);
void* threadGarbageCollector(void* listPtr);

#endif
