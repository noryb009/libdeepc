#include <stdlib.h>

#include "yac_main.h"

int main(int argc, char **argv);

_Noreturn void __yac_main(int argc, char **argv) {
  const int result = main(argc, argv);
  exit(result);
}
