#pragma once

#include <stdio.h>

#include "Exception.h"
#include "Node.h"
#include "Parser.h"

int get_tok(FILE* fp, parser* ps1);
int lex(FILE* fp, parser* ps1);
int swapBuffer(parser* ps1);