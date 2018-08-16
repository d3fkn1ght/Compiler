#pragma once

#include <stdio.h>
#include "Token.h"

typedef enum parserState {
	PS_COMMENT,
	PS_EOF,
	PS_ERR,
	PS_MORE,
	PS_SCAN,
	PS_STRING,
} parserState;

typedef struct parser parser;

#define BUFSIZE 4096

struct parser {
	enum parserState psState;
	int lineno;
	int colno;
	char* start;
	char* end;
	char* buffer1;
	char* buffer2;
	FILE* fp;
};

freeParser(parser* ps1);
parser* newParser();