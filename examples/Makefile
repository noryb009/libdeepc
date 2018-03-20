# TODO: Improve.

CC := gcc
COMMAND := $(CC) -nostdlib -nostdinc -I ../libyaclib/ ../libyaclib/start.S

all: true false argc

clean:
	rm -f true false argc

true:
	$(COMMAND) -o true true.c

false:
	$(COMMAND) -o false false.c

argc:
	$(COMMAND) -o argc argc.c

test: all
	./runTests
