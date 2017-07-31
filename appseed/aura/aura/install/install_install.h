#pragma once


#if defined(INSTALL_SUBSYSTEM)


namespace install
{


   class CLASS_DECL_AURA install :
      virtual public ::aura::department
   {
   public:


      bool                                m_bAdmin;

      string                              m_strId;

      bool                                m_bCa2Installed;

      class trace                         m_trace;

      ::sockets::socket_handler *         m_psockethandler;
      spa(::sockets::http_session)        m_httpsessionptra;


      install(::aura::application * papp);
      virtual ~install();


      inline class trace & trace() { return m_trace; }

      virtual bool is_file_ok(const ::file::path & path1,const char * pszTemplate,const char * pszFormatBuild);
      virtual bool is_file_ok(const ::file::patha & patha,const ::file::patha & straTemplate,stringa & straMd5,int_array & iaLen, int iRetry = 1);
      virtual bool reference_is_file_ok(const ::file::path & path1,const char * pszTemplate,const char * pszConfiguration,const char * pszFormatBuild);

      virtual int32_t synch_install(const char * pszCommand);
      virtual int32_t asynch_install(const char * pszCommand);

      virtual bool is_admin();
      virtual void set_admin(bool bSet);
      virtual string get_id();
      virtual void set_id(const char * psz);

      int32_t spalib_main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32_t nCmdShow);

      int32_t spaboot_registry_register(const char * lpszFile);

      int32_t start_app(const char * id);

      virtual void update_ca2_installed(bool bUnloadIfNotInstalled = true);
      virtual bool is_ca2_installed();

      virtual bool is_installing_ca2();

      virtual string app_install_get_extern_executable_path(stringa * pstraMd5 = NULL, int_array * piaLen = NULL, string_to_string * pmapMd5 = NULL, string_to_intptr * pmapLen = NULL);
      virtual string app_install_get_intern_executable_path();
      virtual void app_install_call_sync(const char * pszCommand);
      virtual bool app_install_send_short_message(const char * psz, bool bLaunch);
      virtual void app_install_send_response(const char * param);

      virtual void add_spa_start(const char * pszAppId);
      virtual void remove_spa_start(const char * pszAppId);
      virtual void add_app_install(const char * pszAppId, const char * pszBuild, const char * pszLocale, const char * pszSchema);
      virtual int32_t start(const char * pszCommand);
      virtual int32_t synch(const char * pszCommand);


   };


} // namespace spa


#endif





