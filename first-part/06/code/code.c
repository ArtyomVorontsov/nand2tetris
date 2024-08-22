#include "./code.h"

char **code(struct CInstruction **instructions){
	char **generatedCode = (char **) malloc(sizeof(char *) * 1000);

	int i = 0;

	struct CInstruction *inst;
	while((inst = *(instructions + i)) != NULL){
		char * binary = malloc(sizeof(char) * 17);

		if(inst->dest || inst->comp || inst->jump){

			binary[0] = '1';
			binary[1] = '1';
			binary[2] = '1';

			char * a = binary + 3;
			char * c0 = binary + 4;
			char * c1 = binary + 5;
			char * c2 = binary + 6;
			char * c3 = binary + 7;
			char * c4 = binary + 8;
			char * c5 = binary + 9;

			char * d0 = binary + 10;
			char * d1 = binary + 11;
			char * d2 = binary + 12;

			char * j0 = binary + 13;
			char * j1 = binary + 14;
			char * j2 = binary + 15;

			// comp			
			if(strcmp(inst->comp, "0") == 0){
				*a = '0'; // a

				*c0 = '1';
				*c1 = '0';
				*c2 = '1';
				*c3 = '0';
				*c4 = '1';
				*c5 = '0';
			}
			else if(strcmp(inst->comp, "1") == 0){
				*a = '0'; // a

				*c0 = '1';
				*c1 = '1';
				*c2 = '1';
				*c3 = '1';
				*c4 = '1';
				*c5 = '1';
			}
			else if(strcmp(inst->comp, "D+1") == 0){
				*a = '0'; // a

				*c0 = '0';
				*c1 = '1';
				*c2 = '1';
				*c3 = '1';
				*c4 = '1';
				*c5 = '1';
			}

			// dest
			if(strcmp(inst->dest, "M") == 0){
				*d0 = '0';
				*d1 = '0';
				*d2 = '1';
			}

			// jump 
			if(inst->jump == NULL){
				*j0 = '0';
				*j1 = '0';
				*j2 = '0';
			}

		}
		binary[16] = '\n';
		generatedCode[i] = binary;
		i++;

	}
	generatedCode[i] = NULL;

	return generatedCode;
	
}

