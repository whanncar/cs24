.globl gcd

gcd:
	pushl %ebp           /* Push caller's frame pointer */
	movl %esp, %ebp      /* Store current stack pointer in ebp */
	movl 12(%ebp), %ebx  /* Store r_(k-1) into ebx */
	movl 8(%ebp), %eax   /* Store r_(k-2) into eax */
	cmpl $0, %ebx        /* Compare r_(k-1) to 0 */
	jne gcd_continue     /* If r_(k-1) is nonzero, continue */
	jmp gcd_return       /* If r_(k-1) is zero, then eax
                              * contains r_(k-2) = gcd(a,b), so jump to return
                              */

gcd_continue:
	xorl %edx, %edx      /* Zero out edx */
	divl %ebx            /* Divide r_(k-2) by r_(k-1), storing quotient
                              * into eax and remainder (= r_k) into edx
                              */
	pushl %edx           /* Push r_k as second argument to subroutine */
	pushl %ebx           /* Push r_(k-1) as first argument to subroutine */
	call gcd             /* Call subroutine */
	
gcd_return:
	movl %ebp, %esp      /* Restore stack pointer to base of frame */
	popl %ebp            /* Restore caller's base pointer */
	ret                  /* Return */

/*
 * Notes:
 *
 * Let r_0 and r_1 denote the two original given arguments
 * (with r_0 >= r_1 and both r_0 and r_1 nonnegative).
 *
 * The function takes as arguments r_(k-2) and r_(k-1) and computes r_k.
 * If r_k = 0, then the function returns r_(k-1), since this is the gcd
 * of the original arguments. If r_k is nonzero, then the function calls
 * itself on r_(k-1) and r_k and returns the result.
 *
 */
