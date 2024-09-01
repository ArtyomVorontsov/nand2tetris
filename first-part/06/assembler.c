#include "assembler.h"

/* 
 * TODO: 
 * improve memory handling
 */


int main(int argc, char **argv){
	char ** generatedCode;
	char *sourceCodeFilename = *(argv + 1);
	char *program;
	struct CInstruction** parsed;
		
	program = getProgramText(sourceCodeFilename);
	
	parsed = parser(program);
	printf("Parsed.\n");

	// Print parsed structures
	for(int i = 0; *(parsed + i) != NULL; i++){
		printf("dest: %s\ncomp: %s\njump: %s\naddress: %s\nsymbol: %s\nlabel: %s\n\n", 
			(*(parsed + i))->dest, 
			(*(parsed + i))->comp, 
			(*(parsed + i))->jump,
			(*(parsed + i))->address,
			(*(parsed + i))->symbol,
			(*(parsed + i))->label
		);
	}

	generatedCode = code(parsed);


	// Print generated code
	for(int i = 0; *(generatedCode + i) != NULL; i++){
		printf("%s", *(generatedCode + i));
	}

	char *fileNameWithoutExtension = 
		getFileNameWithoutExtension(sourceCodeFilename);

	writeTextToFile(fileNameWithoutExtension, generatedCode);

	free(fileNameWithoutExtension);

	return 0;
}

char *getFileNameWithoutExtension(char *filename){
	int i = 0;
	char *filenameWithoutExtension = malloc(sizeof(char) * strlen(filename));
	while(*(filename + i) != '.' && *(filename + i) != '\0'){
		*(filenameWithoutExtension + i) = *(filename + i);

		i++;
	}

	printf("%s\n", filename);

	return filenameWithoutExtension;
}

char * getProgramText(char *sourceCodeFilename){
	const int MAX_LINE_SIZE = 1000;
	const int PROGRAM_SIZE_PAGE = 100;
	int programSize = PROGRAM_SIZE_PAGE;
	FILE *fp;
	char *program = malloc(sizeof(char) * programSize);
	char line[100];

	// Get text from file
	fp = fopen(sourceCodeFilename, "r");
	if(fp == NULL){
		printf("Cannot open file\n");
	}
	
       	int j = 0;	
	while (fgets(line, MAX_LINE_SIZE, fp) != NULL) {
		int i = 0;

		do {
			*(program + j) = *(line + i);
			j++; i++;
			
			if(programSize < j){
				// yep, here we assume that data will be preserved after reallocation
				// not super robust way, but for learning project it's ok.
				program = realloc(program, programSize = programSize + PROGRAM_SIZE_PAGE);
			}
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
	// Open a file in writing mode, clear file
	FILE* fptr = fopen(fileName, "w");
	fprintf(fptr, "%s", "\0");
	fclose(fptr);	

	// Open a file in append mode
	fptr = fopen(fileName, "a");

	// Write some text to the file
	i = 0;
	char *gc;
	while((gc = *(generatedCode + i)) != NULL){
		fprintf(fptr, "%s", gc);
		i++;
	}

	fclose(fptr);	
}

