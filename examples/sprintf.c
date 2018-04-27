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
  sprintf(s, "%+.0d", 0);
  assert(strcmp(s, "+") == 0);

  sprintf(s, "%-6u", 1234);
  assert(strcmp(s, "1234  ") == 0);
  sprintf(s, "%7.5u", 1234);
  assert(strcmp(s, "  01234") == 0);

  sprintf(s, "%7.5o", 01234);
  assert(strcmp(s, "  01234") == 0);
  sprintf(s, "%#7o", 01234);
  assert(strcmp(s, "  01234") == 0);
  sprintf(s, "%#07o", 01234);
  assert(strcmp(s, "0001234") == 0);
  sprintf(s, "%#8.5o", 01234);
  assert(strcmp(s, "   01234") == 0);
  sprintf(s, "%#8.6o", 01234);
  assert(strcmp(s, "  001234") == 0);
  sprintf(s, "%#o", 0);
  assert(strcmp(s, "0") == 0);
  sprintf(s, "%#.0o", 0);
  assert(strcmp(s, "0") == 0);

  sprintf(s, "%7x", 0x12a4);
  assert(strcmp(s, "   12a4") == 0);
  sprintf(s, "%#7X", 0x12a4);
  assert(strcmp(s, " 0X12A4") == 0);
  sprintf(s, "%#07x", 0x12a4);
  assert(strcmp(s, "0x012a4") == 0);
  sprintf(s, "%-#8.5x", 0x12a4);
  assert(strcmp(s, "0x012a4 ") == 0);
  sprintf(s, "%#8.6X", 0x12a4);
  assert(strcmp(s, "0X0012A4") == 0);
  sprintf(s, "%#.0x", 0);
  assert(strcmp(s, "") == 0);

  sprintf(s, "[%c] [%3c] [%-5c] [%0c] [%1c] [%2c]", 'a', 'b', 'c', 'd', 'e', 'f');
  assert(strcmp(s, "[a] [  b] [c    ] [d] [e] [ f]") == 0);

  sprintf(s, "[%s] [%3s] [%-5s] [%0s] [%1s] [%1s] [%2s]", "abc", "de", "f", "ghi", "", "j", "k");
  assert(strcmp(s, "[abc] [ de] [f    ] [ghi] [ ] [j] [ k]") == 0);
}
