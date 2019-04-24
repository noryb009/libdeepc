#pragma once

#include <unistd.h>

#define WNOHANG 0x00000001U
#define WUNTRACED 0x00000002U
#define WCONTINUED 0x00000008U

// The return code of a program is:
// RRRRRRRRCSSSSSSS
// R = Return code/parameter to exit().
// C = Core dump.
// S = Signal. This is 7f (and C = 0) if the program was stopped.
#define WEXITSTATUS(stat) (((stat) >> 8) & 0xff)
#define WTERMSIG(stat) ((stat) & 0x7f)
#define WSTOPSIG(stat) WEXITSTATUS(stat)

#define WIFEXITED(stat) (WTERMSIG(stat) == 0)
// We want to check if 0 < stat < 0x7f, but we don't want to evaluate
// stat twice.
#define WIFSIGNALED(stat) ((WTERMSIG(stat) + 1) & 0x7e)
#define WCOREDUMP(stat) ((stat) & 0x80)
#define WIFSTOPPED(stat) (((stat) & 0xff) == 0x7f)

pid_t waitpid(pid_t pid, int *stat_loc, int options);
