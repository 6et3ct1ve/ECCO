#include "../include/ecco/ecco.h"
#include <gmp.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int main(int argc,char** argv) {
    size_t out_chars;
    char* salt = "";
    mpz_t somenum;
    mpz_init_set_str(somenum, "1991", 10);
    char* msg = "hello from melnfvvlf";
    char* kdf = kdf_hex_from_mpz(somenum, (unsigned char*)salt, 0, 256);
    printf("%s\n", kdf);

    unsigned char* msg_enc = ecnrypt((unsigned char*)msg, strlen(msg), kdf);
    unsigned char* msg_dec = decrypt(msg_enc, &out_chars, kdf);

    printf("orig msg: %s\necrypted msg: %s\ndecrypted msg: %s\n", msg, msg_enc, msg_dec);
}