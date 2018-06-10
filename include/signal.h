#pragma once

#include <stdinternaltypes.h>

// Signals, taken from htop's kill menu.
#define SIGHUP 1
#define SIGINT 2
#define SIGQUIT 3
#define SIGILL 4
#define SIGABRT 6
#define SIGIOT 6
#define SIGBUS 7
#define SIGFPE 8
#define SIGKILL 9
#define SIGUSR1 10
#define SIGSEGV 11
#define SIGUSR2 12
#define SIGPIPE 13
#define SIGALRM 14
#define SIGTERM 15
#define SIGSTKFLT 16
#define SIGCHLD 17
#define SIGCONT 18
#define SIGSTOP 19
#define SIGTSTP 20
#define SIGTTIN 21
#define SIGTTOU 22
#define SIGURG 23
#define SIGXCPU 24
#define SIGXFSZ 25
#define SIGVTALRM 26
#define SIGPROF 27
#define SIGWINCH 28
#define SIGIO 29
#define SIGPOLL 29
#define SIGPWR 30
#define SIGSYS 31
#define __SIGMAX 31

#define SA_RESTORER  0x4000000
#define SA_RESTART 0x10000000

// TODO: Is this OK?
typedef int sig_atomic_t;

typedef void (*__sighandler_t)(int);

typedef struct {
  unsigned long value[1];
} sigset_t;

// TODO: sa_sigaction.
struct sigaction {
  __sighandler_t sa_handler;
  unsigned long sa_flags;
  void (*sa_restorer)(void);
  sigset_t sa_mask;
};

#define SIG_ERR ((__sighandler_t)-1)
#define SIG_DFL ((__sighandler_t)0)
#define SIG_IGN ((__sighandler_t)1)

__sighandler_t signal(int sig, __sighandler_t handler);
int raise(int sig);
int kill(__pid_t pid, int sig);
