#pragma once

#include <bits/size_t.h>
#include <stdinternaltypes.h>

typedef __intptr_t ptrdiff_t;
#define NULL __NULL
typedef __max_align_t max_align_t;

#define offsetof(type, member) __builtin_offsetof(type, member)
