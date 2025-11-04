
#include "../include/ecco/math_module.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <gmp.h>

int main() {
  srand(time(NULL));
  unsigned long long a, b, gcd;
  long int x = 1, y = 1;
  mpz_t a_mpz, b_mpz, x_mpz, y_mpz, gcd_mpz;
  a = rand() % 10000;
  b = rand() % 10000;
  mpz_init(gcd_mpz);
  mpz_init_set_d(a_mpz, a);
  mpz_init_set_d(b_mpz, b);
  mpz_init_set_d(x_mpz, x);
  mpz_init_set_d(y_mpz, y);
  printf("values\ta: %llu\tb: %llu\tx: %ld\ty: %ld\n", a, b, x, y);
  gcd = egcd_legacy(a, b, &x, &y);
  printf("complete: %llu\n", gcd);
  egcd_mpz(gcd_mpz, a_mpz, b_mpz, x_mpz, y_mpz);
  gmp_printf("complete: %Zd\n", gcd_mpz);
  gmp_printf("egcd_legacy result:\t%llu, %llu, %llu, %ld, %ld\negcd_mpz "
             "result:\t%Zd, %Zd, %Zd, %Zd, %Zd\n",
             gcd, a, b, x, y, gcd_mpz, a_mpz, b_mpz, x_mpz, y_mpz);
}