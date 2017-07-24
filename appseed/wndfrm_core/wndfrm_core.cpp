#include "framework.h"

#ifdef WINDOWS

#ifdef METROWIN
[MTAThread]
#endif
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{


   UNREFERENCED_PARAMETER(hInstance);
   UNREFERENCED_PARAMETER(lpReserved);


   if (dwReason == DLL_PROCESS_ATTACH)
   {


      ::OutputDebugStringW(L"wndfrm_core.dll initializing!\n");


   }
   else if (dwReason == DLL_PROCESS_DETACH)
   {


      ::OutputDebugStringW(L"wndfrm_core.dll terminating!\n");


   }

   return 1;   // ok

}


#endif




