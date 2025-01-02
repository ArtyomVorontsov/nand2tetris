#include "jack-compiler.h"
#include "jack-tokinizer.h"
#include "compilation-engine.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

char *SourceFileName;
char *SourceFileNameWithoutExt;

int main(int argc, char **argv)
{
	char *filePath = *(argv + 1);
	char *destFileName;
	const int MAX_LINE_SIZE = 1000;
	char line[MAX_LINE_SIZE];
	FILE *sfp, *dfp;
	int i = 0;
	DIR *dirP = opendir(filePath);
	struct dirent *dir;
	bool argumentIsDirectory = dirP != NULL;

	if (argumentIsDirectory)
	{
		// If directory as argument is provided
		compileDirectoryFiles(dirP, filePath);
	}
	else
	{
		// If file as argument is provided

		// tokenize
		SourceFileName = filePath;
		SourceFileNameWithoutExt = getFileNameWithoutExtension(filePath);
		sfp = fopen(SourceFileName, "r");
		if (sfp == NULL)
		{
			printf("No such file or directory.\n");
			exit(1);
		}
		destFileName = getDestFileName(filePath, "T.xml");
		dfp = fopen(destFileName, "w");
		tokenize(sfp, dfp);
		fclose(sfp);
		fclose(dfp);

		// compile
		SourceFileName = destFileName;
		SourceFileNameWithoutExt = getFileNameWithoutExtension(destFileName);
		destFileName = getDestFileName(filePath, ".xml");
		sfp = fopen(SourceFileName, "r");
		dfp = fopen(destFileName, "w");
		compilationEngine(sfp, dfp);
		fclose(sfp);
		fclose(dfp);
	}

	return 0;
}

char *getFileNameWithoutExtension(char *fileName)
{
	int i = 0, j = 0;
	int fileNameLength = strlen(fileName);
	char *fileNameWithoutExtension = malloc(sizeof(char) * fileNameLength);
	char *fnp = fileName + fileNameLength;
	bool extensionPassed = false;

	while (i <= fileNameLength)
	{
		if (*fnp == '/')
		{
			break;
		}

		if (extensionPassed)
		{
			*(fileNameWithoutExtension + j) = *fnp;
			j++;
		}

		if (*fnp == '.')
		{
			extensionPassed = true;
		}

		i++;
		fnp--;
	}

	fileNameWithoutExtension[j] = '\0';

	int fileNameWithoutExtensionLength = strlen(fileNameWithoutExtension);
	char *fileNameWithoutExtensionReversed = malloc(sizeof(char) * fileNameWithoutExtensionLength);

	for (int i = 0; i < fileNameWithoutExtensionLength; i++)
		fileNameWithoutExtensionReversed[i] = fileNameWithoutExtension[fileNameWithoutExtensionLength - i - 1];

	fileNameWithoutExtensionReversed[fileNameWithoutExtensionLength] = '\0';

	free(fileNameWithoutExtension);
	return fileNameWithoutExtensionReversed;
}

void compileDirectoryFiles(DIR *dirP, char *filePath)
{
	char *sourceFileName, *destFileName;
	FILE *sfp, *dfp;
	struct dirent *dir;

	dir = readdir(dirP);

	do
	{
		sourceFileName = malloc(sizeof(char) * 200);
		destFileName = malloc(sizeof(char) * 200);

		if (dir->d_type != DT_DIR)
		{

			// tokenize
			strcpy(sourceFileName, filePath);
			strcat(sourceFileName, "/");
			strcat(sourceFileName, dir->d_name);
			SourceFileName = sourceFileName;
			SourceFileNameWithoutExt = getFileNameWithoutExtension(sourceFileName);

			strcpy(destFileName, filePath);
			strcat(destFileName, "/");
			strcat(destFileName, dir->d_name);

			destFileName = getDestFileName(destFileName, "T.xml");

			sfp = fopen(sourceFileName, "r");
			dfp = fopen(destFileName, "a");

			tokenize(sfp, dfp);

			fclose(sfp);
			fclose(dfp);

			// compile
			strcpy(SourceFileName, destFileName);
			strcpy(destFileName, filePath);
			strcat(destFileName, "/");
			strcat(destFileName, dir->d_name);
			destFileName = getDestFileName(destFileName, ".xml");

			sfp = fopen(SourceFileName, "r");
			dfp = fopen(destFileName, "w");

			compilationEngine(sfp, dfp);

			fclose(sfp);
			fclose(dfp);
		}
	} while ((dir = readdir(dirP)) != NULL);
}

char *getDestFileName(char *filePath, char *postfix)
{
	DIR *dirP = opendir(filePath);
	struct dirent *dir;
	bool argumentIsDirectory = dirP != NULL;
	char *destFileName;
	char *sourceFileName;
	char *sourceFileNameWithoutExt;

	if (argumentIsDirectory)
	{
		int pathLength = strlen(filePath);

		if (filePath[pathLength - 1] == '/')
		{
			filePath[pathLength - 1] = '\0';
		}

		destFileName = getFileName(filePath);
		strcat(destFileName, postfix);
	}
	else
	{
		sourceFileName = getFileName(filePath);
		sourceFileNameWithoutExt = getFileNameWithoutExtension(sourceFileName);

		/* Assign value to variable which holds destination filename */
		destFileName = malloc(sizeof(char) * (strlen(sourceFileNameWithoutExt) + 5));
		strcpy(destFileName, sourceFileNameWithoutExt);
		strcat(destFileName, postfix);
	}

	return destFileName;
}

char *getFileName(char *path)
{
	int i = 0;
	int pathLength = strlen(path);
	char *fileName = malloc(sizeof(char) * pathLength);
	char *fnp = path + pathLength - 1;

	while (i <= pathLength)
	{
		if (*fnp == '/')
		{
			break;
		}
		*(fileName + i) = *fnp;
		i++;
		fnp--;
	}

	fileName[i] = '\0';

	int fileNameLength = strlen(fileName);
	char *fileNameReversed = malloc(sizeof(char) * fileNameLength);

	for (int i = 0; i < fileNameLength; i++)
		fileNameReversed[i] = fileName[fileNameLength - i - 1];

	fileNameReversed[fileNameLength] = '\0';

	free(fileName);
	return fileNameReversed;
}
