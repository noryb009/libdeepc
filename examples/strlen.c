#include <string.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    return 0;
  }
  return strlen(argv[1]);
}
