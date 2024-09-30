FILE := $(wildcard *.c)
OUTPUT_FILE := $(subst .c,.out,${FILE}) 

vm-translator.out:vm-translator.c ./parser/parser.c ./parser/parser.h ./code-writer/code-writer.h ./code-writer/code-writer.c
	gcc -g -std=c99 -o ${OUTPUT_FILE} vm-translator.h vm-translator.c ./parser/parser.h ./parser/parser.c ./code-writer/code-writer.h ./code-writer/code-writer.c -D_BSD_SOURCE

clean:
	rm -f ${OUTPUT_FILE}


 

