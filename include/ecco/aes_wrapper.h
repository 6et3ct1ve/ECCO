#ifndef AES_WRAPPER_H
#define AES_WRAPPER_H

#include <stddef.h>

char *aes_encrypt_gcm_hex(const unsigned char *plaintext, size_t pt_len,
                          const char *key_hex);

unsigned char *aes_decrypt_gcm_hex(const char *pkg, const char *key_hex,
                                   size_t *out_len);

#endif