#pragma once

#include "stdarg.h"
#include "stddef.h"

#define EOF -1
typedef size_t fpos_t;

typedef struct {
  unsigned int fd;
  unsigned int eof:1;
  unsigned int err:12;
  fpos_t pos;
} FILE;

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

// Error handling.
int feof(FILE *stream);
int ferror(FILE *stream);
void clearerr(FILE *stream);

// Direct IO.
size_t fread(void * restrict buffer, size_t size, size_t count, FILE * restrict stream);
size_t fwrite(void * restrict buffer, size_t size, size_t count, FILE * restrict stream);

int fgetc(FILE *stream);
#define getc(s) fgetc(s)

int fgetc(FILE *stream);

int getchar();
int putchar(int c);

int puts(const char *s);

// Formatted IO.
int vprintf(const char * restrict format, va_list args);
int vsprintf(char * restrict buffer, const char * restrict format, va_list args);
int vsnprintf(char * restrict buffer, size_t buf_size, const char * restrict format, va_list args);

int printf(const char * restrict format, ...);
int sprintf(char * restrict buffer, const char * restrict format, ...);
int snprintf(char * restrict buffer, size_t buf_size, const char * restrict format, ...);
