#include "NetworkInterface.h"

int Initialize_WinSock() 
{
	WSADATA wsa_data;
	int result;
	int retval = 0;
	// Initialize Winsock
	result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (result != 0)
	{
		retval = printf_s("WSAStartup failed: %d\n", result);
		if (retval < 0) printf_s("failed to print error\n");
		return STATUS_CODE_FAILURE;
	}
	return STATUS_CODE_SUCSESS; 
}

int Close_WinSock()
{
	int result;
	int retval = 0;
	// Deinitialize Winsock
	result = WSACleanup();
	if (result != 0) {
		retval = printf_s("WSACleanup failed: %d\n", result);
		if (retval < 0) printf_s("failed to print error\n");
		return STATUS_CODE_FAILURE;
	}
	return  STATUS_CODE_SUCSESS;

}

SOCKET Create_Socket()
{
	SOCKET sock		= INVALID_SOCKET;
	int iFamily		= AF_INET;
	int iType		= SOCK_STREAM;
	int iProtocol	= IPPROTO_TCP;
	int iResult		= 0;

	int retval = 0;
	sock = socket(iFamily, iType, iProtocol);
	if (sock == INVALID_SOCKET)
	{
		wprintf(L"socket function failed with error = %d\n", WSAGetLastError());
		WSACleanup();
	}
	else 
	{
		wprintf_s(L"socket function succeeded\n");
	}
	return sock;
}
int Close_Socket(SOCKET sock)
{
	int iResult = 0;
	int retval = 0;
	iResult = closesocket(sock);
	if (iResult == SOCKET_ERROR)
	{
		wprintf_s(L"closesocket failed with error = %d\n", WSAGetLastError());
		WSACleanup();
		return INVALID_SOCKET;
	}
	return STATUS_CODE_SUCSESS;
}

int Bind_Socket(SOCKET sock ,int port ) 
{
	int retval = 0;
	int iResult;

	
	SOCKADDR_IN	service;

	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr(SERVER_ADDRESS_STR);
	service.sin_port = htons(port);


	iResult = bind(sock, (SOCKADDR*)&service, sizeof(service));
	if (iResult == SOCKET_ERROR) {
		wprintf_s(L"bind failed with error %u\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return Close_Socket(sock);
	}
	else
	{
		retval = wprintf_s(L"bind returned success\n");
		return STATUS_CODE_SUCSESS;
	}
}

int Listen_to_Socket(SOCKET sock, int backlog) 
{
	int iResult = 0;
	int retval = 0;
	iResult = listen(sock, backlog);
	if (iResult == SOCKET_ERROR)
	{
		wprintf_s(L"Listen failed with error %u\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return SOCKET_ERROR;
	}
	return STATUS_CODE_SUCSESS;
}

SOCKET Accept_Socket(SOCKET ListenSocket, const struct sockaddr *addr, int *addrlen)
{
	SOCKET AcceptSocket = INVALID_SOCKET;
	wprintf_s(L"Waiting for client to connect...\n");
	AcceptSocket = accept(ListenSocket, NULL, NULL);
	if (AcceptSocket == INVALID_SOCKET) {
		wprintf_s(L"accept failed with error: %ld\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return INVALID_SOCKET;
	}
	else{
		wprintf_s(L"Client connected.\n");
		return AcceptSocket;
	}
	
}
