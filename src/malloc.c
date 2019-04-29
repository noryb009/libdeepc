#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <unistd.h>

#include "gen/malloc-config.h"

// A simple allocator which does not free anything.

// This allocator has a few goals:

// 1) Avoid having a header for every small allocation.
// 2) Large allocations should be aligned to pages.

// SMALL SIZES

// For small sizes, items are allocated out of arenas. There is a list of
// arenas for each power of 2 between 1 and 2048(?) (inclusive). Allocation
// sizes are rounded up to the next highest power of 2.

// Each arena is for a double allocation size. Each arena is PAGE_SIZE
// bytes long.
// The arena contains a header, followed by as many items of the given size
// that can fit in the arena.

// header | used-bitset | (item 1) | (item 2) | ...

typedef struct small_arena_header {
  struct small_arena_header *next_arena;
  struct small_arena_header *prev_arena;
  // The number of slots that are free in the structure.
  uint16_t free_slots;
  // The word that a search for an empty slot should be started at.
  // If the first free slot is the 10th, then it doesn't make sense to look at
  // slots 1-9 next time.
  uint16_t start_search;
  // The size of each items (stored as log 2).
  uint8_t item_size;
  // Padding, added by the compiler.
  //uint8_t padding;
  //uint64_t padding;
} small_arena_header;

// If this line is updated, also update the malloc-config generator script.
static_assert(sizeof(small_arena_header) == sizeof(void*) * 2 + 8, "Invalid small_arena_header size");

static const uint16_t num_items[] = NUM_ITEMS;

// The next arena pointer allows arenas to be held in a doubly linked list.
// Used bitset is a bitset, with a bit corresponding to each item in the arena.

// There is 2 global doubly circular linked lists for each size of small arenas.
// The first holds arenas that are full, the other contains arenas that are not
// full.

// TODO: Initialize these.
static small_arena_header full_small_arenas[PAGE_SIZE_LOG_2];
static small_arena_header sparse_small_arenas[PAGE_SIZE_LOG_2];

// This flag could also go before main() if that makes things easier.
static volatile atomic_flag malloc_mtx_flag = ATOMIC_FLAG_INIT;
static volatile bool malloc_mtx_init = false;
static mtx_t malloc_mtx;

typedef struct {
  size_t size;
} malloc_metadata;

static void init_malloc(void) {
  if (malloc_mtx_init) {
    // Already initialized.
    return;
  }

  while (atomic_flag_test_and_set(&malloc_mtx_flag) == true) {
    // Busy wait for flag...
  }

  if (malloc_mtx_init) {
    atomic_flag_clear(&malloc_mtx_flag);
    return;
  }

  mtx_init(&malloc_mtx, mtx_plain);

  // Initialize small arena linked lists.
  for (size_t i = 0; i < PAGE_SIZE_LOG_2; ++i) {
    // TODO: Move to `sys/queue.h`?
    full_small_arenas[i].next_arena = &full_small_arenas[i];
    full_small_arenas[i].prev_arena = &full_small_arenas[i];
    sparse_small_arenas[i].next_arena = &sparse_small_arenas[i];
    sparse_small_arenas[i].prev_arena = &sparse_small_arenas[i];

    full_small_arenas[i].free_slots = 1;
    full_small_arenas[i].start_search = 0;
    full_small_arenas[i].item_size = i;
    sparse_small_arenas[i].free_slots = 1;
    sparse_small_arenas[i].start_search = 0;
    sparse_small_arenas[i].item_size = i;
  }

  // TODO: Do we need a barrier here?
  malloc_mtx_init = true;
  atomic_flag_clear(&malloc_mtx_flag);
}

static malloc_metadata *get_meta(void *ptr) {
  unsigned char *chars = (unsigned char *)ptr;
  return (malloc_metadata*)(chars - sizeof(malloc_metadata));
}

static void *get_data(malloc_metadata *meta) {
  unsigned char *chars = (unsigned char *)meta;
  return chars + sizeof(malloc_metadata);
}

static unsigned char *get_bitset(small_arena_header *header) {
  return ((unsigned char *)header) + sizeof(small_arena_header);
}

static unsigned char *get_items(small_arena_header *header) {
  // The first item needs to be aligned. The easiest way to do this is to go
  // from the top, then subtract all the items.
  unsigned char * const last = ((unsigned char *)header) + PAGE_SIZE;
  return last - ((1U << header->item_size) * num_items[header->item_size]);
}

static void *sbrk_malloc(size_t size) {
  if (size == 0) {
    return NULL;
  }

  const uintptr_t cur_break = (uintptr_t)sbrk(0);
  const size_t front_padding = (PAGE_SIZE - (cur_break % PAGE_SIZE)) % PAGE_SIZE;

  if (brk((void*)(cur_break + front_padding + size)) == 0) {
    return (void*)(cur_break + front_padding);
  }
  return NULL;
}

static void sbrk_free(void *ptr) {
  if (ptr == NULL) {
    return;
  }
  // Nothing (for our simple allocation scheme).
}

// Doesn't work when n == 0.
static size_t log2_ceil(size_t n) {
  assert(n != 0);

  // Do a strict ceil.
  --n;
  // Count all digits (if any).
  size_t l = 0;
  while (n != 0) {
    n >>= 1U;
    l++;
  }
  return l;
}

void *malloc(size_t size) {
  if (size > SIZE_MAX / 2) {
    errno = ENOMEM;
    return NULL;
  }

  init_malloc();
  mtx_lock(&malloc_mtx);

  if (size >= PAGE_SIZE) {
    void *ptr = sbrk_malloc(size);
    mtx_unlock(&malloc_mtx);
    return ptr;
  }

  if (size == 0) {
    // C++ requires an object of size 0 to be allocated.
    size = 1;
  }

  const size_t log = log2_ceil(size);

  small_arena_header * const head = &sparse_small_arenas[log];

  const size_t items = num_items[log];

  if (head->next_arena == head) {
    // Allocate new arena.
    small_arena_header * const new_arena = sbrk_malloc(PAGE_SIZE);
    if (new_arena == NULL) {
      mtx_unlock(&malloc_mtx);
      return NULL;
    }
    assert((uintptr_t)new_arena % PAGE_SIZE == 0);
    new_arena->free_slots = items;
    new_arena->start_search = 0;
    new_arena->item_size = log;
    {
      // Clear bit set.
      unsigned char *bitset = get_bitset(new_arena);
      memset(bitset, 0, (items + 7) / 8);
    }

    // Hook up new arena into sparse list.
    new_arena->next_arena = head;
    new_arena->prev_arena = head;
    head->next_arena = new_arena;
    head->prev_arena = new_arena;
  }

  small_arena_header * const from = head->next_arena;
  assert(from->free_slots > 0);

  // The first arena has a slot available. Find it.
  // TODO: Search in bigger chunks.
  unsigned char *bitset = get_bitset(from);
  size_t cur = from->start_search;
  size_t offset;

  for (;;) {
    if (cur == items / 8) {
      // In the last chunk, so some bits may not correspond to a slot.
      const size_t num = items % 8;
      offset = 0;
      // Check each bit, up to the end.
      while (offset < num && (bitset[cur] & (1 << offset))) {
        offset++;
      }
      if (offset < num) {
        break;
      }
      cur = 0;
    } else {
      if (bitset[cur] != UCHAR_MAX) {
        // There's a spot. Find it.
        offset = 0;
        while (bitset[cur] & (1 << offset)) {
          offset++;
        }
        assert(offset <= 7);
        break;
      }
      cur++;
    }
  }

  // Use the slot.
  bitset[cur] |= 1U << offset;
  from->free_slots--;

  if (from->free_slots == 0) {
    from->next_arena->prev_arena = from->prev_arena;
    from->prev_arena->next_arena = from->next_arena;
    
    // Move to end of full list.
    small_arena_header * const newl = &full_small_arenas[log];
    from->next_arena = newl;
    from->prev_arena = newl->prev_arena;

    newl->prev_arena = from;
    from->prev_arena->next_arena = from;
  }

  mtx_unlock(&malloc_mtx);

  return &get_items(from)[(cur * 8 + offset) * (1U << log)];
}

void free(void *ptr) {
  // TODO
  //sbrk_free(ptr);
  //if (size >= PAGE_SIZE) {
  //  sbrk_free(ptr);
  //}
}

void *realloc(void *ptr, size_t new_size) {
  // TODO: Check if this is right for new_size == 0, or ptr == NULL.
  void *new_ptr = malloc(new_size);
  if (new_ptr == NULL) {
    return NULL;
  }

  // Copy old data.
  if (ptr == NULL) {
    return new_ptr;
  }
  const uintptr_t ptr_int = (uintptr_t)ptr;
  size_t old_size;
  if (ptr_int % PAGE_SIZE == 0) {
    // TODO: Support >= PAGE_SIZE
    return NULL;
  } else {
    small_arena_header * const chunk = (small_arena_header *)(ptr_int / PAGE_SIZE * PAGE_SIZE);
    old_size = (1U << chunk->item_size);
  }
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
  (void)alignment;
  // TODO: Actual alignment.
  return malloc(size);
}
