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