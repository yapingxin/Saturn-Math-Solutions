
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include "dirent_Win32.h"

static HANDLE hFind = 0;



DIR* opendir(const char* folder)
{
    DIR* dir = NULL;
    WIN32_FIND_DATA FindData;
    TCHAR namebuf[_MAX_PATH + 1];
    int i;

    memset(&FindData, 0, sizeof(WIN32_FIND_DATA));
    memset(namebuf, 0, (_MAX_PATH + 1) * sizeof(TCHAR));

    for (i = 0; i < _MAX_PATH + 1; i++)
    {
        if (folder[i] != '\0')
        {
            namebuf[i] = folder[i];
        }
        else
        {
            // Shall consider for several conditions.
            namebuf[i] = '\\';
            namebuf[i + 1] = '*';
            namebuf[i + 2] = '.';
            namebuf[i + 3] = '*';

            break;
        }
    }

    hFind = FindFirstFile(namebuf, &FindData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("FindFirstFile failed (%d)\n", GetLastError());
        goto EXIT;
    }

    dir = (DIR *)malloc(sizeof(DIR));
    if (!dir)
    {
        printf("DIR memory allocate fail\n");
        goto EXIT;
    }

    memset(dir, 0, sizeof(DIR));
    dir->dd_fd = 0; // simulate return

EXIT:
    return dir;
}


struct dirent *readdir(DIR *d)
{
    struct dirent *dirent = NULL;
    int i = 0;
    BOOL bf = 0;
    WIN32_FIND_DATA FileData;

    memset(&FileData, 0, sizeof(WIN32_FIND_DATA));

    if (!d)
    {
        goto EXIT;
    }

    bf = FindNextFile(hFind, &FileData);
    if (!bf)
    {
        goto EXIT;
    }

    dirent = (struct dirent *)malloc(sizeof(struct dirent));
    if (!dirent)
    {
        goto EXIT;
    }

    memset(dirent, 0, sizeof(struct dirent));
    for (i = 0; i < _MAX_PATH + 1; i++)
    {
        dirent->d_name[i] = (char)FileData.cFileName[i];
        if (dirent->d_name[i] == '\0') break;
    }

    dirent->d_reclen = i;
    dirent->d_reclen = (unsigned short)FileData.nFileSizeLow;

    //check there is file or directory
    if (FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
        dirent->d_type = 2;
    }
    else
    {
        dirent->d_type = 1;
    }

EXIT:
    return dirent;
}


int closedir(DIR *d)
{
    if (!d) return -1;

    hFind = 0;
    free(d);
    return 0;
}
