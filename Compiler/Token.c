#include <stdlib.h>
#include <string.h>
#include "Token.h"

const int maxTokenNameSz = 128;
const int keywordCount = 3;

const char* keywords[] = {
	"NONE",
	"int",
	"return"
};

char* tokenNames[] = {
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