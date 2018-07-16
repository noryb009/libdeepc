#pragma once

#include <bits/size_t.h>

typedef enum {
  FIND,
  ENTER,
} ACTION;

typedef enum {
  preorder,
  postorder,
  endorder,
  leaf,
} VISIT;

typedef struct {
  char *key;
  void *data;
} ENTRY;

struct hsearch_data {
  ENTRY *data;
  size_t capacity;
  size_t size;
  size_t mask;
};

typedef int (*__compar)(const void *, const void *);
typedef void (*__action)(const void *, VISIT, int);

int hcreate(size_t nel);
void hdestroy(void);
ENTRY *hsearch(ENTRY item, ACTION action);
int hcreate_r(size_t nel, struct hsearch_data *htab);
void hdestroy_r(struct hsearch_data *htab);
int hsearch_r(ENTRY item, ACTION action, ENTRY **retval, struct hsearch_data *htab);

void insque(void *element, void *pred);
void remque(void *element);

void *lfind(const void *key, const void *base, size_t *nelp, size_t width, __compar compar);
void *lsearch(const void *key, void *base, size_t *nelp, size_t width, __compar compar);

void *tdelete(const void *restrict key, void **restrict rootp, __compar compar);
void *tfind(const void *key, void *const *rootp, __compar compar);
void *tsearch(const void *key, void **rootp, __compar compar);
void twalk(const void *root, __action action);
