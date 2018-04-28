#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "file.h"

static FILE stdin_obj = {0, false, 0, 0, 0, 0};
static FILE stdout_obj = {1, false, 0, 0, 0, 0};
static FILE stderr_obj = {2, false, 0, 0, 0, 0};

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

size_t fread(void * restrict buffer, size_t size, size_t count, FILE * restrict stream) {
  // TODO: Fix this function (and fwrite).
  if (stream->eof || stream->err) {
    return EOF;
  }
  if (size == 0 || count == 0) {
    return 0;
  }

  const ssize_t s = read(stream->fd, buffer, size * count);
  if (s <= 0) {
    if (s == 0) {
      stream->eof = true;
    } else {
      stream->err = -s;
    }
    return 0;
  }
  return count;
}
size_t fwrite(const void * restrict buffer, size_t size, size_t count, FILE * restrict stream) {
  if (stream->eof || stream->err) {
    return 0;
  }
  if (size == 0 || count == 0) {
    return 0;
  }

  const ssize_t s = write(stream->fd, buffer, size * count);
  if (s <= 0) {
    if (s == 0) {
      stream->eof = true;
    } else {
      stream->err = -s;
    }
    return 0;
  }
  return count;
}

int fgetc(FILE *stream) {
  unsigned char c;
  if (fread(&c, 1, 1, stream) != 1) {
    return EOF;
  }
  return c;
}

char *fgets(char * const restrict str, int count, FILE * restrict stream) {
  if (count < 0) {
    return str;
  }
  if (ferror(stream)) {
    *str = '\0'; // For safety.
    return NULL;
  }
  if (feof(stream)) {
    *str = '\0';
    return str;
  }


  char *i;
  const char * const str_end = str + count - 1;
  for (i = str; i != str_end;) {
    const int c = fgetc(stream);
    if (ferror(stream)) {
      *str = '\0'; // For safety.
      return NULL;
    }
    if (c == EOF) {
      break;
    }
    *i = c;
    ++i;
    --count;
    if (c == '\n') {
      break;
    }
  }
  *i = '\0';
  return str;
}

int getchar() {
  return fgetc(stdin);
}

int fputc(int ch, FILE *stream) {
  const unsigned char c = (unsigned char)ch;
  if (fwrite(&c, 1, 1, stream) != 1) {
    return EOF;
  }
  return c;
}

int putchar(int c) {
  return fputc(c, stdout);
}

int fputs(const char * restrict s, FILE * restrict stream) {
  const size_t len = strlen(s);
  {
    const ssize_t ret = fwrite(s, 1, len, stream);
    if (ret < len) {
      return EOF;
    }
  }
  {
    // Output newline.
    const int ret = fputc('\n', stream);
    if (ret != '\n') {
      return EOF;
    }
    return len + 1;
  }
}

int puts(const char *s) {
  return fputs(s, stdout);
}
