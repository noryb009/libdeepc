# TODO: Improve.

CC := gcc
CFLAGS := -O0 -g
WARNINGS := -pedantic -Wall -Wextra -Wshadow -Wpointer-arith -Wstrict-prototypes -Wmissing-prototypes
COMMAND := $(CC) $(CFLAGS) $(WARNINGS) -nostdlib -nostdinc -fno-builtin -fno-stack-protector -I ../include/

OBJECTS = errno.o stdlib.o setjmp.o unistd.o syscall.o malloc.o string.o stdio.o ctype.o printf.o assert.o atoi.o start.o atomic_flag.o atomic_flag_set.o mtx.o spinlock.o threads.o thread_bootstrap.o tls.o
DEPENDS = ${OBJECTS:.o=.d}

all: libyaclib.a

libyaclib.a: ${OBJECTS}
	ar rcs $@ $^

%.o: %.c
	$(COMMAND) -o $@ -c $<

clean:
	rm -f *.d *.o libyaclib.a
