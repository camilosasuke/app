#pragma once


#if defined(INSTALL_SUBSYSTEM)


int32_t spaboot_start(const char * pszVersion, const char * pszId);


namespace install
{

   enum e_check
   {
      
      check_before_auth,
      check_before_start

   };


   class CLASS_DECL_BASE plugin :
      virtual public ::hotplugin::plugin,
      virtual public ::fontopus::login_callback
   {
   public:


      class CLASS_DECL_BASE thread_start_ca2 :
         public thread
      {
      public:


         plugin * m_pplugin;

         thread_start_ca2(::aura::application * papp);

         //virtual bool on_run_step();

         virtual int32_t run() override;


      } ;

      thread_start_ca2     m_startca2;
      canvas               m_canvas;
      uint32_t             m_dwLastInstallingCheck;
      uint32_t             m_dwLastRestart;
      // logged in ? alarm comments
      bool                 m_bLogged;
      // in login process, login screen should be shown
      bool                 m_bLogin;
      bool                 m_bCa2Login;
      bool                 m_bCa2Logout;
      bool                 m_bNativeLaunch;
      bool                 m_bPluginTypeTested;
      bool                 m_bPluginDownloaded;
      bool                 m_bNativeLaunchFail;
      bool                 m_bRestartCa2;
      bool                 m_bPendingRestartCa2;
      bool                 m_bPendingStream;
      uint32_t             m_dwLastOk;

      bool                 m_bPluginShiftKey;

      string               m_strLoginRequestingServer;

      ::rect               m_rectSent;
      stringa              m_straLinesNativeLaunch;

      //bool                 m_bHasCred;
      //bool                 m_bHasCredEval;


      plugin(::aura::application * papp);
      virtual ~plugin();


      virtual void install_message_handling(::message::dispatch * pdispatch);



      virtual bool hist(const char * pszUrl);
      virtual void run_start_install(const char * pszType, const char * pszRun, const char * pszLocale, const char * pszSchema);


      virtual void start_ca2();

      virtual bool thread_start_ca2_on_idle();

      virtual bool plugin_initialize();

      virtual void on_paint(::draw2d::graphics * pgraphics, const RECT & lprect);

      virtual void on_prepare_memory();

      virtual void on_login_result(::fontopus::e_result eresult, const char * pszResponse);

      DECL_GEN_SIGNAL(_001OnLButtonUp);
      
      using ::hotplugin::plugin::message_handler;
      virtual void message_handler(signal_details * pobj);

      virtual void on_paint_progress(::draw2d::graphics * pgraphics, const RECT & lprect);

      virtual double extract_spa_progress_rate();

      virtual void deferred_prodevian_redraw();

#ifndef METROWIN

      virtual void on_post(::aura::ipc::rx * prx, int64_t a, int64_t b);

      using ::hotplugin::plugin::on_receive;

      virtual void on_receive(::aura::ipc::rx * prx, int32_t message, void * pdata, int32_t len);

#endif

      virtual bool native_launch();

      virtual bool is_installing();

      virtual void restart_aura_ipc();

      using ::hotplugin::plugin::SetWindowPos;
      virtual bool SetWindowPos(int_ptr z,int32_t x,int32_t y,int32_t cx,int32_t cy,UINT nFlags = SWP_SHOWWINDOW);

      virtual void on_ready();

      virtual string defer_get_plugin();

      using ::hotplugin::plugin::viewport_screen_to_client;
      virtual void viewport_screen_to_client(POINT * ppt);
      
      using ::hotplugin::plugin::viewport_client_to_screen;
      virtual void viewport_client_to_screen(POINT * ppt);

      virtual bool set_host(::hotplugin::host * phost);

      //sp(::user::interaction) GetFocus();

      //sp(::user::interaction) SetFocus();

      virtual void on_host_timer();

   };


} // namespace install



#endif



