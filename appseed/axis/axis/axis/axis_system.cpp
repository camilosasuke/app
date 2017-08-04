#include "framework.h"
#include "framework.h" // from "axis/net/net_sockets.h"
#include "fiasco_finder.h"

//#ifndef WINDOWS
//#include "ft2build.h"
//#include FT_FREETYPE_H
//#endif


namespace axis
{



   system::system(::aura::application * papp):
      aura::system(this, NULL),
//      m_httpsystem(this),
m_visual(this),

      m_emaildepartment(this)
   {

      m_paxissystem = this;


      g_pszCooperativeLevel = "axis";

      m_pDraw2dFactoryExchange = NULL;

      m_puserstr                 = NULL;


      __node_axis_factory_exchange(this);



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





   bool system::process_initialize()
   {

//#ifndef WINDOWS
//
//      int32_t error = FT_Init_FreeType((FT_Library *)&m_ftlibrary);
//      if(error)
//      {
//         TRACE("an error occurred during Free Type library initialization");
//         return false;
//      }
//
//#endif

      //enum_display_monitors();

      //if (m_peengine != NULL)
      //{

      //   m_peengine = new ::exception::engine(this);

      //}


      if(!::axis::application::process_initialize())
         return false;

      if(!::aura::system::process_initialize())
         return false;


      m_spos.alloc(allocer());





      return true;

   }


   bool system::initialize1()
   {

      if(!::aura::system::initialize1())
         return false;

      if(!::axis::application::initialize1())
         return false;


      m_visual.construct(this);

      if (!m_visual.initialize1())
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

   bool system::initialize_application()
   {

      if (!::aura::system::initialize_application())
      {

         return false;

      }

      if (!::axis::application::initialize_application())
      {

         return false;

      }

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
   bool system::initialize()
   {

      if (!::axis::application::initialize())
         return false;
      if (!::aura::system::initialize())
         return false;

      if (!m_visual.initialize())
         return false;





      return true;
   }

   int32_t system::exit_application()
   {

      try
      {
         
         for(auto & pair : System.m_appmap)
         {
            
            try
            {
               
               if(pair.m_element2->m_paxissystem == this)
               {
                  
                  pair.m_element2->m_paxissystem = NULL;
                  
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

      __wait_threading_count(::millis((5000) * 8));

      try
      {

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

         iRet = ::axis::application::exit_application();

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

            m_pmachineeventcentral->post_quit();

         }

      }
      catch(...)
      {

      }


      m_plog.release();

//#ifdef METROWIN
//
//      m_pdevicecontext = nullptr;
//
//#endif

      //::aura::del(m_peengine);

      try
      {

         ::aura::system::exit_application();

      }
      catch(...)
      {

      }

      try
      {

         if(m_mapLibrary["draw2d"]->is_opened())
         {

            if(m_pDraw2dFactoryExchange != NULL)
            {

               delete m_pDraw2dFactoryExchange;

               m_pDraw2dFactoryExchange = NULL;

            }

         }

      }
      catch(...)
      {

      }

      return iRet;

   }


   bool system::is_system()
   {

      return true;

   }


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

      return canew(::axis::session(this));

   }


   ::net::email_department & system::email()
   {

      return m_emaildepartment;

   }


   void system::on_request(::create * pcreate)
   {

      ::aura::system::on_request(pcreate);

   }


} // namespace axis























//#include "framework.h" // from "axis/user/user.h"

//void dappy(const char * psz);



namespace axis
{








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

      index iMainMonitor = 0;

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

      index iMainWkspace = 0;

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

      return get_monitor_count();

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

//#elif defined(LINUX)
//
//return false;
//
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



   void system::hist_hist(const char * psz)
   {
   }

   //void system::on_request(::create * pcreate)
   //{

   //   ::axis::system::on_request(pcreate);

   //}

   ::user::interaction_impl * system::impl_from_handle(void * posdata)
   {

      return NULL;

   }

   ::user::interaction * system::ui_from_handle(void * posdata)
   {

      return NULL;

   }


   string system::url_encode(const string & str)
   {

	   return m_purldepartment->url_encode(str);

   }


   void system::enum_display_monitors()
   {

#ifdef WINDOWSEX

      m_monitorinfoa.remove_all();

      ::EnumDisplayMonitors(NULL,NULL,&system::monitor_enum_proc,(LPARAM)(dynamic_cast < ::axis::system * > (this)));

#else

      // todo
      //      m_monitorinfoa.remove_all();


#endif

   }



#ifdef WINDOWSEX
   BOOL CALLBACK system::monitor_enum_proc(HMONITOR hmonitor,HDC hdcMonitor,LPRECT lprcMonitor,LPARAM dwData)
   {

      ::axis::system * psystem = (::axis::system *) dwData;

      psystem->monitor_enum(hmonitor,hdcMonitor,lprcMonitor);

      return TRUE; // to enumerate all

   }

   void system::monitor_enum(HMONITOR hmonitor,HDC hdcMonitor,LPRECT lprcMonitor)
   {

      UNREFERENCED_PARAMETER(hdcMonitor);
      UNREFERENCED_PARAMETER(lprcMonitor);

      m_monitorinfoa.allocate(m_monitorinfoa.get_size() + 1);

      ZERO(m_monitorinfoa.last());

      m_hmonitora.add(hmonitor);

      m_monitorinfoa.last().cbSize = sizeof(MONITORINFO);

      ::GetMonitorInfo(hmonitor,&m_monitorinfoa.last());

      MONITORINFO mi = m_monitorinfoa.last();

      TRACE0("session::monitor_enum\n");
      TRACE("upper_bound %d\n",m_monitorinfoa.get_upper_bound());
      TRACE("rcMonitor(left, top, right, bottom) %d, %d, %d, %d\n",mi.rcMonitor.left,mi.rcMonitor.top,mi.rcMonitor.right,mi.rcMonitor.bottom);
      TRACE("rcWork(left, top, right, bottom) %d, %d, %d, %d\n",mi.rcWork.left,mi.rcWork.top,mi.rcWork.right,mi.rcWork.bottom);

   }


#endif

} // namespace axis













