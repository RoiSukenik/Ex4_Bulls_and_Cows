#include "WorkManager.h"



char* Manage_Server(char* argv[])
{

	Mutex_readfile = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex	
	if (Mutex_readfile == NULL)	{
		printf("CreateMutex error: %d\n", GetLastError());
		return STATUS_CODE_FAILURE;
	}

	global_connected_clients_counter = 0;
	global_playing_clients_counter = 0;
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
			char* string = writeMessage(SERVER_DENIED, NULL, "No slots available for client, dropping the connection.\n");
			SendRes = SendString(string, MainSocket);
			free(string);
			if (SendCheck(SendRes, MainSocket) != STATUS_CODE_SUCSESS) { Close_Socket(MainSocket); Close_WinSock(); return STATUS_CODE_FAILURE; }
			Close_Socket(AcceptSocket); //Closing the socket, dropping the connection.
			break;
		}
		else
		{
			global_connected_clients_counter++;
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
			printf("Thread opened\n");
		}

	}

	return STATUS_CODE_SUCCESS;
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
	char userName[MAX_USER_NAME_LEN];
	TransferResult_t SendRes;
	TransferResult_t RecvRes;
	char* AcceptedStr = NULL;
	if (Approve_new_player_to_server(t_socket, &userName) != STATUS_CODE_SUCSESS) { return STATUS_CODE_FAILURE; }
	while (true) {
		// send	SERVER_MAIN_MENU message
		SendRes = SendString(SERVER_MAIN_MENU, *t_socket);
		if (SendCheck(SendRes, *t_socket) != STATUS_CODE_SUCSESS) { Close_Socket(t_socket); Close_WinSock(); return STATUS_CODE_FAILURE; }

		// recieve CLIENT_VERSUS or CLIENT_DISCONNECT
		RecvRes = ReceiveString(&AcceptedStr, *t_socket);
		if (RecvCheck(RecvRes, *t_socket, AcceptedStr) != STATUS_CODE_SUCSESS) { return STATUS_CODE_FAILURE; }
		if (strcmp(MessageType(AcceptedStr), CLIENT_DISCONNECT) == 0) {
			global_connected_clients_counter--;
			// graceful close ?. close this thread.
		}
		else if (strcmp(MessageType(AcceptedStr), CLIENT_VERSUS) != 0)
		{
			char* string = writeMessage("WRONG_INPUT", NULL, NULL);
			SendRes = SendString(string, *t_socket);
			free(string);
			if (SendCheck(SendRes, *t_socket) != STATUS_CODE_SUCSESS) { Close_Socket(t_socket); Close_WinSock(); return STATUS_CODE_FAILURE; }
			global_connected_clients_counter--;
			// did not get the expected client versus, close everything !
		}
		free(AcceptedStr);

		if (global_connected_clients_counter == 1) {
			// send	SERVER_NO_OPPONENTS message and ask the player to play again
			SendRes = SendString(SERVER_NO_OPPONENTS, *t_socket);
			if (SendCheck(SendRes, *t_socket) != STATUS_CODE_SUCSESS) { Close_Socket(t_socket); Close_WinSock(); return STATUS_CODE_FAILURE; }
			continue; // this will go directly to 
		}
		global_playing_clients_counter++;
		bool flag_no_players_continue = false;
		// "searching for opponent" "blocking function" 
		//	untill the other connected client will press the play or press the quit
		//  so we decide what to do.
		while (global_playing_clients_counter < 2) {
			if (global_connected_clients_counter == 1)
			{
				// one of the connected clients decided to leave, restart.
				flag_no_players_continue = true;
				break;
			}
		}
		if (flag_no_players_continue == true) {
			global_playing_clients_counter--;
			SendRes = SendString(SERVER_NO_OPPONENTS, *t_socket);
			if (SendCheck(SendRes, *t_socket) != STATUS_CODE_SUCSESS) { Close_Socket(t_socket); Close_WinSock(); return STATUS_CODE_FAILURE; }
			continue;
		}



		// open communication file now ! first to open also closes
					/*HANDLE CommunictionFileCheck = checkFileExistsElseCreate();
					if (CommunictionFileCheck != NULL)
					{
						CommunictionFileHandle = CommunictionFileCheck;
					}*/
		char* other_client_userName = NULL;
		other_client_userName = synced_blocking_read_commutication(userName, userName);
		if (other_client_userName == NULL) { // error handle me }


		// SERVER_INVITE:<other client username>
			SendRes = SendString("ENTER HERE !!!!", *t_socket);
			if (SendCheck(SendRes, *t_socket) != STATUS_CODE_SUCSESS) { Close_Socket(t_socket); Close_WinSock(); return STATUS_CODE_FAILURE; }
			// free built line


			SendRes = SendString(SERVER_SETUP_REQUSET, *t_socket);
			if (SendCheck(SendRes, *t_socket) != STATUS_CODE_SUCSESS) { Close_Socket(t_socket); Close_WinSock(); return STATUS_CODE_FAILURE; }

			RecvRes = ReceiveString(&AcceptedStr, *t_socket);
			if (RecvCheck(RecvRes, *t_socket, AcceptedStr) != STATUS_CODE_SUCSESS) { return STATUS_CODE_FAILURE; }
			if (strcmp(AcceptedStr, CLIENT_SETUP) != 0) {
				char* string = writeMessage("WRONG_INPUT", NULL, NULL);
				SendRes = SendString(string, *t_socket);
				free(string);
				if (SendCheck(SendRes, *t_socket) != STATUS_CODE_SUCSESS) { Close_Socket(t_socket); Close_WinSock(); return STATUS_CODE_FAILURE; }
				global_connected_clients_counter--;
				global_playing_clients_counter--;	//  not supposed to ever happen
													// check what to close here
				// graceful close ?. close this thread.
			}
			// save numbers to guess (player's numbers) in the common file
			free(AcceptedStr);

			while (true) {
				SendRes = SendString(SERVER_PLAYER_MOVE_REQUEST, *t_socket);
				if (SendCheck(SendRes, *t_socket) != STATUS_CODE_SUCSESS) { Close_Socket(t_socket); Close_WinSock(); return STATUS_CODE_FAILURE; }



				RecvRes = ReceiveString(&AcceptedStr, *t_socket);
				if (RecvCheck(RecvRes, *t_socket, AcceptedStr) != STATUS_CODE_SUCSESS) { return STATUS_CODE_FAILURE; }
				if (strcmp(AcceptedStr, CLIENT_PLAYER_MOVE) != 0) {
					char* string = writeMessage("WRONG_INPUT", NULL, NULL);
					SendRes = SendString(string, *t_socket);
					free(string);
					if (SendCheck(SendRes, *t_socket) != STATUS_CODE_SUCSESS) { Close_Socket(t_socket); Close_WinSock(); return STATUS_CODE_FAILURE; }
					// ?global_connected_clients_counter--;
					// ?global_playing_clients_counter--;
					// graceful close ?. close this thread.
				}
				// put guess in common file
			// check guess and calculate results

				free(AcceptedStr);
				//SERVER_GAME_RESULTS + params
				SendRes = SendString(" ENTER HERE ", *t_socket);
				if (SendCheck(SendRes, *t_socket) != STATUS_CODE_SUCSESS) { Close_Socket(t_socket); Close_WinSock(); return STATUS_CODE_FAILURE; }
				// free allocation of SERVER_GAME_RESULTS + params

				SendRes = SendString(SERVER_DRAW, *t_socket);
				if (SendCheck(SendRes, *t_socket) != STATUS_CODE_SUCSESS) { Close_Socket(t_socket); Close_WinSock(); return STATUS_CODE_FAILURE; }

				//	 if win
				//	{
				//	 announce win/draw
				//
				//// SERVER_WIN + PARAMS
				//		SendRes = SendString("server win+params", *t_socket);
				//		if (SendCheck(SendRes, *t_socket) != STATUS_CODE_SUCSESS) { return STATUS_CODE_FAILURE; }
				////	free allocation of SERVER_WIN + PARAMS
				//   global_playing_clients_counter--;
				//// break;
				//	}
				//  else if draw
				//	{
				//		SendRes = SendString(SERVER_DRAW, *t_socket);
				//		if (SendCheck(SendRes, *t_socket) != STATUS_CODE_SUCSESS) { return STATUS_CODE_FAILURE; }
				//  global_playing_clients_counter--;
				//// break;
				////}
			}

		}
	}
}

char* synced_blocking_read_commutication(char* username, char* content) {
	bool local_written = false;
	bool local_read = false;
	DWORD wait_code;
	while ((local_read == false) || (local_written == false)) {

		wait_code = WaitForSingleObject(Mutex_readfile,	INFINITE);
		if (wait_code != WAIT_OBJECT_0) {
			return NULL;
		}
		if ((global_readme == false) && (local_written == false)) {
			// write to file
			global_readme = true;
			local_written = true;
		}
		else if (global_readme == true)
		{
			// read and check its not me "<username>:"
			// if its not me{

				//other_client_userName = // read content

			local_read = true;
			global_readme = false;
			// }//else do nothing
		}	//else do nothing

		if (!ReleaseMutex(Mutex_readfile)) { printf("Release Mutex error: %d\n", GetLastError()); return NULL; }
	}
}

int RecvCheck(TransferResult_t RecvRes,SOCKET* t_socket,char* AcceptedStr)
{
	if (RecvRes == TRNS_FAILED)
	{
		printf("Service socket error while reading, closing thread.\n");
		closesocket(t_socket);
		return STATUS_CODE_FAILURE;
	}
	else if (RecvRes == TRNS_DISCONNECTED)
	{
		printf("Connection closed while reading, closing thread.\n");
		closesocket(t_socket);
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
		closesocket(t_socket);//FIXME -Gentle CLOSE NEEDED
		return STATUS_CODE_FAILURE;
	}
	return STATUS_CODE_SUCSESS;
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

int Approve_new_player_to_server(SOCKET* Client, char* p_userName)
{
	TransferResult_t SendRes;
	TransferResult_t RecvRes;
	char userName[MAX_USER_NAME_LEN];
	char* AcceptedStr = NULL;
	// recieve CLIENT_REQUEST
	RecvRes = ReceiveString(&AcceptedStr, *Client);
	if (RecvCheck(RecvRes, *Client, AcceptedStr) != STATUS_CODE_SUCSESS) { return STATUS_CODE_FAILURE; }
	if (strcmp(MessageType(AcceptedStr), CLIENT_REQUEST) != 0) {
		char* string = writeMessage("WRONG_INPUT", NULL, NULL);
		SendRes = SendString(string, *Client);
		free(string);
		if (SendCheck(SendRes, *Client) != STATUS_CODE_SUCSESS) { Close_Socket(Client); Close_WinSock(); return STATUS_CODE_FAILURE; }
		global_connected_clients_counter--;
		Close_Socket(Client);
		Close_WinSock();
		return STATUS_CODE_FAILURE;
	}
	// save username and free
	char** params = MessageParams(AcceptedStr);
	strcpy_s(p_userName, sizeof(userName), params[0]);
	freeParamList(params);
	free(AcceptedStr);

	// send SERVER_APPROVED message
	SendRes = SendString(SERVER_APPROVED, *Client);
	if (SendCheck(SendRes, *Client) != STATUS_CODE_SUCSESS) { Close_Socket(Client); Close_WinSock(); return STATUS_CODE_FAILURE; }
	return STATUS_CODE_SUCSESS;
}