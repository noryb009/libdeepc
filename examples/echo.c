#include <assert.h>
#include <stdio.h>

int main(void) {
  for (;;) {
    const int ch = getchar();
    if (ch == EOF) {
      break;
    }
    assert(putchar(ch) == ch);
  }
}
