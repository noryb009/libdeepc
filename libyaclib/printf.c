#include "assert.h"
#include "errno.h"
#include "limits.h"
#include "stdarg.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"

typedef enum {
  OK = 0,
  END = 1,
  ERR = 2,
} VPRINT_RET;

typedef enum {
  LENGTH_NONE = 0,
  LENGTH_hh,
  LENGTH_h,
  LENGTH_l,
  LENGTH_ll,
  LENGTH_j,
  LENGTH_z,
  LENGTH_t,
  LENGTH_L,
} VPRINTF_LENGTH;

typedef enum {
  CONV_d,
  CONV_o,
  CONV_u,
  CONV_x,
  CONV_X,
  // TODO: f, e, g, a
  CONV_c,
  CONV_s,
  CONV_p,
  CONV_n,
  CONV_percent,
} VPRINTF_CONV;

struct vprintf_info;
typedef struct vprintf_info vprintf_info;
typedef void (*write_fn_t)(vprintf_info *info, const void *buf, size_t count);

struct vprintf_info {
  // Number of bytes written so far.
  size_t written;
  // Current return code.
  VPRINT_RET rc;
  // Write function.
  write_fn_t write_fn;

  // If writing to a file, the file descriptor.
  int fd;

  // If writing to a string, the string and its length.
  char * restrict buffer;
  size_t buf_size;
};

static void write_fd(vprintf_info *info, const void *buf, const size_t size) {
  if (size == 0) {
    return;
  }

  const ssize_t written = write(info->fd, buf, size);
  // Check for errors.
  if (written < 0) {
    errno = -written;
    info->rc = ERR;
    return;
  }
  info->written += written;
  if (written < size) {
    // Couldn't write everything, stop now.
    info->rc = END;
  }
}

static void write_str(vprintf_info *info, const void *buf, const size_t size) {
  info->written += size;
  if (info->buf_size == 0 || size == 0) {
    return;
  }
  // Copy as much of the string that can fit in the buffer.
  const size_t copy = (size < info->buf_size) ? size : info->buf_size;
  memcpy(info->buffer, buf, copy);
  // Update the buffer and the remaining size.
  info->buffer += copy;
  info->buf_size -= copy;
}

typedef struct {
  // Flags.
  unsigned int flag_minus:1;
  unsigned int flag_plus:1;
  unsigned int flag_space:1;
  unsigned int flag_zero:1;
  unsigned int flag_number:1;

  // Width.
  bool width_set;
  int width;

  // Precision.
  bool precision_set;
  int precision;

  // Length modifier.
  VPRINTF_LENGTH length;

  // Conversion specifier.
  VPRINTF_CONV conv;
} vprint_state;

#define LOG10_T() \
    int l = 0; \
    for (; n != 0; ++l) { \
      n /= 10; \
    } \
    return l;

static int log10_intmax(intmax_t n) {
  LOG10_T()
}
static int log10_uintmax(uintmax_t n) {
  LOG10_T()
}
#undef LOG10_T

static void vprintf_padding(vprintf_info *info, const char p, int len) {
  if (len <= 0) {
    return;
  }
  char str[64];
  memset(str, p, 64);
  while (len > 0) {
    const int l = (len >= 64) ? 64 : len;
    len -= l;

    info->write_fn(info, str, l);
    if (info->rc != OK) {
      return;
    }
  }
}

static void vprintf_output_int(vprintf_info *info, intmax_t val, vprint_state *s) {
  // Figure out some info from the state.
  const int min_precision = s->precision_set ? s->precision : 1;
  const int min_width = s->width_set ? s->width : 0;
  const char * const sign = (val < 0) ? "-" : (s->flag_plus ? "+" : (s->flag_space ? " " : ""));
  const int sign_len = (*sign == '\0') ? 0 : 1;

  // Figure out the precision.
  const int num_precision = log10_intmax(val);
  int precision = (num_precision > min_precision ? num_precision : min_precision);
  if ((s->flag_zero && !s->precision_set && !s->flag_minus) && min_width > precision) {
    // The entire width (except the sign) should be filled with 0s.
    precision = min_width - sign_len;
  }
  
  // Figure out the padding length.
  const int space_padding_len = (min_width < precision + sign_len) ? 0 : (min_width - precision - sign_len);

  // Beginning padding.
  if (!s->flag_minus) {
    vprintf_padding(info, ' ', space_padding_len);
    if (info->rc != OK) {
      return;
    }
  }

  if (sign_len) {
    info->write_fn(info, sign, 1);
    if (info->rc != OK) {
      return;
    }
  }

  // Zero padding.
  {
    const int pad_zeros = precision - num_precision;
    vprintf_padding(info, '0', pad_zeros);
    if (info->rc != OK) {
      return;
    }
  }

  // Output actual number.
  {
    const int buf_len = 32;
    // We calculate digits in reverse order, since it's easier.
    // We place them into this buffer, then output the buffer at the end.
    char buf[buf_len];
    // Make sure the buffer can hold anything (with spare room).
    static_assert(UINTMAX_MAX == UINT64_MAX, "intmax_t too big for printf");

    int cur = buf_len;
    while (val != 0) {
      const int n = val % 10;
      val /= 10;
      const int n_abs = (n < 0 ? -n : n);
      --cur;
      buf[cur] = n_abs + '0';
    }
    const int output_len = buf_len - cur;
    info->write_fn(info, buf + cur, output_len);
    if (info->rc != OK) {
      return;
    }
  }

  // End padding.
  if (s->flag_minus) {
    vprintf_padding(info, ' ', space_padding_len);
    if (info->rc != OK) {
      return;
    }
  }
}

static void vprintf_output_uint(vprintf_info *info, uintmax_t val, vprint_state *s) {
  // TODO
  return;
}

// TODO: Split this function up into helpers.
static void vprintf_percent(vprintf_info *info, const char * restrict * restrict format, va_list args) {
  vprint_state state = {0};

  for (int stage = 0; stage < 5; /* Updated inside. */) {
    const char c = **format;
    if (c == '\0') {
      errno = EINVAL;
      info->rc = ERR;
      return;
    }
    ++(*format);

    // Flags.
    if (stage == 0) {
      switch (c) {
        case '-':
          state.flag_minus = 1;
          break;
        case '+':
          state.flag_plus = 1;
          break;
        case ' ':
          state.flag_space = 1;
          break;
        case '0':
          state.flag_zero = 1;
          break;
        case '#':
          state.flag_number = 1;
          break;
        default:
          // Done this stage.
          ++stage;
      }
    }

    // Width.
    if (stage == 1) {
      if (c == '*') {
        state.width = va_arg(args, int);
        state.width_set = true;
        if (state.width < 0) {
          state.flag_minus = 1;
          state.width = -state.width;
        }
      } else if ('1' <= c && c <= '9') {
        state.width_set = true;

        // Unread the first digit.
        --(*format);
        char *r;
        const long precision = strtol(*format, &r, 10);
        *format = r;
        if (precision > INT_MAX) {
          errno = ERANGE;
          info->rc = ERR;
          return;
        }
        state.width = precision;
      } else {
        // Skip this stage.
        ++stage;
      }
    }

    // Precision.
    if (stage == 2) {
      if (c == '.') {
        state.precision_set = true;
        const char next = **format;
        if (next == '*') {
          // Skip the star.
          ++(*format);
          state.precision = va_arg(args, int);
          if (state.precision < 0) {
            // As if nothing was specified.
            state.precision_set = false;
          }
        } else if ('0' <= next && next <= '9') {
          // Don't decrease format, since it points to the period.
          char *r;
          const long width = strtol(*format, &r, 10);
          *format = r;
          if (width > INT_MAX) {
            errno = ERANGE;
            info->rc = ERR;
            return;
          }
          state.precision = width;
        } else {
          state.precision = 0;
        }
      } else {
        // Skip this stage.
        ++stage;
      }
    } 

    // Length modifer.
    if (stage == 3) {
      switch (c) {
        case 'h':
          if (**format == 'h') {
            ++(*format);
            state.length = LENGTH_hh;
          } else {
            state.length = LENGTH_h;
          }
          break;
        case 'l':
          if (**format == 'l') {
            ++(*format);
            state.conv = LENGTH_ll;
          } else {
            state.conv = LENGTH_l;
          }
          break;
        case 'j':
          state.conv = LENGTH_j;
          break;
        case 'z':
          state.conv = LENGTH_z;
          break;
        case 't':
          state.conv = LENGTH_t;
          break;
        case 'L':
          state.conv = LENGTH_L;
          break;
        default:
          // Skip this stage.
          ++stage;
      }
    }

    // Conversion.
    if (stage == 4) {
      // Last stage, we need to find one (and only one) character.
      ++stage;

      switch (c) {
        case 'd':
        case 'i':
          state.conv = CONV_d;
          break;
        case 'o':
          state.conv = CONV_o;
          break;
        case 'u':
          state.conv = CONV_u;
          break;
        case 'x':
          state.conv = CONV_x;
          break;
        case 'X':
          state.conv = CONV_X;
          break;
        case 'c':
          state.conv = CONV_c;
          break;
        case 's':
          state.conv = CONV_s;
          break;
        case 'P':
          state.conv = CONV_p;
          break;
        case 'n':
          state.conv = CONV_n;
          break;
        case '%':
          state.conv = CONV_percent;
          break;
        default:
          errno = EINVAL;
          info->rc = ERR;
          return;
      }
    }
  }

  // Parse the state we've created.
  switch (state.conv) {
    case CONV_percent:
      {
        const char c = '%';
        info->write_fn(info, &c, 1);
      }
      break;
    case CONV_c:
      {
        const unsigned char c = (unsigned char)va_arg(args, int);
        info->write_fn(info, &c, 1);
      }
      break;
    case CONV_s:
      {
        const char *s = va_arg(args, const char *);
        const size_t str_len = strlen(s);
        const size_t len =
          (state.precision_set && (state.precision < str_len)
           ? state.precision
           : str_len);
        info->write_fn(info, s, len);
      }
      break;
    case CONV_d:
      {
        intmax_t m;
        switch (state.length) {
          case LENGTH_hh:
            m = (signed char)va_arg(args, int);
            break;
          case LENGTH_h:
            m = (short)va_arg(args, int);
            break;
          case LENGTH_l:
            m = va_arg(args, long);
            break;
          case LENGTH_ll:
            m = va_arg(args, long long);
            break;
          case LENGTH_j:
            m = va_arg(args, intmax_t);
            break;
          case LENGTH_z:
            m = va_arg(args, ssize_t);
            break;
          case LENGTH_t:
            m = va_arg(args, ptrdiff_t);
            break;
          case LENGTH_NONE:
          case LENGTH_L:
            m = va_arg(args, int);
            break;
        }
        vprintf_output_int(info, m, &state);
      }
      break;
    case CONV_o:
    case CONV_u:
    case CONV_x:
    case CONV_X:
      {
        uintmax_t m;
        switch (state.length) {
          case LENGTH_hh:
            m = (unsigned char)va_arg(args, unsigned int);
            break;
          case LENGTH_h:
            m = (unsigned short)va_arg(args, unsigned int);
            break;
          case LENGTH_l:
            m = va_arg(args, unsigned long);
            break;
          case LENGTH_ll:
            m = va_arg(args, unsigned long long);
            break;
          case LENGTH_j:
            m = va_arg(args, uintmax_t);
            break;
          case LENGTH_z:
            m = va_arg(args, size_t);
            break;
          case LENGTH_t:
            m = va_arg(args, size_t);
            break;
          case LENGTH_NONE:
          case LENGTH_L:
            m = va_arg(args, unsigned int);
            break;
        }
        vprintf_output_uint(info, m, &state);
      }
      break;
    case CONV_p:
      {
        const void *s = va_arg(args, void *);
        // We either output "(nil)" or "0x1234".

        if (s == NULL) {
          info->write_fn(info, "(nil)", 5);
          break;
        }

        // Set the state to output '0x1234'.
        state.flag_minus = 0;
        state.flag_plus = 0;
        state.flag_space = 0;
        state.flag_zero = 0;
        state.flag_number = 1;
        state.conv = CONV_x;
        state.precision_set = false;
        state.width_set = false;

        const uintmax_t s2 = (uintmax_t)s;
        vprintf_output_uint(info, s2, &state);
      }
      break;
    case CONV_n:
      {
        void *s = va_arg(args, void *);
        switch (state.length) {
          case LENGTH_hh:
            *((signed char *)s) = info->written;
            break;
          case LENGTH_h:
            *((short *)s) = info->written;
            break;
          case LENGTH_l:
            *((long *)s) = info->written;
            break;
          case LENGTH_ll:
            *((long long *)s) = info->written;
            break;
          case LENGTH_j:
            *((intmax_t *)s) = info->written;
            break;
          case LENGTH_z:
            *((ssize_t *)s) = info->written;
            break;
          case LENGTH_t:
            *((ptrdiff_t *)s) = info->written;
            break;
          case LENGTH_NONE:
          case LENGTH_L:
            *((int *)s) = info->written;
            break;
        }
      }
      break;
  }
}

static int vprintf_shared(vprintf_info *info, const char * restrict format, va_list args) {
  const char *start_literal = format;
  while (info->rc == OK) {
    const char c = *format;
    if (c == '\0') {
      break;
    }

    switch (c) {
      case '%':
        if (start_literal != format) {
          const size_t size = format - start_literal;
          info->write_fn(info, start_literal, size);
          if (info->rc != OK) {
            break;
          }
        }
        ++format;
        vprintf_percent(info, &format, args);
        if (info->rc != OK) {
          break;
        }
        start_literal = format;
      default:
        // Next input char, this will be output later using start_literal.
        ++format;
    }
  }
  if (info->rc == OK && start_literal != format) {
    const size_t size = format - start_literal;
    info->write_fn(info, start_literal, size);
  }
  switch (info->rc) {
    case ERR:
      return -1;
    case END:
    case OK:
      return info->written;
  }
}

int vprintf(const char * restrict format, va_list args) {
  vprintf_info info = {0};
  info.write_fn = write_fd;
  info.fd = 1;
  return vprintf_shared(&info, format, args);
}

int vsprintf(char * restrict buffer, const char * restrict format, va_list args) {
  return vsnprintf(buffer, SIZE_MAX, format, args);
}

int vsnprintf(char * restrict buffer, size_t buf_size, const char * restrict format, va_list args) {
  vprintf_info info = {0};
  info.write_fn = write_str;
  info.buffer = buffer;
  info.buf_size = (buf_size == 0) ? 0 : (buf_size - 1);

  const int ret = vprintf_shared(&info, format, args);

  if (buf_size > 0) {
    // String must be null terminated.
    // Note that ret is unreliable, since it might be -1.
    // Instead, we use info->buf_size.
    buffer[buf_size - info.buf_size - 1] = '\0';
  }
  return ret;
}

int printf(const char * restrict format, ...) {
  va_list args;
  va_start(args, format);
  int ret = vprintf(format, args);
  va_end(args);
  return ret;
}

int sprintf(char * restrict buffer, const char * restrict format, ...) {
  va_list args;
  va_start(args, format);
  int ret = vsprintf(buffer, format, args);
  va_end(args);
  return ret;
}

int snprintf(char * restrict buffer, size_t buf_size, const char * restrict format, ...) {
  va_list args;
  va_start(args, format);
  int ret = vsnprintf(buffer, buf_size, format, args);
  va_end(args);
  return ret;
}
