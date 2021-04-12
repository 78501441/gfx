#pragma once

unsigned int
bind_new_buffer(const void* data, int data_size);

unsigned int
bind_new_vertex_array();

void
new_vertex_attrib_pointerf(unsigned int attr_index,
                           unsigned int vertex_attrib_count,
                           const void* attrib_offset);