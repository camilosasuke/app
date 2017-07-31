

#if defined(INSTALL_SUBSYSTEM)


namespace install
{


   install::install(::aura::application * papp) :
      ::object(papp),
      ::aura::department(papp),
      m_trace(papp)
   {

      defer_create_mutex();

      m_psockethandler = NULL;

      m_bAdmin = false;

      m_bCa2Installed = false;

   }


   install::~install()
   {

      ::aura::del(m_psockethandler);

   }


   bool install::is_file_ok(const ::file::path & path1, const char * pszTemplate, const char * pszFormatBuild)
   {

      string strFormatBuild(pszFormatBuild);

      string strUrl;

      strUrl = "http://" + System.get_system_configuration() + "-server.ca2.cc/api/spaignition/md5?authnone&configuration=" + System.get_system_configuration() + "&stage=";
      //strUrl = "http://" + m_strVersion + "-server.ca2.cc/api/spaignition/md5?authnone&configuration=" + m_strVersion + "&stage=";
      strUrl += pszTemplate;
      strUrl += "&build=";
      strUrl += strFormatBuild;

      property_set set(get_app());

      set["raw_http"] = true;

      return file_exists_dup(path1) && !stricmp_dup(Application.file().md5(path1), Application.http().get(strUrl, set));

   }


   bool install::is_file_ok(const ::file::patha & patha, const ::file::patha & straTemplate, stringa & straMd5, int_array & iaLen, int iMd5Retry)
   {

      bool bOk = true;

      if (patha.get_count() != straTemplate.get_count())
         return false;

      if (bOk)
      {

         for (index i = 0; i < patha.get_count(); i++)
         {

            if (!file_exists_dup(patha[i]))
            {

               bOk = false;

               break;

            }

         }

      }

      if (iMd5Retry > 0 || straMd5.get_count() != patha.get_count())
      {

         string strUrl;

         strUrl = "http://" + System.get_system_configuration() + "-server.ca2.cc/api/spaignition/md5a_and_lena?authnone&configuration=" + System.get_system_configuration() + "&stage=";
         strUrl += straTemplate.implode(",");

         property_set set(get_app());

         set["raw_http"] = true;

         string strMd5List = Application.http().get(strUrl, set);

         straMd5.remove_all();

         straMd5.add_tokens(strMd5List, ",", false);

         if (straMd5.get_count() != patha.get_count())
         {
            straMd5.remove_all();
            return false;
         }

         iaLen.set_size(straMd5.get_size());

         for (index i = 0; i < straMd5.get_size(); i++)
         {

            string strMd5AndLen = straMd5[i];
            strsize iFind = strMd5AndLen.find('|');
            if (iFind < 0)
            {
               iaLen.remove_all();
               straMd5.remove_all();
               return false;
            }
            string strMd5 = strMd5AndLen.Left(iFind);
            straMd5[i] = strMd5;
            straMd5[i].trim();
            if (straMd5[i].get_length() != 32)
            {
               iaLen.remove_all();
               straMd5.remove_all();
               return false;
            }
            iaLen[i] = atoi(strMd5AndLen.Mid(iFind + 1));
         }


         if (!bOk)
            return false;

      }

      for (index i = 0; i < straMd5.get_count(); i++)
      {

         if (Application.file().md5(patha[i]).compare_ci(straMd5[i]) != 0)
            return false;

      }

      return true;

   }

   int32_t install::spalib_main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32_t nCmdShow)
   {

      throw todo(get_app());

      /*      install::installer installer(get_app());

            installer.m_hinstance = hInstance;

            installer.m_nCmdShow = nCmdShow;

            return installer.spaadmin_main(lpCmdLine);*/



   }


   int32_t install::synch_install(const char * pszCommand)
   {

      wait_until_mutex_does_not_exist("Global\\::ca::fontopus::ca2_spa::7807e510-5579-11dd-ae16-0800200c7784");

      string strCommand;

      strCommand = "synch_spaadmin:";

      strCommand += "starter_start:";

      strCommand += pszCommand;

      app_install_call_sync(strCommand);

      return 0;

   }


   void install::app_install_call_sync(const char * pszCommand)
   {
      
      bool bLaunch;

      if (stricmp_dup(pszCommand, "exit") == 0 || stricmp_dup(pszCommand, "quit") == 0)
      {
         
         bLaunch = false;

      }
      else
      {

         bLaunch = true;

      }

      app_install_send_short_message(pszCommand, bLaunch);

   }


   bool install::app_install_send_short_message(const char * psz, bool bLaunch)
   {

#ifdef METROWIN

      throw "todo";

#else

      ::aura::ipc::tx txchannel(get_app());

      installer_launcher launcher(get_app(), System.get_system_configuration());

      if (!txchannel.open(::aura::ipc::app_install(System.get_system_platform()), bLaunch ? &launcher : NULL))
         return false;

      txchannel.send(psz, INFINITE);

#endif

      return true;

   }


   void install::app_install_send_response(const char * param)
   {

#ifdef METROWIN

      throw "todo";

#else

      ::aura::ipc::tx txchannel(get_app());

      installer_launcher launcher(get_app(), System.get_system_configuration());

      if (!txchannel.open("core/spaboot_install_callback"))
         return;

      txchannel.send(param, false);

#endif

   }



   int32_t install::asynch_install(const char * pszCommandLine)
   {

      wait_until_mutex_does_not_exist("Global\\::ca::fontopus::ca2_spa::7807e510-5579-11dd-ae16-0800200c7784");

      string strCommand;

      strCommand = "spaadmin:";

      strCommand += "starter_start:";

      strCommand += pszCommandLine;

      app_install_call_sync(strCommand);

      return 0;

   }


   bool install::is_admin()
   {
      
      return m_bAdmin;

   }


   void install::set_admin(bool bSet)
   {

      m_bAdmin = bSet;

   }


   string install::get_id()
   {

      return m_strId;

   }


   void install::set_id(const char * psz)
   {

      m_strId = psz;

   }


   int32_t install::start_app(const char * id)
   {

      string strDir;
      string strFile;

      strDir = ::dir::stage(process_platform_dir_name2());
      strFile = ::path::app(process_platform_dir_name2());

      string strParam;

      strParam = ": app=";
      strParam += id;

#ifdef METROWIN

      throw "todo";

#else

      call_async(strFile, strParam, strDir, SW_SHOW, false);

#endif

      return 0;

   }


   bool install::is_installing_ca2()
   {

#if 1

      sp(mutex) spmutex = ::mutex::open_mutex(get_app(), "Global\\::ca::fontopus::ca2_spaboot_install_" + process_platform_dir_name2() + "::7807e510-5579-11dd-ae16-0800200c7784");

      return spmutex.is_set();

#elif defined(WINDOWS)

      // more deterministic, with process lifetime determined by process hold mutex

      try
      {

         simple_hold_handle hold_handle(::OpenMutexW(SYNCHRONIZE, FALSE, L"Global\\::ca::fontopus::ca2_spaboot_install::7807e510-5579-11dd-ae16-0800200c7784"));

         if (hold_handle.m_handle == NULL)
         {

            return false;

         }

      }
      catch (...)
      {

         return false;

      }

#else

      // more heuristical, and if there is a process with the same name

      try
      {

         if (get_process_pid("app_app_admin") < 0)
         {

            return false;

         }

      }
      catch (...)
      {

         return false;

      }


#endif

      return false;

   }



   void install::update_ca2_installed(bool bUnloadIfNotInstalled)
   {

      if (is_installing_ca2())
      {

         //m_bCa2Installed = false; in fact we cannot determine if ca2 is installed,
         // because it may be installed but unusable (installation file is locked)
         return;

      }


      string strStage(dir::stage(process_platform_dir_name()));

#ifdef WINDOWSEX

      ::SetDllDirectoryW(wstring(strStage));

#endif

      m_bCa2Installed = true;

      if (m_bCa2Installed)
      {
         // this function (update_ca2_installed) calculates is_ca2_installed and
         // is used at browser plugins like npca2.dll and iexca2.dll which now is
         // linked with os.dll and that are shipped during browser plugin installation
         // with its own version of os.dll. Testing if ca2 is installed by using this
         // os.dll, is not a good indicator anymore, and worst, can potentially make
         // os.dll to be unloaded and make the dependant instatiated browser plugins
         // to malfunction. Using only core.dll test only by now.
         //::aura::library libraryOs(get_app());
         //m_bCa2Installed = libraryOs.open(dir::path(strStage, "os"));
         //if (m_bCa2Installed)
         //{
         
         ::aura::library libraryAura(get_app());

         m_bCa2Installed = libraryAura.open(dir::stage(process_platform_dir_name()) / "aura");

         if (m_bCa2Installed)
         {

            // hey !!
            // now ca2 is installed!!
            // great event!!
            // let's celebrate!!
            // but to celebrate we need some planning, preparation...
            // humm, let's think....
            // is m_strCa2 in win_dir initialized correctly now that there is a ca2 module?
            // (reversed enginereed frustation and anger into flowers garden...)

            System.dir().initialize();

         }

      }

   }


   bool install::is_ca2_installed()
   {

      return m_bCa2Installed;

   }



#ifdef WINDOWS

   void __cdecl _null_se_translator(uint32_t uiCode, EXCEPTION_POINTERS * ppointers)
   {
      UNREFERENCED_PARAMETER(uiCode);
      UNREFERENCED_PARAMETER(ppointers);
   }

#endif






   void install::add_spa_start(const char * pszId)
   {

      ::file::path strPath;

      strPath = System.dir().appdata() / "spa_start.xml";

      string strContents;

      strContents = Application.file().as_string(strPath);

      ::xml::document doc(get_app());

      doc.load(strContents);

      doc.get_root()->set_name("spa");

      stringa straName;
      stringa straValue;

      straName.add("id");
      straValue.add(pszId);

      sp(::xml::node) lpnode = doc.get_root()->GetChildByAllAttr("start", straName, straValue);

      if (lpnode == NULL)
      {

         lpnode = doc.get_root()->add_child("start");

         lpnode->add_attr("id", pszId);

         Application.file().put_contents(strPath, doc.get_xml());

      }

   }

   void install::remove_spa_start(const char * pszId)
   {

      ::file::path strPath;

      strPath = System.dir().appdata() / "spa_start.xml";

      string strContents;

      strContents = Application.file().as_string(strPath);

      ::xml::document doc(get_app());

      doc.load(strContents);

      doc.get_root()->set_name("spa");

      stringa straName;
      stringa straValue;

      straName.add("id");
      straValue.add(pszId);

      sp(::xml::node) lpnode = doc.get_root()->GetChildByAllAttr("start", straName, straValue);

      if (lpnode != NULL)
      {

         doc.get_root()->remove_child(lpnode);

         Application.file().put_contents(strPath, doc.get_xml());

      }

   }


   void install::add_app_install(const char * pszAppId, const char * pszBuild, const char * pszLocale, const char * pszSchema)
   {

      synch_lock sl(m_pmutex);

      ::file::path path;

      path = System.dir().commonappdata_locale_schema(pszAppId, pszBuild, System.get_system_platform(), System.get_system_configuration(), pszLocale, pszSchema);

      path /= "installed.txt";

      ::output_debug_string(path);

      ::output_debug_string("\n");

      Application.file().put_contents(path, "");

   }


   int32_t install::start(const char * pszCommand)
   {

      return System.install().asynch_install(pszCommand);

   }


   int32_t install::synch(const char * pszCommandLine)
   {

      return System.install().synch_install(pszCommandLine);

   }




   string install::app_install_get_extern_executable_path(stringa * pstraMd5, int_array * piaLen, string_to_string * pmapMd5, string_to_intptr * pmapLen)
   {

      bool bPrivileged = false;

      ::file::path strPath;

#ifdef WINDOWSEX

      xxdebug_box("installer::launcher::ensure_executable", "installer::launcher::ensure_executable", 0);

      HINSTANCE hinstanceAppInstall = (HINSTANCE) ::GetModuleHandleA("app_app_admin.exe");

      bPrivileged = hinstanceAppInstall != NULL && hinstanceAppInstall == ::GetModuleHandleA(NULL);

      HINSTANCE hinstancePlugin = (HINSTANCE) ::GetModuleHandleA("npca2.dll");

      if (hinstancePlugin == NULL)
         hinstancePlugin = (HINSTANCE) ::GetModuleHandleA("iexca2.dll");

      if (hinstancePlugin != NULL)
      {

         char szModulePath[MAX_PATH * 3];

         ::GetModuleFileNameA((HINSTANCE)hinstancePlugin, szModulePath, sizeof(szModulePath));

         char * file = NULL;

         int iSpabootInstallStrSize = MAX_PATH * 16;

         LPSTR psz = strPath.GetBufferSetLength(iSpabootInstallStrSize);

         ::GetFullPathNameA(szModulePath, iSpabootInstallStrSize, psz, &file);

         file[0] = '\0';

         strcat_dup(psz, "app_app_admin.exe");

         strPath.ReleaseBuffer();

      }
      else
      {

         strPath = ::path::app_app_admin(System.get_system_platform());

      }

#else

      throw "TODO";

#endif

      if (!file_exists_dup(::dir::system() / "config\\plugin\\do_not_download_file_list.txt") && bPrivileged)
      {

         trace().rich_trace("***Verifying installer");

         ::file::patha straFile;

         ::lemon::array::copy(straFile, ::install::get_app_app_install_module_list());

         if (!::dir::is(strPath.folder()))
         {

            ::dir::mk(strPath.folder());

         }

         ::file::patha straDownload;

         for (index iFile = 0; iFile < straFile.get_size(); iFile++)
         {

            ::file::path strFile = straFile[iFile];

            ::file::path strDownload = strPath.sibling(strFile);

            straDownload.add(strDownload);

         }

         stringa straMd5;

         int_array iaLen;

         if (pstraMd5 != NULL)
         {

            straMd5 = *pstraMd5;

         }

         if (piaLen != NULL)
         {

            iaLen = *piaLen;

         }

         int iMd5Retry = 0;

      md5retry:

         if (!System.install().is_file_ok(straDownload, straFile, straMd5, iaLen, iMd5Retry))
         {

            if (straMd5.get_count() != straFile.get_count())
            {

               iMd5Retry++;

               if (iMd5Retry < 8)
                  goto md5retry;

               return "";

            }

#ifdef WINDOWSEX

            // first try to copy from current path (may be there is a version of app_app_admin at the same folder).

            HMODULE hmodule = ::GetModuleHandle("aura.dll");

            if (hmodule != NULL)
            {

               ::file::path str = System.os().get_module_path(hmodule);

               if (str.has_char())
               {

                  ::file::path strAuraDir = str.folder();

                  for (index iFile = 0; iFile < straFile.get_size(); iFile++)
                  {

                     ::file::path strFile = strAuraDir / straFile[iFile];

                     if (!file_exists_dup(straDownload[iFile]) && file_exists_dup(strFile) && Application.file().md5(strFile) == straMd5[iFile])
                     {

                        ::file_copy_dup(straDownload[iFile], strFile, false);

                     }

                  }


               }


            }

#endif

         }

      }

      return strPath;

   }


   string install::app_install_get_intern_executable_path()
   {

      ::file::path path;

#ifdef WINDOWSEX

      xxdebug_box("installer::launcher::ensure_executable", "installer::launcher::ensure_executable", 0);

      path = ::dir::stage(process_platform_dir_name2()) / "app_app_admin.exe";

#else

      throw "TODO";

#endif

      return path;

   }


   bool install::reference_is_file_ok(const ::file::path & path1, const char * pszTemplate, const char * pszConfiguration, const char * pszFormatBuild)
   {

      string strConfiguration(pszConfiguration);

      string strFormatBuild(pszFormatBuild);

      string strUrl;

      strUrl = "http://" + strConfiguration + ".spaignition.api.server.ca2.cc/md5?version=" + strConfiguration + "&stage=";
      strUrl += pszTemplate;
      strUrl += "&build=";
      strUrl += strFormatBuild;
      strUrl += "&platform=";
      strUrl += System.get_system_platform();


      property_set set(get_app());

      set["raw_http"] = true;

      bool bOk = file_exists_dup(path1) && !stricmp_dup(Application.file().md5(path1), Application.http().get(strUrl, set));

      return bOk;

   }


} // namespace install



#endif



