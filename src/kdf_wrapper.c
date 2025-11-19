#include "../include/ecco/kdf_wrapper.h"
#include <openssl/evp.h>
#include <openssl/kdf.h>
#include <stdlib.h>
#include <string.h>

static void to_hex(const unsigned char *in, size_t len, char *out) {
  static const char *hex = "0123456789abcdef";
  for (size_t i = 0; i < len; i++) {
    out[2 * i] = hex[(in[i] >> 4) & 0xF];
    out[2 * i + 1] = hex[in[i] & 0xF];
  }
}

char *kdf_hex(const void *data, size_t data_len, const unsigned char *salt,
              size_t salt_len, size_t out_hex_chars) {
  if (out_hex_chars == 0 || (out_hex_chars % 2) != 0)
    return NULL;

  size_t out_bytes = out_hex_chars / 2;
  unsigned char *okm = malloc(out_bytes);
  char *hex = malloc(out_hex_chars + 1);

  if (!okm || !hex) {
    free(okm);
    free(hex);
    return NULL;
  }

  EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_HKDF, NULL);
  if (!ctx) {
    free(okm);
    free(hex);
    return NULL;
  }

  int ok = 1;

  ok &= EVP_PKEY_derive_init(ctx) > 0;
  ok &= EVP_PKEY_CTX_set_hkdf_md(ctx, EVP_sha256()) > 0;
  ok &= EVP_PKEY_CTX_set1_hkdf_salt(ctx, salt, (int)salt_len) > 0;
  ok &= EVP_PKEY_CTX_set1_hkdf_key(ctx, data, (int)data_len) > 0;
  ok &= EVP_PKEY_CTX_add1_hkdf_info(ctx, (const unsigned char *)"ECCO-KDF", 8) >
        0;
  ok &= EVP_PKEY_derive(ctx, okm, &out_bytes) > 0;
  EVP_PKEY_CTX_free(ctx);

  if (!ok) {
    free(okm);
    free(hex);
    return NULL;
  }

  to_hex(okm, out_bytes, hex);
  hex[out_hex_chars] = '\0';
  free(okm);
  return hex;
}

char *kdf_hex_from_mpz(const mpz_t val, const unsigned char *salt,
                       size_t salt_len, size_t out_hex_chars) {

  char *buf = mpz_get_str(NULL, 16, val);

  if (!buf) {
    fprintf(stderr, "mpz_get_str failed\n");
    return NULL;
  }
  size_t data_len = strlen(buf);

  char *hex = kdf_hex(buf, data_len, salt, salt_len, out_hex_chars);

  free(buf);
  return hex;
}