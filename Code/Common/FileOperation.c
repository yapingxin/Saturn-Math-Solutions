#include "FileOperation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Issue #01: Fix Linux compiling issue [Begin] */
#ifndef LONG_MAX
#define LONG_MAX      2147483647L   // maximum (signed) long value
#endif

#ifndef LONG_MIN
#define LONG_MIN    (-2147483647L - 1) // minimum (signed) long value
#endif
/* Issue #01: Fix Linux compiling issue [END] */


/* returns UInt32_NOP on error. */
size_t get_filesize(const char* filename)
{
	FILE* file;
	long size;
	file = fopen(filename, "rb");
	if (!file) return UInt32_NOP;

	if (fseek(file, 0, SEEK_END) != 0)
	{
		fclose(file);
		return UInt32_NOP;
	}

	size = ftell(file);
	/* It may give LONG_MAX as directory size, this is invalid for us. */
	if (size == LONG_MAX) return UInt32_NOP;

	fclose(file);

	if (size < 0)
	{
		return UInt32_NOP;
	}

	return (size_t)size;
}


bool is_file_exist(const char* filepath)
{
	return access(filepath, 0) == 0;
}


const char* get_file_extension(const char *filepath)
{
	static const char dir_separator_1 = '/';
	static const char dir_separator_2 = '\\';

	char* dir_pos_1 = strrchr(filepath, dir_separator_1);
	char* dir_pos_2 = strrchr(filepath, dir_separator_2);
	char* dir_pos = dir_pos_2 > dir_pos_1 ? dir_pos_2 : dir_pos_1;
	char* ext_pos = strrchr(filepath, '.');

	if (ext_pos <= dir_pos + 1)
	{
		return NULL;
	}
	else
	{
		return filepath + (size_t)(ext_pos - (char*)filepath);
	}
}



// Return values:
//      HResult_OK                  | Success
//      HResult_DIR_LOOKUP_FAILED   | Failed
HResult lookup_dir_files(const char* folder, lookup_dir_file_callback callback, void* data)
{
    HResult rc = HResult_OK;
    struct dirent *ptr;
    DIR *dir = NULL;

    dir = opendir(folder);
    if (dir == NULL)
    {
        rc = HResult_DIR_LOOKUP_FAILED;
        goto EXIT;
    }

    while ((ptr = readdir(dir)) != NULL)
    {
        // ignore "." and ".."
        if (ptr->d_name[0] == '.')
        {
#ifdef _WIN32
            free(ptr);
#endif
            continue;
        }

        if (ptr->d_type == DT_REG)
        {
            if (callback)
            {
                callback(ptr->d_name, data);
            }
        }

#ifdef _WIN32
        free(ptr);
#endif

    }

EXIT:
    if (dir)
    {
        closedir(dir);
    }
    return rc;
}



// Return values:
//	    HResult_OK                         1 | Success;
//		HResult_PARAM_NULL        0x00110000 | Any of the input parameters is NULL;
//      HResult_PARAM_NULL | 1    0x00110001 | The input parameter path or filename is empty;
HResult path_filename_combine(char* fullpath_buf, const char* path, const char* filename)
{
    static const char dir_separator_1 = '/';
    static const char dir_separator_2 = '\\';

    HResult rc = HResult_OK;
    size_t path_len_0 = 0;
    size_t path_len_1 = 0;
    char* dir_split_pos_1 = NULL;
    char* dir_split_pos_2 = NULL;
    char* dir_split_pos = NULL;
    char split_ch = '\0';

    if (fullpath_buf == NULL || path == NULL || filename == NULL)
    {
        rc = HResult_PARAM_NULL;
        goto EXIT;
    }

    path_len_0 = strlen(path);
    path_len_1 = strlen(filename);

    if (path_len_0 == 0 || path_len_1 == 0)
    {
        rc = HResult_PARAM_NULL | 1;
        goto EXIT;
    }

    dir_split_pos_1 = strrchr(path, dir_separator_1);
    dir_split_pos_2 = strrchr(path, dir_separator_2);
    dir_split_pos = dir_split_pos_2 > dir_split_pos_1 ? dir_split_pos_2 : dir_split_pos_1;

    strcpy(fullpath_buf, path);

    if (dir_split_pos != (path + path_len_0 - 1))
    {
        split_ch = dir_split_pos[0];
        fullpath_buf[path_len_0] = split_ch;
    }

    strcat(fullpath_buf, filename);

EXIT:
    return rc;
}
