# Author: Neo Zhou - zhouaea@bc.edu

	.file	"multiply.c"
	.text
	.globl	multiply
	.type	multiply, @function
multiply:
.LFB0:
	.cfi_startproc
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	.cfi_def_cfa_register 6
	movq	%rsi, %rax 
	imulq	%rdi, %rax
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	multiply, .-multiply
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-44)"
	.section	.note.GNU-stack,"",@progbits
