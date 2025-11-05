#include "../include/ecco/ecco.h"
#include <gmp.h>

mpz_t VARIABLE_GLOBAL_MODULO;

void _debug_print_mpz(mpz_t num) { gmp_printf("%Zd\n", num); }

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

void find_mmi(mpz_t denom) {
  // mmi stands for modular multiplicative inverse. Needed for correct division
  mpz_t mmi, aux, gcd;
  mpz_init(gcd);
  mpz_init_set_d(mmi, 1);
  mpz_init_set_d(aux, 1);

  egcd_mpz(gcd, denom, VARIABLE_GLOBAL_MODULO, mmi, aux);
  modulo_eval(mmi);
  mpz_set(denom, mmi);

  mpz_clear(mmi);
  mpz_clear(aux);
  mpz_clear(gcd);
}

void set_global_modulo(mpz_t mod) {
  // small setter for modulo. Should be called at least once before modulo_eval
  mpz_init_set(VARIABLE_GLOBAL_MODULO, mod);
}

void modulo_eval(mpz_t num) {
  // just a wrapper
  mpz_mod(num, num, VARIABLE_GLOBAL_MODULO);
}

void calculate_slope(struct point *point_1, struct point *point_2,
                     mpz_t multiplicative, mpz_t slope) {
  // λ signifies slope
  // λ = (x2​ − x1​) / (y2 ​− y1​)​ (mod p) for normal addition
  // λ = (3 * x1^2 + a)/(2 * y1) for doubling​

  if (!mpz_cmp(point_1->x, point_2->y) && !mpz_cmp(point_1->y, point_2->y)) {
    // case for doubling
    mpz_t tmp_upper, tmp_lower;

    mpz_init_set(tmp_upper, point_1->x);
    mpz_mul(tmp_upper, tmp_upper, tmp_upper);
    mpz_mul_si(tmp_upper, tmp_upper, 3);
    mpz_add(tmp_upper, tmp_upper, multiplicative);
    modulo_eval(tmp_upper);

    mpz_init_set(tmp_lower, point_1->y);
    mpz_mul_si(tmp_lower, tmp_lower, 2);
    modulo_eval(tmp_lower);

    find_mmi(tmp_lower);
    mpz_mul(slope, tmp_upper, tmp_lower);
    modulo_eval(slope);

    mpz_clear(tmp_lower);
    mpz_clear(tmp_upper);
    return;
  } else {
    // case for normal addition
    mpz_t tmp_upper, tmp_lower;

    mpz_init_set(tmp_upper, point_2->y);
    mpz_sub(tmp_upper, tmp_upper, point_1->y);

    mpz_init_set(tmp_lower, point_2->x);
    mpz_sub(tmp_lower, tmp_lower, point_1->x);

    find_mmi(tmp_lower);
    mpz_mul(slope, tmp_upper, tmp_lower);
    modulo_eval(slope);

    mpz_clear(tmp_lower);
    mpz_clear(tmp_upper);
    return;
  }
}

void point_addition(struct point *output_R, struct point *point_P,
                    struct point *point_Q, struct curve curve) {
  mpz_t slope, slope_sqr;
  mpz_init(slope);
  mpz_init(slope_sqr);
  calculate_slope(point_P, point_Q, curve.multiplicative_a, slope);
  mpz_mul(slope_sqr, slope, slope);

  // x3​ = λ^2 − x1 ​− x2​
  mpz_sub(output_R->x, slope_sqr, point_P->x);
  mpz_sub(output_R->x, output_R->x, point_Q->x);
  modulo_eval(output_R->x);

  mpz_sub(output_R->y, point_P->x, output_R->x);
  mpz_mul(output_R->y, slope, output_R->y);
  mpz_sub(output_R->y, output_R->y, point_P->y);
  modulo_eval(output_R->y);
  mpz_clear(slope);
  mpz_clear(slope_sqr);
}