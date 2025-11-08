#ifndef KDF_WRAPPER_H
#define KDF_WRAPPER_H

#include <stddef.h>
#include <gmp.h>

/*
    HKDF-SHA256 wrapper.
    Returns malloc'ed hex string with given out_hex_chars length.
    salt can be NULL. Caller must free().
*/
char *kdf_hex(const void *data, size_t data_len,
              const unsigned char *salt, size_t salt_len,
              size_t out_hex_chars);

/*
    Same as above but input is mpz_t.
    Useful when the secret is a big integer from ECC math.
*/
char *kdf_hex_from_mpz(const mpz_t val,
                       const unsigned char *salt, size_t salt_len,
                       size_t out_hex_chars);

#endif