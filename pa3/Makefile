CC = gcc 
CFLAGS = -g -v

default: mem_test

mem_test: mymalloc.o myfree.o main.o
	$(CC) $(CFLAGS) -o mem_test main.o mymalloc.o myfree.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

mymalloc.o: mymalloc.c
	$(CC) $(CFLAGS) -c mymalloc.c

myfree.o: myfree.c
	$(CC) $(CFLAGS) -c myfree.c

clean: 
	rm -rf *.o mem_test
