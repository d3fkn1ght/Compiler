#include <stdlib.h>
#include <string.h>

#include "Node.h"
#include "Parser.h"

parser* newParser() {
	parser* ps1 = (parser*)malloc(sizeof(parser));
	if (ps1 != NULL) {
		ps1->psState = PS_SCAN;
		ps1->lineno = 0;
		ps1->colno = 0;

		if ((ps1->buffer1 = (char*)malloc(sizeof(char) * BUFSIZE)) == NULL) {
			free(ps1);
			return NULL;
		}

		if ((ps1->buffer2 = (char*)malloc(sizeof(char) * BUFSIZE)) == NULL) {
			free(ps1->buffer1);
			free(ps1);
			return NULL;
		}

		memset(ps1->buffer1, 0, BUFSIZE);
		memset(ps1->buffer2, 0, BUFSIZE);

		ps1->start = ps1->end = ps1->buffer1;
	}
	return ps1;
}

void freeParser(parser* ps1)
{
	if (!ps1) {
		return;
	}

	if (ps1->buffer1 != NULL) {
		free(ps1->start);
	}

	if (ps1->buffer2 != NULL) {
		free(ps1->buffer2);
	}

	free(ps1);
	ps1 = NULL;
}