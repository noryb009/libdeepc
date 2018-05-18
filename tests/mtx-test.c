#include <threads.h>

int main(void) {
  // TODO: Test on multiple threads.
  mtx_t m;
  mtx_init(&m, mtx_plain);
  mtx_lock(&m);
  mtx_unlock(&m);
  mtx_trylock(&m);
  mtx_unlock(&m);
}
