#ifndef WORK_MANAGER_DOT_H
#define WORK_MANAGER_DOT_H

#include "NetworkInterface.h"
#include "SocketSendRecvTools.h"
#include "MessageAPI.h"
#include <conio.h>




#define NUM_OF_WORKER_THREADS		2
#define MAX_LOOPS					3
#define NUMBER_OF_ARGV_ALLOWED		1
#define MAX_USER_NAME_LEN			20
#define EXIT_WORD_LEN				5
#define SLEEP_TIME					1000

#define EXIT_WORD					"exit"						
#define SERVER_MAIN_MENU			"SERVER_MAIN_MENU\n"
#define SERVER_APPROVED				"SERVER_APPROVED\n"
#define SERVER_DENIED				"SERVER_DENIED"
#define SERVER_INVITE				"SERVER_INVITE"
#define SERVER_SETUP_REQUSET		"SERVER_SETUP_REQUSET\n"
#define SERVER_PLAYER_MOVE_REQUEST	"SERVER_PLAYER_MOVE_REQUEST\n"
#define SERVER_GAME_RESULTS			"SERVER_GAME_RESULTS"
#define SERVER_WIN					"SERVER_WIN"
#define SERVER_DRAW					"SERVER_DRAW\n"
#define SERVER_NO_OPPONENTS			"SERVER_NO_OPPONENTS\n"
#define SERVER_OPPONENT_QUIT		"SERVER_OPPONENT_QUIT\n"

#define CLIENT_DISCONNECT			"CLIENT_DISCONNECT"
#define CLIENT_REQUEST				"CLIENT_REQUEST"
#define CLIENT_VERSUS				"CLIENT_VERSUS"
#define CLIENT_SETUP				"CLIENT_SETUP"
#define CLIENT_PLAYER_MOVE			"CLIENT_PLAYER_MOVE"

#define PATH						"GameSession.txt"

int global_connected_clients_counter;	// amount of users connected to the server
int global_playing_clients_counter;		// amount of users that want to play (clicked '1')



HANDLE ThreadHandles[NUM_OF_WORKER_THREADS];
HANDLE CommunictionFileHandle;
SOCKET ThreadInputs[NUM_OF_WORKER_THREADS];
volatile HANDLE Mutex_que;

char* Manage_Server(char* argv[]);

static int FindFirstUnusedThreadSlot();

static DWORD ServiceThread(SOCKET* t_socket);

HANDLE checkFileExistsElseCreate(void);

#endif // !WORK_MANAGER_DOT_H
