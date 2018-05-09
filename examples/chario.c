#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
  for (int i = 1; i < argc; ++i) {
    const size_t len = strlen(argv[i]);
    assert(puts(argv[i]) == (ssize_t)(len + 1));
  }
}
