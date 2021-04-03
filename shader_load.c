
#include <alloca.h>
#include <stdio.h>

#include "shader_load.h"

#include "include/GL/glew.h"


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
#ifndef NO_DEBUG
  glValidateProgram(program);
#endif

  return program;

}
