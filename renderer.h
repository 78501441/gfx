#ifndef RENDERER_H_
#define RENDERER_H_


#include <sys/types.h>


typedef enum {
  x_changed,
  y_changed,
  z_changed
} vec_change_type;

struct move_uniform {
  int modified;
  int offset_location;
  float x_offset;
  float y_offset;
  float z_offset;
};

struct default_renderer {
  const void *box_data;
  size_t box_data_size;
  const void *scene_data;
  size_t scene_data_size;

  unsigned int box_vao;
  unsigned int scene_vao;
  unsigned int box_vbo;
  unsigned int scene_vbo;
  unsigned int box_shaders;
  unsigned int scene_shaders;

  struct move_uniform move_vec;
};


void
renderer_init(struct default_renderer *state);


void
renderer_prepare(struct default_renderer *state);


void
renderer_render_scene(struct default_renderer *state);


void
renderer_mark_scene_dirty(struct default_renderer *state,
                          vec_change_type direction,
                          float val);


#endif
