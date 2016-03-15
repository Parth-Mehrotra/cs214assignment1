#include<stdio.h>
#include<stdlib.h>

#include"mymalloc.h"

int main(int argc, char** argv)
{
	void* thing = malloc(1000);
	free(thing);
	thing = malloc(1000);
	free(thing);
	return 0;
}
