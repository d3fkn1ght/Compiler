#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Exception.h"

int maxErrorStrSz = 2050;
int	maxErrorInfoSz = 130;

exception* e1 = NULL;

exception* new_exception() {
	// check if malloc works for each piece of struct
	exception* e2;
	e2 = (exception*)malloc(sizeof(exception));
	e2->errorType = E_NONE;
	e2->errorStr = (char *)malloc(sizeof(char) * maxErrorStrSz);
	memset(e2->errorStr, 0, maxErrorStrSz);

	e2->errorInfo = (char *)malloc(sizeof(char) * maxErrorInfoSz);
	memset(e2->errorInfo, 0, maxErrorInfoSz);

	if (e2 == NULL) {
		return NULL;
	}

	return e2;
}

int setException(exception* e1, const char* errorStr, enum errorType eType, const char* errorInfo) {
	e1->errorType = eType;
	strncpy_s(e1->errorStr, maxErrorStrSz, errorStr, strlen(errorStr));
	strncpy_s(e1->errorInfo, maxErrorInfoSz, errorInfo, strlen(errorInfo));
	return 0;
}