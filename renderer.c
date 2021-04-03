
#include <alloca.h>
#include <string.h>

#include "include/GL/glew.h"

#include "gl_primitives.h"
#include "files.h"
#include "shaders_def.h"
#include "shader_load.h"

#include "renderer.h"

#ifndef NO_DEBUG
#include <stdio.h>
#include "include/GLFW/glfw3.h"
static void
gl_error_callback(unsigned int source,
                  unsigned int type,
                  unsigned int id,
                  unsigned int severity,
                  int len,
                  const char *message,
                  const void *user_ptr);
#endif


static unsigned int
prepare_shaders();


void
renderer_init(struct default_renderer *state) {
  state->box_data = box;
  state->box_data_size = sizeof(box);
  state->scene_data = positions;
  state->scene_data_size = sizeof(positions);

  glewInit();

#ifndef NO_DEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(gl_error_callback, NULL);
#endif

}


void
renderer_prepare(struct default_renderer *state) {
  state->box_vao = bind_new_vertex_array();
  state->box_vbo = bind_new_buffer(state->box_data, state->box_data_size);
  new_vertex_attrib_pointerf(0, 3, 0);

  state->scene_vao = bind_new_vertex_array();
  state->scene_vbo = bind_new_buffer(state->scene_data, state->scene_data_size);
  new_vertex_attrib_pointerf(0, 3, 0);

  state->scene_shaders = prepare_shaders();
  glUseProgram(state->scene_shaders);

  memset(&state->move_vec, 0, sizeof(struct move_uniform));

  state->move_vec.offset_location =
      glGetUniformLocation(state->scene_shaders, "pos_offset");

  glUniform4f(state->move_vec.offset_location,
              0.0f,
              0.0f,
              0.0f,
              0.0f);

  struct shader_desc default_box_shaders[] = {
    { GL_VERTEX_SHADER, identity_shader_src_, "Box Identity" },
    { GL_FRAGMENT_SHADER, aqua_shader_src_, "Box Aqua Color" }
  };
  state->box_shaders = compile_shaders(default_box_shaders, 2);
}


void
renderer_render_scene(struct default_renderer *state) {

  glClear(GL_COLOR_BUFFER_BIT);

  glBindVertexArray(state->scene_vao);
  glUseProgram(state->scene_shaders);

  if (state->move_vec.modified) {

    glUniform4f(state->move_vec.offset_location,
                state->move_vec.x_offset,
                state->move_vec.y_offset,
                state->move_vec.z_offset,
                0.0f);

    state->move_vec.modified = 0;

  }
  glDrawArrays(GL_TRIANGLES, 0, 3);

  glBindVertexArray(state->box_vao);
  glUseProgram(state->box_shaders);
  glDrawArrays(GL_LINE_LOOP, 0, 4);

}


void
renderer_mark_scene_dirty(struct default_renderer *state,
                          vec_change_type direction,
                          float val) {

  float *ptr = NULL;

  if (direction == x_changed) {
    ptr = &state->move_vec.x_offset;
  } else if (direction == y_changed) {
    ptr = &state->move_vec.y_offset;
  } else {
    ptr = &state->move_vec.z_offset;
  }

  *ptr += val;

  state->move_vec.modified = 1;

}


static unsigned int
prepare_shaders() {

  long vert_src_size = file_size(vert_shader_src_fp_);
  long frag_src_size = file_size(frag_shader_src_fp_);

  if (check_size_error(vert_src_size, vert_shader_src_fp_) != 0 ||
      check_size_error(frag_src_size, frag_shader_src_fp_) != 0) {

    return 0;

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

  return compile_shaders(targets, 2);

}


#ifndef NO_DEBUG
static void
gl_error_callback(unsigned int source,
                  unsigned int type,
                  unsigned int id,
                  unsigned int severity,
                  int len,
                  const char *message,
                  const void *user_ptr) {

  fprintf(stderr,
          "%.4f(s): %s (0x%x) Severity 0x%x: \"%s\"\n",
          glfwGetTime(),
          (type == GL_DEBUG_TYPE_ERROR ? "[Error]" : "[Debug]"),
          type,
          severity,
          message);

  UNUSED(source)
  UNUSED(id)
  UNUSED(len)
  UNUSED(user_ptr)

}
#endif
