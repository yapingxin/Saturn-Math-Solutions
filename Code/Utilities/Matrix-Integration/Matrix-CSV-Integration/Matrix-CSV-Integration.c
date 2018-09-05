// Matrix-CSV-Integration.c : Defines the entry point for the console application.
//

#ifdef _WIN32
	#include "stdafx.h"
	#ifdef HAVE_VLD
		#include <vld.h>
	#endif
#endif

#include <stdio.h>
#include "Common/datatypes.h"
#include "Matrix-CSV-Func.h"

#ifdef _WIN32
    #define IN__PATH "D:\\Pub\\Test"
    #define OUT_PATH "D:\\Pub\\TEST_Out"
#else
    #define IN__PATH "~/test_folders/in"
    #define OUT_PATH "~/test_folders/out"
#endif


int main()
{
    HResult rc = HResult_Unknown;


    rc = matrix_add_csv(IN__PATH, OUT_PATH);

    printf("rc: 0x%08X\n", rc);

	return 0;
}

