#ifndef GEN_ARRAY_H
#define GEN_ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
long long egcd(long long a, long long b, long int *x, long int *y) {
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
  long int x1, y1;
  long long gcd = egcd(b % a, a, &x1, &y1);
  *x = y1 - (b / a) * x1;
  *y = x1;
  return gcd;
}

long long gcd(long long a, long long b) {
  /* wrapper for egcd to return just the gcd */

  long int x = 1, y = 1;
  return egcd(a, b, &x, &y);
}

long long euler_totient_for_primes(int p, int q) {
  /* magic math shit. Calculates lcm(p - 1, q - 1).
  lcm is lowest common multiple*/
  long long mod = (p - 1) * (q - 1);
  return mod;
}

long long generate_exponents(long long fn, long long *d) {
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

void generate_keys(long long p, long long q, long long *maximum,
                   long long *public_exponent, long long *private_exponent) {
  *maximum = p * q;
  long long fn = euler_totient_for_primes(p, q);
  *public_exponent = generate_exponents(fn, private_exponent);
}

void RSA_encrypt(long long *value, long long maximum, int public_exponent) {
  long long base_num = *value;
  long long res = base_num;
  for (int i = 1; i < public_exponent; i++) {
    res = (res * base_num) % maximum;
  }
  *value = res;
}

void RSA_decrypt(long long *value, long long maximum, int private_exponent) {
  long long base_num = *value;
  long long res = base_num;
  for (int i = 1; i < private_exponent; i++) {
    res = (res * base_num) % maximum;
  }
  *value = res;
}

#endif // GEN_ARRAY_H