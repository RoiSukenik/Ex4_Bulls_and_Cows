
#include "ClientMainThread.h"


char* CreateClientSend_one_Param(char* message_type, char* paramm) {
	char* merged = NULL;
	merged = (char*)malloc(strlen(paramm) + strlen(message_type) + 3);
	int i = 0;
	int j = 0;
	while (i < strlen(message_type))
	{
		merged[j] = message_type[i];
		i++;
		j++;
	}

	merged[j] = ":";
	j++;
	i = 0;
	while (i < strlen(paramm))
	{
		merged[j] = paramm[i];
		i++;
		j++;
	}

	merged[j] = "\n";
	j++;
	merged[j] = "\0";
	j++;

	return merged;
}




//Reading data coming from the server
static DWORD RecvDataThread(void)
{
	TransferResult_t RecvRes;
	while (1)
	{
		char* AcceptedStr = NULL;
		RecvRes = ReceiveString(&AcceptedStr, m_socket);


		if (RecvRes == TRNS_FAILED)	{
			printf("Socket error while trying to write data to socket\n");
			return 0x555;
		}
		else if (RecvRes == TRNS_DISCONNECTED)	{
			printf("Server closed connection. Bye!\n");
			return 0x555;
		}


		if (strstr(AcceptedStr, SERVER_APPROVED) != NULL) {
			printf("The server approved you.\n");
		}
		else if (strstr(AcceptedStr, SERVER_MAIN_MENU) != NULL) {
			printf("Choose what to do next:\n1. Play against another client\n2. Quit\n");
		}
		else if (strstr(AcceptedStr, SERVER_DENIED) != NULL) {
			printf("Server on %s:%s denied the connection request.\nChoose what to do next:\n1. Try to reconnect\n2. Exit\n", global_serverIP, global_serverPort);
		}
		else if (strstr(AcceptedStr, SERVER_INVITE) != NULL) {
			printf("Game is on!\n");
		}
		else if (strstr(AcceptedStr, SERVER_SETUP_REQUSET) != NULL) {
			global_client_state = SETUP;
			printf("Choose your 4 digits:\n");
		}
		else if (strstr(AcceptedStr, SERVER_PLAYER_MOVE_REQUEST) != NULL) {
			global_client_state = GUESS;
			printf("Choose your guess:\n");
		}
		else if (strstr(AcceptedStr, SERVER_GAME_RESULTS) != NULL) {
			// get parameters
			char** params = MessageParams(AcceptedStr);
			if (params == STATUS_CODE_FAILURE) { return STATUS_CODE_FAILURE;}
			// print
			printf("Bulls: %s\nCows: %s\n%s played: %s\n", *params[0], *params[1], *params[2], *params[3]);
			// free
			for (int i = 0; i < 4; i++) {free(params[i]);}
			free(params);
		}
		else if (strstr(AcceptedStr, SERVER_WIN) != NULL) {
			// get parameters
			char* params = MessageParams(AcceptedStr);
			if (params == STATUS_CODE_FAILURE) { return STATUS_CODE_FAILURE; }
			// print
			printf("%s won !\nopponents number was %s\n", params[0], params[1]);
			// free
			for (int i = 0; i < 2; i++) { free(params[i]); }
			free(params);
		}
		else if (strstr(AcceptedStr, SERVER_DRAW) != NULL) {
			printf("It’s a tie\n");
		}
		else if (strstr(AcceptedStr, SERVER_NO_OPPONENTS) != NULL) {
			 // HANDLE THIS ! count to 30 ?
		}
		else if (strstr(AcceptedStr, SERVER_OPPONENT_QUIT) != NULL) {
			printf("Opponent quit.\n");
		}
		else {
			printf("did not recieve an expected message_type.\n");
		}


		free(AcceptedStr);
	}

	return 0;
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

//Sending data to the server
static DWORD SendDataThread(void)
{
	char SendStr[256];
	TransferResult_t SendRes;

	while (1)
	{
		gets_s(SendStr, sizeof(SendStr)); //Reading a string from the keyboard

			  //?
//		if (STRINGS_ARE_EQUAL(SendStr, "quit"))
//			return 0x555; //"quit" signals an exit from the client side


		if (strcmp(SendStr, "1") == 0) {
			SendRes = SendString(CLIENT_VERSUS, m_socket);
			if (SendRes == TRNS_FAILED) {
				printf("Socket error while trying to write data to socket\n");
				return 0x555;
			}
		}
		else if (strcmp(SendStr, "2") == 0) {
			SendRes = SendString(CLIENT_DISCONNECT, m_socket);
			if (SendRes == TRNS_FAILED) {
				printf("Socket error while trying to write data to socket\n");
				return 0x555;
			}
			return STATUS_CODE_SUCCESS;
		}
		else {
			if (strlen(SendStr) != 4) { printf("tryin to send wierd thing FIXME: %s\n",SendStr); }
			char* send_me = NULL;
			if (global_client_state == SETUP) {
				send_me = CreateClientSend_one_Param(CLIENT_SETUP, SendStr);
			}
			else {
				send_me = CreateClientSend_one_Param(CLIENT_PLAYER_MOVE, SendStr);
			}

			SendRes = SendString(send_me, m_socket);
			if (SendRes == TRNS_FAILED) {
				printf("Socket error while trying to write data to socket\n");
				return 0x555;
			}
			free(send_me);
		}
	}
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

int MainClient(char* argv[])
{
	char* global_serverIP	 = argv[1];
	char* global_serverPort	 = argv[2];
	char* tmp_ptr;
	int	serv_port = strtol(argv[2],&tmp_ptr, 10);
	char* global_username	 = argv[3];

	SOCKADDR_IN clientService;
	HANDLE hThread[2];

	// Initialize Winsock.
	WSADATA wsaData; //Create a WSADATA object called wsaData.
	//The WSADATA structure contains information about the Windows Sockets implementation.

	//Call WSAStartup and check for errors.
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
		printf("Error at WSAStartup()\n");

	//Call the socket function and return its value to the m_socket variable. 
	// For this application, use the Internet address family, streaming sockets, and the TCP/IP protocol.

	// Create a socket.
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Check for errors to ensure that the socket is a valid socket.
	if (m_socket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return STATUS_CODE_FAILURE;
	}
	/*
	 The parameters passed to the socket function can be changed for different implementations.
	 Error detection is a key part of successful networking code.
	 If the socket call fails, it returns INVALID_SOCKET.
	 The if statement in the previous code is used to catch any errors that may have occurred while creating
	 the socket. WSAGetLastError returns an error number associated with the last error that occurred.
	 */


	 //For a client to communicate on a network, it must connect to a server.
	 // Connect to a server.

	 //Create a sockaddr_in object clientService and set  values.
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr(global_serverIP); //Setting the IP address to connect to
	clientService.sin_port = htons(serv_port); //Setting the port to connect to.

	/*
		AF_INET is the Internet address family.
	*/


	// Call the connect function, passing the created socket and the sockaddr_in structure as parameters. 
	// Check for general errors.
	if (connect(m_socket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
		while (1) {
			printf("Failed connecting to server on %s:%s.\nChoose what to do next:\n1. Try to reconnect\n2. Exit\n", global_serverIP, global_serverPort);
				char digit_answer[4];
				gets_s(digit_answer,2* sizeof(char)); //Reading a string from the keyboard
				char* answer = digit_answer;
			if (strstr(answer, "1") != NULL) {
				if (connect(m_socket, (SOCKADDR*)&clientService, sizeof(clientService)) != SOCKET_ERROR) { break; }
			}
			else {
				printf("you chose 2 (%c=2 ?)  bye\n", digit_answer); //FIXME
				WSACleanup();
				return STATUS_CODE_SUCCESS;
			}
		}
	}
	printf("Connected to server on %s:%s\n", global_serverIP, global_serverPort);
	char** param = &argv[3];
	char* send_me = writeMessage(CLIENT_REQUEST,param);
	
	TransferResult_t SendRes = SendString(send_me, m_socket);
	if (SendRes == TRNS_FAILED)
	{
		printf("Socket error while trying to write data to socket\n");
		free(send_me); 
		return STATUS_CODE_FAILURE;
	}
	free(send_me);

	// Send and receive data.
	/*
		In this code, two integers are used to keep track of the number of bytes that are sent and received.
		The send and recv functions both return an integer value of the number of bytes sent or received,
		respectively, or an error. Each function also takes the same parameters:
		the active socket, a char buffer, the number of bytes to send or receive, and any flags to use.

	*/

	hThread[0] = CreateThread(NULL,	0,
		(LPTHREAD_START_ROUTINE)SendDataThread,NULL,0,NULL);
	hThread[1] = CreateThread(NULL,	0,
		(LPTHREAD_START_ROUTINE)RecvDataThread,NULL,0,NULL);

	WaitForMultipleObjects(2, hThread, FALSE, INFINITE);

	TerminateThread(hThread[0], 0x555);
	TerminateThread(hThread[1], 0x555);
	printf("\nif timeout give a chance to reconnect\n\n"); // FIXME
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);

	closesocket(m_socket);

	WSACleanup();

	return STATUS_CODE_SUCCESS;
}


