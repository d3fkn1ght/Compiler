#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Allocator.h"
#include "Exception.h"

int maxErrorStrSz = 2050;
int	maxErrorInfoSz = 130;

freeException(exception* e1) {
	if (e1 != NULL) {
		if (e1->errorInfo != NULL) {
			free(e1->errorInfo);
		}

		if (e1->errorStr != NULL) {
			free(e1->errorStr);
		}
	}

	e1->errorInfo = NULL;
	e1->errorStr = NULL;
	free(e1);
}


exception* new_exception() {
	// check if malloc works for each piece of struct
	exception* e1 = NULL;

	malloc(52);

	if ((e1 = (exception*)malloc(sizeof(exception))) == NULL) {
		return e1;
	}

	if ((e1->errorInfo = (char *)malloc(sizeof(char) * maxErrorInfoSz)) == NULL) {
		free(e1);
		return NULL;
	}

	if((e1->errorStr = (char *)malloc(sizeof(char) * maxErrorStrSz)) == NULL) {
		free(e1->errorInfo);
		free(e1);
		return NULL;
	}

	memset(e1->errorInfo, 0, maxErrorInfoSz);
	memset(e1->errorStr, 0, maxErrorStrSz);
	e1->err = 0;
	e1->errorType = E_NONE;

	return e1;
}

int setException(exception* e1, const char* errorStr, enum errorType eType, const char* errorInfo) {
	e1->errorType = eType;
	strncpy_s(e1->errorStr, maxErrorStrSz, errorStr, strlen(errorStr));
	strncpy_s(e1->errorInfo, maxErrorInfoSz, errorInfo, strlen(errorInfo));
	return 0;
}