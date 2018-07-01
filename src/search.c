#include <assert.h>
#include <errno.h>
#include <search.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// TODO: Any aliasing issues?
typedef struct queue {
  struct queue *next;
  struct queue *prev;
} queue;

static struct hsearch_data default_htab;

int hcreate(size_t nel) {
  return hcreate_r(nel, &default_htab);
}

void hdestroy(void) {
  hdestroy_r(&default_htab);
}

ENTRY *hsearch(ENTRY item, ACTION action) {
  ENTRY *retval;
  int ret = hsearch_r(item, action, &retval, &default_htab);
  if (ret == 0) {
    return NULL;
  }
  return retval;
}

static bool is_pow_2(size_t n) {
  return (n & (n - 1)) == 0;
}

static size_t next_pow_2(size_t n) {
  if (is_pow_2(n)) {
    return n;
  }

  // TODO: Make this faster.
  size_t r = 1;
  while (n != 0) {
    n >>= 1;
    r <<= 1;
  }

  return r;
}

int hcreate_r(size_t nel, struct hsearch_data *htab) {
  if (nel == 0) {
    errno = EINVAL;
    return 0;
  }

  // We want at least 50% free space, and we want a power of 2
  // so we can have a size mask.
  const size_t space = next_pow_2(nel * 2);

  htab->data = calloc(sizeof(ENTRY), space);
  if (htab->data == NULL) {
    return 0;
  }
  htab->capacity = nel;
  htab->size = 0;

  assert(is_pow_2(space));
  htab->mask = htab->capacity - 1;

  return 1;
}

void hdestroy_r(struct hsearch_data *htab) {
  free(htab->data);
  htab->data = NULL;
}

static uint64_t hasher(const unsigned char *str) {
  uint64_t hash = UINT64_C(0x0123456789abcdef);
  
  // This is a bad hash function. Some bits are touched more than others,
  // and it is probably trivial to create any given hash.
  // But it's better than returning a constant number. :)
  // TODO: Make better.
  // This works on windows of 3 characters. It xors the three characters
  // together (with c sifted left 1), and does a slightly shifted choice
  // function (Ch() in sha256, plus some shifts) on the three characters.
  for (; *str != '\0'; ++str) {
    hash = (hash << 3U) | (hash >> (64U - 3U));
    const uint64_t a = *str;
    hash ^= a;
    ++str;

    const uint64_t b = *str;
    if (b == '\0') {
      break;
    }
    hash ^= b ^ ((a & b) << 2U);
    ++str;

    const uint64_t c = *str;
    if (c == '\0') {
      break;
    }
    hash ^= (c ^ (~a & c)) << 1U;
  }

  // The bits to the left have had slightly more mixing.
  hash = (hash >> 2U) | (hash << (64U - 2U));
  return hash;
}

int hsearch_r(ENTRY item, ACTION action, ENTRY **retval, struct hsearch_data *htab) {
  *retval = NULL;

  // TODO: Remove modulo.
  const uint64_t start = hasher(item.data) & htab->mask;
  uint64_t i = start;
  const char *k;
  for (;;) {
    k = htab->data[i].key;
    if (k == NULL) {
      break;
    }
    if (strcmp(k, item.key) == 0) {
      *retval = &htab->data[i];
      return 1;
    }
    i = (i + 1) & htab->mask;
    if (i == start) {
      break;
    }
  }

  // The key was not found or we ran out of space.
  if (action == FIND || htab->size == htab->capacity) {
    return 0;
  }

  htab->size++;
  htab->data[i] = item;
  *retval = &htab->data[i];
  return 1;
}

void insque(void *element, void *pred) {
  queue *e = (queue *)element;
  queue *p = (queue *)pred;
  if (p == NULL) {
    e->next = NULL;
    e->prev = NULL;
    return;
  }
  e->next = p->next;
  p->next = e;
  e->prev = p;
  if (e->next) {
    e->next->prev = e;
  }
}

void remque(void *element) {
  queue *e = (queue *)element;
  if (e->next) {
    e->next->prev = e->prev;
  }
  if (e->prev) {
    e->prev->next = e->next;
  }
}

void *lfind(const void *key, const void *base, size_t *nelp, size_t width, __compar compar) {
  const unsigned char *it = base;
  for (size_t i = 0; i < *nelp; ++i, it += width) {
    if (compar(key, it) == 0) {
      return (void *)it;
    }
  }
  return NULL;
}

void *lsearch(const void *key, void *base, size_t *nelp, size_t width, __compar compar) {
  void *needle = lfind(key, base, nelp, width, compar);
  if (needle != NULL) {
    return needle;
  }
  unsigned char *end = ((unsigned char *)base) + (*nelp * width);
  memcpy(end, key, width);
  ++(*nelp);
  return end;
}

// TODO: Use a more balanced tree.
typedef struct bnode {
  const void *val;
  struct bnode *l;
  struct bnode *r;
} bnode;

static void tfind_with_parent_const(const void *key, const bnode **parent, const bnode **cur, __compar compar) {
  while (*cur != NULL) {
    // Check if the current node matches.
    const int c = compar(key, (*cur)->val);
    if (c == 0) {
      return;
    }

    // If not, go to a child.
    *parent = *cur;
    if (c < 0) {
      *cur = (*cur)->l;
    } else {
      *cur = (*cur)->r;
    }
  }
}

static void tfind_with_parent(const void *key, bnode **parent, bnode **cur, __compar compar) {
  tfind_with_parent_const(key, (const bnode **)parent, (const bnode **)cur, compar);
}

void *tdelete(const void *restrict key, void **restrict rootp, __compar compar) {
  bnode *parent = NULL;
  bnode *cur = *rootp;
  tfind_with_parent(key, &parent, &cur, compar);
  if (cur == NULL) {
    return NULL;
  }

  bnode **dst;
  if (parent == NULL) {
    dst = (bnode **)rootp;
  } else if (parent->l == cur) {
    dst = (bnode **)&parent->l;
  } else {
    dst = (bnode **)&parent->r;
  }

  bnode *const original_cur = cur;
  if (cur->l == NULL) {
    *dst = cur->r;
  } else if (cur->r == NULL) {
    *dst = cur->l;
  } else {
    // Promote the left branch.
    *dst = cur->l;

    // Find rightmost child of left branch.
    cur = cur->l;
    while (cur->r != NULL) {
      cur = cur->r;
    }
    // Add the right side of the original node as the right child.
    cur->r = original_cur->r;
  }

  free(original_cur);
  return parent;
}

void *tfind(const void *key, void *const *rootp, __compar compar) {
  const bnode *parent = NULL;
  const bnode *child = *rootp;
  tfind_with_parent_const(key, &parent, &child, compar);
  return (void *)child;
}

void *tsearch(const void *key, void **rootp, __compar compar) {
  bnode *parent = NULL;
  bnode *child = *rootp;
  tfind_with_parent(key, &parent, &child, compar);

  if (child != NULL) {
    return (void *)child;
  }

  bnode *n = calloc(sizeof(bnode), 1);
  if (n == NULL) {
    return NULL;
  }
  n->val = key;

  if (parent == NULL) {
    *rootp = n;
    return n;
  }

  const int c = compar(key, parent->val);
  if (c < 0) {
    ((bnode *)parent)->l = n;
  } else {
    ((bnode *)parent)->r = n;
  }
  return n;
}

static void twalk_internal(const bnode *cur, int depth, __action action) {
  if (cur->l == NULL && cur->r == NULL) {
    action(cur, leaf, depth);
  } else {
    action(cur, preorder, depth);
    if (cur->l != NULL) {
      twalk_internal(cur->l, depth + 1, action);
    }
    action(cur, postorder, depth);
    if (cur->r != NULL) {
      twalk_internal(cur->r, depth + 1, action);
    }
    action(cur, endorder, depth);
  }
}

void twalk(const void *root, __action action) {
  twalk_internal(root, 0, action);
}
