/* 
 * That is parser for hack assemler which is one of core components of nand2tetris course.
 * Parser follows recomendation and specification which was described in nand2tetris book.
 * Do with that file whathever you want, I don't care.
 */

#include "./parser.h"
#include "../../../lib/cutest-1.5/CuTest.h"
void RunAllTests(void);

struct CInstruction** parser(char *sp){
	struct CInstruction** cInstructions = malloc(sizeof(struct CInstruction*) * 1000);
	int i = 0;
	bool hml = false;
	while((hml = hasMoreLines(sp))){
		struct CInstruction* cInst = (struct CInstruction*) malloc(sizeof (struct CInstruction));
		
		char *it = instructionType(sp);

		if(strcmp("C_INSTRUCTION", it) == 0){
			cInst->dest = NULL;
			cInst->comp = NULL;
			cInst->jump = NULL;

			if(isAssignement(sp)) cInst->dest = dest(sp);
			if(isAssignement(sp)) cInst->comp = comp(sp);
			if(isJump(sp)) cInst->jump = jump(sp);
		} 
		else if(strcmp("A_INSTRUCTION", it) == 0){
			if(isSymbol(sp)) cInst->symbol = symbolC(sp);
			if(isAddress(sp)) cInst->address = address(sp);
		}
		else if(strcmp("L_INSTRUCTION", it) == 0){

		} 
		else {
			exit(1);
		}

		*(cInstructions + i) = cInst;
		i++;	
		sp = advance(sp);
	}
	*(cInstructions + i) = NULL;
	
	return cInstructions;
}


bool hasMoreLines(char *sp){
	return *sp != '\0' && *sp != '\n';
}

char *advance(char *sp){
	while(*sp++ != '\n' && *sp != '\0');
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

/* C command */


char *dest(char* sp){
	char *destP = (char*) malloc(sizeof(char) * 10);

	int i = 0;
	while(*sp != '=' && *sp != '\0'){
		*(destP + i++) = *sp++;
	}
	*(destP + i++) = '\0';

	return destP;
};

char *comp(char* sp){
	char *compP = (char*) malloc(sizeof(char) * 10);

	int i = 0;
	while(*sp++ != '=' && *sp != '\0');
	while(*sp != '\n'){
		*(compP + i++) = *sp++;
	}
	*(compP + i++) = '\0';

	return compP;
}

char *jump(char* sp){
	char *jumpP = (char*) malloc(sizeof(char) * 10);

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
	*(jumpP + 3) = '\0';

	return jumpP;
}

bool isJump(char *sp){
	bool isJ = false;
	while(*sp != '\0' && *sp != '\n' && isJ == false){
		isJ = *sp == ';';
		sp++;
	}

	return isJ;
}

bool isAssignement(char *sp){
	bool isA = false;
	while(*sp != '\0' && *sp != '\n' && isA == false){
		isA = *sp == '=';
		sp++;
	}

	return isA;
}

/* A command */
char *address(char *sp){
	char *addressP = (char*) malloc(sizeof(char) * 10);
	int i = 0, j = 0;
	while(*(sp + i) != '\n' && *(sp + i) != '\0'){
		if(*(sp + i) != '@'){
			*(addressP + j) = *(sp + i);
			j++;
		}
		i++;
	}

	*(addressP + j) = '\0';

	return addressP;
}

char *symbolC(char *sp){
	char *symbolP = (char*) malloc(sizeof(char) * 10);
	int i = 0, j = 0;
	while(*(sp + i) != '\n' && *(sp + i) != '\0'){
		if(*(sp + i) != '@'){
			*(symbolP + j) = *(sp + i);
			j++;
		}
		i++;
	}

	*(symbolP + j) = '\0';

	return symbolP;
}

bool isSymbol(char *sp){
	sp++; // Skip @ char
	char isS = false;
	while(*sp != '\0' && *sp != '\n' && isS == false){
		isS = (*sp >= 'a' && *sp <= 'z') ||
			(*sp >= 'A' && *sp <= 'Z');

		sp++;
	}
	return isS;
}

bool isAddress(char *sp){
	sp++; // Skip @ char
	char isA = false;
	while(*sp != '\0' && *sp != '\n' && isA == false){
		isA = (*sp >= '0' && *sp <= '9');
		sp++;
	}
	return isA;
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
void isJumpTest(CuTest *tc);
void isJumpTest2(CuTest *tc);
void isAssignementTest(CuTest *tc);
void isAssignementTest2(CuTest *tc);

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
	SUITE_ADD_TEST(suite, isJumpTest);
	SUITE_ADD_TEST(suite, isJumpTest2);
	SUITE_ADD_TEST(suite, isAssignementTest);
	SUITE_ADD_TEST(suite, isAssignementTest2);

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

void isJumpTest(CuTest *tc){
	char *instruction = "0;JNE";

	bool expected = true;
	bool actual = isJump(instruction);
	CuAssertIntEquals(tc, expected, actual);
}

void isJumpTest2(CuTest *tc){
	char *instruction = "A=A+B";

	bool expected = false;
	bool actual = isJump(instruction);
	CuAssertIntEquals(tc, expected, actual);
}

void isAssignementTest(CuTest *tc){
	char *instruction = "A=A+B";

	bool expected = true;
	bool actual = isAssignement(instruction);
	CuAssertIntEquals(tc, expected, actual);
}

void isAssignementTest2(CuTest *tc){
	char *instruction = "0;JNE";

	bool expected = false;
	bool actual = isAssignement(instruction);
	CuAssertIntEquals(tc, expected, actual);
}

