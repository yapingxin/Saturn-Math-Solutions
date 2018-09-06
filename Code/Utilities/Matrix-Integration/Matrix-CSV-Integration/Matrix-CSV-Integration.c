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
    #define IN__PATH "/home/yaping/test_folders/in"
    #define OUT_PATH "/home/yaping/test_folders/out"
#endif


int main(int argc, char* argv[])
{
    HResult rc = HResult_Unknown;

    if (argc < 3)
    {
        printf("\n\nPlease provide 2 input parametes, the first one is input directory path, the second is output directory path.\n");
        printf("For example: \n\n");
#ifdef _WIN32
        printf("Usage: Matrix-CSV-Integration.exe <input_folder> <output_folder>\n\n");
#else
        printf("Usage: ./Matrix-CSV-Integration <input_folder> <output_folder>\n\n");
#endif
        printf("Abort.\n\n");

        exit(-1);
    }

    rc = matrix_add_csv(argv[1], argv[2]);
    if (rc != HResult_OK)
    {
        printf("Error code    : 0x%08X\n", rc);
        printf("Error message : %s\n", get_latest_errmsg());
    }
    else
    {
        printf("\n\nThe output file Result.csvm has been put into your <output_folder>.\n\n");
    }

	return 0;
}

