.intel_syntax noprefix

.text

# See this: https://stackoverflow.com/a/27635527/321792
# x86 gives us acq_rel consistency, so we only need a special case for seq_cst.
# There are a few exceptions, but we can't test for those cases yet.

.globl atomic_flag_test_and_set
atomic_flag_test_and_set:
	xor rax, rax
	mov eax, 1      # rax = true
	xchg eax, [rdi] # swap(rax, *rdi)
	# rax now has the old value of *rdi, and *rdi has true.
  ret

# _Bool atomic_flag_test_and_set(volatile atomic_flag *flag);
.globl __atomic_flag_set_relaxed
__atomic_flag_set_relaxed:
.globl __atomic_flag_set_consume
__atomic_flag_set_consume:
.globl __atomic_flag_set_acquire
__atomic_flag_set_acquire:
.globl __atomic_flag_set_release
__atomic_flag_set_release:
.globl __atomic_flag_set_acq_rel
__atomic_flag_set_acq_rel:
	xor rax, rax
	mov eax, 1      # rax = true
	xchg eax, [rdi] # swap(rax, *rdi)
	# rax now has the old value of *rdi, and *rdi has true.
  ret

# void atomic_flag_clear(volatile atomic_flag *flag);
.globl atomic_flag_test_and_set_explicit
atomic_flag_test_and_set_explicit:
  # TODO: Handle memory_order.
	xor rax, rax    # rax = false
	xchg eax, [rdi] # swap(rax, *rdi)
  ret

.globl atomic_flag_clear
atomic_flag_clear:
.globl __atomic_flag_clear_relaxed
__atomic_flag_clear_relaxed:
.globl __atomic_flag_clear_consume
__atomic_flag_clear_consume:
.globl __atomic_flag_clear_acquire
__atomic_flag_clear_acquire:
.globl __atomic_flag_clear_release
__atomic_flag_clear_release:
  # TODO: Handle memory_order.
	xor rax, rax    # rax = false
	xchg eax, [rdi] # swap(rax, *rdi)
  ret
