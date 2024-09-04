#include "./code-writer.h"


char *codeWriter(struct VmInst *inst){

	char *asmInst = malloc(sizeof(char) * 100);

	if(strcmp(inst->type, "C_PUSH") == 0){
		sprintf(asmInst, "// push command\n");
	}
	else if(strcmp(inst->type, "C_POP") == 0){
		sprintf(asmInst, "// pop command\n");
	}
	else if(strcmp(inst->type, "C_ARITHMETIC") == 0){
		sprintf(asmInst, "// arithmetic\n");
	} 
	else {
		sprintf(asmInst, "// not determined\n");
	}

	return asmInst;

}
