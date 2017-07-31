#pragma once


namespace axis
{


   class CLASS_DECL_AXIS application :
      virtual public ::aura::application,
      virtual public ::database::client
   {
   public:

      bool m_bInitializeDataCentral;
      bool                                            m_bAxisProcessInitialize;
      bool                                            m_bAxisProcessInitializeResult;

      bool                                            m_bAxisInitializeInstance;
      bool                                            m_bAxisInitializeInstanceResult;

      bool                                            m_bAxisInitialize1;
      bool                                            m_bAxisInitialize1Result;

      bool                                            m_bAxisInitialize;
      bool                                            m_bAxisInitializeResult;

      string                                          m_strLicense;


      string                                          m_strDataIdAddUp;


      bool                                            m_bUpdateMatterOnInstall;


      int32_t                                         m_iWaitCursorCount;         // for wait cursor (>0 => waiting)



      ::simpledb::simpledb                            m_simpledb;






      static UINT                                     APPM_LANGUAGE;
      static WPARAM                                   WPARAM_LANGUAGE_UPDATE;

      ::user::interaction *                           m_pwndMain;




      application();
      virtual ~application();

      virtual bool app_data_get(class id id, ::file::ostream & os);
      virtual bool app_data_set(class id id, ::file::istream & is);

      virtual bool app_data_set(class id id, ::file::serializable & obj);
      virtual bool app_data_get(class id id, ::file::serializable & obj);


      virtual string fontopus_get_cred(::aura::application * papp, const string & strRequestUrl, const RECT & rect, string & strUsername, string & strPassword, string strToken, string strTitle, bool bInteractive, ::user::interactive * pinteractive) override;
      virtual string fontopus_get_cred(string & strUsername, string & strPassword, string strToken);
      virtual void set_cred(string strToken, const char * pszUsername, const char * pszPassword);
      virtual void set_cred_ok(string strToken, bool bOk);



      virtual string load_string(id id) override;
      virtual bool load_string(string & str, id id) override;
      virtual void load_string_table() override;
      virtual      bool load_cached_string(string & str, id id, bool bLoadStringTable) override;
      virtual   bool load_cached_string_by_id(string & str, id id, bool bLoadStringTable) override;
      virtual   void load_string_table(const string & pszApp, const string & pszId) override;



      virtual bool is_system() override;
      virtual bool is_session() override;
      virtual bool is_serviceable() override;








      inline class ::simpledb::simpledb         & simpledb() { return m_simpledb; }
      inline ::database::server &               dataserver() { return *m_simpledb.get_data_server(); }




      virtual bool verb() override;




      virtual bool Ex2OnAppInstall() override;
      virtual bool Ex2OnAppUninstall() override;



      virtual bool initialize1() override;
      virtual bool initialize2() override;
      virtual bool initialize3() override;

      virtual bool initialize_application();
      virtual int32_t exit_application() override;

      virtual bool initialize() override;
      virtual bool finalize() override;



      virtual int32_t run() override;

      virtual int32_t main() override;
      virtual int32_t on_run() override;
      virtual int32_t application_pre_run() override;
      virtual bool initial_check_directrix() override;
      virtual bool os_native_bergedge_start() override;

      virtual bool InitApplication() override;

      virtual bool on_install() override;
      virtual bool on_uninstall() override;





      virtual bool update_appmatter(::sockets::socket_handler & handler, sp(::sockets::http_session) & psession, const ::file::path & pszRoot, const string & pszRelative);
      virtual bool update_appmatter(::sockets::socket_handler & handler, sp(::sockets::http_session) & psession, const ::file::path & pszRoot, const string & pszRelative, const string & strLocale, const string & strStyle);


      virtual void SetCurrentHandles() override;



      virtual bool get_temp_file_name_template(string & str, const char * pszName, const char * pszExtension, const char * pszTemplate) override;

      virtual bool get_temp_file_name(string & str, const char * pszName, const char * pszExtension) override;





      virtual void assert_valid() const override;
      virtual void dump(dump_context & dumpcontext) const override;

      virtual bool final_handle_exception(::exception::exception & e) override;




      virtual sp(::aura::application) assert_running(const char * pszAppId) override;

      virtual bool is_running() override;



      virtual void on_request(::create * pcreate) override;



      virtual bool assert_user_logged_in() override;

      virtual string matter_as_string(const char * pszMatter, const char * pszMatter2 = NULL);

      virtual bool file_is_read_only(const char * pszPath);






      virtual void on_create_keyboard();




      virtual int32_t simple_message_box(::user::primitive * puiOwner, const char * pszMessage, UINT fuStyle = MB_OK) override;
      virtual int32_t simple_message_box_timeout(::user::primitive * pwndOwner, const char * pszMessage, ::duration durationTimeOut, UINT fuStyle) override;
      int32_t simple_message_box(const char * pszMessage, UINT fuStyle);
      virtual string message_box(const string & pszMatter, property_set & propertyset) override;



      virtual bool do_prompt_file_name(var & varFile, UINT nIDSTitle, uint32_t lFlags, bool bOpenFileDialog, ::user::impact_system * ptemplate, ::user::document * pdocument);


      virtual void process_message_filter(int32_t code, signal_details * pobj) override;



      virtual void DoWaitCursor(int32_t nCode) override; // 0 => restore, 1=> begin, -1=> end

      virtual void _001CloseApplication() override;


      virtual string get_license_id() override;




      virtual void construct(const char * pszAppId) override;


      virtual bool process_initialize() override;





      virtual void TermThread(HINSTANCE hInstTerm) override;


      virtual void set_env_var(const string & var, const string & value) override;


      virtual sp(::aura::printer) get_printer(const char * pszDeviceName);


      virtual ::visual::icon * set_icon(object * pobject, ::visual::icon * picon, bool bBigIcon);

      virtual ::visual::icon * get_icon(object * pobject, bool bBigIcon) const;

      virtual void on_service_request(::create * pcreate) override;

      virtual string get_mutex_name_gen() override;

      //virtual void draw2d_factory_exchange();

      virtual sp(::message::base) get_message_base(LPMESSAGE lpmsg) override;
      virtual ::user::primitive * window_from_os_data(void * pdata);


      virtual bool set_keyboard_layout(const char * pszPath, ::action::context actioncontext);

#ifdef HOTPLUGIN_SUBSYSTEM

      virtual int32_t hotplugin_host_starter_start_sync(const char * pszCommandLine, ::aura::application * papp, hotplugin::host * phost, hotplugin::plugin * pplugin = NULL) override;
      virtual int32_t hotplugin_host_host_starter_start_sync(const char * pszCommandLine, ::aura::application * papp, hotplugin::host * phost, hotplugin::plugin * pplugin = NULL);

#endif

      virtual bool BaseOnControlEvent(::user::control_event * pevent);

      virtual void on_update_view(::user::impact * pview, ::user::impact * pviewSender, LPARAM lHint, object* pHint);

      virtual bool keyboard_focus_is_focusable(::user::elemental * pue);
      virtual bool keyboard_focus_OnSetFocus(::user::elemental * pue);


      virtual bool on_open_document(::user::document * pdocument, var varFile);
      virtual bool on_save_document(::user::document * pdocument, var varFile);




      virtual bool check_install() override;

      inline ::html::html * html() { return m_pauraapp->m_paurasystem->m_phtml; }

      virtual ::html::html * create_html();

      virtual string http_get(const string & strUrl, ::property_set & set) override;

      virtual bool compress_ungz(::file::ostream & ostreamUncompressed, const ::file::path & lpcszGzFileCompressed) override;
      virtual bool compress_ungz(::primitive::memory_base & mem);
      virtual bool compress_gz(::file::file * pfileOut, const ::file::path & lpcszUncompressed, int iLevel = 6);
      virtual bool compress_gz(::file::file * pfileOut, ::file::file * pfileIn, int iLevel = 6);
      


   };


} // namespace axis










