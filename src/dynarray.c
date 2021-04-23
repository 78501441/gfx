#include <stdlib.h>
#include <string.h>

#include "dynarray.h"

#define DEFAULT_INITIAL_CAPACITY 4

void
dyn_init(struct dynarray *darr, unsigned long element_size)
{
  darr->capacity     = DEFAULT_INITIAL_CAPACITY;
  darr->count        = 0;
  darr->element_size = element_size;
  darr->data         = malloc(element_size * darr->capacity);
}

void
dyn_append(struct dynarray *darr, const void *element)
{
  darr->count += 1;
  if (darr->count > darr->capacity) {
    darr->data     = realloc(darr->data, darr->count * darr->element_size);
    darr->capacity = darr->count;
  }
  memcpy(darr->data + (darr->count - 1) * darr->element_size,
         element,
         darr->element_size);
}

void
dyn_release(struct dynarray *darr)
{
  if (darr->data) {
    free(darr->data);
    darr->data         = NULL;
    darr->capacity     = 0;
    darr->count        = 0;
    darr->element_size = 0;
  }
}
