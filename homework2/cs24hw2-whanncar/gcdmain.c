#include <stdio.h>
#include <stdlib.h>
#include "gcd.h"

char wrong_number_of_arguments[] = "You did not provide the right number "
                                   "of arguments.\nYou should provide two "
                                   "nonnegative integer arguments.";

char first_argument_negative[] = "Your first argument was negative.\nYou "
                                 "should provide two nonnegative integer "
                                 "arguments.";

char second_argument_negative[] = "Your second argument was negative.\nYou "
                                  "should provide two nonnegative integer "
                                  "arguments.";

int main (int argc, char *argv[]) {

  int x, y, smaller_value, bigger_value;

  if (argc != 3) {

    printf("%s\n", wrong_number_of_arguments);

    return 0;

  }

  x = atoi(argv[1]);
  y = atoi(argv[2]);

  if (x < 0) {

    printf("%s\n", first_argument_negative);

    return 0;

  }

  if (y < 0) {

    printf("%s\n", second_argument_negative);

    return 0;

  }

  if (x < y) {

    smaller_value = x;
    bigger_value = y;

  }

  else {

    smaller_value = y;
    bigger_value = x;

  }

  printf("gcd(%d,%d) = %d\n", x, y, gcd(bigger_value, smaller_value));

  return 0;

}
