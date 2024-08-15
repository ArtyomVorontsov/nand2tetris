/* 
 * That is parser for hack assemler which is one of core components of nand2tetris course.
 * Parser follows recomendation and specification which was described in nand2tetris book.
 * Do with that file whathever you want, I don't care.
 */

#include "./parser.h"
#include "../../../lib/cutest-1.5/CuTest.h"
void RunAllTests(void);

int main(int argc, char *argv[]){
	//TEST_FLAG  

	char *program = "A=A+B\nA=A+C\n";
		
	struct CInstruction **parsed = parser(program);

	printf("%s\n", (*parsed)->comp);

	return 0;
}


struct CInstruction** parser(char *sp){

	struct CInstruction** cInstructions = malloc(sizeof(struct CInstruction*) * 1000);
	while(hasMoreLines(sp)){
		struct CInstruction* cInst = (struct CInstruction*) malloc(sizeof (struct CInstruction));
		
		char *it = instructionType(sp);

		if(strcmp("C_INSTRUCTION", it)){
			cInst->dest = dest(sp);
			cInst->comp = comp(sp);
			cInst->jump = jump(sp);

		} 
		else if(strcmp("A_INSTRUCTION", it)){

		}
		else if(strcmp("L_INSTRUCTION", it)){

		} 
		else {
			exit(1);
		}
	
		sp = advance(sp);
	}
	
	return cInstructions;
}


bool hasMoreLines(char *sp){
	return *sp != '\0';
}

char *advance(char *sp){
	while(*sp++ != '\n');
	return sp;
}

char *instructionType(char *sp){
	while(*sp != '\n' && *sp != '\0'){
		if(*sp == ';' || *sp == '='){
			return "C_INSTRUCTION";
		}

		if(*sp == '@'){
			return "A_INSTRUCTION";
		}

		if(*sp == '('){
			return "L_INSTRUCTION";
		}
		sp++;
	}
	return "NOT_DETERMINED";
}

char *dest(char* sp){
	char *destP = (char*) malloc(sizeof(char) * 4);

	int i = 0;
	while(*sp != '='){
		*(destP + i++) = *sp++;
	}
	*(destP + i++) = '\0';

	return destP;
};

char *comp(char* sp){
	char *compP = (char*) malloc(sizeof(char) * 10);

	int i = 0;
	while(*sp++ != '=');
	while(i < 4){
		*(compP + i++) = *sp++;
	}
	*(compP + i++) = '\0';

	return compP;
}

char *jump(char* sp){
	char *jumpP = (char*) malloc(sizeof(char) * 4);

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
void hasMoreLinesTest(CuTest *tc);
void hasMoreLinesTest2(CuTest *tc);
void advanceTest(CuTest *tc);
void instructionTypeTest(CuTest *tc);
void instructionTypeTest2(CuTest *tc);
void instructionTypeTest3(CuTest *tc);
void instructionTypeTest4(CuTest *tc);

CuSuite *parser_exercise1_chapter6_getSuite(void){
	CuSuite *suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, destTest);
	SUITE_ADD_TEST(suite, compTest);
	SUITE_ADD_TEST(suite, jumpTest);
	SUITE_ADD_TEST(suite, jumpTest2);
	SUITE_ADD_TEST(suite, hasMoreLinesTest);
	SUITE_ADD_TEST(suite, hasMoreLinesTest2);
	SUITE_ADD_TEST(suite, advanceTest);
	SUITE_ADD_TEST(suite, instructionTypeTest);
	SUITE_ADD_TEST(suite, instructionTypeTest2);
	SUITE_ADD_TEST(suite, instructionTypeTest3);
	SUITE_ADD_TEST(suite, instructionTypeTest4);

	return suite;
}

void advanceTest(CuTest *tc){
	char *text = "A=A+D\nA=A+C\n0;JMP";
	char *expected = "A=A+C\n0;JMP";
	char *actual = advance(text);
	CuAssertStrEquals(tc, expected, actual);
}


void hasMoreLinesTest(CuTest *tc){
	char *text = "A=A+D\nA=A+C\n0;JMP";
	bool expected = true;
	bool actual = hasMoreLines(text);
	CuAssertIntEquals(tc, expected, actual);
}

void hasMoreLinesTest2(CuTest *tc){
	char *text = "A=A+D\nA=A+C\n0;JMP\n";
	bool expected = false;
	bool actual = hasMoreLines(text + 18);
	CuAssertIntEquals(tc, expected, actual);
}

void instructionTypeTest(CuTest *tc){
	char *instruction = "A=A+D";

	char *expected = "C_INSTRUCTION";
	char *actual = instructionType(instruction);
	CuAssertStrEquals(tc, expected, actual);
}

void instructionTypeTest2(CuTest *tc){
	char *instruction = "0;JMP";

	char *expected = "C_INSTRUCTION";
	char *actual = instructionType(instruction);
	CuAssertStrEquals(tc, expected, actual);
}

void instructionTypeTest3(CuTest *tc){
	char *instruction = "@hello";

	char *expected = "A_INSTRUCTION";
	char *actual = instructionType(instruction);
	CuAssertStrEquals(tc, expected, actual);
}

void instructionTypeTest4(CuTest *tc){
	char *instruction = "(hello)";

	char *expected = "L_INSTRUCTION";
	char *actual = instructionType(instruction);
	CuAssertStrEquals(tc, expected, actual);
}

void destTest(CuTest *tc){
	char *instruction = "A=A+D";

	char *expected = "A";
	char *actual = dest(instruction);
	CuAssertStrEquals(tc, expected, actual);
}

void compTest(CuTest *tc){
	char *instruction = "A=A+D";

	char *expected = "A+D";
	char *actual = comp(instruction);
	CuAssertStrEquals(tc, expected, actual);
}

void jumpTest(CuTest *tc){
	char *instruction = "0;JMP";

	char *expected = "JMP";
	char *actual = jump(instruction);
	CuAssertStrEquals(tc, expected, actual);
}

void jumpTest2(CuTest *tc){
	char *instruction = "0;JNE";

	char *expected = "JNE";
	char *actual = jump(instruction);
	CuAssertStrEquals(tc, expected, actual);
}
