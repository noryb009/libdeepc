# TODO: Improve.

CC := gcc
COMMAND := $(CC) -nostdlib -nostdinc -I ../libyaclib/ ../libyaclib/start.S

all:
	$(COMMAND) -o true true.c
	$(COMMAND) -o false false.c

test: all
	./runTests
