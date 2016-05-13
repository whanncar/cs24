#include <stdlib.h>
#include <stdio.h>

int value;

int accum(int n) {

  value += n;
  return value;

}

int reset() {

  int old = value;
  value = 0;
  return old;

}

int main(void){

  int i, n;

  reset();

  for(i = 0; i < 10; i++){

    n = rand() % 1000;

  }

  printf("n = %d\taccum = %d\n", n, accum(n));

  return 0;

}
