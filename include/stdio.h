#pragma once

#include <stdarg.h>
#include <stddef.h>

// TODO: This assumes char is unsigned.
#define EOF -1
typedef size_t fpos_t;

typedef struct FILE FILE;

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

// Error handling.
int feof(FILE *stream);
int ferror(FILE *stream);
void clearerr(FILE *stream);

// Direct IO.
size_t fread(void * restrict buffer, size_t size, size_t count, FILE * restrict stream);
size_t fwrite(const void * restrict buffer, size_t size, size_t count, FILE * restrict stream);

// Unformatted IO.
int fgetc(FILE *stream);
#define getc(s) fgetc(s)
int getchar(void);
int fputc(int ch, FILE *stream);
#define putc(c, s) fputc(c, s)
int putchar(int c);

char *fgets(char * restrict str, int count, FILE * restrict stream);
int fputs(const char * restrict s, FILE * restrict stream);
int puts(const char *s);

// TODO: ungetc.

// Formatted IO.
int vprintf(const char * restrict format, va_list args);
int vfprintf(FILE * restrict stream, const char * restrict format, va_list args);
int vsprintf(char * restrict buffer, const char * restrict format, va_list args);
int vsnprintf(char * restrict buffer, size_t buf_size, const char * restrict format, va_list args);

int printf(const char * restrict format, ...);
int fprintf(FILE * restrict stream, const char * restrict format, ...);
int sprintf(char * restrict buffer, const char * restrict format, ...);
int snprintf(char * restrict buffer, size_t buf_size, const char * restrict format, ...);
