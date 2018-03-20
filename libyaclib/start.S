.intel_syntax noprefix

.text
.globl _start
.extern main

_start:
  mov rdi, [rsp]    # rdi = argc
  mov rsi, [rsp+8]  # rsi = argv
  call main         # rax = main()
  mov rbx, rax      # rbx = rax
  mov rax, 1        # rax = Syscall 1, SYS_EXIT.
  int 0x80          # syscall()
