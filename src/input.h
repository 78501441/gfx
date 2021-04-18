#pragma once

struct vertex_data {
  float x;
  float y;
  float z;
  float r;
  float g;
  float b;
};

void
read_coords(const char *filename, void *out_array);
