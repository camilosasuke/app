//#include "framework.h"


namespace userstack
{


   frame::frame(::aura::application * papp) :
      ::object(papp),
      simple_frame_window(papp),
      m_spqueue(allocer())
   {

      m_pdocument = NULL;

      WfiEnableFullScreen();

      m_bLayered = true;
      select_layered_frame();

      m_ppaneview = NULL;

      m_bMouseOver = false;

   }


   frame::~frame()
   {

   }


   void frame::_001OnCreate(signal_details * pobj)
   {

      SCAST_PTR(::message::create, pcreate, pobj);

      m_bWindowFrame = GetTypedParent < ::plugin::host_interaction >() == NULL;

      if(pobj->previous())
         return;

      if(!m_spqueue->create_message_queue("::ca2::fontopus::message_wnd::bergedge::" + System.get_local_mutex_id(), this))
      {
         pcreate->set_lresult(-1);
         pcreate->m_bRet = true;
         return;
      }

      set_window_text("bergedge");

   }


   void frame::install_message_handling(::message::dispatch * pinterface)
   {
      simple_frame_window::install_message_handling(pinterface);
      IGUI_WIN_MSG_LINK(WM_CLOSE,          pinterface, this, &frame::_001OnClose);
//      //IGUI_WIN_MSG_LINK(WM_TIMER,          pinterface, this, &frame::_001OnTimer);
      IGUI_WIN_MSG_LINK(WM_CREATE,         pinterface, this, &frame::_001OnCreate);
      IGUI_WIN_MSG_LINK(WM_APP + 1,        pinterface, this, &frame::_001OnApp1);
      IGUI_WIN_MSG_LINK(WM_MOUSELEAVE,     pinterface, this, &frame::_001OnMouseLeave);
   }


   bool frame::pre_create_window(::user::create_struct& cs)
   {

      if( !simple_frame_window::pre_create_window(cs) )
         return FALSE;

      cs.dwExStyle &= ~WS_EX_WINDOWEDGE;

      return TRUE;

   }


#ifdef DEBUG
   void frame::assert_valid() const
   {
      simple_frame_window::assert_valid();
   }

   void frame::dump(dump_context & dumpcontext) const
   {
      simple_frame_window::dump(dumpcontext);
   }
#endif //DEBUG


   void frame::_001OnTimer(::timer * ptimer)
   {
      
      simple_frame_window::_001OnTimer(ptimer);;

      UINT nIDEvent = ptimer->m_nIDEvent;
      static float theta;
      if(nIDEvent == 3)
      {
         
      }
      else if(nIDEvent == 1000)
      {
         ShowWindow(SW_RESTORE);
         KillTimer(nIDEvent);
         m_bTimerOn = false;
      }
      else if(nIDEvent == 8913)
      {
      }
      else if(nIDEvent == 4033)
      {
         point pt;
         if(m_bHoverMouse && get_tick_count() > m_dwLastHover + 300)
         {
            OnHoverAction();
         }
         Session.get_cursor_pos(&pt);
         if(!m_bHoverMouse && pt.x == 0 && pt.y == 0)
         {
            m_dwLastHover = ::get_tick_count();
            m_bHoverMouse = true;

         }
         else if(m_bHoverMouse && (pt.x > 10 || pt.y > 0))
         {
            m_bHoverMouse = false;
         }
            theta += 2.0f;

      }
   }


   void frame::ShowControlBars(bool bShow)
   {
      UINT nShow;
      if(bShow)
      {
         nShow = SW_SHOW;
      }
      else
      {
         nShow = SW_HIDE;
      }
   }



   void frame::_000OnMouse(::message::mouse * pmouse)
   {
      Session.m_ptCursor = pmouse->m_pt;
//      ::aspheresp(::core::application) pappParent = &App(Application.m_pauraapp->m_pcoreapp);
//      ::aspheresp(::core::application) papp = &Application;
/*      if(pmouse->m_uiMessage == WM_MOUSEMOVE
      && m_pdocument != NULL
      && m_pdocument->m_pplatformdocument != NULL
      && m_pdocument->m_pplatformdocument->get_platform_frame() != NULL)
      {
         rect rectClient;
         GetClientRect(rectClient);
         point pt = pmouse->m_pt;
         ScreenToClient(&pt);
         if(rectClient.contains(pt))
         {
            get_wnd()->set_capture();
            if(!m_bMouseOver)
            {
               m_bMouseOver = true;
               m_pdocument->m_pplatformdocument->get_platform_frame()->super_dock_on_bergedge();
            }
         }
         else
         {
            get_wnd()->release_capture();
            m_bMouseOver = false;
            m_pdocument->m_pplatformdocument->get_platform_frame()->super_dock_on_bergedge();
         }
      }*/
      simple_frame_window::_000OnMouse(pmouse);
   }

   void frame::_001OnMouseLeave(signal_details * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

//      SCAST_PTR(::message::mouse, pmouse, pobj);
//      m_bMouseOver = false;
//      bergedgesp(::core::application) papp = dynamic_cast < bergedgesp(::core::application) > (get_app());
   }

   void frame::pre_translate_message(signal_details * pobj)
   {
//      SCAST_PTR(::message::base, pbase, pobj);
      simple_frame_window::pre_translate_message(pobj);
   }


   void frame::OnHoverAction()
   {
      if(WfiIsFullScreen())
      {
   //      bergedge::get_app()->show_platform();
      }
      else
      {
         WfiFullScreen();
      }
   }

   void frame::message_handler(signal_details * pobj)
   {
      simple_frame_window::message_handler(pobj);
   }


   void frame::message_queue_message_handler(signal_details * pobj)
   {
      SCAST_PTR(::message::base, pbase, pobj);
      if(pbase->m_uiMessage == (WM_APP + 2000))
      {
         _001OnApp2000(pbase);
         pbase->m_bRet = true;
      }
      else if(pbase->m_uiMessage == WM_COPYDATA)
      {
#ifdef WINDOWSEX
         int32_t iEdge = 0;
         COPYDATASTRUCT * pstruct = (COPYDATASTRUCT *) pbase->m_lparam.m_lparam;
         if(pstruct->dwData == 1984)
         {
            ::memory_file file(get_app(), pstruct->lpData, pstruct->cbData);
            string strPath;
            file.to_string(strPath);
            Session.open_by_file_extension(strPath);
         }
#else
         throw todo(get_app());
#endif
      }

   }


   void frame::_001OnApp2000(signal_details * pobj)
   {
      SCAST_PTR(::message::base, pbase, pobj);


      if(pbase->m_wparam == 0)
      {
         if(pbase->m_lparam == 2)
         {
           //OnHoverAction(true);
            ShowWindow(SW_SHOW);
            SetWindowPos(ZORDER_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
            WfiFullScreen();
         }
         else if(pbase->m_lparam == 5)
         {
#ifdef WINDOWSEX
            class rect rect;
            ::GetWindowRect(::GetDesktopWindow(), rect);
            point pt = rect.center();
            if(!IsWindowVisible()
            || ::WindowFromPoint(pt) != get_safe_handle())
            {
               ShowWindow(SW_SHOW);
               SetWindowPos(ZORDER_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
               WfiFullScreen();
               BringWindowToTop();
            }
            else
            {
               ShowWindow(SW_HIDE);
            }
#else
            throw todo(get_app());
#endif
         }
      }
/*         else if(pbase->m_lparam == 1)
         {
            m_bTimerOn = true;
            SetTimer(1000, 23, NULL);
         }
         else if(pbase->m_lparam == 0)
         {
            KillTimer(1000);
            m_bTimerOn = false;
         }
         else if(pbase->m_lparam == 3)
         {
           ShowWindow(SW_HIDE);
         }
  */
    /*  }
*/
      else if(pbase->m_wparam == 1)
      {
         pbase->set_lresult(2);
      }
      else if(pbase->m_wparam == 2)
      {
         pbase->set_lresult(4);
      }
      else if(pbase->m_wparam == 3)
      {
         if(pbase->m_lparam == 6)
         {
            GetTopLevelFrame()->ShowWindow(SW_HIDE);
  //          m_pwinutilview->KillTimer(21977);
            __post_quit_message(36);
         }
      }
      else if(pbase->m_wparam == 4)
      {
         pbase->set_lresult(5);
      }
      else if(pbase->m_wparam == 5)
      {
         pbase->set_lresult(8);
      }
      else if(pbase->m_wparam == 8)
      {
         pbase->set_lresult(11);
      }
      else if(pbase->m_wparam == 11)
      {
         pbase->set_lresult(23);
      }
      else if(pbase->m_wparam == 23)
      {
         pbase->set_lresult(33);
      }
      else if(pbase->m_wparam == 33)
      {
         string str = System.get_local_mutex_id();
         ::str::begins_eat_ci(str, "bergedge");
         int32_t iEdge = atoi(str);
         if(iEdge == 0)
            iEdge = 77;
         else
            iEdge += 1984;
         pbase->set_lresult(iEdge);
      }
      else if(pbase->m_wparam == 25)
      {
#ifdef WINDOWSEX
         pbase->set_lresult((LRESULT) GetTopLevelFrame()->get_safe_handle());
#else
         throw todo(get_app());
#endif
      }
      pbase->m_bRet = true;
   }

      /*else if(pbase->m_wparam == 1)
      {
         pbase->set_lresult(2);
      }
      else if(pbase->m_wparam == 2)
      {
         pbase->set_lresult(4);
      }
      else if(pbase->m_wparam == 3)
      {
         if(pbase->m_lparam == 6)
         {
            GetTopLevelFrame()->ShowWindow(SW_HIDE);
            __post_quit_message(36);
         }
      }
      else if(pbase->m_wparam == 4)
      {
         pbase->set_lresult(5);
      }
      else if(pbase->m_wparam == 5)
      {
         pbase->set_lresult(8);
      }
      else if(pbase->m_wparam == 8)
      {
         pbase->set_lresult(11);
      }
      else if(pbase->m_wparam == 11)
      {
         pbase->set_lresult(23);
      }
      else if(pbase->m_wparam == 23)
      {
         pbase->set_lresult(33);
      }
      else if(pbase->m_wparam == 33)
      {
         pbase->set_lresult(68);
      }
      pbase->m_bRet = true;*/
   //}


   void frame::_001OnApp1(signal_details * pobj)
   {

#ifdef WINDOWSEX

      SCAST_PTR(::message::base, pbase, pobj);

      MESSAGE * pmsg = (MESSAGE *) pbase->m_lparam.m_lparam;

      pmsg->hwnd = get_safe_handle();

      try
      {

         if(pmsg->message != WM_KICKIDLE)
         {
            ::smart_pointer < ::message::base > spbase;
            spbase = Application.get_base(pmsg);
            pre_translate_message(spbase);
            if(!spbase->m_bRet)
            {
               send(spbase);
            }
         }

      }
      catch(const ::exception::exception & )
      {
      }
      catch(...)
      {
      }

      delete pmsg;

#else

      throw todo(get_app());

#endif

   }


} // namespace userstack



