#pragma once

#ifndef _INC_WIN32_OsacbmLib_COMMON_ICONV_WIN32_H
#define _INC_WIN32_OsacbmLib_COMMON_ICONV_WIN32_H

#include "Common/datatypes.h"


bool iconv_utf8_to_utf16le(wchar_t* wbuf, size_t* pLen, const char* p_utf8, const size_t n_utf8);
bool iconv_utf16le_to_utf8(char* buf, size_t* pLen, const wchar_t* p_unicode, const size_t n_unicode);

#endif // !_INC_WIN32_OsacbmLib_COMMON_ICONV_WIN32_H
