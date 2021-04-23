#pragma once

typedef enum { st_unknown, st_vertex, st_fragment } shader_type;

struct shader_source {
  shader_type type;
  const char *start;
  unsigned long length;
};

unsigned long
prepare_programs_list(void *storage, const char *blob_start,
                      unsigned long blob_size);
