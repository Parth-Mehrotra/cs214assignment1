#include<stdio.h>
#include<stdlib.h>

#include "mymalloc.h"

int main(int argc, char** argv)
{
	printf("Double free example:\n");
	void* doubleFreeExample = malloc(50);
	free(doubleFreeExample);
	free(doubleFreeExample);

	printf("Outofrange pointer free\n");
	void* invPointerFree = malloc(10);
	free(invPointerFree+50000);

	printf("Invalid pointer free\n");
	free(invPointerFree+2);
	return 0;
}
