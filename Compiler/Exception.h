#pragma once

typedef struct exception exception;

typedef enum errorType {
	E_NONE,
	E_ERR,
	E_ARGPARSE,
	E_BADARGUMENT,
	E_FILEACCESS,
	E_MEMORY
} errorType;

struct exception {
	errorType errorType;
	char* errorStr;
	char* errorInfo;
	errno_t err;
};

freeException(exception* e1);
exception* new_exception();
int setException(exception* e1, const char* errorStr, enum errorType eType, const char* errorInfo);