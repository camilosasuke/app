#include "framework.h"
#include "base/user/user/user.h"
#include "base/base/node/metrowin/metrowin.h"


#undef CoreApplication


CLASS_DECL_CORE int app_core_main(const char * pszCmdLine)
{

   if(!defer_core_init())
      return -1;

   if(file_exists_dup(::dir::system() / "config\\system\\beg_debug_box.txt"))
   {
      debug_box("zzzAPPzzz app","zzzAPPzzz app",MB_ICONINFORMATION);
   }

   ::core::system * psystem                     = new ::core::system;

   auto source = ::metrowin::new_directx_application_source(psystem,pszCmdLine);

   ::Windows::ApplicationModel::Core::CoreApplication::Run(source);
   
   try
   {

      delete psystem;

   }
   catch(...)
   {

   }

   psystem = NULL;


   defer_core_term();

   char szTimeMessage[2048];

   sprintf(szTimeMessage,"\n\n\n\n\n\n-------------------------------\n|\n|\n|  Total Elapsed Time %d\n|\n|\n-------------------------------\n\n\n",(uint32_t) ::get_tick_count() - g_dwStartTime);

   ::OutputDebugStringA(szTimeMessage);

   printf(szTimeMessage);

   return 0;




}