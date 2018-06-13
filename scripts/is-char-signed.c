// Returns 0 if char is signed.
int main(void) {
  const int neg_one = -1;
  // The standard states that if a value can be represented after a cast,
  // the value will not change. If the cast is from a signed type to an
  // unsigned type, the maximum unsigned value will be added or subtracted
  // until the value fits in the number's range.
  const char c = (char)neg_one;
  // This doesn't change the value.
  const int value = (int)c;
  // If the value was not modified, we have a signed char.
  return value == -1 ? 0 : 1;
}
