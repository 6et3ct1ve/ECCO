#include <stdio.h>
#include <string.h>

#include <gmp.h>
#include "output_service.h"

int write_decrypted_msg(FILE* fp, unsigned char* msg, size_t msg_len){
  if (fwrite(msg, sizeof(char), msg_len, fp) > 0) {
    return 1;
  }
  return 0;
}

int write_encrypted_msg(FILE* fp, unsigned char* msg, size_t msg_len, char* curve_name, struct point* eph_pub_key) {
  char* eph_x = mpz_get_str(NULL, 10, eph_pub_key->x);
  char* eph_y = mpz_get_str(NULL, 10, eph_pub_key->y);
  if (ftell(fp) == 0) {
    if (gmp_fprintf(fp, "----BEGIN-EPHEMERAL-KEY----\n%s\n%Zd\n%Zd\n-------BEGIN-MESSAGE-------\n%s\n", curve_name, eph_pub_key->x, eph_pub_key->y, msg)) {
      return 1;
    }
    return 0;
  }
  else {
    if (gmp_fprintf(fp, "%s\n", msg)) {
      return 1;
    }
    return 0;
  }
}

int write_key(FILE* fp, struct keyring* keyring) {
  if (keyring->is_private) {
    if (gmp_fprintf(fp, "-----BEGIN-PRIVATE-KEY-----\n%s\n%Zd\n------END-PRIVATE-KEY------", keyring->curve->name, keyring->key_priv)) {
      return 1;
    }
    return 0;
  } else {
    if (gmp_fprintf(fp, "-----BEGIN-PUBLIC-KEY-----\n%s\n%Zd\n%Zd\n------END-PUBLIC-KEY------", keyring->curve->name, keyring->key_pub.x, keyring->key_pub.y)) {
      return 1;
    }
    return 0;
  }
}
