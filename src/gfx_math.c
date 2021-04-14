
#include <math.h>
#include <string.h>

#include "gfx_math.h"

void
get_idenitity_matrix(mat4x4 m)
{
  memset(m, 0, sizeof(float) * 16);
  for (int ind = 0; ind < 4; ++ind) {
    m[ind][ind] = 1;
  }
}

void
get_rotation_matrix_z(mat4x4 m, float angle)
{
  m[0][0] = cosf(angle);
  m[0][1] = -sinf(angle);
  m[1][0] = sinf(angle);
  m[1][1] = cosf(angle);
}

float
deg_to_rad(float deg_value)
{
  return M_PI / (360.0f / deg_value);
}
