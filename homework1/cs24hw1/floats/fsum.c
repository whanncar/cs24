#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "ffunc.h"


/* This function takes an array of single-precision floating point values,
 * and computes a sum in the order of the inputs.  Very simple.
 */
float fsum(FloatArray *floats) {
    float sum = 0;
    int i;

    for (i = 0; i < floats->count; i++)
        sum += floats->values[i];

    return sum;
}


/* TODO:  IMPLEMENT my_fsum() HERE, AND DESCRIBE YOUR APPROACH. */

/*
 *  I implemented Python's fsum algorithm.
 *
 *  It works by keeping track of error terms so that
 *  there is no loss until the final sum at the end.
 *
 *
 *  My implementation runs as follows:
 *
 *
 *  Keep a list of "partial sums" which is initially empty
 *
 *  For each element x in the original list
 *
 *    Initialize a temporary local sum "remain" with the value x
 *
 *    Make a new partial sums list
 *
 *    For each element y in the old partial sums list
 *
 *      Add y to remain
 *
 *      If the result has an error due to floating point representation
 *
 *        Store the error in the new partial sums list
 *
 *    Store remain in the new partial sums list
 *
 *    Replace the old partial sums list by the new one
 *
 *  Add the partial sums in order and return the result
 *
 */

float my_fsum(FloatArray *floats) {

  /* This will hold the final sum */
  float sum;
  /* Pairwise sum */
  float pair_sum;
  /* Compensation */
  float comp;
  /* Value with smaller absolute value */
  float smaller;
  /* Value with bigger absolute value */
  float bigger;
  /* Portion of sum that remains to be dealt with */
  float remain;

  int i;
  int j;

  /* Effective size of the partial_sum array */
  int size_of_partial_sum = 0;

  /* Index in partial_sum array where next value should be stored */
  int partial_sum_index;

  /* Absolute value of remain */
  float remain_abs;
  /* Absolute value of partial_sum[j] */
  float partial_sum_j_abs;


  float *partial_sum = (float *) calloc(floats->count, sizeof(float));

  for (i = 0; i < floats->count; i++) {

    remain = floats->values[i];

    partial_sum_index = 0;

    for (j = 0; j < size_of_partial_sum; j++) {

      remain_abs = remain;
      partial_sum_j_abs = partial_sum[j];

      if (remain < 0) {

        remain_abs = -remain;

      }

      if (partial_sum[j] < 0) {

        partial_sum_j_abs = -partial_sum[j];

      }

      if (remain_abs < partial_sum_j_abs) {

        smaller = remain;
        bigger = partial_sum[j];

      }

      else {

        smaller = partial_sum[j];
        bigger = remain;

      }

      pair_sum = smaller + bigger;
      comp = smaller - (pairwise_sum - bigger);

      if (comp) {

        partial_sum[partial_sum_index] = comp;
        partial_sum_index++;

      }

      remain = pair_sum;

    }

    partial_sum[partial_sum_index] = remain;

    size_of_partial_sum = partial_sum_index + 1;

  }

  sum = 0;

  for (i = 0; i < size_of_partial_sum; i++) {

    sum += partial_sum[i];

  }

  free(partial_sum);

  return sum;

}


int main() {
    FloatArray floats;
    float sum1, sum2, sum3, my_sum;

    load_floats(stdin, &floats);
    printf("Loaded %d floats from stdin.\n", floats.count);

    /* Compute a sum, in the order of input. */
    sum1 = fsum(&floats);

    /* Use my_fsum() to compute a sum of the values.  Ideally, your
     * summation function won't be affected by the order of the input floats.
     */
    my_sum = my_fsum(&floats);

    /* Compute a sum, in order of increasing magnitude. */
    sort_incmag(&floats);
    sum2 = fsum(&floats);

    /* Compute a sum, in order of decreasing magnitude. */
    sort_decmag(&floats);
    sum3 = fsum(&floats);

    /* %e prints the floating-point value in full precision,
     * using scientific notation.
     */
    printf("Sum computed in order of input:  %e\n", sum1);
    printf("Sum computed in order of increasing magnitude:  %e\n", sum2);
    printf("Sum computed in order of decreasing magnitude:  %e\n", sum3);

    printf("My sum:  %e\n", my_sum);
    

    return 0;
}

