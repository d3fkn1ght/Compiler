#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Node.h"
#include "Keyword.h"

typedef enum bool{
	false,
	true
} bool;

int keywordCount = 0;
int totalNodes = 0;

nodeList* doubleCharTokens = NULL;
nodeList* keywords = NULL;
nodeList* specials = NULL;
nodeList* singleCharTokens = NULL;

int maxRead = 4096;

typedef enum parseGrammarState {
	PS_STARTED,
	PS_KEYWORDS,
	PS_SPECIAL,
	PS_TWOCHARTOKENS,
	PS_ONECHARTOKENS,
	PS_DEFINES,
	PS_DONE,
	PS_SUCCESS,
	PS_ERROR
} parseGrammarState;

char* readBuffer = NULL;
parseGrammarState psgState = PS_STARTED;

int idPadding(int len) {
	int logNum = 2;
	while (len >= 10 && logNum >= 1) {
		logNum--;
		len /= 10;
	}

	return logNum;
}

void indent(FILE* fp, keyword* kw) {
	int spaces = 20 + (idPadding(kw->id)) - ((int)strlen(kw->Name));

	for (int i = 0; i < spaces; i++) {
		fprintf_s(fp, " ");
	}
}

void ucase(char* input) {
	int index = 0;
	size_t len = strlen(input);

	while (*input && (index < len)) {
		*input = toupper((unsigned char)*input);
		++index;
		++input;
	}
}

int allocateLists() {
	if ((keywords = newNodeList()) == NULL) {
		goto end;
	}

	if ((specials = newNodeList()) == NULL) {
		goto end;
	}

	if ((doubleCharTokens = newNodeList()) == NULL) {
		goto end;
	}

	if ((singleCharTokens = newNodeList()) == NULL) {
		goto end;
	}

	return 0;

end:
	if (keywords != NULL) {
		free(keywords);
	}

	if (specials != NULL) {
		free(specials);
	}

	if (doubleCharTokens != NULL) {
		free(doubleCharTokens);
	}

	if (singleCharTokens != NULL) {
		free(singleCharTokens);
	}

	return -1;
}

void error(char* str, errno_t err) {
	fprintf_s(stderr, "%s: %d\n", str, err);
	psgState = PS_ERROR;
}

void fprintTokens(FILE* Grammar, FILE* Token) {
	char* upper = NULL;

	// write keywords to Grammar
	while (keywords->current != NULL) {
		fprintf_s(Grammar, ",\n\t\"%s\"", (getNode(keywords))->kw->Name);
	}

	fprintf_s(Grammar, "\n};");
	resetHead(keywords);

	// write keywords to Token.h
	while (keywords->current != NULL) {
		node* node1 = getNode(keywords);
		ucase(node1->kw->Name);
		fprintf_s(Token, ",\n\t%s", node1->kw->Name);
	}

	// write specials to Token.h
	while (specials->current != NULL) {
		fprintf_s(Token, ",\n\t%s", (getNode(specials))->kw->Name);
	}

	// write doubleCharTokens to Token.h
	while (doubleCharTokens->current != NULL) {
		fprintf_s(Token, ",\n\t%s", (getNode(doubleCharTokens))->kw->Name);
	}

	// write singleCharTokens to Token.h
	while (singleCharTokens->current != NULL) {
		fprintf_s(Token, ",\n\t%s", (getNode(singleCharTokens))->kw->Name);
	}

	fprintf_s(Token, "\n};\n\n");
}

void freeLists() {
	if (keywords != NULL) {
		freeNodeList(keywords);
	}

	if (specials != NULL) {
		freeNodeList(specials);
	}


	if (doubleCharTokens != NULL) {
		freeNodeList(doubleCharTokens);
	}


	if (singleCharTokens != NULL) {
		freeNodeList(singleCharTokens);
	}

}

int parseKeywords(FILE* input, FILE* Grammar, FILE* Token) {
	node* node1 = NULL;

	fgets(readBuffer, maxRead, input);
	if (strncmp(readBuffer, "// keywords", 11) != 0) {
		error("Format error: expected", EOTHER);
		return -1;
	}

	for (;;) {
		if (fgets(readBuffer, maxRead, input) == NULL) {
			break;
		}

		if (readBuffer[0] == '\n') {
			break;
		}

		if (readBuffer[strlen(readBuffer) - 1] == '\n') {
			readBuffer[strlen(readBuffer) - 1] = '\0';
		}

			
		node1 = newNode();
		if (node1 == NULL) {
			error("Error allocating node", ENOMEM);
			return -1;
		}

		node1->kw = newKeyword();
		if (node1->kw == NULL) {
			error("Error allocating keyword", ENOMEM);
			return -1;
		}

		if ((setKeywordName(node1->kw, readBuffer, strlen(readBuffer))) == -1) {
			error("Error setting keyword name", EOTHER);
			return -1;
		}

		node1->kw->id = ++totalNodes;
		appendNode(keywords,node1);
	}

	keywordCount = totalNodes;
	psgState = PS_KEYWORDS;
	return 0;
}

int parseSpecialTokens(FILE* input, FILE* Grammar, FILE* Token) {
	node* node1 = NULL;

	fgets(readBuffer, maxRead, input);
	if (strncmp(readBuffer, "// special ", 10) != 0) {
		error("Format error: expected // special", EOTHER);
		return -1;
	}

	for (;;) {
		if (fgets(readBuffer, maxRead, input) == NULL) {
			break;
		}

		if (readBuffer[0] == '\n') {
			break;
		}

		if (readBuffer[strlen(readBuffer) - 1] == '\n') {
			readBuffer[strlen(readBuffer) - 1] = '\0';
		}

		node1 = newNode();
		if (node1 == NULL) {
			error("Error allocating node", ENOMEM);
			return -1;
		}

		node1->kw = newKeyword();
		if (node1->kw == NULL) {
			error("Error allocating keyword", ENOMEM);
			return -1;
		}

		if ((setKeywordName(node1->kw, readBuffer, strlen(readBuffer))) == -1) {
			error("Error setting keyword name", EOTHER);
			return -1;
		}
		node1->kw->id = ++totalNodes;
		appendNode(specials, node1);
	}

	psgState = PS_SPECIAL;
	return 0;
}

parseSingleCharTokens(FILE* input, FILE* Grammar, FILE* Token) {
	node* node1 = NULL;

	fgets(readBuffer, maxRead, input);
	if (strncmp(readBuffer, "// single char tokens", 21) != 0) {
		error("Format error: expected // single char tokens", EOTHER);
		return -1;
	}

	for (;;) {
		if (fgets(readBuffer, maxRead, input) == NULL) {
			break;
		}

		if (readBuffer[0] == '\n') {
			break;
		}

		if (readBuffer[strlen(readBuffer) - 1] == '\n') {
			readBuffer[strlen(readBuffer) - 1] = '\0';
		}

		node1 = newNode();
		if (node1 == NULL) {
			error("Error allocating node", ENOMEM);
			return -1;
		}

		node1->kw = newKeyword();
		if (node1->kw == NULL) {
			error("Error allocating keyword", ENOMEM);
			return -1;
		}

		if ((setKeywordName(node1->kw, readBuffer, strlen(readBuffer) - 2)) == -1) {
			error("Error setting keyword name", EOTHER);
			return -1;
		}

		if ((setKeywordString(node1->kw, readBuffer, 1)) == -1) {
			error("Error setting keyword string", EOTHER);
			return -1;
		}

		node1->kw->id = ++totalNodes;
		appendNode(singleCharTokens, node1);
	}

	psgState = PS_ONECHARTOKENS;
	return 0;
}

parseDoubleCharTokens(FILE* input, FILE* Grammar, FILE* Token) {
	node* node1 = NULL;

	fgets(readBuffer, maxRead, input);
	if (strncmp(readBuffer, "// double char tokens ", 21) != 0) {
		error("Format error: expected // double char tokens", EOTHER);
		return -1;
	}

	for (;;) {
		if (fgets(readBuffer, maxRead, input) == NULL) {
			break;
		}

		if (readBuffer[0] == '\n') {
			break;
		}

		if (readBuffer[strlen(readBuffer) - 1] == '\n') {
			readBuffer[strlen(readBuffer) - 1] = '\0';
		}

		node1 = newNode();
		if (node1 == NULL) {
			error("Error allocating node", ENOMEM);
			return -1;
		}

		node1->kw = newKeyword();
		if (node1->kw == NULL) {
			error("Error allocating keyword", ENOMEM);
			return -1;
		}

		if ((setKeywordName(node1->kw, readBuffer, strlen(readBuffer) - 3)) == -1) {
			error("Error setting keyword name", EOTHER);
			return -1;
		}

		if ((setKeywordString(node1->kw, readBuffer, 2)) == -1) {
			error("Error setting keyword string", EOTHER);
			return -1;
		}

		node1->kw->id = ++totalNodes;
		appendNode(doubleCharTokens, node1);
	}

	psgState = PS_TWOCHARTOKENS;
	return 0;
}

void writeFooter(FILE* Token) {
	fprintf_s(Token, "const int keywordCount = %d\n\n", keywordCount);

	resetHead(keywords);
	fprintf_s(Token, "// keywords\n");

	while (keywords->current != NULL) {
		keyword* kw = (getNode(keywords))->kw;
		fprintf_s(Token, "#define %s", kw->Name);
		indent(Token, kw);
		fprintf_s(Token, "%d\n", kw->id);
	}

	resetHead(specials);
	fprintf_s(Token, "\n// special tokens\n");
	while (specials->current != NULL) {
		keyword* kw = (getNode(specials))->kw;
		fprintf_s(Token, "#define %s", kw->Name);
		indent(Token, kw);
		fprintf_s(Token, "%d\n", kw->id);
	}

	resetHead(doubleCharTokens);
	fprintf_s(Token, "\n// double char tokens\n");
	while (doubleCharTokens->current != NULL) {
		keyword* kw = (getNode(doubleCharTokens))->kw;
		fprintf_s(Token, "#define %s", kw->Name);
		indent(Token, kw);
		fprintf_s(Token, "%d\n", kw->id);
	}

	resetHead(singleCharTokens);
	fprintf_s(Token, "\n// single char tokens\n");
	while (singleCharTokens->current != NULL) {
		keyword* kw = (getNode(singleCharTokens))->kw;
		fprintf_s(Token, "#define %s", kw->Name);
		indent(Token, kw);
		fprintf_s(Token, "%d\n", kw->id);
	}
}

writeHeader(FILE* input, FILE* Grammar, FILE* Token) {
	// header for Grammar.c
	fprintf_s(Grammar, "#define \"Grammar.h\"\n\n");
	fprintf_s(Grammar, "const char* keywords[] = {\n");
	fprintf_s(Grammar, "\t\"NONE\"");

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
	fprintf_s(Token, "int freeToken(token* t1);\n\n");
	fprintf_s(Token, "#ifdef _DEBUG\n");
	fprintf_s(Token, "const char* getTokenName(token* t1);\n");
	fprintf_s(Token, "#endif // _DEBUG\n");
	fprintf_s(Token, "\n");
	fprintf_s(Token, "\n");
	fprintf_s(Token, "token* newToken();\n");
	fprintf_s(Token, "int setTokenName(token* t1, char* buf, size_t len);\n");
	fprintf_s(Token, "\n");
	fprintf_s(Token, "enum tokenType {\n");
	fprintf_s(Token, "\tNONE");
}

int main(int argc, char** argv[]) {
	char* srcFile = "CGrammar";
	char* destFile = "Grammar.tmp";
	char* destFile2 = "Token.h";

	FILE* input = NULL;
	FILE* Grammar = NULL;
	FILE* Token = NULL;
	errno_t err;

	if (allocateLists() == -1) {
		// set error
		goto end;
	}

	if((readBuffer = (char*)malloc(sizeof(char) * maxRead)) == NULL) {
		error("Could not create readBuffer", ENOMEM);
		goto end;
	}


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

	parseKeywords(input, Grammar, Token);
	parseSpecialTokens(input, Grammar, Token);
	parseDoubleCharTokens(input, Grammar, Token);
	parseSingleCharTokens(input, Grammar, Token);

	writeHeader(input, Grammar, Token);
	fprintTokens(Grammar, Token);
	writeFooter(Token);
	
end:
	if (readBuffer != NULL) {
		free(readBuffer);
	}

	if (input != NULL) {
		fclose(input);
	}

	if (Grammar != NULL) {
		fclose(Grammar);
	}

	if (Token != NULL) {
		fclose(Token);
	}

	freeLists();

	return 0;
}