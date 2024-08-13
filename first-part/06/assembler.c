#include "./parser.h"
#include <stdio.h>

/* TODO: 
 * Use debugger gdb
 * Use some unit testing suite
 */


int main(){

	struct CInstruction* cInst = parser("A=A+B");

	printf("comp: %s, dest: %s\n", cInst->comp, cInst->dest);

	return 0;
}
