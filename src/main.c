#include "../include/ecco/ecco.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t BUFFER_SIZE = 1024;
size_t DKEY_SIZE = 64;
// double the  buffer size + tag size + IV size + 2 ':' = 2106
size_t ENC_BUFF_SIZE = (1024 * 2) + 32 + 24 + 2;

int handle_keygen(struct arguments* args, struct keyring* keyring, struct curve* curve) {
    set_keyring_private(keyring);
    generate_keyring(curve, keyring);
    printf("Done\nWriting keys to files\n");
    FILE* fp;
    if ((fp = fopen(args->priv_key_file, "w")) != NULL) {
        if (!write_key(fp, keyring)) {
            fclose(fp);
            return 0;
        }
        fclose(fp);
        printf("File written: %s\n", args->priv_key_file);
    } else {
        return 0;
    }

    set_keyring_public(keyring);
    if ((fp = fopen(args->pub_key_file, "w")) != NULL) {
        if (!write_key(fp, keyring)) {
            fclose(fp);
            return 0;
        }
        fclose(fp);
        printf("File written: %s\n", args->pub_key_file);
    } else {
        return 0;
    }
    return 1;
}

int handle_encryption(struct arguments* args, struct keyring* keyring, struct curve* curve) {
    generate_keyring(curve, keyring);
    printf("Done\nComputing shared secret\n");
    
    compute_shsecret(keyring);
    printf("Done\nDeriving key\n");
    
    char* shared_key = kdf_hex_from_mpz(keyring->shared_secret.x, (unsigned char*)"", 0, DKEY_SIZE);
    printf("Done\nOpenning files\n");

    FILE* src = fopen(args->source_file, "r");
    FILE* dst = fopen(args->result_file, "w");
    
    if (src == NULL) {
        perror("Could not open source file. Wrapping up execution");
        exit(EXIT_FAILURE);
    } else if (dst ==  NULL) {
        perror("Could not open destination file. Wrapping up execution");
        exit(EXIT_FAILURE);
    }

    printf("Done\n");
    unsigned char buff[BUFFER_SIZE];
    size_t bytes_read;
    size_t count = 1;

    while ((bytes_read = read_uncrypt_msg(src, buff, BUFFER_SIZE)) > 0) {
        unsigned char* encrypted_buff = ecnrypt(buff, bytes_read, shared_key);
        
        if (!(write_encrypted_msg(dst, encrypted_buff, strlen(encrypted_buff), keyring->curve->name, &keyring->key_pub_eph))) {
            return 0;
        }
    }
    fclose(src);
    fclose(dst);
    return 1;
}

int handle_decryption(struct arguments* args, struct keyring* keyring, struct curve* curve) {
    FILE* src = fopen(args->source_file, "r");
    FILE* dst = fopen(args->result_file, "w");
    unsigned char buff[ENC_BUFF_SIZE];
    size_t bytes_read;

    if (src == NULL) {
        perror("Could not open source file. Wrapping up execution");
        exit(EXIT_FAILURE);
    } else if (dst ==  NULL) {
        perror("Could not open destination file. Wrapping up execution");
        exit(EXIT_FAILURE);
    }
    if (!read_encmsg_head(src, keyring, curve)) {
        perror("Could not parse message header. Wrapping up execution");
        exit(EXIT_FAILURE);
    }
    compute_shsecret(keyring);
    char* shared_key = kdf_hex_from_mpz(keyring->shared_secret.x, (unsigned char*)"", 0, DKEY_SIZE);

    while ((bytes_read = read_encmsg_body(src, buff, ENC_BUFF_SIZE)) > 0) {
        size_t decr_len;
        unsigned char* decr_msg = decrypt(buff, &decr_len, shared_key);
        if (!write_decrypted_msg(dst, decr_msg, decr_len)) {
            return 0;
        }
    }
    fclose(src);
    fclose(dst);
    return 1;
}

int main(int argc,char** argv) {
    printf("ECCO is starting\n");

    struct arguments args;
    struct keyring keyring;
    struct curve curve;

    get_arguments(&args, argc, argv);
    init_keyring(&keyring);

    switch (args.action) {
        case ERROR:
            perror("something wrong with arguments\n");
            exit(EXIT_FAILURE);
            break;
        case KEYGEN:
            printf("Program is set to generate keys\nPopulating curve\n");
            if (!curve_populate(&curve, args.curve)) {
                perror("Curve error: check if curve you request exists");
                exit(EXIT_FAILURE);
            }
            keyring.curve = &curve;
            printf("Done\n");
            break;
        case ENCRYPT:
            printf("Program is set to encryption\nPopulating curve\n");
            set_keyring_public(&keyring);
            if (!read_keyfile(args.pub_key_file, &keyring, &curve)) {
                perror("Key error: check if you inputed correct key");
                exit(EXIT_FAILURE);
            }
            printf("Done\n");
            break;
        case DECRYPT:
            printf("Program is set to decryption\nPopulating curve\n");
            set_keyring_private(&keyring);
            if (!read_keyfile(args.priv_key_file, &keyring, &curve)) {
                perror("Key error: check if you inputed correct key");
                exit(EXIT_FAILURE);
            }
            printf("Done\n");
            break;
    }
    
    // print_keyring(&keyring);
    set_global_modulo(curve.modulus_p);

    switch (args.action) {
        case KEYGEN:
            printf("Generating keys\n");
            if (handle_keygen(&args, &keyring, &curve)) {
                printf("Done generating keys. Wrapping up execution\n");
                exit(EXIT_SUCCESS);
            } else {
                perror("Something went wrong. Wrapping up execution");
                exit(EXIT_FAILURE);
            };
        case ENCRYPT:
            printf("Generating ephemeral keys\n");
            if (handle_encryption(&args, &keyring, &curve)) {
                printf("Done encrypting. Wrapping up execution\n");
                exit(EXIT_SUCCESS);
            } else {
                perror("Something went wrong. Wrapping up execution");
                exit(EXIT_FAILURE);
            }
        case DECRYPT:
            printf("Extracting key and cyphertext from message\n");
            if (handle_decryption(&args, &keyring, &curve)) {
                printf("Done decrypting. Wrapping up execution\n");
                exit(EXIT_SUCCESS);
            } else {
                perror("Something went wrong. Wrapping up execution");
                exit(EXIT_FAILURE);
            }
    }
}