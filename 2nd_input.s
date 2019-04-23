	.text
	.globl	_start
_start:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$2, -16(%rbp)
	movl	$3, -12(%rbp)
	movl	$3, -8(%rbp)
	movl	$8, -4(%rbp)
	jmp	.L2
.L3:
	addl	$1, -16(%rbp)
	movl	-16(%rbp), %eax
	addl	$2, %eax
	movl	%eax, -12(%rbp)
	movl	-16(%rbp), %eax
	addl	$3, %eax
	movl	%eax, -8(%rbp)
.L2:
	movl	-16(%rbp), %eax
	cmpl	-4(%rbp), %eax
	jl	.L3
	movl	$0, %eax
	popq	%rbp
	movq	$60, %rax
	movq	$0, %rdi
	syscall
