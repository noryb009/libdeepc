#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

int main(void) {
  assert(atoi("5") == 5);
  assert(atoi("214748363") == 214748363);
  assert(atoi("2147483639") == 2147483639);
  assert(atoi("214748364") == 214748364);
  assert(atoi("2147483645") == 2147483645);
  assert(atoi("2147483646") == 2147483646);
  assert(atoi("2147483647") == 2147483647);
  assert(atoi("2147483648") == 0);
  assert(atoi("2147483649") == 0);
  assert(atoi("214748365") == 214748365);
  assert(atoi("2147483650") == 0);

  assert(atoi("-5") == -5);
  assert(atoi("-214748363") == -214748363);
  assert(atoi("-2147483639") == -2147483639);
  assert(atoi("-214748364") == -214748364);
  assert(atoi("-2147483645") == -2147483645);
  assert(atoi("-2147483646") == -2147483646);
  assert(atoi("-2147483647") == -2147483647);
  assert(atoi("-2147483648") == -2147483648);
  assert(atoi("-2147483649") == 0);
  assert(atoi("-214748365") == -214748365);
  assert(atoi("-2147483650") == 0);

  assert(atol("922337203685477580") == 922337203685477580l);
  assert(atol("9223372036854775806") == 9223372036854775806l);
  assert(atol("9223372036854775807") == 9223372036854775807l);
  assert(atol("9223372036854775808") == 0);
  assert(atol("922337203685477581") == 922337203685477581l);

  assert(atol("-922337203685477580") == -922337203685477580l);
  assert(atol("-9223372036854775806") == -9223372036854775806l);
  assert(atol("-9223372036854775807") == -9223372036854775807l);
  assert(atol("-9223372036854775808") == LONG_MIN);
  assert(atol("-9223372036854775809") == 0);
  assert(atol("-922337203685477581") == -922337203685477581l);

  errno = 0;
  assert(strtol("+922337203685477580", NULL, 10) == 922337203685477580l);
  assert(strtol("9223372036854775806", NULL, 10) == 9223372036854775806l);
  assert(strtol("9223372036854775807", NULL, 10) == LONG_MAX);
  assert(errno == 0);
  assert(strtol("9223372036854775808", NULL, 10) == LONG_MAX);
  assert(errno == ERANGE);
  assert(strtol("922337203685477581", NULL, 10) == 922337203685477581l);

  errno = 0;
  assert(strtol("-922337203685477580", NULL, 10) == -922337203685477580l);
  assert(strtol("-9223372036854775806", NULL, 10) == -9223372036854775806l);
  assert(strtol("-9223372036854775807", NULL, 10) == -9223372036854775807l);
  assert(strtol("-9223372036854775808", NULL, 10) == LONG_MIN);
  assert(errno == 0);
  assert(strtol("-9223372036854775809", NULL, 10) == LONG_MIN);
  assert(errno == ERANGE);
  assert(strtol("-922337203685477581", NULL, 10) == -922337203685477581l);


  assert(strtol("   -922337203685477581a", NULL, 10) == -922337203685477581l);
  char *s;
  assert(strtol("9999999999999999999999999abc", &s, 10) == LONG_MAX);
  assert(*s == 'a');

  assert(strtol("12", NULL, 0) == 12);
  assert(strtol("0xa", NULL, 0) == 10);
  assert(strtol("0XA", NULL, 0) == 10);
  assert(strtol("010", NULL, 0) == 8);
  assert(strtol("z", NULL, 36) == 35);

  assert(strtol("1y2p0ij32e8e6", NULL, 36) == LONG_MAX-1);
  assert(strtol("1y2p0ij32e8e7", NULL, 36) == LONG_MAX);
  assert(strtol("1y2p0ij32e8e8", NULL, 36) == LONG_MAX);

  assert(strtol("-1y2p0ij32e8e7", NULL, 36) == LONG_MIN+1);
  assert(strtol("-1y2p0ij32e8e8", NULL, 36) == LONG_MIN);
  assert(strtol("-1y2p0ij32e8e9", NULL, 36) == LONG_MIN);

  errno = 0;
  assert(strtoul("", NULL, 10) == 0);
  assert(strtoul("0", NULL, 10) == 0);
  assert(strtoul("18446744073709551614", NULL, 10) == ULLONG_MAX-1);
  assert(strtoul("18446744073709551615", NULL, 10) == ULLONG_MAX);
  assert(errno == 0);
  assert(strtoul("18446744073709551616", NULL, 10) == ULLONG_MAX);
  assert(errno == ERANGE);

  errno = 0;
  assert(strtoul("-1", NULL, 10) == ULLONG_MAX);
  assert(strtoul("-2", NULL, 10) == ULLONG_MAX-1);
  assert(strtoul("-18446744073709551614", NULL, 10) == 2);
  assert(strtoul("-18446744073709551615", NULL, 10) == 1);
  assert(errno == 0);
  assert(strtoul("18446744073709551616", NULL, 10) == ULLONG_MAX);
  assert(errno == ERANGE);
}
