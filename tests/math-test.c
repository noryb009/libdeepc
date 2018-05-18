#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>

int main(void) {
  assert(abs(3) == 3);
  assert(abs(-3) == 3);
  assert(abs(0) == 0);
  assert(labs(-2l) == 2);
  assert(llabs(-2ll) == 2);

  {
    div_t d = div(5, 2);
    assert(d.quot == 2);
    assert(d.rem == 1);
  }
  {
    ldiv_t d = ldiv(5, 2);
    assert(d.quot == 2);
    assert(d.rem == 1);
  }
  {
    lldiv_t d = lldiv(5, 2);
    assert(d.quot == 2);
    assert(d.rem == 1);
  }
  {
    imaxdiv_t d = imaxdiv(5, 2);
    assert(d.quot == 2);
    assert(d.rem == 1);
  }
}
