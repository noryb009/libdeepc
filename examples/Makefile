# TODO: Improve.

CC := gcc
COMMAND := $(CC) -nostdlib -nostdinc -I ../libyaclib/ ../libyaclib/start.S

all: true false argc stdint

clean:
	rm -f true false argc stdint

true:
	$(COMMAND) -o true true.c

false:
	$(COMMAND) -o false false.c

argc:
	$(COMMAND) -o argc argc.c

stdint:
	$(COMMAND) -o stdint stdint.c

test: all
	./runTests
