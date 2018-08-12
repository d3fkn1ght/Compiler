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
};

exception* new_exception();
int setException(exception* e1, const char* errorStr, enum errorType eType, const char* errorInfo);