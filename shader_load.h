
#ifndef SHADER_LOAD_H_
#define SHADER_LOAD_H_


#include <sys/types.h>


#define E_NOT_FILE 2


struct shader_desc {
  unsigned int type;
  const char *source;
  const char *name;
};


/**
 * @brief read_file Reads file by path.
 * @param fn File path.
 * @param buf Buffer to store content into.
 * @param buflen Size of buffer in bytes.
 * @return Read bytecount.
 */
ssize_t
read_file(const char *fn, char *buf, size_t buflen);


/**
 * @brief file_size Determines file size.
 * @param fpath Target file path.
 * @return Filesize in bytes, or error value < 0 otherwise.
 */
long
file_size(const char *fpath);


int
check_size_error(long result, const char *fn);


unsigned int
compile_shaders(const struct shader_desc *shaders,
                unsigned int shaders_count);


#endif /* SHADER_LOAD_H_ */
