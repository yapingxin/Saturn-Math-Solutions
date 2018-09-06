#ifndef _INC_UTILITIES_Matrix_CSV_Integration_Matrix_CSV_Func_H
#define _INC_UTILITIES_Matrix_CSV_Integration_Matrix_CSV_Func_H

// !!! Notice !!!
// For DSP C2000 serial chip, please use uint16_t as HResult
#ifndef HResult
#define HResult unsigned int
#endif

#define MAX_FOLDER_LEN  240

HResult matrix_add_csv(const char* in_folder, const char* out_folder);

const char* get_latest_errmsg();


#endif // !_INC_UTILITIES_Matrix_CSV_Integration_Matrix_CSV_Func_H
