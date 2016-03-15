#include<stdio.h>
#include<stdlib.h>

#include"mymalloc.h"

int main(int argc, char** argv)
{
	void* thing = malloc(950);
	void* thing2 = malloc(2950);
	void* thing3 = malloc(950);
	free(thing);
	free(thing3);
	void* thing4 = malloc(2000);
	return 0;
}
