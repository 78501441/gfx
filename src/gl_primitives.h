#pragma once

unsigned int
bind_new_buffer(const void *data, int data_size, unsigned int type);

unsigned int
bind_new_vertex_array();

void
new_vertex_attrib_pointerf(unsigned int attr_index,
                           unsigned int vertex_attrib_count,
                           unsigned long attrib_stride,
                           unsigned long attrib_offset);
