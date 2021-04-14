
#include <stdio.h>
#include <stdlib.h>

#include "../include/GLFW/glfw3.h"

#include "renderer.h"

void
report_glfw_error(const char *err_template);

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
  struct default_renderer r;
  renderer_init(&r);
  renderer_prepare(&r);
  glfwSetWindowUserPointer(main_window, &r);
  glfwSetKeyCallback(main_window, key_callback);

  while (!glfwWindowShouldClose(main_window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    renderer_render_scene(&r);
    glfwSwapBuffers(main_window);
    glfwPollEvents();
  }

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

    struct default_renderer *r =
        (struct default_renderer *)glfwGetWindowUserPointer(window);
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
      renderer_rotate(r, -1);
    } else if (key == GLFW_KEY_E) {
      renderer_rotate(r, 1);
    }
  }
}