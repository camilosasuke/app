#include "framework.h" // from "axis/user/user.h"
#include "base/user/core_user.h"
#include "base/user/common_user.h"
#include "base/os/windows/windows_system_interaction_impl.h"



extern thread_pointer < memdleak_block > t_plastblock;

namespace base
{


   //UINT application::APPM_LANGUAGE = WM_APP + 117;
   //WPARAM application::WPARAM_LANGUAGE_UPDATE = 1;


   application::application()
   {

      m_puiptraFrame = new ::user::interaction_spa();
      m_peventReady = NULL;


#ifdef WINDOWS

      m_hinstance                = NULL;

#endif

      if(m_pauraapp != NULL)
      {

         m_paxisapp              = m_pauraapp->m_paxisapp;

      }

      m_pbaseapp                 = this;

      if(m_pauraapp != NULL)
      {

         m_paxissystem           = m_pauraapp->m_paxissystem;

         if(m_pauraapp->m_pbasesession == NULL && m_paxissystem != NULL)
         {

            m_pbasesession       = m_paxissystem->m_pbasesession;

         }
         else
         {

            m_pbasesession       = m_pauraapp->m_pbasesession;

         }

#ifdef WINDOWS

         m_hinstance             = m_pauraapp->m_hinstance;

#endif

      }
      else
      {

         m_paxissystem           = NULL;

      }

      m_bBaseProcessInitialize         = false;
      m_bBaseProcessInitializeResult   = false;

      m_bBaseInitialize1               = false;
      m_bBaseInitialize1Result         = false;

      m_bBaseInitialize                = false;
      m_bBaseInitializeResult          = false;

      m_bBaseInitializeInstance        = false;
      m_bBaseInitializeInstanceResult  = false;




   }

   application::~application()
   {
      ::aura::del(m_puiptraFrame);
   }

   
   bool application::process_initialize()
   {

      if(::axis::application::process_initialize())
      {
         
         return true;
         
      }
      
      
      return true;
      
      
   }


   void application::process_message_filter(int32_t code,signal_details * pobj)
   {

      if(pobj == NULL)
         return;   // not handled

      SCAST_PTR(::message::base,pbase,pobj);

      sp(::user::frame_window) pTopFrameWnd;
      //::user::interaction * pMainWnd;
      ::user::interaction * pMsgWnd;
      switch(code)
      {
      //case MSGF_DDEMGR:
      // Unlike other WH_MSGFILTER codes, MSGF_DDEMGR should
      //  never call the next hook.
      // By returning FALSE, the message will be dispatched
      //  instead (the default behavior).
      //return;

      case MSGF_MENU:
            
         pMsgWnd = pbase->m_pwnd.cast < ::user::interaction > ();
            
         if(pMsgWnd != NULL)
         {
            pTopFrameWnd = pMsgWnd->GetTopLevelFrame();
            if(pTopFrameWnd != NULL && pTopFrameWnd->IsTracking() &&
                  pTopFrameWnd->m_bHelpMode)
            {
               //pMainWnd = __get_main_window();
               //if((m_puiMain != NULL) && (IsEnterKey(pbase) || IsButtonUp(pbase)))
               //{
               //   //                  pMainWnd->SendMessage(WM_COMMAND, ID_HELP);
               //   pbase->m_bRet = true;
               //   return;
               //}
            }
         }
      // fall through...

      case MSGF_DIALOGBOX:    // handles message boxes as well.
         //pMainWnd = __get_main_window();
         if(code == MSGF_DIALOGBOX && m_puiActive != NULL &&
               pbase->m_uiMessage >= WM_KEYFIRST && pbase->m_uiMessage <= WM_KEYLAST)
         {
            //// need to translate messages for the in-place container
            //___THREAD_STATE* pThreadState = __get_thread_state();
            //ENSURE(pThreadState);

            //if (pThreadState->m_bInMsgFilter)
            //   return;
            //pThreadState->m_bInMsgFilter = TRUE;    // avoid reentering this code
            //if (m_puiActive->is_window_enabled())
            //{
            //   pre_translate_message(pobj);
            //   if(pobj->m_bRet)
            //   {
            //      pThreadState->m_bInMsgFilter = FALSE;
            //      return;
            //   }
            //}
            //pThreadState->m_bInMsgFilter = FALSE;    // ok again
         }
         break;
      }
      // default to not handled

   }














   bool application::get_frame(sp(::user::interaction) & pui)
   {

#ifdef VSNORD

      if (System.m_possystemwindow != NULL)
      {

         if (System.m_possystemwindow->m_pui != NULL)
         {

            return System.m_possystemwindow->m_pui->m_uiptraChild.get_child(pui);

         }

      }

#endif

      synch_lock sl(&m_mutexFrame);

      return m_puiptraFrame->get_child(pui);

   }










   void application::add_frame(::user::interaction * pwnd)
   {

#if !defined(LINUX) && !defined(METROWIN) && !defined(APPLEOS) && !defined(VSNORD)
      
      if (dynamic_cast <::base::system_interaction_impl *>(pwnd) != NULL)
      {
      
         return;

      }

#endif

      if (pwnd == NULL)
      {

         return;

      }

      synch_lock sl(&m_mutexFrame); // recursive lock (on m_framea.add(pwnd)) but m_puiMain is "cared" by m_frame.m_mutex

      if(m_puiptraFrame->add_unique(pwnd))
      {

         TRACE("::base::application::add_frame ::user::interaction = %0x016x (%s) app=%s", pwnd, typeid(*pwnd).name(), typeid(*this).name());

         System.defer_create_system_frame_window();

         Session.on_create_frame_window();

         if(m_puiMain == NULL)
         {

            m_puiMain = pwnd;

         }

      }

   }


   void application::remove_frame(::user::interaction * pwnd)
   {

      synch_lock sl(&m_mutexFrame); // recursive lock (on m_framea.remove(pwnd)) but m_puiMain is "cared" by m_frame.m_mutex


      //if(get_active_uie() == pwnd)
      //{

      //   set_a

      //}


      if(m_puiMain == pwnd)
      {

         m_puiMain = NULL;

      }

      if (m_puiptraFrame != NULL)
      {

         if (m_puiptraFrame->remove(pwnd) > 0)
         {

            TRACE("::base::application::remove_frame ::user::interaction = %0x016x (%s) app=%s", pwnd, typeid(*pwnd).name(), typeid(*this).name());

         }

      }


   }




   /*string application::get_cred(const string & strRequestUrlParam,const RECT & rect,string & strUsername,string & strPassword,string strToken,string strTitle,bool bInteractive)
   {

      string str = ::fontopus::get_cred(this,strUsername,strPassword,strToken);

      if(str == "ok" && strUsername.has_char() && strPassword.has_char())
         return "ok";

      if(!bInteractive)
         return "failed";

      sp(::fontopus::simple_ui) pui;

      string strRequestUrl(strRequestUrlParam);

      if(strRequestUrl.is_empty())
      {

         string strIgnitionServer = file_as_string_dup(::dir::system() / "config\\system\\ignition_server.txt");

         if(::str::ends_ci(strIgnitionServer,".ca2.cc"))
         {

            strRequestUrl = "https://" + strIgnitionServer + "/";

         }
         else
         {

            strRequestUrl = "https://account.ca2.cc/";

         }

         strRequestUrl = "https://account.ca2.cc/";

      }

      pui = canew(::fontopus::simple_ui(this,strRequestUrl));

      pui->m_login.m_peditUser->set_window_text(strUsername);

      pui->m_login.m_ppassword->set_window_text("");

      string strResult = pui->get_cred(rect,strUsername,strPassword,strToken,strTitle);

      pui->DestroyWindow();

      ::fontopus::set_cred(this,strToken,strUsername,strPassword);

      return strResult;

   }*/


   ::user::user * application::create_user()
   {

      return canew(::user::user(this));

   }




   bool application::on_thread_on_idle(::thread_impl * pimpl,LONG lCount)
   {

      //      ASSERT_VALID(this);
      //
      //#if defined(WINDOWS) && defined(DEBUG) && !defined(___NO_DEBUG_CRT)
      //      // check core API's allocator (before idle)
      //      if(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) & _CRTDBG_CHECK_ALWAYS_DF)
      //         ASSERT(__check_memory());
      //#endif

      //single_lock sl(&pimpl->m_mutexUiPtra,TRUE);

      if(lCount <= 0)
      {

         sp(::user::interaction) pui;

         while(get_frame(pui))
         {
            //::user::interaction * pui = (::user::interaction *) pimpl->m_spuiptra->element_at(i)->m_pvoidUserInteraction;
            bool bOk = false;
            try
            {

               bOk = pui != NULL && pui->IsWindowVisible();
            }
            catch(...)
            {
            }
            if(!bOk)
            {
               //   try
               //   {
               //      Application.remove_frame(pui);
               //   }
               //   catch(...)
               //   {
               //   }
               //   try
               //   {
               //      Session.remove_frame(pui);
               //   }
               //   catch(...)
               //   {
               //   }
               //   try
               //   {
               //      System.remove_frame(pui);
               //   }
               //   catch(...)
               //   {
               //   }
            }
            else
            {
//               sl.unlock();
               try
               {
                  pui->send_message(WM_IDLEUPDATECMDUI,(WPARAM)TRUE);
               }
               catch(...)
               {

               }
               //             sl.lock();
            }
         }


      }
      else if(lCount >= 0)
      {
      }

      //#if defined(WINDOWS) && defined(DEBUG) && !defined(___NO_DEBUG_CRT)
      //      // check core API's allocator (after idle)
      //      if(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) & _CRTDBG_CHECK_ALWAYS_DF)
      //         ASSERT(__check_memory());
      //#endif
      //


      return lCount < 0;  // nothing more to do if lCount >= 0

   }


   bool application::post_user_message(::thread * pthread,::user::primitive * pui,UINT message,WPARAM wparam,lparam lparam)
   {

      if(pthread->m_hthread == NULL)
         return false;

      sp(::user::message) pmessage    = canew(::user::message);

#if MEMDLEAK

      ::aura::application * papp = this;

      if(pthread == NULL)
      {

         set_last_block_file_name(string(typeid(*papp).name()));

      }
      else
      {

         set_last_block_file_name(string(typeid(*papp).name()) +"&nbsp;-&nbsp;" +string(typeid(*pthread).name()));

      }

#endif

      pmessage->m_pui               = (::user::interaction *) pui->m_pvoidUserInteraction;
      pmessage->m_uiMessage         = message;
      pmessage->m_wparam            = wparam;
      pmessage->m_lparam            = lparam;

      return pthread->post_object(message_system,system_message_user,pmessage) != FALSE;

   }



   bool application::is_window(::user::primitive * pui)
   {

      return ((::user::interaction *)pui->m_pvoidUserInteraction)->IsWindow();

   }

   LRESULT application::send_message(::user::primitive * pui,UINT message,WPARAM wparam,lparam lparam)
   {

      return ((::user::interaction *)pui->m_pvoidUserInteraction)->send_message(message,wparam,lparam);

   }

   oswindow application::get_safe_handle(::user::primitive * pui)
   {

      return ((::user::interaction *)pui->m_pvoidUserInteraction)->get_safe_handle();

   }


   void application::dispatch_user_message_object(::object * pobject)
   {

      dispatch_user_message(dynamic_cast < ::user::message * > (pobject));

   }


   void application::dispatch_user_message(::user::message * pmessage)
   {

      pmessage->send();

   }


   ::user::interaction * application::get_parent(::user::interaction * pui)
   {

      return pui->GetParent();

   }


   bool application::enable_window(::user::primitive * pui,bool bEnable)
   {

      // if control has the focus, move the focus before disabling
      if(!bEnable)
      {

         if(Session.get_focus_ui() == ((::user::interaction *)pui->m_pvoidUserInteraction))
         {

            Application.send_message(Application.get_parent(((::user::interaction *)pui->m_pvoidUserInteraction)),WM_NEXTDLGCTL,0,(LPARAM)FALSE);

         }

      }

      return ((::user::interaction *)pui->m_pvoidUserInteraction)->enable_window(bEnable);

   }


   bool application::set_window_text(::user::interaction * pui,const string & strText)
   {

      pui->set_window_text(strText);

      return true;

   }




   sp(::message::base) application::get_message_base(LPMESSAGE lpmsg)
   {

      ::user::interaction * pwnd = NULL;

      if(pwnd == NULL && lpmsg->hwnd != NULL)
      {

         if(lpmsg->message == 126)
         {

            TRACE0("WM_DISPLAYCHANGE");

         }

         ::user::interaction_impl * pimpl = System.impl_from_handle(lpmsg->hwnd);

         if(pimpl != NULL)
         {
            
            try
            {
               
               pwnd = pimpl->m_pui;

            }
            catch(...)
            {

               pwnd = NULL;

            }

         }

         if(pwnd == NULL)
            return NULL;

      }


      if(pwnd != NULL)
         return pwnd->get_base(lpmsg->message,lpmsg->wParam,lpmsg->lParam);

      return ::axis::application::get_message_base(lpmsg);


   }


   void application::process_message(signal_details * pobj)
   {

      sp(::message::base) pbase = pobj;

      if(pbase.is_null() || pbase->m_pwnd == NULL)
      {

         try
         {

            message_handler(pobj);

         }
         catch(const ::exception::exception & e)
         {

            TRACE("application::process_message : error processing application thread message (const ::exception::exception & )");

            if(App(this).on_run_exception((::exception::exception &) e))
               goto run;

            if(App(this).final_handle_exception((::exception::exception &) e))
               goto run;

            __post_quit_message(-1);

            pbase->set_lresult(-1);

            return;

         }
         catch(::exception::base * pe)
         {

            process_window_procedure_exception(pe,pbase);

            TRACE(::aura::trace::category_AppMsg,0,"Warning: Uncaught exception in message_handler (returning %ld) (application::process_message : error processing application thread message).\n",(int_ptr)pbase->get_lresult());

            pe->Delete();

         }
         catch(...)
         {

            TRACE("application::process_message : error processing application thread message (...)");

         }

         return;

      }

      try
      {

         ((::user::interaction *)pbase->m_pwnd->m_pvoidUserInteraction)->message_handler(pobj);


      }
      catch(const ::exception::exception & e)
      {

         TRACE("application::process_message : error processing window message (const ::exception::exception & )");

         if(App(this).on_run_exception((::exception::exception &) e))
            goto run;

         if(App(this).final_handle_exception((::exception::exception &) e))
            goto run;

         __post_quit_message(-1);

         pbase->set_lresult(-1);

         return;

      }
      catch(::exception::base * pe)
      {

         process_window_procedure_exception(pe,pbase);

         TRACE(::aura::trace::category_AppMsg,0,"Warning: Uncaught exception in message_handler (returning %ld) (application::process_message : error processing window message).\n",(int_ptr)pbase->get_lresult());

         pe->Delete();

      }
      catch(...)
      {

         TRACE("application::process_message : error processing window message (...)");

      }

run:
      ;

   }


   ::user::interaction * application::main_window()
   {

      if(m_puiMain == NULL)
         return NULL;

      return (::user::interaction *) m_puiMain->m_pvoidUserInteraction;

   }


   sp(type) application::user_default_controltype_to_typeinfo(::user::e_control_type e_type)
   {

      return sp(type)();

   }


#ifdef HOTPLUGIN_SUBSYSTEM

   int32_t application::hotplugin_host_host_starter_start_sync(const char * pszCommandLine,::aura::application * papp,::hotplugin::host * phost,::hotplugin::plugin * pplugin)
   {

      return hotplugin::host::host_starter_start_sync(pszCommandLine,get_app(),NULL);

   }

#endif


   ::user::interaction * application::FindWindow(const char * lpszClassName,const char * lpszWindowName)
   {

      ::exception::throw_interface_only(this);

      return NULL;

   }


   ::user::interaction * application::FindWindowEx(oswindow oswindowParent,oswindow oswindowChildAfter,const char * lpszClass,const char * lpszWindow)
   {

      ::exception::throw_interface_only(this);

      return NULL;

   }

   void application::remove_document_template(::user::impact_system * pimpactsystem)
   {

   }


   string application::preferred_userschema()
   {

      return "";

   }

   ::user::schema * application::userschema()
   {

      if (m_pbasesession->m_puserschemaSchema == NULL)
      {

         m_pbasesession->defer_create_user_schema(preferred_userschema());

      }

      return m_pbasesession->m_puserschemaSchema;

   }


   void application::on_create_view(::user::view_creator_data * pcreatordata)
   {


   }


   bool application::process_message(LPMESSAGE lpmessage)
   {


      return ::thread::process_message(lpmessage);

   }
   
   
   void application::SetCurrentHandles()
   {

      ::axis::application::SetCurrentHandles();



   }

   int32_t application::exit_application()
   {

      try
      {
         
         for(auto & pair : System.m_appmap)
         {
            
            try
            {
               
               if(pair.m_element2->m_pbaseapp == this)
               {
                  
                  pair.m_element2->m_pbaseapp = NULL;
                  
               }
               
            }
            catch(...)
            {
               
            }
            
         }
         
      }
      catch(...)
      {
   
      }

      try
      {

            axis::application::exit_application();

      }
      catch(...)
      {

      }

      return m_iReturnCode;

   }








   ::visual::icon * application::set_icon(object * pobject, ::visual::icon * picon, bool bBigIcon)
   {

      ::visual::icon * piconOld = get_icon(pobject, bBigIcon);

      if (bBigIcon)
      {

         pobject->oprop("big_icon").operator =((sp(object)) picon);

      }
      else
      {

         pobject->oprop("small_icon").operator =((sp(object)) picon);

      }

      return piconOld;

   }


   ::visual::icon * application::get_icon(object * pobject, bool bBigIcon) const
   {

      if (bBigIcon)
      {

         return const_cast <object *> (pobject)->oprop("big_icon").cast < ::visual::icon >();

      }
      else
      {

         return const_cast <object *> (pobject)->oprop("small_icon").cast < ::visual::icon >();

      }

   }


   bool application::BaseOnControlEvent(::user::form_window * pview, ::user::control_event * pevent)
   {

      if (pevent->m_eevent == ::user::event_initialize_control)
      {

         if (pevent->m_puie->m_id == __id(system_startup_checkbox))
         {

            try
            {
               pevent->m_puie->_001SetCheck(
                  System.os().is_user_auto_start(get_executable_appid()),
                  ::action::source_initialize);
            }
            catch (...)
            {

            }

         }

      }
      else if (pevent->m_eevent == ::user::event_set_check)
      {

         if (pevent->m_puie->m_id == __id(system_startup_checkbox)
            && pevent->m_actioncontext.is_user_source())
         {

            try
            {

               System.os().register_user_auto_start(
                  get_executable_appid(),
                  get_executable_path(),
                  pevent->m_puie->_001GetCheck() == ::check::checked);

               return true;

            }
            catch (...)
            {

            }

         }

      }

      return false;

   }


} // namespace base







