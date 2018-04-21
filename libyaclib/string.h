#pragma once

#include "stddef.h"

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

// TODO: String examination functions.
