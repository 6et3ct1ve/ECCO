#include <stddef.h>
#include "aes_wrapper.h"

unsigned char* ecnrypt(unsigned char* buff, size_t buff_len, char* kdf_hash) {
    return (unsigned char*)aes_encrypt_gcm_hex(buff, buff_len, kdf_hash);
}

unsigned char* decrypt(unsigned char* buff, size_t* return_len, char* kdf_hash) {
    return aes_decrypt_gcm_hex((char*)buff, kdf_hash, return_len);
}
