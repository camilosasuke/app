#pragma once


#ifdef HOTPLUGIN_SUBSYSTEM


#include "base/user/simple_ui/simple_ui_style.h"
#include "base/user/simple_ui/simple_ui_interaction.h"
#include "base/base/base/base_session.h"


namespace hotplugin
{


   class host;


   class CLASS_DECL_BASE plugin :
      virtual public ::simple_ui::style,
      virtual public ::simple_ui::interaction,
      virtual public ::base::session
//#ifndef METROWIN
      , virtual public ::aura::ipc::ipc
//#endif
   {
   public:


      bool                          m_bOnPaint;
      bool                          m_bInitialized;
      FILE *                        m_pfile;
      bool                          m_bPreCheck;
      int32_t                       m_iHealingSurface;
      uint32_t                      m_last_redraw;
      int32_t                       m_iEdge;
      bool                          m_bAppStarted;
      manual_reset_event            m_evReady;
      bool                          m_bApp;
      host *                        m_phost;
      bool                          m_bOk;
      bool                          m_bStream;

      void *                        m_pbitmap;
      void *                        m_pgraphics;
      local_memory_map              m_memBitmap;
      SIZE                          m_sizeBitmap;

      string                        m_strStatus;
      string                        m_strStatus2;

      IDTHREAD                      m_nCa2StarterStartThreadID;
      bool                          m_bReload;
      bool                          m_bInstalling;
      const char *                  m_pszReloadCommandLine;

      mutex *                       m_pmutexBitmap;
      SIZE                          m_sizeBitmapData;
      string                        m_strBitmapChannel;
      POINT                         m_ptCursorPhase;

      uint32_t                      m_dwSync;
      int32_t                       m_iDelta;


      plugin(::aura::application * papp);
      virtual ~plugin();


      virtual void install_message_handling(::message::dispatch * pdispatch);

      // host should implement
      virtual bool         open_link(const string & strLink,const string & pszTarget);
      virtual bool         reload_plugin();

      virtual void delete_this();

      // Host location is not the updated url - if there is a way to update the url and
      // maintain the same plugin instance, what would lead to an out-of-date location url.
      // It is the location url of the page that hosts the plugin when the plugin was created.
      virtual string     get_host_location_url();


      virtual void         redraw();
      virtual void         post_message(UINT uiMessage, WPARAM wparam, LPARAM lparam);
      virtual oswindow         get_host_window();

      using ::simple_ui::interaction::ClientToScreen;
      virtual bool ClientToScreen(POINT * ppt);

      using ::simple_ui::interaction::ScreenToClient;
      virtual bool ScreenToClient(POINT * ppt);

      using ::simple_ui::interaction::GetWindowRect;
      virtual bool GetWindowRect(RECT64 * prect);

      using ::simple_ui::interaction::GetClientRect;
      virtual bool GetClientRect(RECT64 * lprect);



      virtual void translate_mouse_message(int * px, int * py);

      //virtual bool ShowWindow(int nCmdShow);
      //virtual bool destroy_window();
      //
      // call host memory
      virtual void   set_memory(void * puchMemory, ::count c);
      virtual void   append_memory(void * puchMemory, ::count c);
      virtual ::count get_memory_length();
      virtual ::count read_memory(void * puchMemory, ::count c);
      virtual void   free_memory();
      //static_function  void   free_memory(byte ** ppuchMemory);


      virtual bool is_ok();
      virtual void set_ready();
      virtual void on_ready();

      virtual bool plugin_initialize();
      virtual bool plugin_finalize();

      virtual void start_plugin();

      virtual int32_t  start_ca2_system();

      virtual void on_paint(::draw2d::graphics * pgraphics,const RECT & lprect);

      virtual void deferred_prodevian_redraw();

      virtual void plugin_system_redraw();


#if !defined(WINDOWS) && !defined(APPLEOS) && !defined(ANDROID)
      using ::user::interaction::message_handler;
      virtual int32_t message_handler(XEvent * pevent);
#endif
      virtual void message_handler(signal_details * pobj);
      virtual void plugin_message_handler(UINT message, WPARAM wparam, LPARAM lparam, bool bEnsureTx);
      virtual void plugin_message_handler(MESSAGE * pmsg,bool bEnsureTx);


      virtual void on_bare_paint(::draw2d::graphics * pgraphics,const RECT & lprect);

      virtual void on_bare_paint_full_screen(::draw2d::graphics * pgraphics,const RECT & lprect);

      virtual void on_bare_paint_discreet(::draw2d::graphics * pgraphics,const RECT & lprect);

      virtual void on_bare_paint_veri_discreet(::draw2d::graphics * pgraphics,const RECT & lprect);

      virtual void start_ca2();

      virtual void set_progress_rate(double dRate);

      virtual double get_progress_rate();

      virtual void on_update_progress();

      virtual void on_paint_progress(::draw2d::graphics * pgraphics,const RECT & lprect);

      virtual double extract_spa_progress_rate();

      virtual void set_ca2_installation_ready(bool bReady = true);

      virtual void set_status(const char * pszStatus);

      virtual void restart_aura_ipc();

      virtual void ensure_bitmap_data(int32_t cx, int32_t cy, bool bCreateFile);

      virtual bool set_host(::hotplugin::host * phost);

      virtual void get_progress_color(BYTE & uchR, BYTE & uchG, BYTE & uchB, double dRate, int32_t iProfile);

      virtual void on_host_timer();

   };


} // namespace hotplugin


#endif



