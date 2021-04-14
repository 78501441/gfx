
#include <stdlib.h>
#include <string.h>

#include "ranges.h"

int
sympos(const char *start, size_t len, int c)
{
  const char *symptr = memchr(start, c, len);
  return symptr ? symptr - start : -1;
}

const char *
next_signature(const char *start, size_t len, const char *signature,
               size_t sig_len)
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