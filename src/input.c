
#include <stdio.h>

#include "dynarray.h"

#include "input.h"

void
read_coords(const char *filename, dyn_p out_array)
{
  FILE *f = fopen(filename, "r");
  if (!f)
    return;
  struct coords crd;
  crd.z = 0.0f;
  dyn_init(out_array, sizeof(struct coords));
  while (fscanf(f, "%f %f", &crd.x, &crd.y) != EOF) {
    dyn_append(out_array, &crd);
  }
}