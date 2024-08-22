#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


struct CInstruction {
	char *dest;
	char *comp;
	char *jump;
};

char **code(struct CInstruction **instructions);
struct CInstruction** parser(char *sp);
