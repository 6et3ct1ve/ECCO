#include "test.h"
#include <stdio.h>

int main() {
  /*
  toy implementation of RSA. Don't put in too big values cuz it'll
  take ages.

  P and Q values have to be changed for different key
  generation but they have to be primes.

  val is the value to be encrypted. It can't be bigger then p * q.

  private key is private exponent & modulo(max)

  public key os public exponent & modulo
  */

  long long p = 3089;
  long long q = 2011;
  long long val = 15;
  long long max = p * q;
  long long private_exponent;
  long long public_exponent;
  generate_keys(p, q, &max, &public_exponent, &private_exponent);

  printf("debug values\nvalue of max, or modulo: %lld\npublic exp:"
         "              %lld\nprivate exp:             %lld\nvalue before "
         "encryption: "
         "%lld\n",
         max, public_exponent, private_exponent, val);
  RSA_encrypt(&val, max, public_exponent);
  printf("value after encrypting:  %lld\n", val);
  RSA_decrypt(&val, max, private_exponent);
  printf("value after decrypting:  %lld\n", val);
}