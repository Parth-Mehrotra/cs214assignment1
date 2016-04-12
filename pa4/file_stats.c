#include <stdio.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>

#include "header.h"

hashTablePtr table;

void find_files(char* dir_name) {
	DIR* d;
	d = opendir(dir_name);

	if (d == NULL) {
		hashFile(table, dir_name);
		closedir(d);
		return;
	}

	while (1) { 
		struct dirent* entry; 
		char* d_name = (char*) malloc(strlen(dir_name) + 256);
		memset(d_name, '\0', strlen(dir_name) + 256);

		entry = readdir(d);
		if (entry == NULL) {
			break;
		}

		d_name = strcpy(d_name, dir_name);
		*(d_name + strlen(d_name)) = '/';
		d_name = strcat(d_name, entry -> d_name);
		if (entry -> d_type == DT_REG) { 
			hashFile(table, d_name);
		}

		if (entry -> d_type == DT_DIR) {
			if (strcmp (parseFilePath(d_name), "..") != 0 && strcmp (parseFilePath(d_name), ".") != 0) {
				int path_length;
				char path[PATH_MAX];
				path_length = snprintf (path, PATH_MAX, "%s/%s", dir_name, parseFilePath(d_name));
				find_files(path);
			}
		}
	}

	closedir(d);
}

int main(int argc, char** argv) {
	if(argc != 3){printf("Invalid input.\n"); return 0;}
	FILE* test;
	if((test = fopen(argv[1], "r")) != NULL)
	{
		fclose(test);
		printf("Output file already exists. Replace?\n");
		char answer = '\0';
		scanf("%c", &answer);
		if(tolower(answer) != 'y')
			return 0;
	}
	if((test = fopen(argv[2], "r")) == NULL){printf("Input file does not exist.\n"); return 0;}
	fclose(test);
	table = createTable(1000);
	find_files(argv[2]);
	int length = table -> numUniqueEntries;
	char** words = getSortedList(table);

	FILE *fp;
	fp = fopen(argv[1], "w+");

	fprintf(fp, "{\"list\" : [\n");
	int i;
	for (i = 0; i < length; i++) {
		NodePtr node = getListOfFiles(table, *(words+i));
		fprintf(fp, "\t{\"%s\" : [\n", words[i]);

		NodePtr temp = node;
		while (1) {
			char* fileName = parseFilePath(temp->filePath);
			fprintf(fp, "\t\t{\"%s\" : %d}", parseFilePath(temp -> filePath), temp -> frequency);
			if (temp -> next == NULL) {
				fprintf(fp, "\n");
				break;
			} else {
				fprintf(fp, ",\n");
				temp = temp->next;
			}
		}

		if (i < length - 1) {
			fprintf(fp, "\t]},\n");
		} else {
			fprintf(fp, "\t]}\n");
		}
	}

	fprintf(fp, "]}\n");

	return 0;
}
