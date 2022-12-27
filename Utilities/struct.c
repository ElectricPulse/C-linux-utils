#include <stdio.h>
#include <string.h>

struct Player
{
	char name[12];
	int score;
};

int main() 
{
	struct Player player1;
	struct Player player2;

	strcpy(player1.name, "Adam");
	player1.score = 4;

	strcpy(player2.name, "Brock");
	player2.score = 5
	return 0;
}
