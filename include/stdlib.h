#pragma once

#include <stddef.h>
#include <stdinternaltypes.h>

//Termination.
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

typedef void __atexit_func(void);

int atexit(__atexit_func func);
int at_quick_exit(__atexit_func func);

_Noreturn void exit(int ret);
_Noreturn void quick_exit(int ret);
_Noreturn void _Exit(int ret);
_Noreturn void abort(void);

// Memory management.
void *malloc(size_t size);
void *calloc(size_t num, size_t size);
void *realloc(void *ptr, size_t new_size);
void free(void *ptr);
void *aligned_alloc(size_t alignment, size_t size);

// String conversions.
int atoi(const char *str);
long atol(const char *str);
long long atoll(const char *str);

long strtol(const char * restrict str, char ** restrict str_end, int base);
long long strtoll(const char * restrict str, char ** restrict str_end, int base);
unsigned long strtoul(const char * restrict str, char ** restrict str_end, int base);
unsigned long long strtoull(const char * restrict str, char ** restrict str_end, int base);

// Simple math.
int abs(int n);
long labs(long n);
long long llabs(long long n);

typedef struct {
  int quot;
  int rem;
} div_t;
typedef struct {
  long quot;
  long rem;
} ldiv_t;
typedef struct {
  long long quot;
  long long rem;
} lldiv_t;

div_t div(int x, int y);
ldiv_t ldiv(long x, long y);
lldiv_t lldiv(long long x, long long y);

// Algorithms.
void qsort(void *ptr, size_t count, size_t size, int (*comp)(const void *, const void *));
void *bsearch(
    const void *key,
    const void *ptr,
    size_t count,
    size_t size,
    int (*comp)(const void *, const void *));

// Environment functions.
extern char **environ;
char *getenv(const char *name);
int system(const char *command);
