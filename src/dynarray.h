#pragma once

struct dynarray {
  void *data;
  unsigned long capacity;
  unsigned long count;
  unsigned long element_size;
};

typedef struct dynarray *dyn_p;
typedef const struct dynarray *dyn_cp;

void
dyn_init(struct dynarray *darr, unsigned long element_size);

void
dyn_append(struct dynarray *darr, const void *element);

void
dyn_release(struct dynarray *darr);
