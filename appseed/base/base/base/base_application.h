#pragma once


namespace base
{


   class CLASS_DECL_BASE application :
      virtual public ::axis::application,
      virtual public ::user::form_callback
   {
   public:


      mutex                                           m_mutexFrame;
      ::user::interaction_spa *                       m_puiptraFrame;



      bool                                            m_bBaseProcessInitialize;
      bool                                            m_bBaseProcessInitializeResult;

      bool                                            m_bBaseInitializeInstance;
      bool                                            m_bBaseInitializeInstanceResult;

      bool                                            m_bBaseInitialize1;
      bool                                            m_bBaseInitialize1Result;

      bool                                            m_bBaseInitialize;
      bool                                            m_bBaseInitializeResult;



      application();
      virtual ~application();




      virtual bool process_initialize() override;



      virtual sp(type) user_default_controltype_to_typeinfo(::user::e_control_type e_type);


      virtual sp(::message::base) get_message_base(LPMESSAGE lpmsg);

      virtual ::user::interaction * main_window();

      virtual void on_create_view(::user::view_creator_data * pcreatordata);
      void process_message_filter(int32_t code,signal_details * pobj);

      virtual bool get_frame(sp(::user::interaction) & pui);
      virtual void add_frame(::user::interaction * pwnd);
      virtual void remove_frame(::user::interaction * pwnd);

      //virtual string get_cred(const string & strRequestUrl,const RECT & rect,string & strUsername,string & strPassword,string strToken,string strTitle,bool bInteractive);

      virtual ::user::user * create_user();

      virtual bool on_thread_on_idle(::thread_impl * pimpl,LONG lCount);

      virtual bool is_window(::user::primitive * pui);

      using ::aura::application::send_message;
      virtual LRESULT send_message(::user::primitive * pui,UINT message,WPARAM wparam = 0,lparam lparam = 0);

      virtual oswindow get_safe_handle(::user::primitive * pui);
      virtual void dispatch_user_message(::user::message * pmessage);
      virtual void dispatch_user_message_object(::object * pobject);
      virtual ::user::interaction * get_parent(::user::interaction * pui);
      virtual bool enable_window(::user::primitive * pui,bool bEnable = true);
      virtual bool set_window_text(::user::interaction * pui,const string & strText);

      virtual void process_message(signal_details * pobj);
      virtual bool process_message(LPMESSAGE lpmessage);


#ifdef HOTPLUGIN_SUBSYSTEM

      int32_t hotplugin_host_host_starter_start_sync(const char * pszCommandLine,::aura::application * papp,::hotplugin::host * phost,::hotplugin::plugin * pplugin);

#endif

      virtual ::user::interaction * FindWindow(const char * lpszClassName,const char * lpszWindowName);
      virtual ::user::interaction * FindWindowEx(oswindow oswindowParent,oswindow oswindowChildAfter,const char * lpszClass,const char * lpszWindow);
      virtual bool post_user_message(::thread * pthread,::user::primitive * pui,UINT message,WPARAM wparam = 0,lparam lparam = 0);



      using ::axis::application::BaseOnControlEvent;
      using ::user::form_callback::BaseOnControlEvent;


      virtual void remove_document_template(::user::impact_system * pimpactsystem);

      virtual string preferred_userschema();

      virtual ::user::schema * userschema();

      //virtual int32_t exit_instance();
      
      virtual int32_t exit_application() override;
      
      
      virtual void SetCurrentHandles() override;
      
      virtual ::visual::icon * set_icon(object * pobject, ::visual::icon * picon, bool bBigIcon);
      
      
      virtual ::visual::icon * get_icon(object * pobject, bool bBigIcon) const;
  
      virtual bool BaseOnControlEvent(::user::form_window * pview, ::user::control_event * pevent) override;



   };


} // namespace base






