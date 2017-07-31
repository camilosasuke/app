#include "framework.h"
#include "fiasco_finder.h"
#include "aura/compress/zip/zip.h"


namespace axis
{


   application::application() :
      m_simpledb(this)
   {
      m_bInitializeDataCentral = true;
      if (m_paxissystem != NULL)
      {
         m_bInitializeDataCentral = m_paxissystem->m_bInitializeDataCentral;

      }
      m_peventReady = NULL;


#ifdef WINDOWS

      m_hinstance                = NULL;

#endif

      m_paxisapp              = this;

      if(m_pauraapp != NULL)
      {

         m_paxissystem           = m_pauraapp->m_paxissystem;

         if(m_pauraapp->m_paxissession == NULL && m_paxissystem != NULL)
         {

            m_paxissession       = m_paxissystem->m_paxissession;

         }
         else
         {

            m_paxissession       = m_pauraapp->m_paxissession;

         }

#ifdef WINDOWS

         m_hinstance             = m_pauraapp->m_hinstance;

#endif

      }
      else
      {

         m_paxissystem           = NULL;

      }

//      m_pcoreapp                 = NULL;





      //m_psignal                  = canew(class signal());





      m_bAxisProcessInitialize         = false;
      m_bAxisProcessInitializeResult   = false;

      m_bAxisInitialize1               = false;
      m_bAxisInitialize1Result         = false;

      m_bAxisInitialize                = false;
      m_bAxisInitializeResult          = false;

      m_bAxisInitializeInstance        = false;
      m_bAxisInitializeInstanceResult  = false;


      m_pdataserver = NULL;

      m_iWaitCursorCount         = 0;

      m_pinitmaindata = NULL;

   }

   application::~application()
   {
   }


   void application::assert_valid() const
   {

      thread::assert_valid();


   }

   void application::dump(dump_context & dumpcontext) const
   {

      thread::dump(dumpcontext);

#ifdef WINDOWS

      dumpcontext << "m_hinstance = " << (void *)m_hinstance;

#endif

      //dumpcontext << "\nm_strCmdLine = " << m_strCmdLine;
      //dumpcontext << "\nm_nCmdShow = " << m_nCmdShow;
      dumpcontext << "\nm_bHelpMode = " << m_strAppName;

      dumpcontext << "\n";

   }



   string application::load_string(id id)
   {

      synch_lock sl(&m_mutexStr);

      string str;

      if(m_stringmap.Lookup(id, str))
      {

         return str;

      }

      if(!load_string(str,id))
      {

         id.to_string(str);

      }

      m_stringmap.set_at(id, str);

      return str;

   }


   bool application::load_string(string & str,id id)
   {

      synch_lock sl(&m_mutexStr);

      if(!load_cached_string(str,id,true))
      {

         return false;

      }

      return true;

   }


   bool application::load_cached_string(string & str,id id,bool bLoadStringTable)
   {

      synch_lock sl(&m_mutexStr);

      ::xml::document doc(this);

      if(!doc.load(id))
      {

         return load_cached_string_by_id(str,id,bLoadStringTable);

      }

      sp(::xml::node) pnodeRoot = doc.get_root();

      if(pnodeRoot->get_name() == "string")
      {

         string strId = pnodeRoot->attr("id");

         if (!load_cached_string_by_id(str, strId, bLoadStringTable))
         {

            str = pnodeRoot->get_value();

         }

         return true;

      }

      return false;

   }


   bool application::load_cached_string_by_id(string & str,id id,bool bLoadStringTable)
   {

      synch_lock sl(&m_mutexStr);

      string strId(id.str());
      string strTable;
      string strString;
      string_to_string * pmap = NULL;
      index iFind = 0;
      if((iFind = strId.find(':')) <= 0)
      {
         strTable = "";
         strString = strId;
      }
      else
      {
         strTable = strId.Mid(0,iFind);
         strString = strId.Mid(iFind + 1);
      }
      if(m_stringtableStd.Lookup(strTable,pmap))
      {
         if(pmap->Lookup(strString,str))
         {
            return true;
         }
      }
      else if(m_stringtable.Lookup(strTable,pmap))
      {
         if(pmap->Lookup(strString,str))
         {
            return true;
         }
      }
      else if(bLoadStringTable)
      {
         load_string_table(strTable,"");
         return load_cached_string_by_id(str,id,false);
      }

      return false;

   }


   void application::load_string_table(const string & pszApp,const string & pszId)
   {

      synch_lock sl(&m_mutexStr);

      string strApp(pszApp);
      string strMatter;
      string strLocator;

      if(strApp.is_empty())
      {
         strLocator = System.dir().appmatter_locator(this);
      }
      else
      {
         strLocator = System.dir().appmatter_locator(strApp);
      }

      if(pszId.is_empty())
      {
         strMatter = "stringtable.xml";
      }
      else
      {
         strMatter = pszId + ".xml";
      }

      string strTableId = strApp;

      if(pszId.has_char() && *pszId != '\0')
      {
         strTableId += "\\";
         strTableId += pszId;
      }

      ::xml::document doc(get_app());
      string strFilePath = System.dir().matter_from_locator(Sess(this).str_context(),strLocator,strMatter);
      if(!System.file().exists(strFilePath,this))
      {
         //try
         //{
         //   if(m_stringtable[pszId] != NULL)
         //      delete m_stringtable[pszId];
         //}
         //catch(...)
         //{
         //}
         //m_stringtable.set_at(pszId,new string_to_string);
         return;
      }
      string strFile = Application.file().as_string(strFilePath);
      if(!doc.load(strFile))
         return;
      string_to_string * pmapNew = new string_to_string;
      for(int32_t i = 0; i < doc.get_root()->children().get_count(); i++)
      {
         string strId = doc.get_root()->child_at(i)->attr("id");
         string strValue = doc.get_root()->child_at(i)->get_value();
         pmapNew->set_at(strId,strValue);
      }

      string_to_string * pmapOld = m_stringtable[strTableId];

      m_stringtable[strTableId] = NULL;

      if(pmapOld != NULL)
      {

         try
         {

            delete pmapOld;

         }
         catch(...)
         {

         }

      }

      m_stringtable[strTableId] = pmapNew;
      ASSERT(m_stringtable[strTableId] == pmapNew);
   }




   void application::load_string_table()
   {
      load_string_table("","");
   }


   //sp(object) application::alloc(sp(type) info)
   //{
   //   return System.alloc(this,info);
   //}

   //sp(object) application::alloc(const  id & idType)
   //{
   //   return System.alloc(this,idType);
   //}

   bool application::is_system()
   {

      return false;

   }

   bool application::is_session()
   {

      return false;

   }


   bool application::is_serviceable()
   {

      return false;

   }


   //bool application::app_map_lookup(const char * psz,void * & p)
   //{
   //   return m_appmap.Lookup(psz,p) != FALSE;
   //}

   //void application::app_map_set(const char * psz,void * p)
   //{
   //   m_appmap.set_at(psz,p);
   //}


   //sp(::command_thread) application::command_central()
   //{
   //   return m_pcommandthread;
   //}

   //sp(::command_thread) application::command_thread()
   //{
   //   return m_pcommandthread;
   //}

   //sp(::command_thread) application::command()
   //{
   //   return m_pcommandthread;
   //}

   //sp(::command_thread) application::guideline()
   //{
   //   return m_pcommandthread;
   //}

   //sp(::command_thread) application::directrix()
   //{
   //   return m_pcommandthread;
   //}

   //sp(::command_thread) application::axiom()
   //{
   //   return m_pcommandthread;
   //}

   bool application::verb()
   {
      //axiom()->run();

      return true;

   }

   //sp(::command_thread) application::creation()
   //{
   //   return m_pcommandthread;
   //}

//   class open_url
//   {
//   public:
//      string m_strLink;
//      string m_strTarget;
//      open_url(const string & strLink,const string & pszTarget);
//      bool open();
//   };
//
//
//   open_url::open_url(const string & strLink,const string & pszTarget)
//   {
//      m_strLink = strLink;
//      m_strTarget = pszTarget;
//   }
//
//
//   uint32_t c_cdecl thread_proc_open_url(void * p)
//   {
//
//      open_url * popenurl = (open_url *) p;
//
//      if(!popenurl->open())
//         return -1;
//
//      return 0;
//   }
//
//   bool open_url::open()
//   {
//      string strLink = m_strLink;
//      string pszTarget = m_strTarget;
//#ifdef WINDOWSEX
//      string strUrl = strLink;
//      if(!::str::begins_ci(strUrl,"http://")
//         && !::str::begins_ci(strUrl,"https://"))
//      {
//         strUrl = "http://" + strUrl;
//      }
//      ::ShellExecuteA(NULL,"open",strUrl,NULL,NULL,SW_SHOW);
//      return true;
//#elif defined METROWIN
//#pragma push_macro("System")
//#undef System
//      ::Windows::Foundation::Uri ^ uri = ref new ::Windows::Foundation::Uri(strLink);
//      ::Windows::System::LauncherOptions ^ options = ref new ::Windows::System::LauncherOptions();
//      options->TreatAsUntrusted = false;
//      bool success = ::wait(::Windows::System::Launcher::LaunchUriAsync(uri,options));
//
//      return success;
//
//#pragma pop_macro("System")
//#elif defined(LINUX)
//      ::system("xdg-open " + strLink);
//      return true;
//#elif defined(APPLEOS)
//      openURL(strLink);
//      return true;
//#else
//      throw not_implemented(get_thread_app());
//#endif
//
//   }


   //bool application::open_link(const string & strLink,const string & pszTarget)
   //{
   //   if(is_system())
   //   {
   //      return __begin_thread(this,thread_proc_open_url,new open_url(strLink, pszTarget)) != FALSE;
   //   }
   //   else
   //   {
   //      return System.open_link(strLink,pszTarget);
   //   }

   //   return false;

   //}


   //ref_array < ::user::interaction > application::frames()
   //{

   //   return m_framea.axis_ptra();

   //}


   //void application::add_frame(sp(::user::interaction) pwnd)
   //{

   //   synch_lock sl(&m_framea.m_mutex); // recursive lock (on m_framea.add(pwnd)) but m_puiMain is "cared" by m_frame.m_mutex

   //   m_framea.add(pwnd);

   //   if(m_puiMain == NULL)
   //   {

   //      m_puiMain = pwnd;

   //   }

   //}


   //void application::remove_frame(sp(::user::interaction) pwnd)
   //{

   //   synch_lock sl(&m_framea.m_mutex); // recursive lock (on m_framea.remove(pwnd)) but m_puiMain is "cared" by m_frame.m_mutex

   //   if(m_puiMain == pwnd)
   //   {

   //      m_puiMain = NULL;

   //   }

   //   m_framea.remove(pwnd);

   //}




//#if defined(METROWIN) || defined(APPLE_IOS)
//
//   sp(::user::interaction) application::window_from_os_data(void * pdata)
//   {
//
//      return window_from_handle((oswindow)pdata);
//
//   }
//
//#else
//
//   sp(::user::interaction) application::window_from_os_data(void * pdata)
//   {
//
//      return window_from_handle((oswindow)pdata);
//
//   }
//
//#endif



   //void application::DoWaitCursor(int32_t nCode)
   //{

   //   if(nCode < 0)
   //   {

   //      m_iWaitCursorCount = 0;
   //      ShowWaitCursor(false);

   //   }
   //   else if(nCode == 0)
   //   {

   //      if(m_iWaitCursorCount > 0)
   //      {
   //         m_iWaitCursorCount--;
   //      }

   //      if(m_iWaitCursorCount > 0)
   //      {

   //         ShowWaitCursor(true);

   //      }

   //      m_iWaitCursorCount = 0;
   //      ShowWaitCursor(false);

   //   }
   //   else
   //   {

   //      if(m_iWaitCursorCount < 0)
   //      {
   //         m_iWaitCursorCount = 0;
   //      }

   //      m_iWaitCursorCount++;

   //      ShowWaitCursor(true);

   //   }



   //}

   //void application::ShowWaitCursor(bool bShow)
   //{

   //   if(m_pimpl == NULL)
   //      return;

   //   m_pimpl->ShowWaitCursor(bShow);

   //}



   void application::construct(const char * pszAppId)
   {

   }



   //void application::TermThread(HINSTANCE hInstTerm)
   //{

   //   ::exception::throw_interface_only(this);

   //}



   //sp(::user::interaction) application::FindWindow(const char * lpszClassName,const char * lpszWindowName)
   //{

   //   ::exception::throw_interface_only(this);

   //}


   //sp(::user::interaction) application::FindWindowEx(oswindow oswindowParent,oswindow oswindowChildAfter,const char * lpszClass,const char * lpszWindow)
   //{

   //   ::exception::throw_interface_only(this);

   //}


   //string application::get_version()
   //{

   //   ::exception::throw_interface_only(this);

   //}


   void application::SetCurrentHandles()
   {

      ::aura::application::SetCurrentHandles();

#ifdef INSTALL_SUBSYSTEM

      if (is_installing() || is_uninstalling())
      {

         if (is_system())
         {

            System.install().trace().initialize();

         }

      }

#endif

   }


   bool application::get_temp_file_name_template(string & strRet, const char * pszName, const char * pszExtension, const char * pszTemplate)
   {

#ifdef METROWIN

      string str(::Windows::Storage::ApplicationData::Current->TemporaryFolder->Path);

#else

      char lpPathBuffer[MAX_PATH * 4];

      uint32_t dwRetVal = GetTempPath(sizeof(lpPathBuffer), lpPathBuffer);

      if (dwRetVal > sizeof(lpPathBuffer) || (dwRetVal == 0))
      {

         return FALSE;

      }

      string str(lpPathBuffer);

#endif

      char bufItem[64];

      string strRelative;

      SYSTEMTIME st;

      memset_dup(&st, 0, sizeof(st));

      GetSystemTime(&st);

      itoa_dup(bufItem, st.wYear, 10);
      zero_pad(bufItem, 4);
      strRelative += bufItem;

      itoa_dup(bufItem, st.wMonth, 10);
      zero_pad(bufItem, 2);
      strRelative += "-";
      strRelative += bufItem;

      itoa_dup(bufItem, st.wDay, 10);
      zero_pad(bufItem, 2);
      strRelative += "-";
      strRelative += bufItem;

      itoa_dup(bufItem, st.wHour, 10);
      zero_pad(bufItem, 2);
      strRelative += " ";
      strRelative += bufItem;

      itoa_dup(bufItem, st.wMinute, 10);
      zero_pad(bufItem, 2);
      strRelative += "-";
      strRelative += bufItem;

      itoa_dup(bufItem, st.wSecond, 10);
      zero_pad(bufItem, 2);
      strRelative += "-";
      strRelative += bufItem;

      for (int32_t i = 0; i < (1024 * 1024); i++)
      {

         strRet = ::file::path(str) / (strRelative + "-" + hex::lower_from(i + 1)) / (string(pszName) + string(".") + pszExtension);

#ifdef INSTALL_SUBSYSTEM

         if (pszTemplate != NULL)
         {

            if (System.install().is_file_ok(strRet, pszTemplate, ""))
               return true;

         }

#endif


         if (file_exists_dup(strRet))
         {

            try
            {

               Application.file().del(strRet);

            }
            catch (...)
            {

               continue;

            }

            return true;

         }
         else
         {
            return true;
         }
      }
      return false;

   }


   //void application::set_env_var(const string & var,const string & value)
   //{

   //   ::exception::throw_interface_only(this);

   //}




   //bool application::_001OnDDECommand(const char * lpcsz)
   //{

   //   ::exception::throw_interface_only(this);

   //   return false;

   //}


   //void application::_001EnableShellOpen()
   //{

   //   ::exception::throw_interface_only(this);

   //}


   //::user::document * application::_001OpenDocumentFile(var varFile)
   //{

   //   ::exception::throw_interface_only(this);

   //   return NULL;

   //}


   //void application::_001OnFileNew(signal_details * pobj)
   //{

   //   ::exception::throw_interface_only(this);

   //}


   //sp(::aura::printer) application::get_printer(const char * pszDeviceName)
   //{

   //   ::exception::throw_interface_only(this);

   //}


   //bool application::update_module_paths()
   //{


   //   if(is_system())
   //   {

   //      if(!m_pimpl->update_module_paths())
   //         return false;


   //   }

   //   return true;


   //}











/*
   void application::process_machine_event_data(machine_event_data * pdata)
   {
      if(pdata->m_fixed.m_bRequestCloseApplication)
      {
         _001CloseApplication();
      }
   }
   */

   void application::_001CloseApplication()
   {

      throw todo(get_app());

   }





//   sp(::user::interaction) application::release_capture_uie()
//   {
//
//#if defined(LINUX)
//
//      oswindow oswindowCapture = ::GetCapture();
//      if (oswindowCapture == NULL)
//         return NULL;
//      return oswindowCapture->get_user_interaction()->ReleaseCapture();
//
//#elif defined(WINDOWS)
//
//      oswindow oswindowCapture = ::GetCapture();
//      if(oswindowCapture == NULL)
//         return NULL;
//      return System.ui_from_handle(oswindowCapture)->ReleaseCapture();
//
//#elif defined(APPLEOS)
//
//      oswindow oswindowCapture = ::GetCapture();
//      if (oswindowCapture == NULL)
//         return NULL;
//      return oswindowCapture->get_user_interaction()->ReleaseCapture();
//
//#else
//
//      ::exception::throw_not_implemented(get_app());
//
//#endif
//
//   }
//
//
//   sp(::user::interaction) application::get_capture_uie()
//   {
//
//#if defined(METROWIN)
//
//      oswindow oswindowCapture = ::GetCapture();
//
//      if (oswindowCapture == NULL)
//         return NULL;
//
//      ::user::interaction * pui = oswindowCapture->window();
//
//      if (pui == NULL)
//         return NULL;
//
//      return pui->GetCapture();
//
//#elif defined(WINDOWS) || defined(APPLE_IOS)
//
//      oswindow oswindowCapture = ::GetCapture();
//
//      if(oswindowCapture == NULL)
//         return NULL;
//
//      sp(::user::interaction) pui = System.ui_from_handle(oswindowCapture);
//
//      if(pui == NULL)
//         return NULL;
//
//      return pui->GetCapture();
//
//#else
//
//      //      ::exception::throw_not_implemented(get_app());
//
//      oswindow oswindowCapture = ::GetCapture();
//
//      if (oswindowCapture == NULL)
//         return NULL;
//
//      return ::GetCapture()->get_user_interaction()->m_pimpl->GetCapture();
//
//#endif
//
//   }
//
//
//
//
//
//
//
//   uint32_t application::get_thread_id()
//   {
//      return m_pimpl->get_thread_id();
//   }
//
//
//

//
//#ifndef METROWIN
//
//   void application::get_time(timeval *p)
//   {
//      m_pimpl->get_time(p);
//   }
//
//#endif
//

   //bool application::do_prompt_file_name(var & varFile,UINT nIDSTitle,uint32_t lFlags,bool bOpenFileDialog,sp(::user::impact_system) ptemplate, ::user::document * pdocument)
   //{

   //   UNREFERENCED_PARAMETER(varFile);
   //   UNREFERENCED_PARAMETER(nIDSTitle);

   //   return false;

   //}

   //string CLASS_DECL_AXIS application::get_cred(const string & strRequestUrl,const RECT & rect,string & strUsername,string & strPassword,string strToken,string strTitle,bool bInteractive)
   //{

   //   return ::fontopus::get_cred(this,strRequestUrl, rect,strUsername,strPassword,strToken,strTitle,bInteractive);

   //}







   bool application::get_temp_file_name(string & strRet,const char * pszName,const char * pszExtension)
   {

      return get_temp_file_name_template(strRet,pszName,pszExtension,NULL);

   }






/*

   ::visual::icon * application::set_icon(object * pobject,::visual::icon * picon,bool bBigIcon)
   {

      ::visual::icon * piconOld = get_icon(pobject,bBigIcon);

      if(bBigIcon)
      {

         pobject->oprop("big_icon").operator =((sp(object)) picon);

      }
      else
      {

         pobject->oprop("small_icon").operator =((sp(object)) picon);

      }

      return piconOld;

   }


   ::visual::icon * application::get_icon(object * pobject,bool bBigIcon) const
   {

      if(bBigIcon)
      {

         return const_cast <object *> (pobject)->oprop("big_icon").cast < ::visual::icon >();

      }
      else
      {

         return const_cast <object *> (pobject)->oprop("small_icon").cast < ::visual::icon >();

      }

   }
*/


   bool application::final_handle_exception(::exception::exception & e)
   {
      UNREFERENCED_PARAMETER(e);
      //linux      exit(-1);

      if(!is_system())
      {

         // get_app() may be it self, it is ok...
         if(Sys(get_app()).final_handle_exception((::exception::exception &) e))
            return true;


      }

      return false;
   }





   int32_t application::main()
   {

      TRACE(string(typeid(*this).name()) + " main");;

      dappy(string(typeid(*this).name()) + " : application::main 1");

      try
      {

         TRACE(string(typeid(*this).name()) + " on_run");;
         dappy(string(typeid(*this).name()) + " : going to on_run : " + ::str::from(m_iReturnCode));
         m_iReturnCode = 0;
         m_bReady = true;
         m_iReturnCode = on_run();
         if(m_iReturnCode != 0)
         {
            dappy(string(typeid(*this).name()) + " : on_run failure : " + ::str::from(m_iReturnCode));
            ::output_debug_string("application::main on_run termination failure\n");
         }

      }
      catch(::exit_exception &)
      {

         dappy(string(typeid(*this).name()) + " : on_run exit_exception");

         ::multithreading::post_quit(&System);

         goto exit_application;

      }
      catch(...)
      {

         dappy(string(typeid(*this).name()) + " : on_run general exception");

         goto exit_application;

      }

      try
      {

         if(is_system())
         {

            dappy(string(typeid(*this).name()) + " : quiting main");

            //::aura::post_quit_thread(&System);

            //Sleep(5000);

         }

      }
      catch(...)
      {

      }

   exit_application:


      try
      {

         m_iReturnCode = exit_thread();

      }
      catch(::exit_exception &)
      {

         ::multithreading::post_quit(&System);

         m_iReturnCode = -1;

      }
      catch(...)
      {

         m_iReturnCode = -1;

      }

      return m_iReturnCode;

   }




   //bool application::pre_run()
   //{

   //   TRACE(string(typeid(*this).name()) + " main_start");;
   //   try
   //   {

   //      m_dwAlive = ::get_tick_count();
   //      TRACE(string(typeid(*this).name()) + "application_pre_run");;
   //      int32_t m_iReturnCode = application_pre_run();
   //      if(m_iReturnCode != 0)
   //      {
   //         dappy(string(typeid(*this).name()) + " : applicationpre_run failure : " + ::str::from(m_iReturnCode));
   //         m_bReady = true;
   //         TRACE("application::main application_pre_run failure");
   //         return false;
   //      }

   //      xxdebug_box("pre_runnned","pre_runnned",MB_ICONINFORMATION);
   //      dappy(string(typeid(*this).name()) + " : pre_runned : " + ::str::from(m_iReturnCode));
   //      TRACE(string(typeid(*this).name()) + " initial_check_directrix");;
   //      if(!initial_check_directrix())
   //      {
   //         dappy(string(typeid(*this).name()) + " : initial_check_directrix failure");
   //         m_iReturnCode = -1;
   //         exit_thread();
   //         m_bReady = true;
   //         ::output_debug_string("exiting on check directrix\n");
   //         return false;
   //      }


   //      TRACE(string(typeid(*this).name()) + " os_native_bergedge_start");;
   //      m_dwAlive = ::get_tick_count();
   //      if(!os_native_bergedge_start())
   //      {
   //         dappy(string(typeid(*this).name()) + " : os_native_bergedge_start failure");
   //         exit_thread();
   //         m_iReturnCode = -1;
   //         m_bReady = true;
   //         ::output_debug_string("application::main os_native_bergedge_start failure");
   //         return false;
   //      }

   //      return true;
   //   }
   //   catch(::exit_exception &)
   //   {

   //      dappy(string(typeid(*this).name()) + " : main_start exit_exception");

   //      ::multithreading::post_quit(&System);

   //   }

   //   return false;

   //}




   int32_t application::on_run()
   {
      int32_t m_iReturnCode = 0;

      try
      {
         ::aura::application_signal_details signal(m_psignal,::aura::application_signal_start);
         m_psignal->emit(&signal);
      }
      catch(...)
      {
      }

      dappy(string(typeid(*this).name()) + " : starting on_run : " + ::str::from(m_iReturnCode));

      thread * pthread = ::get_thread();

      install_message_handling(pthread);

      dappy(string(typeid(*this).name()) + " : starting on_run 2 : " + ::str::from(m_iReturnCode));

      try
      {
         try
         {
            m_bReady = true;
            if(m_peventReady != NULL)
               m_peventReady->SetEvent();
         }
         catch(...)
         {
         }
      run:
         try
         {
            m_iReturnCode = run();

         }
         catch(::exit_exception & e)
         {

            throw e;

         }
         catch(const ::exception::exception & e)
         {
            if(on_run_exception((::exception::exception &) e))
               goto run;
            if(final_handle_exception((::exception::exception &) e))
               goto run;
            try
            {
               m_iReturnCode = exit_thread();
            }
            catch(::exit_exception & e)
            {

               throw e;

            }
            catch(...)
            {
               m_iReturnCode = -1;
            }
            goto InitFailure;
         }
      }
      catch(::exit_exception & e)
      {

         throw e;

      }
      catch(...)
      {
         // linux-like exit style on crash, differently from general windows error message approach
         // to prevent or correct from crash, should:
         // - look at dumps - to do;
         // - look at trace and log - always doing;
         // - look at debugger with the visual or other tool atashed - to doing;
         // - fill other possibilities;
         // - restart and send information in the holy back, and stateful or self-heal as feedback from below;
         // - ...
         // - ..
         // - .
         // - .
         // - .
         // - .
         // -  .
         // - ...
         // - ...
         // - ...
         // to pro-activia and overall benefits workaround:
         // - stateful applications:
         //      - browser urls, tabs, entire history, in the ca2computing cloud;
         //      - uint16_t - html document to simplify here - with all history of undo and redos per document optimized by cvs, svn, syllomatter;
         //           - not directly related but use date and title to name document;
         //      - save forms after every key press in .undo.redo.form file parallel to appmatter / form/undo.redo.file parity;
         //      - last ::ikaraoke::karaoke song and scoring, a little less motivated at time of writing;
         //
         // - ex-new-revolut-dynamic-news-self-healing
         //      - pre-history, antecendentes
         //            - sometimes we can't recover from the last state
         //            - to start from the beggining can be too heavy, waity, worky, bory(ing)
         //      - try to creativetily under-auto-domain with constrained-learning, or heuristcally recover from restart, shutdown, login, logoff;
         //           - reification :
         //           - if the document is corrupted, try to open the most of it
         //           - if can only detect that the document cannot be opened or damaged, should creatively workarounds as it comes, as could it be
         //              done, for example search in the web for a proper solution?
         //           - ::ikaraoke::karaoke file does not open? can open next? do it... may animate with a temporary icon...
         //               speed into cartesian dimensions of
         //               I think no one likes to be boring, but a entire background in black... I don't know... only for your personal office, may be...
         //           - could an online colaborator investigate crashes promptly in a funny way, and make news and jokes? Like terra and UOL for the real world?
         //               - new crash, two documents lost, weathers fault, too hot, can't think, my mother was angry with me, lead to buggy code;
         //               - new version with bug fixes;
         //      - new versions
         //      - automatic updates
         //      - upgrades
         //      - rearrangemntes
         //      - downgrade in the form of retro
         // - ...
         // - ..
         // - .
         // - .
         // - .
         // - .
         // -  .
         // - ...
         // - ...
         // - ...

      }
   InitFailure:
      try
      {
         if(m_peventReady != NULL)
            m_peventReady->SetEvent();
      }
      catch(...)
      {
      }
      try
      {
         thread * pthread = this;
         if(pthread != NULL && pthread->m_pevReady != NULL)
         {
            pthread->m_pevReady->SetEvent();
         }
      }
      catch(...)
      {
      }
      /*try
      {
      thread * pthread = dynamic_cast < thread * > (this);
      ::SetEvent((HANDLE) pthread->m_peventReady);
      }
      catch(...)
      {
      }*/

      // let translator run undefinetely
      /*if(is_system())
      {
      translator::detach();
      }*/

      return m_iReturnCode;
   }




   int32_t application::application_pre_run()
   {

      return ::aura::application::application_pre_run();

   }



   bool application::InitApplication()
   {

      return true;

   }


   bool application::check_install()
   {

      if (directrix()->m_varTopicQuery.has_property("install"))
      {

         if (!on_install())
         {
            
            ::output_debug_string("Failed at on_install : " + m_strAppId + "\n\n");
            
            System.m_iReturnCode = -1;
            
            return false;

         }

         string strBuild; 

         string strAppId = m_strAppId;

         xxdebug_box("on_install1", strAppId, 0);

         if (strAppId.is_empty())
         {

            strAppId = m_strAppName;

         }

         if (strAppId.has_char() && command()->m_varTopicQuery.has_property("app") && strAppId == command()->m_varTopicQuery["app"])
         {

            system_add_app_install(strAppId, "installed");

            if (strBuild.has_char())
            {

               system_add_app_install(strAppId, strBuild);

            }

         }
         else if (strAppId.has_char() && command()->m_varTopicQuery.has_property("session_start") && strAppId == command()->m_varTopicQuery["session_start"])
         {

            system_add_app_install(strAppId, "installed");

            if (strBuild.has_char())
            {

               system_add_app_install(strAppId, strBuild);

            }

         }
         else if (m_strInstallToken.has_char())
         {

            system_add_app_install(m_strInstallToken, "installed");

            if (strBuild.has_char())
            {

               system_add_app_install(m_strInstallToken, strBuild);

            }

         }

      }
      else if (directrix()->m_varTopicQuery.has_property("uninstall"))
      {

         if (!on_uninstall())
         {

            return false;

         }

#ifdef INSTALL_SUBSYSTEM

         System.install().remove_spa_start(m_strAppId);

#endif

      }

      return true;

   }


   bool application::initial_check_directrix()
   {


      string strLicense = get_license_id();

      var & varTopicQuey = System.directrix()->m_varTopicQuery;

      bool bHasInstall = varTopicQuey.has_property("install");

      bool bHasUninstall = varTopicQuey.has_property("uninstall");

      if(!(bHasInstall || bHasUninstall)
         && m_bLicense
         && strLicense.has_char())
      {

         if(!Session.assert_user_logged_in())
         {
            return false;
         }

         // call application's is_licensed function
         // because if delay is needed for authentication -
         // or either asking for authentication -
         // current application startup won't be
         // exited by timeout.

         int32_t iRetry = 1;

      retry_license:

         iRetry--;

         if(!Session.is_licensed(strLicense))
         {

            if(iRetry > 0)
               goto retry_license;

            return false;

         }

      }


      ::output_debug_string("initial_check_directrix : ok ("+string(typeid(*this).name())+")" + m_strAppId + "\n\n");


      return true;

   }


   bool application::on_install()
   {

      if(!aura::application::on_install())
         return false;

      return true;

   }


   bool application::on_uninstall()
   {

      bool bOk = aura::application::on_uninstall();

      return bOk;

   }






   bool application::os_native_bergedge_start()
   {

      return true;

   }











   void application::on_request(::create * pcreate)
   {

      ::object::on_request(pcreate);

      command()->consolidate(pcreate);

   }


   int32_t application::run()
   {

      return ::aura::application::run();

   }




   sp(::aura::application) application::assert_running(const char * pszAppId)
   {

      sp(::aura::application) papp;

      papp = Session.m_appptra.find_running_defer_try_quit_damaged(pszAppId);

      if(papp.is_null())
      {

         sp(::create) spcreate(allocer());

         papp = Session.start_application(pszAppId,spcreate);

      }

      return papp;

   }




/*
   typedef  void(*PFN_ca2_factory_exchange)(sp(application) papp);

*/


   /*::file::file_sp application::friendly_get_file(var varFile, UINT nOpenFlags)
   {

   try
   {

   return m_file.get_file(varFile, nOpenFlags);

   }
   catch (::file::exception & e)
   {

   string strMessage = e.get_message();

   App(this).simple_message_box(NULL, strMessage, MB_OK);

   return NULL;

   }

   }
   */




   //bool application::is_installing()
   //{

   //   return directrix()->has_property("install");

   //}


   //bool application::is_uninstalling()
   //{

   //   return directrix()->has_property("uninstall");

   //}


   //bool application::create_new_service()
   //{

   //   if(m_pservice != NULL)
   //      return false;

   //   m_pservice = allocate_new_service();

   //   if(m_pservice == NULL)
   //      return false;

   //   return true;

   //}



   //bool application::create_service()
   //{

   //   return System.os().create_service(this);

   //}

   //bool application::remove_service()
   //{

   //   return System.os().remove_service(this);

   //}

   //bool application::start_service()
   //{

   //   return System.os().start_service(this);

   //}

   //bool application::stop_service()
   //{

   //   return System.os().stop_service(this);

   //}


   //void application::on_service_request(::create * pcreate)
   //{

   //   if(!is_serviceable())
   //      return;

   //   if(pcreate->m_spCommandLine->m_varQuery.has_property("create_service"))
   //   {
   //      create_service();
   //   }
   //   else if(pcreate->m_spCommandLine->m_varQuery.has_property("start_service"))
   //   {
   //      start_service();
   //   }
   //   else if(pcreate->m_spCommandLine->m_varQuery.has_property("stop_service"))
   //   {
   //      stop_service();
   //   }
   //   else if(pcreate->m_spCommandLine->m_varQuery.has_property("remove_service"))
   //   {
   //      remove_service();
   //   }


   //}







   bool application::process_initialize()
   {

      if (m_bAxisProcessInitialize)
      {

         return m_bAxisProcessInitializeResult;

      }

      thisstart;

      m_bAxisProcessInitialize = true;

      m_bAxisProcessInitializeResult = false;

      if (!::aura::application::process_initialize())
      {

         thisfail << 1;

         return false;

      }

      if (m_paurasystem->m_phtml == NULL)
      {

         m_paurasystem->m_phtml = create_html();

         if (m_paurasystem->m_phtml == NULL)
         {

            thisfail << 2;

            return false;

         }

         m_paurasystem->m_phtml->construct(m_pauraapp);

      }

      m_bAxisProcessInitializeResult = true;

      thisend;

      return true;

   }


   bool application::initialize_application()
   {

      if(m_bAxisInitializeInstance)
         return m_bAxisInitializeInstanceResult;

      thisstart;

      m_bAxisInitializeInstance = true;
      m_bAxisInitializeInstanceResult = false;

      if(!::aura::application::initialize_application())
      {

         thisfail << 1;

         return false;

      }

      thisok << 1;

      m_dataid += m_strDataIdAddUp;

      m_bAxisInitializeInstanceResult = true;

      thisend;

      return true;

   }


   bool application::initialize1()
   {

      if(m_bAxisInitialize1)
         return m_bAxisInitialize1Result;

      if (m_bInitializeDataCentral)
      {



         m_simpledb.construct(this);

         if (!m_simpledb.initialize())
            return false;

         if (!m_simpledb.initialize2())
            return false;


         set_data_server(&Application.dataserver());


      }

      m_bAxisInitialize1 = true;

      m_bAxisInitialize1Result = false;

      m_dwAlive = ::get_tick_count();

      if(!::aura::application::initialize1())
         return false;


      if (!m_paurasystem->m_phtml->initialize())
      {

         return false;

      }



      m_bAxisInitialize1Result = true;

      return true;

   }


   bool application::initialize2()
   {

      if(!::aura::application::initialize2())
         return false;

      if(!ca_initialize2())
         return false;

      return true;

   }


   bool application::initialize3()
   {

      if(!::aura::application::initialize3())
      {

         return false;

      }

      if(!ca_initialize3())
         return false;

      return true;

   }


   bool application::initialize()
   {

      if(m_bAxisInitialize)
         return m_bAxisInitializeResult;

      m_bAxisInitialize = true;
      m_bAxisInitializeResult = false;

      ::aura::application_signal_details signal(m_psignal,::aura::application_signal_initialize);

      m_psignal->emit(&signal);

      if(!signal.m_bOk)
         return false;

      m_dwAlive = ::get_tick_count();

      if(is_system())
      {
         if(guideline()->m_varTopicQuery.propset().has_property("save_processing"))
         {
            Session.savings().save(::aura::resource_processing);
         }
         if(guideline()->m_varTopicQuery.propset().has_property("save_blur_back"))
         {
            Session.savings().save(::aura::resource_blur_background);
         }
         if(guideline()->m_varTopicQuery.propset().has_property("save_transparent_back"))
         {
            Session.savings().save(::aura::resource_translucent_background);
         }
      }

      if(directrix()->m_varTopicQuery.propset().has_property("install"))
      {
         // core level app install
         if(!Ex2OnAppInstall())
            return false;
      }
      else if(directrix()->m_varTopicQuery.propset().has_property("uninstall"))
      {
         // core level app uninstall
         if(!Ex2OnAppUninstall())
            return false;
      }
      else
      {
#ifdef WINDOWSEX
         // when this process is started in the context of a system account,
         // for example, this code ensure that the process will
         // impersonate a loggen on ::fontopus::user
         HANDLE hprocess;
         HANDLE htoken;

         hprocess = ::GetCurrentProcess();

         if(!OpenProcessToken(
            hprocess,
            TOKEN_ALL_ACCESS,
            &htoken))
            return false;

         if(!ImpersonateLoggedOnUser(htoken))
         {
            TRACELASTERROR();
            return false;
         }
#endif
      }

      m_dwAlive = ::get_tick_count();

      if(is_system()
         && !command_thread()->m_varTopicQuery["app"].get_string().begins_ci("app-core/netnode")
         && command_thread()->m_varTopicQuery["app"] != "app-core/netnode_dynamic_web_server"
         && command_thread()->m_varTopicQuery["app"] != "app-gtech/alarm"
         && command_thread()->m_varTopicQuery["app"] != "app-gtech/sensible_service")
      {
         System.http().defer_auto_initialize_proxy_configuration();
      }

      m_dwAlive = ::get_tick_count();

      m_bAxisInitializeResult = true;

      dappy(string(typeid(*this).name()) + " : initialize ok : " + ::str::from(m_iReturnCode));

      return true;

   }


   int32_t application::exit_application()
   {

      try
      {
         
         for(auto & pair : System.m_appmap)
         {
            
            try
            {
               
               if(pair.m_element2->m_paxisapp == this)
               {
                  
                  pair.m_element2->m_paxisapp = NULL;
                  
               }
               
            }
            catch(...)
            {
               
            }
            
         }
         
      }
      catch(...)
      {
   
      }

      try
      {


         //destroy_message_queue();

      }
      catch(...)
      {

         m_iReturnCode = -1;

      }




      //try
      //{


         /*      try
         {
         if(m_plemonarray != NULL)
         {
         delete m_plemonarray;
         }
         }
         catch(...)
         {
         }
         m_plemonarray = NULL;
         */


         //m_pcommandthread.release();

         release_exclusive();

//         if(m_spuiMessage.is_set())
         {

            //if(!destroy_message_queue())
            {

               // TRACE("Could not finalize message interaction_impl");

            }

         }


         if(m_psignal != NULL)
         {

            ::aura::application_signal_details signal(m_psignal,::aura::application_signal_exit_instance);

            try
            {

               m_psignal->emit(&signal);

            }
            catch(...)
            {

            }

            m_psignal.release();

         }

         //try
         //{
         //   if (!is_system())
         //   {
         //      System.unregister_bergedge_application(this);
         //   }
         //}
         //catch (...)
         //{
         //}

         /*try
         {
         ::release(smart_pointer <thread>::m_p);
         }
         catch(...)
         {
         }*/


         if(is_system())
         {

            //         try
            //       {
            //        if(m_spfilesystem.m_p != NULL)
            //      {
            //       ::core::del(m_spfilesystem.m_p);
            //  }
            //         }
            //       catch(...)
            //     {
            //   }
         }


         if (m_paurasystem->m_phtml != NULL)
         {

            ::aura::del(m_paurasystem->m_phtml);

            m_paurasystem->m_phtml = NULL;

         }


         try
         {

               try
               {

                  aura::application::exit_application();

               }
               catch(...)
               {

               }

         }
         catch(...)
         {

         }

         /*try
         {

            m_pthreadimpl.release();

         }
         catch(...)
         {

         }
         */

         try
         {

            m_simpledb.finalize();

         }
         catch (...)
         {


         }

      return m_iReturnCode;

   }


   bool application::finalize()
   {

      bool bOk = false;

      try
      {

         bOk = ::aura::application::finalize();

      }
      catch(...)
      {

         bOk = false;

      }

      return bOk;

   }






   bool application::is_running()
   {
      return is_alive();
   }



   bool application::Ex2OnAppInstall()
   {
      return true;
   }


   bool application::Ex2OnAppUninstall()
   {

      return true;

   }


   bool application::update_appmatter(::sockets::socket_handler & handler, sp(::sockets::http_session) & psession,const ::file::path & pszRoot,const string & pszRelative)
   {

      ::str::international::locale_schema localeschema(this);

      Session.fill_locale_schema(localeschema);

      bool bIgnoreStdStd = string(pszRoot) == "app" && (string(pszRelative) == "main" || string(pszRelative) == "bergedge");

      //update_appmatter(h, psession, pszRoot, pszRelative, localeschema.m_idLocale, localeschema.m_idSchema);

      ::count iCount = localeschema.m_idaLocale.get_count();

      for(index i = 0; i < iCount; i++)
      {

         if(localeschema.m_idaLocale[i] == __id(std) && localeschema.m_idaSchema[i] == __id(std) && bIgnoreStdStd)
            continue;

         string strLocale;

         localeschema.m_idaLocale[i].to_string(strLocale);

         string strSchema;

         localeschema.m_idaSchema[i].to_string(strSchema);

         update_appmatter(handler, psession,pszRoot,pszRelative,strLocale,strSchema);

         System.install_progress_add_up();

      }


      return true;

   }

   bool application::update_appmatter(::sockets::socket_handler & handler, sp(::sockets::http_session) & psession,const ::file::path & pszRoot,const string & pszRelative,const string & pszLocale,const string & pszStyle)
   {

      string strLocale;
      string strSchema;
      TRACE("update_appmatter(root=%s, relative=%s, locale=%s, style=%s)",pszRoot,pszRelative,pszLocale,pszStyle);
      ::file::path strRelative = ::file::path(pszRoot) / "appmatter" / pszRelative  / Sess(this).get_locale_schema_dir(pszLocale,pszStyle) + ".zip";
      ::file::path strFile = System.dir().element() / strRelative;
      ::file::path strUrl(::file::path_url);
      if(_ca_is_basis())
      {
         strUrl = "http://basis-server.ca2.cc/api/spaignition/download?authnone&configuration=basis&stage=";
      }
      else
      {
         strUrl = "http://stage-server.ca2.cc/api/spaignition/download?authnone&configuration=stage&stage=";
      }

      strUrl += System.url().url_encode(strRelative);


      if(psession == NULL)
      {

         while(true)
         {

            property_set setEmpty(get_app());

            if (System.http().open(handler, psession, System.url().get_server(strUrl), System.url().get_protocol(strUrl), setEmpty, NULL, NULL))
            {

               break;

            }

            Sleep(184);

         }

      }

      property_set set;

      set["get_memory"] = "";

      if (!System.http().request(handler, psession, strUrl, set))
      {

         return false;

      }

      ::memory_file file(get_app(),set["get_memory"].cast < memory >());

      if(set["get_memory"].cast < memory >() != NULL && set["get_memory"].cast < memory >()->get_size() > 0)
      {

         zip::util util;

         string strDir = strFile;

         ::str::ends_eat_ci(strDir,".zip");

         try
         {

            util.extract_all(this, strDir,&file);

         }
         catch(...)
         {

            // spa app_app_admin.exe would recover by retrying or someone would fix the resource packaging problem and then zip extraction at least should work.

            return false;

         }

         //System.compress().extract_all(strFile, this);

      }

      return true;

   }







   //void application::on_set_scalar(e_scalar escalar,int64_t iValue, int iFlags)
   //{

   //   //if (escalar == scalar_app_install_progress)
   //   //{

   //   //   m_iProgressInstallStep = iValue;

   //   //}
   //   //else if (escalar == scalar_app_install_progress_min)
   //   //{

   //   //   m_iProgressInstallStart = iValue;

   //   //}
   //   //else if (escalar == scalar_app_install_progress_max)
   //   //{

   //   //   m_iProgressInstallEnd = iValue;

   //   //}
   //   //else
   //   {

   //      return ::int_scalar_source::on_set_scalar(escalar,iValue, int iFlags);

   //   }

   //}


   //void application::get_scalar_minimum(e_scalar escalar,int64_t & i)
   //{

   //   //if (escalar == scalar_app_install_progress)
   //   //{

   //   //   i = m_iProgressInstallStart;

   //   //}
   //   //else if (escalar == scalar_app_install_progress_min)
   //   //{

   //   //   i = 0;

   //   //}
   //   //else if (escalar == scalar_app_install_progress_max)
   //   //{

   //   //   i = 0;

   //   //}
   //   //else
   //   {

   //      ::int_scalar_source::get_scalar_minimum(escalar,i);

   //   }

   //}

   //void application::get_scalar(e_scalar escalar,int64_t & i)
   //{

   //   //if (escalar == scalar_app_install_progress)
   //   //{

   //   //   i = m_iProgressInstallStep;

   //   //}
   //   //else if (escalar == scalar_app_install_progress_min)
   //   //{

   //   //   i = m_iProgressInstallStart;

   //   //}
   //   //else if (escalar == scalar_app_install_progress_max)
   //   //{

   //   //   i = m_iProgressInstallEnd;

   //   //}
   //   //else
   //   {

   //      ::int_scalar_source::get_scalar(escalar,i);

   //   }

   //}

   //void application::get_scalar_maximum(e_scalar escalar,int64_t & i)
   //{

   //   //if (escalar == scalar_download_size)
   //   //{

   //   //   i = m_iProgressInstallEnd;

   //   //}
   //   //else if (escalar == scalar_app_install_progress_min)
   //   //{

   //   //   i = 0x7fffffff;

   //   //}
   //   //else if (escalar == scalar_app_install_progress_max)
   //   //{

   //   //   i = 0x7fffffff;

   //   //}
   //   //else
   //   {

   //      ::int_scalar_source::get_scalar_minimum(escalar,i);

   //   }

   //}




   //service_base * application::get_service()
   //{

   //   return m_pservice;

   //}





   //string application::get_license_id()
   //{

   //   if(m_strLicense.is_empty())
   //   {
   //
   //      m_strLicense = m_strAppId;

   //      if(m_strLicense.is_empty())
   //      {

   //         m_strLicense = m_strAppName;

   //      }

   //   }



   //   return m_strLicense;

   //}







   bool application::assert_user_logged_in()
   {

      const char * pszRequestUrl = NULL;

      string strRequestUrl;

      if(file_as_string_dup(::dir::system() / "config\\system\\ignition_server.txt").has_char())
      {

         strRequestUrl = "https://" + file_as_string_dup(::dir::system() / "config\\system\\ignition_server.txt") + "/api/spaignition";

         pszRequestUrl = strRequestUrl;

      }

      if(m_paxissession->fontopus()->get_user(true,pszRequestUrl) == NULL)
         //   throw exit_exception(get_app(),"You have not logged in!! db_str_set::load");
         return false;

      return true;

   }




   string application::matter_as_string(const char * pszMatter,const char * pszMatter2)
   {

      var varQuery;

      varQuery["disable_ca2_sessid"] = true;

      string strMatter = dir().matter(::file::path(pszMatter) / pszMatter2);

      return file().as_string(strMatter,varQuery);

   }

   //string application::dir().matter(const char * pszMatter,const char * pszMatter2)
   //{

   //   return dir().matter(pszMatter,pszMatter2);

   //}

   //bool application::is_inside_time_dir(const char * pszPath)
   //{
   //   return dir().is_inside_time(pszPath);
   //}


   bool application::file_is_read_only(const char * pszPath)
   {

      return false;
      //return file().is_read_only(pszPath);

   }


//   string application::file_as_string(var varFile)
//   {
//
//      //if(::str::begins_ci(varFile.get_string(),"http://")
//      //   || ::str::begins_ci(varFile.get_string(),"https://"))
//      //{
//
//      //   ::property_set set(get_app());
//
//      //   return http().get(varFile.get_string(),set);
//
//      //}
//      //else if(varFile.has_property("url") &&
//      //   (::str::begins_ci(varFile["url"].get_string(),"http://")
//      //   || ::str::begins_ci(varFile["url"].get_string(),"https://")))
//      //{
//
//      //   ::property_set set(get_app());
//
//      //   return http().get(varFile["url"].get_string(),set);
//
//      //}
//      //else
//      //{
//         return file().as_string(varFile.get_string());
////      }
//
//   }
//
//   //string application::dir_path(const char * psz1,const char * psz2,const char * psz3)
//   //{
//   //   return psz1,psz2,psz3);
//   //}
//
//   string application::dir_name(const char * psz)
//   {
//      return System.dir().name(psz);
//   }
//
//   bool application::dir_mk(const char * psz)
//   {
//      return dir().mk(psz);
//   }
//
//   string application::file_title(const char * psz)
//   {
//      return System.file().title_(psz);
//   }
//   string application::file_name(const char * psz)
//   {
//      return System.file().name_(psz);
//   }
//
//
//   ::file::file_sp application::file_get_file(var varFile,uint32_t uiFlags)
//   {
//
//      return file().get_file(varFile,uiFlags);
//
//   }

   //void application::dir_matter_ls_file(const string & str,stringa & stra)
   //{

   //   return dir().matter_ls_file(str,stra);

   //}

//#ifdef APPLEOS
//
//   string application::dir_pathfind(const char * pszEnv, const char * pszTopic, const char * pszMode)
//   {
//
//      return dir().pathfind(pszEnv, pszTopic, pszMode);
//
//   }
//
//#endif
//
//   string application::file_as_string(var varFile,var & varQuery)
//   {
//
//      return file().as_string(varFile);
//
//   }
//
//
//
//   bool application::file().exists(const char * pszPath)
//   {
//
//      return file().exists(pszPath) != FALSE;
//
//   }
//
//
//   bool application::file_is_equal_path(const char * pszPath1,const char * pszPath2)
//   {
//
//      return System.file().path().is_equal(pszPath1,pszPath2) != FALSE;
//
//   }
//
//
//   bool application::dir_is(const char * psz)
//   {
//
//      return dir().is(psz);
//
//   }


   //bool application::file_del(const char * psz)
   //{

   //   try
   //   {

   //      System.file().del(psz);

   //   }
   //   catch(...)
   //   {

   //      return false;

   //   }

   //   return true;

   //}

   //string application::file_extension(const char * pszPath)
   //{

   //   return file_extension_dup(pszPath);

   //}


   //string application::dir_userappdata(const char * lpcsz,const char * lpcsz2)
   //{

   //   return dir().userappdata(lpcsz,lpcsz2);

   //}

   //string application::dir_appdata(const char * lpcsz,const char * lpcsz2)
   //{

   //   return System.dir().appdata(lpcsz,lpcsz2);

   //}


   //string application::dir_simple_path(const string & str1,const string & str2)
   //{

   //   return System.dir().simple_path(str1,str2);

   //}


   //string application::dir_element(const char * psz)
   //{

   //   return System.dir().element(psz);

   //}


   //string application::dir_ca2module(const char * psz)
   //{

   //   return System.dir().ca2module(psz);

   //}


   //void application::dir_ls_dir(const char * lpcsz,::file::patha & patha)
   //{

   //   dir().ls_dir(lpcsz,ppatha,ppathaName);

   //}


   //void application::dir_rls(const char * lpcsz,::file::patha & patha)
   //{

   //   dir().rls(lpcsz,ppatha,ppathaName,ppathaRelative);

   //}




   //string application::file_time_square()
   //{

   //   return file().time_square();

   //}


   //string application::http_get_locale_schema(const char * pszUrl,const char * pszLocale,const char * pszSchema)
   //{

   //   return http().get_locale_schema(pszUrl,pszLocale,pszSchema);

   //}





} // namespace axis







//#include "framework.h" // from "axis/user/user.h"

namespace axis
{


   UINT application::APPM_LANGUAGE = WM_APP + 117;
   WPARAM application::WPARAM_LANGUAGE_UPDATE = 1;






   void application::process_message_filter(int32_t code,signal_details * pobj)
   {

      SCAST_PTR(::message::base,pbase,pobj);


   }







   void application::DoWaitCursor(int32_t nCode)
   {

      if(nCode < 0)
      {

         m_iWaitCursorCount = 0;
         ShowWaitCursor(false);

      }
      else if(nCode == 0)
      {

         if(m_iWaitCursorCount > 0)
         {
            m_iWaitCursorCount--;
         }

         if(m_iWaitCursorCount > 0)
         {

            ShowWaitCursor(true);

         }

         m_iWaitCursorCount = 0;
         ShowWaitCursor(false);

      }
      else
      {

         if(m_iWaitCursorCount < 0)
         {
            m_iWaitCursorCount = 0;
         }

         m_iWaitCursorCount++;

         ShowWaitCursor(true);

      }



   }

   //void application::ShowWaitCursor(bool bShow)
   //{

   //}



   void application::TermThread(HINSTANCE hInstTerm)
   {

      ::exception::throw_interface_only(this);

   }




   //string application::get_version()
   //{

   //   ::exception::throw_interface_only(this);

   //   return "";

   //}




   void application::set_env_var(const string & var,const string & value)
   {

      ::exception::throw_interface_only(this);

   }



   sp(::aura::printer) application::get_printer(const char * pszDeviceName)
   {

      ::exception::throw_interface_only(this);

      return NULL;

   }


   //bool application::update_module_paths()
   //{


   //   if(is_system())
   //   {

   //      if(!m_pimpl->update_module_paths())
   //         return false;


   //   }

   //   return true;


   //}










   //IDTHREAD application::get_thread_id()
   //{
   //   return  == NULL->get_thread_id();
   //}





//#ifndef METROWIN
//
//   void application::get_time(timeval *p)
//   {
//      m_pimpl->get_time(p);
//   }
//
//#endif


   bool application::do_prompt_file_name(var & varFile,UINT nIDSTitle,uint32_t lFlags,bool bOpenFileDialog,::user::impact_system * ptemplate,::user::document * pdocument)
   {

      UNREFERENCED_PARAMETER(varFile);
      UNREFERENCED_PARAMETER(nIDSTitle);

      return false;

   }






   //bool application::get_temp_file_name_template(string & strRet,const char * pszName,const char * pszExtension,const char * pszTemplate)
   //{

   //   throw not_implemented(this);

   //   return false;

   //}


   //bool application::get_temp_file_name(string & strRet,const char * pszName,const char * pszExtension)
   //{

   //   return get_temp_file_name_template(strRet,pszName,pszExtension,NULL);

   //}








   ::visual::icon * application::set_icon(object * pobject,::visual::icon * picon,bool bBigIcon)
   {

      return NULL;

   }


   ::visual::icon * application::get_icon(object * pobject,bool bBigIcon) const
   {

      return NULL;

   }



   //bool application::final_handle_exception(::exception::exception & e)
   //{
   //   UNREFERENCED_PARAMETER(e);
   //   //linux      exit(-1);

   //   if(!is_system())
   //   {

   //      // get_app() may be it self, it is ok...
   //      if(Sys(get_app()).final_handle_exception((::exception::exception &) e))
   //         return true;


   //   }

   //   return false;
   //}





   //int32_t application::main()
   //{

   //   TRACE(string(typeid(*this).name()) + " main");;

   //   dappy(string(typeid(*this).name()) + " : application::main 1");

   //   try
   //   {

   //      TRACE(string(typeid(*this).name()) + " on_run");;
   //      dappy(string(typeid(*this).name()) + " : going to on_run : " + ::str::from(m_iReturnCode));
   //      m_iReturnCode = 0;
   //      m_bReady = true;
   //      m_bRun = true;
   //      m_iReturnCode = on_run();
   //      if(m_iReturnCode != 0)
   //      {
   //         dappy(string(typeid(*this).name()) + " : on_run failure : " + ::str::from(m_iReturnCode));
   //         ::OutputDebugStringW(L"application::main on_run termination failure");
   //      }

   //   }
   //   catch(::exit_exception &)
   //   {

   //      dappy(string(typeid(*this).name()) + " : on_run exit_exception");

   //      ::aura::post_quit_thread(&System);

   //      goto exit_application;

   //   }
   //   catch(...)
   //   {

   //      dappy(string(typeid(*this).name()) + " : on_run general exception");

   //      goto exit_application;

   //   }

   //   try
   //   {

   //      if(is_system())
   //      {

   //         dappy(string(typeid(*this).name()) + " : quiting main");

   //         //::aura::post_quit_thread(&System);

   //         //Sleep(5000);

   //      }

   //   }
   //   catch(...)
   //   {

   //   }

   //exit_application:

   //   try
   //   {

   //      m_iReturnCode = exit();

   //   }
   //   catch(::exit_exception &)
   //   {

   //      ::aura::post_quit_thread(&System);

   //      m_iReturnCode = -1;

   //   }
   //   catch(...)
   //   {

   //      m_iReturnCode = -1;

   //   }

   //   return m_iReturnCode;

   //}




   //bool application::pre_run()
   //{

   //   TRACE(string(typeid(*this).name()) + " main_start");;
   //   try
   //   {

   //      m_dwAlive = ::get_tick_count();
   //      TRACE(string(typeid(*this).name()) + "application_pre_run");;
   //      int32_t m_iReturnCode = application_pre_run();
   //      if(m_iReturnCode != 0)
   //      {
   //         dappy(string(typeid(*this).name()) + " : applicationpre_run failure : " + ::str::from(m_iReturnCode));
   //         m_bReady = true;
   //         TRACE("application::main application_pre_run failure");
   //         return false;
   //      }

   //      xxdebug_box("pre_runnned","pre_runnned",MB_ICONINFORMATION);
   //      dappy(string(typeid(*this).name()) + " : pre_runned : " + ::str::from(m_iReturnCode));
   //      TRACE(string(typeid(*this).name()) + " initial_check_directrix");;
   //      if(!initial_check_directrix())
   //      {
   //         dappy(string(typeid(*this).name()) + " : initial_check_directrix failure");
   //         m_iReturnCode = -1;
   //         exit();
   //         m_bReady = true;
   //         ::OutputDebugStringW(L"exiting on check directrix");
   //         return false;
   //      }


   //      TRACE(string(typeid(*this).name()) + " os_native_bergedge_start");;
   //      m_dwAlive = ::get_tick_count();
   //      if(!os_native_bergedge_start())
   //      {
   //         dappy(string(typeid(*this).name()) + " : os_native_bergedge_start failure");
   //         exit();
   //         m_iReturnCode = -1;
   //         m_bReady = true;
   //         ::OutputDebugStringW(L"application::main os_native_bergedge_start failure");
   //         return false;
   //      }

   //      return true;
   //   }
   //   catch(::exit_exception &)
   //   {

   //      dappy(string(typeid(*this).name()) + " : main_start exit_exception");

   //      ::aura::post_quit_thread(&System);

   //   }

   //   return false;

   //}




   //int32_t application::on_run()
   //{
   //   int32_t m_iReturnCode = 0;

   //   try
   //   {
   //      ::aura::application_signal_details signal(this,m_psignal,::aura::application_signal_start);
   //      m_psignal->emit(&signal);
   //   }
   //   catch(...)
   //   {
   //   }

   //   dappy(string(typeid(*this).name()) + " : starting on_run : " + ::str::from(m_iReturnCode));

   //   thread * pthread = ::get_thread();

   //   install_message_handling(pthread->m_pthreadimpl);

   //   dappy(string(typeid(*this).name()) + " : starting on_run 2 : " + ::str::from(m_iReturnCode));

   //   try
   //   {
   //      try
   //      {
   //         m_bReady = true;
   //         if(m_peventReady != NULL)
   //            m_peventReady->SetEvent();
   //      }
   //      catch(...)
   //      {
   //      }
   //   run:
   //      try
   //      {
   //         m_iReturnCode = run();

   //      }
   //      catch(::exit_exception & e)
   //      {

   //         throw e;

   //      }
   //      catch(const ::exception::exception & e)
   //      {
   //         if(on_run_exception((::exception::exception &) e))
   //            goto run;
   //         if(final_handle_exception((::exception::exception &) e))
   //            goto run;
   //         try
   //         {
   //            m_iReturnCode = exit();
   //         }
   //         catch(::exit_exception & e)
   //         {

   //            throw e;

   //         }
   //         catch(...)
   //         {
   //            m_iReturnCode = -1;
   //         }
   //         goto InitFailure;
   //      }
   //   }
   //   catch(::exit_exception & e)
   //   {

   //      throw e;

   //   }
   //   catch(...)
   //   {
   //      // linux-like exit style on crash, differently from general windows error message approach
   //      // to prevent or correct from crash, should:
   //      // - look at dumps - to do;
   //      // - look at trace and log - always doing;
   //      // - look at debugger with the visual or other tool atashed - to doing;
   //      // - fill other possibilities;
   //      // - restart and send information in the holy back, and stateful or self-heal as feedback from below;
   //      // - ...
   //      // - ..
   //      // - .
   //      // - .
   //      // - .
   //      // - .
   //      // -  .
   //      // - ...
   //      // - ...
   //      // - ...
   //      // to pro-activia and overall benefits workaround:
   //      // - stateful applications:
   //      //      - browser urls, tabs, entire history, in the ca2computing cloud;
   //      //      - uint16_t - html document to simplify here - with all history of undo and redos per document optimized by cvs, svn, syllomatter;
   //      //           - not directly related but use date and title to name document;
   //      //      - save forms after every key press in .undo.redo.form file parallel to appmatter / form/undo.redo.file parity;
   //      //      - last ::ikaraoke::karaoke song and scoring, a little less motivated at time of writing;
   //      //
   //      // - ex-new-revolut-dynamic-news-self-healing
   //      //      - pre-history, antecendentes
   //      //            - sometimes we can't recover from the last state
   //      //            - to start from the beggining can be too heavy, waity, worky, bory(ing)
   //      //      - try to creativetily under-auto-domain with constrained-learning, or heuristcally recover from restart, shutdown, login, logoff;
   //      //           - reification :
   //      //           - if the document is corrupted, try to open the most of it
   //      //           - if can only detect that the document cannot be opened or damaged, should creatively workarounds as it comes, as could it be
   //      //              done, for example search in the web for a proper solution?
   //      //           - ::ikaraoke::karaoke file does not open? can open next? do it... may animate with a temporary icon...
   //      //               speed into cartesian dimensions of
   //      //               black letters, or colorful and pink are accepted and sometimes desired, for example, hello kity prodevian, pirarucu games,
   //      //               I think no one likes to be boring, but a entire background in black... I don't know... only for your personal office, may be...
   //      //           - could an online colaborator investigate crashes promptly in a funny way, and make news and jokes? Like terra and UOL for the real world?
   //      //               - new crash, two documents lost, weathers fault, too hot, can't think, my mother was angry with me, lead to buggy code;
   //      //               - new version with bug fixes;
   //      //      - new versions
   //      //      - automatic updates
   //      //      - upgrades
   //      //      - rearrangemntes
   //      //      - downgrade in the form of retro
   //      // - ...
   //      // - ..
   //      // - .
   //      // - .
   //      // - .
   //      // - .
   //      // -  .
   //      // - ...
   //      // - ...
   //      // - ...

   //   }
   //InitFailure:
   //   try
   //   {
   //      if(m_peventReady != NULL)
   //         m_peventReady->SetEvent();
   //   }
   //   catch(...)
   //   {
   //   }
   //   try
   //   {
   //      thread * pthread = this;
   //      if(pthread != NULL && pthread->m_pbReady != NULL)
   //      {
   //         *pthread->m_pbReady = true;
   //      }
   //   }
   //   catch(...)
   //   {
   //   }
   //   /*try
   //   {
   //   thread * pthread = dynamic_cast < thread * > (this);
   //   ::SetEvent((HANDLE) pthread->m_peventReady);
   //   }
   //   catch(...)
   //   {
   //   }*/

   //   // let translator run undefinetely
   //   /*if(is_system())
   //   {
   //   translator::detach();
   //   }*/

   //   return m_iReturnCode;
   //}




   //int32_t application::application_pre_run()
   //{

   //   return ::axis::application::application_pre_run();

   //}



   //bool application::InitApplication()
   //{

   //   return true;

   //}



   //bool application::initial_check_directrix()
   //{

   //   return axis::application::initial_check_directrix();

   //}


   //bool application::on_install()
   //{

   //   if(!axis::application::on_install())
   //      return false;



   //   return true;

   //}


   //bool application::on_uninstall()
   //{

   //   bool bOk = axis::application::on_uninstall();


   //   if(is_serviceable())
   //   {

   //      if(!remove_service())
   //         bOk = false;

   //   }

   //   return bOk;

   //}


   //bool application::system_add_app_install(const char * pszId)
   //{



   //   return axis::application::system_add_app_install(pszId);

   //}



   //bool application::os_native_bergedge_start()
   //{

   //   return axis::application::os_native_bergedge_start();

   //}











   //   void application::construct(const char *pszId)
   //   {
   //      //if(m_strAppName.has_char())
   //      //   return;
   //      //m_strAppName.Empty();
   //      //m_strId.Empty();
   //      if (pszId == NULL)
   //      {
   //#ifdef WINDOWSEX
   //         wstring wstr = ::GetCommandLineW();
   //         string str = ::str::international::unicode_to_utf8(wstr);
   //         strsize iFind = str.find(" : ");
   //         if (iFind >= 0)
   //         {
   //            iFind = str.find("app=", iFind);
   //            if (iFind >= 0)
   //            {
   //               strsize iEnd = str.find(" ", iFind);
   //               if (iEnd < 0)
   //               {
   //                  m_strId = str.Mid(iFind + 4);
   //               }
   //               else
   //               {
   //                  m_strId = str.Mid(iFind + 4, iEnd - iFind - 4);
   //               }
   //               ::str::begins_eat(m_strId, "\"");
   //               ::str::ends_eat(m_strId, "\"");
   //            }
   //         }
   //#endif
   //      }
   //      else
   //      {
   //         m_strId = pszId;
   //      }
   //      if (m_strId.is_empty())
   //         m_strId = "mplite";
   //      construct();
   //      if (m_strAppName.is_empty())
   //      {
   //         if (m_strAppId.has_char())
   //            m_strAppName = m_strAppId;
   //         else if (m_strInstallToken.has_char())
   //            m_strAppName = m_strInstallToken;
   //      }
   //   }


   //void application::construct()
   //{
   //   string strId = m_strId;
   //   char chFirst = '\0';
   //   if (strId.get_length() > 0)
   //   {
   //      chFirst = strId[0];
   //   }

   //   ::core::application::construct();

   //}




   //void application::_001OnFileNew()
   //{
   //   string strId = m_strId;
   //   char chFirst = '\0';
   //   if (strId.get_length() > 0)
   //   {
   //      chFirst = strId[0];
   //   }
   //   ::core::application::_001OnFileNew(NULL);
   //}


   //bool application::bergedge_start()
   //{
   //   string strId = m_strId;
   //   char chFirst = '\0';
   //   if (strId.get_length() > 0)
   //   {
   //      chFirst = strId[0];
   //   }
   //   return ::core::application::bergedge_start();
   //}



   //bool application::on_install()
   //{
   //   string strId = m_strId;
   //   char chFirst = '\0';
   //   if (strId.get_length() > 0)
   //   {
   //      chFirst = strId[0];
   //   }
   //   return ::core::application::on_install();
   //}

   //bool application::on_uninstall()
   //{
   //   string strId = m_strId;
   //   char chFirst = '\0';
   //   if (strId.get_length() > 0)
   //   {
   //      chFirst = strId[0];
   //   }
   //   return ::core::application::on_uninstall();
   //}


   //void application::on_request(::create * pcreate)
   //{

   //   ::object::on_request(pcreate);

   //   command()->consolidate(pcreate);

   //}


   //bool application::is_serviceable()
   //{


   //   string strId = m_strId;
   //   char chFirst = '\0';
   //   if (strId.get_length() > 0)
   //   {
   //      chFirst = strId[0];
   //   }
   //   return ::core::application::is_serviceable();
   //}

   //service_base * application::allocate_new_service()
   //{

   //   return NULL;

   //}


   //sp(::user::document) application::_001OpenDocumentFile(var varFile)
   //{
   //   string strId = m_strId;
   //   char chFirst = '\0';
   //   if (strId.get_length() > 0)
   //   {
   //      chFirst = strId[0];
   //   }
   //   return ::core::application::_001OpenDocumentFile(varFile);

   //}


   //int32_t application::run()
   //{

   //   return ::axis::application::run();


   //}











   void application::on_service_request(::create * pcreate)
   {

      if(!is_serviceable())
         return;

      if(pcreate->m_spCommandLine->m_varQuery.has_property("create_service"))
      {
         create_service();
      }
      else if(pcreate->m_spCommandLine->m_varQuery.has_property("start_service"))
      {
         start_service();
      }
      else if(pcreate->m_spCommandLine->m_varQuery.has_property("stop_service"))
      {
         stop_service();
      }
      else if(pcreate->m_spCommandLine->m_varQuery.has_property("remove_service"))
      {
         remove_service();
      }


   }




   string application::get_mutex_name_gen()
   {
      return m_strAppName;
   }



   //bool application::Ex2OnAppInstall()
   //{
   //   return true;
   //}

   //bool application::Ex2OnAppUninstall()
   //{
   //   return true;
   //}







   int32_t application::simple_message_box_timeout(::user::primitive * pwndOwner,const char * pszMessage,::duration durationTimeOut,UINT fuStyle)
   {
      UNREFERENCED_PARAMETER(durationTimeOut);
      return simple_message_box(pwndOwner,pszMessage,fuStyle);
   }


   //   void application::tellme_destroyed(::user::interaction * pui, bool * pDestroyed, bool bTell)
   //   {
   //
   //      if(pui == NULL)
   //         return;
   //
   //      try
   //      {
   //
   //#ifdef LINUX
   //
   //         synch_lock sl(get_ui_destroyed_mutex());
   //
   //         if(bTell)
   //         {
   //
   //            if(pui->m_bDestroying)
   //            {
   //
   //               *pDestroyed = true;
   //
   //            }
   //            else
   //            {
   //
   //               pui->m_bptraTellMeDestroyed.add(pDestroyed);
   //
   //            }
   //
   //         }
   //         else
   //         {
   //
   //            pui->m_bptraTellMeDestroyed.remove(pDestroyed);
   //
   //         }
   //
   //#endif
   //
   //      }
   //      catch(...)
   //      {
   //
   //      }
   //
   //   }


   string application::get_license_id()
   {

      if(m_strLicense.has_char())
         return m_strLicense;

      if(m_strInstallToken.has_char())
         return m_strInstallToken;

      return m_strAppId;

   }


   void application::on_create_keyboard()
   {

      Session.set_keyboard_layout(NULL,::action::source::database());

   }




   //bool application::assert_user_logged_in()
   //{

   //   return axis::application::assert_user_logged_in();

   //}








   ::user::primitive * application::window_from_os_data(void * pdata)
   {

      return NULL;

   }

   sp(::message::base) application::get_message_base(LPMESSAGE lpmsg)
   {



      ::thread * pthread = ::get_thread();

      if(pthread != NULL)
         return pthread->get_base(lpmsg->message,lpmsg->wParam,lpmsg->lParam);

      return get_base(lpmsg->message,lpmsg->wParam,lpmsg->lParam);

   }



   int32_t application::simple_message_box(::user::primitive * puiOwner,const char * pszMessage,UINT fuStyle)
   {

#if defined(WINDOWSEX)

      return ::simple_message_box(puiOwner->get_safe_handle(),pszMessage,m_strAppName,fuStyle);
      //return MessageBoxW(puiOwner->get_safe_handle(),wstring(pszMessage),wstring(m_strAppName),fuStyle);
      //return MessageBoxW((puiOwner == NULL ? NULL : (puiOwner->get_wnd() == NULL ? NULL : puiOwner->get_handle())),
      //   wstring(pszMessage), wstring(m_strAppName), fuStyle);

#elif  defined(LINUX) || defined(APPLEOS) || defined(ANDROID)

      return ::simple_message_box(puiOwner->get_safe_handle(),pszMessage,m_strAppName,fuStyle);
      //   return simple_message_box((puiOwner == NULL ? NULL : (puiOwner->get_wnd() == NULL ? NULL : puiOwner->get_handle())), pszMessage, m_strAppName, fuStyle);

#else

      return ::simple_message_box(puiOwner->get_safe_handle(),pszMessage,m_strAppName,fuStyle);

#endif

   }

   /*
   int32_t application::simple_message_box(sp(::user::interaction) puiOwner, const char * pszMessage, UINT fuStyle)
   {

   #if defined(WINDOWSEX)

   return MessageBoxW((puiOwner == NULL ? NULL : (puiOwner->get_wnd() == NULL ? NULL : puiOwner->get_handle())),
   wstring(pszMessage), wstring(m_strAppName), fuStyle);

   #elif  defined(LINUX) || defined(APPLEOS) || defined(ANDROID)

   return simple_message_box((puiOwner == NULL ? NULL : (puiOwner->get_wnd() == NULL ? NULL : puiOwner->get_handle())), pszMessage, m_strAppName, fuStyle);

   #else

   return simple_message_box(m_psystem->m_posdata->m_pui->get_handle(), pszMessage, m_strAppName, fuStyle);

   #endif

   }
   */

   int32_t application::simple_message_box(const char * pszMessage,UINT fuStyle)
   {

#if defined(WINDOWSEX)

      return ::simple_message_box(NULL,pszMessage,m_strAppName,fuStyle);
      //return MessageBoxW(NULL,wstring(pszMessage),wstring(m_strAppName),fuStyle);
      //return MessageBoxW((puiOwner == NULL ? NULL : (puiOwner->get_wnd() == NULL ? NULL : puiOwner->get_handle())),
      //   wstring(pszMessage), wstring(m_strAppName), fuStyle);

#elif  defined(LINUX) || defined(APPLEOS) || defined(ANDROID)

      return ::simple_message_box(NULL,pszMessage,m_strAppName,fuStyle);
      //   return simple_message_box((puiOwner == NULL ? NULL : (puiOwner->get_wnd() == NULL ? NULL : puiOwner->get_handle())), pszMessage, m_strAppName, fuStyle);

#else

      return ::simple_message_box(NULL,pszMessage,m_strAppName,fuStyle);

#endif

   }

   string application::message_box(const string & pszMatter,property_set & propertyset)
   {

      simple_message_box(pszMatter,0);

      return "";

   }


   bool application::set_keyboard_layout(const char * pszPath,::action::context actioncontext)
   {


         if(get_app()->m_pbasesession == NULL)
            return false;

         if(pszPath == NULL)
         {

            if(&Session.keyboard().on_layout() != NULL)
            {

               //            if(Session.fontopus()->m_puser != NULL
               //             && Session.fontopus()->m_puser->m_strFontopusServerSessId.has_char())
               //        {

               // xxx data_set("keyboard_layout", keyboard().on_layout().m_strPath);

               //      }

               return true;
            }

            string strCurrentSystemLayout = Session.keyboard().get_current_system_layout();

            if(strCurrentSystemLayout.is_empty())
               return false;

            if(!set_keyboard_layout(strCurrentSystemLayout,::action::source_database))
               return false;

            //         if(Session.fontopus()->m_puser != NULL
            //          && Session.fontopus()->m_puser->m_strFontopusServerSessId.has_char())
            //     {

            // xxx            data_set("keyboard_layout", keyboard().on_layout().m_strPath);

            //   }

            return true;
         }

         if(!Session.keyboard().load_layout(pszPath,actioncontext))
            return false;

         // xxx Application.simpledb().on_set_keyboard_layout(pszPath, actioncontext);

         return true;
      }





#ifdef HOTPLUGIN_SUBSYSTEM

   int32_t application::hotplugin_host_starter_start_sync(const char * pszCommandLine,::aura::application * papp,hotplugin::host * phost,hotplugin::plugin * pplugin)
   {

      {

         ::install::mutex mutex(process_platform_dir_name2());

         if (mutex.already_exists())
         {

//            simple_message_box("Could not launch spa installer. It is already running.", MB_OK);

            return -35;

         }

      }

      string strValue;

      if(get_command_line_param(strValue,pszCommandLine,"enable_desktop_launch"))
      {

#ifdef METROWIN

         return -1;

#else

         return ::call_sync(::path::app_app(process_platform_dir_name2()),pszCommandLine,::path::app_app(process_platform_dir_name2()),SW_SHOWNORMAL,840,84,NULL,0);

#endif

      }
      else
      {

         return hotplugin_host_host_starter_start_sync(pszCommandLine,get_app(),NULL);

      }

   }

   int32_t application::hotplugin_host_host_starter_start_sync(const char * pszCommandLine, ::aura::application * papp, hotplugin::host * phost, hotplugin::plugin * pplugin)
   {

      return -1;

   }


#endif



   bool application::BaseOnControlEvent(::user::control_event * pevent)
   {

      return false;

   }


   bool application::on_open_document(::user::document * pdocument,var varFile)
   {

      UNREFERENCED_PARAMETER(pdocument);
      UNREFERENCED_PARAMETER(varFile);
      return true;

   }


   bool application::on_save_document(::user::document * pdocument,var varFile)
   {

      UNREFERENCED_PARAMETER(pdocument);
      UNREFERENCED_PARAMETER(varFile);
      return true;

   }


   void application::on_update_view(::user::impact * pview,::user::impact * pviewSender,LPARAM lHint,object* pHint)
   {


   }


   bool application::keyboard_focus_is_focusable(::user::elemental * pue)
   {

      return false;

   }


   bool application::keyboard_focus_OnSetFocus(::user::elemental * pue)
   {

      return true;

   }


   string application::fontopus_get_cred(::aura::application * papp,const string & strRequestUrl, const RECT & rect, string & strUsername, string & strPassword, string strToken, string strTitle, bool bInteractive, ::user::interactive * pinteractive)
   {

      return ::fontopus::get_cred(papp, strRequestUrl, rect, strUsername, strPassword, strToken, strTitle, bInteractive, pinteractive);

   }

   string application::fontopus_get_cred(string & strUsername, string & strPassword, string strToken)
   {

      return ::fontopus::get_cred(this, strUsername, strPassword, strToken);

   }

   void application::set_cred(string strToken, const char * pszUsername, const char * pszPassword)
   {

      ::fontopus::set_cred(this,strToken, pszUsername, pszPassword);

   }

   void application::set_cred_ok(string strToken, bool bOk)
   {

      ::fontopus::set_cred_ok(this, strToken, bOk);

   }

   ::html::html * application::create_html()
   {

      return new ::html::html(m_pauraapp);

   }


   string application::http_get(const string & strUrl, ::property_set & set)
   {

      return http().get(strUrl, set);

   }

   bool application::app_data_set(class id id, ::file::istream & is)
   {

      return data_set(id, is);

   }


   bool application::app_data_get(class id id, ::file::ostream & os)
   {

      return data_get(id, os);

   }


   bool application::app_data_set(class id id, ::file::serializable & obj)
   {

      return data_set(id, obj);

   }


   bool application::app_data_get(class id id, ::file::serializable & obj)
   {

      return data_get(id, obj);

   }


   bool application::compress_ungz(::file::ostream & ostreamUncompressed, const ::file::path & lpcszGzFileCompressed)
   {

      return System.compress().ungz(this, ostreamUncompressed, lpcszGzFileCompressed);

   }

   
   bool application::compress_ungz(::primitive::memory_base & mem)
   {

      return System.compress().ungz(this, mem);

   }


   bool application::compress_gz(::file::file * pfileOut, const ::file::path & lpcszUncompressed, int iLevel)
   {

      return System.compress().gz(this, pfileOut, lpcszUncompressed, iLevel);

   }


   bool application::compress_gz(::file::file * pfileOut, ::file::file * pfileIn, int iLevel)
   {

      return System.compress().gz(this, pfileOut, pfileIn, iLevel);

   }


} // namespace axis







