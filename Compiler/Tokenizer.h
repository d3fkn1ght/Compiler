#pragma once

#include <stdio.h>

#include "Exception.h"
#include "Node.h"
#include "Parser.h"
#include "Token.h"

size_t containsTokenChar(char* buffer);
void eatWhiteSpace(FILE* fp, parser* ps1);
void getLexeme(char* buffer1, size_t* tokenLength);
token* get_tok(FILE* fp, parser* ps1);
int lex(FILE* fp, parser* ps1);
tokenType matchOne(parser* ps1);
tokenType matchTwo(parser* ps1);
tokenType matchToken(parser* ps1, size_t* tokenLength);