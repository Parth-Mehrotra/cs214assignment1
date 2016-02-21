CC = gcc 
CFLAGS = -g -Wall -Werror

default: sorter

sorter: sorted-list.o sorted-list-tester.o
	$(CC) $(CFLAGS) -o sorter sorted-list.o	sorted-list-tester.o

sorted-list-tester.o: sorted-list-tester.c
	$(CC) $(CFLAGS) -c sorted-list-tester.c

sorted-list.o: sorted-list.c
	$(CC) $(CFLAGS) -c sorted-list.c

clean: 
	rm -rf *.out sorted_list
