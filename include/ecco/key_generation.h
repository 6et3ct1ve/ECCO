#ifndef KEY_GEN_H
#define KEY_GEN_H

#include "curves.h"
#include "math_module.h"

struct keyring {
    int is_private;
    struct curve* curve;

    mpz_t key_priv;
    struct point key_pub;
    mpz_t key_priv_eph;
    struct point key_pub_eph;

    struct point shared_secret;
};

void set_keyring_private(struct keyring* keyring);

void set_keyring_public(struct keyring* keyring);

void init_keyring(struct keyring* keyring);

void generate_keyring(struct curve* curve, struct keyring* keyring);

int compute_shsecret(struct keyring* keyring);

void print_keyring(struct keyring* keyring);

#endif
