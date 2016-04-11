CC = gcc 
CFLAGS = -g

default: file_stats

file_stats: sorted_list.o hash.o tokenizer.o file_stats.o
	$(CC) $(CFLAGS) -o file_stats file_stats.o tokenizer.o hash.o sorted_list.o

file_stats.o: file_stats.c
	$(CC) $(CFLAGS) -c file_stats.c

sorted_list.o: sorted_list.c
	$(CC) $(CFLAGS) -c sorted_list.c

hash.o: hash.c
	$(CC) $(CFLAGS) -c hash.c

tokenizer.o: tokenizer.c
	$(CC) $(CFLAGS) -c tokenizer.c

clean: 
	rm -rf *.o file_stats
