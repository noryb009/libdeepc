#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main(void) {
  assert(system(NULL) != 0);
  assert(system("true") == 0);
  assert(system("true || false") == 0);
  assert(system("false || true") == 0);
  assert(WEXITSTATUS(system("false && true")) == 1);
  assert(WEXITSTATUS(system("exit 10")) == 10);
  assert(system("if true; then echo a; fi") == 0);
  assert(system("if false; then echo b; fi") == 0);
}
