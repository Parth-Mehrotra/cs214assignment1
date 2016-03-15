#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

#include"mymalloc.c"

void myfree(void* ptr, char* errorLocation, int errorLine) {
	ptr = (mementryPtr) ptr;

	//Could this be a valid pointer
	if (ptr > 5000) {
		fprintf(stderr, "Invalid pointer. Aborting...\n(Error at %s, line%d)\n", errorLocation, errorLine);
		return;
	}

	//Is it a valid initialized ptr?
	if (strcmp(ptr->confirmCode, "dEL8zWd9Ik") != 0) {
		fprintf(stderr, "Invalid pointer. Aborting...\n(Error at %s, line%d)\n", errorLocation, errorLine);
		return;
	}

	//Has it already been freed?
	if (ptr->isFree) {
		fprintf(stderr, "Double Free\n(Error at %s, line%d)\n", errorLocation, errorLine);
		return;
	}

	//free it
	ptr->isFree = 1;

	//is the one after it free?
	if (ptr -> next != NULL && ptr -> next -> isFree) {
		ptr -> sizeOfAllocation += ptr -> next -> sizeOfAllocation + sizeof(mementry);
		ptr -> next = ptr -> next -> next;
	}

	//Is the one before it free?
	if (ptr -> prev != NULL && ptr -> prev -> isFree) {
		ptr -> prev -> sizeOfAllocation += ptr -> sizeOfAllocation + sizeof(mementry);
		ptr -> prev -> next = ptr -> next;
	}
}
