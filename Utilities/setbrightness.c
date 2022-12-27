#include <stdio.h>
#include <stdlib.h>
#define prefix "/sys/class/backlight/intel_backlight/"

int main(int argc, char *argv[])
{
	if(argc != 2) 
		return 0;
	
	
	FILE *file, *file_max;

	file = fopen(prefix"brightness", "r");	
	file_max = fopen(prefix"max_brightness", "r");

	int max_brightness;
	fscanf(file_max, "%d", &max_brightness);
	
	int percent;
	percent = atoi(argv[1]);

	int brightness;
	fscanf(file, "%d", &brightness);
	fclose(file);

	file = fopen(prefix"brightness", "w");

	int new;
	new = max_brightness/100*percent + brightness;
	if(new > max_brightness)
		new = max_brightness;
	if(new < 0)
		new = 0;
	printf("want to write: %d", new);
	fprintf(file, "%d", new);

	fclose(file);
	return 0;
}
