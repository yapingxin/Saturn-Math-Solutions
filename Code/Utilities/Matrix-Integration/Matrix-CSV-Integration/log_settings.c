#include "log_settings.h"

#define LOG_STYLES_FILE     ( LOG_STYLE_DATETIMEMS | LOG_STYLE_LOGLEVEL | LOG_STYLE_PID | LOG_STYLE_TID | LOG_STYLE_SOURCE | LOG_STYLE_FORMAT | LOG_STYLE_NEWLINE )
#define LOG_STYLES_STDOUT   ( LOG_STYLE_DATETIMEMS | LOG_STYLE_CUSTLABEL1 | LOG_STYLE_FORMAT | LOG_STYLE_NEWLINE )



// Return value:
//      HResult_OK                        1 | Success;
//      HResult_LOG_SETUP_FAIL   0x001C0000 | Log setup failed;
HResult iLOG3_setup()
{
    HResult rc = HResult_OK;
    LOGS* gs = NULL;
    LOG* g = NULL;

    gs = CreateLogsHandleG();
    if (gs == NULL)
    {
        rc = HResult_LOG_SETUP_FAIL;
        goto EXIT;
    }

    g = CreateLogHandle();
    if (g == NULL)
    {
        rc = HResult_LOG_SETUP_FAIL;
        goto EXIT;
    }

    SetLogOutput(g, LOG_OUTPUT_FILE, "Matrix-CSV-Integration_Main.log", LOG_NO_OUTPUTFUNC);
    SetLogLevel(g, LOG_LEVEL_DEBUG);
    SetLogStyles(g, LOG_STYLES_FILE, LOG_NO_STYLEFUNC);
    AddLogToLogs(gs, "file", g);

    g = CreateLogHandle();
    if (g == NULL)
    {
        rc = HResult_LOG_SETUP_FAIL;
        goto EXIT;
    }

    SetLogOutput(g, LOG_OUTPUT_STDOUT, NULL, LOG_NO_OUTPUTFUNC);
    SetLogLevel(g, LOG_LEVEL_DEBUG);
    SetLogCustLabel(g, 1, "StdLog");
    SetLogStyles(g, LOG_STYLES_STDOUT, LOG_NO_STYLEFUNC);
    AddLogToLogs(gs, "stdout", g);

EXIT:
    return rc;
}

void iLOG3_cleanup()
{
    DestroyLogsHandleG();
}
