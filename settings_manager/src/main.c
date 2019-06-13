#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PRINT(x) printf(x); printf("\n")

void stringtest()
{
	char string[] = "Hehe ekisde hehe";

	// get first
	char* hehe = strtok(string, " ");
	PRINT(hehe);

	char* s = strtok(NULL, " ");
	PRINT(s);

	char* j = strtok(NULL, " ");
	PRINT(j);
}

void generate_file(const char* file_name);
void write_settings(const char* file_name, unsigned char times, unsigned char time);
void console_interface(const char* line);


int main(int argc, char** argv)
{
	stringtest();

	/*
	if(argc < 1)
	{
		printf("no path speficied");
		return -1;
	}

	char file_name[200];
	strcpy(file_name, argv[1]);
	strcat(file_name, "/settings.settings");
	PRINT(file_name);
	
	while(!end)
	{
		char* command_buffer[100];
		scanf("%[^\n]s ",command_buffer);
		end = console_interface()
	}
	*/
			
	return 0;
}

void console_interface(char* line)
{
	const char* command = strtok(line, " ");
	const char*	argument1 = strtok(NULL, " ");
	const char* argument2 = strtok(NULL, " ");

	if(strcmp(command, "set"))
	{
		
	}
}


void generate_file(const char* file_name)
{
	write_settings(file_name, 0, 0);
}

void write_settings(const char* file_name, unsigned char times, unsigned char time)
{
	FILE* file = fopen(file_name, "wb");
	const char data[2] = { times, time };
	fwrite(data, 1, 2, file);
	fclose(file);
}
