
#pragma once

/* Forward declare. */
struct dyn_p;

struct coords {
  float x;
  float y;
  float z;
};

void
read_coords(const char *filename, dyn_p out_array);
