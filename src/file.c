#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "file.h"

typedef enum {
  NONE,
  WRITE,
  READ,
  APPEND
} access_mode;

typedef struct {
  bool flag_b;
  bool flag_e;
  bool flag_u;
  bool flag_x;
  bool plus_flag;
  access_mode access;
} parse_mode_t;

static bool parse_mode(parse_mode_t *info, const char * mode, bool auto_u_flag) {
  info->flag_b = false;
  info->flag_e = false;
  info->flag_u = auto_u_flag;
  info->flag_x = false;
  info->plus_flag = false;
  info->access = NONE;
  for (; *mode != '\0'; ++mode) {
    switch (*mode) {
      case '+':
        if (info->plus_flag) {
          return false;
        }
        info->plus_flag = true;
        break;
      case 'b':
        if (info->flag_b) {
          return false;
        }
        info->flag_b = true;
        break;
      case 'e':
        if (info->flag_e) {
          return false;
        }
        info->flag_e = true;
        break;
      case 'u':
        if (info->flag_u || info->access == READ) {
          return false;
        }
        info->flag_u = true;
        break;
      case 'x':
        if (info->flag_x || info->access != WRITE) {
          return false;
        }
        info->flag_x = true;
        break;
      default:
        if (info->access != NONE) {
          return false;
        }
        switch (*mode) {
          case 'r':
            // Don't allow "ru", unless we injected the u flag (using fopen()).
            if (!auto_u_flag && info->flag_u) {
              return false;
            }
            info->access = READ;
            break;
          case 'w':
            info->access = WRITE;
            break;
          case 'a':
            info->access = APPEND;
            break;
        }
        break;
    }
  }
  if (info->access == NONE) {
    return false;
  }

  return true;
}

static int mode_to_flags(parse_mode_t *info) {
  int flags = 0;

  // Ignore flag_b, we don't do any conversions.
  if (info->flag_e) {
    flags |= O_CLOEXEC;
  }
  if (info->flag_x) {
    flags |= O_EXCL;
  }

  if (info->plus_flag) {
    flags |= O_RDWR;
  } else if (info->access == READ) {
    flags |= O_RDONLY;
  } else {
    flags |= O_WRONLY;
  }

  if (info->access != READ) {
    flags |= O_CREAT;
    if (info->access == APPEND) {
      flags |= O_APPEND;
    } else {
      flags |= O_TRUNC;
    }
  }
  return flags;
}

static int mode_to_mode_t(parse_mode_t *info) {
  mode_t mode = S_IWUSR | S_IRUSR;
  if (info->flag_u) {
    mode |=
      S_IWOTH | S_IROTH |
      S_IWGRP | S_IRGRP;
  }

  return mode;
}

FILE *fopen(const char *restrict filename, const char *restrict mode) {
  // Parse the mode and make sure it is valid.
  parse_mode_t info;
  if (!parse_mode(&info, mode, true)) {
    errno = EINVAL;
    return NULL;
  }

  // Allocate the file structure. We do this next because `open()` might
  // create or truncate a file.
  FILE *f = malloc(sizeof(FILE));
  if (f == NULL) {
    errno = ENOMEM;
    return NULL;
  }
  f->fd = -1;
  f->eof = 0;
  f->err = 0;
  f->pos = 0; // TODO: Non-zero on append?
  f->ungetc_buf_size = 0;
  f->ungetc_buf = 0;

  // TODO: Track f and possibly close on exit().

  const int flags = mode_to_flags(&info);
  const mode_t mode_arg = mode_to_mode_t(&info);
  f->fd = open(filename, flags, mode_arg);
  if (f->fd == -1) {
    free(f);
    return NULL;
  }

  return f;
}

int fclose(FILE *stream) {
  // TODO: Only flush output/update streams.
  if (fflush(stream) == EOF) {
    return EOF;
  }

  // TODO: Free buffer allocated by setbuf().

  bool success = close((uintptr_t)stream->fd) == -1;
  free(stream);
  if (!success) {
    return EOF;
  }
  return 0;
}

int fflush(FILE *stream) {
  // Nothing, until we support buffers.
  (void)stream;
  return 0;
}
