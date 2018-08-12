#include <stdlib.h>
#include "Parser.h"

parser* newParser() {
	parser* ps1 = (parser*)malloc(sizeof(parser));
	if (ps1 != NULL) {
		ps1->psState = PS_START;
		ps1->lineno = 0;
		ps1->colno = 0;
		ps1->buf = (char*)malloc(sizeof(char) * BUFSIZE);
		if (ps1->buf == NULL) {
			free(ps1);
			return NULL;
		}
		ps1->start = ps1->end = ps1->buf;
	}
	return ps1;
}

freeParser(parser* ps1)
{
	if (ps1->buf != NULL) {
		free(ps1->buf);
	}

	if (ps1 != NULL) {
		free(ps1);
	}
}