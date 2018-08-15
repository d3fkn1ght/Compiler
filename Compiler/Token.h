#pragma once

extern const int maxTokenNameSz;

typedef struct s_token s_token;
typedef enum tokenType tokenType;

typedef struct s_token {
	tokenType tType;
	char* Name;
} s_token;

freeToken(s_token* t1);
s_token* newToken();

enum tokenType {
	NONE,
	ID,
	INT,
	LBRACE,
	LPAREN,
	NEWLINE,
	NUMBER,
	RBRACE,
	RETURN,
	RPAREN,
	SEMICOLON,
	ERROR
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

#define ERROR 11