#include <assert.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv) {
  for (int i = 1; i < argc; ++i) {
    const size_t len = strlen(argv[i]);
    assert(puts(argv[i]) == len);
    assert(putchar('\n') == '\n');
  }
}
