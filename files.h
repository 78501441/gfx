#ifndef FILES_H_
#define FILES_H_


#include <sys/types.h>


#define GFXDEMO_E_NOT_FILE 2


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
 * If target is not a file then -2 returned.
 */
long
file_size(const char *fpath);


/**
 * @brief check_size_error Checks file_size() return status.
 * @param result result from file_size
 * @param fn Filepath
 * @return
 */
int
check_size_error(long result, const char *fn);


#endif
