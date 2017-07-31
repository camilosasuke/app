//#include "framework.h"
//#include "base/user/user.h"


#ifdef HOTPLUGIN_SUBSYSTEM


#undef new
#ifdef WINDOWS
#define min MIN
#define max MAX
//#include <gdiplus.h>
#undef min
#undef max
#endif

#if defined(LINUX) || defined(ANDROID) || defined(APPLEOS) || defined(SOLARIS)
int_ptr get_map_failed();
void my_munmap(void * pcolorref,HANDLE hfile);
void * my_open_map(const char * psz,HANDLE * pfile,bool bRead,bool bWrite,int64_t size);
#endif


namespace hotplugin
{


   host::host() :
      ::object(this),
      hotplugin::plugin(this),
      ::simple_ui::style(this),
      ::aura::session(this),
      ::axis::session(this),
      ::base::session(this),
      ::aura::ipc::base(this),
      ::aura::ipc::tx(this),
      ::aura::ipc::ipc(this)
   {

      m_pbasecomposer            = NULL;
      m_pplugin                  = NULL;
      m_dProgressRate            = 0.0;
      m_bShowProgress            = true;
      m_bCa2InstallationReady    = false;
      m_bStream                  = false;

      m_pbitmap                  = NULL;
      m_pmutexBitmap             = NULL;
      m_bHostStarterStart        = false;

   }


   host::~host()
   {

      free_memory();

   }


   bool host::open_link(const string & strLink,const string & strTarget)
   {

      if(m_pbasecomposer != NULL)
      {

         return m_pbasecomposer->open_link(strLink, strTarget);

      }

      return false;

   }


   bool host::reload_plugin()
   {

      return false;

   }


   string host::get_host_location_url()
   {

      if(m_pbasecomposer != NULL)
      {

         return m_pbasecomposer->m_strHostPluginLocation;

      }

      return "";

   }


   void host::redraw()
   {

   }


   void host::post_message(UINT uiMessage, WPARAM wparam, LPARAM lparam)
   {

   }


   oswindow host::get_host_window()
   {

      return NULL;

   }


   void host::set_memory(void * puchMemory, ::count c)
   {

      m_memory.assign(puchMemory, c);

   }


   void host::append_memory(void * puchMemory, ::count c)
   {

      m_memory.append(puchMemory, c);

   }


   ::count host::get_memory_length()
   {

      return m_memory.get_size();

   }


   ::count host::read_memory(void * puchMemory, ::count c)
   {

      if (natural(c) > m_memory.get_size())
         c = m_memory.get_size();

      memcpy_dup(puchMemory, m_memory.get_data(), c);

      return c;

   }


   void host::free_memory()
   {

      m_memory.allocate(0);

   }


   int32_t host::start_ca2_system()
   {

      if(m_pplugin != NULL)
      {

         return m_pplugin->start_ca2_system();

      }

      return -1;

   }


   void host::on_paint(::draw2d::graphics * pgraphics,const RECT & lprect)
   {

      if(m_pplugin != NULL)
      {

         try
         {
            //DWORD dwTime1 = ::get_tick_count();

            m_pplugin->on_paint(pgraphics, lprect);
            //DWORD dwTime9 = ::get_tick_count();

            //TRACE("plugin->on_paint %d",dwTime9 - dwTime1);

         }
         catch(...)
         {

         }

      }
      else
      {

         plugin::on_paint(pgraphics, lprect);

      }

      if(::user::interaction::m_pimpl != NULL && pgraphics != NULL)
      {
         //DWORD dwTime1 = ::get_tick_count();


         ::user::interaction::m_pimpl->_001Print(pgraphics);
         //DWORD dwTime9 = ::get_tick_count();

         //TRACE("m_pimpl->_001Print %d",dwTime9 - dwTime1);

      }
      //DWORD dwTime1 = ::get_tick_count();

      deferred_prodevian_redraw();
      //DWORD dwTime9 = ::get_tick_count();

      //TRACE("deferred_prodevian_redraw %d",dwTime9 - dwTime1);

   }


   void host::set_ready()
   {

      m_bOk = true;

      if(is_ok() && m_pplugin != NULL && m_pplugin->is_ok())
      {

         on_ready();

      }

   }


   void host::on_ready()
   {

      if(m_pplugin != NULL)
      {

         m_pplugin->on_ready();

      }

   }

   bool host::plugin_initialize()
   {

      if(m_pplugin != NULL)
      {

         return m_pplugin->plugin_initialize();

      }

      return true;

   }

   bool host::plugin_finalize()
   {

      if(m_pplugin != NULL)
      {

         plugin * pplugin = m_pplugin;

         m_pplugin = NULL;

         pplugin->plugin_finalize();

         return true;

      }

      return true;

   }

   void host::start_plugin()
   {

      {

         sp(mutex) pmutex = canew(mutex(get_thread_app(),"Global\\::ca::fontopus::ca2_spa::7807e510-5579-11dd-ae16-0800200c7784"));

         if(::GetLastError() == ERROR_ALREADY_EXISTS)
         {

            m_bRunningSpaAdmin = true;

            return;

         }

      }

      m_pplugin = new install::plugin(get_app());

      m_pplugin->set_host(this);

   }


   void host::start_ca2()
   {

      if(m_pplugin != NULL)
      {

         m_pplugin->start_ca2();

      }
      else
      {

         ::hotplugin::plugin::start_ca2();

      }

   }


   void host::set_progress_rate(double dRate)
   {

      m_dProgressRate = dRate;

   }


   double host::get_progress_rate()
   {

      if(m_dProgressRate < 0.0)
         return 0.0;
      else if(m_dProgressRate > 1.0)
         return 1.0;
      else
         return m_dProgressRate;

   }


   void host::set_ca2_installation_ready(bool bReady)
   {

      m_bCa2InstallationReady = bReady;

   }


   bool host::is_ca2_installation_ready()
   {

      return m_bCa2InstallationReady;

   }


   int32_t host::start_app_install(const char * pszCommandLine)
   {

      return start_app_install(pszCommandLine, get_app(), this);

   }


   int32_t host::start_app_install(const char * pszCommandLine, ::aura::application * papp, host * phost, plugin * pplugin)
   {

      if(phost->m_bHostStarterStart)
         return 0;

      phost->m_bHostStarterStart = true;

      ::install::starter_start * pstart    = new ::install::starter_start(papp);

      pstart->m_phost               = phost;

      pstart->m_pplugin             = pplugin;

      pstart->m_strCommandLine      = pszCommandLine;

      ::create_thread(NULL, 0, &::install::_ca2_starter_start, pstart, 0, pplugin == NULL ? NULL : &pplugin->m_nCa2StarterStartThreadID);

      return 0;

   }


   int32_t host::host_starter_start_sync(const char * pszCommandLine, ::aura::application * papp, host * phost, plugin * pplugin)
   {

      if(phost != NULL)
      {

         if(phost->m_bHostStarterStart)
            return 0;

         phost->m_bHostStarterStart = true;

      }

      ::install::starter_start * pstart = new ::install::starter_start(papp);

      pstart->m_phost               = phost;

      pstart->m_pplugin             = pplugin;

      pstart->m_strCommandLine      = pszCommandLine;

      ::install::_ca2_starter_start(pstart);

      return 0;

   }


   void host::deferred_prodevian_redraw()
   {

      if(m_pplugin != NULL)
      {

         m_pplugin->deferred_prodevian_redraw();

      }

   }


   void * host::get_system()
   {

      return this;

   }


   void host::set_status(const char * pszStatus)
   {

      ::hotplugin::plugin::set_status(pszStatus);

      if(m_pplugin != NULL)
      {

         m_pplugin->set_status(pszStatus);

      }

   }


   void host::set_bitmap(::draw2d::graphics * pgraphics,const RECT & lprect)
   {

      ensure_bitmap_data((int32_t)width(lprect), (int32_t)height(lprect), false);

      if(!m_memBitmap.is_mapped())
         return;

      synch_lock ml(m_pmutexBitmap);

      m_sizeBitmap.cx = abs(lprect.right - lprect.left);

      m_sizeBitmap.cy = abs(lprect.bottom - lprect.top);

      try
      {

         throw todo(get_app());

         //::draw2d::bitmap_sp b(allocer());

         //b->create_from_data(m_sizeBitmap.cx, m_sizeBitmap.cy, m_pcolorref, pgraphics);

         //::draw2d::graphics_sp g(allocer());

         //g->create_from_bitmap(b);

         //g.bit_blt(0, 0, m_sizeBitmap.cx, m_sizeBitmap.cy, pgraphics, lprect.left, lprect.top, SRCCOPY);

      }
      catch(...)
      {

      }

      return;

   }


   void host::paint_bitmap(::draw2d::graphics * pgraphics,const RECT & lprect)
   {

      ensure_bitmap_data((int32_t)width(lprect), (int32_t)height(lprect), false);

      if(!m_memBitmap.is_mapped())
         return;

      synch_lock ml(m_pmutexBitmap);

      m_sizeBitmap.cx = abs(lprect.right - lprect.left);

      m_sizeBitmap.cy = abs(lprect.bottom - lprect.top);

      try
      {

         throw todo(get_app());

         //simple_bitmap b;

         //b.create_from_data(m_sizeBitmap.cx, m_sizeBitmap.cy, m_pcolorref, pgraphics);

         //simple_graphics g;

         //g.create_from_bitmap(b);

         //pgraphics.bit_blt(lprect.left, lprect.top, m_sizeBitmap.cx, m_sizeBitmap.cy, g, 0, 0, SRCCOPY);

      }
      catch(...)
      {

      }

      return;

   }


   void host::blend_bitmap(::draw2d::graphics * pgraphics,const RECT & lprectOut)
   {

      ::rect rect;

      GetWindowRect(rect);

      m_sizeBitmap.cx = abs((int32_t)width(rect));

      m_sizeBitmap.cy = abs((int32_t)height(rect));

      ensure_bitmap_data(m_sizeBitmap.cx, m_sizeBitmap.cy, false);

      if(!m_memBitmap.is_mapped())
         return;

      synch_lock ml(m_pmutexBitmap);

      //throw todo(get_app());
      //pgraphics.blend_bitmap_data(lprect.left, lprect.top, m_sizeBitmap.cx, m_sizeBitmap.cy, m_pcolorref);

      if (m_dib.is_null())
         m_dib.alloc(allocer());

      if(!m_dib->create(m_sizeBitmap))
         return;

      m_dib->map();

      memcpy(m_dib->m_pcolorref, m_memBitmap.get_data(), (size_t) (m_dib->area() * sizeof(COLORREF)));

      pgraphics->BitBlt(point(lprectOut.left,lprectOut.top), size(m_sizeBitmap.cx, m_sizeBitmap.cy), m_dib->get_graphics(), null_point());

      //{

      //   pgraphics->FillSolidRect(100, 100, 100, 100,ARGB(255,50,255,180));

      //}

   }


   void host::translate_mouse_message(int * px, int * py)
   {

   }


   bool host::GetWindowRect(RECT64 * prect)
   {

      ::copy(prect, m_rectWindow);

      return true;

   }


   bool host::GetClientRect(RECT64 * prect)
   {

      ::copy(prect,m_rectClient);

      return true;

   }


   void host::message_handler(signal_details * pobj)
   {

      if(pobj == NULL)
         return;

      if(m_pplugin != NULL)
      {

         m_pplugin->message_handler(pobj);

      }

      ::simple_ui::interaction::message_handler(pobj);

   }

   
   void host::plugin_message_handler(UINT message,WPARAM wparam,LPARAM lparam, bool bEnsureTx)
   {

      if(m_pplugin != NULL)
      {

         m_pplugin->plugin_message_handler(message, wparam, lparam, bEnsureTx);

      }

   }


   void host::plugin_message_handler(MESSAGE * pmsg,bool bEnsureTx)
   {

      if(m_pplugin != NULL)
      {

         m_pplugin->plugin_message_handler(pmsg, bEnsureTx);

      }

   }



   bool host::hotplugin_host_begin()
   {

      try
      {

         if(!begin())
            return false;

      }
      catch(...)
      {

         return false;

      }


      return true;

   }


   bool host::hotplugin_host_is_initialized()
   {

      return m_bInitialized;

   }


   bool host::hotplugin_host_initialize()
   {

      m_bInitialized = true;

      return true;

   }


   void host::hotplugin_host_on_write()
   {


   }



   bool host::SetWindowPos(int_ptr z,int32_t x,int32_t y,int32_t cx,int32_t cy,UINT nFlags)
   {

      bool bOk = ::hotplugin::plugin::SetWindowPos(z,x,y,cx,cy,nFlags);

      for(index i = 0; i < m_uiptraChild.get_count(); i++)
      {

         m_uiptraChild[i]->on_layout();

      }

      try
      {

         if(m_pplugin != NULL)
         {

            m_pplugin->SetWindowPos(z,x,y,cx,cy,nFlags);

         }

      }
      catch(...)
      {
      }

      return bOk;

   }



} // namespace hotplugin


#endif







