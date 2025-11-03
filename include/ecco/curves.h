#ifndef SMART_CURVES_H
#define SMART_CURVES_H

#include <gmp.h>

struct base_point_G {
  mpz_t x;
  mpz_t y;
};
struct curve {
  // all the parameters of a curve
  char *name;
  mpz_t multiplicative_a;
  mpz_t addendum_b;
  struct base_point_G base_point_G;
  mpz_t modulus_p;
  mpz_t field_size_n;
};

int curve_populate(struct curve *curve_ptr, char *curve_name);
void curve_print(struct curve *curve_ptr);

#endif /* SMART_CURVES_H */