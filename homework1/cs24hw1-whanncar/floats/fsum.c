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
 *  It works by keeping track of partial sums so that there is no loss until the final sum at the end.
 *
 *
 *  My implementation runs as follows:
 *
 *
 *  Keep a list of "partial sums" which is initially empty
 *
 *  For each element x in the array to be added
 *
 *    Set portion_to_still_be_added equal to x
 *
 *    For each element y in the list of partial sums
 *
 *      Add portion_to_still_be_added and y and store the result in pairwise_sum
 *      
 *      Compute which of portion_to_still_be_added and y is larger in absolute value, store that value in value_with_bigger_magnitude, and store the other value in value_with_smaller_magnitude
 *
 *      Subtract the value with larger absolute value from pairwise_sum, then subtract the result from the value with smaller absolute value, and store this into compensation.
 *
 *      If compensation is non_zero, store the value in partial_sums
 *
 *      Set portion_to_still_be_added equal to pairwise_sum
 *      
 *
 *    Store whatever is left in portion_to_still_be_added in partial_sums
 *
 *  Add all of the elements of partial_sums in order and return this value
 *
 */

float my_fsum(FloatArray *floats) {
    
	float sum;
	float pairwise_sum;
	float compensation;
	float value_with_smaller_magnitude;
	float value_with_bigger_magnitude;
	float portion_to_still_be_added;

	int i;
	int j;
	int size_of_partial_sum = 0;		/*This keeps track of the "current length" of the partial sum list*/
	int position_to_record_partial_sum;	/*This keeps track of the next "empty" cell in the partial sum list*/

	float absolute_value_of_portion_to_still_be_added;
	float absolute_value_of_partial_sum_j;


	float *partial_sum = (float *) calloc(floats->count,sizeof(float));

	for(i = 0; i < floats->count; i++){

		/*Set portion to still be added equal to the current value in floats.values*/

		portion_to_still_be_added = floats->values[i];
		
		/*Set location of next "empty" cell to 0*/

		position_to_record_partial_sum = 0;

		for(j = 0; j < size_of_partial_sum; j++){

			absolute_value_of_portion_to_still_be_added = portion_to_still_be_added;
			absolute_value_of_partial_sum_j = partial_sum[j];

			if(portion_to_still_be_added < 0){

				absolute_value_of_portion_to_still_be_added = -portion_to_still_be_added;

			}

			if(partial_sum[j] < 0){

				absolute_value_of_partial_sum_j = -partial_sum[j];

			}



			/*Store which of portion_to_still_be_added and partial_sum[j] is larger and which is smaller*/

			if(absolute_value_of_portion_to_still_be_added < absolute_value_of_partial_sum_j){

				value_with_smaller_magnitude = portion_to_still_be_added;
				value_with_bigger_magnitude = partial_sum[j];

			}

			else{

				value_with_smaller_magnitude = partial_sum[j];
				value_with_bigger_magnitude = portion_to_still_be_added;

			}

			/*Compute the float sum of portion_to_still_be_added and partial_sum[j]*/

			pairwise_sum = value_with_smaller_magnitude + value_with_bigger_magnitude;
			
			/*Compute the error*/

			compensation = value_with_smaller_magnitude - (pairwise_sum - value_with_bigger_magnitude);



			/*If the there is any error*/

			if(compensation){

				/*Store the error in partial_sum*/

				partial_sum[position_to_record_partial_sum] = compensation;

				/*Increment the location of the next "empty" cell*/

				position_to_record_partial_sum++;
					

			}

			/*Save the float sum to portion_to_still_be_added*/
	
			portion_to_still_be_added = pairwise_sum;


		}

		/*Store the remaining value in portion_to_still_be_added in partial_sum*/

		partial_sum[position_to_record_partial_sum] = portion_to_still_be_added;

		/*Store the current "size" of the list of partial sums*/

		size_of_partial_sum = position_to_record_partial_sum + 1;

	}

	/*Add the values of the partial sums in order*/

	sum = 0;

	for(i = 0; i < size_of_partial_sum; i++){

		sum += partial_sum[i];

	}


	free(partial_sum);

	/*Return*/
			
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

