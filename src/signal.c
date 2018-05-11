#include <errno.h>
#include <signal.h>

int sigaction(
    int signum,
    const struct sigaction *act,
    struct sigaction *oldact) {
  // Make sure we have a valid signal number.
  switch (sig) {
    case SIGTERM:
    case SIGSEGV:
    case SIGINT:
    case SIGILL:
    case SIGABRT:
    case SIGFPE:
      break;
    default:
      errno = EINVAL;
      return SIG_ERR;
  }

  // Save the old sigaction 
}

sighandler_t signal(int sig, void (*handler)(int)) {

  // TODO
}

int raise(int sig) {
  // TODO
}
