#include "math_module.h"
#include "key_generation.h"
#include <stdio.h>
#include <gmp.h>
#include <time.h>

void init_seed_state(gmp_randstate_t* state) {
    gmp_randinit_default(*state);
    unsigned long int seed;
    FILE* fp;
    if ((fp = fopen("/dev/urandom", "r")) != NULL) {
        fread((unsigned char*)&seed, sizeof(unsigned long int), 1, fp);
        fclose(fp);
        gmp_randseed_ui(*state, seed);
    } else {
        gmp_randseed_ui(*state, time(NULL));
    }
}

void init_keyring(struct keyring* keyring) {
    mpz_inits(keyring->key_priv, keyring->key_pub.x, keyring->key_pub.y, 
        keyring->key_priv_eph, keyring->key_pub_eph.x, keyring->key_pub_eph.y, 
        keyring->shared_secret.x, keyring->shared_secret.y, NULL);
}

void set_keyring_private(struct keyring* keyring) {
    keyring->is_private = 1;
}

void set_keyring_public(struct keyring* keyring) {
    keyring->is_private = 0;
}

void generate_keyring(struct curve* curve, struct keyring* result_keyring) {
    result_keyring->curve = curve;
    gmp_randstate_t state;
    init_seed_state(&state);

    if (result_keyring->is_private) {
        mpz_urandomm(result_keyring->key_priv, state, result_keyring->curve->field_size_n);
        
        point_mult(&(result_keyring->key_pub), &(result_keyring->curve->base_point_G), 
        result_keyring->key_priv, curve);
    } else if (!result_keyring->is_private) {
        mpz_urandomm(result_keyring->key_priv_eph, state, result_keyring->curve->field_size_n);

        point_mult(&(result_keyring->key_pub_eph), &(result_keyring->curve->base_point_G), 
        result_keyring->key_priv_eph, curve);
    }
    
    gmp_randclear(state);
}

int compute_shsecret(struct keyring* keyring) {
    if (keyring->is_private) {
        point_mult(&(keyring->shared_secret), &(keyring->key_pub_eph), keyring->key_priv, keyring->curve);
        return 1;
    } else if (!keyring->is_private) {
        point_mult(&(keyring->shared_secret), &(keyring->key_pub), keyring->key_priv_eph, keyring->curve);
        return 1;
    }
    return 0;
}

void print_keyring(struct keyring* keyring) {
    gmp_printf("keyring\nprivate: %s\nprivate key: %Zd\npublic key: (%Zd, %Zd)\neph private key: %Zd\neph public key: (%Zd, %Zd)\nshared secret: (%Zd, %Zd)\n\ncurve\n", 
    keyring->is_private ? "true" : "false", keyring->key_priv, keyring->key_pub.x, keyring->key_pub.y, keyring->key_priv_eph, keyring->key_pub_eph.x, 
    keyring->key_pub_eph.y, keyring->shared_secret.x, keyring->shared_secret.y);
    curve_print(keyring->curve);
    printf("\n");
}