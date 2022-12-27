#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

struct dirent *proc_ent;

int main(int argc, char *argv[]) {
	if(argc < 2 || argc > 3) {
		printf("Invalid amount of arguments\n");
		return 1;
	}
	
	DIR* proc;
	bool found = false;

	proc = opendir("/proc");
	if(proc == NULL) {
		printf("Couldn't open proc filesystem");
		return 1;	
	}
	

	while((proc_ent = readdir(proc)) != NULL) {
		char* dirname;
		dirname = proc_ent->d_name;
		
		bool number = true;
		
		char *temp = dirname;
		while(temp++ != NULL){
			if(*dirname < '0' || *dirname > '9') {
				number = false;
				break;
			}
		}

		if(number){
			FILE *comm;
			char file[100] = "/proc/";
			strcat(file,dirname);
			strcat(file, "/comm");
			comm = fopen(file, "r");
			if(comm == NULL)
				continue;
			
			char name[100];
			int i;
			for(i = 0; (name[i] = fgetc(comm)) != EOF; ++i);

			name[--i] = '\0';
			
			if(!strcmp(name, argv[1])) {

				if(argc == 3 && !strcmp(argv[2],"-k")) {
					pid_t pid;
					pid = atoi(dirname);	
					kill(pid, 9);
				}

				found = true;
				break;
			}

			fclose(comm);
		}

	}

	closedir(proc);
	return !found;
}
