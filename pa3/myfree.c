#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

#include"mymalloc.c"

void myfree(void* ptr, char* errorLocation, int errorLine) {
	ptr = (mementryPtr) ptr;
	if (strcmp(ptr->confirmCode, "dEL8zWd9Ik") != 0) {
		fprintf(stderr, "Invalid pointer. Aborting...\n(Error at %s, line%d)\n", errorLocation, errorLine);
		return;
	}

	if (ptr > 5000) {
		fprintf(stderr, "Invalid pointer. Aborting...\n(Error at %s, line%d)\n", errorLocation, errorLine);
		return;
	}

	if (ptr->isFree) {
		fprintf(stderr, "Double Free\n(Error at %s, line%d)\n", errorLocation, errorLine);
		return;
	}

	
}
