
#include "bulls_and_cows_logic.h"


int check_bulls(char* secret[GAME_NUM_LENGHT_PLUS_EOS], char* guess[GAME_NUM_LENGHT_PLUS_EOS])
{
	int bulls = 0;
	for (int i = 0; i < GAME_NUM_LENGHT; i++)
	{
		if (secret[i] == guess[i]) bulls++;
	}
		return bulls;
}

int check_cows(char* secret[GAME_NUM_LENGHT_PLUS_EOS], char* guess[GAME_NUM_LENGHT_PLUS_EOS])
{
	int cows = 0;
	for (int j = 0; j < GAME_NUM_LENGHT; j++)
	{
		for (int i = 0; i < GAME_NUM_LENGHT; i++)
		{
			if (secret[j] == guess[i]) cows++; // pretty simple as every number will only be allowed once
		}
	}
	return cows;
}
