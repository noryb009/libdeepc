#include "ctype.h"

#define BETWEEN(a, b) ((a) <= ch && ch <= (b))

int iscntrl(int ch) {
  return BETWEEN(0, 31) || ch == 127;
}

int isprint(int ch) {
  return BETWEEN(32, 126);
}

int isspace(int ch) {
  return BETWEEN(9, 13) || ch == 32;
}

int isblank(int ch) {
  return ch == 9 || ch == 32;
}

int isgraph(int ch) {
  return BETWEEN(33, 126);
}

int ispunct(int ch) {
  return BETWEEN(33, 47) || BETWEEN(58, 64) || BETWEEN(91, 96) || BETWEEN(123, 126);
}

int islower(int ch) {
  return BETWEEN('a', 'z');
}

int isupper(int ch) {
  return BETWEEN('A', 'Z');
}

int isalpha(int ch) {
  return islower(ch) || isupper(ch);
}

int isdigit(int ch) {
  return BETWEEN('0', '9');
}

int isxdigit(int ch) {
  return isdigit(ch) || BETWEEN('a', 'f') || BETWEEN('A', 'F');
}

int isalnum(int ch) {
  return isalpha(ch) || isdigit(ch);
}

int tolower(int ch) {
  if (isupper(ch)) {
    return ch + ('a' - 'A');
  }
  return ch;
}

int toupper(int ch) {
  if (islower(ch)) {
    return ch + ('A' - 'a');
  }
  return ch;
}
