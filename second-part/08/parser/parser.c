#include "./parser.h"

struct VmInst *parser(char *line, int lineNumber){
	char *cmnd = getCmnd(line);
	char *cmndType = commandType(cmnd);
	char *a1, *a2;
	struct VmInst *vmInst = malloc(sizeof(struct VmInst));

	if((strcmp("C_RETURN", cmndType) != 0)){
		vmInst->arg1 = cmnd;

		if(
			(strcmp("C_POP", cmndType) == 0) ||
			(strcmp("C_PUSH", cmndType) == 0) ||
			(strcmp("C_GOTO", cmndType) == 0) ||
			(strcmp("C_IF_GOTO", cmndType) == 0) ||
			(strcmp("LABEL", cmndType) == 0) ||
			(strcmp("C_FUNCTION", cmndType) == 0) ||
			(strcmp("C_CALL", cmndType) == 0)
		){
			vmInst->arg1 = argX(line, 1);
		}
	}
	
	if(
		(strcmp("C_PUSH", cmndType) == 0) ||
		(strcmp("C_POP", cmndType) == 0) ||
		(strcmp("C_FUNCTION", cmndType) == 0) ||
		(strcmp("C_CALL", cmndType) == 0) ||
		(strcmp("C_FUNCTION", cmndType) == 0) ||
		(strcmp("C_CALL", cmndType) == 0)
	){
		vmInst->arg2 = argX(line, 2);
	}

	vmInst->cmnd = cmnd;
	vmInst->type = cmndType;
	vmInst->line = lineNumber;

	return vmInst;
}


char *argX(char *s, int x){
	int i = 0;
	char *cmnd = malloc(sizeof(char) * 100);
	while(*(s + i) == ' ') i++;

	while(x--){
		while(*(s + i) != ' ') i++;
		while(*(s + i) == ' ') i++;
	}

	int j = 0;
	while((*(s + i) != '\t') && (*(s + i) != ' ') && (*(s + i) != '\n')){
		*(cmnd + j) = *(s + i);

		i++;
		j++;
	}
	cmnd[j] = '\0';

	return cmnd;
}

char *getCmnd(char *s){
	int i = 0;
	char *cmnd = malloc(sizeof(char) * 100);
	while(*(s + i) == ' ') i++;

	int j = 0;
	while((*(s + i) != '\t') && (*(s + i) != ' ') && (*(s + i) != '\n')){
		*(cmnd + j) = *(s + i);

		i++;
		j++;
	}
	cmnd[j] = '\0';

	return cmnd;
}


char *commandType(char *cmnd){
	if(strcmp(cmnd, "push") == 0){
		return "C_PUSH";
	} 
	else if(strcmp(cmnd, "pop") == 0){
		return "C_POP";
	} 
	else if(
		(strcmp(cmnd, "add") == 0) ||
		(strcmp(cmnd, "sub") == 0) ||
		(strcmp(cmnd, "neg") == 0) ||
		(strcmp(cmnd, "eq") == 0) ||
		(strcmp(cmnd, "gt") == 0) ||
		(strcmp(cmnd, "lt") == 0) ||
		(strcmp(cmnd, "and") == 0) ||
		(strcmp(cmnd, "or") == 0) ||
		(strcmp(cmnd, "not") == 0) 
	){
		return "C_ARITHMETIC";
	} 
	else if((strcmp(cmnd, "goto") == 0)){
		return "C_GOTO";

	}
	else if((strcmp(cmnd, "if-goto") == 0)){
		return "C_IF_GOTO";
	}
	else if((strcmp(cmnd, "label") == 0)){
		return "LABEL";
	}
	else if((strcmp(cmnd, "function") == 0)){
		return "C_FUNCTION";
	}
	else if((strcmp(cmnd, "return") == 0)){
		return "C_RETURN";
	}
	else if((strcmp(cmnd, "call") == 0)){
		return "C_CALL";
	}
	else {
		return "NOT_DEFINED";
	}

}
