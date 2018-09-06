#include "Matrix-CSV-Func.h"
#include "Common/datatypes.h"
#include "Common/FileOperation.h"
#include "Common/DS/vector.h"
#include "Matrix-CSV-Common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


static HResult check_in_out_folder(const char* in_folder, const char* out_folder);

static void lookup_subfiles_callback(const char* filename, void* data);


// Return values:
//	    HResult_OK                         1 | Success;
//		HResult_PARAM_NULL        0x00110000 | Any of the input parameters is NULL;
//      HResult_PARAM_NULL | 1    0x00110001 | Any of the input parameters is empty;
//      HResult_PARAM_NULL | 2    0x00110002 | Any of the input parameters is too long (over 240 characters);
//      HResult_FILE_NOTEXISTS    0x00130001 | Any of the input parameters is not exist as folder path;
//      HResult_FILE_CannotRead   0x00130002 | The path of in_folder cannot be read;
//      HResult_FILE_CannotWrite  0x00130004 | The path of out_folder cannot be written;
//      HResult_DIR_LOOKUP_FAILED 0x00130008 | Failed when lookup the path of in_folder for files;
//      HResult_OBJECT_IS_NULL|1  0x00190001 | Lookup in_folder but no .csv file found;
HResult matrix_add_csv(const char* in_folder, const char* out_folder)
{
	HResult rc = HResult_OK;
    HResult decode_rc= HResult_OK;
    
    Vector vector_filepath;
    Vector vector_decode_info;
    size_t i;
    FileName* p_FileName;
    FullPath fullpath;
    CSV_Parse_Info* p_decode_info = NULL;

    rc = check_in_out_folder(in_folder, out_folder);
    if (rc != HResult_OK)
    {
        // will get the error code defined for function check_in_out_folder()
        goto EXIT;
    }

    vector_setup(&vector_filepath, FILENAME_VEC_PRECAP, sizeof(FileName));

    rc = lookup_dir_files(in_folder, lookup_subfiles_callback, &vector_filepath);
    if (rc != HResult_OK)
    {
        // will get the error code HResult_DIR_LOOKUP_FAILED
        goto EXIT;
    }

    if (vector_filepath.size == 0)
    {
        rc = HResult_OBJECT_IS_NULL + 1;
    }

    vector_setup(&vector_decode_info, vector_filepath.size, sizeof(CSV_Parse_Info));

    for (i = 0; i < vector_filepath.size; i++)
    {
        p_FileName = (FileName*)vector_get(&vector_filepath, i);
        
        memset(&fullpath, 0, sizeof(FullPath));
        path_filename_combine(fullpath.data, in_folder, p_FileName->data);

        p_decode_info = (CSV_Parse_Info*)malloc(sizeof(CSV_Parse_Info));
        memset(p_decode_info, 0, sizeof(CSV_Parse_Info));

        decode_rc = Create_CSV_Parse_Info(p_decode_info, fullpath.data);

        vector_push_back(&vector_decode_info, p_decode_info);

        //CSV_Parse_Info_Cleanup(p_decode_info);
        free(p_decode_info);

        printf("Lookup file: %s\n", fullpath.data);
    }

    vector_clear(&vector_filepath);
    vector_destroy(&vector_filepath);

    
    if (vector_decode_info.size > 0)
    {
        for (i = 0; i < vector_decode_info.size; i++)
        {
            p_decode_info = vector_get(&vector_decode_info, i);
            CSV_Parse_Info_Cleanup(p_decode_info);
        }
    }
    

    vector_clear(&vector_decode_info);
    vector_destroy(&vector_decode_info);

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
    Vector* vector;
    FileName filename_st;
    
    if (filename == NULL || data == NULL)
    {
        goto EXIT;
    }

    vector = (Vector*)data;

    filename_len = strlen(filename);
    if (filename_len <= expected_ext_len)
    {
        goto EXIT;
    }

    if (filename_len >= FILENAME_BUF_SIZE)
    {
        // Error: File name is too long
        goto EXIT;
    }

    if (strnicmp(filename + filename_len - expected_ext_len, expected_ext, expected_ext_len) != 0)
    {
        goto EXIT;
    }

    memset(&filename_st, 0, sizeof(FileName));
    strncpy(filename_st.data, filename, filename_len);

    vector_push_back(vector, &filename_st);

EXIT:
    return;
}
