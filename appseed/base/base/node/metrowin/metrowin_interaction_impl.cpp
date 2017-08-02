#include "framework.h"
//#include "base/user/user.h"
#include "metrowin.h"

using namespace Windows::UI::Core;
using namespace Windows::Foundation;
using namespace Microsoft::WRL;
using namespace Windows::Graphics::Display;
using namespace D2D1;

static_function void __pre_init_dialog(::user::interaction * pWnd,LPRECT lpRectOld,uint32_t* pdwStyleOld);
static_function void __post_init_dialog(::user::interaction * pWnd,const RECT& rectOld,uint32_t dwStyleOld);
LRESULT CALLBACK __activation_window_procedure(oswindow hWnd,UINT nMsg,WPARAM wParam,LPARAM lParam);


namespace metrowin
{


   interaction_impl::interaction_impl() :
      ::aura::timer_array(get_app())
   {

      m_bScreenRelativeMouseMessagePosition  = false;
      m_plistener                            = NULL;
      m_nModalResult                         = 0;
//      m_bMouseHover                          = false;
      m_pfont                                = NULL;
      m_pguieCapture                         = NULL;
      m_pwindow                              = new ::user::native_window;

   }


   void interaction_impl::construct(oswindow hWnd)
   {

      m_bScreenRelativeMouseMessagePosition  = false;
      m_plistener = NULL;
      m_nModalResult = 0;
     // m_bMouseHover = false;
      m_pfont = NULL;
      m_pguieCapture = NULL;
      m_pwindow = NULL;

   }

   interaction_impl::interaction_impl(::aura::application * papp):
      ::object(papp),
      ::aura::timer_array(papp)
   {

      m_bScreenRelativeMouseMessagePosition  = false;
      m_plistener = NULL;
      m_nModalResult = 0;
     // m_bMouseHover = false;
      m_pfont = NULL;
      m_pguieCapture = NULL;
      m_pwindow = new ::user::native_window;

   }


   /*   ::user::interaction_impl * interaction_impl::from_os_data(void * pdata)
      {
      return dynamic_cast <::user::interaction_impl *>(from_handle((oswindow) pdata));
      }

      void * interaction_impl::get_os_data() const
      {
      //      return hwnd_handle::get_handle();
      return oswindow(m_pui);
      }

      */

   // Change a interaction_impl's style

   static_function bool __modify_style(oswindow hWnd,int nStyleOffset,uint32_t dwRemove,uint32_t dwAdd,UINT nFlags)
   {

#ifdef WINDOWSEX

      ASSERT(hWnd != NULL);
      uint32_t dwStyle = ::GetWindowLong(hWnd, nStyleOffset);
      uint32_t dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
      if (dwStyle == dwNewStyle)
         return FALSE;

      ::SetWindowLong(hWnd, nStyleOffset, dwNewStyle);
      if (nFlags != 0)
      {
         ::SetWindowPos(hWnd, NULL, 0, 0, 0, 0,
                        SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
      }

#else

      throw todo(::get_thread_app());

#endif

      return TRUE;

   }

   bool interaction_impl::ModifyStyle(oswindow hWnd,uint32_t dwRemove,uint32_t dwAdd,UINT nFlags)
   {
      return __modify_style(hWnd,GWL_STYLE,dwRemove,dwAdd,nFlags);
   }

   bool interaction_impl::ModifyStyleEx(oswindow hWnd,uint32_t dwRemove,uint32_t dwAdd,UINT nFlags)
   {
      return __modify_style(hWnd,GWL_EXSTYLE,dwRemove,dwAdd,nFlags);
   }


   const MSG* interaction_impl::GetCurrentMessage()
   {

      return NULL;

   }


   LRESULT interaction_impl::Default()
   {

      return 0;

   }


   void interaction_impl::pre_subclass_window()
   {

   }


   bool interaction_impl::CreateEx(uint32_t dwExStyle,const char * lpszClassName,
                                   const char * lpszWindowName,uint32_t dwStyle,
                                   const RECT& rect,::user::interaction* pParentWnd,id id,
                                   LPVOID lpParam /* = NULL */)
   {
      return CreateEx(dwExStyle,lpszClassName,lpszWindowName,dwStyle,
                      rect.left,rect.top,rect.right - rect.left,rect.bottom - rect.top,
                      pParentWnd->get_safe_handle(),id,lpParam);
   }

   bool interaction_impl::CreateEx(uint32_t dwExStyle,const char * lpszClassName,
                                   const char * lpszWindowName,uint32_t dwStyle,
                                   int x,int y,int nWidth,int nHeight,
                                   oswindow hWndParent,id id,LPVOID lpParam)
   {
      UNREFERENCED_PARAMETER(id);
      //ASSERT(lpszClassName == NULL || __is_valid_string(lpszClassName) || __is_valid_atom(lpszClassName));
      ASSERT(lpszClassName == NULL || __is_valid_string(lpszClassName));
      ENSURE_ARG(lpszWindowName == NULL || __is_valid_string(lpszWindowName));

      ::rect rect;

      rect.left = x;
      rect.top = y;
      rect.right = x + nWidth;
      rect.bottom = y + nHeight;


      // allow modification of several common create parameters
      ::user::create_struct cs;
      cs.dwExStyle = dwExStyle;
#ifdef WINDOWSEX
      cs.lpszClass = lpszClassName;
      cs.lpszName = lpszWindowName;
#else
      throw todo(get_app());
#endif
      cs.style = dwStyle;
      cs.x = x;
      cs.y = y;
      cs.cx = nWidth;
      cs.cy = nHeight;
#ifdef WINDOWSEX
      cs.hwndParent = hWndParent;
#else
      throw todo(get_app());
#endif
      //   cs.hMenu = hWndParent == NULL ? NULL : nIDorHMenu;
      cs.hMenu = NULL;
      //      cs.hInstance = System.m_hInstance;
      cs.lpCreateParams = lpParam;


      if(IsWindow())
      {
         DestroyWindow();
      }

      cs.dwExStyle   = dwExStyle;
      cs.style       = dwStyle;
      cs.x           = rect.left;
      cs.y           = rect.top;
      cs.cx          = rect.right - rect.left;
      cs.cy          = rect.bottom - rect.top;


      cs.lpszClass         = NULL;
      cs.lpszName          = NULL;
      cs.hwndParent        = NULL;
      cs.hMenu = NULL;

      cs.hInstance = NULL;


      cs.lpCreateParams = lpParam;

      if(m_pui != NULL)
      {
         if(!m_pui->pre_create_window(cs))
         {
            PostNcDestroy();
            return FALSE;
         }
      }
      else
      {
         if(!pre_create_window(cs))
         {
            PostNcDestroy();
            return FALSE;
         }
      }


      set_window_long(GWL_STYLE, cs.style);

      set_window_long(GWL_EXSTYLE, cs.dwExStyle);

      //m_pui->install_message_handling(dynamic_cast < ::message::dispatch * > (this));

      send_message(WM_CREATE,0,(LPARAM)&cs);

      m_pui->SetWindowPos(0,rect.left,rect.top,cs.cx,cs.cy,0);

      send_message(WM_SIZE);

      //   on_set_parent(pparent);

      m_oswindow = oswindow_get(this);

      return true;

#ifdef WINDOWSEX
      if(cs.hwndParent == NULL)
      {
         cs.style &= ~WS_CHILD;
      }

      hook_window_create(this);
      oswindow hWnd = ::CreateWindowEx(cs.dwExStyle, cs.lpszClass,
                                       cs.lpszName, cs.style, cs.x, cs.y, cs.cx, cs.cy,
                                       cs.hwndParent, cs.hMenu, cs.hInstance, cs.lpCreateParams);

#ifdef DEBUG
      if (hWnd == NULL)
      {
         uint32_t dwLastError = GetLastError();
         string strLastError = FormatMessageFromSystem(dwLastError);
         string strMessage;
         strMessage.Format("%s\n\nSystem Error Code: %d", strLastError, dwLastError);

         TRACE(::core::trace::category_AppMsg, 0, "Warning: Window creation failed: GetLastError returned:\n");
         TRACE(::core::trace::category_AppMsg, 0, "%s\n", strMessage);
         try
         {
            if(dwLastError == 0x0000057e)
            {
               System.simple_message_box(NULL, "Cannot create a top-level child interaction_impl.");
            }
            else
            {
               System.simple_message_box(NULL, strMessage);
            }
         }
         catch(...)
         {
         }
      }
#endif

      if (!unhook_window_create())
         PostNcDestroy();        // cleanup if CreateWindowEx fails too soon

      if (hWnd == NULL)
         return FALSE;
      WNDCLASS wndcls;
      if(lpszClassName != NULL &&
            GetClassInfo(System.m_hInstance, lpszClassName, &wndcls) &&
            wndcls.hIcon != NULL)
      {
         m_pui->set_icon(new ::visual::icon(wndcls.hIcon), false);
         m_pui->set_icon(new ::visual::icon(wndcls.hIcon), true);
      }
      //      oswindow hwndHandle = get_handle();
      if(hWnd != get_handle())
      {
         ASSERT(FALSE); // should have been set in send msg hook
      }
#else
      throw todo(get_app());
#endif

      return TRUE;
   }

   // for child windows
   bool interaction_impl::pre_create_window(::user::create_struct& cs)
   {
#ifdef WINDOWSEX
      if (cs.lpszClass == NULL)
      {
         // make sure the default interaction_impl class is registered
         VERIFY(__end_defer_register_class(__WND_REG, &cs.lpszClass));

         // no WNDCLASS provided - use child interaction_impl default
         ASSERT(cs.style & WS_CHILD);
      }
#else
      throw todo(get_app());
#endif
      return TRUE;
   }

   bool interaction_impl::create(const char * lpszClassName,
                                 const char * lpszWindowName,uint32_t dwStyle,
                                 const RECT& rect,
                                 ::user::interaction* pParentWnd,id id,
                                 ::create* pContext)
   {
      // can't use for desktop or pop-up windows (use CreateEx instead)
      ASSERT(pParentWnd != NULL);
      ASSERT((dwStyle & WS_POPUP) == 0);

      return CreateEx(0,lpszClassName,lpszWindowName,
                      dwStyle | WS_CHILD,
                      rect.left,rect.top,
                      rect.right - rect.left,rect.bottom - rect.top,
                      pParentWnd->get_handle(),id,(LPVOID)pContext);
   }

   bool interaction_impl::initialize_native_window(::user::native_window_initialize * pinitialize)
   {

      m_window = pinitialize->window;

      m_pwindow->m_pwindow  = pinitialize->pwindow;

      m_oswindow = oswindow_get(this);

      ModifyStyle(0, WS_VISIBLE);

      m_pthreadDraw = ::fork(get_app(), [&]()
      {

         DWORD dwLastRedraw;

         manual_reset_event evDraw(get_app());

         evDraw.SetEvent();

         while (::get_thread_run())
         {

            dwLastRedraw = ::get_tick_count();

            if (::aura::system::g_p->m_pbasesystem->m_possystemwindow->m_bWindowSizeChange)
            {

               ::aura::system::g_p->m_pbasesystem->m_possystemwindow->m_bWindowSizeChange = false;

               m_xapp->m_directx->OnWindowSizeChange();

            }

            if (m_pui->has_pending_graphical_update()
               || m_pui->defer_check_layout())
            {

               if (m_xapp != nullptr)
               {

                  evDraw.ResetEvent();

                  m_window->Dispatcher->RunAsync(
                        CoreDispatcherPriority::Normal, 
                        ref new Windows::UI::Core::DispatchedHandler([this, &evDraw]()
                  {

                     try
                     {

                        synch_lock sl(&draw2d_mutex());

                        HRESULT hr = m_xapp->m_directx->Render();

                        if (SUCCEEDED(hr))
                        {

                           m_xapp->m_directx->Present();


                        }

                     }
                     catch(...)
                     {
                     }
                     
                     evDraw.SetEvent();

                  }));

               }

            }

            {

               single_lock sl(&draw2d_mutex());

               if (evDraw.wait(millis(16)).succeeded() && sl.lock(millis(1)))
               {

                  on_after_graphical_update();

                  _001UpdateBuffer();

                  if (::get_tick_count() - dwLastRedraw < 5)
                  {

                     Sleep(5);

                  }

               }

            }

         }


      });


//      m_pthread = dynamic_cast <::thread *> (::get_thread());


      return true;

   }

//   bool interaction_impl::create_message_queue(const char * pszName,::user::message_queue_listener* pcallback)
   bool interaction_impl::create_message_queue(::user::interaction * pui, const char * pszName)
   {

#ifdef METROWIN

      // metrowin todo
      return true;

#endif

      //m_plistener = pcallback;
      if(IsWindow())
      {
         SetWindowText(pszName);
      }
      else
      {
         string strName = "ca2::fontopus::message_wnd::winservice_1";
         if(!CreateEx(0,NULL,pszName,WS_CHILD,0,0,0,0,(oswindow)HWND_MESSAGE,NULL,NULL))
         {
            return false;
         }
         m_pui = pui;
      }
      return true;
   }


   interaction_impl::~interaction_impl()
   {

      if(m_pauraapp != NULL && m_pauraapp->m_pbasesession != NULL && m_pauraapp->m_pbasesession->user() != NULL && m_pauraapp->m_pbasesession->user()->m_pwindowmap != NULL)
      {

         m_pauraapp->m_pbasesession->user()->m_pwindowmap->m_map.remove_key((int_ptr)(void *)get_handle());

      }

   }


   void interaction_impl::install_message_handling(::message::dispatch * pinterface)
   {
      //m_pbuffer->InstallMessageHandling(pinterface);
      IGUI_WIN_MSG_LINK(WM_DESTROY,pinterface,this,&interaction_impl::_001OnDestroy);
      IGUI_WIN_MSG_LINK(WM_NCDESTROY,pinterface,this,&interaction_impl::_001OnNcDestroy);
      IGUI_WIN_MSG_LINK(WM_PAINT,pinterface,this,&interaction_impl::_001OnPaint);
      IGUI_WIN_MSG_LINK(WM_PRINT,pinterface,this,&interaction_impl::_001OnPrint);
      if(m_pui != NULL)
      {
         m_pui->install_message_handling(pinterface);
      }
      IGUI_WIN_MSG_LINK(WM_CREATE,pinterface,this,&interaction_impl::_001OnCreate);
      IGUI_WIN_MSG_LINK(WM_SETCURSOR,pinterface,this,&interaction_impl::_001OnSetCursor);
      IGUI_WIN_MSG_LINK(WM_ERASEBKGND,pinterface,this,&interaction_impl::_001OnEraseBkgnd);
      IGUI_WIN_MSG_LINK(WM_MOVE,pinterface,this,&interaction_impl::_001OnMove);
      IGUI_WIN_MSG_LINK(WM_SIZE,pinterface,this,&interaction_impl::_001OnSize);
      IGUI_WIN_MSG_LINK(WM_SHOWWINDOW,pinterface,this,&interaction_impl::_001OnShowWindow);
//      IGUI_WIN_MSG_LINK(ca2m_PRODEVIAN_SYNCH,pinterface,this,&interaction_impl::_001OnProdevianSynch);
   }

   void interaction_impl::_001OnMove(signal_details * pobj)
   {
      throw todo(get_app());
      //UNREFERENCED_PARAMETER(pobj);
      //if(!m_bRectOk && !(GetExStyle() & WS_EX_LAYERED))
      //{
      //   class rect rectWindow;
      //   ::GetWindowRect(get_handle(), rectWindow);
      //   m_pui->m_rectParentClient = rectWindow;
      //   m_rectParentClient = rectWindow;
      //}
   }

   void interaction_impl::_001OnSize(signal_details * pobj)
   {

      //throw todo(get_app());

      //UNREFERENCED_PARAMETER(pobj);


      //if(!m_bRectOk && !(GetExStyle() & WS_EX_LAYERED))
      //{
      //   class rect rectWindow;
      //   ::GetWindowRect(get_handle(), rectWindow);
      //   m_pui->m_rectParentClient = rectWindow;
      //   m_rectParentClient = rectWindow;
      //}

      ///*      if(m_spdibMultAlphaWork.is_null())
      //{
      //m_spdibMultAlphaWork.create(get_app());
      //}

      //if(m_spdib.is_null())
      //{
      //m_spdib.create(get_app());
      //}

      //if(m_spdib.is_set() && m_rectParentClient.area() > 0)
      //{
      //m_spdib->create(m_rectParentClient.size());
      //}*/

      //m_pui->on_layout();



   }


   void interaction_impl::_001OnDestroy(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      Default();
      //::metrowin::window_draw * pdraw = dynamic_cast <::metrowin::window_draw *> (System.get_twf());
      //if(pdraw != NULL)
      //{
      //   retry_single_lock sl(&pdraw->m_eventFree,millis(84),millis(84));
      //   //pdraw->m_wndpaOut.remove(this);
      //   pdraw->m_wndpaOut.remove(m_pui);
      //}
   }

   // WM_NCDESTROY is the absolute LAST message sent.
   void interaction_impl::_001OnNcDestroy(signal_details * pobj)
   {
      //single_lock sl(m_pthread == NULL ? NULL : &m_pthread->m_mutex,TRUE);
      //pobj->m_bRet = true;
      //// cleanup main and active windows
      //::thread* pThread = System.GetThread();
      //if(pThread != NULL)
      //{
      //   if(pThread->GetMainWnd() == this)
      //   {
      //      if(!afxContextIsDLL)
      //      {
      //         // shut down current thread if possible
      //         if(pThread != &System)
      //            __post_quit_message(0);
      //      }
      //      pThread->SetMainWnd(NULL);
      //   }
      //   if(pThread->get_active_ui() == this)
      //      pThread->set_active_ui(NULL);
      //}

      // cleanup tooltip support
      if(m_pui != NULL)
      {
         if(m_pui->m_nFlags & WF_TOOLTIPS)
         {
         }
      }

#ifdef WINDOWSEX
      // call default, unsubclass, and detach from the ::map
      WNDPROC pfnWndProc = WNDPROC(GetWindowLongPtr(get_handle(), GWLP_WNDPROC));
      Default();
      if (WNDPROC(GetWindowLongPtr(get_handle(), GWLP_WNDPROC)) == pfnWndProc)
      {
         WNDPROC pfnSuper = *GetSuperWndProcAddr();
         if (pfnSuper != NULL)
            SetWindowLongPtr(get_handle(), GWLP_WNDPROC, reinterpret_cast<int_ptr>(pfnSuper));
      }
#else
      throw todo(get_app());
#endif
      //      Detach();
      ASSERT(get_handle() == NULL);
      m_pfnDispatchWindowProc = &interaction_impl::_start_user_message_handler;
      // call special post-cleanup routine
      PostNcDestroy();
      if(m_pui != NULL)
      {
         m_pui->PostNcDestroy();
      }
   }

   void interaction_impl::PostNcDestroy()
   {
      //      set_handle(NULL);
      // default to nothing
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
      if(get_handle() == NULL)
         return;     // NULL (unattached) windows are valid

      // check for special wnd??? values
      ASSERT(HWND_TOP == NULL);       // same as desktop
#ifdef WINDOWSEX
      if (get_handle() == HWND_BOTTOM)
      {
      }
      else if (get_handle() == HWND_TOPMOST)
      {
      }
      else if (get_handle() == HWND_NOTOPMOST)
      {
      }
      else
#endif
      {
         // should be a normal interaction_impl
#ifdef WINDOWSEX
         ASSERT(::WinIsWindow(get_handle()));
#else
         throw todo(get_app());
#endif

         // should also be in the permanent or temporary handle ::map
         //single_lock sl(afxMutexHwnd(),TRUE);
         //hwnd_map * pMap = afxMapHWND();
         //if(pMap == NULL) // inside thread not having windows
         //   return; // let go
         //ASSERT(pMap != NULL);

         //         ::object* p=NULL;
         /*if(pMap)
         {
         ASSERT( (p = pMap->lookup_permanent(get_handle())) != NULL ||
         (p = pMap->lookup_temporary(get_handle())) != NULL);
         }*/

         //ASSERT(dynamic_cast < ::user::interaction_impl * > (p) == this);   // must be us

         // Note: if either of the above asserts fire and you are
         // writing a multithreaded application, it is likely that
         // you have passed a C++ object from one thread to another
         // and have used that object in a way that was not intended.
         // (only simple inline wrapper functions should be used)
         //
         // In general, interaction_impl objects should be passed by oswindow from
         // one thread to another.  The receiving thread can wrap
         // the oswindow with a interaction_impl object by using ::metrowin::interaction_impl::from_handle.
         //
         // It is dangerous to pass C++ objects from one thread to
         // another, unless the objects are designed to be used in
         // such a manner.
      }
   }


   void interaction_impl::dump(dump_context & dumpcontext) const
   {
      ::object::dump(dumpcontext);

      dumpcontext << "\nm_hWnd = " << (void *)get_handle();

#ifdef WINDOWSEX

      if(get_handle() == NULL || get_handle() == HWND_BOTTOM || get_handle() == HWND_TOPMOST || get_handle() == HWND_NOTOPMOST)
      {
         // not a normal interaction_impl - nothing more to dump
         return;
      }


      if (!::WinIsWindow(get_handle()))
      {
         // not a valid interaction_impl
         dumpcontext << " (illegal oswindow)";
         return; // don't do anything more
      }
#endif

      //      ::user::interaction_impl * pWnd = ::metrowin::interaction_impl::FromHandlePermanent(get_handle());
      //   if (pWnd != this)
      //      dumpcontext << " (Detached or temporary interaction_impl)";
      //     else
      //dumpcontext << " (permanent interaction_impl)";
      //
      // dump out interaction_impl specific statistics
      //      if (!const_cast < interaction_impl * > (this)->send_message(WM_QUERYAFXWNDPROC, 0, 0) && pWnd == this)
      //       ((::user::interaction_impl *) this)->get_window_text(szBuf, _countof(szBuf));
#ifdef WINDOWSEX
      char szBuf [64];
      else
         ::DefWindowProc(get_handle(), WM_GETTEXT, _countof(szBuf), (LPARAM)&szBuf[0]);
      dumpcontext << "\ncaption = \"" << szBuf << "\"";

      ::GetClassName(get_handle(), szBuf, _countof(szBuf));
      dumpcontext << "\nclass name = \"" << szBuf << "\"";
#endif

      rect rect;
      ((::user::interaction_impl *) this)->GetWindowRect(&rect);
      dumpcontext << "\nrect = " << rect;
      dumpcontext << "\nparent ::user::interaction_impl * = " << (void *)((::user::interaction_impl *) this)->GetParent();

#ifdef WINDOWSEX
      dumpcontext << "\nstyle = " << (void *)(dword_ptr)::GetWindowLong(get_handle(), GWL_STYLE);
      if (::GetWindowLong(get_handle(), GWL_STYLE) & WS_CHILD)
         dumpcontext << "\nid = " << __get_dialog_control_id(get_handle());
#endif
      dumpcontext << "\n";
   }

   bool interaction_impl::DestroyWindow()
   {
      //single_lock sl(m_pthread == NULL ? NULL : &m_pthread->m_mutex,TRUE);
      //::user::interaction_impl * pWnd;
      //hwnd_map * pMap;
      //oswindow hWndOrig;
      bool bResult = false;

      if((get_handle() == NULL))
         return FALSE;

      ShowWindow(SW_HIDE);

      //bResult = FALSE;
      //pMap = NULL;
      //pWnd = NULL;
      //hWndOrig = NULL;
      ///*      if (get_handle() != NULL)
      //{
      //single_lock sl(afxMutexHwnd(), TRUE);
      //pMap = afxMapHWND();
      //if(pMap != NULL)
      //{
      //pWnd = dynamic_cast < ::user::interaction_impl * > (pMap->lookup_permanent(get_handle()));
      //#ifdef DEBUG
      //hWndOrig = get_handle();
      //#endif
      //}
      //}
      //sl.unlock();
      //#ifdef WINDOWSEX
      //if (get_handle() != NULL)
      //bResult = ::DestroyWindow(get_handle()) != FALSE;
      //#else
      //throw todo(get_app());
      //#endif
      //sl.lock();
      //if (hWndOrig != NULL)
      //{
      //// Note that 'this' may have been deleted at this point,
      ////  (but only if pWnd != NULL)
      //if (pWnd != NULL)
      //{
      //// Should have been detached by OnNcDestroy
      //#ifdef DEBUG
      //::user::interaction_impl * pWndPermanent = dynamic_cast < ::user::interaction_impl * > (pMap->lookup_permanent(hWndOrig));;
      //ASSERT(pWndPermanent == NULL);
      //// It is important to call aura class, including ca2 core
      //// aura classes implementation of install_message_handling
      //// inside derived class install_message_handling
      //#endif
      //}
      //else
      //{
      //#ifdef DEBUG
      //ASSERT(get_handle() == hWndOrig);
      //#endif
      //// Detach after DestroyWindow called just in case
      //Detach();
      //}
      //}*/

      return bResult;
   }

   /////////////////////////////////////////////////////////////////////////////
   // Default interaction_impl implementation


   LRESULT interaction_impl::DefWindowProc(UINT nMsg,WPARAM wParam,lparam lParam)
   {
      return 0;

#ifdef WINDOWSEX
      if (m_pfnSuper != NULL)
         return ::CallWindowProc(m_pfnSuper, get_handle(), nMsg, wParam, lParam);

      WNDPROC pfnWndProc;
      if ((pfnWndProc = *GetSuperWndProcAddr()) == NULL)
         return ::DefWindowProc(get_handle(), nMsg, wParam, lParam);
      else
         return ::CallWindowProc(pfnWndProc, get_handle(), nMsg, wParam, lParam);
#else
      throw todo(get_app());
#endif
   }

#ifdef WINDOWSEX
   WNDPROC* interaction_impl::GetSuperWndProcAddr()
   {
      // Note: it is no longer necessary to override GetSuperWndProcAddr
      //  for each control class with a different WNDCLASS.
      //  This implementation now uses instance data, such that the previous
      //  WNDPROC can be anything.

      return &m_pfnSuper;
   }
#endif


   void interaction_impl::pre_translate_message(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      // no default processing
   }


   void interaction_impl::GetWindowText(string & rString)
   {
      /*ASSERT(::WinIsWindow(get_handle()));

      int nLen = ::GetWindowTextLength(get_handle());
      ::GetWindowText(get_handle(), rString.GetBufferSetLength(nLen), nLen+1);
      rString.ReleaseBuffer();*/
      rString = m_strWindowText;

   }


#ifdef WINDOWSEX

   int interaction_impl::GetDlgItemText(int nID, string & rString) const
   {
      ASSERT(::WinIsWindow(get_handle()));
      rString = "";    // is_empty without deallocating

      oswindow hWnd = ::GetDlgItem(get_handle(), nID);
      if (hWnd != NULL)
      {
         int nLen = ::GetWindowTextLength(hWnd);
         ::GetWindowText(hWnd, rString.GetBufferSetLength(nLen), nLen+1);
         rString.ReleaseBuffer();
      }

      return (int)rString.get_length();

   }

#endif

#ifdef WINDOWSEX
   bool interaction_impl::GetWindowPlacement(WINDOWPLACEMENT* lpwndpl)
   {
      ASSERT(::WinIsWindow(get_handle()));
      lpwndpl->length = sizeof(WINDOWPLACEMENT);
      return ::GetWindowPlacement(get_handle(), lpwndpl) != FALSE;
   }

   bool interaction_impl::SetWindowPlacement(const WINDOWPLACEMENT* lpwndpl)
   {
      ASSERT(::WinIsWindow(get_handle()));
      ((WINDOWPLACEMENT*)lpwndpl)->length = sizeof(WINDOWPLACEMENT);
      return ::SetWindowPlacement(get_handle(), lpwndpl) != FALSE;
   }
   /////////////////////////////////////////////////////////////////////////////
   // interaction_impl will delegate owner draw messages to self drawing controls

   // Drawing: for all 4 control types
   void interaction_impl::OnDrawItem(int /*nIDCtl*/, LPDRAWITEMSTRUCT lpDrawItemStruct)
   {

      // reflect notification to child interaction_impl control
      if (ReflectLastMsg(lpDrawItemStruct->hwndItem))
         return;     // eat it

      // not handled - do default
      Default();
   }

   // Drawing: for all 4 control types
   int interaction_impl::OnCompareItem(int /*nIDCtl*/, LPCOMPAREITEMSTRUCT lpCompareItemStruct)
   {
      // reflect notification to child interaction_impl control
      LRESULT lResult;
      if (ReflectLastMsg(lpCompareItemStruct->hwndItem, &lResult))
         return (int)lResult;        // eat it

      // not handled - do default
      return (int)Default();
   }

   void interaction_impl::OnDeleteItem(int /*nIDCtl*/, LPDELETEITEMSTRUCT lpDeleteItemStruct)
   {
      // reflect notification to child interaction_impl control
      if (ReflectLastMsg(lpDeleteItemStruct->hwndItem))
         return;     // eat it
      // not handled - do default
      Default();
   }

#endif


   bool interaction_impl::_EnableToolTips(bool bEnable,UINT nFlag)
   {
      UNREFERENCED_PARAMETER(bEnable);
      UNREFERENCED_PARAMETER(nFlag);
      return FALSE;
   }


   // Measure item implementation relies on unique control/menu IDs
#ifdef WINDOWSEX
   void interaction_impl::OnMeasureItem(int /*nIDCtl*/, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
   {
      if (lpMeasureItemStruct->CtlType == ODT_MENU)
      {
         ASSERT(lpMeasureItemStruct->CtlID == 0);
      }
      else
      {
         ::user::interaction * pChild = GetDescendantWindow(lpMeasureItemStruct->CtlID);
         if (pChild != NULL && pChild->SendChildNotifyLastMsg())
            return;     // eaten by child
      }
      // not handled - do default
      Default();
   }


   bool interaction_impl::GetWindowInfo(PWINDOWINFO pwi) const
   {
      ASSERT(::WinIsWindow((oswindow)get_os_data()));
      return ::GetWindowInfo((oswindow)get_os_data(), pwi) != FALSE;
   }

#endif

   sp(::user::interaction_impl) interaction_impl::GetAncestor(UINT gaFlags) const
   {

#ifdef WINDOWSEX
      ASSERT(::WinIsWindow((oswindow)get_os_data()));
      return  ::metrowin::interaction_impl::from_handle(::GetAncestor((oswindow)get_os_data(), gaFlags));
#else
      throw todo(get_app());
#endif


   }



#ifdef WINDOWSEX
   bool interaction_impl::GetScrollBarInfo(LONG idObject, PSCROLLBARINFO psbi) const
   {
      ASSERT(::WinIsWindow((oswindow)get_os_data()));
      ASSERT(psbi != NULL);
      return ::GetScrollBarInfo((oswindow)get_os_data(), idObject, psbi) != FALSE;
   }

   bool interaction_impl::GetTitleBarInfo(PTITLEBARINFO pti) const
   {
      ASSERT(::WinIsWindow((oswindow)get_os_data()));
      ASSERT(pti != NULL);
      return ::GetTitleBarInfo((oswindow)get_os_data(), pti) != FALSE;
   }
#endif

   bool interaction_impl::AnimateWindow(uint32_t dwTime,uint32_t dwFlags)
   {
#ifdef WINDOWSEX
      ASSERT(::WinIsWindow((oswindow)get_os_data()));
      return ::AnimateWindow((oswindow)get_os_data(), dwTime, dwFlags) != FALSE;
#else
      throw todo(get_app());
#endif
   }

   bool interaction_impl::FlashWindowEx(uint32_t dwFlags,UINT  uCount,uint32_t dwTimeout)
   {
      throw todo(get_app());

      //   ASSERT(::WinIsWindow((oswindow)get_os_data()));
      //   FLASHWINFO fwi;
      //   fwi.cbSize = sizeof(fwi);
      //   fwi.hwnd = (oswindow)get_os_data();
      //   fwi.dwFlags = dwFlags;
      //   fwi.uCount = uCount;
      //   fwi.dwTimeout = dwTimeout;

      //   return ::FlashWindowEx(&fwi) != FALSE;
   }



   bool interaction_impl::SetLayeredWindowAttributes(COLORREF crKey,BYTE bAlpha,uint32_t dwFlags)
   {
      throw todo(get_app());
      //ASSERT(::WinIsWindow((oswindow)get_os_data()));
      //return ::SetLayeredWindowAttributes((oswindow)get_os_data(), crKey, bAlpha, dwFlags) != FALSE;
   }

   bool interaction_impl::UpdateLayeredWindow(::draw2d::graphics * pDCDst,POINT *pptDst,SIZE *psize,
         ::draw2d::graphics * pDCSrc,POINT *pptSrc,COLORREF crKey,BLENDFUNCTION *pblend,uint32_t dwFlags)
   {
      throw todo(get_app());
      //ASSERT(::WinIsWindow((oswindow)get_os_data()));
      //return ::UpdateLayeredWindow((oswindow)get_os_data(), WIN_HDC(pDCDst), pptDst, psize,
      //   WIN_HDC(pDCSrc), pptSrc, crKey, pblend, dwFlags) != FALSE;
   }


   bool interaction_impl::GetLayeredWindowAttributes(COLORREF *pcrKey,BYTE *pbAlpha,uint32_t *pdwFlags) const
   {
      throw todo(get_app());
      //ASSERT(::WinIsWindow((oswindow)get_os_data()));
      //return ::GetLayeredWindowAttributes((oswindow)get_os_data(), pcrKey, pbAlpha, pdwFlags) != FALSE;
   }

   bool interaction_impl::PrintWindow(::draw2d::graphics * pgraphics,UINT nFlags) const
   {
      throw todo(get_app());
      //ASSERT(::WinIsWindow((oswindow)get_os_data()));
      //return ::PrintWindow((oswindow)get_os_data(), (HDC)(dynamic_cast<::metrowin::graphics * >(pgraphics))->get_os_data(), nFlags) != FALSE;
   }


   LRESULT interaction_impl::OnNTCtlColor(WPARAM wParam,LPARAM lParam)
   {
      throw todo(get_app());
      //// fill in special struct for compatiblity with 16-bit WM_CTLCOLOR
      //__CTLCOLOR ctl;
      //ctl.hDC = (HDC)wParam;
      //ctl.hWnd = (oswindow)lParam;
      //___THREAD_STATE* pThreadState = gen_ThreadState.get_data();
      //ctl.nCtlType = pThreadState->m_lastSentMsg.message - WM_CTLCOLORMSGBOX;
      ////ASSERT(ctl.nCtlType >= CTLCOLOR_MSGBOX);
      //ASSERT(ctl.nCtlType <= CTLCOLOR_STATIC);

      //// Note: We call the virtual message_handler for this interaction_impl directly,
      ////  instead of calling ::ca2::CallWindowProc, so that Default()
      ////  will still work (it will call the Default interaction_impl proc with
      ////  the original Win32 WM_CTLCOLOR message).
      ///*
      //return message_handler(WM_CTLCOLOR, 0, (LPARAM)&ctl);*/
      //return 0;
   }

   /////////////////////////////////////////////////////////////////////////////
   // interaction_impl extensions for help support
   /*
      void interaction_impl::WinHelp(dword_ptr dwData, UINT nCmd)
      {
      UNREFERENCED_PARAMETER(dwData);
      UNREFERENCED_PARAMETER(nCmd);
      throw not_implemented(get_app());

      /*      application* pApp = &System;
      ASSERT_VALID(pApp);
      ASSERT(pApp->m_pszHelpFilePath != NULL);
      ASSERT(pApp->m_eHelpType == afxWinHelp);

      wait_cursor wait(get_app());

      PrepareForHelp();

      // need to use top level parent (for the case where get_handle() is in DLL)
      ::user::interaction * pWnd = EnsureTopLevelParent();

      TRACE(::core::trace::category_AppMsg, 0, "WinHelp: pszHelpFile = '%s', dwData: $%lx, fuCommand: %d.\n", pApp->m_pszHelpFilePath, dwData, nCmd);

      // finally, run the Windows Help engine
      /* trans   if (!::WinHelp(WIN_WINDOW(pWnd)->get_handle(), pApp->m_pszHelpFilePath, nCmd, dwData))
      {
      // linux System.simple_message_box(__IDP_FAILED_TO_LAUNCH_HELP);
      System.simple_message_box("Failed to launch help");
      }*/
   // }

   //void interaction_impl::HtmlHelp(dword_ptr dwData, UINT nCmd)
   //{
   // throw not_implemented(get_app());
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
   ::user::interaction * pWnd = EnsureTopLevelParent();

   TRACE(::core::trace::category_AppMsg, 0, "HtmlHelp: pszHelpFile = '%s', dwData: $%lx, fuCommand: %d.\n", pApp->m_pszHelpFilePath, dwData, nCmd);

   // run the HTML Help engine
   /* trans   if (!::ca2::HtmlHelp(WIN_WINDOW(pWnd)->get_handle(), pApp->m_pszHelpFilePath, nCmd, dwData))
   {
   // linux System.simple_message_box(__IDP_FAILED_TO_LAUNCH_HELP);
   System.simple_message_box("Failed to launch help");
   }*/
   //}

   void interaction_impl::PrepareForHelp()
   {
      if(is_frame_window())
      {
         // frame_window windows should be allowed to exit help mode first
         sp(::user::frame_window) pFrameWnd = this;
         pFrameWnd->ExitHelpMode();
      }

      // cancel any tracking modes
      send_message(WM_CANCELMODE);
      SendMessageToDescendants(WM_CANCELMODE,0,0,TRUE,TRUE);

      // need to use top level parent (for the case where get_handle() is in DLL)
      ::user::interaction * pWnd = EnsureTopLevel();
      WIN_WINDOW(pWnd)->send_message(WM_CANCELMODE);
      WIN_WINDOW(pWnd)->SendMessageToDescendants(WM_CANCELMODE,0,0,TRUE,TRUE);

      throw todo(get_app());
      // attempt to cancel capture
      //oswindow hWndCapture = ::GetCapture();
      //if (hWndCapture != NULL)
      //   ::SendMessage(hWndCapture, WM_CANCELMODE, 0, 0);
   }

#ifdef WINDOWSEX
   void interaction_impl::WinHelpInternal(dword_ptr dwData, UINT nCmd)
   {
      UNREFERENCED_PARAMETER(dwData);
      UNREFERENCED_PARAMETER(nCmd);
      throw not_implemented(get_app());
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

#endif

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


   bool interaction_impl::BaseOnControlEvent(::user::control_event * pevent)
   {
      UNREFERENCED_PARAMETER(pevent);
      return false;
   }

   void interaction_impl::_002OnDraw(::draw2d::dib * pdib)
   {
      throw todo(get_app());
      //::CallWindowProc(*GetSuperWndProcAddr(), get_handle(), WM_PRINT, (WPARAM)((dynamic_cast<::metrowin::graphics * >(pgraphics))->get_os_data()), (LPARAM)(PRF_CHILDREN | PRF_CLIENT));

   }

   /////////////////////////////////////////////////////////////////////////////
   // main message_handler implementation

   void interaction_impl::message_handler(signal_details * pobj)
   {
      SCAST_PTR(::message::base,pbase,pobj);

      if(m_pui != NULL)
      {
         m_pui->pre_translate_message(pobj);
         if(pobj->m_bRet)
            return;
      }

      if(m_plistener != NULL)
      {
         m_plistener->message_queue_message_handler(pobj);
         if(pobj->m_bRet)
            return;
      }


      if(pbase->m_uiMessage == WM_KEYDOWN ||
            pbase->m_uiMessage == WM_KEYUP ||
            pbase->m_uiMessage == WM_CHAR ||
            pbase->m_uiMessage == WM_SYSKEYDOWN ||
            pbase->m_uiMessage == WM_SYSKEYUP ||
            pbase->m_uiMessage == WM_SYSCHAR)
      {

         SCAST_PTR(::message::key,pkey,pobj);

         //Session.keyboard().translate_os_key_message(pkey);

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

      if(pbase->m_uiMessage == WM_TIMER)
      {
//         m_pui->m_pauraapp->step_timer();
      }
      else if(pbase->m_uiMessage == WM_LBUTTONDOWN)
      {
         //g_pwndLastLButtonDown = m_pui;
      }
      /*      else if(pbase->m_uiMessage == CA2M_BERGEDGE)
      {
      if(pbase->m_wparam == BERGEDGE_GETAPP)
      {
      ::aura::application ** ppapp= (::aura::application **) pbase->m_lparam;
      *ppapp = get_app();
      pbase->m_bRet = true;
      return;
      }
      }*/
      pbase->set_lresult(0);

      _000OnMouseLeave(pbase);

      if(pbase->m_uiMessage == WM_LBUTTONDOWN ||
            pbase->m_uiMessage == WM_LBUTTONUP ||
            pbase->m_uiMessage == WM_MBUTTONDOWN ||
            pbase->m_uiMessage == WM_MBUTTONUP ||
            pbase->m_uiMessage == WM_RBUTTONDOWN ||
            pbase->m_uiMessage == WM_RBUTTONUP ||
            pbase->m_uiMessage == WM_MOUSEMOVE ||
            pbase->m_uiMessage == WM_MOUSEWHEEL)
      {
         message::mouse * pmouse = (::message::mouse *) pbase;

         Session.on_ui_mouse_message(pmouse);


         //Application.m_ptCursor = pmouse->m_pt;
         //if(m_paxisapp->m_pcoreapp->m_psession != NULL)
         //{
         //   Session.m_ptCursor = pmouse->m_pt;
         //}
         //if(m_pui != NULL && m_pui->m_paxisapp->m_pcoreapp->m_psession != NULL && m_pui->m_paxisapp->m_pcoreapp->m_psession != m_paxisapp->m_pcoreapp->m_psession)
         //{
         //   Sess(m_pui->m_paxisapp->m_pcoreapp->m_psession).m_ptCursor = pmouse->m_pt;
         //}

         //sp(base_session) psession;
         //if(m_paxisapp->m_pcoreapp->is_system())
         //{
         //   psession = System.query_session(0);
         //   if(psession != NULL && psession->m_bSessionSynchronizedCursor)
         //   {
         //      psession->m_ptCursor = pmouse->m_pt;
         //   }
         //}

         if(m_bTranslateMouseMessageCursor && !pmouse->m_bTranslated)
         {
            pmouse->m_bTranslated = true;
            rect64 rectWindow;
            if(m_bScreenRelativeMouseMessagePosition)
            {
               /*               class rect rectWindow32;
                              ::GetWindowRect(get_handle(), &rectWindow32);
                              ::copy(rectWindow, rectWindow32);*/
            }
            else
            {
               m_pui->GetWindowRect(rectWindow);
            }
            if(System.get_monitor_count() > 0)
            {
               rect rcMonitor;
               System.get_monitor_rect(0,&rcMonitor);
               if(rectWindow.left >= rcMonitor.left)
                  pmouse->m_pt.x += (LONG)rectWindow.left;
               if(rectWindow.top >= rcMonitor.top)
                  pmouse->m_pt.y += (LONG)rectWindow.top;
            }
            else
            {
               if(rectWindow.left >= 0)
                  pmouse->m_pt.x += (LONG)rectWindow.left;
               if(rectWindow.top >= 0)
                  pmouse->m_pt.y += (LONG)rectWindow.top;
            }
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

         _008OnMouse(pmouse);

         return;

      }
      else if(pbase->m_uiMessage == WM_KEYDOWN ||
              pbase->m_uiMessage == WM_KEYUP ||
              pbase->m_uiMessage == WM_CHAR)
      {

         ::message::key * pkey = (::message::key *) pbase;


         ::user::interaction * puiFocus = dynamic_cast <::user::interaction *> (Session.get_keyboard_focus());
         if(puiFocus != NULL
               && puiFocus->IsWindow()
               && puiFocus->GetTopLevel() != NULL)
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
            m_pui->BaseOnControlEvent((::user::control_event *) pbase->m_lparam);
         }
         else
         {
            BaseOnControlEvent((::user::control_event *) pbase->m_lparam);
         }
         return;
      }
      (this->*m_pfnDispatchWindowProc)(pobj);
      if(pobj->m_bRet)
         return;
      /*
      if(m_pui != NULL)
      {
      m_pui->_user_message_handler(pobj);
      if(pobj->m_bRet)
      return;
      }
      */
      pbase->set_lresult(DefWindowProc(pbase->m_uiMessage,pbase->m_wparam,pbase->m_lparam));
   }

   /*
   bool interaction_impl::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
   {
   LRESULT lResult = 0;
   union MessageMapFunctions mmf;
   mmf.pfn = 0;
   CInternalGlobalLock winMsgLock;
   // special case for commands
   if (message == WM_COMMAND)
   {
   if (OnCommand(wParam, lParam))
   {
   lResult = 1;
   goto LReturnTrue;
   }
   return FALSE;
   }

   // special case for notifies
   if (message == WM_NOTIFY)
   {
   NMHDR* pNMHDR = (NMHDR*)lParam;
   if (pNMHDR->hwndFrom != NULL && OnNotify(wParam, lParam, &lResult))
   goto LReturnTrue;
   return FALSE;
   }

   // special case for activation
   if (message == WM_ACTIVATE)
   __handle_activate(this, wParam, ::metrowin::interaction_impl::from_handle((oswindow)lParam));

   // special case for set cursor HTERROR
   if (message == WM_SETCURSOR &&
   __handle_set_cursor(this, (short)LOWORD(lParam), HIWORD(lParam)))
   {
   lResult = 1;
   goto LReturnTrue;
   }

   // special case for windows that contain windowless ActiveX controls
   bool bHandled;

   bHandled = FALSE;

   const __MSGMAP* pMessageMap; pMessageMap = GetMessageMap();
   UINT iHash; iHash = (LOWORD((dword_ptr)pMessageMap) ^ message) & (iHashMax-1);
   winMsgLock.lock(CRIT_WINMSGCACHE);
   __MSG_CACHE* pMsgCache; pMsgCache = &gen_MsgCache[iHash];
   const __MSGMAP_ENTRY* lpEntry;
   if (message == pMsgCache->nMsg && pMessageMap == pMsgCache->pMessageMap)
   {
   // cache hit
   lpEntry = pMsgCache->lpEntry;
   winMsgLock.unlock();
   if (lpEntry == NULL)
   return FALSE;

   // cache hit, and it needs to be handled
   if (message < 0xC000)
   goto LDispatch;
   else
   goto LDispatchRegistered;
   }
   else
   {
   // not in cache, look for it
   pMsgCache->nMsg = message;
   pMsgCache->pMessageMap = pMessageMap;

   for (/* pMessageMap already init'ed */ /*; pMessageMap->pfnGetBaseMap != NULL;
pMessageMap = (*pMessageMap->pfnGetBaseMap)())
{
// Note: catch not so common but fatal mistake!!
//       // BEGIN_MESSAGE_MAP(CMyWnd, CMyWnd)
ASSERT(pMessageMap != (*pMessageMap->pfnGetBaseMap)());
if (message < 0xC000)
{
// constant interaction_impl message
if ((lpEntry = ::ca2::FindMessageEntry(pMessageMap->lpEntries,
message, 0, 0)) != NULL)
{
pMsgCache->lpEntry = lpEntry;
winMsgLock.unlock();
goto LDispatch;
}
}
else
{
// registered windows message
lpEntry = pMessageMap->lpEntries;
while ((lpEntry = ::ca2::FindMessageEntry(lpEntry, 0xC000, 0, 0)) != NULL)
{
UINT* pnID = (UINT*)(lpEntry->nSig);
ASSERT(*pnID >= 0xC000 || *pnID == 0);
// must be successfully registered
if (*pnID == message)
{
pMsgCache->lpEntry = lpEntry;
winMsgLock.unlock();
goto LDispatchRegistered;
}
lpEntry++;      // keep looking past this one
}
}
}

pMsgCache->lpEntry = NULL;
winMsgLock.unlock();
return FALSE;
}

LDispatch:
ASSERT(message < 0xC000);

mmf.pfn = lpEntry->pfn;

switch (lpEntry->nSig)
{
default:
ASSERT(FALSE);
break;
case ::ca2::Sig_l_p:
{
point point(lParam);
lResult = (this->*mmf.pfn_l_p)(point);
break;
}
case ::ca2::Sig_b_D_v:
lResult = (this->*mmf.pfn_b_D)(::metrowin::graphics::from_handle(reinterpret_cast<HDC>(wParam)));
break;

case ::ca2::Sig_b_b_v:
lResult = (this->*mmf.pfn_b_b)(static_cast<bool>(wParam));
break;

case ::ca2::Sig_b_u_v:
lResult = (this->*mmf.pfn_b_u)(static_cast<UINT>(wParam));
break;

case ::ca2::Sig_b_h_v:
lResult = (this->*mmf.pfn_b_h)(reinterpret_cast<HANDLE>(wParam));
break;

case ::ca2::Sig_i_u_v:
lResult = (this->*mmf.pfn_i_u)(static_cast<UINT>(wParam));
break;

case ::ca2::Sig_C_v_v:
lResult = reinterpret_cast<LRESULT>((this->*mmf.pfn_C_v)());
break;

case ::ca2::Sig_v_u_W:
(this->*mmf.pfn_v_u_W)(static_cast<UINT>(wParam),
::metrowin::interaction_impl::from_handle(reinterpret_cast<oswindow>(lParam)));
break;

case ::ca2::Sig_u_u_v:
lResult = (this->*mmf.pfn_u_u)(static_cast<UINT>(wParam));
break;

case ::ca2::Sig_b_v_v:
lResult = (this->*mmf.pfn_b_v)();
break;

case ::ca2::Sig_b_W_uu:
lResult = (this->*mmf.pfn_b_W_u_u)(::metrowin::interaction_impl::from_handle(reinterpret_cast<oswindow>(wParam)),
LOWORD(lParam), HIWORD(lParam));
break;

case ::ca2::Sig_b_W_COPYDATASTRUCT:
lResult = (this->*mmf.pfn_b_W_COPYDATASTRUCT)(
::metrowin::interaction_impl::from_handle(reinterpret_cast<oswindow>(wParam)),
reinterpret_cast<COPYDATASTRUCT*>(lParam));
break;

case ::ca2::Sig_b_v_HELPINFO:
lResult = (this->*mmf.pfn_b_HELPINFO)(reinterpret_cast<LPHELPINFO>(lParam));
break;

case ::ca2::Sig_CTLCOLOR:
{
// special case for OnCtlColor to avoid too many temporary objects
ASSERT(message == WM_CTLCOLOR);
__CTLCOLOR* pCtl = reinterpret_cast<__CTLCOLOR*>(lParam);
::draw2d::graphics_sp dcTemp;
dcTemp.set_handle1(pCtl->hDC);
interaction_impl wndTemp;
wndTemp.set_handle(pCtl->hWnd);
UINT nCtlType = pCtl->nCtlType;
// if not coming from a permanent interaction_impl, use stack temporary
::user::interaction_impl * pWnd = ::metrowin::interaction_impl::FromHandlePermanent(wndTemp.get_handle());
if (pWnd == NULL)
{
pWnd = &wndTemp;
}
HBRUSH hbr = (this->*mmf.pfn_B_D_W_u)(&dcTemp, pWnd, nCtlType);
// fast detach of temporary objects
dcTemp.set_handle1(NULL);
wndTemp.set_handle(NULL);
lResult = reinterpret_cast<LRESULT>(hbr);
}
break;

case ::ca2::Sig_CTLCOLOR_REFLECT:
{
// special case for CtlColor to avoid too many temporary objects
ASSERT(message == WM_REFLECT_BASE+WM_CTLCOLOR);
__CTLCOLOR* pCtl = reinterpret_cast<__CTLCOLOR*>(lParam);
::draw2d::graphics_sp dcTemp;
dcTemp.set_handle1(pCtl->hDC);
UINT nCtlType = pCtl->nCtlType;
HBRUSH hbr = (this->*mmf.pfn_B_D_u)(&dcTemp, nCtlType);
// fast detach of temporary objects
dcTemp.set_handle1(NULL);
lResult = reinterpret_cast<LRESULT>(hbr);
}
break;

case ::ca2::Sig_i_u_W_u:
lResult = (this->*mmf.pfn_i_u_W_u)(LOWORD(wParam),
::metrowin::interaction_impl::from_handle(reinterpret_cast<oswindow>(lParam)), HIWORD(wParam));
break;

case ::ca2::Sig_i_uu_v:
lResult = (this->*mmf.pfn_i_u_u)(LOWORD(wParam), HIWORD(wParam));
break;

case ::ca2::Sig_i_W_uu:
lResult = (this->*mmf.pfn_i_W_u_u)(::metrowin::interaction_impl::from_handle(reinterpret_cast<oswindow>(wParam)),
LOWORD(lParam), HIWORD(lParam));
break;

case ::ca2::Sig_i_v_s:
lResult = (this->*mmf.pfn_i_s)(reinterpret_cast<LPTSTR>(lParam));
break;

case ::ca2::Sig_l_w_l:
lResult = (this->*mmf.pfn_l_w_l)(wParam, lParam);
break;



case ::ca2::Sig_v_b_h:
(this->*mmf.pfn_v_b_h)(static_cast<bool>(wParam),
reinterpret_cast<HANDLE>(lParam));
break;

case ::ca2::Sig_v_h_v:
(this->*mmf.pfn_v_h)(reinterpret_cast<HANDLE>(wParam));
break;

case ::ca2::Sig_v_h_h:
(this->*mmf.pfn_v_h_h)(reinterpret_cast<HANDLE>(wParam),
reinterpret_cast<HANDLE>(lParam));
break;

case ::ca2::Sig_v_v_v:
(this->*mmf.pfn_v_v)();
break;

case ::ca2::Sig_v_u_v:
(this->*mmf.pfn_v_u)(static_cast<UINT>(wParam));
break;

case ::ca2::Sig_v_u_u:
(this->*mmf.pfn_v_u_u)(static_cast<UINT>(wParam), static_cast<UINT>(lParam));
break;

case ::ca2::Sig_v_uu_v:
(this->*mmf.pfn_v_u_u)(LOWORD(wParam), HIWORD(wParam));
break;

case ::ca2::Sig_v_v_ii:
(this->*mmf.pfn_v_i_i)(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
break;

case ::ca2::Sig_v_u_uu:
(this->*mmf.pfn_v_u_u_u)(static_cast<UINT>(wParam), LOWORD(lParam), HIWORD(lParam));
break;

case ::ca2::Sig_v_u_ii:
(this->*mmf.pfn_v_u_i_i)(static_cast<UINT>(wParam), LOWORD(lParam), HIWORD(lParam));
break;

case ::ca2::Sig_v_w_l:
(this->*mmf.pfn_v_w_l)(wParam, lParam);
break;

case ::ca2::Sig_MDIACTIVATE:
(this->*mmf.pfn_v_b_W_W)(get_handle() == reinterpret_cast<oswindow>(lParam),
::metrowin::interaction_impl::from_handle(reinterpret_cast<oswindow>(lParam)),
::metrowin::interaction_impl::from_handle(reinterpret_cast<oswindow>(wParam)));
break;

case ::ca2::Sig_v_D_v:
(this->*mmf.pfn_v_D)(::metrowin::graphics::from_handle(reinterpret_cast<HDC>(wParam)));
break;


case ::ca2::Sig_v_W_v:
(this->*mmf.pfn_v_W)(::metrowin::interaction_impl::from_handle(reinterpret_cast<oswindow>(wParam)));
break;

case ::ca2::Sig_v_v_W:
(this->*mmf.pfn_v_W)(::metrowin::interaction_impl::from_handle(reinterpret_cast<oswindow>(lParam)));
break;

case ::ca2::Sig_v_W_uu:
(this->*mmf.pfn_v_W_u_u)(::metrowin::interaction_impl::from_handle(reinterpret_cast<oswindow>(wParam)), LOWORD(lParam),
HIWORD(lParam));
break;

case ::ca2::Sig_v_W_p:
{
point point(lParam);
(this->*mmf.pfn_v_W_p)(::metrowin::interaction_impl::from_handle(reinterpret_cast<oswindow>(wParam)), point);
}
break;

case ::ca2::Sig_v_W_h:
(this->*mmf.pfn_v_W_h)(::metrowin::interaction_impl::from_handle(reinterpret_cast<oswindow>(wParam)),
reinterpret_cast<HANDLE>(lParam));
break;

case ::ca2::Sig_ACTIVATE:
(this->*mmf.pfn_v_u_W_b)(LOWORD(wParam),
::metrowin::interaction_impl::from_handle(reinterpret_cast<oswindow>(lParam)), HIWORD(wParam));
break;

case ::ca2::Sig_SCROLL:
case ::ca2::Sig_SCROLL_REFLECT:
{
// special case for WM_VSCROLL and WM_HSCROLL
ASSERT(message == WM_VSCROLL || message == WM_HSCROLL ||
message == WM_VSCROLL+WM_REFLECT_BASE || message == WM_HSCROLL+WM_REFLECT_BASE);
int nScrollCode = (short)LOWORD(wParam);
int nPos = (short)HIWORD(wParam);
if (lpEntry->nSig == ::ca2::Sig_SCROLL)
(this->*mmf.pfn_v_u_u_W)(nScrollCode, nPos,
::metrowin::interaction_impl::from_handle(reinterpret_cast<oswindow>(lParam)));
else
(this->*mmf.pfn_v_u_u)(nScrollCode, nPos);
}
break;

case ::ca2::Sig_v_v_s:
(this->*mmf.pfn_v_s)(reinterpret_cast<LPTSTR>(lParam));
break;

case ::ca2::Sig_v_u_cs:
(this->*mmf.pfn_v_u_cs)(static_cast<UINT>(wParam), reinterpret_cast<const char *>(lParam));
break;

case ::ca2::Sig_OWNERDRAW:
(this->*mmf.pfn_v_i_s)(static_cast<int>(wParam), reinterpret_cast<LPTSTR>(lParam));
lResult = TRUE;
break;

case ::ca2::Sig_i_i_s:
lResult = (this->*mmf.pfn_i_i_s)(static_cast<int>(wParam), reinterpret_cast<LPTSTR>(lParam));
break;

case ::ca2::Sig_u_v_p:
{
point point(lParam);
lResult = (this->*mmf.pfn_u_p)(point);
}
break;

case ::ca2::Sig_u_v_v:
lResult = (this->*mmf.pfn_u_v)();
break;

case ::ca2::Sig_v_b_NCCALCSIZEPARAMS:
(this->*mmf.pfn_v_b_NCCALCSIZEPARAMS)(static_cast<bool>(wParam),
reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam));
break;

case ::ca2::Sig_v_v_WINDOWPOS:
(this->*mmf.pfn_v_v_WINDOWPOS)(reinterpret_cast<WINDOWPOS*>(lParam));
break;

case ::ca2::Sig_v_uu_M:
(this->*mmf.pfn_v_u_u_M)(LOWORD(wParam), HIWORD(wParam), reinterpret_cast<HMENU>(lParam));
break;

case ::ca2::Sig_v_u_p:
{
point point(lParam);
(this->*mmf.pfn_v_u_p)(static_cast<UINT>(wParam), point);
}
break;

case ::ca2::Sig_SIZING:
(this->*mmf.pfn_v_u_pr)(static_cast<UINT>(wParam), reinterpret_cast<LPRECT>(lParam));
lResult = TRUE;
break;

case ::ca2::Sig_MOUSEWHEEL:
lResult = (this->*mmf.pfn_b_u_s_p)(LOWORD(wParam), (short)HIWORD(wParam),
point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
if (!lResult)
return FALSE;
break;
case ::ca2::Sig_l:
lResult = (this->*mmf.pfn_l_v)();
if (lResult != 0)
return FALSE;
break;
}
goto LReturnTrue;

LDispatchRegistered:    // for registered windows messages
ASSERT(message >= 0xC000);
ASSERT(sizeof(mmf) == sizeof(mmf.pfn));
mmf.pfn = lpEntry->pfn;
lResult = (this->*mmf.pfn_l_w_l)(wParam, lParam);

LReturnTrue:
if (pResult != NULL)
*pResult = lResult;
return TRUE;
}
*/


   /////////////////////////////////////////////////////////////////////////////
   // interaction_impl command handling

   bool interaction_impl::OnCommand(WPARAM wParam,LPARAM lParam)
   // return TRUE if command invocation was attempted
   {
      UNREFERENCED_PARAMETER(wParam);
      UNREFERENCED_PARAMETER(lParam);
      /*   UINT nID = LOWORD(wParam);
      oswindow hWndCtrl = (oswindow)lParam;
      int nCode = HIWORD(wParam);

      // default routing for command messages (through closure table)

      if (hWndCtrl == NULL)
      {
      // zero IDs for normal commands are not allowed
      if (nID == 0)
      return FALSE;

      // make sure command has not become disabled before routing
      CTestCmdUI state;
      state.m_id = nID;
      _001OnCommand(nID, CN_UPDATE_COMMAND_UI, &state, NULL);
      if (!state.m_bEnabled)
      {
      TRACE(::core::trace::category_AppMsg, 0, "Warning: not executing disabled command %d\n", nID);
      return TRUE;
      }

      // menu or accelerator
      nCode = CN_COMMAND;
      }
      else
      {
      // control notification
      ASSERT(nID == 0 || ::WinIsWindow(hWndCtrl));

      if (gen_ThreadState->m_hLockoutNotifyWindow == get_handle())
      return TRUE;        // locked out - ignore control notification

      // reflect notification to child interaction_impl control
      if (ReflectLastMsg(hWndCtrl))
      return TRUE;    // eaten by child

      // zero IDs for normal commands are not allowed
      if (nID == 0)
      return FALSE;
      }

      #ifdef DEBUG
      if (nCode < 0 && nCode != (int)0x8000)
      TRACE(::core::trace::category_AppMsg, 0, "Implementation Warning: control notification = $%X.\n",
      nCode);
      #endif

      return _001OnCommand(nID, nCode, NULL, NULL);*/
      return FALSE;
   }

   bool interaction_impl::OnNotify(WPARAM,LPARAM lParam,LRESULT* pResult)
   {
      ASSERT(pResult != NULL);
      NMHDR* pNMHDR = (NMHDR*)lParam;
      oswindow hWndCtrl = (oswindow)pNMHDR->hwndFrom;

      // get the child ID from the interaction_impl itself
      //      uint_ptr nID = __get_dialog_control_id(hWndCtrl);
      //      int nCode = pNMHDR->code;

      ASSERT(hWndCtrl != NULL);
      ASSERT(::WinIsWindow(hWndCtrl));

      //if(gen_ThreadState->m_hLockoutNotifyWindow == get_handle())
      //   return true;        // locked out - ignore control notification

      // reflect notification to child interaction_impl control
      if(ReflectLastMsg(hWndCtrl,pResult))
         return true;        // eaten by child

      //      __NOTIFY notify;
      //    notify.pResult = pResult;
      //  notify.pNMHDR = pNMHDR;
      //xxx   return _001OnCommand((UINT)nID, MAKELONG(nCode, WM_NOTIFY), &notify, NULL);
      return false;
   }

   /////////////////////////////////////////////////////////////////////////////
   // interaction_impl extensions

   //::user::frame_window *  interaction_impl::GetParentFrame()
   //{
   //   if(get_handle() == NULL) // no Window attached
   //   {
   //      return NULL;
   //   }

   //   ASSERT_VALID(this);

   //   ::user::interaction * pParentWnd = GetParent();  // start with one parent up
   //   while(pParentWnd != NULL)
   //   {
   //      if(pParentWnd->is_frame_window())
   //      {
   //         return dynamic_cast <::user::frame_window *> (pParentWnd);
   //      }
   //      pParentWnd = pParentWnd->GetParent();
   //   }
   //   return NULL;
   //}

   ///* trans oswindow CLASS_DECL_BASE __get_parent_owner(::user::interaction * hWnd)
   //{
   //// check for permanent-owned interaction_impl first
   //::user::interaction_impl * pWnd = ::metrowin::interaction_impl::FromHandlePermanent(hWnd);
   //if (pWnd != NULL)
   //return WIN_WINDOW(pWnd)->GetOwner();

   //// otherwise, return parent in the Windows sense
   //return (::GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD) ?
   //::GetParent(hWnd) : ::GetWindow(hWnd, GW_OWNER);
   //}*/


   //::user::interaction *  interaction_impl::GetTopLevel()
   //{
   //   if(get_handle() == NULL) // no Window attached
   //      return NULL;

   //   ASSERT_VALID(this);

   //   ::user::interaction * hWndParent = this;
   //   ::user::interaction * hWndT;
   //   while((hWndT = ::user::get_parent_owner(hWndParent)) != NULL)
   //      hWndParent = hWndT;

   //   return hWndParent;
   //}

   //::user::interaction *  interaction_impl::GetTopLevelOwner()
   //{

   //   throw todo(get_app());


   //   //if (get_handle() == NULL) // no Window attached
   //   //   return NULL;

   //   //ASSERT_VALID(this);

   //   //oswindow hWndOwner = get_handle();
   //   //oswindow hWndT;
   //   //while ((hWndT = ::GetWindow(hWndOwner, GW_OWNER)) != NULL)
   //   //   hWndOwner = hWndT;

   //   //return ::metrowin::interaction_impl::from_handle(hWndOwner);
   //}

   //::user::interaction *  interaction_impl::GetParentOwner()
   //{

   //   throw todo(get_app());

   //   //if (get_handle() == NULL) // no Window attached
   //   //   return NULL;

   //   //ASSERT_VALID(this);

   //   //oswindow hWndParent = get_handle();
   //   //oswindow hWndT;
   //   //while ((::GetWindowLong(hWndParent, GWL_STYLE) & WS_CHILD) &&
   //   //   (hWndT = ::GetParent(hWndParent)) != NULL)
   //   //{
   //   //   hWndParent = hWndT;
   //   //}

   //   //return ::metrowin::interaction_impl::from_handle(hWndParent);
   //}

   //bool interaction_impl::IsTopParentActive()
   //{
   //   ASSERT(get_handle() != NULL);
   //   ASSERT_VALID(this);

   //   ::user::interaction *pWndTopLevel=EnsureTopLevelParent();

   //   return interaction_impl::GetForegroundWindow() == pWndTopLevel->GetLastActivePopup();
   //}

   void interaction_impl::ActivateTopParent()
   {

      throw todo(get_app());

      //// special activate logic for floating toolbars and palettes
      //::user::interaction_impl * pActiveWnd = GetForegroundWindow();
      //if (pActiveWnd == NULL || !(WIN_WINDOW(pActiveWnd)->get_handle() == get_handle() || ::IsChild(WIN_WINDOW(pActiveWnd)->get_handle(), get_handle())))
      //{
      //   // clicking on floating frame when it does not have
      //   // focus itself -- activate the toplevel frame instead.
      //   EnsureTopLevelParent()->SetForegroundWindow();
      //}
   }

   //::user::frame_window *  interaction_impl::GetTopLevelFrame()
   //{
   //   if(get_handle() == NULL) // no Window attached
   //      return NULL;

   //   ASSERT_VALID(this);

   //   ::user::frame_window* pFrameWnd = NULL;
   //   if(m_pui != this)
   //      pFrameWnd = dynamic_cast <::user::frame_window *> (m_pui.m_p);
   //   else
   //      pFrameWnd = dynamic_cast <::user::frame_window *> (this);
   //   if(pFrameWnd == NULL || !pFrameWnd->is_frame_window())
   //      pFrameWnd = GetParentFrame();

   //   if(pFrameWnd != NULL)
   //   {
   //      ::user:: frame_window* pTemp;
   //      while((pTemp = pFrameWnd->GetParentFrame()) != NULL)
   //         pFrameWnd = pTemp;
   //   }
   //   return pFrameWnd;
   //}

   /*   ::user::interaction_impl * interaction_impl::GetSafeOwner(::user::interaction_impl * pParent, oswindow* pWndTop)
      {
      oswindow hWnd = GetSafeOwner_((oswindow) pParent->get_os_data(), pWndTop);
      return ::metrowin::interaction_impl::from_handle(hWnd);
      }
      */
   int interaction_impl::message_box(const char * lpszText,const char * lpszCaption,UINT nType)
   {
      if(lpszCaption == NULL)
         lpszCaption = Application.m_strAppName;
      int nResult = ::simple_message_box(get_handle(),lpszText,lpszCaption,nType);
      return nResult;
   }

   ::user::interaction * interaction_impl::GetDescendantWindow(::user::interaction * hWnd,id id)
   {

      throw todo(::get_thread_app());

      //single_lock sl(&hWnd->m_pthread->m_pthread->m_mutex, TRUE);
      //// GetDlgItem recursive (return first found)
      //// breadth-first for 1 level, then depth-first for next level

      //// use GetDlgItem since it is a fast USER function
      //::user::interaction * pWndChild;
      //if ((pWndChild = hWnd->GetDlgItem(id)) != NULL)
      //{
      //   if (pWndChild->GetTopWindow() != NULL)
      //   {
      //      // children with the same ID as their parent have priority
      //      pWndChild = GetDescendantWindow(pWndChild, id);
      //      if (pWndChild != NULL)
      //         return pWndChild;
      //   }
      //   if (pWndChild != NULL)
      //      return pWndChild;
      //}

      //for(int i = 0; i < hWnd->m_uiptraChild.get_count(); i++)
      //{
      //   if(hWnd->m_uiptraChild[i]->GetDlgCtrlId() == id)
      //   {
      //      if(hWnd->m_uiptraChild[i]->GetDescendantWindow(id))
      //         return hWnd->m_uiptraChild[i]->GetDescendantWindow(id);
      //      else
      //         return hWnd->m_uiptraChild[i];
      //   }
      //}

      //if(pWndChild == NULL)
      //   return NULL;

      //// walk each child
      //for (pWndChild = pWndChild->GetTopWindow(); pWndChild != NULL;
      //   pWndChild = pWndChild->GetNextWindow(GW_HWNDNEXT))
      //{
      //   pWndChild = GetDescendantWindow(pWndChild, id);
      //   if (pWndChild != NULL)
      //      return pWndChild;
      //}
      //return NULL;    // not found
   }

   void interaction_impl::SendMessageToDescendants(oswindow hWnd,UINT message,WPARAM wParam,LPARAM lParam,bool bDeep,bool bOnlyPerm)
   {

      throw todo(::get_thread_app());

      // walk through HWNDs to avoid creating temporary interaction_impl objects
      // unless we need to call this function recursively
      //for (oswindow hWndChild = ::GetTopWindow(hWnd); hWndChild != NULL;
      //   hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
      //{
      //   // if bOnlyPerm is TRUE, don't send to non-permanent windows
      //   if (bOnlyPerm)
      //   {
      //      ::user::interaction_impl * pWnd = ::metrowin::interaction_impl::FromHandlePermanent(hWndChild);
      //      if (pWnd != NULL)
      //      {
      //         // call interaction_impl proc directly since it is a C++ interaction_impl
      //         __call_window_procedure(dynamic_cast < ::user::interaction * > (pWnd), WIN_WINDOW(pWnd)->get_handle(), message, wParam, lParam);
      //      }
      //   }
      //   else
      //   {
      //      try
      //      {
      //         // send message with Windows SendMessage API
      //         ::SendMessage(hWndChild, message, wParam, lParam);
      //      }
      //      catch(...)
      //      {
      //      }
      //   }
      //   if (bDeep && ::GetTopWindow(hWndChild) != NULL)
      //   {
      //      // send to child windows after parent
      //      try
      //      {
      //         SendMessageToDescendants(hWndChild, message, wParam, lParam,
      //            bDeep, bOnlyPerm);
      //      }
      //      catch(...)
      //      {
      //      }
      //   }
      //}

   }

   /////////////////////////////////////////////////////////////////////////////
   // Scroll bar helpers
   //  hook for interaction_impl functions
   //    only works for derived class (eg: ::view) that override 'GetScrollBarCtrl'
   // if the interaction_impl doesn't have a _visible_ windows scrollbar - then
   //   look for a sibling with the appropriate ID

   //CScrollBar* interaction_impl::GetScrollBarCtrl(int) const
   //{
   //   return NULL;        // no special scrollers supported
   //}

   int interaction_impl::SetScrollPos(int nBar,int nPos,bool bRedraw)
   {

      throw todo(get_app());

      //return ::SetScrollPos(get_handle(), nBar, nPos, bRedraw);
   }

   int interaction_impl::GetScrollPos(int nBar) const
   {

      throw todo(get_app());

      //return ::GetScrollPos(get_handle(), nBar);
   }

   void interaction_impl::SetScrollRange(int nBar,int nMinPos,int nMaxPos,bool bRedraw)
   {

      throw todo(get_app());

      //::SetScrollRange(get_handle(), nBar, nMinPos, nMaxPos, bRedraw);
   }

   void interaction_impl::GetScrollRange(int nBar,LPINT lpMinPos,LPINT lpMaxPos) const
   {

      throw todo(get_app());

      //::GetScrollRange(get_handle(), nBar, lpMinPos, lpMaxPos);
   }

   // Turn on/off non-control scrollbars
   //   for WS_?SCROLL scrollbars - show/hide them
   //   for control scrollbar - enable/disable them
   void interaction_impl::EnableScrollBarCtrl(int nBar,bool bEnable)
   {
      // WS_?SCROLL scrollbar - show or hide
      ShowScrollBar(nBar,bEnable);
   }

#ifdef WINDOWSEX

   bool interaction_impl::SetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, bool bRedraw)
   {
      ASSERT(lpScrollInfo != NULL);

      oswindow hWnd = get_handle();
      lpScrollInfo->cbSize = sizeof(*lpScrollInfo);
      ::SetScrollInfo(hWnd, nBar, lpScrollInfo, bRedraw);
      return true;
   }

   bool interaction_impl::GetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, UINT nMask)
   {
      UNREFERENCED_PARAMETER(nMask);
      ASSERT(lpScrollInfo != NULL);

      oswindow hWnd = get_handle();
      return ::GetScrollInfo(hWnd, nBar, lpScrollInfo) != FALSE;
   }

#endif

   int interaction_impl::GetScrollLimit(int nBar)
   {

#ifdef WINDOWSEX

      int nMin, nMax;
      GetScrollRange(nBar, &nMin, &nMax);
      SCROLLINFO info;
      if (GetScrollInfo(nBar, &info, SIF_PAGE))
      {
         nMax -= __max(info.nPage-1,0);
      }
      return nMax;

#endif

      return 0;

   }

   void interaction_impl::ScrollWindow(int xAmount,int yAmount,LPCRECT lpRect,LPCRECT lpClipRect)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));

      //if (IsWindowVisible() || lpRect != NULL || lpClipRect != NULL)
      //{
      //   // When visible, let Windows do the scrolling
      //   ::ScrollWindow(get_handle(), xAmount, yAmount, lpRect, lpClipRect);
      //}
      //else
      //{
      //   // Windows does not perform any scrolling if the interaction_impl is
      //   // not visible.  This leaves child windows unscrolled.
      //   // To account for this oversight, the child windows are moved
      //   // directly instead.
      //   oswindow hWndChild = ::GetWindow(get_handle(), GW_CHILD);
      //   if (hWndChild != NULL)
      //   {
      //      for (; hWndChild != NULL;
      //         hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
      //      {
      //         rect rect;
      //         ::GetWindowRect(hWndChild, &rect);
      //         ScreenToClient(&rect);
      //         ::SetWindowPos(hWndChild, NULL,
      //            rect.left+xAmount, rect.top+yAmount, 0, 0,
      //            SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER);
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
   ::user::interaction * hWndLeftOver = NULL;

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

   if(m_pui != NULL)
   {
   for (::user::interaction * hWndChild = m_pui->GetTopWindow(); hWndChild != NULL;
   hWndChild = hWndChild->GetNextWindow(GW_HWNDNEXT))
   {
   string strIdc = hWndChild->GetDlgCtrlId();
   ::user::interaction * pWnd = hWndChild;
   if (strIdc == pszIdLeftOver)
   hWndLeftOver = hWndChild;
   else if (::str::begins(strIdc, pszPrefix) && pWnd != NULL)
   hWndChild->SendMessage(WM_SIZEPARENT, 0, (LPARAM)&on_layout);
   }
   for (int i = 0; i < m_pui->m_uiptra.get_count();   i++)
   {
   ::user::interaction * hWndChild = m_pui->m_uiptra[i];
   string strIdc = hWndChild->GetDlgCtrlId();
   ::user::interaction * pWnd = hWndChild;
   if (strIdc == pszIdLeftOver)
   hWndLeftOver = hWndChild;
   else if (::str::begins(strIdc, pszPrefix) && pWnd != NULL)
   hWndChild->SendMessage(WM_SIZEPARENT, 0, (LPARAM)&on_layout);
   }
   }
   else
   {
   for (::user::interaction * hWndChild = GetTopWindow(); hWndChild != NULL;
   hWndChild = hWndChild->GetNextWindow(GW_HWNDNEXT))
   {
   string strIdc = hWndChild->GetDlgCtrlId();
   ::user::interaction * pWnd = hWndChild;
   if (strIdc == pszIdLeftOver)
   hWndLeftOver = hWndChild;
   else if (::str::begins(strIdc, pszPrefix) && pWnd != NULL)
   hWndChild->SendMessage(WM_SIZEPARENT, 0, (LPARAM)&on_layout);
   }
   for (int i = 0; i < m_uiptra.get_count();   i++)
   {
   ::user::interaction * hWndChild = m_uiptra[i];
   string strIdc = hWndChild->GetDlgCtrlId();
   ::user::interaction * pWnd = hWndChild;
   if (strIdc == pszIdLeftOver)
   hWndLeftOver = hWndChild;
   else if (::str::begins(strIdc, pszPrefix) && pWnd != NULL)
   hWndChild->SendMessage(WM_SIZEPARENT, 0, (LPARAM)&on_layout);
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
   if (pszIdLeftOver != NULL && hWndLeftOver != NULL)
   {
   ::user::interaction * pLeftOver = hWndLeftOver;
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

   // move and resize all the windows at once!
   if (on_layout.hDWP == NULL || !::EndDeferWindowPos(on_layout.hDWP))
   TRACE(::core::trace::category_AppMsg, 0, "Warning: DeferWindowPos failed - low system resources.\n");
   }

   */

   //void interaction_impl::RepositionBars(UINT nIDFirst,UINT nIDLast,id nIdLeftOver,UINT nFlags,LPRECT lpRectParam,LPCRECT lpRectClient,bool bStretch)
   //{


   //   throw todo(get_app());


   //   //UNREFERENCED_PARAMETER(nIDFirst);
   //   //UNREFERENCED_PARAMETER(nIDLast);

   //   //ASSERT(nFlags == 0 || (nFlags & ~reposNoPosLeftOver) == reposQuery ||
   //   //   (nFlags & ~reposNoPosLeftOver) == reposExtra);

   //   //// walk kids in order, control bars get the resize notification
   //   ////   which allow them to shrink the client area
   //   //// remaining size goes to the 'nIDLeftOver' pane
   //   //// NOTE: nIDFirst->nIDLast are usually 0->0xffff

   //   //__SIZEPARENTPARAMS on_layout;
   //   //::user::interaction * hWndLeftOver = NULL;

   //   //on_layout.bStretch = bStretch;
   //   //on_layout.sizeTotal.cx = on_layout.sizeTotal.cy = 0;
   //   //if (lpRectClient != NULL)
   //   //   on_layout.rect = *lpRectClient;    // starting rect comes from parameter
   //   //else
   //   //{
   //   //   if(m_pui != this)
   //   //      m_pui->GetClientRect(&on_layout.rect);    // starting rect comes from client rect
   //   //   else
   //   //      GetClientRect(&on_layout.rect);    // starting rect comes from client rect
   //   //}

   //   //if ((nFlags & ~reposNoPosLeftOver) != reposQuery)
   //   //   on_layout.hDWP = ::BeginDeferWindowPos(8); // reasonable guess
   //   //else
   //   //   on_layout.hDWP = NULL; // not actually doing on_layout

   //   //if(m_pui != NULL)
   //   //{
   //   //   for (::user::interaction * hWndChild = m_pui->GetTopWindow(); hWndChild != NULL;
   //   //      hWndChild = hWndChild->GetNextWindow(GW_HWNDNEXT))
   //   //   {
   //   //      id id = hWndChild->GetDlgCtrlId();
   //   //      ::user::interaction * pWnd = hWndChild;
   //   //      if (id == (int) nIdLeftOver)
   //   //         hWndLeftOver = hWndChild;
   //   //      else if (pWnd != NULL)
   //   //         hWndChild->send_message(WM_SIZEPARENT, 0, (LPARAM)&on_layout);
   //   //   }
   //   //   for (::user::interaction * hWndChild = m_pui->get_top_child(); hWndChild != NULL;
   //   //      hWndChild = hWndChild->under_sibling())
   //   //   {
   //   //      id id = hWndChild->GetDlgCtrlId();
   //   //      ::user::interaction * pWnd = hWndChild;
   //   //      if (id == nIdLeftOver)
   //   //         hWndLeftOver = hWndChild;
   //   //      else if (pWnd != NULL)
   //   //         hWndChild->send_message(WM_SIZEPARENT, 0, (LPARAM)&on_layout);
   //   //   }
   //   //}
   //   //else
   //   //{
   //   //   for (::user::interaction * hWndChild = GetTopWindow(); hWndChild != NULL;
   //   //      hWndChild = hWndChild->GetNextWindow(GW_HWNDNEXT))
   //   //   {
   //   //      id id = hWndChild->GetDlgCtrlId();
   //   //      ::user::interaction * pWnd = hWndChild;
   //   //      if (id == nIdLeftOver)
   //   //         hWndLeftOver = hWndChild;
   //   //      else if (pWnd != NULL)
   //   //         hWndChild->send_message(WM_SIZEPARENT, 0, (LPARAM)&on_layout);
   //   //   }
   //   //   for (::user::interaction * hWndChild = m_pui->get_top_child(); hWndChild != NULL;
   //   //      hWndChild = hWndChild->under_sibling())
   //   //   {
   //   //      id id = hWndChild->GetDlgCtrlId();
   //   //      ::user::interaction * pWnd = hWndChild;
   //   //      if (id == nIdLeftOver)
   //   //         hWndLeftOver = hWndChild;
   //   //      else if (pWnd != NULL)
   //   //         hWndChild->send_message(WM_SIZEPARENT, 0, (LPARAM)&on_layout);
   //   //   }
   //   //}

   //   //// if just getting the available rectangle, return it now...
   //   //if ((nFlags & ~reposNoPosLeftOver) == reposQuery)
   //   //{
   //   //   ASSERT(lpRectParam != NULL);
   //   //   if (bStretch)
   //   //      ::CopyRect(lpRectParam, &on_layout.rect);
   //   //   else
   //   //   {
   //   //      lpRectParam->left = lpRectParam->top = 0;
   //   //      lpRectParam->right = on_layout.sizeTotal.cx;
   //   //      lpRectParam->bottom = on_layout.sizeTotal.cy;
   //   //   }
   //   //   return;
   //   //}

   //   //// the rest is the client size of the left-over pane
   //   //if(nIdLeftOver != NULL && hWndLeftOver != NULL)
   //   //{
   //   //   ::user::interaction * pLeftOver = hWndLeftOver;
   //   //   // allow extra space as specified by lpRectBorder
   //   //   if ((nFlags & ~reposNoPosLeftOver) == reposExtra)
   //   //   {
   //   //      ASSERT(lpRectParam != NULL);
   //   //      on_layout.rect.left += lpRectParam->left;
   //   //      on_layout.rect.top += lpRectParam->top;
   //   //      on_layout.rect.right -= lpRectParam->right;
   //   //      on_layout.rect.bottom -= lpRectParam->bottom;
   //   //   }
   //   //   // reposition the interaction_impl
   //   //   if ((nFlags & reposNoPosLeftOver) != reposNoPosLeftOver)
   //   //   {
   //   //      pLeftOver->CalcWindowRect(&on_layout.rect);
   //   //      __reposition_window(&on_layout, pLeftOver, &on_layout.rect);
   //   //   }
   //   //}

   //   //// move and resize all the windows at once!
   //   //if (on_layout.hDWP == NULL || !::EndDeferWindowPos(on_layout.hDWP))
   //   //   TRACE(::core::trace::category_AppMsg, 0, "Warning: DeferWindowPos failed - low system resources.\n");
   //}



   void interaction_impl::CalcWindowRect(LPRECT lpClientRect,UINT nAdjustType)
   {

      throw todo(get_app());


      //uint32_t dwExStyle = GetExStyle();
      //if (nAdjustType == 0)
      //   dwExStyle &= ~WS_EX_CLIENTEDGE;
      //::AdjustWindowRectEx(lpClientRect, GetStyle(), FALSE, dwExStyle);
   }

   /////////////////////////////////////////////////////////////////////////////
   // Special keyboard/system command processing

   bool interaction_impl::HandleFloatingSysCommand(UINT nID,LPARAM lParam)
   {


      throw todo(get_app());


      //::user::interaction* pParent = GetTopLevel();
      //switch (nID & 0xfff0)
      //{
      //case SC_PREVWINDOW:
      //case SC_NEXTWINDOW:
      //   if (LOWORD(lParam) == VK_F6 && pParent != NULL)
      //   {
      //      pParent->SetFocus();
      //      return true;
      //   }
      //   break;

      //case SC_CLOSE:
      //case SC_KEYMENU:
      //   // Check lParam.  If it is 0L, then the ::fontopus::user may have done
      //   // an Alt+Tab, so just ignore it.  This breaks the ability to
      //   // just press the Alt-key and have the first menu selected,
      //   // but this is minor compared to what happens in the Alt+Tab
      //   // case.
      //   if ((nID & 0xfff0) == SC_CLOSE || lParam != 0L)
      //   {
      //      if (pParent != NULL)
      //      {
      //         // Sending the above WM_SYSCOMMAND may destroy the cast,
      //         // so we have to be careful about restoring activation
      //         // and focus after sending it.
      //         oswindow hWndSave = get_handle();
      //         oswindow hWndFocus = ::GetFocus();
      //         pParent->SetActiveWindow();
      //         pParent->send_message(WM_SYSCOMMAND, nID, lParam);

      //         // be very careful here...
      //         if (::WinIsWindow(hWndSave))
      //            ::SetActiveWindow(hWndSave);
      //         if (::WinIsWindow(hWndFocus))
      //            ::SetFocus(hWndFocus);
      //      }
      //   }
      //   return true;
      //}
      //return false;
   }

   void interaction_impl::WalkPreTranslateTree(::user::interaction * puiStop,signal_details * pobj)
   {
      ASSERT(puiStop == NULL || puiStop->IsWindow());
      ASSERT(pobj != NULL);

      SCAST_PTR(::message::base,pbase,pobj);
      // walk from the target interaction_impl up to the hWndStop interaction_impl checking
      //  if any interaction_impl wants to translate this message

      for(::user::interaction * pui = (::user::interaction *) pbase->m_pwnd->m_pvoidUserInteraction; pui != NULL; pui->GetParent())
      {

         pui->pre_translate_message(pobj);

         if(pobj->m_bRet)
            return; // trapped by target interaction_impl (eg: accelerators)

         // got to hWndStop interaction_impl without interest
         if(pui == puiStop)
            break;

      }
      // no special processing
   }

   bool interaction_impl::SendChildNotifyLastMsg(LRESULT* pResult)
   {
      //___THREAD_STATE* pThreadState = gen_ThreadState.get_data();
      //return OnChildNotify(pThreadState->m_lastSentMsg.message,
      //   pThreadState->m_lastSentMsg.wParam,pThreadState->m_lastSentMsg.lParam,pResult);
      return false;
   }

   bool interaction_impl::ReflectLastMsg(oswindow hWndChild,LRESULT* pResult)
   {
      return false;
      //// get the ::map, and if no ::map, then this message does not need reflection
      //single_lock sl(afxMutexHwnd(),TRUE);
      //hwnd_map * pMap = afxMapHWND();
      //if(pMap == NULL)
      //   return FALSE;

      //// check if in permanent ::map, if it is reflect it (could be OLE control)
      //::user::interaction * pWnd = hWndChild->interaction_impl();
      //if(pWnd == NULL)
      //{
      //   return false;
      //}

      //// only OLE controls and permanent windows will get reflected msgs
      //ASSERT(pWnd != NULL);
      //return WIN_WINDOW(pWnd)->SendChildNotifyLastMsg(pResult);
   }

   bool interaction_impl::OnChildNotify(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT* pResult)
   {

      return ReflectChildNotify(uMsg,wParam,lParam,pResult);
   }

   bool interaction_impl::ReflectChildNotify(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT* pResult)
   {
      UNREFERENCED_PARAMETER(wParam);
      // Note: reflected messages are send directly to interaction_impl::OnWndMsg
      //  and interaction_impl::_001OnCommand for speed and because these messages are not
      //  routed by normal _001OnCommand routing (they are only dispatched)

      switch(uMsg)
      {
      // normal messages (just wParam, lParam through OnWndMsg)
      case WM_HSCROLL:
      case WM_VSCROLL:
      case WM_PARENTNOTIFY:
      case WM_DRAWITEM:
      case WM_MEASUREITEM:
      case WM_DELETEITEM:
      case WM_VKEYTOITEM:
      case WM_CHARTOITEM:
      case WM_COMPAREITEM:
         // reflect the message through the message ::map as WM_REFLECT_BASE+uMsg
         //return interaction_impl::OnWndMsg(WM_REFLECT_BASE+uMsg, wParam, lParam, pResult);
         return FALSE;

      // special case for WM_COMMAND
      case WM_COMMAND:
         {
            // reflect the message through the message ::map as OCM_COMMAND
            /* xxx         int nCode = HIWORD(wParam);
            if (interaction_impl::_001OnCommand(0, MAKELONG(nCode, WM_REFLECT_BASE+WM_COMMAND), NULL, NULL))
            {
            if (pResult != NULL)
            *pResult = 1;
            return TRUE;
            } */
         }
         break;

      // special case for WM_NOTIFY
      case WM_NOTIFY:
         {
            // reflect the message through the message ::map as OCM_NOTIFY
            NMHDR* pNMHDR = (NMHDR*)lParam;
            //            int nCode = pNMHDR->code;
            //            __NOTIFY notify;
            //          notify.pResult = pResult;
            //        notify.pNMHDR = pNMHDR;
            // xxxx         return interaction_impl::_001OnCommand(0, MAKELONG(nCode, WM_REFLECT_BASE+WM_NOTIFY), &notify, NULL);
         }

      // other special cases (WM_CTLCOLOR family)
      default:
         if(uMsg >= WM_CTLCOLORMSGBOX && uMsg <= WM_CTLCOLORSTATIC)
         {
            // fill in special struct for compatiblity with 16-bit WM_CTLCOLOR
            /*__CTLCOLOR ctl;
            ctl.hDC = (HDC)wParam;
            ctl.nCtlType = uMsg - WM_CTLCOLORMSGBOX;
            //ASSERT(ctl.nCtlType >= CTLCOLOR_MSGBOX);
            ASSERT(ctl.nCtlType <= CTLCOLOR_STATIC);

            // reflect the message through the message ::map as OCM_CTLCOLOR
            bool bResult = interaction_impl::OnWndMsg(WM_REFLECT_BASE+WM_CTLCOLOR, 0, (LPARAM)&ctl, pResult);
            if ((HBRUSH)*pResult == NULL)
            bResult = FALSE;
            return bResult;*/
            return false;
         }
         break;
      }

      return false;   // let the parent handle it
   }

   void interaction_impl::OnParentNotify(UINT message,LPARAM lParam)
   {
      if((LOWORD(message) == WM_CREATE || LOWORD(message) == WM_DESTROY))
      {
         if(ReflectLastMsg((oswindow)lParam))
            return;     // eat it
      }
      // not handled - do default
      Default();
   }

   void interaction_impl::OnSetFocus(::user::interaction_impl *)
   {
      bool bHandled;

      bHandled = FALSE;
      if(!bHandled)
      {
         Default();
      }
   }

   LRESULT interaction_impl::OnActivateTopLevel(WPARAM wParam,LPARAM)
   {
      if(LOWORD(wParam) == WA_INACTIVE)
      {
         //         __MODULE_THREAD_STATE* pModuleThreadState = __get_module_thread_state();
      }

      return 0;
   }

   void interaction_impl::OnSysColorChange()
   {
      throw not_implemented(get_app());

      /*      application* pApp = &System;
      if (pApp != NULL && pApp->GetMainWnd() == this)
      {
      // recolor global brushes used by control bars
      afxData.UpdateSysColors();
      }

      // forward this message to all other child windows
      if (!(GetStyle() & WS_CHILD))
      SendMessageToDescendants(WM_SYSCOLORCHANGE, 0, 0L, TRUE, TRUE);

      Default();*/
   }

   bool gen_GotScrollLines;

   void interaction_impl::OnSettingChange(UINT uFlags,const char * lpszSection)
   {
      UNUSED_ALWAYS(uFlags);
      UNUSED_ALWAYS(lpszSection);

      // force refresh of settings that we cache
      gen_GotScrollLines = FALSE;


      interaction_impl::OnDisplayChange(0,0);    // to update system metrics, etc.
   }

   void interaction_impl::OnDevModeChange(char * lpDeviceName)
   {
      UNREFERENCED_PARAMETER(lpDeviceName);
      throw not_implemented(get_app());
      /*application* pApp = &System;
      if (pApp != NULL && pApp->GetMainWnd() == this)
      pApp->DevModeChange(lpDeviceName);

      // forward this message to all other child windows
      if (!(GetStyle() & WS_CHILD))
      {
      const MSG* pMsg = GetCurrentMessage();
      SendMessageToDescendants(pMsg->message, pMsg->wParam, pMsg->lParam,
      TRUE, TRUE);
      }*/
   }


#ifdef WINDOWSEX

   bool interaction_impl::OnHelpInfo(HELPINFO* /*pHelpInfo*/)
   {
      if (!(GetStyle() & WS_CHILD))
      {
         ::user::interaction* pMainWnd = System.GetMainWnd();
         if (pMainWnd != NULL &&
               GetKeyState(VK_SHIFT) >= 0 &&
               GetKeyState(VK_CONTROL) >= 0 &&
               GetKeyState(VK_MENU) >= 0)
         {
            //            pMainWnd->SendMessage(WM_COMMAND, ID_HELP);
            return TRUE;
         }
      }
      return Default() != 0;
   }

#endif

   LRESULT interaction_impl::OnDisplayChange(WPARAM,LPARAM)
   {
      // update metrics if this interaction_impl is the main interaction_impl
      //if(System.GetMainWnd() == this)
      //{
      //   // update any system metrics cache
      //   //         afxData.UpdateSysMetrics();
      //}

      // forward this message to all other child windows
      if(!(GetStyle() & WS_CHILD))
      {
         const MSG* pMsg = GetCurrentMessage();
         SendMessageToDescendants(pMsg->message,pMsg->wParam,pMsg->lParam,
                                  TRUE,TRUE);
      }

      return Default();
   }

   LRESULT interaction_impl::OnDragList(WPARAM,LPARAM lParam)
   {

      throw todo(get_app());

      //LPDRAGLISTINFO lpInfo = (LPDRAGLISTINFO)lParam;
      //ASSERT(lpInfo != NULL);

      //LRESULT lResult;
      //if (ReflectLastMsg(lpInfo->hWnd, &lResult))
      //   return (int)lResult;    // eat it

      //// not handled - do default
      //return (int)Default();
   }

   void interaction_impl::_001OnCreate(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      Default();


   }

   //void interaction_impl::OnHScroll(UINT,UINT,CScrollBar* pScrollBar)
   //{
   //   UNREFERENCED_PARAMETER(pScrollBar);
   //   Default();
   //}

   //void interaction_impl::OnVScroll(UINT,UINT,CScrollBar* pScrollBar)
   //{
   //   UNREFERENCED_PARAMETER(pScrollBar);
   //   Default();
   //}

   BOOL CALLBACK interaction_impl::GetAppsEnumWindowsProc(oswindow hwnd,LPARAM lParam)
   {
      user::oswindow_array * phwnda = (user::oswindow_array *) lParam;
      phwnda->add(hwnd);
      return TRUE;
   }

   void interaction_impl::get_app_wnda(user::oswindow_array & wnda)
   {

      throw todo(::get_thread_app());

      //      EnumWindows(GetAppsEnumWindowsProc, (LPARAM) &wnda);
   }

   /*   void interaction_impl::_001OnDeferPaintLayeredWindowBackground(::draw2d::dib * pdib)
   {
   _001DeferPaintLayeredWindowBackground(pgraphics);
   }*/


   class print_window:
      virtual ::object
   {
   public:



      manual_reset_event m_event;
      oswindow m_hwnd;
      HDC m_hdc;

      print_window(::aura::application * papp,oswindow hwnd,HDC hdc,uint32_t dwTimeout):
         ::object(papp),
         m_event(papp)
      {
         m_event.ResetEvent();
         m_hwnd = hwnd;
         m_hdc = hdc;
         __begin_thread(papp,&print_window::s_print_window,(LPVOID) this,::multithreading::priority_above_normal);
         if(m_event.wait(millis(dwTimeout)).timeout())
         {
            TRACE("print_window::time_out");
         }
      }


      static_function UINT c_cdecl s_print_window(LPVOID pvoid)
      {
         throw todo(::get_thread_app());

         //print_window * pprintwindow = (print_window *) pvoid;
         //try
         //{
         //   HANDLE hevent = (HANDLE) pprintwindow->m_event.get_os_data();
         //   ::PrintWindow(pprintwindow->m_hwnd, pprintwindow->m_hdc, 0);
         //   ::SetEvent(hevent);
         //}
         //catch(...)
         //{
         //}
         //return 0;
      }
   };

   void interaction_impl::_001DeferPaintLayeredWindowBackground(HDC hdc)
   {

#ifdef WINDOWSEX
      rect rectClient;

      GetClientRect(rectClient);


      //pgraphics->FillSolidRect(rectClient, 0x00000000);

      //return;
      rect rectUpdate;
      GetWindowRect(rectUpdate);
      SetViewportOrgEx(hdc, 0, 0, NULL);
      rect rectPaint;
      rectPaint = rectUpdate;
      ScreenToClient(rectPaint);
      user::oswindow_array wndaApp;


      HRGN rgnWindow;
      HRGN rgnIntersect;
      HRGN rgnUpdate = NULL;


      rgnWindow = CreateRectRgn(0, 0, 0, 0);
      rgnIntersect = CreateRectRgn(0, 0, 0, 0);

      //      int iCount = wndaApp.get_count();

      try
      {

         if(GetWindowLong(GWL_EXSTYLE) & WS_EX_LAYERED)
         {
            rect rect5;
            rect rect9;

            rgnUpdate = CreateRectRgnIndirect(&rectUpdate);
            oswindow hwndOrder = ::GetWindow(get_handle(), GW_HWNDNEXT);
            for(;;)
            {
               //            char szText[1024];
               //::GetWindowTextA(hwndOrder, szText, sizeof(szText));
               if(hwndOrder == NULL ||
                     !::WinIsWindow(hwndOrder))
                  break;
               if(!::IsWindowVisible(hwndOrder) ||
                     ::IsIconic(hwndOrder) ||
                     hwndOrder == get_handle()
                     || wndaApp.contains(hwndOrder))
               {
                  if(hwndOrder == get_handle())
                  {
                     // add as bookmark - doesn't paint it
                     wndaApp.add(hwndOrder);
                  }
               }
               else
               {
                  rect rectWindow;
                  ::GetWindowRect(hwndOrder, rectWindow);
                  SetRectRgn(rgnWindow, rectWindow.left, rectWindow.top, rectWindow.right, rectWindow.bottom);
                  SetRectRgn(rgnIntersect, 0, 0, 0, 0);
                  CombineRgn(rgnIntersect, rgnUpdate, rgnWindow, RGN_AND);
                  rect rectIntersectBox;
                  GetRgnBox(rgnIntersect, rectIntersectBox);
                  if(rectIntersectBox.is_empty())
                  {
                  }
                  else
                  {
                     CombineRgn(rgnUpdate, rgnUpdate, rgnWindow, RGN_DIFF);
                     rect rectDiffBox;
                     GetRgnBox(rgnUpdate, rectDiffBox);
                     wndaApp.add(hwndOrder);
                     if(rectDiffBox.is_empty())
                     {
                        break;
                     }
                  }
               }
               hwndOrder = ::GetWindow(hwndOrder, GW_HWNDNEXT);


            }
            for(index j = wndaApp.get_upper_bound(); j >= 0; j--)
            {
               oswindow hWnd = wndaApp[j];
               if(hWnd == get_handle())
                  break;
               if(!::IsWindowVisible(hWnd) || ::IsIconic(hWnd))
                  continue;
               ::GetWindowRect(hWnd, rect5);
               rect9.intersect(rect5, rectUpdate);
               if(rect9.width() >0 && rect9.height() > 0)
               {
                  /*::user::interaction_impl * pwnd = dynamic_cast < ::user::interaction_impl * > (interaction_impl::FromHandlePermanent(hWnd));
                  if(pwnd == NULL)
                  {
                  for(int l = 0; l < wndpa.get_count(); l++)
                  {
                  if(wndpa[l]->get_safe_handle() == hWnd)
                  {
                  pwnd = dynamic_cast < ::user::interaction_impl * > (wndpa[l]->m_pimpl);
                  break;
                  }
                  }
                  }
                  if(pwnd != NULL)
                  {
                  pwnd->_001Print(pgraphics);
                  }*/
                  //if(::GetWindowLong(wndaApp[j], GWL_EXSTYLE) & WS_EX_LAYERED)
                  if(true)
                  {
                     HDC hDCMem = CreateCompatibleDC(NULL);
                     HBITMAP hBmp = NULL;
                     {
                        HDC hDC = ::GetWindowDC(hWnd);
                        hBmp = CreateCompatibleBitmap(hDC, rect5.width(), rect5.height());
                        ::ReleaseDC(hWnd, hDC);
                     }
                     HGDIOBJ hOld = SelectObject(hDCMem, hBmp);
                     //print_window printwindow(get_app(), hWnd, hDCMem, 284);
                     ::PrintWindow(hWnd, hDCMem, 0);
                     ::BitBlt(
                        hdc ,
                        //rect5.left,
                        //rect5.top,
                        0, 0,
                        rect5.width(), rect5.height(),
                        hDCMem,
                        rectUpdate.left - rect5.left,
                        rectUpdate.top - rect5.top,
                        SRCCOPY);
                     ::SelectObject(hDCMem, hOld);
                     ::DeleteObject(hDCMem);
                     ::DeleteObject(hBmp);
                  }
                  else
                  {
                     SetViewportOrgEx(hdc, 0, 0, NULL);
                     HDC hdcWindow = ::GetDCEx(wndaApp[j], NULL, DCX_WINDOW);
                     if(hdcWindow == NULL)
                        hdcWindow = ::GetDCEx(wndaApp[j], NULL, DCX_WINDOW | DCX_CACHE);
                     if(hdcWindow != NULL)
                     {
                        ::BitBlt(
                           hdc,
                           rect5.left - rectUpdate.left,
                           rect5.top - rectUpdate.top,
                           rect5.width(), rect5.height(),
                           hdcWindow,
                           rect5.left - rect5.left,
                           rect5.top - rect5.top,
                           SRCCOPY);
                        ::ReleaseDC(wndaApp[j], hdcWindow);
                     }
                     else
                     {
                        TRACE0("WARNING: failed to draw a background. this surface probably will be black.");
                     }
                  }
               }
            }
         }
      }
      catch(...)
      {

      }

      ::DeleteObject(rgnWindow);
      ::DeleteObject(rgnIntersect);
      ::DeleteObject(rgnUpdate);
#endif
   }

   void interaction_impl::_001OnProdevianSynch(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      //      System.get_event(m_pthread)->SetEvent();
      //    System.get_event(System.get_twf())->wait(millis(8400));
   }

   void interaction_impl::_001OnPaint(signal_details * pobj)
   {

      //lock lock(m_pui, 1984);
      throw todo(get_app());

      //SCAST_PTR(::message::base, pbase, pobj);

      //PAINTSTRUCT paint;
      //memset(&paint, 0, sizeof(paint));
      //HDC hdc = ::BeginPaint(get_handle(), &paint);
      //::SelectClipRgn(hdc, NULL);

      //try
      //{

      //   ::draw2d::dib_sp dib(get_app());

      //   rect rectWindow;
      //   GetWindowRect(rectWindow);

      //   if(!dib->create(rectWindow.bottom_right()))
      //      return;

      //   ::draw2d::graphics * pgraphics = dib->get_graphics();

      //   if((dynamic_cast<::metrowin::graphics * >(pgraphics))->get_os_data() == NULL
      //      || (dynamic_cast<::metrowin::graphics * >(pgraphics))->get_handle2() == NULL)
      //      return;

      //   rect rectPaint;
      //   rect rectUpdate;
      //   rectPaint = paint.rcPaint;
      //   if(rectPaint.is_null() || (GetExStyle() & WS_EX_LAYERED))
      //   {
      //      rectUpdate = rectWindow;
      //      rectPaint = rectWindow;
      //      ScreenToClient(rectPaint);
      //   }
      //   else
      //   {
      //      rectUpdate = rectPaint;
      //      ClientToScreen(rectUpdate);
      //   }
      //   (dynamic_cast<::metrowin::graphics * >(pgraphics))->SelectClipRgn(NULL);
      //   if(m_pui != NULL)
      //   {
      //      m_pui->_001OnDeferPaintLayeredWindowBackground(pgraphics);
      //   }
      //   else
      //   {
      //      _001OnDeferPaintLayeredWindowBackground(pgraphics);
      //   }
      //   (dynamic_cast<::metrowin::graphics * >(pgraphics))->SelectClipRgn(NULL);
      //   (dynamic_cast<::metrowin::graphics * >(pgraphics))->SetViewportOrg(point(0, 0));
      //   _000OnDraw(pgraphics);
      //   (dynamic_cast<::metrowin::graphics * >(pgraphics))->SetViewportOrg(point(0, 0));
      //   //(dynamic_cast<::metrowin::graphics * >(pgraphics))->FillSolidRect(rectUpdate.left, rectUpdate.top, 100, 100, 255);
      //   (dynamic_cast<::metrowin::graphics * >(pgraphics))->SelectClipRgn(NULL);
      //   (dynamic_cast<::metrowin::graphics * >(pgraphics))->SetViewportOrg(point(0, 0));
      //   BitBlt(hdc, rectPaint.left, rectPaint.top,
      //      rectPaint.width(), rectPaint.height(),
      //      (HDC) pgraphics->get_os_data(), rectUpdate.left, rectUpdate.top,
      //      SRCCOPY);

      //}
      //catch(...)
      //{
      //}

      //::EndPaint(get_handle(), &paint);
      //pobj->m_bRet = true;
      //pbase->set_lresult(0);
   }


   void interaction_impl::_001OnPrint(signal_details * pobj)
   {
      throw todo(get_app());

      //SCAST_PTR(::message::base, pbase, pobj);

      //if(pbase->m_wparam == NULL)
      //   return;

      //::draw2d::graphics_sp graphics(get_app());
      //METROWIN_DC(graphics.m_p)->Attach((HDC) pbase->m_wparam);
      //rect rectx;
      //::draw2d::bitmap * pbitmap = &graphics->GetCurrentBitmap();
      //::GetCurrentObject((HDC) pbase->m_wparam, OBJ_BITMAP);
      ////      uint32_t dw = ::GetLastError();
      //class size size = pbitmap->get_size();
      //rectx.left = 0;
      //rectx.top = 0;
      //rectx.right = size.cx;
      //rectx.bottom = size.cy;
      //try
      //{
      //   rect rectWindow;
      //   GetWindowRect(rectWindow);

      //   ::draw2d::dib_sp dib(get_app());
      //   if(!dib->create(rectWindow.bottom_right()))
      //      return;

      //   ::draw2d::graphics * pgraphics = dib->get_graphics();

      //   if(pgraphics->get_os_data() == NULL)
      //      return;

      //   rect rectPaint;
      //   rect rectUpdate;
      //   rectUpdate = rectWindow;
      //   rectPaint = rectWindow;
      //   rectPaint.offset(-rectPaint.top_left());
      //   (dynamic_cast<::metrowin::graphics * >(pgraphics))->SelectClipRgn(NULL);
      //   if(m_pui != NULL)
      //   {
      //      m_pui->_001OnDeferPaintLayeredWindowBackground(pgraphics);
      //   }
      //   else
      //   {
      //      _001OnDeferPaintLayeredWindowBackground(pgraphics);
      //   }
      //   (dynamic_cast<::metrowin::graphics * >(pgraphics))->SelectClipRgn(NULL);
      //   (dynamic_cast<::metrowin::graphics * >(pgraphics))->SetViewportOrg(point(0, 0));
      //   _000OnDraw(pgraphics);
      //   (dynamic_cast<::metrowin::graphics * >(pgraphics))->SetViewportOrg(point(0, 0));
      //   //(dynamic_cast<::metrowin::graphics * >(pgraphics))->FillSolidRect(rectUpdate.left, rectUpdate.top, 100, 100, 255);
      //   (dynamic_cast<::metrowin::graphics * >(pgraphics))->SelectClipRgn(NULL);
      //   (dynamic_cast<::metrowin::graphics * >(pgraphics))->SetViewportOrg(point(0, 0));

      //   graphics->SelectClipRgn( NULL);
      //   graphics->BitBlt(rectPaint.left, rectPaint.top,
      //      rectPaint.width(), rectPaint.height(),
      //      pgraphics, rectUpdate.left, rectUpdate.top,
      //      SRCCOPY);

      //   graphics->text_out(0, 0, "Te Amo Carlinhos!!", 11);
      //}
      //catch(...)
      //{
      //}
      //graphics->FillSolidRect(rectx, RGB(255, 255, 255));
      //METROWIN_DC(graphics.m_p)->Detach();
      //pobj->m_bRet = true;
      //pbase->set_lresult(0);
   }


   void interaction_impl::OnEnterIdle(UINT /*nWhy*/,::user::interaction_impl * /*pWho*/)
   {

      throw todo(get_app());


      //// In some OLE inplace active scenarios, OLE will post a
      //// message instead of sending it.  This causes so many WM_ENTERIDLE
      //// messages to be sent that tasks running in the background stop
      //// running.  By dispatching the pending WM_ENTERIDLE messages
      //// when the first one is received, we trick Windows into thinking
      //// that only one was really sent and dispatched.
      //{
      //   MSG msg;
      //   while (PeekMessage(&msg, NULL, WM_ENTERIDLE, WM_ENTERIDLE, PM_REMOVE))
      //      DispatchMessage(&msg);
      //}

      //Default();
   }

   HBRUSH interaction_impl::OnCtlColor(::draw2d::graphics *,::user::interaction_impl * pWnd,UINT)
   {
      ASSERT(pWnd != NULL && WIN_WINDOW(pWnd)->get_handle() != NULL);
      LRESULT lResult;
      if(WIN_WINDOW(pWnd)->SendChildNotifyLastMsg(&lResult))
         return (HBRUSH)lResult;     // eat it
      return (HBRUSH)Default();
   }

   // implementation of OnCtlColor for default gray backgrounds
   //   (works for any interaction_impl containing controls)
   //  return value of FALSE means caller must call DefWindowProc's default
   //  TRUE means that 'hbrGray' will be used and the appropriate text
   //    ('clrText') and background colors are set.
   bool interaction_impl::GrayCtlColor(HDC hDC,oswindow hWnd,UINT nCtlColor,HBRUSH hbrGray,COLORREF clrText)
   {

      //if (hDC == NULL)
      //{
      //   // sometimes Win32 passes a NULL hDC in the WM_CTLCOLOR message.
      //   //         TRACE(::core::trace::category_AppMsg, 0, "Warning: hDC is NULL in interaction_impl::GrayCtlColor; WM_CTLCOLOR not processed.\n");
      //   return FALSE;
      //}

      //if (hbrGray == NULL ||
      //   nCtlColor == CTLCOLOR_EDIT || nCtlColor == CTLCOLOR_MSGBOX ||
      //   nCtlColor == CTLCOLOR_SCROLLBAR)
      //{
      //   return FALSE;
      //}

      //if (nCtlColor == CTLCOLOR_LISTBOX)
      //{
      //   // only handle requests to draw the space between edit and drop button
      //   //  in a drop-down combo (not a drop-down list)
      //   if (!__is_combo_box_control(hWnd, (UINT)CBS_DROPDOWN))
      //      return FALSE;
      //}

      //// set background color and return handle to brush
      //LOGBRUSH logbrush;
      //VERIFY(::GetObject(hbrGray, sizeof(LOGBRUSH), (LPVOID)&logbrush));
      //::SetBkColor(hDC, logbrush.lbColor);
      //if (clrText == (COLORREF)-1)
      //   clrText = ::GetSysColor(COLOR_WINDOWTEXT);  // normal text
      //::SetTextColor(hDC, clrText);
      //return TRUE;


      throw todo(::get_thread_app());


   }

   /////////////////////////////////////////////////////////////////////////////
   // 'dialog data' support

   /*bool interaction_impl::UpdateData(bool bSaveAndValidate)
   {
   ASSERT(::WinIsWindow(get_handle())); // calling UpdateData before DoModal?

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

      throw todo(get_app());

      ////ASSERT(::WinIsWindow(get_handle()));

      ////// determine owner interaction_impl to center against
      ////uint32_t dwStyle = GetStyle();
      ////::user::interaction * hWndCenter = pAlternateOwner;
      ////if (pAlternateOwner == NULL)
      ////{
      ////   if (dwStyle & WS_CHILD)
      ////      hWndCenter = GetParent();
      ////   else
      ////      hWndCenter = GetWindow(GW_OWNER);
      ////   if (hWndCenter != NULL)
      ////   {
      ////      // let parent determine alternate center interaction_impl
      ////      ::user::interaction * hWndTemp =
      ////         (::user::interaction * )hWndCenter->send_message(WM_QUERYCENTERWND, 0, 0);
      ////      if (hWndTemp != NULL)
      ////         hWndCenter = hWndTemp;
      ////   }
      ////}

      ////// get coordinates of the interaction_impl relative to its parent
      ////rect rcDlg;
      ////GetWindowRect(&rcDlg);
      ////rect rcArea;
      ////rect rcCenter;
      ////::user::interaction * hWndParent;
      ////if (!(dwStyle & WS_CHILD))
      ////{
      ////   // don't center against invisible or minimized windows
      ////   if (hWndCenter != NULL)
      ////   {
      ////      uint32_t dwAlternateStyle = hWndCenter->GetWindowLong(GWL_STYLE);
      ////      if (!(dwAlternateStyle & WS_VISIBLE) || (dwAlternateStyle & WS_MINIMIZE))
      ////         hWndCenter = NULL;
      ////   }

      ////   MONITORINFO mi;
      ////   mi.cbSize = sizeof(mi);

      ////   // center within appropriate monitor coordinates
      ////   if (hWndCenter == NULL)
      ////   {
      ////      oswindow hwDefault = System.GetMainWnd()->_get_handle();

      ////      GetMonitorInfo(
      ////         MonitorFromWindow(hwDefault, MONITOR_DEFAULTTOPRIMARY), &mi);
      ////      rcCenter = mi.rcWork;
      ////      rcArea = mi.rcWork;
      ////   }
      ////   else
      ////   {
      ////      hWndCenter->GetWindowRect(&rcCenter);
      ////      GetMonitorInfo(
      ////         MonitorFromWindow(hWndCenter->_get_handle(), MONITOR_DEFAULTTONEAREST), &mi);
      ////      rcArea = mi.rcWork;
      ////   }
      ////}
      ////else
      ////{
      ////   // center within parent client coordinates
      ////   hWndParent = GetParent();
      ////   ASSERT(hWndParent->IsWindow());

      ////   hWndParent->GetClientRect(&rcArea);
      ////   ASSERT(hWndCenter->IsWindow());
      ////   hWndCenter->GetClientRect(&rcCenter);
      ////   ::MapWindowPoints(hWndCenter->_get_handle(), hWndParent->_get_handle(), (POINT*)&rcCenter, 2);
      ////}

      ////// find dialog's upper left based on rcCenter
      ////int xLeft = (rcCenter.left + rcCenter.right) / 2 - rcDlg.width() / 2;
      ////int yTop = (rcCenter.top + rcCenter.bottom) / 2 - rcDlg.height() / 2;

      ////// if the dialog is outside the screen, move it inside
      ////if (xLeft < rcArea.left)
      ////   xLeft = rcArea.left;
      ////else if (xLeft + rcDlg.width() > rcArea.right)
      ////   xLeft = rcArea.right - rcDlg.width();

      ////if (yTop < rcArea.top)
      ////   yTop = rcArea.top;
      ////else if (yTop + rcDlg.height() > rcArea.bottom)
      ////   yTop = rcArea.bottom - rcDlg.height();

      ////// ::map screen coordinates to child coordinates
      ////SetWindowPos(NULL, xLeft, yTop, -1, -1,
      ////   SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
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
      if(lpszResourceName != NULL)
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


      throw todo(get_app());

      //// execute it
      //bool bResult = ExecuteDlgInit(lpResource);

      //// cleanup
      //if (lpResource != NULL && hResource != NULL)
      //{
      //   UnlockResource(hResource);
      //   FreeResource(hResource);
      //}
      //return bResult;
   }

   bool interaction_impl::ExecuteDlgInit(LPVOID lpResource)
   {


      throw todo(get_app());


      //      bool bSuccess = TRUE;
      //      if (lpResource != NULL)
      //      {
      //         UNALIGNED WORD* lpnRes = (WORD*)lpResource;
      //         while (bSuccess && *lpnRes != 0)
      //         {
      //            WORD nIDC = *lpnRes++;
      //            WORD nMsg = *lpnRes++;
      //            uint32_t dwLen = *((UNALIGNED uint32_t*&)lpnRes)++;
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

   void interaction_impl::UpdateDialogControls(command_target* pTarget,bool bDisableIfNoHndler)
   {
      UNREFERENCED_PARAMETER(pTarget);
      UNREFERENCED_PARAMETER(bDisableIfNoHndler);
      cmd_ui state(get_app());
      interaction_impl wndTemp;       // very temporary interaction_impl just for CmdUI update

      // walk all the kids - assume the IDs are for buttons
      /* xxx   for (oswindow hWndChild = ::GetTopWindow(get_handle()); hWndChild != NULL;
      hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
      {
      // send to buttons
      wndTemp.set_handle(hWndChild); // quick and dirty attach
      state.m_nID = __get_dialog_control_id(hWndChild);
      state.m_pOther = &wndTemp;

      // check for reflect handlers in the child interaction_impl
      ::user::interaction_impl * pWnd = ::metrowin::interaction_impl::FromHandlePermanent(hWndChild);
      if (pWnd != NULL)
      {
      // call it directly to disable any routing
      if (WIN_WINDOW(pWnd)->interaction_impl::_001OnCommand(0, MAKELONG(0xffff,
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
      // only certain button controls get automagically disabledi
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


   id interaction_impl::RunModalLoop(uint32_t dwFlags,::object * pliveobject)
   {


      // for tracking the idle time state
      bool bIdle = TRUE;
      LONG lIdleCount = 0;
      bool bShowIdle = (dwFlags & MLF_SHOWONIDLE) && !(GetStyle() & WS_VISIBLE);
      oswindow hWndParent = oswindow_get(GetParent()->m_pimpl.cast < ::user::interaction_impl >());
      m_pui->m_iModal = m_pui->m_iModalCount;
      int iLevel = m_pui->m_iModal;
      oprop(string("RunModalLoop.thread(") + ::str::from(iLevel) + ")") = ::get_thread();
      m_pui->m_iModalCount++;

      m_pui->m_threadptraModal.add(::get_thread());
      ::aura::application * pappThis1 = dynamic_cast <::aura::application *> (m_pauraapp);
      ::aura::application * pappThis2 = dynamic_cast <::aura::application *> (m_pauraapp);
      // acquire and dispatch messages until the modal state is done
      MESSAGE msg;
      for(;;)
      {
         ASSERT(ContinueModal(iLevel));

         // phase1: check to see if we can do idle work
         while(bIdle && !::PeekMessage(&msg,NULL,NULL,NULL,PM_NOREMOVE))
         {
            ASSERT(ContinueModal(iLevel));

            // show the dialog when the message queue goes idle
            if(bShowIdle)
            {
               ShowWindow(SW_SHOWNORMAL);
               UpdateWindow();
               bShowIdle = FALSE;
            }

            // call on_idle while in bIdle state
            if(!(dwFlags & MLF_NOIDLEMSG) && hWndParent != NULL && lIdleCount == 0)
            {
               // send WM_ENTERIDLE to the parent
               // xxx todo               ::SendMessage(hWndParent, WM_ENTERIDLE, MSGF_DIALOGBOX, (LPARAM)get_handle());
            }
            if((dwFlags & MLF_NOKICKIDLE))

               // xxx todo    ||           !__call_window_procedure(this, get_handle(), WM_KICKIDLE, MSGF_DIALOGBOX, lIdleCount++))
            {
               // stop idle processing next time
               bIdle = FALSE;
            }

            get_thread()->m_dwAlive = get_thread()->m_dwAlive = ::get_tick_count();
            if(pappThis1 != NULL)
            {
               pappThis1->m_dwAlive = get_thread()->m_dwAlive;
            }
            if(pappThis2 != NULL)
            {
               pappThis1->m_dwAlive = get_thread()->m_dwAlive;
            }
            if(pliveobject != NULL)
            {
               pliveobject->keep_alive();
            }
         }


         // phase2: pump messages while available
         do
         {
            if(!ContinueModal(iLevel))
               goto ExitModal;

            if(!get_thread()->pump_message())
            {
               __post_quit_message(0);
               return -1;
            }

            // show the interaction_impl when certain special messages rec'd
            if(bShowIdle &&
                  (msg.message == 0x118 || msg.message == WM_SYSKEYDOWN))
            {
               ShowWindow(SW_SHOWNORMAL);
               UpdateWindow();
               bShowIdle = FALSE;
            }

            if(!ContinueModal(iLevel))
               goto ExitModal;

            //// reset "no idle" state after pumping "normal" message
            //if(__is_idle_message(&msg))
            //{
            //   bIdle = TRUE;
            //   lIdleCount = 0;
            //}

            get_thread()->m_dwAlive = ::get_tick_count();
            if(pappThis1 != NULL)
            {
               pappThis1->m_dwAlive = get_thread()->m_dwAlive;
            }
            if(pappThis2 != NULL)
            {
               pappThis2->m_dwAlive = get_thread()->m_dwAlive;
            }
            if(pliveobject != NULL)
            {
               pliveobject->keep_alive();
            }

            /*            if(pliveobject != NULL)
            {
            pliveobject->keep();
            }*/

         } while(::PeekMessage(&msg,NULL,NULL,NULL,PM_NOREMOVE) != FALSE);


         if(m_pauraapp != NULL)
         {
//            m_pauraapp->step_timer();
         }
         if(!ContinueModal(iLevel))
            goto ExitModal;


      }

ExitModal:
      m_pui->m_threadptraModal.remove_first(::get_thread());
      m_pui->m_iModal = m_pui->m_iModalCount;
      return m_nModalResult;
   }

   bool interaction_impl::ContinueModal(int iLevel)
   {
      return iLevel < m_pui->m_iModalCount;
   }

   void interaction_impl::EndModalLoop(id nResult)
   {

      throw todo(get_app());

   }

   void interaction_impl::EndAllModalLoops(id nResult)
   {

      //throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));

      // this result will be returned from interaction_impl::RunModalLoop
      m_pui->m_idModalResult = nResult;

      // make sure a message goes through to exit the modal loop
      if(m_pui->m_iModalCount > 0)
      {
         int iLevel = m_pui->m_iModalCount - 1;
         m_pui->m_iModalCount = 0;
         m_pui->kick_queue();
         ::get_thread()->kick_thread();
         for(int i = iLevel; i >= 0; i--)
         {
            ::thread * pthread = oprop(string("RunModalLoop.thread(") + ::str::from(i) + ")").cast < ::thread >();
            try
            {
               pthread->kick_thread();
            }
            catch(...)
            {
            }
         }
      }
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

      throw todo(get_app());

      //      if (!Attach(hWnd))
      //         return FALSE;
      //
      //      // allow any other subclassing to occur
      //      pre_subclass_window();
      //
      //      m_pfnSuper = (WNDPROC)::GetWindowLongPtr(hWnd, GWLP_WNDPROC);
      //
      //      // now hook into the AFX WndProc
      //      WNDPROC* lplpfn = GetSuperWndProcAddr();
      //      WNDPROC oldWndProc = (WNDPROC)::SetWindowLongPtr(hWnd, GWLP_WNDPROC,
      //         (int_ptr)__get_window_procedure());
      //      ASSERT(oldWndProc != __get_window_procedure());
      //
      //      if (*lplpfn == NULL)
      //         *lplpfn = oldWndProc;   // the first control of that type created
      //#ifdef DEBUG
      //      else if (*lplpfn != oldWndProc)
      //      {
      //         TRACE(::core::trace::category_AppMsg, 0, "p: Trying to use SubclassWindow with incorrect interaction_impl\n");
      //         TRACE(::core::trace::category_AppMsg, 0, "\tderived class.\n");
      //         TRACE(::core::trace::category_AppMsg, 0, "\thWnd = $%08X (nIDC=$%08X) is not a %hs.\n", (UINT)(uint_ptr)hWnd,
      //            __get_dialog_control_id(hWnd), typeid(*this).name());
      //         ASSERT(FALSE);
      //         // undo the subclassing if continuing after assert
      //         ::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (int_ptr)oldWndProc);
      //      }
      //#endif
      //      ::message::size size(get_app());
      //      _001OnSize(&size);
      //      return TRUE;
   }

   bool interaction_impl::SubclassDlgItem(UINT nID,::user::interaction_impl * pParent)
   {

      throw todo(get_app());


      //ASSERT(pParent != NULL);
      //ASSERT(::WinIsWindow(WIN_WINDOW(pParent)->get_handle()));

      //// check for normal dialog control first
      //oswindow hWndControl = ::GetDlgItem(WIN_WINDOW(pParent)->get_handle(), nID);
      //if (hWndControl != NULL)
      //   return SubclassWindow(hWndControl);


      //return FALSE;   // control not found
   }

   oswindow interaction_impl::UnsubclassWindow()
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));

      //// set WNDPROC back to original value
      //WNDPROC* lplpfn = GetSuperWndProcAddr();
      //SetWindowLongPtr(get_handle(), GWLP_WNDPROC, (int_ptr)*lplpfn);
      //*lplpfn = NULL;

      //// and Detach the oswindow from the interaction_impl object
      //return Detach();
   }


   /*   view_update_hint::view_update_hint(::aura::application * papp) :
   ::object(papp)
   {
   }
   */



   bool interaction_impl::IsChild(::user::interaction *  pWnd)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //if(WIN_WINDOW(pWnd)->_get_handle() == NULL)
      //{
      //   return ::user::interaction::IsChild(pWnd);
      //}
      //else
      //{
      //   return ::IsChild(get_handle(), WIN_WINDOW(pWnd)->_get_handle()) != FALSE;
      //}
   }

   bool interaction_impl::IsWindow() const
   {
      return ::WinIsWindow(get_handle()) != FALSE;
   }

   oswindow interaction_impl::_get_handle()
   {
      return get_handle();
   }

   bool interaction_impl::SetWindowPos(int_ptr z,int x,int y,int cx,int cy,UINT nFlags)
   {
      /*bool b;
      bool * pb = &b;
      if(m_paxisapp->m_pcoreapp->s_ptwf != NULL)
      pb = &m_paxisapp->m_pcoreapp->s_ptwf->m_bProDevianMode;
      keeper < bool > keepOnDemandDraw(pb, false, *pb, true);
      */
      //ASSERT(::WinIsWindow(get_handle()));
      ///*   return ::SetWindowPos(get_handle(), pWndInsertAfter->get_handle(),
      //x, y, cx, cy, nFlags) != FALSE; */
      //rect64 rectWindowOld = m_rectParentClient;
      if(nFlags & SWP_NOMOVE)
      {
         if(nFlags & SWP_NOSIZE)
         {
         }
         else
         {
            m_rectParentClient.right   = m_rectParentClient.left + cx;
            m_rectParentClient.bottom  = m_rectParentClient.top + cy;
         }
      }
      else
      {
         if(nFlags & SWP_NOSIZE)
         {
            m_rectParentClient.offset(x - m_rectParentClient.left,y - m_rectParentClient.top);
         }
         else
         {
            m_rectParentClient.left    = x;
            m_rectParentClient.top     = y;
            m_rectParentClient.right   = m_rectParentClient.left + cx;
            m_rectParentClient.bottom  = m_rectParentClient.top + cy;
         }
      }
      //if(m_pui != this
      //   && m_pui != NULL)
      //{
      //   m_pui->m_rectParentClient = m_rectParentClient;
      //}

      ////if(GetExStyle() & WS_EX_LAYERED)
      //{
      //   if(rectWindowOld.top_left() != m_rectParentClient.top_left())
      //   {
      //      send_message(WM_MOVE);
      //   }
      //   if(rectWindowOld.size() != m_rectParentClient.size())
      //   {
      //      send_message(WM_SIZE);
      //   }

      //}
      //if(GetExStyle() & WS_EX_LAYERED)
      //{
      //   throw todo(get_app());
      //   //   nFlags |= SWP_NOCOPYBITS;
      //   //   nFlags |= SWP_NOREDRAW;
      //   //   nFlags |= SWP_NOMOVE;
      //   //   nFlags |= SWP_NOSIZE;
      //   //   //nFlags |= SWP_NOZORDER;
      //   //   //nFlags |= SWP_FRAMECHANGED;
      //   //   if(nFlags & SWP_SHOWWINDOW)
      //   //   {
      //   //      ::SetWindowPos(get_handle(), (oswindow) z, x, y, cx, cy, nFlags);
      //   //      ShowWindow(SW_SHOW);
      //   //   }
      //   //   else
      //   //   {
      //   //      ::SetWindowPos(get_handle(), (oswindow) z, x, y, cx, cy, nFlags);
      //   //   }
      //   //   /*if(m_pui != NULL)
      //   //   {
      //   //   m_pui->oprop("pending_layout") = true;
      //   //   m_pui->oprop("pending_zorder") = z;
      //   //   }*/
      //   //   /*if(&System != NULL && System.get_twf() != NULL)
      //   //   {
      //   //   System.get_twf()->synch_redraw();
      //   //   }*/
      //   //}
      //   //else
      //   //{
      //   //   if(z == -3)
      //   //   {
      //   //      ::SetWindowPos(get_handle(), (oswindow) 0, x, y, cx, cy, nFlags);
      //   //   }
      //   //   else
      //   //   {
      //   //      ::SetWindowPos(get_handle(), (oswindow) z, x, y, cx, cy, nFlags);
      //   //   }
      //}
      return true;

   }

   void interaction_impl::MoveWindow(int x,int y,int nWidth,int nHeight,bool bRepaint)
   {

      ASSERT(::WinIsWindow(get_handle()));

      SetWindowPos(NULL,x,y,nWidth,nHeight,bRepaint ? SWP_SHOWWINDOW : 0);

   }


   bool interaction_impl::ClientToScreen(LPRECT lprect)
   {

      class rect64 rectWindow;

      if(!m_pui->GetWindowRect(rectWindow))
      {

         return false;

      }

      lprect->left   += (LONG)rectWindow.left;
      lprect->right  += (LONG)rectWindow.left;
      lprect->top    += (LONG)rectWindow.top;
      lprect->bottom += (LONG)rectWindow.top;

      return true;

   }


   bool interaction_impl::ClientToScreen(LPPOINT lppoint)
   {

      class rect64 rectWindow;

      if(!m_pui->GetWindowRect(rectWindow))
      {

         return false;

      }

      lppoint->x     += (LONG)rectWindow.left;
      lppoint->y     += (LONG)rectWindow.top;

      return true;

   }


   bool interaction_impl::ClientToScreen(RECT64 * lprect)
   {

      class rect rectWindow;

      if(!m_pui->GetWindowRect(rectWindow))
      {

         return false;

      }

      lprect->left   += rectWindow.left;
      lprect->right  += rectWindow.left;
      lprect->top    += rectWindow.top;
      lprect->bottom += rectWindow.top;

      return true;

   }


   bool interaction_impl::ClientToScreen(POINT64 * lppoint)
   {

      class rect64 rectWindow;

      if(!m_pui->GetWindowRect(rectWindow))
      {

         return false;

      }

      lppoint->x     += rectWindow.left;
      lppoint->y     += rectWindow.top;

      return true;

   }


   bool interaction_impl::ScreenToClient(LPRECT lprect)
   {

      class rect64 rectWindow;

      if(!m_pui->GetWindowRect(rectWindow))
      {

         return false;

      }

      lprect->left   -= (LONG)rectWindow.left;
      lprect->right  -= (LONG)rectWindow.left;
      lprect->top    -= (LONG)rectWindow.top;
      lprect->bottom -= (LONG)rectWindow.top;

      return true;

   }


   bool interaction_impl::ScreenToClient(LPPOINT lppoint)
   {

      class rect64 rectWindow;

      if(!m_pui->GetWindowRect(rectWindow))
      {

         return false;

      }

      lppoint->x     -= (LONG)rectWindow.left;
      lppoint->y     -= (LONG)rectWindow.top;

      return true;

   }


   bool interaction_impl::ScreenToClient(RECT64 * lprect)
   {

      class rect64 rectWindow;

      if(!m_pui->GetWindowRect(rectWindow))
      {

         return false;

      }

      lprect->left   -= rectWindow.left;
      lprect->right  -= rectWindow.left;
      lprect->top    -= rectWindow.top;
      lprect->bottom -= rectWindow.top;

      return true;

   }


   bool interaction_impl::ScreenToClient(POINT64 * lppoint)
   {

      class rect64 rectWindow;

      if(!m_pui->GetWindowRect(rectWindow))
      {

         return false;

      }

      lppoint->x     -= rectWindow.left;
      lppoint->y     -= rectWindow.top;

      return true;

   }



   bool interaction_impl::GetWindowRect(RECT64 * lprect)
   {

      if(m_pwindow == NULL)
      {

         return false;

      }

      if(m_pwindow->m_pwindow == nullptr)
      {

         return false;

      }

      if(!::WinIsWindow(get_handle()))
      {

         return false;

      }

      Windows::Foundation::Rect rect = m_pwindow->m_pwindow->get_window_rect();


      lprect->left   = (int64_t)rect.X;

      lprect->top    = (int64_t)rect.Y;

      lprect->right  = (int64_t)(lprect->left + rect.Width);

      lprect->bottom = (int64_t)(lprect->top + rect.Height);

      /*if(!::WinIsWindow(get_handle()))
         throw simple_exception(get_app(), "no more a window");
         // if it is temporary interaction_impl - probably not ca2 wrapped interaction_impl
         if(m_pui == NULL || m_pui == this)
         {
         throw todo(get_app());
         //rect rect32;
         //::GetWindowRect(get_handle(), rect32);
         //::copy(lprect, rect32);

         //#endif
         }
         else
         {
         interaction::GetWindowRect(lprect);
         }*/

      return true;

   }


   bool interaction_impl::GetClientRect(RECT64 * lprect)
   {

      if(!::WinIsWindow(get_handle()))
      {

         return false;

      }

      GetWindowRect(lprect);

      lprect->right -= lprect->left;
      lprect->left = 0;
      lprect->bottom -= lprect->top;
      lprect->top = 0;
      // if it is temporary interaction_impl - probably not ca2 wrapped interaction_impl
      //if(m_pui == NULL || m_pui == this)
      {
         // throw todo(get_app());
         //rect rect32;
         //::GetClientRect(get_handle(), rect32);
         //::copy(lprect, rect32);
      }
      //else
      {
         // interaction::GetClientRect(lprect);
      }

      return true;

   }


   id interaction_impl::SetDlgCtrlId(id id)
   {
//      m_id = id;
      //    return m_id;
      m_pui->m_id = id;
      return m_pui->m_id;
   }

   id interaction_impl::GetDlgCtrlId()
   {
      return m_pui->m_id;
   }

   /*   guie_message_wnd::guie_message_wnd(::aura::application * papp) :
   ::object(papp)
   {
   m_pguieForward = NULL;
   }

   LRESULT guie_message_wnd::message_handler(signal_details * pobj)
   {
   if(m_pguieForward != NULL)
   {
   return m_pguieForward->message_handler(uiMessage, wparam, lparam);
   }
   else
   {
   return 0;
   }
   }*/

   void interaction_impl::_001WindowMaximize()
   {
      //::user::interaction::_001WindowMaximize();
   }

   void interaction_impl::_001WindowRestore()
   {

      throw todo(get_app());

      /*      m_eappearance = appearance_normal;
            if(m_pui != NULL)
            m_pui->m_eappearance = appearance_normal;
            ::ShowWindow(get_handle(), SW_RESTORE);*/
   }


   bool interaction_impl::ShowWindow(int nCmdShow)
   {

      return m_pui->IsWindowVisible();

   }


   //bool interaction_impl::IsIconic()
   //{

   //   throw todo(get_app());

   //   //ASSERT(::WinIsWindow(get_handle()));
   //   //if(GetExStyle() & WS_EX_LAYERED)
   //   //{
   //   //   return m_pui->m_eappearance == appearance_iconic;
   //   //}
   //   //else
   //   //{
   //   //   return ::IsIconic(get_handle()) != FALSE;
   //   //}
   //}

   //bool interaction_impl::IsZoomed()
   //{
   //   ASSERT(::WinIsWindow(get_handle()));
   //   //return m_pui->m_eappearance == appearance_zoomed;
   //}


   ::user::interaction *  interaction_impl::GetParent()
   {

      throw todo(get_app());

      //if(!::WinIsWindow(get_handle()))
      //   return NULL;
      //if(get_handle() == NULL)
      //   return NULL;
      //return ::metrowin::interaction_impl::from_handle(::GetParent(get_handle()));
   }

   LONG interaction_impl::GetWindowLong(int nIndex)
   {

      throw todo(get_app());

      //return ::GetWindowLong(get_handle(), nIndex);
   }

   LONG interaction_impl::SetWindowLong(int nIndex,LONG lValue)
   {

      throw todo(get_app());

      //return ::SetWindowLong(get_handle(), nIndex, lValue);
   }


   // interaction_impl
   /* interaction_impl::operator oswindow() const
   { return this == NULL ? NULL : get_handle(); }*/
   bool interaction_impl::operator==(const ::user::interaction_impl& wnd) const
   {
      return ((oswindow)WIN_WINDOW(const_cast <::user::interaction_impl *> (&wnd))->get_handle()) == get_handle();
   }

   bool interaction_impl::operator!=(const ::user::interaction_impl& wnd) const
   {
      return ((oswindow)WIN_WINDOW(const_cast <::user::interaction_impl *> (&wnd))->get_handle()) != get_handle();
   }


   LONG_PTR interaction_impl::get_window_long_ptr(int32_t nIndex) const
   {

      return m_mapLong[nIndex];

   }


   LONG_PTR interaction_impl::set_window_long_ptr(int32_t nIndex, LONG_PTR lValue)
   {

      return m_mapLong[nIndex] = lValue;

   }


   uint32_t interaction_impl::GetStyle() const
   {

      return get_window_long_ptr(GWL_STYLE);
      
   }

   uint32_t interaction_impl::GetExStyle() const
   {

      return get_window_long_ptr(GWL_EXSTYLE);

   }


   bool interaction_impl::ModifyStyle(uint32_t dwRemove,uint32_t dwAdd,UINT nFlags)
   {

      set_window_long_ptr(GWL_STYLE, (get_window_long_ptr(GWL_STYLE) | dwAdd) & ~(dwRemove));

      if (nFlags != 0)
      {

         RedrawWindow();

      }

      return true;
   }


   bool interaction_impl::ModifyStyleEx(uint32_t dwRemove,uint32_t dwAdd,UINT nFlags)
   {

      set_window_long_ptr(GWL_EXSTYLE, (get_window_long_ptr(GWL_EXSTYLE) | dwAdd) & ~(dwRemove));

      if (nFlags != 0)
      {

         RedrawWindow();

      }

      return true;
   }

   //void interaction_impl::SetOwner(::user::interaction * pOwnerWnd)
   //{
   //   m_pguieOwner = pOwnerWnd;
   //}

   LRESULT interaction_impl::send_message(UINT uiMessage,WPARAM wparam,lparam lparam)
   {

      smart_pointer < ::message::base > spbase;

      spbase = m_pui->get_base(uiMessage,wparam,lparam);

      /*      try
            {
            ::user::interaction * pui = m_pui;
            while(pui != NULL)
            {
            try
            {
            pui->pre_translate_message(spbase);
            }
            catch(...)
            {
            break;
            }
            if(spbase->m_bRet)
            return spbase->get_lresult();
            try
            {
            pui = pui->GetParent();
            }
            catch(...)
            {
            break;
            }
            }
            }
            catch(...)
            {
            }*/
      message_handler(spbase);
      return spbase->get_lresult();

      //throw todo(get_app());

      ////ASSERT(::WinIsWindow(get_handle()));
      //return ::SendMessage(get_handle(), message, wParam, lParam);
   }

   bool interaction_impl::post_message(UINT message,WPARAM wParam,lparam lParam)
   {

      //      throw todo(get_app());

      return ::PostMessageW(get_handle(),message,wParam,lParam) != FALSE;
   }

   bool interaction_impl::DragDetect(POINT pt) const
   {


      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));

      //return ::DragDetect(get_handle(), pt) != FALSE;

   }

   void interaction_impl::SetWindowText(const char * lpszString)
   {
      m_strWindowText = lpszString;
   }

   strsize interaction_impl::GetWindowText(LPSTR lpszString,strsize nMaxCount)
   {
      strncpy(lpszString,m_strWindowText,nMaxCount);
      return MIN(nMaxCount,m_strWindowText.get_length());
   }

   strsize interaction_impl::GetWindowTextLength()
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));

      //return ::GetWindowTextLength(get_handle());

   }

   void interaction_impl::SetFont(::draw2d::font* pfont,bool bRedraw)
   {
      UNREFERENCED_PARAMETER(bRedraw);
      ASSERT(::WinIsWindow(get_handle()));
      m_pfont = new ::draw2d::font(*pfont);
   }

   ::draw2d::font* interaction_impl::GetFont()
   {
      ASSERT(::WinIsWindow(get_handle()));
      return m_pfont;
   }

   void interaction_impl::DragAcceptFiles(bool bAccept)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //::DragAcceptFiles(get_handle(), bAccept);
   }

   //::user::frame_window *  interaction_impl::EnsureParentFrame()
   //{
   //   ::user::frame_window * pFrameWnd=GetParentFrame();
   //   ENSURE_VALID(pFrameWnd);
   //   return pFrameWnd;
   //}

   //::user::interaction *  interaction_impl::EnsureTopLevelParent()
   //{
   //   ::user::interaction *pWnd=GetTopLevel();
   //   ENSURE_VALID(pWnd);
   //   return pWnd;
   //}

   void interaction_impl::MoveWindow(LPCRECT lpRect,bool bRepaint)
   {
      MoveWindow(lpRect->left,lpRect->top,lpRect->right - lpRect->left,lpRect->bottom - lpRect->top,bRepaint);
   }

   UINT interaction_impl::ArrangeIconicWindows()
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle())); return ::ArrangeIconicWindows(get_handle());
   }

   int interaction_impl::SetWindowRgn(HRGN hRgn,bool bRedraw)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle())); return ::SetWindowRgn(get_handle(), hRgn, bRedraw);
   }

   int interaction_impl::GetWindowRgn(HRGN hRgn)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()) && hRgn != NULL); return ::GetWindowRgn(get_handle(), hRgn);
   }

   bool interaction_impl::BringWindowToTop()
   {

      throw todo(get_app());

      //return ::BringWindowToTop(get_handle()) != FALSE;

   }

   void interaction_impl::MapWindowPoints(::user::interaction_impl * pwndTo,LPPOINT lpPoint,UINT nCount)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //::MapWindowPoints(get_handle(), (oswindow) pwndTo->get_os_data(), lpPoint, nCount);
   }

   void interaction_impl::MapWindowPoints(::user::interaction_impl * pwndTo,LPRECT lpRect)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //::MapWindowPoints(get_handle(), (oswindow) pwndTo->get_os_data(), (LPPOINT)lpRect, 2);
   }

   ::draw2d::graphics * interaction_impl::GetDC()
   {

      throw todo(get_app());

      //::draw2d::graphics_sp g(get_app());
      //if(get_handle() == NULL)
      //{
      //   (dynamic_cast < ::metrowin::graphics * >(g.m_p))->Attach(::GetDC(NULL));
      //}
      //else
      //{
      //   (dynamic_cast < ::metrowin::graphics * >(g.m_p))->Attach(::GetDC(get_handle()));
      //}
      //return g.detach();
   }

   ::draw2d::graphics * interaction_impl::GetWindowDC()
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //::draw2d::graphics_sp g(get_app());
      //g->attach(::GetWindowDC(get_handle()));
      //return g.detach();
   }

   bool interaction_impl::ReleaseDC(::draw2d::dib * pdib)
   {

      throw todo(get_app());

      //if(pgraphics == NULL)
      //   return false;

      //if(((Gdiplus::Graphics *)(dynamic_cast<::metrowin::graphics * >(pgraphics))->get_os_data()) == NULL)
      //   return false;

      //::ReleaseDC(get_handle(), (dynamic_cast<::metrowin::graphics * >(pgraphics))->m_hdc);

      //(dynamic_cast<::metrowin::graphics * >(pgraphics))->m_hdc = NULL;

      //pgraphics->release();

      //return true;

   }

   void interaction_impl::UpdateWindow()
   {

      throw todo(get_app());

      //::UpdateWindow(get_handle());
   }

   void interaction_impl::SetRedraw(bool bRedraw)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //::SendMessage(get_handle(), WM_SETREDRAW, bRedraw, 0);
   }

   bool interaction_impl::GetUpdateRect(LPRECT lpRect,bool bErase)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //return ::GetUpdateRect(get_handle(), lpRect, bErase) != FALSE;
   }

   int interaction_impl::GetUpdateRgn(::draw2d::region* pRgn,bool bErase)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //return ::GetUpdateRgn(get_handle(), (HRGN)pRgn->get_os_data(), bErase);
   }

   void interaction_impl::Invalidate(bool bErase)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //::InvalidateRect(get_handle(), NULL, bErase);
   }

   void interaction_impl::InvalidateRect(LPCRECT lpRect,bool bErase)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //::InvalidateRect(get_handle(), lpRect, bErase);
   }

   void interaction_impl::InvalidateRgn(::draw2d::region* pRgn,bool bErase)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //::InvalidateRgn(get_handle(), (HRGN)pRgn->get_os_data(), bErase);
   }

   void interaction_impl::ValidateRect(LPCRECT lpRect)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //
      //::ValidateRect(get_handle(), lpRect);

   }

   void interaction_impl::ValidateRgn(::draw2d::region* pRgn)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //
      //::ValidateRgn(get_handle(), (HRGN)pRgn->get_os_data());

   }

   bool interaction_impl::IsWindowVisible()
   {

      return true;

      if(!::WinIsWindow(get_handle()))
         return false;

      if(m_pui != NULL)
      {

         if(!m_pui->is_this_visible())
            return false;

         if(m_pui->GetParent() != NULL && !m_pui->GetParent()->IsWindowVisible())
            return false;

      }


      throw todo(get_app());


      //if(!::IsWindowVisible(get_handle()))
      //   return false;

      return true;

   }


   void interaction_impl::ShowOwnedPopups(bool bShow)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //
      //::ShowOwnedPopups(get_handle(), bShow);

   }

   void interaction_impl::SendMessageToDescendants(UINT message,WPARAM wParam,lparam lParam,bool bDeep,bool bOnlyPerm)
   {
      ASSERT(::WinIsWindow(get_handle()));
      //interaction_impl::SendMessageToDescendants(get_handle(), message, wParam, lParam, bDeep, bOnlyPerm);

      // walk through HWNDs to avoid creating temporary interaction_impl objects
      // unless we need to call this function recursively
      user::interaction * pui = m_pui->top_child();
      while(pui != NULL)
      {
         try
         {
            pui->send_message(message,wParam,lParam);
         }
         catch(...)
         {
         }
         if(bDeep)
         {
            // send to child windows after parent
            try
            {
               pui->SendMessageToDescendants(message,wParam,lParam,bDeep,bOnlyPerm);
            }
            catch(...)
            {
            }
         }
         try
         {
            pui = pui->under_sibling();
         }
         catch(...)
         {
            break;
         }
      }
   }

   //::user::interaction *  interaction_impl::GetDescendantWindow(id id)
   //{
   //   ASSERT(::WinIsWindow(get_handle()));
   //   return interaction_impl::GetDescendantWindow(this,id);
   //}


   ::draw2d::graphics * interaction_impl::GetDCEx(::draw2d::region* prgnClip,uint32_t flags)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //::draw2d::graphics_sp g(get_app());
      //g->attach(::GetDCEx(get_handle(), (HRGN)prgnClip->get_os_data(), flags));
      //return g.detach();

   }

   bool interaction_impl::LockWindowUpdate()
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));

      //return ::LockWindowUpdate(get_handle()) != FALSE;

   }

   void interaction_impl::UnlockWindowUpdate()
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //
      //::LockWindowUpdate(NULL);

   }

   bool interaction_impl::RedrawWindow(LPCRECT lpRectUpdate,::draw2d::region* prgnUpdate,UINT flags)
   {

      m_pui->m_bRedraw = true;

      //throw todo(get_app());

      //if(System.get_twf() == NULL)
      //   return false;

      //if(System.get_twf()->m_bProDevianMode)
      //   return true;

      //ASSERT(::WinIsWindow(get_handle()));

      //return ::RedrawWindow(get_handle(), lpRectUpdate, prgnUpdate == NULL ? NULL : (HRGN)prgnUpdate->get_os_data(), flags) != FALSE;

      return true;

   }

#ifdef WINDOWSEX

   bool interaction_impl::EnableScrollBar(int nSBFlags, UINT nArrowFlags)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //
      //return ::EnableScrollBar(get_handle(), nSBFlags, nArrowFlags) != FALSE;

   }

#endif

   bool interaction_impl::DrawAnimatedRects(int idAni,CONST RECT *lprcFrom,CONST RECT *lprcTo)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //
      //return ::DrawAnimatedRects(get_handle(), idAni, lprcFrom, lprcTo) != FALSE;

   }

   bool interaction_impl::DrawCaption(::draw2d::graphics * pgraphics,LPCRECT lprc,UINT uFlags)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //
      //return ::DrawCaption(get_handle(), (HDC)(dynamic_cast<::metrowin::graphics * >(pgraphics))->get_os_data(), lprc, uFlags) != FALSE;

   }

   bool interaction_impl::SetTimer(uint_ptr nIDEvent,UINT nElapse,PFN_TIMER pfnTimer)
   {


      return ::user::interaction_impl::SetTimer(nIDEvent,nElapse,pfnTimer);


   }

   bool interaction_impl::KillTimer(uint_ptr nIDEvent)
   {

      return ::user::interaction_impl::KillTimer(nIDEvent);

   }

   bool interaction_impl::IsWindowEnabled()
   {

      return true;

      return interaction_impl::IsWindowEnabled();

   }


   bool interaction_impl::EnableWindow(bool bEnable)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //
      //return ::EnableWindow(get_handle(), bEnable) != FALSE;

   }

   ::user::interaction *  interaction_impl::GetActiveWindow()
   {

      //throw todo(get_app());

      oswindow window = ::WinGetActiveWindow();

      return window == NULL ? NULL : window->m_pimpl->m_pui;

   }

   ::user::interaction *  interaction_impl::SetActiveWindow()
   {

      //throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //
      oswindow window = ::WinSetActiveWindow(get_handle());

      return window == NULL ? NULL : window->m_pimpl->m_pui;

   }

   //::user::interaction *  interaction_impl::GetCapture()
   //{

   //   //throw todo(::get_thread_app());

   //   return ::GetCapture();

   //}

   ::user::interaction *  interaction_impl::GetFocus()
   {

      return ::WinGetFocus()->window()->m_pui;

   }

   bool interaction_impl::SetFocus()
   {

      //ASSERT(::WinIsWindow(get_handle()));
      ::WinSetFocus(m_pui->get_handle());

      return true;

   }

   ::user::interaction *  interaction_impl::GetDesktopWindow()
   {

      return NULL;

      //return System.m_pui;

      //return ::metrowin::interaction_impl::from_handle(::GetDesktopWindow());

   }


   // Helper for radio buttons
   int interaction_impl::GetCheckedRadioButton(int nIDFirstButton,int nIDLastButton)
   {
      for(int nID = nIDFirstButton; nID <= nIDLastButton; nID++)
      {
         if(IsDlgButtonChecked(nID))
            return nID; // id that matched
      }
      return 0; // invalid ID
   }

   void interaction_impl::CheckDlgButton(int nIDButton,UINT nCheck)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));

      //::CheckDlgButton(get_handle(), nIDButton, nCheck);

   }

   void interaction_impl::CheckRadioButton(int nIDFirstButton,int nIDLastButton,int nIDCheckButton)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));

      //::CheckRadioButton(get_handle(), nIDFirstButton, nIDLastButton, nIDCheckButton);

   }

#ifdef WINDOWSEX

   int interaction_impl::DlgDirList(__inout_z LPTSTR lpPathSpec, __in int nIDListBox, __in int nIDStaticPath, __in UINT nFileType)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));

      //return ::DlgDirList(get_handle(), lpPathSpec, nIDListBox, nIDStaticPath, nFileType);

   }

   int interaction_impl::DlgDirListComboBox(__inout_z LPTSTR lpPathSpec, __in int nIDComboBox,  __in int nIDStaticPath, __in UINT nFileType)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));

      //return ::DlgDirListComboBox(get_handle(), lpPathSpec, nIDComboBox, nIDStaticPath, nFileType);

   }

   bool interaction_impl::DlgDirSelect(LPTSTR lpString, int nSize, int nIDListBox)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));

      //return ::DlgDirSelectEx(get_handle(), lpString, nSize, nIDListBox) != FALSE;

   }

   bool interaction_impl::DlgDirSelectComboBox(LPTSTR lpString, int nSize, int nIDComboBox)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));

      //return ::DlgDirSelectComboBoxEx(get_handle(), lpString, nSize, nIDComboBox) != FALSE;

   }

#endif

   void interaction_impl::GetDlgItem(id id,oswindow* phWnd) const
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //ASSERT(phWnd != NULL);
      //*phWnd = ::GetDlgItem(get_handle(), (int) id);

   }

   UINT interaction_impl::GetDlgItemInt(int nID,BOOL * lpTrans,bool bSigned) const
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));

      //return ::GetDlgItemInt(get_handle(), nID, lpTrans, bSigned);

   }

#ifdef WINDOWSEX

   int interaction_impl::GetDlgItemText(__in int nID, __out_ecount_part_z(nMaxCount, return + 1) LPTSTR lpStr, __in int nMaxCount) const
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //
      //return ::GetDlgItemText(get_handle(), nID, lpStr, nMaxCount);

   }

#endif

   ::user::interaction_impl * interaction_impl::GetNextDlgGroupItem(::user::interaction_impl * pWndCtl,bool bPrevious) const
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //return ::metrowin::interaction_impl::from_handle(::GetNextDlgGroupItem(get_handle(), (oswindow) pWndCtl->get_os_data(), bPrevious));
   }

   ::user::interaction_impl * interaction_impl::GetNextDlgTabItem(::user::interaction_impl * pWndCtl,bool bPrevious) const
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //return ::metrowin::interaction_impl::from_handle(::GetNextDlgTabItem(get_handle(), (oswindow) pWndCtl->get_os_data(), bPrevious));
   }

   UINT interaction_impl::IsDlgButtonChecked(int nIDButton) const
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //
      //return ::IsDlgButtonChecked(get_handle(), nIDButton);

   }
   LPARAM interaction_impl::SendDlgItemMessage(int nID,UINT message,WPARAM wParam,LPARAM lParam)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //
      //return ::SendDlgItemMessage(get_handle(), nID, message, wParam, lParam);

   }
   void interaction_impl::SetDlgItemInt(int nID,UINT nValue,bool bSigned)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //
      //::SetDlgItemInt(get_handle(), nID, nValue, bSigned);

   }
   void interaction_impl::SetDlgItemText(int nID,const char * lpszString)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //
      //::SetDlgItemText(get_handle(), nID, lpszString);

   }

   int interaction_impl::ScrollWindowEx(int dx,int dy,LPCRECT lpRectScroll,LPCRECT lpRectClip,::draw2d::region* prgnUpdate,LPRECT lpRectUpdate,UINT flags)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //
      //return ::ScrollWindowEx(get_handle(), dx, dy, lpRectScroll, lpRectClip, (HRGN)prgnUpdate->get_os_data(), lpRectUpdate, flags);

   }

   void interaction_impl::ShowScrollBar(UINT nBar,bool bShow)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //
      //::ShowScrollBar(get_handle(), nBar, bShow);

   }

   ::user::interaction *  interaction_impl::ChildWindowFromPoint(POINT point)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //
      //return ::metrowin::interaction_impl::from_handle(::ChildWindowFromPoint(get_handle(), point));

   }

   ::user::interaction *  interaction_impl::ChildWindowFromPoint(POINT point,UINT nFlags)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //
      //return ::metrowin::interaction_impl::from_handle(::ChildWindowFromPointEx(get_handle(), point, nFlags));

   }


   sp(::user::interaction_impl) interaction_impl::FindWindow(const char * lpszClassName,const char * lpszWindowName)
   {

      throw todo(::get_thread_app());

      //return ::metrowin::interaction_impl::from_handle(::FindWindow(lpszClassName, lpszWindowName));

   }

   sp(::user::interaction_impl) interaction_impl::FindWindowEx(oswindow hwndParent,oswindow hwndChildAfter,const char * lpszClass,const char * lpszWindow)
   {

      throw todo(::get_thread_app());

      //return ::metrowin::interaction_impl::from_handle(::FindWindowEx(hwndParent, hwndChildAfter, lpszClass, lpszWindow));

   }
#ifdef WINDOWSEX

   ::user::interaction* interaction_impl::GetNextWindow(UINT nFlag)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //
      //return ::metrowin::interaction_impl::from_handle(::GetNextWindow(get_handle(), nFlag));

   }

#endif


   ::user::interaction *  interaction_impl::GetTopWindow()
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //
      //return ::metrowin::interaction_impl::from_handle(::GetTopWindow(get_handle()));

   }


   ::user::interaction *  interaction_impl::GetWindow(UINT nCmd)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //
      //return ::metrowin::interaction_impl::from_handle(::GetWindow(get_handle(), nCmd));

   }

   ::user::interaction *  interaction_impl::GetLastActivePopup()
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //
      //return ::metrowin::interaction_impl::from_handle(::GetLastActivePopup(get_handle()));

   }

   //::user::interaction *  interaction_impl::set_parent(::user::interaction *  pWndNewParent)
   //{

   //   throw todo(get_app());

   //   //ASSERT(::WinIsWindow(get_handle()));
   //   //return ::metrowin::interaction_impl::from_handle(::SetParent(get_handle(), (oswindow) pWndNewParent->get_os_data()));
   //}

   sp(::user::interaction_impl) interaction_impl::WindowFromPoint(POINT point)
   {

      throw todo(::get_thread_app());

      //return ::metrowin::interaction_impl::from_handle(::WindowFromPoint(point));

   }

   bool interaction_impl::FlashWindow(bool bInvert)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));

      //return ::FlashWindow(get_handle(), bInvert) != FALSE;

   }

   bool interaction_impl::ChangeClipboardChain(oswindow hWndNext)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));

      //return ::ChangeClipboardChain(get_handle(), hWndNext) != FALSE;

   }

   oswindow interaction_impl::SetClipboardViewer()
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));

      //return ::SetClipboardViewer(get_handle());

   }

   bool interaction_impl::OpenClipboard()
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));

      //return ::OpenClipboard(get_handle()) != FALSE;

   }

   ::user::interaction_impl * interaction_impl::GetOpenClipboardWindow()
   {

      throw todo(::get_thread_app());

      //return ::metrowin::interaction_impl::from_handle(::GetOpenClipboardWindow());

   }

   ::user::interaction_impl * interaction_impl::GetClipboardOwner()
   {

      throw todo(::get_thread_app());

      //return ::metrowin::interaction_impl::from_handle(::GetClipboardOwner());

   }

   ::user::interaction_impl * interaction_impl::GetClipboardViewer()
   {

      throw todo(::get_thread_app());

      //return ::metrowin::interaction_impl::from_handle(::GetClipboardViewer());

   }

   void interaction_impl::CreateCaret(::draw2d::bitmap* pBitmap)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));

      //::CreateCaret(get_handle(), (HBITMAP)pBitmap->get_os_data(), 0, 0);

   }

   void interaction_impl::CreateSolidCaret(int nWidth,int nHeight)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));

      //::CreateCaret(get_handle(), (HBITMAP)0, nWidth, nHeight);

   }

   void interaction_impl::CreateGrayCaret(int nWidth,int nHeight)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));

      //::CreateCaret(get_handle(), (HBITMAP)1, nWidth, nHeight);

   }

   point interaction_impl::GetCaretPos()
   {

      throw todo(::get_thread_app());

      //point point;

      //::GetCaretPos((LPPOINT)&point); return point;

   }

   void interaction_impl::SetCaretPos(POINT point)
   {

      throw todo(::get_thread_app());

      //::SetCaretPos(point.x, point.y);

   }

   void interaction_impl::HideCaret()
   {

      throw todo(get_app());

      //::HideCaret(get_handle());

   }

   void interaction_impl::ShowCaret()
   {

      throw todo(get_app());

      //::ShowCaret(get_handle());

   }

   bool interaction_impl::SetForegroundWindow()
   {

      throw todo(get_app());

      //return ::SetForegroundWindow(get_handle()) != FALSE;

   }

   sp(::user::interaction_impl) interaction_impl::GetForegroundWindow()
   {

      throw todo(::get_thread_app());

      //return ::metrowin::interaction_impl::from_handle(::GetForegroundWindow());

   }

   bool interaction_impl::SendNotifyMessage(UINT message,WPARAM wParam,LPARAM lParam)
   {

      throw todo(get_app());

      //return ::SendNotifyMessage(get_handle(), message, wParam, lParam) != FALSE;

   }

   // Win4
   HICON interaction_impl::SetIcon(HICON hIcon,bool bBigIcon)
   {

      throw todo(get_app());

      //return (HICON)send_message(WM_SETICON, bBigIcon, (LPARAM)hIcon);

   }

   HICON interaction_impl::GetIcon(bool bBigIcon) const
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));

      //return (HICON)const_cast < interaction_impl * > (this)->send_message(WM_GETICON, bBigIcon, 0);

   }

   void interaction_impl::Print(::draw2d::graphics * pgraphics,uint32_t dwFlags) const
   {

      ASSERT(::WinIsWindow(get_handle()));

      throw not_implemented(get_app());
      //      const_cast < interaction_impl * > (this)->send_message(WM_PRINT, (WPARAM)(dynamic_cast<::metrowin::graphics * >(pgraphics))->get_os_data(), (LPARAM) dwFlags);

   }

   void interaction_impl::PrintClient(::draw2d::graphics * pgraphics,uint32_t dwFlags) const
   {

      ASSERT(::WinIsWindow(get_handle()));

      throw not_implemented(get_app());
      //const_cast < interaction_impl * > (this)->send_message(WM_PRINTCLIENT, (WPARAM)(dynamic_cast<::metrowin::graphics * >(pgraphics))->get_os_data(), (LPARAM) dwFlags);

   }

   bool interaction_impl::SetWindowContextHelpId(uint32_t dwContextHelpId)
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));

      //return ::SetWindowContextHelpId(get_handle(), dwContextHelpId) != FALSE;

   }

   uint32_t interaction_impl::GetWindowContextHelpId() const
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));

      //return ::GetWindowContextHelpId(get_handle());

   }


   // Default message ::map implementations
   void interaction_impl::OnActivateApp(bool,uint32_t)
   {
      Default();
   }
   void interaction_impl::OnActivate(UINT,::user::interaction_impl *,bool)
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
   void interaction_impl::OnContextMenu(::user::interaction_impl *,point)
   {
      Default();
   }

#ifdef WINDOWSEX

   bool interaction_impl::OnCopyData(::user::interaction_impl *, COPYDATASTRUCT*)
   {

      return Default() != FALSE;

   }

#endif

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


#ifdef WINDOWSEX

   void interaction_impl::OnGetMinMaxInfo(MINMAXINFO*)
   {
      Default();
   }

#endif

   void interaction_impl::OnIconEraseBkgnd(::draw2d::graphics *)
   {
      Default();
   }
   void interaction_impl::OnKillFocus(::user::interaction_impl *)
   {
      Default();
   }
   LRESULT interaction_impl::OnMenuChar(UINT,UINT,::user::menu*)
   {
      return Default();
   }
   void interaction_impl::OnMenuSelect(UINT,UINT,HMENU)
   {
      Default();
   }
   void interaction_impl::OnMove(int,int)
   {
      Default();
   }
   HCURSOR interaction_impl::OnQueryDragIcon()
   {
      return (HCURSOR)Default();
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

   void interaction_impl::_001OnSetCursor(signal_details * pobj)
   {

      throw todo(get_app());

      //SCAST_PTR(::message::base, pbase, pobj);
      //if(System.get_cursor() != NULL
      //   && System.get_cursor()->m_ecursor != ::visual::cursor_system)
      //{
      //   ::SetCursor(NULL);
      //}
      //pbase->set_lresult(1);
      //pbase->m_bRet = true;
      ////(bool)Default();

   }

   void interaction_impl::OnShowWindow(bool,UINT)
   {
      Default();
   }
   void interaction_impl::OnSize(UINT,int,int)
   {
      Default();
   }
   void interaction_impl::OnTCard(UINT,uint32_t)
   {
      Default();
   }

#ifdef WINDOWSEX

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

#endif

   void interaction_impl::OnPaletteIsChanging(::user::interaction_impl *)
   {
      Default();
   }

   bool interaction_impl::OnNcActivate(bool)
   {

      return Default() != FALSE;

   }


#ifdef WINDOWSEX

   void interaction_impl::OnNcCalcSize(bool, NCCALCSIZE_PARAMS*)
   {
      Default();
   }

#endif

   bool interaction_impl::OnNcCreate(LPCREATESTRUCT)
   {

      return Default() != FALSE;

   }

   LRESULT interaction_impl::OnNcHitTest(point)
   {
      return Default();
   }
   void interaction_impl::OnNcLButtonDblClk(UINT,point)
   {
      Default();
   }
   void interaction_impl::OnNcLButtonDown(UINT,point)
   {
      Default();
   }
   void interaction_impl::OnNcLButtonUp(UINT,point)
   {
      Default();
   }
   void interaction_impl::OnNcMButtonDblClk(UINT,point)
   {
      Default();
   }
   void interaction_impl::OnNcMButtonDown(UINT,point)
   {
      Default();
   }
   void interaction_impl::OnNcMButtonUp(UINT,point)
   {
      Default();
   }
   void interaction_impl::OnNcMouseMove(UINT,point)
   {
      Default();
   }
   void interaction_impl::OnNcPaint()
   {
      Default();
   }
   void interaction_impl::OnNcRButtonDblClk(UINT,point)
   {
      Default();
   }
   void interaction_impl::OnNcRButtonDown(UINT,point)
   {
      Default();
   }
   void interaction_impl::OnNcRButtonUp(UINT,point)
   {
      Default();
   }
   void interaction_impl::OnSysChar(UINT,UINT,UINT)
   {
      Default();
   }
   void interaction_impl::OnSysCommand(UINT,LPARAM)
   {
      Default();
   }
   void interaction_impl::OnSysDeadChar(UINT,UINT,UINT)
   {
      Default();
   }
   void interaction_impl::OnSysKeyDown(UINT,UINT,UINT)
   {
      Default();
   }
   void interaction_impl::OnSysKeyUp(UINT,UINT,UINT)
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
   void interaction_impl::OnPaletteChanged(::user::interaction_impl *)
   {
      Default();
   }
   void interaction_impl::OnSpoolerStatus(UINT,UINT)
   {
      Default();
   }
   void interaction_impl::OnTimeChange()
   {
      Default();
   }
   void interaction_impl::OnChar(UINT,UINT,UINT)
   {
      Default();
   }
   void interaction_impl::OnDeadChar(UINT,UINT,UINT)
   {
      Default();
   }
   void interaction_impl::OnKeyDown(UINT,UINT,UINT)
   {
      Default();
   }
   void interaction_impl::OnKeyUp(UINT,UINT,UINT)
   {
      Default();
   }
   void interaction_impl::OnLButtonDblClk(UINT,point)
   {
      Default();
   }
   void interaction_impl::OnLButtonDown(UINT,point)
   {
      Default();
   }
   void interaction_impl::OnLButtonUp(UINT,point)
   {
      Default();
   }
   void interaction_impl::OnMButtonDblClk(UINT,point)
   {
      Default();
   }
   void interaction_impl::OnMButtonDown(UINT,point)
   {
      Default();
   }
   void interaction_impl::OnMButtonUp(UINT,point)
   {
      Default();
   }
   int interaction_impl::OnMouseActivate(::user::interaction_impl *,UINT,UINT)
   {
      return (int)Default();
   }
   void interaction_impl::OnMouseMove(UINT,point)
   {
      Default();
   }

   bool interaction_impl::OnMouseWheel(UINT,short,point)
   {

      return Default() != FALSE;

   }

   LRESULT interaction_impl::OnRegisteredMouseWheel(WPARAM,LPARAM)
   {
      return Default();
   }
   void interaction_impl::OnRButtonDblClk(UINT,point)
   {
      Default();
   }
   void interaction_impl::OnRButtonDown(UINT,point)
   {
      Default();
   }
   void interaction_impl::OnRButtonUp(UINT,point)
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
   void interaction_impl::OnInitMenuPopup(::user::menu*,UINT,bool)
   {
      Default();
   }

#ifdef WINDOWSEX

   void interaction_impl::OnAskCbFormatName(__in UINT nMaxCount, __out_ecount_z(nMaxCount) LPTSTR pszName)
   {
      (nMaxCount);
      if(nMaxCount>0)
      {
         /* defwindow proc should do this for us, but to be safe, we'll do it here too */
         pszName[0]='\0';
      }
      Default();
   }

#endif

   void interaction_impl::OnChangeCbChain(oswindow,oswindow)
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
   void interaction_impl::OnHScrollClipboard(::user::interaction_impl *,UINT,UINT)
   {
      Default();
   }
   void interaction_impl::OnPaintClipboard(::user::interaction_impl *,HGLOBAL)
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
   void interaction_impl::OnSizeClipboard(::user::interaction_impl *,HGLOBAL)
   {
      Default();
   }
   void interaction_impl::OnVScrollClipboard(::user::interaction_impl *,UINT,UINT)
   {
      Default();
   }
   UINT interaction_impl::OnGetDlgCode()
   {
      return (UINT)Default();
   }
   void interaction_impl::OnMDIActivate(bool,::user::interaction_impl *,::user::interaction_impl *)
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

#ifdef WINDOWSEX

   void interaction_impl::OnStyleChanged(int, LPSTYLESTRUCT)
   {
      Default();
   }
   void interaction_impl::OnStyleChanging(int, LPSTYLESTRUCT)
   {
      Default();
   }

#endif

   void interaction_impl::OnSizing(UINT,LPRECT)
   {
      Default();
   }
   void interaction_impl::OnMoving(UINT,LPRECT)
   {
      Default();
   }
   void interaction_impl::OnCaptureChanged(::user::interaction_impl *)
   {
      Default();
   }

   bool interaction_impl::OnDeviceChange(UINT,dword_ptr)
   {

      return Default() != FALSE;

   }

   void interaction_impl::OnWinIniChange(const char *)
   {
      Default();
   }
   void interaction_impl::OnChangeUIState(UINT,UINT)
   {
      Default();
   }
   void interaction_impl::OnUpdateUIState(UINT,UINT)
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

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //
      //::CloseWindow(get_handle());

   }

   bool interaction_impl::OpenIcon()
   {

      throw todo(get_app());

      //ASSERT(::WinIsWindow(get_handle()));
      //
      //return ::OpenIcon(get_handle()) != FALSE;

   }

   ////////////////////////////////////////////////////////////////////////////
   // UI related ::user::interaction_impl functions

   oswindow interaction_impl::GetSafeOwner_(oswindow hParent,oswindow* pWndTop)
   {

      throw todo(::get_thread_app());

      //// get ::user::interaction_impl to start with
      //oswindow hWnd = hParent;
      //if (hWnd == NULL)
      //{
      //   /* trans      frame_window* pFrame = command_target::GetRoutingFrame_();
      //   if (pFrame != NULL)
      //   hWnd = pFrame->get_handle();
      //   else
      //   hWnd = System.GetMainWnd()->get_handle();*/
      //}

      //// a popup ::user::interaction_impl cannot be owned by a child ::user::interaction_impl
      //while (hWnd != NULL && (::GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD))
      //   hWnd = ::GetParent(hWnd);

      //// determine toplevel ::user::interaction_impl to disable as well
      //oswindow hWndTop = hWnd, hWndTemp = hWnd;
      //for (;;)
      //{
      //   if (hWndTemp == NULL)
      //      break;
      //   else
      //      hWndTop = hWndTemp;
      //   hWndTemp = ::GetParent(hWndTop);
      //}

      //// get last active popup of first non-child that was found
      //if (hParent == NULL && hWnd != NULL)
      //   hWnd = ::GetLastActivePopup(hWnd);

      //// disable and store top level parent ::user::interaction_impl if specified
      //if (pWndTop != NULL)
      //{
      //   if (hWndTop != NULL && ::IsWindowEnabled(hWndTop) && hWndTop != hWnd)
      //   {
      //      *pWndTop = hWndTop;
      //      ::EnableWindow(hWndTop, FALSE);
      //   }
      //   else
      //      *pWndTop = NULL;
      //}

      //return hWnd;    // return the owner as oswindow
   }


   /////////////////////////////////////////////////////////////////////////////
   // Official way to send message to a interaction_impl

#ifdef WINDOWSEX

   CLASS_DECL_BASE LRESULT __call_window_procedure(::user::interaction * pinteraction, oswindow hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
   {
      ___THREAD_STATE* pThreadState = gen_ThreadState.get_data();
      MSG oldState = pThreadState->m_lastSentMsg;   // save for nesting
      pThreadState->m_lastSentMsg.hwnd = hWnd;
      pThreadState->m_lastSentMsg.message = nMsg;
      pThreadState->m_lastSentMsg.wParam = wParam;
      pThreadState->m_lastSentMsg.lParam = lParam;

      // Catch exceptions thrown outside the scope of a callback
      // in debug builds and warn the ::fontopus::user.
      smart_pointer < ::message::base > spbase;

      spbase(pinteraction->get_base(pinteraction, nMsg, wParam, lParam));

      __trace_message("WndProc", spbase);

      try
      {

         // special case for WM_INITDIALOG
         rect rectOld;
         uint32_t dwStyle = 0;
         if (nMsg == WM_INITDIALOG)
            __pre_init_dialog(pinteraction, &rectOld, &dwStyle);

         // delegate to object's message_handler
         if(pinteraction->m_pui != NULL && pinteraction->m_pui != pinteraction)
         {
            pinteraction->m_pui->message_handler(spbase);
         }
         else
         {
            pinteraction->message_handler(spbase);
         }
         // more special case for WM_INITDIALOG
         if (nMsg == WM_INITDIALOG)
            __post_init_dialog(pinteraction, rectOld, dwStyle);
      }
      catch(const ::exception::exception & e)
      {
         try
         {
            if(App(pinteraction->m_papp).on_run_exception((::exception::exception &) e))
               goto run;
         }
         catch(...)
         {
         }
         return -1;
      }
      catch(::exception::aura * pe)
      {
         __process_window_procedure_exception(pe, spbase);
         //         TRACE(::core::trace::category_AppMsg, 0, "Warning: Uncaught exception in message_handler (returning %ld).\n", spbase->get_lresult());
         pe->Delete();
      }
      catch(...)
      {
      }
run:
      try
      {
         pThreadState->m_lastSentMsg = oldState;
         LRESULT lresult = spbase->get_lresult();
         return lresult;
      }
      catch(...)
      {
         return 0;
      }
   }

#endif


   /*CDataExchange::CDataExchange(::user::interaction_impl * pDlgWnd, bool bSaveAndValidate)
   {
   ASSERT_VALID(pDlgWnd);
   m_bSaveAndValidate = bSaveAndValidate;
   m_pDlgWnd = pDlgWnd;
   m_idLastControl = 0;
   }*/

   /////////////////////////////////////////////////////////////////////////////
   // Window creation hooks


#ifdef WINDOWSEX

   LRESULT CALLBACK __cbt_filter_hook(int code, WPARAM wParam, LPARAM lParam)
   {
      ___THREAD_STATE* pThreadState = gen_ThreadState.get_data();
      if (code != HCBT_CREATEWND)
      {
         // wait for HCBT_CREATEWND just pass others on...
         return CallNextHookEx(pThreadState->m_hHookOldCbtFilter, code,
                               wParam, lParam);
      }

      ASSERT(lParam != NULL);
      LPCREATESTRUCT lpcs = ((LPCBT_CREATEWND)lParam)->lpcs;
      ASSERT(lpcs != NULL);

      ::user::interaction * pWndInit = pThreadState->m_pWndInit;
      bool bContextIsDLL = afxContextIsDLL;
      if (pWndInit != NULL || (!(lpcs->style & WS_CHILD) && !bContextIsDLL))
      {
         // Note: special check to avoid subclassing the IME interaction_impl
         //if (gen_DBCS)
         {
            // check for cheap CS_IME style first...
            if (GetClassLong((oswindow)wParam, GCL_STYLE) & CS_IME)
               goto lCallNextHook;

            // get class name of the interaction_impl that is being created
            const char * pszClassName;
            char szClassName[_countof("ime")+1];
            if (dword_ptr(lpcs->lpszClass) > 0xffff)
            {
               pszClassName = lpcs->lpszClass;
            }
            else
            {
               szClassName[0] = '\0';
               GlobalGetAtomName((ATOM)lpcs->lpszClass, szClassName, _countof(szClassName));
               pszClassName = szClassName;
            }

            // a little more expensive to test this way, but necessary...
            if (::__invariant_stricmp(pszClassName, "ime") == 0)
               goto lCallNextHook;
         }

         ASSERT(wParam != NULL); // should be non-NULL oswindow
         oswindow hWnd = (oswindow)wParam;
         WNDPROC oldWndProc;
         if (pWndInit != NULL)
         {
            // the interaction_impl should not be in the permanent ::map at this time
            ASSERT(::metrowin::interaction_impl::FromHandlePermanent(hWnd) == NULL);

            pWndInit->m_pthread = dynamic_cast < ::thread * > (::metrowin::get_thread());
            pWndInit->m_pthread->add(pWndInit);
            pWndInit->m_pui->m_pthread = pWndInit->m_pthread;
            pWndInit->m_pui->m_pthread->add(pWndInit->m_pui);
            pWndInit->m_pui->m_pimpl = pWndInit;

            // connect the oswindow to pWndInit...
            pWndInit->Attach(hWnd);
            // allow other subclassing to occur first
            pWndInit->pre_subclass_window();

            WNDPROC *pOldWndProc = pWndInit->GetSuperWndProcAddr();
            ASSERT(pOldWndProc != NULL);

            // subclass the interaction_impl with standard __window_procedure
            WNDPROC afxWndProc = __get_window_procedure();
            oldWndProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC,
                                                   (dword_ptr)afxWndProc);
            ASSERT(oldWndProc != NULL);
            if (oldWndProc != afxWndProc)
               *pOldWndProc = oldWndProc;

            pThreadState->m_pWndInit = NULL;
         }
         else
         {
            ASSERT(!bContextIsDLL);   // should never get here

            static ATOM s_atomMenu = 0;
            bool bSubclass = true;

            if (s_atomMenu == 0)
            {
               WNDCLASSEX wc;
               memset(&wc, 0, sizeof(WNDCLASSEX));
               wc.cbSize = sizeof(WNDCLASSEX);
               s_atomMenu = (ATOM)::GetClassInfoEx(NULL, "#32768", &wc);
            }

            // Do not subclass menus.
            if (s_atomMenu != 0)
            {
               ATOM atomWnd = (ATOM)::GetClassLongPtr(hWnd, GCW_ATOM);
               if (atomWnd == s_atomMenu)
                  bSubclass = false;
            }
            else
            {
               char szClassName[256];
               if (::GetClassName(hWnd, szClassName, 256))
               {
                  szClassName[255] = NULL;
                  if (_tcscmp(szClassName, "#32768") == 0)
                     bSubclass = false;
               }
            }
            if (bSubclass)
            {
               // subclass the interaction_impl with the proc which does gray backgrounds
               oldWndProc = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_WNDPROC);
               if (oldWndProc != NULL && GetProp(hWnd, gen_OldWndProc) == NULL)
               {
                  SetProp(hWnd, gen_OldWndProc, oldWndProc);
                  if ((WNDPROC)GetProp(hWnd, gen_OldWndProc) == oldWndProc)
                  {
                     GlobalAddAtom(gen_OldWndProc);
                     SetWindowLongPtr(hWnd, GWLP_WNDPROC, (dword_ptr)__activation_window_procedure);
                     ASSERT(oldWndProc != NULL);
                  }
               }
            }
         }
      }

lCallNextHook:
      LRESULT lResult = CallNextHookEx(pThreadState->m_hHookOldCbtFilter, code,
                                       wParam, lParam);

      return lResult;
   }

#endif


   void interaction_impl::_001OnEraseBkgnd(signal_details * pobj)
   {
      SCAST_PTR(::message::erase_bkgnd,perasebkgnd,pobj);
      perasebkgnd->m_bRet = true;
      perasebkgnd->set_result(TRUE);
   }

   void interaction_impl::_001BaseWndInterfaceMap()
   {
      Session.user()->window_map().set((int_ptr)(void *)get_handle(),this);

   }



   void interaction_impl::_001OnTriggerMouseInside()
   {

      //throw todo(get_app());


//      m_bMouseHover = true;
      //TRACKMOUSEEVENT tme = { sizeof(tme) };
      //tme.dwFlags = TME_LEAVE;
      //tme.hwndTrack = get_handle();
      //TrackMouseEvent(&tme);

   }


} // namespace metrowin




/////////////////////////////////////////////////////////////////////////////
// Map from oswindow to ::user::interaction_impl *

//hwnd_map* afxMapHWND(bool bCreate)
//{
//   UNREFERENCED_PARAMETER(bCreate);
//   try
//   {
//      __MODULE_STATE* pState = __get_module_state();
//      if(pState == NULL)
//         return NULL;
//      return pState->m_pmapHWND;
//   }
//   catch(...)
//   {
//      return NULL;
//   }
//}


//mutex * afxMutexHwnd()
//{
//   try
//   {
//      __MODULE_STATE* pState = __get_module_state();
//      if(pState == NULL)
//         return NULL;
//      return pState->m_pmutexHwnd;
//   }
//   catch(...)
//   {
//      return NULL;
//   }
//}

/////////////////////////////////////////////////////////////////////////////
// The WndProc for all interaction_impl's and derived classes

#ifdef WINDOWSEX

#undef __window_procedure

LRESULT CALLBACK __window_procedure(oswindow hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
   // special message which identifies the interaction_impl as using __window_procedure
   if (nMsg == WM_QUERYAFXWNDPROC)
      return 1;

   // all other messages route through message ::map
   ::user::interaction_impl * pWnd = ::metrowin::interaction_impl::FromHandlePermanent(hWnd);
   //ASSERT(pWnd != NULL);
   //ASSERT(pWnd==NULL || WIN_WINDOW(pWnd)->get_handle() == hWnd);
   if (pWnd == NULL || WIN_WINDOW(pWnd)->get_handle() != hWnd)
      return ::DefWindowProc(hWnd, nMsg, wParam, lParam);
   return metrowin::__call_window_procedure(pWnd, hWnd, nMsg, wParam, lParam);
}

// always indirectly accessed via __get_window_procedure
WNDPROC CLASS_DECL_BASE __get_window_procedure()
{
   return __get_module_state()->m_pfn_window_procedure;
}
/////////////////////////////////////////////////////////////////////////////
// Special helpers for certain windows messages

__STATIC void CLASS_DECL_BASE __pre_init_dialog(
   ::user::interaction * pWnd, LPRECT lpRectOld, uint32_t* pdwStyleOld)
{
   ASSERT(lpRectOld != NULL);
   ASSERT(pdwStyleOld != NULL);

   WIN_WINDOW(pWnd)->GetWindowRect(lpRectOld);
   *pdwStyleOld = WIN_WINDOW(pWnd)->GetStyle();
}

__STATIC void CLASS_DECL_BASE __post_init_dialog(
   ::user::interaction * pWnd, const RECT& rectOld, uint32_t dwStyleOld)
{
   // must be hidden to start with
   if (dwStyleOld & WS_VISIBLE)
      return;

   // must not be visible after WM_INITDIALOG
   if (WIN_WINDOW(pWnd)->GetStyle() & (WS_VISIBLE|WS_CHILD))
      return;

   // must not move during WM_INITDIALOG
   rect rect;
   WIN_WINDOW(pWnd)->GetWindowRect(rect);
   if (rectOld.left != rect.left || rectOld.top != rect.top)
      return;

   // must be unowned or owner disabled
   ::user::interaction * pParent = WIN_WINDOW(pWnd)->GetWindow(GW_OWNER);
   if (pParent != NULL && pParent->IsWindowEnabled())
      return;

   if (!WIN_WINDOW(pWnd)->CheckAutoCenter())
      return;

   // center modal dialog boxes/message boxes
   //WIN_WINDOW(pWnd)->CenterWindow();
}



CLASS_DECL_BASE void hook_window_create(::user::interaction * pWnd)
{
   ___THREAD_STATE* pThreadState = gen_ThreadState.get_data();
   if (pThreadState->m_pWndInit == pWnd)
      return;

   if (pThreadState->m_hHookOldCbtFilter == NULL)
   {
      pThreadState->m_hHookOldCbtFilter = ::SetWindowsHookEx(WH_CBT,
                                          metrowin::__cbt_filter_hook, NULL, ::GetCurrentThreadId());
      if (pThreadState->m_hHookOldCbtFilter == NULL)
         throw memory_exception();
   }
   ASSERT(pThreadState->m_hHookOldCbtFilter != NULL);
   ASSERT(pWnd != NULL);
   // trans   ASSERT(WIN_WINDOW(pWnd)->get_handle() == NULL);   // only do once

   ASSERT(pThreadState->m_pWndInit == NULL);   // hook not already in progress
   pThreadState->m_pWndInit = pWnd;
}

CLASS_DECL_BASE bool unhook_window_create()
{
   ___THREAD_STATE* pThreadState = gen_ThreadState.get_data();
   if (pThreadState->m_pWndInit != NULL)
   {
      pThreadState->m_pWndInit = NULL;
      return FALSE;   // was not successfully hooked
   }
   return TRUE;
}



CLASS_DECL_BASE const char * __register_window_class(UINT nClassStyle,
      HCURSOR hCursor, HBRUSH hbrBackground, HICON hIcon)
{
   // Returns a temporary string name for the class
   //  Save in a string if you want to use it for a long time
   LPTSTR lpszName = __get_thread_state()->m_szTempClassName;

   // generate a synthetic name for this class
   HINSTANCE hInst = Sys(::metrowin::get_thread()->m_papp).m_hInstance;

   if (hCursor == NULL && hbrBackground == NULL && hIcon == NULL)
   {
      C_RUNTIME_ERRORCHECK_SPRINTF(_sntprintf_s(lpszName, ___TEMP_CLASS_NAME_SIZE, ___TEMP_CLASS_NAME_SIZE - 1, "::ca2:::%p:%x", hInst, nClassStyle));
   }
   else
   {
      C_RUNTIME_ERRORCHECK_SPRINTF(_sntprintf_s(lpszName, ___TEMP_CLASS_NAME_SIZE, ___TEMP_CLASS_NAME_SIZE - 1, "::ca2:::%p:%x:%p:%p:%p", hInst, nClassStyle,
                                   hCursor, hbrBackground, hIcon));
   }

   // see if the class already exists
   WNDCLASS wndcls;
   if (::GetClassInfo(hInst, lpszName, &wndcls))
   {
      // already registered, assert everything is good
      ASSERT(wndcls.style == nClassStyle);

      // NOTE: We have to trust that the hIcon, hbrBackground, and the
      //  hCursor are semantically the same, because sometimes Windows does
      //  some internal translation or copying of those handles before
      //  storing them in the internal WNDCLASS retrieved by GetClassInfo.
      return lpszName;
   }

   // otherwise we need to register a new class
   wndcls.style = nClassStyle;
   wndcls.lpfnWndProc = DefWindowProc;
   wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
   wndcls.hInstance = hInst;
   wndcls.hIcon = hIcon;
   //wndcls.hCursor = hCursor;
   wndcls.hCursor = NULL;
   wndcls.hbrBackground = hbrBackground;
   wndcls.lpszMenuName = NULL;
   wndcls.lpszClassName = lpszName;
   if (!__register_class(&wndcls))
      throw resource_exception();

   // return thread-local pointer
   return lpszName;
}



__STATIC void CLASS_DECL_BASE
__handle_activate(::user::interaction_impl * pWnd, WPARAM nState, ::user::interaction_impl * pWndOther)
{
   ASSERT(pWnd != NULL);

   // send WM_ACTIVATETOPLEVEL when top-level parents change
   if (!(WIN_WINDOW(pWnd)->GetStyle() & WS_CHILD))
   {
      ::user::interaction * pTopLevel= WIN_WINDOW(pWnd)->GetTopLevel();
      if (pTopLevel && (pWndOther == NULL || !::WinIsWindow(WIN_WINDOW(pWndOther)->get_handle()) || pTopLevel != WIN_WINDOW(pWndOther)->GetTopLevel()))
      {
         // lParam points to interaction_impl getting the WM_ACTIVATE message and
         //  hWndOther from the WM_ACTIVATE.
         oswindow hWnd2[2];
         hWnd2[0] = WIN_WINDOW(pWnd)->get_handle();
         if(pWndOther == NULL || WIN_WINDOW(pWndOther) == NULL)
         {
            hWnd2[1] = NULL;
         }
         else
         {
            hWnd2[1] = WIN_WINDOW(pWndOther)->get_handle();
         }
         // send it...
         pTopLevel->send_message(WM_ACTIVATETOPLEVEL, nState, (LPARAM)&hWnd2[0]);
      }
   }
}

__STATIC bool CLASS_DECL_BASE
__handle_set_cursor(::user::interaction_impl * pWnd, UINT nHitTest, UINT nMsg)
{
   if (nHitTest == HTERROR &&
         (nMsg == WM_LBUTTONDOWN || nMsg == WM_MBUTTONDOWN ||
          nMsg == WM_RBUTTONDOWN))
   {
      // activate the last active interaction_impl if not active
      ::user::interaction * pLastActive = WIN_WINDOW(pWnd)->GetTopLevel();
      if (pLastActive != NULL)
         pLastActive = pLastActive->GetLastActivePopup();
      if (pLastActive != NULL &&
            pLastActive != ::metrowin::interaction_impl::GetForegroundWindow() &&
            pLastActive->IsWindowEnabled())
      {
         pLastActive->SetForegroundWindow();
         return TRUE;
      }
   }
   return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// Standard init called by WinMain

__STATIC bool CLASS_DECL_BASE __register_with_icon(WNDCLASS* pWndCls,
      const char * lpszClassName, UINT nIDIcon)
{
   pWndCls->lpszClassName = lpszClassName;
   pWndCls->hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
   return __register_class(pWndCls);
}


bool CLASS_DECL_BASE __end_defer_register_class(LONG fToRegisterParam, const char ** ppszClass)
{
   // mask off all classes that are already registered
   __MODULE_STATE* pModuleState = __get_module_state();
   LONG fToRegister = fToRegisterParam & ~pModuleState->m_fRegisteredClasses;
   if (fToRegister == 0)
   {
      fToRegister = fToRegisterParam;
      if(ppszClass != NULL)
      {
         if(fToRegister & __WND_REG)
         {
            *ppszClass = gen_Wnd;
         }
         else if (fToRegister & __WNDOLECONTROL_REG)
         {
            *ppszClass = gen_WndOleControl;
         }
         else if (fToRegister & __WNDCONTROLBAR_REG)
         {
            *ppszClass = gen_WndControlBar;
         }
         else if(fToRegister & __WNDMDIFRAME_REG)
         {
            *ppszClass = gen_WndMDIFrame;
         }
         else if(fToRegister & __WNDFRAMEORVIEW_REG)
         {
            *ppszClass = gen_WndFrameOrView;
         }
      }
      return TRUE;
   }

   LONG fRegisteredClasses = 0;

   // common initialization
   WNDCLASS wndcls;
   memset(&wndcls, 0, sizeof(WNDCLASS));   // start with NULL defaults
   wndcls.lpfnWndProc = DefWindowProc;
   wndcls.hInstance = Sys(::metrowin::get_thread()->m_papp).m_hInstance;
   //wndcls.hCursor = afxData.hcurArrow;

   INITCOMMONCONTROLSEX init;
   init.dwSize = sizeof(init);

   // work to register classes as specified by fToRegister, populate fRegisteredClasses as we go
   if (fToRegister & __WND_REG)
   {
      // Child windows - no brush, no icon, safest default class styles
      wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
      wndcls.lpszClassName = gen_Wnd;
      if (__register_class(&wndcls))
      {
         if(ppszClass != NULL)
         {
            *ppszClass = wndcls.lpszClassName;
         }
         fRegisteredClasses |= __WND_REG;
      }
   }
   if (fToRegister & __WNDOLECONTROL_REG)
   {
      // OLE control windows - use parent DC for speed
      wndcls.style |= CS_PARENTDC | CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
      wndcls.lpszClassName = gen_WndOleControl;
      if (__register_class(&wndcls))
      {
         if(ppszClass != NULL)
         {
            *ppszClass = wndcls.lpszClassName;
         }
         fRegisteredClasses |= __WNDOLECONTROL_REG;
      }
   }
   if (fToRegister & __WNDCONTROLBAR_REG)
   {
      // control bar windows
      wndcls.style = 0;   // control bars don't handle double click
      wndcls.lpszClassName = gen_WndControlBar;
      wndcls.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
      if (__register_class(&wndcls))
      {
         if(ppszClass != NULL)
         {
            *ppszClass = wndcls.lpszClassName;
         }
         fRegisteredClasses |= __WNDCONTROLBAR_REG;
      }
   }
   if (fToRegister & __WNDMDIFRAME_REG)
   {
      // MDI Frame interaction_impl (also used for splitter interaction_impl)
      wndcls.style = CS_DBLCLKS;
      wndcls.hbrBackground = NULL;
      /*      if (__register_with_icon(&wndcls, gen_WndMDIFrame, __IDI_STD_MDIFRAME))
      {
      if(ppszClass != NULL)
      {
      *ppszClass = gen_WndMDIFrame;
      }
      fRegisteredClasses |= __WNDMDIFRAME_REG;
      }*/
   }
   if (fToRegister & __WNDFRAMEORVIEW_REG)
   {
      // SDI Frame or MDI Child windows or views - normal colors
      wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
      wndcls.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
      if (__register_with_icon(&wndcls, gen_WndFrameOrView, 123))
      {
         if(ppszClass != NULL)
         {
            *ppszClass = gen_WndFrameOrView;
         }
         fRegisteredClasses |= __WNDFRAMEORVIEW_REG;
      }
   }


   // save new state of registered controls
   pModuleState->m_fRegisteredClasses |= fRegisteredClasses;

   // special case for all common controls registered, turn on __WNDCOMMCTLS_REG
   if ((pModuleState->m_fRegisteredClasses & __WIN95CTLS_MASK) == __WIN95CTLS_MASK)
   {
      pModuleState->m_fRegisteredClasses |= __WNDCOMMCTLS_REG;
      fRegisteredClasses |= __WNDCOMMCTLS_REG;
   }

   // must have registered at least as mamy classes as requested
   return (fToRegister & fRegisteredClasses) == fToRegister;
}


/////////////////////////////////////////////////////////////////////////////
// Special WndProcs (activation handling & gray dialogs)


LRESULT CALLBACK
__activation_window_procedure(oswindow hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
   WNDPROC oldWndProc = (WNDPROC)::GetProp(hWnd, gen_OldWndProc);
   ASSERT(oldWndProc != NULL);

   LRESULT lResult = 0;
   try
   {
      bool bCallDefault = TRUE;
      switch (nMsg)
      {
      case WM_INITDIALOG:
         {
            uint32_t dwStyle;
            rect rectOld;
            ::user::interaction_impl * pWnd = ::metrowin::interaction_impl::from_handle(hWnd);
            __pre_init_dialog(pWnd, &rectOld, &dwStyle);
            bCallDefault = FALSE;
            lResult = CallWindowProc(oldWndProc, hWnd, nMsg, wParam, lParam);
            __post_init_dialog(pWnd, rectOld, dwStyle);
         }
         break;

      case WM_ACTIVATE:
         __handle_activate(::metrowin::interaction_impl::from_handle(hWnd), wParam,
                           ::metrowin::interaction_impl::from_handle((oswindow)lParam));
         break;

      case WM_SETCURSOR:
         bCallDefault = !__handle_set_cursor(::metrowin::interaction_impl::from_handle(hWnd),
                                             (short)LOWORD(lParam), HIWORD(lParam));
         break;

      case WM_NCDESTROY:
         SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<int_ptr>(oldWndProc));
         RemoveProp(hWnd, gen_OldWndProc);
         GlobalDeleteAtom(GlobalFindAtom(gen_OldWndProc));
         break;
      }

      // call original wndproc for default handling
      if (bCallDefault)
         lResult = CallWindowProc(oldWndProc, hWnd, nMsg, wParam, lParam);
   }
   catch(::exception::aura * pe)
   {
      // handle exception
      MSG msg;
      msg.hwnd = hWnd;
      msg.message = nMsg;
      msg.wParam = wParam;
      msg.lParam = lParam;

      //lResult = __process_window_procedure_exception(pe, &msg);
      //      TRACE(::core::trace::category_AppMsg, 0, "Warning: Uncaught exception in __activation_window_procedure (returning %ld).\n",
      //       lResult);
      pe->Delete();
   }


   return lResult;
}




/////////////////////////////////////////////////////////////////////////////
// Additional helpers for WNDCLASS init

// like RegisterClass, except will automatically call UnregisterClass
bool CLASS_DECL_BASE __register_class(WNDCLASS* lpWndClass)
{
   WNDCLASS wndcls;
   if (GetClassInfo(lpWndClass->hInstance, lpWndClass->lpszClassName,
                    &wndcls))
   {
      // class already registered
      return TRUE;
   }

   if (!::RegisterClass(lpWndClass))
   {
      //      TRACE(::core::trace::category_AppMsg, 0, "Can't register interaction_impl class named %s\n",
      //       lpWndClass->lpszClassName);
      return FALSE;
   }

   bool bRet = TRUE;

   if (afxContextIsDLL)
   {

      try
      {
         // class registered successfully, add to registered list
         __MODULE_STATE* pModuleState = __get_module_state();
         single_lock sl(&pModuleState->m_mutexRegClassList, TRUE);
         if(pModuleState->m_pstrUnregisterList == NULL)
            pModuleState->m_pstrUnregisterList = new string;
         *pModuleState->m_pstrUnregisterList += lpWndClass->lpszClassName;
         *pModuleState->m_pstrUnregisterList +='\n';
      }
      catch(::exception::aura * pe)
      {
         ::ca2::rethrow(pe);
         // Note: DELETE_EXCEPTION not required.
      }

   }

   return bRet;
}

#endif


namespace metrowin
{

   /*   void interaction_impl::_001DeferPaintLayeredWindowBackground(::draw2d::dib * pdib)
   {


   }*/

   //void interaction_impl::_001UpdateWindow()
   //{

   //   throw todo(get_app());


   //   //rect rectWindow;

   //   //GetWindowRect(rectWindow);

   //   //if(rectWindow.area() <= 0)
   //   //   return;


   //   //POINT pt;
   //   //SIZE sz;

   //   //pt.x = rectWindow.left;
   //   //pt.y = rectWindow.top;
   //   //sz.cx = rectWindow.right - rectWindow.left;
   //   //sz.cy = rectWindow.bottom - rectWindow.top;

   //   //int cx = sz.cx;
   //   //int cy = sz.cy;

   //   //BITMAPINFO info;
   //   //COLORREF * pcolorref;

   //   //ZeroMemory(&info, sizeof (BITMAPINFO));

   //   //info.bmiHeader.biSize          = sizeof (BITMAPINFOHEADER);
   //   //info.bmiHeader.biWidth         = cx;
   //   //info.bmiHeader.biHeight        = - cy;
   //   //info.bmiHeader.biPlanes        = 1;
   //   //info.bmiHeader.biBitCount      = 32;
   //   //info.bmiHeader.biCompression   = BI_RGB;
   //   //info.bmiHeader.biSizeImage     = cx * cy * 4;

   //   //HBITMAP hbitmap = CreateDIBSection(NULL, &info, DIB_RGB_COLORS, (void **) &pcolorref, NULL, NULL);

   //   //{

   //   //   memset(pcolorref, 0, cx * cy * 4);

   //   //   Gdiplus::Bitmap b(cx, cy, cx *4 , PixelFormat32bppARGB, (BYTE *) pcolorref);

   //   //   ::draw2d::graphics_sp spg(get_app());

   //   //   (dynamic_cast < ::metrowin::graphics * > (spg.m_p))->attach(new Gdiplus::Graphics(&b));

   //   //   _001Print(spg);

   //   //}

   //   //if(GetExStyle() & WS_EX_LAYERED)
   //   //{
   //   //   BYTE *dst=(BYTE*)pcolorref;
   //   //   int64_t size = cx * cy;


   //   //   // >> 8 instead of / 255 subsequent alpha_blend operations say thanks on true_blend because (255) * (1/254) + (255) * (254/255) > 255


   //   //   while (size >= 8)
   //   //   {
   //   //      dst[0] = LOBYTE(((int)dst[0] * (int)dst[3])>> 8);
   //   //      dst[1] = LOBYTE(((int)dst[1] * (int)dst[3])>> 8);
   //   //      dst[2] = LOBYTE(((int)dst[2] * (int)dst[3])>> 8);

   //   //      dst[4+0] = LOBYTE(((int)dst[4+0] * (int)dst[4+3])>> 8);
   //   //      dst[4+1] = LOBYTE(((int)dst[4+1] * (int)dst[4+3])>> 8);
   //   //      dst[4+2] = LOBYTE(((int)dst[4+2] * (int)dst[4+3])>> 8);

   //   //      dst[8+0] = LOBYTE(((int)dst[8+0] * (int)dst[8+3])>> 8);
   //   //      dst[8+1] = LOBYTE(((int)dst[8+1] * (int)dst[8+3])>> 8);
   //   //      dst[8+2] = LOBYTE(((int)dst[8+2] * (int)dst[8+3])>> 8);

   //   //      dst[12+0] = LOBYTE(((int)dst[12+0] * (int)dst[12+3])>> 8);
   //   //      dst[12+1] = LOBYTE(((int)dst[12+1] * (int)dst[12+3])>> 8);
   //   //      dst[12+2] = LOBYTE(((int)dst[12+2] * (int)dst[12+3])>> 8);

   //   //      dst[16+0] = LOBYTE(((int)dst[16+0] * (int)dst[16+3])>> 8);
   //   //      dst[16+1] = LOBYTE(((int)dst[16+1] * (int)dst[16+3])>> 8);
   //   //      dst[16+2] = LOBYTE(((int)dst[16+2] * (int)dst[16+3])>> 8);

   //   //      dst[20+0] = LOBYTE(((int)dst[20+0] * (int)dst[20+3])>> 8);
   //   //      dst[20+1] = LOBYTE(((int)dst[20+1] * (int)dst[20+3])>> 8);
   //   //      dst[20+2] = LOBYTE(((int)dst[20+2] * (int)dst[20+3])>> 8);

   //   //      dst[24+0] = LOBYTE(((int)dst[24+0] * (int)dst[24+3])>> 8);
   //   //      dst[24+1] = LOBYTE(((int)dst[24+1] * (int)dst[24+3])>> 8);
   //   //      dst[24+2] = LOBYTE(((int)dst[24+2] * (int)dst[24+3])>> 8);

   //   //      dst[28+0] = LOBYTE(((int)dst[28+0] * (int)dst[28+3])>> 8);
   //   //      dst[28+1] = LOBYTE(((int)dst[28+1] * (int)dst[28+3])>> 8);
   //   //      dst[28+2] = LOBYTE(((int)dst[28+2] * (int)dst[28+3])>> 8);

   //   //      dst += 4 * 8;
   //   //      size -= 8;
   //   //   }
   //   //   while(size--)
   //   //   {
   //   //      dst[0] = LOBYTE(((int)dst[0] * (int)dst[3])>> 8);
   //   //      dst[1] = LOBYTE(((int)dst[1] * (int)dst[3])>> 8);
   //   //      dst[2] = LOBYTE(((int)dst[2] * (int)dst[3])>> 8);
   //   //      dst += 4;
   //   //   }


   //   //   {
   //   //      HDC hdcScreen = ::GetDC(get_handle());

   //   //      HDC hdcMem = ::CreateCompatibleDC(NULL);

   //   //      HBITMAP hbitmapOld = (HBITMAP) ::SelectObject(hdcMem, hbitmap);

   //   //      BLENDFUNCTION blendPixelFunction = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

   //   //      POINT ptZero = { 0 };

   //   //      point ptSrc(0, 0);

   //   //      bool bOk = ::UpdateLayeredWindow(get_handle(), hdcScreen, &pt, &sz, hdcMem, &ptSrc, RGB(0, 0, 0), &blendPixelFunction, ULW_ALPHA) != FALSE;

   //   //      ::SelectObject(hdcMem, hbitmapOld);

   //   //      ::DeleteDC(hdcMem);

   //   //      ::ReleaseDC(get_handle(), hdcScreen);
   //   //   }


   //   //}
   //   //else
   //   //{

   //   //   {
   //   //      HDC hdcScreen = ::GetDC(get_handle());

   //   //      HDC hdcMem = ::CreateCompatibleDC(NULL);

   //   //      HBITMAP hbitmapOld = (HBITMAP) ::SelectObject(hdcMem, hbitmap);

   //   //      BLENDFUNCTION blendPixelFunction = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

   //   //      POINT ptZero = { 0 };

   //   //      point ptSrc(0, 0);

   //   //      ::BitBlt(hdcScreen, 0, 0, sz.cx, sz.cy, hdcMem, 0, 0, SRCCOPY);

   //   //      ::SelectObject(hdcMem, hbitmapOld);

   //   //      ::DeleteDC(hdcMem);

   //   //      ::ReleaseDC(get_handle(), hdcScreen);
   //   //   }

   //   //}

   //   //::DeleteObject(hbitmap);

   //}


   ::user::interaction *  interaction_impl::get_wnd() const
   {

      return  m_pui;

   }


   //::user::interaction_impl *  interaction_impl::get_impl() const
   //{

   //   return  this;

   //}

   Agile<Windows::UI::Core::CoreWindow> interaction_impl::get_os_window()
   {
      return m_window;
   }


   void interaction_impl::set_view_port_org(::draw2d::dib * pdib)
   {
      // graphics will be already set its view port to the interaction_impl for linux - cairo with xlib


      rect64 rectWindow;
      GetWindowRect(rectWindow);
      //      get_wnd()->ScreenToClient(rectWindow);
      pdib->SetViewportOrg(point(rectWindow.top_left()));
      //pdib->SelectClipRgn(NULL);

   }


   void interaction_impl::offset_view_port_org(LPRECT lprectScreen)
   {
   }


   bool interaction_impl::has_pending_graphical_update()
   {

      if (::user::interaction_impl::has_pending_graphical_update())
      {

         return true;

      }

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


   void interaction_impl::on_after_graphical_update()
   {

      m_pui->on_after_graphical_update();

      synch_lock sl(m_pui->m_pmutex);

      for (auto p : m_pui->m_uiptraChild)
      {

         p->on_after_graphical_update();

      }

   }


} // namespace metrowin





