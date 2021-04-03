
#include <stdio.h>
#include <stdlib.h>


#include "include/GLFW/glfw3.h"

#include "renderer.h"


void
key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);


GLFWwindow *
main_prepare_window(const char *title, int w, int h);


int
main(void) {

  if (glfwInit() != GLFW_TRUE) {
    fprintf(stderr, "GLFW initialization error\n");
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *main_window = main_prepare_window("GL", 400, 400);
  if (main_window == NULL) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  printf("OpenGL version string: %s\n", glGetString(GL_VERSION));

  struct default_renderer rend;
  glfwSetWindowUserPointer(main_window, &rend);

  renderer_init(&rend);
  renderer_prepare(&rend);

  glfwSetKeyCallback(main_window, key_callback);

  while (!glfwWindowShouldClose(main_window)) {

    renderer_render_scene(&rend);

    glfwSwapBuffers(main_window);
    glfwPollEvents();

  }

  glfwTerminate();

  return 0;
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

    struct default_renderer *r =
        (struct default_renderer *)glfwGetWindowUserPointer(window);

    if (r == NULL) {
      return;
    }

    if (key == GLFW_KEY_ESCAPE) {
      printf("ESC pressed; exiting\n");
      glfwSetWindowShouldClose(window, GLFW_TRUE);

    } else if (key == GLFW_KEY_LEFT) {
      renderer_mark_scene_dirty(r, x_changed, -0.1f);
    } else if (key == GLFW_KEY_RIGHT) {
      renderer_mark_scene_dirty(r, x_changed, 0.1f);
    } else if (key == GLFW_KEY_DOWN) {
      renderer_mark_scene_dirty(r, y_changed, 0.1f);
    } else if (key == GLFW_KEY_UP) {
      renderer_mark_scene_dirty(r, y_changed, -0.1f);
    } else if (key == GLFW_KEY_Z) {
      renderer_mark_scene_dirty(r, z_changed, 0.1f);
    } else if (key == GLFW_KEY_X) {
      renderer_mark_scene_dirty(r, z_changed, -0.1f);
    }

  }
}
