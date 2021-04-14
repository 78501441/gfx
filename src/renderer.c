
#include <alloca.h>
#include <math.h>
#include <string.h>

#include "../include/GL/glew.h"

#include "gl_primitives.h"
#include "programs_list.h"
#include "resource.h"
#include "shader_load.h"
#include "shaders_def.h"

#include "renderer.h"

#ifndef NO_DEBUG
#include "../include/GLFW/glfw3.h"
#include <stdio.h>
static void
gl_error_callback(unsigned int source, unsigned int type, unsigned int id,
                  unsigned int severity, int len, const char *message,
                  const void *user_ptr);
#endif

static unsigned int
prepare_shaders();

static void
get_identity_matrix(float *mptr)
{
  memset(mptr, 0, sizeof(float) * 4 * 4);
  int j = 0;
  for (int i = 0; i < 4; ++i) {
    (&mptr[i * 4])[j] = 1.2f;
    ++j;
  }
}

static void
rotate_matrix_z(float *mptr, float angle)
{
  mptr[0] = cosf(angle);
  mptr[1] = -sinf(angle);
  mptr[4] = sinf(angle);
  mptr[5] = cosf(angle);
}

void
renderer_init(struct default_renderer *state)
{
  state->box_data        = box;
  state->box_data_size   = sizeof(box);
  state->scene_data      = positions;
  state->scene_data_size = sizeof(positions);

  glewInit();

#ifndef NO_DEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(gl_error_callback, NULL);
#endif
}

void
renderer_prepare(struct default_renderer *state)
{
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

  glUniform4f(state->move_vec.offset_location, 0.0f, 0.0f, 0.0f, 0.0f);

  struct shader_source default_box_shaders[] = {
      {st_vertex, identity_shader_src_, sizeof(identity_shader_src_) - 1},
      {st_fragment, aqua_shader_src_, sizeof(aqua_shader_src_) - 1}};
  state->box_shaders = compile_shaders(default_box_shaders, 2);
}

void
renderer_render_scene(struct default_renderer *state)
{

  glClear(GL_COLOR_BUFFER_BIT);

  glBindVertexArray(state->scene_vao);
  glUseProgram(state->scene_shaders);

  if (state->move_vec.modified) {

    glUniform4f(state->move_vec.offset_location,
                state->move_vec.x_offset,
                state->move_vec.y_offset,
                state->move_vec.z_offset,
                0.0f);

    float idm[16];
    get_identity_matrix(idm);

    if (state->move_vec.rotation != 0.0f)
      rotate_matrix_z(idm, state->move_vec.rotation);

    int loc =
        glGetUniformLocation(state->scene_shaders, "transformation_matrix");
    glUniformMatrix4fv(loc, 1, GL_FALSE, idm);

    state->move_vec.modified = 0;
  }
  glDrawArrays(GL_TRIANGLES, 0, 3);

  glBindVertexArray(state->box_vao);
  glUseProgram(state->box_shaders);
  glDrawArrays(GL_LINE_LOOP, 0, 4);
}

void
renderer_move(struct default_renderer *state, vec_change_type direction,
              float val)
{

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
prepare_shaders()
{

  struct dynarray targets;
  dyn_init(&targets, sizeof(struct shader_source));

  prepare_programs_list(
      &targets, combined_data, combined_data_end - combined_data);

  unsigned int program_id =
      compile_shaders((struct shader_source *)targets.data, targets.count);

  dyn_release(&targets);

  return program_id;
}

#ifndef NO_DEBUG
static void
gl_error_callback(unsigned int source, unsigned int type, unsigned int id,
                  unsigned int severity, int len, const char *message,
                  const void *user_ptr)
{

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

void
renderer_rotate(struct default_renderer *state, int side)
{
  float angle_delta = 0.0f;
  if (side == 1) {
    angle_delta = M_PI / 36.0f;
  } else if (side == -1) {
    angle_delta = -M_PI / 36.0f;
  }
  if (angle_delta != 0.0f) {
    state->move_vec.rotation += angle_delta;
    state->move_vec.modified = 1;
  }
}