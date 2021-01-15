#include "main.h"

int main(int argc, char* argv[])
{
	if (argc != 4) {
		printf("Wrong number of arguments given for client\n");
		return STATUS_CODE_FAILURE;
	}

	return MainClient(*argv);
}