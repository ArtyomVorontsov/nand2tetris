#include <dirent.h>

char *getFileName(char *path);
char *getFileNameWithoutExtension(char *fileName);
char *getDestFileName(char *filePath, char *postfix);
void compileDirectoryFiles(DIR *dirP, char *filePath);
