#include "MessageAPI.h"


char* MessageType(char* string)
{
	char* MessageType = NULL;
	char* delimiter = ';';
	char* params = NULL;
	MessageType = strtok_s(string, delimiter, &params);
	return MessageType;
}


//Need to release memory after every param extraction and use;
char* MessageParams(char* string)
{
	char* MessageType = NULL;
	char* delimiter = ';';
	char* params = NULL;
	MessageType = strtok_s(string, delimiter, &params);

	char* MessageCloser = '\n';
	char* nextParam = NULL;
	char* currentParam = NULL;

	int amountOfParams = 0;

	currentParam = strtok_s(params, delimiter, &nextParam);
	if (currentParam != MessageCloser) { int amountOfParams = 1; }
	
	while (nextParam != MessageCloser)
	{
		currentParam = strtok_s(params, delimiter, &nextParam);
		amountOfParams++;
	}
	char** p_params_array = NULL;
	p_params_array = malloc(amountOfParams * sizeof(char*));
	if (NULL == p_params_array)
	{
		printf_s("Failed to allocate memory\n\n");
		return STATUS_CODE_FAILURE;
	}
	for (int i = 0; i < amountOfParams; i++)
	{
		currentParam = strtok_s(params, delimiter, &nextParam);
		p_params_array[i] = NULL;
		p_params_array[i] = (char*)malloc(sizeof(currentParam));
		if (NULL == p_params_array[i])
		{
			printf_s("Failed to allocate memory\n\n");
			return STATUS_CODE_FAILURE;
		}
		p_params_array[i] = currentParam;
	}

	return p_params_array;

}
//dont forget to free memory
char* writeMessage(char* MessageType, char* MessageParams[])
{
	size_t buffer = snprintf(NULL, 0, MessageType) + 1;
	char* message = malloc(buffer);
	if (NULL == message) {
		printf_s("Failed to allocate memory\n\n");
		return STATUS_CODE_FAILURE;
	}
	sprintf_s(message, buffer, MessageType);
	for (int i = 0; i < len(MessageParams); i++)
	{
		buffer = snprintf(NULL, 0, "%s;%s",message,MessageParams[i]);
		message = relloc(message, buffer);
		sprintf_s(message, buffer, "%s;%s", message, MessageParams[i]);
	}
	return message;
}