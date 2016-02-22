CC = gcc 
CFLAGS = -g -Wall -Werror

default: sorter

sorter: sorted-list.o main.o
	$(CC) $(CFLAGS) -o sorter sorted-list.o	main.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

sorted-list.o: sorted-list.c
	$(CC) $(CFLAGS) -c sorted-list.c

clean: 
	rm -rf *.o sorter
