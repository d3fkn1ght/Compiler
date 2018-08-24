#include <stdlib.h>
#include <string.h>

#include "Grammar.h"
#include "Lexer.h"


#define isAlpha(c) (\
				(((c >= 'a') && (c <= 'z'))  || ((c >= 'A') && (c <= 'Z'))))

#define isWhiteSpace(c) (\
				(c == ' ') )

#define possibleNumberStart(c) (\
				((c >= '0' && c <= '9') || (c == '-') || (c == '.')))

#define possibleNumberDigit(c) (\
				((c >= '0' && c <= '9') || (c == '.')))

#define possibleIDStart(c) (\
              (c >= 'a' && c <= 'z')\
               || (c >= 'A' && c <= 'Z')\
               || (c == '_')\
               || (c >= 128))

#define possibleIDChar(c) (\
              (c >= 'a' && c <= 'z')\
               || (c >= 'A' && c <= 'Z')\
               || (c == '_')\
               || (c >= 128)\
			   || ((c >= '0') && (c <= '9')))

size_t containsTokenChar(char* buffer) {
	char* tokenChars = " {}();+";
	char* test = strpbrk(buffer, tokenChars);

	if (test != NULL) {
		return (size_t)(test - buffer);
	}

	return 0;
}

void eatWhiteSpace(FILE* fp, parser* ps1) {
	int index = 0;
	size_t len = strlen(ps1->buffer1);

	for (index = 0; index < len; index++) {
		if (ps1->buffer1[0] != ' ') {
			break;
		}
		ps1->buffer1++;
	}

	if (index >= len) {
		ps1->psState = PS_EOF;
	}
}

void getLexeme(char* buffer, size_t* tokenLength) {
	int index = 0;
	while (!(isWhiteSpace(buffer[index])) && (isAlpha(buffer[index]))) {
		index++;
		*tokenLength += 1;
	}
}

int lex(exception* e1, FILE* fp, nodeList* ll_nodelist, parser* ps1) {
	token* token = NULL;
	size_t tokenLength = 0;
	tokenType tType = NONE;

	if ((token = newToken()) == NULL) {
		// setexception
		return -1;
	}

	for (;;) {
		tokenLength = 0;
		resetToken(token);

		if (feof(fp) && (ps1->psState == PS_SCAN)) {
			goto end;
		}

		if (ps1->buffer1 == ps1->end) {
			ps1->psState = PS_SCAN;
		} 

		if (ps1->psState == PS_SCAN) {
			ps1->buffer1 = ps1->start;
			if (feof(fp)) {
				ps1->psState = PS_EOF;
				goto end;
			}
			if ((fgets(ps1->buffer1, BUFSIZE, fp)) == NULL) {
				ps1->psState = PS_EOF;
				goto end;
			}
			ps1->end += strlen(ps1->buffer1);
			ps1->psState = PS_LEX;
		}

		eatWhiteSpace(fp, ps1);

		if ((tType = matchTwo(ps1)) != NONE) {
			if ((setTokenName(token, ps1->buffer1, 2)) == -1) {
				// throw error
				goto end;
			}

			token->tType = tType;
			ps1->buffer1 += 2;
			appendNode(ll_nodelist, newNode(token));
			continue;
		}

		if ((tType = matchOne(ps1)) != NONE) {
			if ((setTokenName(token, ps1->buffer1, 1)) == -1) {
				// throw error
				goto end;
			}

			token->tType = tType;
			ps1->buffer1 += 1;
			appendNode(ll_nodelist, newNode(token));
			continue;
		}

		getLexeme(ps1->buffer1, &tokenLength);

		if (tokenLength == 0) {
			if (feof(fp)) {
				ps1->psState = PS_EOF;
			}
			else {
				ps1->psState = PS_ERR;
			}
			goto end;
		}

		if ((tType = matchToken(ps1, &tokenLength)) != NONE) {

			token->tType = tType;
			ps1->buffer1 += tokenLength;
			appendNode(ll_nodelist, newNode(token));
			continue;
		}

		// check for ID
		int index = 0;

		if (possibleNumberStart(ps1->buffer1[index])) {
			index++;
			while (possibleNumberDigit(ps1->buffer1[index])) {
				index++;
			}

			if ((setTokenName(token, ps1->buffer1, index)) == -1) {
				// throw error
				goto end;
			}

			token->tType = NUMBER;
			ps1->buffer1 += index;
			appendNode(ll_nodelist, newNode(token));
			continue;
		}

		if (possibleIDStart(ps1->buffer1[index])) {
			index++;
			while (possibleIDChar(ps1->buffer1[index])) {
				index++;
			}

			if ((setTokenName(token, ps1->buffer1, index)) == -1) {
				// throw error
				goto end;
			}

			token->tType = ID;
			ps1->buffer1 += index;
			appendNode(ll_nodelist, newNode(token));
			continue;
		}

		// else get more input or handle string, multi-line comment, command, etc
		if (ps1->psState == PS_LEX) {
			ps1->psState = PS_SCAN;
		}
	}

end:
	freeToken(token);
	if (ps1->psState == PS_ERR) {
		return -1;
	}

#ifdef _DEBUG
	printNodes(ll_nodelist);
#endif // _DEBUG
	
	return 0;
}


#ifdef _DEBUG
const char* getTokenName(token* t1) {
	return tokenNames[t1->tType];
}
#endif

tokenType matchOne(parser* ps1) {
	switch (ps1->buffer1[0]) {
	case '+':
		return PLUS;
	case '\n':
		return NEWLINE;
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

tokenType matchTwo(parser* ps1) {
	switch (ps1->buffer1[0]) {
		case '+':
			switch (ps1->buffer1[1]) {
			case '+':
				return PLUSPLUS;
			default:
				return NONE;
			}
		case '-':
			switch (ps1->buffer1[1]) {
			case '-':
				return MINUSMINUS;
			default:
				return NONE;
			}
		default:
			return NONE;
	}
	return NONE;
}

tokenType matchToken(parser* ps1, size_t* tokenLength) {
	if (ps1->buffer1[*tokenLength - 1] == '\n') {
		*tokenLength -= 1;
	}

	for (int i = 1; i < keywordCount; i++) {
		if ((strncmp(ps1->buffer1, keywords[i],*tokenLength)) == 0) {
			return (tokenType)i;
		}
	}
	return NONE;
}