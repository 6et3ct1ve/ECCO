#ifndef AES_WRAPPER_H
#define AES_WRAPPER_H

#include <stddef.h>

/*
    AES-256-GCM encrypt.
    key_hex must have at least 64 hex chars (32 bytes).
    Returns malloc'ed string in format "iv_hex:cipher_hex:tag_hex".
    Caller must free().
*/
char *aes_encrypt_gcm_hex(const unsigned char *plaintext, size_t pt_len,
                          const char *key_hex);

                          
/*
    AES-256-GCM decrypt.
    Expects "iv_hex:cipher_hex:tag_hex".
    Returns malloc'ed plaintext (binary), size in *out_len.
    NULL on error (bad key or auth fail).
*/
unsigned char *aes_decrypt_gcm_hex(const char *pkg,
                                   const char *key_hex,
                                   size_t *out_len);

#endif