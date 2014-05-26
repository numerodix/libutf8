all: libutf8.so test

CFLAGS=-g -Wall -Werror
CFLAGS=-g -fpic


test: test.c utf8.o
	gcc $(CFLAGS) -o $@ test.c utf8.o

libutf8.so: utf8.o
	gcc $(CFLAGS) -shared -o $@ utf8.o

utf8.o: utf8.c utf8.h
	gcc $(CFLAGS) -c -o $@ utf8.c

clean:
	rm -f *.o *.so test core
