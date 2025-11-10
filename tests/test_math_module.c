
#include "../include/ecco/ecco.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <gmp.h>

int test_multiplication(struct point *initial_point, struct curve *curve) {
  struct point tmp_mult, tmp_add;
  mpz_t tmp_scalar;
  mpz_inits(tmp_mult.x, tmp_add.x, tmp_mult.y, tmp_add.y, tmp_scalar, NULL);
  point_addition(&tmp_add, initial_point, &tmp_add, curve);

  gmp_printf("------------------start add/mult testing------------------\n"
    "initial point: (%Zd, %Zd)\nadd initial point: (%Zd, %Zd)\n\n", initial_point->x,
  initial_point->y, tmp_add.x, tmp_add.y);

  for(size_t i = 2; i <= 20; i++) {
    mpz_set_si(tmp_scalar, i);

    point_mult(&tmp_mult, initial_point, tmp_scalar, curve);
    point_addition(&tmp_add, initial_point, &tmp_add, curve);

    gmp_printf("res for P%d\nmult: (%Zd, %Zd)\nadd: (%Zd, %Zd)\npassed: %s\n\n",
    i, tmp_mult.x, tmp_mult.y, tmp_add.x, tmp_add.y, 
    (mpz_cmp(tmp_mult.x, tmp_add.x) == 0 && mpz_cmp(tmp_mult.y, tmp_add.y) == 0) ? "true" : "false");
  }
  mpz_clears(tmp_mult.x, tmp_add.x, tmp_mult.y, tmp_add.y, tmp_scalar, NULL);
}

int main() {
  srand(time(NULL));
  unsigned long long a, b, gcd;
  long int x = 1, y = 1;
  mpz_t a_mpz, b_mpz, x_mpz, y_mpz, gcd_mpz;
  a = 4 /*rand() % 10000*/;
  b = 7 /*rand() % 10000*/;
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
             "result:\t%Zd, %Zd, %Zd, %Zd, %Zd\n\n",
             gcd, a, b, x, y, gcd_mpz, a_mpz, b_mpz, x_mpz, y_mpz);

  struct curve c1 = {
      "c1",
  };
  mpz_init_set_d(c1.multiplicative_a, 2);
  mpz_init_set_d(c1.addendum_b, 3);
  mpz_init_set_d(c1.base_point_G.x, 3);
  mpz_init_set_d(c1.base_point_G.y, 6);
  mpz_init_set_d(c1.modulus_p, 97);
  mpz_init_set_d(c1.field_size_n, 100);

  struct point point_Q;
  mpz_init_set_d(point_Q.x, 28);
  mpz_init_set_d(point_Q.y, 63);

  struct point res_R;
  mpz_init(res_R.x);
  mpz_init(res_R.y);

  set_global_modulo(c1.modulus_p);

  curve_print(&c1);

  point_addition(&res_R, &c1.base_point_G, &point_Q, &c1);

  gmp_printf("\n\nx: %Zd\ny: %Zd\n", res_R.x, res_R.y);

  test_multiplication(&point_Q, &c1);
}