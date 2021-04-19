#include "../include/GL/glew.h"

#include "gl_primitives.h"

unsigned int
bind_new_buffer(const void *data, int data_size, unsigned int type)
{
  unsigned int id = 0;
  glGenBuffers(1, &id);
  glBindBuffer(type, id);
  glBufferData(type, data_size, data, GL_STATIC_DRAW);
  return id;
}

void
new_vertex_attrib_pointerf(unsigned int attr_index,
                           unsigned int vertex_attrib_count,
                           unsigned long attrib_stride,
                           unsigned long attrib_offset)
{

  glEnableVertexAttribArray(attr_index);

  glVertexAttribPointer(attr_index,
                        vertex_attrib_count,
                        GL_FLOAT,
                        GL_FALSE,
                        attrib_stride,
                        (const void *)attrib_offset);
}

unsigned int
bind_new_vertex_array()
{
  unsigned int id;
  glGenVertexArrays(1, &id);
  glBindVertexArray(id);
  return id;
}
