#include <stdio.h>
#include <dirent.h>

#include "header.h"


void recursively_hash(DIR* dir, hashTablePtr table) {
	if (dir -> d_type == DT_REG) {
		hashFile(table, dir->d_name);
		return;
	}
	
	while (1) {
		if (dir != NULL) {
			recursively_hash(readdir(dir));
		} else {
			return;
		}
	}
}

int main(int argc, char** argv) {
	DIR* dir = opendir(argv[2]);
	hashTablePtr table = createTable(1000);
	if (dir != NULL) {
		recursively_hash(dir, table);
	} else {
		hashFile(table, dir->d_name);
	}

	getSortedList(table);

	return 0;
}
