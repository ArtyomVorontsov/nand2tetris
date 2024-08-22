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
	FILE *fp;
	char *sourceCodeFilename = *(argv + 1);

	// Get text from file
	fp = fopen(sourceCodeFilename, "r");
	if(fp == NULL){
		printf("Cannot open file\n");
	}

	char program[1000];
	char line[100];
	
	
       	int j = 0;	
	while (fgets(line, 100, fp) != NULL) {
		int i = 0;
		do {
			*(program + j) = *(line + i);
			j++; i++;
		} while(*(line + i - 1) != '\n');
	}

	// Run parser
	struct CInstruction** parsed = parser(program);

	// Print parsed structures
	for(int i = 0; *(parsed + i) != NULL; i++){
		printf("dest: %s\ncomp: %s\njump: %s\n\n", 
			(*(parsed + i))->dest, 
			(*(parsed + i))->comp, 
			(*(parsed + i))->jump
		);
	}

	char ** generatedCode = code(parsed);


	// Open a file in writing mode
	FILE* fptr = fopen("a.hack", "a");


	// Write some text to the file
	int i = 0;
	while(*(generatedCode + i) != NULL){
		fprintf(fptr, "%s", *(generatedCode + i));
		i++;
	}


	return 0;
}
