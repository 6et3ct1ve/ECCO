#include <gmp.h>

mpz_t VARIABLE_GLOBAL_MODULO;

unsigned long long egcd_legacy(unsigned long long a, unsigned long long b,
                               long int *x, long int *y) {
  /* extended euclydean algorithm
  output: gcd of a and b
  x and y used to solve ax+by=gcd(a,b)
  x and y should be initialised to 1
  code is by w3schools
  good vid to understand: https://www.youtube.com/watch?v=fz1vxres5ts5I
  */

  if (a == 0) {
    *x = 0;
    *y = 1;
    return b;
  }
  long int x1;
  long int y1;
  unsigned long long gcd = egcd_legacy(b % a, a, &x1, &y1);
  *x = y1 - (b / a) * x1;
  *y = x1;
  return gcd;
}

char *_egcd_mpz(mpz_t a, mpz_t b, mpz_t x, mpz_t y) {
  // same as old egcd but for mpz values
  // returns decimal string because you cannot return mpz_t

  char *gcd_str;

  if (mpz_sgn(a) == 0) {
    mpz_t abs_b;
    mpz_set_d(x, 0);
    mpz_set_d(y, 1);
    mpz_init(abs_b);
    mpz_abs(abs_b, b);
    gcd_str = mpz_get_str(NULL, 10, abs_b);
    mpz_clear(abs_b);
    return gcd_str;
  }

  mpz_t x1, y1, res, remainder;

  mpz_init(remainder);
  mpz_init(res);
  mpz_init(x1);
  mpz_init(y1);
  // https://gmplib.org/manual/Integer-Division#index-mpz_005ftdiv_005fresr
  mpz_tdiv_qr(res, remainder, b, a);

  gcd_str = _egcd_mpz(remainder, a, x1, y1);

  mpz_mul(res, res, x1);
  mpz_sub(x, y1, res);
  mpz_set(y, x1);

  mpz_clear(x1);
  mpz_clear(y1);
  mpz_clear(remainder);
  mpz_clear(res);

  return gcd_str;
}

void egcd_mpz(mpz_t gcd, mpz_t a, mpz_t b, mpz_t x, mpz_t y) {
  mpz_set_str(gcd, _egcd_mpz(a, b, x, y), 10);
}

void find_mmi(mpz_t denominator, mpz_t modulo) {
  // mmi stands for modular multiplicative inverse. Needed for correct division
}

void set_global_modulo(mpz_t mod) {
  // small setter for modulo. Should be called at least once before modulo_eval
  mpz_init_set(VARIABLE_GLOBAL_MODULO, mod);
}

void modulo_eval(mpz_t num) {
  // just a wrapper
  mpz_mod(num, num, VARIABLE_GLOBAL_MODULO);
}