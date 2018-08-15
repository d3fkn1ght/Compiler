#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv[]) {

	char key[1];
	char* srcFile = "Grammar";
	char* destFile = "Grammar.h";
	FILE *srcFP = NULL, *destFP = NULL;
	errno_t err;

	if ((err = fopen_s(&srcFP, srcFile, "r")) != 0) {
		//fprintf error
		goto end;
	}

	if ((err = fopen_s(&destFP, destFile, "w")) != 0) {
		//fprintf error
		goto end;
	}
	
	// read in all lines from Grammar.h
	// extern const char tokenNames[]; in token.h
	// write out to const char* tokenNames[] token.c 
	// write out to enum tokenType in token.h
	// write out defines for token.h
	//	modify singleCharTokens string in stringContainsSingleCharToken function of tokenizer.c

end:
	if (srcFP != NULL) {
		fclose(srcFP);
	}

	if (destFP != NULL) {
		fclose(destFP);
	}

	fprintf_s(stdout, "Press any key to continue");
	gets_s(key, 1);

	return 0;
}