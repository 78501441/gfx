
#pragma once

#include <sys/types.h>

struct dynarray {
  void *data;
  size_t capacity;
  size_t count;
  size_t element_size;
};

typedef struct dynarray *dyn_p;
typedef const struct dynarray *dyn_cp;

void
dyn_init(struct dynarray *darr, size_t element_size);

void
dyn_append(struct dynarray *darr, const void *element);

void
dyn_release(struct dynarray *darr);
