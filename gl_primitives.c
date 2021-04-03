
#include "include/GL/glew.h"

#include "gl_primitives.h"


unsigned int
bind_new_buffer(const void *data, int data_size) {
  unsigned int id = 0;
  glGenBuffers(1, &id);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  glBufferData(GL_ARRAY_BUFFER, data_size, data, GL_STATIC_DRAW);
  return id;
}


void
new_vertex_attrib_pointerf(unsigned int attr_index,
                           unsigned int vertex_attrib_count,
                           const void *attrib_offset) {

  glVertexAttribPointer(attr_index,
                        vertex_attrib_count,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(float) * vertex_attrib_count,
                        attrib_offset);

  glEnableVertexAttribArray(attr_index);

}


unsigned int
bind_new_vertex_array() {
  unsigned int id;
  glGenVertexArrays(1, &id);
  glBindVertexArray(id);
  return id;
}