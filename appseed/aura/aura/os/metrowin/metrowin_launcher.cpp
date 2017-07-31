//#include "framework.h"
//#include "metrowin.h"

#if defined(INSTALL_SUBSYSTEM)

bool launcher::start()
{

   if(!ensure_executable())
      return false;

   string strPath(get_executable_path());
   
   string strDir(dir::name(strPath));

   string strParams = get_params();

#ifdef METROWIN

   throw "todo";

#else

   call_async(strPath, strParams, strDir, SW_HIDE);

#endif

   return true;

}

#endif



