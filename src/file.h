#pragma once

#include <stdio.h>

struct FILE {
  unsigned int fd;
  unsigned int eof;
  unsigned int err;
  fpos_t pos;
  unsigned int ungetc_buf_size;
  unsigned char ungetc_buf;
};
