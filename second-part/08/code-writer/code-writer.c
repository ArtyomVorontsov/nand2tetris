#include "./code-writer.h"

extern char *SourceFileNameWithoutExt;
extern char *SourceFileName;
extern struct AmountOfInvocations *functionInvocationsTable; 
extern char *functionNameCtx;

char *codeWriter(struct VmInst *inst){

	char *asmInst = malloc(sizeof(char) * 10000);

	if(strcmp(inst->type, "C_PUSH") == 0){
		sprintf(asmInst, "// push command\n");

		if(strcmp(inst->arg1, "constant") == 0){
			sprintf(asmInst + strlen(asmInst), "@%s\n", inst->arg2);
			sprintf(asmInst + strlen(asmInst), "D=A\n");
		}
		else if(strcmp(inst->arg1, "local") == 0){
			sprintf(asmInst + strlen(asmInst), "@LCL\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
			sprintf(asmInst + strlen(asmInst), "@%s\n", inst->arg2);
			sprintf(asmInst + strlen(asmInst), "A=D+A\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
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
		else if(strcmp(inst->arg1, "this") == 0){
			sprintf(asmInst + strlen(asmInst), "@THIS\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
			sprintf(asmInst + strlen(asmInst), "@%s\n", inst->arg2);
			sprintf(asmInst + strlen(asmInst), "A=D+A\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
		}
		else if(strcmp(inst->arg1, "that") == 0){
			sprintf(asmInst + strlen(asmInst), "@THAT\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
			sprintf(asmInst + strlen(asmInst), "@%s\n", inst->arg2);
			sprintf(asmInst + strlen(asmInst), "A=D+A\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
		}
		else if(strcmp(inst->arg1, "temp") == 0){
			sprintf(asmInst + strlen(asmInst), "@TEMP\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
			sprintf(asmInst + strlen(asmInst), "@%s\n", inst->arg2);
			sprintf(asmInst + strlen(asmInst), "A=D+A\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
		}
		else if(strcmp(inst->arg1, "temp") == 0){
			sprintf(asmInst + strlen(asmInst), "@TEMP\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
			sprintf(asmInst + strlen(asmInst), "@%s\n", inst->arg2);
			sprintf(asmInst + strlen(asmInst), "A=D+A\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
		}
		else if(strcmp(inst->arg1, "static") == 0){
			sprintf(asmInst + strlen(asmInst), "@%s.%s\n", SourceFileNameWithoutExt, inst->arg2);
			sprintf(asmInst + strlen(asmInst), "D=M\n");
		}
		else if(strcmp(inst->arg1, "argument") == 0){
			sprintf(asmInst + strlen(asmInst), "@ARG\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
			sprintf(asmInst + strlen(asmInst), "@%s\n", inst->arg2);
			sprintf(asmInst + strlen(asmInst), "A=D+A\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
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
		else if(strcmp(inst->arg1, "temp") == 0){
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
		else if(strcmp(inst->arg1, "this") == 0){
			sprintf(asmInst + strlen(asmInst), "\n");
			sprintf(asmInst + strlen(asmInst), "@THIS // calculate THIS with offset and safe in R14\n");
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
		else if(strcmp(inst->arg1, "that") == 0){
			sprintf(asmInst + strlen(asmInst), "\n");
			sprintf(asmInst + strlen(asmInst), "@THAT // calculate THAT with offset and safe in R14\n");
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
		else if(strcmp(inst->arg1, "static") == 0){
			sprintf(asmInst + strlen(asmInst), "@R13\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
			sprintf(asmInst + strlen(asmInst), "@%s.%s\n", SourceFileNameWithoutExt,inst->arg2);
			sprintf(asmInst + strlen(asmInst), "M=D\n");
		}
		else if(strcmp(inst->arg1, "argument") == 0){
			sprintf(asmInst + strlen(asmInst), "\n");
			sprintf(asmInst + strlen(asmInst), "@ARG // calculate ARG with offset and safe in R14\n");
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
		else if(strcmp(inst->arg1, "sub") == 0){
			sprintf(asmInst + strlen(asmInst), "// sub instruction\n");
			genPopFromTheStackIntoR13Register(asmInst);
			sprintf(asmInst + strlen(asmInst), "@R13\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
			sprintf(asmInst + strlen(asmInst), "@R14\n");
			sprintf(asmInst + strlen(asmInst), "M=D\n");
			genPopFromTheStackIntoR13Register(asmInst);
			sprintf(asmInst + strlen(asmInst), "@R13\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
			sprintf(asmInst + strlen(asmInst), "@R14\n");
			sprintf(asmInst + strlen(asmInst), "D=D-M\n");
			genPushOnTheStackDRegister(asmInst);
		}
		else if(strcmp(inst->arg1, "neg") == 0){
			sprintf(asmInst + strlen(asmInst), "// neg instruction\n");
			genPopFromTheStackIntoR13Register(asmInst);
			sprintf(asmInst + strlen(asmInst), "@R13\n");
			sprintf(asmInst + strlen(asmInst), "D=-M\n");
			genPushOnTheStackDRegister(asmInst);
		}
		else if(strcmp(inst->arg1, "and") == 0){
			sprintf(asmInst + strlen(asmInst), "// and instruction\n");
			genPopFromTheStackIntoR13Register(asmInst);
			sprintf(asmInst + strlen(asmInst), "@R13\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
			sprintf(asmInst + strlen(asmInst), "@R14\n");
			sprintf(asmInst + strlen(asmInst), "M=D\n");
			genPopFromTheStackIntoR13Register(asmInst);
			sprintf(asmInst + strlen(asmInst), "@R13\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
			sprintf(asmInst + strlen(asmInst), "@R14\n");
			sprintf(asmInst + strlen(asmInst), "D=D&M\n");
			genPushOnTheStackDRegister(asmInst);
		}
		else if(strcmp(inst->arg1, "eq") == 0){
			sprintf(asmInst + strlen(asmInst), "// eq instruction\n");
			genPopFromTheStackIntoR13Register(asmInst);
			sprintf(asmInst + strlen(asmInst), "@R13\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
			sprintf(asmInst + strlen(asmInst), "@R14\n");
			sprintf(asmInst + strlen(asmInst), "M=D\n");
			genPopFromTheStackIntoR13Register(asmInst);
			sprintf(asmInst + strlen(asmInst), "@R13\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
			sprintf(asmInst + strlen(asmInst), "@R14\n");
			sprintf(asmInst + strlen(asmInst), "D=D-M\n");
			sprintf(asmInst + strlen(asmInst), "@NEQ.%s.%d\n", SourceFileNameWithoutExt, inst->line);
			sprintf(asmInst + strlen(asmInst), "D;JNE\n");
			sprintf(asmInst + strlen(asmInst), "D=1\n");
			sprintf(asmInst + strlen(asmInst), "@EQ.%s.%d\n", SourceFileNameWithoutExt, inst->line);
			sprintf(asmInst + strlen(asmInst), "0;JMP\n");
			sprintf(asmInst + strlen(asmInst), "(NEQ.%s.%d)\n", SourceFileNameWithoutExt, inst->line);
			sprintf(asmInst + strlen(asmInst), "D=0\n");
			sprintf(asmInst + strlen(asmInst), "(EQ.%s.%d)\n", SourceFileNameWithoutExt, inst->line);

			genPushOnTheStackDRegister(asmInst);
		}
		else if(strcmp(inst->arg1, "gt") == 0){
			sprintf(asmInst + strlen(asmInst), "// gt instruction\n");
			genPopFromTheStackIntoR13Register(asmInst);
			sprintf(asmInst + strlen(asmInst), "@R13\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
			sprintf(asmInst + strlen(asmInst), "@R14\n");
			sprintf(asmInst + strlen(asmInst), "M=D\n");
			genPopFromTheStackIntoR13Register(asmInst);
			sprintf(asmInst + strlen(asmInst), "@R13\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
			sprintf(asmInst + strlen(asmInst), "@R14\n");
			sprintf(asmInst + strlen(asmInst), "D=D-M\n");
			sprintf(asmInst + strlen(asmInst), "@GT.%s.%d\n", SourceFileNameWithoutExt, inst->line);
			sprintf(asmInst + strlen(asmInst), "D;JGT\n");
			sprintf(asmInst + strlen(asmInst), "D=0\n");
			sprintf(asmInst + strlen(asmInst), "@NGT.%s.%d\n", SourceFileNameWithoutExt, inst->line);
			sprintf(asmInst + strlen(asmInst), "0;JMP\n");
			sprintf(asmInst + strlen(asmInst), "(GT.%s.%d)\n", SourceFileNameWithoutExt, inst->line);
			sprintf(asmInst + strlen(asmInst), "D=1\n");
			sprintf(asmInst + strlen(asmInst), "(NGT.%s.%d)\n", SourceFileNameWithoutExt, inst->line);

			genPushOnTheStackDRegister(asmInst);
		}
		else if(strcmp(inst->arg1, "lt") == 0){
			sprintf(asmInst + strlen(asmInst), "// lt instruction\n");
			genPopFromTheStackIntoR13Register(asmInst);
			sprintf(asmInst + strlen(asmInst), "@R13\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
			sprintf(asmInst + strlen(asmInst), "@R14\n");
			sprintf(asmInst + strlen(asmInst), "M=D\n");
			genPopFromTheStackIntoR13Register(asmInst);
			sprintf(asmInst + strlen(asmInst), "@R13\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
			sprintf(asmInst + strlen(asmInst), "@R14\n");
			sprintf(asmInst + strlen(asmInst), "D=D-M\n");
			sprintf(asmInst + strlen(asmInst), "@LT.%s.%d\n", SourceFileNameWithoutExt, inst->line);
			sprintf(asmInst + strlen(asmInst), "D;JLT\n");
			sprintf(asmInst + strlen(asmInst), "D=0\n");
			sprintf(asmInst + strlen(asmInst), "@NLT.%s.%d\n", SourceFileNameWithoutExt, inst->line);
			sprintf(asmInst + strlen(asmInst), "0;JMP\n");
			sprintf(asmInst + strlen(asmInst), "(LT.%s.%d)\n", SourceFileNameWithoutExt, inst->line);
			sprintf(asmInst + strlen(asmInst), "D=1\n");
			sprintf(asmInst + strlen(asmInst), "(NLT.%s.%d)\n", SourceFileNameWithoutExt, inst->line);

			genPushOnTheStackDRegister(asmInst);
		}
		else if(strcmp(inst->arg1, "or") == 0){
			sprintf(asmInst + strlen(asmInst), "// or instruction\n");
			genPopFromTheStackIntoR13Register(asmInst);
			sprintf(asmInst + strlen(asmInst), "@R13\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
			sprintf(asmInst + strlen(asmInst), "@R14\n");
			sprintf(asmInst + strlen(asmInst), "M=D\n");
			genPopFromTheStackIntoR13Register(asmInst);
			sprintf(asmInst + strlen(asmInst), "@R13\n");
			sprintf(asmInst + strlen(asmInst), "D=M\n");
			sprintf(asmInst + strlen(asmInst), "@R14\n");
			sprintf(asmInst + strlen(asmInst), "D=D|M\n");
			genPushOnTheStackDRegister(asmInst);
		}
		else if(strcmp(inst->arg1, "not") == 0){
			sprintf(asmInst + strlen(asmInst), "// not instruction\n");
			genPopFromTheStackIntoR13Register(asmInst);
			sprintf(asmInst + strlen(asmInst), "@R13\n");
			sprintf(asmInst + strlen(asmInst), "D=!M\n");
			genPushOnTheStackDRegister(asmInst);
		}
	} 
	else if(strcmp(inst->type, "C_GOTO") == 0){
		sprintf(asmInst + strlen(asmInst), "@%s\n", inst->arg1);
		sprintf(asmInst + strlen(asmInst), "0;JMP\n");
	}
	else if(strcmp(inst->type, "C_IF_GOTO") == 0){
		genPopFromTheStackIntoR13Register(asmInst);
		sprintf(asmInst + strlen(asmInst), "@R13\n");
		sprintf(asmInst + strlen(asmInst), "D=M\n");
		sprintf(asmInst + strlen(asmInst), "@%s\n", inst->arg1);
		sprintf(asmInst + strlen(asmInst), "D;JNE\n");
	}
	else if(strcmp(inst->type, "LABEL") == 0){
		sprintf(asmInst + strlen(asmInst), "(%s)\n", inst->arg1);

	}
	else if(strcmp(inst->type, "C_FUNCTION") == 0){
		sprintf(asmInst + strlen(asmInst), "(%s)\n", inst->arg1);
		functionNameCtx = inst->arg1;
	}
	else if(strcmp(inst->type, "C_RETURN") == 0){
		sprintf(asmInst + strlen(asmInst), "// Function %s epilogue\n", functionNameCtx);

		genPopFromTheStackIntoR13Register(asmInst);
		sprintf(asmInst + strlen(asmInst), "// Put return value for function which invoked that function\n");
		sprintf(asmInst + strlen(asmInst), "@R13\n");
		sprintf(asmInst + strlen(asmInst), "D=M\n");
		sprintf(asmInst + strlen(asmInst), "@R14\n");
		sprintf(asmInst + strlen(asmInst), "M=D\n");


		sprintf(asmInst + strlen(asmInst), "// Save SP value for function which invoked that function\n");
		sprintf(asmInst + strlen(asmInst), "@ARG\n");
		sprintf(asmInst + strlen(asmInst), "D=M\n");
		sprintf(asmInst + strlen(asmInst), "@R15\n");
		sprintf(asmInst + strlen(asmInst), "M=D\n");

		sprintf(asmInst + strlen(asmInst), "// Put saved memory segment values back\n");

		genPopFromTheStackIntoR13Register(asmInst);
		sprintf(asmInst + strlen(asmInst), "@R13\n");
		sprintf(asmInst + strlen(asmInst), "D=M\n");
		sprintf(asmInst + strlen(asmInst), "@THAT\n");
		sprintf(asmInst + strlen(asmInst), "M=D\n");
		
		genPopFromTheStackIntoR13Register(asmInst);
		sprintf(asmInst + strlen(asmInst), "@R13\n");
		sprintf(asmInst + strlen(asmInst), "D=M\n");
		sprintf(asmInst + strlen(asmInst), "@THIS\n");
		sprintf(asmInst + strlen(asmInst), "M=D\n");
		
		genPopFromTheStackIntoR13Register(asmInst);
		sprintf(asmInst + strlen(asmInst), "@R13\n");
		sprintf(asmInst + strlen(asmInst), "D=M\n");
		sprintf(asmInst + strlen(asmInst), "@ARG\n");
		sprintf(asmInst + strlen(asmInst), "M=D\n");
		
		genPopFromTheStackIntoR13Register(asmInst);
		sprintf(asmInst + strlen(asmInst), "@R13\n");
		sprintf(asmInst + strlen(asmInst), "D=M\n");
		sprintf(asmInst + strlen(asmInst), "@LCL\n");
		sprintf(asmInst + strlen(asmInst), "M=D\n");
		
		sprintf(asmInst + strlen(asmInst), "// Jump to return label\n");
		genPopFromTheStackIntoR13Register(asmInst);

		sprintf(asmInst + strlen(asmInst), "// Set old SP value\n");
		sprintf(asmInst + strlen(asmInst), "@R15\n");
		sprintf(asmInst + strlen(asmInst), "D=M\n");
		sprintf(asmInst + strlen(asmInst), "@SP\n");
		sprintf(asmInst + strlen(asmInst), "M=D\n");

		sprintf(asmInst + strlen(asmInst), "// Push function return value on the stack\n");
		sprintf(asmInst + strlen(asmInst), "@R14\n");
		sprintf(asmInst + strlen(asmInst), "D=M\n");
		genPushOnTheStackDRegister(asmInst);

		sprintf(asmInst + strlen(asmInst), "// Jump to return label\n");
		sprintf(asmInst + strlen(asmInst), "@R13\n");
		sprintf(asmInst + strlen(asmInst), "A=M\n");
		sprintf(asmInst + strlen(asmInst), "0;JMP\n");
	}
	else if(strcmp(inst->type, "C_CALL") == 0){
		sprintf(asmInst + strlen(asmInst), "// Function %s prologue\n", functionNameCtx);
		addFunctionInvocation(inst->arg1);
		struct AmountOfInvocations *functionInvocation = getFunctionInvocation(inst->arg1);

		sprintf(asmInst + strlen(asmInst), "// Push return label value on top of the stack\n");
		sprintf(asmInst + strlen(asmInst), "@%s$ret.%d\n", inst->arg1, functionInvocation->invocationAmount);
		sprintf(asmInst + strlen(asmInst), "D=A\n");
		genPushOnTheStackDRegister(asmInst);

		sprintf(asmInst + strlen(asmInst), "// Save memory addresses of current function in stack frame\n");
		sprintf(asmInst + strlen(asmInst), "@LCL\n");
		sprintf(asmInst + strlen(asmInst), "D=M\n");
		genPushOnTheStackDRegister(asmInst);

		sprintf(asmInst + strlen(asmInst), "@ARG\n");
		sprintf(asmInst + strlen(asmInst), "D=M\n");
		genPushOnTheStackDRegister(asmInst);

		sprintf(asmInst + strlen(asmInst), "@THIS\n");
		sprintf(asmInst + strlen(asmInst), "D=M\n");
		genPushOnTheStackDRegister(asmInst);

		sprintf(asmInst + strlen(asmInst), "@THAT\n");
		sprintf(asmInst + strlen(asmInst), "D=M\n");
		genPushOnTheStackDRegister(asmInst);

		sprintf(asmInst + strlen(asmInst), "// Assign new values for memory segments\n");
		sprintf(asmInst + strlen(asmInst), "@SP\n");
		sprintf(asmInst + strlen(asmInst), "D=M\n");
		sprintf(asmInst + strlen(asmInst), "// Assign new value for LCL memory segment\n");
		sprintf(asmInst + strlen(asmInst), "@LCL\n");
		sprintf(asmInst + strlen(asmInst), "M=D\n");

		sprintf(asmInst + strlen(asmInst), "// Assign new value for ARG memory segment\n");
		sprintf(asmInst + strlen(asmInst), "@ARG\n");
		sprintf(asmInst + strlen(asmInst), "M=D-1\n");
		sprintf(asmInst + strlen(asmInst), "M=M-1\n");
		sprintf(asmInst + strlen(asmInst), "M=M-1\n");
		sprintf(asmInst + strlen(asmInst), "M=M-1\n");
		sprintf(asmInst + strlen(asmInst), "M=M-1\n");
		sprintf(asmInst + strlen(asmInst), "@%s\n", inst->arg2);
		sprintf(asmInst + strlen(asmInst), "D=A\n");
		sprintf(asmInst + strlen(asmInst), "@ARG\n");
		sprintf(asmInst + strlen(asmInst), "M=M-D\n");


		sprintf(asmInst + strlen(asmInst), "// JMP to function body code\n");
		sprintf(asmInst + strlen(asmInst), "@%s\n", inst->arg1);
		sprintf(asmInst + strlen(asmInst), "0;JMP\n");

		sprintf(asmInst + strlen(asmInst), "(%s$ret.%d)\n", inst->arg1, functionInvocation->invocationAmount);
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
