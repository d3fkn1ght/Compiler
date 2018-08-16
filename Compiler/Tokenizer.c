#include <stdlib.h>
#include <string.h>

#include "Tokenizer.h"

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

int lineSize = 256;

const char* tokenNames[] = {
	"NONE",
	"int",
	"return"
};

size_t containsTokenChar(char* buffer) {
	char* tokenChars = "{}();+";
	char* test = strpbrk(buffer, tokenChars);

	if (test != NULL) {
		return (size_t)(test - buffer);
	}

	return 0;
}


token* get_tok(FILE* fp, parser* ps1) {
	char* substr = NULL;
	token* token = NULL;
	size_t tokenLength = 0;
	tokenType tType = NONE;

	if ((substr = (char*)malloc(sizeof(char) * maxTokenNameSz)) == NULL) {
		// throw error
		goto end;
	}

	if (ps1->psState == PS_SCAN) {
		if ((fgets(ps1->buffer1, BUFSIZE, fp)) == NULL) {
			ps1->psState = PS_EOF;
			goto end;
		}
		ps1->end += strlen(ps1->buffer1);
		ps1->psState = PS_MORE;
	}

	if ((tType = matchTwo(ps1)) != NONE) {
		if ((token = newToken()) == NULL) {
			// throw error
			goto end;
		}
		if ((setTokenName(token, ps1->buffer1, 2)) == -1) {
			// throw error
			goto end;
		}

		token->tType = tType;
		ps1->buffer1 += 2;
		return token;
	}


	if ((tType = matchOne(ps1)) != NONE) {
		if ((token = newToken()) == NULL) {
			// throw error
			goto end;
		}

		if ((setTokenName(token, ps1->buffer1, 1)) == -1) {
			// throw error
			goto end;
		}

		token->tType = tType;
		ps1->buffer1 += 1;
		return token;
	}

	if ((tokenLength = containsTokenChar(ps1->buffer1)) > 0) {
		if (strncpy_s(substr, maxTokenNameSz, ps1->buffer1, tokenLength) == -1) {
			// throw error
			goto end;
		}

		if ((tType = matchToken(ps1, &tokenLength)) != NONE) {
			if ((token = newToken()) == NULL) {
				// throw error
				goto end;
			}
			
			if ((setTokenName(token, ps1->buffer1, tokenLength)) == -1) {
				// throw error
				goto end;
			}

			token->tType = tType;
			ps1->buffer1 += tokenLength;
			return token;
		}

		// throw error
		goto end;
	}

	tokenLength = strlen(ps1->buffer1);
	if ((tType = matchToken(ps1,&tokenLength)) != NONE) {
		if ((token = newToken()) == NULL) {
			// throw error
			goto end;
		}

		token->tType = tType;
		ps1->buffer1 += tokenLength;
		return token;
	}
	
	// check for ID
	int index = 0;

	if (possibleNumberStart(ps1->buffer1[index])) {
		index++;
		while (possibleNumberDigit(ps1->buffer1[index])) {
			index++;
		}

		if ((token = newToken()) == NULL) {
			// throw error
			goto end;
		}

		if ((setTokenName(token, ps1->buffer1, index)) == -1) {
			// throw error
			goto end;
		}

		token->tType = NUMBER;
		ps1->buffer1 += index;
		return token;
	}

	if (possibleIDStart(ps1->buffer1[index])) {
		index++;
		while (possibleIDChar(ps1->buffer1[index])) {
			index++;
		}

		if ((token = newToken()) == NULL) {
			// throw error
			goto end;
		}

		if ((setTokenName(token, ps1->buffer1, index)) == -1) {
		// throw error
			goto end;
		}

		token->tType = ID;
		ps1->buffer1 += index;
		return token;
	}

	// else get more input or handle string, multi-line comment, command, etc
end:
	if (substr != NULL) {
		free(substr);
	}

	return NONE;
}

int lex(FILE* fp, parser* ps1) {
	size_t tokenLength = 0;
	token* t1 = NULL;
	tokenType tType = NONE;

	if ((t1 = newToken()) == NULL) {
		// throw error
		return -1;
	}

	while ((t1 = get_tok(fp, ps1)) != NONE) {
		if (ps1->buffer1 == ps1->end) {
			ps1->psState = PS_SCAN;
		}
		appendNode(newNode(t1));
	}

	free(t1);

#ifdef _DEBUG
	//printNodes();
#endif // _DEBUG

	if (ps1->psState == PS_ERR) {
		// throw error
		return -1;
	}

	return 0;
}

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

	for (int i = 1; i < (sizeof(tokenNames)/sizeof(const char*)); i++) {
		if ((strncmp(ps1->buffer1, tokenNames[i],*tokenLength)) == 0) {
			return (tokenType)i;
		}
	}
	return NONE;
}