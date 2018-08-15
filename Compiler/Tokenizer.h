#pragma once

#include <stdio.h>

#include "Node.h"
#include "Parser.h"

void get_nextTok(FILE** fp, parser* ps1);
node* get_tok(FILE** fp, parser* ps1);
tokenType matchToken(FILE** fp, parser* ps1, int* tokenLength);
tokenType matchOne(parser* ps1);
tokenType matchTwo(parser* ps1);