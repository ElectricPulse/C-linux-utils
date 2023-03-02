#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long DATA;

struct element {
	DATA n;
	DATA out;
};

struct element **memory;
DATA length = 1;
DATA last = 1;


DATA func(DATA n) {
	DATA result;
	
	for(DATA i = 0; i < last; ++i){
		if(memory[i]->n == n)
			return memory[i]->out;
	}

	if(n % 2 == 0) 
		result = func(n / 2) + 1;
	else {
		DATA a = func(n - 1);
		DATA b = func(n + 1);
		result = a > b ? a : b;
	}

	struct element *save = malloc(sizeof(struct element)); 
	
	--length;
	if(length == 0) {
		length = 10;
		memory = realloc(memory, sizeof(char *) * (last+1+length));
		if(memory == NULL)
			printf("Failed to realloc memory\n");
	}
	memory[last++] = save;


	return result;
}

int main(int argc, char* argv[]) {
	memory = malloc(sizeof(char *));
	struct element first;
	first.n = 1;
	first.out = 0;
	memory[0] = &first;
	
	DATA n = atoi(argv[1]);
	printf("%llu: %llu", n, func(n));
	return 0;
}
