#pragma once

#define platform_parent session( )

namespace core
{


   class view;

   class session_docs;


   class CLASS_DECL_CORE session :
      virtual public ::core::application,
      virtual public ::base::session,
      virtual public ::filemanager::item_action
   {
   public:


      class run_application
      {
      public:
         run_application();
         index                   m_iEdge;
         string                  m_strApp;
         string                  m_strQuery;
         sp(::core::session)      m_pbergedgeParent;
         sp(::aura::application)   m_pauraapp;
         bool                    m_bMakeVisible;
         sp(::user::interaction) m_puiParent;
         property_set      m_setParameters;
      };



      class CLASS_DECL_CORE map :
         virtual public ::map < index, index, ::core::session *, ::core::session * >
      {
      };


      index                                              m_iEdge;

      ::filemanager::filemanager *                       m_pfilemanager;

      session_docs *                                     m_pdocs;
      //nature::database *                                 m_pdatabase;


      string_map < sp(::object) >                          m_mapUinteraction; // ::user::wndfrm::wndfrm
      
      sp(::object)                                       m_pobjectUserex;
      ::userex::userex *                                 m_puserex;
      
      ::visual::font_list_data *                         m_pfontlistdata;





      session(::aura::application * papp);
      virtual ~platform_parent;


      virtual int64_t add_ref()
      {
         return ::object::add_ref();
      }
      virtual int64_t dec_ref()
      {
         return ::object::dec_ref();
      }


      virtual void construct(const char * pszAppId);


      virtual void install_message_handling(::message::dispatch * pdispatch);

      virtual bool process_initialize_userex();
      virtual bool initialize1_userex();
      virtual bool initialize2();
      bool initialize_application() override;
      bool on_initial_update();
      virtual int32_t exit_application();

      bool InitializeLocalDataCentral();

      virtual bool bergedge_start();

      void _001OnFileNew();

      virtual bool _001OnCmdMsg(::aura::cmd_msg * pcmdmsg);

      virtual ::user::document * userex_on_request(::create * pcreate);

      void OnFileManagerOpenFile(::filemanager::data * pdata, ::fs::item_array & itema);

      void load_string_table();

      virtual bool open_file(::filemanager::data * pdata, ::fs::item_array & itema);

      void initialize_bergedge_application_interface();

      //virtual bool create_bergedge(::create * pcreate);

      virtual void use_font_sel();

      virtual void on_app_request_bergedge_callback(::aura::application * papp);


      virtual void on_frame_window_drop_files(::user::interaction * pui, ::file::patha & patha);


      // semantics defined by application
      virtual void request_create(::create * pcreate);
      // main loosely coupled semantics
      // varFile   : empty, one file path, many file paths, one file object, one or more file objects to be opened
      // varQuery  : more ellaborated requests for the application - syntax and semantic defined by requested application

      virtual sp(::user::interaction) get_request_parent_ui(sp(::user::interaction) pinteraction, ::create * pcreate);

      virtual bool place(::user::main_frame * pmainframe, ::create * pcreate);

      virtual void request_topic_file(var & varQuery);

      virtual void request_topic_file();

      virtual void check_topic_file_change();

      inline ::filemanager::filemanager            & filemanager() { return *m_pfilemanager; }

      string filemanager_get_initial_browse_path();
      bool on_exclusive_instance_conflict(EExclusiveInstance eexclusive);

      void launch_app(const char * psz);
      void install_app(const char * psz);

      
      
      sp(::user::document)                  get_document();
      sp(::user::impact)                      get_view();
      sp(::user::document)                  get_platform();
      sp(::user::document)                    get_nature();
      
      
      inline ::userex::userex *               userex() { return m_puserex;  }
      

      virtual bool process_initialize();

      virtual bool initialize1();

      virtual bool initialize();

      virtual bool os_native_bergedge_start();

      virtual int32_t main();

      virtual bool on_uninstall();

      virtual bool is_serviceable();

      virtual service_base * allocate_new_service();

      void on_request(::create * pcreate);

      sp(::aura::application) get_current_application();

      virtual bool on_install();

      virtual void set_app_title(const char * pszAppId, const char * pszTitle);

      virtual sp(::core::session) get_session();

      virtual bool finalize();

      virtual bool open_by_file_extension(const char * pszPathName, application_bias * pbiasCreate = NULL);
      virtual bool open_by_file_extension(::create * pcc);

      virtual bool is_session();

      virtual bool is_mouse_button_pressed(::user::e_mouse emouse);

      virtual bool is_remote_session();

      virtual ::visual::cursor * get_cursor();
      virtual ::visual::cursor * get_default_cursor();

      virtual ::count   get_monitor_count();
      virtual bool      get_monitor_rect(index iMonitor,LPRECT lprect);


      virtual void register_bergedge_application(::aura::application * papp);
      virtual void unregister_bergedge_application(::aura::application * papp);

      virtual ::aura::application * application_get(const char * pszAppId, bool bCreate = true, bool bSynch = true, application_bias * pbiasCreate = NULL);

      virtual ::core::session *             query_bergedge();

      virtual void on_user_login(::fontopus::user * puser);


   };


} // namespace core



