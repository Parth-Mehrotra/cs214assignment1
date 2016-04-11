#include <stdio.h>
#include <dirent.h>
#include <limits.h>

#include "header.h"

hashTablePtr table;

void find_files(char* dir_name) {
	DIR* d;
	d = opendir(dir_name);

	if (d == NULL) {
		hashFile(table, dir_name);
	}

	while (1) { 
		struct dirent* entry; 
		char* d_name;

		entry = readdir(d);
		if (entry == NULL) {
			break;
		}

		d_name = entry -> d_name;
		if (entry -> d_type == DT_REG) { 
			hashFile(table, d_name);
		}

		if (entry -> d_type == DT_DIR) {
			if (strcmp (d_name, "..") != 0 && strcmp (d_name, ".") != 0) {
				int path_length;
				char path[PATH_MAX];
				path_length = snprintf (path, PATH_MAX, "%s/%s", dir_name, d_name);
				find_files(path);
			}
		}
	}

	closedir(d);
}

int main(int argc, char** argv) {
	table = createTable(1000);
	find_files(argv[2]);
	int length = table -> numEntries;
	char** words = getSortedList(table);

	FILE *fp;
	fp = fopen(argv[1], "w+");

	fprintf(fp, "{\"list\":[\n");
	int i;
	for (i = 0; i < length; i++) {
		NodePtr node = getListOfFiles(words[i]);
		fprintf(fp, "\"%s\":[\n", words[i]);

		NodePtr temp = node;
		while (1) {
			fprintf(fp, "{\"%s\":%d}", parseFilePath(temp -> filePath), temp -> frequency);
			if (temp -> next == NULL) {
				fprintf(fp, "\n");
				break;
			} else {
				fprintf(fp, ",\n");
			}
		}

		if (i < length - 1) {
			fprintf(fp, "]},\n");
		} else {
			fprintf(fp, "]}\n");
		}
	}

	fprintf(fp, "]}\n");

	return 0;
}
