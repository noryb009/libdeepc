#pragma once

#include <__inttypes.h>
#include <stdint.h>

intmax_t strtoimax(const char * restrict str, char ** restrict str_end, int base);
uintmax_t strtoumax(const char * restrict str, char ** restrict str_end, int base);

typedef struct {
  intmax_t quot;
  intmax_t rem;
} imaxdiv_t;

intmax_t imaxabs(intmax_t n);
imaxdiv_t imaxdiv(intmax_t x, intmax_t y);
