#include "./vm-translator.h"

char * SourceFileName;
/*TODO:
 * Add other arithmetic operations apart of add
 */

int main(int argc, char **argv){

	SourceFileName = *(argv + 1);
	char *fileNameWithoutExtension = getFileNameWithoutExtension(SourceFileName);
	char *destFileName = strcat(fileNameWithoutExtension, ".asm");
	const int MAX_LINE_SIZE = 1000;
	FILE *sfp, *dfp;
	char line[MAX_LINE_SIZE];
	struct VmInst *vmInst;
	char *asmInst;

	sfp = fopen(SourceFileName, "r");
	dfp= fopen(destFileName, "w");

	int i = 0;



	// static linkage to memory addresses, will be improved in chapter 8
	fprintf(dfp, "%s", "@256\n");
	fprintf(dfp, "%s", "D=A\n");
	fprintf(dfp, "%s", "@SP\n");
	fprintf(dfp, "%s", "M=D\n");

	fprintf(dfp, "%s", "@2000\n");
	fprintf(dfp, "%s", "D=A\n");
	fprintf(dfp, "%s", "@LCL\n");
	fprintf(dfp, "%s", "M=D\n");
	fprintf(dfp, "%s", "\n");

	fprintf(dfp, "%s", "@3000\n");
	fprintf(dfp, "%s", "D=A\n");
	fprintf(dfp, "%s", "@TEMP\n");
	fprintf(dfp, "%s", "M=D\n");
	fprintf(dfp, "%s", "\n");

	while(1){
		fgets(line, MAX_LINE_SIZE, sfp);
		if(feof(sfp) == true) break;

		vmInst = parser(line, i);
		/*
		printf("type: %s\ncmnd: %s\narg1: %s\narg2: %s\nline: %d\n\n",
				vmInst->type,
				vmInst->cmnd,
				vmInst->arg1,
				vmInst->arg2,
				vmInst->line); */


		asmInst = codeWriter(vmInst);
		// printf("asmInst: %s\n\n", asmInst);

		fprintf(dfp, "%s", asmInst);
		i++;
	}


	fprintf(dfp, "(EOF_LOOP)\n");
	fprintf(dfp, "@EOF_LOOP\n");
	fprintf(dfp, "0;JMP\n");

	fclose(sfp);
	fclose(dfp);
}

char *getFileNameWithoutExtension(char *fileName){
	int i = 0, j = 0;
	int fileNameLength = strlen(fileName);
	char *fileNameWithoutExtension = malloc(sizeof(char) * fileNameLength);
	char *fnp = fileName + fileNameLength;
	bool extensionPassed = false;

	while(*(fnp - i) != '/' && i <= fileNameLength){
		if(extensionPassed){
			*(fileNameWithoutExtension + j) = *(fnp - i);
			j++;
		}

		if(*(fnp - i) == '.'){
			extensionPassed = true;
		}

		i++;
	}

	fileNameWithoutExtension[i] = '\0';

	int fileNameWithoutExtensionLength = strlen(fileNameWithoutExtension);
	char *fileNameWithoutExtensionReversed = malloc(sizeof(char) * fileNameWithoutExtensionLength);
	
	for(int i = 0; i < fileNameWithoutExtensionLength; i++)
	        fileNameWithoutExtensionReversed[i] = fileNameWithoutExtension[fileNameWithoutExtensionLength - i - 1];

	fileNameWithoutExtensionReversed[fileNameWithoutExtensionLength] = '\0';

	free(fileNameWithoutExtension);
	return fileNameWithoutExtensionReversed;
}

