#ifndef NETWORK_INTERFACE_DOT_H
#define NETWORK_INTERFACE_DOT_H

#pragma comment(lib, "Ws2_32.lib")
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>

#define STATUS_CODE_FAILURE -1
#define STATUS_CODE_SUCSESS  0

typedef struct _SOCKET_INFO
{
	int iFamily;// address family specification
	int iType;// type specification for the new socket
	int iProtocol;
}SOCKET_INFO;


int Initialize_WinSock();

char* Close_WinSock();

SOCKET Create_Socket();

char* Close_Socket(SOCKET sock);

char* Bind_Socket(SOCKET sock, const struct sockaddr* name, int namelen);

char* Listen_to_Socket(SOCKET sock, int backlog);

SOCKET Accept_Socket(SOCKET sock, const struct sockaddr* addr, int* addrlen);

int Send_Data(SOCKET sock, const char* buf, int len, int flags);

int Recive_Data(SOCKET ConnectSocket, char* recvbuf, int recvbuflen, int flags);

#endif // !NETWORK_INTERFACE_DOT_H
