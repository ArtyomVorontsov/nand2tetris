/* 
 * That is parser for hack assemler which is one of core components of nand2tetris course.
 * Parser follows recomendation and specification which was described in nand2tetris book.
 * Do with that file whathever you want, I don't care.
 */

#include "./parser.h"
#include "../../../lib/cutest-1.5/CuTest.h"
void RunAllTests(void);

int main(int argc, char *argv[]){
	TEST_FLAG

	return 0;
}


struct CInstruction* parser(char* sp){
	struct CInstruction* cInst = (struct CInstruction*) malloc(sizeof (struct CInstruction));

	cInst->dest = dest(sp);
	cInst->comp = comp(sp);

	
	return cInst;
};

enum InstructionType instructionType(char* sp){
	int i = 0;
	while(*(sp + i++) != '\n' || *(sp + i) != '\0'){
		if(*sp == ';'){
			return C_INSTRUCTION;
		}
	}

}

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
	char* compP = (char*) malloc(sizeof(char) * 10);

	int i = 0;
	while(*sp++ != '=');
	while(i < 4){
		*(compP + i++) = *sp++;
	}
	*(compP + i++) = '\0';

	return compP;
}

char* jump(char* sp){
	char* jumpP = (char*) malloc(sizeof(char) * 4);

	int i = 0, j = 0;
	bool isJump = false;
	while(*(sp + i++) != '\n' && *(sp + i) != '\0'){
		if(isJump){
			*(jumpP + j++) = *(sp + i);
		}

		if(*(sp + i) == ';'){
			isJump = true;
		}

	}
	*(jumpP + j++) = '\0';

	return jumpP;
}

/* Tests */

void destTest(CuTest *tc);
void compTest(CuTest *tc);
void jumpTest(CuTest *tc);
void jumpTest2(CuTest *tc);

CuSuite *parser_exercise1_chapter6_getSuite(void){
	CuSuite *suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, destTest);
	SUITE_ADD_TEST(suite, compTest);
	SUITE_ADD_TEST(suite, jumpTest);
	SUITE_ADD_TEST(suite, jumpTest2);

	return suite;
}


void destTest(CuTest *tc){
	char* instruction = "A=A+D";

	char* expected = "A";
	char* actual = dest(instruction);
	CuAssertStrEquals(tc, expected, actual);
}


void compTest(CuTest *tc){
	char* instruction = "A=A+D";

	char* expected = "A+D";
	char* actual = comp(instruction);
	CuAssertStrEquals(tc, expected, actual);
}

void jumpTest(CuTest *tc){
	char* instruction = "0;JMP";

	char* expected = "JMP";
	char* actual = jump(instruction);
	CuAssertStrEquals(tc, expected, actual);
}

void jumpTest2(CuTest *tc){
	char* instruction = "0;JNE";

	char* expected = "JNE";
	char* actual = jump(instruction);
	CuAssertStrEquals(tc, expected, actual);
}
