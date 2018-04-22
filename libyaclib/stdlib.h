#pragma once

#include "stddef.h"
#include "stdinternaltypes.h"

//Termination.
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

typedef void __atexit_func(void);

int atexit(__atexit_func func);
int at_quick_exit(__atexit_func func);

_Noreturn void exit(int ret);
_Noreturn void quick_exit(int ret);
_Noreturn void _Exit(int ret);
_Noreturn void abort();

// Memory management.
void *malloc(size_t size);
void *calloc(size_t num, size_t size);
void *realloc(void *ptr, size_t new_size);
void free(void *ptr);
void *aligned_alloc(size_t alignment, size_t size);
