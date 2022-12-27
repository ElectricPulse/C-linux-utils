#include <stdio.h>
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
	struct input_event ev;
	
	FILE *fp;
	fp  = fopen ("/data.log", "w");
	
	int fd = open("/dev/input/event4", O_RDONLY);
	
	if(fd == -1){
		printf("Insufficient priviliges\n");
		return -1;
	}

	bool modifier = false;
	bool key = false;
	char kbmap[100];
	strcat(kbmap, "us");
	
	while (1){
		read(fd, &ev, sizeof(ev));
		
		printf("%d\n", ev.value);
		if((ev.type == EV_KEY) && (ev.code == 42)){	

			if(ev.value == 1)
				modifier = true;
			if(ev.value == 0)
				modifier = false;
		}

		if((ev.type == EV_KEY) && (ev.code == 56)){
			if(ev.value == 1);
				key = true;
			if(ev.value == 0)
				key = false;
		}

		if(modifier && key){
			strcpy(kbmap, strcmp(kbmap, "us") ? "us" : "-variant qwerty sk");

			int pid = fork();

			if (pid == 0) {
				char text[100];
				strcpy(text, "export PATH='/usr/bin:/bin';");
				strcat(text, "sudo setxkbmap ");
				strcat(text, kbmap);
				int result = execlp("/bin/bash", "--norc", "--noprofile", "-c",text, NULL);
				fprintf(fp, "print this: %d", result); 
			}
			else
				wait(NULL);
		}
	}
}
