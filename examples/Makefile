# TODO: Improve.

CC := gcc
CFLAGS := -O0 -g
COMMAND := $(CC) $(CFLAGS) -nostdlib -nostdinc -I ../libyaclib/ -L ../libyaclib ../libyaclib/start.S
LIBS := -l:libyaclib.a

all: libyaclib true false argc stdint stdarg assert assert-fail

libyaclib:
	cd ../libyaclib/ && make all

clean:
	cd ../libyaclib && make clean
	rm -f true false argc stdint stdarg assert assert-fail

true:
	$(COMMAND) -o true true.c $(LIBS)

false:
	$(COMMAND) -o false false.c $(LIBS)

argc:
	$(COMMAND) -o argc argc.c $(LIBS)

stdint:
	$(COMMAND) -o stdint stdint.c $(LIBS)

stdarg:
	$(COMMAND) -o stdarg stdarg.c $(LIBS)

assert:
	$(COMMAND) -o assert assert.c $(LIBS)

assert-fail:
	$(COMMAND) -o assert-fail assert-fail.c $(LIBS)

test: all
	./runTests
