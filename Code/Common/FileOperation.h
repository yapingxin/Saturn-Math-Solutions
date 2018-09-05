#ifndef _INC_CLANG_COMMON_FileOperation_H
#define _INC_CLANG_COMMON_FileOperation_H

#include "datatypes.h"

/* returns UInt32_NOP on error. */
size_t get_filesize(const char* filename);

bool is_file_exist(const char* filepath);
const char* get_file_extension(const char *filepath);

#endif // !_INC_CLANG_COMMON_FileOperation_H
