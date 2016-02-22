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
	data* d = (data*) malloc(sizeof(data));
	d -> value = 5;
	SortedListPtr slr = SLCreate(cf, df);
	
	SLInsert(slr, a);
	SLInsert(slr, b);
	SLInsert(slr, c);
	SLInsert(slr, d);

	SortedListIteratorPtr iter = SLCreateIterator(slr);
	SortedListIteratorPtr iter2 = SLCreateIterator(slr);
	SortedListIteratorPtr iter3 = SLCreateIterator(slr);
	SLNextItem(iter);
	SLNextItem(iter2);
	SLNextItem(iter2);
	SLNextItem(iter3);
	SLNextItem(iter3);
	SLNextItem(iter3);

	SLDestroy(slr);
	SLDestroyIterator(iter);
	SLDestroyIterator(iter2);
	SLDestroyIterator(iter3);
	return 0;
}
