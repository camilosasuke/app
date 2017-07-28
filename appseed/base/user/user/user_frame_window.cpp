//#include "framework.h" // from "base/user/user.h"
//#include "base/user/user.h"


#ifdef WINDOWS
//#include <dde.h>        // for DDE execute shell requests
#endif

extern CLASS_DECL_BASE thread_int_ptr < DWORD_PTR > t_time1;

namespace user
{


   frame_window::frame_window():
      ::object(get_app()),
      ::user::interaction(get_app())
   {
      
      m_flagNonClient.unsignalize(non_client_background);
      m_flagNonClient.unsignalize(non_client_focus_rect);

      m_pviewMain = NULL;
      m_bAutoWindowFrame = true;
      m_bWindowFrame = true;
      m_nWindow = -1;                 // unknown interaction_impl ID
      m_bAutoMenuEnable = TRUE;       // auto enable on by default
      //m_lpfnCloseProc = NULL;
      m_hMenuDefault = NULL;
      m_hAccelTable = NULL;
      //m_nIDHelp = 0;
      m_nIDTracking = 0;
      m_nIDLastMessage = 0;

      m_cModalStack = 0;              // initialize modality support
      m_hMenuAlt = NULL;
      m_nIdleFlags = 0;               // no idle work at start
      m_rectBorder.null();

      m_bHelpMode = 0;    // not in Shift+F1 help mode
      m_dwPromptContext = 0;

      m_pNextFrameWnd = NULL;         // not in list yet

      m_bInRecalcLayout = FALSE;
      m_nShowDelay = -1;              // no delay pending

      m_bFrameMoveEnable = true;

      m_pviewActive = NULL;

      AddFrameWnd();

   }


   frame_window::~frame_window()
   {
      RemoveFrameWnd();


   }

   bool frame_window::_001IsFrameWnd()
   {
      return true;
   }

   void frame_window::GetBorderRect(LPRECT lprect)
   {
      UNREFERENCED_PARAMETER(lprect);
   }


   void frame_window::SetBorderRect(const RECT & rect)
   {
      UNREFERENCED_PARAMETER(rect);
   }

   /*   ::user::OleFrameHook * frame_window::GetNotifyHook()
   {
   return NULL;
   }*/

   void frame_window::NotifyFloatingWindows(uint32_t dwFlags)
   {
      UNREFERENCED_PARAMETER(dwFlags);
   }

   string frame_window::get_window_default_matter()
   {

      return "frame";

   }

   ::user::interaction::e_type frame_window::get_window_type()
   {
      return type_frame;
   }


   bool frame_window::on_simple_command(e_simple_command ecommand, lparam lparam, LRESULT & lresult)
   {

      if (::user::interaction::on_simple_command(ecommand, lparam, lresult))
         return true;

      switch (ecommand)
      {
      case simple_command_update_frame_title:
         on_update_frame_title(lparam != FALSE);
         break;
      default:
         break;
      }


      return false;

   }


   void frame_window::_guserbaseOnInitialUpdate(signal_details * pobj)
   {

      guserbaseOnInitialUpdate(pobj);

   }


   void frame_window::guserbaseOnInitialUpdate(signal_details * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

   }


   void frame_window::install_message_handling(::message::dispatch *pinterface)
   {

      ::user::box::install_message_handling(pinterface);
      IGUI_MSG_LINK(::message::message_frame_initial_update, pinterface, this, &frame_window::_guserbaseOnInitialUpdate);
      IGUI_WIN_MSG_LINK(WM_DESTROY, pinterface, this, &frame_window::_001OnDestroy);
      IGUI_WIN_MSG_LINK(WM_SYSCOMMAND, pinterface, this, &frame_window::_001OnSysCommand);
      IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &frame_window::_001OnCreate);
      IGUI_WIN_MSG_LINK(WM_SIZE, pinterface, this, &frame_window::_001OnSize);
      IGUI_WIN_MSG_LINK(WM_IDLEUPDATECMDUI, pinterface, this, &frame_window::_001OnIdleUpdateCmdUI);
      IGUI_WIN_MSG_LINK(WM_SETFOCUS, pinterface, this, &frame_window::_001OnSetFocus);
      IGUI_WIN_MSG_LINK(WM_ACTIVATE, pinterface, this, &frame_window::_001OnActivate);
      IGUI_WIN_MSG_LINK(WM_NCACTIVATE, pinterface, this, &frame_window::_001OnNcActivate);
      IGUI_WIN_MSG_LINK(WM_QUERYENDSESSION, pinterface, this, &frame_window::_001OnQueryEndSession);

   }


   void frame_window::defer_synch_layered()
   {
      bool bCalcLayered = calc_layered();
      if (is_different((GetExStyle() & WS_EX_LAYERED) != 0, bCalcLayered) != FALSE)
      {
         if (bCalcLayered)
         {
            ModifyStyleEx(0, WS_EX_LAYERED);
         }
         else
         {
            ModifyStyleEx(WS_EX_LAYERED, 0);
         }
         RedrawWindow();
      }
   }


   bool frame_window::calc_layered()
   {

      return true;

   }



   void frame_window::on_set_parent(::user::interaction * puiParent)
   {

      ::user::box::on_set_parent(puiParent);

   }

   void frame_window::assert_valid() const
   {
      try
      {
         ::user::interaction::assert_valid();
      }
      catch (...)
      {
      }
      try
      {
         if (m_pviewActive != NULL)
            ASSERT_VALID(m_pviewActive);
      }
      catch (...)
      {
      }

   }

   void frame_window::dump(dump_context & dumpcontext) const
   {
      ::user::interaction::dump(dumpcontext);

      dumpcontext << "m_hAccelTable = " << (uint_ptr)m_hAccelTable;
      dumpcontext << "\nm_nWindow = " << m_nWindow;
      dumpcontext << "\nm_nIDHelp = " << m_strMatterHelp;
      dumpcontext << "\nm_nIDTracking = " << m_nIDTracking;
      dumpcontext << "\nm_nIDLastMessage = " << m_nIDLastMessage;
      if (m_pviewActive != NULL)
         dumpcontext << "\nwith active ::user::impact: " << m_pviewActive;
      else
         dumpcontext << "\nno active ::user::impact";

      dumpcontext << "\n";
   }




   /////////////////////////////////////////////////////////////////////////////
   // frame_window

   // // BEGIN_MESSAGE_MAP(frame_window, user::frame_window)
   //{{__MSG_MAP(frame_window)
   /* xxx   ON_WM_INITMENU()
   ON_WM_INITMENUPOPUP()
   ON_WM_MENUSELECT()
   ON_MESSAGE(WM_POPMESSAGESTRING, &frame_window::OnPopMessageString)
   ON_MESSAGE(WM_SETMESSAGESTRING, &frame_window::OnSetMessageString)
   ON_MESSAGE(WM_HELPPROMPTADDR, &frame_window::OnHelpPromptAddr)
   ON_MESSAGE_VOID(WM_IDLEUPDATECMDUI, frame_window::OnIdleUpdateCmdUI)
   ON_WM_ENTERIDLE()
   ON_WM_HSCROLL()
   ON_WM_VSCROLL()
   ON_WM_SETFOCUS()

   ON_WM_DESTROY()
   ON_WM_CLOSE()
   ON_WM_SIZE()
   ON_WM_ERASEBKGND()
   ON_WM_ACTIVATE()
   ON_WM_NCACTIVATE()
   ON_WM_DROPFILES()
   ON_WM_QUERYENDSESSION()
   ON_WM_ENDSESSION()
   ON_WM_SETCURSOR()
   ON_WM_ENABLE()
   // OLE palette support
   ON_WM_QUERYNEWPALETTE()
   ON_WM_PALETTECHANGED()
   ON_MESSAGE(WM_COMMANDHELP, &frame_window::OnCommandHelp)
   ON_MESSAGE(WM_HELPHITTEST, &frame_window::OnHelpHitTest)
   ON_MESSAGE(WM_ACTIVATETOPLEVEL, &frame_window::OnActivateTopLevel)
   // turning on and off standard frame gadgetry
   ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS_BAR, &frame_window::OnUpdateControlBarMenu)
   ON_COMMAND_EX(ID_VIEW_STATUS_BAR, &frame_window::OnBarCheck)
   ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, &frame_window::OnUpdateControlBarMenu)
   ON_COMMAND_EX(ID_VIEW_TOOLBAR, &frame_window::OnBarCheck)
   ON_UPDATE_COMMAND_UI(ID_VIEW_REBAR, &frame_window::OnUpdateControlBarMenu)
   ON_COMMAND_EX(ID_VIEW_REBAR, &frame_window::OnBarCheck)
   // turning on and off standard mode indicators
   ON_UPDATE_COMMAND_UI(ID_INDICATOR_CAPS, &frame_window::OnUpdateKeyIndicator)
   ON_UPDATE_COMMAND_UI(ID_INDICATOR_NUM, &frame_window::OnUpdateKeyIndicator)
   ON_UPDATE_COMMAND_UI(ID_INDICATOR_SCRL, &frame_window::OnUpdateKeyIndicator)
   ON_UPDATE_COMMAND_UI(ID_INDICATOR_KANA, &frame_window::OnUpdateKeyIndicator)
   // standard help handling
   ON_UPDATE_COMMAND_UI(ID_CONTEXT_HELP, &frame_window::OnUpdateContextHelp)
   // toolbar "tooltip" notification
   ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, &frame_window::OnToolTipText)
   ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, &frame_window::OnToolTipText)
   ON_NOTIFY_EX_RANGE(RBN_CHEVRONPUSHED, 0, 0xFFFF, &frame_window::OnChevronPushed)
   //}}__MSG_MAP
   // message handling for standard DDE commands
   ON_MESSAGE(WM_DDE_INITIATE, &frame_window::OnDDEInitiate)
   ON_MESSAGE(WM_DDE_EXECUTE, &frame_window::OnDDEExecute)
   ON_MESSAGE(WM_DDE_TERMINATE, &frame_window::OnDDETerminate)
   ON_REGISTERED_MESSAGE(gen_MsgMouseWheel, &frame_window::OnRegisteredMouseWheel) */
   // // END_MESSAGE_MAP()

   /////////////////////////////////////////////////////////////////////////////
   // frame_window construction/destruction



   void frame_window::AddFrameWnd()
   {

   }


   void frame_window::RemoveFrameWnd()
   {

   }


   /////////////////////////////////////////////////////////////////////////////
   // Special processing etc

   bool frame_window::LoadAccelTable(const char * lpszResourceName)
   {
      ASSERT(m_hAccelTable == NULL);  // only do once
      ASSERT(lpszResourceName != NULL);

      /*   HINSTANCE hInst = ::core::FindResourceHandle(lpszResourceName, RT_ACCELERATOR);
      m_hAccelTable = ::LoadAccelerators(hInst, lpszResourceName);*/
      return (m_hAccelTable != NULL);
   }

   HACCEL frame_window::GetDefaultAccelerator()
   {
      // use ::user::document specific accelerator table over m_hAccelTable
      HACCEL hAccelTable = m_hAccelTable;
      HACCEL hAccel;
      sp(::user::document) pDoc = GetActiveDocument();
      if (pDoc != NULL && (hAccel = pDoc->GetDefaultAccelerator()) != NULL)
         hAccelTable = hAccel;

      return hAccelTable;
   }

   void frame_window::pre_translate_message(signal_details * pobj)
   {
      ENSURE_ARG(pobj != NULL);
      // check for special cancel modes for combo boxes
      //if (pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_NCLBUTTONDOWN)
      //   __cancel_modes(pMsg->oswindow);    // filter clicks

      sp(::message::key) pkey = pobj;

      if(pkey.is_set())
      {

         if(Session.is_key_pressed(::user::key_alt) && Session.is_key_pressed(::user::key_control))
         {
            
            if(pkey->m_ekey == ::user::key_p)
            {
               
               sp(::user::interaction_impl) pimpl = m_pimpl;
               
               if(pimpl.is_set())
               {
                  
                  synch_lock sl(pimpl->m_spgraphics->m_pmutex);
                  
                  ::visual::dib_sp dib(allocer());
                  
                  ::rect r;
                  
                  GetWindowRect(r);

                  dib->create(r.size());

                  ::draw2d::graphics * pgraphics = pimpl->m_spgraphics->on_begin_draw();

                  dib->get_graphics()->BitBlt(0,0,r.width(),r.height(),pgraphics,0,0,SRCCOPY);

                  Session.copydesk().dib_to_desk(dib);

                  dib.save_to_file(::dir::system() / "control_alt_p.png");

                  ::visual::dib_sp dib2(allocer());

                  dib2->create(284,r.size().cy * 284 / r.size().cx);
                  dib2->get_graphics()->SetStretchBltMode(HALFTONE);
                  dib2->get_graphics()->StretchBlt(0,0,dib2->m_size.cx,dib2->m_size.cy,dib->get_graphics(),0,0, r.size().cx,r.size().cy,SRCCOPY);
                  dib2.save_to_file(::dir::system() / "control_alt_p_w284.png");
                  pkey->m_bRet = true;
                  pkey->set_lresult(1);
               }
               return;
            }
         }

      }

      // allow tooltip messages to be filtered
      ::user::interaction::pre_translate_message(pobj);
      if (pobj->m_bRet)
         return;




      /* trans if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
      {
      // finally, translate the message
      HACCEL hAccel = GetDefaultAccelerator();
      return hAccel != NULL &&  ::TranslateAccelerator(get_handle(), hAccel, pMsg);
      }*/
   }

   void frame_window::PostNcDestroy()
   {
      ::user::interaction::PostNcDestroy();
      // default for frame windows is to allocate them on the heap
      //  the default post-cleanup is to 'delete this'.
      // never explicitly call 'delete' on a frame_window, use DestroyWindow instead
      //release();
   }

   void frame_window::OnPaletteChanged(sp(::user::interaction) pFocusWnd)
   {
      UNREFERENCED_PARAMETER(pFocusWnd);
      // trans user::frame_window::OnPaletteChanged(pFocusWnd);
   }

   bool frame_window::OnQueryNewPalette()
   {
      // trans    return user::frame_window::OnQueryNewPalette();
      return FALSE;
   }

   /////////////////////////////////////////////////////////////////////////////
   // frame_window support for context sensitive help.

   void frame_window::ExitHelpMode()
   {
      /* trans
      // if not in help mode currently, this is a no-op
      if (!m_bHelpMode)
      return;

      // only post new WM_EXITHELPMODE message if one doesn't already exist
      //  in the queue.
      MESSAGE msg;
      if (!::PeekMessage(&msg, get_handle(), WM_EXITHELPMODE, WM_EXITHELPMODE,
      PM_REMOVE|PM_NOYIELD))
      {
      VERIFY(::PostMessage(get_handle(), WM_EXITHELPMODE, 0, 0));
      }

      // release capture if this interaction_impl has it
      if (Session.GetCapture() == get_handle())
      Session.ReleaseCapture();

      sp(::user::frame_window) pFrameWnd = GetTopLevelFrame();
      ENSURE_VALID(pFrameWnd);
      pFrameWnd->m_bHelpMode = m_bHelpMode = HELP_INACTIVE;
      PostMessage(WM_KICKIDLE);   // trigger idle update
      */
   }

   bool frame_window::OnSetCursor(sp(::user::interaction) pwindow, UINT nHitTest, UINT message)
   {
      UNREFERENCED_PARAMETER(pwindow);
      UNREFERENCED_PARAMETER(nHitTest);
      UNREFERENCED_PARAMETER(message);
      sp(::user::frame_window) pFrameWnd = GetTopLevelFrame();
      ENSURE_VALID(pFrameWnd);
      if (pFrameWnd->m_bHelpMode)
      {
         
         return TRUE;

      }
      // trans   return user::frame_window::OnSetCursor(pwindow, nHitTest, message);

      return TRUE;

   }

   /*LRESULT frame_window::OnCommandHelp(WPARAM, LPARAM lParam)
   {
   if (lParam == 0)
   {
   if (IsTracking())
   lParam = HID_BASE_COMMAND+m_nIDTracking;
   else
   lParam = HID_BASE_RESOURCE+m_nIDHelp;
   }
   if (lParam != 0)
   {
   application* pApp = &System;
   if (pApp != NULL)
   pApp->WinHelpInternal(lParam);
   return TRUE;
   }
   return FALSE;
   }*/

   /*LRESULT frame_window::OnHelpHitTest(WPARAM, LPARAM)
   {
   if (m_nIDHelp != 0)
   return HID_BASE_RESOURCE+m_nIDHelp;
   else
   return 0;
   }*/

   //bool frame_window::OnCommand(WPARAM wParam, LPARAM lParam)
   //   // return TRUE if command invocation was attempted
   //{
   //   //   ::oswindow oswindow_Ctrl = (::oswindow) lParam;
   //   //   UINT nID = LOWORD(wParam);

   //   sp(::user::frame_window) pFrameWnd = GetTopLevelFrame();
   //   ENSURE_VALID(pFrameWnd);
   //   /*   if (pFrameWnd->m_bHelpMode && oswindow_Ctrl == NULL &&
   //   nID != ID_HELP && nID != ID_DEFAULT_HELP && nID != ID_CONTEXT_HELP)
   //   {
   //   // route as help
   //   if (!SendMessage(WM_COMMANDHELP, 0, HID_BASE_COMMAND+nID))
   //   SendMessage(WM_COMMAND, ID_DEFAULT_HELP);
   //   return TRUE;
   //   }
   //   */
   //   // route as normal command
   //   // trans   return user::frame_window::OnCommand(wParam, lParam);
   //   return FALSE;
   //}




   void frame_window::BeginModalState()
   {
      //   ASSERT(get_handle() != NULL);
      ASSERT(IsWindow());

      // allow stacking, but don't do anything
      if (++m_cModalStack > 1)
         return;

      // determine top-level parent, since that is the true parent of any
      //  modeless windows anyway...
      sp(::user::interaction) pParent = EnsureTopLevel();

      m_uiptraDisable.remove_all();

      /*
      // disable all windows connected to this frame (and add them to the list)
      sp(::user::interaction) oswindow = System.get_desktop_window()->GetWindow(GW_CHILD);

      while (oswindow != NULL)
      {

         if (oswindow->is_window_enabled() && ::user::is_descendant(pParent, oswindow) && oswindow->send_message(WM_DISABLEMODAL) == 0)
         {

            oswindow->enable_window(FALSE);

            m_uiptraDisable.add(oswindow);

         }

         oswindow = oswindow->GetWindow(GW_HWNDNEXT);

      }
      */


   }

   void frame_window::EndModalState()
   {
      // pop one off the stack (don't undo modalness unless stack is down to zero)
      if (m_cModalStack == 0 || --m_cModalStack > 0)
         return;

      for (index nIndex = 0; nIndex < m_uiptraDisable.get_count(); nIndex++)
      {

         ASSERT(m_uiptraDisable[nIndex] != NULL);

         if (m_uiptraDisable[nIndex]->IsWindow())
            m_uiptraDisable[nIndex]->enable_window(TRUE);

      }

      m_uiptraDisable.remove_all();


   }

   void frame_window::ShowOwnedWindows(bool bShow)
   {
      UNREFERENCED_PARAMETER(bShow);
      // walk through all top-level windows
      ::exception::throw_not_implemented(get_app());
      /*   oswindow oswindow = ::GetWindow(::GetDesktopWindow(), GW_CHILD);
      while (oswindow != NULL)
      {
      ::window_sp pwindow = interaction_impl::FromHandlePermanent(oswindow);
      if (pwindow != NULL && get_handle() != oswindow && __is_descendant(this, pwindow))
      {
      uint32_t dwStyle = ::GetWindowLong(oswindow, GWL_STYLE);
      if (!bShow && (dwStyle & (WS_VISIBLE|WS_DISABLED)) == WS_VISIBLE)
      {
      ::ShowWindow(oswindow, SW_HIDE);
      pwindow->m_nFlags |= WF_TEMPHIDE;
      }
      // don't show temporarily hidden windows if we're in print preview mode
      else if (bShow && (dwStyle & (WS_VISIBLE|WS_DISABLED)) == 0 &&
      (pwindow->m_nFlags & WF_TEMPHIDE) && !m_lpfnCloseProc)
      {
      ::ShowWindow(oswindow, SW_SHOWNOACTIVATE);
      pwindow->m_nFlags &= ~WF_TEMPHIDE;
      }
      }
      oswindow = ::GetWindow(oswindow, GW_HWNDNEXT);
      }*/
   }

   void frame_window::OnEnable(bool bEnable)
   {

      if (bEnable && (m_nFlags & WF_STAYDISABLED))
      {

         // Work around for MAPI support. This makes sure the main interaction_impl
         // remains disabled even when the mail system is booting.

         enable_window(FALSE);

#ifdef WINDOWSEX

         ::SetFocus(NULL);
#else

         throw todo(get_app());

#endif

         return;

      }


      // this causes modal dialogs to be "truly modal"
      if (!bEnable && !InModalState())
      {
         ASSERT((m_nFlags & WF_MODALDISABLE) == 0);
         m_nFlags |= WF_MODALDISABLE;
         BeginModalState();
      }
      else if (bEnable && (m_nFlags & WF_MODALDISABLE))
      {
         m_nFlags &= ~WF_MODALDISABLE;
         EndModalState();

         // cause normal focus logic to kick in
         if (System.get_active_ui() == this)
            send_message(WM_ACTIVATE, WA_ACTIVE);
      }

      // force WM_NCACTIVATE because Windows may think it is unecessary
      if (bEnable && (m_nFlags & WF_STAYACTIVE))
         send_message(WM_NCACTIVATE, TRUE);
      // force WM_NCACTIVATE for floating windows too
   }

   /////////////////////////////////////////////////////////////////////////////
   // frame_window second phase creation

   bool frame_window::pre_create_window(::user::create_struct& cs)
   {

      if (cs.style & FWS_ADDTOTITLE)
         cs.style |= FWS_PREFIXTITLE;

#ifdef WINDOWS

      //cs.dwExStyle |= WS_EX_CLIENTEDGE;

#endif

      cs.style &= ~WS_VISIBLE;

      return true;

   }


   bool frame_window::create_window(const char * lpszClassName,const char * lpszWindowName,uint32_t dwStyle,const RECT & rect, ::user::interaction * puiParent,const char * lpszMenuName,uint32_t dwExStyle, ::create * pcreate)
   {

      UNREFERENCED_PARAMETER(lpszMenuName);

      m_strTitle = lpszWindowName;    // save title for later

      if (!::user::interaction::create_window_ex(dwExStyle, lpszClassName, lpszWindowName, dwStyle, rect, puiParent, id(), pcreate))
      {

         TRACE(::aura::trace::category_AppMsg, 0, "Warning: failed to create frame_window.\n");

         return false;

      }

      return true;

   }


   bool frame_window::on_create_client(::user::create_struct *, ::create * pcreate)
   {

      if (pcreate != NULL && (pcreate->m_user->m_typeinfoNewView || pcreate->m_user->m_puiNew != NULL))
      {

         if (::user::impact::s_create_view(pcreate, null_rect(), this, "pane_first") == NULL)
         {

            return false;

         }

      }

      return true;

   }


   void frame_window::_001OnCreate(signal_details * pobj)
   {

      if (pobj->previous())
         return;

      SCAST_PTR(::message::create, pcreate, pobj);

         ENSURE_ARG(pcreate->m_lpcreatestruct != NULL);

      sp(::create) pContext = (::create *) pcreate->m_lpcreatestruct->lpCreateParams;

      pcreate->set_lresult(OnCreateHelper(pcreate->m_lpcreatestruct, pContext));

      pcreate->m_bRet = pcreate->get_lresult() == -1;

   }

   
   int32_t frame_window::OnCreateHelper(::user::create_struct * lpcs, ::create * pcreate)
   {

      // create special children first
      if (!on_create_client(lpcs, pcreate))
      {

         TRACE(::aura::trace::category_AppMsg, 0, "Failed to create client pane/::user::impact for frame.\n");

         return -1;

      }

      // post message for initial message string
      // trans   PostMessage(WM_SETMESSAGESTRING, __IDS_IDLEMESSAGE);

      // make sure the child windows have been properly sized
      //   on_layout();

      return 0;   // create ok

   }

   
   void frame_window::OnInitialFrameUpdate(bool bMakeVisible)
   {
      
      if(bMakeVisible)
      {
      
         if (GetParent() == NULL || !GetParent()->is_place_holder())
         {
         
            InitialFramePosition();
         
         }
         
      }

   }
   

   bool frame_window::LoadFrame(const char * pszMatter, uint32_t dwDefaultStyle, ::user::interaction * puiParent, ::create * pcreate)
   {

      UNREFERENCED_PARAMETER(pszMatter);
      UNREFERENCED_PARAMETER(dwDefaultStyle);
      UNREFERENCED_PARAMETER(puiParent);
      UNREFERENCED_PARAMETER(pcreate);

      return false;

      // only do this once
      //   ASSERT_VALID_IDR(nIDResource);
      //   ASSERT(m_nIDHelp == 0 || m_nIDHelp == nIDResource);

      /*m_nIDHelp = pszMatter;    // ID for help context (+HID_BASE_RESOURCE)

      string strFullString;
      if (strFullString.load_string(nIDResource))
      __extract_sub_string(m_strTitle, strFullString, 0);    // first sub-string

      VERIFY(__defer_register_class(__WNDFRAMEORVIEW_REG));

      // attempt to create the interaction_impl
      const char * lpszClass = GetIconWndClass(dwDefaultStyle, nIDResource);
      string strTitle = m_strTitle;
      if (!create_window_ex(0, lpszClass, strTitle, dwDefaultStyle, rectDefault,
      pParentWnd, nIDResource, (LPVOID) pContext))
      {
      return FALSE;   // will self destruct on failure normally
      }

      // save the default menu handle
      //ASSERT(get_handle() != NULL);
      // trans m_hMenuDefault = ::GetMenu(get_handle());
      m_hMenuDefault = NULL; // trans

      // load accelerator resource
      LoadAccelTable(MAKEINTRESOURCE(nIDResource));

      if (pContext == NULL)   // send initial update
      SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, TRUE, TRUE);

      return TRUE;*/
   }


   void frame_window::OnUpdateFrameMenu(HMENU hMenuAlt)
   {
      if (hMenuAlt == NULL)
      {
         // attempt to get default menu from ::user::document
         sp(::user::document) pDoc = GetActiveDocument();
         if (pDoc != NULL)
            hMenuAlt = pDoc->GetDefaultMenu();
         // use default menu stored in frame if none from ::user::document
         if (hMenuAlt == NULL)
            hMenuAlt = m_hMenuDefault;
      }
      // finally, set the menu
      // trans ::SetMenu(get_handle(), hMenuAlt);
   }

   void frame_window::InitialUpdateFrame(::user::document * pDoc, bool bMakeVisible)
   {
      // if the frame does not have an active ::user::impact, set to first pane
      sp(::user::impact) pview = NULL;
      if (GetActiveView() == NULL)
      {
         sp(::user::interaction) pwindow = GetDescendantWindow("pane_first");
         if (pwindow != NULL && base_class < ::user::impact > ::bases(pwindow))
         {
            pview = pwindow;
            SetActiveView(pview, FALSE);
         }
      }

      //   oswindow oswindow = get_handle();
      //   uint32_t dwStyle = ::GetWindowLong(oswindow, GWL_STYLE);
      //   bool bChild =  dwStyle & WS_CHILD;

      OnInitialFrameUpdate(bMakeVisible);
      
      if (bMakeVisible)
      {

         m_bLayoutEnable = true;

         // send initial update to all views (and other controls) in the frame
         SendMessageToDescendants(WM_INITIALUPDATE, 0, (LPARAM)0, TRUE, TRUE);

         // give ::user::impact a chance to save the focus (CFormView needs this)
         if (pview != NULL)
            pview->OnActivateFrame(WA_INACTIVE, this);

         // finally, activate the frame
         // (send the default show command unless the main desktop interaction_impl)
         int32_t nCmdShow = -1;      // default
         ActivateFrame(nCmdShow);
         if (pview != NULL)
            pview->OnActivateView(TRUE, pview, pview);
      }

      m_bLayoutEnable = true;

      // update frame counts and frame title (may already have been visible)
      if (pDoc != NULL)
         pDoc->update_frame_counts();
      on_update_frame_title(TRUE);

      if (GetParent() != NULL
         && GetParent()->is_place_holder()
         && (!oprop("should_not_be_automatically_holded_on_initial_update_frame").is_set()
         || !oprop("should_not_be_automatically_holded_on_initial_update_frame")))
      {
         GetParent()->place(this);
         //GetParent()->on_layout();
      }


   }

   void frame_window::InitialFramePosition(bool bForceRestore)
   {

      UNREFERENCED_PARAMETER(bForceRestore);

      if (m_bFrameMoveEnable)
      {

//         good_restore(NULL, true);

      }
      ActivateTopParent();
      ActivateFrame();
      //BringWindowToTop();
   }

   /////////////////////////////////////////////////////////////////////////////
   // frame_window closing down

   void frame_window::OnClose()
   {
      ::exception::throw_not_implemented(get_app());
      /*if (m_lpfnCloseProc != NULL)
      (*m_lpfnCloseProc)(this);

      // Note: only queries the active ::user::document
      sp(::user::document) pdocument = GetActiveDocument();
      if (pdocument != NULL && !pdocument->can_close_frame(this))
      {
      // ::user::document can't close right now -- don't close it
      return;
      }
      application* pApp = &System;
      if (pApp != NULL && pApp->m_puiMain == this)
      {
      // attempt to save all documents
      if (pdocument == NULL && !pApp->save_all_modified())
      return;     // don't close it

      // hide the application's windows before closing all the documents
      pApp->HideApplication();

      // close all documents first
      pApp->close_all_documents(FALSE);


      // there are cases where destroying the documents may destroy the
      //  main interaction_impl of the application.
      if (!afxContextIsDLL && pApp->m_puiMain == NULL)
      {
      __post_quit_message(0);
      return;
      }
      }

      // detect the case that this is the last frame on the ::user::document and
      // shut down with on_close_document instead.
      if (pdocument != NULL && pdocument->m_bAutoDelete)
      {
      bool bOtherFrame = FALSE;
      POSITION pos = pdocument->get_view_count();
      while (pos != NULL)
      {
      sp(::user::impact) pview = pdocument->get_view(pos);
      ENSURE_VALID(pview);
      if (pview->GetParentFrame() != this)
      {
      bOtherFrame = TRUE;
      break;
      }
      }
      if (!bOtherFrame)
      {
      pdocument->on_close_document();
      return;
      }

      // allow the ::user::document to cleanup before the interaction_impl is destroyed
      pdocument->pre_close_frame(this);
      }

      // then destroy the interaction_impl
      DestroyWindow();*/
   }

   void frame_window::_001OnDestroy(signal_details * pobj)
   {
      
      pobj->previous();
      

   }


   bool frame_window::_001OnCmdMsg(::aura::cmd_msg * pcmdmsg)
   {

      if(::user::interaction::_001OnCmdMsg(pcmdmsg))
         return true;

      return false;
   }



   /*
   // Delegate scroll messages to active ::user::impact as well
   void frame_window::OnHScroll(UINT, UINT, CScrollBar*)
   {
      sp(::user::interaction) pActiveView = GetActiveView();
      if (pActiveView != NULL)
      {
         // trans const MESSAGE* pMsg = GetCurrentMessage();
         // trans pActiveView->SendMessage(WM_HSCROLL, pMsg->wParam, pMsg->lParam);
      }
   }

   void frame_window::OnVScroll(UINT, UINT, CScrollBar*)
   {
      sp(::user::interaction) pActiveView = GetActiveView();
      if (pActiveView != NULL)
      {
         // trans      const MESSAGE* pMsg = GetCurrentMessage();
         // trans      pActiveView->SendMessage(WM_VSCROLL, pMsg->wParam, pMsg->lParam);
      }
   }
   */

   LRESULT frame_window::OnActivateTopLevel(WPARAM wParam, LPARAM lParam)
   {
      UNREFERENCED_PARAMETER(wParam);
      UNREFERENCED_PARAMETER(lParam);
      // trans   user::frame_window::OnActivateTopLevel(wParam, lParam);

      // exit Shift+F1 help mode on activation changes
      ExitHelpMode();


      // deactivate current active ::user::impact
      thread *pThread = get_thread();
      ASSERT(pThread);
      sp(::user::impact) pActiveView = GetActiveView();
      if (pActiveView == NULL)
         pActiveView = GetActiveFrame()->GetActiveView();
      if (pActiveView != NULL)
         pActiveView->OnActivateView(FALSE, pActiveView, pActiveView);

      return 0;

   }

   void frame_window::_001OnActivate(signal_details * pobj)
   {
      SCAST_PTR(::message::activate, pactivate, pobj);

      sp(::user::interaction) pActive = (pactivate->m_nState == WA_INACTIVE ? pactivate->m_pWndOther : this);

      pobj->previous();

      // get top level frame unless this is a child interaction_impl
      // determine if interaction_impl should be active or not
      sp(::user::frame_window) pTopLevel = (GetStyle() & WS_CHILD) ? this : GetTopLevelFrame();

      if (pTopLevel == NULL)
         pTopLevel = this;



      bool bStayActive =
         (pTopLevel == pActive ||
         (pActive && pTopLevel == pActive->GetTopLevelFrame() &&
         (pActive == pTopLevel ||
         (pActive && pActive->send_message(WM_FLOATSTATUS, FS_SYNCACTIVE) != 0))));
      pTopLevel->m_nFlags &= ~WF_STAYACTIVE;
      if (bStayActive)
         pTopLevel->m_nFlags |= WF_STAYACTIVE;

      // sync floating windows to the new state
      NotifyFloatingWindows(bStayActive ? FS_ACTIVATE : FS_DEACTIVATE);

      // get active ::user::impact (use active frame if no active ::user::impact)
      sp(::user::impact) pActiveView = GetActiveView();
      if (pActiveView == NULL)
         pActiveView = (GetActiveFrame()->GetActiveView());

      // when frame gets activated, re-activate current ::user::impact
      if (pActiveView != NULL)
      {
         if (pactivate->m_nState != WA_INACTIVE && !pactivate->m_bMinimized)
            pActiveView->OnActivateView(TRUE, pActiveView, pActiveView);

         // always notify the ::user::impact of frame activations
         pActiveView->OnActivateFrame(pactivate->m_nState, this);
      }

      RedrawWindow();

   }

   void frame_window::_001OnNcActivate(signal_details * pobj)
   {
      SCAST_PTR(::message::nc_activate, pncactivate, pobj);
         // stay active if WF_STAYACTIVE bit is on
      if (m_nFlags & WF_STAYACTIVE)
         pncactivate->m_bActive = TRUE;

      // but do not stay active if the interaction_impl is disabled
      if (!is_window_enabled())
         pncactivate->m_bActive = FALSE;

   }

   void frame_window::OnSysCommand(UINT nID, LPARAM lParam)
   {
      UNREFERENCED_PARAMETER(lParam);
      sp(::user::frame_window) pFrameWnd = GetTopLevelFrame();
      ENSURE_VALID(pFrameWnd);

      // set status bar as appropriate
      //   UINT nItemID = (nID & 0xFFF0);

      // don't interfere with system commands if not in help mode
      if (pFrameWnd->m_bHelpMode)
      {
         /*switch (nItemID)
         {
         case SC_SIZE:
         case SC_MOVE:
         case SC_MINIMIZE:
         case SC_MAXIMIZE:
         case SC_NEXTWINDOW:
         case SC_PREVWINDOW:
         case SC_CLOSE:
         case SC_RESTORE:
         case SC_TASKLIST:
         if (!SendMessage(WM_COMMANDHELP, 0,
         HID_BASE_COMMAND+ID_COMMAND_FROM_SC(nItemID)))
         SendMessage(WM_COMMAND, ID_DEFAULT_HELP);
         return;
         }*/
      }

      // call default functionality
      // trans   user::frame_window::OnSysCommand(nID, lParam);
   }


#ifdef WINDOWSEX


   /////////////////////////////////////////////////////////////////////////////
   // default frame processing

   // default drop processing will attempt to open the file
   void frame_window::OnDropFiles(HDROP hDropInfo)
   {

      UNREFERENCED_PARAMETER(hDropInfo);

   }

#endif

   // query end session for main frame will attempt to close it all down
   bool frame_window::OnQueryEndSession()
   {

      return TRUE;
   }

   // when Windows session ends, close all documents
   void frame_window::OnEndSession(bool bEnding)
   {
      UNREFERENCED_PARAMETER(bEnding);
   }

   /////////////////////////////////////////////////////////////////////////////
   // Support for Shell DDE Execute messages

   LRESULT frame_window::OnDDEInitiate(WPARAM wParam, LPARAM lParam)
   {


      return 0L;
   }


   // always ACK the execute command - even if we do nothing
   LRESULT frame_window::OnDDEExecute(WPARAM wParam, LPARAM lParam)
   {



      return 0L;

   }

   LRESULT frame_window::OnDDETerminate(WPARAM wParam, LPARAM lParam)
   {


      return 0L;

   }


   /////////////////////////////////////////////////////////////////////////////
   // frame_window attributes

   sp(::user::impact) frame_window::GetActiveView() const
   {

      ASSERT(m_pviewActive == NULL || base_class < ::user::impact >::bases(m_pviewActive));

      return m_pviewActive;

   }


   void frame_window::SetActiveView(sp(::user::impact) pViewNew, bool bNotify)
   {
#ifdef DEBUG
      if (pViewNew != NULL)
      {
         //trans      ASSERT(IsChild(pViewNew));
         //trans      ASSERT_KINDOF(::user::impact, pViewNew);
      }
#endif //DEBUG

      sp(::user::impact) pViewOld = m_pviewActive;
      if (pViewNew == pViewOld)
         return;     // do not re-activate if SetActiveView called more than once

      m_pviewActive = NULL;   // no active for the following processing

      // deactivate the old one
      if (pViewOld != NULL)
         pViewOld->OnActivateView(FALSE, pViewNew, pViewOld);

      // if the OnActivateView moves the active interaction_impl,
      //    that will veto this change
      if (m_pviewActive != NULL)
         return;     // already set
      m_pviewActive = pViewNew;
      // activate
      if (pViewNew != NULL)
      {
         if (bNotify)
         {
            pViewNew->OnActivateView(TRUE, pViewNew, pViewOld);
         }
      }
   }



   /////////////////////////////////////////////////////////////////////////////
   // Special ::user::impact swapping/activation

   void frame_window::OnSetFocus(sp(::user::interaction) pOldWnd)
   {
      UNREFERENCED_PARAMETER(pOldWnd);
      if (m_pviewActive != NULL)
         m_pviewActive->SetFocus();
      /*trans else
      user::frame_window::OnSetFocus(pOldWnd);
      */
   }

   ::user::document * frame_window::GetActiveDocument()
   {
      ASSERT_VALID(this);
      sp(::user::impact) pview = GetActiveView();
      if (pview != NULL)
         return pview->get_document();
      return NULL;
   }

   /////////////////////////////////////////////////////////////////////////////
   // Command prompts


   void frame_window::GetMessageString(UINT nID, string & rMessage) const
   {
      UNREFERENCED_PARAMETER(nID);
      UNREFERENCED_PARAMETER(rMessage);
      // load appropriate string
      ::exception::throw_not_implemented(get_app());
      /*   LPTSTR lpsz = rMessage.GetBuffer(255);
      if (::core::LoadString(nID, lpsz) != 0)
      {
      // first newline terminates actual string
      lpsz = _tcschr(lpsz, '\n');
      if (lpsz != NULL)
      *lpsz = '\0';
      }
      else
      {
      // not found
      TRACE(::aura::trace::category_AppMsg, 0, "Warning: no message line prompt for ID 0x%04X.\n", nID);
      }
      rMessage.ReleaseBuffer();*/
   }

   LRESULT frame_window::OnPopMessageString(WPARAM wParam, LPARAM lParam)
   {
      if (m_nFlags & WF_NOPOPMSG)
         return 0;

      return send_message(WM_SETMESSAGESTRING, wParam, lParam);
   }

   LRESULT frame_window::OnSetMessageString(WPARAM wParam, LPARAM lParam)
   {
      UINT nIDLast = m_nIDLastMessage;
      m_nFlags &= ~WF_NOPOPMSG;

      sp(::user::interaction) pMessageBar = GetMessageBar();
      if (pMessageBar != NULL)
      {
         const char * lpsz = NULL;
         string strMessage;

         // set the message bar text
         if (lParam != 0)
         {
            ASSERT(wParam == 0);    // can't have both an ID and a string
            lpsz = (const char *)lParam; // set an explicit string
         }
         else if (wParam != 0)
         {
            // map SC_CLOSE to PREVIEW_CLOSE when in print preview mode
            /*         if (wParam == __IDS_SCCLOSE && m_lpfnCloseProc != NULL)
            wParam = __IDS_PREVIEW_CLOSE;*/

            // get message associated with the ID indicated by wParam
            //NT64: Assume IDs are still 32-bit
            GetMessageString((UINT)wParam, strMessage);
            lpsz = strMessage;
         }
         pMessageBar->set_window_text(lpsz);

         // update owner of the bar in terms of last message selected
         sp(::user::frame_window) pFrameWnd = pMessageBar->GetParentFrame();
         if (pFrameWnd != NULL)
         {
            pFrameWnd->m_nIDLastMessage = (UINT)wParam;
            pFrameWnd->m_nIDTracking = (UINT)wParam;
         }
      }

      m_nIDLastMessage = (UINT)wParam;    // new ID (or 0)
      m_nIDTracking = (UINT)wParam;       // so F1 on toolbar buttons work
      return nIDLast;
   }

   LRESULT frame_window::OnHelpPromptAddr(WPARAM, LPARAM)
   {
      return (LRESULT)&m_dwPromptContext;
   }

   sp(::user::interaction) frame_window::GetMessageBar()
   {
      return GetDescendantWindow("status_bar");
   }

   void frame_window::OnEnterIdle(UINT nWhy, sp(::user::interaction) pWho)
   {
      UNREFERENCED_PARAMETER(pWho);
      // trans user::frame_window::OnEnterIdle(nWhy, pWho);
#ifdef WINDOWS
      if (nWhy != MSGF_MENU || m_nIDTracking == m_nIDLastMessage)
         return;
#else
      if (m_nIDTracking == m_nIDLastMessage)
         return;
#endif

      SetMessageText(m_nIDTracking);
      ASSERT(m_nIDTracking == m_nIDLastMessage);
   }

   void frame_window::SetMessageText(const char * lpszText)
   {
      send_message(WM_SETMESSAGESTRING, 0, (LPARAM)lpszText);
   }

   void frame_window::SetMessageText(UINT nID)
   {
      send_message(WM_SETMESSAGESTRING, (WPARAM)nID);
   }



   /////////////////////////////////////////////////////////////////////////////
   // Setting title of frame interaction_impl - UISG standard

   void frame_window::on_update_frame_title(bool bAddToTitle)
   {
      if ((GetStyle() & FWS_ADDTOTITLE) == 0)
         return;     // leave it alone!


      sp(::user::document) pdocument = GetActiveDocument();
      if (bAddToTitle && pdocument != NULL)
         UpdateFrameTitleForDocument(pdocument->get_title());
      else
         UpdateFrameTitleForDocument(NULL);
   }

   void frame_window::UpdateFrameTitleForDocument(const char * lpszDocName)
   {
      string WindowText;

      if (GetStyle() & FWS_PREFIXTITLE)
      {
         // get name of currently active ::user::impact
         if (lpszDocName != NULL)
         {
            WindowText += lpszDocName;

            // add current interaction_impl # if needed
            if (m_nWindow > 0)
            {

               string strText;

               // :%d will produce a maximum of 11 TCHARs
               strText.Format(":%d", m_nWindow);

               WindowText += strText;

            }

            WindowText += " - ";

         }
         WindowText += m_strTitle;
      }
      else
      {
         // get name of currently active ::user::impact
         WindowText += m_strTitle;
         if (lpszDocName != NULL)
         {
            WindowText += " - ";
            WindowText += lpszDocName;

            // add current interaction_impl # if needed
            if (m_nWindow > 0)
            {

               string strText;

               // :%d will produce a maximum of 11 TCHARs
               strText.Format(":%d", m_nWindow);

               WindowText += strText;

            }
         }
      }

      // set title if changed, but don't remove completely
      // Note: will be excessive for MDI Frame with maximized child
      set_window_text(WindowText);
   }

   /////////////////////////////////////////////////////////////////////////////

/*   void frame_window::OnSetPreviewMode(bool bPreview, CPrintPreviewState* pState)
   {
      ENSURE_ARG(pState != NULL);
      // default implementation changes control bars, menu and main pane interaction_impl


      // set visibility of standard ControlBars (only the first 32)
      //   uint32_t dwOldStates = 0;

      if (bPreview)
      {
         // Entering Print Preview


         // show any modeless dialogs, popup windows, float tools, etc
         ShowOwnedWindows(FALSE);

         // Hide the main pane
      }
      else
      {
         // Leaving Preview
         m_lpfnCloseProc = NULL;

         // shift original "pane_first" back to its rightful ID
         /*      sp(::user::interaction) oswindow = get_child_by_id(__IDW_PANE_SAVE);
         if (oswindow != NULL)
         {
         sp(::user::interaction) oswindow_Temp = get_child_by_id("pane_first");
         if (oswindow_Temp != NULL)
         __set_dialog_control_id_(oswindow_Temp, __IDW_PANE_SAVE);
         __set_dialog_control_id_(oswindow, "pane_first");
         }*/

/*         on_layout();


         // show any modeless dialogs, popup windows, float tools, etc
         ShowOwnedWindows(TRUE);
      }
   }
   */

   void frame_window::DelayUpdateFrameMenu(HMENU hMenuAlt)
   {
      m_hMenuAlt = hMenuAlt;
      m_nIdleFlags |= idleMenu;
   }


   sp(::user::frame_window) frame_window::GetActiveFrame()
   {
      // by default, the active frame is the frame itself (MDI is different)
      return this;
   }

   void frame_window::on_layout()
   {
      if (m_bInRecalcLayout)
         return;

      keep < bool > keepInRecalcLayout(&m_bInRecalcLayout, true, false, true);
      // clear idle flags for recalc on_layout if called elsewhere
      //   if (m_nIdleFlags & idleNotify)
      //   bNotify = TRUE;
      m_nIdleFlags &= ~(idleLayout | idleNotify);
      {

//         DWORD dwTime2 = ::get_tick_count();

         //TRACE("message_handler call time0= %d ms",dwTime2 - t_time1.operator DWORD_PTR());
         //TRACE("userframewindow call time1= %d ms",dwTime2 - t_time1.operator DWORD_PTR());

      }


      // reposition all the child windows (regardless of ID)
      if (GetStyle() & FWS_SNAPTOBARS)
      {

         rect rect(0, 0, 32767, 32767);

         RepositionBars(0, 0xffff, "pane_first", reposQuery, &rect, &rect, FALSE);

         RepositionBars(0, 0xffff, "pane_first", reposExtra, &m_rectBorder, &rect, TRUE);

         CalcWindowRect(&rect);

         SetWindowPos(0, 0, 0, rect.width(), rect.height(), SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);

      }
      else
      {

         RepositionBars(0, 0xffff, "pane_first", reposExtra, &m_rectBorder);

         {

//            DWORD dwTime2 = ::get_tick_count();

            //TRACE("message_handler call time0= %d ms",dwTime2 - t_time1.operator DWORD_PTR());
            //TRACE("userframewindoB call time2= %d ms",dwTime2 - t_time1.operator DWORD_PTR());

         }


      }


   }


   // frame_window implementation of OLE border space negotiation
   bool frame_window::NegotiateBorderSpace(UINT nBorderCmd, LPRECT lpRectBorder)
   {
      rect border, request;

      switch (nBorderCmd)
      {
      case borderGet:
         ASSERT(lpRectBorder != NULL);
         RepositionBars(0, 0xffff, "pane_first", reposQuery,
            lpRectBorder);
         break;

      case borderRequest:
         return TRUE;

      case borderSet:
         if (lpRectBorder == NULL)
         {
            if (!m_rectBorder.is_null())
            {
               // releasing all border space -- recalc needed
               m_rectBorder.null();
               return TRUE;
            }
            // original rect is is_empty & lpRectBorder is NULL, no recalc needed
            return FALSE;
         }
         if (!::EqualRect(m_rectBorder, lpRectBorder))
         {
            // the rects are different -- recalc needed
            m_rectBorder.copy(lpRectBorder);
            return TRUE;
         }
         return FALSE;   // no recalc needed

      default:
         ASSERT(FALSE);  // invalid frame_window::BorderCmd
      }

      return TRUE;
   }

   void frame_window::OnSize(UINT nType, int32_t cx, int32_t cy)
   {
   }

   bool frame_window::OnEraseBkgnd(::draw2d::dib * pdib)
   {
      UNREFERENCED_PARAMETER(pdib);
      if (m_pviewActive != NULL)
         return TRUE;        // active ::user::impact will erase/paint itself
      // for ::user::impact-less frame just use the default background fill
      return TRUE;
      //   return user::frame_window::OnEraseBkgnd(pgraphics);
   }

   LRESULT frame_window::OnRegisteredMouseWheel(WPARAM wParam, LPARAM lParam)
   {

      LRESULT lResult = 0;

      // convert from MSH_MOUSEWHEEL to WM_MOUSEWHEEL

#ifdef WINDOWSEX

      WORD keyState = 0;
      keyState |= Session.is_key_pressed(::user::key_control) ? MK_CONTROL : 0;
      keyState |= Session.is_key_pressed(::user::key_shift) ? MK_SHIFT : 0;

      oswindow hwFocus = ::GetFocus();
      const oswindow hwDesktop = ::GetDesktopWindow();

      if (hwFocus == NULL)
         lResult = send_message(WM_MOUSEWHEEL, (wParam << 16) | keyState, lParam);
      else
      {
         do {
            lResult = ::SendMessage(hwFocus, WM_MOUSEWHEEL,
               (wParam << 16) | keyState, lParam);
            hwFocus = ::GetParent(hwFocus);
         } while (lResult == 0 && hwFocus != NULL && hwFocus != hwDesktop);
      }

#else

      ::exception::throw_not_implemented(get_app());

#endif

      return lResult;

   }


   void frame_window::ActivateFrame(int32_t nCmdShow)
      // nCmdShow is the normal show mode this frame should be in
   {
      // translate default nCmdShow (-1)
      if (nCmdShow == -1)
      {
         if (!IsWindowVisible())
            nCmdShow = SW_SHOWNORMAL;
         else if (WfiIsIconic())
            nCmdShow = SW_RESTORE;
      }

      // bring to top before showing
      BringToTop(nCmdShow);

      if (nCmdShow != -1)
      {
         // show the interaction_impl as specified
         ShowWindow(nCmdShow);

         // and finally, bring to top after showing
         BringToTop(nCmdShow);
      }

      //_001UpdateWindow();

   }

   void frame_window::BringToTop(int32_t nCmdShow)
   {

      ::user::interaction::BringToTop(nCmdShow);

   }


   /////////////////////////////////////////////////////////////////////////////
   // frame_window Diagnostics



   //void frame_window::dump(dump_context & dumpcontext) const
   //{
   //   ::user::frame_window::dump(dumpcontext);

   //   dumpcontext << "m_hAccelTable = " << (void *)m_hAccelTable;
   //   dumpcontext << "\nm_nWindow = " << m_nWindow;
   //   dumpcontext << "\nm_nIDHelp = " << m_strMatterHelp;
   //   dumpcontext << "\nm_nIDTracking = " << m_nIDTracking;
   //   dumpcontext << "\nm_nIDLastMessage = " << m_nIDLastMessage;
   //   if (m_pviewActive != NULL)
   //      dumpcontext << "\nwith active ::user::impact: " << m_pviewActive.m_p;
   //   else
   //      dumpcontext << "\nno active ::user::impact";

   //   dumpcontext << "\n";
   //}


   bool frame_window::IsTracking() const
   {
      /*   return m_nIDTracking != 0 &&
      m_nIDTracking != __IDS_HELPMODEMESSAGE &&
      m_nIDTracking != __IDS_IDLEMESSAGE;*/
      return FALSE;
   }


   bool frame_window::ShowWindow(int32_t nCmdShow)
   {
      bool bResult = interaction::ShowWindow(nCmdShow);
      if (GetParent() != NULL
         && nCmdShow == SW_RESTORE)
      {
         InitialFramePosition(true);
      }
      return bResult;
   }



   void frame_window::_001OnSysCommand(signal_details * pobj)
   {

#ifdef WINDOWS

      SCAST_PTR(::message::base, pbase, pobj);

      if (GetParent() == NULL)
      {

         if (pbase->m_wparam == SC_RESTORE)
         {

            WfiRestore(true);

            pbase->m_bRet = true;

            pbase->set_lresult(0);

         }

      }

#else

      ::exception::throw_not_implemented(get_app());

#endif

   }


   // frame_window
   void frame_window::DelayUpdateFrameTitle()
   {
      m_nIdleFlags |= idleTitle;
   }
   void frame_window::DelayRecalcLayout(bool bNotify)
   {
      m_nIdleFlags |= (idleLayout | (bNotify ? idleNotify : 0));
   };
   bool frame_window::InModalState() const
   {
      return m_cModalStack != 0;
   }
   void frame_window::set_title(const char * lpszTitle)
   {
      m_strTitle = lpszTitle;
   }
   string frame_window::get_title() const
   {
      return m_strTitle;
   }



   bool frame_window::LoadToolBar(id idToolBar, const char * pszToolBar, uint32_t dwCtrlStyle, uint32_t dwStyle)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   void frame_window::CommonConstruct()
   {
      // trans ASSERT(get_handle() == NULL);

      m_nWindow = -1;                 // unknown interaction_impl ID
      m_bAutoMenuEnable = TRUE;       // auto enable on by default
//      m_lpfnCloseProc = NULL;
      m_hMenuDefault = NULL;
      m_hAccelTable = NULL;
      //m_nIDHelp = 0;
      m_nIDTracking = 0;
      m_nIDLastMessage = 0;

      m_cModalStack = 0;              // initialize modality support
      m_hMenuAlt = NULL;
      m_nIdleFlags = 0;               // no idle work at start
      m_rectBorder.null();
      m_dwPromptContext = 0;

      m_pNextFrameWnd = NULL;         // not in list yet

      m_bInRecalcLayout = FALSE;
      m_nShowDelay = -1;              // no delay pending

      AddFrameWnd();
   }






   extern const CHAR _vfxWndFrameOrView[];
   //const CHAR _vfxWndFrameOrView[] = __WNDFRAMEORVIEW;

   /////////////////////////////////////////////////////////////////////////////
   // frame_window second phase creation

   //bool frame_window::pre_create_window(::user::create_struct& cs)
   //{

   //   if ((cs.style & FWS_ADDTOTITLE))
   //      cs.style |= FWS_PREFIXTITLE;

   //   //if (afxData.bWin4)
   //   cs.dwExStyle |= WS_EX_CLIENTEDGE;

   //   return TRUE;
   //}



   //void frame_window::_001OnCreate(signal_details * pobj)
   //{
   //   UNREFERENCED_PARAMETER(pobj);

   //   if(pobj->previous())
   //      return;

   //}




   void frame_window::RemoveControlBar(::user::control_bar *pBar)
   {
      m_barptra.remove(pBar);
   }


   /////////////////////////////////////////////////////////////////////////////
   // frame_window command/message routing







   /////////////////////////////////////////////////////////////////////////////
   // default frame processing


   // query end session for main frame will attempt to close it all down
   void frame_window::_001OnQueryEndSession(signal_details * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

   }


   /////////////////////////////////////////////////////////////////////////////
   // Special ::user::impact swapping/activation

   void frame_window::_001OnSetFocus(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      if (m_pviewActive != NULL)
         m_pviewActive->SetFocus();
   }



   void frame_window::OnUpdateControlBarMenu(cmd_ui * pcmdui)
   {
      /*      ASSERT(ID_VIEW_STATUS_BAR == "status_bar");
      ASSERT(ID_VIEW_TOOLBAR == __IDW_TOOLBAR);
      ASSERT(ID_VIEW_REBAR == __IDW_REBAR);*/

      pcmdui->ContinueRouting();
   }

   bool frame_window::OnBarCheck(UINT nID)
   {
      UNREFERENCED_PARAMETER(nID);
      /*ASSERT(ID_VIEW_STATUS_BAR == "status_bar");
      ASSERT(ID_VIEW_TOOLBAR == __IDW_TOOLBAR);
      ASSERT(ID_VIEW_REBAR == __IDW_REBAR);*/

      return FALSE;
   }


   /////////////////////////////////////////////////////////////////////////////
   // Setting title of frame interaction_impl - UISG standard



   //void frame_window::DelayUpdateFrameMenu(HMENU hMenuAlt)
   //{
   //   m_hMenuAlt = hMenuAlt;
   //   m_nIdleFlags |= idleMenu;
   //}

   //void frame_window::OnIdleUpdateCmdUI(signal_details * pobj)
   //{
   //   // update menu if necessary
   //   if(m_nIdleFlags & idleMenu)
   //   {
   //      m_nIdleFlags &= ~idleMenu;
   //      OnUpdateFrameMenu(m_hMenuAlt);
   //   }

   //   // update title if necessary
   //   if(m_nIdleFlags & idleTitle)
   //      on_update_frame_title(TRUE);

   //   // recalc on_layout if necessary
   //   if(m_nIdleFlags & idleLayout)
   //   {
   //      on_layout();
   //      UpdateWindow();
   //   }

   //   // set the current message string if necessary
   //   if(m_nIDTracking != m_nIDLastMessage)
   //   {
   //      SetMessageText(m_nIDTracking);
   //      ASSERT(m_nIDTracking == m_nIDLastMessage);
   //   }
   //   m_nIdleFlags = 0;

   //   for(auto & bar : m_barptra.refa())
   //   {
   //      bar._001OnIdleUpdateCmdUI(pobj);
   //   }
   //}


   void frame_window::_001OnIdleUpdateCmdUI(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      // update menu if necessary
      if (m_nIdleFlags & idleMenu)
         OnUpdateFrameMenu(m_hMenuAlt);

      // update title if necessary
      if (m_nIdleFlags & idleTitle)
         on_update_frame_title(TRUE);

      // recalc on_layout if necessary
      if (m_nIdleFlags & idleLayout)
      {
         on_layout();
         UpdateWindow();
      }

      // set the current message string if necessary
      if (m_nIDTracking != m_nIDLastMessage)
      {
         SetMessageText(m_nIDTracking);
         ASSERT(m_nIDTracking == m_nIDLastMessage);
      }

      for(auto & bar : m_barptra.refa())
      {
         bar._001OnIdleUpdateCmdUI(pobj);
      }

      m_nIdleFlags = 0;
   }



   void frame_window::_001OnSize(signal_details * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

   }









   // in this file for is_kind_of library granularity (is_kind_of references these)
   //// IMPLEMENT_DYNCREATE(frame_window, ::user::interaction)
   //// IMPLEMENT_DYNAMIC(::user::impact, ::user::interaction)
   //// IMPLEMENT_DYNAMIC(::user::control_bar, ::user::interaction)

   /////////////////////////////////////////////////////////////////////////////



   /////////////////////////////////////////////////////////////////////////////
   // frame_window (here for library granularity)



   sp(::user::interaction) frame_window::WindowDataGetWnd()
   {
      return this;
   }


   // dwDockBarMap
   /*const uint32_t frame_window::dwDockBarMap[4][2] =
   {
   { __IDW_DOCKBAR_TOP,      CBRS_TOP    },
   { __IDW_DOCKBAR_BOTTOM,   CBRS_BOTTOM },
   { __IDW_DOCKBAR_LEFT,     CBRS_LEFT   },
   { __IDW_DOCKBAR_RIGHT,    CBRS_RIGHT  },
   };*/



   bool frame_window::BaseOnControlEvent(::user::control_event * pevent)
   {

      return ::user::interaction::BaseOnControlEvent(pevent);

   }

   
   void frame_window::_000OnDraw(::draw2d::graphics * pgraphics)
   {

      UNREFERENCED_PARAMETER(pgraphics);

   }


   void frame_window::AddControlBar(::user::control_bar *pBar)
   {

      m_barptra.add_unique(pBar);

   }



   bool frame_window::_001HasCommandHandler(id id)
   {

      if (command_target_interface::_001HasCommandHandler(id))
         return true;

      if (m_pviewActive != NULL)
      {

         if (m_pviewActive->_001HasCommandHandler(id))
            return true;

      }

      if (GetParent() != NULL)
      {

         if (GetParent()->_001HasCommandHandler(id))
            return true;

      }

      return false;

   }


   bool frame_window::get_window_minimum_size(::size & sizeMin)
   {

      if(get_appearance() == ::user::AppearanceMinimal)
      {

         sizeMin.cx = 8;

         sizeMin.cy = 8;

      }
      else
      {

         Session.get_window_minimum_size(&sizeMin);

      }

      return true;

   }

   
   ::user::front_end_schema * frame_window::get_user_front_end_schema()
   {

      return NULL;

   }
   

} // namespace user




