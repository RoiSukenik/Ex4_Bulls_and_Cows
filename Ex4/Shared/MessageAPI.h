#ifndef MESSAGEAPI_DOT_H
#define MESSAGEAPI_DOT_H

#include	<string.h>
#include	<stdbool.h>
#include	<stdlib.h>
#include	<stdio.h>

#define STATUS_CODE_FAILURE -1
#define STATUS_CODE_SUCSESS  0
#define SERVER_DENIED				"SERVER_DENIED"
#define CLIENT_REQUEST				"CLIENT_REQUEST"

char* MessageType(char* string);
char** MessageParams(char* string);
char* writeMessage(char* MessageType, char** MessageParams, char* other);
void freeParamList(char** messageParams);

#endif // !MESSAGEAPI_DOT_H
