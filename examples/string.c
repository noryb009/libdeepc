#include <assert.h>
#include <string.h>

static void test_memfuncs(void) {
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

static void test_strfuncs(void) {
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
}

static void test_cmpfuncs(void) {
  assert(strcmp("abc", "abc") == 0);
  assert(strcmp("aac", "abc") < 0);
  assert(strcmp("acc", "abc") > 0);
  assert(strcmp("ab", "abc") < 0);
  assert(strcmp("abcc", "abc") > 0);

  assert(strcmp("abc", "abc") == 0);
  assert(strcmp("aac", "abc") < 0);
  assert(strcmp("acc", "abc") > 0);
  assert(strcmp("ab", "abc") < 0);
  assert(strcmp("abcc", "abc") > 0);

  assert(strncmp("abc", "abc", 8) == 0);
  assert(strncmp("abcd", "abce", 3) == 0);
  assert(strncmp("aac", "abc", 5) < 0);
  assert(strncmp("acc", "abc", 5) > 0);
  assert(strncmp("ab", "abc", 2) == 0);
  assert(strncmp("ab", "abc", 3) < 0);
  assert(strncmp("abcc", "abc", 3) == 0);
  assert(strncmp("abcc", "abc", 4) > 0);

  assert(strchr("", 'c') == NULL);
  const char *c3 = "abcdaa";
  assert(strchr(c3, '\0') == c3 + 6);
  assert(strchr(c3, 'z') == NULL);
  assert(strchr(c3, 'a') == c3);
  assert(strchr(c3, 'b') == c3 + 1);
  assert(strchr(c3 + 2, 'a') == c3 + 4);
  assert(strchr(c3 + 5, 'a') == c3 + 5);
  assert(strchr(c3 + 6, 'a') == NULL);
  assert(strchr(c3 + 6, '\0') == c3 + 6);

  assert(strrchr(c3, '\0') == c3 + 6);
  assert(strrchr(c3, 'z') == NULL);
  assert(strrchr(c3, 'a') == c3 + 5);
  assert(strrchr(c3, 'b') == c3 + 1);
  assert(strrchr(c3 + 2, 'a') == c3 + 5);
  assert(strrchr(c3 + 5, 'a') == c3 + 5);
  assert(strrchr(c3 + 6, 'a') == NULL);
  assert(strrchr(c3 + 6, '\0') == c3 + 6);

  const char *c4 = "abcdefgabcdefg";
  assert(strspn(c4, "") == 0);
  assert(strspn(c4, "zy") == 0);
  assert(strspn(c4, "a") == 1);
  assert(strspn(c4, "ba") == 2);
  assert(strspn(c4 + 2, "abcdehfg") == 12);
  assert(strspn(c4 + 10, "ghfeadusi") == 4);
  assert(strspn(c4 + 10, "ghfeadusi") == 4);

  assert(strcspn(c4, "") == 14);
  assert(strcspn(c4, "zy") == 14);
  assert(strcspn(c4, "a") == 0);
  assert(strcspn(c4, "ba") == 0);
  assert(strcspn(c4, "b") == 1);
  assert(strcspn(c4 + 2, "ab") == 5);
  assert(strcspn(c4 + 10, "zvkploabc") == 4);

  assert(strpbrk(c4, "") == NULL);
  assert(strpbrk(c4, "zy") == NULL);
  assert(strpbrk(c4, "a") == c4);
  assert(strpbrk(c4, "ba") == c4);
  assert(strpbrk(c4, "b") == c4 + 1);
  assert(strpbrk(c4 + 2, "ab") == c4 + 7);
  assert(strpbrk(c4 + 10, "zvkploabc") == NULL);

  const char *n = "bonobobo";
  assert(strstr(n, "") == n);
  assert(strstr(n, "z") == NULL);
  assert(strstr(n, "ba") == NULL);
  assert(strstr(n, "boo") == NULL);
  assert(strstr(n, "bo") == n);
  assert(strstr(n + 1, "bo") == n + 4);
  assert(strstr(n + 3, "bo") == n + 4);
  assert(strstr(n + 4, "bo") == n + 4);
  assert(strstr(n + 5, "bo") == n + 6);
  assert(strstr(n, "bobo") == n + 4);
  assert(strstr(n + 5, "bobo") == NULL);
  assert(strstr(n, "bobobobo") == NULL);
  assert(strstr(n, "bonobobo") == n);
  assert(strstr(n, "onobobo") == n + 1);
}

static void test_strtok(void) {
  char a[] = "";
  assert(strtok(a, " ") == NULL);
  char b[] = "The quick, red dog. The cat.";
  assert(strcmp(strtok(b, " ,."), "The") == 0);
  assert(strcmp(strtok(NULL, " ,."), "quick") == 0);
  assert(strcmp(strtok(NULL, " ,."), "red") == 0);
  assert(strcmp(strtok(NULL, " ,."), "dog") == 0);
  assert(strcmp(strtok(NULL, " ,."), "The") == 0);
  assert(strcmp(strtok(NULL, " ,."), "cat") == 0);
  assert(strtok(NULL, " ,.") == NULL);

  char c[] = "     .. . a . .    . b . .  .";
  assert(strcmp(strtok(c, " ,."), "a") == 0);
  assert(strcmp(strtok(NULL, " ,."), "b") == 0);
  assert(strtok(NULL, " ,.") == NULL);

  char d[] = "aabaaccbb";
  assert(strcmp(strtok(d, "a"), "b") == 0);
  const char *dd = strtok(NULL, "c");
  assert(strcmp(dd, "a") == 0);
  assert(dd == d + 4);
  assert(strcmp(strtok(NULL, "b"), "c") == 0);
  assert(strcmp(strtok(NULL, ""), "b") == 0);
  assert(strtok(NULL, "c") == NULL);

  char e[] = "abcd";
  assert(strtok(e, "edcba") == NULL);

  char f[] = "abcd";
  assert(strcmp(strtok(f, "f"), "abcd") == 0);
  assert(strtok(NULL, "f") == NULL);
}

int main(void) {
  test_memfuncs();

  test_strfuncs();

  test_cmpfuncs();

  test_strtok();

  return 3;
}
