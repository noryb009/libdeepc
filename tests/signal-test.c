#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>

static void print_handler(int n) {
  printf("S%d\n", n);
}

typedef void (*sh)(int);

int main(void) {
  sh ret;
  ret = signal(999, NULL);
  assert(errno == EINVAL);
  assert(ret == SIG_ERR);
  errno = 0;

  ret = signal(SIGFPE, print_handler);
  assert(errno == 0);
  assert(ret == SIG_DFL);
  raise(SIGFPE);

  ret = signal(SIGFPE, SIG_IGN);
  raise(SIGFPE);
  assert(ret == print_handler);

  ret = signal(SIGFPE, SIG_DFL);
  raise(SIGFPE);
  assert(ret == SIG_IGN);

  return 0;
}
