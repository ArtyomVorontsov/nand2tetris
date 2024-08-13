/* 
 * That is parser for hack assemler which is one of core components of nand2tetris course.
 * Parser follows recomendation and specification which was described in nand2tetris book.
 * Do with that file whathever you want, I don't care.
 */

#include "./parser.h"


struct CInstruction* parser(char* sp){
	struct CInstruction* cInst = (struct CInstruction*) malloc(sizeof (struct CInstruction));

	cInst->dest = dest(sp);
	cInst->comp = comp(sp);

	
	return cInst;
};

char* dest(char* sp){
	char* destP = (char*) malloc(sizeof(char) * 4);

	int i = 0;
	while(*sp != '='){
		*(destP + i++) = *sp++;
	}
	*(destP + i++) = '\0';

	return destP;
};

char* comp(char* sp){
	char* compP = (char*) malloc(sizeof(char) * 4);

	int i = 0;
	while(compP++, *sp++ != '=');
	while(*sp != '\n'){
		*(compP + i++) = *sp++;
	}
	*(compP + i++) = '\0';

	return compP;
}

