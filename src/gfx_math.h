#pragma once

typedef float mat4x4[4][4];

void
get_idenitity_matrix(mat4x4 m);

const mat4x4 *
id_matrix_pointer();

void
get_rotation_matrix_z(mat4x4 m, float angle);

float
deg_to_rad(float deg_value);
