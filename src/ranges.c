
#include <stdlib.h>
#include <string.h>

#include "ranges.h"

int
bytes_equal(const void *bytes1, const void *bytes2, unsigned long max_count)
{
  return memcmp(bytes1, bytes2, max_count) == 0;
}

int
sympos(const char *start, unsigned long len, int c)
{
  const char *symptr = memchr(start, c, len);
  return symptr ? symptr - start : -1;
}

const char *
next_signature(const char *start, unsigned long len, const char *signature,
               unsigned long sig_len)
{
  const char *line = start;
  int pos;
  while (1) {
    pos = sympos(line, len, signature[0]);
    if (pos == -1)
      return NULL;
    line += pos;
    if (!bytes_equal(line, signature, sig_len)) {
      line++;
    } else {
      break;
    }
  }
  return line;
}