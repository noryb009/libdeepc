%:include <assert.h>
%:include <iso646.h>
%:include <stdbool.h>
%:include <stdint.h>

int main(void) {
  assert((0x124 bitor  0x214) == 0x334);
  assert((0x124 bitand 0x214) == 0x4);
  assert((compl (uint32_t)UINT32_MAX) == 0);
  assert((compl (uint32_t)UINT32_MAX) not_eq 12);

  assert(( true and  true) ==  true);
  assert((false and  true) == false);
  assert((false and false) == false);
  assert(( true xor  true) == false);
  assert((false xor  true) ==  true);
  assert((false xor false) == false);
  assert(( true  or  true) ==  true);
  assert((false  or  true) ==  true);
  assert((false  or false) == false);
  assert((not  true) == false);
  assert((not false) ==  true);

  bool b;
  b =  true; b and_eq  true; assert(b ==  true);
  b =  true; b and_eq false; assert(b == false);
  b = false; b and_eq false; assert(b == false);
  b =  true; b xor_eq  true; assert(b == false);
  b =  true; b xor_eq false; assert(b ==  true);
  b = false; b xor_eq false; assert(b == false);
  b =  true; b or_eq  true; assert(b ==  true);
  b =  true; b or_eq false; assert(b ==  true);
  b = false; b or_eq false; assert(b == false);
}
