#pragma once

extern const int maxTokenNameSz;

typedef struct s_token s_token;
typedef enum tokenType tokenType;

typedef struct s_token {
	tokenType tType;
	char* Name;
} s_token;

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
#define ID 1
#define INT 2
#define LBRACE 3
#define LPAREN 4
#define NEWLINE 5
#define NUMBER 6
#define RBRACE 7
#define RETURN 8
#define RPAREN 9
#define SEMICOLON 10

#define ERROR 11