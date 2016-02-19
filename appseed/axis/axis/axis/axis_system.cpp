#include "framework.h"
#include "framework.h" // from "axis/net/net_sockets.h"
#include "fiasco_finder.h"

#ifndef WINDOWS
#include "ft2build.h"
#include FT_FREETYPE_H
#endif


namespace axis
{



   system::system(::aura::application * papp):
      aura::system(this),
      m_httpsystem(this),
      m_visual(this),
      m_emaildepartment(this),
      m_libraryDraw2d(this)
   {
      g_pszCooperativeLevel = "axis";

      m_pDraw2dFactoryExchange = NULL;

      m_puserstr                 = NULL;

      m_purldepartment = new url::department(this);

      m_paxissystem = this;

      m_spinstall = canew(::install::install(this));

      __node_axis_factory_exchange(this);

      m_pcompress = new ::axis::compress;

      m_pcompress->set_app(this);

      factory().creatable_small < ::file::axis::application >(System.type_info < ::file::application >());
      factory().creatable_small < ::file::dir::axis::application >(System.type_info < ::file::dir::application >());

      //m_psimpleui       = NULL;


      set_app(this);

//      m_pbasesystem     = this;

      if(papp == NULL)
      {

         oprop("parent_system") = (sp(object)) NULL;

      }
      else
      {

         oprop("parent_system") = papp->m_paxissystem;

      }


      //__node_base_factory_exchange(this);

      m_bSystemSynchronizedCursor = true;

      m_bSystemSynchronizedScreen = true;

      ::draw2d::dib::static_initialize();

      //      draw2d_factory_exchange();




   }


   void system::construct(const char * pszAppId)
   {

      ::axis::application::construct(pszAppId);

   }


   system::~system()
   {

      ::aura::del(m_purldepartment);

      ::aura::del(m_pcompress);

   }

   string system::install_get_platform()
   {

      return install().get_platform();

   }

   void system::install_set_platform(const char * pszPlatform)
   {

      install().set_platform(pszPlatform);

   }

   string system::install_get_version()
   {

      return install().m_strVersion;

   }

   void system::install_set_version(const char * pszVersion)
   {

      install().m_strVersion = pszVersion;

   }


   string system::install_get_latest_build_number(const char * pszVersion)
   {

      return install().get_latest_build_number(pszVersion);

   }


   int32_t system::install_start(const char * pszCommandLine,const char * pszBuild)
   {

      return install().start(pszCommandLine,pszBuild);

   }


   int32_t system::install_progress_app_add_up(int32_t iAddUp)
   {

      UNREFERENCED_PARAMETER(iAddUp);

      return install().m_progressApp()++;

   }



   bool system::process_initialize()
   {

#ifndef WINDOWS

      int32_t error = FT_Init_FreeType((FT_Library *)&m_ftlibrary);
      if(error)
      {
         TRACE("an error occurred during Free Type library initialization");
         return false;
      }

#endif

      enum_display_monitors();

      if (m_peengine != NULL)
      {

         m_peengine = new ::exception::engine(this);

      }


      if(!::axis::application::process_initialize())
         return false;

      if(!::aura::system::process_initialize())
         return false;



      m_spos.alloc(allocer());

      m_spcrypto.alloc(allocer());

      if(!m_spcrypto.is_set())
         return false;




      return true;

   }


   bool system::initialize1()
   {

      if(!::aura::system::initialize1())
         return false;

      if(!::axis::application::initialize1())
         return false;

      Session.m_puserstrcontext->defer_ok(m_puserstr);




      return true;

   }


   bool system::initialize2()
   {

      if(!::aura::system::initialize2())
         return false;

      if(!::axis::application::initialize2())
         return false;


      return true;

   }

   bool system::verb() // ambigous inheritance from ::aura::system/::axis::application
   {
      
      return ::aura::system::verb();
      
   }

   bool system::initialize_instance()
   {

      if(!::aura::system::initialize_instance())
         return false;


      if(!::axis::application::initialize_instance())
         return false;



      return true;

   }


   bool system::finalize()
   {



      __wait_threading_count_except(this,::millis((5000) * 77));

      bool bOk = false;



      try
      {

         if(m_spcrypto.is_set())
         {

            m_spcrypto.release();

         }

      }
      catch(...)
      {

         bOk = false;

      }

      try
      {

         bOk = ::axis::application::finalize();

      }
      catch(...)
      {

         bOk = false;

      }


      try
      {

         if(m_spfile.is_set())
         {

            m_spfile.release();

         }

      }
      catch(...)
      {

         bOk = false;

      }


      return bOk;

   }


   int32_t system::exit_instance()
   {

      __wait_threading_count(::millis((5000) * 8));

      try
      {


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


         m_pmath.release();

         m_pgeometry.release();

      }
      catch(...)
      {
         m_iReturnCode = -86;
      }






      for(int i = 0; i < m_serviceptra.get_size(); i++)
      {
         try
         {
            m_serviceptra[i]->Stop(0);
         }
         catch(...)
         {
         }
      }


      for(int i = 0; i < m_serviceptra.get_size(); i++)
      {
         try
         {
            m_serviceptra[i]->Stop((5000) * 2);
         }
         catch(...)
         {
         }
      }

      m_serviceptra.remove_all();





      int32_t iRet = m_iReturnCode;


      try
      {

         iRet = ::axis::application::exit_instance();

      }
      catch(...)
      {

      }






      try
      {
         if(m_spos.is_set())
         {
            m_spos.release();
         }
      }
      catch(...)
      {
      }
      try
      {
         m_spdir.release();
      }
      catch(...)
      {
      }


      try
      {
         m_spos.release();
      }
      catch(...)
      {
      }
      try
      {
         m_spdir.release();
      }
      catch(...)
      {
      }

      try
      {
         if(m_pmachineeventcentral != NULL)
         {
            m_pmachineeventcentral->set_run(false);
         }
      }
      catch(...)
      {
      }



      m_plog.release();



      //{

      //   synch_lock sl(&m_mutexFactory);

      //   m_typemap.remove_all();

      //   m_typemap.release();

      //}



      //::axis::application::exit_instance();

#ifdef METROWIN
      m_pdevicecontext = nullptr;

//      m_pmutexDc.release();
#endif

#ifdef WINDOWSEX



#endif

      ::aura::del(m_peengine);

#ifdef METROWIN
      //      m_pdevicecontext = nullptr;

      //      m_pmutexDc.release();
#endif



      try
      {

         ::aura::system::exit_instance();

      }
      catch(...)
      {

      }

      try
      {

         if(m_libraryDraw2d.is_opened())
         {

            if(m_pDraw2dFactoryExchange != NULL)
            {

               delete m_pDraw2dFactoryExchange;

               m_pDraw2dFactoryExchange = NULL;

            }

            m_libraryDraw2d.close();

         }

      }
      catch(...)
      {

      }

      return iRet;

   }

   //sp(object) system::clone()
   //{
   //   // by the time, it is not possible to clone a system
   //   return NULL;
   //}



   //void system::discard_to_factory(sp(object) pca)
   //{

   //   if(m_pfactory == NULL)
   //      return;

   //   m_pfactory->discard(pca);

   //}



   //void system::wait_twf()
   //{

   //}

   bool system::is_system()
   {

      return true;

   }



   class ::crypto::crypto & system::crypto()
   {
      return *m_spcrypto;
   }





   ::fontopus::user_set & system::userset()
   {

      return m_userset;

   }



   //::axis::compress & system::compress()
   //{
   //   return m_compress;
   //}







   ::file::path system::dir_appmatter_locator(::aura::application * papp)
   {

      return dir().appmatter_locator(papp);

   }


   string system::crypto_md5_text(const string & str)
   {

      return crypto().md5(str);

   }


   uint32_t _thread_proc_start_system(void * p)
   {

      ::axis::system * psystem = (::axis::system *)p;

      return psystem->main();

   }

   CLASS_DECL_AXIS void __start_system(::axis::system * psystem)
   {

      ::create_thread(NULL,0,&_thread_proc_start_system,(LPVOID)psystem,0,0);

   }





   ::aura::session * system::on_create_session()
   {

      return new ::axis::session(this);

   }


   ::net::email_department & system::email()
   {

      return m_emaildepartment;

   }

   void system::on_request(sp(::create) pcreate)
   {

      ::aura::system::on_request(pcreate);

   }

} // namespace axis























//#include "framework.h" // from "axis/user/user.h"
//#include "base/user/user.h"

//void dappy(const char * psz);



namespace axis
{


   //system::system(::aura::application * papp):
   //   ::aura::system(this),
   //   ::axis::system(this),
   //   m_libraryDraw2d(this)
   //{

   //}


   //void system::construct(const char * pszAppId)
   //{

   //   ::base::application::construct(pszAppId);

   //}


   //system::~system()
   //{

   //}



   //bool system::process_initialize()
   //{




   //   enum_display_monitors();



   //   if(!::base::application::process_initialize())
   //      return false;


   //   if(!::axis::system::process_initialize())
   //      return false;


   //   //if(!m_pbasesession->begin_synch(&m_iReturnCode))
   //   //{
   //   //   return false;
   //   //}

   //   //dappy(string(typeid(*this).name()) + " : ::axis::session OK " + ::str::from(m_iReturnCode));




   //   return true;

   //}



   //bool system::initialize1()
   //{

   //   if(!::axis::system::initialize1())
   //      return false;

   //   if(!::base::application::initialize1())
   //      return false;


   //   return true;

   //}

   //bool system::initialize2()
   //{

   //   if(!::axis::system::initialize2())
   //      return false;

   //   if(!::base::application::initialize2())
   //      return false;


   //   return true;

   //}


   //bool system::initialize_instance()
   //{

   //   if(!::axis::system::initialize_instance())
   //      return false;

   //   if(!::base::application::initialize_instance())
   //      return false;

   //   return true;

   //}


   //bool system::finalize()
   //{

   //   __wait_threading_count_except(this,::millis((5000) * 77));



   //   bool bOk = false;


   //   try
   //   {

   //      bOk = ::base::application::finalize();

   //   }
   //   catch(...)
   //   {

   //      bOk = false;

   //   }

   //   try
   //   {

   //      bOk = ::axis::system::finalize();

   //   }
   //   catch(...)
   //   {

   //      bOk = false;

   //   }


   //   return bOk;

   //}


//   int32_t system::exit_instance()
//   {
//
//      __wait_threading_count(::millis((5000) * 8));
//
//      int32_t iRet = 0;
//
//
//
//      try
//      {
//
//         iRet = ::base::application::exit_instance();
//
//      }
//      catch(...)
//      {
//
//      }
//
//
//
//#ifdef METROWIN
//      //      m_pdevicecontext = nullptr;
//
//      //      m_pmutexDc.release();
//#endif
//

//
//      try
//      {
//
//         iRet = ::axis::system::exit_instance();
//
//      }
//      catch(...)
//      {
//
//      }
//
//
//
//      return iRet;
//
//   }


   //mutex * system::wait_twf()
   //{

   //   return &get_twf()->m_mutex;

   //}


   //bool system::verb()
   //{

   //   return ::axis::system::verb();

   //}



   //bool system::is_system()
   //{

   //   return true;

   //}









   ::user::document * system::place_hold(::user::interaction * pui)
   {


      //if(m_pcubeInterface != NULL)
      //{
      // return m_pcubeInterface->hold(pui);
      //}

      return NULL;

   }


   ::aura::session * system::query_session(index iEdge)
   {

      return NULL;

   }




   index system::get_main_monitor(LPRECT lprect)
   {

      int iMainMonitor = 0;

#ifdef WINDOWSEX

      HMONITOR hmonitorPrimary = GetPrimaryMonitorHandle();

      for(index iMonitor = 0; iMonitor < get_monitor_count(); iMonitor++)
      {

         if(m_hmonitora[iMonitor] == hmonitorPrimary)
         {

            iMainMonitor = iMonitor;

            break;

         }

      }


#endif

      if(lprect != NULL)
      {

         get_monitor_rect(iMainMonitor,lprect);

      }

      return iMainMonitor;

   }


   ::count system::get_monitor_count()
   {

#ifdef WINDOWSEX

      return m_monitorinfoa.get_count();

#elif defined(MACOS)

      return GetScreenCount();

#else

      return 1;

#endif

   }


   bool system::get_monitor_rect(index iMonitor,LPRECT lprect)
   {

#ifdef WINDOWSEX

      if(iMonitor < 0 || iMonitor >= get_monitor_count())
         return false;

      *lprect = m_monitorinfoa[iMonitor].rcMonitor;

#elif defined(METROWIN)


      return false;


#elif defined(LINUX)


      return false;


#elif defined(APPLEOS)

      if(iMonitor < 0 || iMonitor >= get_monitor_count())
         return false;

      GetScreenRect(lprect,iMonitor);

#else

      lprect->left = 0;
      lprect->top = 0;
      lprect->right = m_pandroidinitdata->m_iScreenWidth;
      lprect->bottom = m_pandroidinitdata->m_iScreenHeight;

#endif

      return true;

   }


   ::count system::get_desk_monitor_count()
   {

      return get_monitor_count();

   }


   bool system::get_desk_monitor_rect(index iMonitor,LPRECT lprect)
   {

      return get_monitor_rect(iMonitor,lprect);

   }




   index system::get_main_wkspace(LPRECT lprect)
   {

      int iMainWkspace = 0;

#ifdef WINDOWSEX

      HMONITOR hwkspacePrimary = GetPrimaryMonitorHandle();

      for(index iWkspace = 0; iWkspace < get_wkspace_count(); iWkspace++)
      {

         if(m_hmonitora[iWkspace] == hwkspacePrimary)
         {

            iMainWkspace = iWkspace;

            break;

         }

      }


#endif

      if(lprect != NULL)
      {

         get_wkspace_rect(iMainWkspace,lprect);

      }

      return iMainWkspace;

   }


   ::count system::get_wkspace_count()
   {

#ifdef WINDOWSEX

      return m_monitorinfoa.get_count();

#else

      return 1;

#endif

   }


   bool system::get_wkspace_rect(index iWkspace,LPRECT lprect)
   {

#ifdef WINDOWSEX

      if(iWkspace < 0 || iWkspace >= get_wkspace_count())
         return false;

      *lprect = m_monitorinfoa[iWkspace].rcWork;

#elif defined(METROWIN)

      return get_monitor_rect(iWkspace,lprect);

#elif defined(LINUX)

return false;

#elif defined(APPLEOS)

      if(iWkspace < 0 || iWkspace >= get_wkspace_count())
         return false;

      GetWkspaceRect(lprect,iWkspace);

      //      lprect->top += ::mac::get_system_main_menu_bar_height();
      //    lprect->bottom -= ::mac::get_system_dock_height();

#else

      //throw todo(get_app());

      //::GetWindowRect(::GetDesktopWindow(),lprect);

      get_monitor_rect(iWkspace, lprect);

#endif

      return true;

   }


   ::count system::get_desk_wkspace_count()
   {

      return get_wkspace_count();

   }


   bool system::get_desk_wkspace_rect(index iWkspace,LPRECT lprect)
   {

      return get_wkspace_rect(iWkspace,lprect);

   }




   /*

   string system::get_ca2_module_folder()
   {

   single_lock sl(m_pmutex,true);

   return m_strCa2ModuleFolder;

   }



   string system::get_module_folder()
   {

   return m_strModuleFolder;

   }


   string system::get_module_file_path()
   {

   #ifdef WINDOWSEX

   unichar lpszModuleFilePath[MAX_PATH + 1];

   GetModuleFileNameW(NULL,lpszModuleFilePath,MAX_PATH + 1);

   string strModuleFileName(lpszModuleFilePath);

   return strModuleFileName;

   #elif defined(METROWIN)

   return "m_app.exe";

   #else

   char * lpszModuleFilePath = br_find_exe_dir("app");

   if(lpszModuleFilePath == NULL)
   return "";

   string strModuleFileName(lpszModuleFilePath);

   free(lpszModuleFilePath);

   return strModuleFileName;

   #endif

   }


   string system::get_module_title()
   {

   return file_title(get_module_file_path());

   }


   string system::get_module_name()
   {

   return file_name(get_module_file_path());

   }


   */

   //string system::dir_appmatter_locator(::aura::application * papp)
   //{

   //   ::exception::throw_not_implemented(get_app());

   //   return "";
   //}


   //uint32_t _thread_proc_start_system(void * p)
   //{

   //   ::axis::system * psystem = (::axis::system *)p;

   //   return psystem->main();

   //}

   //CLASS_DECL_AXIS void __start_system(::axis::system * psystem)
   //{

   //   ::create_thread(NULL,0,&_thread_proc_start_system,(LPVOID)psystem,0,0);

   //}


#ifdef METROWIN



   CLASS_DECL_AXIS bool get_window_rect(::axis::system_window ^ pwindow,RECTD * lprect)
   {

      Windows::Foundation::Rect rect =  pwindow->get_window_rect();

      lprect->left = rect.X;
      lprect->top = rect.Y;
      lprect->right = lprect->left + rect.Width;
      lprect->bottom = lprect->top + rect.Height;

      return true;
   }


   CLASS_DECL_AXIS bool get_window_rect(::axis::system_window ^ pwindow,LPRECT lprect)
   {

      rectd r;

      if(!get_window_rect(pwindow,&r))
         return false;

      if(!::copy(lprect,r))
         return false;

      return true;

   }


#endif


   //::aura::session * system::on_create_session()
   //{

   //   return new ::base::session(this);

   //}

   void system::hist_hist(const char * psz)
   {
   }

   //void system::on_request(sp(::create) pcreate)
   //{

   //   ::axis::system::on_request(pcreate);

   //}

   ::user::interaction * system::ui_from_handle(void * posdata)
   {

      return NULL;

   }


} // namespace base













