#include <stdio.h>
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
	if(argc != 2) {
		printf("insufficient amount of arguments");
		return 1;
	}

	struct input_event ev;
	
	int fd = open("/dev/input/event2", O_RDONLY);
	
	if(fd == -1){
		printf("Insufficient priviliges\n");
		return -1;
	}
	
	if(access(argv[1], F_OK) != 0) {
		printf("Invalid script\n");
		return 1;
	}
		

	while (1){
		read(fd, &ev, sizeof(ev));
		if(ev.code == 116 && ev.type == EV_KEY && ev.value == 1) {
				int pid = fork();
			
				if(pid == 0){
					execlp("/bin/bash", "--norc", "--noprofile", "-c", argv[1], NULL);			
					return 0;
				} 
				
		}	
	}
}
