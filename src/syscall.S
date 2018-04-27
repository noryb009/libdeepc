.intel_syntax noprefix

.text

.globl __syscall0
__syscall0:
  mov rax, rdi
  syscall
	ret
.globl __syscall1
.globl __syscall1_noret
__syscall1:
__syscall1_noret:
  mov rax, rsi
  syscall
	ret
.globl __syscall2
__syscall2:
  mov rax, rdx
  syscall
	ret
.globl __syscall3
__syscall3:
  mov rax, rcx
  syscall
	ret
.globl __syscall4
__syscall4:
  mov rax, r8
	mov r10, rcx
  syscall
	ret
.globl __syscall5
__syscall5:
  mov rax, r9
	mov r10, rcx
  syscall
	ret
