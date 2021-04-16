
#pragma once

int
bytes_equal(const void *bytes1, const void *bytes2, unsigned long max_count);

int
sympos(const char *start, unsigned long len, int c);

const char *
next_signature(const char *start, unsigned long len, const char *signature,
               unsigned long sig_len);