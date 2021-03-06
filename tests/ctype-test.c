#include <assert.h>
#include <ctype.h>

int main(void) {
  assert(islower('a'));
  assert(!islower('a'-1));
  assert(islower('z'));
  assert(!islower('z'+1));

  assert(tolower('a') == 'a');
  assert(tolower('a'-1) == 'a'-1);
  assert(tolower('z') == 'z');
  assert(tolower('z'+1) == 'z'+1);

  assert(tolower('A') == 'a');
  assert(tolower('A'-1) == 'A'-1);
  assert(tolower('Z') == 'z');
  assert(tolower('Z'+1) == 'Z'+1);

  assert(toupper('a') == 'A');
  assert(toupper('a'-1) == 'a'-1);
  assert(toupper('z') == 'Z');
  assert(toupper('z'+1) == 'z'+1);

  assert(toupper('A') == 'A');
  assert(toupper('A'-1) == 'A'-1);
  assert(toupper('Z') == 'Z');
  assert(toupper('Z'+1) == 'Z'+1);

  assert(isascii(0));
  assert(isascii('Z'));
  assert(isascii(127));
  assert(!isascii(128));
  assert(!isascii(256));

  assert(toascii(0xfff) == 0x7f);
  assert(toascii(0xf9e) == 0x1e);
}
