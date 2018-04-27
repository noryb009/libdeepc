#pragma once

#include <stdint.h>

typedef signed long ssize_t;

// TODO: Type declarations.

void *sbrk(intptr_t increment);
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
