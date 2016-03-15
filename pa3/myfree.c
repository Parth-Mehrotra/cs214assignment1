#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

#include "mymalloc.h"
#include "myfree.h"

void myfree(void* ptr, char* errorLocation, int errorLine) {
	mementryPtr mem = (mementryPtr) ptr;

	//Could this be a valid pointer
	if (myarray < mem < myarray+5000) {
		fprintf(stderr, "Invalid pointer. Aborting...\n(Error at %s, line%d)\n", errorLocation, errorLine);
		return;
	}

	//Is it a valid initialized ptr?
	if (strcmp(mem->confirmCode, "dEL8zWd9Ik") != 0) {
		fprintf(stderr, "Invalid pointer. Aborting...\n(Error at %s, line%d)\n", errorLocation, errorLine);
		return;
	}

	//Has it already been freed?
	if (mem->isFree) {
		fprintf(stderr, "Double Free\n(Error at %s, line%d)\n", errorLocation, errorLine);
		return;
	}

	//free it
	mem->isFree = 1;

	//is the one after it free?
	if (mem -> next != NULL && mem -> next -> isFree) {
		mem -> sizeOfAllocation += mem -> next -> sizeOfAllocation + sizeof(mementry);
		mem -> next = mem -> next -> next;
	}

	//Is the one before it free?
	if (mem -> prev != NULL && mem -> prev -> isFree) {
		mem -> prev -> sizeOfAllocation += mem -> sizeOfAllocation + sizeof(mementry);
		mem -> prev -> next = mem -> next;
	}
}
