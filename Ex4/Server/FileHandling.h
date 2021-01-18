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
#define MAX_STR_LEN				500

HANDLE gameLog = NULL;
HANDLE communication_semaphore = NULL;

int state = 0;
int critical_section_counter = 0;

#endif // !FILE_HANDLING_DOT_H
