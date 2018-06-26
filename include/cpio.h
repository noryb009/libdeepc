#pragma once

// As required by POSIX:
// http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/cpio.h.html

#define IXOTH    0000001
#define IWOTH    0000002
#define IROTH    0000004
#define IXGRP    0000010
#define IWGRP    0000020
#define IRGRP    0000040
#define IXUSR    0000100
#define IWUSR    0000200
#define IRUSR    0000400
#define C_ISVTX  0001000
#define C_ISGID  0002000
#define C_ISUID  0004000
#define C_ISFIFO 0010000
#define C_ISCHR  0020000
#define C_ISDIR  0040000
#define C_ISBLK  0060000
#define C_ISREG  0100000
#define C_ISCTG  0110000
#define C_ISLNK  0120000
#define C_ISSOCK 0140000

#define MAGIC "070707"
