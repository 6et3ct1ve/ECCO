#ifndef INPUT_SERVICE_H
#define INPUT_SERVICE_H

#include "key_generation.h"
#include <stddef.h>
#include <stdio.h>

typedef enum {
    ERROR,
    KEYGEN,
    ENCRYPT,
    DECRYPT
} action;

struct arguments {
    action action;
    char* source_file;
    char* result_file;
    char* curve;
    char* pub_key_file;
    char* priv_key_file;
};

void get_arguments(struct arguments* args, int argc, char** argv);
// returns 0 if something bad occurs
int read_keyfile(char* filename, struct keyring* keyring, struct curve* curve);
// returns the ammount of read data
int read_uncrypt_msg(FILE* fp, unsigned char* buff, size_t buff_len);
// returns 0 if something bad occurs
int read_encmsg_head(FILE* fp, struct keyring* keyring, struct curve* curve);
// returns the ammount of read data
int read_encmsg_body(FILE* fp, unsigned char* buff, size_t buff_len);

void print_arguments(struct arguments* args);

#endif
