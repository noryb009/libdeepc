#include <assert.h>
#include <setjmp.h>

int run(volatile int *rep, jmp_buf buf, int ret) {
  assert(ret == *rep);
  if (ret < 5) {
    (*rep)++;
    longjmp(buf, ret + 1);
  }
  return *rep;
}

int main() {
  volatile int rep = 0;
  jmp_buf buf;
  int ret = setjmp(buf);
  return run(&rep, buf, ret);
}
