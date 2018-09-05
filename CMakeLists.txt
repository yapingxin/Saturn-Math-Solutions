CMAKE_MINIMUM_REQUIRED (VERSION 3.8)
PROJECT (Saturn-Math-Solutions C)

MESSAGE (STATUS "Solution: " ${CMAKE_PROJECT_NAME})
MESSAGE (STATUS "Dev-Root BINARY dir " ${PROJECT_BINARY_DIR})
MESSAGE (STATUS "Dev-Root SOURCE dir " ${PROJECT_SOURCE_DIR})

# Check Operating System type
MESSAGE(STATUS "CMAKE_SYSTEM_NAME : ${CMAKE_SYSTEM_NAME}")
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    MESSAGE(STATUS "Detecting OS platform: Linux")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
    ADD_DEFINITIONS( -D _LINUX )
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE(STATUS "Detecting OS platform: Windows")
    # suggestion command: cmake -G "Visual Studio 14 2015 Win64" ..

    # Add Preprocessor macro definitions:
    # _WIN32
    # _UNICODE
    # HAVE_VLD
    # _CRT_SECURE_NO_WARNINGS
    ADD_DEFINITIONS( -D _WIN32 )
    ADD_DEFINITIONS( -D _UNICODE )
    ADD_DEFINITIONS( -D HAVE_VLD )
    ADD_DEFINITIONS( -D _CRT_SECURE_NO_WARNINGS )

    # Additional Include Directories for Windows platform only
    INCLUDE_DIRECTORIES(
        Code/Platform/Win32/Utilities/Common
    )

ELSE ()
    MESSAGE(STATUS "Detecting OS platform : (other) : ${CMAKE_SYSTEM_NAME}")
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")

# For Windows platform: set Platform and Configuration
IF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE(STATUS "[INFO] For Windows: Set Visual C++ Default Platform and Configuration as x64 and Debug")
    SET ( VC_Platform "x64" )
    SET ( VC_Configuration "Debug" )
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Windows")

INCLUDE_DIRECTORIES(
    Code/Platform
    Code
)



# Project: Matrix-CSV-Integration

MESSAGE(STATUS "")
MESSAGE(STATUS "======================================================")
MESSAGE(STATUS "| Target Matrix-CSV-Integration (EXECUTABLE) BEGIN   |")
MESSAGE(STATUS "======================================================")
MESSAGE(STATUS "")

# Project Matrix-CSV-Integration source code list - cross platform
SET ( SRC_LIST_Matrix_CSV_Integration

    Code/Utilities/Matrix-Integration/Matrix-CSV-Integration/Matrix-CSV-Integration.c
    Code/Common/DS/vector.c
    Code/Common/iLOG3/LOG.c
    Code/Common/iLOG3/LOGC.c
    Code/Common/iLOG3/LOGS.c
    Code/Common/common_functions.c
    Code/Common/FileOperation.c
)

# Project PHM-Hnode-Std-Server source code list - Win32 OS platform
IF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    SET ( SRC_LIST_Matrix_CSV_Integration

        Code/Platform/Win32/Utilities/Common/stdafx.c 
        Code/Platform/Win32/Common/dirent_Win32.c
        Code/Platform/Win32/Common/iconv_Win32.c
    
        ${SRC_LIST_Matrix_CSV_Integration}
    )
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Windows")


ADD_EXECUTABLE( Matrix-CSV-Integration ${SRC_LIST_Matrix_CSV_Integration} )

MESSAGE(STATUS "")
MESSAGE(STATUS "======================================================")
MESSAGE(STATUS "| Target Matrix-CSV-Integration (EXECUTABLE) END     |")
MESSAGE(STATUS "======================================================")
MESSAGE(STATUS "")
