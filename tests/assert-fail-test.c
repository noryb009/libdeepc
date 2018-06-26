#include <assert.h>

int main(void) {
#define NDEBUG
#include <assert.h>
  assert(1 == 0);
#undef NDEBUG
#include <assert.h>
  assert(0 == 1);
  return 2;
}
