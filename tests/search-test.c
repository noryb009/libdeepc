#include <assert.h>
#include <search.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct queue {
  struct queue *n;
  struct queue *p;
  int val;
} queue;

static int intcmp(const void *a, const void *b) {
  const int aa = *(const int *)a;
  const int bb = *(const int *)b;
  if (aa < bb) {
    return -1;
  } else if (aa > bb) {
    return 1;
  }
  return 0;
}

static queue *setup_pool(int sz) {
  queue *pool = malloc(sizeof(queue) * sz);
  for (int i = 0; i < sz; ++i) {
    pool[i].val = i;
  }
  return pool;
}

static void insert_pool_items(queue *pool) {
  insque(&pool[6], &pool[0]);
  insque(&pool[3], &pool[0]);
  insque(&pool[1], &pool[0]);
  insque(&pool[5], &pool[3]);
  remque(&pool[6]);
  insque(&pool[4], &pool[3]);
  insque(&pool[2], &pool[1]);
  remque(&pool[5]);

  for (int i = 0; i < 4; ++i) {
    assert(pool[i].n == &pool[i + 1]);
    assert(pool[i + 1].p == &pool[i]);
  }

  free(pool);
}

static void linear_queue_test(void) {
  queue *pool = setup_pool(10);

  insque(&pool[0], NULL);
  insert_pool_items(pool);

  assert(pool[0].p == NULL);
  assert(pool[4].n == NULL);

  remque(&pool[1]);
  remque(&pool[2]);
  remque(&pool[3]);
  remque(&pool[4]);
  assert(pool[0].p == NULL);
  assert(pool[0].n == NULL);

  free(pool);
}

static void circular_queue_test(void) {
  queue *pool = setup_pool(10);

  pool[0].p = &pool[0];
  pool[0].n = &pool[0];
  insert_pool_items(pool);

  assert(pool[0].p == &pool[4]);
  assert(pool[4].n == &pool[0]);

  remque(&pool[1]);
  remque(&pool[2]);
  remque(&pool[3]);
  remque(&pool[4]);
  assert(pool[0].p == &pool[0]);
  assert(pool[0].n == &pool[0]);
}

static void lfind_add_num(int needle, int *arr, size_t *sz) {
  size_t orig_sz = *sz;
  assert(lfind(&needle, arr, sz, sizeof(int), intcmp) == NULL);
  assert(*sz == orig_sz);
  assert(lsearch(&needle, arr, sz, sizeof(int), intcmp) == arr + orig_sz);
  assert(*sz == orig_sz + 1);

  assert(lfind(&needle, arr, sz, sizeof(int), intcmp) == arr + orig_sz);
  assert(lsearch(&needle, arr, sz, sizeof(int), intcmp) == arr + orig_sz);
  assert(*sz == orig_sz + 1);
}

static void list_test(void) {
  int arr[11] = {0};
  arr[10] = 42;
  size_t sz = 0;

  lfind_add_num(0, arr, &sz);
  lfind_add_num(3, arr, &sz);
  sz = 9;
  lfind_add_num(4, arr, &sz);
  assert(arr[0] == 0);
  assert(arr[1] == 3);
  assert(arr[2] == 0);
  assert(arr[8] == 0);
  assert(arr[9] == 4);
  assert(arr[10] == 42);
}

static void same_kv(ENTRY *a, ENTRY *b) {
  assert(a);
  assert(b);
  assert(strcmp(a->key, b->key) == 0);
  assert(strcmp(a->data, b->data) == 0);
}

static void same_k_diff_v(ENTRY *a, ENTRY *b) {
  assert(a);
  assert(b);
  assert(strcmp(a->key, b->key) == 0);
  assert(strcmp(a->data, b->data) != 0);
}

static void hash_test(void) {
  assert(hcreate(4) != 0);
  ENTRY e = {.key = "abc", .data = "bcd"};
  ENTRY e_copy = {.key = "abc", .data = "cdb"};
  ENTRY e2 = {.key = "def", .data = "oaf"};
  ENTRY e3 = {.key = "abcd", .data = "onw"};
  ENTRY e4 = {.key = "def2", .data = "okq"};
  ENTRY e5 = {.key = "def3", .data = "okq"};
  assert(hsearch(e, FIND) == NULL);
  same_kv(hsearch(e, ENTER), &e);
  assert(hsearch(e2, FIND) == NULL);
  assert(hsearch(e2, FIND) == NULL);
  same_kv(hsearch(e2, ENTER), &e2);
  same_kv(hsearch(e, FIND), &e);
  same_kv(hsearch(e3, ENTER), &e3);
  same_kv(hsearch(e4, ENTER), &e4);
  same_k_diff_v(hsearch(e_copy, FIND), &e_copy);

  assert(hsearch(e5, ENTER) == NULL);
  hdestroy();
}

static void hash_r_test(void) {
  struct hsearch_data htab1 = {0};
  struct hsearch_data htab2 = {0};
  struct hsearch_data *h1 = &htab1;
  struct hsearch_data *h2 = &htab2;

  ENTRY e = {.key = "", .data = "01"};
  ENTRY e_copy = {.key = "", .data = "02"};
  ENTRY e2 = {.key = "bob", .data = "20"};
  ENTRY e3 = {.key = "joe", .data = "30"};
  
  ENTRY *ret;
  assert(hcreate_r(2, h1) != 0);
  assert(hsearch_r(e, FIND, &ret, h1) == 0);
  assert(ret == NULL);
  assert(hsearch_r(e, FIND, &ret, h1) == 0);
  assert(ret == NULL);
  assert(hsearch_r(e, ENTER, &ret, h1) != 0);
  same_kv(ret, &e);

  assert(hcreate_r(4, h2) != 0);
  assert(hsearch_r(e, FIND, &ret, h1) != 0);
  same_kv(ret, &e);
  assert(hsearch_r(e, FIND, &ret, h2) == 0);
  assert(ret == NULL);
  assert(hsearch_r(e_copy, ENTER, &ret, h2) != 0);
  same_kv(ret, &e_copy);
  assert(hsearch_r(e_copy, ENTER, &ret, h1) != 0);
  same_kv(ret, &e);

  assert(hsearch_r(e2, FIND, &ret, h2) == 0);
  assert(ret == NULL);
  assert(hsearch_r(e2, ENTER, &ret, h2) != 0);
  same_kv(ret, &e2);

  assert(hsearch_r(e, FIND, &ret, h1) != 0);
  same_kv(ret, &e);

  assert(hsearch_r(e2, ENTER, &ret, h1) != 0);
  same_kv(ret, &e2);

  assert(hsearch_r(e3, ENTER, &ret, h1) == 0);
  assert(ret == NULL);

  assert(hsearch_r(e3, ENTER, &ret, h2) != 0);
  same_kv(ret, &e3);

  assert(hsearch_r(e3, FIND, &ret, h1) == 0);
  assert(ret == NULL);

  hdestroy_r(h1);

  assert(hsearch_r(e, ENTER, &ret, h2) != 0);
  same_kv(ret, &e_copy);

  hdestroy_r(h2);
}

static void assert_ret_num(const void *ptr, const int expected) {
  const int n = **(const int **)ptr;
  assert(n == expected);
}

// Expected depth.
static int d = 0;
// Number of times each method is called.
static int n_pre = 0;
static int n_post = 0;
static int n_end = 0;
// Preorder counter.
static int pre = 10;
// Postorder counter.
static int post = -1;

static void twalk_checker(const void *p, VISIT v, int depth) {
  const int n = **(const int **)p;

  if (v == preorder) {
    assert(d == depth);
    ++d;

    ++n_pre;
  } else if (v == postorder) {
    assert(d == depth + 1);

    ++n_post;
  } else if (v == endorder) {
    d--;
    assert(d == depth);

    ++n_end;
  } else {
    assert(d == depth);

    ++n_pre;
    ++n_post;
    ++n_end;
  }

  if (v == preorder || v == leaf) {
    assert(n < pre);
    pre = n;
  }
  if (v == postorder || v == leaf) {
    assert(post < n);
    post = n;
    pre = 10;
  }
}

static int test_twalk(const void *tree) {
  n_pre = 0;
  n_post = 0;
  n_end = 0;

  d = 0;
  pre = 10;
  post = -1;

  twalk(tree, twalk_checker);

  assert(n_pre == n_post);
  assert(n_post == n_end);
  return n_pre;
}

static void tree_test(void) {
  const int nums[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  void *tree = NULL;
  int n = 0;

  assert(tdelete(&n, &tree, intcmp) == NULL);

  // 0-9, except 2 and 7.
  int to_insert[] = {0, 3, 9, 8, 1, 4, 6, 5};
  for (unsigned int i = 0; i < sizeof(to_insert) / sizeof(to_insert[0]); ++i) {
    n = to_insert[i];
    assert(tfind(&nums[n], &tree, intcmp) == NULL);
    assert_ret_num(tsearch(&nums[n], &tree, intcmp), n);
    assert_ret_num(tfind(&nums[n], &tree, intcmp), n);
  }

  for (int i = 0; i < 10; ++i) {
    switch (i) {
      case 2:
      case 7:
        assert(tfind(&i, &tree, intcmp) == NULL);
        break;
      default:
        assert_ret_num(tfind(&i, &tree, intcmp), i);
        break;
    }
  }

  assert(test_twalk(tree) == 8);

  n = 2;
  assert(tdelete(&n, &tree, intcmp) == NULL);

  n = 0;
  tdelete(&n, &tree, intcmp);
  n = 4;
  tdelete(&n, &tree, intcmp);
  n = 8;
  tdelete(&n, &tree, intcmp);
  n = 9;
  tdelete(&n, &tree, intcmp);

  assert(test_twalk(tree) == 4);

  for (int i = 0; i < 10; ++i) {
    switch (i) {
      case 2:
      case 7:
      case 0:
      case 4:
      case 8:
      case 9:
        assert(tfind(&i, &tree, intcmp) == NULL);
        break;
      default:
        assert_ret_num(tfind(&i, &tree, intcmp), i);
        break;
    }
  }
}

int main(void) {
  list_test();
  linear_queue_test();
  circular_queue_test();
  hash_test();
  hash_r_test();
  tree_test();
}
