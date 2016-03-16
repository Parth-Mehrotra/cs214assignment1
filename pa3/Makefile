CC = gcc 
CFLAGS = -g

default: tests

tests: mymalloc.o main1.o main2.o main3.o main4.o main5.o main6.o main7.o main8.o main9.o
	$(CC) $(CFLAGS) -o test1 main1.o mymalloc.o
	$(CC) $(CFLAGS) -o test2 main2.o mymalloc.o
	$(CC) $(CFLAGS) -o test3 main3.o mymalloc.o
	$(CC) $(CFLAGS) -o test4 main4.o mymalloc.o
	$(CC) $(CFLAGS) -o test5 main5.o mymalloc.o
	$(CC) $(CFLAGS) -o test6 main6.o mymalloc.o
	$(CC) $(CFLAGS) -o test7 main7.o mymalloc.o
	$(CC) $(CFLAGS) -o test8 main8.o mymalloc.o
	$(CC) $(CFLAGS) -o test9 main9.o mymalloc.o

main1.o: main1.c
	$(CC) $(CFLAGS) -c main1.c

main2.o: main2.c
	$(CC) $(CFLAGS) -c main2.c

main3.o: main3.c
	$(CC) $(CFLAGS) -c main3.c

main4.o: main4.c
	$(CC) $(CFLAGS) -c main4.c

main5.o: main5.c
	$(CC) $(CFLAGS) -c main5.c

main6.o: main6.c
	$(CC) $(CFLAGS) -c main6.c

main7.o: main7.c
	$(CC) $(CFLAGS) -c main7.c

main8.o: main8.c
	$(CC) $(CFLAGS) -c main8.c

main9.o: main9.c
	$(CC) $(CFLAGS) -c main9.c

mymalloc.o: mymalloc.c
	$(CC) $(CFLAGS) -c mymalloc.c

clean: 
	rm -rf *.o test*
