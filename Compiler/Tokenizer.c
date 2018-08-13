// use int to keep track of buffer available
#include <stdlib.h>
#include <string.h>

#include "Node.h"
#include "Parser.h"
#include "Tokenizer.h"


#define possibleNumberStart(c) (\
				((c >= '0' && c <= '9') || (c == '-') || (c == '.')))

#define possibleNumberDigit(c) (\
				((c >= '0' && c <= '9') || (c == '.')))

#define possibleIDStart(c) (\
              (c >= 'a' && c <= 'z')\
               || (c >= 'A' && c <= 'Z')\
               || c == '_'\
               || (c >= 128))

#define possibleIDChar(c) (\
              (c >= 'a' && c <= 'z')\
               || (c >= 'A' && c <= 'Z')\
               || c == '_'\
               || (c >= 128)\
			   || ((c >= '0') && (c <= '9')))


//start here
void get_nextTok(FILE** fp, parser* ps1) {
	int scanResult = 0;

	if (feof(*fp)) {
		ps1->psState = PS_EOF;
		return;
	}

	scanResult = fscanf_s(*fp, "%s", ps1->buf, BUFSIZE);
	ps1->end += strlen(ps1->buf);

	if (scanResult == 0) {
		// throw error
		ps1->psState = PS_ERR;
		return;
	}
	else if (scanResult == EOF) {
		// throw error
		ps1->psState = PS_EOF;
		return;
	}
}

tokenType matchOne(parser* ps1) {
	switch (ps1->buf[0]) {
	case '{':
		return LBRACE;
	case '}':
		return RBRACE;
	case '(':
		return LPAREN;
	case ')':
		return RPAREN;
	case ';':
		return SEMICOLON;
	default:
		return NONE;
	}
}

tokenType matchTwo(FILE** fp, parser* ps1) {
	// try to match two chars
	return NONE;
}

tokenType matchToken(FILE** fp, parser* ps1, int* tokenLength) {
	// while loop to iterate over rest of buffer
	// check for eof if so handle eof
	// else check for terminal match
	int index = 0;

	get_nextTok(fp, ps1);
	if (ps1->psState == PS_ERR) {
		// throw error
	}

	if (ps1->psState == PS_EOF) {
		// throw error
		return ERROR;
	}
	
	// match int
	if ((strncmp(ps1->buf, "int", strlen(ps1->buf))) == 0) {
		*tokenLength = 3;
		return INT;
	}

	if ((strncmp(ps1->buf, "return", strlen(ps1->buf))) == 0) {
		*tokenLength = 6;
		return RETURN;
	}

	//if (matchOne) return
	//if matchTwo return


	index = 0;
	if (possibleNumberStart(ps1->buf[index])) {
		index++;
		*tokenLength += 1;
		while (possibleNumberDigit(ps1->buf[index])) {
			index++;
			*tokenLength += 1;
		}
		return NUMBER;
	}

	index = 0;
	if (possibleIDStart(ps1->buf[index])) {
		index++;
		*tokenLength += 1;
		while (possibleIDChar(ps1->buf[index])) {
			index++;
			*tokenLength += 1;
		}
		return ID;
	}

	return NONE;
}

node* get_tok(FILE** fp, parser* ps1) {
	node* node1 = NULL;
	int tokenLength = 0;

	if ((node1 = newNode()) == NULL) {
		// throw error
	}

	for (;;) {
		// match token
		// create node
		// return
		node1->token->tType = matchToken(fp, ps1, &tokenLength);
		if (ps1->psState == PS_ERR) {
			// throw ERROR
			goto end;
		}

		if (ps1->psState == PS_EOF)
		{
			return NULL;
			goto end;
		}

		if ((setNodeName(node1, ps1->buf, tokenLength)) == -1) {
			// throw error
		}
		return node1;
	}

end:
	// free node1
	// free line
	return node1;
}