FILE := $(wildcard *.c)
OUTPUT_FILE := $(subst .c,.out,${FILE}) 

vm-translator.out:vm-translator.c
	gcc -g -std=c99 -o ${OUTPUT_FILE} vm-translator.h vm-translator.c

clean:
	rm -f ${OUTPUT_FILE}

