#include "./code-writer.h"


char *codeWriter(struct VmInst *inst){

	char *asmInst = malloc(sizeof(char) * 1000);

	if(strcmp(inst->type, "C_PUSH") == 0){
		sprintf(asmInst, "// push command\n");

		if(strcmp(inst->arg1, "constant") == 0){
			sprintf(asmInst + strlen(asmInst), "@%s\n", inst->arg2);
			sprintf(asmInst + strlen(asmInst), "D=A\n");
		}
		else if(strcmp(inst->arg1, "local") == 0){
			sprintf(asmInst + strlen(asmInst), "@LCL\n");
			sprintf(asmInst + strlen(asmInst), "D=A\n");
			sprintf(asmInst + strlen(asmInst), "@%s\n", inst->arg2);
			sprintf(asmInst + strlen(asmInst), "A=A+D\n");
			sprintf(asmInst + strlen(asmInst), "M=D\n");
		}
		else if(strcmp(inst->arg1, "pointer") == 0){
		 	if(strcmp(inst->arg2, "0") == 0){
				sprintf(asmInst + strlen(asmInst), "@THIS\n");
				sprintf(asmInst + strlen(asmInst), "D=M\n");
			}  
		 	else if(strcmp(inst->arg2, "1") == 0){
				sprintf(asmInst + strlen(asmInst), "@THAT\n");
				sprintf(asmInst + strlen(asmInst), "D=M\n");
			}
		}
		else if(strcmp(inst->arg1, "temp") == 0){
			sprintf(asmInst + strlen(asmInst), "@TEMP\n");
			sprintf(asmInst + strlen(asmInst), "D=A\n");
			sprintf(asmInst + strlen(asmInst), "@%s\n", inst->arg2);
			sprintf(asmInst + strlen(asmInst), "A=A+D\n");
			sprintf(asmInst + strlen(asmInst), "M=D\n");
		}

		genPushOnTheStackDRegister(asmInst);
		sprintf(asmInst + strlen(asmInst), "\n");

	}
	else if(strcmp(inst->type, "C_POP") == 0){
		sprintf(asmInst, "// pop command\n");
		genPopFromTheStackIntoR13Register(asmInst);

		if(strcmp(inst->arg1, "local") == 0){
			sprintf(asmInst + strlen(asmInst), "\n");
			sprintf(asmInst + strlen(asmInst), "@LCL // calculate LCL with offset and safe in R14\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
			sprintf(asmInst + strlen(asmInst), "@%s\n", inst->arg2);
			sprintf(asmInst + strlen(asmInst), "D=D+A\n");
			sprintf(asmInst + strlen(asmInst), "@R14\n");
			sprintf(asmInst + strlen(asmInst), "M=D\n");

			sprintf(asmInst + strlen(asmInst), "\n");
			sprintf(asmInst + strlen(asmInst), "@R13 // store value in R13 in address which is stored in R14\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
			sprintf(asmInst + strlen(asmInst), "@R14\n");
			sprintf(asmInst + strlen(asmInst), "A=M\n");
			sprintf(asmInst + strlen(asmInst), "M=D\n");
		}
		else if(strcmp(inst->arg1, "pointer") == 0){
		 	if(strcmp(inst->arg2, "0") == 0){
				sprintf(asmInst + strlen(asmInst), "@R13\n");
				sprintf(asmInst + strlen(asmInst), "D=M\n");
				sprintf(asmInst + strlen(asmInst), "@THIS\n");
				sprintf(asmInst + strlen(asmInst), "M=D\n");
			}  
		 	else if(strcmp(inst->arg2, "1") == 0){
				sprintf(asmInst + strlen(asmInst), "@R13\n");
				sprintf(asmInst + strlen(asmInst), "D=M\n");
				sprintf(asmInst + strlen(asmInst), "@THAT\n");
				sprintf(asmInst + strlen(asmInst), "M=D\n");
			}
		}
		if(strcmp(inst->arg1, "temp") == 0){
			sprintf(asmInst + strlen(asmInst), "\n");
			sprintf(asmInst + strlen(asmInst), "@TEMP // calculate TEMP with offset and safe in R14\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
			sprintf(asmInst + strlen(asmInst), "@%s\n", inst->arg2);
			sprintf(asmInst + strlen(asmInst), "D=D+A\n");
			sprintf(asmInst + strlen(asmInst), "@R14\n");
			sprintf(asmInst + strlen(asmInst), "M=D\n");

			sprintf(asmInst + strlen(asmInst), "\n");
			sprintf(asmInst + strlen(asmInst), "@R13 // store value in R13 in address which is stored in R14\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
			sprintf(asmInst + strlen(asmInst), "@R14\n");
			sprintf(asmInst + strlen(asmInst), "A=M\n");
			sprintf(asmInst + strlen(asmInst), "M=D\n");
		}

	}
	else if(strcmp(inst->type, "C_ARITHMETIC") == 0){
		sprintf(asmInst, "// arithmetic\n");
		if(strcmp(inst->arg1, "add") == 0){
			sprintf(asmInst + strlen(asmInst), "// add instruction\n");
			genPopFromTheStackIntoR13Register(asmInst);
			sprintf(asmInst + strlen(asmInst), "@R13\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
			sprintf(asmInst + strlen(asmInst), "@R14\n");
			sprintf(asmInst + strlen(asmInst), "M=D\n");
			genPopFromTheStackIntoR13Register(asmInst);
			sprintf(asmInst + strlen(asmInst), "@R13\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
			sprintf(asmInst + strlen(asmInst), "@R14\n");
			sprintf(asmInst + strlen(asmInst), "D=D+M\n");
			genPushOnTheStackDRegister(asmInst);
		}
	} 
	else {
		sprintf(asmInst, "// not determined\n");
	}

	return asmInst;

}

void genPushOnTheStackDRegister(char *buffer){
	sprintf(buffer + strlen(buffer), "\n");
	sprintf(buffer + strlen(buffer), "@SP // push on the stack value in d register\n");
	sprintf(buffer + strlen(buffer), "A=M\n");
	sprintf(buffer + strlen(buffer), "M=D\n");

	sprintf(buffer + strlen(buffer), "\n");
	sprintf(buffer + strlen(buffer), "@SP // increment stack pointer\n");
	sprintf(buffer + strlen(buffer), "M=M+1\n");
}

void genPopFromTheStackIntoR13Register(char *buffer){
	sprintf(buffer + strlen(buffer), "\n");
	sprintf(buffer + strlen(buffer), "@SP // decrement stack pointer\n");
	sprintf(buffer + strlen(buffer), "M=M-1\n");

	sprintf(buffer + strlen(buffer), "\n");
	sprintf(buffer + strlen(buffer), "A=M // set stack address\n");

	sprintf(buffer + strlen(buffer), "\n");
	sprintf(buffer + strlen(buffer), "D=M // save popped value in D register\n");

	sprintf(buffer + strlen(buffer), "\n");
	sprintf(buffer + strlen(buffer), "@R13 // save popped value in R13 register\n");
	sprintf(buffer + strlen(buffer), "M=D\n");
}
