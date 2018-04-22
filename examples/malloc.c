#include <assert.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  assert(malloc(0) == NULL);
  assert(realloc(NULL, 0) == NULL);
  free(NULL);

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
