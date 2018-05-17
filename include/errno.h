#pragma once

typedef int errno_t;

extern _Thread_local errno_t errno;

#define ENOMEM 12
#define EINVAL 22
#define ERANGE 34
