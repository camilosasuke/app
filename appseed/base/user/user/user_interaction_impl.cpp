//#include "framework.h" // from "base/user/user.h"
//#include "base/user/user.h"


namespace user
{


   interaction_impl::interaction_impl() :
      ::aura::timer_array(get_app())
   {

      user_common_construct();

   }


   interaction_impl::~interaction_impl()
   {

      m_guieptraMouseHover.m_pmutex = NULL;

   }


   void interaction_impl::user_common_construct()
   {

      m_dFps = 60.0;
      m_bIpcCopy = false;
      m_pmutex                               = new mutex(get_app());
      m_guieptraMouseHover.m_pmutex          = m_pmutex;
      m_bScreenRelativeMouseMessagePosition  = true;
      m_bTranslateMouseMessageCursor         = true;
      m_bComposite                           = true;
      m_bUpdateGraphics                      = false;
      m_oswindow                             = NULL;
      m_puiFocus                             = NULL;




   }


   mutex * interaction_impl::draw_mutex()
   {

      if (m_apmutexDraw == NULL)
      {

         m_apmutexDraw = new mutex(get_app());

      }

      return m_apmutexDraw;

   }


   ::user::interaction_impl * interaction_impl::get_user_interaction_impl()
   {

      return this;

   }


   bool interaction_impl::check_need_layout()
   {

      return m_pui->m_bNeedLayout || ::user::interaction_impl_base::check_need_layout();

   }


   void interaction_impl::clear_need_layout()
   {

      m_pui->m_bNeedLayout = false;

      ::user::interaction_impl_base::clear_need_layout();

   }


   void interaction_impl::set_need_layout()
   {

      m_pui->set_need_layout();

   }


   bool interaction_impl::create_message_queue(::user::interaction * pui, const char * pszName)
   {

      UNREFERENCED_PARAMETER(pszName);
      ::exception::throw_interface_only(get_app());

      return false;

   }


#ifdef WINDOWSEX


   bool interaction_impl::GetWindowInfo(PWINDOWINFO pwi) const
   {
      UNREFERENCED_PARAMETER(pwi);
      ::exception::throw_interface_only(get_app());
      return false;
   }

   bool interaction_impl::GetScrollBarInfo(LONG idObject,PSCROLLBARINFO psbi) const
   {
      UNREFERENCED_PARAMETER(idObject);
      UNREFERENCED_PARAMETER(psbi);
      ::exception::throw_interface_only(get_app());
      return false;
   }

   bool interaction_impl::GetTitleBarInfo(PTITLEBARINFO pti) const
   {
      UNREFERENCED_PARAMETER(pti);
      ::exception::throw_interface_only(get_app());
      return false;
   }

#endif

#if !defined(LINUX) && !defined(APPLEOS) && !defined(ANDROID)

   ::window_sp interaction_impl::GetAncestor(UINT gaFlags) const
   {
      UNREFERENCED_PARAMETER(gaFlags);
      ::exception::throw_interface_only(get_app());
      return NULL;
   }



   bool interaction_impl::AnimateWindow(uint32_t dwTime,uint32_t dwFlags)
   {
      UNREFERENCED_PARAMETER(dwTime);
      UNREFERENCED_PARAMETER(dwFlags);
      ::exception::throw_interface_only(get_app());
      return false;
   }

   bool interaction_impl::FlashWindowEx(uint32_t dwFlags,UINT uCount,uint32_t dwTimeout)
   {
      UNREFERENCED_PARAMETER(dwFlags);
      UNREFERENCED_PARAMETER(uCount);
      UNREFERENCED_PARAMETER(dwTimeout);
      ::exception::throw_interface_only(get_app());
      return false;
   }

   bool interaction_impl::SetLayeredWindowAttributes(COLORREF crKey,BYTE bAlpha,uint32_t dwFlags)
   {
      UNREFERENCED_PARAMETER(crKey);
      UNREFERENCED_PARAMETER(bAlpha);
      UNREFERENCED_PARAMETER(dwFlags);
      ::exception::throw_interface_only(get_app());
      return false;
   }

   bool interaction_impl::UpdateLayeredWindow(::draw2d::graphics * pDCDst,POINT * pptDst,SIZE * psize,
      ::draw2d::graphics * pDCSrc,POINT * pptSrc,COLORREF crKey,BLENDFUNCTION * pblend,uint32_t dwFlags)
   {
      UNREFERENCED_PARAMETER(pDCDst);
      UNREFERENCED_PARAMETER(pptDst);
      UNREFERENCED_PARAMETER(psize);
      UNREFERENCED_PARAMETER(pDCSrc);
      UNREFERENCED_PARAMETER(pptSrc);
      UNREFERENCED_PARAMETER(crKey);
      UNREFERENCED_PARAMETER(pblend);
      UNREFERENCED_PARAMETER(dwFlags);
      ::exception::throw_interface_only(get_app());

      return false;
   }

   bool interaction_impl::GetLayeredWindowAttributes(COLORREF * pcrKey,BYTE * pbAlpha,uint32_t * pdwFlags) const
   {
      UNREFERENCED_PARAMETER(pcrKey);
      UNREFERENCED_PARAMETER(pbAlpha);
      UNREFERENCED_PARAMETER(pdwFlags);
      ::exception::throw_interface_only(get_app());

      return false;
   }

   bool interaction_impl::PrintWindow(::draw2d::graphics * pgraphics,UINT nFlags) const
   {
      UNREFERENCED_PARAMETER(pgraphics);
      UNREFERENCED_PARAMETER(nFlags);
      ::exception::throw_interface_only(get_app());

      return false;
   }

#endif

   LRESULT interaction_impl::Default()
   {
      ::exception::throw_interface_only(get_app());
      return 0;
   }

   void interaction_impl::pre_subclass_window()
   {
      ::exception::throw_interface_only(get_app());
   }


   ::user::interaction_impl * interaction_impl::from_os_data(void * pdata)
   {

      UNREFERENCED_PARAMETER(pdata);

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   void * interaction_impl::get_os_data() const
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   bool interaction_impl::create_window_ex(::user::interaction * pui, uint32_t dwExStyle,const char * lpszClassName,const char * lpszWindowName,uint32_t dwStyle,const RECT & rect,::user::interaction * pParentWnd,id id,LPVOID lpParam /* = NULL */)
   {
      UNREFERENCED_PARAMETER(dwExStyle);
      UNREFERENCED_PARAMETER(lpszClassName);
      UNREFERENCED_PARAMETER(lpszWindowName);
      UNREFERENCED_PARAMETER(dwStyle);
      UNREFERENCED_PARAMETER(rect);
      UNREFERENCED_PARAMETER(pParentWnd);
      UNREFERENCED_PARAMETER(id);
      UNREFERENCED_PARAMETER(lpParam);
      ::exception::throw_interface_only(get_app());

      return false;
   }



   // for child windows
   bool interaction_impl::pre_create_window(::user::create_struct & cs)
   {
      UNREFERENCED_PARAMETER(cs);
      return true;
   }

   bool interaction_impl::create_window(::user::interaction * pui, const char * lpszClassName,const char * lpszWindowName,uint32_t dwStyle,const RECT & rect,::user::interaction * pParentWnd,id id, ::create * pcreate)
   {
      UNREFERENCED_PARAMETER(lpszClassName);
      UNREFERENCED_PARAMETER(lpszWindowName);
      UNREFERENCED_PARAMETER(dwStyle);
      UNREFERENCED_PARAMETER(rect);
      UNREFERENCED_PARAMETER(pParentWnd);
      UNREFERENCED_PARAMETER(id);
      UNREFERENCED_PARAMETER(pcreate);
      ::exception::throw_interface_only(get_app());

      return false;

   }


   void interaction_impl::prio_install_message_handling(::message::dispatch * pinterface)
   {

      ::user::interaction_impl_base::prio_install_message_handling(pinterface);

   }

   void interaction_impl::last_install_message_handling(::message::dispatch * pinterface)
   {

      IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &interaction_impl::_001OnCreate);

      ::user::interaction_impl_base::last_install_message_handling(pinterface);

      IGUI_WIN_MSG_LINK(WM_SHOWWINDOW, pinterface, this, &interaction_impl::_001OnShowWindow);

   }


   void interaction_impl::_000OnMouseLeave(signal_details * pobj)
   {

      m_pui->m_bMouseHover = false;

      if (!m_pui->m_bTransparentMouseEvents)
      {

         sp(::user::interaction) pui;

         rect rectUi;

         point ptCursor;

         Session.get_cursor_pos(ptCursor);

         ::user::interaction_spa uia;

         {

            synch_lock sl(m_pmutex);

            uia = m_guieptraMouseHover;

            m_guieptraMouseHover.remove_all();

         }

         for(auto & pui : uia)
         {

            try
            {

               pui->send_message(WM_MOUSELEAVE);

            }
            catch (...)
            {

            }

         }


      }

   }

   
   void interaction_impl::_001OnTriggerMouseInside()
   {

      m_pui->m_bMouseHover = true;

   }


   void interaction_impl::_008OnMouse(::message::mouse * pmouse)
   {

      bool bPointInside;
      
      bPointInside = m_pui->_001IsPointInside(pmouse->m_pt);

      if (!bPointInside)
      {

         if (pmouse->m_pwnd == m_pui)
         {

            bPointInside = true;

         }

      }

      
      {

         ::user::interaction_spa uia = m_guieptraMouseHover;

         for(auto & pui : uia)
         {

            if (!pui->_001IsPointInside(pmouse->m_pt))
            {

               pui->send_message(WM_MOUSELEAVE);

               {

                  synch_lock sl(m_pmutex);

                  m_guieptraMouseHover.remove(pui);

               }

            }

         }

      }

      oswindow oswindow = Session.get_capture();

      if (oswindow == NULL || oswindow == get_handle())
      {

         if (!m_pui->m_bMouseHover && bPointInside)
         {

            m_pui->_001OnTriggerMouseInside();

         }

      }

      if(oswindow != NULL)
      {

         ::user::interaction * puiCapture = Session.m_puiCapture;

         if (puiCapture == NULL)
         {

            puiCapture = System.ui_from_handle(oswindow);

            if (puiCapture == NULL)
            {

               puiCapture = m_pui;

            }

         }

         try
         {

            (puiCapture->m_pimpl->*puiCapture->m_pimpl->m_pfnDispatchWindowProc)(dynamic_cast <::signal_details *> (pmouse));

         }
         catch (...)
         {

         }

         return;

      }

      if (m_pui != NULL)
      {

         m_pui->_000OnMouse(pmouse);

      }

      if (pmouse->m_bRet)
      {

         return;

      }

      pmouse->set_lresult(DefWindowProc(pmouse->m_uiMessage, pmouse->m_wparam, pmouse->m_lparam));

   }


   void interaction_impl::mouse_hover_add(::user::interaction * pinterface)
   {

      if (pinterface == NULL)
      {

         return;

      }

      synch_lock sl(m_pmutex);

      m_guieptraMouseHover.add_unique(pinterface);

   }


   void interaction_impl::mouse_hover_remove(::user::interaction * pinterface)
   {

      synch_lock sl(m_pmutex);

      m_guieptraMouseHover.remove(pinterface);

   }


   void interaction_impl::_001OnCaptureChanged(signal_details * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

      Session.m_puiCapture = NULL;

   }


   void interaction_impl::install_message_handling(::message::dispatch * pdispatch)
   {

      ::user::interaction_impl_base::install_message_handling(pdispatch);

      if (!m_pui->m_bMessageWindow)
      {

         IGUI_WIN_MSG_LINK(WM_CAPTURECHANGED, pdispatch, this, &interaction_impl::_001OnCaptureChanged);

      }

   }


   void interaction_impl::_001OnDestroy(signal_details * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

   }


   // WM_NCDESTROY is the absolute LAST message sent.
   void interaction_impl::PostNcDestroy()
   {
     ::user::interaction_base::PostNcDestroy();
   }


   void interaction_impl::on_final_release()
   {

   }


   void interaction_impl::assert_valid() const
   {

      ::user::interaction_impl_base::assert_valid();

   }


   void interaction_impl::dump(dump_context & dumpcontext) const
   {

      ::user::interaction_impl_base::dump(dumpcontext);

   }


   bool interaction_impl::DestroyWindow()
   {

      bool bResult = ::user::interaction_impl_base::DestroyWindow();

      return bResult;

   }


   /////////////////////////////////////////////////////////////////////////////
   // Default interaction_impl implementation


   LRESULT interaction_impl::DefWindowProc(UINT nMsg,WPARAM wParam,lparam lParam)
   {

      return 0;

   }




   void interaction_impl::pre_translate_message(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      ::exception::throw_interface_only(get_app());
   }

   void interaction_impl::GetWindowText(string & rString)
   {
      UNREFERENCED_PARAMETER(rString);
      ::exception::throw_interface_only(get_app());
   }

   int32_t interaction_impl::GetChildByIdText(int32_t nID,string & rString) const
   {
      UNREFERENCED_PARAMETER(nID);
      UNREFERENCED_PARAMETER(rString);
      ::exception::throw_interface_only(get_app());

      return 0;
   }

#ifdef WINDOWSEX

   bool interaction_impl::GetWindowPlacement(WINDOWPLACEMENT* lpwndpl)
   {
      UNREFERENCED_PARAMETER(lpwndpl);
      ::exception::throw_interface_only(get_app());

      return false;
   }

   bool interaction_impl::SetWindowPlacement(const WINDOWPLACEMENT* lpwndpl)
   {
      UNREFERENCED_PARAMETER(lpwndpl);
      ::exception::throw_interface_only(get_app());

      return false;
   }

#endif

   /////////////////////////////////////////////////////////////////////////////
   // interaction_impl will delegate owner draw messages to self drawing controls
//#ifdef WINDOWSEX
//   // Drawing: for all 4 control types
//   void interaction_impl::OnDrawItem(int32_t /*nIDCtl*/,LPDRAWITEMSTRUCT lpDrawItemStruct)
//   {
//      UNREFERENCED_PARAMETER(lpDrawItemStruct);
//      ::exception::throw_interface_only(get_app());
//   }
//
//   // Drawing: for all 4 control types
//   int32_t interaction_impl::OnCompareItem(int32_t /*nIDCtl*/,LPCOMPAREITEMSTRUCT lpCompareItemStruct)
//   {
//      UNREFERENCED_PARAMETER(lpCompareItemStruct);
//      ::exception::throw_interface_only(get_app());
//   }
//
//   void interaction_impl::OnDeleteItem(int32_t /*nIDCtl*/,LPDELETEITEMSTRUCT lpDeleteItemStruct)
//   {
//      UNREFERENCED_PARAMETER(lpDeleteItemStruct);
//      ::exception::throw_interface_only(get_app());
//   }
//
//   // Measure item implementation relies on unique control/menu IDs
//   void interaction_impl::OnMeasureItem(int32_t /*nIDCtl*/,LPMEASUREITEMSTRUCT lpMeasureItemStruct)
//   {
//      UNREFERENCED_PARAMETER(lpMeasureItemStruct);
//      ::exception::throw_interface_only(get_app());
//   }
//#endif


//   LRESULT interaction_impl::OnNTCtlColor(WPARAM wParam,LPARAM lParam)
//   {
//      UNREFERENCED_PARAMETER(wParam);
//      UNREFERENCED_PARAMETER(lParam);
//      ::exception::throw_interface_only(get_app());
//   }
//
//#ifdef WINDOWSEX
//   /////////////////////////////////////////////////////////////////////////////
//   // interaction_impl extensions for help support
//   void interaction_impl::WinHelp(uint_ptr dwData,UINT nCmd)
//   {
//      UNREFERENCED_PARAMETER(dwData);
//      UNREFERENCED_PARAMETER(nCmd);
//      ::exception::throw_interface_only(get_app());
//   }
//
//   void interaction_impl::HtmlHelp(uint_ptr dwData,UINT nCmd)
//   {
//      UNREFERENCED_PARAMETER(dwData);
//      UNREFERENCED_PARAMETER(nCmd);
//      ::exception::throw_interface_only(get_app());
//   }
//#endif
//
  void interaction_impl::PrepareForHelp()
   {
      ::exception::throw_interface_only(get_app());
   }
//
//#ifdef WINDOWSEX
//   void interaction_impl::WinHelpInternal(uint_ptr dwData,UINT nCmd)
//   {
//      UNREFERENCED_PARAMETER(dwData);
//      UNREFERENCED_PARAMETER(nCmd);
//      ::exception::throw_interface_only(get_app());
//   }
//#endif


   bool interaction_impl::_001OnCmdMsg(::aura::cmd_msg * pcmdmsg)
   {

      UNREFERENCED_PARAMETER(pcmdmsg);

      ::exception::throw_interface_only(get_app());

      return false;

   }


   void interaction_impl::_002OnDraw(signal_details * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

      ::exception::throw_interface_only(get_app());

   }


   /////////////////////////////////////////////////////////////////////////////
   // main message_handler implementation

   void interaction_impl::message_handler(signal_details * pobj)
   {

      ::command_target::message_handler(pobj);

   }


   //bool interaction_impl::OnNotify(::message::base * pbase)
   //{

   //   UNREFERENCED_PARAMETER(pbase);

   //   ::exception::throw_interface_only(get_app());

   //}


   bool interaction_impl::IsTopParentActive()
   {
      ::exception::throw_interface_only(get_app());

      return false;
   }

   void interaction_impl::ActivateTopParent()
   {
      ::exception::throw_interface_only(get_app());
   }


   int32_t interaction_impl::message_box(const char * lpszText,const char * lpszCaption,UINT nType)
   {
      UNREFERENCED_PARAMETER(lpszText);
      UNREFERENCED_PARAMETER(lpszCaption);
      UNREFERENCED_PARAMETER(nType);
      ::exception::throw_interface_only(get_app());

      return 0;
   }

   /////////////////////////////////////////////////////////////////////////////
   // Scroll bar helpers
   //  hook for interaction_impl functions
   //    only works for derived class (eg: ::user::impact) that override 'GetScrollBarCtrl'
   // if the interaction_impl doesn't have a _visible_ windows scrollbar - then
   //   look for a sibling with the appropriate ID

   /*   CScrollBar* interaction_impl::GetScrollBarCtrl(int32_t) const
   {
   ::exception::throw_interface_only(get_app());
   }*/

   int32_t interaction_impl::SetScrollPos(int32_t nBar,int32_t nPos,bool bRedraw)
   {
      UNREFERENCED_PARAMETER(nBar);
      UNREFERENCED_PARAMETER(nPos);
      UNREFERENCED_PARAMETER(bRedraw);
      ::exception::throw_interface_only(get_app());

      return 0;
   }

   int32_t interaction_impl::GetScrollPos(int32_t nBar) const
   {
      UNREFERENCED_PARAMETER(nBar);
      ::exception::throw_interface_only(get_app());

      return 0;
   }

   void interaction_impl::SetScrollRange(int32_t nBar,int32_t nMinPos,int32_t nMaxPos,bool bRedraw)
   {
      UNREFERENCED_PARAMETER(nBar);
      UNREFERENCED_PARAMETER(nMinPos);
      UNREFERENCED_PARAMETER(nMaxPos);
      UNREFERENCED_PARAMETER(bRedraw);
      ::exception::throw_interface_only(get_app());


   }


   void interaction_impl::GetScrollRange(int32_t nBar,LPINT lpMinPos,LPINT lpMaxPos) const
   {

      UNREFERENCED_PARAMETER(nBar);
      UNREFERENCED_PARAMETER(lpMinPos);
      UNREFERENCED_PARAMETER(lpMaxPos);
      ::exception::throw_interface_only(get_app());

   }


   // Turn on/off non-control scrollbars
   //   for WS_?SCROLL scrollbars - show/hide them
   //   for control scrollbar - enable/disable them
   void interaction_impl::EnableScrollBarCtrl(int32_t nBar,bool bEnable)
   {
      UNREFERENCED_PARAMETER(nBar);
      UNREFERENCED_PARAMETER(bEnable);
      ::exception::throw_interface_only(get_app());
   }


#ifdef WINDOWSEX


   bool interaction_impl::SetScrollInfo(int32_t nBar,LPSCROLLINFO lpScrollInfo,bool bRedraw)
   {

      UNREFERENCED_PARAMETER(nBar);
      UNREFERENCED_PARAMETER(lpScrollInfo);
      UNREFERENCED_PARAMETER(bRedraw);
      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_impl::GetScrollInfo(int32_t nBar,LPSCROLLINFO lpScrollInfo,UINT nMask)
   {

      UNREFERENCED_PARAMETER(nBar);
      UNREFERENCED_PARAMETER(lpScrollInfo);
      UNREFERENCED_PARAMETER(nMask);
      ::exception::throw_interface_only(get_app());

      return false;

   }

#endif

   int32_t interaction_impl::GetScrollLimit(int32_t nBar)
   {
      UNREFERENCED_PARAMETER(nBar);
      ::exception::throw_interface_only(get_app());

      return 0;
   }

   void interaction_impl::ScrollWindow(int32_t xAmount,int32_t yAmount,LPCRECT lpRect,LPCRECT lpClipRect)
   {
      UNREFERENCED_PARAMETER(xAmount);
      UNREFERENCED_PARAMETER(yAmount);
      UNREFERENCED_PARAMETER(lpRect);
      UNREFERENCED_PARAMETER(lpClipRect);
      ::exception::throw_interface_only(get_app());
   }



   void interaction_impl::CalcWindowRect(LPRECT lpClientRect,UINT nAdjustType)
   {
      UNREFERENCED_PARAMETER(lpClientRect);
      UNREFERENCED_PARAMETER(nAdjustType);
      ::exception::throw_interface_only(get_app());
   }

   /////////////////////////////////////////////////////////////////////////////
   // Special keyboard/system command processing

   bool interaction_impl::HandleFloatingSysCommand(UINT nID,LPARAM lParam)
   {
      UNREFERENCED_PARAMETER(nID);
      UNREFERENCED_PARAMETER(lParam);
      ::exception::throw_interface_only(get_app());

      return false;

   }

   bool interaction_impl::OnChildNotify(::message::base * pbase)
   {

      UNREFERENCED_PARAMETER(pbase);

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_impl::ReflectChildNotify(::message::base * pbase)
   {

      UNREFERENCED_PARAMETER(pbase);

      ::exception::throw_interface_only(get_app());

      return false;

   }


//   void interaction_impl::OnParentNotify(UINT message,LPARAM lParam)
//   {
//      UNREFERENCED_PARAMETER(message);
//      UNREFERENCED_PARAMETER(lParam);
//      ::exception::throw_interface_only(get_app());
//   }
//
//   void interaction_impl::OnSetFocus(::window_sp)
//   {
//      ::exception::throw_interface_only(get_app());
//   }
//
//   LRESULT interaction_impl::OnActivateTopLevel(WPARAM wParam,LPARAM)
//   {
//      UNREFERENCED_PARAMETER(wParam);
//      ::exception::throw_interface_only(get_app());
//   }
//
//   void interaction_impl::OnSysColorChange()
//   {
//      ::exception::throw_interface_only(get_app());
//   }
//
//   void interaction_impl::OnSettingChange(UINT uFlags,const char * lpszSection)
//   {
//      UNREFERENCED_PARAMETER(uFlags);
//      UNREFERENCED_PARAMETER(lpszSection);
//      ::exception::throw_interface_only(get_app());
//   }
//
//#ifdef WINDOWS
//   void interaction_impl::OnDevModeChange(__in LPTSTR lpDeviceName)
//   {
//      UNREFERENCED_PARAMETER(lpDeviceName);
//      ::exception::throw_interface_only(get_app());
//   }
//
//#ifdef WINDOWSEX
//   bool interaction_impl::OnHelpInfo(HELPINFO* /*pHelpInfo*/)
//   {
//      ::exception::throw_interface_only(get_app());
//   }
//#endif
//#endif
//
//   LRESULT interaction_impl::OnDisplayChange(WPARAM,LPARAM)
//   {
//      //   ::exception::throw_interface_only(get_app());
//
//      return 0;
//
//   }
//
//   LRESULT interaction_impl::OnDragList(WPARAM,LPARAM lParam)
//   {
//      UNREFERENCED_PARAMETER(lParam);
//      ::exception::throw_interface_only(get_app());
//   }

   /*   void interaction_impl::OnHScroll(UINT, UINT, CScrollBar* pScrollBar)
   {
   UNREFERENCED_PARAMETER(pScrollBar);
   ::exception::throw_interface_only(get_app());
   }

   void interaction_impl::OnVScroll(UINT, UINT, CScrollBar* pScrollBar)
   {
   UNREFERENCED_PARAMETER(pScrollBar);
   ::exception::throw_interface_only(get_app());
   }
   */

   void interaction_impl::_001OnDeferPaintLayeredWindowBackground(::draw2d::dib * pdib)
   {
      UNREFERENCED_PARAMETER(pdib);
      ::exception::throw_interface_only(get_app());
   }

   void interaction_impl::_001DeferPaintLayeredWindowBackground(::draw2d::dib * pdib)
   {
      UNREFERENCED_PARAMETER(pdib);
      ::exception::throw_interface_only(get_app());
   }

   void interaction_impl::_001OnPaint(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      ::exception::throw_interface_only(get_app());
   }

   void interaction_impl::_001OnPrint(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      ::exception::throw_interface_only(get_app());
   }

   //void interaction_impl::OnEnterIdle(UINT /*nWhy*/,::window_sp /*pWho*/)
   //{
   //   ::exception::throw_interface_only(get_app());
   //}

   //void * interaction_impl::OnCtlColor(::draw2d::graphics *,::window_sp pwindow,UINT)
   //{
   //   UNREFERENCED_PARAMETER(pwindow);
   //   ::exception::throw_interface_only(get_app());
   //}


   /////////////////////////////////////////////////////////////////////////////
   // 'dialog data' support

   bool interaction_impl::update_data(bool bSaveAndValidate)
   {
      UNREFERENCED_PARAMETER(bSaveAndValidate);
      ::exception::throw_interface_only(get_app());

      return false;

   }

   /////////////////////////////////////////////////////////////////////////////
   // Centering dialog support (works for any non-child interaction_impl)

   void interaction_impl::CenterWindow(::user::interaction * pAlternateOwner)
   {
      UNREFERENCED_PARAMETER(pAlternateOwner);
      ::exception::throw_interface_only(get_app());



   }

   bool interaction_impl::CheckAutoCenter()
   {
      ::exception::throw_interface_only(get_app());

      return false;

   }

   /////////////////////////////////////////////////////////////////////////////
   // Dialog initialization support

   bool interaction_impl::ExecuteDlgInit(const char * lpszResourceName)
   {
      UNREFERENCED_PARAMETER(lpszResourceName);
      ::exception::throw_interface_only(get_app());

      return false;

   }

   bool interaction_impl::ExecuteDlgInit(LPVOID lpResource)
   {
      UNREFERENCED_PARAMETER(lpResource);
      ::exception::throw_interface_only(get_app());

      return false;

   }

   void interaction_impl::UpdateDialogControls(command_target * pTarget,bool bDisableIfNoHndler)
   {
      UNREFERENCED_PARAMETER(pTarget);
      UNREFERENCED_PARAMETER(bDisableIfNoHndler);

      ::exception::throw_interface_only(get_app());

   }



   bool interaction_impl::subclass_window(oswindow oswindow)
   {
      UNREFERENCED_PARAMETER(oswindow);
      ::exception::throw_interface_only(get_app());

      return false;

   }

   bool interaction_impl::SubclassDlgItem(UINT nID,::window_sp pParent)
   {
      UNREFERENCED_PARAMETER(nID);
      UNREFERENCED_PARAMETER(pParent);
      ::exception::throw_interface_only(get_app());

      return false;

   }

   oswindow interaction_impl::unsubclass_window()
   {
      ::exception::throw_interface_only(get_app());
      return NULL;
   }

   //bool interaction_impl::IsChild(::user::interaction * pui) const
   //{
   //   UNREFERENCED_PARAMETER(pui);
   //   ::exception::throw_interface_only(get_app());

   //   return false;

   //}

   bool interaction_impl::IsWindow() const
   {
      ::exception::throw_interface_only(get_app());

      return false;

   }

   /*   ::visual::icon * interaction_impl::SetIcon(::visual::icon * picon, bool bBigIcon)
   {
   ::exception::throw_interface_only(get_app());
   }

   ::visual::icon * interaction_impl::GetIcon(bool bBigIcon) const
   {
   ::exception::throw_interface_only(get_app());
   }*/


   bool interaction_impl::SetWindowPos(int_ptr z,int32_t x,int32_t y,int32_t cx,int32_t cy,UINT nFlags)
   {

      return ::user::interaction_impl_base::SetWindowPos(z, x, y, cx, cy, nFlags);

   }


   id interaction_impl::SetDlgCtrlId(id id)
   {

      UNREFERENCED_PARAMETER(id);

      ::exception::throw_interface_only(get_app());

      return ::id();

   }


   id interaction_impl::GetDlgCtrlId() const
   {

      ::exception::throw_interface_only(get_app());

      return ::id();

   }



   bool interaction_impl::ShowWindow(int32_t nCmdShow)
   {

      UNREFERENCED_PARAMETER(nCmdShow);
      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_impl::WfiIsIconic()
   {

      //::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_impl::WfiIsZoomed()
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   //::user::interaction * interaction_impl::get_parent() const
   //{

   //   return NULL;

   //}


   LONG interaction_impl::get_window_long(int32_t nIndex) const
   {

      return (LONG)get_window_long_ptr(nIndex);

   }


   LONG interaction_impl::set_window_long(int32_t nIndex,LONG lValue)
   {

      return (LONG)set_window_long_ptr(nIndex, lValue);

   }


   LONG_PTR interaction_impl::get_window_long_ptr(int32_t nIndex) const
   {

      return ::user::interaction_impl_base::get_window_long_ptr(nIndex);

   }


   LONG_PTR interaction_impl::set_window_long_ptr(int32_t nIndex,LONG_PTR lValue)
   {
      
      return ::user::interaction_impl_base::set_window_long_ptr(nIndex, lValue);

   }


   bool interaction_impl::ReleaseCapture()
   {

#ifdef METROWIN
      ::WinReleaseCapture();
#else
      ::ReleaseCapture();
#endif

      Session.m_puiCapture = NULL;

      return true;

   }


   ::user::interaction * interaction_impl::GetCapture()
   {

#ifdef METROWIN

      oswindow oswindow = ::WinGetCapture();

#else

      oswindow oswindow = ::GetCapture();

#endif

      if (oswindow == NULL)
      {

         return NULL;

      }

      ::user::interaction * pui = System.ui_from_handle(oswindow);

      if (pui != NULL)
      {

         if (Session.m_puiCapture != NULL)
         {

            return Session.m_puiCapture;

         }
         else
         {

            return pui;

         }

      }

      return NULL;

   }


   bool interaction_impl::SetCapture(::user::interaction * pui)
   {

      if (pui == NULL)
      {

         pui = m_pui;

      }

      if (!IsWindow())
      {

         return NULL;

      }

#ifdef METROWIN
      oswindow w = ::WinSetCapture(get_handle());
#else
      oswindow w = ::SetCapture(get_handle());
#endif

      Session.m_puiCapture = pui;

      return true;

   }


   // interaction_impl
   /* interaction_impl::operator oswindow() const
   { return this == NULL ? NULL : get_handle(); }*/
   bool interaction_impl::operator==(const interaction_impl& wnd) const
   {
      UNREFERENCED_PARAMETER(wnd);
      ::exception::throw_interface_only(get_app());
      return false;
   }

   bool interaction_impl::operator!=(const interaction_impl& wnd) const
   {
      UNREFERENCED_PARAMETER(wnd);
      ::exception::throw_interface_only(get_app());
      return false;
   }


   uint32_t interaction_impl::GetStyle() const
   {

      return ::user::interaction_impl_base::GetStyle();

   }


   uint32_t interaction_impl::GetExStyle() const
   {

      return ::user::interaction_impl_base::GetExStyle();

   }


   bool interaction_impl::ModifyStyle(uint32_t dwRemove, uint32_t dwAdd, UINT nFlags)
   {
      
      if (!IsWindow())
      {
       
         return false;
         
      }
      
      DWORD dw = get_window_long(GWL_STYLE);
      
      dw &= ~dwRemove;
      
      dw |= dwAdd;
      
      set_window_long(GWL_STYLE, dw);
      
      if(nFlags != 0)
      {
         
         m_iShowFlags |= nFlags;
         
         m_bShowFlags = true;
         
      }
      
      return false;
      
   }
   

   bool interaction_impl::ModifyStyleEx(uint32_t dwRemove,uint32_t dwAdd,UINT nFlags)
   {
      
      if (!IsWindow())
      {
         
         return false;
         
      }
      
      DWORD dw = get_window_long(GWL_EXSTYLE);
      
      dw &= ~dwRemove;
      
      dw |= dwAdd;
      
      set_window_long(GWL_EXSTYLE, dw);
      
      if(nFlags != 0)
      {
         
         m_iShowFlags |= nFlags;
         
         m_bShowFlags = true;
         
      }
      
      return false;
      
   }

   void interaction_impl::set_owner(::user::interaction * pOwnerWnd)
   {
      UNREFERENCED_PARAMETER(pOwnerWnd);
      ::exception::throw_interface_only(get_app());
   }


   LRESULT interaction_impl::send_message(UINT message, WPARAM wparam, lparam lparam)
   {

      ::smart_pointer < ::message::base > spbase;

      spbase = m_pui->get_base(message, wparam, lparam);

      if(m_pui->WfiIsMoving())
      {
         TRACE("moving: skip walk pre translate tree");
      }
      else if(m_pui->WfiIsSizing())
      {
         TRACE("sizing: skip walk pre translate tree");
      }
      else
      {

         m_pui->walk_pre_translate_tree(spbase);

         if(spbase->m_bRet)
            return spbase->get_lresult();

      }

      message_handler(spbase);

      return spbase->get_lresult();

   }


#ifdef LINUX

   LRESULT interaction_impl::send_message(XEvent * pevent)
   {
      UNREFERENCED_PARAMETER(pevent);
      ::exception::throw_interface_only(get_app());
   }

#endif

   bool interaction_impl::post_message(UINT message,WPARAM wParam,lparam lParam)
   {
      UNREFERENCED_PARAMETER(message);
      UNREFERENCED_PARAMETER(wParam);
      UNREFERENCED_PARAMETER(lParam);
      ::exception::throw_interface_only(get_app());

      return false;
   }

   bool interaction_impl::DragDetect(POINT pt) const
   {
      UNREFERENCED_PARAMETER(pt);
      ::exception::throw_interface_only(get_app());
      return false;
   }


   void interaction_impl::set_window_text(const char * lpszString)
   {

      UNREFERENCED_PARAMETER(lpszString);

      ::exception::throw_interface_only(get_app());

   }

   strsize interaction_impl::GetWindowText(char * lpszString,int32_t nMaxCount)
   {
      UNREFERENCED_PARAMETER(lpszString);
      UNREFERENCED_PARAMETER(nMaxCount);
      ::exception::throw_interface_only(get_app());

      return 0;
   }


   strsize interaction_impl::GetWindowTextLength()
   {

      ::exception::throw_interface_only(get_app());
      return 0;

   }


   void interaction_impl::DragAcceptFiles(bool bAccept)
   {
      UNREFERENCED_PARAMETER(bAccept);
      ::exception::throw_interface_only(get_app());
   }


   UINT interaction_impl::ArrangeIconicWindows()
   {

      ::exception::throw_interface_only(get_app());

      return 0;

   }

   //int32_t interaction_impl::SetWindowRgn(HRGN hRgn,bool bRedraw)
   //{
   //   UNREFERENCED_PARAMETER(hRgn);
   //   UNREFERENCED_PARAMETER(bRedraw);
   //   ::exception::throw_interface_only(get_app());

   //   return 0;
   //}

   //int32_t interaction_impl::GetWindowRgn(HRGN hRgn)
   //{
   //   UNREFERENCED_PARAMETER(hRgn);
   //   ::exception::throw_interface_only(get_app());

   //   return 0;
   //}

   bool interaction_impl::BringWindowToTop()
   {
      ::exception::throw_interface_only(get_app());
      return false;
   }

   void interaction_impl::MapWindowPoints(::window_sp pwndTo,LPPOINT lpPoint,UINT nCount)
   {
      UNREFERENCED_PARAMETER(pwndTo);
      UNREFERENCED_PARAMETER(lpPoint);
      UNREFERENCED_PARAMETER(nCount);
      ::exception::throw_interface_only(get_app());
   }

   void interaction_impl::MapWindowPoints(::window_sp pwndTo,LPRECT lpRect)
   {
      UNREFERENCED_PARAMETER(pwndTo);
      UNREFERENCED_PARAMETER(lpRect);
      ::exception::throw_interface_only(get_app());
   }

   void interaction_impl::UpdateWindow()
   {
      ::exception::throw_interface_only(get_app());
   }

   void interaction_impl::SetRedraw(bool bRedraw)
   {
      UNREFERENCED_PARAMETER(bRedraw);
      ::exception::throw_interface_only(get_app());
   }

   bool interaction_impl::GetUpdateRect(LPRECT lpRect,bool bErase)
   {
      UNREFERENCED_PARAMETER(lpRect);
      UNREFERENCED_PARAMETER(bErase);
      ::exception::throw_interface_only(get_app());

      return false;
   }

   int32_t interaction_impl::GetUpdateRgn(::draw2d::region* pRgn,bool bErase)
   {
      UNREFERENCED_PARAMETER(pRgn);
      UNREFERENCED_PARAMETER(bErase);
      ::exception::throw_interface_only(get_app());

      return 0;
   }

   void interaction_impl::Invalidate(bool bErase)
   {
      UNREFERENCED_PARAMETER(bErase);
      ::exception::throw_interface_only(get_app());
   }

   void interaction_impl::InvalidateRect(LPCRECT lpRect,bool bErase)
   {
      UNREFERENCED_PARAMETER(lpRect);
      UNREFERENCED_PARAMETER(bErase);
      ::exception::throw_interface_only(get_app());
   }

   void interaction_impl::InvalidateRgn(::draw2d::region* pRgn,bool bErase)
   {
      UNREFERENCED_PARAMETER(pRgn);
      UNREFERENCED_PARAMETER(bErase);
      ::exception::throw_interface_only(get_app());
   }

   void interaction_impl::ValidateRect(LPCRECT lpRect)
   {
      UNREFERENCED_PARAMETER(lpRect);
      ::exception::throw_interface_only(get_app());
   }

   void interaction_impl::ValidateRgn(::draw2d::region* pRgn)
   {
      UNREFERENCED_PARAMETER(pRgn);
      ::exception::throw_interface_only(get_app());
   }

   bool interaction_impl::IsWindowVisible()
   {
      ::exception::throw_interface_only(get_app());
      return false;
   }

   void interaction_impl::ShowOwnedPopups(bool bShow)
   {
      UNREFERENCED_PARAMETER(bShow);
      ::exception::throw_interface_only(get_app());
   }

   void interaction_impl::SendMessageToDescendants(UINT message,WPARAM wParam,lparam lParam,bool bDeep,bool bOnlyPerm)
   {
      UNREFERENCED_PARAMETER(message);
      UNREFERENCED_PARAMETER(wParam);
      UNREFERENCED_PARAMETER(lParam);
      UNREFERENCED_PARAMETER(bDeep);
      UNREFERENCED_PARAMETER(bOnlyPerm);
      ::exception::throw_interface_only(get_app());
   }

   ::user::interaction * interaction_impl::GetDescendantWindow(id id) const
   {
      UNREFERENCED_PARAMETER(id);
    //  ::exception::throw_interface_only(get_app());

      return ::user::interaction_impl_base::GetDescendantWindow(id);
   }

   ::draw2d::graphics * interaction_impl::GetDCEx(::draw2d::region * prgnClip,uint32_t flags)
   {
      UNREFERENCED_PARAMETER(prgnClip);
      UNREFERENCED_PARAMETER(flags);
      ::exception::throw_interface_only(get_app());


      return NULL;
   }

   bool interaction_impl::LockWindowUpdate()
   {
      ::exception::throw_interface_only(get_app());
      return false;
   }

   void interaction_impl::UnlockWindowUpdate()
   {
      ::exception::throw_interface_only(get_app());
   }

   bool interaction_impl::RedrawWindow(LPCRECT lpRectUpdate,::draw2d::region * prgnUpdate,UINT flags)
   {
      UNREFERENCED_PARAMETER(lpRectUpdate);
      UNREFERENCED_PARAMETER(prgnUpdate);
      UNREFERENCED_PARAMETER(flags);
      ::exception::throw_interface_only(get_app());
      return false;
   }

   bool interaction_impl::EnableScrollBar(int32_t nSBFlags,UINT nArrowFlags)
   {
      UNREFERENCED_PARAMETER(nSBFlags);
      UNREFERENCED_PARAMETER(nArrowFlags);
      ::exception::throw_interface_only(get_app());
      return false;
   }

   bool interaction_impl::DrawAnimatedRects(int32_t idAni,CONST RECT *lprcFrom,CONST RECT * lprcTo)
   {
      UNREFERENCED_PARAMETER(idAni);
      UNREFERENCED_PARAMETER(lprcFrom);
      UNREFERENCED_PARAMETER(lprcTo);
      ::exception::throw_interface_only(get_app());
      return false;
   }

   bool interaction_impl::DrawCaption(::draw2d::graphics * pgraphics,LPCRECT lprc,UINT uFlags)
   {
      UNREFERENCED_PARAMETER(pgraphics);
      UNREFERENCED_PARAMETER(lprc);
      UNREFERENCED_PARAMETER(uFlags);
      ::exception::throw_interface_only(get_app());
      return false;
   }


   //bool interaction_impl::SetTimer(uint_ptr nIDEvent,UINT nElapse)
   //{

   //   return ::aura::timer_array::SetTimer(nIDEvent, nElapse);

   //}


   //bool interaction_impl::KillTimer(uint_ptr nIDEvent)
   //{

   //   return ::aura::timer_array::KillTimer(nIDEvent);

   //}


   bool interaction_impl::is_window_enabled()
   {

      return true;

   }

   bool interaction_impl::enable_window(bool bEnable)
   {
      UNREFERENCED_PARAMETER(bEnable);
      ::exception::throw_interface_only(get_app());
      return false;
   }

   ::user::interaction * interaction_impl::GetActiveWindow()
   {
      ::exception::throw_interface_only(get_app());

      return NULL;
   }

   ::user::interaction * interaction_impl::SetActiveWindow()
   {
      ::exception::throw_interface_only(get_app());

      return NULL;
   }


   bool interaction_impl::SetFocus()
   {

      return false;

   }


   void interaction_impl::CheckDlgButton(int32_t nIDButton,UINT nCheck)
   {
      UNREFERENCED_PARAMETER(nIDButton);
      UNREFERENCED_PARAMETER(nCheck);
      ::exception::throw_interface_only(get_app());
   }

   void interaction_impl::CheckRadioButton(int32_t nIDFirstButton,int32_t nIDLastButton,int32_t nIDCheckButton)
   {
      UNREFERENCED_PARAMETER(nIDFirstButton);
      UNREFERENCED_PARAMETER(nIDLastButton);
      UNREFERENCED_PARAMETER(nIDCheckButton);
      ::exception::throw_interface_only(get_app());
   }

   int32_t interaction_impl::DlgDirList(LPTSTR lpPathSpec,int32_t nIDListBox,int32_t nIDStaticPath,UINT nFileType)
   {
      UNREFERENCED_PARAMETER(lpPathSpec);
      UNREFERENCED_PARAMETER(nIDListBox);
      UNREFERENCED_PARAMETER(nIDStaticPath);
      UNREFERENCED_PARAMETER(nFileType);
      ::exception::throw_interface_only(get_app());
      return 0;
   }

   int32_t interaction_impl::DlgDirListComboBox(LPTSTR lpPathSpec,int32_t nIDComboBox,int32_t nIDStaticPath,UINT nFileType)
   {
      UNREFERENCED_PARAMETER(lpPathSpec);
      UNREFERENCED_PARAMETER(nIDComboBox);
      UNREFERENCED_PARAMETER(nIDStaticPath);
      UNREFERENCED_PARAMETER(nFileType);
      ::exception::throw_interface_only(get_app());

      return 0;
   }

   bool interaction_impl::DlgDirSelect(LPTSTR lpString,int32_t nSize,int32_t nIDListBox)
   {
      UNREFERENCED_PARAMETER(lpString);
      UNREFERENCED_PARAMETER(nSize);
      UNREFERENCED_PARAMETER(nIDListBox);
      ::exception::throw_interface_only(get_app());

      return false;
   }

   bool interaction_impl::DlgDirSelectComboBox(LPTSTR lpString,int32_t nSize,int32_t nIDComboBox)
   {
      UNREFERENCED_PARAMETER(lpString);
      UNREFERENCED_PARAMETER(nSize);
      UNREFERENCED_PARAMETER(nIDComboBox);
      ::exception::throw_interface_only(get_app());

      return false;
   }

   UINT interaction_impl::GetChildByIdInt(int32_t nID,bool* lpTrans,bool bSigned) const
   {
      UNREFERENCED_PARAMETER(nID);
      UNREFERENCED_PARAMETER(lpTrans);
      UNREFERENCED_PARAMETER(bSigned);
      ::exception::throw_interface_only(get_app());

      return 0;
   }

   int32_t interaction_impl::GetChildByIdText(int32_t nID,LPTSTR lpStr,int32_t nMaxCount) const
   {
      UNREFERENCED_PARAMETER(nID);
      UNREFERENCED_PARAMETER(lpStr);
      UNREFERENCED_PARAMETER(nMaxCount);
      ::exception::throw_interface_only(get_app());

      return 0;
   }

   ::window_sp interaction_impl::GetNextDlgGroupItem(::window_sp pWndCtl,bool bPrevious) const
   {
      UNREFERENCED_PARAMETER(pWndCtl);
      UNREFERENCED_PARAMETER(bPrevious);
      ::exception::throw_interface_only(get_app());

      return NULL;
   }

   ::window_sp interaction_impl::GetNextDlgTabItem(::window_sp pWndCtl,bool bPrevious) const
   {
      UNREFERENCED_PARAMETER(pWndCtl);
      UNREFERENCED_PARAMETER(bPrevious);
      ::exception::throw_interface_only(get_app());

      return NULL;
   }

   UINT interaction_impl::IsDlgButtonChecked(int32_t nIDButton) const
   {
      UNREFERENCED_PARAMETER(nIDButton);
      ::exception::throw_interface_only(get_app());

      return 0;
   }

   LPARAM interaction_impl::SendDlgItemMessage(int32_t nID,UINT message,WPARAM wParam,LPARAM lParam)
   {
      UNREFERENCED_PARAMETER(nID);
      UNREFERENCED_PARAMETER(message);
      UNREFERENCED_PARAMETER(wParam);
      UNREFERENCED_PARAMETER(lParam);
      ::exception::throw_interface_only(get_app());

      return 0;
   }

   void interaction_impl::SetDlgItemInt(int32_t nID,UINT nValue,bool bSigned)
   {
      UNREFERENCED_PARAMETER(nID);
      UNREFERENCED_PARAMETER(nValue);
      UNREFERENCED_PARAMETER(bSigned);
      ::exception::throw_interface_only(get_app());
   }

   void interaction_impl::SetDlgItemText(int32_t nID,const char * lpszString)
   {
      UNREFERENCED_PARAMETER(nID);
      UNREFERENCED_PARAMETER(lpszString);
      ::exception::throw_interface_only(get_app());
   }

   int32_t interaction_impl::ScrollWindowEx(int32_t dx,int32_t dy,
      LPCRECT lpRectScroll,LPCRECT lpRectClip,
      ::draw2d::region* prgnUpdate,LPRECT lpRectUpdate,UINT flags)
   {
      UNREFERENCED_PARAMETER(dx);
      UNREFERENCED_PARAMETER(dy);
      UNREFERENCED_PARAMETER(lpRectScroll);
      UNREFERENCED_PARAMETER(lpRectClip);
      UNREFERENCED_PARAMETER(prgnUpdate);
      UNREFERENCED_PARAMETER(lpRectUpdate);
      UNREFERENCED_PARAMETER(flags);
      ::exception::throw_interface_only(get_app());

      return 0;
   }

   void interaction_impl::ShowScrollBar(UINT nBar,bool bShow)
   {
      UNREFERENCED_PARAMETER(nBar);
      UNREFERENCED_PARAMETER(bShow);
      ::exception::throw_interface_only(get_app());
   }

   ::user::interaction * interaction_impl::ChildWindowFromPoint(POINT point)
   {
      UNREFERENCED_PARAMETER(point);
      ::exception::throw_interface_only(get_app());

      return NULL;

   }

   ::user::interaction * interaction_impl::ChildWindowFromPoint(POINT point,UINT nFlags)
   {
      UNREFERENCED_PARAMETER(point);
      UNREFERENCED_PARAMETER(nFlags);
      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   ::user::interaction * interaction_impl::get_next_window(UINT nFlag)
   {

      UNREFERENCED_PARAMETER(nFlag);

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   ::user::interaction * interaction_impl::GetTopWindow()
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   ::user::interaction * interaction_impl::GetLastActivePopup()
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   bool interaction_impl::FlashWindow(bool bInvert)
   {

      UNREFERENCED_PARAMETER(bInvert);

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_impl::ChangeClipboardChain(oswindow oswindow_Next)
   {

      UNREFERENCED_PARAMETER(oswindow_Next);

      ::exception::throw_interface_only(get_app());

      return false;

   }


   oswindow interaction_impl::SetClipboardViewer()
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   bool interaction_impl::OpenClipboard()
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   void interaction_impl::CreateCaret(::draw2d::bitmap* pBitmap)
   {

      UNREFERENCED_PARAMETER(pBitmap);

      ::exception::throw_interface_only(get_app());

   }


   void interaction_impl::CreateSolidCaret(int32_t nWidth,int32_t nHeight)
   {

      UNREFERENCED_PARAMETER(nWidth);
      UNREFERENCED_PARAMETER(nHeight);

      ::exception::throw_interface_only(get_app());

   }


   void interaction_impl::CreateGrayCaret(int32_t nWidth,int32_t nHeight)
   {

      UNREFERENCED_PARAMETER(nWidth);
      UNREFERENCED_PARAMETER(nHeight);

      ::exception::throw_interface_only(get_app());

   }


   void interaction_impl::HideCaret()
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_impl::ShowCaret()
   {

      ::exception::throw_interface_only(get_app());

   }


   bool interaction_impl::SetForegroundWindow()
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_impl::SendNotifyMessage(UINT message,WPARAM wParam,lparam lParam)
   {

      UNREFERENCED_PARAMETER(message);
      UNREFERENCED_PARAMETER(wParam);
      UNREFERENCED_PARAMETER(lParam);

      ::exception::throw_interface_only(get_app());

      return false;

   }


   void interaction_impl::Print(::draw2d::graphics * pgraphics,uint32_t dwFlags) const
   {

      UNREFERENCED_PARAMETER(pgraphics);
      UNREFERENCED_PARAMETER(dwFlags);

      ::exception::throw_interface_only(get_app());

   }


   void interaction_impl::PrintClient(::draw2d::graphics * pgraphics,uint32_t dwFlags) const
   {

      UNREFERENCED_PARAMETER(pgraphics);
      UNREFERENCED_PARAMETER(dwFlags);

      ::exception::throw_interface_only(get_app());

   }


   bool interaction_impl::SetWindowContextHelpId(uint32_t dwContextHelpId)
   {

      UNREFERENCED_PARAMETER(dwContextHelpId);

      ::exception::throw_interface_only(get_app());

      return false;

   }


   uint32_t interaction_impl::GetWindowContextHelpId() const
   {

      ::exception::throw_interface_only(get_app());

      return -1;

   }

   void interaction_impl::_001OnCreate(signal_details * pobj)
   {

      pobj->previous();

      if(!m_pui->m_bMessageWindow)
      {

         m_apcsDisplay = new critical_section;

         output_debug_string("interaction_impl m_spgraphics alloc");

         update_graphics_resources();

         output_debug_string("interaction_impl on _create_window");

      }

      m_puiThis = m_pui;


   }


   void interaction_impl::_001OnShowWindow(signal_details * pobj)
   {

      SCAST_PTR(::message::show_window, pshowwindow, pobj);

      if (pshowwindow->m_bShow)
      {

      }
      else
      {

         {

            ::user::interaction_spa uia;

            {

               synch_lock sl(m_pmutex);

               uia = m_guieptraMouseHover;

            }

            for (auto & pui : uia)
            {

               pui->send_message(WM_MOUSELEAVE);

            }

            {

               synch_lock sl(m_pmutex);

               m_guieptraMouseHover.remove_all();

            }

         }



      }


   }



#ifdef WINDOWSEX


   void interaction_impl::_001OnWindowPosChanged(signal_details * pobj)
   {

      ::exception::throw_interface_only(get_app());

   }


#endif


   void interaction_impl::BeginModalState()
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_impl::EndModalState()
   {

      ::exception::throw_interface_only(get_app());

   }


   void interaction_impl::CloseWindow()
   {

      ::exception::throw_interface_only(get_app());

   }


   bool interaction_impl::OpenIcon()
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   int32_t interaction_impl::GetCheckedRadioButton(int32_t nIDFirstButton,int32_t nIDLastButton)
   {

      UNREFERENCED_PARAMETER(nIDFirstButton);
      UNREFERENCED_PARAMETER(nIDLastButton);

      ::exception::throw_interface_only(get_app());

      return 0;

   }


   bool interaction_impl::_EnableToolTips(bool bEnable,UINT nFlag)
   {

      UNREFERENCED_PARAMETER(bEnable);
      UNREFERENCED_PARAMETER(nFlag);

      ::exception::throw_interface_only(get_app());

      return false;

   }


   void interaction_impl::_001BaseWndInterfaceMap()
   {

      Session.user()->window_map().set((int_ptr)get_handle(), this);

   }


   interaction_impl * interaction_impl::get_impl() const
   {

      return (::user::interaction_impl *) this;

   }


#define HARD_DEBUG 0


   void interaction_impl::_001UpdateBuffer()
   {

      synch_lock sl(draw2d_mutex());

      m_pui->defer_check_layout();

      m_pui->defer_check_zorder();

      update_graphics_resources();

      if (m_spgraphics.is_null())
         return;

      bool bUpdateBuffer = true;

      if (bUpdateBuffer)
      {

         synch_lock sl(m_spgraphics->m_pmutex);

         {

            rect64 rectWindow;

            m_pui->GetWindowRect(rectWindow);

            ::draw2d::graphics * pgraphics = m_spgraphics->on_begin_draw();

            ::draw2d::savedc savedc(pgraphics);

            if (pgraphics == NULL || pgraphics->get_os_data() == NULL)
            {

               return;

            }

            if (m_pui->m_bMayProDevian)
            {

               if (!pgraphics->is_valid_update_window_thread())
               {

                  return;

               }

            }

            rect r;

            r = rectWindow;

            r.offset(-r.top_left());

            pgraphics->set_alpha_mode(::draw2d::alpha_mode_set);

            if (m_bComposite)
            {

               pgraphics->FillSolidRect(r, ARGB(0, 0, 0, 0));

            }
            else
            {

               pgraphics->FillSolidRect(r, ARGB(255, 184, 184, 177));

            }
            
            //if(m_pui->IsWindowVisible())
            try
            {

               _001Print(pgraphics);
               
            }
            catch (...)
            {


            }

#if HARD_DEBUG

            ::draw2d::graphics_sp g(allocer());

            g->debug();

            m_size.cx = 0;

            m_size.cy = 0;

#endif

         }

      }

      //m_spgraphics->update_window();

   }


   void interaction_impl::_001UpdateScreen()
   {

//      m_pui->defer_check_layout();
//
//      m_pui->defer_check_zorder();
//
//      update_graphics_resources();
//
//      if (m_spgraphics.is_null())
//         return;
//
//      if (bUpdateBuffer)
//      {
//
//         synch_lock sl(m_spgraphics->m_pmutex);
//
//         {
//
//            rect64 rectWindow;
//
//            m_pui->GetWindowRect(rectWindow);
//
//            ::draw2d::graphics * pgraphics = m_spgraphics->on_begin_draw();
//
//            ::draw2d::savedc savedc(pgraphics);
//
//            if (pgraphics == NULL || pgraphics->get_os_data() == NULL)
//            {
//
//               return;
//
//            }
//
//            if (m_pui->m_bMayProDevian)
//            {
//
//               if (!pgraphics->is_valid_update_window_thread())
//               {
//
//                  return;
//
//               }
//
//            }
//
//            rect r;
//
//            r = rectWindow;
//
//            r.offset(-r.top_left());
//
//            pgraphics->set_alpha_mode(::draw2d::alpha_mode_set);
//
//            if (m_bComposite)
//            {
//
//               pgraphics->FillSolidRect(r, ARGB(0, 0, 0, 0));
//
//            }
//            else
//            {
//
//               pgraphics->FillSolidRect(r, ARGB(255, 184, 184, 177));
//
//            }
//
//            //if(m_pui->IsWindowVisible())
//            try
//            {
//
//               _001Print(pgraphics);
//
//            }
//            catch (...)
//            {
//
//
//            }
//
//#if HARD_DEBUG
//
//            ::draw2d::graphics_sp g(allocer());
//
//            g->debug();
//
//            m_size.cx = 0;
//
//            m_size.cy = 0;
//
//#endif
//
//         }
//
//      }
//
      if (m_spgraphics.is_set())
      {
         m_spgraphics->update_window();

      }

   }


//   void interaction_impl::_001UpdateScreen(bool bUpdateBuffer)
//   {
//
//      cslock sl(cs_display());
//
//
//
//      if(m_spdibBuffer.is_set() && m_spdibBuffer->get_graphics() != NULL)
//      {
//
//         m_spdibBuffer->update_window(m_pui,NULL, bUpdateBuffer);
//
////         if(m_pui->m_bMoving && GetExStyle() && WS_EX_LAYERED)
////         {
////
////            Session.m_ptCursor = m_pui->m_ptMoveCursor;
////
////#ifdef WINDOWSEX
////
////            ::SetCursorPos(m_pui->m_ptMoveCursor.x,m_pui->m_ptMoveCursor.y);
////
////
////#endif
////
////         }
//
//      }
//      else
//      {
////         TRACE("Failed to update Screen");
//      }
//
//
//   }


//#define HARD_DEBUG 0
//   void interaction_impl::_001UpdateBuffer()
//   {
//
//      synch_lock sl1(m_pui->m_pmutex);
//
//      if(!m_pui->m_bMayProDevian)
//      {
//
//         //TRACE("interaction_impl::_001UpdateBuffer going to update_graphics_resources");
//
//      }
//
//      update_graphics_resources();
//
//      if(!m_pui->m_bMayProDevian)
//      {
//
//         //TRACE("interaction_impl::_001UpdateBuffer going to lock");
//
//      }
//
//      synch_lock sl(m_spmutexBuffer);
//
//      //if(!sl.lock())
//        // return;
//
//      if(!m_pui->m_bMayProDevian)
//      {
//
//         //TRACE("interaction_impl::_001UpdateBuffer locked");
//
//      }
//
//      if(m_spdibBuffer.is_null())
//         return;
//
//      m_spdibBuffer->map();
//
//      if(m_spdibBuffer->get_data() == NULL)
//         return;
//
//      rect64 rectWindow;
//
//      m_pui->GetWindowRect(rectWindow);
//
//      if(m_bComposite)
//      {
//         m_spdibBuffer->Fill(0,0,0,0);
//      }
//      else
//      {
//         m_spdibBuffer->Fill(255,184,184,177);
//      }
//
//      //m_spdibBuffer->Fill(255,255,255,0);
//
//      //::draw2d::graphics * pgraphics = m_spdibBuffer->get_graphics();
//
//      //if(pgraphics == NULL)
//        //return;
//
//      if (m_spdibBuffer.is_null())
//      {
//
//         return;
//
//      }
//
//      if(!m_pui->m_bMayProDevian)
//      {
//
//         //TRACE("Going to top print");
//
//      }
//
//      _001Print(m_spdibBuffer);
//
//
//      cslock slDisplay(cs_display());
//
//      //if(m_spdibBuffer->get_data() == NULL || m_spdibBuffer->get_data() == NULL)
//        // return;
//
//      m_spdibBuffer->BitBlt(rectWindow.width(), rectWindow.height(), m_spdibBuffer, 1);
//
//      m_spdibBuffer->m_bReduced = false;
//
//#if HARD_DEBUG
//
//      m_spdibBuffer->Destroy();
//
//      m_spdibBuffer->Destroy();
//
//      ::draw2d::graphics_sp g(allocer());
//
//      g->debug();
//
//      m_size.cx = 0;
//      m_size.cy = 0;
//
//#endif
//
//      //       ::rect rectClient;
//
//      //       GetClientRect(rectClient);
//
//      //       g->BitBlt(0, 0, rectClient.width(), rectClient.height(), m_spdibBuffer->get_graphics(), 0, 0, SRCCOPY);
//
//      //       g->BitBlt(0, 0, m_spdibBuffer->m_size.cx, m_spdibBuffer->m_size.cy, m_spdibBuffer->get_graphics(), 0, 0, SRCCOPY);
//
//      //       pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);
//
//      //       pgraphics->FillSolidRect(rectClient, ARGB(128, 0, 255, 0));
//      {
//         //point pt;
//         //::GetCursorPos(&pt);
//      }
//   }


   void interaction_impl::update_graphics_resources()
   {

      single_lock sl(m_pmutex);

      if (m_spgraphics.is_null())
      {

         m_spgraphics.alloc(allocer());

         m_spgraphics->on_create_window(this);

      }

   }


   guie_message_wnd::guie_message_wnd(::aura::application * papp):
      ::object(papp),
      ::aura::timer_array(papp)
   {

      m_puiForward = NULL;

   }


   void guie_message_wnd::message_handler(signal_details * pobj)
   {
      if(m_puiForward != NULL)
      {
         return m_puiForward->message_handler(pobj);
      }
      else
      {
         return;
      }
   }


   void __reposition_window(SIZEPARENTPARAMS * lpLayout,
      ::user::interaction * oswindow,LPCRECT lpRect)
   {
      ASSERT(oswindow != NULL);
      ASSERT(lpRect != NULL);
      sp(::user::interaction) puiParent = oswindow->GetParent();
      ASSERT(puiParent != NULL);

      //if (lpLayout != NULL && lpLayout->hDWP == NULL)
      // return;

      // first check if the new rectangle is the same as the current
      rect rectOld;
      oswindow->GetWindowRect(rectOld);
      puiParent->ScreenToClient(&rectOld.top_left());
      puiParent->ScreenToClient(&rectOld.bottom_right());
      //if (::is_equal(rectOld, lpRect))
      //   return;     // nothing to do

      // try to use DeferWindowPos for speed, otherwise use SetWindowPos
      oswindow->SetWindowPos(0,lpRect->left,lpRect->top,lpRect->right - lpRect->left,lpRect->bottom - lpRect->top,SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW);
   }





   void interaction_impl::set_handle(oswindow oswindow)
   {

      if(m_pui != NULL)
      {

         oswindow_remove(this);

         oswindow_assign(oswindow,this);

      }

      m_oswindow = oswindow;

   }



   bool interaction_impl::attach(oswindow oswindow_New)
   {

      //::simple_message_box(NULL, "d1.a", "d1.a", MB_OK);

      ASSERT(get_handle() == NULL);

      //::simple_message_box(NULL,"d1.b","d1.b",MB_OK);

      ASSERT(System.ui_from_handle(oswindow_New) == NULL);

      //::simple_message_box(NULL,"d1.c","d1.c",MB_OK);

      if(oswindow_New == NULL)
         return FALSE;


      //::simple_message_box(NULL,"d1.d","d1.d",MB_OK);



      //::simple_message_box(NULL,"d1.e","d1.e",MB_OK);


      set_handle(oswindow_New);

      //::simple_message_box(NULL,"d1.f","d1.f",MB_OK);


      ASSERT(System.ui_from_handle(get_handle()) == m_pui);

      //::simple_message_box(NULL,"d1.g","d1.g",MB_OK);



      // ::simple_message_box(NULL,"d1.h","d1.h",MB_OK);
      //

      return true;

   }


   oswindow interaction_impl::detach()
   {

      oswindow oswindow = get_handle();

      if(oswindow != NULL)
      {


         oswindow_remove(this);

         set_handle(NULL);

      }

      return oswindow;

   }


   oswindow interaction_impl::get_handle() const
   {

      return m_oswindow;

   }


   bool interaction_impl::is_composite()
   {

      return (GetExStyle() & WS_EX_LAYERED) != 0;

   }



   //sp(mutex) interaction_impl::cs_display()
   //{

   //   return m_spmutexDisplay;

   //}


   window_graphics * interaction_impl::get_window_graphics()
   {

      return m_spgraphics;

   }

   ::user::interaction * interaction_impl::get_focus_ui()
   {

      return m_puiFocus;

   }


   void interaction_impl::set_focus_guie(::user::interaction * pguie)
   {

      if(pguie == NULL)
      {

         m_puiFocus = NULL;

         return;

      }

      if(pguie == m_pui)
      {

         m_puiFocus = m_pui;

         return;

      }

      if(pguie->is_descendant_of(m_pui))
      {

         m_puiFocus = pguie;

         return;

      }

      throw invalid_argument_exception(get_app(),"Focus of a window implementation should be set NULL, to itself or to a descendant window");

   }


   void interaction_impl::redraw_add(::object * p)
   {

      synch_lock sl(mutex_redraw());

      m_ptraRedraw.add(p);

   }


   void interaction_impl::redraw_remove(::object * p)
   {

      synch_lock sl(mutex_redraw());

      m_ptraRedraw.remove(p);

   }

   bool interaction_impl::has_redraw()
   {

      synch_lock sl(mutex_redraw());

      return m_ptraRedraw.has_elements();

   }

   mutex * interaction_impl::mutex_redraw()
   {

      if (m_apmutexRedraw == NULL)
      {

         m_apmutexRedraw = new mutex(get_app());

      }

      return m_apmutexRedraw;

   }


   bool interaction_impl::has_pending_graphical_update()
   {

      {

         synch_lock sl(m_pmutex);

         if (m_ptraRedraw.has_elements())
         {

            return true;

         }

      }

      return false;


   }



} // namespace user







