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
	}
	result = WSACleanup();
	if (result != 0) {
		printf("WSACleanup failed: %d\n", result);
		return 1;
	}
	else 
	{
		wprintf_s(L"socket function succeeded\n");
		// Close the socket to release the resources associated
	if (iResult == SOCKET_ERROR)
	{
		wprintf(L"closesocket failed with error = %d\n", WSAGetLastError());
		WSACleanup();
		return INVALID_SOCKET;
	}
	if (iResult == SOCKET_ERROR) {
		wprintf(L"bind failed with error %u\n", WSAGetLastError());
		return Close_Socket(sock);
	}
	else
	{
		wprintf(L"bind returned success\n");
		return STATUS_CODE_SUCSESS;
	}
	if (AcceptSocket == INVALID_SOCKET) {
		wprintf(L"accept failed with error: %ld\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return INVALID_SOCKET;
	}
	else
		wprintf(L"Client connected.\n");
	return AcceptSocket;


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