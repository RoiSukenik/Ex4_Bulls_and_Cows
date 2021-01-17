#ifndef MESSAGEAPI_DOT_H
#define MESSAGEAPI_DOT_H

#include	<string.h>
#include	<stdbool.h>
#include	<stdlib.h>

#define STATUS_CODE_FAILURE "-1"
#define STATUS_CODE_SUCSESS  "0"

char* MessageType(char* string);
char* MessageParams(char* string);
char* writeMessage(char* MessageType, char* MessageParams[]);

#endif // !MESSAGEAPI_DOT_H
