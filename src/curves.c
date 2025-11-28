#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gmp.h>
#include "curves.h"
struct base_point {
  char *x;
  char *y;
};

struct curve_prototype {
  // needed to generate actual sctructs
  char *name;
  char *multiplicative_a;
  char *addendum_b;
  struct base_point point;
  char *modulus_p;
  char *field_size_n;
};

struct curve_prototype CURVE_LIST[] = {
    {"P-256",
     "0xffffffff00000001000000000000000000000000fffffffffffffffffffffffc",
     "0x5ac635d8aa3a93e7b3ebbd55769886bc651d06b0cc53b0f63bce3c3e27d2604b",
     "0x6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296",
     "0x4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5",
     "0xffffffff00000001000000000000000000000000ffffffffffffffffffffffff",
     "0xffffffff00000000ffffffffffffffffbce6faada7179e84f3b9cac2fc632551"},

    {"P-384",
     "0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffff"
     "f0000000000000000fffffffc",
     "0xb3312fa7e23ee7e4988e056be3f82d19181d9c6efe8141120314088f5013875ac656398"
     "d8a2ed19d2a85c8edd3ec2aef",
     "0xaa87ca22be8b05378eb1c71ef320ad746e1d3b628ba79b9859f741e082542a385502f25"
     "dbf55296c3a545e3872760ab7",
     "0x3617de4a96262c6f5d9e98bf9292dc29f8f41dbd289a147ce9da3113b5f0b8c00a60b1c"
     "e1d7e819d7a431d7c90ea0e5f",
     "0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffff"
     "f0000000000000000ffffffff",
     "0xffffffffffffffffffffffffffffffffffffffffffffffffc7634d81f4372ddf581a0db"
     "248b0a77aecec196accc52973"},
};

void _init_mpz_with_str(char *str, mpz_t *mpz) {
  mpz_init(*mpz);
  // sets the string and tests value. If not 0 - exits
  // o as a base (third arg) means to use leaing chars (0x, 0b), to define base
  // of a number
  assert(!(mpz_set_str(*mpz, str, 0)));
}

int curve_populate(struct curve *curve_ptr, char *curve_name) {
  // this function is basically a public interface to access curves

  size_t curve_list_size = sizeof(CURVE_LIST) / sizeof(struct curve_prototype);

  for (size_t i = 0; i < curve_list_size; i++) {
    if (strcmp(curve_name, CURVE_LIST[i].name) == 0) {
      (*curve_ptr).name = curve_name;
      _init_mpz_with_str(CURVE_LIST[i].multiplicative_a,
                         &(*curve_ptr).multiplicative_a);
      _init_mpz_with_str(CURVE_LIST[i].addendum_b, &(*curve_ptr).addendum_b);
      _init_mpz_with_str(CURVE_LIST[i].point.x, &(*curve_ptr).base_point_G.x);
      _init_mpz_with_str(CURVE_LIST[i].point.y, &(*curve_ptr).base_point_G.y);
      _init_mpz_with_str(CURVE_LIST[i].modulus_p, &(*curve_ptr).modulus_p);
      _init_mpz_with_str(CURVE_LIST[i].field_size_n,
                         &(*curve_ptr).field_size_n);
      return 1;
    }
  }
  return 0;
}

void curve_print(struct curve *curve_ptr) {
  // mpz_out_str(stdout, 10, curve.addendum_b);
  gmp_printf(
      "name = %s\na = %Zd\nb = %Zd\nGx = %Zd\nGy = %Zd\np = %Zd\nn = %Zd",
      curve_ptr->name, curve_ptr->multiplicative_a, curve_ptr->addendum_b,
      curve_ptr->base_point_G.x, curve_ptr->base_point_G.y, curve_ptr->modulus_p,
      curve_ptr->field_size_n);
}