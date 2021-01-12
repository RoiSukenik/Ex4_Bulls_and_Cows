#ifndef WORK_MANAGER_DOT_H
#define WORK_MANAGER_DOT_H

#include "NetworkInterface.h"
#include "SocketSendRecvTools.h"

#define NUM_OF_WORKER_THREADS 2
#define MAX_LOOPS 3

bool new_file_flag = false;

const char* path = "GameSession.txt";

HANDLE ThreadHandles[NUM_OF_WORKER_THREADS];
SOCKET ThreadInputs[NUM_OF_WORKER_THREADS];


char* Manage_Server(char* argv[]);

static int FindFirstUnusedThreadSlot();

static DWORD ServiceThread(SOCKET* t_socket);

#endif // !WORK_MANAGER_DOT_H
