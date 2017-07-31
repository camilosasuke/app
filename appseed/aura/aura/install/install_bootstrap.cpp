#include "framework.h"
#ifdef WINDOWS
#include <shellapi.h>
#include <shlobj.h>
#include "aura/aura/node/windows/windows_registry.h"
#endif
#include <stdio.h>


#ifdef WINDOWSEX


typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

bool get_os_bit(bool & isWindows64bit)
{
#if _WIN64

   isWindows64bit = true;
   return true;

#elif _WIN32

   BOOL isWow64 = FALSE;

   //IsWow64Process is not available on all supported versions of Windows.
   //Use GetModuleHandle to get a handle to the DLL that contains the function
   //and GetProcAddress to get a pointer to the function if available.

   LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

   if (fnIsWow64Process)
   {
      if (!fnIsWow64Process(GetCurrentProcess(), &isWow64))
         return false;

      if (isWow64)
         isWindows64bit = true;
      else
         isWindows64bit = false;

      return true;
   }
   else
      return false;

#else

   assert(0);
   return false;

#endif
}

#else

bool get_os_bit(bool & b64)
{
   
   b64 = true;
   
   return true;
   
}

#endif


#if defined(INSTALL_SUBSYSTEM)


namespace install
{


   bootstrap::bootstrap(::aura::application * papp) :
      ::object(papp),
      ::sockets::socket_handler(papp),
      ::netserver::socket_handler(papp),
      tool(papp)
   {

      m_bForceUpdatedBuild = false;

      m_bInstalling = false;

   }


   bootstrap::~bootstrap()
   {


   }


   int bootstrap::admin_main(string strPlatform)
   {

      ::install::admin_mutex smutex(strPlatform);

      if (smutex.already_exists())
      {

         Application.post_quit();

         return 0;

      }

      Application.trace("--\r\n");
      Application.trace(":::::Installing install, app_app_admin, app_core_user_service and installer\r\n");
      Application.trace("***Installing ca2 Store Installer Application\r\n");
      Application.trace("Registering spa file handler\r\n");
      Application.trace(0.0);

      if (!check_app_app_admin_bin(strPlatform))
      {

         return 0;

      }

      if (strPlatform == "x86")
      {

         Application.register_spa_file_type();

      }

      Application.trace(0.05);

      int iTry = 100;

      while (true)
      {

         try
         {


            if (check_app_app_installation(strPlatform))
            {

               break;

            }


            if (!check_app_app_admin_bin(strPlatform))
            {

               return 0;

            }

         }
         catch (...)
         {

         }

         iTry--;

         if (iTry < 0)
         {

            return 0;

         }

         Sleep(500);

      }

      //Application.trace("***Preparing bootstrap.install\r\n");

      //Application.trace("Starting bootstrap.install\r\n");

      //start_app_install_in_context(strPlatform, true);

      //if (strPlatform == "x86")
      //{

      //   iTry = 8;

      //   while (true)
      //   {

      //      try
      //      {

      //         {

      //            int iUserServiceTry = 500;

      //            while (!check_user_service("Win32", false, Application.m_dwGoodToCheckAgain))
      //            {

      //               while (::get_tick_count() < Application.m_dwGoodToCheckAgain)
      //               {

      //                  Sleep(100);

      //               }

      //               if (iUserServiceTry <= 0)
      //               {

      //                  break;

      //               }

      //               iUserServiceTry--;

      //            }

      //         }

      //         if (!check_app_app_admin_bin(strPlatform))
      //         {

      //            return 0;

      //         }

      //      }
      //      catch (...)
      //      {

      //      }

      //      iTry--;

      //      if (iTry < 0)
      //      {

      //         return 0;

      //      }

      //      Sleep(500);

      //   }

      //}

      Application.trace(":::::Thank you\r\n");
      Application.trace("***Thank you\r\n");
      Application.trace("Thank you\r\n");
      Application.trace(1.0);

      //end_spa(this);

      return 0;

   }


   int bootstrap::install()
   {

      if (m_strPlatform == "x86" || m_strPlatform.is_empty())
      {

         return app_app_x86();

      }
      else
      {

         return app_app_proper();

      }

   }


   int bootstrap::app_app_proper()
   {

      bool bSomeSortOfInstall = false;

      int iTry;

      iTry = 2000;

      string strPlatform = m_strPlatform;

   spa_admin:

      while (!is_file_ok(::path::app_app_admin(strPlatform), ::path::app_app_admin(strPlatform).name(), NULL, strPlatform))
      {

         bSomeSortOfInstall = true;

         if (!is_downloading_admin())
         {

            check_app_app_admin_bin(strPlatform);

         }

         iTry--;

         if (iTry < 0)
         {

            return 0;

         }

         Sleep(250);

      }

      bSomeSortOfInstall = false;

      int iCommandRetry = 0;

   command_retry:

      iTry = 2000;

      while (!check_app_app_installation(strPlatform))
      {

         bSomeSortOfInstall = true;

         if (!is_downloading_admin()
            && is_file_ok(::path::app_app_admin(strPlatform), ::path::app_app_admin(strPlatform).name(), NULL, strPlatform)
            && !Application.low_is_app_app_admin_running(strPlatform))
         {

            if (!is_file_ok(::path::app_app_admin(strPlatform), ::path::app_app_admin(strPlatform).name(), NULL, strPlatform))
            {

               goto spa_admin;

            }

            Application.defer_start_program_files_app_app_admin(strPlatform);

         }

         iTry--;

         if (iTry < 0)
         {

            return 0;

         }

         Sleep(250);

      }

      int iUserServiceTry = 2000;

      while (!check_user_service("Win32", true, Application.m_dwGoodToCheckAgain))
      {

         while (::get_tick_count() < Application.m_dwGoodToCheckAgain)
         {

            Sleep(100);

         }

         if (iUserServiceTry <= 0)
         {

            break;

         }

         iUserServiceTry--;

      }

      string strId;

      while (true)
      {

         string str = Application.pick_command_line();

         if (str.is_empty())
         {

            break;

         }

         m_straCommand.add(str);

         if (Application.check_soon_launch(str, false, Application.m_dwGoodToCheckAgain))
         {

            continue;

         }

         string strParams;

         int iFind1 = 0;

         if (str[0] == '\"')
         {

            iFind1 = str.find('\"', 1);

         }

         int iFind = str.find(" : ", iFind1 + 1);

         if (iFind >= 0)
         {
            strParams = str.substr(iFind);
            iFind = str.find("app=", iFind);
            if (iFind >= 0)
            {
               int iEnd = str.find(" ", iFind);
               if (iEnd < 0)
               {
                  strId = str.substr(iFind + 4);
               }
               else
               {
                  strId = str.substr(iFind + 4, iEnd - iFind - 4);
               }

               // trim initial quote
               if (strId[0] == '\"')
                  strId = strId.substr(1);

               // trim final quote
               if (strId[strId.length() - 1] == '\"')
                  strId = strId.substr(0, strId.length() - 1);

            }

         }
         else
         {

            strId = Application.get_app_id(Application.directrix()->m_varTopicFile);

            if (strId.length() <= 0)
            {

               continue;

            }

         }


         int iTry = 2000;

         while (iTry > 0)
         {

            do_app_app(strId, strParams);

            if (Application.is_application_updated(strId, Application.m_dwGoodToCheckAgain))
            {

               break;

            }

            while (::get_tick_count() < Application.m_dwGoodToCheckAgain)
            {

               Sleep(100);

            }

            iTry--;

         }

      }

      {

         int iCommandFailCount = 0;

         for (auto & strCommand : m_straCommand)
         {

            if (!Application.check_soon_launch(strCommand, true, Application.m_dwGoodToCheckAgain))
            {

               iCommandFailCount++;

            }

         }

         if (iCommandFailCount > 0)
         {

            if (iCommandRetry < 50)
            {

               iCommandRetry++;

               goto command_retry;

            }

         }

      }

      iUserServiceTry = 2000;

      while (!check_user_service("Win32", true, Application.m_dwGoodToCheckAgain))
      {

         while (::get_tick_count() < Application.m_dwGoodToCheckAgain)
         {

            Sleep(100);

         }

         if (iUserServiceTry <= 0)
         {

            break;

         }

         iUserServiceTry--;

      }


      return 1;

   }


   int bootstrap::app_app_x86()
   {

      int iTry;

      iTry = 2000;

      string strPlatform = m_strPlatform;

   spa_admin:

      while (iTry > 0)
      {

         try
         {

            if (check_app_app_admin_bin("x86"))
            {

               break;

            }

         }
         catch (...)
         {


         }

         iTry--;

         if (iTry < 0)
         {

            return 0;

         }

         Sleep(250);

      }

      ::file::path pathSpaAdmin = ::path::app_app_admin("x86");

      ::file::path pathSpaAdminName = pathSpaAdmin.name();

      int iCommandRetry = 0;

   command_retry:

      while (true)
      {

         try
         {

            if (check_app_app_installation("x86") && check_app_app_installation("x64"))
            {

               break;

            }

            if (!is_file_ok(pathSpaAdmin, pathSpaAdminName, NULL, "x86"))
            {

               goto spa_admin;

            }
            else if (!is_downloading_admin() && !Application.low_is_app_app_admin_running("x86"))
            {

               Application.defer_start_program_files_app_app_admin("x86");

               Sleep(250);

            }

         }
         catch (...)
         {

         }

         iTry--;

         if (iTry < 0)
         {

            return 0;

         }

         Sleep(250);

      }

      int iUserServiceTry = 2000;

      while (!check_user_service("Win32", true, Application.m_dwGoodToCheckAgain))
      {

         while (::get_tick_count() < Application.m_dwGoodToCheckAgain)
         {

            Sleep(100);

         }

         if (iUserServiceTry <= 0)
         {

            break;

         }

         iUserServiceTry--;

      }


      while (true)
      {

         string strId;

         string wstr(Application.pick_command_line());

         if (wstr.is_empty())
         {

            break;

         }

         m_straCommand.add(wstr);

         if (Application.check_soon_launch(wstr, false, Application.m_dwGoodToCheckAgain))
         {

            continue;

         }

         string strParams;

         int iFind1 = 0;

         if (wstr[0] == '\"')
         {

            iFind1 = wstr.find('\"', 1);

         }

         int iFind = wstr.find(" : ", iFind1 + 1);

         if (iFind >= 0)
         {
            strParams = wstr.substr(iFind);
            iFind = wstr.find("app=", iFind);
            if (iFind >= 0)
            {
               int iEnd = wstr.find(" ", iFind);
               if (iEnd < 0)
               {
                  strId = wstr.substr(iFind + 4);
               }
               else
               {
                  strId = wstr.substr(iFind + 4, iEnd - iFind - 4);
               }

               // trim initial quote
               if (strId[0] == '\"')
                  strId = strId.substr(1);

               // trim final quote
               if (strId[strId.length() - 1] == '\"')
                  strId = strId.substr(0, strId.length() - 1);

            }
         }
         else
         {

            strId = Application.get_app_id(Application.directrix()->m_varTopicFile);

            if (strId.length() <= 0)
            {

               continue;

            }

         }

         int iTry = 2000;

         while (iTry > 0)
         {

            do_app_app(strId, strParams);

            if (Application.is_application_updated(strId, Application.m_dwGoodToCheckAgain))
            {

               break;

            }

            while (::get_tick_count() < Application.m_dwGoodToCheckAgain)
            {

               Sleep(100);

            }

            iTry--;

         }

      }

      {

         int iCommandFailCount = 0;

         for (auto & strCommand : m_straCommand)
         {

            if (!Application.check_soon_launch(strCommand, true, Application.m_dwGoodToCheckAgain))
            {

               iCommandFailCount++;

            }

         }

         if (iCommandFailCount > 0)
         {

            if (iCommandRetry < 50)
            {

               iCommandRetry++;

               goto command_retry;

            }

         }

      }

      iUserServiceTry = 2000;

      while (!check_user_service("Win32", true, Application.m_dwGoodToCheckAgain))
      {

         while (::get_tick_count() < Application.m_dwGoodToCheckAgain)
         {

            Sleep(100);

         }

         if (iUserServiceTry <= 0)
         {

            break;

         }

         iUserServiceTry--;

      }

      return 1;

   }

   //
   // do_spa
   // installs the application synchronously
   // it just installs the application, it doesn't launch it 
   int bootstrap::do_app_app(const char * pszId, const char * pszParams)
   {

      string strId(pszId);

      string strParams(pszParams);

      string strCommandLine;

      if (strParams.length() > 0)
      {

         strCommandLine = strParams;

      }
      else
      {

         strCommandLine = " : app=" + strId;

         strCommandLine += " install";

      }

      strCommandLine += " ";

      string strCommand;

      strCommand = "appinstall:";

      strCommand += strCommandLine;

      string strPlatform;

      if (strId == "app-core/user_service")
      {

         strPlatform = "x86";

      }
      else
      {

         strPlatform = process_platform_dir_name2();

      }

      app_install_call_sync(strPlatform, strCommand.c_str(), "");

      return true;

   }





   bool bootstrap::check_app_app_installation(string strPlatform)
   {

      ::install::install_status & status = m_mapInstall[strPlatform];

      if (get_admin())
      {

         Application.trace("Downloading\r\n");

      }

      if (status.m_lProcessing > 0)
      {

         return false;

      }

      stringa straFile;

      straFile.add("app_app_admin");
      straFile.add("install");
#ifdef WINDOWS
      straFile.add("vcredist");
#endif
      
      
      status.m_lTotal = straFile.size();
      status.m_lProcessing = status.m_lTotal;
      status.m_lOk = 0;
      status.m_lBad = 0;


      for (int iFile = 0; iFile < straFile.size(); iFile++)
      {

         new install_item(this, straFile[iFile], strPlatform, &status);

      }

      int iRetry = 0;

      while (iRetry < 360 && status.m_lProcessing > 0 && ::get_thread_run() && (status.m_lBad <= 0 || get_admin()))
      {

         Sleep(500);

         iRetry++;

      }

      Sleep(100);

      if (status.m_lProcessing > 0 || status.m_lBad > 0)
      {

         return false;

      }

      return true;

   }


   int bootstrap::check_user_service(string strPlatform, bool bLaunch, DWORD & dwGoodToCheckAgain)
   {

      if (Application.is_application_updated("app-core/user_service", dwGoodToCheckAgain))
      {

         if (is_user_service_running())
         {

            return TRUE;

         }

         if (!bLaunch || get_admin())
         {

            return 1;

         }

         string strApplication = dir::stage(strPlatform) / "app_core_user_service.exe";

#ifdef WINDOWSEX
         SHELLEXECUTEINFOW sei = {};

         wstring wstrFile(strApplication);

         sei.cbSize = sizeof(SHELLEXECUTEINFOW);

         sei.lpFile = wstrFile.c_str();

         if (::ShellExecuteExW(&sei))
         {

            // don't rely on this result to check if user service is running

            // going to rely on is_user_service_running() function

            return FALSE;

         }
#else
   
         throw todo(get_app());
         
#endif

      }
      

      if (!do_app_app("app-core/user_service"))
      {

         return FALSE;

      }

      // don't rely on this result to check if user service is running

      // going to rely on is_user_service_running() function

      return FALSE;

   }

#ifdef WINDOWSEX
   
   bool bootstrap::is_vcredist_installed(string strPlatform)
   {

      bool b64 = false;

      bool bInstalled = false;

      get_os_bit(b64);

      string strKey;

      if (strPlatform == "x64")
      {

         strKey = "SOFTWARE\\Microsoft\\VisualStudio\\14.0\\VC\\Runtimes\\x64";

      }
      else
      {
        
         if (b64)
         {

            strKey = "SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\14.0\\VC\\Runtimes\\x86";

         }
         else
         {

            strKey = "SOFTWARE\\Microsoft\\VisualStudio\\14.0\\VC\\Runtimes\\x86";

         }

      }
      
      
      ::windows::registry::Key key(HKEY_LOCAL_MACHINE, strKey, false);

      DWORD dwInstalled;

      if (!key.QueryValue("Installed", dwInstalled))
      {

         return false;

      }

      return dwInstalled != false;

   }


   int bootstrap::check_vcredist(string strPlatform)
   {

      if (is_vcredist_installed(strPlatform))
      {

         return 1;

      }

      string str = ::path::vcredist(strPlatform);

      if (!file_exists_dup(str) || !file_exists_dup(str + ".installed.txt"))
      {

         if (!get_admin())
         {

            return 0;

         }

         if (!download_vcredist(strPlatform))
         {

            return 0;

         }

         if (!file_exists_dup(str))
         {

            return 0;

         }

         run_vcredist(strPlatform);

      }

      return 1;

   }


   int bootstrap::download_vcredist(string strPlatform)
   {

      string strTempSpa = download_tmp_vcredist(strPlatform);

      if (!file_exists_dup(strTempSpa.c_str()))
      {

         return 0;

      }

      if (get_admin())
      {

         string str = ::path::vcredist(strPlatform);

         if (!::CopyFileW(u16(strTempSpa.c_str()).c_str(), u16(str), FALSE))
         {

            return 0;

         }

         if (!file_exists_dup(str))
         {

            return 0;

         }

      }
      else
      {

         return 0;

      }

      return 1;

   }


   string bootstrap::download_tmp_vcredist(string strPlatform)
   {

      string strTempSpa = get_temp_file_name_dup(::path::vcredist(strPlatform).title(), ::path::vcredist(strPlatform).extension());

      int iTry = 0;

      bool bOk;

      while (iTry <= 3)
      {

         bOk = Application.http_download("https://server.ca2.cc/" + strPlatform + "/" + ::path::vcredist(strPlatform).name(), strTempSpa);

         if (bOk)
         {

            bOk = file_exists_dup(strTempSpa);

            if (bOk)
            {

               bOk = file_length_dup(strTempSpa) > 0;

               if (bOk)
               {

                  return strTempSpa;

               }

            }

         }

         iTry++;

      }

      return "";

   }
   
#endif


   int bootstrap::check_app_app_bin(string strPlatform)
   {

      string str = ::path::app_app(strPlatform);

      if (!is_file_ok(::path::app_app(strPlatform), ::path::app_app(strPlatform).name(), NULL, strPlatform))
      {

         if (!get_admin())
         {

            return 0;

         }

         if (!download_app_app_bin(strPlatform))
         {

            return 0;

         }

         if (!is_file_ok(::path::app_app(strPlatform), ::path::app_app(strPlatform).name(), NULL, strPlatform))
         {

            return 0;

         }

      }

      return 1;

   }


   int bootstrap::check_app_app_admin_bin(string strPlatform, bool bStartNok)
   {

      ::file::path path = ::path::app_app_admin(strPlatform);

      if (!bStartNok && is_file_ok(path, path.name(), NULL, strPlatform))
      {

         return true;

      }

      if (is_downloading_admin())
      {

         return false;

      }

      keep < bool > keepDownloadingSpaadmin(&m_bDownloadingAdmin, true, false, true);

      if (!download_app_app_admin_bin(strPlatform))
      {

         return false;

      }

      if (!is_file_ok(path, path.name(), NULL, strPlatform))
      {

         return false;

      }

      return true;

   }


   int bootstrap::download_app_app_bin(string strPlatform)
   {

      string strTempSpa = download_tmp_app_app_bin(strPlatform);

      if (!is_file_ok(strTempSpa, ::path::app_app(strPlatform).name(), NULL, strPlatform))
      {

         return 0;

      }

      if (get_admin())
      {

         string str = ::path::app_app(strPlatform);

         if (!Application.file().copy(str, strTempSpa, false))
         {

            return 0;

         }

         if (!is_file_ok(str, ::path::app_app(strPlatform).name(), NULL, strPlatform))
         {

            return 0;

         }

      }
      else
      {

         return 0;

      }


      return 1;



   }

   int bootstrap::download_app_app_admin_bin(string strPlatform)
   {

      string strTempSpa = download_tmp_app_app_admin_bin(strPlatform);

      if (!is_file_ok(strTempSpa, ::path::app_app_admin(strPlatform).name(), NULL, strPlatform))
      {

         return 0;

      }

      if (get_admin())
      {

         string str = ::path::app_app_admin(strPlatform);

         if (!dir::mk(dir::name(str)))
         {

            return 0;

         }

         if (!Application.file().copy(str, strTempSpa, false))
         {

            return 0;

         }

         if (!is_file_ok(str, ::path::app_app_admin(strPlatform).name(), NULL, strPlatform))
         {

            return 0;

         }
         
#ifdef WINDOWSEX

         SHELLEXECUTEINFOW sei = {};

         wstring wstrFile(str);

         {

            ::install::admin_mutex mutexStartup("-startup");

            sei.cbSize = sizeof(SHELLEXECUTEINFOW);

            sei.lpFile = wstrFile.c_str();

            ::ShellExecuteExW(&sei);

         }

      }
      else
      {

         if (strPlatform == "x86" && !Application.low_is_app_app_admin_running(strPlatform))
         {

            SHELLEXECUTEINFOW sei = {};

            wstring wstrFile = u16(strTempSpa.c_str());

            {

               ::install::admin_mutex mutexStartup("-startup");

               sei.cbSize = sizeof(SHELLEXECUTEINFOW);

               sei.fMask = SEE_MASK_NOASYNC | SEE_MASK_NOCLOSEPROCESS;

               sei.lpVerb = L"RunAs";

               sei.lpFile = wstrFile.c_str();

               ::ShellExecuteExW(&sei);

            }

            DWORD dwGetLastError = GetLastError();

            string str = ::path::app_app_admin(strPlatform);

            DWORD dwExitCode = 0;

            for (int i = 0; i < 1440; i++)
            {

               if (::GetExitCodeProcess(sei.hProcess, &dwExitCode))
               {

                  if (dwExitCode != STILL_ACTIVE && !Application.low_is_app_app_admin_running(strPlatform))
                  {

                     break;

                  }

               }
               else
               {

                  break;

               }

               if (is_file_ok(str, ::path::app_app_admin(strPlatform).name(), NULL, strPlatform))
                  break;

               ::WaitForSingleObject(sei.hProcess, 500);

            }

            ::CloseHandle(sei.hProcess);

         }

#endif

      }


      return 1;
   }



   string bootstrap::download_tmp_app_app_admin_bin(string strPlatform)
   {

      string strTempSpa = get_temp_file_name_dup(::path::app_app_admin(strPlatform).title(), ::path::app_app_admin(strPlatform).extension());

      string strUrl;

      strUrl = "https://server.ca2.cc/" + strPlatform + "/" + System.get_system_configuration() + "/" + ::path::app_app_admin(strPlatform).name();

      int iTry = 0;

      bool bOk;

      while (iTry <= 3)
      {

         bOk = Application.http_download(strUrl, strTempSpa);

         if (bOk)
         {

            bOk = is_file_ok(strTempSpa, ::path::app_app_admin(strPlatform).name(), NULL, strPlatform);

            if (bOk)
            {

               bOk = file_length_dup(strTempSpa) > 0;

               if (bOk)
               {

                  return strTempSpa;

               }

            }

         }

         iTry++;

      }


      return "";

   }



   string bootstrap::download_tmp_app_app_bin(string strPlatform)
   {

      string strTempSpa = get_temp_file_name_dup(::path::app_app(strPlatform).title() + "-" + strPlatform, ::path::app_app(strPlatform).extension());

      int iTry = 0;

      bool bOk;

      while (iTry <= 3)
      {

         bOk = Application.http_download("https://server.ca2.cc/" + strPlatform + "/" + System.get_system_configuration() + "/" + ::path::app_app(strPlatform).name(), strTempSpa);

         if (bOk)
         {

            bOk = is_file_ok(strTempSpa, ::path::app_app(strPlatform).name(), NULL, strPlatform);

            if (bOk)
            {

               bOk = file_length_dup(strTempSpa) > 0;

               if (bOk)
               {

                  return strTempSpa;

               }

            }

         }

         iTry++;

      }


      return "";

   }





   bool bootstrap::is_user_service_running()
   {

      ::mutex mutex(get_thread_app(), false, "Local\\ca2_application_local_mutex:app-core/user_service");

      return mutex.already_exists();

   }









   //string bootstrap::get_version(string strVersion)
   //{
   //   static string  s_strVersion;

   //   if (strVersion.has_char())
   //   {
   //      s_strVersion = strVersion;
   //   }

   //   if (s_strVersion.is_empty())
   //   {
   //      if (_ca_is_basis())
   //      {
   //         s_strVersion = "basis";
   //      }
   //      else
   //      {
   //         s_strVersion = "stage";
   //      }
   //   }

   //   return s_strVersion;
   //}


   string bootstrap::get_title(string strTitle)
   {
      static string  s_strTitle;

      if (strTitle.has_char())
      {
         s_strTitle = strTitle;
      }

      return s_strTitle;
   }




   bool bootstrap::is_downloading_admin()
   {

      return m_bDownloadingAdmin;

   }





} // namespace install



#endif




