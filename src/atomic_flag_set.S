.intel_syntax noprefix

.text

// _Bool atomic_flag_test_and_set_explicit(volatile atomic_flag* flag, memory_order order)
.globl atomic_flag_test_and_set_explicit
atomic_flag_test_and_set_explicit:
  // TODO: Handle memory_order.
	mov rax, 1      // rax = true
	xchg rax, [rdi] // swap(rax, *rdi)
	// rax now has the old value of *rdi, and *rdi has true.
  ret
