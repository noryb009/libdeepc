#include <assert.h>
#include <stdio.h>

int main() {
  for (;;) {
    int ch = getchar();
    if (ch == EOF) {
      break;
    }
    assert(putchar(ch) == ch);
  }
}
