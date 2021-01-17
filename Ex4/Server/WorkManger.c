#include "WorkManager.h"

char* Manage_Server(char* argv[])
{
	if((sizeof(argv) / sizeof(argv[0])) != NUMBER_OF_ARGV_ALLOWED) {return STATUS_CODE_FAILURE;}
	char* ptr = NULL;
	int Ind,Loop;
	int port = strtol(argv[1], &ptr, 10);
	char* retval = NULL;
	Initialize_WinSock();
	SOCKET MainSocket = Create_Socket();
	retval = Bind_Socket(MainSocket, port);
	retval = Listen_to_Socket(MainSocket, SOMAXCONN);
	for (Ind = 0; Ind < NUM_OF_WORKER_THREADS; Ind++)
		ThreadHandles[Ind] = NULL;
	while(TRUE)
	{
		SOCKET AcceptSocket = Accept_Socket(MainSocket, NULL, NULL);
		Ind = FindFirstUnusedThreadSlot();
		if (Ind == NUM_OF_WORKER_THREADS) //no slot is available
		{
			printf("No slots available for client, dropping the connection.\n");
			Close_Socket(AcceptSocket); //Closing the socket, dropping the connection.
			//FIXME - Stop listening.
			break;
		}
		else
		{
			ThreadInputs[Ind] = AcceptSocket; // shallow copy: don't close 
											  // AcceptSocket, instead close 
											  // ThreadInputs[Ind] when the
											  // time comes.
			ThreadHandles[Ind] = CreateThread(
				NULL,
				0,
				(LPTHREAD_START_ROUTINE)ServiceThread,
				&(ThreadInputs[Ind]),
				0,
				NULL
			);
			Ind = Ind + 1;
		}

	}


}
static int FindFirstUnusedThreadSlot()
{
	int Ind;

	for (Ind = 0; Ind < NUM_OF_WORKER_THREADS; Ind++)
	{
		if (ThreadHandles[Ind] == NULL)
			break;
		else
		{
			// poll to check if thread finished running:
			DWORD Res = WaitForSingleObject(ThreadHandles[Ind], 0);

			if (Res == WAIT_OBJECT_0) // this thread finished running
			{
				CloseHandle(ThreadHandles[Ind]);
				ThreadHandles[Ind] = NULL;
				break;
			}
		}
	}

	return Ind;
}

static DWORD ServiceThread(SOCKET* t_socket) {
	
	TransferResult_t SendRes;
	TransferResult_t RecvRes;
	char userName[MAX_USER_NAME_LEN];
	while (TRUE)
	{
		char* AcceptedStr = NULL;

		RecvRes = ReceiveString(&AcceptedStr, *t_socket);

		char* messageType = MessageType(RecvRes);
		char** messageParameters[] = MessageParams(RecvRes);

		if (messageType == CLIENT_REQUEST)
		{
			strcpy_s(userName, sizeof(*messageParameters[0]), *messageParameters[0]);

			

		}
	}

}
