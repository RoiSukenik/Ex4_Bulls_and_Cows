#ifndef NETWORK_INTERFACE_DOT_H
#define NETWORK_INTERFACE_DOT_H
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#include <string.h>
#include <stdbool.h>

#define STATUS_CODE_FAILURE -1
#define STATUS_CODE_SUCSESS  0
const char* SERVER_ADDRESS_STR = "127.0.0.1";

typedef struct _SOCKET_INFO
{
	int iFamily;// address family specification
	int iType;// type specification for the new socket
	int iProtocol;
}SOCKET_INFO;


char* Initialize_WinSock();

char* Close_WinSock();

SOCKET Create_Socket();

char* Close_Socket(SOCKET sock);

char* Bind_Socket(SOCKET sock, int port);

char* Listen_to_Socket(SOCKET sock, int backlog);

SOCKET Accept_Socket(SOCKET sock, const struct sockaddr* addr, int* addrlen);



#endif // !NETWORK_INTERFACE_DOT_H
