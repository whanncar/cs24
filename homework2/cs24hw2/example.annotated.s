	.file	"example.c"
	.section	.text.unlikely,"ax",@progbits
.LCOLDB0:
	.text
.LHOTB0:
	.p2align 4,,15
	.globl	ex
	.type	ex, @function
ex:
.LFB0:
	.cfi_startproc
	movl	8(%esp), %eax  /* Move b into eax */
	subl	12(%esp), %eax /* Subtract c from b */
	imull	4(%esp), %eax  /* Multiply (b - c) by a */
	addl	16(%esp), %eax /* Add d to a * (b - c) */
	ret
	.cfi_endproc
.LFE0:
	.size	ex, .-ex
	.section	.text.unlikely
.LCOLDE0:
	.text
.LHOTE0:
	.ident	"GCC: (GNU) 5.3.0"
	.section	.note.GNU-stack,"",@progbits

