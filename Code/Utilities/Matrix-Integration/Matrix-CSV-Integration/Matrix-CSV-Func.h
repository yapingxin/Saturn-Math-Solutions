#ifndef _INC_UTILITIES_Matrix_CSV_Integration_Matrix_CSV_Func_H
#define _INC_UTILITIES_Matrix_CSV_Integration_Matrix_CSV_Func_H

#include "Common/datatypes.h"

#define MAX_FOLDER_LEN  240

HResult matrix_add_csv(const char* in_folder, const char* out_folder, const bool verbose_mode);

const char* get_latest_errmsg();


#endif // !_INC_UTILITIES_Matrix_CSV_Integration_Matrix_CSV_Func_H
