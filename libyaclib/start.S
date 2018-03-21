.intel_syntax noprefix

.text
.globl _start
.extern main

_start:
  # We could pop these, but we don't want to mess up any alignment guarantees we have.
  pop rdi           # rdi = argc
  pop rsi           # rsi = argv
  call main         # rax = main()
  mov rbx, rax      # rbx = rax
  mov rax, 1        # rax = Syscall 1, SYS_EXIT.
  int 0x80          # syscall()
