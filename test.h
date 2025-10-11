#ifndef GEN_ARRAY_H
#define GEN_ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
unsigned long long egcd(unsigned long long a, unsigned long long b, long int *x,
                        long int *y) {
  /* Expanded gcd
  the full big euclydean algorithm
  it allows you to calculate numbers with hard names
  but basically it uses repeated division with remainder a % b to find
  gcd and records all the steps. It then backtracks to find Bézout's identity
  coefisients which is math I understand only partially, but the main thing
  is that it fucking works. Kudos to w3schools for the code and explaination
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

unsigned long long gcd(unsigned long long a, unsigned long long b) {
  /* wrapper for egcd to return just the gcd */

  long int x = 1, y = 1;
  return egcd(a, b, &x, &y);
}

unsigned long long euler_totient_for_primes(int p, int q) {
  /* magic math shit. Calculates lcm(p - 1, q - 1).
  lcm is lowest common multiple*/
  unsigned long long mod = (p - 1) * (q - 1);
  return mod;
}

unsigned long long generate_exponents(unsigned long long fn,
                                      unsigned long long *d) {
  /* d is for private exponent */
  long int e;
  long int throwaway = 1;
  long int temp_d = 1;
  srand(time(NULL));
  do {
    e = rand() % fn;
  } while (egcd(e, fn, &temp_d, &throwaway) != 1);
  while (temp_d < 0) {
    temp_d += fn;
  }
  *d = temp_d;
  return e;
}

void generate_keys(unsigned long long p, unsigned long long q,
                   unsigned long long *maximum,
                   unsigned long long *public_exponent,
                   unsigned long long *private_exponent) {
  *maximum = p * q;
  unsigned long long fn = euler_totient_for_primes(p, q);
  *public_exponent = generate_exponents(fn, private_exponent);
}

void RSA_encrypt(unsigned long long *value, unsigned long long maximum,
                 int public_exponent) {
  unsigned long long base_num = *value;
  unsigned long long res = base_num;
  for (int i = 1; i < public_exponent; i++) {
    res = (res * base_num) % maximum;
  }
  *value = res;
}

void RSA_decrypt(unsigned long long *value, unsigned long long maximum,
                 int private_exponent) {
  unsigned long long base_num = *value;
  unsigned long long res = base_num;
  for (int i = 1; i < private_exponent; i++) {
    res = (res * base_num) % maximum;
  }
  *value = res;
}

#endif // GEN_ARRAY_H