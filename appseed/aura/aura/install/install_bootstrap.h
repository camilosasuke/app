#pragma once


#if defined(INSTALL_SUBSYSTEM)


namespace install
{


   class CLASS_DECL_AURA bootstrap :
      virtual public tool
   {
   public:



      bool                          m_NeedRestartBecauseOfReservedFile;
      bool                          m_NeedRestartFatalError;
      bool                          m_bLoginStartup;


      string                        m_strId;

      string                        m_strPlatform;

      string                        m_strSpaBootName;

      string_map < install_status > m_mapInstall;

      stringa                       m_straCommand;

      sp(::thread)                  m_pthreadInstall;

      bool                          m_bForceUpdatedBuild;
      bool                          m_bInstalling;
      DWORD                         m_dwInstallStartTime;
      bool                          m_bShowPercentage;

      stringa                       m_strSpa;
      stringa                       m_straHost;

      bool                          m_bDownloadingAdmin;


      bootstrap(::aura::application * papp);
      virtual ~bootstrap();


      virtual int install();
      virtual int app_app_x86();
      virtual int app_app_proper();


      bool is_user_service_running();

      //string get_configuration(string strConfiguration);

      string get_title(string strTitle);

      virtual int admin_main(string strPlatform);
      virtual int do_app_app(const char * psz, const char * pszParams = NULL);

      virtual bool check_app_app_installation(string strPlatform);

      virtual int check_user_service(string strPlatform, bool bLaunch, DWORD & dwGoodToCheckAgain);
      
#ifdef WINDOWS
      
      virtual int check_vcredist(string strPlatform);
      virtual int download_vcredist(string strPlatform);
      virtual string download_tmp_vcredist(string strPlatform);
      
#endif
      
      virtual int check_app_app_bin(string strPlatform);
      virtual int download_app_app_bin(string strPlatform);
      virtual string download_tmp_app_app_bin(string strPlatform);
      virtual int check_app_app_admin_bin(string strPlatform, bool bStartNok = false);
      virtual int download_app_app_admin_bin(string strPlatform);
      virtual string download_tmp_app_app_admin_bin(string strPlatform);

      virtual bool is_downloading_admin();
#ifdef WINDOWSEX
      virtual bool is_vcredist_installed(string strPlatform);
#endif

   };


} // namespace install


#endif



