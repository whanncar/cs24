#include <stdio.h>
#include <stdlib.h>
#include "fact.h"

char number_of_arguments_incorrect[] = "You did not give the correct number "
                                       "of arguments.\nThere should be one "
                                       "positive integer argument.";

char argument_is_negative[] = "You gave a negative integer argument.\n"
                              "You need to give one positive "
                              "integer argument.";

int main (int argc, char *argv[]) {

  int x;

  if (argc != 2) {

    printf("%s\n", number_of_arguments_incorrect);

    return 0;

  }

  x = atoi(argv[1]);

  if (x < 0) {

    printf("%s\n", argument_is_negative);

    return 0;

  }

  printf("fact(%d) = %d\n", x, fact(x));

  return 0;

}
