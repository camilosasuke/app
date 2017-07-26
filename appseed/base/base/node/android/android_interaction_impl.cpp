//#include "framework.h" // from "base/user/user.h"
//#include "base/user/user.h"
//#include "aura/node/android/android.h"
//#include "android.h"

CLASS_DECL_BASE int g_iMouseDown = 0;

CLASS_DECL_BASE thread_int_ptr < DWORD_PTR > t_time1;
CLASS_DECL_BASE thread_int_ptr < DWORD_PTR > t_time2;


extern CLASS_DECL_BASE thread_int_ptr < DWORD_PTR > t_time1;







namespace android
{


   interaction_impl::interaction_impl() :
      ::aura::timer_array(get_app())
   {

      m_bScreenRelativeMouseMessagePosition = true;
      m_oswindow           = NULL;
      //m_bMouseHover        = false;
      m_bUpdateGraphics    = false;
      m_bEnabled           = true;
      m_pthreadDraw = NULL;

      ZERO(m_rectParentClient);
      ZERO(m_size);
      ZERO(m_pt);



   }




   void interaction_impl::construct(oswindow oswindow)
   {

      m_bScreenRelativeMouseMessagePosition = true;
      m_oswindow           = NULL;
      //m_bMouseHover        = false;
      m_bUpdateGraphics    = false;
      m_bEnabled           = true;
      m_pthreadDraw = NULL;
      ZERO(m_rectParentClient);
      ZERO(m_size);
      ZERO(m_pt);

      set_handle(oswindow);

   }


   interaction_impl::interaction_impl(::aura::application * papp):
      ::object(papp),
      ::aura::timer_array(papp)
   {
      m_bScreenRelativeMouseMessagePosition = true;
    //  m_bScreenRelativeMouseMessagePosition = false;
      m_oswindow           = NULL;
      m_bUpdateGraphics    = false;
      m_bIgnoreSizeEvent   = false;
      m_bIgnoreMoveEvent   = false;
      m_bEnabled           = true;
      m_pthreadDraw = NULL;
      ZERO(m_rectParentClient);
      ZERO(m_size);
      ZERO(m_pt);

   }


   interaction_impl::~interaction_impl()
   {

      if(m_pauraapp != NULL &&  m_pauraapp->m_pbasesession != NULL &&  m_pauraapp->m_pbasesession->m_puser != NULL)
      {

         if(Session.user()->m_pwindowmap != NULL)
         {

            Session.user()->m_pwindowmap->m_map.remove_key((int_ptr)get_handle());

         }

      }

   }


   ::user::interaction_impl * interaction_impl::from_os_data(void * pdata)
   {

      return from_handle((oswindow)pdata);

   }


   void * interaction_impl::get_os_data() const
   {

      return (void *)get_handle();

   }



   // Change a interaction_impl's style

   __STATIC bool CLASS_DECL_BASE __modify_style(oswindow oswindow,int32_t nStyleOffset,
      uint32_t dwRemove,uint32_t dwAdd,UINT nFlags)
   {
      ASSERT(oswindow != NULL);
      uint32_t dwStyle = ::GetWindowLong(oswindow,nStyleOffset);
      uint32_t dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
      if(dwStyle == dwNewStyle)
         return FALSE;

      ::SetWindowLong(oswindow,nStyleOffset,dwNewStyle);
      if(nFlags != 0)
      {
         ::SetWindowPos(oswindow,NULL,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
      }
      return TRUE;
   }

   bool interaction_impl::ModifyStyle(oswindow oswindow,uint32_t dwRemove,uint32_t dwAdd,UINT nFlags)
   {
      return __modify_style(oswindow,GWL_STYLE,dwRemove,dwAdd,nFlags);
   }


   bool interaction_impl::ModifyStyleEx(oswindow oswindow,uint32_t dwRemove,uint32_t dwAdd,UINT nFlags)
   {

      return __modify_style(oswindow,GWL_EXSTYLE,dwRemove,dwAdd,nFlags);

   }


   LRESULT interaction_impl::Default()
   {

      return 0;

   }


   ::user::interaction * interaction_impl::FromHandlePermanent(oswindow oswindow)
   {

      if (oswindow->get_impl() == NULL)
      {

         return NULL;

      }

      return oswindow->get_impl()->m_pui;

   }


   bool interaction_impl::Attach(oswindow hWndNew)
   {

      ASSERT(get_handle() == NULL);

      if (hWndNew == NULL)
         return false;

      m_oswindow = hWndNew;

      return true;

   }

   oswindow interaction_impl::Detach()
   {

      oswindow hwnd = (oswindow)get_handle();

      if (hwnd != NULL)
      {

         m_oswindow = NULL;

      }

      return hwnd;

   }


   void interaction_impl::pre_subclass_window()
   {


   }


   bool interaction_impl::create_window_ex(::user::interaction * pui, uint32_t dwExStyle,const char * lpszClassName,const char * lpszWindowName,uint32_t dwStyle,const RECT & rect,::user::interaction * puiParent,id id,LPVOID lpParam)
   {

      if(puiParent == NULL)
      {

         if(!native_create_window_ex(pui, dwExStyle,lpszClassName,lpszWindowName,dwStyle,rect,NULL,id,lpParam))
            return false;

      }
      else
      {

         if(!native_create_window_ex(pui, dwExStyle,lpszClassName,lpszWindowName,dwStyle,rect,puiParent->get_safe_handle(),id,lpParam))
            return false;

      }

      return true;

   }


   bool interaction_impl::native_create_window_ex(::user::interaction * pui, uint32_t dwExStyle,const char * lpszClassName,const char * lpszWindowName,uint32_t dwStyle,const RECT & rect,oswindow oswindowParent,id id,LPVOID lpParam)
   {

      //::simple_message_box(NULL,"h1","h1",MB_OK);
      UNREFERENCED_PARAMETER(id);
      ENSURE_ARG(lpszWindowName == NULL || __is_valid_string(lpszWindowName));


      m_pui = pui;

      // allow modification of several common create parameters
      ::user::create_struct cs;
      cs.dwExStyle = dwExStyle;


      //string strClass = calc_window_class();
      //cs.lpszClass = strClass.is_empty() ? NULL : (const char *)strClass;
      cs.lpszClass = NULL;
      cs.lpszName = lpszWindowName;
      cs.style = dwStyle;

      cs.x = rect.left;
      cs.y = rect.top;
      cs.cx = width(rect);
      cs.cy = height(rect);

      cs.hwndParent = oswindowParent;
      //   cs.hMenu = oswindow_Parent == NULL ? NULL : nIDorHMenu;
      cs.hMenu = NULL;
      cs.hInstance = System.m_hinstance;
      cs.lpCreateParams = lpParam;

      if(!m_pui->pre_create_window(cs))
      {

         PostNcDestroy();

         m_pui = NULL;

         return false;

      }

      if(cs.hwndParent == NULL)
      {

         cs.style &= ~WS_CHILD;

      }

      //oswindow oswindow = ::CreateWindowEx(cs.dwExStyle,cs.lpszClass,cs.lpszName,cs.style,cs.x,cs.y,cs.cx,cs.cy,cs.hwndParent,cs.hMenu,cs.hInstance,cs.lpCreateParams);


      if (cs.hwndParent == HWND_MESSAGE)
      {

         m_pui->m_bMessageWindow = true;

         return true;

      }
      else
      {

         m_pui->m_bMessageWindow = false;

         m_oswindow = oswindow_get(this);

         //if (cs.style & WS_VISIBLE)
         {

            m_pui->ModifyStyle(0, WS_VISIBLE);

         }
         //else
         //{

         //   m_pui->ModifyStyle(WS_VISIBLE, 0);

         //}

         // m_pui->m_bVisible = true;

         //m_oswindow->set_user_interaction(m_pui);

      }


      set_handle(m_oswindow);

      //::simple_message_box(NULL,"h2","h2",MB_OK);

      if(m_oswindow == NULL)
      {

         //::simple_message_box(NULL,"h3","h3",MB_OK);

         if (m_pauraapp == NULL)
         {

            PostNcDestroy();

            m_pui = NULL;

            return false;

         }

         uint32_t dwLastError = GetLastError();
         string strLastError = FormatMessageFromSystem(dwLastError);
         string strMessage;
         strMessage.Format("%s\n\nSystem Error Code: %d",strLastError,dwLastError);


         TRACE(::aura::trace::category_AppMsg,0,"Warning: Window creation failed: GetLastError returned:\n");

         TRACE(::aura::trace::category_AppMsg,0,"%s\n",strMessage);

         try
         {

            if(dwLastError == 0x0000057e)
            {

               TRACE("Cannot create a top-level child interaction_impl.");

            }
            else
            {

               TRACE0(strMessage);

            }

         }
         catch(...)
         {

         }

      }

      if(m_oswindow == NULL)
      {

         PostNcDestroy();

         m_pui = NULL;

         return false;

      }

      output_debug_string("android_interaction_impl send WM_CREATE");

      send_message(WM_CREATE, 0, (LPARAM)&cs);

      send_message(WM_SIZE);

      //WNDCLASS wndcls;
      //if(lpszClassName != NULL &&
      //   GetClassInfo(System.m_hinstance,lpszClassName,&wndcls) &&
      //   wndcls.hIcon != NULL)
      //{
      //   Application.set_icon(m_pui,new ::visual::icon(wndcls.hIcon),false);
      //   Application.set_icon(m_pui,new ::visual::icon(wndcls.hIcon),true);
      //}
      //::simple_message_box(NULL,"h4","h4",MB_OK);
      //      oswindow oswindowHandle = get_handle();
      if(m_oswindow != get_handle())
      {
         //::simple_message_box(NULL,"h4.dame","h4.dame",MB_OK);

         ASSERT(FALSE); // should have been set in send msg hook

      }
      //::simple_message_box(NULL,"h4.ok","h4.ok",MB_OK);

      return true;

   }


   bool interaction_impl::initialize_native_window(::user::native_window_initialize * pinitialize)
   {

      //RECT rect;


      //rect.origin.x = pinitialize->m_rect.left;
      //rect.origin.y = pinitialize->m_rect.top;
      //rect.size.width = width(pinitialize->m_rect);
      //rect.size.height = height(pinitialize->m_rect);

      m_oswindow = oswindow_get(this);

      m_oswindow->set_impl(this);

      m_pui->SetWindowPos(ZORDER_TOP, pinitialize->m_rect, SWP_SHOWWINDOW);

      m_pui->ModifyStyle(0, WS_VISIBLE);

      //      m_pthread = dynamic_cast < ::thread * > (::get_thread());

      output_debug_string("android_interaction_impl m_spgraphics alloc");

      m_spgraphics.alloc(allocer());

      m_spgraphics->on_create_window(this);

      //m_oswindow->set_impl(this);

      m_pui->ModifyStyle(0, WS_VISIBLE);

      m_pui->send_message(WM_CREATE);

      m_pui->send_message(WM_SIZE);

      m_pui->send_message(WM_MOVE);

      m_pui->send_message(WM_SHOWWINDOW, 1);

      output_debug_string("android_interaction_impl on _create_window");

      output_debug_string("android_interaction_impl initialize (width=" + ::str::from(width(pinitialize->m_rect)) + ",height=" + ::str::from(height(pinitialize->m_rect)) + ")");

      return true;

   }

   // for child android
   bool interaction_impl::pre_create_window(::user::create_struct& cs)
   {

      return true;

   }


   bool interaction_impl::create_window(::user::interaction * pui, const char * lpszClassName,const char * lpszWindowName,uint32_t dwStyle,const RECT & rect,::user::interaction * pParentWnd,id id, ::create * pcreate)
   {

      // can't use for desktop or pop-up android (use create_window_ex instead)
      ASSERT(pParentWnd != NULL);
      ASSERT((dwStyle & WS_POPUP) == 0);

      return create_window_ex(pui, 0,lpszClassName,lpszWindowName,dwStyle | WS_CHILD,rect,pParentWnd,id,(LPVOID)pcreate);

   }


   bool interaction_impl::create_message_queue(::user::interaction * pui, const char * pszName)
   {

      if(IsWindow())
      {

         DestroyWindow();

      }

      if(!native_create_window_ex(pui, 0,NULL,pszName,WS_CHILD,null_rect(),HWND_MESSAGE,0,NULL))
      {

         return false;

      }

      return true;

   }


   //string interaction_impl:: calc_window_class()
   //{

   //   uint32_t uiStyle = m_pui->get_window_default_style();

   //   string strIcon = m_pui->get_window_icon_matter();

   //   if(strIcon.has_char())
   //   {

   //      return calc_icon_window_class(uiStyle,strIcon);

   //   }
   //   else
   //   {

   //      return get_user_interaction_window_class(m_pui);

   //   }

   //}


   //string interaction_impl::calc_icon_window_class(uint32_t dwDefaultStyle,const char * pszMatter)
   //{

   //   string strPath = Application.dir().matter(pszMatter,"icon.ico");

   //   HICON hIcon = (HICON) ::LoadImage(NULL,strPath,IMAGE_ICON,256,256,LR_LOADFROMFILE);

   //   string strClass = get_user_interaction_window_class(m_pui);

   //   if(hIcon != NULL)
   //   {


   //      // will fill lpszClassName with default WNDCLASS name
   //      // ignore instance handle from pre_create_window.

   //      WNDCLASS wndcls;
   //      if(strClass.has_char() && GetClassInfo(System.m_hinstance,strClass,&wndcls) && wndcls.hIcon != hIcon)
   //      {
   //         // register a very similar WNDCLASS
   //         return __register_window_class(get_app(),wndcls.style,wndcls.hCursor,wndcls.hbrBackground,hIcon);
   //      }
   //   }

   //   return strClass;

   //}


   void interaction_impl::install_message_handling(::message::dispatch * pinterface)
   {

      last_install_message_handling(pinterface);
      ::user::interaction_impl::install_message_handling(pinterface);

      IGUI_WIN_MSG_LINK(WM_NCDESTROY,pinterface,this,&interaction_impl::_001OnNcDestroy);
      if(!m_pui->m_bMessageWindow)
      {
         IGUI_WIN_MSG_LINK(WM_PAINT,pinterface,this,&interaction_impl::_001OnPaint);
         IGUI_WIN_MSG_LINK(WM_PRINT,pinterface,this,&interaction_impl::_001OnPrint);
      }
      m_pui->install_message_handling(pinterface);
      IGUI_WIN_MSG_LINK(WM_CREATE,pinterface,this,&interaction_impl::_001OnCreate);
      if(!m_pui->m_bMessageWindow)
      {

         IGUI_WIN_MSG_LINK(WM_SETCURSOR,pinterface,this,&interaction_impl::_001OnSetCursor);

         //IGUI_WIN_MSG_LINK(WM_ERASEBKGND,pinterface,this,&interaction_impl::_001OnEraseBkgnd);

         //IGUI_WIN_MSG_LINK(WM_SIZE,pinterface,this,&interaction_impl::_001OnSize);

         //IGUI_WIN_MSG_LINK(WM_WINDOWPOSCHANGING,pinterface,this,&interaction_impl::_001OnWindowPosChanging);
         //IGUI_WIN_MSG_LINK(WM_WINDOWPOSCHANGED,pinterface,this,&interaction_impl::_001OnWindowPosChanged);
         //IGUI_WIN_MSG_LINK(WM_GETMINMAXINFO,pinterface,this,&interaction_impl::_001OnGetMinMaxInfo);

//         IGUI_WIN_MSG_LINK(WM_SHOWWINDOW,pinterface,this,&interaction_impl::_001OnShowWindow);

         //IGUI_WIN_MSG_LINK(ca2m_PRODEVIAN_SYNCH,pinterface,this,&interaction_impl::_001OnProdevianSynch);

         prio_install_message_handling(pinterface);

      }

      IGUI_WIN_MSG_LINK(WM_DESTROY,pinterface,this,&interaction_impl::_001OnDestroy);

      IGUI_WIN_MSG_LINK(WM_NCCALCSIZE,pinterface,this,&interaction_impl::_001OnNcCalcSize);

   }

   void interaction_impl::win_update_graphics()
   {

      single_lock sl(m_pui->m_pmutex,false);

      if(!sl.lock())
      {
         m_bUpdateGraphics = true;
         return;
      }

      m_bUpdateGraphics = false;

      rect rectWindow;

      GetWindowRect(rectWindow);

      m_pt = rectWindow.top_left();

      if(rectWindow.area() <= 0)
         return;

      if(m_size != rectWindow.size())
      {

         //if(m_spdib.is_null())
         //   m_spdib.alloc(allocer());

         //m_spdib->create(rectWindow.size());

         m_size = rectWindow.size();

      }




   }




   //void interaction_impl::_001OnSize(signal_details * pobj)
   //{

   //   SCAST_PTR(::message::size,psize,pobj);

   //   if(m_bIgnoreSizeEvent)
   //   {

   //      pobj->m_bRet = true;

   //      return;

   //   }
   //   else if(!(GetExStyle() & WS_EX_LAYERED))
   //   {

   //      ::rect r;

   //      ::GetWindowRect(get_handle(),r);

   //      ::copy(m_rectParentClient,r);

   //      if(GetParent() != NULL)
   //      {

   //         GetParent()->ScreenToClient(m_rectParentClient);

   //      }

   //   }


   //}


   void interaction_impl::_001OnDestroy(signal_details * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

      Default();

   }


   void interaction_impl::_001OnNcCalcSize(signal_details * pobj)
   {

      SCAST_PTR(::message::base,pbase,pobj);

      pbase->set_lresult(0);

      pobj->m_bRet = true;

   }


   // WM_NCDESTROY is the absolute LAST message sent.
   void interaction_impl::_001OnNcDestroy(signal_details * pobj)
   {

      single_lock sl(m_pui->m_pauraapp->m_pmutex,TRUE);

      ::window_sp pwindow;

      //if(m_pui->m_pauraapp != NULL && m_pui->m_pauraapp->m_pthreadimpl.is_set())
      //{

      //   synch_lock sl(&m_pui->m_pauraapp->m_pthreadimpl->m_mutexUiPtra);

      //   if(m_pui->m_pauraapp->m_pthreadimpl->m_spuiptra.is_set())
      //   {

      //      m_pui->m_pauraapp->m_pthreadimpl->m_spuiptra->remove(m_pui);

      //   }

      //}



      pobj->m_bRet = true;
      // cleanup main and active android
      ::thread* pThread = ::get_thread();
      if(pThread != NULL)
      {
         if(pThread->get_active_ui() == m_pui)
            pThread->set_active_ui(NULL);
      }

      // cleanup tooltip support
      if(m_pui != NULL)
      {
         if(m_pui->m_nFlags & WF_TOOLTIPS)
         {
         }
      }

      //// call default, unsubclass, and detach from the map
      //WNDPROC pfnWndProc = WNDPROC(::GetWindowLongPtr(get_handle(),GWLP_WNDPROC));
      //Default();
      //if(WNDPROC(::GetWindowLongPtr(get_handle(),GWLP_WNDPROC)) == pfnWndProc)
      //{
      //   WNDPROC pfnSuper = *GetSuperWndProcAddr();
      //   if(pfnSuper != NULL)
      //      ::SetWindowLongPtr(get_handle(),GWLP_WNDPROC,reinterpret_cast<int_ptr>(pfnSuper));
      //}

      detach();
      ASSERT(get_handle() == NULL);
      //m_pfnDispatchWindowProc = &interaction_impl::_start_user_message_handler;
      // call special post-cleanup routine
      PostNcDestroy();
      m_pui->PostNcDestroy();

   }

   void interaction_impl::PostNcDestroy()
   {

      try
      {

         signalizable_disconnect_all();

      }
      catch(...)
      {

      }


      set_handle(NULL);
      //if(is_heap())
      //{
      //   m_pui = NULL;
      //}
   }

   void interaction_impl::on_final_release()
   {
      if(get_handle() != NULL)
         DestroyWindow();    // will call PostNcDestroy
      else
         PostNcDestroy();
   }

   void interaction_impl::assert_valid() const
   {
      if(((::android::interaction_impl *)this)->get_handle() == NULL)
         return;     // null (unattached) android are valid

      // check for special wnd??? values
///      ASSERT(HWND_TOP == NULL);       // same as desktop
      //if(((::android::interaction_impl *)this)->get_handle() == HWND_BOTTOM)
      //{
      //}
      //else if(((::android::interaction_impl *)this)->get_handle() == HWND_TOPMOST)
      //{
      //}
      //else if(((::android::interaction_impl *)this)->get_handle() == HWND_NOTOPMOST)
      //{
      //}
      //else
      //{
      //   // should be a normal interaction_impl
      //   ASSERT(::IsWindow(((::android::interaction_impl *)this)->get_handle()));

      //}
   }


   void interaction_impl::dump(dump_context & dumpcontext) const
   {
      object::dump(dumpcontext);

      dumpcontext << "\nm_oswindow_ = " << ((::android::interaction_impl *)this)->get_handle();

      //if(((::android::interaction_impl *)this)->get_handle() == NULL || ((::android::interaction_impl *)this)->get_handle() == HWND_BOTTOM ||
      //   ((::android::interaction_impl *)this)->get_handle() == HWND_TOPMOST || ((::android::interaction_impl *)this)->get_handle() == HWND_NOTOPMOST)
      //{
      //   // not a normal interaction_impl - nothing more to dump
      //   return;
      //}

      //if(!::IsWindow(((::android::interaction_impl *)this)->get_handle()))
      //{
      //   // not a valid interaction_impl
      //   dumpcontext << " (illegal oswindow)";
      //   return; // don't do anything more
      //}

      //::window_sp pwindow = ::android::interaction_impl::from_handle(((::android::interaction_impl *)this)->get_handle());
      //if(pwindow.m_p != this)
      //   dumpcontext << " (Detached or temporary interaction_impl)";
      //else
      //   dumpcontext << " (permanent interaction_impl)";

      //// dump out interaction_impl specific statistics
      //char szBuf[64];
      ////if (!const_cast < interaction_impl * > (this)->send_message(WM_QUERYAFXWNDPROC, 0, 0) && pwindow == this)
      //// ((::android::interaction_impl *)this)->GetWindowText(szBuf, _countof(szBuf));
      ////else
      //// ::DefWindowProc(((::android::interaction_impl *)this)->get_handle(), WM_GETTEXT, _countof(szBuf), (LPARAM)&szBuf[0]);
      ////dumpcontext << "\ncaption = \"" << szBuf << "\"";

      //::GetClassName(((::android::interaction_impl *)this)->get_handle(),szBuf,_countof(szBuf));
      //dumpcontext << "\nclass name = \"" << szBuf << "\"";

      //rect rect;
      //((::android::interaction_impl *) this)->GetWindowRect(&rect);
      //dumpcontext << "\nrect = " << rect;
      //dumpcontext << "\nparent ::window_sp = " << ((::android::interaction_impl *)this)->GetParent();

      //dumpcontext << "\nstyle = " << (uint_ptr)::GetWindowLong(((::android::interaction_impl *)this)->get_handle(),GWL_STYLE);
      //if(::GetWindowLong(((::android::interaction_impl *)this)->get_handle(),GWL_STYLE) & WS_CHILD)
      //   dumpcontext << "\nid = " << __get_dialog_control_id(((::android::interaction_impl *)this)->get_handle());

      //dumpcontext << "\n";
   }

   void interaction_impl::on_set_parent(::user::interaction * pui)
   {
      if(pui != NULL)
      {
         detach();
      }
   }

   bool interaction_impl::DestroyWindow()
   {

      bool bResult = false;

      if(get_handle() != NULL)
         bResult = ::DestroyWindow(get_handle()) != FALSE;

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
      // no default processing
   }



   //int32_t interaction_impl::GetChildByIdText(int32_t nID,string & rString) const
   //{
   //   ASSERT(::IsWindow(((::android::interaction_impl *)this)->get_handle()));
   //   rString = "";    // is_empty without deallocating

   //   //oswindow oswindow = ::GetDlgItem(((::android::interaction_impl *)this)->get_handle(),nID);
   //   //if(oswindow != NULL)
   //   //{
   //   //   int32_t nLen = ::GetWindowTextLength(oswindow);
   //   //   ::GetWindowText(oswindow,rString.GetBufferSetLength(nLen),nLen + 1);
   //   //   rString.ReleaseBuffer();
   //   //}

   //   return (int32_t)rString.get_length();
   //}


   bool interaction_impl::GetWindowPlacement(WINDOWPLACEMENT* lpwndpl)
   {

      //ASSERT(::IsWindow(get_handle()));

      //lpwndpl->length = sizeof(WINDOWPLACEMENT);

      //return ::GetWindowPlacement(get_handle(),lpwndpl) != FALSE;

      return false;

   }


   bool interaction_impl::SetWindowPlacement(const WINDOWPLACEMENT* lpwndpl)
   {

      //synch_lock sl(&user_mutex());

      //ASSERT(::IsWindow(get_handle()));

      //((WINDOWPLACEMENT*)lpwndpl)->length = sizeof(WINDOWPLACEMENT);

      //return ::SetWindowPlacement(get_handle(),lpwndpl) != FALSE;

      return false;

   }


   ///////////////////////////////////////////////////////////////////////////////
   //// interaction_impl will delegate owner draw messages to self drawing controls

   //// Drawing: for all 4 control types
   //void interaction_impl::OnDrawItem(int32_t /*nIDCtl*/,LPDRAWITEMSTRUCT lpDrawItemStruct)
   //{

   //   // reflect notification to child interaction_impl control
   //   //      if (ReflectMessage(lpDrawItemStruct->hwndItem))
   //   //       return;     // eat it

   //   // not handled - do default
   //   Default();
   //}

   //// Drawing: for all 4 control types
   //int32_t interaction_impl::OnCompareItem(int32_t /*nIDCtl*/,LPCOMPAREITEMSTRUCT lpCompareItemStruct)
   //{
   //   // reflect notification to child interaction_impl control
   //   //      LRESULT lResult;
   //   //      if (ReflectLastMsg(lpCompareItemStruct->hwndItem, &lResult))
   //   //       return (int32_t)lResult;        // eat it

   //   // not handled - do default
   //   return (int32_t)Default();
   //}

   //void interaction_impl::OnDeleteItem(int32_t /*nIDCtl*/,LPDELETEITEMSTRUCT lpDeleteItemStruct)
   //{
   //   // reflect notification to child interaction_impl control
   //   //if (ReflectLastMsg(lpDeleteItemStruct->hwndItem))
   //   // return;     // eat it
   //   // not handled - do default
   //   Default();
   //}

   bool interaction_impl::_EnableToolTips(bool bEnable,UINT nFlag)
   {
      UNREFERENCED_PARAMETER(bEnable);
      UNREFERENCED_PARAMETER(nFlag);
      return FALSE;
   }


   //// Measure item implementation relies on unique control/menu IDs
   //void interaction_impl::OnMeasureItem(int32_t /*nIDCtl*/,LPMEASUREITEMSTRUCT lpMeasureItemStruct)
   //{
   //   if(lpMeasureItemStruct->CtlType == ODT_MENU)
   //   {
   //      ASSERT(lpMeasureItemStruct->CtlID == 0);
   //   }
   //   else
   //   {
   //      sp(::user::interaction) pChild = GetDescendantWindow(lpMeasureItemStruct->CtlID);
   //      //         if (pChild != NULL && pChild->OnChildNotify(0, 0, 0, NULL))
   //      //          return;     // eaten by child
   //   }
   //   // not handled - do default
   //   Default();
   //}

   //bool interaction_impl::GetWindowInfo(PWINDOWINFO pwi) const
   //{
   //   ASSERT(::IsWindow(((interaction_impl *) this)->get_handle()));
   //   return ::GetWindowInfo(((interaction_impl *) this)->get_handle(),pwi) != FALSE;
   //}

   //::window_sp interaction_impl::GetAncestor(UINT gaFlags) const
   //{
   //   ASSERT(::IsWindow(((interaction_impl *) this)->get_handle())); return  ::android::interaction_impl::from_handle(::GetAncestor(((interaction_impl *) this)->get_handle(),gaFlags));
   //}



   //bool interaction_impl::GetScrollBarInfo(LONG idObject,PSCROLLBARINFO psbi) const
   //{
   //   ASSERT(::IsWindow(((interaction_impl *) this)->get_handle()));
   //   ASSERT(psbi != NULL);
   //   return ::GetScrollBarInfo(((interaction_impl *) this)->get_handle(),idObject,psbi) != FALSE;
   //}

   //bool interaction_impl::GetTitleBarInfo(PTITLEBARINFO pti) const
   //{
   //   ASSERT(::IsWindow(((interaction_impl *) this)->get_handle()));
   //   ASSERT(pti != NULL);
   //   return ::GetTitleBarInfo(((interaction_impl *) this)->get_handle(),pti) != FALSE;
   //}

   //bool interaction_impl::AnimateWindow(uint32_t dwTime,uint32_t dwFlags)
   //{
   //   ASSERT(::IsWindow(get_handle()));
   //   return ::AnimateWindow(get_handle(),dwTime,dwFlags) != FALSE;
   //}

   //bool interaction_impl::FlashWindowEx(uint32_t dwFlags,UINT  uCount,uint32_t dwTimeout)
   //{
   //   ASSERT(::IsWindow(get_handle()));
   //   FLASHWINFO fwi;
   //   fwi.cbSize = sizeof(fwi);
   //   fwi.hwnd = get_handle();
   //   fwi.dwFlags = dwFlags;
   //   fwi.uCount = uCount;
   //   fwi.dwTimeout = dwTimeout;

   //   return ::FlashWindowEx(&fwi) != FALSE;
   //}



   //bool interaction_impl::SetLayeredWindowAttributes(COLORREF crKey,BYTE bAlpha,uint32_t dwFlags)
   //{
   //   ASSERT(::IsWindow(get_handle()));
   //   return ::SetLayeredWindowAttributes(get_handle(),crKey,bAlpha,dwFlags) != FALSE;
   //}

   //bool interaction_impl::UpdateLayeredWindow(::draw2d::graphics * pDCDst,POINT *pptDst,SIZE *psize,
   //   ::draw2d::graphics * pDCSrc,POINT *pptSrc,COLORREF crKey,BLENDFUNCTION *pblend,uint32_t dwFlags)
   //{
   //   ASSERT(::IsWindow(get_handle()));
   //   ::exception::throw_not_implemented(get_app());
   //   return false;
   //   /*      return ::UpdateLayeredWindow(get_handle(), WIN_HDC(pDCDst), pptDst, psize,
   //   WIN_HDC(pDCSrc), pptSrc, crKey, pblend, dwFlags) != FALSE;*/
   //}


   //bool interaction_impl::GetLayeredWindowAttributes(COLORREF *pcrKey,BYTE *pbAlpha,uint32_t *pdwFlags) const
   //{
   //   ASSERT(::IsWindow(((interaction_impl *) this)->get_handle()));
   //   return ::GetLayeredWindowAttributes(((interaction_impl *) this)->get_handle(),pcrKey,pbAlpha,(LPDWORD)pdwFlags) != FALSE;
   //}

   //bool interaction_impl::PrintWindow(::draw2d::graphics * pgraphics,UINT nFlags) const
   //{
   //   ASSERT(::IsWindow(((interaction_impl *) this)->get_handle()));
   //   ::exception::throw_not_implemented(get_app());
   //   //      return ::PrintWindow(get_handle(), (HDC)(dynamic_cast<::android::graphics * >(pgraphics))->get_handle(), nFlags) != FALSE;
   //   return false;
   //}


   LRESULT interaction_impl::OnNTCtlColor(WPARAM wParam,LPARAM lParam)
   {
      // fill in special struct for compatiblity with 16-bit WM_CTLCOLOR
      //__CTLCOLOR ctl;
      //ctl.hDC = (HDC)wParam;
      //ctl.oswindow = (oswindow)lParam;
      return 0;
   }

   /////////////////////////////////////////////////////////////////////////////
   // interaction_impl extensions for help support

   void interaction_impl::WinHelp(uint_ptr dwData,UINT nCmd)
   {
      UNREFERENCED_PARAMETER(dwData);
      UNREFERENCED_PARAMETER(nCmd);
      ::exception::throw_not_implemented(get_app());

      /*      application* pApp = &System;
      ASSERT_VALID(pApp);
      ASSERT(pApp->m_pszHelpFilePath != NULL);
      ASSERT(pApp->m_eHelpType == afxWinHelp);

      wait_cursor wait(get_app());

      PrepareForHelp();

      // need to use top level parent (for the case where get_handle() is in DLL)
      sp(::user::interaction) pwindow = EnsureTopLevelParent();

      TRACE(::aura::trace::category_AppMsg, 0, "WinHelp: pszHelpFile = '%s', dwData: $%lx, fuCommand: %d.\n", pApp->m_pszHelpFilePath, dwData, nCmd);

      // finally, run the Windows Help engine
      /* trans   if (!::WinHelp(NODE_WINDOW(pwindow)->get_handle(), pApp->m_pszHelpFilePath, nCmd, dwData))
      {
      // android System.simple_message_box(__IDP_FAILED_TO_LAUNCH_HELP);
      System.simple_message_box("Failed to launch help");
      }*/
   }

   //void interaction_impl::HtmlHelp(uint_ptr dwData, UINT nCmd)
   //{
   // ::exception::throw_not_implemented(get_app());
   /*
   application* pApp = &System;
   ASSERT_VALID(pApp);
   ASSERT(pApp->m_pszHelpFilePath != NULL);
   // to call HtmlHelp the m_fUseHtmlHelp must be set in
   // the application's constructor
   ASSERT(pApp->m_eHelpType == afxHTMLHelp);

   wait_cursor wait(get_app());

   PrepareForHelp();

   // need to use top level parent (for the case where get_handle() is in DLL)
   sp(::user::interaction) pwindow = EnsureTopLevelParent();

   TRACE(::aura::trace::category_AppMsg, 0, "HtmlHelp: pszHelpFile = '%s', dwData: $%lx, fuCommand: %d.\n", pApp->m_pszHelpFilePath, dwData, nCmd);

   // run the HTML Help engine
   /* trans   if (!::core::HtmlHelp(NODE_WINDOW(pwindow)->get_handle(), pApp->m_pszHelpFilePath, nCmd, dwData))
   {
   // android System.simple_message_box(__IDP_FAILED_TO_LAUNCH_HELP);
   System.simple_message_box("Failed to launch help");
   }*/
   //}

   void interaction_impl::PrepareForHelp()
   {

      sp(::user::frame_window) pFrameWnd = m_pui;

      if(pFrameWnd.is_set())
      {

         // frame_window android should be allowed to exit help mode first

         pFrameWnd->ExitHelpMode();

      }

      // Cancel any tracking modes
      send_message(WM_CANCELMODE);
      SendMessageToDescendants(WM_CANCELMODE,0,0,TRUE,TRUE);

      // need to use top level parent (for the case where get_handle() is in DLL)
      sp(::user::interaction) pwindow = EnsureTopLevel();
      pwindow->send_message(WM_CANCELMODE);
      pwindow->SendMessageToDescendants(WM_CANCELMODE,0,0,TRUE,TRUE);

      // attempt to cancel capture
      oswindow oswindow_Capture = ::GetCapture();
      if(oswindow_Capture != NULL)
         oswindow_Capture->m_pimpl->m_pui->send_message(WM_CANCELMODE);
   }


   void interaction_impl::WinHelpInternal(uint_ptr dwData,UINT nCmd)
   {
      UNREFERENCED_PARAMETER(dwData);
      UNREFERENCED_PARAMETER(nCmd);
      ::exception::throw_not_implemented(get_app());
      /*
      application* pApp = &System;
      ASSERT_VALID(pApp);
      if (pApp->m_eHelpType == afxHTMLHelp)
      {
      // translate from WinHelp commands and data to to HtmlHelp
      ASSERT((nCmd == HELP_CONTEXT) || (nCmd == HELP_CONTENTS) || (nCmd == HELP_FINDER));
      if (nCmd == HELP_CONTEXT)
      nCmd = HH_HELP_CONTEXT;
      else if (nCmd == HELP_CONTENTS)
      nCmd = HH_DISPLAY_TOC;
      else if (nCmd == HELP_FINDER)
      nCmd = HH_HELP_FINDER;
      HtmlHelp(dwData, nCmd);
      }
      else
      WinHelp(dwData, nCmd);*/
   }



   bool interaction_impl::_001OnCmdMsg(::aura::cmd_msg * pcmdmsg)
   {
      if(command_target_interface::_001OnCmdMsg(pcmdmsg))
         return TRUE;

      //      bool b;

      //if(_iguimessageDispatchCommandMessage(pcommand, b))
      // return b;

      command_target * pcmdtarget = dynamic_cast <command_target *> (this);
      return pcmdtarget->command_target::_001OnCmdMsg(pcmdmsg);
   }


   void interaction_impl::_002OnDraw(::draw2d::dib * pdib)
   {

      ::exception::throw_not_implemented(get_app());
      //::CallWindowProc(*GetSuperWndProcAddr(), get_handle(), WM_PRINT, (WPARAM)((dynamic_cast<::android::graphics * >(pgraphics))->get_handle()), (LPARAM)(PRF_CHILDREN | PRF_CLIENT));

   }

   /////////////////////////////////////////////////////////////////////////////
   // main message_handler implementation

   void interaction_impl::message_handler(signal_details * pobj)
   {

      SCAST_PTR(::message::base,pbase,pobj);

      //if(pbase->m_uiMessage == WM_MOUSEMOVE)
      //{
      //
      //   pbase->m_bRet = true;
      //   //pbase->set_lresult(1);
      //   // addictive human (camilo, you are classifying yourself human, you're kind respecting yourself, its a good sign...) profiling with PROFILE_MOVE_MANAGER at app_core_miau.vcxproj...

      //   return;


      //}



      if(pbase->m_uiMessage == WM_SIZE || pbase->m_uiMessage == WM_MOVE)
      {

         //win_update_graphics();

      }

      if(pbase->m_uiMessage == WM_KEYDOWN ||
         pbase->m_uiMessage == WM_KEYUP ||
         pbase->m_uiMessage == WM_CHAR ||
         pbase->m_uiMessage == WM_SYSKEYDOWN ||
         pbase->m_uiMessage == WM_SYSKEYUP ||
         pbase->m_uiMessage == WM_SYSCHAR)
      {

         SCAST_PTR(::message::key,pkey,pobj);



         if(pbase->m_uiMessage == WM_KEYDOWN || pbase->m_uiMessage == WM_SYSKEYDOWN)
         {
            try
            {
               Session.set_key_pressed(pkey->m_ekey,true);
            }
            catch(...)
            {
            }
         }
         else if(pbase->m_uiMessage == WM_KEYUP || pbase->m_uiMessage == WM_SYSKEYUP)
         {
            try
            {
               Session.set_key_pressed(pkey->m_ekey,false);
            }
            catch(...)
            {
            }
         }
      }



      if(m_pui != NULL)
      {
         if(m_pui->WfiIsMoving())
         {
            //TRACE("moving: skip pre translate message");
         }
         else if(m_pui->WfiIsSizing())
         {
            //TRACE("sizing: skip pre translate message");
         }
         else
         {
            m_pui->pre_translate_message(pobj);
            if(pobj->m_bRet)
               return;

         }
      }

      if(pbase->m_uiMessage == WM_TIMER)
      {
//         m_pui->m_pauraapp->step_timer();
      }
      else if(pbase->m_uiMessage == WM_LBUTTONDOWN)
      {
         //::rect rectClient;
         //::GetClientRect(get_handle(),rectClient);
         //::rect rectWindow;
         //::GetWindowRect(get_handle(),rectWindow);
         //::rect rectRegion;
         //HRGN hrgn = CreateRectRgn(0,0,0,0);
         //int regionType = ::GetWindowRgn(get_handle(),hrgn);
         //if(regionType != ERROR)
         //{
         //   ::GetRgnBox(hrgn,rectRegion);
         //}
         //::DeleteObject(hrgn); /* finished with region */
         //WINDOWPLACEMENT wp;
         //ZERO(wp);
         //wp.length = sizeof(WINDOWPLACEMENT);
         //::GetWindowPlacement(get_handle(),&wp);
         //bool bZoomed = ::IsZoomed(get_handle()) != FALSE;
         //bool bIconic = ::IsIconic(get_handle()) != FALSE;
         Session.m_puiLastLButtonDown = m_pui;
      }
      /*      else if(pbase->m_uiMessage == CA2M_BERGEDGE)
      {
      if(pbase->m_wparam == BERGEDGE_GETAPP)
      {
      sp(::aura::application)* ppapp= (sp(::aura::application)*) pbase->m_lparam;
      *ppapp = get_app();
      pbase->m_bRet = true;
      return;
      }
      }*/
      pbase->set_lresult(0);

      if(pbase->m_uiMessage == WM_MOUSELEAVE)
      {
         
         _000OnMouseLeave(pbase);

         return;

      }

      if(pbase->m_uiMessage == WM_LBUTTONDOWN ||
         pbase->m_uiMessage == WM_LBUTTONUP ||
         pbase->m_uiMessage == WM_MBUTTONDOWN ||
         pbase->m_uiMessage == WM_MBUTTONUP ||
         pbase->m_uiMessage == WM_RBUTTONDOWN ||
         pbase->m_uiMessage == WM_RBUTTONUP ||
         pbase->m_uiMessage == WM_LBUTTONDBLCLK ||
         pbase->m_uiMessage == WM_MOUSEMOVE ||
         pbase->m_uiMessage == WM_NCMOUSEMOVE ||
         pbase->m_uiMessage == WM_MOUSEWHEEL)
      {

         message::mouse * pmouse = (::message::mouse *) pbase;

         //if(::GetCapture() == m_oswindow)
         //{

         //   pmouse->m_ptDesired = pmouse->m_pt;
         //
         //   pmouse->m_pt = Session.m_ptCursor;

         //   SetCursorPos(Session.m_ptCursor.x,Session.m_ptCursor.y);

         //}

         Session.on_ui_mouse_message(pmouse);

         if(m_bTranslateMouseMessageCursor && !pmouse->m_bTranslated)
         {
            pmouse->m_bTranslated = true;
            rect64 rectWindow;
            if(m_bScreenRelativeMouseMessagePosition)
            {
               rectWindow.null();
               //class rect rectWindow32;
               //::GetWindowRect(get_handle(), rectWindow32);
               //::copy(rectWindow,rectWindow32);
            }
            else
            {
               m_pui->GetWindowRect(rectWindow);
            }
            /*if(System.get_monitor_count() > 0)
            {
            rect rcMonitor;
            System.get_monitor_rect(0,&rcMonitor);
            if(rectWindow.left >= rcMonitor.left)
            pmouse->m_pt.x += (LONG)rectWindow.left;
            if(rectWindow.top >= rcMonitor.top)
            pmouse->m_pt.y += (LONG)rectWindow.top;
            }
            else*/
            //if(rectWindow.left >= 0)
            pmouse->m_pt.x += (LONG)rectWindow.left;
            //if(rectWindow.top >= 0)
            pmouse->m_pt.y += (LONG)rectWindow.top;
         }

         if(pbase->m_uiMessage == WM_MOUSEMOVE)
         {
            // We are at the message_handler procedure.
            // mouse messages originated from message_handler and that are mouse move events should end up with the correct cursor.
            // So the procedure starts by setting to the default cursor,
            // what forces, at the end of message processing, setting the bergedge cursor to the default cursor, if no other
            // handler has set it to another one.
            pmouse->m_ecursor = visual::cursor_default;
         }
         else if(pbase->m_uiMessage == WM_NCMOUSEMOVE)
         {
            // We are at the message_handler procedure.
            // mouse messages originated from message_handler and that are mouse move events should end up with the correct cursor.
            // So the procedure starts by setting to the default cursor,
            // what forces, at the end of message processing, setting the bergedge cursor to the default cursor, if no other
            // handler has set it to another one.
            pmouse->m_ecursor = visual::cursor_default;
         }

         _008OnMouse(pmouse);

         return;

      }

      //if(pbase->m_uiMessage == MESSAGE_OLE_DRAGENTER ||
      //   pbase->m_uiMessage == MESSAGE_OLE_DRAGOVER ||
      //   pbase->m_uiMessage == MESSAGE_OLE_DRAGLEAVE ||
      //   pbase->m_uiMessage == MESSAGE_OLE_DRAGDROP)
      //{

      //   message::drag_and_drop * pdrag = (::message::drag_and_drop *) pbase;


      //   user::oswindow_array oswindowa;
      //   user::interaction_ptra wnda;
      //   wnda = System.m_uiptraFrame;
      //   oswindowa = wnda.get_hwnda();
      //   user::window_util::SortByZOrder(oswindowa);
      //   for(int32_t i = 0; i < oswindowa.get_size(); i++)
      //   {
      //      sp(::user::interaction) pui = wnda.find_first(oswindowa[i]);
      //      if(pui != NULL)
      //      {
      //         pui->_000OnDrag(pdrag);
      //         if(pdrag->m_bRet)
      //            return;
      //      }
      //   }
      //   return;
      //}
      if(pbase->m_uiMessage == WM_KEYDOWN ||
         pbase->m_uiMessage == WM_KEYUP ||
         pbase->m_uiMessage == WM_CHAR ||
         pbase->m_uiMessage == WM_SYSKEYDOWN ||
         pbase->m_uiMessage == WM_SYSKEYUP ||
         pbase->m_uiMessage == WM_SYSCHAR)
      {

         message::key * pkey = (::message::key *) pbase;

         sp(::user::interaction) puiFocus = Session.get_keyboard_focus();

         if(puiFocus != NULL && puiFocus->IsWindow() && puiFocus != m_pui)
         {

            puiFocus->send(pkey);

            if(pbase->m_bRet)
               return;

         }
         else if(!pkey->m_bRet)
         {

            if(m_pui != NULL)
            {

               m_pui->_000OnKey(pkey);

               if(pbase->m_bRet)
                  return;

            }

         }

         pbase->set_lresult(DefWindowProc(pbase->m_uiMessage,pbase->m_wparam,pbase->m_lparam));

         return;

      }
      if(pbase->m_uiMessage == ::message::message_event)
      {
         if(m_pui != NULL)
         {
            ((::user::control_event *) pbase->m_lparam.m_lparam)->m_bRet = m_pui->BaseOnControlEvent((::user::control_event *) pbase->m_lparam.m_lparam);
         }
         return;
      }
      ::user::interaction_impl::message_handler(pobj);
      //if(pobj->m_bRet && !pbase->m_bDoSystemDefault)
      if(pobj->m_bRet)
         return;
      if(m_pui != NULL)
      {
         pbase->set_lresult(m_pui->DefWindowProc(pbase->m_uiMessage,pbase->m_wparam,pbase->m_lparam));
      }
      else
      {
         pbase->set_lresult(DefWindowProc(pbase->m_uiMessage,pbase->m_wparam,pbase->m_lparam));
      }
   }


   bool interaction_impl::OnCommand(::message::base * pbase)
   {
      UNREFERENCED_PARAMETER(pbase);
      return FALSE;
   }


   bool interaction_impl::OnNotify(::message::base * pbase)
   {

      ASSERT(pbase != NULL);
      NMHDR* pNMHDR = (NMHDR*)pbase->m_lparam;
      oswindow oswindow_Ctrl = pNMHDR->hwndFrom;

      // get the child ID from the interaction_impl itself
      //      uint_ptr nID = __get_dialog_control_id(oswindow_Ctrl);
      //      int32_t nCode = pNMHDR->code;

      ASSERT(oswindow_Ctrl != NULL);
      ASSERT(::IsWindow(oswindow_Ctrl));

      //      if (gen_ThreadState->m_hLockoutNotifyWindow == get_handle())
      //       return true;        // locked out - ignore control notification

      // reflect notification to child interaction_impl control
      //if(ReflectMessage(oswindow_Ctrl,pbase))
        // return true;        // eaten by child

      //      __NOTIFY notify;
      //    notify.pResult = pResult;
      //  notify.pNMHDR = pNMHDR;
      //xxx   return _001OnCommand(pbase);
      return false;
   }




   bool interaction_impl::IsTopParentActive()
   {
      ASSERT(get_handle() != NULL);
      ASSERT_VALID(this);

      sp(::user::interaction)pWndTopLevel=EnsureTopLevel();

      return interaction_impl::GetForegroundWindow() == pWndTopLevel->GetLastActivePopup();
   }

   void interaction_impl::ActivateTopParent()
   {
      // special activate logic for floating toolbars and palettes
      //::window_sp pActiveWnd = GetForegroundWindow();
      //if(pActiveWnd == NULL || !(NODE_WINDOW(pActiveWnd)->get_handle() == get_handle() || ::IsChild(NODE_WINDOW(pActiveWnd)->get_handle(),get_handle())))
      {
         // clicking on floating frame when it does not have
         // focus itself -- activate the toplevel frame instead.
         EnsureTopLevel()->SetForegroundWindow();
      }
   }



   //::user::interaction * interaction_impl::get_safe_owner(::user::interaction * pParent,oswindow* pWndTop)
   //{
   //   oswindow oswindow = get_safe_owner(pParent->get_handle(),pWndTop);
   //   return ::android::interaction_impl::from_handle(oswindow);
   //}

   int32_t interaction_impl::message_box(const char * lpszText,const char * lpszCaption,UINT nType)
   {
      if(lpszCaption == NULL)
         lpszCaption = Application.m_strAppName;
      int32_t nResult = ::simple_message_box(get_handle(),lpszText,lpszCaption,nType);
      return nResult;
   }

   ::user::interaction * interaction_impl::GetDescendantWindow(::user::interaction * pui,id id)
   {
      single_lock sl(pui->m_pauraapp->m_pmutex,TRUE);
      // get_child_by_id recursive (return first found)
      // breadth-first for 1 level, then depth-first for next level

      // use get_child_by_id since it is a fast USER function
      sp(::user::interaction) pWndChild;
      if((pWndChild = pui->get_child_by_id(id)) != NULL)
      {
         if(pWndChild->GetTopWindow() != NULL)
         {
            // children with the same ID as their parent have priority
            pWndChild = GetDescendantWindow(pWndChild,id);
            if(pWndChild != NULL)
               return pWndChild;
         }
         if(pWndChild != NULL)
            return pWndChild;
      }

      for(int32_t i = 0; i < pui->m_uiptraChild.get_count(); i++)
      {
         if(pui->m_uiptraChild[i]->GetDlgCtrlId() == id)
         {
            if(pui->m_uiptraChild[i]->GetDescendantWindow(id))
               return pui->m_uiptraChild[i]->GetDescendantWindow(id);
            else
               return pui->m_uiptraChild[i];
         }
      }

      if(pWndChild == NULL)
         return NULL;

      // walk each child
      for(pWndChild = pWndChild->GetTopWindow(); pWndChild != NULL;
         pWndChild = pWndChild->get_next_window(GW_HWNDNEXT))
      {
         pWndChild = GetDescendantWindow(pWndChild,id);
         if(pWndChild != NULL)
            return pWndChild;
      }
      return NULL;    // not found
   }

   //void interaction_impl::SendMessageToDescendants(oswindow oswindow,UINT message,WPARAM wParam,lparam lParam,bool bDeep,bool bOnlyPerm)
   //{
   //   // walk through HWNDs to avoid creating temporary interaction_impl objects
   //   // unless we need to call this function recursively
   //   //for(::oswindow oswindow_Child = ::GetTopWindow(oswindow); oswindow_Child != NULL; oswindow_Child = ::GetNextWindow(oswindow_Child,GW_HWNDNEXT))
   //   //{
   //   //   // if bOnlyPerm is TRUE, don't send to non-permanent android
   //   //   if(bOnlyPerm)
   //   //   {
   //   //      ::user::interaction * pwindow = System.ui_from_handle(oswindow_Child);
   //   //      if(pwindow != NULL)
   //   //      {
   //   //         // call interaction_impl proc directly since it is a C++ interaction_impl
   //   //         pwindow->call_message_handler(message,wParam,lParam);
   //   //      }
   //   //   }
   //   //   else
   //   //   {
   //   //      try
   //   //      {
   //   //         // send message with Windows SendMessage API
   //   //         ::SendMessage(oswindow_Child,message,wParam,lParam);
   //   //      }
   //   //      catch(...)
   //   //      {
   //   //      }
   //   //   }
   //   //   if(bDeep && ::GetTopWindow(oswindow_Child) != NULL)
   //   //   {
   //   //      // send to child android after parent
   //   //      try
   //   //      {
   //   //         SendMessageToDescendants(oswindow_Child,message,wParam,lParam,
   //   //            bDeep,bOnlyPerm);
   //   //      }
   //   //      catch(...)
   //   //      {
   //   //      }
   //   //   }
   //   //}

   //}

   /////////////////////////////////////////////////////////////////////////////
   // Scroll bar helpers
   //  hook for interaction_impl functions
   //    only works for derived class (eg: ::user::impact) that override 'GetScrollBarCtrl'
   // if the interaction_impl doesn't have a _visible_ android scrollbar - then
   //   look for a sibling with the appropriate ID

   /*   CScrollBar* interaction_impl::GetScrollBarCtrl(int32_t) const
   {
   return NULL;        // no special scrollers supported
   }*/

   int32_t interaction_impl::SetScrollPos(int32_t nBar,int32_t nPos,bool bRedraw)
   {
      //return ::SetScrollPos(get_handle(),nBar,nPos,bRedraw);
      return -1;
   }

   int32_t interaction_impl::GetScrollPos(int32_t nBar) const
   {
      //return ::GetScrollPos(((::android::interaction_impl *)this)->get_handle(),nBar);
      return -1;
   }

   void interaction_impl::SetScrollRange(int32_t nBar,int32_t nMinPos,int32_t nMaxPos,bool bRedraw)
   {
      //::SetScrollRange(get_handle(),nBar,nMinPos,nMaxPos,bRedraw);

   }

   void interaction_impl::GetScrollRange(int32_t nBar,LPINT lpMinPos,LPINT lpMaxPos) const
   {
      //::GetScrollRange(((::android::interaction_impl *)this)->get_handle(),nBar,lpMinPos,lpMaxPos);
   }

   // Turn on/off non-control scrollbars
   //   for WS_?SCROLL scrollbars - show/hide them
   //   for control scrollbar - enable/disable them
   void interaction_impl::EnableScrollBarCtrl(int32_t nBar,bool bEnable)
   {
      // WS_?SCROLL scrollbar - show or hide
      ShowScrollBar(nBar,bEnable);
   }

   //bool interaction_impl::SetScrollInfo(int32_t nBar,LPSCROLLINFO lpScrollInfo,bool bRedraw)
   //{
   //   ASSERT(lpScrollInfo != NULL);

   //   oswindow oswindow = get_handle();
   //   lpScrollInfo->cbSize = sizeof(*lpScrollInfo);
   //   ::SetScrollInfo(oswindow,nBar,lpScrollInfo,bRedraw);
   //   return true;
   //}

   //bool interaction_impl::GetScrollInfo(int32_t nBar,LPSCROLLINFO lpScrollInfo,UINT nMask)
   //{
   //   UNREFERENCED_PARAMETER(nMask);
   //   ASSERT(lpScrollInfo != NULL);

   //   oswindow oswindow = get_handle();
   //   return ::GetScrollInfo(oswindow,nBar,lpScrollInfo) != FALSE;
   //}

   int32_t interaction_impl::GetScrollLimit(int32_t nBar)
   {
      //int32_t nMin,nMax;
      //GetScrollRange(nBar,&nMin,&nMax);
      //SCROLLINFO info;
      //if(GetScrollInfo(nBar,&info,SIF_PAGE))
      //{
      //   nMax -= __max(info.nPage - 1,0);
      //}
      //return nMax;
      return -1;
   }

   void interaction_impl::ScrollWindow(int32_t xAmount,int32_t yAmount,
      LPCRECT lpRect,LPCRECT lpClipRect)
   {
      //ASSERT(::IsWindow(get_handle()));

      //if(IsWindowVisible() || lpRect != NULL || lpClipRect != NULL)
      //{
      //   // When visible, let Windows do the scrolling
      //   ::ScrollWindow(get_handle(),xAmount,yAmount,lpRect,lpClipRect);
      //}
      //else
      //{
      //   // Windows does not perform any scrolling if the interaction_impl is
      //   // not visible.  This leaves child android unscrolled.
      //   // To ac::count for this oversight, the child android are moved
      //   // directly instead.
      //   oswindow oswindow_Child = ::GetWindow(get_handle(),GW_CHILD);
      //   if(oswindow_Child != NULL)
      //   {
      //      for(; oswindow_Child != NULL;
      //         oswindow_Child = ::GetNextWindow(oswindow_Child,GW_HWNDNEXT))
      //      {
      //         rect rect;
      //         ::GetWindowRect(oswindow_Child,&rect);
      //         ScreenToClient(&rect);
      //         ::SetWindowPos(oswindow_Child,NULL,rect.left + xAmount,rect.top + yAmount,0,0,SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER);
      //      }
      //   }
      //}

   }

   /////////////////////////////////////////////////////////////////////////////
   // minimal on_layout support

   /*
   void interaction_impl::RepositionBars(const char * pszPrefix, const char * pszIdLeftOver,
   UINT nFlags, LPRECT lpRectParam, LPCRECT lpRectClient, bool bStretch)
   {
   ASSERT(nFlags == 0 || (nFlags & ~reposNoPosLeftOver) == reposQuery ||
   (nFlags & ~reposNoPosLeftOver) == reposExtra);

   // walk kids in order, control bars get the resize notification
   //   which allow them to shrink the client area
   // remaining size goes to the 'nIDLeftOver' pane
   // NOTE: nIDFirst->nIDLast are usually 0->0xffff

   __SIZEPARENTPARAMS on_layout;
   sp(::user::interaction) oswindow_LeftOver = NULL;

   on_layout.bStretch = bStretch;
   on_layout.sizeTotal.cx = on_layout.sizeTotal.cy = 0;
   if (lpRectClient != NULL)
   on_layout.rect = *lpRectClient;    // starting rect comes from parameter
   else
   {
   if(m_pui != this)
   m_pui->GetClientRect(&on_layout.rect);    // starting rect comes from client rect
   else
   GetClientRect(&on_layout.rect);    // starting rect comes from client rect
   }

   if ((nFlags & ~reposNoPosLeftOver) != reposQuery)
   on_layout.hDWP = ::BeginDeferWindowPos(8); // reasonable guess
   else
   on_layout.hDWP = NULL; // not actually doing on_layout

   if(m_pui != this && m_pui != NULL)
   {
   for (sp(::user::interaction) oswindow_Child = m_pui->GetTopWindow(); oswindow_Child != NULL;
   oswindow_Child = oswindow_Child->GetNextWindow(GW_HWNDNEXT))
   {
   string strIdc = oswindow_Child->GetDlgCtrlId();
   sp(::user::interaction) pwindow = oswindow_Child;
   if (strIdc == pszIdLeftOver)
   oswindow_LeftOver = oswindow_Child;
   else if (::str::begins(strIdc, pszPrefix) && pwindow != NULL)
   oswindow_Child->SendMessage(WM_SIZEPARENT, 0, (LPARAM)&on_layout);
   }
   for (int32_t i = 0; i < m_pui->m_uiptra.get_count();   i++)
   {
   sp(::user::interaction) oswindow_Child = m_pui->m_uiptra[i];
   string strIdc = oswindow_Child->GetDlgCtrlId();
   sp(::user::interaction) pwindow = oswindow_Child;
   if (strIdc == pszIdLeftOver)
   oswindow_LeftOver = oswindow_Child;
   else if (::str::begins(strIdc, pszPrefix) && pwindow != NULL)
   oswindow_Child->SendMessage(WM_SIZEPARENT, 0, (LPARAM)&on_layout);
   }
   }
   else
   {
   for (sp(::user::interaction) oswindow_Child = GetTopWindow(); oswindow_Child != NULL;
   oswindow_Child = oswindow_Child->GetNextWindow(GW_HWNDNEXT))
   {
   string strIdc = oswindow_Child->GetDlgCtrlId();
   sp(::user::interaction) pwindow = oswindow_Child;
   if (strIdc == pszIdLeftOver)
   oswindow_LeftOver = oswindow_Child;
   else if (::str::begins(strIdc, pszPrefix) && pwindow != NULL)
   oswindow_Child->SendMessage(WM_SIZEPARENT, 0, (LPARAM)&on_layout);
   }
   for (int32_t i = 0; i < m_uiptra.get_count();   i++)
   {
   sp(::user::interaction) oswindow_Child = m_uiptra[i];
   string strIdc = oswindow_Child->GetDlgCtrlId();
   sp(::user::interaction) pwindow = oswindow_Child;
   if (strIdc == pszIdLeftOver)
   oswindow_LeftOver = oswindow_Child;
   else if (::str::begins(strIdc, pszPrefix) && pwindow != NULL)
   oswindow_Child->SendMessage(WM_SIZEPARENT, 0, (LPARAM)&on_layout);
   }
   }

   // if just getting the available rectangle, return it now...
   if ((nFlags & ~reposNoPosLeftOver) == reposQuery)
   {
   ASSERT(lpRectParam != NULL);
   if (bStretch)
   ::copy(lpRectParam, &on_layout.rect);
   else
   {
   lpRectParam->left = lpRectParam->top = 0;
   lpRectParam->right = on_layout.sizeTotal.cx;
   lpRectParam->bottom = on_layout.sizeTotal.cy;
   }
   return;
   }

   // the rest is the client size of the left-over pane
   if (pszIdLeftOver != NULL && oswindow_LeftOver != NULL)
   {
   sp(::user::interaction) pLeftOver = oswindow_LeftOver;
   // allow extra space as specified by lpRectBorder
   if ((nFlags & ~reposNoPosLeftOver) == reposExtra)
   {
   ASSERT(lpRectParam != NULL);
   on_layout.rect.left += lpRectParam->left;
   on_layout.rect.top += lpRectParam->top;
   on_layout.rect.right -= lpRectParam->right;
   on_layout.rect.bottom -= lpRectParam->bottom;
   }
   // reposition the interaction_impl
   if ((nFlags & reposNoPosLeftOver) != reposNoPosLeftOver)
   {
   pLeftOver->CalcWindowRect(&on_layout.rect);
   __reposition_window(&on_layout, pLeftOver, &on_layout.rect);
   }
   }

   // move and resize all the android at once!
   if (on_layout.hDWP == NULL || !::EndDeferWindowPos(on_layout.hDWP))
   TRACE(::aura::trace::category_AppMsg, 0, "Warning: DeferWindowPos failed - low system resources.\n");
   }

   */




void interaction_impl::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType)
{
   /*DWORD dwExStyle = GetExStyle();
   if (nAdjustType == 0)
   dwExStyle &= ~WS_EX_CLIENTEDGE;
   ::AdjustWindowRectEx(lpClientRect, GetStyle(), FALSE, dwExStyle);*/
}

/////////////////////////////////////////////////////////////////////////////
// Special keyboard/system command processing

bool interaction_impl::HandleFloatingSysCommand(UINT nID, LPARAM lparam)
{

   return false;
   /*      sp(::user::interaction) pParent = GetTopLevelParent();
   switch (nID & 0xfff0)
   {
   case SC_PREVWINDOW:
   case SC_NEXTWINDOW:
   if (LOWORD(lparam) == VK_F6 && pParent != NULL)
   {
   pParent->SetFocus();
   return true;
   }
   break;

   case SC_CLOSE:
   case SC_KEYMENU:
   // Check lparam.  If it is 0L, then the ::fontopus::user may have done
   // an Alt+Tab, so just ignore it.  This breaks the ability to
   // just press the Alt-key and have the first menu selected,
   // but this is minor compared to what happens in the Alt+Tab
   // case.
   if ((nID & 0xfff0) == SC_CLOSE || lparam != 0L)
   {
   if (pParent != NULL)
   {
   // Sending the above WM_SYSCOMMAND may destroy the cast,
   // so we have to be careful about restoring activation
   // and focus after sending it.
   oswindow hWndSave = get_handle();
   oswindow hWndFocus = ::GetFocus();
   pParent->SetActiveWindow();
   pParent->send_message(WM_SYSCOMMAND, nID, lparam);

   // be very careful here...
   if (::IsWindow(hWndSave))
   ::SetActiveWindow(hWndSave);
   if (::IsWindow(hWndFocus))
   ::SetFocus(hWndFocus);
   }
   }
   return true;
   }
   return false;*/
}

void interaction_impl::WalkPreTranslateTree(::user::interaction * puiStop, ::signal_details * pobj)
{
   ASSERT(puiStop == NULL || puiStop->IsWindow());
   ASSERT(pobj != NULL);

   SCAST_PTR(::message::base, pbase, pobj);
   // walk from the target interaction_impl up to the hWndStop interaction_impl checking
   //  if any interaction_impl wants to translate this message

   for (sp(::user::interaction) pui = pbase->m_pwnd; pui != NULL; pui->GetParent())
   {

      pui->pre_translate_message(pobj);

      if (pobj->m_bRet)
         return; // trapped by target interaction_impl (eg: accelerators)

                 // got to hWndStop interaction_impl without interest
      if (pui == puiStop)
         break;

   }
   // no special processing
}


bool interaction_impl::SendChildNotifyLastMsg(LRESULT* pResult)
{

   return false;

}


//bool PASCAL interaction_impl::ReflectLastMsg(oswindow hWndChild, LRESULT* pResult)
//{
//   // get the ::collection::map, and if no ::collection::map, then this message does not need reflection
//   /*      single_lock sl(afxMutexHwnd(), TRUE);
//   hwnd_map * pMap = afxMapHWND();
//   if (pMap == NULL)
//   return FALSE;
//
//   // check if in permanent ::collection::map, if it is reflect it (could be OLE control)
//   sp(::interaction_impl) pWnd =  (pMap->lookup_permanent(hWndChild)); */
//   sp(::user::interaction) pWnd = (FromHandlePermanent(hWndChild));
//   ASSERT(pWnd == NULL || LNX_WINDOW(pWnd)->get_handle() == hWndChild);
//   if (pWnd == NULL)
//   {
//      return FALSE;
//   }
//
//   // only OLE controls and permanent windows will get reflected msgs
//   ASSERT(pWnd != NULL);
//   return LNX_WINDOW(pWnd)->SendChildNotifyLastMsg(pResult);
//}

//bool interaction_impl::OnChildNotify(UINT uMsg, WPARAM wparam, LPARAM lparam, LRESULT* pResult)
//{
//
//   return ReflectChildNotify(uMsg, wparam, lparam, pResult);
//}
//
//bool interaction_impl::ReflectChildNotify(UINT uMsg, WPARAM wparam, LPARAM lparam, LRESULT* pResult)
//{
//   UNREFERENCED_PARAMETER(wparam);
//   // Note: reflected messages are send directly to interaction_impl::OnWndMsg
//   //  and interaction_impl::_001OnCommand for speed and because these messages are not
//   //  routed by normal _001OnCommand routing (they are only dispatched)
//
//   switch (uMsg)
//   {
//      // normal messages (just wparam, lparam through OnWndMsg)
//   case WM_HSCROLL:
//   case WM_VSCROLL:
//   case WM_PARENTNOTIFY:
//   case WM_DRAWITEM:
//   case WM_MEASUREITEM:
//   case WM_DELETEITEM:
//   case WM_VKEYTOITEM:
//   case WM_CHARTOITEM:
//   case WM_COMPAREITEM:
//      // reflect the message through the message ::collection::map as WM_REFLECT_BASE+uMsg
//      //return interaction_impl::OnWndMsg(WM_REFLECT_BASE+uMsg, wparam, lparam, pResult);
//      return FALSE;
//
//      // special case for WM_COMMAND
//   case WM_COMMAND:
//   {
//      // reflect the message through the message ::collection::map as OCM_COMMAND
//      /* xxx         int32_t nCode = HIWORD(wparam);
//      if (interaction_impl::_001OnCommand(0, MAKELONG(nCode, WM_REFLECT_BASE+WM_COMMAND), NULL, NULL))
//      {
//      if (pResult != NULL)
//      *pResult = 1;
//      return TRUE;
//      } */
//   }
//   break;
//
//   // special case for WM_NOTIFY
//   /*      case WM_NOTIFY:
//   {
//   // reflect the message through the message ::collection::map as OCM_NOTIFY
//   NMHDR* pNMHDR = (NMHDR*)lparam;
//   //            int32_t nCode = pNMHDR->code;
//   //            __NOTIFY notify;
//   //          notify.pResult = pResult;
//   //        notify.pNMHDR = pNMHDR;
//   // xxxx         return interaction_impl::_001OnCommand(0, MAKELONG(nCode, WM_REFLECT_BASE+WM_NOTIFY), &notify, NULL);
//   }
//
//   // other special cases (WM_CTLCOLOR family)*/
//   default:
//      if (uMsg >= WM_CTLCOLORMSGBOX && uMsg <= WM_CTLCOLORSTATIC)
//      {
//         // fill in special struct for compatiblity with 16-bit WM_CTLCOLOR
//         /*__CTLCOLOR ctl;
//         ctl.hDC = (HDC)wparam;
//         ctl.nCtlType = uMsg - WM_CTLCOLORMSGBOX;
//         //ASSERT(ctl.nCtlType >= CTLCOLOR_MSGBOX);
//         ASSERT(ctl.nCtlType <= CTLCOLOR_STATIC);
//
//         // reflect the message through the message ::collection::map as OCM_CTLCOLOR
//         bool bResult = interaction_impl::OnWndMsg(WM_REFLECT_BASE+WM_CTLCOLOR, 0, (LPARAM)&ctl, pResult);
//         if ((HBRUSH)*pResult == NULL)
//         bResult = FALSE;
//         return bResult;*/
//         return false;
//      }
//      break;
//   }
//
//   return false;   // let the parent handle it
//}

void interaction_impl::OnParentNotify(UINT message, LPARAM lparam)
{
   //if ((LOWORD(message) == WM_CREATE || LOWORD(message) == WM_DESTROY))
   //{
   //   if (ReflectLastMsg((oswindow)lparam))
   //      return;     // eat it
   //}
   //// not handled - do default
   //Default();
}

void interaction_impl::OnSetFocus(::user::interaction *)
{
   bool bHandled;

   bHandled = FALSE;
   if (!bHandled)
   {
      Default();
   }
}


LRESULT interaction_impl::OnActivateTopLevel(WPARAM wparam, LPARAM)
{

   return 0;

}


void interaction_impl::OnSysColorChange()
{

}


bool gen_GotScrollLines;


void interaction_impl::OnSettingChange(UINT uFlags, const char * lpszSection)
{

   gen_GotScrollLines = FALSE;

   interaction_impl::OnDisplayChange(0, 0);

}


LRESULT interaction_impl::OnDisplayChange(WPARAM, LPARAM)
{

   if (!(GetStyle() & WS_CHILD))
   {

      //const MESSAGE* pMsg = GetCurrentMessage();

      //SendMessageToDescendants(pMsg->message, pMsg->wParam, pMsg->lParam, TRUE, TRUE);

   }

   return Default();

}


LRESULT interaction_impl::OnDragList(WPARAM, LPARAM lparam)
{

   throw not_implemented(get_app());
   //
   //      LPDRAGLISTINFO lpInfo = (LPDRAGLISTINFO)lparam;
   //      ASSERT(lpInfo != NULL);
   //
   //      LRESULT lResult;
   //      if (ReflectLastMsg(lpInfo->hWnd, &lResult))
   //         return (int32_t)lResult;    // eat it
   //
   //      // not handled - do default
   //      return (int32_t)Default();
}

void interaction_impl::_001OnCreate(::signal_details * pobj)
{
   UNREFERENCED_PARAMETER(pobj);
   Default();


   if (m_pui->is_message_only_window())
   {

      TRACE("good : opt out!");

   }
   else
   {

      m_pthreadDraw = ::fork(get_app(), [&]()
      {

         DWORD dwStart;

         while (::get_thread_run())
         {

            dwStart = ::get_tick_count();

            if (!m_pui->m_bLockWindowUpdate)
            {

               if (m_pui->has_pending_graphical_update())
               {

                  RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE);

                  m_pui->on_after_graphical_update();

               }

            }

            _001UpdateBuffer();

            if (m_spgraphics.is_set())
            {

               m_spgraphics->update_window();

            }

            if (::get_tick_count() - dwStart < 5)
            {

               Sleep(5);

            }

         }

      });

   }

}



bool CALLBACK interaction_impl::GetAppsEnumWindowsProc(oswindow hwnd, LPARAM lparam)
{
   user::oswindow_array * phwnda = (user::oswindow_array *) lparam;
   phwnda->add(hwnd);
   return TRUE;
}

void interaction_impl::get_app_wnda(user::oswindow_array & wnda)
{
   throw not_implemented(::get_thread_app());
   //      EnumWindows(GetAppsEnumWindowsProc, (LPARAM) &wnda);
}

/*   void interaction_impl::_001OnDeferPaintLayeredWindowBackground(::draw2d::dib * pdib)
{
_001DeferPaintLayeredWindowBackground(pgraphics);
}*/


class print_window :
   virtual ::object
{
public:



   manual_reset_event m_event;
   oswindow m_hwnd;
   HDC m_hdc;

   print_window(sp(::aura::application) papp, oswindow hwnd, HDC hdc, DWORD dwTimeout) :
      ::object(papp),
      m_event(papp)

   {
      m_event.ResetEvent();
      m_hwnd = hwnd;
      m_hdc = hdc;
      __begin_thread(papp, &print_window::s_print_window, (LPVOID) this, ::multithreading::priority_normal);
      if (m_event.wait(millis(dwTimeout)).timeout())
      {
         TRACE("print_window::time_out");
      }
   }


   static_function UINT c_cdecl s_print_window(LPVOID pvoid)
   {
      print_window * pprintwindow = (print_window *)pvoid;
      try
      {
         HANDLE hevent = (HANDLE)pprintwindow->m_event.get_os_data();
         throw not_implemented(pprintwindow->get_app());
         /*            ::PrintWindow(pprintwindow->m_hwnd, pprintwindow->m_hdc, 0);
         ::SetEvent(hevent);*/
      }
      catch (...)
      {
      }
      return 0;
   }
};

void interaction_impl::_001DeferPaintLayeredWindowBackground(HDC hdc)
{

   rect rectClient;

   GetClientRect(rectClient);


   //pgraphics->FillSolidRect(rectClient, 0x00000000);

   //return;
   rect rectUpdate;
   GetWindowRect(rectUpdate);
   //      SetViewportOrgEx(hdc, 0, 0, NULL);
   rect rectPaint;
   rectPaint = rectUpdate;
   ScreenToClient(rectPaint);
   user::oswindow_array wndaApp;


   HRGN rgnWindow;
   HRGN rgnIntersect;
   HRGN rgnUpdate = NULL;

   throw not_implemented(get_app());
   /*
   rgnWindow = CreateRectRgn(0, 0, 0, 0);
   rgnIntersect = CreateRectRgn(0, 0, 0, 0);
   */
   //      int32_t iCount = wndaApp.get_count();

   throw not_implemented(get_app());
   //      try
   //      {
   //
   //         if(GetWindowLong(GWL_EXSTYLE) & WS_EX_LAYERED)
   //         {
   //            rect rect5;
   //            rect rect9;
   //
   //            rgnUpdate = CreateRectRgnIndirect(&rectUpdate);
   //            oswindow hwndOrder = ::GetWindow(get_handle(), GW_HWNDNEXT);
   //            for(;;)
   //            {
   //               //            char szText[1024];
   //               //::GetWindowTextA(hwndOrder, szText, sizeof(szText));
   //               if(hwndOrder == NULL ||
   //                  !::IsWindow(hwndOrder))
   //                  break;
   //               if(!::IsWindowVisible(hwndOrder) ||
   //                  ::IsIconic(hwndOrder) ||
   //                  hwndOrder == get_handle()
   //                  || wndaApp.contains(hwndOrder))
   //               {
   //                  if(hwndOrder == get_handle())
   //                  {
   //                     // add as bookmark - doesn't paint it
   //                     wndaApp.add(hwndOrder);
   //                  }
   //               }
   //               else
   //               {
   //                  rect rectWindow;
   //                  ::GetWindowRect(hwndOrder, rectWindow);
   //                  SetRectRgn(rgnWindow, rectWindow.left, rectWindow.top, rectWindow.right, rectWindow.bottom);
   //                  SetRectRgn(rgnIntersect, 0, 0, 0, 0);
   //                  CombineRgn(rgnIntersect, rgnUpdate, rgnWindow, RGN_AND);
   //                  rect rectIntersectBox;
   //                  GetRgnBox(rgnIntersect, rectIntersectBox);
   //                  if(rectIntersectBox.is_empty())
   //                  {
   //                  }
   //                  else
   //                  {
   //                     CombineRgn(rgnUpdate, rgnUpdate, rgnWindow, RGN_DIFF);
   //                     rect rectDiffBox;
   //                     GetRgnBox(rgnUpdate, rectDiffBox);
   //                     wndaApp.add(hwndOrder);
   //                     if(rectDiffBox.is_empty())
   //                     {
   //                        break;
   //                     }
   //                  }
   //               }
   //               hwndOrder = ::GetWindow(hwndOrder, GW_HWNDNEXT);
   //
   //
   //            }
   //            for(index j = wndaApp.get_upper_bound(); j >= 0; j--)
   //            {
   //               oswindow hWnd = wndaApp[j];
   //               if(hWnd == get_handle())
   //                  break;
   //               if(!::IsWindowVisible(hWnd) || ::IsIconic(hWnd))
   //                  continue;
   //               ::GetWindowRect(hWnd, rect5);
   //               rect9.intersect(rect5, rectUpdate);
   //               if(rect9.width() >0 && rect9.height() > 0)
   //               {
   //                  /*::interaction_impl * pwnd =  (interaction_impl::FromHandlePermanent(hWnd));
   //                  if(pwnd == NULL)
   //                  {
   //                  for(int32_t l = 0; l < wndpa.get_count(); l++)
   //                  {
   //                  if(wndpa[l]->get_safe_handle() == hWnd)
   //                  {
   //                  pwnd =  (wndpa[l]->m_pimpl);
   //                  break;
   //                  }
   //                  }
   //                  }
   //                  if(pwnd != NULL)
   //                  {
   //                  pwnd->_001Print(pgraphics);
   //                  }*/
   //                  //if(::GetWindowLong(wndaApp[j], GWL_EXSTYLE) & WS_EX_LAYERED)
   //                  if(true)
   //                  {
   //                     HDC hDCMem = CreateCompatibleDC(NULL);
   //                     HBITMAP hBmp = NULL;
   //                     {
   //                        HDC hDC = ::GetWindowDC(hWnd);
   //                        hBmp = CreateCompatibleBitmap(hDC, rect5.width(), rect5.height());
   //                        ::ReleaseDC(hWnd, hDC);
   //                     }
   //                     HGDIOBJ hOld = SelectObject(hDCMem, hBmp);
   //                     //print_window printwindow(get_app(), hWnd, hDCMem, 284);
   //                     ::PrintWindow(hWnd, hDCMem, 0);
   //                     ::BitBlt(
   //                        hdc ,
   //                        //rect5.left,
   //                        //rect5.top,
   //                        0, 0,
   //                        rect5.width(), rect5.height(),
   //                        hDCMem,
   //                        rectUpdate.left - rect5.left,
   //                        rectUpdate.top - rect5.top,
   //                        SRCCOPY);
   //                     ::SelectObject(hDCMem, hOld);
   //                     ::DeleteObject(hDCMem);
   //                     ::DeleteObject(hBmp);
   //                  }
   //                  else
   //                  {
   //                     SetViewportOrgEx(hdc, 0, 0, NULL);
   //                     HDC hdcWindow = ::GetDCEx(wndaApp[j], NULL, DCX_WINDOW);
   //                     if(hdcWindow == NULL)
   //                        hdcWindow = ::GetDCEx(wndaApp[j], NULL, DCX_WINDOW | DCX_caCHE);
   //                     if(hdcWindow != NULL)
   //                     {
   //                        ::BitBlt(
   //                           hdc,
   //                           rect5.left - rectUpdate.left,
   //                           rect5.top - rectUpdate.top,
   //                           rect5.width(), rect5.height(),
   //                           hdcWindow,
   //                           rect5.left - rect5.left,
   //                           rect5.top - rect5.top,
   //                           SRCCOPY);
   //                        ::ReleaseDC(wndaApp[j], hdcWindow);
   //                     }
   //                     else
   //                     {
   //                        TRACE0("WARNING: failed to draw a background. this surface probably will be black.");
   //                     }
   //                  }
   //               }
   //            }
   //         }
   //      }
   //      catch(...)
   //      {
   //
   //      }
   //
   //      ::DeleteObject(rgnWindow);
   //      ::DeleteObject(rgnIntersect);
   //      ::DeleteObject(rgnUpdate);
}

void interaction_impl::_001OnProdevianSynch(::signal_details * pobj)
{
   
   UNREFERENCED_PARAMETER(pobj);

}

void interaction_impl::_001OnPaint(::signal_details * pobj)
{

   //_001Expose();

   //lock lock(m_pui, 1984);

   //throw not_implemented(get_app());

   //      SCAST_PTR(::message::base, pbase, pobj);
   //
   //      PAINTSTRUCT paint;
   //      memset(&paint, 0, sizeof(paint));
   //      HDC hdc = ::BeginPaint(get_handle(), &paint);
   //      ::SelectClipRgn(hdc, NULL);
   //
   //      try
   //      {
   //
   //         ::draw2d::dib_sp dib(get_app());
   //
   //         rect rectWindow;
   //         GetWindowRect(rectWindow);
   //
   //         if(!dib->create(rectWindow.bottom_right()))
   //            return;
   //
   //         ::draw2d::graphics * pgraphics = dib->get_graphics();
   //
   //         if((dynamic_cast<::android::graphics * >(pgraphics))->get_handle() == NULL
   //            || (dynamic_cast<::android::graphics * >(pgraphics))->get_os_data2() == NULL)
   //            return;
   //
   //         rect rectPaint;
   //         rect rectUpdate;
   //         rectPaint = paint.rcPaint;
   //         if(rectPaint.is_null() || (GetExStyle() & WS_EX_LAYERED))
   //         {
   //            rectUpdate = rectWindow;
   //            rectPaint = rectWindow;
   //            ScreenToClient(rectPaint);
   //         }
   //         else
   //         {
   //            rectUpdate = rectPaint;
   //            ClientToScreen(rectUpdate);
   //         }
   //         (dynamic_cast<::android::graphics * >(pgraphics))->SelectClipRgn(NULL);
   //         if(m_pui != NULL && m_pui != this)
   //         {
   //            m_pui->_001OnDeferPaintLayeredWindowBackground(pgraphics);
   //         }
   //         else
   //         {
   //            _001OnDeferPaintLayeredWindowBackground(pgraphics);
   //         }
   //         (dynamic_cast<::android::graphics * >(pgraphics))->SelectClipRgn(NULL);
   //         (dynamic_cast<::android::graphics * >(pgraphics))->SetViewportOrg(point(0, 0));
   //         _000OnDraw(pgraphics);
   //         (dynamic_cast<::android::graphics * >(pgraphics))->SetViewportOrg(point(0, 0));
   //         //(dynamic_cast<::android::graphics * >(pgraphics))->FillSolidRect(rectUpdate.left, rectUpdate.top, 100, 100, 255);
   //         (dynamic_cast<::android::graphics * >(pgraphics))->SelectClipRgn(NULL);
   //         (dynamic_cast<::android::graphics * >(pgraphics))->SetViewportOrg(point(0, 0));
   //         BitBlt(hdc, rectPaint.left, rectPaint.top,
   //            rectPaint.width(), rectPaint.height(),
   //            (HDC) pgraphics->get_handle(), rectUpdate.left, rectUpdate.top,
   //            SRCCOPY);
   //
   //      }
   //      catch(...)
   //      {
   //      }
   //
   //      ::EndPaint(get_handle(), &paint);
   //      pobj->m_bRet = true;
   //      pbase->set_lresult(0);
}


void interaction_impl::_001OnPrint(::signal_details * pobj)
{
   throw not_implemented(get_app());
   //      SCAST_PTR(::message::base, pbase, pobj);
   //
   //      if(pbase->m_wparam == NULL)
   //         return;
   //
   //      ::draw2d::graphics_sp graphics(get_app());
   //      WIN_DC(graphics.m_p)->Attach((HDC) pbase->m_wparam);
   //      rect rectx;
   //      ::draw2d::bitmap * pbitmap = &graphics->GetCurrentBitmap();
   //      ::GetCurrentObject((HDC) pbase->m_wparam, OBJ_BITMAP);
   //      //      DWORD dw = ::GetLastError();
   //      class size size = pbitmap->get_size();
   //      rectx.left = 0;
   //      rectx.top = 0;
   //      rectx.right = size.cx;
   //      rectx.bottom = size.cy;
   //      try
   //      {
   //         rect rectWindow;
   //         GetWindowRect(rectWindow);
   //
   //         ::draw2d::dib_sp dib(get_app());
   //         if(!dib->create(rectWindow.bottom_right()))
   //            return;
   //
   //         ::draw2d::graphics * pgraphics = dib->get_graphics();
   //
   //         if(pgraphics->get_handle() == NULL)
   //            return;
   //
   //         rect rectPaint;
   //         rect rectUpdate;
   //         rectUpdate = rectWindow;
   //         rectPaint = rectWindow;
   //         rectPaint.offset(-rectPaint.top_left());
   //         (dynamic_cast<::android::graphics * >(pgraphics))->SelectClipRgn(NULL);
   //         if(m_pui != NULL && m_pui != this)
   //         {
   //            m_pui->_001OnDeferPaintLayeredWindowBackground(pgraphics);
   //         }
   //         else
   //         {
   //            _001OnDeferPaintLayeredWindowBackground(pgraphics);
   //         }
   //         (dynamic_cast<::android::graphics * >(pgraphics))->SelectClipRgn(NULL);
   //         (dynamic_cast<::android::graphics * >(pgraphics))->SetViewportOrg(point(0, 0));
   //         _000OnDraw(pgraphics);
   //         (dynamic_cast<::android::graphics * >(pgraphics))->SetViewportOrg(point(0, 0));
   //         //(dynamic_cast<::android::graphics * >(pgraphics))->FillSolidRect(rectUpdate.left, rectUpdate.top, 100, 100, 255);
   //         (dynamic_cast<::android::graphics * >(pgraphics))->SelectClipRgn(NULL);
   //         (dynamic_cast<::android::graphics * >(pgraphics))->SetViewportOrg(point(0, 0));
   //
   //         graphics->SelectClipRgn( NULL);
   //         graphics->BitBlt(rectPaint.left, rectPaint.top,
   //            rectPaint.width(), rectPaint.height(),
   //            pgraphics, rectUpdate.left, rectUpdate.top,
   //            SRCCOPY);
   //
   //         graphics->text_out(0, 0, "Te Amo Carlinhos!!", 11);
   //      }
   //      catch(...)
   //      {
   //      }
   //      graphics->FillSolidRect(rectx, RGB(255, 255, 255));
   //      WIN_DC(graphics.m_p)->Detach();
   //      pobj->m_bRet = true;
   //      pbase->set_lresult(0);
}


void interaction_impl::OnEnterIdle(UINT /*nWhy*/, ::user::interaction * /*pWho*/)
{
   // In some OLE inplace active scenarios, OLE will post a
   // message instead of sending it.  This causes so many WM_ENTERIDLE
   // messages to be sent that tasks running in the background stop
   // running.  By dispatching the pending WM_ENTERIDLE messages
   // when the first one is received, we trick oswindows into thinking
   // that only one was really sent and dispatched.
   {
      MESSAGE msg;
      throw not_implemented(get_app());
      //while (PeekMessage(&msg, NULL, WM_ENTERIDLE, WM_ENTERIDLE, PM_REMOVE))
      //while (PeekMessage(&msg, NULL, WM_ENTERIDLE, WM_ENTERIDLE, TRUE))
      // DispatchMessage(&msg);
   }

   Default();
}

HBRUSH interaction_impl::OnCtlColor(::draw2d::graphics *, ::user::interaction * pWnd, UINT)
{
   ASSERT(pWnd != NULL && pWnd->get_handle() != NULL);
   LRESULT lResult;
   if (pWnd->m_pimpl.cast < interaction_impl >()->SendChildNotifyLastMsg(&lResult))
      return (HBRUSH)lResult;     // eat it
   return (HBRUSH)Default();
}

// implementation of OnCtlColor for default gray backgrounds
//   (works for any interaction_impl containing controls)
//  return value of FALSE means caller must call DefWindowProc's default
//  TRUE means that 'hbrGray' will be used and the appropriate text
//    ('clrText') and background colors are set.
bool PASCAL interaction_impl::GrayCtlColor(HDC hDC, oswindow hWnd, UINT nCtlColor,
   HBRUSH hbrGray, COLORREF clrText)
{
   throw not_implemented(::get_thread_app());
   //      if (hDC == NULL)
   //      {
   //         // sometimes Win32 passes a NULL hDC in the WM_CTLCOLOR message.
   //         //         TRACE(::ca2::trace::category_AppMsg, 0, "Warning: hDC is NULL in interaction_impl::GrayCtlColor; WM_CTLCOLOR not processed.\n");
   //         return FALSE;
   //      }
   //
   //      if (hbrGray == NULL ||
   //         nCtlColor == CTLCOLOR_EDIT || nCtlColor == CTLCOLOR_MSGBOX ||
   //         nCtlColor == CTLCOLOR_SCROLLBAR)
   //      {
   //         return FALSE;
   //      }
   //
   //      if (nCtlColor == CTLCOLOR_LISTBOX)
   //      {
   //         // only handle requests to draw the space between edit and drop button
   //         //  in a drop-down combo (not a drop-down list)
   //         if (!__is_combo_box_control(hWnd, (UINT)CBS_DROPDOWN))
   //            return FALSE;
   //      }
   //
   //      // set background color and return handle to brush
   //      LOGBRUSH logbrush;
   //      VERIFY(::GetObject(hbrGray, sizeof(LOGBRUSH), (LPVOID)&logbrush));
   //      ::SetBkColor(hDC, logbrush.lbColor);
   //      if (clrText == (COLORREF)-1)
   //         clrText = ::GetSysColor(COLOR_WINDOWTEXT);  // normal text
   //      ::SetTextColor(hDC, clrText);
   //      return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 'dialog data' support

/*bool interaction_impl::UpdateData(bool bSaveAndValidate)
{
ASSERT(::IsWindow((oswindow) get_handle())); // calling UpdateData before DoModal?

CDataExchange dx(this, bSaveAndValidate);

// prevent control notifications from being dispatched during UpdateData
___THREAD_STATE* pThreadState = __get_thread_state();
oswindow hWndOldLockout = pThreadState->m_hLockoutNotifyWindow;
ASSERT(hWndOldLockout != get_handle());   // must not recurse
pThreadState->m_hLockoutNotifyWindow = get_handle();

bool bOK = FALSE;       // assume failure
try
{
DoDataExchange(&dx);
bOK = TRUE;         // it worked
}
catch(user_exception * pe)
{
// validation failed - ::fontopus::user already alerted, fall through
ASSERT(!bOK);
// Note: DELETE_EXCEPTION_(e) not required
}
catch(::exception::aura * pe)
{
// validation failed due to OOM or other resource failure
//e->ReportError(MB_ICONEXCLAMATION, __IDP_INTERNAL_FAILURE);
pe->ReportError(MB_ICONEXCLAMATION, "falha interna");
ASSERT(!bOK);
pe->Delete();
}


pThreadState->m_hLockoutNotifyWindow = hWndOldLockout;
return bOK;
}*/


/////////////////////////////////////////////////////////////////////////////
// Centering dialog support (works for any non-child interaction_impl)

void interaction_impl::CenterWindow(::user::interaction * pAlternateOwner)
{
   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //
   //      // determine owner interaction_impl to center against
   //      DWORD dwStyle = GetStyle();
   //      sp(::user::interaction) hWndCenter = pAlternateOwner;
   //      if (pAlternateOwner == NULL)
   //      {
   //         if (dwStyle & WS_CHILD)
   //            hWndCenter = GetParent();
   //         else
   //            hWndCenter = GetWindow(GW_OWNER);
   //         if (hWndCenter != NULL)
   //         {
   //            // let parent determine alternate center interaction_impl
   //            sp(::user::interaction) hWndTemp =
   //               (sp(::user::interaction) )hWndCenter->send_message(WM_QUERYCENTERWND, 0, 0);
   //            if (hWndTemp != NULL)
   //               hWndCenter = hWndTemp;
   //         }
   //      }
   //
   //      // get coordinates of the interaction_impl relative to its parent
   //      rect rcDlg;
   //      GetWindowRect(&rcDlg);
   //      rect rcarea;
   //      rect rcCenter;
   //      sp(::user::interaction) hWndParent;
   //      if (!(dwStyle & WS_CHILD))
   //      {
   //         // don't center against invisible or minimized windows
   //         if (hWndCenter != NULL)
   //         {
   //            DWORD dwAlternateStyle = hWndCenter->GetWindowLong(GWL_STYLE);
   //            if (!(dwAlternateStyle & WS_VISIBLE) || (dwAlternateStyle & WS_MINIMIZE))
   //               hWndCenter = NULL;
   //         }
   //
   //         MONITORINFO mi;
   //         mi.cbSize = sizeof(mi);
   //
   //         // center within appropriate monitor coordinates
   //         if (hWndCenter == NULL)
   //         {
   //            oswindow hwDefault = System.GetMainWnd()->get_handle();
   //
   //            GetMonitorInfo(
   //               MonitorFromWindow(hwDefault, MONITOR_DEFAULTTOPRIMARY), &mi);
   //            rcCenter = mi.rcWork;
   //            rcarea = mi.rcWork;
   //         }
   //         else
   //         {
   //            hWndCenter->GetWindowRect(&rcCenter);
   //            GetMonitorInfo(
   //               MonitorFromWindow(hWndCenter->get_handle(), MONITOR_DEFAULTTONEAREST), &mi);
   //            rcarea = mi.rcWork;
   //         }
   //      }
   //      else
   //      {
   //         // center within parent client coordinates
   //         hWndParent = GetParent();
   //         ASSERT(hWndParent->IsWindow());
   //
   //         hWndParent->GetClientRect(&rcarea);
   //         ASSERT(hWndCenter->IsWindow());
   //         hWndCenter->GetClientRect(&rcCenter);
   //         ::MapWindowPoints(hWndCenter->get_handle(), hWndParent->get_handle(), (POINT*)&rcCenter, 2);
   //      }
   //
   //      // find dialog's upper left based on rcCenter
   //      int32_t xLeft = (rcCenter.left + rcCenter.right) / 2 - rcDlg.width() / 2;
   //      int32_t yTop = (rcCenter.top + rcCenter.bottom) / 2 - rcDlg.height() / 2;
   //
   //      // if the dialog is outside the screen, move it inside
   //      if (xLeft < rcarea.left)
   //         xLeft = rcarea.left;
   //      else if (xLeft + rcDlg.width() > rcarea.right)
   //         xLeft = rcarea.right - rcDlg.width();
   //
   //      if (yTop < rcarea.top)
   //         yTop = rcarea.top;
   //      else if (yTop + rcDlg.height() > rcarea.bottom)
   //         yTop = rcarea.bottom - rcDlg.height();
   //
   //      // ::collection::map screen coordinates to child coordinates
   //      SetWindowPos(NULL, xLeft, yTop, -1, -1,
   //         SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

bool interaction_impl::CheckAutoCenter()
{
   return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Dialog initialization support

bool interaction_impl::ExecuteDlgInit(const char * lpszResourceName)
{
   // find resource handle
   LPVOID lpResource = NULL;
   HGLOBAL hResource = NULL;
   if (lpszResourceName != NULL)
   {
      //         HINSTANCE hInst = ::ca2::FindResourceHandle(lpszResourceName, RT_DLGINIT);
      //       HRSRC hDlgInit = ::FindResource(hInst, lpszResourceName, RT_DLGINIT);
      /*     if (hDlgInit != NULL)
      {
      // load it
      hResource = LoadResource(hInst, hDlgInit);
      if (hResource == NULL)
      return FALSE;
      // lock it
      lpResource = LockResource(hResource);
      ASSERT(lpResource != NULL);
      }*/
   }

   // execute it
   bool bResult = ExecuteDlgInit(lpResource);

   // cleanup
   if (lpResource != NULL && hResource != NULL)
   {
      throw not_implemented(get_app());
      //         UnlockResource(hResource);
      //         FreeResource(hResource);
   }
   return bResult;
}

bool interaction_impl::ExecuteDlgInit(LPVOID lpResource)
{
   throw not_implemented(get_app());
   //      bool bSuccess = TRUE;
   //      if (lpResource != NULL)
   //      {
   //         UNALIGNED WORD* lpnRes = (WORD*)lpResource;
   //         while (bSuccess && *lpnRes != 0)
   //         {
   //            WORD nIDC = *lpnRes++;
   //            WORD nMsg = *lpnRes++;
   //            DWORD dwLen = *((UNALIGNED DWORD*&)lpnRes)++;
   //
   //            // In Win32 the WM_ messages have changed.  They have
   //            // to be translated from the 32-bit values to 16-bit
   //            // values here.
   //
   //#define WIN16_LB_ADDSTRING  0x0401
   //#define WIN16_CB_ADDSTRING  0x0403
   //#define __CB_ADDSTRING   0x1234
   //
   //            // unfortunately, WIN16_CB_ADDSTRING == CBEM_INSERTITEM
   //            if (nMsg == __CB_ADDSTRING)
   //               nMsg = CBEM_INSERTITEM;
   //            else if (nMsg == WIN16_LB_ADDSTRING)
   //               nMsg = LB_ADDSTRING;
   //            else if (nMsg == WIN16_CB_ADDSTRING)
   //               nMsg = CB_ADDSTRING;
   //
   //            // check for invalid/unknown message types
   //            ASSERT(nMsg == LB_ADDSTRING || nMsg == CB_ADDSTRING ||
   //               nMsg == CBEM_INSERTITEM);
   //
   //#ifdef DEBUG
   //            // For AddStrings, the count must exactly delimit the
   //            // string, including the NULL termination.  This check
   //            // will not catch all mal-formed ADDSTRINGs, but will
   //            // catch some.
   //            if (nMsg == LB_ADDSTRING || nMsg == CB_ADDSTRING || nMsg == CBEM_INSERTITEM)
   //               ASSERT(*((LPBYTE)lpnRes + (UINT)dwLen - 1) == 0);
   //#endif
   //
   //            if (nMsg == CBEM_INSERTITEM)
   //            {
   //               COMBOBOXEXITEM item = {0};
   //               item.mask = CBEIF_TEXT;
   //               item.iItem = -1;
   //               string strText(reinterpret_cast<LPTSTR>(lpnRes));
   //               item.pszText = const_cast<LPTSTR>(strText.GetString());
   //               if (::SendDlgItemMessage(get_handle(), nIDC, nMsg, 0, (LPARAM) &item) == -1)
   //                  bSuccess = FALSE;
   //            }
   //            {
   //               // List/Combobox returns -1 for error
   //               if (::SendDlgItemMessageA(get_handle(), nIDC, nMsg, 0, (LPARAM) lpnRes) == -1)
   //                  bSuccess = FALSE;
   //            }
   //
   //
   //            // skip past data
   //            lpnRes = (WORD*)((LPBYTE)lpnRes + (UINT)dwLen);
   //         }
   //      }
   //
   //      // send update message to all controls after all other siblings loaded
   //      if (bSuccess)
   //         SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, FALSE, FALSE);
   //
   //      return bSuccess;
}

void interaction_impl::UpdateDialogControls(command_target* pTarget, bool bDisableIfNoHndler)
{
   UNREFERENCED_PARAMETER(pTarget);
   UNREFERENCED_PARAMETER(bDisableIfNoHndler);
   cmd_ui state(get_app());
   //interaction_impl wndTemp;       // very temporary interaction_impl just for CmdUI update

                                   // walk all the kids - assume the IDs are for buttons
                                   /* xxx   for (oswindow hWndChild = ::GetTopWindow(get_handle()); hWndChild != NULL;
                                   hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
                                   {
                                   // send to buttons
                                   wndTemp.set_handle(hWndChild); // quick and dirty attach
                                   state.m_nID = __get_dialog_control_id(hWndChild);
                                   state.m_pOther = &wndTemp;

                                   // check for reflect handlers in the child interaction_impl
                                   sp(::interaction_impl) pWnd = ::android::interaction_impl::FromHandlePermanent(hWndChild);
                                   if (pWnd != NULL)
                                   {
                                   // call it directly to disable any routing
                                   if (LNX_WINDOW(pWnd)->interaction_impl::_001OnCommand(0, MAKELONG(0xffff,
                                   WM_COMMAND+WM_REFLECT_BASE), &state, NULL))
                                   continue;
                                   }

                                   // check for handlers in the parent interaction_impl
                                   if (interaction_impl::_001OnCommand((UINT)state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
                                   continue;

                                   // determine whether to disable when no handler exists
                                   bool bDisableTemp = bDisableIfNoHndler;
                                   if (bDisableTemp)
                                   {
                                   if ((wndTemp.SendMessage(WM_GETDLGCODE) & DLGC_BUTTON) == 0)
                                   {
                                   // non-button controls don't get automagically disabled
                                   bDisableTemp = FALSE;
                                   }
                                   else
                                   {
                                   // only certain button controls get automagically disabled
                                   UINT nStyle = (UINT)(wndTemp.GetStyle() & 0x0F);
                                   if (nStyle == (UINT)BS_AUTOCHECKBOX ||
                                   nStyle == (UINT)BS_AUTO3STATE ||
                                   nStyle == (UINT)BS_GROUPBOX ||
                                   nStyle == (UINT)BS_AUTORADIOBUTTON)
                                   {
                                   bDisableTemp = FALSE;
                                   }
                                   }
                                   }
                                   // check for handlers in the target (owner)
                                   state.DoUpdate(pTarget, bDisableTemp);
                                   }
                                   wndTemp.set_handle(NULL);      // quick and dirty detach */
}


/////////////////////////////////////////////////////////////////////////////
// frame_window (here for library granularity)

bool interaction_impl::is_frame_window()
{
   return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
// Extra interaction_impl support for dynamic subclassing of controls

bool interaction_impl::SubclassWindow(oswindow hWnd)
{
   if (!Attach(hWnd))
      return FALSE;

   // allow any other subclassing to occur
   pre_subclass_window();

   throw not_implemented(get_app());
   //      m_pfnSuper = (WNDPROC)::GetWindowLongPtr(hWnd, GWLP_WNDPROC);

   // now hook into the AFX WndProc
   //      WNDPROC* lplpfn = GetSuperWndProcaddr();
   //      WNDPROC oldWndProc = (WNDPROC)::SetWindowLongPtr(hWnd, GWLP_WNDPROC,
   //         (int_ptr)__get_window_procedure());
   //      ASSERT(oldWndProc != __get_window_procedure());
   //
   //      if (*lplpfn == NULL)
   //         *lplpfn = oldWndProc;   // the first control of that type created
   //#ifdef DEBUG
   //      else if (*lplpfn != oldWndProc)
   //      {
   //         TRACE(::ca2::trace::category_AppMsg, 0, "p: Trying to use SubclassWindow with incorrect interaction_impl\n");
   //         TRACE(::ca2::trace::category_AppMsg, 0, "\tderived class.\n");
   //         TRACE(::ca2::trace::category_AppMsg, 0, "\thWnd = $%08X (nIDC=$%08X) is not a %hs.\n", (UINT)(uint_ptr)hWnd,
   //            __get_dialog_control_id(hWnd), typeid(*this).name());
   //         ASSERT(FALSE);
   //         // undo the subclassing if continuing after assert
   //         ::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (int_ptr)oldWndProc);
   //      }
   //#endif
   //      ::message::size size(get_app());
   //      _001OnSize(&size);
   //      return TRUE;
   //   }
   //
   //   bool interaction_impl::SubclassDlgItem(UINT nID, sp(::interaction_impl) pParent)
   //   {
   //      ASSERT(pParent != NULL);
   //      ASSERT(::IsWindow(LNX_WINDOW(pParent)->get_handle()));
   //
   //      // check for normal dialog control first
   //      oswindow hWndControl = ::GetDlgItem(LNX_WINDOW(pParent)->get_handle(), nID);
   //      if (hWndControl != NULL)
   //         return SubclassWindow(hWndControl);
   //
   //
   //      return FALSE;   // control not found
}

oswindow interaction_impl::UnsubclassWindow()
{
   ASSERT(::IsWindow((oswindow)get_handle()));

   throw not_implemented(get_app());
   //      // set WNDPROC back to original value
   //      WNDPROC* lplpfn = GetSuperWndProcaddr();
   //      SetWindowLongPtr(get_handle(), GWLP_WNDPROC, (int_ptr)*lplpfn);
   //      *lplpfn = NULL;
   //
   //      // and Detach the oswindow from the interaction_impl object
   //      return Detach();
}


/*   view_update_hint::view_update_hint(sp(::aura::application) papp) :
::object(papp)
{
}
*/



bool interaction_impl::IsChild(::user::interaction * pWnd)
{
   ASSERT(::IsWindow((oswindow)get_handle()));
   if (pWnd->get_handle() == NULL)
   {
      return ::user::interaction_impl::IsChild(pWnd);
   }
   else
   {
      return ::IsChild((oswindow)get_handle(), pWnd->get_handle()) != FALSE;
   }
}


bool interaction_impl::IsWindow() const
{

   return ::IsWindow((oswindow)get_handle()) != FALSE;

}


oswindow interaction_impl::get_handle() const
{

   return m_oswindow;

}


#define SWP_IGNOREPALACEGUARD 0x80000000

bool interaction_impl::SetWindowPos(int_ptr z, int32_t x, int32_t y, int32_t cx, int32_t cy, UINT nFlags)
{

   return ::user::interaction_impl::SetWindowPos(z, x, y, cx, cy, nFlags);

   //      single_lock sl(&user_mutex(), true);

   //xdisplay d(m_oswindow->display());

   rect rectScreen;

   m_pui->best_monitor(rectScreen);

   int iPalaceGuard = 256;

   if (nFlags & SWP_IGNOREPALACEGUARD)
      iPalaceGuard = 1;

   if (x > rectScreen.right - iPalaceGuard)
      x = rectScreen.right - iPalaceGuard;

   if (x < 0)
      x = 0;

   if (y > rectScreen.bottom - iPalaceGuard)
      y = rectScreen.bottom - iPalaceGuard;

   if (y < 0)
      y = 0;

   if (cx > rectScreen.width())
      cx = rectScreen.width();

   if (cx < iPalaceGuard)
      cx = iPalaceGuard;

   if (cy > rectScreen.height())
      cy = rectScreen.height();

   if (cy < iPalaceGuard)
      cy = iPalaceGuard;


   ASSERT(::IsWindow((oswindow)get_handle()));
   /*   return ::SetWindowPos(get_handle(), pWndInsertAfter->get_handle(),
   x, y, cx, cy, nFlags) != FALSE; */

   //throw not_implemented(get_app());

   //XSizeHints hints;

   ::SetWindowPos((oswindow) get_handle(), 0, x, y, cx, cy, nFlags);
   if (nFlags & SWP_NOMOVE)
   {
      if (nFlags & SWP_NOSIZE)
      {
         //hints.flags = 0;
      }
      else
      {
         //hints.flags = PSize;
         //hints.width = cx;
         //hints.height = cy;
         //XResizeWindow(m_oswindow->display(), m_oswindow->window(), cx, cy);
         //            XClearWindow(m_oswindow->display(), m_oswindow->interaction_impl());
      }
   }
   else
   {
      if (nFlags & SWP_NOSIZE)
      {
         //XMoveWindow(m_oswindow->display(), m_oswindow->window(), x, y);
         ////          XClearWindow(m_oswindow->display(), m_oswindow->interaction_impl());
         //hints.flags = PPosition;
         //hints.x = x;
         //hints.y = y;
      }
      else
      {
         //XMoveResizeWindow(m_oswindow->display(), m_oswindow->window(), x, y, cx, cy);
         ////        XClearWindow(m_oswindow->display(), m_oswindow->interaction_impl());
         //hints.flags = PPosition | PSize;
         //hints.x = x;
         //hints.y = y;
         //hints.width = cx;
         //hints.height = cy;
      }
   }

   if (!IsWindowVisible())
   {

      /*XSetNormalHints(m_oswindow->display(), m_oswindow->window(), &hints);*/

   }

   ::rect r;

   ::GetWindowRect(get_handle(), r);

   ::copy(m_rectParentClient, r);

   if ((nFlags & SWP_SHOWWINDOW))
   {

      if (!IsWindowVisible())
      {

         /*XMapWindow(m_oswindow->display(), m_oswindow->window());*/

         ::ShowWindow(get_handle(), SW_SHOW);

      }

   }


   /*
   if(GetExStyle() & WS_EX_LAYERED)
   {
   if(rectWindowOld.top_left() != m_rectParentClient.top_left())
   {
   send_message(WM_MOVE, 0, 0);
   }
   if(rectWindowOld.size() != m_rectParentClient.size())
   {
   send_message(WM_SIZE, 0, 0);
   }

   }
   if(GetExStyle() & WS_EX_LAYERED)
   {
   nFlags |= SWP_NOCOPYBITS;
   nFlags |= SWP_NOREDRAW;
   nFlags |= SWP_NOMOVE;
   nFlags |= SWP_NOSIZE;
   //nFlags |= SWP_NOZORDER;
   //nFlags |= SWP_FRAMECHANGED;
   if(nFlags & SWP_SHOWWINDOW)
   {
   ::SetWindowPos(get_handle(), (oswindow) z, x, y, cx, cy, nFlags);
   ShowWindow(SW_SHOW);
   }
   else
   {
   ::SetWindowPos(get_handle(), (oswindow) z, x, y, cx, cy, nFlags);
   }
   /*if(m_pui != NULL)
   {
   m_pui->oprop("pending_layout") = true;
   m_pui->oprop("pending_zorder") = z;
   }*/
   /*    }
   else
   {
   if(z == -3)
   {
   ::SetWindowPos(get_handle(), (oswindow) 0, x, y, cx, cy, nFlags);
   }
   else
   {
   ::SetWindowPos(get_handle(), (oswindow) z, x, y, cx, cy, nFlags);
   }
   }*/
   return true;

}


void interaction_impl::MoveWindow(int32_t x, int32_t y, int32_t nWidth, int32_t nHeight, bool bRepaint)
{
   ASSERT(::IsWindow((oswindow)get_handle()));
   SetWindowPos(0, x, y, nWidth, nHeight, bRepaint ? SWP_SHOWWINDOW : 0);
}


bool interaction_impl::ClientToScreen(LPRECT lprect)
{

   class rect64 rectWindow;

   if(!GetWindowRect(rectWindow))
   {

      return false;

   }

   lprect->left += (LONG)rectWindow.left;
   lprect->right += (LONG)rectWindow.left;
   lprect->top += (LONG)rectWindow.top;
   lprect->bottom += (LONG)rectWindow.top;

   return true;

}


bool interaction_impl::ClientToScreen(LPPOINT lppoint)
{

   class rect64 rectWindow;

   if(!GetWindowRect(rectWindow))
   {

      return false;

   }

   lppoint->x += (LONG)rectWindow.left;
   lppoint->y += (LONG)rectWindow.top;

   return true;

}


bool interaction_impl::ClientToScreen(RECT64 * lprect)
{

   class rect rectWindow;

   if(!GetWindowRect(rectWindow))
   {

      return false;

   }

   lprect->left += rectWindow.left;
   lprect->right += rectWindow.left;
   lprect->top += rectWindow.top;
   lprect->bottom += rectWindow.top;

   return true;

}


bool interaction_impl::ClientToScreen(POINT64 * lppoint)
{

   class rect64 rectWindow;

   if(!GetWindowRect(rectWindow))
   {

      return false;

   }

   lppoint->x += rectWindow.left;
   lppoint->y += rectWindow.top;

   return true;

}


bool interaction_impl::ScreenToClient(LPRECT lprect)
{

   class rect64 rectWindow;

   if(!GetWindowRect(rectWindow))
   {

      return false;

   }

   lprect->left -= (LONG)rectWindow.left;
   lprect->right -= (LONG)rectWindow.left;
   lprect->top -= (LONG)rectWindow.top;
   lprect->bottom -= (LONG)rectWindow.top;

   return true;

}


bool interaction_impl::ScreenToClient(LPPOINT lppoint)
{

   class rect64 rectWindow;

   if(!GetWindowRect(rectWindow))
   {

      return false;

   }

   lppoint->x -= (LONG)rectWindow.left;
   lppoint->y -= (LONG)rectWindow.top;

   return true;

}


bool interaction_impl::ScreenToClient(RECT64 * lprect)
{

   class rect64 rectWindow;

   if(!GetWindowRect(rectWindow))
   {

      return false;

   }

   lprect->left -= rectWindow.left;
   lprect->right -= rectWindow.left;
   lprect->top -= rectWindow.top;
   lprect->bottom -= rectWindow.top;

   return true;

}


bool interaction_impl::ScreenToClient(POINT64 * lppoint)
{

   class rect64 rectWindow;

   if(!GetWindowRect(rectWindow))
   {

      return false;

   }

   lppoint->x -= rectWindow.left;
   lppoint->y -= rectWindow.top;

   return true;

}


bool interaction_impl::GetWindowRect(RECT64 * lprect)
{

   if (!::IsWindow((oswindow)get_handle()))
   {

      return false;

   }

   // if it is temporary interaction_impl - probably not ca2 wrapped interaction_impl
   //if(m_pui == NULL || m_pui == this)
   {
      rect rect32;

      if(!::GetWindowRect((oswindow)get_handle(),rect32))
      {

         return false;

      }

      ::copy(lprect, rect32);
   }
   //else
   {
      //  interaction::GetWindowRect(lprect);
   }

   return true;

}


bool interaction_impl::GetClientRect(RECT64 * lprect)
{

   if(!::IsWindow((oswindow)get_handle()))
   {

      return false;

   }
   // if it is temporary interaction_impl - probably not ca2 wrapped interaction_impl
   //if(m_pui == NULL || m_pui == this)
   {
      rect rect32;
      if(!::GetClientRect((oswindow)get_handle(),rect32))
      {

         return false;

      }
      ::copy(lprect, rect32);
   }
   //else
   {
      // interaction::GetClientRect(lprect);
   }

   return true;

}


id interaction_impl::SetDlgCtrlId(id id)
{

   return m_pui->SetDlgCtrlId(id);
}

id interaction_impl::GetDlgCtrlId()
{
   return m_pui->GetDlgCtrlId();
}

/*   guie_message_wnd::guie_message_wnd(sp(::aura::application) papp) :
::object(papp)
{
m_puiForward = NULL;
}

LRESULT guie_message_wnd::message_handler(::signal_details * pobj)
{
if(m_puiForward != NULL)
{
return m_puiForward->message_handler(uiMessage, wparam, lparam);
}
else
{
return 0;
}
}*/

void interaction_impl::_001WindowMaximize()
{
   ::user::interaction_impl::_001WindowMaximize();
}

void interaction_impl::_001WindowRestore()
{
   if (m_pui != NULL)
      m_pui->m_eappearance = ::user::AppearanceNormal;
   ::ShowWindow((oswindow)get_handle(), SW_RESTORE);
}

bool interaction_impl::ShowWindow(int32_t nCmdShow)
{

   if (!::IsWindow((oswindow)get_handle()))
      return false;

   ::ShowWindow((oswindow)get_handle(), nCmdShow);

   return m_pui->IsWindowVisible();

}


bool interaction_impl::WfiIsIconic()
{

   if (GetExStyle() & WS_EX_LAYERED)
   {

      return m_pui->m_eappearance == ::user::AppearanceIconic;

   }
   else
   {

      return ::IsIconic((oswindow)get_handle()) != FALSE;

   }

}

bool interaction_impl::WfiIsZoomed()
{
   ASSERT(::IsWindow((oswindow)get_handle()));
   return m_pui->m_eappearance == ::user::AppearanceZoomed;
}


LONG interaction_impl::GetWindowLong(int32_t nIndex)
{
   return ::GetWindowLong((oswindow)get_handle(), nIndex);
}

LONG interaction_impl::SetWindowLong(int32_t nIndex, LONG lValue)
{
   return ::SetWindowLong((oswindow)get_handle(), nIndex, lValue);
}




// interaction_impl
/* interaction_impl::operator oswindow() const
{ return this == NULL ? NULL : get_handle(); }*/
bool interaction_impl::operator==(const ::user::interaction_impl& wnd) const
{
   return const_cast < ::user::interaction_impl * >  (&wnd)->get_handle() == ((interaction_impl *)this)->get_handle();
}

bool interaction_impl::operator!=(const ::user::interaction_impl& wnd) const
{
   return const_cast < ::user::interaction_impl * >  (&wnd)->get_handle() != ((interaction_impl *)this)->get_handle();
}

//DWORD interaction_impl::GetStyle() const
//{
//   ASSERT(::IsWindow((oswindow)get_handle()));
//   return (DWORD)::GetWindowLong((oswindow)get_handle(), GWL_STYLE);
//}
//
//DWORD interaction_impl::GetExStyle() const
//{
//   ASSERT(::IsWindow((oswindow)get_handle()));
//   return (DWORD)::GetWindowLong((oswindow)get_handle(), GWL_EXSTYLE);
//}

//bool interaction_impl::ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags)
//{
//   ASSERT(::IsWindow((oswindow)get_handle()));
//   return ModifyStyle((oswindow)get_handle(), dwRemove, dwAdd, nFlags);
//}
//
//bool interaction_impl::ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags)
//{
//   ASSERT(::IsWindow((oswindow)get_handle()));
//   return ModifyStyleEx((oswindow)get_handle(), dwRemove, dwAdd, nFlags);
//}

void interaction_impl::set_owner(::user::interaction * pOwnerWnd)
{
   //      m_puiOwner = pOwnerWnd;
   m_pui->SetOwner((pOwnerWnd));
}

LRESULT interaction_impl::send_message(UINT message, WPARAM wparam, lparam lparam)
{

   if (::get_thread() == NULL)
   {

      ::set_thread(m_pui->get_app());

   }

   return ::user::interaction_impl::send_message(message, wparam, lparam);

}


bool interaction_impl::post_message(UINT message, WPARAM wparam, lparam lparam)
{

   return ::PostMessage((oswindow)get_handle(), message, wparam, lparam) != FALSE;

}


bool interaction_impl::DragDetect(POINT pt) const
{

   throw not_implemented(get_app());
   //ASSERT(::IsWindow((oswindow) get_handle()));

   //return ::DragDetect(get_handle(), pt) != FALSE;

}


//void interaction_impl::RedrawWindow(UINT nFlags)
//{
//
//   _001UpdateWindow();
//
//}


//void interaction_impl::SetWindowText(const char * lpszString)
//{
//
//   m_strWindowText = lpszString;
//
//}

//
//strsize interaction_impl::GetWindowText(LPTSTR lpszString, strsize nMaxCount)
//{
//
//   strncpy(lpszString, m_strWindowText, nMaxCount);
//
//   return MIN(nMaxCount, m_strWindowText.get_length());
//
//}


//strsize interaction_impl::GetWindowTextLength()
//{
//
//   throw not_implemented(get_app());
//   //ASSERT(::IsWindow((oswindow) get_handle()));
//
//   //return ::GetWindowTextLength(get_handle());
//
//}
//

void interaction_impl::SetFont(::draw2d::font* pfont, bool bRedraw)
{

   UNREFERENCED_PARAMETER(bRedraw);

   //ASSERT(::IsWindow((oswindow) get_handle())); m_pfont = new ::draw2d::font(*pfont);

}


::draw2d::font* interaction_impl::GetFont()
{

   //ASSERT(::IsWindow((oswindow) get_handle()));

   //return m_pfont;
   return NULL;

}


void interaction_impl::DragAcceptFiles(bool bAccept)
{

   throw not_implemented(get_app());

   //ASSERT(::IsWindow((oswindow) get_handle()));
   //::DragAcceptFiles(get_handle(), bAccept);

}

/*
sp(::user::frame_window) interaction_impl::EnsureParentFrame()
{

sp(::user::frame_window) pFrameWnd=GetParentFrame();

ENSURE_VALID(pFrameWnd);

return pFrameWnd;

}


::user::interaction * interaction_impl::EnsureTopLevelParent()
{

sp(::user::interaction)pWnd=GetTopLevelParent();

ENSURE_VALID(pWnd);

return pWnd;

}

*/
void interaction_impl::SetWindowText(const char * lpszString)
{

   m_strWindowText = lpszString;

}


strsize interaction_impl::GetWindowText(LPTSTR lpszString, strsize nMaxCount)
{

   strncpy(lpszString, m_strWindowText, nMaxCount);

   return MIN(nMaxCount, m_strWindowText.get_length());

}


strsize interaction_impl::GetWindowTextLength()
{

   throw not_implemented(get_app());
   //ASSERT(::IsWindow((oswindow) get_handle()));

   //return ::GetWindowTextLength(get_handle());

}



void interaction_impl::GetWindowText(string & rString)
{
   /*ASSERT(::IsWindow((oswindow) get_handle()));

   int32_t nLen = ::GetWindowTextLength(get_handle());
   ::GetWindowText(get_handle(), rString.GetBufferSetLength(nLen), nLen+1);
   rString.ReleaseBuffer();*/
   rString = m_strWindowText;

}


::user::interaction * interaction_impl::GetParent()
{

   if (!::IsWindow((oswindow)get_handle()))
      return NULL;

   if (get_handle() == NULL)
      return NULL;

   //return ::linux::interaction_impl::from_handle(::GetParent(get_handle()));

   return NULL;

}

void interaction_impl::MoveWindow(LPCRECT lpRect, bool bRepaint)
{

   MoveWindow(lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, bRepaint);

}


UINT interaction_impl::ArrangeIconicWindows()
{

   throw not_implemented(get_app());

   //      ASSERT(::IsWindow((oswindow) get_handle())); return ::ArrangeIconicWindows(get_handle());

}


int32_t interaction_impl::SetWindowRgn(HRGN hRgn, bool bRedraw)
{

   UNREFERENCED_PARAMETER(hRgn);
   UNREFERENCED_PARAMETER(bRedraw);

   //throw not_implemented(get_app());

   //      ASSERT(::IsWindow((oswindow) get_handle())); return ::SetWindowRgn(get_handle(), hRgn, bRedraw);

   return 0;

}


int32_t interaction_impl::GetWindowRgn(HRGN hRgn)
{

   throw not_implemented(get_app());

   //      ASSERT(::IsWindow((oswindow) get_handle()) && hRgn != NULL); return ::GetWindowRgn(get_handle(), hRgn);
   return 0;

}


bool interaction_impl::BringWindowToTop()
{

   //      throw not_implemented(get_app());
   //      return ::BringWindowToTop(get_handle()) != FALSE;

}


void interaction_impl::MapWindowPoints(::user::interaction * pwndTo, LPPOINT lpPoint, UINT nCount)
{

   throw not_implemented(get_app());

   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      ::MapWindowPoints(get_handle(), (oswindow) pwndTo->get_handle(), lpPoint, nCount);

}


void interaction_impl::MapWindowPoints(::user::interaction * pwndTo, LPRECT lpRect)
{

   throw not_implemented(get_app());

   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      ::MapWindowPoints(get_handle(), (oswindow) pwndTo->get_handle(), (LPPOINT)lpRect, 2);

}


::draw2d::graphics * interaction_impl::GetDC()
{

   ::draw2d::graphics_sp g(allocer());

   //xdisplay d(m_oswindow->display());

   oswindow oswindow;

   if (get_handle() == NULL)
   {

      //         oswindow = ::GetDesktopWindow();

   }
   else
   {

      oswindow = (::oswindow) get_handle();

   }

   rect rectClient;

   //oswindow->get_client_rect(rectClient);

   rectClient.left = 0;
   rectClient.top = 0;
   rectClient.right = 500;
   rectClient.bottom = 500;
   //      (dynamic_cast < ::android::graphics * >(g.m_p))->attach(cairo_create(cairo_xlib_surface_create(oswindow->display(), oswindow->interaction_impl(), oswindow->visual(),rectClient.width(), rectClient.height())));
   return g.detach();
}

::draw2d::graphics * interaction_impl::GetWindowDC()
{
   /*ASSERT(::IsWindow((oswindow)get_handle()));
   ::draw2d::graphics_sp g(allocer());
   g->attach(::GetWindowDC(get_handle()));
   return g.detach();*/
   return GetDC();
}

bool interaction_impl::ReleaseDC(::draw2d::dib * pdib)
{

   //if (pgraphics == NULL)
     // return false;

   /*      cairo_t * pcairo = (cairo_t *) pgraphics->get_os_data();

   cairo_surface_t * psurface = cairo_get_target(pcairo);
   if(pcairo ==  g_cairo)
   {
   printf("123");

   }

   cairo_destroy(pcairo);

   if(psurface == g_cairosurface)
   {
   printf("123");
   }      cairo_surface_destroy(psurface);*/

   //      if(((Gdiplus::Graphics *)(dynamic_cast<::android::graphics * >(pgraphics))->get_handle()) == NULL)
   //       return false;

   //::ReleaseDC(get_handle(), (dynamic_cast < ::android::graphics * > (pgraphics))->detach());

   //      (dynamic_cast<::android::graphics * >(pgraphics))->m_hdc = NULL;

   //      pgraphics->release();

   return true;

}

void interaction_impl::UpdateWindow()
{
   throw not_implemented(get_app());
   //::UpdateWindow(get_handle());
}

void interaction_impl::SetRedraw(bool bRedraw)
{
   throw not_implemented(get_app());
   //ASSERT(::IsWindow((oswindow) get_handle()));
   //::SendMessage(get_handle(), WM_SETREDRAW, bRedraw, 0);
}

bool interaction_impl::GetUpdateRect(LPRECT lpRect, bool bErase)
{
   throw not_implemented(get_app());
   //ASSERT(::IsWindow((oswindow) get_handle()));
   //return ::GetUpdateRect(get_handle(), lpRect, bErase) != FALSE;
}

int32_t interaction_impl::GetUpdateRgn(draw2d::region * pRgn, bool bErase)
{
   throw not_implemented(get_app());
   //ASSERT(::IsWindow((oswindow) get_handle()));
   //return ::GetUpdateRgn(get_handle(), (HRGN)pRgn->get_handle(), bErase);
}

void interaction_impl::Invalidate(bool bErase)
{
   throw not_implemented(get_app());
   //ASSERT(::IsWindow((oswindow) get_handle()));
   //::InvalidateRect(get_handle(), NULL, bErase);
}

void interaction_impl::InvalidateRect(LPCRECT lpRect, bool bErase)
{
   throw not_implemented(get_app());
   //ASSERT(::IsWindow((oswindow) get_handle()));
   //::InvalidateRect(get_handle(), lpRect, bErase);
}

void interaction_impl::InvalidateRgn(::draw2d::region* pRgn, bool bErase)
{
   throw not_implemented(get_app());
   //ASSERT(::IsWindow((oswindow) get_handle()));
   //::InvalidateRgn(get_handle(), (HRGN)pRgn->get_handle(), bErase);
}

void interaction_impl::ValidateRect(LPCRECT lpRect)
{
   throw not_implemented(get_app());
   //ASSERT(::IsWindow((oswindow) get_handle()));
   //::ValidateRect(get_handle(), lpRect);
}

void interaction_impl::ValidateRgn(::draw2d::region* pRgn)
{
   throw not_implemented(get_app());
   //ASSERT(::IsWindow((oswindow) get_handle()));
   //::ValidateRgn(get_handle(), (HRGN)pRgn->get_handle());
}

bool interaction_impl::IsWindowVisible()
{

   if (!::IsWindow((oswindow)get_handle()))
   {

      return false;

   }

   if (m_pui != NULL)
   {

      if (!m_pui->is_this_visible())
      {

         return false;

      }

      if (m_pui->GetParent() != NULL && !m_pui->GetParent()->IsWindowVisible())
         return false;

   }

   if (!::IsWindowVisible(get_handle()))
      return false;

   return true;

}


void interaction_impl::ShowOwnedPopups(bool bShow)
{

   throw not_implemented(get_app());
   //ASSERT(::IsWindow((oswindow) get_handle()));
   //::ShowOwnedPopups(get_handle(), bShow);

}

void interaction_impl::SendMessageToDescendants(UINT message, WPARAM wparam, lparam lparam, bool bDeep, bool bOnlyPerm)
{
   ASSERT(::IsWindow((oswindow)get_handle()));
   //interaction_impl::SendMessageToDescendants(get_handle(), message, wparam, lparam, bDeep, bOnlyPerm);

   // walk through oswindows to avoid creating temporary interaction_impl objects
   // unless we need to call this function recursively
   user::interaction * pui = m_pui->first_child();
   while (pui != NULL)
   {
      try
      {
         pui->send_message(message, wparam, lparam);
      }
      catch (...)
      {
      }
      if (bDeep)
      {
         // send to child windows after parent
         try
         {
            pui->SendMessageToDescendants(message, wparam, lparam, bDeep, bOnlyPerm);
         }
         catch (...)
         {
         }
      }
      try
      {
         pui = pui->next_sibling();
      }
      catch (...)
      {
         break;
      }
   }
}

::user::interaction * interaction_impl::GetDescendantWindow(id id) const
{
   ASSERT(::IsWindow((oswindow)get_handle()));
   return interaction_impl::GetDescendantWindow(m_pui, id);
}


::draw2d::graphics * interaction_impl::GetDCEx(::draw2d::region * prgnClip, DWORD flags)
{

   throw not_implemented(get_app());
   //ASSERT(::IsWindow((oswindow) get_handle()));
   //::draw2d::graphics_sp g(get_app());
   //g->attach(::GetDCEx(get_handle(), (HRGN)prgnClip->get_handle(), flags));
   //return g.detach();

}

bool interaction_impl::LockWindowUpdate()
{

   throw not_implemented(get_app());
   //ASSERT(::IsWindow((oswindow) get_handle()));
   //return ::LockWindowUpdate(get_handle()) != FALSE;

}

void interaction_impl::UnlockWindowUpdate()
{

   throw not_implemented(get_app());
   //ASSERT(::IsWindow((oswindow) get_handle()));
   //::LockWindowUpdate(NULL);

}

bool interaction_impl::RedrawWindow(LPCRECT lpRectUpdate, ::draw2d::region * prgnUpdate, UINT flags)
{

   if (!IsWindowVisible())
   {

      return true;

   }

   if (flags & RDW_UPDATENOW)
   {

      _001UpdateWindow();

   }
   else
   {

      m_pui->m_bRedraw = true;

   }

   return true;

}

/*
bool interaction_impl::EnableScrollBar(int32_t nSBFlags, UINT nArrowFlags)
{

ASSERT(::IsWindow((oswindow) get_handle()));

return ::EnableScrollBar(get_handle(), nSBFlags, nArrowFlags) != FALSE;

}
*/

bool interaction_impl::DrawAnimatedRects(int32_t idAni, CONST RECT *lprcFrom, CONST RECT *lprcTo)
{

   throw not_implemented(get_app());
   //ASSERT(::IsWindow((oswindow) get_handle()));
   //return ::DrawAnimatedRects(get_handle(), idAni, lprcFrom, lprcTo) != FALSE;

}

bool interaction_impl::DrawCaption(::draw2d::graphics * pgraphics, LPCRECT lprc, UINT uFlags)
{

   throw not_implemented(get_app());
   //ASSERT(::IsWindow((oswindow) get_handle()));
   //return ::DrawCaption(get_handle(), (HDC)(dynamic_cast<::android::graphics * >(pgraphics))->get_handle(), lprc, uFlags) != FALSE;

}

bool interaction_impl::SetTimer(uint_ptr nIDEvent, UINT nElapse, PFN_TIMER pfnTimer)
{

   return ::user::interaction_impl::SetTimer(nIDEvent, nElapse, pfnTimer);

   //return ::user::interaction_impl::SetTimer(nIDEvent, nElapse, lpfnTimer);


   //UNREFERENCED_PARAMETER(lpfnTimer);

   //m_pui->m_pauraapp->set_timer(m_pui, nIDEvent, nElapse);

   //return nIDEvent;

   //throw not_implemented(get_app());
   //ASSERT(::IsWindow((oswindow) get_handle()));
   //return ::SetTimer(get_handle(), nIDEvent, nElapse, lpfnTimer);

}


bool interaction_impl::KillTimer(uint_ptr nIDEvent)
{

   return ::user::interaction_impl::KillTimer(nIDEvent);

   //m_pui->m_pauraapp->unset_timer(m_pui, nIDEvent);

   //return TRUE;

   //ASSERT(::IsWindow((oswindow) get_handle()));
   //return ::KillTimer(get_handle(), nIDEvent)  != FALSE;

}

bool interaction_impl::IsWindowEnabled()
{

   return m_bEnabled;

}


bool interaction_impl::EnableWindow(bool bEnable)
{

   return m_bEnabled = bEnable;

}

::user::interaction * interaction_impl::GetActiveWindow()
{

   return ::android::interaction_impl::ui_from_handle(::GetActiveWindow());

}

::user::interaction * interaction_impl::SetActiveWindow()
{

   return ::android::interaction_impl::ui_from_handle(::SetActiveWindow(get_handle()));

}


//::user::interaction * interaction_impl::SetCapture(::user::interaction * pinterface)
//{
//
//
//}

::user::interaction * PASCAL interaction_impl::GetFocus()
{

   oswindow w = ::GetFocus();

   if (!::IsWindow(w))
      return NULL;

   return w->get_user_interaction();

}

bool interaction_impl::SetFocus()
{

   ASSERT(::IsWindow((oswindow)get_handle()));

   oswindow w = ::SetFocus(get_handle());

   if (!::IsWindow(w))
      return NULL;

   return w->get_user_interaction();

}

::user::interaction * PASCAL interaction_impl::GetDesktopWindow()
{
   /*
   return ::android::interaction_impl::from_handle(::GetDesktopWindow());
   */
   return NULL;
}


// Helper for radio buttons
int32_t interaction_impl::GetCheckedRadioButton(int32_t nIDFirstButton, int32_t nIDLastButton)
{
   for (int32_t nID = nIDFirstButton; nID <= nIDLastButton; nID++)
   {
      if (IsDlgButtonChecked(nID))
         return nID; // id that matched
   }
   return 0; // invalid ID
}

void interaction_impl::CheckDlgButton(int32_t nIDButton, UINT nCheck)
{

   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      ::CheckDlgButton(get_handle(), nIDButton, nCheck);

}

void interaction_impl::CheckRadioButton(int32_t nIDFirstButton, int32_t nIDLastButton, int32_t nIDCheckButton)
{

   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      ::CheckRadioButton(get_handle(), nIDFirstButton, nIDLastButton, nIDCheckButton);

}

int32_t interaction_impl::DlgDirList(LPTSTR lpPathSpec, int32_t nIDListBox, int32_t nIDStaticPath, UINT nFileType)
{

   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      return ::DlgDirList(get_handle(), lpPathSpec, nIDListBox, nIDStaticPath, nFileType);

}

int32_t interaction_impl::DlgDirListComboBox(LPTSTR lpPathSpec, int32_t nIDComboBox, int32_t nIDStaticPath, UINT nFileType)
{

   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      return ::DlgDirListComboBox(get_handle(), lpPathSpec, nIDComboBox, nIDStaticPath, nFileType);

}

bool interaction_impl::DlgDirSelect(LPTSTR lpString, int32_t nSize, int32_t nIDListBox)
{

   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      return ::DlgDirSelectEx(get_handle(), lpString, nSize, nIDListBox) != FALSE;

}

bool interaction_impl::DlgDirSelectComboBox(LPTSTR lpString, int32_t nSize, int32_t nIDComboBox)
{

   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      return ::DlgDirSelectComboBoxEx(get_handle(), lpString, nSize, nIDComboBox) != FALSE;

}

/*
void interaction_impl::GetDlgItem(id id, oswindow* phWnd) const
{

ASSERT(::IsWindow((oswindow) get_handle()));
ASSERT(phWnd != NULL);
*phWnd = ::GetDlgItem(get_handle(), (int32_t) id);

}
*/

/*
UINT interaction_impl::GetDlgItemInt(int32_t nID, WINBOOL * lpTrans, bool bSigned) const
{

ASSERT(::IsWindow((oswindow) get_handle()));

return ::GetDlgItemInt(get_handle(), nID, lpTrans, bSigned);

}
*/

//   int32_t interaction_impl::GetDlgItemText(int32_t nID, LPTSTR lpStr, int32_t nMaxCount) const
//   {
//
//      throw not_implemented(get_app());
//      ASSERT(::IsWindow((oswindow) get_handle())); return ::GetDlgItemText(get_handle(), nID, lpStr, nMaxCount);}

::user::interaction * interaction_impl::GetNextDlgGroupItem(::user::interaction * pWndCtl, bool bPrevious) const
{

   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      return ::android::interaction_impl::from_handle(::GetNextDlgGroupItem(get_handle(), (oswindow) pWndCtl->get_handle(), bPrevious));

}

::user::interaction * interaction_impl::GetNextDlgTabItem(::user::interaction * pWndCtl, bool bPrevious) const
{

   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      return ::android::interaction_impl::from_handle(::GetNextDlgTabItem(get_handle(), (oswindow) pWndCtl->get_handle(), bPrevious));

}

UINT interaction_impl::IsDlgButtonChecked(int32_t nIDButton) const
{

   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      return ::IsDlgButtonChecked(get_handle(), nIDButton);

}

LPARAM interaction_impl::SendDlgItemMessage(int32_t nID, UINT message, WPARAM wparam, LPARAM lparam)
{

   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      return ::SendDlgItemMessage(get_handle(), nID, message, wparam, lparam);

}

void interaction_impl::SetDlgItemInt(int32_t nID, UINT nValue, bool bSigned)
{

   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      ::SetDlgItemInt(get_handle(), nID, nValue, bSigned);

}

void interaction_impl::SetDlgItemText(int32_t nID, const char * lpszString)
{

   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      ::SetDlgItemText(get_handle(), nID, lpszString);

}

int32_t interaction_impl::ScrollWindowEx(int32_t dx, int32_t dy, LPCRECT lpRectScroll, LPCRECT lpRectClip, ::draw2d::region * prgnUpdate, LPRECT lpRectUpdate, UINT flags)
{

   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      return ::ScrollWindowEx(get_handle(), dx, dy, lpRectScroll, lpRectClip, (HRGN)prgnUpdate->get_handle(), lpRectUpdate, flags);

}

void interaction_impl::ShowScrollBar(UINT nBar, bool bShow)
{

   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      ::ShowScrollBar(get_handle(), nBar, bShow);

}

::user::interaction * interaction_impl::ChildWindowFromPoint(POINT point)
{


   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      return ::android::interaction_impl::from_handle(::ChildWindowFromPoint(get_handle(), point));

}

::user::interaction * interaction_impl::ChildWindowFromPoint(POINT point, UINT nFlags)
{

   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      return ::android::interaction_impl::from_handle(::ChildWindowFromPointEx(get_handle(), point, nFlags));

}

::user::interaction * PASCAL interaction_impl::FindWindow(const char * lpszClassName, const char * lpszWindowName)
{

   //      throw not_implemented(get_app());
   //      return ::android::interaction_impl::from_handle(::FindWindow(lpszClassName, lpszWindowName));
   return NULL;

}

::user::interaction * interaction_impl::FindWindowEx(oswindow hwndParent, oswindow hwndChildAfter, const char * lpszClass, const char * lpszWindow)
{

   throw not_implemented(::get_thread_app());
   //      return ::android::interaction_impl::from_handle(::FindWindowEx(hwndParent, hwndChildAfter, lpszClass, lpszWindow));

}




::user::interaction * interaction_impl::GetTopWindow() const
{

   if (m_pui->m_uiptraChild.get_size() <= 0)
      return NULL;

   return m_pui->m_uiptraChild[0];
   //  throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      return ::android::interaction_impl::from_handle(::GetTopWindow(get_handle()));

}

::user::interaction * interaction_impl::GetWindow(UINT nCmd)
{

   ASSERT(::IsWindow((oswindow)get_handle()));
   //      return ::android::interaction_impl::from_handle(::GetWindow(get_handle(), nCmd));
   return NULL;

}

::user::interaction * interaction_impl::GetLastActivePopup()
{


   throw todo(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      return ::android::interaction_impl::from_handle(::GetLastActivePopup(get_handle()));

}


::user::interaction * interaction_impl::SetParent(::user::interaction * pWndNewParent)
{

   ASSERT(::IsWindow((oswindow)get_handle()));

   return ::android::interaction_impl::ui_from_handle(::SetParent(get_handle(), (oswindow)pWndNewParent->get_handle()));

}


::user::interaction * PASCAL interaction_impl::oswindowFromPoint(POINT point)
{


   throw not_implemented(::get_thread_app());
   //      return ::android::interaction_impl::from_handle(::oswindowFromPoint(point));

}

bool interaction_impl::FlashWindow(bool bInvert)
{


   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      return ::FlashWindow(get_handle(), bInvert) != FALSE;

}

bool interaction_impl::ChangeClipboardChain(oswindow hWndNext)
{

   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      return ::ChangeClipboardChain(get_handle(), hWndNext) != FALSE;

}

oswindow interaction_impl::SetClipboardViewer()
{

   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      return ::SetClipboardViewer(get_handle());

}

bool interaction_impl::OpenClipboard()
{

   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      return ::OpenClipboard(get_handle()) != FALSE;

}

::user::interaction * PASCAL interaction_impl::GetOpenClipboardWindow()
{

   throw not_implemented(::get_thread_app());
   //      return ::android::interaction_impl::from_handle(::GetOpenClipboardWindow());

}

::user::interaction * PASCAL interaction_impl::GetClipboardOwner()
{

   throw not_implemented(::get_thread_app());
   //      return ::android::interaction_impl::from_handle(::GetClipboardOwner());

}

::user::interaction * PASCAL interaction_impl::GetClipboardViewer()
{

   throw not_implemented(::get_thread_app());
   //      return ::android::interaction_impl::from_handle(::GetClipboardViewer());

}

void interaction_impl::CreateCaret(::draw2d::bitmap* pBitmap)
{

   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      ::CreateCaret(get_handle(), (HBITMAP)pBitmap->get_handle(), 0, 0);

}

void interaction_impl::CreateSolidCaret(int32_t nWidth, int32_t nHeight)
{

   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      ::CreateCaret(get_handle(), (HBITMAP)0, nWidth, nHeight);

}

void interaction_impl::CreateGrayCaret(int32_t nWidth, int32_t nHeight)
{

   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      ::CreateCaret(get_handle(), (HBITMAP)1, nWidth, nHeight);

}

point PASCAL interaction_impl::GetCaretPos()
{

   throw not_implemented(::get_thread_app());
   //      point point;
   //      ::GetCaretPos((LPPOINT)&point); return point;

}

void PASCAL interaction_impl::SetCaretPos(POINT point)
{

   throw not_implemented(::get_thread_app());
   //      ::SetCaretPos(point.x, point.y);

}

void interaction_impl::HideCaret()
{

   throw not_implemented(get_app());
   //      ::HideCaret(get_handle());

}

void interaction_impl::ShowCaret()
{

   throw not_implemented(get_app());
   //    ::ShowCaret(get_handle());

}

bool interaction_impl::SetForegroundWindow()
{

   //    throw not_implemented(get_app());
   //      return ::SetForegroundWindow(get_handle()) != FALSE;

   return false;

}

::user::interaction * PASCAL interaction_impl::GetForegroundWindow()
{

   return NULL;

   throw not_implemented(::get_thread_app());
   //      return ::android::interaction_impl::from_handle(::GetForegroundWindow());

}

bool interaction_impl::SendNotifyMessage(UINT message, WPARAM wparam, lparam lparam)
{

   throw not_implemented(get_app());
   //      return ::SendNotifyMessage(get_handle(), message, wparam, lparam) != FALSE;

}

void interaction_impl::Print(::draw2d::graphics * pgraphics, DWORD dwFlags) const
{

   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      const_cast < interaction_impl * > (this)->send_message(WM_PRINT, (WPARAM)(dynamic_cast<::android::graphics * >(pgraphics))->get_handle(), dwFlags);

}

void interaction_impl::PrintClient(::draw2d::graphics * pgraphics, DWORD dwFlags) const
{

   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      const_cast < interaction_impl * > (this)->send_message(WM_PRINTCLIENT, (WPARAM)(dynamic_cast<::android::graphics * >(pgraphics))->get_handle(), dwFlags);

}

bool interaction_impl::SetWindowContextHelpId(DWORD dwContextHelpId)
{

   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      return ::SetWindowContextHelpId(get_handle(), dwContextHelpId) != FALSE;

}

DWORD interaction_impl::GetWindowContextHelpId() const
{

   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      return ::GetWindowContextHelpId(get_handle());

}


// Default message ::collection::map implementations
void interaction_impl::OnActivateApp(bool, DWORD)
{
   Default();
}
void interaction_impl::OnActivate(UINT, ::user::interaction *, bool)
{
   Default();
}
void interaction_impl::OnCancelMode()
{
   Default();
}
void interaction_impl::OnChildActivate()
{
   Default();
}
void interaction_impl::OnClose()
{
   Default();
}
void interaction_impl::OnContextMenu(::user::interaction *, point)
{
   Default();
}

bool interaction_impl::OnCopyData(::user::interaction *, COPYDATASTRUCT*)
{

   return Default() != FALSE;

}

void interaction_impl::OnEnable(bool)
{
   Default();
}
void interaction_impl::OnEndSession(bool)
{
   Default();
}

bool interaction_impl::OnEraseBkgnd(::draw2d::graphics *)
{

   return Default() != FALSE;

}

void interaction_impl::OnGetMinMaxInfo(MINMAXINFO*)
{
   Default();
}
void interaction_impl::OnIconEraseBkgnd(::draw2d::graphics *)
{
   Default();
}
void interaction_impl::OnKillFocus(::user::interaction *)
{
   Default();
}
LRESULT interaction_impl::OnMenuChar(UINT, UINT, ::user::menu*)
{
   return Default();
}
void interaction_impl::OnMenuSelect(UINT, UINT, HMENU)
{
   Default();
}
void interaction_impl::OnMove(int32_t, int32_t)
{
   Default();
}

bool interaction_impl::OnQueryEndSession()
{

   return Default() != FALSE;

}

bool interaction_impl::OnQueryNewPalette()
{

   return Default() != FALSE;

}

bool interaction_impl::OnQueryOpen()
{

   return Default() != FALSE;

}

void interaction_impl::_001OnSetCursor(::signal_details * pobj)
{
   SCAST_PTR(::message::base, pbase, pobj);
   if (Session.get_cursor() != NULL
      && Session.get_cursor()->m_ecursor != ::visual::cursor_system)
   {

      throw not_implemented(get_app());
      //         ::SetCursor(NULL);
   }
   pbase->set_lresult(1);
   pbase->m_bRet = true;
   //(bool)Default();
}
void interaction_impl::OnShowWindow(bool, UINT)
{
   Default();
}
void interaction_impl::OnSize(UINT, int32_t, int32_t)
{
   Default();
}
void interaction_impl::OnTCard(UINT, DWORD)
{
   Default();
}
void interaction_impl::OnWindowPosChanging(WINDOWPOS*)
{
   Default();
}
void interaction_impl::OnWindowPosChanged(WINDOWPOS*)
{
   Default();
}
void interaction_impl::OnDropFiles(HDROP)
{
   Default();
}
void interaction_impl::OnPaletteIsChanging(::user::interaction *)
{
   Default();
}

bool interaction_impl::OnNcActivate(bool)
{

   return Default() != FALSE;

}

void interaction_impl::OnNcCalcSize(bool, NCCALCSIZE_PARAMS*)
{
   Default();
}

bool interaction_impl::OnNcCreate(::user::create_struct *)
{

   return Default() != FALSE;

}

LRESULT interaction_impl::OnNcHitTest(point)
{
   return Default();
}
void interaction_impl::OnNcLButtonDblClk(UINT, point)
{
   Default();
}
void interaction_impl::OnNcLButtonDown(UINT, point)
{
   Default();
}
void interaction_impl::OnNcLButtonUp(UINT, point)
{
   Default();
}
void interaction_impl::OnNcMButtonDblClk(UINT, point)
{
   Default();
}
void interaction_impl::OnNcMButtonDown(UINT, point)
{
   Default();
}
void interaction_impl::OnNcMButtonUp(UINT, point)
{
   Default();
}
void interaction_impl::OnNcMouseMove(UINT, point)
{
   Default();
}
void interaction_impl::OnNcPaint()
{
   Default();
}
void interaction_impl::OnNcRButtonDblClk(UINT, point)
{
   Default();
}
void interaction_impl::OnNcRButtonDown(UINT, point)
{
   Default();
}
void interaction_impl::OnNcRButtonUp(UINT, point)
{
   Default();
}
void interaction_impl::OnSysChar(UINT, UINT, UINT)
{
   Default();
}
void interaction_impl::OnSysCommand(UINT, LPARAM)
{
   Default();
}
void interaction_impl::OnSysDeadChar(UINT, UINT, UINT)
{
   Default();
}
void interaction_impl::OnSysKeyDown(UINT, UINT, UINT)
{
   Default();
}
void interaction_impl::OnSysKeyUp(UINT, UINT, UINT)
{
   Default();
}
void interaction_impl::OnCompacting(UINT)
{
   Default();
}
void interaction_impl::OnFontChange()
{
   Default();
}
void interaction_impl::OnPaletteChanged(::user::interaction *)
{
   Default();
}
void interaction_impl::OnSpoolerStatus(UINT, UINT)
{
   Default();
}
void interaction_impl::OnTimeChange()
{
   Default();
}
void interaction_impl::OnChar(UINT, UINT, UINT)
{
   Default();
}
void interaction_impl::OnDeadChar(UINT, UINT, UINT)
{
   Default();
}
void interaction_impl::OnKeyDown(UINT, UINT, UINT)
{
   Default();
}
void interaction_impl::OnKeyUp(UINT, UINT, UINT)
{
   Default();
}
void interaction_impl::OnLButtonDblClk(UINT, point)
{
   Default();
}
void interaction_impl::OnLButtonDown(UINT, point)
{
   Default();
}
void interaction_impl::OnLButtonUp(UINT, point)
{
   Default();
}
void interaction_impl::OnMButtonDblClk(UINT, point)
{
   Default();
}
void interaction_impl::OnMButtonDown(UINT, point)
{
   Default();
}
void interaction_impl::OnMButtonUp(UINT, point)
{
   Default();
}
int32_t interaction_impl::OnMouseActivate(::user::interaction *, UINT, UINT)
{
   return (int32_t)Default();
}
void interaction_impl::OnMouseMove(UINT, point)
{
   Default();
}

bool interaction_impl::OnMouseWheel(UINT, short, point)
{

   return Default() != FALSE;

}

LRESULT interaction_impl::OnRegisteredMouseWheel(WPARAM, LPARAM)
{
   return Default();
}
void interaction_impl::OnRButtonDblClk(UINT, point)
{
   Default();
}
void interaction_impl::OnRButtonDown(UINT, point)
{
   Default();
}
void interaction_impl::OnRButtonUp(UINT, point)
{
   Default();
}
void interaction_impl::OnTimer(uint_ptr)
{
   Default();
}
void interaction_impl::OnInitMenu(::user::menu*)
{
   Default();
}
void interaction_impl::OnInitMenuPopup(::user::menu*, UINT, bool)
{
   Default();
}
void interaction_impl::OnAskCbFormatName(UINT nMaxCount, LPTSTR pszName)
{
   (nMaxCount);
   if (nMaxCount>0)
   {
      /* defwindow proc should do this for us, but to be safe, we'll do it here too */
      pszName[0] = '\0';
   }
   Default();
}
void interaction_impl::OnChangeCbChain(oswindow, oswindow)
{
   Default();
}
void interaction_impl::OnDestroyClipboard()
{
   Default();
}
void interaction_impl::OnDrawClipboard()
{
   Default();
}
void interaction_impl::OnHScrollClipboard(::user::interaction *, UINT, UINT)
{
   Default();
}
void interaction_impl::OnPaintClipboard(::user::interaction *, HGLOBAL)
{
   Default();
}
void interaction_impl::OnRenderAllFormats()
{
   Default();
}
void interaction_impl::OnRenderFormat(UINT)
{
   Default();
}
void interaction_impl::OnSizeClipboard(::user::interaction *, HGLOBAL)
{
   Default();
}
void interaction_impl::OnVScrollClipboard(::user::interaction *, UINT, UINT)
{
   Default();
}
UINT interaction_impl::OnGetDlgCode()
{
   return (UINT)Default();
}
void interaction_impl::OnMDIActivate(bool, ::user::interaction *, ::user::interaction *)
{
   Default();
}
void interaction_impl::OnEnterMenuLoop(bool)
{
   Default();
}
void interaction_impl::OnExitMenuLoop(bool)
{
   Default();
}
// Win4 support
//   void interaction_impl::OnStyleChanged(int32_t, LPSTYLESTRUCT)
//   { Default(); }
//   void interaction_impl::OnStyleChanging(int32_t, LPSTYLESTRUCT)
//   { Default(); }
void interaction_impl::OnSizing(UINT, LPRECT)
{
   Default();
}
void interaction_impl::OnMoving(UINT, LPRECT)
{
   Default();
}
void interaction_impl::OnCaptureChanged(::user::interaction *)
{
   Default();
}

bool interaction_impl::OnDeviceChange(UINT, dword_ptr)
{

   return Default() != FALSE;

}

void interaction_impl::OnWinIniChange(const char *)
{
   Default();
}
void interaction_impl::OnChangeUIState(UINT, UINT)
{
   Default();
}
void interaction_impl::OnUpdateUIState(UINT, UINT)
{
   Default();
}

UINT interaction_impl::OnQueryUIState()
{

   return (UINT)Default();

}

// interaction_impl dialog data support
//    void interaction_impl::DoDataExchange(CDataExchange*)
//   { } // default does nothing

// interaction_impl modality support

void interaction_impl::BeginModalState()
{

   throw todo(get_app());

   //::EnableWindow(get_handle(), FALSE);

}

void interaction_impl::EndModalState()
{

   throw todo(get_app());

   //::EnableWindow(get_handle(), TRUE);

}

// frame_window
/*    void frame_window::DelayUpdateFrameTitle()
{ m_nIdleFlags |= idleTitle; }
void frame_window::DelayRecalcLayout(bool bNotify)
{ m_nIdleFlags |= (idleLayout | (bNotify ? idleNotify : 0)); };
bool frame_window::InModalState() const
{ return m_cModalStack != 0; }
void frame_window::set_title(const char * lpszTitle)
{ m_strTitle = lpszTitle; }
string frame_window::get_title() const
{ return m_strTitle; }
*/



void interaction_impl::CloseWindow()
{

   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      ::CloseWindow(get_handle());

}

bool interaction_impl::OpenIcon()
{

   throw not_implemented(get_app());
   //      ASSERT(::IsWindow((oswindow) get_handle()));
   //      return ::OpenIcon(get_handle()) != FALSE;

}

////////////////////////////////////////////////////////////////////////////
// UI related ::user::interaction_impl functions

oswindow PASCAL interaction_impl::GetSafeOwner_(oswindow hParent, oswindow* pWndTop)
{
   // get ::user::interaction_impl to start with
   oswindow hWnd = hParent;
   if (hWnd == NULL)
   {
      /* trans      sp(frame_window) pFrame = command_target::GetRoutingFrame_();
      if (pFrame != NULL)
      hWnd = pFrame->get_handle();
      else
      hWnd = System.GetMainWnd()->get_handle();*/
   }

   // a popup ::user::interaction_impl cannot be owned by a child ::user::interaction_impl
   while (hWnd != NULL && (::GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD))
      hWnd = ::GetParent(hWnd);

   // determine toplevel ::user::interaction_impl to disable as well
   oswindow hWndTop = hWnd, hWndTemp = hWnd;
   for (;;)
   {
      if (hWndTemp == NULL)
         break;
      else
         hWndTop = hWndTemp;
      hWndTemp = ::GetParent(hWndTop);
   }

   throw todo(::get_thread_app());
   // get last active popup of first non-child that was found
   //    if (hParent == NULL && hWnd != NULL)
   //       hWnd = ::GetLastActivePopup(hWnd);

   // disable and store top level parent ::user::interaction_impl if specified
   if (pWndTop != NULL)
   {
      /*         if (hWndTop != NULL && ::IsWindowEnabled(hWndTop) && hWndTop != hWnd)
      {
      *pWndTop = hWndTop;
      ::EnableWindow(hWndTop, FALSE);
      }
      else
      *pWndTop = NULL;*/
   }

   return hWnd;    // return the owner as oswindow
}

//void interaction_impl::on_delete(::object * pui)
//{
//   UNREFERENCED_PARAMETER(pui);
//}


//
//
//void interaction_impl::_001OnEraseBkgnd(::signal_details * pobj)
//{
//   SCAST_PTR(::message::erase_bkgnd, perasebkgnd, pobj);
//   perasebkgnd->m_bRet = true;
//   perasebkgnd->set_result(TRUE);
//}

void interaction_impl::_001BaseWndInterfaceMap()
{
   Session.user()->window_map().set((int_ptr)get_handle(), this);
}


void interaction_impl::_001OnTriggerMouseInside()
{


   ::user::interaction_impl::_001OnTriggerMouseInside();

}


} // namespace android



  /////////////////////////////////////////////////////////////////////////////
  // The WndProc for all interaction_impl's and derived classes











namespace android
{




   void interaction_impl::set_viewport_org(::draw2d::graphics * pgraphics)
   {
      // graphics will be already set its view port to the interaction_impl for android - cairo with xlib

      pgraphics->SetViewportOrg(point(0, 0));

      /*      rect64 rectWindow;
      GetWindowRect(rectWindow);
      get_wnd()->ScreenToClient(rectWindow);
      pgraphics->SetViewportOrg(point(rectWindow.top_left()));
      pgraphics->SelectClipRgn(NULL);
      */

   }


   ::user::interaction * interaction_impl::GetNextWindow(UINT nFlag)
   {

      if (nFlag == GW_HWNDNEXT)
      {

         return get_next(true, NULL);

      }
      else
      {

         throw interface_only_exception(get_app());

      }

   }


   ::user::interaction * interaction_impl::get_next(bool bIgnoreChildren, int32_t * piLevel)
   {

      return  m_pui->get_next(bIgnoreChildren, piLevel);

   }


   bool interaction_impl::on_keyboard_focus(::user::elemental * pfocus)
   {

      UNREFERENCED_PARAMETER(pfocus);

      System.m_pandroidinitdata->m_bShowKeyboard = true;

      return true;

   }


   bool interaction_impl::keyboard_focus_OnKillFocus()
   {

      output_debug_string("::android::interaction_impl::keyboard_focus_OnKillFocus() (1) \n");

      System.m_pandroidinitdata->m_bHideKeyboard = true;

      return true;

   }

   bool interaction_impl::keyboard_focus_OnChildKillFocus()
   {

      output_debug_string("::android::interaction_impl::keyboard_focus_OnChildKillFocus() (2) \n");

      System.m_pandroidinitdata->m_bHideKeyboard = true;

      return true;

   }


   ::user::interaction_impl * interaction_impl::from_handle(oswindow oswindow)
   {

      return ::window_from_handle(oswindow);

   }


   ::user::interaction * interaction_impl::ui_from_handle(oswindow oswindow)
   {

      if (oswindow == NULL)
      {

         return NULL;

      }

      return oswindow->get_user_interaction();

   }

   bool interaction_impl::has_pending_graphical_update()
   {

      synch_lock sl(m_pui->m_pmutex);

      for (auto p : m_pui->m_uiptraChild)
      {

         if (p->has_pending_graphical_update())
         {

            return true;

         }

      }


      return false;

   }


} // namespace android



