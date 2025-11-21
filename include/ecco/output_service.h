#ifndef OUTPUT_SERVICE_H
#define OUTPUT_SERVICE_H

#include "curves.h"
#include "key_generation.h"
#include <stddef.h>
#include <stdio.h>

int write_decrypted_msg(FILE* fp, unsigned char* msg, size_t msg_len);

int write_encrypted_msg(FILE* fp, unsigned char* msg, size_t msg_len, char* curve_name, struct point* eph_pub_key);

int write_key(FILE* fp, struct keyring* keyring);

#endif
