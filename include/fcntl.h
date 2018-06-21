#pragma once

#include <__stdint.h>

#define O_ACCMODE        03U
#define O_RDONLY         00U
#define O_WRONLY         01U
#define O_RDWR           02U
#define O_CREAT        0100U
#define O_EXCL         0200U
#define O_NOCTTY       0400U
#define O_TRUNC       01000U
#define O_APPEND      02000U
#define O_NONBLOCK    04000U
#define O_DSYNC      010000U
#define O_ASYNC      020000U
#define O_DIRECT     040000U
#define O_LARGEFILE 0100000U
#define O_DIRECTORY 0200000U
#define O_NOFOLLOW  0400000U
#define O_NOATIME  01000000U
#define O_CLOEXEC  02000000U
#define O_PATH    010000000U

#define O_SYNC (04000000U | O_DSYNC)
#define O_TMPFILE (020000000U | O_DIRECTORY)

#define O_NDELAY O_NONBLOCK
#define O_FSYNC      O_SYNC

// Mode arguments.
#define S_IXOTH 00001U
#define S_IWOTH 00002U
#define S_IROTH 00004U
#define S_IRWXO (S_IXOTH|S_IWOTH|S_IROTH)
#define S_IXGRP 00010U
#define S_IWGRP 00020U
#define S_IRGRP 00040U
#define S_IRWXG (S_IXGRP|S_IWGRP|S_IRGRP)
#define S_IXUSR 00100U
#define S_IWUSR 00200U
#define S_IRUSR 00400U
#define S_IRWXU (S_IXUSR|S_IWUSR|S_IRUSR)
#define S_ISVTX 01000U
#define S_ISGID 02000U
#define S_ISUID 04000U

typedef __uint32_t mode_t;

int open(const char *pathname, int flags, ...);
