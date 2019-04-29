#include <assert.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  free(NULL);

  void *p = malloc(0);
  free(p);
  p = realloc(NULL, 0);
  free(p);

  const char *str = "abcdefg";
  void *a = malloc(4);
  memcpy(a, str, 4);
  free(a);

  a = malloc(4);
  memcpy(a, str, 4);
  a = realloc(a, 8);
  assert(memcmp(a, str, 4) == 0);

  memcpy(a, str, 8);
  a = realloc(a, 4);
  assert(memcmp(a, str, 4) == 0);
  free(a);
}
