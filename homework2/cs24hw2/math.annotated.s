/* This file contains IA32 assembly-language implementations of three
 * basic, very common math operations.
 *
 *
 * The common theme is that all of these functions use conditional
 * transfer of data rather than conditional redirection of control
 * flow. That is, each function executes the same code regardless
 * of whether some condition is met, but the final result depends
 * on whether the condition is met.
 *
 *
 */

    .text

/*====================================================================
 * int f1(int x, int y)
 *
 * min(x, y)
 *
 *
 */
.globl f1
f1:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %edx   /* Set edx = x */
	movl	12(%ebp), %eax  /* Set eax = y (set result = y) */
	cmpl	%edx, %eax      /* Compare eax to edx */
	cmovg	%edx, %eax      /* if eax > edx (if y > x), 
                                 * move edx into eax (set result = x)
                                 */
	popl	%ebp
	ret


/*====================================================================
 * int f2(int x)
 *
 * abs(x)
 *
 */
.globl f2
f2:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax  /* Set eax = x */
	movl	%eax, %edx     /* Set edx = eax (set edx = x) */
	sarl	$31, %edx      /* Shift edx arithmetically right by 31
                                * (if x < 0, set edx = -1;
                                * otherwise, set edx = 0)
                                */
	xorl	%edx, %eax     /* (if x < 0, set eax = two's complement of x;
                                * otherwise, leave eax unchanged)
                                */
	subl	%edx, %eax     /* (if x < 0, subtract -1 from eax,
                                * resulting in eax = -x; otherwise,
                                * subtract 0 from eax, resulting in eax = x)
                                */
	popl	%ebp
	ret


/*====================================================================
 * int f3(int x)
 *
 * sgn(x)
 *
 *
 */
.globl f3
f3:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %edx  /* Set edx = x */
	movl	%edx, %eax     /* Set eax = edx (set eax = x) */
	sarl	$31, %eax      /* Shift eax arithmetically right by 31
                                * (if x < 0, set eax = -1; otherwise,
                                * set eax = 0)
                                */
	testl	%edx, %edx     /* (if x < 0, set SF = 1, ZF = 0;
                                * if x = 0, set SF = 0, ZF = 1;
                                * if x > 0, set SF = 0, ZF = 0)
                                */
	movl	$1, %edx       /* Set edx = 1 */
	cmovg	%edx, %eax     /* (if x > 0, set eax = edx, with the result
                                * that eax = 1; if x = 0, do nothing, with
                                * the result that eax = 0; if x < 0, do
                                * nothing, with the result that eax = -1)
                                */
	popl	%ebp
	ret

