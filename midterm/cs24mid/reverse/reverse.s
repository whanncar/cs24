/*
 * reverse_list: takes a pointer to a LinkedList as an argument and
 *               reverses the list
 *
 * arguments: LinkedList *: pointer to the LinkedList to be reversed
 *                          Located at (%ebp) according to the caller.
 *                          Thus, located at 8(%ebp) after the caller's
 *                          ebp has been pushed.
 *
 * return values: void
 *
 * Note 1: Since %ebx will be used for reverse_list and it is a callee-save
 *         register, it needs to be pushed onto the stack at the beginning
 *         of execution and restored at the end.
 *
 * Note 2: As I did in the pseudocode, I will refer to the local variables
 *         used in the general case (ie the case where the list has at least
 *         three nodes) as current_node, next_node, and next_next_node.
 *         Please refer to the pseudocode for any disambiguation or
 *         clarification.
 *
 */

.globl reverse_list

reverse_list:

do_callee_prep:
    pushl %ebp              /* Push caller's frame pointer */
    movl %esp, %ebp         /* Store current stack pointer in ebp */
    pushl %ebx              /* Push caller's ebx */

zero_element_case:
    movl 8(%ebp), %eax     /* Store list's address in eax */
    movl (%eax), %eax       /* Store list->head in eax */
    cmpl $0, %eax           /* If list->head is NULL, */
    je done                 /* return                 */

one_element_case:
    movl 8(%ebp), %ebx     /* Store list's address in ebx */
    movl 4(%ebx), %ebx      /* Store list->tail in ebx */
    cmpl %eax, %ebx         /* If list->head == list->tail,              */
    je done                 /* then list only has one element, so return */

two_element_case:
    movl 4(%eax), %ecx      /* Store list->head->next in ecx */
    cmpl %ebx, %ecx         /* If list->tail != list->head->next, then */
    jne general_case        /* list has more than two elements, so     */
                            /* go to general case.                     */
    movl 8(%ebp), %edx      /* Store list's address in edx */
    movl %ebx, (%edx)       /* Store list's old tail in list->head */
    movl %eax, 4(%edx)      /* Store list's old head in list->tail */
    movl %eax, 4(%ebx)      /* Store list->tail in list->head->next */
    movl $0, 4(%eax)        /* Store NULL in list->tail->next */
    jmp done                /* Return */

general_case:
    movl 4(%eax), %ebx      /* Store list->head->next in ebx */
    movl 4(%ebx), %ecx      /* Store list->head->next->next in ecx */
    movl 8(%ebp), %edx      /* Store list's address in edx */
    movl 4(%edx), %edx      /* Store list->tail in edx */
    movl $0, 4(%eax)        /* Set list->head->next to NULL */

begin_while_loop:
    cmpl %ecx, %edx         /* If next_node->next == list->tail, */
    je finish_while_loop    /* wrap up the while loop            */

while_loop:
    movl %eax, 4(%ebx)      /* Set next_node->next = current_node */
    movl %ebx, %eax         /* Set current_node = next_node */
    movl %ecx, %ebx         /* Set next_node = next_next_node */
    movl 4(%ebx), %ecx      /* Set next_next_node = next_node->next  */
    cmpl %ecx, %edx         /* If next_next_node != list->tail, */
    jne while_loop          /* continue the whlie loop          */

finish_while_loop:
    movl %eax, 4(%ebx)      /* Set next_node->next = current_node */
    movl %ebx, 4(%ecx)      /* Set list->tail->next = next_node */

swap_head_and_tail:
    movl 8(%ebp), %ecx      /* Store list's address in ecx */
    movl (%ecx), %eax       /* Store list->head in eax */
    movl 4(%ecx), %ebx      /* Store list->tail in ebx */
    movl %ebx, (%ecx)       /* Store list's old tail in list->head */
    movl %eax, 4(%ecx)      /* Store list's old head in list->tail */

done:
    popl %ebx               /* Restore caller's ebx */
    movl %ebp, %esp         /* Restore stack pointer to base of frame */
    popl %ebp               /* Restore caller's base pointer */
    ret

