#pragma once
#include "Node.h"

typedef struct keyword keyword;
struct keyword {
	char* Name;
	char* str;
	int id;
};

keyword* newKeyword();
void freeKeyword(keyword* kw);
int setKeywordName(keyword* kw, char* input, size_t len);
int setKeywordString(keyword* kw, char* input, size_t len);