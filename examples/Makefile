# TODO: Improve.

CC := gcc
CFLAGS := -O0 -g
WARNINGS := -pedantic -Wall -Wextra -Wshadow -Wpointer-arith -Wstrict-prototypes -Wmissing-prototypes
COMMAND := $(CC) $(CFLAGS) $(WARNINGS) -nostdlib -nostdinc -fno-builtin -I ../include/ -L ../src
LIBS := -l:libyaclib.a

single-file-tests = true false argc stdint stdarg assert assert-fail string strlen setjmp malloc chario echo ctype printf atoi sprintf qsort bsearch strdup atomic_flag

.PHONY: all libyaclib clean $(single-file-tests)

all: libyaclib $(single-file-tests)

libyaclib:
	cd ../src/ && make all

clean:
	cd ../src && make clean
	rm -f ${single-file-tests}

define make-single-target
  $1:
	  $(COMMAND) -o $1 $1.c $(LIBS)
endef

$(foreach element,$(single-file-tests),$(eval $(call make-single-target,$(element))))

test: all
	./runTests
