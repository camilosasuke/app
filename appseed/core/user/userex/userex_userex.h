#pragma once


namespace userex
{


   class CLASS_DECL_CORE userex :
      virtual public ::aura::department,
      virtual public ::user::server,
      virtual public BaseMenuCentralContainer,
      virtual public ::database::client
   {
   public:



      ::user::impact_system *                   m_ptemplateForm;
      ::user::impact_system *                   m_ptemplateChildForm;
      ::user::impact_system *                   m_ptemplatePlaceHolder;
      ::user::impact_system *                   m_ptemplateFontSel;


      map < ::type *, type *, ::user::impact_system *, ::user::impact_system * > m_mapTemplate;


      sp(::user::shell::shell)                  m_pshell;


      stringa                                   m_straEscape;

      sp(::type)                                m_typeDefaultMeshData;
      sp(::type)                                m_typeDefaultListHeader;
      sp(::type)                                m_typeDefaultListData;




      userex(::aura::application * papp);
      virtual ~userex();


      ::user::shell::shell & shell();

      virtual bool initialize1();
      virtual bool initialize2();
      virtual bool initialize();



      string message_box(const char * pszMatter, property_set & propertyset);
      virtual int32_t simple_message_box_timeout(::user::primitive * puiOwner,const char * pszMessage,::duration durationTimeout,UINT fuStyle = MB_OK, ::aura::application * papp = NULL);
      virtual int32_t simple_message_box(::user::primitive * puiOwner,const char * pszMessage,UINT fuStyle = MB_OK);
      virtual int32_t track_popup_menu(const char * pszMatter, point pt, sp(::user::interaction) puie);

      virtual bool get_fs_size(string & strSize, const char * pszPath, bool & bPending);
      virtual bool get_fs_size(int64_t & i64Size, const char * pszPath, bool & bPending);

      virtual void data_on_after_change(signal_details * pobj);

      class keyboard & keyboard();


      virtual void SendMessageToWindows(UINT message, WPARAM wParam, LPARAM lParam);

      virtual void AddToRecentFileList(const char * lpszPathName);

      virtual bool finalize();

      virtual bool _001OnCmdMsg(::aura::cmd_msg * pcmdmsg);


      virtual void _001OnFileNew();



      ::user::list_header * default_create_list_header(::aura::application * papp);
      ::user::mesh_data * default_create_mesh_data(::aura::application * papp);
      ::user::list_data * default_create_list_data(::aura::application * papp);


      ::type * default_type_mesh_data();
      ::type * default_type_list_header();
      ::type * default_type_list_data();

      DECL_GEN_SIGNAL(VmsGuiiOnAppLanguage);

      virtual int32_t exit_application() override;

      void LoadStdProfileSettings(UINT nMaxMRU = ___MRU_COUNT);

      ::count get_template_count() const;
      sp(impact_system) get_template(index index) const;

      
      sp(::user::document)   create_form(::aura::application * papp, ::user::form_callback * pcallback, sp(::user::interaction) pwndParent, var var = ::var(::var::type_empty_argument));
      sp(::user::document)   create_form(::aura::application * papp, sp(::user::form) pview, ::user::form_callback * pcallback, sp(::user::interaction) pwndParent, var var = ::var(::var::type_empty_argument));
      sp(::user::document)   create_child_form(::aura::application * papp, ::user::form_callback * pcallback, sp(::user::interaction) pwndParent, var var = ::var(::var::type_empty_argument));
      sp(::user::document)   create_child_form(::aura::application * papp, sp(::type) pt, sp(::user::interaction) pwndParent, var var = ::var(::var::type_empty_argument));
      sp(::user::document)   create_child_form(::aura::application * papp, sp(::user::form) pview, ::user::form_callback * pcallback, sp(::user::interaction) pwndParent, var var = ::var(::var::type_empty_argument));
      ::user::document * hold(sp(::user::interaction) pui);

   };


   // CLASS_DECL_CORE::user::front_end_schema * GetUfeSchema(::aura::application * papp);
   ///CLASS_DECL_CORE::user::front_end * GetUfe(::aura::application * papp);


} // namespace core


