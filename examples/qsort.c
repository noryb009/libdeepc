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
  {
    int nums[] = {8, 2, 1, 5, 3};
    int exp[] = {1, 2, 3, 5, 8};
    test(nums, exp, 5);
  }

  {
    int nums[] = {};
    int exp[] = {};
    test(nums, exp, 0);
  }

  {
    int nums[] = {5};
    int exp[] = {5};
    test(nums, exp, 1);
  }

  {
    int nums[] = {5, 2};
    int exp[] = {2, 5};
    test(nums, exp, 2);
  }
 
  {
    int nums[] = {2, 5};
    int exp[] = {2, 5};
    test(nums, exp, 2);
  }
 
  {
    int nums[] = {-5, 2, 9, -2, 0, 100};
    int exp[] = {-5, -2, 0, 2, 9, 100};
    test(nums, exp, 6);
  }
 
  {
    int nums[] = {-5, 2, 9, -2, 0, 100, 12, 13, 14, 16, 15, 17};
    int exp[] = {-5, -2, 0, 2, 9, 12, 13, 14, 15, 16, 17, 100};
    test(nums, exp, 12);
  }
 
  {
    int nums[] = {5, 1, 2, 5, 7, 5};
    int exp[] = {1, 2, 5, 5, 5, 7};
    test(nums, exp, 6);
  }
 
  {
    int nums[] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
    int exp[] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
    test(nums, exp, 10);
  }
}
