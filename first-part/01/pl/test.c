#include "./gates.h"
#include "stdio.h"

int main(){
	int a = 1;
	int b = 1;

	int andOutput = And(a, b);
	int nandOutput = Nand(a, b);
	printf("And: %d\n", andOutput);
	printf("Nand: %d\n", nandOutput);
			
}

