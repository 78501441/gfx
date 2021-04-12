
#pragma once

#include <sys/types.h>

#include "dynarray.h"

typedef enum { st_unknown, st_vertex, st_fragment } shader_type;

struct shader_source {
  shader_type type;
  const char* start;
  size_t length;
};

size_t
prepare_programs_list(dyn_p storage, const char* blob_start, size_t blob_size);
