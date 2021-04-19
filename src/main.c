#include <stdio.h>
#include <stdlib.h>

#include "../include/GLFW/glfw3.h"

#include "dynarray.h"
#include "input.h"
#include "renderer.h"

#define CAST_VAR(varname, type, voidptr) type varname = (type)voidptr;

void
report_glfw_error(const char *err_template);

static int is_animate;

void
key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

int
main(int argc, char **argv)
{
  if (glfwInit() != GLFW_TRUE) {
    report_glfw_error("GLFW init failed");
    exit(EXIT_FAILURE);
  }

  GLFWwindow *main_window = glfwCreateWindow(400, 400, "GL", NULL, NULL);

  if (!main_window) {
    report_glfw_error("Failed to create main window");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(main_window);
  struct gl_renderer r;
  struct dynarray coords;
  is_animate = 0;
  read_coords("shape.txt", &coords);
  renderer_init(&r,
                coords.count ? coords.data : NULL,
                coords.count * sizeof(struct vertex_data));
  renderer_prepare(&r);
  glfwSetWindowUserPointer(main_window, &r);
  glfwSetKeyCallback(main_window, key_callback);

  float last_time = glfwGetTime();
  while (!glfwWindowShouldClose(main_window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    if (is_animate && glfwGetTime() - last_time >= 0.2) {
      renderer_rotate(&r, 1, 5.0f);
      last_time = glfwGetTime();
    }
    renderer_render_scene(&r);
    glfwSwapBuffers(main_window);
    glfwPollEvents();
  }

  dyn_release(&coords);

  glfwTerminate();
}

void
report_glfw_error(const char *err_template)
{
  const char *err_msg;
  glfwGetError(&err_msg);
  fprintf(stderr, "%s: %s\n", err_template, err_msg);
}

void
key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  if (action == GLFW_PRESS) {

    CAST_VAR(r, struct gl_renderer *, glfwGetWindowUserPointer(window))

    if (!r)
      return;
    if (key == GLFW_KEY_ESCAPE) {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    } else if (key == GLFW_KEY_LEFT) {
      renderer_move(r, x_changed, -0.1);
    } else if (key == GLFW_KEY_RIGHT) {
      renderer_move(r, x_changed, 0.1);
    } else if (key == GLFW_KEY_UP) {
      renderer_move(r, y_changed, -0.1);
    } else if (key == GLFW_KEY_DOWN) {
      renderer_move(r, y_changed, 0.1);
    } else if (key == GLFW_KEY_Q) {
      renderer_rotate(r, -1, 5.0f);
    } else if (key == GLFW_KEY_E) {
      renderer_rotate(r, 1, 5.0f);
    } else if (key == GLFW_KEY_A) {
      is_animate = 1 - is_animate;
    }
  }
}