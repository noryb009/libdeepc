.intel_syntax noprefix

.text
.globl _start
.extern main

_start:
  # TODO: Arguments.
  call main
  mov ebx, eax # Move return code to ebx.
  mov eax, 1   # Syscall 1, SYS_EXIT.
  int 0x80
