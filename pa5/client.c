#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// file descriptor for our socket
int sockfd = -1; 

void *read_from_server() {
	char buffer[256];

	while(1) {
		int n = read(sockfd,buffer,255); 
		if (strstr(buffer, "This client session has ended.") != 0) {
			printf("%s\n", buffer);
			exit(0);
		}
		if (strstr(buffer, "The server is shutting down.") != 0) {
			printf("\n%s\n", buffer);
			exit(0);
		}
		if (strlen(buffer) != 0) {
			printf("%s\n", buffer);
		}
		bzero(buffer, 256);
	}
}

int main(int argc, char *argv[]) {

	// server port to connect to
	int portno = 8989;

	// utility variable - for monitoring reading/writing from/to the socket
	int n = -1;

	// char array to store data going to and coming from the server
	char buffer[256];

	// Super-special secret C struct that holds address info for building our socket
    struct sockaddr_in serverAddressInfo;

	// Super-special secret C struct that holds info about a machine's address
    struct hostent *serverIPAddress;
    
    if (argc < 2) {
       fprintf(stderr,"usage %s hostname\n", argv[0]);
       exit(1);
    }

	// look up the IP address that matches up with the name given
    serverIPAddress = gethostbyname(argv[1]);
    if (serverIPAddress == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(1);
    }
				
	// try to build a socket .. if it doesn't work, complain and exit
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("ERROR creating socket\n");
		exit(1);
	}

	// zero out the socket address info struct .. always initialize!
    bzero((char *) &serverAddressInfo, sizeof(serverAddressInfo));

	// set a flag to indicate the type of network address we'll be using 
    serverAddressInfo.sin_family = AF_INET;
	
	// set the remote port .. translate from a 'normal' int to a super-special 'network-port-int'
	serverAddressInfo.sin_port = htons(portno);

	// do a raw copy of the bytes that represent the server's IP address in 
	//   the 'serverIPAddress' struct into our serverIPAddressInfo struct
    bcopy((char *)serverIPAddress->h_addr, (char *)&serverAddressInfo.sin_addr.s_addr, serverIPAddress->h_length);



	/** We now have a blank socket and a fully parameterized address info struct .. time to connect **/
	
	// try to connect to the server using our blank socket and the address info struct 
	//   if it doesn't work, complain and exit
	while(1) {
		if (connect(sockfd,(struct sockaddr *)&serverAddressInfo,sizeof(serverAddressInfo)) < 0) {
			printf("ERROR connecting\n");
			printf("Retrying\n");
			sleep(3);
		}	
		else
			break;
	}

	pthread_t thread0;
	pthread_create(&thread0, NULL, read_from_server, NULL);
	
	while(1) {
		/** If we're here, we're connected to the server .. w00t!  **/
		printf("Please enter the message: ");
		
		// zero out the message buffer
		bzero(buffer,256);

		// get a message from the client
		fgets(buffer,255,stdin);
		
		// try to write it out to the server
		n = write(sockfd,buffer,strlen(buffer));
		
		// if we couldn't write to the server for some reason, complain and exit
		if (n < 0) {
			printf("ERROR writing to socket");
			exit(1);
		}
		sleep(2);

		// sent message to the server, zero the buffer back out to read the server's response
		bzero(buffer,256);
	}

    return 0;
}
