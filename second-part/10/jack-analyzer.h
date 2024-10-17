#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>
#include "jack-tokinizer.h"


char *getFileName(char *path);
char *getFileNameWithoutExtension(char *fileName);
char *getDestFileName(char *filePath, char *postfix);
void compileDirectoryFiles(DIR *dirP, char *filePath);
