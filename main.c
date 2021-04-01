
#include <stdio.h>
#include <stdlib.h>

#include "include/GL/glew.h"
#include "include/GLFW/glfw3.h"

#include "shader_load.h"


#define UNUSED(t) (void) t;


static const char *vert_shader_src_fp_ = "shaders/v-shader.vert";
static const char *frag_shader_src_fp_ = "shaders/f-shader.frag";

struct move_uniform {
  int offset_location;
  float x_offset;
  float y_offset;
  float z_offset;
};


unsigned int
bind_new_buffer(const void *data, int data_size);


void
key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);


unsigned int
main_prepare_shaders();


GLFWwindow *
main_prepare_window(const char *title, int w, int h);


int
main(void) {

  if (glfwInit() != GLFW_TRUE) {
    fprintf(stderr, "GLFW initialization error\n");
    exit(EXIT_FAILURE);
  }

  GLFWwindow *main_window = main_prepare_window("GL", 400, 400);
  if (main_window == NULL) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  static struct move_uniform current_offsets;

  glfwSetWindowUserPointer(main_window, &current_offsets);

  glfwSetKeyCallback(main_window, key_callback);

  printf("OpenGL version string: %s\n", glGetString(GL_VERSION));

  const float positions[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    /* Surrounding box. */
    -0.9f, 0.9f, 0.0f,
    0.9f, 0.9f, 0.0f,
    0.9f, -0.9f, 0.0f,
    -0.9f, -0.9f, 0.0f
  };

  glewInit();

  bind_new_buffer(positions, sizeof(positions));

  glVertexAttribPointer(0,                 /* Attribute index.                */
                        3,                 /* Number of components per vertex */
                        GL_FLOAT,          /* Component types .               */
                        GL_FALSE,          /* Should vals must be normalized. */
                        sizeof(float) * 3, /* Stride delta between 2 elems.   */
                        (const void *)0);  /* Index of attribute inside vert. */

  current_offsets.x_offset = 0.0f;
  current_offsets.y_offset = 0.0f;
  current_offsets.z_offset = 0.0f;

  current_offsets.offset_location =
      glGetUniformLocation(main_prepare_shaders(), "pos_offset");

  while (!glfwWindowShouldClose(main_window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    /* Rendering code goes here. */
    glEnableVertexAttribArray(0);

    glUniform4f(current_offsets.offset_location,
                current_offsets.x_offset,
                current_offsets.y_offset,
                current_offsets.z_offset,
                0.0f);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUniform4f(current_offsets.offset_location, 0.0f, 0.0f, 0.0f, 0.0f);
    glDrawArrays(GL_LINE_LOOP, 3, 4);

    glDisableVertexAttribArray(0);

    glfwSwapBuffers(main_window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}


unsigned int
bind_new_buffer(const void *data, int data_size) {
  unsigned int id = 0;
  glGenBuffers(1, &id);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  glBufferData(GL_ARRAY_BUFFER, data_size, data, GL_STATIC_DRAW);
  return id;
}


GLFWwindow *
main_prepare_window(const char *title, int w, int h) {
  GLFWwindow *window = glfwCreateWindow(w, h, title, NULL, NULL);

  if (window == NULL) {
    const char *err;
    glfwGetError(&err);
    fprintf(stderr, "Unable to create main window\n%s\n", err);
    return NULL;
  }

  glfwMakeContextCurrent(window);
  return window;
}


void
key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {

    struct move_uniform *mu =
        (struct move_uniform *)glfwGetWindowUserPointer(window);

    if (mu == NULL) {
      return;
    }

    float *moveptr = NULL;
    float value = 0.0f;

    if (key == GLFW_KEY_ESCAPE) {
      printf("ESC pressed; exiting\n");
      glfwSetWindowShouldClose(window, GLFW_TRUE);

    } else if (key == GLFW_KEY_LEFT) {
      moveptr = &mu->x_offset;
      value = -0.1f;
    } else if (key == GLFW_KEY_RIGHT) {
      moveptr = &mu->x_offset;
      value = 0.1f;
    } else if (key == GLFW_KEY_DOWN) {
      moveptr = &mu->y_offset;
      value = 0.1f;
    } else if (key == GLFW_KEY_UP) {
      moveptr = &mu->y_offset;
      value = -0.1f;
    } else if (key == GLFW_KEY_Z) {
      moveptr = &mu->z_offset;
      value = 0.1f;
    } else if (key == GLFW_KEY_X) {
      moveptr = &mu->z_offset;
      value = -0.1f;
    }

    if (moveptr) *moveptr += value;

  }
  UNUSED(scancode)
  UNUSED(mods)
}


unsigned int
main_prepare_shaders() {

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

  struct shader_desc targets[] = {
    { GL_VERTEX_SHADER, v_src, "Vertex" },
    { GL_FRAGMENT_SHADER, f_src, "Fragment" }
  };

  unsigned int program = compile_shaders(targets, 2);
  glUseProgram(program);

  return program;
}
