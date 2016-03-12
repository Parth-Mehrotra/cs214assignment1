#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

#include"mymalloc.h"

static char myarray[5000];
int memoryLeft = 5000;

void createMementry(mementryPtr memHead, int size, mementryPtr prev, mementryPtr next)
{
	memset(memHead->confirmCode, '\0', 11);
	strcpy(memHead->confirmCode, "dEL8zWd9Ik");
	memHead->allocatedMemory = (memHead+sizeof(mementry));
	memHead->sizeOfAllocation = size;
	memHead->isFree = 0;
	memHead->prev = prev;
	memHead->next = next;
	memoryLeft -= sizeof(mementry) + size;
}

void* mymalloc(int size, char* errorLocation, int errorLine)
{
	if(myarray == NULL)
	{
		fprintf(stderr, "5000 byte Memory Space Invalid. Aborting...\n(Error at %s, line %d)\n", errorLocation, errorLine);
		return NULL;
	}
	if((sizeof(mementry) + size) > memoryLeft)
	{
		fprintf(stderr, "Memory size exceeded.\n(Error at %s, line %d)\n", errorLocation, errorLine);
		return NULL;	
	}

	mementryPtr memHead = (mementryPtr) myarray;
	mementryPtr memPrev = NULL;
	mementryPtr memNext = NULL;

	int tempMemoryLeft = 5000;
	while(1)
	{
		if(strcmp(memHead->confirmCode, "dEL8zWd9Ik") != 0)
		{
			if(tempMemoryLeft >= (sizeof(mementry) + size))
			{
				createMementry(memHead, size, memPrev, memNext);
				return memHead->allocatedMemory;
			}
			fprintf(stderr, "No more memory available.\n(Error at %s, line %d)\n", errorLocation, errorLine);
			return NULL;	
		}

		memPrev = memHead->prev;
		memNext = memHead->next;
		if(memHead->isFree == 1 && memHead->sizeOfAllocation >= size)
		{
			createMementry(memHead, size, memPrev, memNext);
			return memHead->allocatedMemory;
		}
		tempMemoryLeft -= (sizeof(mementry) + memHead->sizeOfAllocation);
		memHead = memHead->next;
	}
}
