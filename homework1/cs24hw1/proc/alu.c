/*! \file
 *
 * This file contains definitions for an Arithmetic/Logic Unit of an
 * emulated processor.
 */


#include <stdio.h>
#include <stdlib.h>   /* malloc(), free() */
#include <string.h>   /* memset() */

#include "alu.h"
#include "instruction.h"

#define WORDWITHTOPBIT1ANDALLOTHERBITS0 0x80000000
#define WORDWITHTOPBIT0ANDALLOTHERBITS1 0x7FFFFFFF

/*!
 * This function dynamically allocates and initializes the state for a new ALU
 * instance.  If allocation fails, the program is terminated.
 */
ALU * build_alu() {
    /* Try to allocate the ALU struct.  If this fails, report error then exit. */
    ALU *alu = malloc(sizeof(ALU));
    if (!alu) {
        fprintf(stderr, "Out of memory building an ALU!\n");
        exit(11);
    }

    /* Initialize all values in the ALU struct to 0. */
    memset(alu, 0, sizeof(ALU));
    return alu;
}


/*! This function frees the dynamically allocated ALU instance. */
void free_alu(ALU *alu) {
    free(alu);
}


/*!
 * This function implements the logic of the ALU.  It reads the inputs and
 * opcode, then sets the output accordingly.  Note that if the ALU does not
 * recognize the opcode, it should simply produce a zero result.
 */
void alu_eval(ALU *alu) {
    uint32_t A, B, aluop;
    uint32_t result;

    A = pin_read(alu->in1);
    B = pin_read(alu->in2);
    aluop = pin_read(alu->op);

    result = 0;

    /*======================================*/
    /* TODO:  Implement the ALU logic here. */
    /*======================================*/

  switch(aluop) {

  /* compute sum of inputs src1 and src2 */

  case ALUOP_ADD:
    result = A + B;
    break;

  /* compute word with 1 in each place that input
   * src1 has a 0 and 0 in each place that input
   * src1 has a 1
   */

  case ALUOP_INV:
    result = ~A;
    break;

  /* compute unsigned int difference of unsigned
   * int inputs src1 and src2
   */

  case ALUOP_SUB:
    result = A - B;
    break;

  /* compute bitwise XOR of inputs src1 and src2 */

  case ALUOP_XOR:
    result = A^B;
    break;

  /* compute bitwise OR of inputs src1 and src2 */

  case ALUOP_OR:
    result = A|B;
    break;

  /* compute unsigned int sum of unsigned int input
   * src1 and the number 1 (that is, compute result
   * of incrementing src1 by 1)
   */

  case ALUOP_INCR:
    result = A + 1;
    break;

  /* compute bitwise AND of inputs src1 and src2 */

  case ALUOP_AND:
    result = A&B;
    break;

  /* compute word which has as bit i the value of bit
   * i+1 in src1, then set bit 31 equal of result equal
   * to bit 31 of src1 (that is, compute arithmetic shift
   * right of src1)
   */

  case ALUOP_SRA:
    result = (A >> 1) | (A & ((uint32_t) WORDWITHTOPBIT1ANDALLOTHERBITS0));
    break;

  /* compute word which has as bit i the value of bit i+1
   * in src1, then clear bit 31 (that is, compute logical
   * shift right of src1)
   */

  case ALUOP_SRL:
    result = (A >> 1) & ((uint32_t) WORDWITHTOPBIT0ANDALLOTHERBITS1);
    break;

  /* compute word which has as bit i the value of bit i-1
   * in src1 for i > 0 and 0 as bit 0 (that is, compute
   * the arithmetic left shift of src1)
   */

  case ALUOP_SLA:
    result = A << 1;
    break;

  /* compute word which has as bit i the value of bit i-1
   * in src1 for i > 0 and 0 as bit 0 (that is, compute
   * the logical left shift of src1)
   */

  case ALUOP_SLL:
    result = A << 1;
    break;

  }

  pin_set(alu->out, result);
}










