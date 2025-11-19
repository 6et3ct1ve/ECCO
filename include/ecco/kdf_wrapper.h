#ifndef KDF_WRAPPER_H
#define KDF_WRAPPER_H

#include <gmp.h>
#include <stddef.h>

char *kdf_hex(const void *data, size_t data_len, const unsigned char *salt,
              size_t salt_len, size_t out_hex_chars);

char *kdf_hex_from_mpz(const mpz_t val, const unsigned char *salt,
                       size_t salt_len, size_t out_hex_chars);

#endif