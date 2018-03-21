#include "stdarg.h"

inline void __va_start_internal(va_list *args, void *beforeParam, __size_t beforeParamSize) {
  args->ptr = ((char*)beforeParam) + beforeParamSize;
}

inline void __va_end_internal(va_list *args) {
  // Nothing.
}

inline void *__va_arg_internal(va_list *args, __size_t size) {
  char *ret = args->ptr;
  args->ptr += size;
  return ret;
}

inline void __va_copy_internal(va_list *dest, va_list *src) {
  dest->ptr = src->ptr;
}
