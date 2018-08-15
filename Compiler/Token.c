#include <stdlib.h>
#include <string.h>
#include "Token.h"

const int maxTokenNameSz = 128;

freeToken(s_token* token) {
	if (token != NULL) {
		if (token->Name != NULL) {
			free(token->Name);
		}
		free(token);
	}
}

s_token* newToken() {
	s_token* token = NULL;
	if ((token = (s_token*)malloc(sizeof(s_token))) != NULL) {
		if ((token->Name = (char*)malloc(sizeof(char) * 128)) == NULL) {
			free(token);
			return NULL;
		}

		token->tType = NONE;
		if ((memset(token->Name, 0, maxTokenNameSz)) == NULL) {
			free(token->Name);
			free(token);
			return NULL;
		}
	}

	return token;
}

int setTokenName(s_token* token, char* buf, size_t len) {
	return strncpy_s(token->Name, maxTokenNameSz, buf, len);
}