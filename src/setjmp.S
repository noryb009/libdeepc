.intel_syntax noprefix

.text

# int setjmp(jmp_buf env);
.globl setjmp
setjmp:
  # Save all callee-saved registers.
	# Wikipedia (https://en.wikipedia.org/wiki/X86_calling_conventions) says
	#   this is rbx, rbp, and r12-r15
	mov [rdi     ], rbx
	mov [rdi + 8 ], rbp
	mov [rdi + 16], r12
	mov [rdi + 24], r13
	mov [rdi + 32], r14
	mov [rdi + 40], r15
	# We also need the return address and rsp (after the return address has
	#   been popped).
	pop rcx
  mov [rdi + 48], rcx
	mov [rdi + 56], rsp
	# Return 0.
	xor eax, eax		# eax = 0
	# This would normally be `ret`, but the return address has already been popped.
	jmp rcx

// _Noreturn void longjmp(jmp_buf env, int status);
.globl longjmp
longjmp:
	mov rbx, [rdi     ]
	mov rbp, [rdi + 8 ]
	mov r12, [rdi + 16]
	mov r13, [rdi + 24]
	mov r14, [rdi + 32]
	mov r15, [rdi + 40]
  # Restore return address and rsp.
  mov rcx, [rdi + 48]
	mov rsp, [rdi + 56]

  # Fix return code (if status == 0, return 1).
  test rsi, rsi
	jz longjmp_zero
  # Return code is status.
	mov rax, rsi
	jmp rcx
longjmp_zero:
  # Return code is 1.
  mov rax, 1
  # Return.
	jmp rcx
