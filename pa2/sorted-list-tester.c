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
	data* a = (data*) malloc(sizeof(data));
	a -> value = 10;
	data* b = (data*) malloc(sizeof(data));
	b -> value = 30;

	printf("compareFunct(10, 2) = %d\n", compareFunction(a, b));
	destructFunction(a);
	printf("%d\n", a->value);
	return 0;
}
