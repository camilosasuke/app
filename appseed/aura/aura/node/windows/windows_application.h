#pragma once


namespace windows
{


   class CLASS_DECL_AURA main_init_data :
      public ::aura::main_init_data
   {
   public:


      HINSTANCE   m_hInstance;
      HINSTANCE   m_hPrevInstance;
      int32_t         m_nCmdShow;


   };


//   class CLASS_DECL_AURA application :
//      virtual public ::aura::application
//   {
//   public:
//
//
//
//      USHORT               m_atomApp;
//      USHORT               m_atomSystemTopic;
//
//      main_init_data *     m_pmaininitdata;
//
//
//      application(::aura::application * papp);
//      virtual ~application();
//
//
//
//      
//      virtual HINSTANCE GetHinstance();
//      bool _001OnDDECommand(const char * lpcsz);
//      virtual void _001EnableShellOpen();
////      virtual ::user::document *  _001OpenDocumentFile(var varFile);
//      virtual void _001OnFileNew();
//
//
//      virtual string get_version();
//
//      virtual bool Ex2OnAppInstall();
//      virtual bool Ex2OnAppUninstall();
//
//      virtual void TermThread(HINSTANCE hInstTerm);
//
//
//      virtual void SetCurrentHandles();
//
//      virtual bool set_main_init_data(::aura::main_init_data * pdata);
//
//      virtual bool process_initialize();
//      virtual bool initialize1();
//      virtual bool initialize2();
//      virtual bool initialize3();
//      virtual int32_t  exit_application() override;
//
//
//
//      
//      virtual bool win_init(main_init_data * pdata);
//
//
//
//
//      //virtual bool update_module_paths();
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//      virtual ::user::primitive * FindWindow(const char * lpszClassName,const char * lpszWindowName);
//      virtual ::user::primitive * FindWindowEx(oswindow oswindowParent,oswindow oswindowChildAfter,const char * lpszClass,const char * lpszWindow);
//
//      virtual void get_time(struct timeval *p);
//      virtual void set_env_var(const string & var,const string & value);
//      virtual uint32_t get_thread_id();
//
//      virtual void ShowWaitCursor(bool bShow = true);
//
//
//      virtual string draw2d_get_default_library_name();
//      virtual string multimedia_audio_get_default_library_name();
//      virtual string multimedia_audio_mixer_get_default_library_name();
//      virtual string veriwell_multimedia_music_midi_get_default_library_name();
//
//
//   };


} // namespace windows

