# TODO: Improve.

CC := gcc
CFLAGS := -O0 -g
WARNINGS := -pedantic -Wall -Wextra -Wshadow -Wpointer-arith -Wstrict-prototypes -Wmissing-prototypes
COMMAND := $(CC) $(CFLAGS) $(WARNINGS) -nostdlib -nostdinc -fno-builtin -fstack-protector-all -I ../include/

GENERATED = ../include/__inttypes.h ../include/__limits.h ../include/__stdint.h
OBJECTS = errno.o stdlib.o setjmp.o unistd.o syscall.o syscall-asm.o malloc.o string.o stdio.o ctype.o printf.o assert.o atoi.o start.o atomic-flag.o atomic-flag-set.o mtx.o spinlock.o threads.o thread-bootstrap.o tls.o time.o signal.o signal-restorer.o fcntl.o file.o search.o
DEPENDS = ${OBJECTS:.o=.d}

all: libdeepc.a

libdeepc.a: ${OBJECTS}
	ar rcs $@ $^

%.o: %.c ${GENERATED}
	$(COMMAND) -o $@ -c $<

${GENERATED}: ../scripts/stdint.py
	cd ../scripts && ./stdint.py

clean:
	rm -f *.d *.o ${GENERATED} libdeepc.a
