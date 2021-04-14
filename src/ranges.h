
#pragma once

#include <string.h>
#include <sys/types.h> /* For size_t. */

inline int
bytes_equal(const void *bytes1, const void *bytes2, size_t max_count)
{
  return memcmp(bytes1, bytes2, max_count) == 0;
}

int
sympos(const char *start, size_t len, int c);

const char *
next_signature(const char *start, size_t len, const char *signature,
               size_t sig_len);