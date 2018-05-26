# TODO: Improve.

CC := gcc
CFLAGS := -O0 -g
WARNINGS := -pedantic -Wall -Wextra -Wshadow -Wpointer-arith -Wstrict-prototypes -Wmissing-prototypes
COMMAND := $(CC) $(CFLAGS) $(WARNINGS) -nostdlib -nostdinc -fno-builtin -fstack-protector-all -I ../include/

OBJECTS = errno.o stdlib.o setjmp.o unistd.o syscall.o syscall-asm.o malloc.o string.o stdio.o ctype.o printf.o assert.o atoi.o start.o atomic-flag.o atomic-flag-set.o mtx.o spinlock.o threads.o thread-bootstrap.o tls.o time.o
DEPENDS = ${OBJECTS:.o=.d}

all: libyaclib.a

libyaclib.a: ${OBJECTS}
	ar rcs $@ $^

%.o: %.c
	$(COMMAND) -o $@ -c $<

clean:
	rm -f *.d *.o libyaclib.a
