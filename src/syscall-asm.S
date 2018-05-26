.intel_syntax noprefix

.text

.globl __syscall_raw0
__syscall_raw0:
  mov rax, rdi
  syscall
	ret
.globl __syscall_raw1
.globl __syscall1_noret
__syscall_raw1:
__syscall1_noret:
  mov rax, rsi
  syscall
	ret
.globl __syscall_raw2
__syscall_raw2:
  mov rax, rdx
  syscall
	ret
.globl __syscall_raw3
__syscall_raw3:
  mov rax, rcx
  syscall
	ret
.globl __syscall_raw4
__syscall_raw4:
  mov rax, r8
	mov r10, rcx
  syscall
	ret
.globl __syscall_raw5
__syscall_raw5:
  mov rax, r9
	mov r10, rcx
  syscall
	ret
