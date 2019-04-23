	.file	"1.c"
	.text
	.globl	_start
	.type	_start, @function
_start:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$2, -16(%rbp)
	movl	$12345, -12(%rbp)
	movl	$3, -8(%rbp)
	movl	$10, -4(%rbp)
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
	.cfi_def_cfa 7, 8
	movq	$0, %rdi
	movq	$60, %rax
	syscall
	.cfi_endproc
.LFE0:
	.size	_start, .-_start
	.ident	"GCC: (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0"
	.section	.note.GNU-stack,"",@progbits
