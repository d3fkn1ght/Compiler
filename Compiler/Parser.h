#pragma once

#include <stdio.h>

typedef enum parserState {
	PS_EOF,
	PS_ERR,
	PS_OK,
	PS_START
} parserState;

typedef struct parser parser;

#define BUFSIZE 257

struct parser {
	enum parserState psState;
	int lineno;
	int colno;
	char* start;
	char* end;
	char* buf;
	FILE* fp;
};

parser* newParser();
freeParser(parser* ps1);