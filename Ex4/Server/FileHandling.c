#include "FileHandling.h"

int write_to_file(char* string)
{
	int ret_val;
	int string_type;
	DWORD return_val;
	HANDLE gameLog = NULL;

	gameLog = CreateFileA(PATH, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (gameLog == INVALID_HANDLE_VALUE) {
		printf_s("Write request failed, Last Error = %s\n ", GetLastError());
		return STATUS_CODE_FAILURE;
	}
	return_val = SetFilePointer(gameLog, 0, NULL, FILE_BEGIN);//set pointer to end of file. threads start writing from this point
	if (return_val == INVALID_SET_FILE_POINTER) {
		printf("Last Error = %d\n ", GetLastError());
		return STATUS_CODE_FAILURE;
	}
	SetEndOfFile(gameLog);
	ret_val = WriteFile(gameLog, string, strlen(string) + 1, NULL, NULL);//write to output file
	if (ret_val == 0) {
		printf("Write File Falied Last Error = %d\n ", GetLastError());
		return STATUS_CODE_FAILURE;
	}
	return STATUS_CODE_SUCSESS;
}

int critical_read_code(char* read_line)
{
	HANDLE gameLog = NULL;
	gameLog = CreateFileA(PATH, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (gameLog == INVALID_HANDLE_VALUE) {
		printf("Read request failed, Last Error = %s\n ", GetLastError());
		return STATUS_CODE_FAILURE;
	}


	int setfile_returned_value = SetFilePointer(
		gameLog,
		0,
		NULL,
		FILE_BEGIN);
	if (setfile_returned_value == INVALID_SET_FILE_POINTER) {
		printf("SetFilePointer failed in hte request, printing last ERROR = %d\n", GetLastError());
		CloseHandle(gameLog);
		return STATUS_CODE_FAILURE;
	}
	DWORD      nNumberOfBytesToRead = MAX_STR_LEN;
	DWORD      lpNumberOfBytesRead;

	setfile_returned_value = ReadFile(gameLog,
		read_line,
		nNumberOfBytesToRead,
		&lpNumberOfBytesRead,
		NULL);
	if (setfile_returned_value == 0) {
		printf("ReadFile Failed on request, last error reveived : %d \n", GetLastError());
		free(read_line);
		CloseHandle(gameLog);
		return STATUS_CODE_FAILURE;
	}
	errno_t ret_val = CloseHandle(gameLog);
	if (ret_val == 0) printf("oh no");
}
void DeleteFile()
{
	
	DeleteFileA(PATH);
}



