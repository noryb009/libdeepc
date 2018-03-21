.intel_syntax noprefix

.text
.globl _start
.extern main
.extern exit

_start:
  # We could pop these, but we don't want to mess up any alignment guarantees we have.
  pop rdi           # rdi = argc
  pop rsi           # rsi = argv
  call main         # rax = main()
  mov rdi, rax      # First parameter: return code.
  call exit
