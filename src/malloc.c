#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// A simple allocator which does not free anything.

typedef struct {
  size_t size;
} malloc_metadata;

static malloc_metadata *get_meta(void *ptr) {
  unsigned char *chars = (unsigned char *)ptr;
  return (malloc_metadata*)(chars - sizeof(malloc_metadata));
}

static void *get_data(malloc_metadata *meta) {
  unsigned char *chars = (unsigned char *)meta;
  return chars + sizeof(malloc_metadata);
}

void *malloc(size_t size) {
  if (size == 0) {
    return NULL;
  }

  void *ret = sbrk(size + sizeof(malloc_metadata));
  if (ret == (void *)-1) {
    return NULL;
  }

  malloc_metadata *meta = (malloc_metadata *)ret;
  meta->size = size;

  return get_data(meta);
}

void free(void *ptr) {
  if (ptr == NULL) {
    return;
  }
  // Nothing (for our simple allocation scheme).
}

void *realloc(void *ptr, size_t new_size) {
  void *new_ptr = malloc(new_size);
  if (new_ptr == NULL) {
    return NULL;
  }

  // Copy old data.
  if (ptr == NULL) {
    return new_ptr;
  }
  const malloc_metadata *old_meta = get_meta(ptr);
  const size_t old_size = old_meta->size;
  const size_t min_size = (old_size < new_size ? old_size : new_size);
  memcpy(new_ptr, ptr, min_size);

  // Free old data.
  free(ptr);
  return new_ptr;
}

void *calloc(size_t num, size_t size) {
  const size_t sz = num * size; // TODO: Check for overflow.

  void *ret = malloc(sz);
  if (!ret) {
    return ret;
  }

  return memset(ret, size, 0);
}

void *aligned_alloc(size_t alignment, size_t size) {
  // TODO
  return NULL;
}
