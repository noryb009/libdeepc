#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "unistd.h"

static FILE stdin_obj = {0, false, 0, 0};
static FILE stdout_obj = {1, false, 0, 0};
static FILE stderr_obj = {2, false, 0, 0};

FILE *stdin  = &stdin_obj;
FILE *stdout = &stdout_obj;
FILE *stderr = &stderr_obj;

int feof(FILE *stream) {
  return stream->eof;
}

int ferror(FILE *stream) {
  return stream->err;
}

void clearerr(FILE *stream) {
  stream->eof = false;
  stream->err = 0;
}

int fgetc(FILE *stream) {
  unsigned char buf;
  const ssize_t s = read(stream->fd, &buf, 1);
  if (s <= 0) {
    return EOF;
  }
  return buf;
}

int getchar() {
  return fgetc(stdin);
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
  if (ret < len) {
    return ret;
  }
  // Output newline.
  const char c = '\n';
  const ssize_t ret_newline = write(1, &c, 1);
  if (ret_newline < 0) {
    return EOF;
  }
  return ret + ret_newline;
}
