#ifndef _H_LOG_
#define _H_LOG_

#include "Common/datatypes.h"

/*
 * iLOG3 - log function library written in c
 * author	: calvin
 * email	: calvinwilliams.c@gmail.com
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory.
 */

#ifdef __cplusplus
extern "C" {
#endif

#if ( defined _WIN32 )
#ifndef _WINDLL_FUNC
#define _WINDLL_FUNC		_declspec(dllexport)
#endif
#elif ( defined __unix ) || ( defined _AIX ) || ( defined __linux__ ) || ( defined __hpux )
#ifndef _WINDLL_FUNC
#define _WINDLL_FUNC
#endif
#endif

#if ( defined _WIN32 )
#include <windows.h>
#include <share.h>
#include <io.h>
#include <fcntl.h>
#elif ( defined __unix ) || ( defined _AIX ) || ( defined __linux__ ) || ( defined __hpux )
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <syslog.h>
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#if ( defined __STDC_VERSION__ ) && ( __STDC_VERSION__ >= 199901 )
struct tm *localtime_r(const time_t *timep, struct tm *result);
#endif

/* 函数返回值宏 */ /* function returns value macros */
#define LOG_RETURN_ERROR_ALLOC			-11	/* 申请内存失败 */ /* alloc memory failure */
#define LOG_RETURN_ERROR_INTERNAL		-12	/* 内部错误 */ /* internal error */
#define LOG_RETURN_ERROR_ALLOC_MAX		-13	/* 内存使用超限 */ /* memory usage transfinite */
#define LOG_RETURN_ERROR_PARAMETER		-14	/* 参数错误 */ /* parameter invalid */
#define LOG_RETURN_ERROR_NOTSUPPORT		-17	/* 暂不支持 */ /* not support */
#define LOG_RETURN_ERROR_CREATEFILE		-21	/* 创建文件失败 */ /* failed to create file */
#define LOG_RETURN_ERROR_OPENFILE		-22	/* 打开文件失败 */ /* failed to open file  */
#define LOG_RETURN_ERROR_WRITEFILE		-23	/* 写文件失败 */ /* failed to write file */
#define LOG_RETURN_ERROR_TOO_MANY_STYLES	-31	/* 风格列太多了 */
#define LOG_RETURN_ERROR_CONFIGFILE_NOTFOUND	-51	/* 找不到配置文件 */
#define LOG_RETURN_ERROR_CONFIGFILE_INVALID	-52	/* 配置文件无效 */

/********************************************************/
/*   以下为基本特性 The basic features are as follows   */
/********************************************************/

/* 日志输出类型 */ /* log output type macros */
#define LOG_OUTPUT_INVALID	-1	/* 不设置 */ /* no set */
#define LOG_OUTPUT_STDOUT	1	/* 标准输出 */ /* stdout */
#define LOG_OUTPUT_STDERR	2	/* 标准错误输出 */ /* stderr */
#define LOG_OUTPUT_SYSLOG	3	/* UNIX&Linux的syslog 或 Windows的WINDOWS EVENT */
#define LOG_OUTPUT_FILE		11	/* 文件 */ /* file */
#define LOG_OUTPUT_CALLBACK	21	/* （使用自定义日志输出回调函数） */ /* using custom log output callback function */

/* 日志等级宏 */ /* log level macros */
#define LOG_LEVEL_INVALID	-1	/* 无效等级 */ /* no log */
#define LOG_LEVEL_DEBUG		0	/* 调试等级 */ /* debug level */
#define LOG_LEVEL_INFO		1	/* 普通信息等级 */ /* info level */
#define LOG_LEVEL_NOTICE	2	/* 通知等级 */ /* notice level */
#define LOG_LEVEL_WARN		3	/* 警告等级 */ /* warn level */
#define LOG_LEVEL_ERROR		4	/* 错误等级 */ /* error level */
#define LOG_LEVEL_FATAL		5	/* 严重错误等级 */ /* error level */
#define LOG_LEVEL_NOLOG		6	/* 不需要输出日志 */ /* no log */

#define LOG_LEVEL_DEFAULT	LOG_LEVEL_NOLOG

/* 行日志风格组合宏 */ /* Line logging combination style macros */
#define LOG_STYLE_INVALID	-1		/* 无效行日志风格 */
#define LOG_STYLE_DATE		0x00001		/* 日期"YYYY-MM-DD" */ /* date "YYYY-MM-DD" */
#define LOG_STYLE_DATETIME	0x00002		/* 日期时间"YYYY-MM-DD hh:mm:ss" */ /* date time "YYYY-MM-DD hh:mm:ss" */
#define LOG_STYLE_DATETIMEMS	0x00004		/* 日期时间毫秒"YYYY-MM-DD hh:mm:ss.6ms"（日期时间类宏互斥，优先自动选用信息量最多的） */ /* date time "YYYY-MM-DD hh:mm:ss.6ms" */
#define LOG_STYLE_LOGLEVEL	0x00008		/* 日志等级 */ /* log level */
#define LOG_STYLE_PID		0x00010		/* 进程id */ /* pid */
#define LOG_STYLE_TID		0x00020		/* 线程id */ /* tid */
#define LOG_STYLE_SOURCE	0x00040		/* "源代码文件名:源代码行号" */ /* source file name and row number */
#define LOG_STYLE_FORMAT	0x00080		/* 应用日志段 */ /* application text */
#define LOG_STYLE_NEWLINE	0x00100		/* 换行 */ /* new line */
#define LOG_STYLE_CUSTLABEL1	0x00200		/* 自定义标签1 */ /* custom label1 */
#define LOG_STYLE_CUSTLABEL2	0x00400		/* 自定义标签2 */ /* custom label2 */
#define LOG_STYLE_CUSTLABEL3	0x00800		/* 自定义标签3 */ /* custom label3 */
#define LOG_STYLE_CUSTLABEL4	0x01000		/* 自定义标签4 */ /* custom label4 */
#define LOG_STYLE_CUSTLABEL5	0x02000		/* 自定义标签5 */ /* custom label5 */
#define LOG_STYLE_CALLBACK	0x10000		/* （使用自定义行日志风格回调函数） */ /* using custom log style callback function  */

#define LOG_STYLES_DEFAULT	0

/* 其它宏 */ /* other macros */
#ifndef MAXLEN_FILENAME
#define MAXLEN_FILENAME		255
#endif

typedef struct tagLOG LOG ;
typedef struct tagLOGBUF LOGBUF ;

/* 管理日志句柄函数 */ /* log handle functions */
_WINDLL_FUNC LOG *CreateLogHandle();
_WINDLL_FUNC void DestroyLogHandle( LOG *g );

/* 自定义打开、输出、关闭日志函数类型 */ /* custom open , write , close log callback functions */
typedef int funcOpenLog( LOG *g , char *log_pathfilename , void **open_handle );
typedef int funcWriteLog( LOG *g , void **open_handle , int log_level , char *buf , long len , long *writelen );
typedef int funcChangeTest( LOG *g , void **test_handle );
typedef int funcCloseLog( LOG *g , void **open_handle );

#define LOG_NO_OUTPUTFUNC	NULL , NULL , NULL , NULL , NULL , NULL

/* 自定义行日志风格函数类型 */ /* custom log style callback functions */
typedef int funcLogStyle( LOG *g , LOGBUF *logbuf , char *c_filename , long c_fileline , int log_level , char *format , va_list valist );

#define LOG_NO_STYLEFUNC	NULL

#define func_log_style		funcLogStyle

/* 句柄环境设置函数 */ /* set log handle environment */
_WINDLL_FUNC int SetLogOutput( LOG *g , int output , char *log_pathfilename , funcOpenLog *pfuncOpenLogFirst , funcOpenLog *pfuncOpenLog , funcWriteLog *pfuncWriteLog , funcChangeTest *pfuncChangeTest , funcCloseLog *pfuncCloseLog , funcCloseLog *pfuncCloseLogFinally );
_WINDLL_FUNC int SetLogOutput2( LOG *g , int output , funcOpenLog *pfuncOpenLogFirst , funcOpenLog *pfuncOpenLog , funcWriteLog *pfuncWriteLog , funcChangeTest *pfuncChangeTest , funcCloseLog *pfuncCloseLog , funcCloseLog *pfuncCloseLogFinally , char *log_pathfilename_format , ... );
_WINDLL_FUNC int ReOpenLogOutput( LOG *g );
_WINDLL_FUNC int SetLogLevel( LOG *g , int log_level );
_WINDLL_FUNC int SetLogStyles( LOG *g , long log_styles , funcLogStyle *pfuncLogStyle );
_WINDLL_FUNC int SetLogStylesEx( LOG *g , long log_styles , funcLogStyle *pfuncLogStyle , funcLogStyle *pfuncLogStylePrefix );

/* 写日志函数 */ /* output log */
_WINDLL_FUNC int WriteLevelLog( LOG *g , char *c_filename , long c_fileline , int log_level , char *format , ... );
_WINDLL_FUNC int WriteDebugLog( LOG *g , char *c_filename , long c_fileline , char *format , ... );
_WINDLL_FUNC int WriteInfoLog( LOG *g , char *c_filename , long c_fileline , char *format , ... );
_WINDLL_FUNC int WriteNoticeLog( LOG *g , char *c_filename , long c_fileline , char *format , ... );
_WINDLL_FUNC int WriteWarnLog( LOG *g , char *c_filename , long c_fileline , char *format , ... );
_WINDLL_FUNC int WriteErrorLog( LOG *g , char *c_filename , long c_fileline , char *format , ... );
_WINDLL_FUNC int WriteFatalLog( LOG *g , char *c_filename , long c_fileline , char *format , ... );

/* 写十六进制块日志函数 */ /* output hex log */
_WINDLL_FUNC int WriteLevelHexLog( LOG *g , char *c_filename , long c_fileline , int log_level , char *buffer , long buflen , char *format , ... );
_WINDLL_FUNC int WriteDebugHexLog( LOG *g , char *c_filename , long c_fileline , char *buffer , long buflen , char *format , ... );
_WINDLL_FUNC int WriteInfoHexLog( LOG *g , char *c_filename , long c_fileline , char *buffer , long buflen , char *format , ... );
_WINDLL_FUNC int WriteNoticeHexLog( LOG *g , char *c_filename , long c_fileline , char *buffer , long buflen , char *format , ... );
_WINDLL_FUNC int WriteWarnHexLog( LOG *g , char *c_filename , long c_fileline , char *buffer , long buflen , char *format , ... );
_WINDLL_FUNC int WriteErrorHexLog( LOG *g , char *c_filename , long c_fileline , char *buffer , long buflen , char *format , ... );
_WINDLL_FUNC int WriteFatalHexLog( LOG *g , char *c_filename , long c_fileline , char *buffer , long buflen , char *format , ... );

#if ( defined __STDC_VERSION__ ) && ( __STDC_VERSION__ >= 199901 )

/* 写日志函数的可变参数宏 */ /* output log macros */
#define WRITELOG( _g_ , _log_level_ , ... )	WriteLevelLog( _g_ , __FILE__ , __LINE__ , _log_level_ , __VA_ARGS__ );
#define DEBUGLOG( _g_ , ... )			WriteDebugLog( _g_ , __FILE__ , __LINE__ , __VA_ARGS__ );
#define INFOLOG( _g_ , ... )			WriteInfoLog( _g_ , __FILE__ , __LINE__ , __VA_ARGS__ );
#define NOTICELOG( _g_ , ... )			WriteNoticeLog( _g_ , __FILE__ , __LINE__ , __VA_ARGS__ );
#define WARNLOG( _g_ , ... )			WriteWarnLog( _g_ , __FILE__ , __LINE__ , __VA_ARGS__ );
#define ERRORLOG( _g_ , ... )			WriteErrorLog( _g_ , __FILE__ , __LINE__ , __VA_ARGS__ );
#define FATALLOG( _g_ , ... )			WriteFatalLog( _g_ , __FILE__ , __LINE__ , __VA_ARGS__ );

/* 写十六进制块日志的可变参数宏 */ /* output hex log macros */
#define WRITEHEXLOG( _g_ , _log_level_ , _buf_ , _buf_size_ , ... )	WriteLevelHexLog( _g_ , __FILE__ , __LINE__ , _log_level_ , _buf_ , _buf_size_ , __VA_ARGS__ );
#define DEBUGHEXLOG( _g_ , _buf_ , _buf_size_ , ... )			WriteDebugHexLog( _g_ , __FILE__ , __LINE__ , _buf_ , _buf_size_ , __VA_ARGS__ );
#define INFOHEXLOG( _g_ , _buf_ , _buf_size_ , ... )			WriteInfoHexLog( _g_ , __FILE__ , __LINE__ , _buf_ , _buf_size_ , __VA_ARGS__ );
#define NOTICEHEXLOG( _g_ , _buf_ , _buf_size_ , ... )			WriteNoticeHexLog( _g_ , __FILE__ , __LINE__ , _buf_ , _buf_size_ , __VA_ARGS__ );
#define WARNHEXLOG( _g_ , _buf_ , _buf_size_ , ... )			WriteWarnHexLog( _g_ , __FILE__ , __LINE__ , _buf_ , _buf_size_ , __VA_ARGS__ );
#define ERRORHEXLOG( _g_ , _buf_ , _buf_size_ , ... )			WriteErrorHexLog( _g_ , __FILE__ , __LINE__ , _buf_ , _buf_size_ , __VA_ARGS__ );
#define FATALHEXLOG( _g_ , _buf_ , _buf_size_ , ... )			WriteFatalHexLog( _g_ , __FILE__ , __LINE__ , _buf_ , _buf_size_ , __VA_ARGS__ );

#endif

/* 风格替换宏 */
#define create_log_handle	CreateLogHandle
#define destroy_log_handle	DestroyLogHandle

#define func_open_log		funcOpenLog
#define func_write_log		funcWriteLog
#define func_change_test	funcChangeTest
#define func_close_log		funcCloseLog

#define set_log_output		SetLogOutput
#define set_log_output2		SetLogOutput2
#define set_log_level		SetLogLevel
#define set_log_styles		SetLogStyles

#define write_level_log		WriteLevelLog
#define write_debug_log		WriteDebugLog
#define write_info_log		WriteInfoLog
#define write_notice_log	WriteNoticeLog
#define write_warn_log		WriteWarnLog
#define write_error_log		WriteErrorLog
#define write_fatal_log		WriteFatalLog

#define write_level_hex_log	WriteLevelHexLog
#define write_debug_hex_log	WriteDebugHexLog
#define write_info_hex_log	WriteInfoHexLog
#define write_notice_hex_log	WriteNoticeHexLog
#define write_warn_hex_log	WriteWarnHexLog
#define write_error_hex_log	WriteErrorHexLog
#define write_fatal_hex_log	WriteFatalHexLog

#if ( defined _WIN32 ) || ( defined __linux__ ) || ( defined _AIX ) || ( defined __hpux )

_WINDLL_FUNC LOG *CreateLogHandleG();
_WINDLL_FUNC void DestroyLogHandleG();
_WINDLL_FUNC LOG *GetLogHandleG();
_WINDLL_FUNC void GetLogHandlePtrG( LOG **pp_g );
_WINDLL_FUNC void SetLogHandleG( LOG *g );

_WINDLL_FUNC int SetLogOutputG( int output , char *log_pathfilename , funcOpenLog *pfuncOpenLogFirst , funcOpenLog *pfuncOpenLog , funcWriteLog *pfuncWriteLog , funcChangeTest *pfuncChangeTest , funcCloseLog *pfuncCloseLog , funcCloseLog *pfuncCloseLogFinally );
_WINDLL_FUNC int SetLogLevelG( int log_level );
_WINDLL_FUNC int SetLogStylesG( long log_styles , funcLogStyle *pfuncLogStyles );
_WINDLL_FUNC int SetLogStylesExG( long log_styles , funcLogStyle *pfuncLogStyles , funcLogStyle *pfuncLogStylePrefix );
_WINDLL_FUNC int SetLogOutput2V( LOG *g , int output , funcOpenLog *pfuncOpenLogFirst , funcOpenLog *pfuncOpenLog , funcWriteLog *pfuncWriteLog , funcChangeTest *pfuncChangeTest , funcCloseLog *pfuncCloseLog , funcCloseLog *pfuncCloseLogFinally , char *log_pathfilename_format , va_list valist );
_WINDLL_FUNC int SetLogOutput2G( int output , funcOpenLog *pfuncOpenLogFirst , funcOpenLog *pfuncOpenLog , funcWriteLog *pfuncWriteLog , funcChangeTest *pfuncChangeTest , funcCloseLog *pfuncCloseLog , funcCloseLog *pfuncCloseLogFinally , char *log_pathfilename_format , ... );
_WINDLL_FUNC int ReOpenLogOutputG();

/* 写日志函数（基于线程本地存储的缺省日志句柄的函数集合版本） */ /* output log for TLS */
_WINDLL_FUNC int WriteLevelLogG( char *c_filename , long c_fileline , int log_level , char *format , ... );
_WINDLL_FUNC int WriteDebugLogG( char *c_filename , long c_fileline , char *format , ... );
_WINDLL_FUNC int WriteInfoLogG( char *c_filename , long c_fileline , char *format , ... );
_WINDLL_FUNC int WriteNoticeLogG( char *c_filename , long c_fileline , char *format , ... );
_WINDLL_FUNC int WriteWarnLogG( char *c_filename , long c_fileline , char *format , ... );
_WINDLL_FUNC int WriteErrorLogG( char *c_filename , long c_fileline , char *format , ... );
_WINDLL_FUNC int WriteFatalLogG( char *c_filename , long c_fileline , char *format , ... );

/* 写十六进制块日志函数（基于线程本地存储的缺省日志句柄的函数集合版本） */ /* output hex log for TLS */
_WINDLL_FUNC int WriteLevelHexLogG( char *c_filename , long c_fileline , int log_level , char *buffer , long buflen , char *format , ... );
_WINDLL_FUNC int WriteDebugHexLogG( char *c_filename , long c_fileline , char *buffer , long buflen , char *format , ... );
_WINDLL_FUNC int WriteInfoHexLogG( char *c_filename , long c_fileline , char *buffer , long buflen , char *format , ... );
_WINDLL_FUNC int WriteNoticeHexLogG( char *c_filename , long c_fileline , char *buffer , long buflen , char *format , ... );
_WINDLL_FUNC int WriteWarnHexLogG( char *c_filename , long c_fileline , char *buffer , long buflen , char *format , ... );
_WINDLL_FUNC int WriteErrorHexLogG( char *c_filename , long c_fileline , char *buffer , long buflen , char *format , ... );
_WINDLL_FUNC int WriteFatalHexLogG( char *c_filename , long c_fileline , char *buffer , long buflen , char *format , ... );

//#if ( defined __STDC_VERSION__ ) && ( __STDC_VERSION__ >= 199901 )

/* 写日志函数的可变参数宏（基于线程本地存储的缺省日志句柄的函数集合版本） */  /* output log macros for TLS */
#define WRITELOGG( _log_level_ , ... )	WriteLevelLogG( __FILE__ , __LINE__ , _log_level_ , __VA_ARGS__ );
#define DEBUGLOGG( ... )		WriteDebugLogG( __FILE__ , __LINE__ , __VA_ARGS__ );
#define INFOLOGG( ... )			WriteInfoLogG( __FILE__ , __LINE__ , __VA_ARGS__ );
#define NOTICELOGG( ... )		WriteNoticeLogG( __FILE__ , __LINE__ , __VA_ARGS__ );
#define WARNLOGG( ... )			WriteWarnLogG( __FILE__ , __LINE__ , __VA_ARGS__ );
#define ERRORLOGG( ... )		WriteErrorLogG( __FILE__ , __LINE__ , __VA_ARGS__ );
#define FATALLOGG( ... )		WriteFatalLogG( __FILE__ , __LINE__ , __VA_ARGS__ );

/* 写十六进制块日志的可变参数宏（基于线程本地存储的缺省日志句柄的函数集合版本） */ /* output hex log macros for TLS */
#define WRITEHEXLOGG( _log_level_ , _buf_ , _buf_size_ , ... )	WriteLevelHexLogG( __FILE__ , __LINE__ , _log_level_ , _buf_ , _buf_size_ , __VA_ARGS__ );
#define DEBUGHEXLOGG( _buf_ , _buf_size_ , ... )		WriteDebugHexLogG( __FILE__ , __LINE__ , _buf_ , _buf_size_ , __VA_ARGS__ );
#define INFOHEXLOGG( _buf_ , _buf_size_ , ... )			WriteInfoHexLogG( __FILE__ , __LINE__ , _buf_ , _buf_size_ , __VA_ARGS__ );
#define NOTICEHEXLOGG( _buf_ , _buf_size_ , ... )		WriteNoticeHexLogG( __FILE__ , __LINE__ , _buf_ , _buf_size_ , __VA_ARGS__ );
#define WARNHEXLOGG( _buf_ , _buf_size_ , ... )			WriteWarnHexLogG( __FILE__ , __LINE__ , _buf_ , _buf_size_ , __VA_ARGS__ );
#define ERRORHEXLOGG( _buf_ , _buf_size_ , ... )		WriteErrorHexLogG( __FILE__ , __LINE__ , _buf_ , _buf_size_ , __VA_ARGS__ );
#define FATALHEXLOGG( _buf_ , _buf_size_ , ... )		WriteFatalHexLogG( __FILE__ , __LINE__ , _buf_ , _buf_size_ , __VA_ARGS__ );

//#endif

/* 风格替换宏 */
#define create_log_handle_g	CreateLogHandleG
#define destroy_log_handle_g	DestroyLogHandleG

#define set_log_output_g	SetLogOutputG
#define set_log_level_g		SetLogLevelG
#define set_log_styles_g	SetLogStylesG
#define set_log_output2_g	SetLogOutput2G

#define write_level_log_g	WriteLevelLogG
#define write_debug_log_g	WriteDebugLogG
#define write_info_log_g	WriteInfoLogG
#define write_notice_log_g	WriteNoticeLogG
#define write_warn_log_g	WriteWarnLogG
#define write_error_log_g	WriteErrorLogG
#define write_fatal_log_g	WriteFatalLogG

#define write_hex_log_g		WriteHexLogG
#define write_debug_hex_log_g	WriteDebugHexLogG
#define write_info_hex_log_g	WriteInfoHexLogG
#define write_notice_hex_log_g	WriteNoticeHexLogG
#define write_warn_hex_log_g	WriteWarnHexLogG
#define write_error_hex_log_g	WriteErrorHexLogG
#define write_fatal_hex_log_g	WriteFatalHexLogG

#endif

/********************************************************/
/* 以下为高级特性 The following is a advanced features  */
/********************************************************/

/* 自定义检查日志等级回调函数类型 */ /* custom check log level callback function */
typedef int funcFilterLog( LOG *g , void **open_handle , int log_level , char *buf , long len );

#define func_filter_log		funcFilterLog

/* 自定义标签数量 */ /* custom tag amount */
#define LOG_MAXCNT_CUST_LABEL		5
/* 自定义标签最大长度 */ /* custom tag maximum length */
#define LOG_MAXLEN_CUST_LABEL		64

/* 日志选项 */ /* log options */
#define LOG_OPTION_INVALID			-1	/* 无效选项 */
#define LOG_OPTION_OPEN_AND_CLOSE		1	/* 每次都打开日志、写日志、关闭日志 */ /* open , write , close log every time */
#define LOG_OPTION_CHANGE_TEST			2	/* 侦测文件变动 */ /* detect log changed and reopen it */
#define LOG_OPTION_OPEN_ONCE			4	/* 日志打开一次不关闭 */ /* open log once */
#define LOG_OPTION_SET_OUTPUT_BY_FILENAME	8	/* 自动根据文件名重置输出类型 */ /* reset log output type automatically */
							/* "#stdout" -> LOG_OUTPUT_STDOUT */
							/* "#stderr" -> LOG_OUTPUT_STDERR */
							/* "#syslog" -> LOG_OUTPUT_SYSLOG */
#define LOG_OPTION_FILENAME_APPEND_DOT_LOG	16	/* 日志输出文件名后自动加上".log" */ /* append ".log" */

#define LOG_OPTION_OPEN_DEFAULT			LOG_OPTION_CHANGE_TEST

/* 日志自定义标签列最大数量 */
#define LOG_CUST_LABELS_COUNT			30

/* 日志转档模式 */ /* log rotate mode */
#define LOG_ROTATEMODE_INVALID		-1	/* 无效转档模式 */
#define LOG_ROTATEMODE_NONE		0	/* 不转档 */ /* no rotate */
#define LOG_ROTATEMODE_SIZE		1	/* 按日志文件大小转档，和函数SetLogRotateSize配合使用；转档文件名格式"原日志文件名.序号" */ /* according to log file size turn log */
#define LOG_ROTATEMODE_PER_DAY		2	/* 按每天转档；转档文件名格式"原日志文件名.年年年年月月日日" */ /* according to daily turn log */
#define LOG_ROTATEMODE_PER_HOUR		3	/* 按小时转档；转档文件名格式"原日志文件名.年年年年月月日日_小时" */ /* according to hours turn log  */

/* 日志转档缺省值 */ /* log rotate default macros */
#define LOG_ROTATE_SIZE_FILE_COUNT_DEFAULT		99999999
#define LOG_ROTATE_SIZE_PRESSURE_FACTOR_DEFAULT		0
#define LOG_FSYNC_PERIOD				10000

/* 自定义日志转档前后回调函数类型 */ /* custom turn log file callback function */
typedef int funcBeforeRotateFile( LOG *g , char *rotate_log_pathfilename );
typedef int funcAfterRotateFile( LOG *g , char *rotate_log_pathfilename );

#define func_before_rotate_file		funcBeforeRotateFile
#define func_after_rotate_file		funcAfterRotateFile

/* 缓冲区大小缺省值 */ /* default buffer size */
#define LOG_BUFSIZE_DEFAULT		(1024)		/* 缺省行日志缓冲区大小 */
#define LOG_BUFSIZE_MAX			(1024*1024)	/* 最大行日志缓冲区大小 */
#define LOG_HEXLOG_BUFSIZE_DEFAULT	(16*1024)	/* 缺省十六进制块日志缓冲区大小 */
#define LOG_HEXLOG_BUFSIZE_MAX		(10*1024*1024)	/* 最大十六进制块日志缓冲区大小 */

/* 高级句柄环境设置函数 */ /* senior handle environment setting function */
_WINDLL_FUNC int SetLogOptions( LOG *g , int log_options );
_WINDLL_FUNC int SetLogFileChangeTest( LOG *g , long interval );
_WINDLL_FUNC int SetLogFsyncPeriod( LOG *g , long period );
_WINDLL_FUNC int SetLogCustLabel( LOG *g , int index , char *cust_label );
_WINDLL_FUNC int SetLogRotateMode( LOG *g , int rotate_mode );
_WINDLL_FUNC int SetLogRotateSize( LOG *g , long log_rotate_size );
_WINDLL_FUNC int SetLogRotatePressureFactor( LOG *g , long pressure_factor );
_WINDLL_FUNC int SetLogRotateFileCount( LOG *g , long rotate_file_count );
_WINDLL_FUNC int SetBeforeRotateFileFunc( LOG *g , funcAfterRotateFile *pfuncAfterRotateFile );
_WINDLL_FUNC int SetAfterRotateFileFunc( LOG *g , funcAfterRotateFile *pfuncAfterRotateFile );
_WINDLL_FUNC int SetFilterLogFunc( LOG *g , funcFilterLog *pfuncFilterLog );
_WINDLL_FUNC int SetLogBufferSize( LOG *g , long log_bufsize , long max_log_bufsize );
_WINDLL_FUNC int SetHexLogBufferSize( LOG *g , long hexlog_bufsize , long max_log_hexbufsize );
_WINDLL_FUNC int SetLogOutputFuncDirectly( LOG *g , funcOpenLog *pfuncOpenLogFirst , funcOpenLog *pfuncOpenLog , funcWriteLog *pfuncWriteLog , funcChangeTest *pfuncChangeTest , funcCloseLog *pfuncCloseLog , funcCloseLog *pfuncCloseLogFinally );
_WINDLL_FUNC int SetLogStyleFuncDirectly( LOG *g , funcLogStyle *pfuncLogStyle );

/* 风格替换宏 */
#define set_log_options			SetLogOptions
#define set_log_file_change_test	SetLogFileChangeTest
#define set_log_fsync_period		SetLogFsyncPeriod
#define set_log_cust_label		SetLogCustLabel
#define set_log_rotate_mode		SetLogRotateMode
#define set_log_rotate_size		SetLogRotateSize
#define set_log_rotate_pressure_factor	SetLogRotatePressureFactor
#define set_log_rotate_file_count	SetLogRotateFileCount
#define set_before_rotate_file_func	SetBeforeRotateFileFunc
#define set_after_rotate_file_func	SetAfterRotateFileFunc
#define set_filter_log_func		SetFilterLogFunc
#define set_log_buffer_size		SetLogBufferSize
#define set_hex_log_buffer_size		SetHexLogBufferSize
#define set_log_output_func_directly	SetLogOutputFuncDirectly
#define set_log_stlye_func_directly	SetLogStyleFuncDirectly

#if ( defined _WIN32 ) || ( defined __linux__ ) || ( defined _AIX ) || ( defined __hpux )
_WINDLL_FUNC int SetLogOptionsG( int log_options );
_WINDLL_FUNC int SetLogFileChangeTestG( long interval );
_WINDLL_FUNC int SetLogFsyncPeriodG( long period );
_WINDLL_FUNC int SetLogCustLabelG( int index , char *cust_label );
_WINDLL_FUNC int SetLogRotateModeG( int rotate_mode );
_WINDLL_FUNC int SetLogRotateSizeG( long log_rotate_size );
_WINDLL_FUNC int SetLogRotatePressureFactorG( long pressure_factor );
_WINDLL_FUNC int SetLogRotateFileCountG( long rotate_file_count );
_WINDLL_FUNC int SetBeforeRotateFileFuncG( funcAfterRotateFile *pfuncAfterRotateFile );
_WINDLL_FUNC int SetAfterRotateFileFuncG( funcAfterRotateFile *pfuncAfterRotateFile );
_WINDLL_FUNC int SetFilterLogFuncG( funcFilterLog *pfuncFilterLog );
_WINDLL_FUNC int SetLogBufferSizeG( long log_bufsize , long max_log_bufsize );
_WINDLL_FUNC int SetHexLogBufferSizeG( long hexlog_bufsize , long max_log_hexbufsize );
_WINDLL_FUNC int SetLogOutputFuncDirectlyG( funcOpenLog *pfuncOpenLogFirst , funcOpenLog *pfuncOpenLog , funcWriteLog *pfuncWriteLog , funcChangeTest *pfuncChangeTest , funcCloseLog *pfuncCloseLog , funcCloseLog *pfuncCloseLogFinally );
_WINDLL_FUNC int SetLogStyleFuncDirectlyG( funcLogStyle *pfuncLogStyle );
#endif

/* 风格替换宏 */
#define set_log_options_g			SetLogOptionsG
#define set_log_file_change_test_g		SetLogFileChangeTestG
#define set_log_fsync_period_g			SetLogFsyncPeriodG
#define set_log_cust_label_g			SetLogCustLabelG
#define set_log_rotate_mode_g			SetLogRotateModeG
#define set_log_rotate_size_g			SetLogRotateSizeG
#define set_log_rotate_pressure_factor_g	SetLogRotatePressureFactorG
#define set_log_rotate_file_count_g		SetLogRotateFileCountG
#define set_before_rotate_file_func_g		SetBeforeRotateFileFuncG
#define set_after_rotate_file_func_g		SetAfterRotateFileFuncG
#define set_filter_log_func_g			SetFilterLogFuncG
#define set_log_buffer_size_g			SetLogBufferSizeG
#define set_hex_log_buffer_size_g		SetHexLogBufferSizeG
#define set_log_output_func_directly_g		SetLogOutputFuncDirectlyG
#define set_log_stlye_func_directly_g		SetLogStyleFuncDirectlyG

#define TEST_LOGLEVEL_NOTENOUGH(_log_level_,_g_)	( (_log_level_) < (_g_)->log_level )

#define SET_ATTRIBUTE(_param_,_attr_)			( (_param_) = (_attr_) )
#define ADD_ATTRIBUTE(_param_,_attr_)			( (_param_) |= (_attr_) )
#define REMOVE_ATTRIBUTE(_param_,_attr_)		( (_param_) &= ~(_attr_) )
#define TEST_ATTRIBUTE(_param_,_attr_)			( ( (_param_) & (_attr_) ) == (_attr_) )
#define TEST_NO_ATTRIBUTE(_param_,_attr_)		( ( (_param_) & (_attr_) ) != (_attr_) )

#define TEST_STDSTREAM(_fd_)				( (_fd_) != FD_NULL && ( (_fd_) == STDOUT_HANDLE || (_fd_) == STDERR_HANDLE ) )
#define TEST_FILEFD(_fd_)				( (_fd_) != FD_NULL && (_fd_) != STDOUT_HANDLE && (_fd_) != STDERR_HANDLE )

#if ( defined _WIN32 ) || ( defined __linux__ ) || ( defined _AIX ) || ( defined __hpux )
/* 得到基于线程本地存储的缺省日志句柄的函数版本 */ /* log handle get/set function for TLS */
_WINDLL_FUNC LOG *GetGlobalLOG();
_WINDLL_FUNC void SetGlobalLOG( LOG *g );
#define get_global_log	GetGlobalLOG
#define set_global_log	SetGlobalLOG
#endif

/********************************************************/
/* 以下为钩子开发 the following development for the hook*/
/********************************************************/

/* 平台宏 */ /* platfrom macros */
#if ( defined _WIN32 )
#define LOG_NEWLINE		"\r\n"
#define LOG_NEWLINE_LEN		2
#define SNPRINTF		_snprintf
#define VSNPRINTF		_vsnprintf
#define STDOUT_HANDLE		1
#define STDERR_HANDLE		2
#define FD_NULL			-1
#define WRITE			_write
#define STAT			_stat
#define FSTAT			_fstat
#define ACCESS			_access
#define ACCESS_OK		00
#define RENAME			rename

// For _unlink, _wunlink: see MSDN Link: https://msdn.microsoft.com/en-us/library/1c3tczd6.aspx
#define UNLINK			_tunlink // !!! Notice this: For unicode environment, need verification. !!!
#elif ( defined __unix ) || ( defined _AIX ) || ( defined __linux__ ) || ( defined __hpux )
#define LOG_NEWLINE		"\n"
#define LOG_NEWLINE_LEN		1
#define SNPRINTF		snprintf
#define VSNPRINTF		vsnprintf
#define STDOUT_HANDLE		STDOUT_FILENO
#define STDERR_HANDLE		STDERR_FILENO
#define FD_NULL			-1
#define WRITE			write
#define STAT			stat
#define FSTAT			fstat
#define ACCESS			access
#define ACCESS_OK		F_OK
#define RENAME			rename
#define UNLINK			unlink
#endif

/* 代码宏 */ /* code macros */
#if ( defined _WIN32 )
#define SYSTEMTIME2TIMEVAL_USEC(_syst_,_tv_) \
		(_tv_).tv_usec = (_syst_).wMilliseconds * 1000 ;
#define SYSTEMTIME2TM(_syst_,_stime_) \
		(_stime_).tm_year = (_syst_).wYear - 1900 ; \
		(_stime_).tm_mon = (_syst_).wMonth - 1 ; \
		(_stime_).tm_mday = (_syst_).wDay ; \
		(_stime_).tm_hour = (_syst_).wHour ; \
		(_stime_).tm_min = (_syst_).wMinute ; \
		(_stime_).tm_sec = (_syst_).wSecond ;
#define LOCALTIME(_tt_,_stime_) \
	{ \
		SYSTEMTIME	stNow ; \
		GetLocalTime( & stNow ); \
		SYSTEMTIME2TM( stNow , (_stime_) ); \
	}
#elif ( defined __unix ) || ( defined _AIX ) || ( defined __linux__ ) || ( defined __hpux )
#define LOCALTIME(_tt_,_stime_) \
	localtime_r(&(_tt_),&(_stime_));
#endif

#ifndef STRCMP
#define STRCMP(_a_,_C_,_b_) ( strcmp(_a_,_b_) _C_ 0 )
#define STRNCMP(_a_,_C_,_b_,_n_) ( strncmp(_a_,_b_,_n_) _C_ 0 )
#endif

#ifndef STRICMP
#if ( defined _WIN32 )
#define STRICMP(_a_,_C_,_b_) ( stricmp(_a_,_b_) _C_ 0 )
#define STRNICMP(_a_,_C_,_b_,_n_) ( strnicmp(_a_,_b_,_n_) _C_ 0 )
#elif ( defined __unix ) || ( defined _AIX ) || ( defined __linux__ ) || ( defined __hpux )
#define STRICMP(_a_,_C_,_b_) ( strcasecmp(_a_,_b_) _C_ 0 )
#define STRNICMP(_a_,_C_,_b_,_n_) ( strncasecmp(_a_,_b_,_n_) _C_ 0 )
#endif
#endif

#define OFFSET_BUFPTR(_logbuf_,_offset_len_) \
	if( _offset_len_ > 0 && _offset_len_ <= (_logbuf_)->buf_remain_len && (_logbuf_)->bufptr[0] ) \
	{ \
		(_logbuf_)->bufptr += _offset_len_ ; \
		(_logbuf_)->buf_remain_len -= _offset_len_ ; \
	} \

#define OFFSET_BUFPTR_IN_LOOP(_logbuf_,_offset_len_) \
	if( _offset_len_ > 0 && _offset_len_ <= (_logbuf_)->buf_remain_len && (_logbuf_)->bufptr[0] ) \
	{ \
		(_logbuf_)->bufptr += _offset_len_ ; \
		(_logbuf_)->buf_remain_len -= _offset_len_ ; \
	} \
	else \
	{ \
		break; \
	} \

_WINDLL_FUNC char *GetLogPathfilename( LOG *g );
_WINDLL_FUNC int SetOpenFlag( LOG *g , char open_flag );
_WINDLL_FUNC char IsLogOpened( LOG *g );
_WINDLL_FUNC void *GetLogOpenHandle( LOG *g );
_WINDLL_FUNC void SetLogOpenHandle( LOG *g , void *open_handle );

_WINDLL_FUNC int GetLogLevel( LOG *g );

_WINDLL_FUNC LOGBUF *GetLogBuffer( LOG *g );
_WINDLL_FUNC LOGBUF *GetHexLogBuffer( LOG *g );

_WINDLL_FUNC void CleanLogBuffer( LOG *g , LOGBUF *logbuf );
_WINDLL_FUNC long FormatLogBuffer( LOG *g , LOGBUF *logbuf , char *format , ... );
_WINDLL_FUNC long FormatLogBufferV( LOG *g , LOGBUF *logbuf , char *format , va_list valist );
_WINDLL_FUNC long MemcatLogBuffer( LOG *g , LOGBUF *logbuf , char *append , long len );

#define set_open_flag		SetOpenFlag
#define is_log_opened		IsLogOpened

#define get_log_level		GetLogLevel

#define get_log_buffer		GetLogBuffer
#define get_hex_log_buffer	GetHexLogBuffer

#define format_log_buffer	FormatLogBuffer
#define format_log_buffer_v	FormatLogBufferV
#define memcat_log_buffer	MemcatLogBuffer

/* 日志缓冲区结构 */ /* log buffer structure */
struct tagLOGBUF
{
	long			max_buf_size ;
	long			buf_size ;
	char			*bufbase ;
	char			*bufptr ;
	long			buf_remain_len ;
} ;

/* 日志句柄结构 */ /* log handle structure */
struct tagLOG
{
	/* 基本 */ /* basic */
	int			output ;
	char			log_pathfilename[ MAXLEN_FILENAME + 1 ];
	funcOpenLog		*pfuncOpenLogFirst ;
	funcOpenLog		*pfuncOpenLog ;
	funcWriteLog		*pfuncWriteLog ;
	funcChangeTest		*pfuncChangeTest ;
	funcCloseLog		*pfuncCloseLog ;
	funcCloseLog		*pfuncCloseLogFinally ;
	void			*open_handle ;
	void			*test_handle ;
	
	int			fd ;
#if ( defined _WIN32 )
	HANDLE			hFile ;
#endif
	
	char			open_flag ;
	
	/* 日志等级 */ /* log level */
	int			log_level ;
	
	funcFilterLog		*pfuncFilterLog ;
	
	/* 行日志风格 */ /* log style */
	long			log_styles ;
	funcLogStyle		*pfuncLogStyle ;
	/* 分析行格式风格，构建格式函数数组，便于写日志时快速填充行日志缓冲区 */
	funcLogStyle		*pfuncLogStyles[ LOG_CUST_LABELS_COUNT ] ;
	int			style_func_count ;
	
	/* 日志选项 */ /* log options */
	int			log_options ;
	
	long			file_change_test_interval ;
	long			file_change_test_no ;
	struct STAT		file_change_stat ;
	long			fsync_period ;
	long			fsync_elapse ;
	
	/* 自定义标签 */ /* custom labels */
	char			cust_label[LOG_MAXCNT_CUST_LABEL][ LOG_MAXLEN_CUST_LABEL + 1 ] ;
	
	/* 日志转档 */ /* log rotate */
	int			rotate_mode ;
	long			log_rotate_size ;
	long			rotate_file_no ;
	long			rotate_file_count ;
	long			pressure_factor ;
	
	long			skip_count ;
	
	funcAfterRotateFile	*pfuncAfterRotateFile ;
	funcBeforeRotateFile	*pfuncBeforeRotateFile ;
	
	/* 行日志缓冲区 */ /* log buffer */
	LOGBUF			logbuf ;
	
	/* 十六进制日志缓冲区 */ /* hex log buffer */
	LOGBUF			hexlogbuf ;
	
	/* 一级缓存 */ /* level 1 cache */
	struct timeval		cache1_tv ;
	struct tm		cache1_stime ;
	
	/* 二级缓存 */ /* level 2 cache */
	struct timeval		cache2_logstyle_tv ;
	struct tm		cache2_logstyle_stime ;
	char			cache2_logstyle_date_buf[ 10 + 1 ] ;
	long			cache2_logstyle_date_buf_len ;
	char			cache2_logstyle_datetime_buf[ 19 + 1 ] ;
	long			cache2_logstyle_datetime_buf_len ;
	
	unsigned long		cache2_logstyle_pid ;
	char			cache2_logstyle_pid_buf[ 20 + 1 ] ;
	long			cache2_logstyle_pid_buf_len ;
	
	unsigned long		cache2_logstyle_tid ;
	char			cache2_logstyle_tid_buf[ 20 + 1 ] ;
	long			cache2_logstyle_tid_buf_len ;
	
	struct timeval		cache2_rotate_tv ;
	struct tm		cache2_rotate_stime ;
	
	/* 转档文件锁 */ /* rotate file lock */
#if ( defined _WIN32 )
	HANDLE			rotate_lock ;
#elif ( defined __unix ) || ( defined _AIX ) || ( defined __linux__ ) || ( defined __hpux )
	int			rotate_lock ;
	struct flock		lock ;
#endif
} ;

/********************************************************/
/* 以下为辅助接口 the following for the assist interface*/
/********************************************************/

/* 配置辅助函数 */ /* config assist functions */
_WINDLL_FUNC int ConvertLogOutput_atoi( char *output_desc );
_WINDLL_FUNC int ConvertLogLevel_atoi( char *log_level_desc );
_WINDLL_FUNC char *ConvertLogLevel_itoa( int log_level );
_WINDLL_FUNC long ConvertLogStyle_atol( char *line_style_desc );
_WINDLL_FUNC long ConvertLogOption_atol( char *log_option_desc );
_WINDLL_FUNC int ConvertLogRotateMode_atoi( char *rotate_mode_desc );
_WINDLL_FUNC long ConvertBufferSize_atol( char *bufsize_desc );

#define convert_log_output_atoi		ConvertLogOutput_atoi
#define convert_log_level_atoi		ConvertLogLevel_atoi
#define convert_log_level_itoa		ConvertLogLevel_itoa
#define convert_log_style_atol		ConvertLogStyle_atol
#define cnovert_log_option_atol		ConvertLogOption_atol
#define convert_log_rotate_mode_atoi	ConvertLogRotateMode_atoi
#define convert_buffer_size_atol	ConvertBufferSize_atol

/* 公共函数 */
_WINDLL_FUNC int ExpandPathFilename( char *pathfilename , long pathfilename_bufsize );

#ifdef __cplusplus
}
#endif

#endif

