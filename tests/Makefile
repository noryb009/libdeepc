# TODO: Improve.

CC := gcc
CFLAGS := -O0 -g
WARNINGS := -pedantic -Wall -Wextra -Wshadow -Wpointer-arith -Wstrict-prototypes -Wmissing-prototypes
COMMAND := $(CC) $(CFLAGS) $(WARNINGS) -nostdlib -nostdinc -fno-builtin -fstack-protector-all -I ../include/ -L ../src
LIBS := -l:libdeepc.a

single-file-tests = true false includes argc stdint stdarg assert assert-fail string strlen setjmp malloc chario echo ctype printf atoi sprintf qsort bsearch strdup atomic-flag mtx threads threads2 tls tss math iso646 time signal env file system search

.PHONY: all libdeepc clean $(single-file-tests)

all: libdeepc $(single-file-tests)

libdeepc:
	cd ../src/ && make all

clean:
	cd ../src && make clean
	rm -rf bin

bindir:
	mkdir bin || true

define make-single-target
  $1: bindir
	  $(COMMAND) -o bin/$1 $1-test.c $(LIBS)
endef

$(foreach element,$(single-file-tests),$(eval $(call make-single-target,$(element))))

test: all
	./runTests
