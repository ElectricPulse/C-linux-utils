#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>

void help();
bool del(int);
bool move(int, char*);
void add(char*);
void list();
void start();

#define CACHE "/home/hackerman/Scripts/cache"
#define TMP "/home/hackerman/Scripts/tmp"

int main(int argc, char *argv[])
{
	if(argc < 2) {
		fprintf(stderr, "Invalid amount of arguments\n");
		return 1;
	}
	
	bool status = 0;
		
	if(!strcmp(argv[1], "add")) {
		add(argv[2]);
		list();
	} else if(!strcmp(argv[1], "del")) {
		status = del(atoi(argv[2]));
		list();
	} else if(!strcmp(argv[1], "list"))
		list();
	else if(!strcmp(argv[1], "start"))
		start();
	else if(!strcmp(argv[1], "move")) {
		status = move(atoi(argv[3]), argv[2]);
		list();
	}
	else if(!strcmp(argv[1], "help"))
		help();
	else 
		help();
	
	if(status)
		printf("Failed to access files\n");
	return status;	
}

void start() {
	FILE *cache;
	
	
	cache = fopen(CACHE, "r");
	
	char c;
	char line[100];

	int len;
	int commands = 0;
	
	while(1) {
		for(len = 0; (c = fgetc(cache)) != EOF && c != '\n'; ++len){
			line[len] = c;
		}
		if(len == 0)
			break;

		line[len] = '\0';

		int pid = fork();

		char text[100] = "export PATH='/usr/bin:/bin';";
		strcat(text, line);

		if (pid == 0) {	
			execlp("/bin/bash", "--norc", "--noprofile","-c", text, NULL);
		} else {
			wait(NULL);
		}
	}

	fclose(cache);
}

void help() {
	printf("Available options are add, del, list, help, move [from] [to]\n");
}

void list(){
	FILE *cache;

	cache = fopen(CACHE, "r");
	char last_c = '\n';
	char c;
	int i = 0;
	while((c = fgetc(cache)) != EOF){
		if(last_c == '\n')
			printf("[%i]: ", ++i);

		putchar(c);

		last_c = c;
	}
}

void add(char* line){
	FILE *cache;

	cache = fopen(CACHE, "a");

	fprintf(cache, "%s\n", line);
	fclose(cache);
}

bool del(int remove_line){
	FILE *cache, *tmp;
	
	if(access(CACHE, F_OK) != 0)
		return 1;

	cache = fopen(CACHE, "r");
	tmp = fopen(TMP, "w");
	char c;
	int line = 1;
	while((c = fgetc(cache)) != EOF){	
		if(line != remove_line)
			fputc(c, tmp);	
		if(c == '\n')
			++line;
	}
	fclose(cache);

	rename(TMP, CACHE);
	fclose(tmp);
}
	
bool move(int remove_line, char* direction){
	FILE *cache, *tmp;
	
	cache = fopen(CACHE, "r");
	tmp = fopen(TMP, "w");

	if(cache == NULL || tmp == NULL)
		return 1;

	char c;
	int line = 1;
	
	int commandptr = 0;
	char command[100];
	int length = 0;
	while((c = fgetc(cache)) != EOF){
		if(line != remove_line)
			fputc(c, tmp);
		else
			command[commandptr++] = c;

		if(c == '\n')
			++line;
	}
	length = --line;
	commandptr = 0;
	line = 1;

	fclose(tmp);
	tmp = fopen(TMP, "r");
	fclose(cache);
	cache = fopen(CACHE, "w");
	
	if(cache == NULL || tmp == NULL)
		return 1;
	
	short x = 5;

	if(!strcmp(direction, "down"));
		x = 1;

	if(!strcmp(direction, "up"))
		x = -1;

	if(x == 5)
		return 1;

	if(remove_line+x == 0)
		while(fputc(command[commandptr++], cache) != '\n');
		
	while((c = fgetc(tmp)) != EOF){
		if(line == remove_line+x) {
			while(fputc(command[commandptr++], cache) != '\n');
			++line;
		}

		fputc(c, cache);

		if(c == '\n')
			++line;
	}

	if(remove_line+x >= length)
		while(fputc(command[commandptr++], cache) != '\n');
	
	fclose(tmp);
	fclose(cache);
	return 0;
}
	




