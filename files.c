
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/stat.h>

#include "files.h"


ssize_t
read_file(const char *fn, char *buf, size_t buflen) {
  int fd = open(fn, O_RDONLY);
  if (fd == -1) {
    return -1;
  }
  ssize_t r = read(fd, buf, buflen);
  close(fd);
  return r;
}


long
file_size(const char *fpath) {
  struct stat l;
  if (stat(fpath, &l) == -1) {
    return -1;
  }
  if (!S_ISREG(l.st_mode)) {
    return -2;
  }
  return l.st_size;
}


int
check_size_error(long result, const char *fn) {
  if (result < 0) {
    if (result == -2) {
      fprintf(stderr, "Target %s is not a file\n", fn);
      return 2;
    }
    fprintf(stderr, "Unable to stat %s: %s\n", fn, strerror(errno));
    return 1;
  }
  return 0;
}
