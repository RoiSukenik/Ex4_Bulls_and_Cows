#include "MessageAPI.h"


char* MessageType(char* string)
{
	char* MessageType = NULL;
	char* delimiter = ':';
	char* params = NULL;
	MessageType = strtok_s(string, delimiter, &params);
	return MessageType;
}


//Need to release memory after every param extraction and use;
char* MessageParams(char* string)
{
	
	int amountOfParams = amountOfParamsCalc(string);

	char* MessageType = NULL;
	char* messageTypeDelimiter = ':';
	char* delimiter = ';';
	char* params = NULL;
	MessageType = strtok_s(params, messageTypeDelimiter, &params);


	char* nextParam = NULL;
	char* currentParam = NULL;

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
char* writeMessage(char* MessageType, char** MessageParams)
{
	errno_t retval;
	size_t buffer = snprintf(NULL, 0, MessageType) + 1;
	char* message = malloc(buffer);
	if (NULL == message) {
		printf_s("Failed to allocate memory\n\n");
		return STATUS_CODE_FAILURE;
	}
	retval = sprintf_s(message, buffer,"%s", MessageType);
	int paramLength = sizeof(MessageParams) / sizeof(MessageParams[0]);
	for (int i = 0; i < paramLength; i++)
	{
		if (i == 0)
		{
			buffer = snprintf(NULL, 0, "%s:%s", message, MessageParams[i]) + 1;
			message = realloc(message, buffer);
			sprintf_s(message, buffer, "%s:%s", message, MessageParams[i]);
		}
		else {
			buffer = snprintf(NULL, 0, "%s;%s", message, MessageParams[i]) + 1;
			message = realloc(message, buffer);
			sprintf_s(message, buffer, "%s;%s", message, MessageParams[i]);
		}
		
	}
	buffer = snprintf(NULL, 0, "%s;\n", message) + 1;
	message = realloc(message, buffer);
	sprintf_s(message, buffer, "%s;\n", message);

	return message;
}
void freeParamList(char** messageParams)
{
	int paramLength = sizeof(messageParams) / sizeof(messageParams[0]);
	int i = 0;
	while (i < paramLength)
	{
		free(*messageParams[i]);
		i++;
	}
	free(messageParams);
}

int amountOfParamsCalc(char* string)
{
	char* MessageType = NULL;
	char* messageTypeDelimiter = ':';
	char* delimiter = ';';
	char* params = NULL;
	MessageType = strtok_s(params, messageTypeDelimiter, &params);

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
	return amountOfParams;
}