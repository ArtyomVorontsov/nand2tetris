#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>



char *getFileNameWithoutExtension(char *fileName);

struct VmInst {
	char *type;
	char *cmnd;
	char *arg1;
	char *arg2;
	int line;
};

struct VmInst *parser(char *line, int lineNumber);
char *codeWriter(struct VmInst *inst);

struct AmountOfInvocations {
 	char *functionName;
	int invocationAmount;
};

void addFunctionInvocation(char *fnName);
struct AmountOfInvocations *getFunctionInvocation(char *fnName);
