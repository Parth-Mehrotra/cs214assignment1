#include<stdio.h>
#include<stdlib.h>

#include"mymalloc.h"

int main(int argc, char** argv)
{
	void* thing = malloc(1000);
	void* thing2 = malloc(1000);
	free(thing);
	free(thing2);
	void* thing3 = malloc(1000);
	return 0;
}
