#include "framework.h" // from ""axis/user/user.h"
#include "linux.h"
#include <X11/Xatom.h>
#include "base/os/linux/linux_window_xlib.h"
#include "base/os/linux/linux_windowing.h"

#define TEST 0

void wm_iconify_window(oswindow w);

Display * x11_get_display();
void wm_state_above_raw(oswindow w, bool bSet);
void wm_toolwindow(oswindow w, bool bSet);
void wm_state_hidden(oswindow w, bool bSet);
//#include <X11/extensions/Xcomposite.h>

CLASS_DECL_BASE thread_int_ptr < DWORD_PTR > t_time1;

//#define COMPILE_MULTIMON_STUBS
//#include <multimon.h>

//#include "sal.h"

CLASS_DECL_BASE void hook_window_create(::user::interaction * pWnd);
CLASS_DECL_BASE bool unhook_window_create();
void CLASS_DECL_BASE __pre_init_dialog(
   ::user::interaction * pWnd, LPRECT lpRectOld, DWORD* pdwStyleOld);
void CLASS_DECL_BASE __post_init_dialog(
   ::user::interaction * pWnd, const RECT& rectOld, DWORD dwStyleOld);
LRESULT CALLBACK
   __activation_window_procedure(oswindow hWnd, UINT nMsg, WPARAM wparam, LPARAM lparam);

const char gen_OldWndProc[] = "::ca2::OldWndProc423";

/*const char gen_WndControlBar[] = __WNDCONTROLBAR;
const char gen_WndMDIFrame[] = __WNDMDIFRAME;
const char gen_WndFrameOrView[] = __WNDFRAMEORVIEW;
const char gen_WndOleControl[] = __WNDOLECONTROL;
*/
struct __CTLCOLOR
{
   oswindow hWnd;
   HDC hDC;
   UINT nCtlType;
};

//extern cairo_surface_t *  g_cairosurface;
//extern cairo_t *  g_cairo;

WINBOOL PeekMessage(
    LPMESSAGE lpMsg,
    oswindow hWnd,
    UINT wMsgFilterMin,
    UINT wMsgFilterMax,
    UINT wRemoveMsg);


WINBOOL GetMessage(
    LPMESSAGE lpMsg,
    oswindow hWnd,
    UINT wMsgFilterMin,
    UINT wMsgFilterMax);

/*
const char gen_Wnd[] = __WND;
*/

namespace linux
{

   interaction_impl::interaction_impl() :
   ::aura::timer_array(get_app())
   {

      set_handle(NULL);
      m_bExposing    = false;
      m_bEnabled     = true;
      m_pthreadDraw     = NULL;

   }


   void interaction_impl::construct(oswindow hWnd)
   {

      set_handle(hWnd);
      m_bExposing    = false;
      m_bEnabled     = true;

   }

   interaction_impl::interaction_impl(sp(::aura::application) papp) :
      ::object(papp),
      ::aura::timer_array(papp)
   {

      set_handle(NULL);
      m_bExposing    = false;
      m_bEnabled     = true;
      m_pthreadDraw     = NULL;

   }


   interaction_impl::~interaction_impl()
   {


   }


   ::user::interaction_impl * interaction_impl::from_os_data(void * pdata)
   {
      return from_handle((oswindow) pdata);
   }

   void * interaction_impl::get_os_data() const
   {
      return m_oswindow;
   }



   // Change a interaction_impl's style

   /*__STATIC bool CLASS_DECL_BASE __modify_style(oswindow hWnd, int32_t nStyleOffset,
      DWORD dwRemove, DWORD dwAdd, UINT nFlags)
   {
      ASSERT(hWnd != NULL);
      DWORD dwStyle = ::GetWindowLong(hWnd, nStyleOffset);
      DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
      if (dwStyle == dwNewStyle)
         return FALSE;

      ::SetWindowLong(hWnd, nStyleOffset, dwNewStyle);
      if (nFlags != 0)
      {
         ::SetWindowPos(hWnd, NULL, 0, 0, 0, 0,
            SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
      }
      return TRUE;
   }*/

//   bool  interaction_impl::ModifyStyle(oswindow hWnd, DWORD dwRemove, DWORD dwAdd, UINT nFlags)
//   {
//      DWORD dw = (DWORD) hWnd->get_window_long_ptr(GWL_STYLE);
//      dw &= ~dwRemove;
//      dw |= dwAdd;
//      hWnd->set_window_long_ptr(GWL_STYLE, dw);
//      //return __modify_style(hWnd, GWL_STYLE, dwRemove, dwAdd, nFlags);
//      return true;
//   }
//
//   bool interaction_impl::ModifyStyleEx(oswindow hWnd, DWORD dwRemove, DWORD dwAdd, UINT nFlags)
//   {
//      DWORD dw = hWnd->get_window_long(GWL_EXSTYLE);
//      dw &= ~dwRemove;
//      dw |= dwAdd;
//      hWnd->set_window_long(GWL_EXSTYLE, dw);
//      return true;
////      return __modify_style(hWnd, GWL_EXSTYLE, dwRemove, dwAdd, nFlags);
//   }



   const MESSAGE* PASCAL interaction_impl::GetCurrentMessage()
   {
      return NULL;
   }

   LRESULT interaction_impl::Default()
   {
         return 0;
   }


   ::user::interaction_impl * interaction_impl::from_handle(oswindow oswindow)
   {

      if(oswindow == NULL)
         return NULL;

      if(oswindow->m_pimpl == NULL)
         return NULL;

      return oswindow->m_pimpl;

   }


   ::user::interaction_impl * interaction_impl::FromHandlePermanent(oswindow oswindow)
   {

      if(oswindow->m_pimpl == NULL)
         return NULL;

      return oswindow->m_pimpl;

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

      oswindow hwnd = (oswindow) get_handle();

      if (hwnd != NULL)
      {

         m_oswindow = NULL;

      }

      return hwnd;

   }


   void interaction_impl::pre_subclass_window()
   {


   }


   bool interaction_impl::create_window_ex(
      ::user::interaction * pui,
      DWORD dwExStyle,
      const char * lpszClassName,
      const char * lpszWindowName,
      DWORD dwStyle,
      const RECT& rect,
      ::user::interaction * puiParent,
      id id,
      LPVOID lpParam /* = NULL */)
   {

      if(!native_create_window_ex(pui, dwExStyle, lpszClassName, lpszWindowName, dwStyle, rect, puiParent == NULL ? NULL : puiParent->get_safe_handle(), id, lpParam))
         return false;

      return true;

   }


   bool interaction_impl::native_create_window_ex(
      ::user::interaction * pui,
      DWORD dwExStyle,
      const char * lpszClassName,
      const char * lpszWindowName,
      DWORD dwStyle,
      const RECT& rect,
      oswindow hWndParent,
      id id,
      LPVOID lpParam)
   {

      m_pui = pui;

      UNREFERENCED_PARAMETER(id);

      ENSURE_ARG(lpszWindowName == NULL || __is_valid_string(lpszWindowName));

      // allow modification of several common create parameters

      ::user::create_struct cs;

      cs.dwExStyle      = dwExStyle;
      cs.lpszClass      = lpszClassName;
      cs.lpszName       = lpszWindowName;
      cs.style          = dwStyle;
      cs                = rect;
      cs.hwndParent     = hWndParent;
      //cs.hMenu        = hWndParent == NULL ? NULL : nIDorHMenu;
      cs.hMenu          = NULL;
      //cs.hInstance    = System.m_hInstance;
      cs.lpCreateParams = lpParam;


      if(m_pui != NULL)
      {

         if(!m_pui->pre_create_window(cs))
         {

            PostNcDestroy();

            return false;

         }

      }
      else
      {

         if (!pre_create_window(cs))
         {

            PostNcDestroy();

            return false;

         }

      }

      if(cs.hwndParent == NULL)
      {

         cs.style &= ~WS_CHILD;

      }

      //hook_window_create(this);

      if(cs.hwndParent == (oswindow) HWND_MESSAGE)
      {

         m_oswindow = oswindow_get_message_only_window(this);

         if(m_oswindow != NULL)
         {

            m_pui->m_id = id;

         }

         m_pui->m_bMessageWindow = true;

         send_message(WM_CREATE, 0, (LPARAM) &cs);

      }
      else
      {

         m_pui->m_bMessageWindow = false;

         Display * display    = x11_get_display();

         if(display == NULL)
         {

            fprintf(stderr, "ERROR: Could not open display\n");

            return false;

         }

         xdisplay d(display);

         m_iScreen            =  DefaultScreen(display);

         Window rootwin       =  RootWindow(display, m_iScreen);

         XEvent e;

         if(cs.cx <= 256)
         {

            cs.cx = 256;

         }

         if(cs.cy <= 256)
         {

            cs.cy = 256;

         }

         // query Visual for "TrueColor" and 32 bits depth (RGBA)

         Visual * vis = DefaultVisual(display, DefaultScreen(display));

         m_iDepth = 0;

         if(XMatchVisualInfo(display, DefaultScreen(display), 32, TrueColor, &m_visualinfo))
         {

            vis = m_visualinfo.visual;

         }
         else
         {

            ZERO(m_visualinfo);

         }

         m_iDepth = m_visualinfo.depth;

         XSetWindowAttributes attr;

         ZERO(attr);

         attr.colormap = XCreateColormap( display, rootwin, vis, AllocNone);

         attr.event_mask = PropertyChangeMask | ExposureMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask | PointerMotionMask | StructureNotifyMask | FocusChangeMask;

         attr.background_pixmap = None;

         attr.border_pixmap = None;

         attr.border_pixel = 0;

         attr.override_redirect = False;

         Window window = XCreateWindow(display, DefaultRootWindow(display), cs.x, cs.cy, cs.cx, cs.cy, 0, m_iDepth, InputOutput, vis, CWColormap|CWEventMask|CWBackPixmap|CWBorderPixel, &attr);

         if (window == 0)
         {

            DWORD dwLastError = GetLastError();

            string strLastError = FormatMessageFromSystem(dwLastError);

            string strMessage;

            strMessage.Format("%s\n\nSystem Error Code: %d", strLastError, dwLastError);

            TRACE(::aura::trace::category_AppMsg, 0, "Warning: oswindow creation failed: GetLastError returned:\n");

            TRACE(::aura::trace::category_AppMsg, 0, "%s\n", strMessage);

            try
            {

               if(dwLastError == 0x0000057e)
               {

                  System.simple_message_box(NULL, "cannot create a top-level child interaction_impl.");

               }
               else
               {

                  System.simple_message_box(NULL, strMessage);

               }

            }
            catch(...)
            {

            }

            return false;

         }

         m_oswindow = oswindow_get(display, window, vis, m_iDepth, m_iScreen, attr.colormap);

         Window root = 0;

         Window * pchildren = NULL;

         uint32_t ncount = 0;

         XQueryTree(display, window, &root, &m_oswindow->m_parent, &pchildren, &ncount);

         if(pchildren != NULL)
         {

            XFree(pchildren);

         }

         m_oswindow->set_user_interaction(this);

         m_pui->add_ref();

         HTHREAD hthread = ::GetCurrentThread();

         m_oswindow->m_hthread = hthread;

         XGetWindowAttributes(m_oswindow->display(), m_oswindow->window(), &m_attr);

         int event_base, error_base, major_version, minor_version;

         m_bComposite = XGetSelectionOwner(m_oswindow->display(), XInternAtom(m_oswindow->display(), "_NET_WM_CM_S0", True));

         if(lpszWindowName != NULL && strlen(lpszWindowName) > 0)
         {

            XStoreName(m_oswindow->display(), m_oswindow->window(), lpszWindowName);

         }

         if(cs.dwExStyle & WS_EX_TOOLWINDOW)
         {

            m_oswindow->set_window_long_ptr(GWL_EXSTYLE, m_oswindow->get_window_long_ptr(GWL_EXSTYLE) |  WS_EX_TOOLWINDOW);

         }

         wm_nodecorations(m_oswindow, 0);

         if(cs.style & WS_VISIBLE)
         {

            XMapWindow(m_oswindow->display(), m_oswindow->window());

         }


         d.unlock();

                        //if(pshowwindow->m_bShow)
      {

         //::rect rect32;

         //(::GetWindowRect((oswindow) get_handle(), rect32))
         {


            if(m_pauraapp != NULL && m_pauraapp->m_pbasesession != NULL)
            {

            point pt;
            m_pauraapp->m_pbasesession->get_cursor_pos(pt);

            m_rectParentClient.left = pt.x;
            m_rectParentClient.top = pt.y;
            m_rectParentClient.right = pt.x;
            m_rectParentClient.bottom = pt.y;

            }

         }

      }



         m_pui->m_id = id;

         m_pui->send_message(WM_CREATE, 0, (LPARAM) &cs);

         m_pui->send_message(WM_SIZE);

      }

      return true;

   }


   bool interaction_impl::pre_create_window(::user::create_struct & cs)
   {

      return true;

   }


   bool interaction_impl::create_window(
      ::user::interaction * pui,
      const char * lpszClassName,
      const char * lpszWindowName,
      DWORD dwStyle,
      const RECT& rect,
      ::user::interaction * pParentWnd,
      id id,
      sp(::create) pContext)
   {
      // can't use for desktop or pop-up windows (use CreateEx instead)
      ASSERT(pParentWnd != NULL);
      ASSERT((dwStyle & WS_POPUP) == 0);

      if(!create_window_ex(pui, 0, lpszClassName, lpszWindowName,dwStyle | WS_CHILD, rect, pParentWnd, id, (LPVOID)pContext))
        return false;

      return true;

   }


   bool interaction_impl::create_message_queue(
      ::user::interaction * pui,
      const char * pszName)
   {

      if(IsWindow())
      {

         SetWindowText(pszName);

      }
      else
      {

         if(!native_create_window_ex(pui, 0, NULL, pszName, WS_CHILD,null_rect(),HWND_MESSAGE))
         {

            return false;

         }

      }

      return true;

   }



   void interaction_impl::install_message_handling(::message::dispatch * pinterface)
   {
      //m_pbuffer->InstallMessageHandling(pinterface);

      ::user::interaction_impl::last_install_message_handling(pinterface);
      ::user::interaction_impl::install_message_handling(pinterface);

      IGUI_WIN_MSG_LINK(WM_NCDESTROY,pinterface,this,&interaction_impl::_001OnNcDestroy);

      if(!m_pui->m_bMessageWindow)
      {
         IGUI_WIN_MSG_LINK(WM_PAINT,pinterface,this,&interaction_impl::_001OnPaint);
         IGUI_WIN_MSG_LINK(WM_PRINT,pinterface,this,&interaction_impl::_001OnPrint);
      }

      m_pui->install_message_handling(pinterface);
      IGUI_WIN_MSG_LINK(WM_CREATE,pinterface,this,&interaction_impl::_001OnCreate);


      //IGUI_WIN_MSG_LINK(WM_SIZE              , pinterface, this, &interaction_impl::_001OnSize);

      if(!m_pui->m_bMessageWindow)
      {
         IGUI_WIN_MSG_LINK(WM_SETCURSOR,pinterface,this,&interaction_impl::_001OnSetCursor);
         //IGUI_WIN_MSG_LINK(WM_ERASEBKGND,pinterface,this,&interaction_impl::_001OnEraseBkgnd);
         //IGUI_WIN_MSG_LINK(WM_SIZE,pinterface,this,&interaction_impl::_001OnSize);
         //IGUI_WIN_MSG_LINK(WM_NCCALCSIZE,pinterface,this,&interaction_impl::_001OnNcCalcSize);

         // linux
         //IGUI_WIN_MSG_LINK(WM_MOVE              , pinterface, this, &interaction_impl::_001OnMove);
         IGUI_WIN_MSG_LINK(WM_SHOWWINDOW        , pinterface, this, &interaction_impl::_001OnShowWindow);

         //IGUI_WIN_MSG_LINK(WM_WINDOWPOSCHANGING,pinterface,this,&interaction_impl::_001OnWindowPosChanging);
         //IGUI_WIN_MSG_LINK(WM_WINDOWPOSCHANGED,pinterface,this,&interaction_impl::_001OnWindowPosChanged);
         //IGUI_WIN_MSG_LINK(WM_GETMINMAXINFO,pinterface,this,&interaction_impl::_001OnGetMinMaxInfo);
         //IGUI_WIN_MSG_LINK(WM_SETFOCUS,pinterface,this,&interaction_impl::_001OnSetFocus);
         //IGUI_WIN_MSG_LINK(WM_KILLFOCUS,pinterface,this,&interaction_impl::_001OnKillFocus);
//         IGUI_WIN_MSG_LINK(ca2m_PRODEVIAN_SYNCH,pinterface,this,&interaction_impl::_001OnProdevianSynch);
         ::user::interaction_impl::prio_install_message_handling(pinterface);
      }
      IGUI_WIN_MSG_LINK(WM_DESTROY,pinterface,this,&interaction_impl::_001OnDestroy);

   }

   void interaction_impl::_001OnShowWindow(::signal_details * pobj)
   {

      SCAST_PTR(::message::show_window, pshowwindow, pobj);

    }


   void interaction_impl::_001OnDestroy(::signal_details * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

      Default();

      ::multithreading::post_quit_and_wait(m_pthreadDraw, seconds(10));

   }


   // WM_NCDESTROY is the absolute LAST message sent.
   void interaction_impl::_001OnNcDestroy(::signal_details * pobj)
   {

      single_lock sl(m_pauraapp == NULL ? NULL : m_pauraapp->m_pmutex, TRUE);

      pobj->m_bRet = true;

      // cleanup main and active windows

      ::thread* pThread = ::get_thread();

      if (pThread != NULL)
      {

         if (pThread->get_active_ui() == m_pui)
            pThread->set_active_ui(NULL);

      }

      if(m_pui != NULL)
      {

         if (m_pui->m_nFlags & WF_TOOLTIPS)
         {

         }

      }

      Detach();

      ASSERT(get_handle() == NULL);

      m_pfnDispatchWindowProc = &interaction_impl::_start_user_message_handler;

      PostNcDestroy();

      if(m_pui != NULL)
      {
         m_pui->PostNcDestroy();
      }

   }


   void interaction_impl::PostNcDestroy()
   {

    ::user::interaction_impl::PostNcDestroy();

      m_oswindow->post_nc_destroy();

      //m_pui->PostNcDestroy();

   }

   void interaction_impl::on_final_release()
   {

      if (get_handle() != NULL)
         DestroyWindow();
      else
         PostNcDestroy();

   }


   void interaction_impl::assert_valid() const
   {

      if (((interaction_impl *) this)->get_handle() == NULL)
         return;

   }


   void interaction_impl::dump(dump_context & dumpcontext) const
   {
      ::object::dump(dumpcontext);

      dumpcontext << "\nm_hWnd = " << (void *)((interaction_impl *) this)->get_handle();

/*      if (get_handle() == NULL || get_handle() == oswindow_BOTTOM ||
         get_handle() == oswindow_TOPMOST || get_handle() == oswindow_NOTOPMOST)
      {
         // not a normal interaction_impl - nothing more to dump
         return;
      }*/

/*      if (!::IsWindow((oswindow) get_handle()))
      {
         // not a valid interaction_impl
         dumpcontext << " (illegal oswindow)";
         return; // don't do anything more
      }*/

      sp(::user::interaction_impl) pWnd = (::user::interaction_impl *) this;
      if (pWnd.m_p != this)
         dumpcontext << " (Detached or temporary interaction_impl)";
      else
         dumpcontext << " (permanent interaction_impl)";

      // dump out interaction_impl specific statistics
      char szBuf [64];
//      if (!const_cast < interaction_impl * > (this)->send_message(WM_QUERYAFXWNDPROC, 0, 0) && pWnd == this)
  //       ((::interaction_impl *) this)->get_window_text(szBuf, _countof(szBuf));
  //    else
//         ::DefWindowProc(get_handle(), WM_GETTEXT, _countof(szBuf), (LPARAM)&szBuf[0]);
    //  dumpcontext << "\ncaption = \"" << szBuf << "\"";

//      ::GetClassName(get_handle(), szBuf, _countof(szBuf));
  //    dumpcontext << "\nclass name = \"" << szBuf << "\"";

      rect rect;
      ((::user::interaction_impl *) this)->GetWindowRect(&rect);
      dumpcontext << "\nrect = " << rect;
      dumpcontext << "\nparent sp(::interaction_impl) = " << (void *)((::user::interaction_impl *) this)->GetParent();

//      dumpcontext << "\nstyle = " << (void *)(dword_ptr)::GetWindowLong(get_handle(), GWL_STYLE);
  //    if (::GetWindowLong(get_handle(), GWL_STYLE) & WS_CHILD)
    //     dumpcontext << "\nid = " << __get_dialog_control_id(get_handle());

      dumpcontext << "\n";
   }

   bool interaction_impl::DestroyWindow()
   {

      if((get_handle() == NULL))
         return true;

      if(m_oswindow->m_bMessageOnlyWindow)
      {

        ::user::interaction * pui = m_pui;

         send_message(WM_DESTROY, 0, 0);

         send_message(WM_NCDESTROY, 0, 0);

         ::oswindow_remove_message_only_window(this);

         return true;

      }

      bool bResult = false;

      oswindow window = get_handle();

      if (window != NULL)
      {

         bResult = ::DestroyWindow(window) != FALSE;

         //Detach();

      }

      return bResult;

   }

   /////////////////////////////////////////////////////////////////////////////
   // Default interaction_impl implementation


   LRESULT interaction_impl::DefWindowProc(UINT nMsg, WPARAM wparam, LPARAM lparam)
   {
    /*  if (m_pfnSuper != NULL)
         return ::callWindowProc(m_pfnSuper, get_handle(), nMsg, wparam, lparam);

      WNDPROC pfnWndProc;
      if ((pfnWndProc = *GetSuperWndProcaddr()) == NULL)
         return ::DefWindowProc(get_handle(), nMsg, wparam, lparam);
      else
         return ::callWindowProc(pfnWndProc, get_handle(), nMsg, wparam, lparam);*/

         return 0;
   }

/*
   WNDPROC* interaction_impl::GetSuperWndProcaddr()
   {
      // Note: it is no longer necessary to override GetSuperWndProcaddr
      //  for each control class with a different WNDCLASS.
      //  This implementation now uses instance data, such that the previous
      //  WNDPROC can be anything.

      return &m_pfnSuper;
   }
*/
   void interaction_impl::pre_translate_message(::signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      // no default processing
   }


   void interaction_impl::GetWindowText(string & rString)
   {
      /*ASSERT(::IsWindow((oswindow) get_handle()));

      int32_t nLen = ::GetWindowTextLength(get_handle());
      ::GetWindowText(get_handle(), rString.GetBufferSetLength(nLen), nLen+1);
      rString.ReleaseBuffer();*/
      rString = m_strWindowText;

   }

/*
   int32_t interaction_impl::GetDlgItemText(int32_t nID, string & rString) const
   {
      ASSERT(::IsWindow((oswindow) get_handle()));
      rString = "";    // is_empty without deallocating

      oswindow hWnd = ::GetDlgItem(get_handle(), nID);
      if (hWnd != NULL)
      {
         int32_t nLen = ::GetWindowTextLength(hWnd);
         ::GetWindowText(hWnd, rString.GetBufferSetLength(nLen), nLen+1);
         rString.ReleaseBuffer();
      }

      return (int32_t)rString.get_length();
   }
*/

   bool interaction_impl::GetWindowPlacement(WINDOWPLACEMENT* lpwndpl)
   {
  /*    ASSERT(::IsWindow((oswindow) get_handle()));
      lpwndpl->length = sizeof(WINDOWPLACEMENT);
      return ::GetWindowPlacement(get_handle(), lpwndpl) != FALSE;*/
      return false;
   }

   bool interaction_impl::SetWindowPlacement(const WINDOWPLACEMENT* lpwndpl)
   {
/*      ASSERT(::IsWindow((oswindow) get_handle()));
      ((WINDOWPLACEMENT*)lpwndpl)->length = sizeof(WINDOWPLACEMENT);
      return ::SetWindowPlacement(get_handle(), lpwndpl) != FALSE;*/
      return false;
   }

   /////////////////////////////////////////////////////////////////////////////
   // interaction_impl will delegate owner draw messages to self drawing controls

   // Drawing: for all 4 control types
// /*   void interaction_impl::OnDrawItem(int32_t /*nIDCtl*/, LPDRAWITEMSTRUCT lpDrawItemStruct)
  // {

      // reflect notification to child interaction_impl control
    //  if (ReflectLastMsg(lpDrawItemStruct->hwndItem))
      //   return;     // eat it

      // not handled - do default
//      Default();
  // }

   // Drawing: for all 4 control types
//   int32_t interaction_impl::OnCompareItem(int32_t /*nIDCtl*/, LPCOMPAREITEMSTRUCT lpCompareItemStruct)
  // {
    //  // reflect notification to child interaction_impl control
      //LRESULT lResult;
//      if (ReflectLastMsg(lpCompareItemStruct->hwndItem, &lResult))
  //       return (int32_t)lResult;        // eat it

      // not handled - do default
    //  return (int32_t)Default();
//   }

  // void interaction_impl::OnDeleteItem(int32_t /*nIDCtl*/, LPDELETEITEMSTRUCT lpDeleteItemStruct)
   //{
      // reflect notification to child interaction_impl control
     // if (ReflectLastMsg(lpDeleteItemStruct->hwndItem))
       //  return;     // eat it
      // not handled - do default
//      Default();
  // }

   bool interaction_impl::_EnableToolTips(bool bEnable, UINT nFlag)
   {
      UNREFERENCED_PARAMETER(bEnable);
      UNREFERENCED_PARAMETER(nFlag);
      return FALSE;
   }


   // Measure item implementation relies on unique control/menu IDs
//   void interaction_impl::OnMeasureItem(int32_t /*nIDCtl*/, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
  /* {
      if (lpMeasureItemStruct->CtlType == ODT_MENU)
      {
         ASSERT(lpMeasureItemStruct->CtlID == 0);
      }
      else
      {
         sp(::user::interaction) pChild = GetDescendantWindow(lpMeasureItemStruct->CtlID);
         if (pChild != NULL && pChild->SendChildNotifyLastMsg())
            return;     // eaten by child
      }
      // not handled - do default
      Default();
   }*/

/*   bool interaction_impl::GetWindowInfo(PWINDOWINFO pwi) const
   {
      ASSERT(::IsWindow((oswindow)get_handle()));
      return ::GetWindowInfo((oswindow)get_handle(), pwi) != FALSE;
   }*/

/*   sp(::interaction_impl) interaction_impl::GetAncestor(UINT gaFlags) const
   { ASSERT(::IsWindow((oswindow)get_handle())); return  ::linux::interaction_impl::from_handle(::GetAncestor((oswindow)get_handle(), gaFlags)); }

*/

/*   bool interaction_impl::GetScrollBarInfo(LONG idObject, PSCROLLBARINFO psbi) const
   {
      ASSERT(::IsWindow((oswindow)get_handle()));
      ASSERT(psbi != NULL);
      return ::GetScrollBarInfo((oswindow)get_handle(), idObject, psbi) != FALSE;
   }
*/
/*   bool interaction_impl::GetTitleBarInfo(PTITLEBARINFO pti) const
   {
      ASSERT(::IsWindow((oswindow)get_handle()));
      ASSERT(pti != NULL);
      return ::GetTitleBarInfo((oswindow)get_handle(), pti) != FALSE;
   }
*/
/*   bool interaction_impl::AnimateWindow(DWORD dwTime, DWORD dwFlags)
   {
      ASSERT(::IsWindow((oswindow)get_handle()));
      return ::AnimateWindow((oswindow)get_handle(), dwTime, dwFlags) != FALSE;
   }

   bool interaction_impl::FlashWindowEx(DWORD dwFlags, UINT  uCount, DWORD dwTimeout)
   {
      ASSERT(::IsWindow((oswindow)get_handle()));
      FLASHWINFO fwi;
      fwi.cbSize = sizeof(fwi);
      fwi.hwnd = (oswindow)get_handle();
      fwi.dwFlags = dwFlags;
      fwi.uCount = uCount;
      fwi.dwTimeout = dwTimeout;

      return ::FlashWindowEx(&fwi) != FALSE;
   }
*/

/*
   bool interaction_impl::SetLayeredWindowAttributes(COLORREF crKey, BYTE bAlpha, DWORD dwFlags)
   {
      ASSERT(::IsWindow((oswindow)get_handle()));
      return ::SetLayeredWindowAttributes((oswindow)get_handle(), crKey, bAlpha, dwFlags) != FALSE;
   }

   bool interaction_impl::UpdateLayeredWindow(::draw2d::graphics * pDCDst, POINT *pptDst, SIZE *psize,
      ::draw2d::graphics * pDCSrc, POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags)
   {
      ASSERT(::IsWindow((oswindow)get_handle()));
      return ::UpdateLayeredWindow((oswindow)get_handle(), WIN_HDC(pDCDst), pptDst, psize,
         WIN_HDC(pDCSrc), pptSrc, crKey, pblend, dwFlags) != FALSE;
   }

*/
/*
   bool interaction_impl::GetLayeredWindowAttributes(COLORREF *pcrKey, BYTE *pbAlpha, DWORD *pdwFlags) const
   {
      ASSERT(::IsWindow((oswindow)get_handle()));
      return ::GetLayeredWindowAttributes((oswindow)get_handle(), pcrKey, pbAlpha, pdwFlags) != FALSE;
   }

   bool interaction_impl::PrintWindow(::draw2d::graphics * pgraphics, UINT nFlags) const
   {
      ASSERT(::IsWindow((oswindow)get_handle()));
      return ::PrintWindow((oswindow)get_handle(), (HDC)(dynamic_cast<::linux::graphics * >(pgraphics))->get_handle(), nFlags) != FALSE;
   }

*/
   LRESULT interaction_impl::OnNTCtlColor(WPARAM wparam, LPARAM lparam)
   {
      return 0;
   }


   void interaction_impl::PrepareForHelp()
   {
   }




   bool interaction_impl::_001OnCmdMsg(::aura::cmd_msg * pcmdmsg)
   {
      if(command_target_interface::_001OnCmdMsg(pcmdmsg))
         return TRUE;

      //      bool b;

      //if(_iguimessageDispatchCommandMessage(pcommand, b))
      // return b;

      command_target * pcmdtarget = dynamic_cast < command_target * > (this);
      return pcmdtarget->command_target::_001OnCmdMsg(pcmdmsg);
   }


   bool interaction_impl::BaseOnControlEvent(::user::control_event * pevent)
   {
      UNREFERENCED_PARAMETER(pevent);
      return false;
   }

   void interaction_impl::_002OnDraw(::draw2d::dib * pdib)
   {

//      ::callWindowProc(*GetSuperWndProcaddr(), get_handle(), WM_PRINT, (WPARAM)((dynamic_cast<::linux::graphics * >(pgraphics))->get_handle()), (LPARAM)(PRF_CHILDREN | PRF_CLIENT));

   }

   /////////////////////////////////////////////////////////////////////////////
   // main message_handler implementation
   DWORD dwDebugmessage_handlerTime;
int iDebugmessage_handlerTime;

DWORD dwLastMouseMove;
DWORD dwLastPaint;
   void interaction_impl::message_handler(::signal_details * pobj)
   {

      SCAST_PTR(::message::base, pbase, pobj);
      if(pbase->m_uiMessage == WM_TIMER)
      {
         //m_pthread->step_timer();
      }
      else if(pbase->m_uiMessage == WM_LBUTTONDOWN)
      {

         TRACE("WM_LBUTTONDOWN (0)");

         //g_pwndLastLButtonDown = this;
      }
      else if(pbase->m_uiMessage == WM_MOUSEMOVE)
      {

         TRACE("WM_MOUSEMOVE (0)");

//         DWORD dwNow = get_tick_count();
//
//
//         if(dwNow - dwLastMouseMove < 25)
//         {
//
//            pobj->m_bRet = true;
//
//            return;
//
//         }
//         else
//         {
//
//            dwLastMouseMove = dwNow;
//
//         }



         //g_pwndLastLButtonDown = this;
      }
      else if(pbase->m_uiMessage == WM_PAINT)
      {

         TRACE("WM_MOUSEMOVE (0)");
//
//         DWORD dwNow = get_tick_count();
//
//
//         if(dwNow - dwLastPaint < 25)
//         {
//
//            pobj->m_bRet = true;
//
//            return;
//
//         }
//         else
//         {
//
//            dwLastPaint = dwNow;
//
//         }



         //g_pwndLastLButtonDown = this;
      }
      else if(pbase->m_uiMessage == WM_LBUTTONUP)
      {

         TRACE("WM_LBUTTONUP (0)");

         //g_pwndLastLButtonDown = this;
      }
      /*      else if(pbase->m_uiMessage == ca2M_BERGEDGE)
      {
      if(pbase->m_wparam == BERGEDGE_GETAPP)
      {
      sp(::ca2::application)* ppapp= (sp(::ca2::application)*) pbase->m_lparam;
      *ppapp = get_app();
      pbase->m_bRet = true;
      return;
      }
      }*/

   DWORD dwNow = get_tick_count();

   if(dwNow - dwDebugmessage_handlerTime < 1)
   {

      if(iDebugmessage_handlerTime > 20)
      {

         writeln("interaction_impl::message_handler flooded?");

      }
      else
      {

         iDebugmessage_handlerTime++;

      }

   }
   else
   {

      iDebugmessage_handlerTime = 0;

   }


   dwDebugmessage_handlerTime = dwNow;


      if(m_pui != NULL)
      {

         m_pui->pre_translate_message(pobj);

         if(pobj->m_bRet)
            return;

      }




      if(pbase->m_uiMessage == WM_KEYDOWN ||
         pbase->m_uiMessage == WM_KEYUP ||
         pbase->m_uiMessage == WM_CHAR)
      {

         ::message::key * pkey = (::message::key *) pbase;

         Session.keyboard().translate_os_key_message(pkey);

         if(pbase->m_uiMessage == WM_KEYDOWN)
         {
            try
            {
               Session.set_key_pressed(pkey->m_ekey, true);
            }
            catch(...)
            {
            }
         }
         else if(pbase->m_uiMessage == WM_KEYUP)
         {
            try
            {
               Session.set_key_pressed(pkey->m_ekey, false);
            }
            catch(...)
            {
            }
         }


      }
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
         pbase->m_uiMessage == WM_MOUSEMOVE ||
       pbase->m_uiMessage == WM_MOUSEMOVE)
//         pbase->m_uiMessage == WM_MOUSEWHEEL)
      {

         if(m_pui->m_bDestroying)
            return;

         if(pbase->m_uiMessage == WM_LBUTTONDOWN)
         {

               TRACE("WM_LBUTTONDOWN");

         }

         ::message::mouse * pmouse = (::message::mouse *) pbase;

         Session.on_ui_mouse_message(pmouse);

         if(m_pauraapp->m_paxissession != NULL)
         {
            Session.m_ptCursor = pmouse->m_pt;
         }

         if(m_pui != NULL && m_pui->m_pauraapp->m_paxissession != NULL && m_pui->m_pauraapp->m_paxissession != m_pauraapp->m_paxissession)
         {

            Sess(m_pui->m_pauraapp->m_paxissession).m_ptCursor = pmouse->m_pt;

         }

         if(m_bTranslateMouseMessageCursor && !pmouse->m_bTranslated)
         {
            pmouse->m_bTranslated = true;
            rect64 rectWindow;
            if(m_bScreenRelativeMouseMessagePosition)
            {
               class rect rectWindow32;
               ::GetWindowRect((oswindow) get_handle(), &rectWindow32);
               ::copy(rectWindow, rectWindow32);
            }
            else
            {
               m_pui->GetWindowRect(rectWindow);
            }
            if(System.get_monitor_count() > 0)
            {
               rect rcMonitor;
               System.get_monitor_rect(0, &rcMonitor);
               if(rectWindow.left >= rcMonitor.left)
                  pmouse->m_pt.x += (LONG) rectWindow.left;
               if(rectWindow.top >= rcMonitor.top)
                  pmouse->m_pt.y += (LONG) rectWindow.top;
            }
            else
            {
               if(rectWindow.left >= 0)
                  pmouse->m_pt.x += (LONG) rectWindow.left;
               if(rectWindow.top >= 0)
                  pmouse->m_pt.y += (LONG) rectWindow.top;
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

         //_000OnMouseHoverCheck(pmouse);


         //user::oswindow_array hwnda;
         //user::interaction_ptra wnda;
         //wnda = System.get_hwnda();
         /*user::oswindow_array oswindowa;
         user::interaction_spa wnda(get_app());
         wnda = System.m_uiptraFrame;
         oswindowa = wnda.get_hwnda();
         user::window_util::SortByZOrder(oswindowa);

         if(pbase->m_uiMessage == WM_LBUTTONDOWN)
         {

               TRACE("WM_LBUTTONDOWN 2");

         }

         //user::window_util::SortByZOrder(hwnda);
         for(int32_t i = oswindowa.get_upper_bound(); i >= 0 ; i--)
         {
            sp(::user::interaction) pguie = wnda.find_first(oswindowa[i]);
            if(pguie != NULL)
            {
               pguie->_000OnMouse(pmouse);
               if(pmouse->m_bRet)
                  return;
            }
         }*/

         _008OnMouse(pmouse);

//         if(pmouse->m_uiMessage == WM_LBUTTONDOWN)
//         {
////         TRACE("button down\n");
//         }
//               m_pui->_000OnMouse(pmouse);
//               if(pmouse->m_bRet)
//                  return;
//
//         sp(::user::interaction) pui;
//
//         while(System.get_frame(pui))
//         {
//            if(pui != m_pui)
//            {
//               pui->_000OnMouse(pmouse);
//               if(pmouse->m_bRet)
//                  return;
//
//            }
//         }
//
//         pbase->set_lresult(DefWindowProc(pbase->m_uiMessage, pbase->m_wparam, pbase->m_lparam));
         return;
      }
      else if(pbase->m_uiMessage == WM_KEYDOWN ||
         pbase->m_uiMessage == WM_KEYUP ||
         pbase->m_uiMessage == WM_CHAR)
      {

         ::message::key * pkey = (::message::key *) pbase;
         sp(::user::interaction) puiFocus =  (Session.get_keyboard_focus());
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
            /*else
            {
               _000OnKey(pkey);
               if(pbase->m_bRet)
                  return;
            }*/
         }
         pbase->set_lresult(DefWindowProc(pbase->m_uiMessage, pbase->m_wparam, pbase->m_lparam));
         return;
      }
      if(pbase->m_uiMessage == ::message::message_event)
      {
         if(m_pui != NULL)
         {
            m_pui->BaseOnControlEvent((::user::control_event *) pbase->m_lparam.m_lparam);
         }
         else
         {
            BaseOnControlEvent((::user::control_event *) pbase->m_lparam.m_lparam);
         }
         return;
      }
      (this->*m_pfnDispatchWindowProc)(pobj);
      if(pobj->m_bRet)
         return;
      /*
      if(m_pui != NULL && m_pui != this)
      {
      m_pui->_user_message_handler(pobj);
      if(pobj->m_bRet)
      return;
      }
      */
      pbase->set_lresult(DefWindowProc(pbase->m_uiMessage, pbase->m_wparam, pbase->m_lparam));
   }

   /*
   bool interaction_impl::OnWndMsg(UINT message, WPARAM wparam, LPARAM lparam, LRESULT* pResult)
   {
   LRESULT lResult = 0;
   union MessageMapFunctions mmf;
   mmf.pfn = 0;
   CInternalGlobalLock winMsgLock;
   // special case for commands
   if (message == WM_COMMAND)
   {
   if (OnCommand(wparam, lparam))
   {
   lResult = 1;
   goto LReturnTrue;
   }
   return FALSE;
   }

   // special case for notifies
   if (message == WM_NOTIFY)
   {
   NMHDR* pNMHDR = (NMHDR*)lparam;
   if (pNMHDR->hwndFrom != NULL && OnNotify(wparam, lparam, &lResult))
   goto LReturnTrue;
   return FALSE;
   }

   // special case for activation
   if (message == WM_ACTIVATE)
   __handle_activate(this, wparam, ::linux::interaction_impl::from_handle((oswindow)lparam));

   // special case for set cursor HTERROR
   if (message == WM_SETCURSOR &&
   __handle_set_cursor(this, (short)LOWORD(lparam), HIWORD(lparam)))
   {
   lResult = 1;
   goto LReturnTrue;
   }

   // special case for windows that contain windowless ActiveX controls
   bool bHandled;

   bHandled = FALSE;

   const __MSGMAP* pMessageMap; pMessageMap = GetMessageMap();
   UINT iHash; iHash = (LOWORD((dword_ptr)pMessageMap) ^ message) & (iHashMax-1);
   winMsgLock.lock(CRIT_WINMSGcaCHE);
   __MSG_caCHE* pMsgcache; pMsgcache = &gen_Msgcache[iHash];
   const __MSGMAP_ENTRY* lpEntry;
   if (message == pMsgcache->nMsg && pMessageMap == pMsgcache->pMessageMap)
   {
   // cache hit
   lpEntry = pMsgcache->lpEntry;
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
   pMsgcache->nMsg = message;
   pMsgcache->pMessageMap = pMessageMap;

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
   pMsgcache->lpEntry = lpEntry;
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
   pMsgcache->lpEntry = lpEntry;
   winMsgLock.unlock();
   goto LDispatchRegistered;
   }
   lpEntry++;      // keep looking past this one
   }
   }
   }

   pMsgcache->lpEntry = NULL;
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
   point point(lparam);
   lResult = (this->*mmf.pfn_l_p)(point);
   break;
   }
   case ::ca2::Sig_b_D_v:
   lResult = (this->*mmf.pfn_b_D)(::linux::graphics::from_handle(reinterpret_cast<HDC>(wparam)));
   break;

   case ::ca2::Sig_b_b_v:
   lResult = (this->*mmf.pfn_b_b)(static_cast<bool>(wparam));
   break;

   case ::ca2::Sig_b_u_v:
   lResult = (this->*mmf.pfn_b_u)(static_cast<UINT>(wparam));
   break;

   case ::ca2::Sig_b_h_v:
   lResult = (this->*mmf.pfn_b_h)(reinterpret_cast<HANDLE>(wparam));
   break;

   case ::ca2::Sig_i_u_v:
   lResult = (this->*mmf.pfn_i_u)(static_cast<UINT>(wparam));
   break;

   case ::ca2::Sig_C_v_v:
   lResult = reinterpret_cast<LRESULT>((this->*mmf.pfn_C_v)());
   break;

   case ::ca2::Sig_v_u_W:
   (this->*mmf.pfn_v_u_W)(static_cast<UINT>(wparam),
   ::linux::interaction_impl::from_handle(reinterpret_cast<oswindow>(lparam)));
   break;

   case ::ca2::Sig_u_u_v:
   lResult = (this->*mmf.pfn_u_u)(static_cast<UINT>(wparam));
   break;

   case ::ca2::Sig_b_v_v:
   lResult = (this->*mmf.pfn_b_v)();
   break;

   case ::ca2::Sig_b_W_uu:
   lResult = (this->*mmf.pfn_b_W_u_u)(::linux::interaction_impl::from_handle(reinterpret_cast<oswindow>(wparam)),
   LOWORD(lparam), HIWORD(lparam));
   break;

   case ::ca2::Sig_b_W_COPYDATASTRUCT:
   lResult = (this->*mmf.pfn_b_W_COPYDATASTRUCT)(
   ::linux::interaction_impl::from_handle(reinterpret_cast<oswindow>(wparam)),
   reinterpret_cast<COPYDATASTRUCT*>(lparam));
   break;

   case ::ca2::Sig_b_v_HELPINFO:
   lResult = (this->*mmf.pfn_b_HELPINFO)(reinterpret_cast<LPHELPINFO>(lparam));
   break;

   case ::ca2::Sig_CTLCOLOR:
   {
   // special case for OnCtlColor to avoid too many temporary objects
   ASSERT(message == WM_CTLCOLOR);
   __CTLCOLOR* pCtl = reinterpret_cast<__CTLCOLOR*>(lparam);
   ::draw2d::graphics_sp dcTemp;
   dcTemp.set_handle1(pCtl->hDC);
   interaction_impl wndTemp;
   wndTemp.set_handle(pCtl->hWnd);
   UINT nCtlType = pCtl->nCtlType;
   // if not coming from a permanent interaction_impl, use stack temporary
   sp(::interaction_impl) pWnd = ::linux::interaction_impl::FromHandlePermanent(wndTemp.get_handle());
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
   __CTLCOLOR* pCtl = reinterpret_cast<__CTLCOLOR*>(lparam);
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
   lResult = (this->*mmf.pfn_i_u_W_u)(LOWORD(wparam),
   ::linux::interaction_impl::from_handle(reinterpret_cast<oswindow>(lparam)), HIWORD(wparam));
   break;

   case ::ca2::Sig_i_uu_v:
   lResult = (this->*mmf.pfn_i_u_u)(LOWORD(wparam), HIWORD(wparam));
   break;

   case ::ca2::Sig_i_W_uu:
   lResult = (this->*mmf.pfn_i_W_u_u)(::linux::interaction_impl::from_handle(reinterpret_cast<oswindow>(wparam)),
   LOWORD(lparam), HIWORD(lparam));
   break;

   case ::ca2::Sig_i_v_s:
   lResult = (this->*mmf.pfn_i_s)(reinterpret_cast<LPTSTR>(lparam));
   break;

   case ::ca2::Sig_l_w_l:
   lResult = (this->*mmf.pfn_l_w_l)(wparam, lparam);
   break;



   case ::ca2::Sig_v_b_h:
   (this->*mmf.pfn_v_b_h)(static_cast<bool>(wparam),
   reinterpret_cast<HANDLE>(lparam));
   break;

   case ::ca2::Sig_v_h_v:
   (this->*mmf.pfn_v_h)(reinterpret_cast<HANDLE>(wparam));
   break;

   case ::ca2::Sig_v_h_h:
   (this->*mmf.pfn_v_h_h)(reinterpret_cast<HANDLE>(wparam),
   reinterpret_cast<HANDLE>(lparam));
   break;

   case ::ca2::Sig_v_v_v:
   (this->*mmf.pfn_v_v)();
   break;

   case ::ca2::Sig_v_u_v:
   (this->*mmf.pfn_v_u)(static_cast<UINT>(wparam));
   break;

   case ::ca2::Sig_v_u_u:
   (this->*mmf.pfn_v_u_u)(static_cast<UINT>(wparam), static_cast<UINT>(lparam));
   break;

   case ::ca2::Sig_v_uu_v:
   (this->*mmf.pfn_v_u_u)(LOWORD(wparam), HIWORD(wparam));
   break;

   case ::ca2::Sig_v_v_ii:
   (this->*mmf.pfn_v_i_i)(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
   break;

   case ::ca2::Sig_v_u_uu:
   (this->*mmf.pfn_v_u_u_u)(static_cast<UINT>(wparam), LOWORD(lparam), HIWORD(lparam));
   break;

   case ::ca2::Sig_v_u_ii:
   (this->*mmf.pfn_v_u_i_i)(static_cast<UINT>(wparam), LOWORD(lparam), HIWORD(lparam));
   break;

   case ::ca2::Sig_v_w_l:
   (this->*mmf.pfn_v_w_l)(wparam, lparam);
   break;

   case ::ca2::Sig_MDIACTIVATE:
   (this->*mmf.pfn_v_b_W_W)(get_handle() == reinterpret_cast<oswindow>(lparam),
   ::linux::interaction_impl::from_handle(reinterpret_cast<oswindow>(lparam)),
   ::linux::interaction_impl::from_handle(reinterpret_cast<oswindow>(wparam)));
   break;

   case ::ca2::Sig_v_D_v:
   (this->*mmf.pfn_v_D)(::linux::graphics::from_handle(reinterpret_cast<HDC>(wparam)));
   break;


   case ::ca2::Sig_v_W_v:
   (this->*mmf.pfn_v_W)(::linux::interaction_impl::from_handle(reinterpret_cast<oswindow>(wparam)));
   break;

   case ::ca2::Sig_v_v_W:
   (this->*mmf.pfn_v_W)(::linux::interaction_impl::from_handle(reinterpret_cast<oswindow>(lparam)));
   break;

   case ::ca2::Sig_v_W_uu:
   (this->*mmf.pfn_v_W_u_u)(::linux::interaction_impl::from_handle(reinterpret_cast<oswindow>(wparam)), LOWORD(lparam),
   HIWORD(lparam));
   break;

   case ::ca2::Sig_v_W_p:
   {
   point point(lparam);
   (this->*mmf.pfn_v_W_p)(::linux::interaction_impl::from_handle(reinterpret_cast<oswindow>(wparam)), point);
   }
   break;

   case ::ca2::Sig_v_W_h:
   (this->*mmf.pfn_v_W_h)(::linux::interaction_impl::from_handle(reinterpret_cast<oswindow>(wparam)),
   reinterpret_cast<HANDLE>(lparam));
   break;

   case ::ca2::Sig_ACTIVATE:
   (this->*mmf.pfn_v_u_W_b)(LOWORD(wparam),
   ::linux::interaction_impl::from_handle(reinterpret_cast<oswindow>(lparam)), HIWORD(wparam));
   break;

   case ::ca2::Sig_SCROLL:
   case ::ca2::Sig_SCROLL_REFLECT:
   {
   // special case for WM_VSCROLL and WM_HSCROLL
   ASSERT(message == WM_VSCROLL || message == WM_HSCROLL ||
   message == WM_VSCROLL+WM_REFLECT_BASE || message == WM_HSCROLL+WM_REFLECT_BASE);
   int32_t nScrollCode = (short)LOWORD(wparam);
   int32_t nPos = (short)HIWORD(wparam);
   if (lpEntry->nSig == ::ca2::Sig_SCROLL)
   (this->*mmf.pfn_v_u_u_W)(nScrollCode, nPos,
   ::linux::interaction_impl::from_handle(reinterpret_cast<oswindow>(lparam)));
   else
   (this->*mmf.pfn_v_u_u)(nScrollCode, nPos);
   }
   break;

   case ::ca2::Sig_v_v_s:
   (this->*mmf.pfn_v_s)(reinterpret_cast<LPTSTR>(lparam));
   break;

   case ::ca2::Sig_v_u_cs:
   (this->*mmf.pfn_v_u_cs)(static_cast<UINT>(wparam), reinterpret_cast<const char *>(lparam));
   break;

   case ::ca2::Sig_OWNERDRAW:
   (this->*mmf.pfn_v_i_s)(static_cast<int32_t>(wparam), reinterpret_cast<LPTSTR>(lparam));
   lResult = TRUE;
   break;

   case ::ca2::Sig_i_i_s:
   lResult = (this->*mmf.pfn_i_i_s)(static_cast<int32_t>(wparam), reinterpret_cast<LPTSTR>(lparam));
   break;

   case ::ca2::Sig_u_v_p:
   {
   point point(lparam);
   lResult = (this->*mmf.pfn_u_p)(point);
   }
   break;

   case ::ca2::Sig_u_v_v:
   lResult = (this->*mmf.pfn_u_v)();
   break;

   case ::ca2::Sig_v_b_NCCALCSIZEPARAMS:
   (this->*mmf.pfn_v_b_NCCALCSIZEPARAMS)(static_cast<bool>(wparam),
   reinterpret_cast<NCCALCSIZE_PARAMS*>(lparam));
   break;

   case ::ca2::Sig_v_v_WINDOWPOS:
   (this->*mmf.pfn_v_v_WINDOWPOS)(reinterpret_cast<WINDOWPOS*>(lparam));
   break;

   case ::ca2::Sig_v_uu_M:
   (this->*mmf.pfn_v_u_u_M)(LOWORD(wparam), HIWORD(wparam), reinterpret_cast<HMENU>(lparam));
   break;

   case ::ca2::Sig_v_u_p:
   {
   point point(lparam);
   (this->*mmf.pfn_v_u_p)(static_cast<UINT>(wparam), point);
   }
   break;

   case ::ca2::Sig_SIZING:
   (this->*mmf.pfn_v_u_pr)(static_cast<UINT>(wparam), reinterpret_cast<LPRECT>(lparam));
   lResult = TRUE;
   break;

   case ::ca2::Sig_MOUSEWHEEL:
   lResult = (this->*mmf.pfn_b_u_s_p)(LOWORD(wparam), (short)HIWORD(wparam),
   point(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam)));
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
   lResult = (this->*mmf.pfn_l_w_l)(wparam, lparam);

   LReturnTrue:
   if (pResult != NULL)
   *pResult = lResult;
   return TRUE;
   }
   */


   /////////////////////////////////////////////////////////////////////////////
   // interaction_impl command handling

   bool interaction_impl::OnCommand(WPARAM wparam, LPARAM lparam)
      // return TRUE if command invocation was attempted
   {
      UNREFERENCED_PARAMETER(wparam);
      UNREFERENCED_PARAMETER(lparam);
      /*   UINT nID = LOWORD(wparam);
      oswindow hWndCtrl = (oswindow)lparam;
      int32_t nCode = HIWORD(wparam);

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
      TRACE(::ca2::trace::category_AppMsg, 0, "Warning: not executing disabled command %d\n", nID);
      return TRUE;
      }

      // menu or accelerator
      nCode = CN_COMMAND;
      }
      else
      {
      // control notification
      ASSERT(nID == 0 || ::IsWindow(hWndCtrl));

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
      if (nCode < 0 && nCode != (int32_t)0x8000)
      TRACE(::ca2::trace::category_AppMsg, 0, "Implementation Warning: control notification = $%X.\n",
      nCode);
      #endif

      return _001OnCommand(nID, nCode, NULL, NULL);*/
      return FALSE;
   }

   bool interaction_impl::OnNotify(WPARAM, LPARAM lparam, LRESULT* pResult)
   {
  /*    ASSERT(pResult != NULL);
      NMHDR* pNMHDR = (NMHDR*)lparam;
      oswindow hWndCtrl = pNMHDR->hwndFrom;

      // get the child ID from the interaction_impl itself
      //      uint_ptr nID = __get_dialog_control_id(hWndCtrl);
      //      int32_t nCode = pNMHDR->code;

      ASSERT(hWndCtrl != NULL);
      ASSERT(::IsWindow(hWndCtrl));

      if (gen_ThreadState->m_hLockoutNotifyWindow == get_handle())
         return true;        // locked out - ignore control notification

      // reflect notification to child interaction_impl control
      if (ReflectLastMsg(hWndCtrl, pResult))
         return true;        // eaten by child

      //      __NOTIFY notify;
      //    notify.pResult = pResult;
      //  notify.pNMHDR = pNMHDR;
      //xxx   return _001OnCommand((UINT)nID, MAKELONG(nCode, WM_NOTIFY), &notify, NULL);*/
      return false;
   }

   /////////////////////////////////////////////////////////////////////////////
   // interaction_impl extensions

/*
   sp(::user::frame_window) interaction_impl::GetParentFrame()
   {

      if (get_handle() == NULL) // no oswindow attached
      {

         return NULL;

      }

      ASSERT_VALID(this);

      sp(::user::interaction) pParentWnd = GetParent();  // start with one parent up

      while (pParentWnd != NULL)
      {
         if (pParentWnd->is_frame_window())
         {
            return  (pParentWnd);
         }
         pParentWnd = pParentWnd->GetParent();
      }

      return NULL;

   }
*/
   /* trans oswindow CLASS_DECL_BASE __get_parent_owner(sp(::user::interaction) hWnd)
   {
   // check for permanent-owned interaction_impl first
   sp(::interaction_impl) pWnd = ::linux::interaction_impl::FromHandlePermanent(hWnd);
   if (pWnd != NULL)
   return LNX_WINDOW(pWnd)->GetOwner();

   // otherwise, return parent in the oswindows sense
   return (::GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD) ?
   ::GetParent(hWnd) : ::GetWindow(hWnd, GW_OWNER);
   }*/
/*

*/

   ::user::interaction * interaction_impl::GetParent()
   {

      if(!::IsWindow((oswindow) get_handle()))
         return NULL;

      if(get_handle() == NULL)
         return NULL;

      //return ::linux::interaction_impl::from_handle(::GetParent(get_handle()));

      return NULL;

   }



   ::user::interaction * interaction_impl::GetTopLevel()
   {

      if(!::IsWindow((oswindow) get_handle()))
         return NULL;

      if(get_handle() == NULL)
         return NULL;

      return m_pui;

   }


/*
   ::user::interaction * interaction_impl::GetTopLevelOwner()
   {
      if (get_handle() == NULL) // no oswindow attached
         return NULL;

      ASSERT_VALID(this);

//      oswindow hWndOwner = get_handle();
  //    oswindow hWndT;
    //  while ((hWndT = ::GetWindow(hWndOwner, GW_OWNER)) != NULL)
      //   hWndOwner = hWndT;

//      return ::linux::interaction_impl::from_handle(hWndOwner);
        return NULL;
   }

   ::user::interaction * interaction_impl::GetParentOwner()
   {
      if (get_handle() == NULL) // no oswindow attached
         return NULL;

      ASSERT_VALID(this);

/*      oswindow hWndParent = get_handle();
      oswindow hWndT;
      while ((::GetWindowLong(hWndParent, GWL_STYLE) & WS_CHILD) &&
         (hWndT = ::GetParent(hWndParent)) != NULL)
      {
         hWndParent = hWndT;
      }

      return ::linux::interaction_impl::from_handle(hWndParent);*/

      //return NULL;
   //}

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
      sp(::user::interaction) pActiveWnd = GetForegroundWindow();
//      if (pActiveWnd == NULL || !(LNX_WINDOW(pActiveWnd)->get_handle() == get_handle() || ::IsChild(LNX_WINDOW(pActiveWnd)->get_handle(), get_handle())))
      {
         // clicking on floating frame when it does not have
         // focus itself -- activate the toplevel frame instead.
         EnsureTopLevel()->SetForegroundWindow();
      }
   }

/*
   sp(::user::frame_window) interaction_impl::GetTopLevelFrame()
   {
      if (get_handle() == NULL) // no oswindow attached
         return NULL;

      ASSERT_VALID(this);

      sp(::user::frame_window) pFrameWnd = NULL;
      if(m_pui != this)
         pFrameWnd =  (m_pui);
      else
         pFrameWnd =  (this);
      if (pFrameWnd == NULL || !pFrameWnd->is_frame_window())
         pFrameWnd = GetParentFrame();

      if (pFrameWnd != NULL)
      {
         sp(::user::frame_window) pTemp;
         while ((pTemp = pFrameWnd->GetParentFrame()) != NULL)
            pFrameWnd = pTemp;
      }
      return pFrameWnd;
   }*/

/*   sp(::interaction_impl) interaction_impl::GetSafeOwner(::interaction_impl * pParent, oswindow* pWndTop)
   {
      oswindow hWnd = GetSafeOwner_((oswindow) pParent->get_handle(), pWndTop);
      return ::linux::interaction_impl::from_handle(hWnd);
   }
*/
   int32_t interaction_impl::message_box(const char * lpszText, const char * lpszcaption, UINT nType)
   {

      string strCaption;

      if (lpszcaption == NULL)
         strCaption = Application.m_strAppName;
      else
         strCaption = lpszcaption;

      int32_t nResult = ::MessageBox((oswindow)get_handle(), lpszText, strCaption, nType);

      return nResult;

   }

   ::user::interaction * interaction_impl::GetDescendantWindow(::user::interaction * hWnd, id id)
   {

      single_lock sl(hWnd->m_pauraapp->m_pmutex, TRUE);

      for(int32_t i = 0; i < hWnd->m_uiptraChild.get_count(); i++)
      {
         if(hWnd->m_uiptraChild[i]->GetDlgCtrlId() == id)
         {
            if(hWnd->m_uiptraChild[i]->GetDescendantWindow(id))
               return hWnd->m_uiptraChild[i]->GetDescendantWindow(id);
            else
               return hWnd->m_uiptraChild[i];
         }
      }

      return NULL;

   }

   void PASCAL interaction_impl::SendMessageToDescendants(void * hWnd, UINT message, WPARAM wparam, lparam lparam, bool bDeep, bool bOnlyPerm)
   {
      // walk through oswindows to avoid creating temporary interaction_impl objects
      // unless we need to call this function recursively
      /*for (oswindow hWndChild = ::GetTopWindow(hWnd); hWndChild != NULL;
         hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))*/
      {
         // if bOnlyPerm is TRUE, don't send to non-permanent windows
         /*if (bOnlyPerm)
         {
            sp(::interaction_impl) pWnd = ::linux::interaction_impl::FromHandlePermanent(hWndChild);
            if (pWnd != NULL)
            {
               // call interaction_impl proc directly since it is a C++ interaction_impl
               __call_window_procedure( (pWnd), LNX_WINDOW(pWnd)->get_handle(), message, wparam, lparam);
            }
         }
         else
         {
            try
            {
               // send message with oswindows SendMessage API
               ::SendMessage(hWndChild, message, wparam, lparam);
            }
            catch(...)
            {
            }
         }
         if (bDeep && ::GetTopWindow(hWndChild) != NULL)
         {
            // send to child windows after parent
            try
            {
               SendMessageToDescendants(hWndChild, message, wparam, lparam,
                  bDeep, bOnlyPerm);
            }
            catch(...)
            {
            }
         } */
      }

   }

   /////////////////////////////////////////////////////////////////////////////
   // Scroll bar helpers
   //  hook for interaction_impl functions
   //    only works for derived class (eg: ::view) that override 'GetScrollBarCtrl'
   // if the interaction_impl doesn't have a _visible_ windows scrollbar - then
   //   look for a sibling with the appropriate ID


   int32_t interaction_impl::SetScrollPos(int32_t nBar, int32_t nPos, bool bRedraw)
   {
//      return ::SetScrollPos(get_handle(), nBar, nPos, bRedraw);
return 0;
   }

   int32_t interaction_impl::GetScrollPos(int32_t nBar) const
   {
      //return ::GetScrollPos(get_handle(), nBar);
      return 0;
   }

   void interaction_impl::SetScrollRange(int32_t nBar, int32_t nMinPos, int32_t nMaxPos, bool bRedraw)
   {
      //::SetScrollRange(get_handle(), nBar, nMinPos, nMaxPos, bRedraw);
   }

   void interaction_impl::GetScrollRange(int32_t nBar, LPINT lpMinPos, LPINT lpMaxPos) const
   {
      //::GetScrollRange(get_handle(), nBar, lpMinPos, lpMaxPos);
   }

   // Turn on/off non-control scrollbars
   //   for WS_?SCROLL scrollbars - show/hide them
   //   for control scrollbar - enable/disable them
   void interaction_impl::EnableScrollBarCtrl(int32_t nBar, bool bEnable)
   {
      // WS_?SCROLL scrollbar - show or hide
      ShowScrollBar(nBar, bEnable);
   }

/*
   bool interaction_impl::SetScrollInfo(int32_t nBar, LPSCROLLINFO lpScrollInfo, bool bRedraw)
   {
      ASSERT(lpScrollInfo != NULL);

      oswindow hWnd = get_handle();
      lpScrollInfo->cbSize = sizeof(*lpScrollInfo);
      ::SetScrollInfo(hWnd, nBar, lpScrollInfo, bRedraw);
      return true;
   }

   bool interaction_impl::GetScrollInfo(int32_t nBar, LPSCROLLINFO lpScrollInfo, UINT nMask)
   {
      UNREFERENCED_PARAMETER(nMask);
      ASSERT(lpScrollInfo != NULL);

      oswindow hWnd = get_handle();
      return ::GetScrollInfo(hWnd, nBar, lpScrollInfo) != FALSE;
   }
*/
   int32_t interaction_impl::GetScrollLimit(int32_t nBar)
   {
      int32_t nMin, nMax;
      GetScrollRange(nBar, &nMin, &nMax);
/*      SCROLLINFO info;
      if (GetScrollInfo(nBar, &info, SIF_PAGE))
      {
         nMax -= __max(info.nPage-1,0);
      }*/
      return nMax;
   }

   void interaction_impl::ScrollWindow(int32_t xAmount, int32_t yAmount,
      LPCRECT lpRect, LPCRECT lpClipRect)
   {
/*      ASSERT(::IsWindow((oswindow) get_handle()));

      if (IsWindowVisible() || lpRect != NULL || lpClipRect != NULL)
      {
         // When visible, let oswindows do the scrolling
         ::ScrollWindow(get_handle(), xAmount, yAmount, lpRect, lpClipRect);
      }
      else
      {
         // oswindows does not perform any scrolling if the interaction_impl is
         // not visible.  This leaves child windows unscrolled.
         // To account for this oversight, the child windows are moved
         // directly instead.
         oswindow hWndChild = ::GetWindow(get_handle(), GW_CHILD);
         if (hWndChild != NULL)
         {
            for (; hWndChild != NULL;
               hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
            {
               rect rect;
               ::GetWindowRect(hWndChild, &rect);
               ScreenToClient(&rect);
               ::SetWindowPos(hWndChild, NULL,
                  rect.left+xAmount, rect.top+yAmount, 0, 0,
                  SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER);
            }
         }
      }
*/
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
   sp(::user::interaction) hWndLeftOver = NULL;

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
   for (sp(::user::interaction) hWndChild = m_pui->GetTopWindow(); hWndChild != NULL;
   hWndChild = hWndChild->GetNextWindow(GW_HWNDNEXT))
   {
   string strIdc = hWndChild->GetDlgCtrlId();
   sp(::user::interaction) pWnd = hWndChild;
   if (strIdc == pszIdLeftOver)
   hWndLeftOver = hWndChild;
   else if (::str::begins(strIdc, pszPrefix) && pWnd != NULL)
   hWndChild->SendMessage(WM_SIZEPARENT, 0, (LPARAM)&on_layout);
   }
   for (int32_t i = 0; i < m_pui->m_uiptra.get_count();   i++)
   {
   sp(::user::interaction) hWndChild = m_pui->m_uiptra[i];
   string strIdc = hWndChild->GetDlgCtrlId();
   sp(::user::interaction) pWnd = hWndChild;
   if (strIdc == pszIdLeftOver)
   hWndLeftOver = hWndChild;
   else if (::str::begins(strIdc, pszPrefix) && pWnd != NULL)
   hWndChild->SendMessage(WM_SIZEPARENT, 0, (LPARAM)&on_layout);
   }
   }
   else
   {
   for (sp(::user::interaction) hWndChild = GetTopWindow(); hWndChild != NULL;
   hWndChild = hWndChild->GetNextWindow(GW_HWNDNEXT))
   {
   string strIdc = hWndChild->GetDlgCtrlId();
   sp(::user::interaction) pWnd = hWndChild;
   if (strIdc == pszIdLeftOver)
   hWndLeftOver = hWndChild;
   else if (::str::begins(strIdc, pszPrefix) && pWnd != NULL)
   hWndChild->SendMessage(WM_SIZEPARENT, 0, (LPARAM)&on_layout);
   }
   for (int32_t i = 0; i < m_uiptra.get_count();   i++)
   {
   sp(::user::interaction) hWndChild = m_uiptra[i];
   string strIdc = hWndChild->GetDlgCtrlId();
   sp(::user::interaction) pWnd = hWndChild;
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
   sp(::user::interaction) pLeftOver = hWndLeftOver;
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
   TRACE(::ca2::trace::category_AppMsg, 0, "Warning: DeferWindowPos failed - low system resources.\n");
   }

   */

   /*

   void interaction_impl::RepositionBars(UINT nIDFirst, UINT nIDLast, id nIdLeftOver,
      UINT nFlags, LPRECT lpRectParam, LPCRECT lpRectClient, bool bStretch)
   {
      UNREFERENCED_PARAMETER(nIDFirst);
      UNREFERENCED_PARAMETER(nIDLast);

      ASSERT(nFlags == 0 || (nFlags & ~reposNoPosLeftOver) == reposQuery ||
         (nFlags & ~reposNoPosLeftOver) == reposExtra);

      // walk kids in order, control bars get the resize notification
      //   which allow them to shrink the client area
      // remaining size goes to the 'nIDLeftOver' pane
      // NOTE: nIDFirst->nIDLast are usually 0->0xffff

      __SIZEPARENTPARAMS on_layout;
      sp(::user::interaction) hWndLeftOver = NULL;

      on_layout.bStretch = bStretch;
      on_layout.sizeTotal.cx = on_layout.sizeTotal.cy = 0;
      if (lpRectClient != NULL)
         on_layout.rect = *lpRectClient;    // starting rect comes from parameter
      else
      {
            m_pui->GetClientRect(&on_layout.rect);    // starting rect comes from client rect
      }

//      if ((nFlags & ~reposNoPosLeftOver) != reposQuery)
  //       on_layout.hDWP = ::BeginDeferWindowPos(8); // reasonable guess
    //  else
         on_layout.hDWP = NULL; // not actually doing on_layout

      if(m_pui != NULL)
      {
         for (sp(::user::interaction) hWndChild = m_pui->GetTopWindow(); hWndChild != NULL;
            hWndChild = hWndChild->GetNextWindow(GW_HWNDNEXT))
         {
            id id = hWndChild->GetDlgCtrlId();
            sp(::user::interaction) pWnd = hWndChild;
            if (id == (int32_t) nIdLeftOver)
               hWndLeftOver = hWndChild;
            else if (pWnd != NULL)
               hWndChild->send_message(WM_SIZEPARENT, 0, (LPARAM)&on_layout);
         }
         for (sp(::user::interaction) hWndChild = m_pui->get_top_child(); hWndChild != NULL;
            hWndChild = hWndChild->under_sibling())
         {
            id id = hWndChild->GetDlgCtrlId();
            sp(::user::interaction) pWnd = hWndChild;
            if (id == nIdLeftOver)
               hWndLeftOver = hWndChild;
            else if (pWnd != NULL)
               hWndChild->send_message(WM_SIZEPARENT, 0, (LPARAM)&on_layout);
         }
      }
      else
      {
         for (sp(::user::interaction) hWndChild = GetTopWindow(); hWndChild != NULL;
            hWndChild = hWndChild->GetNextWindow(GW_HWNDNEXT))
         {
            id id = hWndChild->GetDlgCtrlId();
            sp(::user::interaction) pWnd = hWndChild;
            if (id == nIdLeftOver)
               hWndLeftOver = hWndChild;
            else if (pWnd != NULL)
               hWndChild->send_message(WM_SIZEPARENT, 0, (LPARAM)&on_layout);
         }
         for (sp(::user::interaction) hWndChild = m_pui->get_top_child(); hWndChild != NULL;
            hWndChild = hWndChild->under_sibling())
         {
            id id = hWndChild->GetDlgCtrlId();
            sp(::user::interaction) pWnd = hWndChild;
            if (id == nIdLeftOver)
               hWndLeftOver = hWndChild;
            else if (pWnd != NULL)
               hWndChild->send_message(WM_SIZEPARENT, 0, (LPARAM)&on_layout);
         }
      }

      // if just getting the available rectangle, return it now...
      if ((nFlags & ~reposNoPosLeftOver) == reposQuery)
      {
         ASSERT(lpRectParam != NULL);
         if (bStretch)
            ::CopyRect(lpRectParam, &on_layout.rect);
         else
         {
            lpRectParam->left = lpRectParam->top = 0;
            lpRectParam->right = on_layout.sizeTotal.cx;
            lpRectParam->bottom = on_layout.sizeTotal.cy;
         }
         return;
      }

      // the rest is the client size of the left-over pane
      if(!nIdLeftOver.is_null() && ((void *) hWndLeftOver) != NULL)
      {
         sp(::user::interaction) pLeftOver = hWndLeftOver;
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
//      if (on_layout.hDWP == NULL || !::EndDeferWindowPos(on_layout.hDWP))
  //       TRACE(::ca2::trace::category_AppMsg, 0, "Warning: DeferWindowPos failed - low system resources.\n");*/
  // }


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

      return false;

   }


   bool PASCAL interaction_impl::ReflectLastMsg(oswindow hWndChild, LRESULT* pResult)
   {
      // get the ::collection::map, and if no ::collection::map, then this message does not need reflection
/*      single_lock sl(afxMutexHwnd(), TRUE);
      hwnd_map * pMap = afxMapHWND();
      if (pMap == NULL)
         return FALSE;

      // check if in permanent ::collection::map, if it is reflect it (could be OLE control)
      sp(::interaction_impl) pWnd =  (pMap->lookup_permanent(hWndChild)); */
      sp(::user::interaction) pWnd =  (FromHandlePermanent(hWndChild));
      ASSERT(pWnd == NULL || LNX_WINDOW(pWnd)->get_handle() == hWndChild);
      if (pWnd == NULL)
      {
         return FALSE;
      }

      // only OLE controls and permanent windows will get reflected msgs
      ASSERT(pWnd != NULL);
      return LNX_WINDOW(pWnd)->SendChildNotifyLastMsg(pResult);
   }

   bool interaction_impl::OnChildNotify(UINT uMsg, WPARAM wparam, LPARAM lparam, LRESULT* pResult)
   {

      return ReflectChildNotify(uMsg, wparam, lparam, pResult);
   }

   bool interaction_impl::ReflectChildNotify(UINT uMsg, WPARAM wparam, LPARAM lparam, LRESULT* pResult)
   {
      UNREFERENCED_PARAMETER(wparam);
      // Note: reflected messages are send directly to interaction_impl::OnWndMsg
      //  and interaction_impl::_001OnCommand for speed and because these messages are not
      //  routed by normal _001OnCommand routing (they are only dispatched)

      switch (uMsg)
      {
         // normal messages (just wparam, lparam through OnWndMsg)
      case WM_HSCROLL:
      case WM_VSCROLL:
      case WM_PARENTNOTIFY:
      case WM_DRAWITEM:
      case WM_MEASUREITEM:
      case WM_DELETEITEM:
      case WM_VKEYTOITEM:
      case WM_CHARTOITEM:
      case WM_COMPAREITEM:
         // reflect the message through the message ::collection::map as WM_REFLECT_BASE+uMsg
         //return interaction_impl::OnWndMsg(WM_REFLECT_BASE+uMsg, wparam, lparam, pResult);
         return FALSE;

         // special case for WM_COMMAND
      case WM_COMMAND:
         {
            // reflect the message through the message ::collection::map as OCM_COMMAND
            /* xxx         int32_t nCode = HIWORD(wparam);
            if (interaction_impl::_001OnCommand(0, MAKELONG(nCode, WM_REFLECT_BASE+WM_COMMAND), NULL, NULL))
            {
            if (pResult != NULL)
            *pResult = 1;
            return TRUE;
            } */
         }
         break;

         // special case for WM_NOTIFY
/*      case WM_NOTIFY:
         {
            // reflect the message through the message ::collection::map as OCM_NOTIFY
            NMHDR* pNMHDR = (NMHDR*)lparam;
            //            int32_t nCode = pNMHDR->code;
            //            __NOTIFY notify;
            //          notify.pResult = pResult;
            //        notify.pNMHDR = pNMHDR;
            // xxxx         return interaction_impl::_001OnCommand(0, MAKELONG(nCode, WM_REFLECT_BASE+WM_NOTIFY), &notify, NULL);
         }

         // other special cases (WM_CTLCOLOR family)*/
      default:
         if (uMsg >= WM_CTLCOLORMSGBOX && uMsg <= WM_CTLCOLORSTATIC)
         {
            // fill in special struct for compatiblity with 16-bit WM_CTLCOLOR
            /*__CTLCOLOR ctl;
            ctl.hDC = (HDC)wparam;
            ctl.nCtlType = uMsg - WM_CTLCOLORMSGBOX;
            //ASSERT(ctl.nCtlType >= CTLCOLOR_MSGBOX);
            ASSERT(ctl.nCtlType <= CTLCOLOR_STATIC);

            // reflect the message through the message ::collection::map as OCM_CTLCOLOR
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

   void interaction_impl::OnParentNotify(UINT message, LPARAM lparam)
   {
      if ((LOWORD(message) == WM_CREATE || LOWORD(message) == WM_DESTROY))
      {
         if (ReflectLastMsg((oswindow)lparam))
            return;     // eat it
      }
      // not handled - do default
      Default();
   }

   void interaction_impl::OnSetFocus(::user::interaction *)
   {
      bool bHandled;

      bHandled = FALSE;
      if( !bHandled )
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

         const MESSAGE* pMsg = GetCurrentMessage();

         SendMessageToDescendants(pMsg->message, pMsg->wParam, pMsg->lParam, TRUE, TRUE);

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

      if(m_pui->is_message_only_window())
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

                  if (m_pui->has_pending_graphical_update() || m_pui->check_need_layout())
                  {

                     RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE);

                     m_pui->on_after_graphical_update();

                  }

               }

               DWORD dwDiff = ::get_tick_count() - dwStart;

               if (dwDiff < 20)
               {

                  Sleep(20 - dwDiff);

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
         if(m_event.wait(millis(dwTimeout)).timeout())
         {
            TRACE("print_window::time_out");
         }
      }


	  static_function UINT c_cdecl s_print_window(LPVOID pvoid)
      {
         print_window * pprintwindow = (print_window *) pvoid;
         try
         {
            HANDLE hevent = (HANDLE) pprintwindow->m_event.get_os_data();
            throw not_implemented(pprintwindow->get_app());
/*            ::PrintWindow(pprintwindow->m_hwnd, pprintwindow->m_hdc, 0);
            ::SetEvent(hevent);*/
         }
         catch(...)
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

      _001UpdateWindow();

   }


   void interaction_impl::_001OnPrint(::signal_details * pobj)
   {

      _001UpdateWindow();

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
      ASSERT(pWnd != NULL && LNX_WINDOW(pWnd)->get_handle() != NULL);
      LRESULT lResult;
      if (LNX_WINDOW(pWnd)->SendChildNotifyLastMsg(&lResult))
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
      sp(::interaction_impl) pWnd = ::linux::interaction_impl::FromHandlePermanent(hWndChild);
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
      ASSERT(::IsWindow((oswindow) get_handle()));

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
      ASSERT(::IsWindow((oswindow) get_handle()));
      if(LNX_WINDOW(pWnd)->get_handle() == NULL)
      {
         return ::user::interaction_impl::IsChild(pWnd);
      }
      else
      {
         return ::IsChild((oswindow) get_handle(), LNX_WINDOW(pWnd)->get_handle()) != FALSE;
      }
   }


   bool interaction_impl::IsWindow() const
   {

      return ::IsWindow(m_oswindow) != FALSE;

   }


   oswindow interaction_impl::get_handle() const
   {

      return m_oswindow;

   }


   #define SWP_IGNOREPALACEGUARD 0x80000000

   bool interaction_impl::SetWindowPos(int_ptr z, int32_t x, int32_t y, int32_t cx, int32_t cy, UINT nFlags)
   {

      //::user::interaction_impl::SetWindowPos(z, x, y, cx, cy, nFlags);

      //m_rectParentClient = m_rectParentClientRequest;

      xdisplay d(m_oswindow->display());

      if(!::IsWindowVisibleRaw(m_oswindow))
      {

         XSizeHints hints = {};

         if(!(nFlags & SWP_NOSIZE))
         {

            hints.flags |= PSize;

            hints.width = cx;

            hints.height = cy;

         }

         if(!(nFlags & SWP_NOMOVE))
         {

            hints.flags |= PPosition;

            hints.x = x;

            hints.y = y;

         }

         XSetNormalHints(m_oswindow->display(), m_oswindow->window(), &hints);

         if((nFlags & SWP_SHOWWINDOW))
         {

            XMapWindow(m_oswindow->display(), m_oswindow->window());

         }

      }

      {

         if(!(nFlags & SWP_NOMOVE) && !(nFlags & SWP_NOSIZE))
         {

            XMoveResizeWindow(m_oswindow->display(), m_oswindow->window(), x, y, cx, cy);

         }
         else if(!(nFlags & SWP_NOMOVE))
         {

            XMoveWindow(m_oswindow->display(), m_oswindow->window(), x, y);

         }
         else if(!(nFlags & SWP_NOSIZE))
         {

            XResizeWindow(m_oswindow->display(), m_oswindow->window(), cx, cy);

         }

      }


      if(!(nFlags & SWP_NOZORDER))
      {

         wm_state_above_raw((oswindow)get_handle(), z == ZORDER_TOPMOST);

         if(z == ZORDER_TOP)
         {

            XRaiseWindow(m_oswindow->display(), m_oswindow->window());

         }

      }

   }


   void interaction_impl::MoveWindow(int32_t x, int32_t y, int32_t nWidth, int32_t nHeight, bool bRepaint)
   {

      SetWindowPos(0, x, y, nWidth, nHeight, bRepaint ? SWP_SHOWWINDOW : 0);

   }


   bool interaction_impl::ClientToScreen(LPRECT lprect)
   {

      class rect64 rectWindow;

      if(!GetWindowRect(rectWindow))
      {

         return false;

      }

      lprect->left   += (LONG) rectWindow.left;
      lprect->right  += (LONG) rectWindow.left;
      lprect->top    += (LONG) rectWindow.top;
      lprect->bottom += (LONG) rectWindow.top;

      return true;

   }


   bool interaction_impl::ClientToScreen(LPPOINT lppoint)
   {

      class rect64 rectWindow;

      if(!GetWindowRect(rectWindow))
      {

         return false;

      }

      lppoint->x     += (LONG) rectWindow.left;
      lppoint->y     += (LONG) rectWindow.top;

      return true;

   }


   bool interaction_impl::ClientToScreen(RECT64 * lprect)
   {

      class rect rectWindow;

      if(!GetWindowRect(rectWindow))
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

      if(!GetWindowRect(rectWindow))
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

      if(!GetWindowRect(rectWindow))
      {

         return false;

      }

      lprect->left   -= (LONG) rectWindow.left;
      lprect->right  -= (LONG) rectWindow.left;
      lprect->top    -= (LONG) rectWindow.top;
      lprect->bottom -= (LONG) rectWindow.top;

      return true;

   }


   bool interaction_impl::ScreenToClient(LPPOINT lppoint)
   {

      class rect64 rectWindow;

      if(!GetWindowRect(rectWindow))
      {

         return false;

      }

      lppoint->x     -= (LONG) rectWindow.left;
      lppoint->y     -= (LONG) rectWindow.top;

      return true;

   }


   bool interaction_impl::ScreenToClient(RECT64 * lprect)
   {

      class rect64 rectWindow;

      if(!GetWindowRect(rectWindow))
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

      if(!GetWindowRect(rectWindow))
      {

         return false;

      }

      lppoint->x     -= rectWindow.left;
      lppoint->y     -= rectWindow.top;

      return true;

   }


   bool interaction_impl::_001GetWindowRect(RECT64 * lprect)
   {

      return ::user::interaction_impl::GetWindowRect(lprect);



      if(!::IsWindow((oswindow) get_handle()))
      {

         return false;

      }

      // if it is temporary interaction_impl - probably not ca2 wrapped interaction_impl
      //if(m_pui == NULL || m_pui == this)
      {
         rect rect32;

         if(!::GetWindowRect((oswindow) get_handle(), rect32))
         {

            return false;

         }

         ::copy(lprect, rect32);

      }
//      else
      {
  //      interaction_impl_base::GetWindowRect(lprect);
      }

      return true;

   }


   bool interaction_impl::GetWindowRect(RECT64 * lprect)
   {

      if(!_001GetWindowRect(lprect))
      {

         return false;

      }

      return true;

   }

   bool interaction_impl::GetClientRect(RECT64 * lprect)
   {

      if(!::IsWindow((oswindow) get_handle()))
      {

         return false;

      }
      // if it is temporary interaction_impl - probably not ca2 wrapped interaction_impl
      //if(m_pui == NULL || m_pui == this)
      {

         rect rect32;

         if(!::GetClientRect((oswindow) get_handle(), rect32))
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

   void interaction_impl::_001WindowMinimize()
   {

      if(m_pui != NULL)
      {

         m_pui->m_eappearanceBefore = m_pui->m_eappearance;

         m_pui->m_eappearance = ::user::AppearanceIconic;

         m_pui->ModifyStyleEx(WS_VISIBLE, 0, 0);

      }

      wm_iconify_window(get_handle());

   }

   void interaction_impl::_001WindowRestore()
   {

      if(m_pui != NULL)
      {

         m_pui->m_eappearance = ::user::AppearanceNormal;

      }

      ::ShowWindow(m_oswindow, SW_RESTORE);

   }


   bool interaction_impl::ShowWindow(int32_t nCmdShow)
   {

      ::ShowWindow(m_oswindow, nCmdShow);

      return m_pui->IsWindowVisible();

   }


   bool interaction_impl::WfiIsIconic()
   {

      if(!::IsWindow(m_oswindow))
      {

         return false;

      }

      if(GetExStyle() & WS_EX_LAYERED)
      {

         return m_pui->m_eappearance == ::user::AppearanceIconic;

      }
      else
      {

         return ::IsIconic((oswindow) get_handle()) != FALSE;

      }

   }


   bool interaction_impl::WfiIsZoomed()
   {

      if(!::IsWindow(m_oswindow))
      {

         return false;

      }

      return m_pui->m_eappearance == ::user::AppearanceZoomed;

   }


   LONG_PTR interaction_impl::get_window_long_ptr(int32_t nIndex) const
   {

      return ::GetWindowLongPtr((oswindow) get_handle(), nIndex);

   }


   LONG_PTR interaction_impl::set_window_long_ptr(int32_t nIndex, LONG_PTR lValue)
   {

      return ::SetWindowLongPtr((oswindow) get_handle(), nIndex, lValue);

   }





   // interaction_impl
   /* interaction_impl::operator oswindow() const
   { return this == NULL ? NULL : get_handle(); }*/
   bool interaction_impl::operator==(const ::user::interaction_impl& wnd) const
   {
      return LNX_WINDOW(const_cast < ::user::interaction_impl * >  (&wnd))->get_handle() ==((interaction_impl *)this)->get_handle();
   }

   bool interaction_impl::operator!=(const ::user::interaction_impl& wnd) const
   {
      return LNX_WINDOW(const_cast < ::user::interaction_impl * >  (&wnd))->get_handle() != ((interaction_impl *)this)->get_handle();
   }


   DWORD interaction_impl::GetStyle() const
   {

      return (DWORD)::GetWindowLong((oswindow) get_handle(), GWL_STYLE);

   }


   DWORD interaction_impl::GetExStyle() const
   {

      return (DWORD)::GetWindowLong((oswindow) get_handle(), GWL_EXSTYLE);

   }


   bool interaction_impl::ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags)
   {

      set_window_long(GWL_STYLE, (GetStyle() | dwAdd) & ~(dwRemove));

      if(nFlags != 0)
      {

         SetWindowPos(0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOMOVE | nFlags);

      }

      return true;

   }


   bool interaction_impl::ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags)
   {

      set_window_long(GWL_EXSTYLE, (GetStyle() | dwAdd) & ~(dwRemove));

      if(nFlags != 0)
      {

         SetWindowPos(0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOMOVE | nFlags);

      }

      return true;

   }


   void interaction_impl::set_owner(::user::interaction * pOwnerWnd)
   {
//      m_puiOwner = pOwnerWnd;
m_pui->SetOwner((pOwnerWnd));
   }

   LRESULT interaction_impl::send_message(UINT message, WPARAM wparam, lparam lparam)
   {

      return ::user::interaction_impl::send_message(message, wparam, lparam);

   }


   bool interaction_impl::post_message(UINT message, WPARAM wparam, lparam lparam)
   {

      return ::PostMessage((oswindow) get_handle(), message, wparam, lparam) != FALSE;

   }


   bool interaction_impl::DragDetect(POINT pt) const
   {

      throw not_implemented(get_app());
      //ASSERT(::IsWindow((oswindow) get_handle()));

      //return ::DragDetect(get_handle(), pt) != FALSE;

   }


   void interaction_impl::SetWindowText(const char * lpszString)
   {

      m_strWindowText = lpszString;

      xdisplay d(m_oswindow->display());

      XStoreName(m_oswindow->display(), m_oswindow->window(), m_strWindowText);

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

   }


   int32_t interaction_impl::GetWindowRgn(HRGN hRgn)
   {

      throw not_implemented(get_app());

//      ASSERT(::IsWindow((oswindow) get_handle()) && hRgn != NULL); return ::GetWindowRgn(get_handle(), hRgn);

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

      xdisplay d(m_oswindow->display());

      oswindow oswindow;

      if(get_handle() == NULL)
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
//      (dynamic_cast < ::linux::graphics * >(g.m_p))->attach(cairo_create(cairo_xlib_surface_create(oswindow->display(), oswindow->interaction_impl(), oswindow->visual(),rectClient.width(), rectClient.height())));
      return g.detach();
   }

   ::draw2d::graphics * interaction_impl::GetWindowDC()
   {
      ASSERT(::IsWindow((oswindow) get_handle()));
      ::draw2d::graphics_sp g(allocer());
      g->attach(::GetWindowDC(get_handle()));
      return g.detach();
   }

   bool interaction_impl::ReleaseDC(::draw2d::dib * pdib)
   {

      if(pdib == NULL)
         return false;

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

//      if(((Gdiplus::Graphics *)(dynamic_cast<::linux::graphics * >(pgraphics))->get_handle()) == NULL)
  //       return false;

      //::ReleaseDC(get_handle(), (dynamic_cast < ::linux::graphics * > (pgraphics))->detach());

//      (dynamic_cast<::linux::graphics * >(pgraphics))->m_hdc = NULL;

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

      if(!m_pui->is_this_visible())
      {

         return false;

      }

      if(m_pui->m_pparent != NULL)
      {

         if(!m_pui->m_pparent->IsWindowVisible())
         {

            return false;

         }

      }

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
      ASSERT(::IsWindow((oswindow) get_handle()));
      //interaction_impl::SendMessageToDescendants(get_handle(), message, wparam, lparam, bDeep, bOnlyPerm);

      // walk through oswindows to avoid creating temporary interaction_impl objects
      // unless we need to call this function recursively
      user::interaction * pui = m_pui->first_child();
      while(pui != NULL)
      {
         try
         {
            pui->send_message(message, wparam, lparam);
         }
         catch(...)
         {
         }
         if (bDeep)
         {
            // send to child windows after parent
            try
            {
               pui->SendMessageToDescendants(message, wparam, lparam, bDeep, bOnlyPerm);
            }
            catch(...)
            {
            }
         }
         try
         {
            pui = pui->next_sibling();
         }
         catch(...)
         {
            break;
         }
      }
   }

   ::user::interaction * interaction_impl::GetDescendantWindow(id id) const
   {
      ASSERT(::IsWindow((oswindow) get_handle()));
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

//      ASSERT(::IsWindow((oswindow) get_handle()));

      if(flags & RDW_UPDATENOW)
      {

         _001UpdateWindow();

      }
      else
      {

         m_pui->m_bRedraw = true;

      }

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
      //return ::DrawCaption(get_handle(), (HDC)(dynamic_cast<::linux::graphics * >(pgraphics))->get_handle(), lprc, uFlags) != FALSE;

   }


   bool interaction_impl::SetTimer(uint_ptr nIDEvent, UINT nElapse, PFN_TIMER pfnTimer)
   {

      return ::user::interaction_impl::SetTimer(nIDEvent, nElapse, pfnTimer);


//        UNREFERENCED_PARAMETER(lpfnTimer);
//
//        m_pui->m_pauraapp->set_timer(m_pui, nIDEvent, nElapse);
//
//        return nIDEvent;

      //throw not_implemented(get_app());
      //ASSERT(::IsWindow((oswindow) get_handle()));
      //return ::SetTimer(get_handle(), nIDEvent, nElapse, lpfnTimer);

   }


   bool interaction_impl::KillTimer(uint_ptr nIDEvent)
   {

      return ::user::interaction_impl::KillTimer(nIDEvent);

//       m_pui->m_pauraapp->unset_timer(m_pui, nIDEvent);

  //     return TRUE;

  //return ::user::int

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

      ::user::interaction_impl * pimpl = ::linux::interaction_impl::from_handle(::GetActiveWindow());

      if(pimpl == NULL)
      {

         return NULL;

      }

      return pimpl->m_pui;

   }

   ::user::interaction * interaction_impl::SetActiveWindow()
   {


      ::user::interaction_impl * pimpl = ::linux::interaction_impl::from_handle(::SetActiveWindow(get_handle()));

      if(pimpl == NULL)
      {

         return NULL;

      }

      return pimpl->m_pui;

   }


   ::user::interaction * PASCAL interaction_impl::GetFocus()
   {

      oswindow w = ::GetFocus();

      if(w == NULL)
         return NULL;

      return w->m_pimpl->m_pui;

   }


   bool interaction_impl::SetFocus()
   {

      oswindow w = ::SetFocus(get_handle());

      if(w  == NULL)
         return NULL;

      return w->m_pimpl->m_pui;

   }


   ::user::interaction * PASCAL interaction_impl::GetDesktopWindow()
   {
/*
      return ::linux::interaction_impl::from_handle(::GetDesktopWindow());
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
//      return ::linux::interaction_impl::from_handle(::GetNextDlgGroupItem(get_handle(), (oswindow) pWndCtl->get_handle(), bPrevious));

   }

   ::user::interaction * interaction_impl::GetNextDlgTabItem(::user::interaction * pWndCtl, bool bPrevious) const
   {

      throw not_implemented(get_app());
//      ASSERT(::IsWindow((oswindow) get_handle()));
//      return ::linux::interaction_impl::from_handle(::GetNextDlgTabItem(get_handle(), (oswindow) pWndCtl->get_handle(), bPrevious));

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
//      return ::linux::interaction_impl::from_handle(::ChildWindowFromPoint(get_handle(), point));

   }

   ::user::interaction * interaction_impl::ChildWindowFromPoint(POINT point, UINT nFlags)
   {

      throw not_implemented(get_app());
//      ASSERT(::IsWindow((oswindow) get_handle()));
//      return ::linux::interaction_impl::from_handle(::ChildWindowFromPointEx(get_handle(), point, nFlags));

   }

   ::user::interaction * PASCAL interaction_impl::FindWindow(const char * lpszClassName, const char * lpszWindowName)
   {

//      throw not_implemented(get_app());
//      return ::linux::interaction_impl::from_handle(::FindWindow(lpszClassName, lpszWindowName));
      return NULL;

   }

   ::user::interaction * interaction_impl::FindWindowEx(oswindow hwndParent, oswindow hwndChildAfter, const char * lpszClass, const char * lpszWindow)
   {

      throw not_implemented(::get_thread_app());
//      return ::linux::interaction_impl::from_handle(::FindWindowEx(hwndParent, hwndChildAfter, lpszClass, lpszWindow));

   }




   ::user::interaction * interaction_impl::GetTopWindow() const
   {

      if(m_pui->m_uiptraChild.get_size() <= 0)
         return NULL;

      return m_pui->m_uiptraChild[0];
    //  throw not_implemented(get_app());
//      ASSERT(::IsWindow((oswindow) get_handle()));
//      return ::linux::interaction_impl::from_handle(::GetTopWindow(get_handle()));

   }

   ::user::interaction * interaction_impl::GetWindow(UINT nCmd)
   {

      ASSERT(::IsWindow((oswindow) get_handle()));
//      return ::linux::interaction_impl::from_handle(::GetWindow(get_handle(), nCmd));
      return NULL;

   }

   ::user::interaction * interaction_impl::GetLastActivePopup()
   {


      throw todo(get_app());
//      ASSERT(::IsWindow((oswindow) get_handle()));
//      return ::linux::interaction_impl::from_handle(::GetLastActivePopup(get_handle()));

   }


   ::user::interaction * interaction_impl::SetParent(::user::interaction * pWndNewParent)
   {

      ASSERT(::IsWindow((oswindow) get_handle()));

      return ::linux::interaction_impl::from_handle(::SetParent(get_handle(), (oswindow) pWndNewParent->get_handle()))->m_pui;

   }


   ::user::interaction * PASCAL interaction_impl::oswindowFromPoint(POINT point)
   {


      throw not_implemented(::get_thread_app());
//      return ::linux::interaction_impl::from_handle(::oswindowFromPoint(point));

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
//      return ::linux::interaction_impl::from_handle(::GetOpenClipboardWindow());

   }

   ::user::interaction * PASCAL interaction_impl::GetClipboardOwner()
   {

      throw not_implemented(::get_thread_app());
//      return ::linux::interaction_impl::from_handle(::GetClipboardOwner());

   }

   ::user::interaction * PASCAL interaction_impl::GetClipboardViewer()
   {

      throw not_implemented(::get_thread_app());
//      return ::linux::interaction_impl::from_handle(::GetClipboardViewer());

   }

   void interaction_impl::CreateCaret(::draw2d::bitmap* pBitmap)
   {

      throw not_implemented(get_app());
//      ASSERT(::IsWindow((oswindow) get_handle()));
//      ::Createcaret(get_handle(), (HBITMAP)pBitmap->get_handle(), 0, 0);

   }

   void interaction_impl::CreateSolidCaret(int32_t nWidth, int32_t nHeight)
   {

      throw not_implemented(get_app());
//      ASSERT(::IsWindow((oswindow) get_handle()));
//      ::Createcaret(get_handle(), (HBITMAP)0, nWidth, nHeight);

   }

   void interaction_impl::CreateGrayCaret(int32_t nWidth, int32_t nHeight)
   {

      throw not_implemented(get_app());
//      ASSERT(::IsWindow((oswindow) get_handle()));
//      ::Createcaret(get_handle(), (HBITMAP)1, nWidth, nHeight);

   }

   point PASCAL interaction_impl::GetCaretPos()
   {

      throw not_implemented(::get_thread_app());
//      point point;
//      ::GetcaretPos((LPPOINT)&point); return point;

   }

   void PASCAL interaction_impl::SetCaretPos(POINT point)
   {

    throw not_implemented(::get_thread_app());
//      ::SetcaretPos(point.x, point.y);

   }

   void interaction_impl::HideCaret()
   {

      throw not_implemented(get_app());
//      ::Hidecaret(get_handle());

   }

   void interaction_impl::ShowCaret()
   {

      throw not_implemented(get_app());
//    ::Showcaret(get_handle());

   }

   bool interaction_impl::SetForegroundWindow()
   {

//    throw not_implemented(get_app());
//      return ::SetForegroundWindow(get_handle()) != FALSE;

   }

   ::user::interaction * PASCAL interaction_impl::GetForegroundWindow()
   {

      return NULL;

         throw not_implemented(::get_thread_app());
//      return ::linux::interaction_impl::from_handle(::GetForegroundWindow());

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
//      const_cast < interaction_impl * > (this)->send_message(WM_PRINT, (WPARAM)(dynamic_cast<::linux::graphics * >(pgraphics))->get_handle(), dwFlags);

   }

   void interaction_impl::PrintClient(::draw2d::graphics * pgraphics, DWORD dwFlags) const
   {

      throw not_implemented(get_app());
//      ASSERT(::IsWindow((oswindow) get_handle()));
//      const_cast < interaction_impl * > (this)->send_message(WM_PRINTCLIENT, (WPARAM)(dynamic_cast<::linux::graphics * >(pgraphics))->get_handle(), dwFlags);

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
   { Default(); }
   void interaction_impl::OnActivate(UINT, ::user::interaction *, bool)
   { Default(); }
   void interaction_impl::OncancelMode()
   { Default(); }
   void interaction_impl::OnChildActivate()
   { Default(); }
   void interaction_impl::OnClose()
   { Default(); }
   void interaction_impl::OnContextMenu(::user::interaction *, point)
   { Default(); }

   bool interaction_impl::OnCopyData(::user::interaction *, COPYDATASTRUCT*)
   {

      return Default() != FALSE;

   }

   void interaction_impl::OnEnable(bool)
   { Default(); }
   void interaction_impl::OnEndSession(bool)
   { Default(); }

   bool interaction_impl::OnEraseBkgnd(::draw2d::graphics *)
   {

      return Default() != FALSE;

   }

   void interaction_impl::OnGetMinMaxInfo(MINMAXINFO*)
   { Default(); }
   void interaction_impl::OnIconEraseBkgnd(::draw2d::graphics *)
   { Default(); }
   void interaction_impl::OnKillFocus(::user::interaction *)
   { Default(); }
   LRESULT interaction_impl::OnMenuChar(UINT, UINT, ::user::menu*)
   { return Default(); }
   void interaction_impl::OnMenuSelect(UINT, UINT, HMENU)
   { Default(); }
   void interaction_impl::OnMove(int32_t, int32_t)
   { Default(); }

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
      if(Session.get_cursor() != NULL
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
   { Default(); }
   void interaction_impl::OnSize(UINT, int32_t, int32_t)
   { Default(); }
   void interaction_impl::OnTcard(UINT, DWORD)
   { Default(); }
   void interaction_impl::OnWindowPosChanging(WINDOWPOS*)
   { Default(); }
   void interaction_impl::OnWindowPosChanged(WINDOWPOS*)
   { Default(); }
   void interaction_impl::OnDropFiles(HDROP)
   { Default(); }
   void interaction_impl::OnPaletteIsChanging(::user::interaction *)
   { Default(); }

   bool interaction_impl::OnNcActivate(bool)
   {

      return Default() != FALSE;

   }

   void interaction_impl::OnNcCalcSize(bool, NCCALCSIZE_PARAMS*)
   { Default(); }

   bool interaction_impl::OnNcCreate(::user::create_struct *)
   {

      return Default() != FALSE;

   }

   LRESULT interaction_impl::OnNcHitTest(point)
   { return Default(); }
   void interaction_impl::OnNcLButtonDblClk(UINT, point)
   { Default(); }
   void interaction_impl::OnNcLButtonDown(UINT, point)
   { Default(); }
   void interaction_impl::OnNcLButtonUp(UINT, point)
   { Default(); }
   void interaction_impl::OnNcMButtonDblClk(UINT, point)
   { Default(); }
   void interaction_impl::OnNcMButtonDown(UINT, point)
   { Default(); }
   void interaction_impl::OnNcMButtonUp(UINT, point)
   { Default(); }
   void interaction_impl::OnNcMouseMove(UINT, point)
   { Default(); }
   void interaction_impl::OnNcPaint()
   { Default(); }
   void interaction_impl::OnNcRButtonDblClk(UINT, point)
   { Default(); }
   void interaction_impl::OnNcRButtonDown(UINT, point)
   { Default(); }
   void interaction_impl::OnNcRButtonUp(UINT, point)
   { Default(); }
   void interaction_impl::OnSysChar(UINT, UINT, UINT)
   { Default(); }
   void interaction_impl::OnSysCommand(UINT, LPARAM)
   { Default(); }
   void interaction_impl::OnSysDeadChar(UINT, UINT, UINT)
   { Default(); }
   void interaction_impl::OnSysKeyDown(UINT, UINT, UINT)
   { Default(); }
   void interaction_impl::OnSysKeyUp(UINT, UINT, UINT)
   { Default(); }
   void interaction_impl::OnCompacting(UINT)
   { Default(); }
   void interaction_impl::OnFontChange()
   { Default(); }
   void interaction_impl::OnPaletteChanged(::user::interaction *)
   { Default(); }
   void interaction_impl::OnSpoolerStatus(UINT, UINT)
   { Default(); }
   void interaction_impl::OnTimeChange()
   { Default(); }
   void interaction_impl::OnChar(UINT, UINT, UINT)
   { Default(); }
   void interaction_impl::OnDeadChar(UINT, UINT, UINT)
   { Default(); }
   void interaction_impl::OnKeyDown(UINT, UINT, UINT)
   { Default(); }
   void interaction_impl::OnKeyUp(UINT, UINT, UINT)
   { Default(); }
   void interaction_impl::OnLButtonDblClk(UINT, point)
   { Default(); }
   void interaction_impl::OnLButtonDown(UINT, point)
   { Default(); }
   void interaction_impl::OnLButtonUp(UINT, point)
   { Default(); }
   void interaction_impl::OnMButtonDblClk(UINT, point)
   { Default(); }
   void interaction_impl::OnMButtonDown(UINT, point)
   { Default(); }
   void interaction_impl::OnMButtonUp(UINT, point)
   { Default(); }
   int32_t interaction_impl::OnMouseActivate(::user::interaction *, UINT, UINT)
   { return (int32_t)Default(); }
   void interaction_impl::OnMouseMove(UINT, point)
   { Default(); }

   bool interaction_impl::OnMouseWheel(UINT, short, point)
   {

      return Default() != FALSE;

   }

   LRESULT interaction_impl::OnRegisteredMouseWheel(WPARAM, LPARAM)
   { return Default(); }
   void interaction_impl::OnRButtonDblClk(UINT, point)
   { Default(); }
   void interaction_impl::OnRButtonDown(UINT, point)
   { Default(); }
   void interaction_impl::OnRButtonUp(UINT, point)
   { Default(); }
   void interaction_impl::OnTimer(uint_ptr)
   { Default(); }
   void interaction_impl::OnInitMenu(::user::menu*)
   { Default(); }
   void interaction_impl::OnInitMenuPopup(::user::menu*, UINT, bool)
   { Default(); }
   void interaction_impl::OnAskCbFormatName(UINT nMaxCount, LPTSTR pszName)
   {
      (nMaxCount);
      if(nMaxCount>0)
      {
         /* defwindow proc should do this for us, but to be safe, we'll do it here too */
         pszName[0]='\0';
      }
      Default();
   }
   void interaction_impl::OnChangeCbChain(oswindow, oswindow)
   { Default(); }
   void interaction_impl::OnDestroyClipboard()
   { Default(); }
   void interaction_impl::OnDrawClipboard()
   { Default(); }
   void interaction_impl::OnHScrollClipboard(::user::interaction *, UINT, UINT)
   { Default(); }
   void interaction_impl::OnPaintClipboard(::user::interaction *, HGLOBAL)
   { Default(); }
   void interaction_impl::OnRenderAllFormats()
   { Default(); }
   void interaction_impl::OnRenderFormat(UINT)
   { Default(); }
   void interaction_impl::OnSizeClipboard(::user::interaction *, HGLOBAL)
   { Default(); }
   void interaction_impl::OnVScrollClipboard(::user::interaction *, UINT, UINT)
   { Default(); }
   UINT interaction_impl::OnGetDlgCode()
   { return (UINT)Default(); }
   void interaction_impl::OnMDIActivate(bool, ::user::interaction *, ::user::interaction *)
   { Default(); }
   void interaction_impl::OnEnterMenuLoop(bool)
   { Default(); }
   void interaction_impl::OnExitMenuLoop(bool)
   { Default(); }
   // Win4 support
//   void interaction_impl::OnStyleChanged(int32_t, LPSTYLESTRUCT)
//   { Default(); }
//   void interaction_impl::OnStyleChanging(int32_t, LPSTYLESTRUCT)
//   { Default(); }
   void interaction_impl::OnSizing(UINT, LPRECT)
   { Default(); }
   void interaction_impl::OnMoving(UINT, LPRECT)
   { Default(); }
   void interaction_impl::OncaptureChanged(::user::interaction *)
   { Default(); }

   bool interaction_impl::OnDeviceChange(UINT, dword_ptr)
   {

      return Default() != FALSE;

   }

   void interaction_impl::OnWinIniChange(const char *)
   { Default(); }
   void interaction_impl::OnChangeUIState(UINT, UINT)
   { Default(); }
   void interaction_impl::OnUpdateUIState(UINT, UINT)
   { Default(); }

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

   void interaction_impl::on_delete(::object * pui)
   {
      UNREFERENCED_PARAMETER(pui);
   }




//   void interaction_impl::_001OnEraseBkgnd(::signal_details * pobj)
//   {
//      SCAST_PTR(::message::erase_bkgnd, perasebkgnd, pobj);
//      perasebkgnd->m_bRet = true;
//      perasebkgnd->set_result(TRUE);
//   }


   void interaction_impl::_001BaseWndInterfaceMap()
   {

      Session.user()->window_map().set((int_ptr)get_handle(), this);

   }


   void interaction_impl::_001OnTriggerMouseInside()
   {


//      m_bMouseHover = true;

   }


} // namespace linux



/////////////////////////////////////////////////////////////////////////////
// The WndProc for all interaction_impl's and derived classes


__STATIC void CLASS_DECL_BASE __pre_init_dialog(
   ::user::interaction * pWnd, LPRECT lpRectOld, DWORD* pdwStyleOld)
{
   ASSERT(lpRectOld != NULL);
   ASSERT(pdwStyleOld != NULL);

   LNX_WINDOW(pWnd)->GetWindowRect(lpRectOld);
   *pdwStyleOld = LNX_WINDOW(pWnd)->GetStyle();
}

__STATIC void CLASS_DECL_BASE __post_init_dialog(
   ::user::interaction * pWnd, const RECT& rectOld, DWORD dwStyleOld)
{
   // must be hidden to start with
   if (dwStyleOld & WS_VISIBLE)
      return;

   // must not be visible after WM_INITDIALOG
   if (LNX_WINDOW(pWnd)->GetStyle() & (WS_VISIBLE|WS_CHILD))
      return;

   // must not move during WM_INITDIALOG
   rect rect;
   LNX_WINDOW(pWnd)->GetWindowRect(rect);
   if (rectOld.left != rect.left || rectOld.top != rect.top)
      return;

   // must be unowned or owner disabled
   sp(::user::interaction) pParent = LNX_WINDOW(pWnd)->GetWindow(GW_OWNER);
   if (pParent != NULL && pParent->is_window_enabled())
      return;

   if (!LNX_WINDOW(pWnd)->CheckAutoCenter())
      return;

   // center modal dialog boxes/message boxes
   //LNX_WINDOW(pWnd)->CenterWindow();
}






void wm_state_above_raw(oswindow w, bool bSet);



namespace linux
{


   void interaction_impl::_001Expose()
   {

      ::user::interaction_impl::_001UpdateWindow();

   }


   void interaction_impl::_001UpdateWindow()
   {

      ::user::interaction_impl::_001UpdateWindow();

   }


   void interaction_impl::set_viewport_org(::draw2d::graphics * pgraphics)
   {
      // graphics will be already set its view port to the interaction_impl for linux - cairo with xlib

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

      if(nFlag == GW_HWNDNEXT)
      {

         return get_next(true, NULL);

      }
      else
      {

         throw interface_only_exception(get_app());

      }

   }


   ::user::interaction * interaction_impl::get_next(bool bIgnoreChildren,int32_t * piLevel)
   {

      return  m_pui->get_next(bIgnoreChildren, piLevel);

   }

   void interaction_impl::show_task(bool bShow)
   {

      wm_toolwindow(get_handle(), bShow ? 0: 1);

   }

}




