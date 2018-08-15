#pragma once

#include <stdio.h>
#include "Token.h"

typedef enum parserState {
	PS_EOF,
	PS_ERR,
	PS_OK,
	PS_PARSE,
	PS_START,
	PS_SCAN
} parserState;

typedef struct parser parser;

#define BUFSIZE 4096

struct parser {
	enum parserState psState;
	int lineno;
	int colno;
	char* start;
	char* end;
	char* buf;
	FILE* fp;
};

freeParser(parser* ps1);
tokenType matchToken(FILE** fp, parser* ps1);
tokenType matchOne(parser* ps1);
tokenType matchTwo(parser* ps1);
parser* newParser();
size_t stringContainsSingleCharToken(char* buffer);