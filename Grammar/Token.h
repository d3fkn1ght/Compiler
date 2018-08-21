#pragma once

extern const int maxTokenNameSz;
typedef enum tokenType tokenType;

typedef struct token {
	tokenType tType;
	char* str;
} token;

int freeToken(token* t1);

#ifdef _DEBUG
const char* getTokenName(token* t1);
#endif // _DEBUG


token* newToken();
int setTokenName(token* t1, char* buf, size_t len);

enum tokenType {
	NONE,
	INT,
	RETURN,
	ID,
	NUMBER,
	PLUSPLUS,
	MINUSMINUS,
	LPAREN,
	RPAREN,
	LBRACE,
	RBRACE,
	SEMICOLON,
	PLUS
};

const int keywordCount = 2

// keywords
#define INT                   1
#define RETURN                2

// special tokens
#define ID                    3
#define NUMBER                4

// double char tokens
#define PLUSPLUS              5
#define MINUSMINUS            6

// single char tokens
#define LPAREN                7
#define RPAREN                8
#define LBRACE                9
#define RBRACE               10
#define SEMICOLON            11
#define PLUS                 12
