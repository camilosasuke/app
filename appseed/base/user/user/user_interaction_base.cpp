//#include "framework.h" // from "base/user/user.h"
//#include "base/user/user.h"


namespace user
{


   interaction_base::interaction_base() :
      m_evNewMessage(::get_thread_app())
   {

   }


   interaction_base::interaction_base(::aura::application * papp) :
      ::object(papp),
      command_target_interface(papp),
      m_evNewMessage(papp)
   {

   }


   interaction_base::~interaction_base()
   {

   }


   void interaction_base::install_message_handling(::message::dispatch * pinterface)
   {

      ::message::dispatch::install_message_handling(pinterface);

   }


   void interaction_base::TwfGetWndArray(::user::interaction_spa & wndpa)
   {

      wndpa = *dynamic_cast < ::user::interaction_spa * >(get_app());

   }


   void interaction_base::TwfGetWndArray(::user::oswindow_array & oswindowa)
   {

      ::user::interaction_spa & wndpa = *dynamic_cast < ::user::interaction_spa * >(get_app());

      oswindowa = wndpa.get_hwnda();

   }


   void interaction_base::RedrawOptimize(rect_array &array)
   {

      rect rect;

Restart:

      for(int32_t i = 0; i < array.get_size(); i++)
      {
         for(int32_t j = i + 1; j < array.get_size(); j++)
         {
            if(RedrawOptimize(
                     rect,
                     array[i],
                     array[j]))
            {
               array[i] = rect;
               array.remove_at(j);
               goto Restart;

            }

         }

      }

   }


   bool interaction_base::RedrawOptimize(LPRECT lprectOut, LPCRECT lpcrect1, LPCRECT lpcrect2)
   {

      rect rect1(lpcrect1);
      rect rect2(lpcrect2);
      rect rect3;

      int32_t iArea1 = rect1.width() * rect1.height();
      int32_t iArea2 = rect2.width() * rect2.height();
      rect3.unite(rect1, rect2);
      int32_t iArea3 = rect3.width() * rect3.height();

      if(iArea3 < (110 * (iArea1 + iArea2) / 100))
      {

         *lprectOut = rect3;

         return true;

      }
      else
      {

         return false;

      }

   }


   void interaction_base::PreSubClassWindow()
   {

      _001BaseWndInterfaceMap();

   }


   void interaction_base::_001BaseWndInterfaceMap()
   {

   }


   ::user::interaction * interaction_base::get_wnd() const
   {

      return NULL;

   }


   ::user::interaction_impl * interaction_base::get_impl() const
   {

      ::user::interaction * pui = get_wnd();

      if (pui == NULL)
      {

         return NULL;

      }

      if (pui->m_pimpl.is_null())
      {

         return NULL;

      }

      try
      {

         return pui->m_pimpl->get_impl();

      }
      catch (...)
      {

      }

      return NULL;

   }


   bool interaction_base::is_window_enabled()
   {

      return true;

   }


   bool interaction_base::enable_window(bool bEnable)
   {

      UNREFERENCED_PARAMETER(bEnable);

      return true;

   }


   void interaction_base::_on_start_user_message_handler()
   {

      dispatch::_on_start_user_message_handler();

      _001BaseWndInterfaceMap();

   }


   void interaction_base::_000OnDraw(::draw2d::graphics * pgraphics)
   {

      _001OnDraw(pgraphics);
      _008OnDraw(pgraphics);

   }


   void interaction_base::UpdateWindow()
   {

      //ASSERT(::IsWindow(GetHandle()));
      //::UpdateWindow(GetHandle());

   }


   void interaction_base::Invalidate(bool bErase)
   {

      UNREFERENCED_PARAMETER(bErase);

      //ASSERT(::IsWindow(GetHandle()));
      //::InvalidateRect(GetHandle(), NULL, bErase);

   }


   bool interaction_base::Redraw(rect_array & recta)
   {

      bool bOk = true;

      for(int32_t i = 0; i < recta.get_size(); i++)
      {

         if(!get_wnd()->RedrawWindow(recta[i]))
            bOk = false;

      }

      return bOk;

   }


   bool interaction_base::Redraw(LPCRECT lprect, ::draw2d::region * prgn)
   {

      if(get_wnd() == NULL)
         return false;

      return get_wnd()->RedrawWindow(lprect, prgn, RDW_INVALIDATE);

   }


   bool interaction_base::Redraw(::draw2d::graphics * pgraphics)
   {

      UNREFERENCED_PARAMETER(pgraphics);

      if(get_wnd() == NULL)
         return false;

      return get_wnd()->RedrawWindow();

   }


   bool interaction_base::ClientToScreen(LPRECT lprect)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::ClientToScreen(LPPOINT lppoint)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::ClientToScreen(RECT64 * lprect)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::ClientToScreen(POINT64 * lppoint)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::ScreenToClient(LPRECT lprect)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::ScreenToClient(LPPOINT lppoint)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::ScreenToClient(RECT64 * lprect)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::ScreenToClient(POINT64 * lppoint)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::GetWindowRect(LPRECT lprect)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   //bool interaction_base::GetFocusRect(LPRECT lprect)
   //{

   //   return GetClientRect(lprect);

   //}


   bool interaction_base::GetClientRect(LPRECT lprect)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::GetClientRect(RECT64 * lprect)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::GetWindowRect(RECT64 * lprect)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   rect interaction_base::GetWindowRect()
   {

      rect rect;

      GetWindowRect(&rect);

      return rect;

   }


   rect64 interaction_base::GetWindowRect64()
   {

      rect64 rect;

      GetWindowRect(&rect);

      return rect;

   }












/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





































   bool interaction_base::IsAscendant(const interaction * puiIsAscendant) const
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::IsParent(const interaction * puiIsParent) const
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::IsChild(const interaction * puiIsChild) const
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::IsDescendant(const interaction * puiIsDescendant) const
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   sp(place_holder) interaction_base::place(::user::interaction * pui)
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }



   void interaction_base::set_timer(smart_pointer_array < ::aura::timer_item > timera)
   {

      ::exception::throw_interface_only(get_app());

   }


   bool interaction_base::SetPlacement(const RECT & rect,UINT nFlags)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::RepositionWindow(const RECT & rect,UINT nFlags)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::RepositionWindow(int32_t x,int32_t y,int32_t cx,int32_t cy,UINT nFlags)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::MoveWindow(int32_t x,int32_t y,UINT nFlags)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::MoveWindow(POINT pt,UINT nFlags)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::SizeWindow(int32_t cx,int32_t cy,UINT nFlags)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::SizeWindow(SIZE sz,UINT nFlags)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::ResizeWindow(int32_t cx,int32_t cy,UINT nFlags)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::ResizeWindow(SIZE sz,UINT nFlags)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::SetWindowPos(int_ptr z,const RECT & rect,UINT nFlags)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::defer_set_window_pos(int32_t z,int32_t x,int32_t y,int32_t cx,int32_t cy,UINT nFlags) // only set_windows_pos if get_parent()->ScreenToClient(get_window_rect) different of rect(x, y, cx, cy)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   void interaction_base::_001DrawThis(::draw2d::graphics * pgraphics)
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_base::_001DrawChildren(::draw2d::graphics * pgraphics)
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_base::_001Print(::draw2d::graphics * pgraphics)
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_base::_001OnDraw(::draw2d::graphics * pgraphics)
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_base::_008OnDraw(::draw2d::graphics * pgraphics)
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_base::draw_control_background(::draw2d::graphics * pgraphics)
   {

      ::exception::throw_interface_only(get_app());

   }


   ::user::interaction * interaction_base::_001FromPoint(point64 pt,bool bTestedIfParentVisible)
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   bool interaction_base::_001IsPointInside(point64 pt)
   {

      
      return _001IsPointInside(point(pt));

   }


   ::user::interaction * interaction_base::get_child_by_name(const char * pszName,int32_t iLevel)
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   ::user::interaction * interaction_base::get_child_by_id(id id,int32_t iLevel)
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   LRESULT interaction_base::send(::message::base * pbase)
   {
      
      if(pbase->m_pwnd.is_null())
      {
         
         pbase->m_pwnd = this;
         
      }

      message_handler(pbase);

      return pbase->get_lresult();

   }


   // pbase object should be allocated with new in
   // base or derived object and will be delete after
   // handling
   bool interaction_base::post(::message::base * pbase)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   LRESULT interaction_base::send_message(UINT uiMessage,WPARAM wparam,lparam lparam)
   {

      return message_call(uiMessage, wparam, lparam);

   }

   LRESULT interaction_base::message_call(UINT uiMessage, WPARAM wparam, lparam lparam)
   {

      return 0;

   }


#ifdef LINUX


   LRESULT interaction_base::send_message(XEvent * pevent)
   {

      ::exception::throw_interface_only(get_app());

      return 0;

   }


#endif


   bool interaction_base::IsWindowVisible()
   {

      return false;

   }


   bool interaction_base::is_this_visible()
   {

      return (GetStyle() & WS_VISIBLE) != 0;

   }


   bool interaction_base::ModifyStyle(uint32_t dwRemove,uint32_t dwAdd,UINT nFlags)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::ModifyStyleEx(uint32_t dwRemove,uint32_t dwAdd,UINT nFlags)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::ShowWindow(int32_t nCmdShow)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   ::user::frame_window * interaction_base::GetTopLevelFrame() const
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   void interaction_base::SendMessageToDescendants(UINT message,WPARAM wparam,lparam lparam,bool bDeep,bool bOnlyPerm)
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_base::pre_translate_message(signal_details * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

   }


   oswindow interaction_base::get_handle() const
   {

      return NULL;

   }


   bool interaction_base::subclass_window(oswindow posdata)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   oswindow interaction_base::unsubclass_window()
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


#if defined(METROWIN) || defined(APPLE_IOS) || defined(ANDROID)


   bool interaction_base::initialize_native_window(::user::native_window_initialize * pinitialize)
   {

      ::exception::throw_interface_only(get_app());

      return true;

   }


#endif


   bool interaction_base::create_window(const RECT & rect,::user::interaction *pparent,id id)
   {

      ::exception::throw_interface_only(get_app());

      return true;

   }


   bool interaction_base::create_window(const char * lpszClassName,const char * lpszWindowName,uint32_t dwStyle,const RECT & rect,::user::interaction * pParentWnd,id id, ::create * pcreate)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::create_window_ex(uint32_t dwExStyle,const char * lpszClassName,const char * lpszWindowName,uint32_t dwStyle,const RECT & rect,::user::interaction * pParentWnd,id id,LPVOID lpParam)
   {

      ::exception::throw_interface_only(get_app());

      return true;

   }


   void interaction_base::BringToTop(int nCmdShow)
   {

      ::exception::throw_interface_only(get_app());

   }


   bool interaction_base::BringWindowToTop()
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::IsWindow() const
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   LONG interaction_base::get_window_long(int32_t nIndex) const
   {

      ::exception::throw_interface_only(get_app());

      return 0;

   }


   LONG interaction_base::set_window_long(int32_t nIndex,LONG lValue)
   {

      ::exception::throw_interface_only(get_app());

      return 0;

   }


   LONG_PTR interaction_base::get_window_long_ptr(int32_t nIndex) const
   {

      ::exception::throw_interface_only(get_app());

      return 0;

   }


   LONG_PTR interaction_base::set_window_long_ptr(int32_t nIndex,LONG_PTR lValue)
   {

      ::exception::throw_interface_only(get_app());

      return 0;

   }


   bool interaction_base::RedrawWindow(LPCRECT lpRectUpdate, ::draw2d::region* prgnUpdate, UINT flags)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   ::user::interaction * interaction_base::ChildWindowFromPoint(POINT point)
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   ::user::interaction * interaction_base::ChildWindowFromPoint(POINT point,UINT nFlags)
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   ::user::interaction * interaction_base::get_next_window(UINT nFlag)
   {

      if(nFlag == GW_HWNDNEXT)
      {

         return get_next(true, NULL);

      }
      else
      {

         ::exception::throw_interface_only(get_app());

         return NULL;

      }

   }


   ::user::interaction * interaction_base::get_next(bool bIgnoreChildren,int32_t * piLevel)
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   bool interaction_base::is_message_only_window() const
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   ::user::interaction * interaction_base::GetTopWindow() const
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   ::user::interaction * interaction_base::get_wnd(UINT nCmd) const
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   ::user::interaction * interaction_base::GetActiveWindow()
   {

      ::user::interaction * pui = get_wnd();

      if(pui == NULL)
         return NULL;

      return get_wnd()->GetActiveWindow();

   }


   bool interaction_base::SetFocus()
   {

      sp(::user::interaction) pui = get_wnd();

      if(pui.is_null())
         return NULL;

      return get_wnd()->SetFocus();

   }


   interaction * interaction_base::SetActiveWindow()
   {

      //::exception::throw_interface_only(get_app());

      return NULL;

   }


   bool interaction_base::SetForegroundWindow()
   {

      return true;

   }


   ::user::interaction * interaction_base::GetLastActivePopup()
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   void interaction_base::set_window_text(const char * lpszString)
   {

   }


   strsize interaction_base::get_window_text(char * lpszStringBuf, strsize nMaxCount)
   {

      ::exception::throw_interface_only(get_app());

      return 0;

   }


   string interaction_base::get_window_text()
   {

      ::exception::throw_interface_only(get_app());

      return "";

   }


   void interaction_base::get_window_text(string & rString)
   {

      ::exception::throw_interface_only(get_app());

   }


   strsize interaction_base::get_window_text_length()
   {

      ::exception::throw_interface_only(get_app());

      return 0;

   }


   ::user::interaction * interaction_base::EnsureTopLevel()
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   ::user::interaction * interaction_base::EnsureParentTopLevel()
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   ::user::interaction * interaction_base::GetTopLevel() const
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   ::user::interaction * interaction_base::GetParentTopLevel() const
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   ::user::frame_window * interaction_base::GetParentTopLevelFrame() const
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   ::user::frame_window * interaction_base::EnsureParentFrame()
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   LRESULT interaction_base::Default()
   {

      ::exception::throw_interface_only(get_app());

      return 0;

   }


   uint32_t interaction_base::GetStyle() const
   {

      ::exception::throw_interface_only(get_app());

      return 0;

   }


   uint32_t interaction_base::GetExStyle() const
   {

      ::exception::throw_interface_only(get_app());

      return 0;

   }


   bool interaction_base::DestroyWindow()
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   void interaction_base::PostNcDestroy()
   {

      synch_lock sl(m_pmutex);

      try
      {

         signalizable_disconnect_all();

      }
      catch(...)
      {

      }

      try
      {

         m_signala.remove_all();

      }
      catch(...)
      {

      }

      try
      {

         m_dispatchUpdateCmdUi.m_signala.remove_all();

      }
      catch(...)
      {

      }


      try
      {

         m_dispatchCommand.m_signala.remove_all();

      }
      catch(...)
      {

      }



      try
      {

         m_signalidaCommand.remove_all();

      }
      catch(...)
      {

      }

      try
      {

         ::signalizable::m_signalptra.remove_all();

      }
      catch(...)
      {

      }

      //try
      //{

      //   m_pauraapp = NULL;

      //}
      //catch (...)
      //{

      //}

   }


   ::user::frame_window * interaction_base::GetParentFrame() const
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   void interaction_base::CalcWindowRect(LPRECT lprect,UINT nAdjustType)
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_base::RepositionBars(UINT nIDFirst,UINT nIDLast,id nIDLeftOver,
                                         UINT nFlag,LPRECT lpRectParam,
                                         LPCRECT lpRectClient,bool bStretch)
   {

      ::exception::throw_interface_only(get_app());

   }


   ::user::interaction * interaction_base::SetOwner(::user::interaction * pui)
   {

      //::exception::throw_interface_only(get_app());

      return NULL;

   }


   ::user::interaction * interaction_base::SetParent(::user::interaction * pui)
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   ::user::interaction * interaction_base::GetOwner() const
   {

      //::exception::throw_interface_only(get_app());

      return NULL;

   }


   interaction * interaction_base::GetParent() const
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   ::user::frame_window * interaction_base::GetFrame() const
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   ::user::interaction * interaction_base::GetParentOwner() const
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   ::user::interaction * interaction_base::GetTopLevelOwner() const
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   interaction * interaction_base::GetDescendantWindow(id iId) const
   {

//      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   void interaction_base::set_viewport_org(::draw2d::graphics * pgraphics)
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_base::viewport_client_to_screen(POINT * ppt)
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_base::viewport_screen_to_client(POINT * ppt)
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_base::viewport_client_to_screen(RECT * prect)
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_base::viewport_screen_to_client(RECT * prect)
   {

      ::exception::throw_interface_only(get_app());

   }


   //int32_t interaction_base::SetWindowRgn(HRGN hRgn,bool bRedraw)
   //{

   //   ::exception::throw_interface_only(get_app());

   //   return -1;

   //}


   //int32_t interaction_base::GetWindowRgn(HRGN hRgn)
   //{

   //   ::exception::throw_interface_only(get_app());

   //   return -1;

   //}


   bool interaction_base::WfiIsZoomed()
   {

      return false;

   }


   bool interaction_base::WfiIsFullScreen()
   {

      return false;

   }


   bool interaction_base::WfiIsIconic()
   {

      return false;

   }


   bool interaction_base::CheckAutoCenter()
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   void interaction_base::CenterWindow(::user::interaction * pAlternateOwner)
   {

      ::exception::throw_interface_only(get_app());

   }


   LRESULT interaction_base::DefWindowProc(UINT uiMessage,WPARAM wparam,lparam lparam)
   {

      return 0;

   }


   void interaction_base::queue_message_handler(signal_details * pobj)
   {

      if (m_pthread.is_null())
      {

         m_evNewMessage.ResetEvent();

         m_pthread = ::fork(get_app(),
            [&]()
         {

            m_messagequeue.defer_create_mutex();

            m_evNewMessage.SetEvent();

            while (get_thread_run())
            {

               if (m_evNewMessage.wait(seconds(1)).succeeded())
               {


                  m_evNewMessage.ResetEvent();

               }

               {

                  single_lock sl(m_messagequeue.m_pmutex);

                  if (m_messagequeue.has_elements())
                  {

                     smart_pointer < ::message::base > pbase = m_messagequeue.element_at(0);

                     m_messagequeue.remove_at(0);

                     sl.unlock();

                     message_handler(pbase);

                  }

               }

            }

         });

         m_evNewMessage.wait();

      }


      smart_pointer < ::message::base > pbase = pobj;

      if (pbase.is_set())
      {

         {

            single_lock sl(m_messagequeue.m_pmutex);

            m_messagequeue.add(pbase);

         }

         m_evNewMessage.SetEvent();

      }


   }



   void interaction_base::message_handler(signal_details * pobj)
   {

      ::exception::throw_interface_only(get_app());

   }


   LRESULT interaction_base::message_handler(LPMESSAGE lpmessage)
   {

      ::exception::throw_interface_only(get_app());

      return 0;

   }


#ifdef WINDOWSEX


   bool interaction_base::GetWindowPlacement(WINDOWPLACEMENT* lpwndpl)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::SetWindowPlacement(const WINDOWPLACEMENT* lpwndpl)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


#endif


   bool interaction_base::pre_create_window(::user::create_struct& cs)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::IsTopParentActive()
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   void interaction_base::ActivateTopParent()
   {

   }


   void interaction_base::UpdateDialogControls(command_target* pTarget,bool bDisableIfNoHandler)
   {

      //::exception::throw_interface_only(get_app());

   }


   void interaction_base::SetRedraw(bool bRedraw)
   {

      ::exception::throw_interface_only(get_app());

   }


   bool interaction_base::GetUpdateRect(LPRECT lpRect,bool bErase)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   int32_t interaction_base::GetUpdateRgn(::draw2d::region* pRgn,bool bErase)
   {

      ::exception::throw_interface_only(get_app());

      return -1;

   }


   void interaction_base::InvalidateRect(LPCRECT lpRect,bool bErase)
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_base::InvalidateRgn(::draw2d::region* pRgn,bool bErase)
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_base::ValidateRect(LPCRECT lpRect)
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_base::ValidateRgn(::draw2d::region* pRgn)
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_base::on_layout()
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_base::on_translate()
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_base::on_do_show_flags()
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_base::ShowOwnedPopups(bool bShow)
   {

      ::exception::throw_interface_only(get_app());

   }


   bool interaction_base::attach(oswindow oswindow_New)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   oswindow interaction_base::detach()
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   void interaction_base::pre_subclass_window()
   {

      ::exception::throw_interface_only(get_app());

   }


   id interaction_base::run_modal_loop(::user::interaction * pui,uint32_t dwFlags,::object * pliveobject)
   {

      ::exception::throw_interface_only(get_app());

      return ::id();

   }


   id interaction_base::RunModalLoop(uint32_t dwFlags,::object * pliveobject)
   {

      ::exception::throw_interface_only(get_app());

      return ::id();

   }


   id interaction_base::_001RunModalLoop(uint32_t dwFlags,::object * pliveobject)
   {

      ::exception::throw_interface_only(get_app());

      return id();

   }


   bool interaction_base::ContinueModal(int32_t iLevel)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   void interaction_base::EndModalLoop(id nResult)
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_base::EndAllModalLoops(id nResult)
   {

      ::exception::throw_interface_only(get_app());

   }


   bool interaction_base::BaseOnControlEvent(control_event * pevent)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::post_message(UINT uiMessage,WPARAM wparam,lparam lparam)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::SetTimer(uint_ptr nIDEvent,UINT nElapse, PFN_TIMER pfnTimer)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::KillTimer(uint_ptr nIDEvent)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::has_focus()
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::SetCapture(::user::interaction * pui)
   {

      return false;

   }


   ::user::interaction * interaction_base::GetCapture()
   {

      return NULL;

   }


   bool interaction_base::ReleaseCapture()
   {

      return false;

   }


   void interaction_base::track_mouse_leave()
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_base::track_mouse_hover()
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_base::_001UpdateWindow(bool bUpdateBuffer)
   {

      UNREFERENCED_PARAMETER(bUpdateBuffer);

   }


   void interaction_base::_001WindowMinimize(bool bNoActivate)
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_base::_001WindowMaximize()
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_base::_001WindowFullScreen()
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_base::_001WindowRestore()
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_base::GuieProc(signal_details * pobj)
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_base::_001DeferPaintLayeredWindowBackground(::draw2d::graphics * pgraphics)
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_base::_001OnDeferPaintLayeredWindowBackground(::draw2d::graphics * pgraphics)
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_base::OnLinkClick(const char * psz,const char * pszTarget)
   {

      ::exception::throw_interface_only(get_app());

   }


   bool interaction_base::create_message_queue(const char * pszName)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   void interaction_base::on_select()
   {

      ::exception::throw_interface_only(get_app());

   }


   bool interaction_base::is_place_holder()
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   ::visual::e_cursor interaction_base::get_cursor()
   {

      ::exception::throw_interface_only(get_app());

      return ::visual::cursor_none;

   }


   void interaction_base::set_cursor(::visual::e_cursor ecursor)
   {

      ::exception::throw_interface_only(get_app());

   }


   bool interaction_base::can_merge(::user::interaction * pui)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::merge(::user::interaction * pui)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   ::user::interaction * interaction_base::top_child()
   {

      return last_child();

   }


   ::user::interaction * interaction_base::bottom_child()
   {

      return first_child();

   }


   ::user::interaction * interaction_base::above_sibling(::user::interaction * pui)
   {

      return next_sibling(pui);

   }


   ::user::interaction * interaction_base::above_sibling()
   {

      return next_sibling();

   }


   ::user::interaction * interaction_base::under_sibling(::user::interaction * pui)
   {

      return previous_sibling(pui);

   }


   ::user::interaction * interaction_base::under_sibling()
   {

      return previous_sibling();

   }

   ::user::interaction * interaction_base::top_sibling()
   {

      return last_sibling();

   }


   ::user::interaction * interaction_base::bottom_sibling()
   {

      return first_sibling();

      return NULL;

   }

   ::user::interaction * interaction_base::first_child()
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   ::user::interaction * interaction_base::last_child()
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }

   ::user::interaction * interaction_base::first_sibling()
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   ::user::interaction * interaction_base::last_sibling()
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   ::user::interaction * interaction_base::previous_sibling()
   {

      return NULL;

   }

   ::user::interaction * interaction_base::previous_sibling(::user::interaction * pui)
   {

      UNREFERENCED_PARAMETER(pui);

      return NULL;

   }

   ::user::interaction * interaction_base::next_sibling()
   {

      return NULL;

   }

   ::user::interaction * interaction_base::next_sibling(::user::interaction * pui)
   {

      UNREFERENCED_PARAMETER(pui);

      return NULL;

   }


   void interaction_base::mouse_hover_add(::user::interaction * pinterface)
   {

      ::exception::throw_interface_only(get_app());

   }

   void interaction_base::mouse_hover_remove(::user::interaction * pinterface)
   {

      ::exception::throw_interface_only(get_app());

   }


   bool interaction_base::on_keyboard_focus(::user::elemental * pfocus)
   {

      RedrawWindow();


      return true;

   }


   ::user::interaction * interaction_base::GetFocus()
   {

      ::user::elemental * pelemental = Session.get_keyboard_focus();

      if(pelemental == NULL)
      {

         return NULL;

      }

      return dynamic_cast < ::user::interaction * > (pelemental);

   }


   int32_t interaction_base::get_descendant_level(::user::interaction * pui)
   {

      ::exception::throw_interface_only(get_app());

      return -1;

   }


   bool interaction_base::is_descendant(::user::interaction * pui,bool bIncludeSelf)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   ::user::interaction * interaction_base::get_focusable_descendant(::user::interaction * pui)
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   void interaction_base::_001OnTriggerMouseInside()
   {

      ::exception::throw_interface_only(get_app());

   }


   bool interaction_base::update_data(bool bSaveAndValidate)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   string interaction_base::get_window_default_matter()
   {

      ::exception::throw_interface_only(get_app());

      return "";

   }


   string interaction_base::get_window_icon_matter()
   {

      ::exception::throw_interface_only(get_app());

      return get_window_default_matter();

   }


   uint32_t interaction_base::get_window_default_style()
   {

      ::exception::throw_interface_only(get_app());

      return 0;

   }


   ::user::interaction_base::e_type interaction_base::get_window_type()
   {

      return type_window;

   }


   bool interaction_base::post_simple_command(e_simple_command ecommand,lparam lparam)
   {

      post_message(message_simple_command,(WPARAM)ecommand,lparam);

      return true;

   }


   bool interaction_base::OnCommand(::message::base * pbase)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::OnNotify(::message::base * pbase)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::OnChildNotify(::message::base * pbase)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::on_simple_command(e_simple_command ecommand,lparam lparam,LRESULT & lresult)
   {

      if(ecommand == simple_command_full_screen)
      {

         WfiFullScreen();

         return true;

      }

      return false;

   }


   bool interaction_base::is_selected(::data::item * pitem)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::_001HasCommandHandler(id id)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::track_popup_menu(::user::menu_base_item * pitem,int32_t iFlags)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::track_popup_menu(::xml::node * lpnode,int32_t iFlags)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::track_popup_xml_matter_menu(const char * pszMatter,int32_t iFlags)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }



   bool interaction_base::track_popup_menu(::user::menu_base_item * pitem,int32_t iFlags,signal_details * pobj)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::track_popup_menu(::xml::node * lpnode,int32_t iFlags,signal_details * pobj)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::track_popup_xml_matter_menu(const char * pszMatter,int32_t iFlags,signal_details * pobj)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::track_popup_menu(::user::menu_base_item * pitem,int32_t iFlags, POINT pt)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }

   bool interaction_base::track_popup_menu(::xml::node * lpnode,int32_t iFlags, POINT pt)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::track_popup_xml_matter_menu(const char * pszMatter,int32_t iFlags, POINT pt)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   void interaction_base::WfiEnableFullScreen(bool bEnable)
   {

      ::exception::throw_interface_only(get_app());

   }


   bool interaction_base::WfiIsFullScreenEnabled()
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::WfiClose()
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }
   bool interaction_base::Wfi(EAppearance eapperance)
   {

      return false;

   }


   bool interaction_base::WfiDock(EAppearance eapperance)
   {

      return false;

   }


   bool interaction_base::WfiRestore(bool bForceNormal)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::WfiMinimize()
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::WfiMaximize()
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::WfiFullScreen()
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::WfiUp()
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::WfiDown()
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::WfiNotifyIcon()
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   EAppearance interaction_base::get_appearance()
   {

      return AppearanceNone;

   }


   EAppearance interaction_base::get_appearance_before()
   {

      return AppearanceNone;

   }


   bool interaction_base::set_appearance(EAppearance eappearance)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_base::set_appearance_before(EAppearance eappearance)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   void interaction_base::show_keyboard(bool bShow)
   {

      UNREFERENCED_PARAMETER(bShow);

   }


   LRESULT interaction_base::call_message_handler(UINT message,WPARAM wparam,LPARAM lparam)
   {

      ::exception::throw_interface_only(get_app());

      return 0;

   }


   void interaction_base::keep_alive(::object * pliveobject)
   {

      ::exception::throw_interface_only(get_app());

   }


   bool interaction_base::SetWindowPos(int_ptr z,int32_t x,int32_t y,int32_t cx,int32_t cy,UINT nFlags)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   id interaction_base::GetDlgCtrlId() const
   {

      ::exception::throw_interface_only(get_app());

      return id();

   }


   id interaction_base::SetDlgCtrlId(id id)
   {

      ::exception::throw_interface_only(get_app());

      return ::id();

   }


   oswindow interaction_base::get_safe_handle() const
   {

      if(::is_null(this))
         return NULL;

      return get_handle();

   }


   window_graphics * interaction_base::get_window_graphics()
   {

      return NULL;

   }


   bool interaction_base::is_composite()
   {

      return true; // optimistic response, assume always true alpha blendable

   }


   //void interaction_base::_user_message_handler(signal_details * pobj)
   //{
   //}

   //::message::PFN_DISPATCH_MESSAGE_HANDLER interaction_base::_calc_user_message_handler()
   //{
   //   return &::user::interaction_base::_user_message_handler;
   //}

   void interaction_base::_001OnTimer(::timer * ptimer)
   {



   }


   bool interaction_base::has_pending_graphical_update()
   {

      return false;

   }


   void interaction_base::on_after_graphical_update()
   {

   }


} // namespace user















