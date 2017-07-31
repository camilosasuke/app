#pragma once


#ifdef HOTPLUGIN_SUBSYSTEM


namespace plugin
{


   class instance;
   enum e_check;
   class plugin;


   class CLASS_DECL_CORE host_interaction :
      public ::user::box
   {
   public:


      enum
      {
         message_check = WM_USER + 10,
      };


      ::user::interaction *     m_pframe;
      plugin *                m_pplugin;


      

      host_interaction(::aura::application * papp);
      virtual ~host_interaction();

      void install_message_handling(::message::dispatch * pmessage);

      virtual void _000OnDraw(::draw2d::graphics * pgraphics);

      virtual bool defer_check(e_check echeck);
      virtual void on_layout();

      
      using ::user::box::GetWindowRect;
      virtual bool GetWindowRect(RECT64 * lprect);

      
      using ::user::box::GetClientRect;
      virtual bool GetClientRect(RECT64 * lprect);

      
      virtual bool IsWindowVisible();

      DECL_GEN_SIGNAL(_001OnMouseMove);
      DECL_GEN_SIGNAL(_001OnCheck);
      DECL_GEN_SIGNAL(_001OnCreate);

      virtual bool RedrawWindow(const RECT & lpRectUpdate,::draw2d::region* prgnUpdate,UINT flags);

      ::user::interaction * get_os_focus_uie();

      void _on_start_user_message_handler();
      void _user_message_handler(signal_details * pobj);

      virtual void _000OnMouse(::message::mouse * pmouse);

      DECL_GEN_SIGNAL(on_ignore_message);
      void _001OnTimer(::timer * ptimer);


      virtual LRESULT DefWindowProc(UINT uiMessage, WPARAM wparam, lparam lparam);


   };


} // namespace plugin



#endif


