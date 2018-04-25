#include <assert.h>
#include <stdlib.h>

int cmp(const void *a, const void *b) {
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

void test(int *nums, int *exp, size_t count) {
  qsort(nums, count, sizeof(int), cmp);

  for (size_t i = 0; i < count; ++i) {
    assert(nums[i] == exp[i]);
  }
}

int main() {
  assert(bsearch(NULL, NULL, 0, 4, cmp) == NULL);

  const size_t len = 9;
  const int nums[] = {-5, -2, 1, 3, 6, 8, 10, 100, 1000};

  for (int i = 0; i < len; ++i) {
    int key = nums[0] - 1;
    assert(bsearch(&key, nums, i, sizeof(int), cmp) == NULL);

    for (int n = 0; n < i; ++n) {
      int key = nums[n];
      assert(bsearch(&key, nums, i, sizeof(int), cmp) == nums + n);
      key++;
      assert(bsearch(&key, nums, i, sizeof(int), cmp) == NULL);
    }
  }
}
