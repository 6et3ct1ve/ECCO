#include <stddef.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <gmp.h>
#include "ecco.h"
#include "tinyargs.h"

void test_file_read(char* filename) {
    if (!access(filename, F_OK) && !access(filename, R_OK)) {
        return;
    } else {
        perror("access error: source file does not exist or cannot be accessed");
        exit(EXIT_FAILURE);
    }
}

void test_file_write(char* filename) {
    if (access(filename, F_OK) || !access(filename, W_OK)) {
        return;
    } else {
        perror("access error: output file cannot be accessed");
        exit(EXIT_FAILURE);
    }
}


void get_arguments(struct arguments* args, int argc, char** argv) {
    ArgTable argTable;
	InitArgTable(&argTable,argv,argc);

    NewArgument("-k", "private or public key file or name in case of generation", 
        StrArg(""), ARG_STRING, &argTable);
    NewArgument("-e", "file to be encrypted", 
        StrArg(""), ARG_STRING, &argTable);
    NewArgument("-d", "file to be decrypted", 
        StrArg(""), ARG_STRING, &argTable);
    NewArgument("-o", "output file", 
        StrArg(""), ARG_STRING, &argTable);

    Parse(&argTable);

    // if both -e and -d are set - something's wrong
    if (strlen(GetStrArg(&argTable, "-e")) 
    && strlen(GetStrArg(&argTable, "-d"))) {
        perror("invalid arguments: can't encrypt and decrypt at the same time");
        exit(EXIT_FAILURE);
    
    // if neither -e nor -d are set then we are generating keys
    } else if (!strlen(GetStrArg(&argTable, "-e")) 
    && !strlen(GetStrArg(&argTable, "-d"))) {
        args->action = KEYGEN;

        if (strlen(GetStrArg(&argTable, "-k"))) {
            args->curve = GetStrArg(&argTable, "-k");
        } else {
            args->curve = "P-256";
        }

        if (strlen(GetStrArg(&argTable, "-o"))) {
            args->pub_key_file = calloc(1, strlen(GetStrArg(&argTable, "-o")) + strlen(".pub") + 1);
            args->priv_key_file = calloc(1, strlen(GetStrArg(&argTable, "-o")) + strlen(".priv") + 1);
            
            sprintf(args->pub_key_file, "%s.pub", GetStrArg(&argTable, "-o"));
            sprintf(args->priv_key_file, "%s.priv", GetStrArg(&argTable, "-o"));
        } else {
            args->pub_key_file = "keyring.pub";
            args->priv_key_file = "keyring.priv";
        }

        test_file_write(args->pub_key_file);
        test_file_write(args->priv_key_file);
        return;
    }

    // if this is not keygen then -k needs to be set
    if (!strlen(GetStrArg(&argTable, "-k"))) {
        perror("invalid arguments: no key file for encryption/decryption");
        exit(EXIT_FAILURE);
    } else {
        test_file_read(GetStrArg(&argTable, "-k"));
    }

    if (strlen(GetStrArg(&argTable, "-e"))) {
        args->action = ENCRYPT;
        args->pub_key_file = GetStrArg(&argTable, "-k");

        args->source_file = GetStrArg(&argTable, "-e");
        test_file_read(args->source_file);

        if (strlen(GetStrArg(&argTable, "-o"))) {
            args->result_file = GetStrArg(&argTable, "-o");
            test_file_write(args->result_file);
        } else {
            args->result_file = "out.ecco";
        }
        test_file_write(args->result_file);
        return;

    } else if (strlen(GetStrArg(&argTable, "-d"))) {
        args->action = DECRYPT;
        args->priv_key_file = GetStrArg(&argTable, "-k");

        args->source_file = GetStrArg(&argTable, "-d");
        test_file_read(args->source_file);

        if (strlen(GetStrArg(&argTable, "-o"))) {
            args->result_file = GetStrArg(&argTable, "-o");
            test_file_write(args->result_file);
        } else {
            args->result_file = "out.txt";
        }
        test_file_write(args->result_file);
        return;
    } else {
        perror("access error: no valid action selected");
        exit(EXIT_FAILURE);
    }
}

void print_arguments(struct arguments* args) {
    printf("action: %d\nsource_file: %s\nresult_file: %s\ncurve: %s\npub_key_file: %s\npriv_key_file: %s\n",
        args->action, args->source_file, args->result_file, args->curve, args->pub_key_file, args->priv_key_file);
}

int read_keyfile(char* filename, struct keyring* keyring, struct curve* curve) {
    FILE* fp;
    char* curve_name = calloc(32, 1);

    
    if ((fp = fopen(filename, "r")) != NULL) {
        if (keyring->is_private) {
            if (gmp_fscanf(fp, "-----BEGIN-PRIVATE-KEY-----\n%s\n%Zd\n------END-PRIVATE-KEY------", curve_name, keyring->key_priv)) {
                if (curve_populate(curve, curve_name)) {
                    fclose(fp);

                    set_global_modulo(curve->modulus_p);
                    keyring->curve = curve;

                    point_mult(&(keyring->key_pub), &(keyring->curve->base_point_G), 
                        keyring->key_priv, keyring->curve);
                    
                    return 1;
                }
            }
        } else if (!keyring->is_private) {
            if (gmp_fscanf(fp, "-----BEGIN-PUBLIC-KEY-----\n%s\n%Zd\n%Zd\n------END-PUBLIC-KEY------", curve_name, keyring->key_pub.x, keyring->key_pub.y)) {
                if (curve_populate(curve, curve_name)) {
                    fclose(fp);

                    set_global_modulo(curve->modulus_p);
                    keyring->curve = curve;

                    set_global_modulo(keyring->curve->modulus_p);
            
                    return 1;
                }
            }
        }
        fclose(fp);
        return 0;
    } else {
        return 0;
    }
}

int read_encmsg_head(FILE* fp, struct keyring* keyring, struct curve* curve) {
    char* curve_name = calloc(32, 1);

    if (gmp_fscanf(fp, "----BEGIN-EPHEMERAL-KEY----\n%s\n%Zd\n%Zd\n-------BEGIN-MESSAGE-------\n", curve_name, keyring->key_pub_eph.x, keyring->key_pub_eph.y)) {
        if (curve_populate(curve, curve_name)) {
            set_global_modulo(curve->modulus_p);
            keyring->curve = curve;
            return 1;
        }
    }
    return 0;
}

int read_encmsg_body(FILE* fp, unsigned char* buff, size_t buff_len) {
    int matched = fscanf(fp, "%s", buff);  // Remove \n - fscanf skips whitespace automatically
    
    if (matched == 1) {
        return strlen((char*)buff); 
    } else if (matched == EOF) {
        return 0;  // No more data
    } else {
        return -1;  // Error
    }
}

int read_uncrypt_msg(FILE* fp, unsigned char* buff, size_t buff_len) {
    return fread(buff, sizeof(unsigned char), buff_len, fp);
}
