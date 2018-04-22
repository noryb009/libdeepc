#include "stdio.h"
#include "string.h"
#include "unistd.h"

int getchar() {
  unsigned char buf;
  const ssize_t s = read(0, &buf, 1);
  if (s <= 0) {
    return EOF;
  }
  return buf;
}

int putchar(int c) {
  const unsigned char buf = (unsigned char)c;
  const ssize_t s = write(1, &buf, 1);
  if (s <= 0) {
    return EOF;
  }
  return buf;
}

int puts(const char *s) {
  const size_t len = strlen(s);
  const ssize_t ret = write(1, s, len);
  if (ret < 0) {
    return EOF;
  }
  return ret;
}
