#include <errno.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "syscall.h"

void __restore(void);

static void set_mask_bit(sigset_t *set, int sig) {
  sig -= 1;
  const size_t word_size = (sizeof(set->value[0]) * 8);
  const size_t word = sig / word_size;
  const size_t bit = sig % word_size;
  set->value[word] |= (1U << bit);
}

__sighandler_t signal(int sig, void (*handler)(int)) {
  if (sig <= 0 || sig > __SIGMAX) {
    errno = EINVAL;
    return SIG_ERR;
  }
  struct sigaction new_act;
  struct sigaction old_act;
  new_act.sa_handler = handler;
  new_act.sa_flags = SA_RESTART | SA_RESTORER;
  new_act.sa_restorer = __restore;
  memset(&new_act.sa_mask, 0, sizeof(new_act.sa_mask));
  set_mask_bit(&new_act.sa_mask, sig);

  const size_t mask_size = sizeof(new_act.sa_mask);
  if (__syscall4(sig, (uintptr_t)&new_act, (uintptr_t)&old_act, mask_size, SYS_RT_SIGACTION) == -1) {
    return SIG_ERR;
  }
  return old_act.sa_handler;
}

int raise(int sig) {
  // TODO: Use tgkill().
  return kill(getpid(), sig);
}

int kill(pid_t pid, int sig) {
  return __syscall2(pid, sig, SYS_KILL);
}
