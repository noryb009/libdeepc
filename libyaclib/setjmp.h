#pragma once

typedef struct {
  // 8 registers, each 8 bytes.
  unsigned char data[8 * 8];
} jmp_buf[1];

int setjmp(jmp_buf env);
_Noreturn void longjmp(jmp_buf env, int status);
