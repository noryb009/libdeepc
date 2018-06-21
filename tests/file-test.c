#include <assert.h>
#include <stdio.h>
#include <string.h>

static void assert_file_contents(FILE *f, const char *expected_str) {
  for (int i = 0; ; ++i) {
    const int actual = fgetc(f);
    const int expected = expected_str[i];
    if (actual == EOF) {
      assert(expected == '\0');
      break;
    }
    assert(actual == expected);
  }
}

int main(void) {
  assert(fopen("test-data/non-exist", "r") == NULL);
  assert(fopen("test-data/unreadable", "r") == NULL);
  assert(fopen("test-data/readable", "r+") == NULL);
  assert(fopen("test-data/readable", "a") == NULL);
  assert(fopen("test-data/readable", "a+") == NULL);
  assert(fopen("test-data/readable", "w") == NULL);
  assert(fopen("test-data/readable", "w+") == NULL);

  const char *expected_str = "This file\nhas been read.\n";

  FILE *f1 = fopen("test-data/readable", "r");
  assert(f1 != NULL);
  assert_file_contents(f1, expected_str);
  fclose(f1);

  const char *scratch_file = "test-data/scratch";

  f1 = fopen(scratch_file, "w+");
  assert(fprintf(f1, "%s", expected_str) == (int)strlen(expected_str));
  fflush(f1);
  FILE *f2 = fopen(scratch_file, "r");
  assert_file_contents(f2, expected_str);
  fclose(f1);
  fclose(f2);
  f1 = fopen(scratch_file, "r");
  assert_file_contents(f1, expected_str);
  fclose(f1);
  f1 = fopen(scratch_file, "w");
  fprintf(f1, "abc");
  fclose(f1);
  f1 = fopen(scratch_file, "a");
  fprintf(f1, "def");
  fclose(f1);
  f1 = fopen(scratch_file, "r+");
  assert_file_contents(f1, "abcdef");
  fclose(f1);
}
