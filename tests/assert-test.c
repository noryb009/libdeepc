int main(void) {
#include <assert.h>
  assert(0 == 0);
#define NDEBUG
#include <assert.h>
  assert(0 == 1);
#undef NDEBUG
#include <assert.h>
  assert(0 == 0);
  return 2;
}
