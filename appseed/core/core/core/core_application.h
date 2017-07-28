#pragma once

namespace userex {
   class pane_tab_view;
}
namespace core
{

   enum e_end
   {

      end_close,
      end_app,
      end_session,
      end_system

   };

   class CLASS_DECL_CORE application:
      virtual public ::base::application,
//      virtual public ::html_lite::application,
      virtual public ::filemanager::callback
   {
   public:


//      ::calculator::calculator *             m_pcalculator;

      ::userex::pane_tab_view *              m_pmainpane;

      sp(::object)                           m_spobjectUserFs;

      ::userfs::userfs *                     m_puserfs;

      class signal                           m_signalAppLanguageChange;
      string                                 m_strHelpFilePath;

#ifdef WINDOWS


      HGLOBAL                                m_hDevMode;             // printer Dev Mode
      HGLOBAL                                m_hDevNames;            // printer Device Names


#endif

      uint32_t                               m_dwPromptContext;        // help context override for message box
      // LKG
      uint32_t                               m_dwPolicies;            // block for storing boolean system policies


      // Pointer to ::user::document_manager used to manage document templates
      // for this application instance.
      sp(::object)                             m_pdocmanager;

      // Support for Shift+F1 help mode.
      // TRUE if we're in SHIFT+F1 mode.
      bool                                   m_bHelpMode;

      // set in constructor to override default


      // Default based on this application's name.
      string                                 m_strProfileName;
      // help mode used by the cast
      //      __HELP_TYPE m_eHelpType;

      ATOM m_atomApp,m_atomSystemTopic;   // for DDE open
      UINT m_nNumPreviewPages;        // number of default printed pages

      string                                 m_strId;



      int32_t                                m_iResourceId;

      sp(::object)                           m_pwndfrm;


      stringa                                m_straAppInterest;
      string_map < oswindow,oswindow >       m_mapAppInterest;

      int32_t                                m_iGcomBackgroundUpdateMillis;


      application();
      virtual ~application();



      virtual void construct(const char * pszId);

      virtual bool process_initialize();

      virtual bool initialize1(); // first initialization
      virtual bool initialize2(); // second initialization
      virtual bool initialize3(); // third initialization and so on...

      virtual bool initialize(); // last initialization

      virtual bool bergedge_start();
      virtual bool os_native_bergedge_start();

      virtual int32_t exit_application() override;

      virtual bool finalize();

      virtual int32_t main();

      virtual application * get_app() const;

      virtual bool is_system();
      virtual bool is_session();

      virtual bool is_installing();
      virtual bool is_uninstalling();

      virtual bool is_serviceable();



      virtual void pre_translate_message(signal_details * pobj);


      virtual void install_message_handling(::message::dispatch * pdispatch);

      //virtual int32_t run();


      virtual void EnableShellOpen();




      virtual void _001CloseApplication();



      virtual void on_create_split_view(::user::split_view * pview);

      //virtual bool update_module_paths();


//      inline ::calculator::calculator           & calculator() { return *m_pcalculator; }




      virtual bool base_support();

      virtual string message_box(const string & pszMatter,property_set & propertyset);


      virtual sp(::user::interaction) uie_from_point(point pt);



      virtual bool on_install();
      virtual bool on_uninstall();
      virtual bool on_run_install();
      virtual bool on_run_uninstall();

      DECL_GEN_SIGNAL(on_application_signal);


      // open named file, trying to match a regsitered
      // document template to it.
      virtual void on_request(::create * pcreate) override;

      // overrides for implementation
      virtual bool on_idle(LONG lCount); // return TRUE if more idle processing
      virtual void process_window_procedure_exception(::exception::base* e,signal_details * pobj);


      void EnableModelessEx(bool bEnable);
#ifdef WINDOWS
      HENHMETAFILE LoadEnhMetaFile(UINT uiResource);
#endif
      bool GetResourceData(UINT nID,const char * lcszType,memory & storage);

#ifdef WINDOWS
      virtual bool OnMessageWindowMessage(LPMESSAGE lpmsg);
#elif defined(LINUX)
      virtual bool OnMessageWindowMessage(XEvent * pev);
#endif

      bool CreateFileFromRawResource(UINT nID,const char * lcszType,const char * lpcszFilePath);
      virtual LRESULT GetPaintMsgProc(int32_t nCode,WPARAM wParam,LPARAM lParam);


      void OnUpdateRecentFileMenu(cmd_ui * pcmdui);

      virtual DECL_GEN_SIGNAL(OnAppLanguage);
      virtual bool _001OnCmdMsg(::aura::cmd_msg * pcmdmsg);




      void EnableHtmlHelp();


      virtual int32_t simple_message_box_timeout(::user::primitive * puiOwner,const char * pszMessage,::duration durationTimeout,UINT fuStyle = MB_OK);
      virtual int32_t simple_message_box(::user::primitive * puiOwner,const char * pszMessage,UINT fuStyle = MB_OK);



      virtual bool on_run_exception(::exception::exception &);


      // set regsitry key name to be used by application's
      // profile member functions; prevents writing to an INI spfile->
      void SetRegistryKey(const char * lpszRegistryKey);
      void SetRegistryKey(UINT nIDRegistryKey);


      void RegisterShellFileTypes(bool bCompat = FALSE);

      // call after all doc templates are registered
      void UnregisterShellFileTypes();



      // Printer DC Setup routine, 'struct tagPD' is a PRINTDLG structure.
      void SelectPrinter(HANDLE hDevNames,HANDLE hDevMode,bool bFreeOld = TRUE);

      // create a DC for the system default printer.
      ::draw2d::graphics * CreatePrinterDC();


      //   bool GetPrinterDeviceDefaults(PRINTDLG* pPrintDlg);

      // run this cast as an embedded object.
      bool RunEmbedded();

      // run this cast as an OLE automation server.
      bool RunAutomated();

      // Parse the command line for stock options and commands.
      //   void ParseCommandLine(CCommandLineInfo& rCmdInfo);

      // React to a shell-issued command line directive.
      //   bool ProcessShellCommand(CCommandLineInfo& rCmdInfo);

      // Overridables


      // exiting
      virtual bool save_all_modified(); // save before exit
      void HideApplication();
      void close(e_end eend); // close documents before exiting

      // Advanced: to override message boxes and other hooks
      virtual int32_t DoMessageBox(const char * lpszPrompt,UINT nType,UINT nIDPrompt);

      // Advanced: process async DDE request
      virtual bool OnDDECommand(LPTSTR lpszCommand);

#ifdef WINDOWSEX
      // Advanced: Help support
      virtual void WinHelp(uint_ptr dwData,UINT nCmd = HELP_CONTEXT);
      virtual void HtmlHelp(uint_ptr dwData,UINT nCmd = 0x000F);
      virtual void WinHelpInternal(uint_ptr dwData,UINT nCmd = HELP_CONTEXT);
#endif

      // Command Handlers
      // map to the following for file new/open
      void _001OnFileNew();
      void on_file_open();

      // map to the following to enable print setup
      void OnFilePrintSetup();

      // map to the following to enable help
      void OnContextHelp();   // shift-F1
      void OnHelp();          // F1 (uses current context)
      void OnHelpIndex();     // ID_HELP_INDEX
      void OnHelpFinder();    // ID_HELP_FINDER, ID_DEFAULT_HELP
      void OnHelpUsing();     // ID_HELP_USING

      // Implementation

      void UpdatePrinterSelection(bool bForceDefaults);
      void SaveStdProfileSettings();  // save options to .INI file



      void DevModeChange(LPTSTR lpDeviceName);

      // Finds number of opened document items owned by templates
      // registered with the doc manager.
      int32_t get_open_document_count();

      bool do_prompt_file_name(var & varFile,UINT nIDSTitle,uint32_t lFlags,bool bOpenFileDialog, ::user::impact_system * ptemplate,::user::document * pdocument);

      void EnableModeless(bool bEnable); // to disable OLE in-place dialogs


      // Helper for message boxes; can work when no application can be found
      static int32_t ShowAppMessageBox(sp(application)pApp,const char * lpszPrompt,UINT nType,UINT nIDPrompt);
      static void DoEnableModeless(bool bEnable); // to disable OLE in-place dialogs

#ifdef WINDOWSEX
      // helpers for registration
      HKEY GetSectionKey(const char * lpszSection);
      HKEY GetAppRegistryKey();
#endif

      void OnAppExit();
      // System Policy Settings
      virtual bool LoadSysPolicies(); // Override to load policies other than the system policies that core API loads.
      bool GetSysPolicyValue(uint32_t dwPolicyID,bool *pbValue); // returns the policy's setting in the out parameter
      bool _LoadSysPolicies() throw(); // Implementation helper
      static const char gen_FileSection[];
      static const char gen_FileEntry[];
      static const char gen_PreviewSection[];
      static const char gen_PreviewEntry[];








      virtual bool does_launch_window_on_startup();
      virtual bool activate_app();

      // Hooks for your initialization code
      virtual bool InitApplication();



      virtual bool initialize_application();




      virtual bool final_handle_exception(::exception::exception &);





      virtual sp(::user::interaction) get_request_parent_ui(sp(::user::interaction) pinteraction, ::create * pcreate);




      //      virtual ::core::file_system & file_system();
      virtual bool _001OnDDECommand(const char * lpcsz);
      virtual ::user::document * _001OpenDocumentFile(var varFile);
      DECL_GEN_SIGNAL(_001OnFileNew);


      //virtual string get_version();


      virtual ::window_sp get_desktop_window();

      virtual int32_t run();

      sp(::aura::application) get_system();

      virtual bool set_keyboard_layout(const char * pszPath,::action::context actioncontext);

      ::user::wndfrm::wndfrm          & wndfrm();
      ::user::document_manager          & document_manager();


      string message_box(const char * pszMatter,property_set & propertyset);

      virtual int32_t track_popup_menu(const char * pszMatter,point pt,sp(::user::interaction) puie);

      virtual bool get_fs_size(string & strSize,const char * pszPath,bool & bPending);
      virtual bool get_fs_size(int64_t & i64Size,const char * pszPath,bool & bPending);

      virtual void set_title(const char * pszTitle);


      virtual bool _001CloseApplicationByUser(sp(::user::interaction) pwndExcept);


#ifdef WINDOWSEX

      static BOOL CALLBACK GetAppsEnumWindowsProc(oswindow oswindow,LPARAM lParam);

#endif

      void update_app_interest();
      void ensure_app_interest();


      virtual oswindow get_ca2_app_wnd(const char * psz);


      //virtual void request_create(::create * pcreate);

      //      virtual void on_exclusive_instance_local_conflict();

      virtual int32_t send_simple_command(const char * psz,void * osdataSender);
      virtual int32_t send_simple_command(void * osdata,const char * psz,void * osdataSender);

      virtual sp(::aura::printer) get_printer(const char * pszDeviceName);

      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;











      virtual sp(::aura::application) create_platform(::aura::session * psession) override;





      //////////////////////////////////////////////////////////////////////////////////////////////////
      // System/System
      //
      ::user::document * place_hold(::user::interaction * pui);

      /*
      virtual ::count get_monitor_count();
      virtual bool  get_monitor_rect(index i, LPRECT lprect);
      virtual ::count get_desk_monitor_count();
      virtual bool  get_desk_monitor_rect(index i, LPRECT lprect);
      */


      //////////////////////////////////////////////////////////////////////////////////////////////////
      // Session/Session
      //
      //         virtual sp(::bergedge::view) get_view();
      //       virtual sp(::bergedge::document) get_document();


      virtual bool add_library(::aura::library * plibrary);

      virtual bool initialize_userex();
      virtual bool initialize1_userfs();
      virtual bool initialize1_wndfrm();
      virtual bool process_initialize_userfs();

      virtual ::userex::userex * create_userex();
      virtual ::userfs::userfs * create_userfs();



      //virtual void assert_valid() const;
      //virtual void dump(dump_context & context) const;


      //virtual void construct();
      //virtual void construct(const char * pszId);


      //virtual void _001OnFileNew();


      //virtual void on_request(::create * pcreate);

      //sp(::aura::application) get_system();

      //virtual sp(::core::application) assert_running(const char * pszAppdId);









      // smart_pointer < ::cubebase::application >::oattrib
      // or any function needing it
      application & operator = (const application & app)
      {

         UNREFERENCED_PARAMETER(app);

         // do nothing

         return *this;

      }

      // get a file and if there are exceptions, should show end user friendly messages
      virtual ::file::file_sp friendly_get_file(var varFile,UINT nOpenFlags);


      virtual void data_on_after_change(signal_details * pobj);

      virtual void add_document_template(::user::impact_system * ptemplate);

      virtual ::user::document * open_document_file(const char * lpszFileName);


      virtual int32_t GetVisibleTopLevelFrameCountExcept(sp(::user::interaction) pwndExcept);
      virtual int32_t GetVisibleFrameCount();

      virtual void on_create_keyboard();

      virtual sp(type) user_default_controltype_to_typeinfo(::user::e_control_type e_type);

      virtual void set_form_impact_system(::user::impact_system * pdoctemplate,::user::impact_system * pdoctemplateChild,::user::impact_system * pdoctemplatePlaceHolder);

      sp(::user::document)   create_form(::user::form_callback * pcallback,sp(::user::interaction) pwndParent,var var = ::var(::var::type_empty_argument));
      sp(::user::document)   create_form(sp(::user::form) pview,::user::form_callback * pcallback,sp(::user::interaction) pwndParent,var var = ::var(::var::type_empty_argument));
      sp(::user::document)   create_child_form(::user::form_callback * pcallback,sp(::user::interaction) pwndParent,var var = ::var(::var::type_empty_argument));
      template < typename VIEW >
      sp(::user::document)   create_child_form(sp(::user::interaction) pwndParent, var var = ::var(::var::type_empty_argument));
      sp(::user::document)   create_child_form(sp(::type) pt, sp(::user::interaction) pwndParent, var var = ::var(::var::type_empty_argument));
      sp(::user::document)   create_child_form(sp(::user::form) pview,::user::form_callback * pcallback,sp(::user::interaction) pwndParent,var var = ::var(::var::type_empty_argument));
      ::user::document * hold(sp(::user::interaction) pui);

      virtual bool platform_open_by_file_extension(int iEdge,const char * pszPathName,application_bias * pbiasCreate = NULL);
      virtual bool platform_open_by_file_extension(int iEdge,::create * pcc);


      virtual void on_show_view(::user::tab * ptab);


      virtual string get_cred(const string & strRequestUrl, const RECT & rect, string & strUsername, string & strPassword, string strToken, string strTitle, bool bInteractive);
      virtual string get_cred(string & strUsername, string & strPassword, string strToken);
      virtual void set_cred(string strToken, const char * pszUsername, const char * pszPassword);
      virtual void set_cred_ok(string strToken, bool bOk);

      virtual void remove_document_template(::user::impact_system * pimpactsystem);

      virtual bool _001OnAgreeExit();

   };


} // namespace core


CLASS_DECL_CORE UINT c_cdecl application_thread_procedure(LPVOID pvoid);

typedef sp(::aura::application) (*LPFN_instantiate_application)(sp(::aura::application) pappParent, const char * pszId);

extern CLASS_DECL_CORE LPFN_instantiate_application g_lpfn_instantiate_application;

#ifdef WINDOWSEX

CLASS_DECL_CORE BOOL LaunchAppIntoDifferentSession(const char * pszProcess, const char * pszCommand, const char * pszDir, STARTUPINFO * psi, PROCESS_INFORMATION * ppi, int iSession = -1);

CLASS_DECL_CORE BOOL LaunchAppIntoSystemAcc(const char * pszProcess,const char * pszCommand,const char * pszDir,STARTUPINFO * psi,PROCESS_INFORMATION * ppi);

#endif // WINDOWSEX





