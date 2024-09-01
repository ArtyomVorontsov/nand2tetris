FILE := $(wildcard *.c)
OUTPUT_FILE := $(subst .c,.out,${FILE}) 

assembler.out:assembler.c ./parser/parser.c  ./code/code.c
	gcc -g -std=c99 -o ${OUTPUT_FILE} ./code/code.h ./code/code.c ./parser/parser.h ./parser/parser.c ../../AllTests.c ../../lib/cutest-1.5/CuTest.c assembler.h assembler.c

clean:
	rm -f ${OUTPUT_FILE}

