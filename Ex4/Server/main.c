/*
Authors – Roi Sukenik 312230741 && Eldor Fadida 205774748
Project – Bulls & Cows - ISP Ex 4
Description – This Project will Show case the use of:
Multi-Threading, Semaphore, Mutex and Network Protocals - in perticular the use of TCP Sockets
*/

#include "main.h"
#include "FileHandling.h"
int main(int argc, char* argv[])
{
	char* userName = "Tom";
	char* message = "Im home";
	char* returnmsg =NULL;
	fileMessageFormatter(userName, message, returnmsg);
	char otheruserName[MAX_USER_NAME_LEN];
	char content[500];
	getMessageContent(returnmsg,&userName,&content);
	

	Manage_Server(argv);
	return 0;
}