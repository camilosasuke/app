#pragma once


#include "user_window_util.h"


namespace user
{

   
   class menu_base_item;


   class CLASS_DECL_BASE interaction_base :
      virtual public ::user::primitive,
      virtual public ::user::elemental,
      virtual public ::user::schema
   {
   public:


      enum RepositionFlags
      {

         reposDefault = 0,
         reposQuery = 1,
         reposExtra = 2,
         reposNoPosLeftOver = 0x8000

      };



      enum EProperty
      {

         PropertyBaseWndInterface = 0,
         PropertyDrawBaseWndInterface

      };


      enum EGeneralEvent
      {

         GeneralEventPosCreate1,
         GeneralEventPosCreate2,
         GeneralEventPosCreate3,
         GeneralEventPosCreate4,
         GeneralEventPosCreate5

      };

      enum e_simple_command
      {

         simple_command_load_window_rect,
         simple_command_update_frame_title,
         simple_command_set_edit_file,
         simple_command_layout,
         simple_command_full_screen

      };


      enum e_type
      {

         type_window,
         type_frame,
         type_view

      };

      enum EOptimize
      {

         OptimizeNone,
         OptimizeThis

      };

      enum AdjustType
      {
         
         adjustBorder = 0, 
         adjustOutside = 1
      
      };


      interaction_base();
      interaction_base(::aura::application * papp);
      virtual ~interaction_base();


      virtual void _001OnTimer(::timer * ptimer) override;


      virtual bool create_message_queue(const char * pszName);


#if defined(METROWIN) || defined(APPLE_IOS) || defined(ANDROID)
      
      virtual bool initialize_native_window(::user::native_window_initialize * pinitialize);
      
#endif


//      virtual bool is_window_enabled();
      virtual bool enable_window(bool bEnable = true);
      virtual void _000OnDraw(::draw2d::graphics * pgraphics) override;

      virtual void PreSubClassWindow();

      //virtual bool GetFocusRect(LPRECT lprect);
      virtual bool GetClientRect(LPRECT lprect);
      virtual bool GetClientRect(RECT64 * lprect);
      virtual bool GetWindowRect(LPRECT lprect) override;
      virtual bool GetWindowRect(RECT64 * lprect) override;
      virtual bool ClientToScreen(LPRECT lprect);
      virtual bool ClientToScreen(RECT64 * lprect);
      virtual bool ClientToScreen(LPPOINT lppoint);
      virtual bool ClientToScreen(POINT64 * lppoint);
      virtual bool ScreenToClient(LPRECT lprect);
      virtual bool ScreenToClient(RECT64 * lprect);
      virtual bool ScreenToClient(LPPOINT lppoint);
      virtual bool ScreenToClient(POINT64 * lppoint);
      virtual rect GetWindowRect() override;
      virtual rect64 GetWindowRect64() override;

      virtual bool SetPlacement(const RECT & lprect,UINT nFlags = SWP_SHOWWINDOW) override;
      virtual bool RepositionWindow(const RECT & rect,UINT nFlags = SWP_SHOWWINDOW);
      virtual bool RepositionWindow(int32_t x,int32_t y,int32_t cx,int32_t cy,UINT nFlags = SWP_SHOWWINDOW);
      virtual bool MoveWindow(int32_t x,int32_t y,UINT nFlags = SWP_SHOWWINDOW);
      virtual bool MoveWindow(POINT pt,UINT nFlags = SWP_SHOWWINDOW);
      virtual bool SizeWindow(int32_t x,int32_t y,UINT nFlags = SWP_SHOWWINDOW);
      virtual bool SizeWindow(SIZE sz,UINT nFlags = SWP_SHOWWINDOW);
      virtual bool ResizeWindow(int32_t cx,int32_t cy,UINT nFlags = SWP_SHOWWINDOW);
      virtual bool ResizeWindow(SIZE sz,UINT nFlags = SWP_SHOWWINDOW);
      virtual bool SetWindowPos(int_ptr z,const RECT & rect,UINT nFlags = SWP_SHOWWINDOW);
      virtual bool SetWindowPos(int_ptr z,int32_t x,int32_t y,int32_t cx,int32_t cy,UINT nFlags = SWP_SHOWWINDOW);
      virtual bool defer_set_window_pos(int32_t z,int32_t x,int32_t y,int32_t cx,int32_t cy,UINT nFlags); // only set_windows_pos if GetParent()->ScreenToClient(get_window_rect) different of rect(x, y, cx, cy)      virtual bool set_placement(LPRECT lprect);
      //virtual int32_t SetWindowRgn(HRGN hRgn,bool bRedraw);
      //virtual int32_t GetWindowRgn(HRGN hRgn);


      virtual void install_message_handling(::message::dispatch * pinterface) override;

      virtual void _on_start_user_message_handler() override;


      virtual void UpdateWindow();
      virtual void Invalidate(bool bErase = TRUE);


      virtual bool RedrawOptimize(LPRECT lprectOut,LPCRECT lpcrect1,LPCRECT lpcrect2);
      virtual void RedrawOptimize(rect_array & array);







      void Optimize008(
         ::user::oswindow_tree::Array & oswindowtreea,
         const RECT & rect);

      EOptimize Optimize008(
         ::user::oswindow_tree::Array & oswindowtreea,
         HRGN hrgn);

      EOptimize Optimize008(
         ::user::oswindow_tree & oswindowtree,
         HRGN hrgn);



      void TwfGetWndArray(::user::interaction_spa & wndpa);
      void TwfGetWndArray(::user::oswindow_array & oswindowa);

      virtual bool Redraw(rect_array & recta);
      virtual bool Redraw(LPCRECT lprect = NULL,::draw2d::region * prgn = NULL);
      virtual bool Redraw(::draw2d::graphics * pgraphics);

      void _001BaseWndInterfaceMap();

      virtual void message_handler(signal_details * pobj) override;
      virtual LRESULT message_handler(LPMESSAGE lpmessage);
      virtual void on_select();

      virtual bool is_place_holder();

      ::visual::e_cursor get_cursor();
      void set_cursor(::visual::e_cursor ecursor);


      void set_timer(spa(::aura::timer_item) timera);


      virtual bool IsWindow() const;

      virtual LONG get_window_long(int32_t nIndex) const;
      virtual LONG set_window_long(int32_t nIndex,LONG lValue);

      virtual LONG_PTR get_window_long_ptr(int32_t nIndex) const;
      virtual LONG_PTR set_window_long_ptr(int32_t nIndex,LONG_PTR lValue);



      virtual ::user::interaction * first_child();
      virtual ::user::interaction * last_child();
      virtual ::user::interaction * last_sibling();
      virtual ::user::interaction * next_sibling();
      virtual ::user::interaction * previous_sibling();
      virtual ::user::interaction * first_sibling();
      virtual ::user::interaction * next_sibling(::user::interaction * pui);
      virtual ::user::interaction * previous_sibling(::user::interaction * pui);

      virtual ::user::interaction * bottom_child();
      virtual ::user::interaction * top_child();
      virtual ::user::interaction * bottom_sibling();
      virtual ::user::interaction * under_sibling();
      virtual ::user::interaction * above_sibling();
      virtual ::user::interaction * top_sibling();
      virtual ::user::interaction * above_sibling(::user::interaction * pui);
      virtual ::user::interaction * under_sibling(::user::interaction * pui);




      virtual void mouse_hover_add(::user::interaction * pinterface);
      virtual void mouse_hover_remove(::user::interaction * pinterface);


      virtual bool CheckAutoCenter();

      virtual void track_mouse_hover();
      virtual void track_mouse_leave();

      // dialog support
      void UpdateDialogControls(command_target* pTarget,bool bDisableIfNoHndler);
      virtual void CenterWindow(::user::interaction * pAlternateOwner = NULL);
      virtual id   run_modal_loop(::user::interaction * pui,uint32_t dwFlags = 0,::object * pliveobject = NULL);
      virtual id   RunModalLoop(uint32_t dwFlags = 0,::object * pliveobject = NULL);
      virtual id   _001RunModalLoop(uint32_t dwFlags = 0,::object * pliveobject = NULL);
      virtual bool ContinueModal(int32_t iLevel);
      virtual void EndModalLoop(id nResult);
      virtual void EndAllModalLoops(id nResult);

      virtual bool BaseOnControlEvent(::user::control_event * pevent) override;

      // Dialog data support
      virtual bool update_data(bool bSaveAndValidate = true);


      virtual ::user::interaction * GetFocus();


      virtual bool on_keyboard_focus(::user::elemental * pfocus) override;


      virtual void _001UpdateWindow(bool bUpdateBuffer = true);


      virtual void _001WindowMinimize(bool bNoActivate);
      virtual void _001WindowMaximize();
      virtual void _001WindowFullScreen();
      virtual void _001WindowRestore();




      virtual void on_layout();
      virtual void on_translate();
      virtual void on_do_show_flags();


      virtual void BringToTop(int32_t nCmdShow);
      virtual bool BringWindowToTop();

#ifdef WINDOWSEX
      virtual bool GetWindowPlacement(WINDOWPLACEMENT* lpwndpl);
      virtual bool SetWindowPlacement(const WINDOWPLACEMENT* lpwndpl);
#endif

      //virtual bool SendChildNotifyLastMsg(LRESULT* pResult = NULL);


      virtual bool pre_create_window(::user::create_struct& cs);


      virtual bool subclass_window(oswindow posdata);
      virtual oswindow unsubclass_window();


      virtual bool create_window(const RECT & rect, ::user::interaction *pparent,id id);
      virtual bool create_window(const char * lpszClassName, const char * lpszWindowName,uint32_t dwStyle, const RECT & rect, ::user::interaction * pParentWnd,id id, ::create * pcreate = NULL);
      virtual bool create_window_ex(uint32_t dwExStyle,const char * lpszClassName, const char * lpszWindowName,uint32_t dwStyle, const RECT & rect,  ::user::interaction * pParentWnd,id id,LPVOID lpParam = NULL);
      virtual void CalcWindowRect(LPRECT lpClientRect,UINT nAdjustType = adjustBorder);

      virtual bool IsTopParentActive();
      virtual void ActivateTopParent();

      virtual bool DestroyWindow();


#ifdef WINDOWS

      virtual bool RedrawWindow(LPCRECT lpRectUpdate = NULL, ::draw2d::region* prgnUpdate = NULL, UINT flags = RDW_INVALIDATE | RDW_ERASE);

#else

      virtual bool RedrawWindow(LPCRECT lpRectUpdate = NULL, ::draw2d::region* prgnUpdate = NULL, UINT flags = 0);

#endif


//      virtual void UpdateWindow();
      virtual void SetRedraw(bool bRedraw = TRUE);
      virtual bool GetUpdateRect(LPRECT lpRect,bool bErase = FALSE);
      virtual int32_t GetUpdateRgn(::draw2d::region* pRgn,bool bErase = FALSE);
//      virtual void Invalidate(bool bErase = TRUE);
      virtual void InvalidateRect(LPCRECT lpRect,bool bErase = TRUE);
      virtual void InvalidateRgn(::draw2d::region* pRgn,bool bErase = TRUE);
      virtual void ValidateRect(LPCRECT lpRect);
      virtual void ValidateRgn(::draw2d::region* pRgn);
      virtual void ShowOwnedPopups(bool bShow = TRUE);


      virtual uint32_t GetStyle() const;
      virtual uint32_t GetExStyle() const;
      virtual LRESULT Default();

      virtual LRESULT send(::message::base * pbase);
      virtual bool post(::message::base * pbase);
      
      virtual LRESULT send_message(UINT uiMessage, WPARAM wparam = 0, lparam lparam = 0);
      virtual LRESULT message_call(UINT uiMessage, WPARAM wparam = 0, lparam lparam = 0);

#ifdef LINUX

      virtual LRESULT send_message(XEvent * pevent);

#endif

      virtual bool post_message(UINT message,WPARAM wParam = 0,lparam lParam = 0);
      virtual bool post_simple_command(e_simple_command ecommand,lparam lParam = 0);

      virtual bool ModifyStyle(uint32_t dwRemove,uint32_t dwAdd,UINT nFlags = 0);
      virtual bool ModifyStyleEx(uint32_t dwRemove,uint32_t dwAdd,UINT nFlags = 0);
      virtual bool ShowWindow(int32_t nCmdShow);

      // timer Functions
      virtual bool SetTimer(uint_ptr nIDEvent,UINT nElapse,PFN_TIMER pfnTimer);
      virtual bool KillTimer(uint_ptr nIDEvent);

      virtual bool is_window_enabled();

      virtual void _001Print(::draw2d::graphics * pgraphics);
      virtual void _001DrawThis(::draw2d::graphics * pgraphics);
      virtual void _001DrawChildren(::draw2d::graphics * pgraphics);
      virtual void _001OnDraw(::draw2d::graphics * pgraphics) override;
      virtual void _008OnDraw(::draw2d::graphics * pgraphics);
      virtual void draw_control_background(::draw2d::graphics * pgraphics);


      virtual bool IsAscendant(const interaction * puiIsAscendant) const;
      virtual bool IsParent(const interaction * puiIsParent) const;
      virtual bool IsChild(const interaction * puiIsChild) const;
      virtual bool IsDescendant(const interaction * puiIsDescendant) const;


      virtual id GetDlgCtrlId() const;
      virtual id SetDlgCtrlId(class id id);

      virtual bool SetCapture(::user::interaction * pui = NULL);
      virtual bool ReleaseCapture();
      virtual ::user::interaction * GetCapture();
      


      virtual bool has_focus();
      virtual bool SetFocus();
      virtual bool SetForegroundWindow();
      virtual ::user::interaction * GetActiveWindow();
      virtual ::user::interaction * SetActiveWindow();


      virtual ::user::interaction * GetDescendantWindow(id id) const;

      virtual void set_window_text(const char * lpszString);
      virtual strsize get_window_text(char * lpszStringBuf,strsize nMaxCount);
      virtual string get_window_text();
      virtual void get_window_text(string & rString);
      virtual strsize get_window_text_length();


      virtual bool IsWindowVisible();
      virtual bool is_this_visible();



      virtual bool _001IsPointInside(point64 pt);
      virtual ::user::interaction * _001FromPoint(point64 pt,bool bTestedIfParentVisible = false);

      virtual void OnLinkClick(const char * psz,const char * pszTarget = NULL);

      ::user::interaction * get_child_by_name(const char * pszName,int32_t iLevel = -1);
      ::user::interaction * get_child_by_id(id id,int32_t iLevel = -1);


      virtual ::user::interaction * get_wnd() const override;
      virtual ::user::interaction * get_wnd(UINT nCmd) const;
      virtual ::user::interaction_impl * get_impl() const override;

      virtual ::user::interaction * SetParent(::user::interaction * pui);
      virtual ::user::interaction * SetOwner(::user::interaction * pui);


      virtual ::user::interaction * GetTopWindow() const;
      virtual ::user::interaction * GetParent() const override;
      virtual ::user::interaction * GetTopLevel() const;
      virtual ::user::interaction * GetParentTopLevel() const;
      virtual ::user::interaction * EnsureTopLevel();
      virtual ::user::interaction * EnsureParentTopLevel();
      virtual ::user::interaction * GetOwner() const;
      virtual ::user::interaction * GetParentOwner() const;
      virtual ::user::interaction * GetTopLevelOwner() const;
      virtual ::user::frame_window * GetFrame() const;
      virtual ::user::frame_window * GetParentFrame() const;
      virtual ::user::frame_window * GetTopLevelFrame() const;
      virtual ::user::frame_window * GetParentTopLevelFrame() const;
      virtual ::user::frame_window * EnsureParentFrame();



      virtual void SendMessageToDescendants(UINT message,WPARAM wParam = 0,lparam lParam = 0,bool bDeep = TRUE,bool bOnlyPerm = FALSE);
      virtual void pre_translate_message(signal_details * pobj) override;


      virtual int32_t get_descendant_level(::user::interaction * pui);
      virtual bool is_descendant(::user::interaction * pui,bool bIncludeSelf = false);
      virtual ::user::interaction * get_focusable_descendant(::user::interaction * pui = NULL);


//      virtual interaction_impl * get_wnd() const;




      virtual void RepositionBars(UINT nIDFirst,UINT nIDLast,id nIDLeftOver,UINT nFlag = reposDefault,LPRECT lpRectParam = NULL,LPCRECT lpRectClient = NULL,bool bStretch = TRUE);


      virtual ::user::interaction * ChildWindowFromPoint(POINT point);
      virtual ::user::interaction * ChildWindowFromPoint(POINT point,UINT nFlags);


#ifdef WINDOWSEX
      virtual ::user::interaction * get_next_window(UINT nFlag = GW_HWNDNEXT);
#else
      virtual ::user::interaction * get_next_window(UINT nFlag = 0);
#endif

      virtual ::user::interaction * get_next(bool bIgnoreChildren = false,int32_t * piLevel = NULL);

      virtual ::user::interaction * GetLastActivePopup();

      virtual bool is_message_only_window() const;

      virtual void pre_subclass_window();

      // for custom cleanup after WM_NCDESTROY
      virtual void PostNcDestroy();

      virtual LRESULT DefWindowProc(UINT uiMessage,WPARAM wparam,lparam lparam);

      virtual LRESULT call_message_handler(UINT message,WPARAM wparam,LPARAM lparam);

//      virtual void message_handler(signal_details * pobj);
//      virtual LRESULT message_handler(LPMESSAGE lpmessage);
      virtual void GuieProc(signal_details * pobj);

      virtual void _001DeferPaintLayeredWindowBackground(::draw2d::graphics * pgraphics) override;

      virtual void _001OnDeferPaintLayeredWindowBackground(::draw2d::graphics * pgraphics) override;


      oswindow get_safe_handle() const;
      virtual oswindow get_handle() const override;
      virtual bool attach(oswindow oswindow_New);
      virtual oswindow detach();


      virtual bool can_merge(::user::interaction * pui);
      virtual bool merge(::user::interaction * pui);


      virtual void _001OnTriggerMouseInside();

      //#ifdef METROWIN
      //      Agile<Windows::UI::Core::CoreWindow> get_os_window();
      //#endif


      virtual void set_viewport_org(::draw2d::graphics * pgraphics);

      virtual void viewport_screen_to_client(POINT * ppt);
      virtual void viewport_client_to_screen(POINT * ppt);
      virtual void viewport_client_to_screen(RECT * ppt);
      virtual void viewport_screen_to_client(RECT * ppt);


      virtual string get_window_default_matter();
      virtual string get_window_icon_matter();
      virtual uint32_t get_window_default_style();
      virtual e_type get_window_type();


      virtual bool on_simple_command(e_simple_command ecommand,lparam lparam,LRESULT & lresult);


      // Window-Management message handler member functions
      virtual bool OnCommand(::message::base * pbase);
      virtual bool OnNotify(::message::base * pbase);
      virtual bool OnChildNotify(::message::base * pbase);



      virtual bool is_selected(::data::item * pitem);

      virtual sp(place_holder) place(::user::interaction * pui);

      virtual bool _001HasCommandHandler(id id) override;



      virtual bool track_popup_menu(::user::menu_base_item * pitem,int32_t iFlags, POINT pt);
      virtual bool track_popup_menu(::xml::node * lpnode,int32_t iFlags, POINT pt);
      virtual bool track_popup_xml_matter_menu(const char * pszMatter,int32_t iFlags, POINT pt);

      virtual bool track_popup_menu(::user::menu_base_item * pitem,int32_t iFlags,signal_details * pobj);
      virtual bool track_popup_menu(::xml::node * lpnode,int32_t iFlags,signal_details * pobj);
      virtual bool track_popup_xml_matter_menu(const char * pszMatter,int32_t iFlags,signal_details * pobj);

      virtual bool track_popup_menu(::user::menu_base_item * pitem,int32_t iFlags);
      virtual bool track_popup_menu(::xml::node * lpnode,int32_t iFlags);
      virtual bool track_popup_xml_matter_menu(const char * pszMatter,int32_t iFlags);


      virtual void WfiEnableFullScreen(bool bEnable = true);
      virtual bool WfiIsFullScreen();
      virtual bool WfiIsFullScreenEnabled();
      virtual bool WfiIsZoomed();
      virtual bool WfiIsIconic();

      virtual bool Wfi(EAppearance eapperance = AppearanceCurrent);

      virtual bool WfiDock(EAppearance eapperance);
      virtual bool WfiClose();
      virtual bool WfiRestore(bool bForceNormal = false);
      virtual bool WfiMinimize();
      virtual bool WfiMaximize();
      virtual bool WfiFullScreen();
      virtual bool WfiUp();
      virtual bool WfiDown();
      virtual bool WfiNotifyIcon();

      virtual EAppearance get_appearance();
      virtual EAppearance get_appearance_before();

      virtual bool set_appearance(EAppearance eappearance);
      virtual bool set_appearance_before(EAppearance eappearance);


      virtual void show_keyboard(bool bShow = true);

      virtual void keep_alive(::object * pliveobject = NULL);


      virtual window_graphics * get_window_graphics() override;


      virtual bool is_composite() override;


      //virtual void _user_message_handler(signal_details * pobj);

      //virtual PFN_DISPATCH_MESSAGE_HANDLER _calc_user_message_handler();

      virtual bool has_pending_graphical_update();
      virtual void on_after_graphical_update();

      

   };


} // namespace user





