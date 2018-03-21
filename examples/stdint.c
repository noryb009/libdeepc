#include <stdint.h>

int main() {
  // TODO: Use assert().
  int8_t i8;
  uint8_t u8;
  if (sizeof(i8) != 1 || sizeof(u8) != 1)
    return 8;
  int16_t i16;
  uint16_t u16;
  if (sizeof(i16) != 2 || sizeof(u16) != 2)
    return 16;
  int32_t i32;
  uint32_t u32;
  if (sizeof(i32) != 4 || sizeof(u32) != 4)
    return 32;
  int64_t i64;
  uint64_t u64;
  if (sizeof(i64) != 8 || sizeof(u64) != 8)
    return 64;

  u8 = 0;
  --u8;
  if (u8 != UINT8_MAX)
    return 1;
  u16 = 0;
  --u16;
  if (u16 != UINT16_MAX)
    return 2;
  u32 = 0;
  --u32;
  if (u32 != UINT32_MAX)
    return 3;
  u64 = 0;
  --u64;
  if (u64 != UINT64_MAX)
    return 4;

  return 0;
}
