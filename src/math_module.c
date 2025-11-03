#include <gmp.h>

// TODO: redo egcd to use gmp
unsigned long long egcd(unsigned long long a, unsigned long long b, long int *x,
                        long int *y) {
  /* extended euclydean algorithm
  output: gcd of a and b
  x and y used to solve ax+by=gcd(a,b)
  x and y should be initialised to 1
  code is by w3schools
  good vid to understand: https://www.youtube.com/watch?v=fz1vxq5ts5I
  */

  if (a == 0) {
    *x = 0;
    *y = 1;
    return b;
  }
  long int x1;
  long int y1;
  unsigned long long gcd = egcd(b % a, a, &x1, &y1);
  *x = y1 - (b / a) * x1;
  *y = x1;
  return gcd;
}

void find_mmi(mpz_t denominator, mpz_t modulo) {
  // mmi stands for modular multiplicative inverse. Needed for division
}

void modulo() {}