#include "../include/ecco/ecco.h"
#include <stdio.h>
#include <string.h>

void test_output_key(char* filename, struct keyring* keyring) {
    FILE* fp = fopen(filename, "w");
    write_key(fp, keyring);
    fclose(fp);
}

void test_output_msg(char* filename, unsigned char* msg) {
    FILE* fp = fopen(filename, "w");
    write_decrypted_msg(fp, msg, strlen((char*)msg));
    fclose(fp);
}

void test_output_encmsg(char* filename, unsigned char* msg, struct point* eph_pub_key) {
    FILE* fp = fopen(filename, "w");
    write_encrypted_msg(fp, msg, 10, "P-384", eph_pub_key);
    fclose(fp);
}

void test_all_output() {
    unsigned char* msg = "this is a test message\n*&@^&@(*)#*@#_)@#@($_@)#";
    struct keyring keyring;
    struct curve curve;
    init_keyring(&keyring);
    curve_populate(&curve, "P-384");
    set_global_modulo(curve.modulus_p);
    set_keyring_private(&keyring);

    generate_keyring(&curve, &keyring);
    set_keyring_public(&keyring);
    generate_keyring(&curve, &keyring);
    print_keyring(&keyring);
    set_keyring_private(&keyring);


    test_output_key("keytest.key", &keyring);
    test_output_msg("msgtest.txt", msg);
    test_output_encmsg("encmsg.txt", msg, &keyring.key_pub_eph);
}

void test_all_input() {
    unsigned char msg[256];
    struct keyring keyring;
    struct curve curve;
    init_keyring(&keyring);
    set_keyring_private(&keyring);

    read_keyfile("keytest.key", &keyring, &curve);
    print_keyring(&keyring);

    FILE* fp = fopen("msgtest.txt", "r");
    read_uncrypt_msg(fp, msg, 256);
    fclose(fp);
    gmp_printf("\n%s\n", msg);

    fp = fopen("encmsg.txt", "r");
    read_encmsg_head(fp, &keyring, &curve);
    read_encmsg_body(fp, msg, 256);
    fclose(fp);
    gmp_printf("\n%s\n%Zd\n", msg, keyring.key_pub_eph.x);

}

int main(int argc,char** argv) {
    struct arguments args;
    get_arguments(&args, argc, argv);
    print_arguments(&args);

    test_all_output();
    test_all_input();
}
