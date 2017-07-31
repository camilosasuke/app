#include "framework.h"


#if defined(INSTALL_SUBSYSTEM)


namespace install
{


   launcher::launcher(const char * pszPlatform, const char * pszVersion, const char * pszBuild)
   {

      m_strVersion = pszVersion;

      m_strBuild = pszBuild;

      m_strPlatform = pszPlatform;

      m_bPrivileged = true;

   }


   bool launcher::ensure_executable()
   {

      m_strPath = path::app_app_admin(m_strPlatform);

      return true;

   }

   string launcher::get_executable_path()
   {

      ensure_executable();

      return m_strPath;

   }


   void launcher::start_in_context(bool bAlreadyElevated)
   {

      if (!ensure_executable())
         return;
      
      
      call_async(m_strPath, NULL, ::file::path(m_strPath).folder(), SW_HIDE, false);

//      string strDir = dir::name(m_strPath.c_str());
//
//      wstring wstrDir = u16(strDir.c_str());
//
//      wstring wstr = u16(m_strPath.c_str());
//
//      STARTUPINFOW si;
//      memset(&si, 0, sizeof(si));
//      si.cb = sizeof(si);
//      si.dwFlags = 0;
//      si.wShowWindow = SW_HIDE;
//      PROCESS_INFORMATION pi;
//      memset(&pi, 0, sizeof(pi));
//
//      if (!::CreateProcessW(NULL, (wchar_t *)wstr.c_str(), NULL, NULL, FALSE, 0, NULL, wstrDir.c_str(), &si, &pi))
//      {
//
//         output_debug_string("Could not create process \"" + m_strPath + "\"");
//
//      }

      //Sleep(2000);

   }



} // namespace install



#endif







