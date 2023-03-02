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

	if(argc != 2) {
		printf("Incorrect amount of args\n");
		return -1;
	}
		
	
	FILE *fp;
	fp  = fopen ("/data.log", "w");
	
	int fd = open("/dev/input/event8", O_RDONLY);
	bool twice = false;
	while (1){
		read(fd, &ev, sizeof(ev));
		if(ev.code == 4 && ev.type == 4 && ev.value == 248) {
			if(!twice) {
				twice = true;
				continue;
			}
				
			int pid = fork();
			if (pid == 0) {
				execlp("/bin/bash", "--norc", "--noprofile", "-c", argv[1], NULL);
			}
			twice = false;
		}
	}
}
