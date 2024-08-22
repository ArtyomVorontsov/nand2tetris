FILE := $(wildcard *.c)
OUTPUT_FILE := $(subst .c,.out,${FILE}) 
TEST_FLAG_DATA := if(argc > 1){ if(strcmp(argv[1], "--test") == 0){ RunAllTests(); return 0; }; printf("Not supported parameter(s) is(are) passed\\n"); return 1; }
TEST_FLAG = \/\/TEST_FLAG


${OUTPUT_FILE}:${FILE}
	rm -f a.hack 
	touch buffer.c
	cat ${FILE} > buffer.c
	sed -i 's/${TEST_FLAG}/${TEST_FLAG_DATA}/' buffer.c
	gcc -g -std=c99 -o ${OUTPUT_FILE} ./code/code.h ./code/code.c ./parser/parser.h ./parser/parser.c ../../AllTests.c ../../lib/cutest-1.5/CuTest.c buffer.c
	rm -f buffer.c

clean:
	rm -f ${OUTPUT_FILE}

