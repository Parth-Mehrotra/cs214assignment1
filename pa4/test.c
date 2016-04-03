#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>

int main(int argc, char** argv)
{
	DIR* dir = opendir("./whatsgood");
	DIR* dir2 = opendir(".");
	struct dirent* thing;
	struct dirent* thing2;
	struct dirent* thing3;
	struct dirent* thing4;
	struct dirent* thing5;
	if(dir != NULL)
	{
		thing = readdir(dir);
		thing2 = readdir(dir);
		thing3 = readdir(dir);
		thing4 = readdir(dir);
		thing5 = readdir(dir);
	}
	else
	{
		printf("Not a directory.\n");
		return 0;
	}
	if(thing->d_type == DT_DIR)
		printf("We did it!\n");
	printf("%s\n", thing->d_name);

	return 0;
}
