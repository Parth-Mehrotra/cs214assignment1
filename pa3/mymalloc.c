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

void* mymalloc(unsigned int size, char* errorLocation, int errorLine)
{
	if(myarray == NULL)
	{
		fprintf(stderr, "5000 byte Memory Space Invalid. Aborting...\n(Error at %s, line %d)\n", errorLocation, errorLine);
		return NULL;
	}
	if(size == 0)
	{
		fprintf(stderr, "Cannot allocate 0 memory.\n(Error at %s, line %d)\n", errorLocation, errorLine);
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
		// checks if the pointer points to an initialized mementry 
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

		// checks if current mementry is free or not. if it is free, use it if it has enough space
		memPrev = memHead->prev;
		memNext = memHead->next;
		if(memHead->isFree == 1 && memHead->sizeOfAllocation >= size)
		{
			// if the previously allocated space is significantly larger than the space the user asked for, we create a 
			// mementry that manages the unused space
			if(memHead->sizeOfAllocation > size + sizeof(mementry))
			{
				void* voidEntry = (void*)(memHead + sizeof(mementry) + size);
				mementryPtr tempEntry = (mementryPtr) voidEntry;
				createMementry(tempEntry, memHead->sizeOfAllocation - size - sizeof(mementry), memHead, memHead->next);
				tempEntry->isFree = 1;
				createMementry(memHead, size, memPrev, tempEntry);
				return memHead->allocatedMemory;
			}
			else
			{
				memHead->isFree = 0;
				return memHead->allocatedMemory;
			}
		}
		if(memHead->next != NULL)
			memHead = memHead->next;
		else if((void*)(memHead + sizeof(mementry) + memHead->sizeOfAllocation) <= (void*)(myarray+5000-sizeof(mementry)))
		{
			memPrev = memHead;
			memNext = NULL;
			void* voidHead = (void*)(memHead + sizeof(mementry) + memHead->sizeOfAllocation);
			memHead = (mementryPtr) voidHead;
		}
		else
		{
			fprintf(stderr, "No more memory available.\n(Error at %s, line %d)\n", errorLocation, errorLine);
			return NULL;
		}
		int tempMemoryLeft = (void*)(myarray+5000) - (void*)(memHead);	
	}
}
