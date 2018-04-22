#include <assert.h>
#include <errno.h>
#include <stdio.h>

int main() {
  assert(printf("%") == -1);
  assert(errno == EINVAL);

  errno = 0;
  assert(printf("%z") == -1);
  assert(errno == EINVAL);

  errno = 0;
  assert(printf("abc, %s, %c\n", "defgh", 'i') == 5 + 5 + 2 + 1 + 1);
  assert(errno == 0);
}
