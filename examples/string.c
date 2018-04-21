#include <assert.h>
#include <string.h>

void test_memfuncs() {
  char *c = "abcdefghijklmnopqrstuvwxyz";

  assert(memchr(c, 'c', 26) == c + 2);
  assert(memchr(c, '$', 26) == NULL);
  assert(memchr(c, 'c', 3) == c + 2);
  assert(memchr(c, 'c', 2) == NULL);
  assert(memchr(c + 2, 'c', 2) == c + 2);
  assert(memchr(c + 3, 'c', 2) == NULL);

  assert(memcmp(c, "abc", 3) == 0);
  assert(memcmp(c, "aac", 3) > 0);
  assert(memcmp(c, "acc", 3) < 0);
  assert(memcmp(c, "abd", 2) == 0);
  assert(memcmp(c, "", 0) == 0);

  char c2[30];
  c2[28] = 'f';
  assert(memcpy(c2, c, 27) == c2);
  assert(c2[28] == 'f');
  assert(memcmp(c, c2, 27) == 0);

  assert(memset(c2, 'a', 6) == c2);
  assert(memcmp(c2, "aaaaaaghi", 9) == 0);

  assert(memcpy(c2, c, 26) == c2);
  assert(memmove(c2 + 4, c2, 6) == c2 + 4);
  assert(memcmp(c2, "abcdabcdefklmnop", 16) == 0);

  assert(memcpy(c2, c, 26) == c2);
  assert(memmove(c2, c2 + 4, 6) == c2);
  assert(memcmp(c2, "efghijghijklmnop", 16) == 0);
}

void test_strfuncs() {
  assert(strlen("") == 0);
  assert(strlen("abc") == 3);
  assert(strlen("abcdef\0z") == 6);

  char c[40];
  c[4] = 'f';
  assert(strcpy(c, "abc") == c);
  assert(memcmp(c, "abc\0f", 5) == 0);
  //assert(strcmp(c, "abc") == 0);
  
  assert(memset(c, 'z', 32) == c);
  assert(strncpy(c, "abc", 6) == c);
  assert(memcmp(c, "abc\0\0\0z", 7) == 0);
  assert(strncpy(c, "abcdef", 7) == c);
  assert(memcmp(c, "abcdef\0z", 8) == 0);
  assert(strncpy(c, "abcdefghijkl", 9) == c);
  assert(memcmp(c, "abcdefghiz", 10) == 0);

  assert(memset(c, '\0', 32) == c);
  assert(strcat(c, "ab") == c);
  assert(memcmp(c, "ab\0\0", 4) == 0);
  assert(strcat(c, "cd") == c);
  assert(memcmp(c, "abcd\0\0", 6) == 0);
  assert(strcat(c, "") == c);
  assert(memcmp(c, "abcd\0\0", 6) == 0);
  assert(strcat(c, "efgh") == c);
  assert(memcmp(c, "abcdefgh\0\0", 10) == 0);

  assert(memset(c, '\0', 32) == c);
  assert(strncat(c, "abcd", 2) == c);
  assert(memcmp(c, "ab\0\0", 4) == 0);
  assert(strncat(c, "cd", 2) == c);
  assert(memcmp(c, "abcd\0\0", 6) == 0);
  assert(strncat(c, "", 4) == c);
  assert(memcmp(c, "abcd\0\0", 6) == 0);
  assert(strncat(c, "efgh", 8) == c);
  assert(memcmp(c, "abcdefgh\0\0", 10) == 0);

  /*assert(strcmp("abc", "abc") == 0);
  assert(strcmp("aac", "abc") < 0);
  assert(strcmp("acc", "abc") > 0);
  assert(strcmp("ab", "abc") < 0);
  assert(strcmp("abcc", "abc") > 0);*/
}

int main() {
  test_memfuncs();

  test_strfuncs();

  return 3;
}
