FILE := $(wildcard *.c)
FILE_H:= $(wildcard *.h)
OUTPUT_FILE := $(subst .c,.out,jack-analyzer.c) 

jack-analyzer.out: ${FILE_H} ${FILE}
	gcc -g -std=c99 -o ${OUTPUT_FILE} ${FILE_H} ${FILE} -D_BSD_SOURCE 
clean:
	rm -f ${OUTPUT_FILE}


 

