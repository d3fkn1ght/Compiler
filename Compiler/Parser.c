#include <stdlib.h>
#include <string.h>

#include "Node.h"
#include "Parser.h"

const char* tokenNames[] = {
	"NONE",
	"int",
	"return"
};

node* node1 = NULL;
size_t currentBUFSIZE = 0;

#define possibleNumberStart(c) (\
				((c >= '0' && c <= '9') || (c == '-') || (c == '.')))

#define possibleNumberDigit(c) (\
				((c >= '0' && c <= '9') || (c == '.')))

#define possibleIDStart(c) (\
              (c >= 'a' && c <= 'z')\
               || (c >= 'A' && c <= 'Z')\
               || (c == '_')\
               || (c >= 128))

#define possibleIDChar(c) (\
              (c >= 'a' && c <= 'z')\
               || (c >= 'A' && c <= 'Z')\
               || (c == '_')\
               || (c >= 128)\
			   || ((c >= '0') && (c <= '9')))

parser* newParser() {
	parser* ps1 = (parser*)malloc(sizeof(parser));
	if (ps1 != NULL) {
		ps1->psState = PS_START;
		ps1->lineno = 0;
		ps1->colno = 0;
		ps1->buf = (char*)malloc(sizeof(char) * BUFSIZE);
		if (ps1->buf == NULL) {
			free(ps1);
			return NULL;
		}
		ps1->start = ps1->buf;
		ps1->end = ps1->buf + BUFSIZE;
	}
	return ps1;
}

freeParser(parser* ps1)
{
	if (ps1->buf != NULL) {
		free(ps1->buf);
	}

	if (ps1 != NULL) {
		free(ps1);
	}
}

tokenType matchOne(parser* ps1) {
	switch (ps1->buf[0]) {
	case '{':
		return LBRACE;
	case '}':
		return RBRACE;
	case '(':
		return LPAREN;
	case ')':
		return RPAREN;
	case ';':
		return SEMICOLON;
	default:
		return NONE;
	}
}

tokenType matchTwo(parser* ps1) {
	// try to match two chars
	return NONE;
}

tokenType matchToken(FILE** fp, parser* ps1) {
	int index = 0;
	size_t tokenLength = 0;
	size_t tokenScanLength = 0;
	tokenType matchToken = NONE;

	if (ps1->psState == PS_ERR) {
		// throw error
	}

	if (ps1->psState == PS_EOF) {
		// throw error
		return ERROR;
	}

	if ((matchToken = matchOne(ps1)) != NONE) {
		tokenLength = 1;
		return (int)matchToken;
	}

	if ((matchToken = matchTwo(ps1)) != NONE) {
		tokenLength = 2;
		return (int)matchToken;
	}

	if ((tokenScanLength = stringContainsSingleCharToken(ps1->start)) < 0) {
		tokenScanLength = strlen(ps1->buf);
	}

	for (int i = 1; i < (sizeof(tokenNames) / sizeof(const char*)); i++) {
		if ((strncmp(ps1->start, tokenNames[i], tokenScanLength)) == 0) {
			tokenLength = strlen(tokenNames[i]);
			return (tokenType)i;
		}
	}

	index = 0;
	if (possibleNumberStart(ps1->buf[index])) {
		index++;
		tokenLength += 1;
		while (possibleNumberDigit(ps1->buf[index])) {
			index++;
			tokenLength += 1;
		}
		return NUMBER;
	}

	if (possibleIDStart(ps1->buf[index])) {
		index++;
		tokenLength += 1;
		while (possibleIDChar(ps1->buf[index])) {
			index++;
			tokenLength += 1;
		}
		return ID;
	}

	return NONE;
}

size_t stringContainsSingleCharToken(char* buffer) {
	char* singleCharTokens = "{}();";
	char* test = strpbrk(buffer, singleCharTokens);

	if (test != NULL) {
		return (size_t)(test - buffer);
	}

	return -1;
}