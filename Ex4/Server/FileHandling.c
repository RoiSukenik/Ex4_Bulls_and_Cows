#include "FileHandling.h"

int write_to_file(HANDLE gameLog, char* string)
{
	int ret_val;
	int string_type;
	DWORD return_val;
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
//
//
//char* critical_section(int* my_state, char* string_to_write,HANDLE communication_semaphore, HANDLE gameLog ,char* path) {
//	WaitForSingleObject(communication_semaphore, WAIT_15_SECONDS);
//	int retval;
//	int string_type;
//
//	DWORD return_val;
//	DWORD read_bytes = MIN_READ_SIZE;
//
//	char* string_from_file = NULL;
//	
//	if (gameLog == NULL) {
//		gameLog = CreateFileA(path, (GENERIC_WRITE | GENERIC_READ), (FILE_SHARE_WRITE | FILE_SHARE_READ), NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//		if (gameLog == INVALID_HANDLE_VALUE) {
//			printf("File Openning Failed Last Error = %d\n ", GetLastError());
//			return NULL;
//		}
//	}
//	if (state == 0) {
//		*my_state = 1;
//		state = 1;
//	}
//	else if (state == 2) {
//		if (*my_state == 0) {
//			*my_state = 2;
//		}
//	}
//	if (state == *my_state) {
//		if (critical_section_counter != 0) {
//			string_from_file = (char*)malloc(MAX_STR_LEN * sizeof(char));
//			if (string_from_file == NULL) {
//				ReleaseSemaphore(communication_semaphore, 1, NULL);
//				return NULL;
//			}
//			return_val = SetFilePointer(gameLog, 0, NULL, FILE_BEGIN);// set pointer to line for reading
//			if (return_val == INVALID_SET_FILE_POINTER) {
//				printf("set file pointer failed error code = %d\n", GetLastError());
//				free(string_from_file);
//				ReleaseSemaphore(communication_semaphore, 1, NULL);
//				return NULL;
//			}
//			retval = ReadFile(gameLog, string_from_file, MAX_STR_LEN, &read_bytes, NULL);// read number from file
//			if (retval == 0) {
//				printf("ReadFile Failed error code = %d\n", GetLastError());
//				free(string_from_file);
//				ReleaseSemaphore(communication_semaphore, 1, NULL);
//				return NULL;
//			}
//			if (critical_section_counter == 2) {
//				return_val = SetFilePointer(gameLog, 0, NULL, FILE_BEGIN);// set pointer to line for reading
//				if (return_val == INVALID_SET_FILE_POINTER) {
//					printf("set file pointer failed error code = %d\n", GetLastError());
//					free(string_from_file);
//					ReleaseSemaphore(communication_semaphore, 1, NULL);
//					return NULL;
//				}
//				SetEndOfFile(gameLog);
//			}
//		}
//		if (critical_section_counter != 2) {
//			write_to_file(gameLog, string_to_write);
//		}
//		state = 3 - *my_state;
//		critical_section_counter++;
//		if (critical_section_counter == 3) {
//			critical_section_counter = 0;
//		}
//	}
//	ReleaseSemaphore(communication_semaphore, 1, NULL);
//	return string_from_file;
//}
int critical_read_code(HANDLE gameLog,char* read_line)
{
	

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