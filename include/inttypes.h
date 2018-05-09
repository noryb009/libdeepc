#pragma once

#include <stdint.h>

intmax_t strtoimax(const char * restrict str, char ** restrict str_end, int base);
uintmax_t strtoumax(const char * restrict str, char ** restrict str_end, int base);
