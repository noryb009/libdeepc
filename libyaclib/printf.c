#include "errno.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "unistd.h"

typedef enum {
  ERR = 0,
  END = 1,
  OK = 2,
} VPRINT_RET;

static VPRINT_RET vprintf_output(ssize_t rc, size_t expected, size_t *ret) {
  if (rc < 0) {
    return ERR;
  }
  *ret += rc;
  if (rc < expected) {
    return END;
  }
  return OK;
}

static VPRINT_RET vprintf_percent(const char **format, va_list args, size_t *ret) {
  for (;;) {
    const char c = **format;
    if (c == '\0') {
      errno = EINVAL;
      return ERR;
    }
    ++(*format);

    ssize_t r;
    switch (c) {
      case '%':
        {
          const char c = '%';
          const ssize_t rc = write(1, &c, 1);
          return vprintf_output(rc, 1, ret);
        }
      case 'c':
        {
          const unsigned char c = (unsigned char)va_arg(args, int);
          const ssize_t rc = write(1, &c, 1);
          return vprintf_output(rc, 1, ret);
        }
      case 's':
        {
          const char *s = va_arg(args, const char *);
          const size_t len = strlen(s);
          const ssize_t rc = write(1, s, len);
          return vprintf_output(rc, len, ret);
        }
      default:
        errno = EINVAL;
        return ERR;
    }
  }
}

int vprintf(const char *format, va_list args) {
  size_t ret = 0;
  const char *start_literal = format;
  for (;;) {
    const char c = *format;
    if (c == '\0') {
      break;
    }

    switch (c) {
      case '%':
        if (start_literal != format) {
          const size_t size = format - start_literal;
          const size_t written = write(1, start_literal, size);
          // Check for errors.
          if (written < 0) {
            return written;
          }
          ret += written;
          if (written < size) {
            // Couldn't write everything, stop now.
            return ret;
          }
        }
        ++format;
        switch(vprintf_percent(&format, args, &ret)) {
          case ERR:
            return -1;
          case END:
            return ret;
          case OK:
            break;
            // Continue.
        }
        start_literal = format;
      default:
        // Next input char, this will be output later using start_literal.
        ++format;
    }
  }
  if (start_literal != format) {
    const size_t size = format - start_literal;
    const size_t written = write(1, start_literal, size);
    if (written < 0) {
      return written;
    }
    ret += written;
  }
  return ret;
}

int printf(const char *format, ...) {
  va_list args;
  va_start(args, format);
  int ret = vprintf(format, args);
  va_end(args);
  return ret;
}
