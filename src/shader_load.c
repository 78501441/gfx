
#include <alloca.h>
#include <stdio.h>
#include <string.h>

#include "dynarray.h"
#include "programs_list.h"
#include "shader_load.h"

#include "../include/GL/glew.h"

unsigned int
compile_shaders(const struct shader_source *sources, unsigned int shaders_count)
{

  unsigned int shader_id;
  int is_compiled;
  unsigned int program = glCreateProgram();
  int len;

  for (unsigned int i = 0; i < shaders_count; ++i) {

    shader_id = glCreateShader(
        sources[i].type == st_vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);

    len = sources[i].length;
    glShaderSource(shader_id, 1, &sources[i].start, &len);
    glCompileShader(shader_id);
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &is_compiled);

    if (is_compiled != GL_TRUE) {
      int len;
      char *text              = (char *)alloca(sources[i].length + 1);
      text[sources[i].length] = 0;
      memcpy(text, sources[i].start, sources[i].length);
      glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len);
      char *log = (char *)alloca(len);
      glGetShaderInfoLog(shader_id, len, &len, log);

      fprintf(stderr,
              "%s Shader compilation error:\n%s\nCode:\n%s\n",
              sources[i].type == st_vertex ? "Vertex" : "Fragment",
              log,
              text);

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
