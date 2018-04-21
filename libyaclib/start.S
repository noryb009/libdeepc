.intel_syntax noprefix

.text
.globl _start
.extern main
.extern exit

_start:
  # We could pop these, but we don't want to mess up any alignment guarantees we have.
  mov rdi, [rsp]    # rdi = argc
  mov rsi, rsp      # rsi = argv (= (&argc) + 8)
  add rsi, 8
  call main         # rax = main()
  mov rdi, rax      # First parameter: return code.
  call exit
