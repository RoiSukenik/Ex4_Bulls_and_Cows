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

#define CLIENT_DISCONNECT			"CLIENT_DISCONNECT"
#define CLIENT_REQUEST				"CLIENT_REQUEST"
#define CLIENT_VERSUS				"CLIENT_VERSUS"
#define CLIENT_SETUP				"CLIENT_SETUP"
#define CLIENT_PLAYER_MOVE			"CLIENT_PLAYER_MOVE"
#define CLIENT_DISCONNECT			"CLIENT_DISCONNECT"

#define STATUS_CODE_FAILURE -1
#define STATUS_CODE_SUCCESS 0


#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#include "NetworkInterface.h"
#include "SocketSendRecvTools.h"
	// #include "SocketSendRecvTools.h"

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

char* global_serverIP;
char* global_serverPort;
char* global_username;
SOCKET m_socket;

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

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
			printf("Choose your 4 digits:\n");
		}
		else if (strstr(AcceptedStr, SERVER_PLAYER_MOVE_REQUEST) != NULL) {
			printf("Choose your guess:\n");
		}
		else if (strstr(AcceptedStr, SERVER_GAME_RESULTS) != NULL) {
			printf("\n");
		}
		else if (strstr(AcceptedStr, SERVER_WIN) != NULL) {
			printf("\n");
		}
		else if (strstr(AcceptedStr, SERVER_DRAW) != NULL) {
			printf("It’s a tie\n");
		}
		else if (strstr(AcceptedStr, SERVER_NO_OPPONENTS) != NULL) {
			printf("\n"); // HANDLE THIS !
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

	//while (1)
	//{
		gets_s(SendStr, sizeof(SendStr)); //Reading a string from the keyboard
		
			  //?
		if (STRINGS_ARE_EQUAL(SendStr, "quit"))
			return 0x555; //"quit" signals an exit from the client side



		SendRes = SendString(SendStr, m_socket);

		if (SendRes == TRNS_FAILED)
		{
			printf("Socket error while trying to write data to socket\n");
			return 0x555;
		}

		// do the flow:
		// first time, send username 20 chars

		//


	//}
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

int MainClient(char* argv[])
{
	char* global_serverIP	 = argv[1];
	char* global_serverPort	 = argv[2];
	int	serv_port	    = atoi(argv[2]);
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
			printf("Failed connecting to server on %s:%s.\nChoose what to do next:\n1. Try to reconnect\n2. Exit\n");
				char digit_answer;
				gets_s(digit_answer, sizeof(digit_answer)); //Reading a string from the keyboard
			if (strstr(digit_answer, "1") != NULL) {
				if (connect(m_socket, (SOCKADDR*)&clientService, sizeof(clientService)) != SOCKET_ERROR) { break; }
			}
			else {
				printf("you chose 2 (%c=2 ?)  bye\n", digit_answer); //FIXME
				WSACleanup();
				return STATUS_CODE_FAILURE;
			}
		}
	}
	printf("Connected to server on %s:%s\n", global_serverIP, serv_port);
	char* send_me = CreateUsernameClientSend(argv[4]);
	TransferResult_t SendRes = SendString(send_me, m_socket);
	if (SendRes == TRNS_FAILED)
	{
		printf("Socket error while trying to write data to socket\n");
		free(send_me);
		return STATUS_CODE_SUCCESS;
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

char* CreateUsernameClientSend(char* UserName){
	char* merged = NULL;
	merged = (char*)malloc(strlen(UserName) + strlen(CLIENT_REQUEST) + 3);
	strcat(merged, CLIENT_REQUEST);
	strcat(merged, ":");
	strcat(merged, UserName);
	strcat(merged, "\n\0");
	return merged;			//check this FIXME
}