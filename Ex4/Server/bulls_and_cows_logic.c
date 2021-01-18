
#include "bulls_and_cows_logic.h"


char check_bulls(char* secret, char* guess)
{
	int bulls = 0;
	for (int i = 0; i < GAME_NUM_LENGHT; i++)
	{
		if (secret[i] == guess[i]) bulls++;
	}
	return bulls + '0';
}

char check_cows(char* secret, char* guess)
{
	int cows = 0;
	for (int j = 0; j < GAME_NUM_LENGHT; j++)
	{
		for (int i = 0; i < GAME_NUM_LENGHT; i++)
		{
			if (secret[j] == guess[i]) cows++; // pretty simple as every number will only be allowed once
		}
	}
	return  cows + '0';
}
