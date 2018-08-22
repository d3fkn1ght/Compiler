#include <vld.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Allocator.h"
#include "Exception.h"
#include "Node.h"
#include "Parser.h"
#include "Tokenizer.h"

void error(exception* e1) {
	switch (e1->errorType) {
	case E_FILEACCESS:
		if (e1->errorType == E_FILEACCESS) {
			fprintf_s(stderr, "ERROR:   %s\nDETAILS: %s\n", e1->errorStr, e1->errorInfo);
		}
		break;
	default:
		fprintf_s(stderr, "Error: %s\n", e1->errorStr);
	}
}

int init_compiler(exception** e1, char* destFile, char* srcFile, FILE** destFP, FILE** srcFP, nodeList** ll_nodelist, parser** ps1) {
	
	if ((*e1 = new_exception()) == NULL) {
		fprintf(stderr, "Error allocating memory for exception handler");
		return -1;
	}

	if ((fopen_s(srcFP, srcFile, "r")) != 0) {
		setException(*e1, "Error opening source file", E_FILEACCESS, srcFile);
		return -1;
	}

	/*	if ((err = fopen_s(&destFP, destFile, "w")) != 0) {
	setException(e1,"Error opening destination file", FILEACCESS, destFile);
	goto end;
	}
	*/

	if ((*ll_nodelist = newNodeList()) == NULL) {
		setException(*e1, "Error allocating memory", ENOMEM, "NodeList");
		return -1;
	}


	if ((*ps1 = newParser()) == NULL) {
		// throw error
		return -1;
	}


	// create allocator

	return 0;
}

int parseArgs() {
	// for loop to parse and set args
	// if error return BADARGUMENT;
	// else return NONE
	return 0;
}

int main(int argc, char** argv) {
	// change to use name so that main.c becomes main.o
	char* destFile = "Object";
	char* srcFile = "Source";
	exception* e1 = NULL;
	FILE *srcFP = NULL, *destFP = NULL;
	nodeList* ll_nodelist = NULL;
	parser* ps1 = NULL;

	if (init_compiler(&e1, destFile, srcFile, &destFP, &srcFP, &ll_nodelist, &ps1) != 0) {
		// throw error
		return -1;
	}

	if (argc > 1) {
		if (parseArgs != 0) {
			setException(e1, "Error parsing arguments", E_ARGPARSE, NULL);
		}
	}

	if (lex(e1,srcFP,ll_nodelist, ps1) != 0) {
		error(e1);
		goto end;
	}

end:
	if (e1->errorType != E_NONE) {
		error(e1);
	}

	if (e1 != NULL) {
		freeException(e1);
	}

	if (srcFP != NULL) {
		fclose(srcFP);
	}
	
	if (destFP != NULL) {
		fclose(destFP);
	}
	
	freeNlList(ll_nodelist);
	return 0;

}