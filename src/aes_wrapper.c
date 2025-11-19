#include "../include/ecco/aes_wrapper.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int from_hex_n(const char *hex, unsigned char *out, size_t len) {
  for (size_t i = 0; i < len; i++) {
    char c1 = hex[2 * i], c2 = hex[2 * i + 1];

    int hi = (c1 >= '0' && c1 <= '9')   ? c1 - '0'
             : (c1 >= 'a' && c1 <= 'f') ? c1 - 'a' + 10
             : (c1 >= 'A' && c1 <= 'F') ? c1 - 'A' + 10
                                        : -1;

    int lo = (c2 >= '0' && c2 <= '9')   ? c2 - '0'
             : (c2 >= 'a' && c2 <= 'f') ? c2 - 'a' + 10
             : (c2 >= 'A' && c2 <= 'F') ? c2 - 'A' + 10
                                        : -1;

    if (hi < 0 || lo < 0)
      return 0;
    out[i] = (unsigned char)((hi << 4) | lo);
  }
  return 1;
}

static char *to_hex_alloc(const unsigned char *in, size_t len) {
  static const char *hex = "0123456789abcdef";
  char *out = malloc(len * 2 + 1);

  if (!out)
    return NULL;

  for (size_t i = 0; i < len; i++) {
    out[2 * i] = hex[(in[i] >> 4) & 0xF];
    out[2 * i + 1] = hex[in[i] & 0xF];
  }
  out[len * 2] = '\0';
  return out;
}

static int parse_key32_from_hex(const char *key_hex, unsigned char key[32]) {
  if (!key_hex || strlen(key_hex) < 64)
    return 0;
  return from_hex_n(key_hex, key, 32);
}

char *aes_encrypt_gcm_hex(const unsigned char *plaintext, size_t pt_len,
                          const char *key_hex) {
  unsigned char key[32];

  if (!parse_key32_from_hex(key_hex, key))
    return NULL;
  unsigned char iv[12];

  if (RAND_bytes(iv, sizeof(iv)) != 1)
    return NULL;

  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

  if (!ctx)
    return NULL;

  unsigned char *cipher = malloc(pt_len + 16);
  unsigned char tag[16];

  int len = 0, total = 0, ok = 1;

  ok &= EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL) == 1;
  ok &= EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, sizeof(iv), NULL) == 1;
  ok &= EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv) == 1;
  ok &= EVP_EncryptUpdate(ctx, cipher, &len, plaintext, (int)pt_len) == 1;
  total = len;
  ok &= EVP_EncryptFinal_ex(ctx, cipher + total, &len) == 1;
  total += len;
  ok &= EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, sizeof(tag), tag) == 1;

  EVP_CIPHER_CTX_free(ctx);

  if (!ok) {
    free(cipher);
    return NULL;
  }

  char *iv_hex = to_hex_alloc(iv, sizeof(iv));
  char *ct_hex = to_hex_alloc(cipher, total);
  char *tag_hex = to_hex_alloc(tag, sizeof(tag));

  free(cipher);

  if (!iv_hex || !ct_hex || !tag_hex) {
    free(iv_hex);
    free(ct_hex);
    free(tag_hex);
    return NULL;
  }

  size_t out_len = strlen(iv_hex) + strlen(ct_hex) + strlen(tag_hex) + 3;
  char *out = malloc(out_len);

  snprintf(out, out_len, "%s:%s:%s", iv_hex, ct_hex, tag_hex);
  free(iv_hex);
  free(ct_hex);
  free(tag_hex);
  return out;
}

unsigned char *aes_decrypt_gcm_hex(const char *pkg, const char *key_hex,
                                   size_t *out_len) {
  unsigned char key[32];

  if (!pkg || !parse_key32_from_hex(key_hex, key))
    return NULL;

  const char *p1 = strchr(pkg, ':');
  const char *p2 = NULL;

  if (p1) {
    p2 = strchr(p1 + 1, ':');
  }

  if (!p1 || !p2)
    return NULL;

  size_t iv_len = p1 - pkg;
  size_t ct_len = p2 - (p1 + 1);
  const char *iv_hex = pkg;
  char *ct_hex = strndup(p1 + 1, ct_len);
  const char *tag_hex = p2 + 1;

  unsigned char iv[12], tag[16];

  if (!from_hex_n(iv_hex, iv, sizeof(iv)) ||
      !from_hex_n(tag_hex, tag, sizeof(tag))) {
    free(ct_hex);
    return NULL;
  }

  size_t ct_bytes = ct_len / 2;
  unsigned char *cipher = malloc(ct_bytes);
  from_hex_n(ct_hex, cipher, ct_bytes);

  free(ct_hex);

  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  unsigned char *plain = malloc(ct_bytes);
  int len = 0, total = 0, ok = 1;

  ok &= EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL) == 1;
  ok &= EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, sizeof(iv), NULL) == 1;
  ok &= EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv) == 1;
  ok &= EVP_DecryptUpdate(ctx, plain, &len, cipher, (int)ct_bytes) == 1;
  total = len;
  ok &= EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, sizeof(tag), tag) == 1;
  ok &= EVP_DecryptFinal_ex(ctx, plain + total, &len) == 1;
  total += len;

  EVP_CIPHER_CTX_free(ctx);

  free(cipher);

  if (!ok) {
    free(plain);
    return NULL;
  }

  *out_len = total;
  return plain;
}