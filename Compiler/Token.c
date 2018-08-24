#include <stdlib.h>
#include <string.h>

#include "Grammar.h"
#include "Token.h"

const int maxTokenNameSz = 128;

freeToken(token* t1) {
	if (t1 != NULL) {
		if (t1->str != NULL) {
			free(t1->str);
		}

		free(t1);
	}
}

token* newToken() {
	token* t1 = NULL;
	if ((t1 = (token*)malloc(sizeof(token))) != NULL) {
		if ((t1->str = (char*)malloc(sizeof(char) * 128)) == NULL) {
			free(t1);
			return NULL;
		}

		t1->tType = NONE;
		memset(t1->str, 0, maxTokenNameSz);
	}

	return t1;
}

resetToken(token* token) {
	token->tType = NONE;
	memset(token->str, 0, maxTokenNameSz);
}

int setTokenName(token* t1, char* buf, size_t len) {
	return strncpy_s(t1->str, maxTokenNameSz, buf, len);
}