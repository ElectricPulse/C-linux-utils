#include <stdio.h>
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>
#include <sys/wait.h>
#include "main.h"

int main(int argc, char **argv) {
	struct input_event ev;
	
	int fd = open("/dev/input/event4", O_RDONLY);
	
	if(fd == -1){
		printf("Insufficient priviliges\n");
		return -1;
	}
	

	while (1){
		read(fd, &ev, sizeof(ev));
		if(ev.code == 113 && ev.type == EV_KEY && ev.value == 1) {
			mute();	
		}	
	}
}
