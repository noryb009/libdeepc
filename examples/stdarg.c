#include <stdarg.h>

int sum(int count, ...) {
  va_list args;
  va_start(args, count);
  int sum = 0;
  for (int i = 0; i < count; ++i) {
    sum += va_arg(args, int);
  }
  va_end(args);
  return sum;
}

int main() {
  int a = 1, b = 2, c = 3, d = 5;
  // TODO: Use assert().
  if (sum(0) != 0)
    return 1;
  if (sum(0, a, b, c, d) != 0)
    return 2;
  if (sum(1, a, b, c, d) != 1)
    return 3;
  if (sum(2, a, b) != 3)
    return 4;
  if (sum(3, a, b, c) != 6)
    return 5;
  if (sum(4, a, b, c, d) != 11)
    return 6;

  return 0;
}
