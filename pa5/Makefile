CC = gcc 
CFLAGS = -g

default: server client

server client: server.o serverFunctions.o client.o
	$(CC) $(CFLAGS) -pthread -o server server.o serverFunctions.o
	$(CC) $(CFLAGS) -pthread -o client client.c

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

serverFunctions.o: serverFunctions.c
	$(CC) $(CFLAGS) -c serverFunctions.c

client.o: client.c
	$(CC) $(CFLAGS) -c client.c

clean: 
	rm -rf *.o server client
