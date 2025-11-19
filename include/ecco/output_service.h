#ifndef OUTPUT_SERVICE_H
#define OUTPUT_SERVICE_H

#include <stddef.h>

int write_string_to_file(const char *path, const char *s, int append);

int write_buffer_to_file(const char *path, const void *buf, size_t len,
                         int append);

#endif