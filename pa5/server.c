/* A simple server in the internet domain using TCP
 *    The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

#include "serverFunctions.h"

ThreadListPtr threadList;
AccountPtr* accountList;
int numAccounts;

pthread_mutex_t openAccMutex;
pthread_mutex_t startAccMutex;
pthread_mutex_t changeBalanceMutex;

int* threadArgs;
pthread_attr_t threadAttr;
pthread_t* garbageCollector;

void error(char *msg)
{
    perror(msg);
    exit(1);
}

void sigHandler(int signum)
{
	NodePtr temp = threadList->head;
	while(temp != NULL && temp->inSession)
	{
		write(temp->newsockfd,"The server is shutting down.\n",30);
		temp = temp->next;
	}
	free(threadArgs);
	pthread_attr_destroy(&threadAttr);

	pthread_mutex_destroy(&openAccMutex);
	pthread_mutex_destroy(&startAccMutex);
	pthread_mutex_destroy(&changeBalanceMutex);

	pthread_cancel(*garbageCollector);	
	free(garbageCollector);
	destroyThreadList();
	
	int i;
	for(i = 0; i < numAccounts; i++)
	{
		free((*(accountList+i))->name);
		free(*(accountList+i));
	}
	free(accountList);
	exit(0);
}

void sigPrintout(int signum)
{
	char* outputStr = (char*)malloc(3072);
	memset(outputStr, '\0', 3072);
	strcat(outputStr, "\nCURRENT BANK STATE\n\n");
	int i;
	for(i = 0; i < numAccounts; i++)
	{
		AccountPtr thisAcc = accountList[i];
		strcat(outputStr, thisAcc->name);
		strcat(outputStr, "\n");

		if(!thisAcc->isInSession)
		{
			strcat(outputStr, "Balance: ");
			char* balanceStr = (char*)malloc(100); 
			sprintf(balanceStr, "%.2f\n\n", thisAcc->balance);
			strcat(outputStr, balanceStr);
			free(balanceStr);
		}
		else
		{
			strcat(outputStr, "IN-SESSION\n\n");
		}
	}
	if(numAccounts == 0)
		strcat(outputStr, "NO OPEN ACCOUNTS\n\n");
	
	printf(outputStr);
	free(outputStr);
	alarm(20);
}

void* communicate(void* args)
{
	signal(SIGINT, sigHandler);

	/** If we're here, a client tried to connect **/
	int newsockfd = *((int*)args);

	char buffer[256];
	int n = -1;
	int command = -1;
	int inClientSession = 0;
	AccountPtr thisAcc = NULL;
	int thisAccIndex = -1;
	int tries = 0;
		 
	// if the connection blew up for some reason, complain and exit
	if (newsockfd < 0) 
	{
		error("ERROR on accept");
	}
	printf("A new client connection has opened.\n");

	while(1)
	{
		// zero out the char buffer to receive a client message
		bzero(buffer,256);    	
		// try to read from the client socket
		n = read(newsockfd,buffer,255);
		// if the read from the client blew up, complain and exit
		if (n < 0)
		{
			// try to write to the client socket
			n = write(newsockfd,"ERROR reading from socket\n",26);
			// if the write to the client below up, complain and exit
			if (n < 0)
			{
				error("ERROR writing to socket");
			}
			continue;
		}
		else if(n == 0)
		{
			if(tries < 2)
			{
				break;
			}
			else
			{
				tries++;
				continue;
			}
		}
		tries = 0;
		*(buffer+n-1) = '\0';
		command = parseCommand(buffer);
		if(command == -1)
		{
			// try to write to the client socket
			n = write(newsockfd,"Invalid Command\n",16);
			// if the write to the client below up, complain and exit
			if (n < 0)
			{
				error("ERROR writing to socket");
			}
			continue;
		}
		if(command == OPEN)
		{
			if(inClientSession)
			{
				// try to write to the client socket
				n = write(newsockfd,"A customer session is already open on this client.\n",51);
				// if the write to the client below up, complain and exit
				if (n < 0)
				{
					error("ERROR writing to socket");
				}
				continue;
			}
			char* accName = (char*)malloc(101);
			memset(accName, '\0', 101);
			strncpy(accName, buffer+5, 100);
			if(strcmp(accName, "\0") == 0)
			{
				// try to write to the client socket
				n = write(newsockfd,"You must enter a non-empty account name.\n",41);
				// if the write to the client below up, complain and exit
				if (n < 0)
				{
					error("ERROR writing to socket");
				}
				free(accName);
				continue;
			}
			thisAccIndex = openAccount(accName);
			if(thisAccIndex == -1)
			{
				// try to write to the client socket
				n = write(newsockfd,"There are already 20 accounts initialized in the bank.\n",55);
				// if the write to the client below up, complain and exit
				if (n < 0)
				{
					error("ERROR writing to socket");
				}
				free(accName);
				continue;
			}
			if(thisAccIndex == -2)
			{
				// try to write to the client socket
				n = write(newsockfd,"An account with that name already exists.\n",42);
				// if the write to the client below up, complain and exit
				if (n < 0)
				{
					error("ERROR writing to socket");
				}
				free(accName);
				continue;
			}
			thisAcc = accountList[thisAccIndex];
			inClientSession = 1;
			// try to write to the client socket
			n = write(newsockfd,"Your account has been created successfully.\n",44);
			// if the write to the client below up, complain and exit
			if (n < 0)
			{
				error("ERROR writing to socket");
			}
			continue;
		}
		if(command == START)
		{
			if(inClientSession)
			{
				// try to write to the client socket
				n = write(newsockfd,"A customer session is already open on this client.\n",51);
				// if the write to the client below up, complain and exit
				if (n < 0)
				{
					error("ERROR writing to socket");
				}
				continue;
			}	
			char* accName = (char*)malloc(101);
			memset(accName, '\0', 101);
			strncpy(accName, buffer+6, 100);
			if(strcmp(accName, "\0") == 0)
			{
				// try to write to the client socket
				n = write(newsockfd,"You must enter a non-empty account name.\n",41);
				// if the write to the client below up, complain and exit
				if (n < 0)
				{
					error("ERROR writing to socket");
				}
				free(accName);
				continue;
			}
			thisAccIndex = startAccount(accName);
			if(thisAccIndex == -1)
			{
				// try to write to the client socket
				n = write(newsockfd,"An account with that name does not exist.\n",42);
				// if the write to the client below up, complain and exit
				if (n < 0)
				{
					error("ERROR writing to socket");
				}
				free(accName);
				continue;
			}
			if(thisAccIndex == -2)
			{
				// try to write to the client socket
				n = write(newsockfd,"That account is already in-session.\n",36);
				// if the write to the client below up, complain and exit
				if (n < 0)
				{
					error("ERROR writing to socket");
				}
				free(accName);
				continue;
			}
			thisAcc = accountList[thisAccIndex];
			inClientSession = 1;
			// try to write to the client socket
			n = write(newsockfd,"Your account has been started successfully.\n",44);
			// if the write to the client below up, complain and exit
			if (n < 0)
			{
				error("ERROR writing to socket");
			}
			free(accName);
			continue;
		}
		if(command == CREDIT)
		{
			if(!inClientSession)
			{
				// try to write to the client socket
				n = write(newsockfd,"There is no open customer session on this client.\n",50);
				// if the write to the client below up, complain and exit
				if (n < 0)
				{
					error("ERROR writing to socket");
				}
				continue;
			}	
			char* amountStr = (char*)malloc(101);
			memset(amountStr, '\0', 101);
			strncpy(amountStr, buffer+7, 100);
			int amount = (float)atof(amountStr);
			if(amount <= 0)
			{
				// try to write to the client socket
				n = write(newsockfd,"You entered an invalid quantity.\n",33);
				// if the write to the client below up, complain and exit
				if (n < 0)
				{
					error("ERROR writing to socket");
				}
				free(amountStr);
				continue;
			}
			changeBalance(thisAcc, amount);
			// try to write to the client socket
			n = write(newsockfd,"Your balance has been successfully changed.\n",44);
			// if the write to the client below up, complain and exit
			if (n < 0)
			{
				error("ERROR writing to socket");
			}
			free(amountStr);
			continue;
		}
		if(command == DEBIT)
		{
			if(!inClientSession)
			{
				// try to write to the client socket
				n = write(newsockfd,"There is no open customer session on this client.\n",50);
				// if the write to the client below up, complain and exit
				if (n < 0)
				{
					error("ERROR writing to socket");
				}
				continue;
			}	
			char* amountStr = (char*)malloc(101);
			memset(amountStr, '\0', 101);
			strncpy(amountStr, buffer+6, 100);
			int amount = (float)atof(amountStr);
			if(amount <= 0)
			{
				// try to write to the client socket
				n = write(newsockfd,"You entered an invalid quantity.\n",33);
				// if the write to the client below up, complain and exit
				if (n < 0)
				{
					error("ERROR writing to socket");
				}
				free(amountStr);
				continue;
			}
			int result = changeBalance(thisAcc, -1 * amount);
			if(result == 0)
			{
				// try to write to the client socket
				n = write(newsockfd,"The quantity you entered exceeds what you currently have on this account.\n",74);
				// if the write to the client below up, complain and exit
				if (n < 0)
				{
					error("ERROR writing to socket");
				}
				free(amountStr);
				continue;
			}
			// try to write to the client socket
			n = write(newsockfd,"Your balance has been successfully changed.\n",44);
			// if the write to the client below up, complain and exit
			if (n < 0)
			{
				error("ERROR writing to socket");
			}
			free(amountStr);
			continue;
		}
		if(command == BALANCE)
		{
			if(!inClientSession)
			{
				// try to write to the client socket
				n = write(newsockfd,"There is no open customer session on this client.\n",50);
				// if the write to the client below up, complain and exit
				if (n < 0)
				{
					error("ERROR writing to socket");
				}
				continue;
			}	
			char* output = (char*)malloc(100);
			sprintf(output, "Your current balance is: %.2f.\n", thisAcc->balance);
			// try to write to the client socket
			n = write(newsockfd, output, strlen(output) + 1);
			// if the write to the client below up, complain and exit
			if (n < 0)
			{
				error("ERROR writing to socket");
			}
			free(output);
			continue;
		}
		if(command == FINISH)
		{
			if(!inClientSession)
			{
				// try to write to the client socket
				n = write(newsockfd,"There is no open customer session on this client.\n",50);
				// if the write to the client below up, complain and exit
				if (n < 0)
				{
					error("ERROR writing to socket");
				}
				continue;
			}	
			inClientSession = 0;
			thisAccIndex = -1;
			thisAcc->isInSession = 0;
			thisAcc = NULL;
			// try to write to the client socket
			n = write(newsockfd,"This client session has been closed.\n",37);
			// if the write to the client below up, complain and exit
			if (n < 0)
			{
				error("ERROR writing to socket");
			}
			continue;
		}
		if(command == EXIT)
		{
			if(inClientSession)
				thisAcc->isInSession = 0;
			break;
		}
	}

	// try to write to the client socket
	n = write(newsockfd,"This client session has ended.\n",31);
	// if the write to the client below up, complain and exit
	if (n < 0)
	{
		error("ERROR writing to socket");
	}
	endPThread(newsockfd);
	pthread_exit(NULL);
}

void* threadGarbageCollector(void* junk)
{
	signal(SIGINT, sigHandler);

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
				pthread_join(*(temp->pthreadPtr), NULL);
				free(temp->pthreadPtr);
				free(temp);
				break;
			}
			prev = temp;
			temp = temp->next;
		}
	}
}

int main(int argc, char *argv[])
{
	// Initialize global variables
	accountList = (AccountPtr*)malloc(20*sizeof(AccountPtr));
	int i;
	for(i = 0; i < 20; i++)
		*(accountList+i) = NULL;
	numAccounts = 0;
	// Thread Variables
	threadArgs = (int*)malloc(sizeof(int));
	*threadArgs = 0;

	void* threadStatus;

	pthread_attr_init(&threadAttr);
	pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_JOINABLE);
	pthread_attr_setscope(&threadAttr, PTHREAD_SCOPE_SYSTEM);
	
	// Initialize Mutexes
	pthread_mutex_init(&openAccMutex, NULL);
	pthread_mutex_init(&startAccMutex, NULL);
	pthread_mutex_init(&changeBalanceMutex, NULL);

	// Other variables
	int sockfd = -1;
	int newsockfd = -1;
	int portno = -1;
	int clilen = -1;

	struct sockaddr_in serverAddressInfo;
	struct sockaddr_in clientAddressInfo;

	// Thread LinkedList initialization
	threadList = (ThreadListPtr)malloc(sizeof(struct ThreadList));
	threadList->head = NULL;

	garbageCollector = (pthread_t*)malloc(sizeof(pthread_t));
	pthread_create(garbageCollector, &threadAttr, threadGarbageCollector, NULL);

	// try to build a socket .. if it doesn't work, complain and exit
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		error("ERROR opening socket");
	}

	// zero out the socket address info struct .. always initialize!
	bzero((char *) &serverAddressInfo, sizeof(serverAddressInfo));

	// set the remote port .. translate from a 'normal' int to a super-special 'network-port-int'
	serverAddressInfo.sin_port = htons(8989);

	// set a flag to indicate the type of network address we'll be using  
	serverAddressInfo.sin_family = AF_INET;

	// set a flag to indicate the type of network address we'll be willing to accept connections from
	serverAddressInfo.sin_addr.s_addr = INADDR_ANY;

	// bind the server socket to a specific local port, so the client has a target to connect to      
	if (bind(sockfd, (struct sockaddr *) &serverAddressInfo, sizeof(serverAddressInfo)) < 0)
	{
		error("ERROR on binding");
	}

	// set up the server socket to listen for client connections
	listen(sockfd,20);
	
	// determine the size of a clientAddressInfo struct
	clilen = sizeof(clientAddressInfo);

	// Setup signal-handler
	signal(SIGALRM, sigPrintout);
	signal(SIGINT, sigHandler);
	alarm(20);

	while(1)
	{
		// block until a client connects, when it does, create a client socket
		newsockfd = accept(sockfd, (struct sockaddr *) &clientAddressInfo, &clilen);
		*threadArgs = newsockfd;

		pthread_t* threadPtr = (pthread_t*)malloc(sizeof(pthread_t));
		pthread_create(threadPtr, &threadAttr, communicate, (void*)threadArgs);

		addPThread(threadPtr, newsockfd);
	}

	return 0;
}
