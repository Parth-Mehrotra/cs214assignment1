#include <stdio.h>
#include "sorted-list.h"

struct data {
	int value;
};
typedef struct data data;

int compareFunction(void* arg1, void* arg2) { 

	data* a = arg1;
	data* b = arg2;

	if (a -> value == b -> value) return 0;
	if (a -> value > b -> value) return 1;
	if (a -> value < b -> value) return -1;
	else return 37707;
}

void destructFunction(void* arg1) {
	free(arg1);
}

int main() {
	int (*cf)(void*, void*) = &compareFunction;
	void (*df)(void*) = &destructFunction;

	data* a = (data*) malloc(sizeof(data));
	a -> value = 10;
	data* b = (data*) malloc(sizeof(data));
	b -> value = 30;

	SortedListPtr slr = SLCreate(cf, df);
	
	SLInsert(slr, a);

	data* d1 = slr -> head -> data;
	int n1 = d1 -> value;

	
	printf("%d\n", compareFunction(a, b));
	printf("%d -> %d\n", n1, n2);

	free(slr);
	return 0;
}
