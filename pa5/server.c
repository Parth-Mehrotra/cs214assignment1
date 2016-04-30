/* A simple server in the internet domain using TCP
 *    The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

void* communicate(void* args)
{
	int newsockfd = *((int*)args)
	/** If we're here, a client tried to connect **/
		 
	// if the connection blew up for some reason, complain and exit
	if (newsockfd < 0) 
	{
		error("ERROR on accept");
	}

	// zero out the char buffer to receive a client message
	bzero(buffer,256);    	
	
	// try to read from the client socket
	n = read(newsockfd,buffer,255);

	// if the read from the client blew up, complain and exit
	if (n < 0)
	{
		error("ERROR reading from socket");
	}

	printf("Here is the message: %s\n",buffer);
		
	// try to write to the client socket
	n = write(newsockfd,"I got your message",18);

	// if the write to the client below up, complain and exit
	if (n < 0)
	{
		error("ERROR writing to socket");
	}

	endPThread(newsockfd);
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	// Thread Variables
	int * threadArgs = (int*)malloc(sizeof(int));
	*threadArgs = 0;

	void* threadStatus;

	pthread_attr_t threadAttr;
	pthread_attr_init(&threadAttr);
	pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_JOINABLE);
	pthread_attr_setscope(&threadAttr, PTHREAD_SCOPE_SYSTEM);
	
	// Other variables
	int sockfd = -1;
	int newsockfd = -1;
	int portno = -1;
	int clilen = -1;
	int n = -1;

	char buffer[256];

	struct sockaddr_in serverAddressInfo;
	struct sockaddr_in clientAddressInfo;

	// Thread LinkedList initialization
	ThreadListPtr threadList = (ThreadListPtr)malloc(sizeof(struct ThreadList));
	threadList->head = NULL;

	pthread_t* garbageCollector = (pthread_t*)malloc(sizeof(pthread_t));
	pthread_create(garbageCollector, &threadAttr, threadGarbageCollector, (void*)threadList);

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
	listen(sockfd,5);
	
	// determine the size of a clientAddressInfo struct
	clilen = sizeof(clientAddressInfo);

	while(1)
	{
		// block until a client connects, when it does, create a client socket
		newsockfd = accept(sockfd, (struct sockaddr *) &clientAddressInfo, &clilen);
		*threadArgs = newsockfd;

		pthread_t* threadPtr = (pthread_t*)malloc(sizeof(pthread_t));
		pthread_create(threadPtr, &threadAttr, communicate, (void*)threadArgs);

		addPThread(threadList, threadPtr, newsockfd);
	}

	free(threadArgs);
	pthread_attr_destroy(&threadAttr);

	pthread_cancel(*garbageCollectorThread);	
	free(garbageCollectorThread);
	destroyThreadList(threadList);

	return 0;
}
