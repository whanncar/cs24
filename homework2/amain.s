	.file	"amain.c"
	.section	.text.unlikely,"ax",@progbits
.LCOLDB0:
	.text
.LHOTB0:
	.p2align 4,,15
	.globl	accum
	.type	accum, @function
accum:
.LFB21:
	.cfi_startproc
	movl	value(%rip), %eax
	addl	%edi, %eax
	movl	%eax, value(%rip)
	ret
	.cfi_endproc
.LFE21:
	.size	accum, .-accum
	.section	.text.unlikely
.LCOLDE0:
	.text
.LHOTE0:
	.section	.text.unlikely
.LCOLDB1:
	.text
.LHOTB1:
	.p2align 4,,15
	.globl	reset
	.type	reset, @function
reset:
.LFB22:
	.cfi_startproc
	movl	value(%rip), %eax
	movl	$0, value(%rip)
	ret
	.cfi_endproc
.LFE22:
	.size	reset, .-reset
	.section	.text.unlikely
.LCOLDE1:
	.text
.LHOTE1:
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC2:
	.string	"n = %d\taccum = %d\n"
	.section	.text.unlikely
.LCOLDB3:
	.section	.text.startup,"ax",@progbits
.LHOTB3:
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB23:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	$0, value(%rip)
	movl	$10, %ebx
	.p2align 4,,10
	.p2align 3
.L4:
	call	rand
	subl	$1, %ebx
	movl	%eax, %ecx
	jne	.L4
	movl	$274877907, %edx
	movl	$.LC2, %edi
	imull	%edx
	movl	%ecx, %eax
	sarl	$31, %eax
	sarl	$6, %edx
	movl	%edx, %esi
	movl	value(%rip), %edx
	subl	%eax, %esi
	xorl	%eax, %eax
	imull	$1000, %esi, %esi
	subl	%esi, %ecx
	addl	%ecx, %edx
	movl	%ecx, %esi
	movl	%edx, value(%rip)
	call	printf
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE23:
	.size	main, .-main
	.section	.text.unlikely
.LCOLDE3:
	.section	.text.startup
.LHOTE3:
	.comm	value,4,4
	.ident	"GCC: (GNU) 5.2.0"
	.section	.note.GNU-stack,"",@progbits
