#include <stdio.h>

#define UNSIGNEDINTWITHLASTBIT1ANDALLOTHERS0 1

int count_onebits(unsigned int n);


int main(int argc, char **argv) {
    int i, res;
    unsigned int n;

    if (argc == 1) {
        printf("usage:  %s N1 [N2 ...]\n\n", argv[0]);
        printf("\tN1, N2, ... are positive integers\n\n");
        printf("\tFor each argument, the program prints out the number of\n"
               "\tbits that are 1 in the number.\n\n");
        return 1;
    }

    for (i = 1; i < argc; i++) {
        res = sscanf(argv[i], "%u", &n);
        if (res == 1)
            printf("Input:  %u\tOne-bits:  %u\n\n", n, count_onebits(n));
        else
            printf("Unparseable input \"%s\".  Skipping.\n\n", argv[i]);
    }

    return 0;
}

/*
 * Given an unsigned integer n, this function returns the number of bits in n
 * that are 1.
 */
int count_onebits(unsigned int n) {

    /*=================================*/
    /* TODO:  IMPLEMENT THIS FUNCTION. */
    /*=================================*/


  /* Count will keep count of the number of 1 bits we've seen. */

  int count = 0;

  do{

    /* Adds 1 to count if last bit is 1, 0 otherwise. */

    count += n & UNSIGNEDINTWITHLASTBIT1ANDALLOTHERS0;

  }

  /* Right shifts n by one bit. If n is still nonzero, 
   * executes do code.
   */

  while(n >>= 1);

  return count;

}
