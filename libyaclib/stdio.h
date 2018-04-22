#pragma once

#include "stdarg.h"
#include "stddef.h"

#define EOF -1

int getchar();
int putchar(int c);

int puts(const char *s);

// TODO: unlocked_stdio

int vprintf(const char *format, va_list args);
int printf(const char *format, ...);
