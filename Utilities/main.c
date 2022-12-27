#include <stdio.h>
#include <stdbool.h>

int main() {
	bool status1, status2;
	status1 = false;
	status2 = false;
	for(int a = 1; a < 10; ++a){
		for(int b = 1; b < 10; ++b){
			for(int c = 1; c < 10; ++c){
				if(100 * a + 10 * b + c == 4*a*b*c)
					status1 = true;

				if(99 * a - 99 * c == -99.0)
					status2 = true;

				if(status1 && status2) {
					printf("%d", a * b * c);
					status1 = false;
					status2 = false;
				}
			}
		}
	}
}
