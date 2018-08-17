#pragma once

extern const int maxTokenNameSz;
typedef enum tokenType tokenType;

typedef struct token {
	tokenType tType;
	char* str;
} token;

freeToken(token* t1);

#ifdef _DEBUG
const char* getTokenName(token* t1);
#endif // _DEBUG


token* newToken();
int setTokenName(token* t1, char* buf, size_t len);

enum tokenType {
	NONE,
	INT,
	RETURN,
