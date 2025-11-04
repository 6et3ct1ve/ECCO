#ifndef MATH_MODULE_H
#define MATH_MODULE_H

#include <gmp.h>

unsigned long long egcd_legacy(unsigned long long a, unsigned long long b,
                               long int *x, long int *y);

void egcd_mpz(mpz_t gcd, mpz_t a, mpz_t b, mpz_t x, mpz_t y);
void find_mmi(mpz_t denominator, mpz_t modulo);
void set_global_modulo(mpz_t mod);
void modulo_eval(mpz_t num);

#endif