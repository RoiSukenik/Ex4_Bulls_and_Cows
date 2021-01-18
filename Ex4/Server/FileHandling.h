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







int write_to_file(HANDLE gameLog, char* string);
int critical_read_code(HANDLE gameLog, char* read_line);
int fileMessageFormatter(char* userName, char* message, char* p_formatted_message_returned);
int getMessageContent(char* message, char* writerUserName, char* content);
#endif // !FILE_HANDLING_DOT_H
