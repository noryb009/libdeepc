#include <assert.h>
#include <stdlib.h>
#include <string.h>

int main() {
  const char *a = "abcdef";
  char *b = strdup(a);
  assert(a != b);
  assert(strcmp(a, b) == 0);
  free(b);

  b = strndup(a, 0);
  assert(strcmp(b, "") == 0);
  free(b);
  b = strndup(a, 1);
  assert(strcmp(b, "a") == 0);
  free(b);
  b = strndup(a, 3);
  assert(strcmp(b, "abc") == 0);
  free(b);
  b = strndup(a, 5);
  assert(strcmp(b, "abcde") == 0);
  free(b);
  b = strndup(a, 6);
  assert(strcmp(b, "abcdef") == 0);
  free(b);
  b = strndup(a, 7);
  assert(strcmp(b, "abcdef") == 0);
  free(b);
}
