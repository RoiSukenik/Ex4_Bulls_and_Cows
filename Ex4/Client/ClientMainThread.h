#pragma once
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/*
 This file was written for instruction purposes for the
 course "Introduction to Systems Programming" at Tel-Aviv
 University, School of Electrical Engineering.
Last updated by Amnon Drory, Winter 2011.
 */
 /*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

#define _WINSOCK_DEPRECATED_NO_WARNINGS



#define SERVER_MAIN_MENU			"SERVER_MAIN_MENU"
#define SERVER_APPROVED				"SERVER_APPROVED"
#define SERVER_DENIED				"SERVER_DENIED"
#define SERVER_INVITE				"SERVER_INVITE"
#define SERVER_SETUP_REQUSET		"SERVER_SETUP_REQUSET"
#define SERVER_PLAYER_MOVE_REQUEST	"SERVER_PLAYER_MOVE_REQUEST"
#define SERVER_GAME_RESULTS			"SERVER_GAME_RESULTS"
#define SERVER_WIN					"SERVER_WIN"
#define SERVER_DRAW					"SERVER_DRAW"
#define SERVER_NO_OPPONENTS			"SERVER_NO_OPPONENTS"
#define SERVER_OPPONENT_QUIT		"SERVER_OPPONENT_QUIT"

#define CLIENT_DISCONNECT			"CLIENT_DISCONNECT\n"		// \n here notice
#define CLIENT_REQUEST				"CLIENT_REQUEST"
#define CLIENT_VERSUS				"CLIENT_VERSUS\n"			// \n here notice
#define CLIENT_SETUP				"CLIENT_SETUP"
#define CLIENT_PLAYER_MOVE			"CLIENT_PLAYER_MOVE"

#define STATUS_CODE_FAILURE -1
#define STATUS_CODE_SUCCESS 0

#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#include "NetworkInterface.h"
#include "SocketSendRecvTools.h"
#include "MessageAPI.h"



char* global_serverIP;
char* global_serverPort;
char* global_username;
SOCKET m_socket;

#define SETUP 0
#define GUESS 1
int global_client_state = SETUP;





char* CreateClientSend_one_Param(char* message_type, char* paramm);

int MainClient(char* argv[]);

static DWORD SendDataThread(void);

static DWORD RecvDataThread(void);