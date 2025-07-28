FILE := $(wildcard *.c)
FILE_H:= $(wildcard *.h)
OUTPUT_FILE := $(subst .c,.out,jack-compiler.c) 
CFLAGS = -D_BSD_SOURCE -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -lglib-2.0

jack-compiler.out: ${FILE_H} ${FILE}
	gcc -g -std=c99 -o ${OUTPUT_FILE} ${FILE_H} ${FILE} ${CFLAGS}


clean:
	rm -f ${OUTPUT_FILE}
