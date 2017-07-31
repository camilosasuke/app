#include "framework.h"
//#ifdef APPLEOS
////#include <iconv.h>
//#else
//#endif

#ifndef METROWIN

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "iconv.h"

#endif

//O volume na unidade C n∆o tem nome.
 //O N£mero de SÇrie do Volume Ç 72AC-5E14

 //Pasta de C:\nordroid\app\appseed\aura\aura\os\cross\windows

#include "cross_windows_threading.cpp"
#include "cross_windows_user.cpp"
#include "cross_windows_datetime.cpp"

#ifndef METROWIN
#include "cross_windows_crt.cpp"
#include "cross_windows_crt_debug.cpp"
#include "cross_windows_error.cpp"
#include "cross_windows_locale.cpp"
#include "cross_windows_nt.cpp"
#include "cross_windows_shared_memory.cpp"
#include "cross_windows_windows.cpp"
#endif


//2015-01-05  09:49                 0 _cross_windows.cpp"
//               9 arquivo(s)        191.858 bytes
//               0 pasta(s)   19.172.888.576 bytes dispon°veis
