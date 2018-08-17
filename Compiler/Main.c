#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Exception.h"
#include "Node.h"
#include "Parser.h"
#include "Tokenizer.h"

int error(exception* e1) {
	if (e1->errorType == E_FILEACCESS) {
		fprintf_s(stderr, "ERROR:   %s\nDETAILS: %s\n", e1->errorStr,
			e1->errorInfo);
		return (int)e1->errorType;
	}
	else {
		fprintf_s(stderr, "Error: %s\n",
			e1->errorStr);
		(int)e1->errorType;
	}

	return -1;
}

int parseArgs() {
	// for loop to parse and set args
	// if error return BADARGUMENT;
	// else return NONE
	return 0;
}

int main(int argc, char** argv) {
	char key[1];
	char* srcFile = "Source";
	char* destFile = "Object";
	exception* e1 = NULL;
	FILE *srcFP = NULL, *destFP = NULL;
	errno_t err;
	parser* ps1 = NULL;

	if (argc > 1) {
		if (parseArgs != 0) {
			setException(e1, "Error parsing arguments", E_ARGPARSE, NULL);
		}
	}

	e1 = new_exception();

	if (e1 == NULL)
	{
		fprintf(stderr,"Error allocating memory for exception handler");
		return 1;
	}

	if ((err = fopen_s(&srcFP, srcFile, "r")) != 0) {
		setException(e1, "Error opening source file", E_FILEACCESS, srcFile);
		goto end;
	}

/*	if ((err = fopen_s(&destFP, destFile, "w")) != 0) {
		setException(e1,"Error opening destination file", FILEACCESS, destFile);
		goto end;
	}
*/

	if ((ps1 = newParser()) == NULL) {
		// throw error
		goto end;
	}

	if (lex(srcFP,ps1) != 0) {
		error(e1);
	}

	end:
	if (e1->errorType != E_NONE) {
		err = error(e1);
	}

	if (srcFP != NULL) {
		fclose(srcFP);
	}
	
	if (destFP != NULL) {
		fclose(destFP);
	}
	
	freeNlList();

#ifdef _DEBUG
	fprintf_s(stdout, "Press any key to continue");
	gets_s(key, 1);

#endif // _DEBUG

	return err;
}