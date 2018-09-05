#include "FileOperation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
