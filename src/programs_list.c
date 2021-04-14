

#include "dynarray.h"
#include "ranges.h"

#include "programs_list.h"

size_t
prepare_programs_list(dyn_p storage, const char *blob_start, size_t blob_size)
{
  const char shader_signature[] = "#shader";
  struct shader_source entry;
  const char *spec     = blob_start;
  const char *blob_end = blob_start + blob_size;
  while (1) {

    spec = next_signature(
        spec, blob_size, shader_signature, sizeof(shader_signature) - 1);

    if (!spec) {

      struct shader_source *last_elem =
          &((struct shader_source *)storage->data)[storage->count - 1];

      last_elem->length = blob_end - last_elem->start;

      break;
    }

    if (storage->count) {

      struct shader_source *last_elem =
          &((struct shader_source *)storage->data)[storage->count - 1];

      last_elem->length = spec - last_elem->start;
    }

    spec += sizeof(shader_signature);
    entry.start = spec + sympos(spec, blob_end - spec, '\n') + 1;

    if (bytes_equal(spec, "vert", 4)) {
      entry.type = st_vertex;
    } else if (bytes_equal(spec, "frag", 4)) {
      entry.type = st_fragment;
    } else {
      entry.type = st_unknown;
    }

    dyn_append(storage, &entry);
  }
  return storage->count;
}
