#include "./parser.h"



struct VmInst *parser(char *line, int lineNumber){
	char *cmnd = arg1(line);
	char *cmndType = commandType(cmnd);
	char *a1, *a2;
	struct VmInst *vmInst = malloc(sizeof(struct VmInst));

	if((strcmp("C_RETURN", cmndType) > 0)){
		vmInst->arg1 = cmnd;
	}

	if(
		(strcmp("C_PUSH", cmndType) == 0) ||
		(strcmp("C_POP", cmndType) == 0) ||
		(strcmp("C_FUNCTION", cmndType) == 0) ||
		(strcmp("C_CALL", cmndType) == 0)
	){
		vmInst->arg2 = arg2(line);
	}

	vmInst->cmnd = cmnd;
	vmInst->type = cmndType;
	vmInst->line = lineNumber;

	return vmInst;
}

char *arg2(char *s){
	int i = 0;
	char *cmnd = malloc(sizeof(char) * 100);
	while(*(s + i) == ' ') i++;
	while(*(s + i) != ' ') i++;
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

char *arg1(char *s){
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
	else {
		return "NOT_DEFINED";
	}

}
