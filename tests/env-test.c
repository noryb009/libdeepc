#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv, char **envp) {
  (void)argc;
  (void)argv;
  for (; *envp != NULL; ++envp) {
    if (memcmp(*envp, "TEST=", 5) == 0) {
      printf("TEST is: %s\n", *envp);
    }
  }

  printf("TEST2 is: %s\n", getenv("TEST2"));
  assert(getenv("DOES_NOT_EXIST") == NULL);
}
