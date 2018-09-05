#ifndef _INC_OsacbmLib_COMMON_datatypes_H
#define _INC_OsacbmLib_COMMON_datatypes_H

#include <stdbool.h>

#ifdef _WIN32

	#include <tchar.h>

	#include <sys/timeb.h>
	#include <sys/types.h>
	#include <winsock2.h>

	#include <sys/stat.h>
	#include <errno.h>

	#include "Win32/Common/unistd_Win32.h"
	#include "Win32/Common/dirent_Win32.h"
	#include "Win32/Common/iconv_Win32.h"

#else
	#include "unistd.h"
    #include <inttypes.h>
    #include <sys/types.h>
	#include <sys/time.h>
    #include <dirent.h>

    // Link: 
    // http://bbs.chinaunix.net/thread-2188912-2-1.html
    // https://stackoverflow.com/questions/5918697/undefined-reference-to-stricmp
    // https://blog.csdn.net/ACb0y/article/details/5333334
    #define strnicmp strncasecmp
    #define stricmp strcasecmp

    #define MAX_PATH 255
    typedef uint16_t TCHAR;
#endif

// ========================================
// Definition of reserved values
// ----------------------------------------
#define UInt16_MAX      0xFFFF
#define UInt16_NOP      0xFFFF
#define UInt16_ANY      0xFFFF


#define UInt32_MAX      0xFFFFFFFF
#define UInt32_NOP      0xFFFFFFFF
#define UInt32_ANY      0xFFFFFFFF

#define UInt64_MAX      0xFFFFFFFFFFFFFFFF
#define UInt64_NOP      0xFFFFFFFFFFFFFFFF
#define UInt64_ANY      0xFFFFFFFFFFFFFFFF
// ========================================


// =======================================
// Definition and macros for HResult
// ---------------------------------------
//typedef uint32_t HResult;
#ifndef HResult
    #define HResult uint32_t
#endif

#define PASS 1
#define FAIL 0

#define STATUS_OK               1
#define STATUS_Unknown          0
#define STATUS_ENCODE_FAIL 0x0100

#define HResult_Unknown 0
#define HResult_OK      1


#define HResult_FAILED              0x00100000
#define HResult_PARAM_NULL          0x00110000
#define HResult_PARAM_OUTRANGE      0x00120000
#define HResult_FILE_NO_ACCESS      0x00130000
#define HResult_FILE_NOTEXISTS      0x00130001
#define HResult_FILE_CannotRead     0x00130002
#define HResult_FILE_CannotWrite    0x00130004
#define HResult_DIR_LOOKUP_FAILED   0x00130008
#define HResult_FILE_OPEN_FAIL      0x00140000
#define HResult_SQLite3OpenFail     0x00140001
#define HResult_FILE_READ_FAIL      0x00150000
#define HResult_SQLite3QueryFail    0x00150001
#define HResult_SQLite3QueryNULL    0x00150002
#define HResult_SQLite3ExecFailed   0x00150004
#define HResult_SQLite3PrepareErr   0x00150008
#define HResult_FILE_WRITEFAIL      0x00160000
#define HResult_ALLOC_FAIL          0x00170000
#define HResult_DATA_INVALID        0x00180000
#define HResult_OBJECT_IS_NULL      0x00190000
#define HResult_STRING_TOOLONG      0x001A0000
#define HResult_DECODE_FAIL         0x001B0000

// =======================================

// Link: https://stackoverflow.com/questions/4114284/stdfloat-h-version-of-stdint-h
typedef float  float32_t;
typedef double float64_t;


typedef unsigned char	byte;
typedef char            sbyte;

// Link: https://blog.csdn.net/mybelief321/article/details/8994424
typedef int pid_t;

// Link: https://blog.csdn.net/u014357799/article/details/79108840
// Considering to use "sys/time.h" Replacement for Windows

#endif // !_INC_OsacbmLib_COMMON_datatypes_H
