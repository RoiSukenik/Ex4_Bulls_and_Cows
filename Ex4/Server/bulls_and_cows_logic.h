#pragma once

#define GAME_NUM_LENGHT 4
#define GAME_NUM_LENGHT_PLUS_EOS 5
typedef struct _player_strct {
	char* user_id; // agreed apon way of knowing who is the first user when listening for calls
	char* username;
	char* user_secret[GAME_NUM_LENGHT_PLUS_EOS];		// 4 digits and \0
	char* user_guess[GAME_NUM_LENGHT_PLUS_EOS];
}player_strct;

typedef struct _game_struct {
	player_strct* first_player;
	player_strct* second_player;
}game_struct;


/*
	• Description	– initialize a structure (allocation) (2 pointers) to
	• Parameters	– 2 player structures
	• Returns		– a pointer to a structure that will hold those nubmers (as strings)
*/
game_struct* initialize_game_struct(player_strct* player1, player_strct* player2);


/*
	• Description	– initialize a structure (allocation) on which we will work later
					  this struct should be filled with data either here(using funcs) or in the calling func (up to us later)
	• Parameters	– void
	• Returns		– a pointer to a structure that will hold those nubmers (as strings)
*/
player_strct* initialize_player_struct(void);


/*
• Description	– allocation for username inside player struct
• Parameters	– player struct and a char* to enter as a username
• Returns		– code success/failiure
*/
int init_username(player_strct* player, char* username);

/*
• Description	– allocation for userid inside player struct
• Parameters	– player struct and a char* to enter as a userid
• Returns		– code success/failiure
*/
int init_userid(player_strct* player, char* userid);



/*
• Description	–	checks the amount of cows in a guess
• Parameters	–	the secret number to work on and the guess we will check
• Returns		–	the number of cows guessed
*/
int check_cows(char* secret[GAME_NUM_LENGHT_PLUS_EOS], char* guess[GAME_NUM_LENGHT_PLUS_EOS]);


/*
• Description	–	checks the amount of bulls in a guess
• Parameters	–	the secret number to work on and the guess we will check
• Returns		–	the number of bulls guessed
*/
int check_bulls(char* secret[GAME_NUM_LENGHT_PLUS_EOS], char* guess[GAME_NUM_LENGHT_PLUS_EOS]);


char*





/*
• Description	–
• Parameters	–
• Returns		–
*/

/*
• Description	–
• Parameters	–
• Returns		–
*/