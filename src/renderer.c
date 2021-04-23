#include <alloca.h>
#include <string.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "dynarray.h"
#include "gfx_math.h"
#include "gl_primitives.h"
#include "programs_list.h"
#include "resource.h"
#include "shader_load.h"
#include "shaders_def.h"

#include "renderer.h"

#ifndef NO_DEBUG

#include <stdio.h>
static void
gl_error_callback(unsigned int source, unsigned int type, unsigned int id,
                  unsigned int severity, int len, const char *message,
                  const void *user_ptr);
#endif

static unsigned int
prepare_shaders();

void
renderer_init(struct gl_renderer *state, const void *scene_data,
              unsigned long scene_data_size)
{
  state->box_data        = box;
  state->box_data_size   = sizeof(box);
  state->scene_data      = tri_vertexes;
  state->scene_data_size = sizeof(tri_vertexes);

  if (scene_data && scene_data_size) {
    state->scene_data      = scene_data;
    state->scene_data_size = scene_data_size;
  }

  gladLoadGL(glfwGetProcAddress);

#ifndef NO_DEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(gl_error_callback, NULL);
#endif
}

void
renderer_prepare(struct gl_renderer *state)
{
  state->box_vao = bind_new_vertex_array();
  state->box_vbo =
      bind_new_buffer(state->box_data, state->box_data_size, GL_ARRAY_BUFFER);
  new_vertex_attrib_pointerf(0, 3, 0, 0);

  state->scene_vao = bind_new_vertex_array();
  state->scene_vbo = bind_new_buffer(state->scene_data,
                                     state->scene_data_size,
                                     GL_ARRAY_BUFFER);
  state->scene_indexes =
      bind_new_buffer(indices, sizeof(indices), GL_ELEMENT_ARRAY_BUFFER);
  new_vertex_attrib_pointerf(0, 3, sizeof(float) * 6, 0);
  new_vertex_attrib_pointerf(1, 3, sizeof(float) * 6, sizeof(float) * 3);

  state->scene_shaders = prepare_shaders();
  glUseProgram(state->scene_shaders);

  memset(&state->tr, 0, sizeof(struct translation));

  state->tr.offset_location =
      glGetUniformLocation(state->scene_shaders, "pos_offset");

  state->tr.transform_location =
      glGetUniformLocation(state->scene_shaders, "transformation_matrix");

  /* Setup initial scene to 0 moved and identity multiplied. */
  state->tr.modified = flmod_move | flmod_rotation;

  struct shader_source default_box_shaders[] = {
      {st_vertex, identity_shader_src_, sizeof(identity_shader_src_) - 1},
      {st_fragment, aqua_shader_src_, sizeof(aqua_shader_src_) - 1}};
  state->box_shaders = compile_shaders(default_box_shaders, 2);
}

void
renderer_render_scene(struct gl_renderer *state)
{

  glClear(GL_COLOR_BUFFER_BIT);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glBindVertexArray(state->scene_vao);
  glUseProgram(state->scene_shaders);

  if (state->tr.modified) {

    if (state->tr.modified & flmod_move) {

      glUniform4f(state->tr.offset_location,
                  state->tr.x_offset,
                  state->tr.y_offset,
                  state->tr.z_offset,
                  0.0f);
    }

    if (state->tr.modified & flmod_rotation) {

      mat4x4 opm;
      get_idenitity_matrix(opm);

      if (state->tr.rotation != 0.0f)
        get_rotation_matrix_z(opm, state->tr.rotation);

      glUniformMatrix4fv(state->tr.transform_location,
                         1,
                         GL_FALSE,
                         (const float *)opm);
    }

    state->tr.modified = 0;
  }

  glDrawElements(GL_TRIANGLE_STRIP,
                 sizeof(indices),
                 GL_UNSIGNED_BYTE,
                 (const void *)0);

  glBindVertexArray(state->box_vao);
  glUseProgram(state->box_shaders);
  glDrawArrays(GL_LINE_LOOP, 0, 4);
}

void
renderer_move(struct gl_renderer *state, vec_change_type direction, float val)
{

  float *ptr = NULL;

  if (direction == x_changed) {
    ptr = &state->tr.x_offset;
  } else if (direction == y_changed) {
    ptr = &state->tr.y_offset;
  } else {
    ptr = &state->tr.z_offset;
  }

  *ptr += val;
  state->tr.modified |= flmod_move;
}

static unsigned int
prepare_shaders()
{

  struct dynarray targets;
  dyn_init(&targets, sizeof(struct shader_source));

  prepare_programs_list(&targets,
                        combined_data,
                        combined_data_end - combined_data);

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
renderer_rotate(struct gl_renderer *state, int side, float angle)
{
  float angle_delta = 0.0f;
  if (side == 1) {
    angle_delta = deg_to_rad(angle);
  } else if (side == -1) {
    angle_delta = deg_to_rad(-angle);
  }
  if (angle_delta != 0.0f) {
    state->tr.rotation += angle_delta;
    state->tr.modified |= flmod_rotation;
  }
}
