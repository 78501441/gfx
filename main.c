
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>

#include <sys/stat.h>


#include "include/GL/glew.h"
#include "include/GLFW/glfw3.h"


#define E_NOT_FILE 2
#define UNUSED(t) (void) t;


struct shader_desc {
  unsigned int type;
  const char *source;
  const char *name;
};


static const char *vert_shader_src_fp_ = "shaders/v-shader.vert";
static const char *frag_shader_src_fp_ = "shaders/f-shader.frag";
static struct {
  int offset_location;
  float x_offset;
  float y_offset;
} current_offsets;


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


void
key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);


unsigned int
compile_shaders(const struct shader_desc *shaders,
                unsigned int shaders_count);


int
main(void) {

  long vert_src_size = file_size(vert_shader_src_fp_);
  long frag_src_size = file_size(frag_shader_src_fp_);

  if (check_size_error(vert_src_size, vert_shader_src_fp_) != 0 ||
      check_size_error(frag_src_size, frag_shader_src_fp_) != 0) {

    exit(EXIT_FAILURE);

  }

  char *v_src = (char *)alloca(vert_src_size + frag_src_size + 2);
  char *f_src = v_src + vert_src_size + 1;

  v_src[vert_src_size] = '\0';
  f_src[frag_src_size] = '\0';

  read_file(vert_shader_src_fp_, v_src, vert_src_size);
  read_file(frag_shader_src_fp_, f_src, frag_src_size);

  if (glfwInit() != GLFW_TRUE) {
    fprintf(stderr, "GLFW initialization error\n");
    exit(EXIT_FAILURE);
  }

  GLFWwindow *main_window = glfwCreateWindow(400, 400, "GL", NULL, NULL);

  if (main_window == NULL) {
    const char *err;
    glfwGetError(&err);
    fprintf(stderr, "Unable to create main window\n%s\n", err);
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(main_window);
  glfwSetKeyCallback(main_window, key_callback);

  printf("OpenGL version string: %s\n", glGetString(GL_VERSION));

  float positions[] = {
    0.0f, 0.5f,
    0.5f, -0.5f,
    -0.5f, -0.5f
  };

  glewInit();

  unsigned int buffer_id;
  glGenBuffers(1, &buffer_id);
  glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
  glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

  glVertexAttribPointer(0,                 /* Attribute index.                */
                        2,                 /* Number of components per vertex */
                        GL_FLOAT,          /* Component types .               */
                        GL_FALSE,          /* Should vals must be normalized. */
                        sizeof(float) * 2, /* Stride delta between 2 elems.   */
                        (const void *)0);  /* Index of attribute inside vert. */

  struct shader_desc targets[] = {
    { GL_VERTEX_SHADER, v_src, "Vertex" },
    { GL_FRAGMENT_SHADER, f_src, "Fragment" }
  };

  unsigned int program = compile_shaders(targets, 2);
  glUseProgram(program);
  current_offsets.x_offset = 0.0f;
  current_offsets.y_offset = 0.0f;
  int loc = glGetUniformLocation(program, "pos_offset");
  glUniform4f(loc, 0.0f, 0.0f, 0.0f, 0.0f);
  current_offsets.offset_location = loc;

  while (!glfwWindowShouldClose(main_window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    /* Rendering code goes here. */
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);

    glfwSwapBuffers(main_window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}


void
key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {
    float *moveptr = NULL;
    float value = 0.0f;

    if (key == GLFW_KEY_ESCAPE) {
      printf("ESC pressed; exiting\n");
      glfwSetWindowShouldClose(window, GLFW_TRUE);

    } else if (key == GLFW_KEY_LEFT) {
      moveptr = &current_offsets.x_offset;
      value = -0.1f;
    } else if (key == GLFW_KEY_RIGHT) {
      moveptr = &current_offsets.x_offset;
      value = 0.1f;
    } else if (key == GLFW_KEY_DOWN) {
      moveptr = &current_offsets.y_offset;
      value = 0.1f;
    } else if (key == GLFW_KEY_UP) {
      moveptr = &current_offsets.y_offset;
      value = -0.1f;
    }

    if (moveptr) {

      *moveptr += value;
      glUniform4f(current_offsets.offset_location,
                  current_offsets.x_offset,
                  current_offsets.y_offset,
                  0.0f,
                  0.0f);

    }
  }
  UNUSED(scancode)
  UNUSED(mods)
}


unsigned int
compile_shaders(const struct shader_desc *shaders,
                unsigned int shaders_count) {

  unsigned int shader_id;
  int is_compiled;
  unsigned int program = glCreateProgram();

  int offset_loc = glGetUniformLocation(program, "pos_offset");
  glUniform4f(offset_loc, 0.0f, 0.0f, 0.0f, 0.0f);

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
