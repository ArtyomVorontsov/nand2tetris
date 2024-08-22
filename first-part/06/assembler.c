#include "assembler.h"

/* 
 * TODO: 
 * improve memory handling
 * move read file logic to separate function 
 * write code generator
 * extend assembler with A and L type of commands as well as symbol table
 * generate file with the same name as input file, but with different .hack extension
 */


int main(int argc, char **argv){
	char ** generatedCode;
	char *sourceCodeFilename = *(argv + 1);
	char *program;
	struct CInstruction** parsed;
		
	program = getProgramText(sourceCodeFilename);
	
	parsed = parser(program);

	// Print parsed structures
	for(int i = 0; *(parsed + i) != NULL; i++){
		printf("dest: %s\ncomp: %s\njump: %s\n\n", 
			(*(parsed + i))->dest, 
			(*(parsed + i))->comp, 
			(*(parsed + i))->jump
		);
	}

	generatedCode = code(parsed);
	char *fileNameWithoutExtension = 
		getFileNameWithoutExtension(sourceCodeFilename);

	writeTextToFile(fileNameWithoutExtension, generatedCode);

	return 0;
}

char *getFileNameWithoutExtension(char *filename){
	int i = 0;
	char *filenameWithoutExtension = malloc(sizeof(char) * 1000);
	while(*(filename + i) != '.' && *(filename + i) != '\0'){
		*(filenameWithoutExtension + i) = *(filename + i);

		i++;
	}

	printf("%s\n", filename);

	return filenameWithoutExtension;
}

char * getProgramText(char *sourceCodeFilename){
	FILE *fp;
	char *program = malloc(sizeof(char) * 1000);
	char line[100];

	// Get text from file
	fp = fopen(sourceCodeFilename, "r");
	if(fp == NULL){
		printf("Cannot open file\n");
	}
	
       	int j = 0;	
	while (fgets(line, 100, fp) != NULL) {
		int i = 0;
		do {
			*(program + j) = *(line + i);
			j++; i++;
		} while(*(line + i - 1) != '\n');
	}

	return program;
}

void writeTextToFile(char *fileName, char **generatedCode){
	int i = 0, j = 0;
	bool endMet = false;
	char *ext = ".hack";
	while(true){
		if(*(fileName + i) == '\0'){
			endMet = true;
		}

		if(endMet){
			while(*(ext + j) != '\0'){
				*(fileName + i++) = *(ext + j++);
			}
			*(fileName + i) = '\0';
			break;
		}

		i++;

	}
	// Open a file in writing mode
	FILE* fptr = fopen(fileName, "a");

	// Write some text to the file
	i = 0;
	char *gc;
	while((gc = *(generatedCode + i)) != NULL){
		fprintf(fptr, "%s", gc);
		i++;
	}
}

