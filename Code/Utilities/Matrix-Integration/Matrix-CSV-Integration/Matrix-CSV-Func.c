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
static void matrix_add(CSV_Parse_Info* dst, CSV_Parse_Info* src);
static HResult matrix_save(CSV_Parse_Info* p_csv_matrix, const char* out_folder);

static const char* err_msg = "Demo";

const char* get_latest_errmsg()
{
    return err_msg;
}


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
HResult matrix_add_csv(const char* in_folder, const char* out_folder, const bool verbose_mode)
{
    HResult rc = HResult_OK;
    HResult decode_rc = HResult_OK;

    Vector vector_filepath;
    Vector vector_decode_info;
    size_t i;
    FileName* p_FileName;
    FullPath fullpath;
    CSV_Parse_Info* p_decode_info = NULL;
    CSV_Parse_Info* p_result_matrix = NULL;

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

    p_result_matrix = (CSV_Parse_Info*)malloc(sizeof(CSV_Parse_Info));
    memset(p_result_matrix, 0, sizeof(CSV_Parse_Info));

    for (i = 0; i < vector_filepath.size; i++)
    {
        p_FileName = (FileName*)vector_get(&vector_filepath, i);

        memset(&fullpath, 0, sizeof(FullPath));
        path_filename_combine(fullpath.data, in_folder, p_FileName->data);

        p_decode_info = (CSV_Parse_Info*)malloc(sizeof(CSV_Parse_Info));
        memset(p_decode_info, 0, sizeof(CSV_Parse_Info));

        decode_rc = Create_CSV_Parse_Info(p_decode_info, fullpath.data);

        if (p_result_matrix->elems_count == 0)
        {
            p_result_matrix->cols = p_decode_info->cols;
            p_result_matrix->rows = p_decode_info->rows;
            p_result_matrix->elems_count = p_decode_info->elems_count;

            p_result_matrix->elems = malloc(p_result_matrix->elems_count * sizeof(double));
            memset(p_result_matrix->elems, 0, p_result_matrix->elems_count * sizeof(double));
        }
        else
        {
            if (p_result_matrix->cols != p_decode_info->cols ||
                p_result_matrix->rows != p_decode_info->rows ||
                p_result_matrix->elems_count != p_decode_info->elems_count)
            {
                // for all the csv files, rows and cols is not the same.
                rc = HResult_DECODE_FAIL;
                free(p_decode_info);

                goto EXIT_VEC_RESOURCES;
            }
        }

        vector_push_back(&vector_decode_info, p_decode_info);

        //CSV_Parse_Info_Cleanup(p_decode_info);
        free(p_decode_info);

        //printf("Lookup file: %s\n", fullpath.data);
    }

    if (vector_decode_info.size > 0)
    {
        for (i = 0; i < vector_decode_info.size; i++)
        {
            p_decode_info = vector_get(&vector_decode_info, i);
            matrix_add(p_result_matrix, p_decode_info);
        }
    }

    rc = matrix_save(p_result_matrix, out_folder);

EXIT_VEC_RESOURCES:

    CSV_Parse_Info_Cleanup(p_result_matrix);
    free(p_result_matrix);

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


static void matrix_add(CSV_Parse_Info* dst, CSV_Parse_Info* src)
{
    uint32_t index = 0;

    for (index = 0; index < src->elems_count; index++)
    {
        dst->elems[index] += src->elems[index];
    }
}


// Return values:
//	    HResult_OK                         1 | Success;
//		HResult_FILE_OPEN_FAIL    0x00140001 | Any of the input parameters is NULL;

static HResult matrix_save(CSV_Parse_Info* p_csv_matrix, const char* out_folder)
{
    HResult rc = HResult_OK;
    FILE* pFile;
    FullPath fullpath;
    uint32_t iRow;
    uint32_t iCol;

    memset(&fullpath, 0, sizeof(FullPath));
    path_filename_combine(fullpath.data, out_folder, "Result.csv");

    pFile = fopen(fullpath.data, "w+");
    if (!pFile)
    {
        rc = HResult_FILE_OPEN_FAIL + 1;
        goto EXIT;
    }

    for (iRow = 0; iRow < p_csv_matrix->rows; iRow++)
    {
        for (iCol = 0; iCol < p_csv_matrix->cols; iCol++)
        {
            fseek(pFile, 0, SEEK_END);
            fprintf(pFile, "%f", p_csv_matrix->elems[iRow * p_csv_matrix->cols + iCol]);

            if (iCol < p_csv_matrix->cols - 1)
            {
                fseek(pFile, 0, SEEK_END);
                fprintf(pFile, ",");
            }
        }

        //fseek(pFile, 0, SEEK_END);
        //fprintf(pFile, "%c", 0x0D);

        fseek(pFile, 0, SEEK_END);
        fprintf(pFile, "%c", 0x0A);
    }

    fclose(pFile);

EXIT:
    return rc;
}

