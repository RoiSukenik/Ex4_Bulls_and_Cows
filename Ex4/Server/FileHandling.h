#ifndef FILE_HANDLING_DOT_H
#define	FILE_HANDLING_DOT_H

#include <Windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define STATUS_CODE_FAILURE		-1
#define STATUS_CODE_SUCSESS		0
#define WAIT_15_SECONDS			150000
#define MIN_READ_SIZE			0
#define MAX_STR_LEN				55



#define PATH						"GameSession.txt"



int write_to_file( char* string);
int critical_read_code( char* read_line);

#endif // !FILE_HANDLING_DOT_H
