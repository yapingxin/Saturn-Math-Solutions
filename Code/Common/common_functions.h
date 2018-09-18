#ifndef _INC_CLANG_COMMON_common_functions_H
#define _INC_CLANG_COMMON_common_functions_H

#include "datatypes.h"

double precision_round(const double x, const uint8_t k);

void get_precision_round(double* p_Value, const double x, const uint8_t k);

#ifdef _WIN32
// Shall fix for UTF-8
const TCHAR* dump_LPCSTR_to_LCPTSTR(const char* str);
#endif

#endif // !_INC_CLANG_COMMON_common_functions_H
