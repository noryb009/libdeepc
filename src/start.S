.intel_syntax noprefix

.text
.globl _start
.extern main
.extern exit

_start:
  pop rdi           # rdi = argc
  mov rsi, rsp      # rsi = argv
	and rsp, -16      # Align the stack to 16 bytes.
  call main         # rax = main()
  # TODO: Call another bootstrapping function that calls exit for us.
  mov rdi, rax      # First parameter: return code.
  call exit
