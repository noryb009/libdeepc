.intel_syntax noprefix

.text

.globl __restore
__restore:
  # Just call the `rt_sigreturn` syscall.
	mov eax, 15
	syscall
