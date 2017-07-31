#pragma once


#include "aura/os/os_binreloc.h"


#if defined(MACOS)


#include "aura/os/macos/os.h"


#elif defined(APPLE_IOS)


#include "aura/os/ios/os.h"


#elif defined(LINUX)


#include "aura/os/linux/linux_linux.h"


#define BSD_STYLE_SOCKETS
#define HAVE_MYSQL
#define HAVE_OPENSSL


#elif defined(METROWIN)


#include "aura/os/metrowin/metrowin_metrowin.h"


#define BYESHYTOULA_STYLE_SOCKS


#elif defined(WINDOWSEX)


#include "aura/os/windows/windows_windows.h"


#define BSD_STYLE_SOCKETS
#define HAVE_MYSQL
#define HAVE_OPENSSL


#elif defined(ANDROID)


#include "aura/os/android/android_android.h"

#define BSD_STYLE_SOCKETS
#define HAVE_MYSQL
#define HAVE_OPENSSL


#elif defined(SOLARIS)


#include "aura/os/solaris/solaris_solaris.h"


#define BSD_STYLE_SOCKETS
#define HAVE_MYSQL
#define HAVE_OPENSSL


#elif defined(APPLE_IOS)


#include "os/ansios/ansios.h"
#include "aura/os/ios/ca_os.h"


#else


#error Not supported operating system


#endif


#ifdef AMD64
#define OS64BIT
#elif defined(__LP64)
#define OS64BIT
#else
#define OS32BIT
#endif









