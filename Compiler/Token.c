#include <stdlib.h>
#include <string.h>
#include "Token.h"

const int maxTokenNameSz = 128;

const char* tokenSzNames[] = {
	"NONE",
	"INT",
	"RETURN",
	"ID",
	"NUMBER",
	"LBRACE",
	"LPAREN",
	"NEWLINE",
	"RBRACE",
	"RPAREN",
	"SEMICOLON",
	"PLUS",
	"PLUSPLUS",
	"MINUSMINUS",
	"E_EOF",
	"ERROR"
};

freeToken(token* _token) {
	if (_token != NULL) {
		if (_token->str != NULL) {
			free(_token->str);
		}
		free(_token);
	}
}

#ifdef _DEBUG
const char* getTokenName(token* t1) {
	return tokenSzNames[t1->tType];
}
#endif

token* newToken() {
	token* t1 = NULL;
	if ((t1 = (token*)malloc(sizeof(token))) != NULL) {
		if ((t1->str = (char*)malloc(sizeof(char) * 128)) == NULL) {
			free(t1);
			return NULL;
		}

		t1->tType = NONE;
		if ((memset(t1->str, 0, maxTokenNameSz)) == NULL) {
			free(t1->str);
			free(t1);
			return NULL;
		}
	}

	return t1;
}

int setTokenName(token* t1, char* buf, size_t len) {
	return strncpy_s(t1->str, maxTokenNameSz, buf, len);
}