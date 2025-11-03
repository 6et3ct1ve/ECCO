#include <assert.h>
#include <gmp.h>

struct base_point_prototype {
  char *x;
  char *y;
};

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

struct curve_prototype {
  // needed to generate actual sctructs
  char *name;
  char *multiplicative_a;
  char *addendum_b;
  struct base_point_prototype base_point_G;
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

void curve_populate(struct curve *curve_ptr, char *curve_name) {
  // this function is basically a public interface to access curves
}