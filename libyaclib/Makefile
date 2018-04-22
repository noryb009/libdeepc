# TODO: Improve.

CC := gcc
CFLAGS := -O0 -g

OBJECTS = libc.o errno.o stdlib.o setjmp.o
DEPENDS = ${OBJECTS:.o=.d}

all: libyaclib.a

libyaclib.a: ${OBJECTS}
	ar rcs $@ $^

%.o: %.cc
	$(CC) -nostdlib -nostdinc $(CFLAGS) -o $@ -c $<

clean:
	rm -f *.d *.o libyaclib.a
