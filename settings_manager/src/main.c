#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
	if(argc == 1)
		return -1;

	char file_name[200];
	strcpy(file_name, argv[1]);
	strcat(file_name, "/settings.settings");
	printf(file_name);

	FILE* file = fopen(file_name, "rb+");

	fclose(file);

	return 0;
}
