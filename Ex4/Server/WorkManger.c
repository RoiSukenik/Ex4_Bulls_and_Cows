#include "WorkManager.h"

char* Manage_Server(char* argv[])
{
	
	if((sizeof(argv) / sizeof(argv[0])) != NUMBER_OF_ARGV_ALLOWED) {return STATUS_CODE_FAILURE;}
	char* ptr;
	int Ind,Loop;
	for (Ind = 0; Ind < NUM_OF_WORKER_THREADS; Ind++)
		ThreadHandles[Ind] = NULL;
	int port = strtol(argv[1], &ptr, 10);
	char* retval = NULL;
	Initialize_WinSock();
	SOCKET MainSocket = Create_Socket();
	retval = Bind_Socket(MainSocket, port);
	retval = Listen_to_Socket(MainSocket, SOMAXCONN);
	TransferResult_t SendRes;
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
	HANDLE CommunictionFileCheck = checkFileExistsElseCreate();
	if (CommunictionFileCheck != NULL)
	{
		CommunictionFileHandle = CommunictionFileCheck;
	}
	char userName[MAX_USER_NAME_LEN];
	while (TRUE)
	{
		char* AcceptedStr = NULL;
		RecvRes = ReceiveString(&AcceptedStr, *t_socket);
		if (RecvCheck(RecvRes, *t_socket, AcceptedStr) != STATUS_CODE_SUCSESS) { return STATUS_CODE_FAILURE; }
		char* messageType = MessageType(AcceptedStr);
		char** messageParameters = MessageParams(AcceptedStr);

		if (messageType == CLIENT_REQUEST)
		{
			
			strcpy_s(userName, sizeof(*messageParameters[0]), *messageParameters[0]);
			freeParamList(messageParameters);
			free(AcceptedStr);
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

HANDLE checkFileExistsElseCreate(void)
{
	HANDLE file_handle = NULL;
	file_handle = CreateFileA(PATH, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file_handle == INVALID_HANDLE_VALUE) {
		printf("Read request failed, Last Error = %s\n ", GetLastError());
		return file_handle;
	}
	else if (file_handle == ERROR_FILE_EXISTS)
	{
		printf("Read request failed, Last Error = %s\n ", GetLastError());
		return file_handle;
	}
	else
	{
		return file_handle;
	}
	
}

int checkForExit()
{
	HANDLE exitThread;

	char exit_word[EXIT_WORD_LEN + 1];
	while (TRUE) {
		if (_kbhit() != 0) {
			scanf_s(" %s", exit_word, EXIT_WORD_LEN + 1);
			if (strcmp(exit_word, EXIT_WORD) == 0)
			{
				printf("exiting by users...\n");
				break;
			}
		}
		else
			Sleep(SLEEP_TIME);
	}
	//clean_working_threads();
	return STATUS_CODE_SUCSESS;
}