#ifndef OUTPUT_SERVICE_H
#define OUTPUT_SERVICE_H

#include <stddef.h>

/*
    Write null-terminated string to file.
    append != 0 -> append mode, else overwrite.
    0 on success, negative on error.
*/
int write_string_to_file(const char *path, const char *s, int append);

/*
    Write raw buffer to file.
    append != 0 -> append mode, else overwrite.
    0 on success, negative on error.
*/
int write_buffer_to_file(const char *path, const void *buf, size_t len, int append);

#endif