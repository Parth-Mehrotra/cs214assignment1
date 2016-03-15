#ifndef MYMALLOC_H_
#define MYMALLOC_H_

#define malloc(x) mymalloc(x,__FILE__,__LINE__)
#define free(x) myfree((void*)(x),__FILE__,__LINE__)

void* mymalloc(unsigned int size, char* errorLocation, int errorLine);
void myfree(void* ptr, char* errorLocation, int errorLine);

typedef struct mementry_T {
	char confirmCode[11]; // The confirm code will be the string "dEL8zWd9Ik"
	void* allocatedMemory;
	int sizeOfAllocation;
	int isFree;
	struct mementry_T* prev;
	struct mementry_T* next;
} mementry;
typedef mementry* mementryPtr;

#endif
