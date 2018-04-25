#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  assert(snprintf(NULL, 0, "abc%sgh\n", "def") == 6 + 3);
  char s[4096];
  assert(sprintf(s, "a%db", 123) == 5);
  assert(strcmp(s, "a123b") == 0);

  sprintf(s, "a%.4db", 123);
  assert(strcmp(s, "a0123b") == 0);
  sprintf(s, "a%4db", 12);
  assert(strcmp(s, "a  12b") == 0);
  sprintf(s, "%04d", 12);
  assert(strcmp(s, "0012") == 0);
  sprintf(s, "%+4d", 12);
  assert(strcmp(s, " +12") == 0);
  sprintf(s, "%+.4d", 12);
  assert(strcmp(s, "+0012") == 0);
  sprintf(s, "%-4d", -12);
  assert(strcmp(s, "-12 ") == 0);
}
