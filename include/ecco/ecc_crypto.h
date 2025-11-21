#ifndef ECC_CRYPTO_H
#define ECC_CRYPTO_H

#include <stddef.h>

unsigned char* ecnrypt(unsigned char* buff, size_t buff_len, char* kdf_hash);

unsigned char* decrypt(unsigned char* buff, size_t* return_len, char* kdf_hash);

#endif