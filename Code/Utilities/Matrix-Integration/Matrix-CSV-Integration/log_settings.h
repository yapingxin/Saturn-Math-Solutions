#ifndef _INC_Utility_log_settings_H
#define _INC_Utility_log_settings_H

#include "Common/datatypes.h"
#include "Common/iLOG3/LOGS.h"

// For detailed usage of iLOG3, please refer to the following urls:
// [1] https://www.oschina.net/p/iLOG3
// [2] https://my.oschina.net/u/988092/blog/198377
#define LOG_SETUP   iLOG3_setup
#define LOG_CLEANUP iLOG3_cleanup
#define LOG_DEBUG   DEBUGLOGSG
#define LOG_INFO    INFOLOGSG
#define LOG_WARNING WARNLOGSG
#define LOG_ERROR   ERRORLOGSG
#define LOG_FATAL   FATALLOGSG


// Return value:
//      HResult_OK                        1 | Success;
//      HResult_LOG_SETUP_FAIL   0x001C0000 | Log setup failed;
HResult iLOG3_setup();

void iLOG3_cleanup();

#endif // !_INC_Utility_log_settings_H

