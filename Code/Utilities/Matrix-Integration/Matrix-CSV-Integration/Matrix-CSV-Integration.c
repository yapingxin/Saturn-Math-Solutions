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
#include "log_settings.h"

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
    bool verbose_mode = false;

    if (argc < 3)
    {
        printf("\n\nPlease provide 2 input parametes, the first one is input directory path, the second is output directory path.\n");
        printf("For example: \n\n");
#ifdef _WIN32
        printf("Usage: Matrix-CSV-Integration.exe <input_folder> <output_folder> [-v]\n\n");
#else
        printf("Usage: ./Matrix-CSV-Integration <input_folder> <output_folder> [-v]\n\n");
#endif
        printf("Abort.\n\n");

        exit(-1);
    }

    if (argc >= 4)
    {
        if (strnicmp(argv[3], "-v", 2) == 0)
        {
            verbose_mode = true;
        }
    }

    printf("\n\n");
    if (verbose_mode)
    {
        LOG_SETUP();
        LOG_INFO("Entering verbose mode via standard console output and text log file creation.");
    }
    else
    {
        printf("\n\n");
    }

    rc = matrix_add_csv(argv[1], argv[2], verbose_mode);
    if (rc != HResult_OK)
    {
        if (verbose_mode)
        {
            LOG_ERROR("Error code    : 0x%08X", rc);
            LOG_ERROR("Error message : %s", get_latest_errmsg());
        }
        else
        {
            printf("Error code    : 0x%08X\n", rc);
            printf("Error message : %s\n", get_latest_errmsg());
        }
    }
    else
    {
        if (verbose_mode)
        {
            LOG_INFO("The output file Result.csv has been put into the output path: %s.", argv[2]);
        }
        else
        {
            printf("The output file Result.csv has been put into the output path: %s.\n", argv[2]);
        }
    }
    printf("\n");

    if (verbose_mode)
    {
        LOG_CLEANUP();
    }

	return 0;
}

