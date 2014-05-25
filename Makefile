all: test

CFLAGS=-Wall -Werror
CFLAGS=


test: test.c utf8.c utf8.h
	gcc $(CFLAGS) -o $@ test.c utf8.c

clean:
	rm -f *.o *.so test
