#include "NetworkInterface.h"

int Initialize_WinSock() 
{
	WSADATA wsa_data;
	int result;
	// Initialize Winsock
	result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (result != 0)
	{
		printf_s("WSAStartup failed: %d\n", result);
		return 1;
	}}int Close_WinSock(){	int result;	// Deinitialize Winsock
	result = WSACleanup();
	if (result != 0) {
		printf("WSACleanup failed: %d\n", result);
		return 1;
	}}SOCKET Create_Socket(SOCKET_INFO info){	SOCKET sock		= INVALID_SOCKET;	int iFamily		= info.iFamily;	int iType		= info.iType;	int iProtocol	= info.iProtocol;	int iResult		= 0;	sock = socket(iFamily, iType, iProtocol);	if (sock == INVALID_SOCKET) wprintf(L"socket function failed with error = %d\n", WSAGetLastError());
	else 
	{
		wprintf_s(L"socket function succeeded\n");
		// Close the socket to release the resources associated		return Close_Socket(sock);	}	return sock;}int Close_Socket(SOCKET sock){	int iResult = 0;	iResult = closesocket(sock);
	if (iResult == SOCKET_ERROR)
	{
		wprintf(L"closesocket failed with error = %d\n", WSAGetLastError());
		WSACleanup();
		return INVALID_SOCKET;
	}	return STATUS_CODE_SUCSESS;}int Bind_Socket(SOCKET sock, const struct sockaddr* name, int namelen) {	int iResult;	iResult = bind(sock, (SOCKADDR*)&name, sizeof(namelen));
	if (iResult == SOCKET_ERROR) {
		wprintf(L"bind failed with error %u\n", WSAGetLastError());
		return Close_Socket(sock);
	}
	else
	{
		wprintf(L"bind returned success\n");
		return STATUS_CODE_SUCSESS;
	}}int Listen_to_Socket(SOCKET sock, int backlog) {	int iResult = 0;	iResult = listen(sock, backlog);	if (iResult == SOCKET_ERROR)	{		wprintf(L"listen failed with error %u\n", WSAGetLastError());		return SOCKET_ERROR;	}	wprintf(L"bind failed with error %u\n", WSAGetLastError());	return STATUS_CODE_SUCSESS;}SOCKET Accept_Socket(SOCKET ListenSocket, const struct sockaddr *addr, int *addrlen){	SOCKET AcceptSocket = INVALID_SOCKET;	wprintf(L"Waiting for client to connect...\n");	AcceptSocket = accept(ListenSocket, NULL, NULL);
	if (AcceptSocket == INVALID_SOCKET) {
		wprintf(L"accept failed with error: %ld\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return INVALID_SOCKET;
	}
	else
		wprintf(L"Client connected.\n");
	return AcceptSocket;
}int Send_Data(SOCKET sock, const char* buf, int len, int flags){	int iResult = 0;	iResult = send(sock, buf, len, flags);	if (iResult == SOCKET_ERROR)	{		wprintf(L"send failed with error = %d\n", WSAGetLastError());		return STATUS_CODE_FAILURE;	}	return STATUS_CODE_SUCSESS;}
int Recive_Data(SOCKET ConnectSocket, char* recvbuf, int recvbuflen, int flags)
{
	int iResult = 0;
	iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
	if (iResult > 0)
	{
		printf_s("Bytes received: %d\n", iResult);
		return STATUS_CODE_SUCSESS;
	}
	else if (iResult == 0)
	{
		printf_s("Connection closed\n");
		return STATUS_CODE_SUCSESS;
	}
	else
	{
		wprintf_s("recv failed: %d\n", WSAGetLastError());
		return SOCKET_ERROR;
	}
}
