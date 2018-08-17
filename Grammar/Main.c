#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
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
*/

int specialTokenOffset = 0;
int oneCharTokenOffset = 0;
int twoCharTokenOffset = 0;
int keywordCount = 0;
int maxRead = 4096;

void error(char* str, errno_t err) {
	fprintf(stderr, "%s: %d\n", str, err);
}

generateHeader(FILE* input, FILE* Grammar, FILE* Token) {
	// header for Grammar.c
	fprintf_s(Grammar, "#define \"Grammar.h\"\n\n");
	fprintf_s(Grammar, "const char* keywords[] = {\n");
	fprintf_s(Grammar, "\t\"NONE\",\n");

	// header for Token.h
	fprintf_s(Token, "#pragma once\n");
	fprintf_s(Token, "\n");
	fprintf_s(Token, "extern const int maxTokenNameSz;\n");
	fprintf_s(Token, "typedef enum tokenType tokenType;\n");
	fprintf_s(Token, "\n");
	fprintf_s(Token, "typedef struct token {\n");
	fprintf_s(Token, "\ttokenType tType;\n");
	fprintf_s(Token, "\tchar* str;\n");
	fprintf_s(Token, "} token;\n");
	fprintf_s(Token, "\n");
	fprintf_s(Token, "freeToken(token* t1);\n\n");
	fprintf_s(Token, "#ifdef _DEBUG\n");
	fprintf_s(Token, "const char* getTokenName(token* t1);\n");
	fprintf_s(Token, "#endif // _DEBUG\n");
	fprintf_s(Token,"\n");
	fprintf_s(Token,"\n");
	fprintf_s(Token,"token* newToken();\n");
	fprintf_s(Token,"int setTokenName(token* t1, char* buf, size_t len);\n");
	fprintf_s(Token,"\n");
	fprintf_s(Token,"enum tokenType {\n");
	fprintf_s(Token,"\tNONE,\n");
}

generateKeywords(FILE* input, FILE* Grammar, FILE* Token) {
	char* line = (char*)malloc(sizeof(char) * maxRead);
	fgets(line, maxRead, Grammar);
	
	if (strncmp(line, "// keywords\n", 11) != 0) {
		error("Top line should be a // keywords", EOTHER);
	}

	for (;;) {
		if (fgets(line, maxRead, Grammar) == NULL) {
			break;
		}
	
		fprintf(Grammar, "\t\"%s\",\n", line);
		break;
		// read in keyword from Grammar
		// write out to Grammar.h
		// write out to Token.h
		// append to buffer to write for Token.h defines
	}
}

generateSpecialTokens(FILE* input, FILE* Grammar, FILE* Token) {

}

generateOneCharTokens(FILE* input, FILE* Grammar, FILE* Token) {

}

generateTwoCharTokens(FILE* input, FILE* Grammar, FILE* Token) {

}

generateFooter(FILE* destFP) {
	fprintf_s(destFP, "const int keywordCount = %d", keywordCount);
}

	/*
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
;

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
#define ERROR		15*/

int main(int argc, char** argv[]) {
	char key[1];
	char* srcFile = "CGrammar";
	char* destFile = "Grammar.tmp";
	char* destFile2 = "Token.h";

	FILE *input = NULL, *Grammar = NULL, *Token = NULL;
	errno_t err;

	if ((err = fopen_s(&input, srcFile, "r")) != 0) {
		error("Error opening source file", err);
		goto end;
	}

	if ((err = fopen_s(&Grammar, destFile, "w")) != 0) {
		error("Error opening destination file", err);
		goto end;
	}

	if ((err = fopen_s(&Token, destFile2, "w")) != 0) {
		error("Error opening destination file", err);
		goto end;
	}

	generateHeader(input, Grammar, Token);
	//generateKeywords(input, Grammar, Token);
	//generateSpecialTokens(input, Grammar, Token);
	//generateOneCharTokens(input, Grammar, Token);
	//generateTwoCharTokens(input, Grammar, Token);
	//generateFooter(Grammar);

end:
	if (input != NULL) {
		fclose(input);
	}

	if (Grammar != NULL) {
		fclose(Grammar);
	}

	if (Token != NULL) {
		fclose(Token);
	}

#ifdef _DEBUG
	fprintf_s(stdout, "Press any key to continue");
	gets_s(key, 1);
#endif // _DEBUG

	return 0;
}