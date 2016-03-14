CC = gcc 
CFLAGS = -g

default: mem_test

mem_test: mymalloc.o main.o
	$(CC) $(CFLAGS) -o mem_test mymalloc.o	main.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

mymalloc.o: mymalloc.c
	$(CC) $(CFLAGS) -c mymalloc.c

clean: 
	rm -rf *.o mem_test
