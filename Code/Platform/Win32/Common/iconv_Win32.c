#include "iconv_Win32.h"
#include <Windows.h>
#include <memory.h>

bool iconv_utf8_to_utf16le(wchar_t* wbuf, size_t* pLen, const char* p_utf8, const size_t n_utf8)
{
    int32_t n_len = 0;
    int32_t n_ret = 0;

    n_len = MultiByteToWideChar(CP_UTF8, 0, p_utf8, (int)n_utf8, NULL, 0);
    if (n_len <= 0)
    {
        return false;
    }

    *pLen = n_len;
    wbuf[0] = L'\0';

    n_ret= MultiByteToWideChar(CP_UTF8, 0, p_utf8, (int)n_utf8, wbuf, n_len);
    if (n_ret != n_len)
    {
        return false;
    }

    return true;
}


bool iconv_utf16le_to_utf8(char* buf, size_t* pLen, const wchar_t* p_unicode, const size_t n_unicode)
{
    int32_t n_len = 0;
    int32_t n_ret = 0;

    n_len= WideCharToMultiByte(CP_UTF8, 0, p_unicode, (int)n_unicode, NULL, 0, NULL, NULL);
    if (n_len <= 0)
    {
        return false;
    }

    *pLen = n_len;
    buf[0] = '\0';

    n_ret = WideCharToMultiByte(CP_UTF8, 0, p_unicode, (int)n_unicode, buf, n_len, NULL, NULL);
    if (n_ret != n_len)
    {
        return false;
    }

    return true;
}


/*
wchar_t* utf16le_text_init(wchar_t* wbuf, uint32_t len)
{
    return (wchar_t*)memset(wbuf, 0, (len + 1) * sizeof(wchar_t));
}
*/
