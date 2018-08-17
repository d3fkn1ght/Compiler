#include "Grammar.h"

const int keywordCount = 3;

const char* keywords[] = {
	"NONE",
	"int",
	"return"
};

const char* tokenNames[] = {
	"NONE",
	"INT",
	"RETURN",
	"ID",
	"NUMBER",
	"LBRACE",
	"LPAREN",
	"NEWLINE",
	"RBRACE",
	"RPAREN",
	"SEMICOLON",
	"PLUS",
	"PLUSPLUS",
	"MINUSMINUS",
	"E_EOF",
	"ERROR"
};