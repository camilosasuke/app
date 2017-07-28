#include "framework.h" // from "base/user/user.h"
#include "base/user/core_user.h"
#include "aura/node/windows/windows.h"
#include "windows.h"
#include <windows.h>
#include <shobjidl.h>
#include "base/os/windows/windows_system_interaction_impl.h"


// const GUID CLSID_TaskbarList = { 0x56FDF344, 0xFD6D, 0x11D0,{ 0x95, 0x8A, 0x00, 0x60, 0x97, 0xC9, 0xA0, 0x90 } };
//const GUID IID_ITaskbarList = { 0x56FDF342, 0xFD6D, 0x11D0,{ 0x95, 0x8A, 0x00, 0x60, 0x97, 0xC9, 0xA0, 0x90 } };
//const GUID IID_ITaskbarList2 = { 0x602D4995, 0xB13A, 0x429b,{ 0xA6, 0x6E, 0x19, 0x35, 0xE4, 0x4F, 0x43, 0x17 } };
//const GUID IID_ITaskList3 = { 0xEA1AFB91, 0x9E28, 0x4B86,{ 0x90, 0xE9, 0x9E, 0x9F, 0x8A, 0x5E, 0xEF, 0xAF } };

CLASS_DECL_BASE int g_iMouseDown = 0;

CLASS_DECL_BASE thread_int_ptr < DWORD_PTR > t_time1;
CLASS_DECL_BASE thread_int_ptr < DWORD_PTR > t_time2;


extern CLASS_DECL_BASE thread_int_ptr < DWORD_PTR > t_time1;

thread_int_ptr < HHOOK > t_hHookOldCbtFilter;
thread_pointer < ::windows::interaction_impl  > t_pwndInit;

//LRESULT CALLBACK __activation_window_procedure(oswindow oswindow,UINT nMsg,WPARAM wParam,LPARAM lParam);

const char * gen_OldWndProc = "::core::OldWndProc423";


#define __WNDCLASS(s)    L"ca2" L##s
#define __WND             __WNDCLASS("Wnd")
#define __WNDCONTROLBAR   __WNDCLASS("ControlBar")
#define __WNDMDIFRAME     __WNDCLASS("MDIFrame")
#define __WNDFRAMEORVIEW  __WNDCLASS("FrameOrView")
#define __WNDOLECONTROL   __WNDCLASS("OleControl")


const unichar gen_WndControlBar[] = __WNDCONTROLBAR;
const unichar gen_WndMDIFrame[] = __WNDMDIFRAME;
const unichar gen_WndFrameOrView[] = __WNDFRAMEORVIEW;
const unichar gen_WndOleControl[] = __WNDOLECONTROL;

struct __CTLCOLOR
{
   oswindow oswindow;
   HDC hDC;
   UINT nCtlType;
};


const unichar gen_Wnd[] = __WND;


namespace windows
{


   interaction_impl::interaction_impl() :
      ::aura::timer_array(get_app())
   {

      user_common_construct();

   }


   HRESULT STDMETHODCALLTYPE interaction_impl::DragEnter(
      /* [unique][in] */ __RPC__in_opt IDataObject *pDataObj,
      /* [in] */ DWORD grfKeyState,
      /* [in] */ POINTL pt,
      /* [out][in] */ __RPC__inout DWORD *pdwEffect)
   {

      ::message::drag_and_drop m(this, MESSAGE_OLE_DRAGENTER);

      m.pDataObj = pDataObj;
      m.grfKeyState = grfKeyState;
      m.pt = pt;
      m.dwEffect = DROPEFFECT_NONE;

      try
      {

         message_handler(&m);

      }
      catch (...)
      {

      }

      // Call the drag and drop helper.
      if (m_bUseDnDHelper)
      {
         // The DnD helper needs an IDataObject interface, so get one from
         // the COleDataObject.  Note that the FALSE param means that
         // GetIDataObject will not AddRef() the returned interface, so
         // we do not Release() it.
         POINT p;
         p.x = m.pt.x;
         p.y = m.pt.y;
         m_piDropHelper->DragEnter(get_handle(), m.pDataObj, &p, m.dwEffect);
      }

      *pdwEffect = m.dwEffect;

      return S_OK;

   }

   HRESULT STDMETHODCALLTYPE interaction_impl::DragOver(
      /* [in] */ DWORD grfKeyState,
      /* [in] */ POINTL pt,
      /* [out][in] */ __RPC__inout DWORD *pdwEffect)
   {
      ::message::drag_and_drop m(this, MESSAGE_OLE_DRAGOVER);

      m.pDataObj = NULL;
      m.grfKeyState = grfKeyState;
      m.pt = pt;
      m.dwEffect = DROPEFFECT_NONE;

      try
      {

         message_handler(&m);

      }
      catch (...)
      {

      }

      // Call the drag and drop helper.
      if (m_bUseDnDHelper)
      {
         // The DnD helper needs an IDataObject interface, so get one from
         // the COleDataObject.  Note that the FALSE param means that
         // GetIDataObject will not AddRef() the returned interface, so
         // we do not Release() it.
         POINT p;
         p.x = m.pt.x;
         p.y = m.pt.y;
         m_piDropHelper->DragOver(&p, m.dwEffect);
      }


      *pdwEffect = m.dwEffect;

      return S_OK;
   }

   HRESULT STDMETHODCALLTYPE interaction_impl::DragLeave(void)
   {
      ::message::drag_and_drop m(this, MESSAGE_OLE_DRAGLEAVE);

      m.pDataObj = NULL;
      m.grfKeyState = 0;
      m.pt = { 0,0 };
      m.dwEffect = DROPEFFECT_NONE;

      try
      {

         message_handler(&m);

      }
      catch (...)
      {

      }

      // Call the drag and drop helper.
      if (m_bUseDnDHelper)
      {
         m_piDropHelper->DragLeave();
      }


      return S_OK;

   }

   HRESULT STDMETHODCALLTYPE interaction_impl::Drop(
      /* [unique][in] */ __RPC__in_opt IDataObject *pDataObj,
      /* [in] */ DWORD grfKeyState,
      /* [in] */ POINTL pt,
      /* [out][in] */ __RPC__inout DWORD *pdwEffect)
   {

      ::message::drag_and_drop m(this, MESSAGE_OLE_DRAGDROP);

      m.pDataObj = pDataObj;
      m.grfKeyState = grfKeyState;
      m.pt = pt;
      m.dwEffect = DROPEFFECT_NONE;

      try
      {

         message_handler(&m);

      }
      catch (...)
      {

      }

      // Call the drag and drop helper.
      if (m_bUseDnDHelper)
      {
         // The DnD helper needs an IDataObject interface, so get one from
         // the COleDataObject.  Note that the FALSE param means that
         // GetIDataObject will not AddRef() the returned interface, so
         // we do not Release() it.
         POINT p;
         p.x = m.pt.x;
         p.y = m.pt.y;
         m_piDropHelper->Drop(m.pDataObj, &p, m.dwEffect);
      }

      *pdwEffect = m.dwEffect;

      return S_OK;

   }

   HRESULT STDMETHODCALLTYPE interaction_impl::QueryInterface(REFIID riid, void **ppvObject)
   {

      if (__uuidof(IDropTarget) == riid)
      {
         *ppvObject = this;
      }
      else if (__uuidof(IUnknown) == riid)
      {
         *ppvObject = this;
      }
      else
      {
         *ppvObject = NULL;
         return E_FAIL;
      }

      this->AddRef();

      return S_OK;

   }

   ULONG STDMETHODCALLTYPE interaction_impl::AddRef(void)
   {
      return (ULONG)add_ref();
   }

   ULONG STDMETHODCALLTYPE interaction_impl::Release(void)
   {
      return (ULONG)release();
   }


   void interaction_impl::construct(oswindow oswindow)
   {

      set_handle(oswindow);

   }


   void interaction_impl::user_common_construct()
   {

      m_pfnSuper = NULL;
      //m_pthreadDraw        = NULL;
      m_bUseDnDHelper = false;
      m_bUpdateGraphics = false;
      m_bIgnoreSizeEvent = false;
      m_bIgnoreMoveEvent = false;

      set_handle(NULL);

      ZERO(m_rectParentClient);
      ZERO(m_size);
      ZERO(m_pt);

   }


   interaction_impl::interaction_impl(::aura::application * papp) :
      ::object(papp),
      ::aura::timer_array(papp)
   {

      user_common_construct();

   }


   interaction_impl::~interaction_impl()
   {

      ::multithreading::post_quit_and_wait(m_pthreadUpdateWindow, seconds(10));

   }




   void * interaction_impl::get_os_data() const
   {

      return (void *)((::windows::interaction_impl *) this)->get_handle();

   }



   // Change a interaction_impl's style

   __STATIC bool CLASS_DECL_BASE __modify_style(oswindow oswindow, int32_t nStyleOffset,
      uint32_t dwRemove, uint32_t dwAdd, UINT nFlags)
   {
      ASSERT(oswindow != NULL);
      uint32_t dwStyle = ::GetWindowLong(oswindow, nStyleOffset);
      uint32_t dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
      if (dwStyle == dwNewStyle)
         return FALSE;

      ::SetWindowLong(oswindow, nStyleOffset, dwNewStyle);
      if (nFlags != 0)
      {
         ::SetWindowPos(oswindow, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
      }
      return TRUE;
   }

   bool interaction_impl::ModifyStyle(oswindow oswindow, uint32_t dwRemove, uint32_t dwAdd, UINT nFlags)
   {

      bool bOk = __modify_style(oswindow, GWL_STYLE, dwRemove, dwAdd, 0);

      if (nFlags != 0)
      {

         m_iShowFlags = nFlags;



      }


      return bOk;

   }


   bool interaction_impl::ModifyStyleEx(oswindow oswindow, uint32_t dwRemove, uint32_t dwAdd, UINT nFlags)
   {

      return __modify_style(oswindow, GWL_EXSTYLE, dwRemove, dwAdd, nFlags);

   }


   LRESULT interaction_impl::Default()
   {

      return 0;

   }





   void interaction_impl::pre_subclass_window()
   {
      // no default processing
   }


   bool interaction_impl::create_window_ex(::user::interaction * pui, uint32_t dwExStyle, const char * lpszClassName, const char * lpszWindowName, uint32_t dwStyle, const RECT & rect, ::user::interaction * puiParent, id id, LPVOID lpParam)
   {

      if (puiParent == NULL)
      {

         if (!native_create_window_ex(pui, dwExStyle, lpszClassName, lpszWindowName, dwStyle, rect, NULL, id, lpParam))
            return false;

      }
      else
      {

         if (!native_create_window_ex(pui, dwExStyle, lpszClassName, lpszWindowName, dwStyle, rect, puiParent->get_safe_handle(), id, lpParam))
            return false;

      }

      return true;

   }


   bool interaction_impl::native_create_window_ex(::user::interaction * pui, uint32_t dwExStyle, const char * lpszClassName, const char * lpszWindowName, uint32_t dwStyle, const RECT & rect, oswindow oswindowParent, id id, LPVOID lpParam)
   {
      //::simple_message_box(NULL,"h1","h1",MB_OK);
      UNREFERENCED_PARAMETER(id);
      ASSERT(lpszClassName == NULL || __is_valid_string(lpszClassName) ||
         __is_valid_atom(lpszClassName));
      ENSURE_ARG(lpszWindowName == NULL || __is_valid_string(lpszWindowName));

      // allow modification of several common create parameters
      ::user::create_struct cs;
      cs.dwExStyle = dwExStyle;

      m_pui = pui;
      wstring wstrClassName = calc_window_class();
      wstring wstrWindowName(lpszWindowName);
      cs.lpszClass = wstrClassName.is_empty() ? NULL : (const unichar *)wstrClassName;
      cs.lpszName = wstrWindowName;
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

      if (!m_pui->pre_create_window(cs))
      {

         PostNcDestroy();

         return false;

      }

      if (cs.hwndParent == NULL)
      {

         cs.style &= ~WS_CHILD;

      }

      if (!hook_window_create(this))
      {

         PostNcDestroy();

         return false;

      }

      // if window is not created, it may destroy this object,
      // so keep the app as local var
      ::aura::application * papp = m_pauraapp;

      oswindow oswindow = ::CreateWindowExW(cs.dwExStyle, cs.lpszClass, cs.lpszName, cs.style, cs.x, cs.y, cs.cx, cs.cy, cs.hwndParent, cs.hMenu, cs.hInstance, cs.lpCreateParams);

      if (!unhook_window_create())
         PostNcDestroy();        // cleanup if CreateWindowEx fails too soon

      //::simple_message_box(NULL,"h2","h2",MB_OK);

      //if (cs.hwndParent != HWND_MESSAGE)
      //{

      //   m_spgraphics.alloc(allocer());

      //   m_spgraphics->on_create_window(this);

      //}

      if (oswindow == NULL)
      {

         //::simple_message_box(NULL,"h3","h3",MB_OK);

         //if(m_pauraapp == NULL)
         //   return FALSE;

         if (papp == NULL)
         {

            return false;

         }

         uint32_t dwLastError = GetLastError();
         string strLastError = FormatMessageFromSystem(dwLastError);
         string strMessage;
         strMessage.Format("%s\n\nSystem Error Code: %d", strLastError, dwLastError);


         APPTRACE(::aura::trace::category_AppMsg, 0, "Warning: Window creation failed: GetLastError returned:\n");

         APPTRACE(::aura::trace::category_AppMsg, 0, "%s\n", strMessage);

         try
         {

            if (dwLastError == 0x0000057e)
            {

               APPTRACE("Cannot create a top-level child interaction_impl.");

            }
            else
            {

               APPTRACE("%s", strMessage);

            }

         }
         catch (...)
         {

         }

      }

      if (oswindow == NULL)
         return FALSE;

      //if (!m_pui->m_bMessageWindow)
      //{

      //   m_spgraphics.alloc

      //}

      bool bUnicode = ::IsWindowUnicode(oswindow) != FALSE;

      if (bUnicode)
      {

         output_debug_string("window is unicode");

      }
      else
      {

         output_debug_string("window is ANSI");

      }

      WNDCLASSW wndcls;
      if (wstrClassName.get_length() > 0 && GetClassInfoW(System.m_hinstance, wstrClassName, &wndcls) &&
         wndcls.hIcon != NULL)
      {
         Application.set_icon(m_pui, canew(::visual::icon(get_app(), wndcls.hIcon)), false);
         Application.set_icon(m_pui, canew(::visual::icon(get_app(), wndcls.hIcon)), true);
      }
      //::simple_message_box(NULL,"h4","h4",MB_OK);
      //      oswindow oswindowHandle = get_handle();
      if (oswindow != get_handle())
      {
         //::simple_message_box(NULL,"h4.dame","h4.dame",MB_OK);

         ASSERT(FALSE); // should have been set in send msg hook

      }
      //::simple_message_box(NULL,"h4.ok","h4.ok",MB_OK);

      m_pui = pui;
      //pui->add_ref();
      return TRUE;
   }


   // for child windows
   bool interaction_impl::pre_create_window(::user::create_struct& cs)
   {

      return true;

   }


   bool interaction_impl::create_window(::user::interaction * pui, const char * lpszClassName, const char * lpszWindowName, uint32_t dwStyle, const RECT & rect, ::user::interaction * pParentWnd, id id, ::create * pcreate)
   {

      // can't use for desktop or pop-up windows (use create_window_ex instead)
      ASSERT(pParentWnd != NULL);
      ASSERT((dwStyle & WS_POPUP) == 0);

      return create_window_ex(pui, 0, lpszClassName, lpszWindowName, dwStyle | WS_CHILD, rect, pParentWnd, id, (LPVOID)pcreate);

   }


   bool interaction_impl::create_message_queue(::user::interaction * pui, const char * pszName)
   {

      if (IsWindow())
      {

         DestroyWindow();

      }

      if (!native_create_window_ex(pui, 0, NULL, pszName, WS_CHILD, null_rect(), HWND_MESSAGE, 0, NULL))
      {

         return false;

      }

      return true;

   }


   wstring interaction_impl::calc_window_class()
   {

      uint32_t uiStyle = m_pui->get_window_default_style();

      string strIcon = m_pui->get_window_icon_matter();

      if (strIcon.has_char())
      {

         return calc_icon_window_class(uiStyle, strIcon);

      }
      else
      {

         return get_user_interaction_window_class(m_pui);

      }

   }


   wstring interaction_impl::calc_icon_window_class(uint32_t dwDefaultStyle, const char * pszMatter)
   {

      string strPath = Application.dir().matter(pszMatter, "icon.ico");

      HICON hIcon = (HICON) ::LoadImageW(NULL, wstring(strPath), IMAGE_ICON, 256, 256, LR_LOADFROMFILE);

      wstring strClass = get_user_interaction_window_class(m_pui);

      if (hIcon != NULL)
      {


         // will fill lpszClassName with default WNDCLASS name
         // ignore instance handle from pre_create_window.

         WNDCLASSW wndcls;
         if (strClass.get_length() > 0 && GetClassInfoW(System.m_hinstance, strClass, &wndcls) && wndcls.hIcon != hIcon)
         {
            // register a very similar WNDCLASS
            return __register_window_class(get_app(), wndcls.style, wndcls.hCursor, wndcls.hbrBackground, hIcon);
         }
      }

      return strClass;

   }


   void interaction_impl::install_message_handling(::message::dispatch * pinterface)
   {

      last_install_message_handling(pinterface);
      ::user::interaction_impl::install_message_handling(pinterface);

      IGUI_WIN_MSG_LINK(WM_NCDESTROY, pinterface, this, &interaction_impl::_001OnNcDestroy);
      if (!m_pui->m_bMessageWindow)
      {
         IGUI_WIN_MSG_LINK(WM_PAINT, pinterface, this, &interaction_impl::_001OnPaint);
         IGUI_WIN_MSG_LINK(WM_PRINT, pinterface, this, &interaction_impl::_001OnPrint);
      }
      m_pui->install_message_handling(pinterface);
      IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &interaction_impl::_001OnCreate);
      if (!m_pui->m_bMessageWindow)
      {
         IGUI_WIN_MSG_LINK(WM_SETCURSOR, pinterface, this, &interaction_impl::_001OnSetCursor);
         IGUI_WIN_MSG_LINK(WM_ERASEBKGND, pinterface, this, &interaction_impl::_001OnEraseBkgnd);
         IGUI_WIN_MSG_LINK(WM_NCCALCSIZE, pinterface, this, &interaction_impl::_001OnNcCalcSize);
         IGUI_WIN_MSG_LINK(WM_SIZE, pinterface, this, &interaction_impl::_001OnSize);
         //IGUI_WIN_MSG_LINK(WM_WINDOWPOSCHANGING,pinterface,this,&interaction_impl::_001OnWindowPosChanging);
         //IGUI_WIN_MSG_LINK(WM_WINDOWPOSCHANGED,pinterface,this,&interaction_impl::_001OnWindowPosChanged);
         //IGUI_WIN_MSG_LINK(WM_GETMINMAXINFO,pinterface,this,&interaction_impl::_001OnGetMinMaxInfo);
         IGUI_WIN_MSG_LINK(WM_SETFOCUS, pinterface, this, &interaction_impl::_001OnSetFocus);
         IGUI_WIN_MSG_LINK(WM_KILLFOCUS, pinterface, this, &interaction_impl::_001OnKillFocus);

         prio_install_message_handling(pinterface);

      }

      IGUI_WIN_MSG_LINK(WM_DESTROY, pinterface, this, &interaction_impl::_001OnDestroy);

   }


   void interaction_impl::win_update_graphics()
   {

      single_lock sl(m_pui->m_pmutex, false);

      if (!sl.lock())
      {
         m_bUpdateGraphics = true;
         return;
      }

      m_bUpdateGraphics = false;

      rect rectWindow;

      GetWindowRect(rectWindow);

      m_pt = rectWindow.top_left();

      if (rectWindow.area() <= 0)
         return;

      if (m_size != rectWindow.size())
      {

         //if(m_spdib.is_null())
         //   m_spdib.alloc(allocer());

         //m_spdib->create(rectWindow.size());

         m_size = rectWindow.size();

      }




   }




   void interaction_impl::_001OnSize(signal_details * pobj)
   {

      SCAST_PTR(::message::size, psize, pobj);

      if (!(GetExStyle() & WS_EX_LAYERED))
      {

         m_pui->on_layout();

      }

      if (m_bIgnoreSizeEvent)
      {

         pobj->m_bRet = true;

         return;

      }





   }




   void interaction_impl::_001OnDestroy(signal_details * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

      ::multithreading::post_quit_and_wait(m_pthreadUpdateWindow, seconds(10));

      Default();

   }


   void interaction_impl::_001OnNcCalcSize(signal_details * pobj)
   {

      SCAST_PTR(::message::base, pbase, pobj);

      pbase->set_lresult(0);

      pobj->m_bRet = true;

   }


   // WM_NCDESTROY is the absolute LAST message sent.
   void interaction_impl::_001OnNcDestroy(signal_details * pobj)
   {

      single_lock sl(m_pui->m_pauraapp->m_pmutex, TRUE);

      //::window_sp pwindow;

      //if(m_pui->m_pauraapp != NULL)
      //{

      //   synch_lock sl(&m_pui->m_pauraapp->m_mutexUiPtra);

      //   if(m_pui->m_pauraapp->m_spuiptra.is_set())
      //   {

      //      m_pui->m_pauraapp->m_spuiptra->remove(m_pui);

      //   }

      //}



      pobj->m_bRet = true;
      // cleanup main and active windows
      ::thread* pThread = ::get_thread();
      if (pThread != NULL)
      {
         if (pThread->get_active_ui() == m_pui)
            pThread->set_active_ui(NULL);
      }

      // cleanup tooltip support
      if (m_pui != NULL)
      {
         if (m_pui->m_nFlags & WF_TOOLTIPS)
         {
         }
      }

      // call default, unsubclass, and detach from the map
      WNDPROC pfnWndProc = WNDPROC(::GetWindowLongPtr(get_handle(), GWLP_WNDPROC));
      Default();
      if (WNDPROC(::GetWindowLongPtr(get_handle(), GWLP_WNDPROC)) == pfnWndProc)
      {
         WNDPROC pfnSuper = *GetSuperWndProcAddr();
         if (pfnSuper != NULL)
            ::SetWindowLongPtr(get_handle(), GWLP_WNDPROC, reinterpret_cast<int_ptr>(pfnSuper));
      }

      detach();
      ASSERT(get_handle() == NULL);
      m_pfnDispatchWindowProc = &interaction_impl::_start_user_message_handler;
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
      catch (...)
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
      if (get_handle() != NULL)
         DestroyWindow();    // will call PostNcDestroy
      else
         PostNcDestroy();
   }

   void interaction_impl::assert_valid() const
   {
      if (((::windows::interaction_impl *)this)->get_handle() == NULL)
         return;     // null (unattached) windows are valid

      // check for special wnd??? values
      ASSERT(HWND_TOP == NULL);       // same as desktop
      if (((::windows::interaction_impl *)this)->get_handle() == HWND_BOTTOM)
      {
      }
      else if (((::windows::interaction_impl *)this)->get_handle() == HWND_TOPMOST)
      {
      }
      else if (((::windows::interaction_impl *)this)->get_handle() == HWND_NOTOPMOST)
      {
      }
      else
      {
         // should be a normal interaction_impl
         ASSERT(::IsWindow(((::windows::interaction_impl *)this)->get_handle()));

      }
   }


   void interaction_impl::dump(dump_context & dumpcontext) const
   {
      object::dump(dumpcontext);

      dumpcontext << "\nm_oswindow_ = " << ((::windows::interaction_impl *)this)->get_handle();

      if (((::windows::interaction_impl *)this)->get_handle() == NULL || ((::windows::interaction_impl *)this)->get_handle() == HWND_BOTTOM ||
         ((::windows::interaction_impl *)this)->get_handle() == HWND_TOPMOST || ((::windows::interaction_impl *)this)->get_handle() == HWND_NOTOPMOST)
      {
         // not a normal interaction_impl - nothing more to dump
         return;
      }

      if (!::IsWindow(((::windows::interaction_impl *)this)->get_handle()))
      {
         // not a valid interaction_impl
         dumpcontext << " (illegal oswindow)";
         return; // don't do anything more
      }

      ::window_sp pwindow = System.ui_from_handle(((::windows::interaction_impl *)this)->get_handle());
      if (pwindow.m_p != this)
         dumpcontext << " (Detached or temporary interaction_impl)";
      else
         dumpcontext << " (permanent interaction_impl)";

      // dump out interaction_impl specific statistics
      char szBuf[64];
      //if (!const_cast < interaction_impl * > (this)->send_message(WM_QUERYAFXWNDPROC, 0, 0) && pwindow == this)
      // ((::windows::interaction_impl *)this)->get_window_text(szBuf, _countof(szBuf));
      //else
      // ::DefWindowProc(((::windows::interaction_impl *)this)->get_handle(), WM_GETTEXT, _countof(szBuf), (LPARAM)&szBuf[0]);
      //dumpcontext << "\ncaption = \"" << szBuf << "\"";

      ::GetClassName(((::windows::interaction_impl *)this)->get_handle(), szBuf, _countof(szBuf));
      dumpcontext << "\nclass name = \"" << szBuf << "\"";

      rect rect;
      ((::windows::interaction_impl *) this)->GetWindowRect(&rect);
      dumpcontext << "\nrect = " << rect;
      dumpcontext << "\nparent ::window_sp = " << ((::windows::interaction_impl *)this)->GetParent();

      dumpcontext << "\nstyle = " << (uint_ptr)::GetWindowLong(((::windows::interaction_impl *)this)->get_handle(), GWL_STYLE);
      if (::GetWindowLong(((::windows::interaction_impl *)this)->get_handle(), GWL_STYLE) & WS_CHILD)
         dumpcontext << "\nid = " << __get_dialog_control_id(((::windows::interaction_impl *)this)->get_handle());

      dumpcontext << "\n";
   }

   void interaction_impl::on_set_parent(::user::interaction * pui)
   {
      if (pui != NULL)
      {
         detach();
      }
   }

   bool interaction_impl::DestroyWindow()
   {

      bool bResult = false;

      if (get_handle() != NULL)
         bResult = ::DestroyWindow(get_handle()) != FALSE;

      return bResult;

   }

   /////////////////////////////////////////////////////////////////////////////
   // Default interaction_impl implementation


   LRESULT interaction_impl::DefWindowProc(UINT nMsg, WPARAM wParam, lparam lParam)
   {

      if (get_handle() == NULL)
      {

         return 0;

      }

      if (m_pfnSuper != NULL)
      {

         return ::CallWindowProc(m_pfnSuper, get_handle(), nMsg, wParam, lParam);

      }

      WNDPROC pfnWndProc;

      if ((pfnWndProc = *GetSuperWndProcAddr()) == NULL)
      {

         return ::DefWindowProc(get_handle(), nMsg, wParam, lParam);

      }
      else
      {

         return ::CallWindowProc(pfnWndProc, get_handle(), nMsg, wParam, lParam);

      }

   }


   WNDPROC* interaction_impl::GetSuperWndProcAddr()
   {
      // Note: it is no longer necessary to override GetSuperWndProcAddr
      //  for each control class with a different WNDCLASS.
      //  This implementation now uses instance data, such that the previous
      //  WNDPROC can be anything.

      return &m_pfnSuper;
   }

   void interaction_impl::pre_translate_message(signal_details * pobj)
   {
      SCAST_PTR(::message::base, pbase, pobj);
      // no default processing

      if (pbase->m_uiMessage == WM_APP + 1933)
      {
         //ModifyStyleEx(WS_EX_LAYERED, 0);
         //ModifyStyleEx(0,WS_EX_LAYERED);
      }

   }



   int32_t interaction_impl::GetChildByIdText(int32_t nID, string & rString) const
   {
      ASSERT(::IsWindow(((::windows::interaction_impl *)this)->get_handle()));
      rString = "";    // is_empty without deallocating

      oswindow oswindow = ::GetDlgItem(((::windows::interaction_impl *)this)->get_handle(), nID);
      if (oswindow != NULL)
      {
         int32_t nLen = ::GetWindowTextLength(oswindow);
         ::GetWindowText(oswindow, rString.GetBufferSetLength(nLen), nLen + 1);
         rString.ReleaseBuffer();
      }

      return (int32_t)rString.get_length();
   }


   bool interaction_impl::GetWindowPlacement(WINDOWPLACEMENT* lpwndpl)
   {

      ASSERT(::IsWindow(get_handle()));

      lpwndpl->length = sizeof(WINDOWPLACEMENT);

      return ::GetWindowPlacement(get_handle(), lpwndpl) != FALSE;

   }


   bool interaction_impl::SetWindowPlacement(const WINDOWPLACEMENT* lpwndpl)
   {

      synch_lock sl(m_pui->m_pmutex);

      ASSERT(::IsWindow(get_handle()));

      ((WINDOWPLACEMENT*)lpwndpl)->length = sizeof(WINDOWPLACEMENT);

      return ::SetWindowPlacement(get_handle(), lpwndpl) != FALSE;

   }


   /////////////////////////////////////////////////////////////////////////////
   // interaction_impl will delegate owner draw messages to self drawing controls

   // Drawing: for all 4 control types
   void interaction_impl::OnDrawItem(int32_t /*nIDCtl*/, LPDRAWITEMSTRUCT lpDrawItemStruct)
   {

      // reflect notification to child interaction_impl control
      //      if (ReflectMessage(lpDrawItemStruct->hwndItem))
      //       return;     // eat it

      // not handled - do default
      Default();
   }

   // Drawing: for all 4 control types
   int32_t interaction_impl::OnCompareItem(int32_t /*nIDCtl*/, LPCOMPAREITEMSTRUCT lpCompareItemStruct)
   {
      // reflect notification to child interaction_impl control
      //      LRESULT lResult;
      //      if (ReflectLastMsg(lpCompareItemStruct->hwndItem, &lResult))
      //       return (int32_t)lResult;        // eat it

      // not handled - do default
      return (int32_t)Default();
   }

   void interaction_impl::OnDeleteItem(int32_t /*nIDCtl*/, LPDELETEITEMSTRUCT lpDeleteItemStruct)
   {
      // reflect notification to child interaction_impl control
      //if (ReflectLastMsg(lpDeleteItemStruct->hwndItem))
      // return;     // eat it
      // not handled - do default
      Default();
   }

   bool interaction_impl::_EnableToolTips(bool bEnable, UINT nFlag)
   {
      UNREFERENCED_PARAMETER(bEnable);
      UNREFERENCED_PARAMETER(nFlag);
      return FALSE;
   }


   // Measure item implementation relies on unique control/menu IDs
   void interaction_impl::OnMeasureItem(int32_t /*nIDCtl*/, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
   {
      if (lpMeasureItemStruct->CtlType == ODT_MENU)
      {
         ASSERT(lpMeasureItemStruct->CtlID == 0);
      }
      else
      {
         //         sp(::user::interaction) pChild = GetDescendantWindow(lpMeasureItemStruct->CtlID);
                  //         if (pChild != NULL && pChild->OnChildNotify(0, 0, 0, NULL))
                  //          return;     // eaten by child
      }
      // not handled - do default
      Default();
   }

   bool interaction_impl::GetWindowInfo(PWINDOWINFO pwi) const
   {
      ASSERT(::IsWindow(((interaction_impl *) this)->get_handle()));
      return ::GetWindowInfo(((interaction_impl *) this)->get_handle(), pwi) != FALSE;
   }

   ::window_sp interaction_impl::GetAncestor(UINT gaFlags) const
   {
      ASSERT(::IsWindow(((interaction_impl *) this)->get_handle()));
      return  System.ui_from_handle(::GetAncestor(((interaction_impl *) this)->get_handle(), gaFlags));
   }



   bool interaction_impl::GetScrollBarInfo(LONG idObject, PSCROLLBARINFO psbi) const
   {
      ASSERT(::IsWindow(((interaction_impl *) this)->get_handle()));
      ASSERT(psbi != NULL);
      return ::GetScrollBarInfo(((interaction_impl *) this)->get_handle(), idObject, psbi) != FALSE;
   }

   bool interaction_impl::GetTitleBarInfo(PTITLEBARINFO pti) const
   {
      ASSERT(::IsWindow(((interaction_impl *) this)->get_handle()));
      ASSERT(pti != NULL);
      return ::GetTitleBarInfo(((interaction_impl *) this)->get_handle(), pti) != FALSE;
   }

   bool interaction_impl::AnimateWindow(uint32_t dwTime, uint32_t dwFlags)
   {
      ASSERT(::IsWindow(get_handle()));
      return ::AnimateWindow(get_handle(), dwTime, dwFlags) != FALSE;
   }

   bool interaction_impl::FlashWindowEx(uint32_t dwFlags, UINT  uCount, uint32_t dwTimeout)
   {
      ASSERT(::IsWindow(get_handle()));
      FLASHWINFO fwi;
      fwi.cbSize = sizeof(fwi);
      fwi.hwnd = get_handle();
      fwi.dwFlags = dwFlags;
      fwi.uCount = uCount;
      fwi.dwTimeout = dwTimeout;

      return ::FlashWindowEx(&fwi) != FALSE;
   }



   bool interaction_impl::SetLayeredWindowAttributes(COLORREF crKey, BYTE bAlpha, uint32_t dwFlags)
   {
      ASSERT(::IsWindow(get_handle()));
      return ::SetLayeredWindowAttributes(get_handle(), crKey, bAlpha, dwFlags) != FALSE;
   }

   bool interaction_impl::UpdateLayeredWindow(::draw2d::graphics * pDCDst, POINT *pptDst, SIZE *psize,
      ::draw2d::graphics * pDCSrc, POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, uint32_t dwFlags)
   {
      ASSERT(::IsWindow(get_handle()));
      ::exception::throw_not_implemented(get_app());
      return false;
      /*      return ::UpdateLayeredWindow(get_handle(), WIN_HDC(pDCDst), pptDst, psize,
      WIN_HDC(pDCSrc), pptSrc, crKey, pblend, dwFlags) != FALSE;*/
   }


   bool interaction_impl::GetLayeredWindowAttributes(COLORREF *pcrKey, BYTE *pbAlpha, uint32_t *pdwFlags) const
   {
      ASSERT(::IsWindow(((interaction_impl *) this)->get_handle()));
      return ::GetLayeredWindowAttributes(((interaction_impl *) this)->get_handle(), pcrKey, pbAlpha, (LPDWORD)pdwFlags) != FALSE;
   }

   bool interaction_impl::PrintWindow(::draw2d::graphics * pgraphics, UINT nFlags) const
   {
      ASSERT(::IsWindow(((interaction_impl *) this)->get_handle()));
      ::exception::throw_not_implemented(get_app());
      //      return ::PrintWindow(get_handle(), (HDC)(dynamic_cast<::windows::graphics * >(pgraphics))->get_handle(), nFlags) != FALSE;
      return false;
   }


   LRESULT interaction_impl::OnNTCtlColor(WPARAM wParam, LPARAM lParam)
   {
      // fill in special struct for compatiblity with 16-bit WM_CTLCOLOR
      __CTLCOLOR ctl;
      ctl.hDC = (HDC)wParam;
      ctl.oswindow = (oswindow)lParam;
      return 0;
   }

   /////////////////////////////////////////////////////////////////////////////
   // interaction_impl extensions for help support

   void interaction_impl::WinHelp(uint_ptr dwData, UINT nCmd)
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
      // linux System.simple_message_box(__IDP_FAILED_TO_LAUNCH_HELP);
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
   // linux System.simple_message_box(__IDP_FAILED_TO_LAUNCH_HELP);
   System.simple_message_box("Failed to launch help");
   }*/
   //}

   void interaction_impl::PrepareForHelp()
   {

      sp(::user::interaction) pFrameWnd = m_pui;

      if (pFrameWnd.is_set())
      {

         // frame_window windows should be allowed to exit help mode first

         pFrameWnd->ExitHelpMode();

      }

      // cancel any tracking modes
      send_message(WM_CANCELMODE);
      SendMessageToDescendants(WM_CANCELMODE, 0, 0, TRUE, TRUE);

      // need to use top level parent (for the case where get_handle() is in DLL)
      sp(::user::interaction) pwindow = EnsureTopLevel();
      NODE_WINDOW(pwindow.m_p)->send_message(WM_CANCELMODE);
      NODE_WINDOW(pwindow.m_p)->SendMessageToDescendants(WM_CANCELMODE, 0, 0, TRUE, TRUE);

      // attempt to cancel capture
      oswindow oswindow_Capture = ::GetCapture();
      if (oswindow_Capture != NULL)
         ::SendMessage(oswindow_Capture, WM_CANCELMODE, 0, 0);
   }


   void interaction_impl::WinHelpInternal(uint_ptr dwData, UINT nCmd)
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
      if (command_target_interface::_001OnCmdMsg(pcmdmsg))
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
      //::CallWindowProc(*GetSuperWndProcAddr(), get_handle(), WM_PRINT, (WPARAM)((dynamic_cast<::windows::graphics * >(pgraphics))->get_handle()), (LPARAM)(PRF_CHILDREN | PRF_CLIENT));

   }

   /////////////////////////////////////////////////////////////////////////////
   // main message_handler implementation

   void interaction_impl::message_handler(signal_details * pobj)
   {

      SCAST_PTR(::message::base, pbase, pobj);

      //if(pbase->m_uiMessage == WM_MOUSEMOVE)
      //{
      //
      //   pbase->m_bRet = true;
      //   //pbase->set_lresult(1);
      //   // addictive human (camilo, you are classifying yourself human, you're kind respecting yourself, its a good sign...) profiling with PROFILE_MOVE_MANAGER at app_core_miau.vcxproj...

      //   return;


      //}


      bool bDestroying = m_pui->m_bDestroying;
      if (pbase->m_uiMessage == WM_ENABLE)
      {

         TRACE("WM_ENABLE enable = " + string(pbase->m_wparam ? "true" : "false"));

      }
      if (pbase->m_uiMessage == WM_SIZE || pbase->m_uiMessage == WM_MOVE)
      {

         //win_update_graphics();

      }

      if (pbase->m_uiMessage == WM_KEYDOWN ||
         pbase->m_uiMessage == WM_KEYUP ||
         pbase->m_uiMessage == WM_CHAR ||
         pbase->m_uiMessage == WM_SYSKEYDOWN ||
         pbase->m_uiMessage == WM_SYSKEYUP ||
         pbase->m_uiMessage == WM_SYSCHAR)
      {

         SCAST_PTR(::message::key, pkey, pobj);



         if (pbase->m_uiMessage == WM_KEYDOWN || pbase->m_uiMessage == WM_SYSKEYDOWN)
         {
            try
            {
               Session.set_key_pressed(pkey->m_ekey, true);
            }
            catch (...)
            {
            }
         }
         else if (pbase->m_uiMessage == WM_KEYUP || pbase->m_uiMessage == WM_SYSKEYUP)
         {
            try
            {
               Session.set_key_pressed(pkey->m_ekey, false);
            }
            catch (...)
            {
            }
         }
      }



      if (m_pui != NULL)
      {
         if (m_pui->WfiIsMoving())
         {
            //TRACE("moving: skip pre translate message");
         }
         else if (m_pui->WfiIsSizing())
         {
            //TRACE("sizing: skip pre translate message");
         }
         else
         {
            m_pui->pre_translate_message(pobj);
            if (pobj->m_bRet)
               return;

         }
      }

      if (pbase->m_uiMessage == WM_TIMER)
      {
         //         m_pui->m_pauraapp->step_timer();
      }
      else if (pbase->m_uiMessage == WM_LBUTTONDOWN)
      {
         ::rect rectClient;
         ::GetClientRect(get_handle(), rectClient);
         ::rect rectWindow;
         ::GetWindowRect(get_handle(), rectWindow);
         ::rect rectRegion;
         HRGN hrgn = CreateRectRgn(0, 0, 0, 0);
         int regionType = ::GetWindowRgn(get_handle(), hrgn);
         if (regionType != ERROR)
         {
            ::GetRgnBox(hrgn, rectRegion);
         }
         ::DeleteObject(hrgn); /* finished with region */
         WINDOWPLACEMENT wp;
         ZERO(wp);
         wp.length = sizeof(WINDOWPLACEMENT);
         ::GetWindowPlacement(get_handle(), &wp);
         bool bZoomed = ::IsZoomed(get_handle()) != FALSE;
         bool bIconic = ::IsIconic(get_handle()) != FALSE;
         Session.m_puiLastLButtonDown = m_pui;
      }
      else if(pbase->m_uiMessage == WM_MOUSEMOVE
         || pbase->m_uiMessage == WM_SETCURSOR
         || pbase->m_uiMessage == WM_NCMOUSEMOVE)
      {

      }
      else
      {
#ifdef DEBUG

         if (0)
         {
            switch (pbase->m_uiMessage)
            {
            case WM_CREATE:
               TRACE("WM_CREATE wparam=%08x lparam=%08x", pbase->m_wparam, pbase->m_lparam);
               break;
            case WM_WINDOWPOSCHANGING:
               TRACE("WM_WINDOWPOSCHANGING wparam=%08x lparam=%08x", pbase->m_wparam, pbase->m_lparam);
               break;
            case WM_WINDOWPOSCHANGED:
               TRACE("WM_WINDOWPOSCHANGED wparam=%08x lparam=%08x", pbase->m_wparam, pbase->m_lparam);
               break;
            case WM_ACTIVATE:
               TRACE("WM_ACTIVATE wparam=%08x lparam=%08x", pbase->m_wparam, pbase->m_lparam);
               break;
            case WM_ACTIVATEAPP:
               TRACE("WM_ACTIVATEAPP wparam=%08x lparam=%08x", pbase->m_wparam, pbase->m_lparam);
               break;
            case WM_MOUSEACTIVATE:
               TRACE("WM_MOUSEACTIVATE wparam=%08x lparam=%08x", pbase->m_wparam, pbase->m_lparam);
               break;
            case WM_NCACTIVATE:
               TRACE("WM_NCACTIVATE wparam=%08x lparam=%08x", pbase->m_wparam, pbase->m_lparam);
               break;
            case WM_SETFOCUS:
               TRACE("WM_SETFOCUS wparam=%08x lparam=%08x", pbase->m_wparam, pbase->m_lparam);
               break;
            case WM_KILLFOCUS:
               TRACE("WM_KILLFOCUS wparam=%08x lparam=%08x", pbase->m_wparam, pbase->m_lparam);
               break;
            case WM_MOVE:
               TRACE("WM_MOVE wparam=%08x lparam=%08x", pbase->m_wparam, pbase->m_lparam);
               break;
            case WM_SIZE:
               TRACE("WM_SIZE wparam=%08x lparam=%08x", pbase->m_wparam, pbase->m_lparam);
               break;
            default:
               TRACE("MESSAGE %08x wparam=%08x lparam=%08x", pbase->m_uiMessage, pbase->m_wparam, pbase->m_lparam);
               break;
            }

         }
         
#endif
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

      if (pbase->m_uiMessage == WM_MOUSELEAVE)
      {

         _000OnMouseLeave(pbase);

         return;

      }

      if (pbase->m_uiMessage == WM_LBUTTONDOWN ||
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

         if (m_bTranslateMouseMessageCursor && !pmouse->m_bTranslated)
         {
            pmouse->m_bTranslated = true;
            rect64 rectWindow;
            // People always ask for feature centric Flag/Macros...
            // ok, sometimes we attend them, besides it is easier to name the pigs...
            // (but I like pigs... and the porks... :-)
            if (m_bScreenRelativeMouseMessagePosition)
            {
               class rect rectWindow32;
               ::GetWindowRect(get_handle(), rectWindow32);
               ::copy(rectWindow, rectWindow32);
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

         if (pbase->m_uiMessage == WM_MOUSEMOVE)
         {
            // We are at the message_handler procedure.
            // mouse messages originated from message_handler and that are mouse move events should end up with the correct cursor.
            // So the procedure starts by setting to the default cursor,
            // what forces, at the end of message processing, setting the bergedge cursor to the default cursor, if no other
            // handler has set it to another one.
            pmouse->m_ecursor = visual::cursor_default;
         }
         else if (pbase->m_uiMessage == WM_NCMOUSEMOVE)
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

      if (pbase->m_uiMessage == MESSAGE_OLE_DRAGENTER ||
         pbase->m_uiMessage == MESSAGE_OLE_DRAGOVER ||
         pbase->m_uiMessage == MESSAGE_OLE_DRAGLEAVE ||
         pbase->m_uiMessage == MESSAGE_OLE_DRAGDROP)
      {

         message::drag_and_drop * pdrag = (::message::drag_and_drop *) pbase;


         user::oswindow_array oswindowa;
         user::interaction_spa wnda;
         wnda = *System.m_puiptraFrame;
         oswindowa = wnda.get_hwnda();
         user::window_util::SortByZOrder(oswindowa);
         for (int32_t i = 0; i < oswindowa.get_size(); i++)
         {
            sp(::user::interaction) pui = wnda.find_first(oswindowa[i]);
            if (pui != NULL)
            {
               pui->_000OnDrag(pdrag);
               if (pdrag->m_bRet)
                  return;
            }
         }
         return;
      }
      if (pbase->m_uiMessage == WM_KEYDOWN ||
         pbase->m_uiMessage == WM_KEYUP ||
         pbase->m_uiMessage == WM_CHAR ||
         pbase->m_uiMessage == WM_SYSKEYDOWN ||
         pbase->m_uiMessage == WM_SYSKEYUP ||
         pbase->m_uiMessage == WM_SYSCHAR)
      {

         message::key * pkey = (::message::key *) pbase;

         sp(::user::interaction) puiFocus = Session.get_keyboard_focus();

         if (puiFocus != NULL && puiFocus->IsWindow() && puiFocus != m_pui)
         {

            puiFocus->send(pkey);

            if (pbase->m_bRet)
               return;

         }
         else if (!pkey->m_bRet)
         {

            if (m_pui != NULL)
            {

               m_pui->_000OnKey(pkey);

               if (pbase->m_bRet)
                  return;

            }

         }

         pbase->set_lresult(DefWindowProc(pbase->m_uiMessage, pbase->m_wparam, pbase->m_lparam));

         return;

      }
      if (pbase->m_uiMessage == ::message::message_event)
      {
         m_pui->BaseOnControlEvent(pbase);
         return;
      }
      ::user::interaction_impl::message_handler(pobj);
      //if(pobj->m_bRet && !pbase->m_bDoSystemDefault)
      if (pobj->m_bRet)
         return;
      if (!bDestroying)
      {
         if (m_pui != NULL)
         {
            pbase->set_lresult(m_pui->DefWindowProc(pbase->m_uiMessage, pbase->m_wparam, pbase->m_lparam));
         }
         else
         {
            pbase->set_lresult(DefWindowProc(pbase->m_uiMessage, pbase->m_wparam, pbase->m_lparam));
         }
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
      if (ReflectMessage(oswindow_Ctrl, pbase))
         return true;        // eaten by child

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

      sp(::user::interaction)pWndTopLevel = EnsureTopLevel();

      return interaction_impl::GetForegroundWindow() == pWndTopLevel->GetLastActivePopup();
   }

   void interaction_impl::ActivateTopParent()
   {
      // special activate logic for floating toolbars and palettes
      ::window_sp pActiveWnd = GetForegroundWindow();
      if (pActiveWnd == NULL || !(NODE_WINDOW(pActiveWnd)->get_handle() == get_handle() || ::IsChild(NODE_WINDOW(pActiveWnd)->get_handle(), get_handle())))
      {
         // clicking on floating frame when it does not have
         // focus itself -- activate the toplevel frame instead.
         EnsureTopLevel()->SetForegroundWindow();
      }
   }



   int32_t interaction_impl::message_box(const char * lpszText, const char * lpszCaption, UINT nType)
   {
      if (lpszCaption == NULL)
         lpszCaption = Application.m_strAppName;
      int32_t nResult = ::simple_message_box(get_handle(), lpszText, lpszCaption, nType);
      return nResult;
   }

   ::user::interaction * interaction_impl::GetDescendantWindow(::user::interaction * pui, id id)
   {
      single_lock sl(pui->m_pauraapp->m_pmutex, TRUE);
      // get_child_by_id recursive (return first found)
      // breadth-first for 1 level, then depth-first for next level

      // use get_child_by_id since it is a fast USER function
      sp(::user::interaction) pWndChild;
      if ((pWndChild = pui->get_child_by_id(id)) != NULL)
      {
         if (pWndChild->GetTopWindow() != NULL)
         {
            // children with the same ID as their parent have priority
            pWndChild = GetDescendantWindow(pWndChild, id);
            if (pWndChild != NULL)
               return pWndChild;
         }
         if (pWndChild != NULL)
            return pWndChild;
      }

      for (int32_t i = 0; i < pui->m_uiptraChild.get_count(); i++)
      {
         if (pui->m_uiptraChild[i]->GetDlgCtrlId() == id)
         {
            if (pui->m_uiptraChild[i]->GetDescendantWindow(id))
               return pui->m_uiptraChild[i]->GetDescendantWindow(id);
            else
               return pui->m_uiptraChild[i];
         }
      }

      if (pWndChild == NULL)
         return NULL;

      // walk each child
      for (pWndChild = pWndChild->GetTopWindow(); pWndChild != NULL;
         pWndChild = pWndChild->get_next_window(GW_HWNDNEXT))
      {
         pWndChild = GetDescendantWindow(pWndChild, id);
         if (pWndChild != NULL)
            return pWndChild;
      }
      return NULL;    // not found
   }

   void interaction_impl::SendMessageToDescendants(oswindow oswindow, UINT message, WPARAM wParam, lparam lParam, bool bDeep, bool bOnlyPerm)
   {
      // walk through HWNDs to avoid creating temporary interaction_impl objects
      // unless we need to call this function recursively
      for (::oswindow oswindow_Child = ::GetTopWindow(oswindow); oswindow_Child != NULL; oswindow_Child = ::GetNextWindow(oswindow_Child, GW_HWNDNEXT))
      {
         // if bOnlyPerm is TRUE, don't send to non-permanent windows
         if (bOnlyPerm)
         {
            ::window_sp pwindow = System.ui_from_handle(oswindow_Child);
            if (pwindow != NULL)
            {
               // call interaction_impl proc directly since it is a C++ interaction_impl
               pwindow->call_message_handler(message, wParam, lParam);
            }
         }
         else
         {
            try
            {
               // send message with Windows SendMessage API
               ::SendMessage(oswindow_Child, message, wParam, lParam);
            }
            catch (...)
            {
            }
         }
         if (bDeep && ::GetTopWindow(oswindow_Child) != NULL)
         {
            // send to child windows after parent
            try
            {
               SendMessageToDescendants(oswindow_Child, message, wParam, lParam,
                  bDeep, bOnlyPerm);
            }
            catch (...)
            {
            }
         }
      }

   }

   /////////////////////////////////////////////////////////////////////////////
   // Scroll bar helpers
   //  hook for interaction_impl functions
   //    only works for derived class (eg: ::user::impact) that override 'GetScrollBarCtrl'
   // if the interaction_impl doesn't have a _visible_ windows scrollbar - then
   //   look for a sibling with the appropriate ID

   /*   CScrollBar* interaction_impl::GetScrollBarCtrl(int32_t) const
   {
   return NULL;        // no special scrollers supported
   }*/

   int32_t interaction_impl::SetScrollPos(int32_t nBar, int32_t nPos, bool bRedraw)
   {
      return ::SetScrollPos(get_handle(), nBar, nPos, bRedraw);
   }

   int32_t interaction_impl::GetScrollPos(int32_t nBar) const
   {
      return ::GetScrollPos(((::windows::interaction_impl *)this)->get_handle(), nBar);
   }

   void interaction_impl::SetScrollRange(int32_t nBar, int32_t nMinPos, int32_t nMaxPos, bool bRedraw)
   {
      ::SetScrollRange(get_handle(), nBar, nMinPos, nMaxPos, bRedraw);
   }

   void interaction_impl::GetScrollRange(int32_t nBar, LPINT lpMinPos, LPINT lpMaxPos) const
   {
      ::GetScrollRange(((::windows::interaction_impl *)this)->get_handle(), nBar, lpMinPos, lpMaxPos);
   }

   // Turn on/off non-control scrollbars
   //   for WS_?SCROLL scrollbars - show/hide them
   //   for control scrollbar - enable/disable them
   void interaction_impl::EnableScrollBarCtrl(int32_t nBar, bool bEnable)
   {
      // WS_?SCROLL scrollbar - show or hide
      ShowScrollBar(nBar, bEnable);
   }

   bool interaction_impl::SetScrollInfo(int32_t nBar, LPSCROLLINFO lpScrollInfo, bool bRedraw)
   {
      ASSERT(lpScrollInfo != NULL);

      oswindow oswindow = get_handle();
      lpScrollInfo->cbSize = sizeof(*lpScrollInfo);
      ::SetScrollInfo(oswindow, nBar, lpScrollInfo, bRedraw);
      return true;
   }

   bool interaction_impl::GetScrollInfo(int32_t nBar, LPSCROLLINFO lpScrollInfo, UINT nMask)
   {
      UNREFERENCED_PARAMETER(nMask);
      ASSERT(lpScrollInfo != NULL);

      oswindow oswindow = get_handle();
      return ::GetScrollInfo(oswindow, nBar, lpScrollInfo) != FALSE;
   }

   int32_t interaction_impl::GetScrollLimit(int32_t nBar)
   {
      int32_t nMin, nMax;
      GetScrollRange(nBar, &nMin, &nMax);
      SCROLLINFO info;
      if (GetScrollInfo(nBar, &info, SIF_PAGE))
      {
         nMax -= __max(info.nPage - 1, 0);
      }
      return nMax;
   }

   void interaction_impl::ScrollWindow(int32_t xAmount, int32_t yAmount,
      LPCRECT lpRect, LPCRECT lpClipRect)
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
      //   // not visible.  This leaves child windows unscrolled.
      //   // To ac::count for this oversight, the child windows are moved
      //   // directly instead.
      //   oswindow oswindow_Child = ::GetWindow(get_handle(),GW_CHILD);
      //   if(oswindow_Child != NULL)
      //   {
      //      for(; oswindow_Child != NULL;
      //            oswindow_Child = ::GetNextWindow(oswindow_Child,GW_HWNDNEXT))
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

   // move and resize all the windows at once!
   if (on_layout.hDWP == NULL || !::EndDeferWindowPos(on_layout.hDWP))
   TRACE(::aura::trace::category_AppMsg, 0, "Warning: DeferWindowPos failed - low system resources.\n");
   }

   */




   void interaction_impl::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType)
   {
      uint32_t dwExStyle = GetExStyle();
      if (nAdjustType == 0)
         dwExStyle &= ~WS_EX_CLIENTEDGE;
      ::AdjustWindowRectEx(lpClientRect, GetStyle(), FALSE, dwExStyle);
   }


   bool interaction_impl::HandleFloatingSysCommand(UINT nID, LPARAM lParam)
   {

      sp(::user::interaction) pParent = GetTopLevel();

      switch (nID & 0xfff0)
      {
      case SC_PREVWINDOW:
      case SC_NEXTWINDOW:
         if (LOWORD(lParam) == VK_F6 && pParent != NULL)
         {
            pParent->SetFocus();
            return true;
         }
         break;

      case SC_CLOSE:
      case SC_KEYMENU:
         // Check lParam.  If it is 0L, then the ::fontopus::user may have done
         // an Alt+Tab, so just ignore it.  This breaks the ability to
         // just press the Alt-key and have the first menu selected,
         // but this is minor compared to what happens in the Alt+Tab
         // case.
         if ((nID & 0xfff0) == SC_CLOSE || lParam != 0L)
         {
            if (pParent != NULL)
            {
               // Sending the above WM_SYSCOMMAND may destroy the cast,
               // so we have to be careful about restoring activation
               // and focus after sending it.
               oswindow oswindow_Save = get_handle();
               oswindow oswindow_Focus = ::GetFocus();
               pParent->SetActiveWindow();
               pParent->send_message(WM_SYSCOMMAND, nID, lParam);

               // be very careful here...
               if (::IsWindow(oswindow_Save))
                  ::SetActiveWindow(oswindow_Save);
               if (::IsWindow(oswindow_Focus))
                  ::SetFocus(oswindow_Focus);
            }
         }
         return true;
      }
      return false;
   }

   bool interaction_impl::ReflectMessage(oswindow oswindow_Child, ::message::base * pbase)
   {

      // check if in permanent map, if it is reflect it (could be OLE control)
      ::window_sp pwindow = System.ui_from_handle(oswindow_Child);
      ASSERT(pwindow == NULL || NODE_WINDOW(pwindow)->get_handle() == oswindow_Child);
      if (pwindow == NULL)
      {
         return FALSE;
      }

      // only OLE controls and permanent windows will get reflected msgs
      ASSERT(pwindow != NULL);
      return NODE_WINDOW(pwindow)->OnChildNotify(pbase);
   }

   bool interaction_impl::OnChildNotify(::message::base * pbase)
   {

      return ReflectChildNotify(pbase);
   }

   bool interaction_impl::ReflectChildNotify(::message::base * pbase)
   {

      // Note: reflected messages are send directly to interaction_impl::OnWndMsg
      //  and interaction_impl::_001OnCommand for speed and because these messages are not
      //  routed by normal _001OnCommand routing (they are only dispatched)

      switch (pbase->m_uiMessage)
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
         // reflect the message through the message map as WM_REFLECT_BASE+uMsg
         //return interaction_impl::OnWndMsg(WM_REFLECT_BASE+uMsg, wParam, lParam, pResult);
         return FALSE;

         // special case for WM_COMMAND
      case WM_COMMAND:
      {
         // reflect the message through the message map as OCM_COMMAND
         keep < bool > keepReflect(&pbase->m_bReflect, true, pbase->m_bReflect, true);
         if (interaction_impl::OnCommand(pbase))
         {
            pbase->m_bRet = true;
            return true;
         }
      }
      break;

      // special case for WM_NOTIFY
      case WM_NOTIFY:
      {
         // reflect the message through the message map as OCM_NOTIFY
         NMHDR* pNMHDR = (NMHDR*)pbase->m_lparam;
         //            int32_t nCode = pNMHDR->code;
         //            __NOTIFY notify;
         //          notify.pResult = pResult;
         //        notify.pNMHDR = pNMHDR;
         // xxxx         return interaction_impl::_001OnCommand(0, MAKELONG(nCode, WM_REFLECT_BASE+WM_NOTIFY), &notify, NULL);
      }

      // other special cases (WM_CTLCOLOR family)
      default:
         if (pbase->m_uiMessage >= WM_CTLCOLORMSGBOX && pbase->m_uiMessage <= WM_CTLCOLORSTATIC)
         {
            // fill in special struct for compatiblity with 16-bit WM_CTLCOLOR
            /*__CTLCOLOR ctl;
            ctl.hDC = (HDC)wParam;
            ctl.nCtlType = uMsg - WM_CTLCOLORMSGBOX;
            //ASSERT(ctl.nCtlType >= CTLCOLOR_MSGBOX);
            ASSERT(ctl.nCtlType <= CTLCOLOR_STATIC);

            // reflect the message through the message map as OCM_CTLCOLOR
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

   void interaction_impl::OnParentNotify(UINT message, LPARAM lParam)
   {
      if ((LOWORD(message) == WM_CREATE || LOWORD(message) == WM_DESTROY))
      {
         //if (ReflectMessage((oswindow) lParam))
         // return;     // eat it
      }
      // not handled - do default
      Default();
   }

   //void interaction_impl::_001OnSetFocus(::signal_details * pdetails)
   //{
   //
   //   //bool bHandled;

   //   //bHandled = FALSE;
   //   //if(!bHandled)
   //   //{
   //   //   Default();
   //   //}



   //}


   LRESULT interaction_impl::OnActivateTopLevel(WPARAM wParam, LPARAM)
   {

      return 0;

   }


   void interaction_impl::OnSysColorChange()
   {
      ::exception::throw_not_implemented(get_app());

      /*      application* pApp = &System;
      if (pApp != NULL && pApp->m_puiMain == this)
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

   void interaction_impl::OnSettingChange(UINT uFlags, const char * lpszSection)
   {
      UNUSED_ALWAYS(uFlags);
      UNUSED_ALWAYS(lpszSection);

      // force refresh of settings that we cache
      gen_GotScrollLines = FALSE;


      interaction_impl::OnDisplayChange(0, 0);    // to update system metrics, etc.
   }

   void interaction_impl::OnDevModeChange(__in LPTSTR lpDeviceName)
   {
      UNREFERENCED_PARAMETER(lpDeviceName);
      ::exception::throw_not_implemented(get_app());
      /*application* pApp = &System;
      if (pApp != NULL && pApp->m_puiMain == this)
      pApp->DevModeChange(lpDeviceName);

      // forward this message to all other child windows
      if (!(GetStyle() & WS_CHILD))
      {
      const MSG* pMsg = GetCurrentMessage();
      SendMessageToDescendants(pMsg->message, pMsg->wParam, pMsg->lParam,
      TRUE, TRUE);
      }*/
   }

   bool interaction_impl::OnHelpInfo(HELPINFO* /*pHelpInfo*/)
   {
      return Default() != 0;
   }

   LRESULT interaction_impl::OnDisplayChange(WPARAM wparam, LPARAM lparam)
   {

      // forward this message to all other child windows
      if (!(GetStyle() & WS_CHILD))
      {
         //         const MSG* pMsg = GetCurrentMessage();
         SendMessageToDescendants(WM_DISPLAYCHANGE, wparam, lparam, TRUE, TRUE);
      }

      return Default();
   }

   LRESULT interaction_impl::OnDragList(WPARAM, LPARAM lParam)
   {
      LPDRAGLISTINFO lpInfo = (LPDRAGLISTINFO)lParam;
      ASSERT(lpInfo != NULL);

      //      LRESULT lResult;
      //if (ReflectLastMsg(lpInfo->hWnd, &lResult))
      //   return (int32_t)lResult;    // eat it

      // not handled - do default
      return (int32_t)Default();
   }


   void interaction_impl::_001OnCreate(signal_details * pobj)
   {

      SCAST_PTR(::message::create, pcreate, pobj);

      {
         DEVMODE dm;
         if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm))
         {
            m_dFps = dm.dmDisplayFrequency;
         }

      }

      Default();

      if (m_pui->is_message_only_window()
         || dynamic_cast <::base::system_interaction_impl *>(m_pui) != NULL)
      {

         TRACE("good : opt out!");

      }
      else
      {

         if (m_pthreadUpdateWindow.is_null())
         {

            m_pthreadUpdateWindow = ::fork(get_app(), [&]()
            {

               ::multithreading::set_priority(::multithreading::priority_time_critical);

               HANDLE timer;

               LARGE_INTEGER li = {};

               timer = CreateWaitableTimer(NULL, TRUE, NULL);

               DWORD dwStart;

               double dFps = m_dFps;

               double dPeriod = (1000.0 / dFps);

               dPeriod = MIN(MAX(1.0, dPeriod), 1000.0);

               index iLastFrameId = millis_now() / dPeriod;

               index iFrameId;

               double_array daFrame;

               bool bUpdateScreen = false;

               while (::get_thread_run())
               {

                  dwStart = ::get_tick_count();

                  if (GetExStyle() & WS_EX_LAYERED)
                  {

                     if (m_pui == NULL)
                     {

                        break;

                     }

                     if (!m_pui->m_bLockWindowUpdate)
                     {

                        synch_lock sl(m_pui->m_pmutex);

                        if (m_pui->has_pending_graphical_update() || m_pui->check_need_layout())
                        {

                           sl.unlock();

                           _001UpdateBuffer();

                           try
                           {

                              m_pui->on_after_graphical_update();

                           }
                           catch (...)
                           {

                           }

                           bUpdateScreen = true;

                        }
                        else if (m_pui->check_need_translation() || m_pui->check_show_flags() || m_pui->check_need_zorder())
                        {

                           sl.unlock();

                           _001UpdateBuffer();

                           m_pui->on_after_graphical_update();

                        }

                        bUpdateScreen = true;

                     }

                  }
                  else if(::IsWindowVisible(get_handle()))
                  {

                     ::RedrawWindow(get_handle(), NULL, NULL, RDW_UPDATENOW | RDW_INVALIDATE);

                     bUpdateScreen = false;

                  }

                  double dNow = millis_now();

                  double dPeriod = (1000.0 / dFps);

                  dPeriod = MIN(MAX(1.0, dPeriod), 1000.0);

                  double dWait = dPeriod - fmod(dNow, dPeriod);

                  iFrameId = dNow / dPeriod;

                  ::count cLost = iFrameId - iLastFrameId - 1;

                  if (cLost < 0)
                  {

                     dWait = dPeriod;

                  }

                  iLastFrameId = iFrameId;

                  li.QuadPart = -((LONGLONG)(dWait * 1000.0) * 10LL);

                  if (!SetWaitableTimer(timer, &li, 0, NULL, NULL, FALSE))
                  {

                     Sleep(dWait);

                  }
                  else
                  {

                     WaitForSingleObject(timer, INFINITE);

                  }

                  if (bUpdateScreen)
                  {

                     _001UpdateScreen();

                     for (index i = 0; i < daFrame.get_size(); i++)
                     {

                        if (dNow - daFrame[i] >= 1000.0)
                        {

                           daFrame.remove_at(i);

                        }
                        else
                        {

                           break;

                        }

                     }

                  }

                  m_dUpdateScreenFps = daFrame.get_size();

                  daFrame.add(dNow);

               }

               CloseHandle(timer);

               m_pthreadUpdateWindow.release();

            });

         }

      }

      if (m_pui->m_bCreated)
      {

         pcreate->set_lresult(0);

         pcreate->m_bRet = true;

      }

   }

   /*   void interaction_impl::OnHScroll(UINT, UINT, CScrollBar* pScrollBar)
   {
   UNREFERENCED_PARAMETER(pScrollBar);
   Default();
   }

   void interaction_impl::OnVScroll(UINT, UINT, CScrollBar* pScrollBar)
   {
   UNREFERENCED_PARAMETER(pScrollBar);
   Default();
   }*/

   BOOL CALLBACK GetAppsEnumWindowsProc(oswindow oswindow, LPARAM lParam);


   BOOL CALLBACK GetAppsEnumWindowsProc(oswindow oswindow, LPARAM lParam)
   {

      user::oswindow_array * poswindowa = (user::oswindow_array *) lParam;

      poswindowa->add(oswindow);

      return TRUE;

   }

   void interaction_impl::get_app_wnda(user::oswindow_array & wnda)
   {
      EnumWindows(GetAppsEnumWindowsProc, (LPARAM)&wnda);
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
      oswindow m_oswindow;
      HDC m_hdc;

      print_window(::aura::application * papp, oswindow oswindow, HDC hdc, uint32_t dwTimeout) :
         ::object(papp),
         m_event(papp)
      {
         m_event.ResetEvent();
         m_oswindow = oswindow;
         m_hdc = hdc;
         __begin_thread(papp, &print_window::s_print_window, (LPVOID) this, ::multithreading::priority_above_normal);
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

            ::PrintWindow(pprintwindow->m_oswindow, pprintwindow->m_hdc, 0);

            ::SetEvent(hevent);

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

      //      int32_t iCount = wndaApp.get_count();

      try
      {

         if (get_window_long(GWL_EXSTYLE) & WS_EX_LAYERED)
         {
            rect rect5;
            rect rect9;

            rgnUpdate = ::CreateRectRgnIndirect(&rectUpdate);
            oswindow oswindowOrder = ::GetWindow(get_handle(), GW_HWNDNEXT);
            for (;;)
            {
               //            char szText[1024];
               //::GetWindowTextA(oswindowOrder, szText, sizeof(szText));
               if (oswindowOrder == NULL || !::IsWindow(oswindowOrder))
                  break;

               if (!::IsWindowVisible(oswindowOrder) || ::IsIconic(oswindowOrder) || oswindowOrder == get_handle() || wndaApp.contains(oswindowOrder))
               {

                  if (oswindowOrder == get_handle())
                  {

                     // add as bookmark - doesn't paint it
                     wndaApp.add(oswindowOrder);

                  }

               }
               else
               {

                  rect rectWindow;

                  ::GetWindowRect(oswindowOrder, rectWindow);

                  SetRectRgn(rgnWindow, rectWindow.left, rectWindow.top, rectWindow.right, rectWindow.bottom);

                  SetRectRgn(rgnIntersect, 0, 0, 0, 0);

                  CombineRgn(rgnIntersect, rgnUpdate, rgnWindow, RGN_AND);

                  rect rectIntersectBox;

                  GetRgnBox(rgnIntersect, rectIntersectBox);

                  if (rectIntersectBox.is_empty())
                  {

                  }
                  else
                  {

                     CombineRgn(rgnUpdate, rgnUpdate, rgnWindow, ::draw2d::region::combine_exclude);

                     rect rectDiffBox;

                     GetRgnBox(rgnUpdate, rectDiffBox);

                     wndaApp.add(oswindowOrder);

                     if (rectDiffBox.is_empty())
                     {

                        break;

                     }

                  }

               }

               oswindowOrder = ::GetWindow(oswindowOrder, GW_HWNDNEXT);

            }

            for (index j = wndaApp.get_upper_bound(); j >= 0; j--)
            {

               oswindow oswindow = wndaApp[j];

               if (oswindow == get_handle())
                  break;

               if (!::IsWindowVisible(oswindow) || ::IsIconic(oswindow))
                  continue;

               ::GetWindowRect(oswindow, rect5);

               rect9.intersect(rect5, rectUpdate);

               if (rect9.width() > 0 && rect9.height() > 0)
               {

                  /*::window_sp pwnd = (interaction_impl::FromHandlePermanent);

                  if(pwnd == NULL)
                  {

                  for(int32_t l = 0; l < wndpa.get_count(); l++)
                  {
                  if(wndpa[l]->get_safe_handle() == oswindow)
                  {
                  pwnd = (wndpa[l]->m_pimpl);
                  break;
                  }
                  }
                  }
                  if(pwnd != NULL)
                  {
                  pwnd->_001Print(pgraphics);
                  }*/

                  //if(::GetWindowLong(wndaApp[j], GWL_EXSTYLE) & WS_EX_LAYERED)

                  if (true)
                  {

                     HDC hDCMem = CreateCompatibleDC(NULL);

                     HBITMAP hBmp = NULL;

                     {

                        HDC hDC = ::GetWindowDC(oswindow);

                        hBmp = CreateCompatibleBitmap(hDC, rect5.width(), rect5.height());

                        ::ReleaseDC(oswindow, hDC);

                     }

                     HGDIOBJ hOld = SelectObject(hDCMem, hBmp);

                     //print_window printwindow(get_app(), oswindow, hDCMem, 284);

                     ::PrintWindow(oswindow, hDCMem, 0);

                     ::BitBlt(
                        hdc,
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

                     if (hdcWindow == NULL)
                        hdcWindow = ::GetDCEx(wndaApp[j], NULL, DCX_WINDOW | DCX_CACHE);

                     if (hdcWindow != NULL)
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
      catch (...)
      {

      }

      ::DeleteObject(rgnWindow);
      ::DeleteObject(rgnIntersect);
      ::DeleteObject(rgnUpdate);
   }


   void interaction_impl::_001OnProdevianSynch(signal_details * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

      //      System.get_event(m_pui->m_pthread)->SetEvent();

      //    System.get_event(System.get_twf())->wait(millis(8400));

      //ModifyStyleEx(WS_EX_LAYERED,0);
      //ModifyStyleEx(0,WS_EX_LAYERED);

   }


   void interaction_impl::_001OnPaint(signal_details * pobj)
   {

      SCAST_PTR(::message::base, pbase, pobj);

      rect64 rectWindow;

      m_pui->GetWindowRect(rectWindow);

      PAINTSTRUCT paint;

      memset(&paint, 0, sizeof(paint));

      HDC hdc = ::BeginPaint(get_handle(), &paint);

      ::SelectClipRgn(hdc, NULL);

      rect rectPaint;

      rect rectUpdate;

      rectPaint = paint.rcPaint;

      if (rectPaint.is_null() || (GetExStyle() & WS_EX_LAYERED))
      {

         rectUpdate = rectWindow;

         rectPaint = rectWindow;

         ScreenToClient(rectPaint);

      }
      else
      {

         rectUpdate = rectPaint;

      }

      if (m_spgraphics.is_null())
      {

         update_graphics_resources();

      }

      if (m_spgraphics.is_set())
      {

         ::draw2d::graphics * pgraphics = m_spgraphics->on_begin_draw();

         if (pgraphics != NULL)
         {

            try
            {

               _001Print(pgraphics);

            }
            catch (...)
            {


            }

            ::draw2d::graphics_sp g(allocer());

            try
            {

               if (pgraphics != NULL && g->Attach(hdc))
               {

                  pgraphics->SetViewportOrg(0, 0);

                  g->BitBlt(rectPaint.left, rectPaint.top, rectPaint.width(), rectPaint.height(), pgraphics, rectUpdate.left, rectUpdate.top, SRCCOPY);

                  g->Detach();

               }

            }
            catch (...)
            {

            }

         }

      }

      ::EndPaint(get_handle(), &paint);

      pobj->m_bRet = true;

      pbase->set_lresult(0);

   }


   void interaction_impl::_001OnPrint(signal_details * pobj)
   {

      //      if(m_spdib.is_null())
            //       m_spdib.alloc(allocer());

            //m_spdib->print_window(this,pobj);

            //m_spdibBuffer->print_window(this, pobj);

   }


   void interaction_impl::OnEnterIdle(UINT /*nWhy*/, ::window_sp /*pWho*/)
   {
      // In some OLE inplace active scenarios, OLE will post a
      // message instead of sending it.  This causes so many WM_ENTERIDLE
      // messages to be sent that tasks running in the background stop
      // running.  By dispatching the pending WM_ENTERIDLE messages
      // when the first one is received, we trick Windows into thinking
      // that only one was really sent and dispatched.
      {
         MSG msg;
         while (PeekMessage(&msg, NULL, WM_ENTERIDLE, WM_ENTERIDLE, PM_REMOVE))
            DispatchMessage(&msg);
      }

      Default();
   }

   HBRUSH interaction_impl::OnCtlColor(::draw2d::graphics *, ::window_sp pwindow, UINT)
   {
      ASSERT(pwindow != NULL && NODE_WINDOW(pwindow)->get_handle() != NULL);
      //      LRESULT lResult;
      //if (NODE_WINDOW(pwindow)->OnChildNotify(&lResult))
      //   return (HBRUSH)lResult;     // eat it
      return (HBRUSH)Default();
   }

   // implementation of OnCtlColor for default gray backgrounds
   //   (works for any interaction_impl containing controls)
   //  return value of FALSE means caller must call DefWindowProc's default
   //  TRUE means that 'hbrGray' will be used and the appropriate text
   //    ('clrText') and background colors are set.
   bool interaction_impl::GrayCtlColor(HDC hDC, oswindow oswindow, UINT nCtlColor, HBRUSH hbrGray, COLORREF clrText)
   {
      if (hDC == NULL)
      {
         // sometimes Win32 passes a NULL hDC in the WM_CTLCOLOR message.
         //         TRACE(::aura::trace::category_AppMsg, 0, "Warning: hDC is NULL in interaction_impl::GrayCtlColor; WM_CTLCOLOR not processed.\n");
         return FALSE;
      }

      if (hbrGray == NULL ||
         nCtlColor == CTLCOLOR_EDIT || nCtlColor == CTLCOLOR_MSGBOX ||
         nCtlColor == CTLCOLOR_SCROLLBAR)
      {
         return FALSE;
      }

      if (nCtlColor == CTLCOLOR_LISTBOX)
      {
         // only handle requests to draw the space between edit and drop button
         //  in a drop-down combo (not a drop-down list)
         if (!__is_combo_box_control(oswindow, (UINT)CBS_DROPDOWN))
            return FALSE;
      }

      // set background color and return handle to brush
      LOGBRUSH logbrush;
      VERIFY(::GetObject(hbrGray, sizeof(LOGBRUSH), (LPVOID)&logbrush));
      ::SetBkColor(hDC, logbrush.lbColor);
      if (clrText == (COLORREF)-1)
         clrText = Session.get_default_color(COLOR_WINDOWTEXT);  // normal text
      ::SetTextColor(hDC, clrText);
      return TRUE;
   }


   void interaction_impl::CenterWindow(::user::interaction * pAlternateOwner)
   {
      //ASSERT(::IsWindow(get_handle()));

      //// determine owner interaction_impl to center against
      //uint32_t dwStyle = GetStyle();
      //sp(::user::interaction) oswindow_Center = pAlternateOwner;
      //if(pAlternateOwner == NULL)
      //{
      //   if(dwStyle & WS_CHILD)
      //      oswindow_Center = GetParent();
      //   else
      //      oswindow_Center = GetWindow(GW_OWNER);
      //   if(oswindow_Center != NULL)
      //   {
      //      // let parent determine alternate center interaction_impl
      //      sp(::user::interaction) oswindow_Temp((lparam)oswindow_Center->send_message(WM_QUERYCENTERWND));
      //      if(oswindow_Temp != NULL)
      //         oswindow_Center = oswindow_Temp;
      //   }
      //}

      //// get coordinates of the interaction_impl relative to its parent
      //rect rcDlg;
      //GetWindowRect(&rcDlg);
      //rect rcArea;
      //rect rcCenter;
      //sp(::user::interaction) oswindow_Parent;
      //if(!(dwStyle & WS_CHILD))
      //{
      //   // don't center against invisible or minimized windows
      //   if(oswindow_Center != NULL)
      //   {
      //      uint32_t dwAlternateStyle = oswindow_Center->get_window_long(GWL_STYLE);
      //      if(!(dwAlternateStyle & WS_VISIBLE) || (dwAlternateStyle & WS_MINIMIZE))
      //         oswindow_Center = NULL;
      //   }

      //   /*
      //   MONITORINFO mi;
      //   mi.cbSize = sizeof(mi);

      //   // center within appropriate monitor coordinates
      //   if(oswindow_Center == NULL)
      //   {

      //   :: GetMonitorInfo(::MonitorFromWindow(hwDefault,MONITOR_DEFAULTTOPRIMARY),&mi);
      //   rcCenter = mi.rcWork;
      //   rcArea = mi.rcWork;
      //   }
      //   else
      //   {
      //   oswindow_Center->GetWindowRect(&rcCenter);
      //   ::GetMonitorInfo(::MonitorFromWindow(oswindow_Center->get_handle(),MONITOR_DEFAULTTONEAREST),&mi);
      //   rcArea = mi.rcWork;
      //   }
      //   */
      //}
      //else
      //{
      //   // center within parent client coordinates
      //   oswindow_Parent = GetParent();
      //   ASSERT(oswindow_Parent->IsWindow());

      //   oswindow_Parent->GetClientRect(&rcArea);
      //   ASSERT(oswindow_Center->IsWindow());
      //   oswindow_Center->GetClientRect(&rcCenter);
      //   ::MapWindowPoints(oswindow_Center->get_handle(),oswindow_Parent->get_handle(),(POINT*)&rcCenter,2);
      //}

      //// find dialog's upper left based on rcCenter
      //int32_t xLeft = (rcCenter.left + rcCenter.right) / 2 - rcDlg.width() / 2;
      //int32_t yTop = (rcCenter.top + rcCenter.bottom) / 2 - rcDlg.height() / 2;

      //// if the dialog is outside the screen, move it inside
      //if(xLeft < rcArea.left)
      //   xLeft = rcArea.left;
      //else if(xLeft + rcDlg.width() > rcArea.right)
      //   xLeft = rcArea.right - rcDlg.width();

      //if(yTop < rcArea.top)
      //   yTop = rcArea.top;
      //else if(yTop + rcDlg.height() > rcArea.bottom)
      //   yTop = rcArea.bottom - rcDlg.height();

      //// map screen coordinates to child coordinates
      //SetWindowPos(0,xLeft,yTop,-1,-1,
      //             SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
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
         //         HINSTANCE hInst = ::core::FindResourceHandle(lpszResourceName, RT_DLGINIT);
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
         UnlockResource(hResource);
         FreeResource(hResource);
      }
      return bResult;
   }

   bool interaction_impl::ExecuteDlgInit(LPVOID lpResource)
   {
      bool bSuccess = TRUE;
      if (lpResource != NULL)
      {
         UNALIGNED WORD* lpnRes = (WORD*)lpResource;
         while (bSuccess && *lpnRes != 0)
         {
            WORD nIDC = *lpnRes++;
            WORD nMsg = *lpnRes++;
            DWORD dwLen = *((UNALIGNED DWORD*&)lpnRes)++;

            // In Win32 the WM_ messages have changed.  They have
            // to be translated from the 32-bit values to 16-bit
            // values here.

#define WIN16_LB_ADDSTRING  0x0401
#define WIN16_CB_ADDSTRING  0x0403
#define __CB_ADDSTRING   0x1234

            // unfortunately, WIN16_CB_ADDSTRING == CBEM_INSERTITEM
            if (nMsg == __CB_ADDSTRING)
               nMsg = CBEM_INSERTITEM;
            else if (nMsg == WIN16_LB_ADDSTRING)
               nMsg = LB_ADDSTRING;
            else if (nMsg == WIN16_CB_ADDSTRING)
               nMsg = CB_ADDSTRING;

            // check for invalid/unknown message types
            ASSERT(nMsg == LB_ADDSTRING || nMsg == CB_ADDSTRING ||
               nMsg == CBEM_INSERTITEM);

#ifdef DEBUG
            // For AddStrings, the ::count must exactly delimit the
            // string, including the NULL termination.  This check
            // will not catch all mal-formed ADDSTRINGs, but will
            // catch some.
            if (nMsg == LB_ADDSTRING || nMsg == CB_ADDSTRING || nMsg == CBEM_INSERTITEM)
               ASSERT(*((LPBYTE)lpnRes + (UINT)dwLen - 1) == 0);
#endif

            if (nMsg == CBEM_INSERTITEM)
            {
               COMBOBOXEXITEM item = { 0 };
               item.mask = CBEIF_TEXT;
               item.iItem = -1;
               string strText(reinterpret_cast<LPTSTR>(lpnRes));
               item.pszText = const_cast<LPTSTR>(strText.GetString());
               if (::SendDlgItemMessage(get_handle(), nIDC, nMsg, 0, (LPARAM)&item) == -1)
                  bSuccess = FALSE;
            }
            {
               // List/Combobox returns -1 for error
               if (::SendDlgItemMessageA(get_handle(), nIDC, nMsg, 0, (LPARAM)lpnRes) == -1)
                  bSuccess = FALSE;
            }


            // skip past data
            lpnRes = (WORD*)((LPBYTE)lpnRes + (UINT)dwLen);
         }
      }

      // send update message to all controls after all other siblings loaded
      if (bSuccess)
         SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, FALSE, FALSE);

      return bSuccess;
   }

   void interaction_impl::UpdateDialogControls(command_target* pTarget, bool bDisableIfNoHndler)
   {
      UNREFERENCED_PARAMETER(pTarget);
      UNREFERENCED_PARAMETER(bDisableIfNoHndler);
      cmd_ui state(get_app());
      interaction_impl wndTemp;       // very temporary interaction_impl just for CmdUI update


   }


   bool interaction_impl::ContinueModal(int32_t iLevel)
   {

      return iLevel < m_pui->m_iModalCount;

   }


   void interaction_impl::EndModalLoop(id nResult)
   {

      ASSERT(::IsWindow(get_handle()));

      // this result will be returned from interaction_impl::RunModalLoop
      m_pui->m_nModalResult = (int32_t)nResult;

      // make sure a message goes through to exit the modal loop
      if (m_pui->m_iModalCount > 0)
      {

         m_pui->m_iModalCount--;

         for (index i = 0; i < m_pui->m_threadptraModal.get_count(); i++)
         {

            m_pui->m_threadptraModal[i]->kick_thread();

         }

         m_pui->kick_queue();

         get_thread()->kick_thread();

      }

   }


   void interaction_impl::EndAllModalLoops(id nResult)
   {

      ASSERT(::IsWindow(get_handle()));

      // this result will be returned from interaction_impl::RunModalLoop
      m_pui->m_idModalResult = nResult;

      // make sure a message goes through to exit the modal loop
      if (m_pui->m_iModalCount > 0)
      {

         int32_t iLevel = m_pui->m_iModalCount - 1;

         m_pui->m_iModalCount = 0;

         m_pui->kick_queue();

         get_thread()->kick_thread();

         for (int32_t i = iLevel; i >= 0; i--)
         {

            ::thread * pthread = oprop(string("RunModalLoop.thread(") + ::str::from(i) + ")").cast < ::thread >();

            try
            {

               pthread->kick_thread();

            }
            catch (...)
            {

            }

         }

      }

   }


   bool interaction_impl::subclass_window(oswindow oswindow)
   {

      if (!attach(oswindow))
         return FALSE;

      // allow any other subclassing to occur
      pre_subclass_window();

      m_pfnSuper = (WNDPROC)::GetWindowLongPtr(oswindow, GWLP_WNDPROC);

      // now hook into the AFX WndProc
      WNDPROC* lplpfn = GetSuperWndProcAddr();
      WNDPROC oldWndProc = (WNDPROC)::SetWindowLongPtr(oswindow, GWLP_WNDPROC, (int_ptr)__get_window_procedure());
      ASSERT(oldWndProc != __get_window_procedure());

      if (*lplpfn == NULL)
         *lplpfn = oldWndProc;   // the first control of that type created
#ifdef DEBUG
      else if (*lplpfn != oldWndProc)
      {

         TRACE(::aura::trace::category_AppMsg, 0, "p: Trying to use subclass_window with incorrect interaction_impl\n");
         TRACE(::aura::trace::category_AppMsg, 0, "\tderived class.\n");
         TRACE(::aura::trace::category_AppMsg, 0, "\toswindow_ = $%08X (nIDC=$%08X) is not a %hs.\n", (UINT)(uint_ptr)oswindow, __get_dialog_control_id(oswindow), typeid(*this).name());

         ASSERT(FALSE);

         // undo the subclassing if continuing after assert

         ::SetWindowLongPtr(oswindow, GWLP_WNDPROC, (int_ptr)oldWndProc);

      }
#endif

      message::size size(get_app());

      _001OnSize(&size);

      return true;

   }


   bool interaction_impl::SubclassDlgItem(UINT nID, ::window_sp pParent)
   {

      ASSERT(pParent != NULL);

      ASSERT(::IsWindow(NODE_WINDOW(pParent)->get_handle()));

      // check for normal dialog control first
      oswindow oswindow_Control = ::GetDlgItem(NODE_WINDOW(pParent)->get_handle(), nID);
      if (oswindow_Control != NULL)
         return subclass_window(oswindow_Control);


      return false;   // control not found
   }


   oswindow interaction_impl::unsubclass_window()
   {
      //ASSERT(::IsWindow(get_handle()));

      if (!IsWindow())
         return NULL;

      // set WNDPROC back to original value
      WNDPROC* lplpfn = GetSuperWndProcAddr();
      ::SetWindowLongPtr(get_handle(), GWLP_WNDPROC, (int_ptr)*lplpfn);
      *lplpfn = NULL;

      // and detach the oswindow from the interaction_impl object
      return detach();
   }


   /*   ::user::view_update_hint::user::view_update_hint(::aura::application * papp) :
   ::object(papp)
   {
   }
   */


   /*

   bool interaction_impl::IsChild(const ::user::interaction * pwindow) const
   {

   ASSERT(::IsWindow(get_handle()));

   if(pwindow->get_handle() == NULL)
   {
   return ::user::interaction_impl_base::IsChild(pwindow);
   }
   else
   {
   return ::IsChild(get_handle(),pwindow->get_handle()) != FALSE;
   }

   }

   */


   bool interaction_impl::IsWindow() const
   {

      return ::IsWindow(get_handle()) != FALSE;

   }


   void interaction_impl::on_layout()
   {

      ::rect64 rectOld = m_rectParentClient;

      ::rect64 rectNew = m_rectParentClientRequest;

      m_rectParentClient = m_rectParentClientRequest;

      if (rectOld.top_left() != rectNew.top_left())
      {

         m_pui->message_call(WM_MOVE);

         sp(::user::interaction) pui;

         while (m_pui->get_child(pui))
         {

            pui->on_translate();

         }

      }

      if (rectOld.size() != rectNew.size())
      {

         m_pui->message_call(WM_SIZE);

      }

      if (rectNew != rectOld)
      {

         m_pui->m_dwLastSizeMove = ::get_tick_count();

         m_pui->m_bSizeMove = true;

      }

   }


   void interaction_impl::clear_need_layout()
   {

      m_pui->m_bNeedLayout = false;

   }


   void interaction_impl::on_translate()
   {

      ::rect64 rectOld = m_rectParentClient;

      ::rect64 rectNew = m_rectParentClientRequest;

      m_rectParentClient = m_rectParentClientRequest;

      if (rectOld.top_left() != rectNew.top_left())
      {

         m_pui->message_call(WM_MOVE);

         sp(::user::interaction) pui;

         while (m_pui->get_child(pui))
         {

            pui->on_translate();

         }

      }

      if (rectOld.size() != rectNew.size())
      {

         m_pui->message_call(WM_SIZE);

      }

      if (rectNew != rectOld)
      {

         m_pui->m_dwLastSizeMove = ::get_tick_count();

         m_pui->m_bSizeMove = true;

      }

   }


   void interaction_impl::clear_need_translation()
   {

      m_pui->m_bNeedLayout = false;

   }


   void interaction_impl::on_do_show_flags()
   {

      ::rect64 rectOld = m_rectParentClient;

      ::rect64 rectNew = m_rectParentClientRequest;

      m_rectParentClient = m_rectParentClientRequest;

      if (rectOld.top_left() != rectNew.top_left())
      {

         m_pui->message_call(WM_MOVE);

         sp(::user::interaction) pui;

         while (m_pui->get_child(pui))
         {

            pui->on_translate();

         }

      }

      if (rectOld.size() != rectNew.size())
      {

         m_pui->message_call(WM_SIZE);

      }

      if (rectNew != rectOld)
      {

         m_pui->m_dwLastSizeMove = ::get_tick_count();

         m_pui->m_bSizeMove = true;

      }

      //if (!(GetExStyle() & WS_EX_LAYERED))
      //{
      //   
      //   if (m_bShowFlags && (m_iShowFlags & SWP_SHOWWINDOW))
      //   {

      //      ShowWindow(SW_SHOW);

      //   }
      //   else if (m_bShowFlags && (m_iShowFlags & SWP_HIDEWINDOW))
      //   {

      //      ShowWindow(SW_HIDE);

      //   }

      //}

   }


   void interaction_impl::clear_show_flags()
   {

      m_pui->m_bNeedLayout = false;

   }


   void interaction_impl::on_zorder()
   {

   }


   void interaction_impl::clear_need_zorder()
   {

   }


   bool interaction_impl::ClientToScreen(LPRECT lprect)
   {

      class rect64 rectWindow;

      ::copy(rectWindow, lprect);

      if (!ClientToScreen(rectWindow))
      {

         return false;

      }

      ::copy(lprect, rectWindow);

      return true;

   }


   bool interaction_impl::ClientToScreen(LPPOINT lppoint)
   {

      class point64 pt;

      ::copy(pt, lppoint);

      if (!ClientToScreen(pt))
      {

         return false;

      }

      ::copy(lppoint, pt);

      return true;

   }


   bool interaction_impl::ClientToScreen(RECT64 * lprect)
   {

      class rect rectWindow;

      if (!GetWindowRect(rectWindow))
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

      if (!m_pui->GetWindowRect(rectWindow))
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

      ::copy(rectWindow, lprect);

      if (!ScreenToClient(rectWindow))
      {

         return false;

      }

      ::copy(lprect, rectWindow);

      return true;

   }


   bool interaction_impl::ScreenToClient(LPPOINT lppoint)
   {

      class point64 pt;

      ::copy(pt, lppoint);

      if (!ScreenToClient(pt))
      {

         return false;

      }

      ::copy(lppoint, pt);

      return true;

   }


   bool interaction_impl::ScreenToClient(RECT64 * lprect)
   {

      class rect64 rectWindow;

      if (!m_pui->GetWindowRect(rectWindow))
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

      if (!m_pui->GetWindowRect(rectWindow))
      {

         return false;

      }

      lppoint->x -= rectWindow.left;
      lppoint->y -= rectWindow.top;

      return true;

   }


   bool interaction_impl::GetWindowRect(RECT64 * lprect)
   {

      if (!::IsWindow(get_handle()))
      {

         return false;

      }


      *lprect = m_rectParentClientRequest;

      if (GetParent() != NULL)
      {

         if (!GetParent()->ClientToScreen(lprect))
         {

            return false;

         }

      }

      return true;

   }


   bool interaction_impl::GetClientRect(RECT64 * lprect)
   {

      oswindow handle = get_handle();

      if (!::IsWindow(get_handle()))
      {

         return false;

      }

      rect rect32;

      if (!(GetExStyle() & WS_EX_LAYERED))
      {

         if (!::GetWindowRect(get_handle(), rect32))
         {

            return false;

         }

         if (GetParent() != NULL)
         {

            if (!GetParent()->ScreenToClient(rect32))
            {

               return false;

            }

         }

         ::copy(m_rectParentClient, rect32);

      }

      rect32 = m_rectParentClient;

      rect32.offset(-rect32.top_left());

      ::copy(lprect, rect32);

      return true;

   }


   void interaction_impl::_001WindowMinimize(bool bNoActivate)
   {

      interaction_impl_base::_001WindowMinimize(bNoActivate);

   }


   void interaction_impl::_001WindowMaximize()
   {

      interaction_impl_base::_001WindowMaximize();

   }


   void interaction_impl::_001WindowFullScreen()
   {

      interaction_impl_base::_001WindowFullScreen();

   }


   void interaction_impl::_001WindowRestore()
   {

      interaction_impl_base::_001WindowRestore();

   }


   bool interaction_impl::ShowWindow(int32_t nCmdShow)
   {

      if (!::IsWindow(get_handle()))
         return false;

      if (nCmdShow == SW_MAXIMIZE)
      {

         m_pui->_001WindowMaximize();

      }
      else if (nCmdShow == SW_RESTORE)
      {

         m_pui->_001WindowRestore();

      }
      else if (nCmdShow == SW_MINIMIZE)
      {

         m_pui->_001WindowMinimize(false);

      }
      else if (nCmdShow == SW_SHOWMINNOACTIVE)
      {

         m_pui->_001WindowMinimize(true);

      }
      else
      {

         ::ShowWindow(get_handle(), nCmdShow);

      }

      if ((GetExStyle() & WS_EX_LAYERED) && (!m_pui->IsWindowVisible() || m_pui->WfiIsIconic()))
      {

         ::UpdateLayeredWindow(get_handle(), NULL, NULL, NULL, NULL, NULL, 0, NULL, 0);

      }

      return m_pui->IsWindowVisible();

   }


   bool interaction_impl::WfiIsIconic()
   {

      if (!::IsWindow(get_handle()))
         return false;

      if (GetExStyle() & WS_EX_LAYERED)
      {

         return m_pui->m_eappearance == ::user::AppearanceIconic;

      }
      else
      {

         return ::IsIconic(get_handle()) != FALSE;

      }

   }


   bool interaction_impl::WfiIsZoomed()
   {

      if (!::IsWindow(get_handle()))
         return false;

      return m_pui->get_appearance() == ::user::AppearanceZoomed;

   }


   ::user::interaction * interaction_impl::GetParent() const
   {

      if (!::IsWindow(get_handle()))
         return NULL;

      if (get_handle() == NULL)
         return NULL;

      HWND hwndParent = ::GetParent(get_handle());

      if (hwndParent == NULL)
         return NULL;

      return System.ui_from_handle(hwndParent);

   }


   ::user::interaction * interaction_impl::SetParent(::user::interaction * pWndNewParent)
   {

      ASSERT(::IsWindow(get_handle()));

      return System.ui_from_handle(::SetParent(get_handle(), pWndNewParent->get_handle()));

   }

   ::user::interaction * interaction_impl::GetOwner() const
   {

      if (!::IsWindow(get_handle()))
         return NULL;

      if (get_handle() == NULL)
         return NULL;

      HWND hwndParent = ::GetWindow(get_handle(), GW_OWNER);

      if (hwndParent == NULL)
         return GetParent();

      return System.ui_from_handle(hwndParent);
   }

   ::user::interaction * interaction_impl::SetOwner(::user::interaction * pWndNewParent)
   {

      return NULL;

   }



   LONG interaction_impl::get_window_long(int32_t nIndex) const
   {
      return ::GetWindowLong(get_handle(), nIndex);
   }

   LONG interaction_impl::set_window_long(int32_t nIndex, LONG lValue)
   {
      return ::SetWindowLong(get_handle(), nIndex, lValue);
   }

   LONG_PTR interaction_impl::get_window_long_ptr(int32_t nIndex) const
   {
      return ::GetWindowLongPtr(get_handle(), nIndex);
   }


   LONG_PTR interaction_impl::set_window_long_ptr(int32_t nIndex, LONG_PTR lValue)
   {

      return ::SetWindowLongPtr(get_handle(), nIndex, lValue);

   }




   // interaction_impl
   /* interaction_impl::operator oswindow() const
   { return this == NULL ? NULL : get_handle(); }*/
   bool interaction_impl::operator==(const interaction_impl& wnd) const
   {

      return (NODE_WINDOW((interaction_impl *)&wnd)->get_handle()) == ((interaction_impl *) this)->get_handle();

   }


   bool interaction_impl::operator!=(const interaction_impl& wnd) const
   {

      return (NODE_WINDOW((interaction_impl *)&wnd)->get_handle()) != ((interaction_impl *) this)->get_handle();

   }


   uint32_t interaction_impl::GetStyle() const
   {

      if (!::IsWindow(get_handle()))
         return 0;

      return (uint32_t)::GetWindowLong(get_handle(), GWL_STYLE);

   }


   uint32_t interaction_impl::GetExStyle() const
   {

      if (!::IsWindow(get_handle()))
         return 0;

      return (uint32_t)::GetWindowLong(get_handle(), GWL_EXSTYLE);

   }


   bool interaction_impl::ModifyStyle(uint32_t dwRemove, uint32_t dwAdd, UINT nFlags)
   {

      if (!::IsWindow(get_handle()))
         return false;

      return ModifyStyle(get_handle(), dwRemove, dwAdd, nFlags);

   }


   bool interaction_impl::ModifyStyleEx(uint32_t dwRemove, uint32_t dwAdd, UINT nFlags)
   {

      if (!::IsWindow(get_handle()))
         return false;

      return ModifyStyleEx(get_handle(), dwRemove, dwAdd, nFlags);

   }


   LRESULT interaction_impl::send_message(UINT message, WPARAM wParam, lparam lParam)
   {
      //ASSERT(::IsWindow(get_handle()));
      return ::SendMessage(get_handle(), message, wParam, lParam);

   }


   bool interaction_impl::post_message(UINT message, WPARAM wParam, lparam lParam)
   {

      return ::PostMessage(get_handle(), message, wParam, lParam) != FALSE;

   }


   bool interaction_impl::DragDetect(POINT pt) const
   {

      ASSERT(::IsWindow(((interaction_impl *) this)->get_handle()));

      return ::DragDetect(((interaction_impl *) this)->get_handle(), pt) != FALSE;

   }

   void interaction_impl::set_window_text(const char * lpszString)
   {

      DWORD_PTR lresult = 0;

      m_pui->m_strWindowText = lpszString;

      wstring wstrText(m_pui->m_strWindowText);

      const unichar * pwszText = wstrText;

      if (!::SendMessageTimeoutW(get_handle(), WM_SETTEXT, 0, (LPARAM)pwszText, SMTO_ABORTIFHUNG, 84, &lresult))
         return;

   }

   strsize interaction_impl::get_window_text(char * lpszString, strsize nMaxCount)
   {

      string str;

      get_window_text(str);

      strncpy(lpszString, str, MIN(nMaxCount, str.get_length()));

      return str.get_length();

   }

   void interaction_impl::get_window_text(string & str)
   {

      DWORD_PTR lresult = 0;

      if (!::SendMessageTimeoutW(get_handle(), WM_GETTEXTLENGTH, 0, 0, SMTO_ABORTIFHUNG, 84, &lresult))
         return;

      wstring wstr;

      if (!::SendMessageTimeoutW(get_handle(), WM_GETTEXT, (LPARAM)wstr.alloc(lresult + 1), lresult + 1, SMTO_ABORTIFHUNG, 84, &lresult))
         return;

      str = wstr;

   }

   strsize interaction_impl::get_window_text_length()
   {

      ASSERT(::IsWindow(get_handle()));

      return ::GetWindowTextLength(get_handle());

   }


   void interaction_impl::DragAcceptFiles(bool bAccept)
   {
      ASSERT(::IsWindow(get_handle()));
      ::DragAcceptFiles(get_handle(), bAccept);
   }


   UINT interaction_impl::ArrangeIconicWindows()
   {

      ASSERT(::IsWindow(get_handle()));
      return ::ArrangeIconicWindows(get_handle());

   }


   //int32_t interaction_impl::SetWindowRgn(HRGN hRgn,bool bRedraw)
   //{

   //   //ASSERT(::IsWindow(get_handle())); return ::SetWindowRgn(get_handle(),hRgn,bRedraw);

   //}


   //int32_t interaction_impl::GetWindowRgn(HRGN hRgn)
   //{

   //   //ASSERT(::IsWindow(get_handle()) && hRgn != NULL); return ::GetWindowRgn(get_handle(),hRgn);

   //}


   void interaction_impl::BringToTop(int nCmdShow)
   {

      if (GetParent() == NULL)
      {

         // place the interaction_impl on top except for certain nCmdShow

         if (
            nCmdShow != SW_HIDE
            && nCmdShow != SW_MINIMIZE
            && nCmdShow != SW_SHOWMINNOACTIVE
            && nCmdShow != SW_SHOWNA
            && nCmdShow != SW_SHOWNOACTIVATE
            )
         {

            oswindow oswindow = get_handle();

            oswindow = ::GetLastActivePopup(oswindow);

            ::user::interaction * pui = System.ui_from_handle(oswindow);

            if (pui == NULL)
               BringWindowToTop();
            else
               pui->BringWindowToTop();

         }

      }

   }

   bool interaction_impl::BringWindowToTop()
   {

      bool bOk = ::BringWindowToTop(get_handle()) != FALSE;


      if (GetExStyle() & WS_EX_LAYERED)
      {

         if (!(GetExStyle() & WS_EX_TOPMOST))
         {

            ::SetWindowPos(get_handle(), (HWND)ZORDER_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

            //::SetWindowPos(get_handle(),(HWND)ZORDER_TOP,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

            //::SetWindowPos(get_handle(),(HWND)ZORDER_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

            //::SetWindowPos(get_handle(),(HWND)ZORDER_TOP,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

         }

      }

      return bOk;

   }

   void interaction_impl::MapWindowPoints(::window_sp pwndTo, LPPOINT lpPoint, UINT nCount)
   {
      ASSERT(::IsWindow(get_handle()));
      ::MapWindowPoints(get_handle(), pwndTo->get_handle(), lpPoint, nCount);
   }

   void interaction_impl::MapWindowPoints(::window_sp pwndTo, LPRECT lpRect)
   {
      ASSERT(::IsWindow(get_handle()));
      ::MapWindowPoints(get_handle(), pwndTo->get_handle(), (LPPOINT)lpRect, 2);
   }



   void interaction_impl::UpdateWindow()
   {
      ::UpdateWindow(get_handle());
   }

   void interaction_impl::SetRedraw(bool bRedraw)
   {
      ASSERT(::IsWindow(get_handle()));
      ::SendMessage(get_handle(), WM_SETREDRAW, bRedraw, 0);
   }

   bool interaction_impl::GetUpdateRect(LPRECT lpRect, bool bErase)
   {
      ASSERT(::IsWindow(get_handle()));
      return ::GetUpdateRect(get_handle(), lpRect, bErase) != FALSE;
   }

   int32_t interaction_impl::GetUpdateRgn(::draw2d::region* pRgn, bool bErase)
   {
      ASSERT(::IsWindow(get_handle()));
      return ::GetUpdateRgn(get_handle(), (HRGN)pRgn->get_os_data(), bErase);
   }

   void interaction_impl::Invalidate(bool bErase)
   {
      ASSERT(::IsWindow(get_handle()));
      ::InvalidateRect(get_handle(), NULL, bErase);
   }

   void interaction_impl::InvalidateRect(LPCRECT lpRect, bool bErase)
   {
      ASSERT(::IsWindow(get_handle()));
      ::InvalidateRect(get_handle(), lpRect, bErase);
   }

   void interaction_impl::InvalidateRgn(::draw2d::region* pRgn, bool bErase)
   {
      ASSERT(::IsWindow(get_handle()));
      ::InvalidateRgn(get_handle(), (HRGN)pRgn->get_os_data(), bErase);
   }

   void interaction_impl::ValidateRect(LPCRECT lpRect)
   {
      ASSERT(::IsWindow(get_handle()));
      ::ValidateRect(get_handle(), lpRect);
   }
   void interaction_impl::ValidateRgn(::draw2d::region* pRgn)
   {
      ASSERT(::IsWindow(get_handle()));
      ::ValidateRgn(get_handle(), (HRGN)pRgn->get_os_data());
   }

   bool interaction_impl::IsWindowVisible()
   {

      try
      {

         if (!::IsWindowVisible(get_handle()))
            return false;

         return true;

      }
      catch (...)
      {

         return false;

      }

   }


   void interaction_impl::ShowOwnedPopups(bool bShow)
   {

      ASSERT(::IsWindow(get_handle()));

      ::ShowOwnedPopups(get_handle(), bShow);

   }

   void interaction_impl::SendMessageToDescendants(UINT message, WPARAM wParam, lparam lParam, bool bDeep, bool bOnlyPerm)
   {

      ASSERT(::IsWindow(get_handle()));
      //interaction_impl::SendMessageToDescendants(get_handle(), message, wParam, lParam, bDeep, bOnlyPerm);

      // walk through HWNDs to avoid creating temporary interaction_impl objects
      // unless we need to call this function recursively
      sp(::user::interaction) pui = m_pui->top_child();
      while (pui != NULL)
      {
         try
         {
            pui->send_message(message, wParam, lParam);
         }
         catch (...)
         {
         }
         if (bDeep)
         {
            // send to child windows after parent
            try
            {
               pui->SendMessageToDescendants(message, wParam, lParam, bDeep, bOnlyPerm);
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



   ::draw2d::graphics * interaction_impl::GetDCEx(::draw2d::region* prgnClip, uint32_t flags)
   {

      ASSERT(::IsWindow(get_handle()));

      ::draw2d::graphics_sp g(allocer());

      g->attach(::GetDCEx(get_handle(), (HRGN)prgnClip->get_os_data(), flags));

      return g.detach();


   }


   bool interaction_impl::LockWindowUpdate()
   {

      ASSERT(::IsWindow(get_handle()));

      return ::LockWindowUpdate(get_handle()) != FALSE;

   }

   void interaction_impl::UnlockWindowUpdate()
   {

      ASSERT(::IsWindow(get_handle()));

      ::LockWindowUpdate(NULL);

   }


   bool interaction_impl::RedrawWindow(LPCRECT lpRectUpdate, ::draw2d::region* prgnUpdate, UINT flags)
   {

      if (!IsWindowVisible())
      {

         return true;

      }

      if (flags & RDW_UPDATENOW)
      {





         //if (GetExStyle() & WS_EX_LAYERED)
         {

            _001UpdateWindow();

            return true;

         }
         //else
         //{

         //   if (!IsWindowVisible() && !m_bShowFlags)
         //      return false;

         //   if (!IsWindowVisible())
         //   {

         //      if (m_iShowFlags & SWP_SHOWWINDOW)
         //      {

         //         ShowWindow(SW_SHOW);

         //      }

         //   }

         //   return ::RedrawWindow(get_handle(), lpRectUpdate, prgnUpdate == NULL ? NULL : (HRGN)prgnUpdate->get_os_data(),
         //      flags | RDW_NOERASE | RDW_NOFRAME | RDW_INVALIDATE) != FALSE;

         //}

      }
      else
      {

         m_pui->m_bRedraw = true;

      }

      return true;

   }





   bool interaction_impl::EnableScrollBar(int32_t nSBFlags, UINT nArrowFlags)
   {

      ASSERT(::IsWindow(get_handle()));

      return ::EnableScrollBar(get_handle(), nSBFlags, nArrowFlags) != FALSE;

   }

   bool interaction_impl::DrawAnimatedRects(int32_t idAni, CONST RECT *lprcFrom, CONST RECT *lprcTo)
   {

      ASSERT(::IsWindow(get_handle()));

      return ::DrawAnimatedRects(get_handle(), idAni, lprcFrom, lprcTo) != FALSE;

   }

   bool interaction_impl::DrawCaption(::draw2d::graphics * pgraphics, LPCRECT lprc, UINT uFlags)
   {

      ASSERT(::IsWindow(get_handle()));

      ::exception::throw_not_implemented(get_app());
      return false;
      //      return ::DrawCaption(get_handle(), (HDC)(dynamic_cast<::windows::graphics * >(pgraphics))->get_handle(), lprc, uFlags) != FALSE;

   }

   bool interaction_impl::SetTimer(uint_ptr nIDEvent, UINT nElapse, PFN_TIMER pfnTimer)
   {

      return create_timer(nIDEvent, nElapse, pfnTimer, true, m_pui);

   }


   bool interaction_impl::KillTimer(uint_ptr nIDEvent)
   {

      return delete_timer(nIDEvent);

   }

   bool interaction_impl::is_window_enabled()
   {

      if (!::IsWindow(get_handle()))
         return false;

      return ::IsWindowEnabled(get_handle()) != FALSE;

   }


   bool interaction_impl::enable_window(bool bEnable)
   {

      ASSERT(::IsWindow(get_handle()));

      return ::EnableWindow(get_handle(), bEnable) != FALSE;

   }

   ::user::interaction * interaction_impl::GetActiveWindow()
   {

      return System.ui_from_handle(::GetActiveWindow());

   }


   ::user::interaction * interaction_impl::SetActiveWindow()
   {

      if (!::IsWindow(get_handle()))
      {

         return NULL;

      }

      if (!::IsWindowVisible(get_handle()))
      {

         return NULL;

      }

      oswindow window = ::SetActiveWindow(get_handle());

      ::user::interaction * pui = System.ui_from_handle(window);

      return pui;

   }



   ::user::interaction * interaction_impl::GetFocus()
   {

      return System.ui_from_handle(::GetFocus());

   }


   bool interaction_impl::SetFocus()
   {

      if (!::IsWindow(get_handle()))
      {

         return false;

      }

      if (!::IsWindowVisible(get_handle()))
      {

         return false;

      }

      oswindow window = ::SetFocus(get_handle());

      return window != NULL;

   }


   ::user::interaction *  interaction_impl::GetDesktopWindow()
   {

      return System.ui_from_handle(::GetDesktopWindow());

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

      ASSERT(::IsWindow(get_handle()));

      ::CheckDlgButton(get_handle(), nIDButton, nCheck);

   }


   void interaction_impl::CheckRadioButton(int32_t nIDFirstButton, int32_t nIDLastButton, int32_t nIDCheckButton)
   {

      ASSERT(::IsWindow(get_handle()));

      ::CheckRadioButton(get_handle(), nIDFirstButton, nIDLastButton, nIDCheckButton);

   }


   int32_t interaction_impl::DlgDirList(LPTSTR lpPathSpec, int32_t nIDListBox, int32_t nIDStaticPath, UINT nFileType)
   {

      ASSERT(::IsWindow(get_handle()));

      return ::DlgDirList(get_handle(), lpPathSpec, nIDListBox, nIDStaticPath, nFileType);

   }


   int32_t interaction_impl::DlgDirListComboBox(LPTSTR lpPathSpec, int32_t nIDComboBox, int32_t nIDStaticPath, UINT nFileType)
   {

      ASSERT(::IsWindow(get_handle()));

      return ::DlgDirListComboBox(get_handle(), lpPathSpec, nIDComboBox, nIDStaticPath, nFileType);

   }


   bool interaction_impl::DlgDirSelect(LPTSTR lpString, int32_t nSize, int32_t nIDListBox)
   {

      ASSERT(::IsWindow(get_handle()));

      return ::DlgDirSelectEx(get_handle(), lpString, nSize, nIDListBox) != FALSE;

   }


   bool interaction_impl::DlgDirSelectComboBox(LPTSTR lpString, int32_t nSize, int32_t nIDComboBox)
   {

      ASSERT(::IsWindow(get_handle()));

      return ::DlgDirSelectComboBoxEx(get_handle(), lpString, nSize, nIDComboBox) != FALSE;

   }


   void interaction_impl::get_child_by_id(id id, oswindow* poswindow_) const
   {

      ASSERT(::IsWindow(((interaction_impl *) this)->get_handle()));

      ASSERT(poswindow_ != NULL);

      *poswindow_ = ::GetDlgItem(((interaction_impl *) this)->get_handle(), (int32_t)id);

   }


   UINT interaction_impl::GetChildByIdInt(int32_t nID, BOOL * lpTrans, bool bSigned) const
   {

      ASSERT(::IsWindow(((interaction_impl *) this)->get_handle()));

      return ::GetDlgItemInt(((interaction_impl *) this)->get_handle(), nID, lpTrans, bSigned);

   }


   int32_t interaction_impl::GetChildByIdText(int32_t nID, LPTSTR lpStr, int32_t nMaxCount) const
   {

      ASSERT(::IsWindow(((interaction_impl *) this)->get_handle()));

      return ::GetDlgItemText(((interaction_impl *) this)->get_handle(), nID, lpStr, nMaxCount);

   }


   ::window_sp interaction_impl::GetNextDlgGroupItem(::window_sp pWndCtl, bool bPrevious) const
   {

      ASSERT(::IsWindow(((interaction_impl *) this)->get_handle()));

      return System.ui_from_handle(::GetNextDlgGroupItem(((interaction_impl *) this)->get_handle(), pWndCtl->get_handle(), bPrevious));

   }


   ::window_sp interaction_impl::GetNextDlgTabItem(::window_sp pWndCtl, bool bPrevious) const
   {

      ASSERT(::IsWindow(((interaction_impl *) this)->get_handle()));

      return System.ui_from_handle(::GetNextDlgTabItem(((interaction_impl *) this)->get_handle(), pWndCtl->get_handle(), bPrevious));

   }


   UINT interaction_impl::IsDlgButtonChecked(int32_t nIDButton) const
   {
      ASSERT(::IsWindow(((interaction_impl *) this)->get_handle()));
      return ::IsDlgButtonChecked(((interaction_impl *) this)->get_handle(), nIDButton);
   }
   LPARAM interaction_impl::SendDlgItemMessage(int32_t nID, UINT message, WPARAM wParam, LPARAM lParam)
   {
      ASSERT(::IsWindow(((interaction_impl *) this)->get_handle()));
      return ::SendDlgItemMessage(((interaction_impl *) this)->get_handle(), nID, message, wParam, lParam);
   }
   void interaction_impl::SetDlgItemInt(int32_t nID, UINT nValue, bool bSigned)
   {
      ASSERT(::IsWindow(get_handle()));
      ::SetDlgItemInt(get_handle(), nID, nValue, bSigned);
   }
   void interaction_impl::SetDlgItemText(int32_t nID, const char * lpszString)
   {
      ASSERT(::IsWindow(get_handle()));
      ::SetDlgItemText(get_handle(), nID, lpszString);
   }
   int32_t interaction_impl::ScrollWindowEx(int32_t dx, int32_t dy,
      LPCRECT lpRectScroll, LPCRECT lpRectClip,
      ::draw2d::region* prgnUpdate, LPRECT lpRectUpdate, UINT flags)
   {
      ASSERT(::IsWindow(get_handle()));
      return ::ScrollWindowEx(get_handle(), dx, dy, lpRectScroll, lpRectClip,
         (HRGN)prgnUpdate->get_os_data(), lpRectUpdate, flags);
   }

   void interaction_impl::ShowScrollBar(UINT nBar, bool bShow)
   {
      ASSERT(::IsWindow(get_handle()));
      ::ShowScrollBar(get_handle(), nBar, bShow);
   }
   ::user::interaction * interaction_impl::ChildWindowFromPoint(POINT point)
   {
      ASSERT(::IsWindow(get_handle()));

      return  System.ui_from_handle(::ChildWindowFromPoint(get_handle(), point));


   }

   ::user::interaction * interaction_impl::ChildWindowFromPoint(POINT point, UINT nFlags)
   {
      ASSERT(::IsWindow(get_handle()));

      return  System.ui_from_handle(::ChildWindowFromPointEx(get_handle(), point, nFlags));


   }



   ::user::interaction * interaction_impl::get_next_window(UINT nFlag)
   {

      if (!::IsWindow(get_handle()))
         return NULL;

      return  System.ui_from_handle(::GetNextWindow(get_handle(), nFlag));

   }


   ::user::interaction * interaction_impl::GetTopWindow() const
   {

      ASSERT(::IsWindow(get_handle()));

      return System.ui_from_handle(::GetTopWindow(get_handle()));

   }


   ::user::interaction * interaction_impl::GetWindow(UINT nCmd) const
   {

      ASSERT(::IsWindow(get_handle()));

      return System.ui_from_handle(::GetWindow(get_handle(), nCmd));

   }


   ::user::interaction * interaction_impl::GetLastActivePopup() const
   {

      ASSERT(::IsWindow(get_handle()));

      return System.ui_from_handle(::GetLastActivePopup(get_handle()));

   }



   bool interaction_impl::FlashWindow(bool bInvert)
   {

      ASSERT(::IsWindow(get_handle()));

      return ::FlashWindow(get_handle(), bInvert) != FALSE;

   }

   bool interaction_impl::ChangeClipboardChain(oswindow oswindow_Next)
   {

      ASSERT(::IsWindow(get_handle()));

      return ::ChangeClipboardChain(get_handle(), oswindow_Next) != FALSE;

   }

   oswindow interaction_impl::SetClipboardViewer()
   {

      ASSERT(::IsWindow(get_handle()));

      return ::SetClipboardViewer(get_handle());

   }

   bool interaction_impl::OpenClipboard()
   {

      ASSERT(::IsWindow(get_handle()));

      return ::OpenClipboard(get_handle()) != FALSE;

   }

   ::window_sp interaction_impl::GetOpenClipboardWindow()
   {

      return System.ui_from_handle(::GetOpenClipboardWindow());

   }

   ::window_sp interaction_impl::GetClipboardOwner()
   {

      return System.ui_from_handle(::GetClipboardOwner());

   }

   ::window_sp interaction_impl::GetClipboardViewer()
   {

      return System.ui_from_handle(::GetClipboardViewer());

   }

   void interaction_impl::CreateCaret(::draw2d::bitmap* pBitmap)
   {

      ASSERT(::IsWindow(get_handle()));

      ::CreateCaret(get_handle(), (HBITMAP)pBitmap->get_os_data(), 0, 0);

   }

   void interaction_impl::CreateSolidCaret(int32_t nWidth, int32_t nHeight)
   {

      ASSERT(::IsWindow(get_handle()));

      ::CreateCaret(get_handle(), (HBITMAP)0, nWidth, nHeight);

   }

   void interaction_impl::CreateGrayCaret(int32_t nWidth, int32_t nHeight)
   {

      ASSERT(::IsWindow(get_handle()));

      ::CreateCaret(get_handle(), (HBITMAP)1, nWidth, nHeight);

   }

   point interaction_impl::GetCaretPos()
   {

      point point;

      ::GetCaretPos((LPPOINT)&point);
      return point;

   }

   void interaction_impl::SetCaretPos(POINT point)
   {

      ::SetCaretPos(point.x, point.y);

   }

   void interaction_impl::HideCaret()
   {

      ::HideCaret(get_handle());

   }

   void interaction_impl::ShowCaret()
   {

      ::ShowCaret(get_handle());

   }

   bool interaction_impl::SetForegroundWindow()
   {

      return ::SetForegroundWindow(get_handle()) != FALSE;

   }

   ::user::interaction * interaction_impl::GetForegroundWindow()
   {

      return System.ui_from_handle(::GetForegroundWindow());

   }

   bool interaction_impl::SendNotifyMessage(UINT message, WPARAM wParam, lparam lParam)
   {

      return ::SendNotifyMessage(get_handle(), message, wParam, lParam) != FALSE;

   }

   // Win4
   HICON interaction_impl::SetIcon(HICON hIcon, bool bBigIcon)
   {

      return (HICON)send_message(WM_SETICON, bBigIcon, (LPARAM)hIcon);

   }

   HICON interaction_impl::GetIcon(bool bBigIcon) const
   {

      ASSERT(::IsWindow(((interaction_impl *) this)->get_handle()));

      return (HICON)const_cast <::windows::interaction_impl *> (this)->send_message(WM_GETICON, bBigIcon);

   }

   void interaction_impl::Print(::draw2d::graphics * pgraphics, uint32_t dwFlags) const
   {

      ASSERT(::IsWindow(((interaction_impl *) this)->get_handle()));

      ::exception::throw_not_implemented(get_app());
      //      const_cast < ::windows::interaction_impl * > (this)->send_message(WM_PRINT, (WPARAM)(dynamic_cast<::windows::graphics * >(pgraphics))->get_handle(), (LPARAM) dwFlags);

   }

   void interaction_impl::PrintClient(::draw2d::graphics * pgraphics, uint32_t dwFlags) const
   {

      ASSERT(::IsWindow(((interaction_impl *) this)->get_handle()));

      ::exception::throw_not_implemented(get_app());
      //const_cast < ::windows::interaction_impl * > (this)->send_message(WM_PRINTCLIENT, (WPARAM)(dynamic_cast<::windows::graphics * >(pgraphics))->get_handle(), (LPARAM) dwFlags);

   }

   bool interaction_impl::SetWindowContextHelpId(uint32_t dwContextHelpId)
   {

      ASSERT(::IsWindow(((interaction_impl *) this)->get_handle()));

      return ::SetWindowContextHelpId(((interaction_impl *) this)->get_handle(), dwContextHelpId) != FALSE;

   }

   uint32_t interaction_impl::GetWindowContextHelpId() const
   {

      ASSERT(::IsWindow(((interaction_impl *) this)->get_handle()));

      return ::GetWindowContextHelpId(((interaction_impl *) this)->get_handle());

   }


   // Default message map implementations
   void interaction_impl::OnActivateApp(bool, uint32_t)
   {
      Default();
   }
   void interaction_impl::OnActivate(UINT, ::window_sp, bool)
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
   void interaction_impl::OnContextMenu(::window_sp, point)
   {
      Default();
   }

   bool interaction_impl::OnCopyData(::window_sp, COPYDATASTRUCT*)
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


   void interaction_impl::_001OnSetFocus(signal_details * pobj)
   {
      //   Default();

      Default();

      if (GetParent() == NULL)
      {

         sp(::user::elemental) pelementalFocus = Session.get_keyboard_focus();

         if (pelementalFocus.is_set())
         {

            sp(::user::interaction) puiFocus = pelementalFocus;

            if (puiFocus.is_set())
            {

               puiFocus->keyboard_focus_OnSetFocus();

            }

            on_keyboard_focus(pelementalFocus);

         }

      }

   }




   void interaction_impl::_001OnKillFocus(signal_details * pobj)
   {
      //   Default();

      //Default();

      if (GetParent() == NULL)
      {

         m_puiFocus = NULL;

         sp(::user::interaction) puiKeyboardFocus = Session.get_keyboard_focus();

         if (puiKeyboardFocus.is_set())
         {

            if (puiKeyboardFocus == m_pui
               || puiKeyboardFocus->is_descendant_of(m_pui))
            {

               Session.set_keyboard_focus(NULL);

            }

         }

      }

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

      SCAST_PTR(::message::base, pbase, pobj);

      auto pcursor = Session.get_cursor();

      if (pcursor != NULL && pcursor->m_ecursor != ::visual::cursor_system)
      {

         ::SetCursor(pcursor->get_HCURSOR());

      }

      pbase->set_lresult(1);

      pbase->m_bRet = true;

   }


   void interaction_impl::OnShowWindow(bool, UINT)
   {

      Default();

   }


   void interaction_impl::OnSize(UINT, int32_t, int32_t)
   {

      Default();

   }


   void interaction_impl::OnTCard(UINT, uint32_t)
   {

      Default();

   }


   void interaction_impl::_001OnWindowPosChanging(signal_details * pobj)
   {

      return;


   }


   void interaction_impl::_001OnWindowPosChanged(signal_details * pobj)
   {


      return;


      //SCAST_PTR(::message::window_pos,pwindowpos,pobj);

      //if(GetExStyle() & WS_EX_LAYERED)
      //{

      //   pwindowpos->m_bRet = true;

      //}

   }


   void interaction_impl::_001OnGetMinMaxInfo(signal_details * pobj)
   {

      SCAST_PTR(::message::base, pbase, pobj);

   }

   void interaction_impl::OnDropFiles(HDROP)
   {
      Default();
   }
   void interaction_impl::OnPaletteIsChanging(::window_sp)
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
   void interaction_impl::OnPaletteChanged(::window_sp)
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
   int32_t interaction_impl::OnMouseActivate(::window_sp, UINT, UINT)
   {
      return (int32_t)Default();
   }
   void interaction_impl::OnMouseMove(UINT, point)
   {
      Default();
   }

   bool interaction_impl::OnMouseWheel(UINT, int16_t, point)
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
   void interaction_impl::OnAskCbFormatName(__in UINT nMaxCount, __out_ecount_z(nMaxCount) LPTSTR pszName)
   {
      (nMaxCount);
      if (nMaxCount > 0)
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
   void interaction_impl::OnHScrollClipboard(::window_sp, UINT, UINT)
   {
      Default();
   }
   void interaction_impl::OnPaintClipboard(::window_sp, HGLOBAL)
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
   void interaction_impl::OnSizeClipboard(::window_sp, HGLOBAL)
   {
      Default();
   }
   void interaction_impl::OnVScrollClipboard(::window_sp, UINT, UINT)
   {
      Default();
   }
   UINT interaction_impl::OnGetDlgCode()
   {
      return (UINT)Default();
   }
   void interaction_impl::OnMDIActivate(bool, ::window_sp, ::window_sp)
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
   void interaction_impl::OnStyleChanged(int32_t, LPSTYLESTRUCT)
   {
      Default();
   }
   void interaction_impl::OnStyleChanging(int32_t, LPSTYLESTRUCT)
   {
      Default();
   }
   void interaction_impl::OnSizing(UINT, LPRECT)
   {
      Default();
   }
   void interaction_impl::OnMoving(UINT, LPRECT)
   {
      Default();
   }
   void interaction_impl::OnCaptureChanged(::window_sp)
   {
      Default();
   }

   bool interaction_impl::OnDeviceChange(UINT, uint_ptr)
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
   //    void interaction_impl::do_data_exchange(CDataExchange*)
   //   { } // default does nothing

   // interaction_impl modality support

   void interaction_impl::BeginModalState()
   {

      ::EnableWindow(get_handle(), FALSE);

   }

   void interaction_impl::EndModalState()
   {

      ::EnableWindow(get_handle(), TRUE);

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
      ASSERT(::IsWindow(get_handle()));
      ::CloseWindow(get_handle());
   }

   bool interaction_impl::OpenIcon()
   {

      ASSERT(::IsWindow(get_handle()));

      return ::OpenIcon(get_handle()) != FALSE;

   }

   ////////////////////////////////////////////////////////////////////////////
   // UI related interaction_impl functions

   oswindow interaction_impl::get_safe_owner(oswindow hParent, oswindow* pWndTop)
   {
      // get interaction_impl to start with
      oswindow oswindow = hParent;
      if (oswindow == NULL)
      {
         /* trans      sp(::user::frame_window) pFrame = command_target::GetRoutingFrame_();
         if (pFrame != NULL)
         oswindow = pFrame->get_handle();
         else
         oswindow = System.m_puiMain->get_handle();*/
      }

      // a popup interaction_impl cannot be owned by a child interaction_impl
      while (oswindow != NULL && (::GetWindowLong(oswindow, GWL_STYLE) & WS_CHILD))
         oswindow = ::GetParent(oswindow);

      // determine toplevel interaction_impl to disable as well
      ::oswindow oswindow_Top = oswindow;
      ::oswindow oswindow_Temp = oswindow;
      for (;;)
      {
         if (oswindow_Temp == NULL)
            break;
         else
            oswindow_Top = oswindow_Temp;
         oswindow_Temp = ::GetParent(oswindow_Top);
      }

      // get last active popup of first non-child that was found
      if (hParent == NULL && oswindow != NULL)
         oswindow = ::GetLastActivePopup(oswindow);

      // disable and store top level parent interaction_impl if specified
      if (pWndTop != NULL)
      {
         if (oswindow_Top != NULL && ::IsWindowEnabled(oswindow_Top) && oswindow_Top != oswindow)
         {
            *pWndTop = oswindow_Top;
            ::EnableWindow(oswindow_Top, FALSE);
         }
         else
            *pWndTop = NULL;
      }

      return oswindow;    // return the owner as oswindow
   }


   LRESULT CALLBACK __cbt_filter_hook(int32_t code, WPARAM wParam, LPARAM lParam)
   {

      if (code != HCBT_CREATEWND)
      {

         // wait for HCBT_CREATEWND just pass others on...

         return CallNextHookEx(t_hHookOldCbtFilter, code, wParam, lParam);

      }

      ASSERT(lParam != NULL);

      ::user::create_struct * lpcs = (::user::create_struct *) ((LPCBT_CREATEWND)lParam)->lpcs;

      ASSERT(lpcs != NULL);

      ::windows::interaction_impl * pwnd = t_pwndInit;

      if (pwnd != NULL || (!(lpcs->style & WS_CHILD)))
      {

         t_pwndInit = NULL;

         // Note: special check to avoid subclassing the IME interaction_impl
         //if (gen_DBCS)
         {
            // check for cheap CS_IME style first...
            if (GetClassLongW((oswindow)wParam, GCL_STYLE) & CS_IME)
               goto lCallNextHook;

            //// get class name of the interaction_impl that is being created
            //const char * pszClassName;
            //char szClassName[_countof("ime") + 1];
            //if(uint_ptr(lpcs->lpszClass) > 0xffff)
            //{
            //   pszClassName = lpcs->lpszClass;
            //}
            //else
            //{
            //   szClassName[0] = '\0';
            //   GlobalGetAtomName((ATOM)lpcs->lpszClass,szClassName,_countof(szClassName));
            //   pszClassName = szClassName;
            //}

            //// a little more expensive to test this way, but necessary...
            //if(::__invariant_stricmp(pszClassName,"ime") == 0)
            //   goto lCallNextHook;

         }

         ASSERT(wParam != NULL);

         ::oswindow oswindow = (::oswindow) wParam;

         WNDPROC oldWndProc;

         if (pwnd != NULL)
         {

            ASSERT(Sys(pwnd->get_app()).ui_from_handle(oswindow) == NULL);

            pwnd->m_pui->m_pimpl = pwnd;

            pwnd->attach(oswindow);

            pwnd->pre_subclass_window();

            WNDPROC *pOldWndProc = pwnd->GetSuperWndProcAddr();
            ASSERT(pOldWndProc != NULL);

            // subclass the interaction_impl with standard __window_procedure
            WNDPROC afxWndProc = __get_window_procedure();
            oldWndProc = (WNDPROC)SetWindowLongPtrW(oswindow, GWLP_WNDPROC,
               (uint_ptr)afxWndProc);
            ASSERT(oldWndProc != NULL);
            if (oldWndProc != afxWndProc)
               *pOldWndProc = oldWndProc;



         }
      }

   lCallNextHook:
      LRESULT lResult = CallNextHookEx(t_hHookOldCbtFilter, code, wParam, lParam);

      return lResult;
   }



   void interaction_impl::_001OnEraseBkgnd(signal_details * pobj)
   {
      SCAST_PTR(::message::erase_bkgnd, perasebkgnd, pobj);
      perasebkgnd->m_bRet = true;
      perasebkgnd->set_result(TRUE);
   }



   void interaction_impl::_001OnTriggerMouseInside()
   {

      ::user::interaction_impl::_001OnTriggerMouseInside();

      TRACKMOUSEEVENT tme = { sizeof(tme) };
      tme.dwFlags = TME_LEAVE;
      tme.hwndTrack = get_handle();
      TrackMouseEvent(&tme);

   }



   void interaction_impl::set_viewport_org(::draw2d::graphics * pgraphics)
   {

      // graphics will be already set its view port to the interaction_impl for linux - cairo with xlib

      pgraphics->SetViewportOrg(point(0, 0));

   }


   /*

   void interaction_impl::_001DeferPaintLayeredWindowBackground(::draw2d::dib * pdib)
   {


   }

   */



   bool interaction_impl::get_rect_normal(LPRECT lprect)
   {

      WINDOWPLACEMENT wp;

      ZERO(wp);

      if (!GetWindowPlacement(&wp))
         return false;

      *lprect = wp.rcNormalPosition;

      return true;

   }


   DWORD WINAPI drop_target(LPVOID lp)
   {
      interaction_impl* p = (interaction_impl *)lp;

      HRESULT hr = OleInitialize(NULL);

      hr = ::RegisterDragDrop(p->get_handle(), p);

      if (SUCCEEDED(CoCreateInstance(CLSID_DragDropHelper, NULL,
         CLSCTX_INPROC_SERVER,
         IID_IDropTargetHelper,
         (void**)&p->m_piDropHelper)))
      {
         p->m_bUseDnDHelper = true;
      }

      MSG msg;
      while (GetMessage(&msg, NULL, 0, 0xffffffffu))
      {

         TranslateMessage(&msg);
         DispatchMessage(&msg);
      }

      return 0;
   }

   void interaction_impl::register_drop_target()
   {
      m_bUseDnDHelper = false;

      ::CreateThread(NULL, 0, drop_target, this, 0, NULL);


   }

   void interaction_impl::show_task(bool bShow)
   {

      synch_lock sl(m_pmutex);

      // https://www.daniweb.com/programming/software-development/threads/457564/mfc-application-disablehide-taskbar-icon

      if (bShow)
      {

         ModifyStyleEx(WS_EX_TOOLWINDOW, 0, SWP_FRAMECHANGED);

      }
      else
      {

         ModifyStyleEx(0, WS_EX_TOOLWINDOW, SWP_FRAMECHANGED);

      }


      defer_co_initialize_ex(false);


      HRESULT Result = S_OK;

      comptr < ITaskbarList>                     tasklist;

      tasklist.CoCreateInstance(CLSID_TaskbarList, nullptr, CLSCTX_SERVER);

      {

         if (FAILED(tasklist->HrInit()))
         {

            return;

         }

      }



      if (SUCCEEDED(Result))
      {


         if (bShow)
         {

            HRESULT hr = tasklist->AddTab(get_handle());

            TRACE("result = %d", hr);

         }
         else
         {

            tasklist->DeleteTab(get_handle());

         }

      }

   }
   bool interaction_impl::SetWindowPos(int_ptr z, int32_t x, int32_t y, int32_t cx, int32_t cy, UINT nFlags)
   {

      if (GetExStyle() & WS_EX_LAYERED)
      {

         return ::user::interaction_impl::SetWindowPos(z, x, y, cx, cy, nFlags);

      }
      else
      {

         return ::SetWindowPos(get_handle(), (HWND)z, x, y, cx, cy, nFlags) != 0;

      }

   }

} // namespace windows


#undef __window_procedure


#define VARIAS_MENSAGENS_DO_WINDOWS_RECEBIDAS_PELA_FUNCAO_QUE_RECEBE_AS_MENSAGENS_A_FUNCAO_QUE_RECEBE_AS_MENSAGENS_RETORNA_1_SE_A_MENSAGEM_SE_PROCESSOU_OU_QUER_DIZER_QUE_PROCESSOU_SERA_QUE_ATINGIU_O_LIMITE 1
// isso � um comentario e zero (0) se n�o processou
LRESULT CALLBACK __window_procedure(oswindow oswindow, UINT message, WPARAM wparam, LPARAM lparam)
{

   if (message == WM_LBUTTONDOWN)
   {

      output_debug_string("wm_lbuttondown");

   }


   if (::aura::system::g_p == NULL)
   {

      return 0;

   }

   if (::aura::system::g_p->m_pbasesystem == NULL)
   {

      return 0;

   }

   ::user::interaction * pui = ::aura::system::g_p->m_pbasesystem->ui_from_handle(oswindow);

   //if(message == WM_SETCURSOR)
   //{

   //   return 0;

   //}

   if (pui == NULL)
   {

      return ::DefWindowProc(oswindow, message, wparam, lparam);

   }

   if (message == WM_SHOWWINDOW)
   {

      output_debug_string("WM_SHOWWINDOW");
      //return 0;

      //

   }


   if (pui->m_bMoving || pui->m_bMoveWindow)
   {

      //if(message == WM_MOUSEMOVE)
      //{
      //
      //   return 0;

      //}
      //else
      if (message == WM_WINDOWPOSCHANGING)
      {

         //return 0;

         return ::DefWindowProc(oswindow, message, wparam, lparam);

      }
      else if (message == WM_WINDOWPOSCHANGED)
      {

         //return 0;

         return ::DefWindowProc(oswindow, message, wparam, lparam);

      }
      //else if(message == WM_MOVE)
      //{

      //   //return 0;

      //   return ::DefWindowProc(oswindow,message,wparam,lparam);

      //}

   }

   LRESULT lresult = pui->call_message_handler(message, wparam, lparam);

   return lresult;

}


// always indirectly accessed via __get_window_procedure
WNDPROC CLASS_DECL_BASE __get_window_procedure()
{
   //return __get_module_state()->m_pfn_window_procedure;
   return &::__window_procedure;
}




CLASS_DECL_BASE bool hook_window_create(::windows::interaction_impl * pwindow)
{

   if (pwindow == NULL)
      return false;

   if (pwindow->get_handle() != NULL)
      return false;

   if (t_hHookOldCbtFilter == NULL)
   {

      t_hHookOldCbtFilter = ::SetWindowsHookExW(WH_CBT, windows::__cbt_filter_hook, NULL, ::GetCurrentThreadId());

      if (t_hHookOldCbtFilter == NULL)
      {

         return false;

      }

   }

   if (t_hHookOldCbtFilter == NULL)
      return false;


   t_pwndInit = pwindow;

   if (t_pwndInit == NULL)   // hook not already in progress
      return false;

   if (t_pwndInit != pwindow)
      return false;

   return true;

}

CLASS_DECL_BASE bool unhook_window_create()
{

   if (t_pwndInit != NULL)
   {
      t_pwndInit = NULL;
      return FALSE;   // was not successfully hooked
   }
   return TRUE;
}

#define ___TEMP_CLASS_NAME_SIZE 4096

__declspec(thread) unichar t_szTempClassName[___TEMP_CLASS_NAME_SIZE] = { 0 };

CLASS_DECL_BASE const unichar * __register_window_class(::aura::application * papp, UINT nClassStyle, HCURSOR hCursor, HBRUSH hbrBackground, HICON hIcon)
{
   // Returns a temporary string name for the class
   //  Save in a string if you want to use it for a long time
   LPWSTR lpszName = t_szTempClassName;

   // generate a synthetic name for this class

   if (hCursor == NULL && hbrBackground == NULL && hIcon == NULL)
   {
      C_RUNTIME_ERRORCHECK_SPRINTF(_snwprintf_s(lpszName, ___TEMP_CLASS_NAME_SIZE, ___TEMP_CLASS_NAME_SIZE - 1, L"::core:::%p:%x", papp->m_hinstance, nClassStyle));
   }
   else
   {
      C_RUNTIME_ERRORCHECK_SPRINTF(_snwprintf_s(lpszName, ___TEMP_CLASS_NAME_SIZE, ___TEMP_CLASS_NAME_SIZE - 1, L"::core:::%p:%x:%p:%p:%p", papp->m_hinstance, nClassStyle,
         hCursor, hbrBackground, hIcon));
   }

   // see if the class already exists
   WNDCLASSW wndcls;
   if (::GetClassInfoW(papp->m_paxissystem->m_hinstance, lpszName, &wndcls))
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
   wndcls.hInstance = NULL;
   wndcls.hIcon = hIcon;
   //wndcls.hCursor = hCursor;
   wndcls.hCursor = NULL;
   wndcls.hbrBackground = hbrBackground;
   wndcls.lpszMenuName = NULL;
   wndcls.lpszClassName = lpszName;
   if (!__register_class(&wndcls))
      throw resource_exception(get_thread_app());

   // return thread-local pointer
   return lpszName;
}


__STATIC void CLASS_DECL_BASE
__handle_activate(::window_sp pwindow, WPARAM nState, ::window_sp pWndOther)
{
   ASSERT(pwindow != NULL);

   // send WM_ACTIVATETOPLEVEL when top-level parents change
   if (!(NODE_WINDOW(pwindow)->GetStyle() & WS_CHILD))
   {
      sp(::user::interaction) pTopLevel = NODE_WINDOW(pwindow)->GetTopLevel();
      if (pTopLevel && (pWndOther == NULL || !::IsWindow(NODE_WINDOW(pWndOther)->get_handle()) || pTopLevel != NODE_WINDOW(pWndOther)->GetTopLevel()))
      {
         // lParam points to interaction_impl getting the WM_ACTIVATE message and
         //  oswindow_Other from the WM_ACTIVATE.
         oswindow oswindow_2[2];
         oswindow_2[0] = NODE_WINDOW(pwindow)->get_handle();
         if (pWndOther == NULL || NODE_WINDOW(pWndOther) == NULL)
         {
            oswindow_2[1] = NULL;
         }
         else
         {
            oswindow_2[1] = NODE_WINDOW(pWndOther)->get_handle();
         }
         // send it...
         pTopLevel->send_message(WM_ACTIVATETOPLEVEL, nState, (LPARAM)&oswindow_2[0]);
      }
   }
}

//__STATIC bool CLASS_DECL_BASE
//__handle_set_cursor(::window_sp pwindow,UINT nHitTest,UINT nMsg)
//{
//   if(nHitTest == HTERROR &&
//      (nMsg == WM_LBUTTONDOWN || nMsg == WM_MBUTTONDOWN ||
//      nMsg == WM_RBUTTONDOWN))
//   {
//      // activate the last active interaction_impl if not active
//      sp(::user::interaction) pLastActive = NODE_WINDOW(pwindow)->GetTopLevel();
//      if(pLastActive != NULL)
//         pLastActive = pLastActive->GetLastActivePopup();
//      if(pLastActive != NULL &&
//         pLastActive != ::windows::interaction_impl::GetForegroundWindow() &&
//         pLastActive->is_window_enabled())
//      {
//         pLastActive->SetForegroundWindow();
//         return TRUE;
//      }
//   }
//   return FALSE;
//}



/////////////////////////////////////////////////////////////////////////////
// Standard init called by WinMain

__STATIC bool CLASS_DECL_BASE __register_with_icon(WNDCLASSW* pWndCls,
   const unichar * lpszClassName, UINT nIDIcon)
{
   pWndCls->lpszClassName = lpszClassName;
   pWndCls->hIcon = ::LoadIconW(NULL, MAKEINTRESOURCEW(32512));
   return __register_class(pWndCls);
}


wstring CLASS_DECL_BASE get_user_interaction_window_class(::user::interaction * pui)
{

   ::user::interaction::e_type etype = pui->get_window_type();

   WNDCLASSW wndcls;
   memset(&wndcls, 0, sizeof(WNDCLASSW));   // start with NULL defaults
   wndcls.lpfnWndProc = DefWindowProcW;
   wndcls.hInstance = pui->m_pauraapp->m_hinstance;

   INITCOMMONCONTROLSEX init;
   init.dwSize = sizeof(init);
   if (etype == ::user::interaction::type_frame || etype == ::user::interaction::type_view)
   {
      // SDI Frame or MDI Child windows or views - normal colors
      wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
      //wndcls.style = CS_HREDRAW | CS_VREDRAW;
      wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
      if (__register_with_icon(&wndcls, gen_WndFrameOrView, 0))
      {
         return wndcls.lpszClassName;
      }
   }

   return __register_window_class(pui->get_app(), 0);

}


/////////////////////////////////////////////////////////////////////////////
// Special WndProcs (activation handling & gray dialogs)


//LRESULT CALLBACK
//__activation_window_procedure(oswindow oswindow,UINT nMsg,WPARAM wParam,LPARAM lParam)
//{
//   WNDPROC oldWndProc = (WNDPROC)::GetProp(oswindow,gen_OldWndProc);
//   ASSERT(oldWndProc != NULL);
//
//   LRESULT lResult = 0;
//   try
//   {
//      bool bCallDefault = TRUE;
//      switch(nMsg)
//      {
//      case WM_INITDIALOG:
//      {
//                           rect rectOld;
//                           ::window_sp pwindow = System.ui_from_handle(oswindow);
//                           bCallDefault = FALSE;
//                           lResult = CallWindowProc(oldWndProc,oswindow,nMsg,wParam,lParam);
//      }
//         break;
//
//      case WM_ACTIVATE:
//         __handle_activate(System.ui_from_handle(oswindow),wParam,
//            System.ui_from_handle((::oswindow) lParam));
//         break;
//
//      case WM_SETCURSOR:
////         bCallDefault = !__handle_set_cursor(System.ui_from_handle(oswindow),
//  //          (int16_t)LOWORD(lParam),HIWORD(lParam));
//         break;
//
//      case WM_NCDESTROY:
//         SetWindowLongPtr(oswindow,GWLP_WNDPROC,reinterpret_cast<int_ptr>(oldWndProc));
//         RemoveProp(oswindow,gen_OldWndProc);
//         GlobalDeleteAtom(GlobalFindAtom(gen_OldWndProc));
//         break;
//      }
//
//      // call original wndproc for default handling
//      if(bCallDefault)
//         lResult = CallWindowProc(oldWndProc,oswindow,nMsg,wParam,lParam);
//   }
//   catch(::exception::base * pe)
//   {
//      // handle exception
//      MSG msg;
//      msg.hwnd = oswindow;
//      msg.message = nMsg;
//      msg.wParam = wParam;
//      msg.lParam = lParam;
//
//      //lResult = __process_window_procedure_exception(pe, &msg);
//      //      TRACE(::aura::trace::category_AppMsg, 0, "Warning: Uncaught exception in __activation_window_procedure (returning %ld).\n",
//      //       lResult);
//      pe->Delete();
//   }
//
//
//   return lResult;
//}




/////////////////////////////////////////////////////////////////////////////
// Additional helpers for WNDCLASS init

// like RegisterClass, except will automatically call UnregisterClass
bool CLASS_DECL_BASE __register_class(WNDCLASSW* lpWndClass)
{

   WNDCLASSW wndcls;

   if (GetClassInfoW(lpWndClass->hInstance, lpWndClass->lpszClassName, &wndcls))
   {
      return TRUE;
   }

   if (!::RegisterClassW(lpWndClass))
   {
      return FALSE;
   }

   bool bRet = TRUE;


   return bRet;
}









void __term_windowing()
{

   if (t_hHookOldCbtFilter != NULL)
   {
      ::UnhookWindowsHookEx(t_hHookOldCbtFilter);
      t_hHookOldCbtFilter = NULL;
   }

}
