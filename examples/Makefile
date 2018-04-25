# TODO: Improve.

CC := gcc
CFLAGS := -O0 -g
COMMAND := $(CC) $(CFLAGS) -nostdlib -nostdinc -fno-builtin -I ../libyaclib/ -L ../libyaclib ../libyaclib/start.S
LIBS := -l:libyaclib.a

single-file-tests = true false argc stdint stdarg assert assert-fail string strlen setjmp malloc chario echo ctype printf atoi sprintf qsort bsearch

.PHONY: all libyaclib clean $(single-file-tests)

all: libyaclib $(single-file-tests)

libyaclib:
	cd ../libyaclib/ && make all

clean:
	cd ../libyaclib && make clean
	rm -f ${single-file-tests}

define make-single-target
  $1:
	  $(COMMAND) -o $1 $1.c $(LIBS)
endef

$(foreach element,$(single-file-tests),$(eval $(call make-single-target,$(element))))

test: all
	./runTests
