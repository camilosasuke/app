#pragma once


#if defined(INSTALL_SUBSYSTEM)


namespace install
{


   class CLASS_DECL_AURA application :
      virtual public ::aura::application,
      virtual public ::aura::ipc::rx::receiver
   {
   public:


      enum e_message
      {
         message_none,
         message_ok,
         message_failed,
         message_unknown,
      };

      struct status
      {

         int         m_iCheck;
         //bool        m_bSlopeOk;
         bool        m_bOk;
         //DWORD       m_dwLastOk;

         status()
         {

            m_iCheck = 0;
            //m_bSlopeOk = false;
            m_bOk = false;

         }


         DWORD calc_when_is_good_to_check_again();

      };


      window *                         m_pwindow;
      bool                             m_bRunMainLoop;
      bool                             m_bAdmin;

      double                           m_dProgress;
      double                           m_dProgress1;
      double                           m_dProgress2;

      string                           m_strId;

      ::install::socket_thread *       m_pthreadSsl;

      string                           m_strConfiguration;
      bool                             m_bFinished;

      string                           m_strBuild;

      string_map < bootstrap * >       m_bootstrap;

      string                           m_strTraceLabel;
      string_map < status >            m_mapUpdated;
      string_map < status >            m_mapInstalled;

      DWORD                            m_dwGoodToCheckAgain;

      ::install::installer *           m_pinstaller;


      e_message                        m_emessage;
      HANDLE                           m_hmutexSpabootInstall;
      ::aura::ipc::rx                  m_rxchannel;

      bool                             m_bInstallerInstalling;
      sp(mutex)                        m_spmutexAppInstall;


      application();
      virtual ~application();


      virtual bool start_instance();

      virtual int32_t exit_application();

      virtual void on_request(::create * pcreate);


      void add_command_line(string str);
      string pick_command_line();


      string get_id();

      void set_id(const char * psz);


      virtual int app_app_main();
      virtual int start_app_app(string strPlatform);

      virtual bool get_admin();
      virtual void set_admin(bool bSet);

      virtual bool show_window();
      virtual bool hide_window();

      virtual void end_app_app();


      virtual bool is_user_service_running();

      //virtual string get_configuration(string strConfiguration);

      virtual string get_title(string strTitle);

      virtual string get_build();

      virtual string get_app_id(string wstr);
      virtual int check_soon_launch(string str, bool bLaunch, DWORD & dwGoodToCheckAgain);
      virtual int check_soon_file_launch(string wstr, bool bLaunch, DWORD & dwGoodToCheckAgain);
      virtual int check_soon_app_id(string wstr, bool bLaunch, DWORD & dwGoodToCheckAgain);
      virtual int check_soon_app_id1(string wstr, bool bLaunch, DWORD & dwGoodToCheckAgain);
      virtual int check_soon_app_id2(string wstr, bool bLaunch, DWORD & dwGoodToCheckAgain);

      virtual void trace(const string & str);
      virtual void trace(double dRate);

      virtual void start_web_server();

      virtual bool register_spa_file_type();
      virtual void defer_start_program_files_app_app_admin(string strPlatform);
      virtual void start_program_files_app_app_admin(string strPlatform);
      virtual void get_system_locale_schema(string & strLocale, string & strSchema);

      virtual void defer_show_debug_box();

      
      bool are_ca2_framework_shared_libraries_busy();

      

      virtual bool low_is_app_app_admin_running(string strPlatform);

      virtual bool is_application_installed(string strAppId, DWORD & dwGoodToCheckAgain);

      virtual bool is_application_updated(string strAppId, DWORD & dwGoodToCheckAgain);

      ::install::installer & installer() { return *m_pinstaller; }

      virtual void on_receive(::aura::ipc::rx * prx, const char * pszMessage);

      virtual bool initialize_app_install();


   };


} // namespace install



#endif





