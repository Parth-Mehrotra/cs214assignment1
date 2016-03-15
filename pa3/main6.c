#include<stdio.h>
#include<stdlib.h>

#include"mymalloc.h"

int main(int argc, char** argv)
{
	void* thing = NULL;
	free(thing);
	return 0;
}
