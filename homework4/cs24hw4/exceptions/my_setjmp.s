/*
 * Implementation of setjmp and longjmp.
 *
 * Setjmp allows the current state to be saved in case there is need to
 * return to this state and returns 0 to signal that no error has
 * been seen yet.
 *
 * Longjmp allows the state to be set to the state that was saved by
 * setjmp, as well as returning the value associated with the error
 * that required return to the saved state.
 *
 */


.globl my_setjmp

/* MY_SETJMP */

my_setjmp:
	pushl %ebp                              /* Push caller's ebp */
	movl %esp, %ebp                         /* Store current stack pointer 
                                             * in ebp 
                                             */

my_setjmp_save_to_jmp_buf:
    movl 8(%ebp), %ecx                      /* Store pointer to jump buffer
                                             * in ecx
                                             */
	movl %ebx, (%ecx)                       /* Store ebx into jump buffer */
	movl %esi, 4(%ecx)                      /* Store esi into jump buffer */
	movl %edi, 8(%ecx)                      /* Store edi into jump buffer */
    movl %ebp, 12(%ecx)                     /* Store ebp into jump buffer */
    movl (%ebp), %eax                       /* Store caller's ebp into jump
                                             * buffer
                                             */
    movl %eax, 16(%ecx)
    movl 4(%ebp), %eax                      /* Store caller's ret address
                                             * into jump buffer
                                             */
    movl %eax, 20(%ecx)

my_setjmp_finish:
	movl $0, %eax                           /* Set return value to 0 */

my_setjmp_return:
	movl %ebp, %esp
	popl %ebp
	ret

.globl my_longjmp

/* MY_LONGJMP */

my_longjmp:
	pushl %ebp                              /* Push caller's ebp */
	movl %esp, %ebp                         /* Store current stack pointer
                                             * in ebp 
                                             */

my_longjmp_restore_data_registers:
    movl 8(%ebp), %ecx                      /* Store pointer to jump buffer
                                             * in ecx
                                             */
	movl (%ecx), %ebx                       /* Restore ebx from jump buffer */
	movl 4(%ecx), %esi                      /* Restore esi form jump buffer */
	movl 8(%ecx), %edi                      /* Restore edi from jump buffer */

my_longjmp_set_return_value:
    movl 12(%ebp), %eax                     /* Set return value to value
                                             * of second argument
                                             */
    testl %eax, %eax                        /* *************************** */
    jnz my_longjmp_restore_stack_pointer    /* If value of second argument */
    movl $1, %eax                           /* was nonzero, continue;      */
                                            /* otherwise, set return value */
                                            /* to 1                        */
                                            /* *************************** */

my_longjmp_restore_stack_pointer:
    movl 12(%ecx), %esp                      /* Restore esp from jump buffer */
    movl 16(%ecx), %ebp                      /* Restore ebp from jump buffer */
    add $8, %esp                             /* Restore stack pointer to where
                                              * it was just before my_setjmp
                                              * was initially called
                                              */
    pushl 20(%ecx)                           /* Push return address of
                                              * function that originally
                                              * called my_setjmp onto the
                                              * stack
                                              */
	
my_longjmp_return:
    ret                                      /* Return */
