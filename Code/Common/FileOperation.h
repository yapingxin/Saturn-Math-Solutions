#ifndef _INC_CLANG_COMMON_FileOperation_H
#define _INC_CLANG_COMMON_FileOperation_H

#include "datatypes.h"

typedef void (*lookup_dir_file_callback)(const char* filename, void* data);

// Return values:
//      HResult_OK      | Success
//      HResult_FAILED  | Failed
HResult lookup_dir_files(const char* folder, lookup_dir_file_callback callback, void* data);

/* returns UInt32_NOP on error. */
size_t get_filesize(const char* filename);

bool is_file_exist(const char* filepath);
const char* get_file_extension(const char *filepath);

// Return values:
//	    HResult_OK                         1 | Success;
//		HResult_PARAM_NULL        0x00110000 | Any of the input parameters is NULL;
//      HResult_PARAM_NULL | 1    0x00110001 | The input parameter path or filename is empty;
HResult path_filename_combine(char* fullpath_buf, const char* path, const char* filename);

#endif // !_INC_CLANG_COMMON_FileOperation_H
