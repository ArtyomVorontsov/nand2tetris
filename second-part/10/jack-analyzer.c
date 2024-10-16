#include "jack-analyzer.h"

char * SourceFileName;
char * SourceFileNameWithoutExt;

int main(int argc, char **argv){
	char *filePath = *(argv + 1);
	char *destFileName;
	const int MAX_LINE_SIZE = 1000;
	char line[MAX_LINE_SIZE];
	FILE *sfp, *dfp;
	int i = 0;
	DIR *dirP = opendir(filePath);
	struct dirent *dir;
	bool argumentIsDirectory = dirP != NULL;

	if(argumentIsDirectory) {
		// If directory as argument is provided
		compileDirectoryFiles(dirP, filePath);
		fclose(dfp);
	}
	else {
		destFileName = getDestFileName(filePath, "xml");
		// If file as argument is provided
		SourceFileName = filePath;
		SourceFileNameWithoutExt = getFileNameWithoutExtension(filePath);

		sfp = fopen(SourceFileName, "r");
		dfp = fopen(destFileName, "w");
		tokenize(sfp, dfp);
		fclose(sfp);
		fclose(dfp);
	}

	return 0;
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

void compileDirectoryFiles(DIR *dirP, char *filePath){
	char *sourceFileName, *destFileName;
	FILE *sfp, *dfp;
	struct dirent *dir;

	while((dir = readdir(dirP)) != NULL){
		sourceFileName = malloc(sizeof(char) * 200);

		if(dir->d_type != DT_DIR){

			strcpy(sourceFileName, filePath);
			strcat(sourceFileName, "/");
			strcat(sourceFileName, dir->d_name);
			SourceFileName = sourceFileName;
			SourceFileNameWithoutExt = getFileNameWithoutExtension(sourceFileName);

			destFileName = getDestFileName(filePath, "xml");

			sfp = fopen(sourceFileName, "r");
			dfp = fopen(destFileName, "a");
			
			tokenize(sfp, dfp);
			fclose(sfp);
		}
	}
}

char *getDestFileName(char *filePath, char *ext){
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
		strcat(destFileName, ".");
		strcat(destFileName, ext);
	} else {
		SourceFileName = getFileName(filePath);
		SourceFileNameWithoutExt = getFileNameWithoutExtension(SourceFileName);

		/* Assign value to variable which holds destination filename */
		destFileName  = malloc(sizeof(char) * (strlen(SourceFileNameWithoutExt) + 5));
		strcpy(destFileName, SourceFileNameWithoutExt);
		strcat(destFileName, ".");
		strcat(destFileName, ext);
	}

	return destFileName;
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

