#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NUMOPTS 3

void help();
void algorithm(char* const, char *const, char* const, char* const);

int main(int argc, char *argv[]){
	if((argc-2)/2 != NUMOPTS) {
		printf("Invalid amount of arguments\n");
		return 1;
	}

	char* prepend;
	char* text;
	char* output;

	for(int i = 0; i < (argc-2)/2; ++i) {
		char *current_arg  = argv[i*2+1];
		char *next_arg = argv[i*2+2];
		int next_arg_len = strlen(next_arg);

		if(!strcmp(current_arg, "-h")) {
			help();
			return 0;
		}
	
		if(!strcmp(current_arg, "-p")){
			prepend = malloc(next_arg_len * sizeof(char));
			strcpy(prepend, next_arg);
		}

		if(!strcmp(current_arg, "-t")){
			text = malloc(next_arg_len * sizeof(char));
			strcpy(text, next_arg);
		}		
		if(!strcmp(current_arg, "-o")){
			output = malloc(next_arg_len * sizeof(char));
			strcpy(output, next_arg);
		}
	}

	algorithm(prepend, text, argv[argc-1], output);

	return 0;
}

void algorithm(char * const prepend,char * const text, char * const filename, char * const output) {
	FILE *target_file, *temp_file;

	target_file = fopen(filename, "r");
	temp_file = fopen(output, "w");

	if(target_file == NULL || temp_file == NULL){
		printf("Couldnt open temp or target file\n");
		return;
	}
	
	char c;
	int match = 0;
	int line = 1;
	bool matched = 1;
	while((c = fgetc(target_file)) != EOF){
		if(c == prepend[match])
			match++;
		else if(prepend[match] != '\0') {
			match = 0;
		}

		if(prepend[match] == '\0') {
			matched = 1;
		}

		fputc(c, temp_file);

		if(c == '\n') {
			if(matched) {
				printf("Appended after line: %d\n", line);
				char* tmp_text = text;
				while(*tmp_text != '\0'){
					fputc(*tmp_text++, temp_file);
				}
				fputc('\n', temp_file);
				match = 0;
				matched = 0;
			}

			++line;
		}
	}
	
	fclose(target_file);
	fclose(temp_file);
}

void help(){
	printf("-p Prepending line\n -t Text");
}
