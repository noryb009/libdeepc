#pragma once

// Well, this is disappointing...
// We can't determine what the second parameter to `va_start` is in the
// parameter list, so we can't figure out where the vararg list is.
// Instead, just use builtins.

#define va_list __builtin_va_list

#define va_start(a, b) __builtin_va_start(a, b)
#define va_arg(a, b) __builtin_va_arg(a, b)
#define va_end(a) __builtin_va_end(a)
#define va_copy(a, b) __builtin_va_copy(a, b)
