#ifndef INPUT_SERVICE_H
#define INPUT_SERVICE_H

#include "key_generation.h"

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
    char* key_file;
};

void get_arguments(struct arguments* args, int argc, char** argv);

int read_keyfile(char* filename, struct keyring* keyring);

int read_uncrypted_msg(char* filename, unsigned char* buff);

int read_encrypted_msg(char* filename, unsigned char* buff, struct keyring* keyring);

#endif
