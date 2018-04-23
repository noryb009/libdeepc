#include "ctype.h"
#include "errno.h"
#include "limits.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"

typedef enum {
  NEG = -1,
  ERR = 0,
  POS = 1,
} STRTO_RET;

static STRTO_RET atoi_start(const char * restrict * restrict str) {
  const char *s = *str;

  // Eat leading whitespace.
  while (isspace(*s)) {
    ++s;
  }

  // Optional +/-.
  STRTO_RET ret = POS;
  switch (*s) {
    case '-':
      ret = NEG;
      // Fallthrough.
    case '+':
      ++s;
      break;
    default:
      // Nothing.
      break;
  }

  *str = s;
  return ret;
}

static STRTO_RET strto_start(const char * restrict * restrict str, int *base) {
  if (*base < 0 || *base == 1 || *base > 36) {
    errno = EINVAL;
    return ERR;
  }

  const STRTO_RET ret = atoi_start(str);
  const char *s = *str;

  // '0' prefix means octal, '0x' means hex.
  if (*s == '0') {
    ++s;
    if ((*base == 0 || *base == 16) && (*s == 'x' || *s == 'X')) {
      ++s;
      *base = 16;
    } else if (*base == 0) {
      *base = 8;
    }
  } else if (*base == 0) {
    *base = 10;
  }

  *str = s;
  return ret;
}

static int to_base_n(int base, char c) {
  if (base <= 10) {
    if (c < '0' || ('0' + base) <= c) {
      return -1;
    }
    return c - '0';
  } else {
    if (isdigit(c)) {
      return c - '0';
    } else if ('a' <= c && c < ('a' - 10 + base)) {
      return c - 'a' + 10;
    } else if ('A' <= c && c < ('A' - 10 + base)) {
      return c - 'A' + 10;
    } else {
      return -1;
    }
  }
}

/* For this example, I'm going to use bytes (-128 - 127) and base 10, since
 * they are easy. Below, MAX is 127, even for negative numbers.
 * Whenever we have read all characters, finish the algorithm (so it is assumed
 * there are always more values to read).
 * Step 1) We read everything up until abs(MAX/base) = 12 like normal.
 *   For example, if we have 8 or 11, we can read an extra digit,
 *   but if we have 12 or 110, we can't read an extra digit.
 *   At the end of this step, we have a value in 12-19 or 100-119
 * Step 2) If our value is equal to abs(MAX/base), and our next character
 *   is not greater than abs(MAX%base), we can read one more character.
 *   Otherwise, go to step 3.
 *   For example, if we are working with positive bytes in base 10,
 *   and our current value is 12, we can read 0-7, but not 8-9.
 * Step 3) If there is another character, reject because it will overflow.
 */

// TODO: Combine this and the other one?
static void read_pos_int(const uintmax_t max, const int base, uintmax_t *result, const char * restrict * restrict str, bool *overflow) {
  const char *s = *str;
  uintmax_t n = 0;
  *overflow = false;
  const uintmax_t max_div10 = (max / base);
  const uintmax_t max_mod10 = (max % base);
  for (;;) {
    const int d = to_base_n(base, *s);
    if (d == -1) {
      break;
    }
    ++s;
    if (n < max_div10 || (n == max_div10 && d <= max_mod10)) {
      /* Step 1 (or 2). */
      n *= base;
      n += d;
    } else {
      /* Step 3. */
      *overflow = true;
      n = max;
      while (to_base_n(base, *s) != -1) {
        ++s;
      }
      break;
    }
  }
  *result = n;
  *str = s;
}

// This is the same as read_pos_int, except max_div_10 is negative.
// I believe this is needed since there's a bunch of undefined behaviour
// related to casting between signed and unsigned numbers, and I
// couldn't find an efficient way to cover all cases with one function.
static void read_neg_int(const intmax_t max, const int base, intmax_t *result, const char * restrict * restrict str, bool *overflow) {
  const char *s = *str;
  intmax_t n = 0;
  *overflow = false;
  const intmax_t max_div10 = (max / base);
  const intmax_t max_mod10 = (max % base) * -1;
  for (;;) {
    const int d = to_base_n(base, *s);
    if (d == -1) {
      break;
    }
    ++s;
    if (n > max_div10 || (n == max_div10 && d <= max_mod10)) {
      /* Step 1 (or 2). */
      n *= base;
      n -= d;
    } else {
      /* Step 3. */
      *overflow = true;
      n = max;
      while (to_base_n(base, *s) != -1) {
        ++s;
      }
      break;
    }
  }
  *result = n;
  *str = s;
}

#define ATOI(MAX, MIN) \
  STRTO_RET sign = atoi_start(&str); \
  \
  bool overflow; \
  if (sign == POS) { \
    uintmax_t n; \
    read_pos_int(MAX, 10, &n, &str, &overflow); \
    if (overflow) { \
      return 0; \
    } \
    return n; \
  } else { \
    intmax_t n; \
    read_neg_int(MIN, 10, &n, &str, &overflow); \
    if (overflow) { \
      return 0; \
    } \
    return n; \
  }

int atoi(const char *str) {
  ATOI(INT_MAX, INT_MIN)
}

long atol(const char *str) {
  ATOI(LONG_MAX, LONG_MIN)
}

long long atoll(const char *str) {
  ATOI(LLONG_MAX, LLONG_MIN)
}

#define STRTOL(MAX, MIN) \
  const STRTO_RET sign = strto_start(&str, &base); \
  bool overflow; \
  uintmax_t n_pos = 0; \
  intmax_t n_neg = 0; \
  switch (sign) { \
    case ERR: \
      return 0; \
    case POS: \
      read_pos_int(MAX, base, &n_pos, &str, &overflow); \
      break; \
    case NEG: \
      read_neg_int(MIN, base, &n_neg, &str, &overflow); \
      break; \
  } \
  if (str_end) { \
    *str_end = (char *)str; \
  } \
  if (overflow) { \
    errno = ERANGE; \
  } \
  if (sign == POS) { \
    return n_pos; \
  } else { \
    return n_neg; \
  }

long strtol(const char * restrict str, char ** restrict str_end, int base) {
  STRTOL(LONG_MAX, LONG_MIN)
}

long long strtoll(const char * restrict str, char ** restrict str_end, int base) {
  STRTOL(LLONG_MAX, LLONG_MIN)
}

intmax_t strtoimax(const char * restrict str, char ** restrict str_end, int base) {
  STRTOL(INTMAX_MAX, INTMAX_MIN)
}

#define STRTOUL(MAX) \
  const STRTO_RET sign = strto_start(&str, &base); \
  bool overflow; \
  uintmax_t n_pos = 0; \
  switch (sign) { \
    case ERR: \
      return 0; \
    case POS: \
      /* Fallthrough. */ \
    case NEG: \
      read_pos_int(MAX, base, &n_pos, &str, &overflow); \
      break; \
  } \
  if (str_end) { \
    *str_end = (char *)str; \
  } \
  if (overflow) { \
    errno = ERANGE; \
  } else if (sign == NEG) { \
    n_pos = -n_pos; \
  } \
  return n_pos;

unsigned long strtoul(const char * restrict str, char ** restrict str_end, int base) {
  STRTOUL(ULONG_MAX)
}

unsigned long long strtoull(const char * restrict str, char ** restrict str_end, int base) {
  STRTOUL(ULLONG_MAX)
}

uintmax_t strtoumax(const char * restrict str, char ** restrict str_end, int base) {
  STRTOUL(UINTMAX_MAX)
}
