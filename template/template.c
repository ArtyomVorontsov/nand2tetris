#include "../lib/cutest-1.5/CuTest.h"
#include <string.h>
#include <stdio.h>
void RunAllTests(void);

int main(int argc, char *argv[]){
	TEST_FLAG

	return 0;
}

int foo(int i){
	return 0;
}

// Tests
void Test1(CuTest *tc);

CuSuite *exercise4_chapter13_getSuite(void){
	CuSuite *suite = CuSuiteNew();

	SUITE_ADD_TEST(suite , Test1);

	return suite;
}

// happy

void Test1(CuTest *tc){
	int input = 1;
	int expected = 1;

	int actual = foo(input);

	CuAssertIntEquals(tc, expected, actual);
}
