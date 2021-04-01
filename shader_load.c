
#include <alloca.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>

#include "shader_load.h"

#include "include/GL/glew.h"


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
    return -E_NOT_FILE;
  }
  return l.st_size;
}


int
check_size_error(long result, const char *fn) {
  if (result < 0) {
    if (result == -E_NOT_FILE) {
      fprintf(stderr, "Target %s is not a file\n", fn);
      return 2;
    }
    fprintf(stderr, "Unable to stat %s: %s\n", fn, strerror(errno));
    return 1;
  }
  return 0;
}


unsigned int
compile_shaders(const struct shader_desc *shaders,
                unsigned int shaders_count) {

  unsigned int shader_id;
  int is_compiled;
  unsigned int program = glCreateProgram();

  for (unsigned int i = 0; i < shaders_count; ++ i) {

    shader_id = glCreateShader(shaders[i].type);
    glShaderSource(shader_id, 1, &shaders[i].source, NULL);
    glCompileShader(shader_id);
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &is_compiled);

    if (is_compiled != GL_TRUE) {
      int len;
      glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len);
      char *log = (char *)alloca(len);
      glGetShaderInfoLog(shader_id, len, &len, log);

      fprintf(stderr,
              "%s Shader compilation error:\n%s\nCode:\n%s\n",
              shaders[i].name,
              log,
              shaders[i].source);

    } else {

      glAttachShader(program, shader_id);
      glDeleteShader(shader_id);

    }

  }

  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &is_compiled);
  if (is_compiled != GL_TRUE) {
    int len;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
      char *log = (char *)alloca(len);
      glGetProgramInfoLog(program, len, &len, log);
      fprintf(stderr, "Program linking error:\n%s\n", log);
    }
  }
  glValidateProgram(program);

  return program;

}
