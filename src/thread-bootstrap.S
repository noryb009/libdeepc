.intel_syntax noprefix

.text

.extern abort

.globl __thrd_bootstrap_asm
__thrd_bootstrap_asm:
  # When clone() returned, the stack had a thrd_bootstrap_info on it, pointed
  # to by rsp. When syscall() ran `ret`, the first 8 bytes in the structure was
  # popped and called, which was the __thrd_bootstrap_asm pointer.
  # We want to get a pointer to info, then call __thrd_bootstrap_c() to get
  # out of assembly.
	lea rdi, [rsp - 8]      # Get a pointer to the info struct.
	and rsp, -16            # Align the stack to 16 bytes.
	call __thrd_bootstrap_c # Call the C bootstrap function.

# thrd_t thrd_current(void);
.globl thrd_current
thrd_current:
  mov rax, %fs:0
  ret

# void __copy_stack_protector(uintptr_t *to)
.global __copy_stack_protector
__copy_stack_protector:
  mov rsi, %fs:0x28
	mov [rdi], rsi
	ret
