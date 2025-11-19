#include "../include/ecco/output_service.h"
#include <stdio.h>
#include <string.h>

static int write_core(const char *path, const void *buf, size_t len,
                      int append) {
  if (!path || !buf)
    return -1;
  FILE *f = fopen(path, append ? "ab" : "wb");
  if (!f)
    return -2;
  size_t w = fwrite(buf, 1, len, f);
  fclose(f);
  return (w == len) ? 0 : -3;
}

int write_string_to_file(const char *path, const char *s, int append) {
  if (!s)
    return -1;
  return write_core(path, s, strlen(s), append);
}

int write_buffer_to_file(const char *path, const void *buf, size_t len,
                         int append) {
  return write_core(path, buf, len, append);
}
