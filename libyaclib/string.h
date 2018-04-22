#pragma once

#include "stddef.h"

void *memchr(const void *ptr, int ch, size_t count);
int memcmp(const void *lhs, const void *rhs, size_t count);
void *memset(void *dest, int ch, size_t count);
void *memcpy(void * restrict dest, const void * restrict src, size_t count);
void *memmove(void *dest, const void *src, size_t count);

char *strcpy(char * restrict dest, const char * restrict src);
char *strncpy(char * restrict dest, const char * restrict src, size_t count);
size_t strlen(const char *str);
char *strcat(char * restrict dest, const char * restrict src);
char *strncat(char * restrict dest, const char * restrict src, size_t count);

// TODO: String examination functions.
