#include <stdio.h>

#include "dynarray.h"

#include "input.h"

void
read_coords(const char *filename, void *out_array)
{
  FILE *f = fopen(filename, "r");
  if (!f)
    return;
  struct vertex_data crd;
  crd.z = 0.0f;
  dyn_init(out_array, sizeof(struct vertex_data));
  while (fscanf(f, "%f %f %f %f %f", &crd.x, &crd.y, &crd.r, &crd.g, &crd.b) !=
         EOF) {
    dyn_append(out_array, &crd);
  }
}