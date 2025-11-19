#include "../include/ecco/aes_wrapper.h"
#include "../include/ecco/kdf_wrapper.h"
#include "../include/ecco/output_service.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  printf("[TEST] KDF + AES wrappers\n");

  const char *shared_secret = "fake_shared_secret_for_testing";
  const unsigned char *salt = (const unsigned char *)"ecco_salt";

  char *key_hex = kdf_hex(shared_secret, strlen(shared_secret), salt,
                          strlen((const char *)salt), 64);
  if (!key_hex) {
    printf("[FAILED] KDF returned NULL\n");
    return 1;
  }
  printf("[OK] derived key: %s\n", key_hex);

  const char *msg = "Hello from ECCO!";
  char *enc =
      aes_encrypt_gcm_hex((const unsigned char *)msg, strlen(msg), key_hex);
  if (!enc) {
    printf("[FAILED] AES encryption failed\n");

    free(key_hex);
    return 1;
  }
  printf("[OK] encrypted: %.32s...\n", enc);

  size_t out_len = 0;
  unsigned char *dec = aes_decrypt_gcm_hex(enc, key_hex, &out_len);
  if (!dec) {
    printf("[FAILED] AES decrypt failed\n");

    free(enc);
    free(key_hex);
    return 1;
  }
  printf("[OK] decrypted: %.*s\n", (int)out_len, dec);

  int w = write_string_to_file("bin/test_output.txt", (char *)dec, 0);
  if (w == 0)
    printf("[OK] file write: success\n");
  else
    printf("[OK] file write: fail\n");

  free(key_hex);
  free(enc);
  free(dec);

  return 0;
}
