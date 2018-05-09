#include <assert.h>
#include <stdlib.h>

static int cmp(const void *a, const void *b) {
  const int aa = *(const int *)a;
  const int bb = *(const int *)b;

  if (aa < bb) {
    return -1;
  } else if (aa > bb) {
    return 1;
  } else {
    return 0;
  }
}

int main(void) {
  assert(bsearch(NULL, NULL, 0, 4, cmp) == NULL);

  const size_t len = 9;
  const int nums[] = {-5, -2, 1, 3, 6, 8, 10, 100, 1000};

  for (size_t i = 0; i < len; ++i) {
    int key = nums[0] - 1;
    assert(bsearch(&key, nums, i, sizeof(int), cmp) == NULL);

    for (size_t n = 0; n < i; ++n) {
      int sub_key = nums[n];
      assert(bsearch(&sub_key, nums, i, sizeof(int), cmp) == nums + n);
      sub_key++;
      assert(bsearch(&sub_key, nums, i, sizeof(int), cmp) == NULL);
    }
  }
}
