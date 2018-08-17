#pragma once

extern const int maxTokenNameSz;
extern const int keywordCount;
extern const char* keywords[];
extern char* tokenNames[];

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
	ID,
	NUMBER,
	LBRACE,
	LPAREN,
	NEWLINE,
	RBRACE,
	RPAREN,
	SEMICOLON,
	PLUS,
	PLUSPLUS,
	MINUSMINUS,
	E_EOF,
	ERROR,
	SPACE
};

#define NONE 0
// keywords
#define INT			1
#define RETURN		2

#define ID			3
#define NUMBER		4

// one and two char symbols like operators
#define LBRACE		5
#define LPAREN		6
#define NEWLINE		7
#define RBRACE		8
#define RPAREN		9
#define SEMICOLON	10
#define PLUS		11
#define PLUSPLUS	12
#define MINUSMINUS  13

#define E_EOF		14
#define ERROR		15