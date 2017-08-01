#include "framework.h"


#include "cross/_cross.cpp"


#include "os_file.cpp"
#include "os_http.cpp"
#include "os_process.cpp"
#include "os_thread.cpp"

#ifdef WINDOWSEX
#include "ansios/ansios_file_std.cpp"
//#include "cross/ansios/ansios_datetime.cpp"
//#include "cross/metrowin/cross_metrowin_user.cpp"
#include "windows_common/_windows_common.cpp"
#include "windows/_windows_os.cpp"
#elif defined(METROWIN)
#include "ansios/ansios_message_loop.cpp"
//#include "cross/ansios/ansios_datetime.cpp"
//#include "cross/metrowin/cross_metrowin_user.cpp"
//#include "cross/windows/windows_internals.h"
//#include "cross/windows/cross_windows_datetime.cpp"
//#include "cross/windows/cross_windows_threading.cpp"
//#include "cross/windows/cross_windows_user.cpp"
#include "metrowin/_metrowin_os.cpp"
#include "windows_common/_windows_common.cpp"
#include "windows/windows_ansi_file.cpp"
#elif defined(ANDROID)
//#include "cross/windows/_cross_windows.cpp"
#include "android/_android_os.cpp"
#include "ansios/_ansios_os.cpp"
#elif defined(LINUX)
//#include "cross/windows/_cross_windows.cpp"
#include "linux/_linux_os.cpp"
#include "ansios/_ansios_os.cpp"
#include "x11/x11_windowing.cpp"
#elif defined(MACOS)
//#include "cross/windows/_cross_windows.cpp"
#include "macos/_macos_os.cpp"
#include "ansios/_ansios_os.cpp"
#elif defined(APPLE_IOS)
//#include "cross/windows/_cross_windows.cpp"
#include "ios/_ios_os.cpp"
#include "ansios/_ansios_os.cpp"
#endif

#ifdef ENABLE_BINRELOC
#include <sys/stat.h>
#include <unistd.h>
#endif /* ENABLE_BINRELOC */

#ifndef METROWIN
#include "os_binreloc.cpp"
#endif


#include "os_alloc.cpp"


