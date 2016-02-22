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
	data* a = arg1;
	free(a);
}

int main() {
	int (*cf)(void*, void*) = &compareFunction;
	void (*df)(void*) = &destructFunction;

	data* a = (data*) malloc(sizeof(data));
	a -> value = 10;
	data* b = (data*) malloc(sizeof(data));
	b -> value = 30;
	data* c = (data*) malloc(sizeof(data));
	c -> value = 50;

	SortedListPtr slr = SLCreate(cf, df);
	
	SLInsert(slr, a);
	SLInsert(slr, b);
	SLInsert(slr, c);

	SortedListIteratorPtr iter = SLCreateIterator(slr);
	printf("%d\n", *(int*)SLGetItem(iter));

	SLDestroy(slr);
	SLDestroyIterator(iter);
	return 0;
}
