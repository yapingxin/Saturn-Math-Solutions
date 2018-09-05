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


#endif // !_INC_UTILITIES_Matrix_CSV_Integration_Matrix_CSV_Common_H
