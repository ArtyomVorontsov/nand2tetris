#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


struct CInstruction {
	char *dest;
	char *comp;
	char *jump;
	char *address;
	char *symbol;
	char *label;
};


char **code(struct CInstruction **instructions);
struct CInstruction** parser(char *sp);
char * getProgramText(char *sourceCodeFilename);
void writeTextToFile(char *fileName, char **generatedCode);
char *getFileNameWithoutExtension(char *filename);

