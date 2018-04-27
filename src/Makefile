# TODO: Improve.

CC := gcc
CFLAGS := -O0 -g
COMMAND := $(CC) $(CFLAGS) -nostdlib -nostdinc -fno-builtin -I ../include/

OBJECTS = errno.o stdlib.o setjmp.o unistd.o syscall.o malloc.o string.o stdio.o ctype.o printf.o assert.o atoi.o start.o
DEPENDS = ${OBJECTS:.o=.d}

all: libyaclib.a

libyaclib.a: ${OBJECTS}
	ar rcs $@ $^

%.o: %.c
	$(COMMAND) -o $@ -c $<

clean:
	rm -f *.d *.o libyaclib.a
