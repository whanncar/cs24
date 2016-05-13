.globl rl_decode

#============================================================================
# rl_decode:  decode RLE-encoded input into a malloc'd buffer
#
# Author:  Ben Bitdiddle (those guys are lucky I said I'd do this before
#          going on my vacation.  they never appreciate my work ethic!)
#
# Arguments to rl_decode are at these stack locations:
#
#      8(%ebp) = data buffer containing run-length encoded input data
#
#     12(%ebp) = length of the run-length-encoded data in the buffer
#
#     16(%ebp) = OUTPUT pointer to where the length of the decoded result
#                should be stored
#
# Return-value in %eax is the pointer to the malloc'd buffer containing
# the decoded data.
#
rl_decode:
        # Set up stack frame.
        pushl   %ebp
        movl    %esp, %ebp

        # Save callee-save registers.
        pushl   %ebx
        pushl   %esi
        pushl   %edi

        # First, figure out how much space is required to decode the data.
        # We do this by summing up the counts, which are in the odd memory
        # locations.

        movl    8(%ebp), %ecx             # %ecx = start of source array
        xorl    %esi, %esi                # %esi = loop variable
        xorl    %ebx, %ebx                # %ebx = size required
        xorl    %eax, %eax

        # Find-space while-loop starts here...
        cmpl   12(%ebp), %esi
        ja     find_space_done

find_space_loop:
        mov     (%ecx, %esi), %al
        addl    %eax , %ebx        # Add in the count, then move
        addl    $2, %esi                  # forward to the next count!

        cmpl    12(%ebp), %esi
        jl      find_space_loop

find_space_done:

        # Write the length of the decoded output to the output-variable
        movl    16(%ebp), %edx    # edx = last pointer-argument to function
        movl    %ebx, (%edx)      # store computed size into this location

        # Allocate memory for the decoded data using malloc.
        # Pointer to allocated memory will be returned in %eax.
        push    %ebx              # Number of bytes to allocate...
        call    malloc
        addl    $4, %esp          # Clean up stack after call.
        movl    8(%ebp), %ecx

        # Now, decode the data from the input buffer into the output buffer.
        xor     %esi, %esi
        xor     %edi, %edi

        # First comparison of decode while-loop here...
        cmpl    12(%ebp), %esi
        jge     decode_done

decode_loop:
        # Pull out the next [count][value] pair from the encoded data.
        mov     (%ecx, %esi), %bh         # bh is the count of repetitions
        mov     1(%ecx, %esi), %bl        # bl is the value to repeat

write_loop:
        mov     %bl, (%eax, %edi)
        incl    %edi
        sub     $1, %bh
        jnz     write_loop

        addl    $2, %esi

        cmpl    12(%ebp), %esi
        jl       decode_loop

decode_done:

        # Restore callee-save registers.
        pop     %edi
        pop     %esi
        pop     %ebx

        # Clean up stack frame.
        mov     %ebp, %esp
        pop     %ebp

        ret

