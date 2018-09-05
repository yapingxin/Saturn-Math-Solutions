#include "Matrix-CSV-Func.h"
#include "Common/datatypes.h"
#include "Common/FileOperation.h"
#include "Common/DS/vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


static HResult check_in_out_folder(const char* in_folder, const char* out_folder);

static void lookup_subfiles_callback(const char* filename, void* data);


// Return values:
//	    HResult_OK                         1 | Success;
//		HResult_PARAM_NULL        0x00110000 | Any of the input parameters is NULL;
//                                0x00110001 | Any of the input parameters is empty;
//                                0x00110002 | Any of the input parameters is too long (over 240 characters);
//      HResult_FILE_NOTEXISTS    0x00130001 | Any of the input parameters is not exist as folder path;
//      HResult_FILE_CannotRead   0x00130002 | The path of in_folder cannot be read;
//      HResult_FILE_CannotWrite  0x00130004 | The path of out_folder cannot be written;
//      HResult_DIR_LOOKUP_FAILED 0x00130008 | Failed when lookup the path of in_folder for files;
HResult matrix_add_csv(const char* in_folder, const char* out_folder)
{
	HResult rc = HResult_OK;
    
    //_finddata_t fileInfo;

    rc = check_in_out_folder(in_folder, out_folder);
    if (rc != HResult_OK)
    {
        // will get the error code defined for function check_in_out_folder()
        goto EXIT;
    }

    rc = lookup_dir_files(in_folder, lookup_subfiles_callback, NULL);
    if (rc != HResult_OK)
    {
        // will get the error code HResult_DIR_LOOKUP_FAILED
        goto EXIT;
    }

EXIT:
	return rc;
}



// Return values:
//	    HResult_OK (1)                      | Success;
//		HResult_PARAM_NULL (0x00110000)     | Any of the input parameters is NULL;
//                          0x00110001      | Any of the input parameters is empty;
//                          0x00110002      | Any of the input parameters is too long (over 240 characters);
//      HResult_FILE_NOTEXISTS (0x00130001) | Any of the input parameters is not exist as folder path.
//      HResult_FILE_CannotRead (0x00130002)| The path of in_folder cannot be read.
//      HResult_FILE_CannotWrite 0x00130004 | The path of out_folder cannot be written.
static HResult check_in_out_folder(const char* in_folder, const char* out_folder)
{
    HResult rc = HResult_OK;
    size_t path_len_0 = 0;
    size_t path_len_1 = 0;

    /******************************/
    /* Check folder path :: Begin */
    /******************************/

    if (in_folder == NULL || out_folder == NULL)
    {
        rc = HResult_PARAM_NULL;
        goto EXIT;
    }

    path_len_0 = strlen(in_folder);
    path_len_1 = strlen(out_folder);

    if (path_len_0 == 0 || path_len_1 == 0)
    {
        rc = HResult_PARAM_NULL | 1;
        goto EXIT;
    }

    if (path_len_0 > MAX_FOLDER_LEN || path_len_1 > MAX_FOLDER_LEN)
    {
        rc = HResult_PARAM_NULL | 2;
        goto EXIT;
    }

    if (access(in_folder, 0) != 0 || access(out_folder, 0) != 0)
    {
        rc = HResult_FILE_NOTEXISTS;
        goto EXIT;
    }

    if (access(in_folder, 4) != 0)
    {
        rc = HResult_FILE_CannotRead;
        goto EXIT;
    }

    if (access(out_folder, 2) != 0)
    {
        rc = HResult_FILE_CannotWrite;
        goto EXIT;
    }

    /******************************/
    /* Check folder path :: End   */
    /******************************/

EXIT:
    return rc;
}


static void lookup_subfiles_callback(const char* filename, void* data)
{
    const char* expected_ext = ".csv";
    const size_t expected_ext_len = strlen(expected_ext);
    size_t filename_len = 0;
    
    if (!filename)
    {
        goto EXIT;
    }

    filename_len = strlen(filename);
    if (filename_len <= expected_ext_len)
    {
        goto EXIT;
    }

    if (strnicmp(filename + filename_len - expected_ext_len, expected_ext, expected_ext_len) != 0)
    {
        goto EXIT;
    }

    printf("Filename: %s\n", filename);

EXIT:
    return;
}
