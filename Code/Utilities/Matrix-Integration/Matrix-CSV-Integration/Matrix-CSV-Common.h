#ifndef _INC_UTILITIES_Matrix_CSV_Integration_Matrix_CSV_Common_H
#define _INC_UTILITIES_Matrix_CSV_Integration_Matrix_CSV_Common_H

#include "Common/datatypes.h"

#define FILENAME_BUF_SIZE   256
#define FULLPATH_BUF_SIZE   512
#define FILENAME_VEC_PRECAP 16


typedef struct
{
    char data[FILENAME_BUF_SIZE];
} FileName;


typedef struct
{
    char data[FILENAME_BUF_SIZE];
} FullPath;


typedef struct
{
    uint32_t cols;
    uint32_t rows;

    uint32_t elems_count;
    double* elems;

} CSV_Parse_Info;

void CSV_Parse_Info_Cleanup(CSV_Parse_Info* data);
HResult Create_CSV_Parse_Info(CSV_Parse_Info* decode_data, const char* csv_path);

#endif // !_INC_UTILITIES_Matrix_CSV_Integration_Matrix_CSV_Common_H
