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
		return 1;
	}
}

char* Close_WinSock()
{
	int result;
	int retval = 0;
	// Deinitialize Winsock
	result = WSACleanup();
	if (result != 0) {
		retval = printf_s("WSACleanup failed: %d\n", result);
		if (retval < 0) printf_s("failed to print error\n");
		return STATUS_CODE_SUCSESS;
	}

}

SOCKET Create_Socket(SOCKET_INFO info)
{
	SOCKET sock		= INVALID_SOCKET;
	int iFamily		= info.iFamily;
	int iType		= info.iType;
	int iProtocol	= info.iProtocol;
	int iResult		= 0;

	int retval = 0;
	sock = socket(iFamily, iType, iProtocol);
	if (sock == INVALID_SOCKET) wprintf(L"socket function failed with error = %d\n", WSAGetLastError());
	else 
	{
		retval = wprintf_s(L"socket function succeeded\n");
		if (retval < 0) printf_s("failed to print error\n");
		// Close the socket to release the resources associated
		return Close_Socket(sock);

	}
	return sock;
}
char* Close_Socket(SOCKET sock)
{
	int iResult = 0;
	int retval = 0;
	iResult = closesocket(sock);
	if (iResult == SOCKET_ERROR)
	{
		retval = wprintf_s(L"closesocket failed with error = %d\n", WSAGetLastError());
		if (retval < 0) printf_s("failed to print error\n");
		WSACleanup();
		return INVALID_SOCKET;
	}
	return STATUS_CODE_SUCSESS;
}

char* Bind_Socket(SOCKET sock, const struct sockaddr* name, int namelen) 
{
	int retval = 0;
	int iResult;
	iResult = bind(sock, (SOCKADDR*)&name, sizeof(namelen));
	if (iResult == SOCKET_ERROR) {
		retval = wprintf_s(L"bind failed with error %u\n", WSAGetLastError());
		if (retval < 0) printf_s("failed to print error\n");
		return Close_Socket(sock);
	}
	else
	{
		retval = wprintf_s(L"bind returned success\n");
		if (retval < 0) printf_s("failed to print error\n");
		return STATUS_CODE_SUCSESS;
	}
}

char* Listen_to_Socket(SOCKET sock, int backlog) 
{
	int iResult = 0;
	int retval = 0;
	iResult = listen(sock, backlog);
	if (iResult == SOCKET_ERROR)
	{
		retval = wprintf_s(L"listen failed with error %u\n", WSAGetLastError());
		if (retval < 0) printf_s("failed to print error\n");
		return SOCKET_ERROR;
	}
	retval = wprintf_s(L"bind failed with error %u\n", WSAGetLastError());
	if (retval < 0) printf_s("failed to print error\n");
	return STATUS_CODE_SUCSESS;
}

SOCKET Accept_Socket(SOCKET ListenSocket, const struct sockaddr *addr, int *addrlen)
{
	SOCKET AcceptSocket = INVALID_SOCKET;
	int ret = 0;
	ret = wprintf_s(L"Waiting for client to connect...\n");
	if (ret < 0) printf_s("failed to print waiting for connection\n");
	AcceptSocket = accept(ListenSocket, NULL, NULL);
	if (AcceptSocket == INVALID_SOCKET) {
		ret = wprintf_S(L"accept failed with error: %ld\n", WSAGetLastError());
		if (ret < 0) printf_s("failed to print error\n");
		closesocket(ListenSocket);
		WSACleanup();
		return INVALID_SOCKET;
	}
	else
		wprintf(L"Client connected.\n");
	return AcceptSocket;

}

char* Send_Data(SOCKET sock, const char* buf, int len, int flags)
{
	int iResult = 0;
	int retval = 0;
	iResult = send(sock, buf, len, flags);
	if (iResult == SOCKET_ERROR)
	{
		retval = wprintf_s(L"send failed with error = %d\n", WSAGetLastError());
		if (retval< 0) printf_s("failed to print error\n");
		return STATUS_CODE_FAILURE;
	}
	return STATUS_CODE_SUCSESS;
}

char* Recive_Data(SOCKET ConnectSocket, char* recvbuf, int recvbuflen, int flags)
{
	int iResult = 0;
	int retval = 0;
	iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
	if (iResult > 0)
	{
		retval = printf_s("Bytes received: %d\n", iResult);
		if (retval < 0) printf_s("failed to print bytes received\n");
		return STATUS_CODE_SUCSESS;
	}
	else if (iResult == 0)
	{
		retval = printf_s("Connection closed\n");
		if (retval < 0) printf_s("failed to print connection closed\n");
		return STATUS_CODE_SUCSESS;
	}
	else
	{
		retval = wprintf_s("recv failed: %d\n", WSAGetLastError());
		if (retval < 0) printf_s("failed to print error\n");
		return SOCKET_ERROR;
	}
}
