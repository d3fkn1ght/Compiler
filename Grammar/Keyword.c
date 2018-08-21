#include <stdlib.h>
#include <string.h>

#include "Node.h"
#include "Keyword.h"

size_t maxKeywordNameLength = 128;
size_t maxKeywordStrLenght = 3;

void freeKeyword(keyword* kw) {
	if (kw != NULL) {
		if (kw->Name != NULL) {
			free(kw->Name);
		}
		free(kw);
	}
}

keyword* newKeyword() {
	keyword* kw = (keyword*)malloc(sizeof(keyword));

	if (kw != NULL) {
		kw->Name = (char*)malloc(sizeof(char) * maxKeywordNameLength);
		kw->str = (char*)malloc(sizeof(char) * 3);
		if ((kw->Name != NULL) && (kw->str != NULL)) {
			return kw;
		}
	}
	return NULL;
}
int setKeywordName(keyword* kw, char* input, size_t len) {
	if (strncpy_s(kw->Name, maxKeywordNameLength, input, len))
		return -1;

	return 0;
}

int setKeywordString(keyword* kw, char* input, size_t len) {
	if (strncpy_s(kw->str, maxKeywordStrLenght, input, len))
		return -1;

	return 0;
}