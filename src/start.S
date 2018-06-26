.intel_syntax noprefix

.text
.globl _start
.extern main
.extern exit

_start:
  pop rdi           # rdi = argc
  mov rsi, rsp      # rsi = argv
	and rsp, -16      # Align the stack to 16 bytes.
  call __libc_start_main
