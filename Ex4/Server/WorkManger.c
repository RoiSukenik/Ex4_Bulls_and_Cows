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
	TransferResult_t SendRes;
	for (Ind = 0; Ind < NUM_OF_WORKER_THREADS; Ind++)
		ThreadHandles[Ind] = NULL;
	while(TRUE)
	{
		SOCKET AcceptSocket = Accept_Socket(MainSocket, NULL, NULL);
		Ind = FindFirstUnusedThreadSlot();
		if (Ind == NUM_OF_WORKER_THREADS) //no slot is available
		{
			SendRes = SendString(SERVER_DENIED, MainSocket);
			if (SendCheck(SendRes, MainSocket) != STATUS_CODE_SUCSESS) { return STATUS_CODE_FAILURE; }
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
		if (RecvCheck(RecvRes, *t_socket, AcceptedStr) != STATUS_CODE_SUCSESS) { return STATUS_CODE_FAILURE; }
		char* messageType = MessageType(RecvRes);
		char** messageParameters[] = MessageParams(RecvRes);

		if (messageType == CLIENT_REQUEST)
		{
			free(AcceptedStr);
			strcpy_s(userName, sizeof(*messageParameters[0]), *messageParameters[0]);
			SendRes = SendString(SERVER_APPROVED, *t_socket);
			if (SendCheck(SendRes, *t_socket) != STATUS_CODE_SUCSESS) {return STATUS_CODE_FAILURE;}
			SendRes = SendString(SERVER_MAIN_MENU, *t_socket);
			if (SendCheck(SendRes, *t_socket) != STATUS_CODE_SUCSESS) { return STATUS_CODE_FAILURE; }
		}
		if (messageType == CLIENT_VERSUS)
		{

		}
		

	}

}
int RecvCheck(TransferResult_t RecvRes,SOCKET* t_socket,char* AcceptedStr)
{
	if (RecvRes == TRNS_FAILED)
	{
		printf("Service socket error while reading, closing thread.\n");
		closesocket(*t_socket);
		return STATUS_CODE_FAILURE;
	}
	else if (RecvRes == TRNS_DISCONNECTED)
	{
		printf("Connection closed while reading, closing thread.\n");
		closesocket(*t_socket);
		return STATUS_CODE_FAILURE;
	}
	else
	{
		printf("Got string : %s\n", AcceptedStr);
		return STATUS_CODE_SUCSESS;
	}
}
int SendCheck(TransferResult_t SendRes, SOCKET* t_socket)
{
	if (SendRes == TRNS_FAILED)
	{
		printf("Service socket error while writing, closing thread.\n");
		closesocket(*t_socket);//FIXME -Gentle CLOSE NEEDED
		return STATUS_CODE_FAILURE;
	}
}

char* checkFileExists()
{
	FILE* p_stream;
	if (p_stream = fopen(path, 'r'))
	{
		fclose(p_stream);
		return STATUS_CODE_SUCSESS;
	}
	else
	{
		fclose(p_stream);
		return STATUS_CODE_FAILURE;
	}
}