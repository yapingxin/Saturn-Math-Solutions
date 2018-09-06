#include "Matrix-CSV-Common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* buf_seek_char(char* buf_begin, char* buf_end, const char ch);
static uint32_t get_cols_count_by_comma(char* data_begin, char* data_lend);
static HResult get_double_elem_by_comma(char* data_begin, char* data_lend, uint32_t* p_cur_index, double* data_buf);


void CSV_Parse_Info_Cleanup(CSV_Parse_Info* data)
{
    if (data)
    {
        if (data->elems)
        {
            free(data->elems);
        }
    }
}



// Return values:
//	    HResult_OK (1)                      | Success;
//		HResult_PARAM_NULL (0x00110000)     | Any of the input parameters is NULL;
//      HResult_FILE_CannotRead             | SCV file cannnot read;
//      HResult_ALLOC_FAIL                  | Memory alloc failed;
//      HResult_DATA_INVALID                | Failed when reading CSV file;
HResult Create_CSV_Parse_Info(CSV_Parse_Info* decode_data, const char* csv_path)
{
    HResult rc = HResult_OK;
    FILE* pFile;
    long file_seek_len;
    size_t buf_len;
    char* buffer = NULL;
    char* data_begin = NULL;
    char* buf_end = NULL;
    char* data_lend = NULL;
    uint32_t cur_cols = 0;
    uint32_t cur_rows = 0;
    uint32_t cur_elem = 0;
    uint32_t comma_cols = 0;
    uint32_t count_rows = 0;

    if (decode_data == NULL || csv_path == NULL)
    {
        rc = HResult_PARAM_NULL;
        goto EXIT;
    }

    pFile = fopen(csv_path, "rb");
    if (pFile == NULL)
    {
        rc = HResult_FILE_CannotRead;
        goto EXIT;
    }

    // get file size
    fseek(pFile, 0, SEEK_END);
    file_seek_len = ftell(pFile);
    rewind(pFile);

    buffer = (char*)malloc(sizeof(char)*file_seek_len);
    if (buffer == NULL)
    {
        rc = HResult_ALLOC_FAIL;
        fclose(pFile);
        goto EXIT;
    }

    buf_len = fread(buffer, 1, file_seek_len, pFile);
    if (buf_len != file_seek_len)
    {
        rc = HResult_DATA_INVALID;
        fclose(pFile);
        goto EXIT;
    }

    fclose(pFile);

    data_begin = buffer;
    buf_end = buffer + buf_len - 1;

    data_lend = buf_seek_char(data_begin, buf_end, 0x0D);
    if (data_lend == NULL)
    {
        rc = HResult_DECODE_FAIL;
        goto EXIT;
    }

    comma_cols = get_cols_count_by_comma(data_begin, data_lend);
    cur_cols = comma_cols;

    if (comma_cols == 0)
    {
        rc = HResult_DECODE_FAIL;
        goto EXIT;
    }

    count_rows = 1;

    if (data_lend[1] == 0x0A)
    {
        data_begin = data_lend + 2;
    }
    else
    {
        data_begin = data_lend + 1;
    }

    while (data_begin < buf_end)
    {
        data_lend = buf_seek_char(data_begin, buf_end, 0x0D);

        comma_cols = get_cols_count_by_comma(data_begin, data_lend);
        if (comma_cols != cur_cols)
        {
            rc = HResult_DECODE_FAIL;
            goto EXIT;
        }

        count_rows++;

        if (data_lend[1] == 0x0A)
        {
            data_begin = data_lend + 2;
        }
        else
        {
            data_begin = data_lend + 1;
        }
    }

    decode_data->cols = comma_cols;
    decode_data->rows = count_rows;
    decode_data->elems_count = comma_cols * count_rows;
    decode_data->elems = malloc(decode_data->elems_count * sizeof(double));

    cur_cols = 0;
    cur_rows = 0;
    cur_elem = 0;

    data_begin = buffer;
    data_lend = buf_seek_char(data_begin, buf_end, 0x0D);

    rc = get_double_elem_by_comma(data_begin, data_lend, &cur_elem, decode_data->elems);
    if (rc != HResult_OK)
    {
        goto EXIT;
    }

    if (data_lend[1] == 0x0A)
    {
        data_begin = data_lend + 2;
    }
    else
    {
        data_begin = data_lend + 1;
    }

    while (data_begin < buf_end)
    {
        data_lend = buf_seek_char(data_begin, buf_end, 0x0D);

        rc = get_double_elem_by_comma(data_begin, data_lend, &cur_elem, decode_data->elems);
        if (rc != HResult_OK)
        {
            goto EXIT;
        }

        if (data_lend[1] == 0x0A)
        {
            data_begin = data_lend + 2;
        }
        else
        {
            data_begin = data_lend + 1;
        }
    }

EXIT:
    if (buffer)
    {
        free(buffer);
    }

    return rc;
}


// Return value:
//      NULL        | Not found
//      Not null    | found
static char* buf_seek_char(char* buf_begin, char* buf_end, const char ch)
{
    char* found = NULL;
    char* find_ptr = NULL;
    
    for (find_ptr = buf_begin; find_ptr < buf_end; find_ptr++)
    {
        if (find_ptr[0] == ch)
        {
            found = find_ptr;
            break;
        }
    }

    return found;
}

static uint32_t get_cols_count_by_comma(char* data_begin, char* data_lend)
{
    const char comma = ',';
    uint32_t count = 0;
    char* pos = data_begin;

    if (data_begin + 1 >= data_lend)
    {
        goto EXIT;
    }

    count = 1;

    while (pos != NULL)
    {
        pos = buf_seek_char(data_begin, data_lend, comma);
        if (pos == NULL)
        {
            break;
        }

        count++;

        if (pos + 1 >= data_lend)
        {
            pos = NULL;
        }
        else
        {
            data_begin = pos + 1;
        }
    }

EXIT:
    return count;
}

static HResult get_double_elem_by_comma(char* data_begin, char* data_lend, uint32_t* p_cur_index, double* data_buf)
{
    HResult rc = HResult_OK;
    double value = 0.0;
    const char comma = ',';
    char* pos = data_begin;

    if (data_begin + 1 >= data_lend)
    {
        rc = HResult_DECODE_FAIL;
        goto EXIT;
    }

    value = atof(pos);
    data_buf[*p_cur_index] = value;
    *p_cur_index = *p_cur_index + 1;

    while (pos != NULL)
    {
        pos = buf_seek_char(data_begin, data_lend, comma);
        if (pos == NULL)
        {
            break;
        }

        if (pos + 1 < data_lend)
        {
            value = atof(pos + 1);
        }
        else
        {
            value = 0.0;
        }

        data_buf[*p_cur_index] = value;
        *p_cur_index = *p_cur_index + 1;

        if (pos + 1 >= data_lend)
        {
            pos = NULL;
        }
        else
        {
            data_begin = pos + 1;
        }
    }

EXIT:
    return rc;
}
