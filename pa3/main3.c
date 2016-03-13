#include<stdio.h>
#include<stdlib.h>

#include"mymalloc.h"

int main(int argc, char** argv)
{
	void* thing = malloc(50);
	void* thing2 = malloc(150);
	free(thing2);
	void* thing3 = malloc(110);
	return 0;
}
