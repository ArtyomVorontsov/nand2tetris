#include "./vm-translator.h"

char * SourceFileName;
char * SourceFileNameWithoutExt;
/*TODO:
 * Add support for multifile compilation 
 */
char *functionNameCtx;

struct AmountOfInvocations **functionInvocationsTable;
int functionInvocationsTableLength = 0;

int main(int argc, char **argv){
	functionNameCtx = malloc(sizeof(char) * 1000);
	functionInvocationsTable = malloc(sizeof(struct AmountOfInvocations *) * 1000);
	char *filePath = *(argv + 1);
	char *destFileName;
	char *tempFileName;
	char *tempFileNameWithoutExtension;
	const int MAX_LINE_SIZE = 1000;
	FILE *sfp, *dfp;
	char line[MAX_LINE_SIZE];
	struct VmInst *vmInst;
	char *asmInst;
	int i = 0;

	/* directory handling */
	DIR *dirP = opendir(filePath);
	struct dirent *dir;
	bool argumentIsDirectory = dirP != NULL;

	destFileName = getDestFileName(filePath);

	dfp = fopen(destFileName, "w");
	writeInitialCode(dfp);
	fclose(dfp);

	if(argumentIsDirectory) {
		while((dir = readdir(dirP)) != NULL){
			tempFileName = malloc(sizeof(char) * 200);

			if(dir->d_type != DT_DIR){
				*tempFileName = '\0';
				strcpy(tempFileName, filePath);
				strcat(tempFileName, "/");
				strcat(tempFileName, dir->d_name);
				SourceFileName = tempFileName;
				SourceFileNameWithoutExt = getFileNameWithoutExtension(tempFileName);

				sfp = fopen(tempFileName, "r");
				dfp= fopen(destFileName, "a");
				
				fprintf(dfp, "\n// NEW FILE %s STARTED\n\n", dir->d_name);
				translateToAsm(sfp, dfp);
				fclose(sfp);
			}
		}

		SourceFileName = tempFileName;
		SourceFileNameWithoutExt = getFileNameWithoutExtension(tempFileName);

		fclose(dfp);
	}
	else {
		SourceFileName = filePath;
		SourceFileNameWithoutExt = getFileNameWithoutExtension(filePath);
		sfp = fopen(SourceFileName, "r");
		dfp = fopen(destFileName, "w");
		translateToAsm(sfp, dfp);
		fclose(sfp);
		fclose(dfp);
	}

	sfp = fopen(SourceFileName, "r");
	dfp = fopen(destFileName, "a");

	fprintf(dfp, "(EOF_LOOP)\n");
	fprintf(dfp, "@EOF_LOOP\n");
	fprintf(dfp, "0;JMP\n");

	fclose(dfp);
}

void translateToAsm(FILE *sfp, FILE *dfp){
	const int MAX_LINE_SIZE = 1000;
	char line[MAX_LINE_SIZE];
	int i = 0;
	struct VmInst *vmInst;
	char *asmInst;

	while(1){
		fgets(line, MAX_LINE_SIZE, sfp);
		if(feof(sfp) == true) break;

		vmInst = parser(line, i);
		
		printf("type: %s\ncmnd: %s\narg1: %s\narg2: %s\nline: %d\n\n",
				vmInst->type,
				vmInst->cmnd,
				vmInst->arg1,
				vmInst->arg2,
				vmInst->line); 


		asmInst = codeWriter(vmInst);

		fprintf(dfp, "%s", asmInst);
		i++;
	}
}

char *getFileName(char *path){
	int i = 0;
	int pathLength = strlen(path);
	char *fileName = malloc(sizeof(char) * pathLength);
	char *fnp = path + pathLength - 1;

	while(i <= pathLength){
		if(*fnp == '/'){
			break;
		}
		*(fileName + i) = *fnp;
		i++;
		fnp--;
	}

	fileName[i] = '\0';

	int fileNameLength = strlen(fileName);
	char *fileNameReversed = malloc(sizeof(char) * fileNameLength);
	
	for(int i = 0; i < fileNameLength; i++)
	        fileNameReversed[i] = fileName[fileNameLength - i - 1];

	fileNameReversed[fileNameLength] = '\0';

	free(fileName);
	return fileNameReversed;
}




char *getFileNameWithoutExtension(char *fileName){
	int i = 0, j = 0;
	int fileNameLength = strlen(fileName);
	char *fileNameWithoutExtension = malloc(sizeof(char) * fileNameLength);
	char *fnp = fileName + fileNameLength;
	bool extensionPassed = false;

	while(i <= fileNameLength){
		if(*fnp == '/'){
			break;
		}

		if(extensionPassed){
			*(fileNameWithoutExtension + j) = *fnp;
			j++;
		}

		if(*fnp  == '.'){
			extensionPassed = true;
		}

		i++;
		fnp--;
	}

	fileNameWithoutExtension[j] = '\0';

	int fileNameWithoutExtensionLength = strlen(fileNameWithoutExtension);
	char *fileNameWithoutExtensionReversed = malloc(sizeof(char) * fileNameWithoutExtensionLength);
	
	for(int i = 0; i < fileNameWithoutExtensionLength; i++)
	        fileNameWithoutExtensionReversed[i] = fileNameWithoutExtension[fileNameWithoutExtensionLength - i - 1];

	fileNameWithoutExtensionReversed[fileNameWithoutExtensionLength] = '\0';

	free(fileNameWithoutExtension);
	return fileNameWithoutExtensionReversed;
}

void addFunctionInvocation(char *fnName){
	struct AmountOfInvocations *functionInvocation = getFunctionInvocation(fnName);


	if(functionInvocation){
		functionInvocation->invocationAmount++;
	} else {
		functionInvocation = (struct AmountOfInvocations *) malloc(sizeof(struct AmountOfInvocations));
		functionInvocation->functionName = fnName;
		functionInvocation->invocationAmount = 1;
		*(functionInvocationsTable + functionInvocationsTableLength) = functionInvocation;
		functionInvocationsTableLength++;
	}
}

struct AmountOfInvocations *getFunctionInvocation(char *fnName){
	struct AmountOfInvocations *functionInvocation = NULL;

	for(int i = 0; i < functionInvocationsTableLength; i++){
		struct AmountOfInvocations *fnI = *(functionInvocationsTable + i);
		if(strcmp(fnI->functionName, fnName) == 0){
			functionInvocation = fnI;
		}
	}

	return functionInvocation;
}

char *getDestFileName(char *filePath){
	DIR *dirP = opendir(filePath);
	struct dirent *dir;
	bool argumentIsDirectory = dirP != NULL;
	char *destFileName;

	if(argumentIsDirectory){
		int pathLength = strlen(filePath);

		if(filePath[pathLength - 1] == '/'){
			filePath[pathLength - 1] = '\0';
		}

		destFileName = getFileName(filePath);
		strcat(destFileName, ".asm");
	} else {
		SourceFileName = getFileName(filePath);
		SourceFileNameWithoutExt = getFileNameWithoutExtension(SourceFileName);

		/* Assign value to variable which holds destination filename */
		strcpy(destFileName, SourceFileNameWithoutExt);
		strcat(destFileName, ".asm");
	}

	return destFileName;
}

void writeInitialCode(FILE *dfp){
	// static linkage to memory addresses, will be improved in chapter 8
	fprintf(dfp, "%s", "@256\n");
	fprintf(dfp, "%s", "D=A\n");
	fprintf(dfp, "%s", "@SP\n");
	fprintf(dfp, "%s", "M=D\n");

	fprintf(dfp, "%s", "@0\n");
	fprintf(dfp, "%s", "D=A\n");
	fprintf(dfp, "%s", "@LCL\n");
	fprintf(dfp, "%s", "M=D\n");
	fprintf(dfp, "%s", "\n");

	fprintf(dfp, "%s", "@0\n");
	fprintf(dfp, "%s", "D=A\n");
	fprintf(dfp, "%s", "@TEMP\n");
	fprintf(dfp, "%s", "M=D\n");
	fprintf(dfp, "%s", "\n");

	fprintf(dfp, "%s", "@0\n");
	fprintf(dfp, "%s", "D=A\n");
	fprintf(dfp, "%s", "@ARG\n");
	fprintf(dfp, "%s", "M=D\n");
	fprintf(dfp, "%s", "\n");

	fprintf(dfp, "%s", "@0\n");
	fprintf(dfp, "%s", "D=A\n");
	fprintf(dfp, "%s", "@THIS\n");
	fprintf(dfp, "%s", "M=D\n");
	fprintf(dfp, "%s", "\n");

	fprintf(dfp, "%s", "@0\n");
	fprintf(dfp, "%s", "D=A\n");
	fprintf(dfp, "%s", "@THAT\n");
	fprintf(dfp, "%s", "M=D\n");
	fprintf(dfp, "%s", "\n");

	fprintf(dfp, "@Sys.init\n");
	fprintf(dfp, "0;JMP\n");
	fprintf(dfp, "%s", "\n");
}

