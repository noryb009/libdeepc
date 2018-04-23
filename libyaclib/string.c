#include "string.h"

void *memchr(const void *ptr, int ch, size_t count) {
  const unsigned char needle = (unsigned char)ch;
  const unsigned char *haystack = (const unsigned char *)ptr;
  const unsigned char * const end = haystack + count;

  for (; haystack != end; ++haystack) {
    if (*haystack == needle) {
      return (void *)haystack;
    }
  }

  return NULL;
}

int memcmp(const void *lhs, const void *rhs, size_t count) {
  // TODO: Speed up.
  const char *l = (const char *)lhs;
  const char *r = (const char *)rhs;
  const char *end = l + count;
  for (; l != end; ++l, ++r) {
    if (*l != *r) {
      if (*l < *r) {
        return -1;
      } else {
        return 1;
      }
    }
  }
  return 0;
}

void *memset(void *dest, int ch, size_t count) {
  const unsigned char c = (unsigned char)ch;
  unsigned char * d = (unsigned char *)dest;
  unsigned char * const end = d + count;

  for (; d != end; ++d) {
    *d = c;
  }
  
  return dest;
}

void *memcpy(void * restrict dest, const void * restrict src, size_t count) {
  // TODO: Speed up.
  unsigned char *d = (unsigned char *)dest;
  const unsigned char *s  = (const unsigned char *)src;
  unsigned char *end = d + count;
  for (; d != end; ++d, ++s) {
    *d = *s;
  }
  return dest;
}

void *memmove(void *dest, const void *src, size_t count) {
  unsigned char *d = (unsigned char *)dest;
  const unsigned char *s = (const unsigned char *)src;
  unsigned char * const start = d;
  unsigned char * const end = start + count;

  // We can use memcpy unless `dest` and `src` overlap (since the signature
  // uses `restricted`).
  if (end <= s || s + count < d) {
    return memcpy(dest, src, count);
  }

  if (s < d) {
    s += count;
    for (d = end; d != start;) {
      --d; --s;
      *d = *s;
    }
  } else {
    for (; d != end; ++d, ++s) {
      *d = *s;
    }
  }

  return dest;
}

char *strcpy(char * restrict dest, const char * restrict src) {
  // TODO: make faster.
  int i;
  for (i = 0; src[i] != '\0'; ++i) {
    dest[i] = src[i];
  }
  dest[i] = '\0';
  return dest;
}

char *strncpy(char * restrict dest, const char * restrict src, size_t count) {
  int i;
  for (i = 0; src[i] != '\0' && count > 0; ++i, --count) {
    dest[i] = src[i];
  }
  for (; count > 0; ++i, --count) {
    dest[i] = '\0';
  }
  return dest;
}

size_t strlen(const char *str) {
  // TODO: Make faster.
  size_t len = 0;
  while (str[len] != '\0') {
    ++len;
  }
  return len;
}

char *strcat(char * restrict dest, const char * restrict src) {
  const size_t orig_len = strlen(dest);
  strcpy(dest + orig_len, src);
  return dest;
}

char *strncat(char * restrict dest, const char * restrict src, size_t count) {
  const size_t orig_len = strlen(dest);
  dest += orig_len;

  int i;
  for (i = 0; src[i] != '\0' && count > 0; ++i, --count) {
    dest[i] = src[i];
  }
  // NOTE: The null terminator is always appended.
  dest[i] = '\0';

  return dest - orig_len;
}

int strcmp(const char *lhs, const char *rhs) {
  for (;; ++lhs, ++rhs) {
    const char l = *lhs;
    const char r = *rhs;

    // Check if the string are different at this location.
    if (l != r) {
      return (l < r) ? -1 : 1;
    }

    // If lhs ends, and l == r, they both end.
    if (l == '\0') {
      return 0;
    }
  }
}

int strncmp(const char *lhs, const char *rhs, size_t count) {
  const char * const end = lhs + count;
  for (; lhs != end; ++lhs, ++rhs) {
    const char l = *lhs;
    const char r = *rhs;

    // Check if the string are different at this location.
    if (l != r) {
      return (l < r) ? -1 : 1;
    }

    // If lhs ends, and l == r, they both end.
    if (l == '\0') {
      return 0;
    }
  }

  return 0;
}

char *strchr(const char *str, int ch) {
  const char c = (char)ch;

  for (;; ++str) {
    const char d = *str;
    if (d == c) {
      return (char *)str;
    } else if (d == '\0') {
      return NULL;
    }
  }
}

char *strrchr(const char *str, int ch) {
  const char c = (char)ch;
  const char *ret = NULL;

  for (;; ++str) {
    const char d = *str;
    if (d == c) {
      ret = str;
    }
    if (d == '\0') {
      return (char *)ret;
    }
  }
}

size_t strspn(const char *dest, const char *src) {
  const char *d = dest;
  for (;; ++d) {
    const char c = *d;
    if (c == '\0') {
      return d - dest;
    }

    for (const char *sr = src; ; ++sr) {
      const char s = *sr;
      if (s == c) {
        break;
      } else if (s == '\0') {
        return d - dest;
      }
    }
  }
}

size_t strcspn(const char *dest, const char *src) {
  const char *d = dest;
  for (;; ++d) {
    const char c = *d;
    if (c == '\0') {
      return d - dest;
    }

    for (const char *sr = src; ; ++sr) {
      const char s = *sr;
      if (s == '\0') {
        break;
      } else if (s == c) {
        return d - dest;
      }
    }
  }
}

char *strpbrk(const char *dest, const char *breakset) {
  const size_t len = strcspn(dest, breakset);
  dest += len;
  if (*dest == '\0') {
    return NULL;
  } else {
    return (char *)dest;
  }
}

char *strstr(const char *str, const char *substr) {
  const size_t substr_size = strlen(substr);
  if (substr_size == 0) {
    return (char *)str;
  }
  const size_t str_size = strlen(str);

  char *s = strchr(str, *substr);
  while (s && str + str_size >= s + substr_size) {
    ++s;
    // We have the first character. Check the rest of the string.
    if (memcmp(s, substr + 1, substr_size - 1) == 0) {
      return s - 1;
    }
    // Didn't match, find the next candidate.
    s = strchr(s, *substr);
  }
  return NULL;
}

static char *main_strtok_state;

char *strtok(char * restrict str, const char * restrict delim) {
  return strtok_r(str, delim, &main_strtok_state);
}

char *strtok_r(char * restrict str, const char * restrict delim, char ** restrict state) {
  if (str == NULL) {
    str = *state;
  }

  // Skip beginning delim.
  char *t = str + strspn(str, delim);

  // Check if we have a token or not.
  if (*t == '\0') {
    *state = NULL;
    return NULL;
  }

  // We have a token, find where it ends.
  char *t_end = t + strcspn(t, delim);
  if (*t_end == '\0') {
    // Last token.
    *state = t_end;
  } else {
    // We found a delimiter.
    *state = t_end + 1;
    *t_end = '\0';
  }

  // Return the token.
  return t;
}
