#ifndef _INC_OsacbmLib_COMMON_DIRENT_WIN32_H
#define _INC_OsacbmLib_COMMON_DIRENT_WIN32_H

#include <sys/types.h>


#ifndef _MAX_PATH
#define _MAX_PATH 255
#endif

typedef struct _dirdesc {
    int     dd_fd;      /** file descriptor associated with directory */
    long    dd_loc;     /** offset in current buffer */
    long    dd_size;    /** amount of data returned by getdirentries */
    char    *dd_buf;    /** data buffer */
    int     dd_len;     /** size of data buffer */
    long    dd_seek;    /** magic cookie returned by getdirentries */
} DIR;

#define __dirfd(dp)    ((dp)->dd_fd)


struct dirent
{
    long d_ino;                /* inode number*/
    off_t d_off;               /* offset to this dirent*/
    unsigned short d_reclen;   /* length of this d_name*/
    unsigned char d_type;      /* the type of d_name*/
    char d_name[_MAX_PATH + 1]; /* file name (null-terminated)*/
};




DIR* opendir(const char* folder);
struct dirent *readdir(DIR *d);
int closedir(DIR *d);

#endif // !_INC_OsacbmLib_COMMON_DIRENT_WIN32_H

