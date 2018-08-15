#include <stdlib.h>
#include <string.h>

#include "Tokenizer.h"

size_t bufferCount = 0;
char* readBuffer = NULL;
int lineSize = 256;

int get_tok(FILE* fp, parser* ps1) {
	if ((fgets(readBuffer, BUFSIZE, fp)) == NULL) {
			return -1;
	}

	if (readBuffer != NULL) {
		if ((strncpy_s(ps1->buf, BUFSIZE - bufferCount, readBuffer, strlen(readBuffer)) == -1)) {
			// throw error
			return -1;
		}
	}

	bufferCount += strlen(readBuffer);
	ps1->buf += strlen(readBuffer);

	if ((BUFSIZE - bufferCount < lineSize)) {
		bufferCount = 0;
		swapBuffer(ps1);
		ps1->psState = PS_PARSE;
		return -1;
	}
	return 0;
}


int lex(FILE* fp, parser* ps1) {
	node* node1 = NULL;
	if ((readBuffer = (char*)malloc(sizeof(char) * BUFSIZE)) == NULL) {
		// throw error
		return -1;
	}
	memset(readBuffer, 0, BUFSIZE);

	while (get_tok(fp, ps1) == 0) {
		fprintf(stderr, "%s", ps1->start);
	}


#ifdef _DEBUG
	printNodes();
#endif // _DEBUG

	//freeParser(ps1);
	//freeNlList();
	return 0;
}

int swapBuffer(parser* ps1) {
	return 0;
}
