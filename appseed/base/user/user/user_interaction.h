#pragma once


#include "user_interaction_base.h"
#include "user_interaction_impl_base.h"

#define MESSAGE_POST_MESSAGE  14000

#define SWP_CHANGEVIEWPORTOFFSET  0x100000

namespace message
{


   class key;
   class drag_and_drop;


} // namespace message


namespace user
{

   
   class tooltip;


   class CLASS_DECL_BASE alpha_source
   {
   public:


      virtual double get_alpha(::user::interaction * puiTarget);
      virtual void on_alpha_target_initial_frame_position();


   };


   class CLASS_DECL_BASE interaction:
      virtual public interaction_base
   {
   public:


      enum e_non_client
      {

         non_client_background = 1,
         non_client_focus_rect = 2,

      };

      enum e_updown
      {
         updown_normal_frame,
         updown_none,
         updown_up,
         updown_down,
      };

      flags < e_non_client >              m_flagNonClient;

      bool                                m_bMouseHover;

      bool                                m_bTransparentMouseEvents;
      bool                                m_bRedraw;
      DWORD                               m_dwLastRedraw;
      bool                                m_bCursorRedraw;
      point                               m_ptCursor;

      bool                                m_bWorkspaceFullScreen;
      point                               m_ptScrollPassword1;
      bool                                m_bHideOnTransparentFrame;
      bool                                m_bMoving;
      bool                                m_bMoveWindow;
      bool                                m_bMayProDevian;
      bool                                m_bVoidPaint;
      bool                                m_bRedrawing;
      bool                                m_bLockWindowUpdate;
      bool                                m_bEnableSaveWindowRect;
      bool                                m_bLayoutEnable;
      bool                                m_bNeedLayout;
      bool                                m_bRedrawOnVisible;


      alpha_source *                      m_palphasource;

      int32_t                             m_iItemHeight;

      point                               m_ptMoveCursor;

      bool                                m_bDefaultWalkPreTranslateParentTree;

      draw_interface *                    m_pdrawinterfaceBackground;
      bool                                m_bBackgroundBypass;

      uint32_t                            m_dwLastFullUpdate;

      bool                                m_bSizeMove;
      uint32_t                            m_dwLastSizeMove;

      ::user::interaction *               m_pparent;

      EAppearance                         m_eappearance;
      EAppearance                         m_eappearanceBefore;
      sp(interaction_impl_base)           m_pimpl;


      interaction_spa                     m_uiptraChild;
      string                              m_strName;
      id                                  m_id;
      interaction *                       m_puiOwner;
      UINT                                m_nFlags;      // see WF_ flags above
      bool                                m_bCursorInside;
      ::visual::e_cursor                  m_ecursor;
      int32_t                             m_iModal;
      int32_t                             m_iModalCount;
      bool                                m_bRectOk;
      sp(::axis::session)                 m_psession;
      bool                                m_bMessageWindow;

      bool                                m_bCreated;

      string                              m_strWindowText;

      ref_array < ::thread >              m_threadptraModal;

      id                                  m_idModalResult; // for return values from interaction_impl::RunModalLoop

      int32_t                             m_nModalResult; // for return values from ::interaction_impl::RunModalLoop

      sp(interaction)                     m_ptooltip;




      interaction();
      interaction(::aura::application * papp);
      virtual ~interaction();


      tooltip *         get_tooltip();


      void user_interaction_common_construct();

      virtual bool defer_check_layout();
      virtual bool check_need_layout();
      virtual void clear_need_layout();
      virtual void set_need_layout(bool bAscendants = false);
      virtual void layout();

      virtual bool defer_check_translation();
      virtual bool check_need_translation();
      virtual void clear_need_translation();
      virtual void translate();

      virtual bool defer_check_show_flags();
      virtual bool check_show_flags();
      virtual void clear_show_flags();
      virtual void do_show_flags();

      virtual bool defer_check_zorder();
      virtual bool check_need_zorder();
      virtual void clear_need_zorder();
      virtual void zorder();

      virtual bool create_message_queue(const char * pszName) override;


      virtual bool has_action_hover();
      virtual bool has_text_input();
      virtual COLORREF get_action_hover_border_color();

      virtual bool kick_queue();

#if defined(METROWIN) || defined(APPLE_IOS) || defined(ANDROID)

      virtual bool initialize_native_window(::user::native_window_initialize * pinitialize) override;

#endif



      virtual void on_select() override;

      virtual bool is_place_holder() override;

      ::visual::e_cursor get_cursor();
      void set_cursor(::visual::e_cursor ecursor);


      void set_timer(spa(::aura::timer_item) timera);

      virtual bool set_may_pro_devian(bool bSet = true);
      virtual void on_set_may_pro_devian();

      virtual bool IsWindow() const override;

      virtual void ExitHelpMode();

      virtual void BaseOnControlEvent(::message::base * pbase);

      virtual LONG get_window_long(int32_t nIndex) const override;
      virtual LONG set_window_long(int32_t nIndex,LONG lValue) override;

      virtual LONG_PTR get_window_long_ptr(int32_t nIndex) const override;
      virtual LONG_PTR set_window_long_ptr(int32_t nIndex,LONG_PTR lValue) override;

      virtual bool on_before_set_parent(::user::interaction * pinterface);
      virtual void on_set_parent(::user::interaction * pinterface);

      virtual ::user::elemental * first_child_elemental() override;
      virtual ::user::elemental * top_elemental() override;
      virtual ::user::elemental * under_elemental() override;
      virtual ::user::elemental * above_elemental() override;
      virtual ::user::elemental * next_elemental() override;
      virtual ::user::elemental * previous_elemental() override;



      virtual ::user::interaction * first_child() override;
      virtual ::user::interaction * last_child() override;
      virtual ::user::interaction * last_sibling() override;
      virtual ::user::interaction * next_sibling() override;
      virtual ::user::interaction * previous_sibling() override;
      virtual ::user::interaction * first_sibling() override;
      virtual ::user::interaction * next_sibling(::user::interaction * pui) override;
      virtual ::user::interaction * previous_sibling(::user::interaction * pui) override;


      virtual void mouse_hover_add(::user::interaction * pinterface) override;
      virtual void mouse_hover_remove(::user::interaction * pinterface) override;


      template < typename TYPE >
      TYPE * typed_descedant(::user::interaction * puiExclude = NULL)
      {

         for (auto & pui : m_uiptraChild)
         {

            if (pui != puiExclude)
            {

               TYPE * p = dynamic_cast < TYPE * > (pui.m_p);

               if (p != NULL)
               {

                  return p;

               }

            }


         }

         for (auto & pui : m_uiptraChild)
         {

            if (pui != puiExclude)
            {

               TYPE * p = pui->typed_descedant < TYPE >(pui.m_p);

               if (p != NULL)
               {

                  return p;

               }

            }


         }

         return NULL;

      }


      template < typename TYPE >
      TYPE * _001TypedWindow() // search descedants first, then ascendants descedants.
      {

         ::user::interaction * pui = this;

         ::user::interaction * puiExclude = NULL;

         while (pui != NULL)
         {

            TYPE * p = pui->typed_descedant < TYPE >(puiExclude);

            if (p != NULL)
            {

               return p;

            }

            puiExclude = pui;

            pui = pui->GetParent();

            if (pui == NULL)
            {

               return NULL;

            }

            p = dynamic_cast < TYPE * > (pui);

            if (p != NULL)
            {

               return p;

            }

         }

         return NULL;

      }


      virtual bool CheckAutoCenter() override;

      virtual void track_mouse_hover() override;
      virtual void track_mouse_leave() override;

      // dialog support
      void UpdateDialogControls(command_target* pTarget,bool bDisableIfNoHndler);
      virtual void CenterWindow(::user::interaction * pAlternateOwner = NULL) override;
      virtual id   run_modal_loop(::user::interaction * pui,uint32_t dwFlags = 0,::object * pliveobject = NULL) override;
      virtual id   RunModalLoop(uint32_t dwFlags = 0,::object * pliveobject = NULL) override;
      virtual id   _001RunModalLoop(uint32_t dwFlags = 0,::object * pliveobject = NULL) override;
      virtual bool ContinueModal(int32_t iLevel) override;
      virtual void EndModalLoop(id nResult) override;
      virtual void EndAllModalLoops(id nResult) override;

      virtual bool BaseOnControlEvent(::user::control_event * pevent) override;

      // Dialog data support
      virtual bool update_data(bool bSaveAndValidate = true) override;


      virtual ::user::interaction * get_os_focus_uie();


      virtual bool on_keyboard_focus(::user::elemental * pfocus) override;


      virtual void _001UpdateWindow(bool bUpdateBuffer = true) override;


      virtual void _001WindowMinimize(bool bNoActivate) override;
      virtual void _001WindowMaximize() override;
      virtual void _001WindowFullScreen() override;
      virtual void _001WindowRestore() override;
      virtual void _001WindowDock(::user::EAppearance eappearance);

      using ::user::interaction_base::GetWindowRect;
      virtual bool GetClientRect(LPRECT lprect) override;
      virtual bool GetClientRect(RECT64 * lprect) override;
      virtual bool GetWindowRect(LPRECT lprect) override;
      virtual bool GetWindowRect(RECT64 * lprect) override;
      virtual bool ClientToScreen(LPRECT lprect) override;
      virtual bool ClientToScreen(RECT64 * lprect) override;
      virtual bool ClientToScreen(LPPOINT lppoint) override;
      virtual bool ClientToScreen(POINT64 * lppoint) override;
      virtual bool ScreenToClient(LPRECT lprect) override;
      virtual bool ScreenToClient(RECT64 * lprect) override;
      virtual bool ScreenToClient(LPPOINT lppoint) override;
      virtual bool ScreenToClient(POINT64 * lprect) override;
      virtual bool SetPlacement(const RECT & rect,UINT nFlags = 0) override;
      virtual bool RepositionWindow(const RECT & rect, UINT nFlags = 0) override;
      virtual bool RepositionWindow(int32_t x,int32_t y,int32_t cx,int32_t cy,UINT nFlags = 0) override;
      virtual bool MoveWindow(int32_t x,int32_t y,UINT nFlags = 0) override;
      virtual bool MoveWindow(POINT pt,UINT nFlags = 0) override;
      virtual bool SizeWindow(int32_t x,int32_t y,UINT nFlags = 0) override;
      virtual bool SizeWindow(SIZE sz,UINT nFlags = 0) override;
      virtual bool ResizeWindow(int32_t cx,int32_t cy,UINT nFlags = 0) override;
      virtual bool ResizeWindow(SIZE sz,UINT nFlags = 0) override;
      virtual bool SetWindowPos(int_ptr z,const RECT & rect,UINT nFlags = 0) override;
      virtual bool SetWindowPos(int_ptr z,int32_t x,int32_t y,int32_t cx,int32_t cy,UINT nFlags = 0) override;
      virtual bool defer_set_window_pos(int32_t z,int32_t x,int32_t y,int32_t cx,int32_t cy,UINT nFlags) override; // only set_windows_pos if get_parent()->ScreenToClient(get_window_rect) different of rect(x, y, cx, cy)      virtual bool set_placement(LPRECT lprect) override;
      //virtual int32_t SetWindowRgn(HRGN hRgn,bool bRedraw) override;
      //virtual int32_t GetWindowRgn(HRGN hRgn) override;

      virtual void on_set_window_pos();

      virtual void on_layout() override;
      virtual void on_translate() override;
      virtual void on_do_show_flags() override;

      virtual void BringToTop(int32_t nCmdShow) override;
      virtual bool BringWindowToTop() override;

#ifdef WINDOWSEX
      virtual bool GetWindowPlacement(WINDOWPLACEMENT* lpwndpl);
      virtual bool SetWindowPlacement(const WINDOWPLACEMENT* lpwndpl);
#endif

      //virtual bool SendChildNotifyLastMsg(LRESULT* pResult = NULL);


      virtual bool pre_create_window(::user::create_struct& cs) override;


      virtual bool subclass_window(oswindow posdata) override;
      virtual oswindow unsubclass_window() override;


      virtual bool create_window(const RECT & rect, ::user::interaction *pparent,id id) override;
      virtual bool create_window(const char * lpszClassName, const char * lpszWindowName,uint32_t dwStyle,const RECT & rect,::user::interaction * pParentWnd,id id, ::create * pcreate = NULL) override;
      virtual bool create_window_ex(uint32_t dwExStyle = 0,const char * lpszClassName = NULL,const char * lpszWindowName= NULL,uint32_t dwStyle = 0,const RECT & rect=null_rect(),::user::interaction * pParentWnd=NULL,id id=::id(),LPVOID lpParam = NULL) override;
      enum AdjustType { adjustBorder = 0,adjustOutside = 1 };
      virtual void CalcWindowRect(LPRECT lpClientRect,UINT nAdjustType = adjustBorder) override;

      virtual bool IsTopParentActive() override;
      virtual void ActivateTopParent() override;

      virtual bool DestroyWindow() override;


#ifdef WINDOWS
      virtual bool RedrawWindow(LPCRECT lpRectUpdate = NULL,
         ::draw2d::region* prgnUpdate = NULL,
         UINT flags = RDW_INVALIDATE | RDW_ERASE);
#else
      virtual bool RedrawWindow(LPCRECT lpRectUpdate = NULL,
         ::draw2d::region* prgnUpdate = NULL,
         UINT flags = 0) override;
#endif


      virtual void register_drop_target();




      virtual void UpdateWindow() override;
      virtual void SetRedraw(bool bRedraw = TRUE) override;
      virtual bool GetUpdateRect(LPRECT lpRect,bool bErase = FALSE) override;
      virtual int32_t GetUpdateRgn(::draw2d::region* pRgn,bool bErase = FALSE) override;
      virtual void Invalidate(bool bErase = TRUE) override;
      virtual void InvalidateRect(LPCRECT lpRect,bool bErase = TRUE) override;
      virtual void InvalidateRgn(::draw2d::region* pRgn,bool bErase = TRUE) override;
      virtual void ValidateRect(LPCRECT lpRect) override;
      virtual void ValidateRgn(::draw2d::region* pRgn) override;
      virtual void ShowOwnedPopups(bool bShow = TRUE) override;


      virtual uint32_t GetStyle() const override;
      virtual uint32_t GetExStyle() const override;
      virtual bool ModifyStyle(uint32_t dwRemove,uint32_t dwAdd,UINT nFlags = 0) override;
      virtual bool ModifyStyleEx(uint32_t dwRemove,uint32_t dwAdd,UINT nFlags = 0) override;
      virtual LRESULT Default() override;

      virtual LRESULT send(::message::base * pbase) override;
      virtual bool post(::message::base * pbase) override;
      virtual LRESULT send_message(UINT uiMessage,WPARAM wparam = 0,lparam lparam = 0) override;
      virtual LRESULT message_call(UINT uiMessage, WPARAM wparam = 0, lparam lparam = 0) override;

#ifdef LINUX

      virtual LRESULT send_message(XEvent * pevent);

#endif

      virtual bool post_message(UINT message,WPARAM wParam = 0,lparam lParam = 0) override;
      virtual bool post_object(UINT message, WPARAM wParam, lparam lParam);
      virtual bool post_simple_command(e_simple_command ecommand,lparam lParam = 0) override;

      virtual bool ShowWindow(int32_t nCmdShow) override;

      // timer Functions
      virtual bool SetTimer(uint_ptr nIDEvent,UINT nElapse,PFN_TIMER pfnTimer) override;
      virtual bool KillTimer(uint_ptr nIDEvent) override;

      virtual bool is_window_enabled() override;
      virtual bool enable_window(bool bEnable = TRUE) override;

      virtual void _001Print(::draw2d::graphics * pgraphics) override;
      virtual void _000OnDraw(::draw2d::graphics * pgraphics) override;
      virtual void _001DrawThis(::draw2d::graphics * pgraphics) override;
      virtual void _001DrawChildren(::draw2d::graphics * pgraphics) override;
      virtual void _001OnNcDraw(::draw2d::graphics * pgraphics);
      virtual void _001CallOnDraw(::draw2d::graphics * pgraphics);
      virtual void _001OnDraw(::draw2d::graphics * pgraphics) override;
      // drawn after children
      virtual void _008CallOnDraw(::draw2d::graphics * pgraphics);
      virtual void _008OnDraw(::draw2d::graphics * pgraphics) override;
      virtual void _001OnClip(::draw2d::graphics * pgraphics);
      virtual void draw_control_background(::draw2d::graphics * pgraphics) override;

      virtual bool is_custom_draw();

      virtual id GetDlgCtrlId() const override;
      virtual id SetDlgCtrlId(class id id) override;


      virtual bool SetCapture(::user::interaction * pui = NULL) override;
      virtual bool ReleaseCapture() override;
      virtual ::user::interaction * GetCapture() override;


      virtual bool has_focus() override;
      virtual bool SetFocus() override;
      virtual ::user::interaction * GetFocus() override;
      virtual bool SetForegroundWindow() override;
      virtual interaction * GetActiveWindow() override;
      virtual interaction * SetActiveWindow() override;

      virtual void walk_pre_translate_tree(signal_details * pobj, ::user::interaction * puiStop = NULL);

      virtual interaction * GetDescendantWindow(id id) const override;

      virtual void set_window_text(const char * lpszString) override;
      virtual strsize get_window_text(char * lpszStringBuf,strsize nMaxCount) override;
      virtual string get_window_text() override;
      virtual void get_window_text(string & rString) override;
      virtual strsize get_window_text_length() override;

      virtual void install_message_handling(::message::dispatch * pinterface) override;
      virtual bool IsWindowVisible() override;

      virtual void _000OnMouse(::message::mouse * pmouse);
      virtual void _000OnKey(::message::key * pkey);
      virtual void _000OnDrag(::message::drag_and_drop * pdrag);
      DECL_GEN_SIGNAL(_001OnLButtonDown);
      DECL_GEN_SIGNAL(_001OnShowWindow);
      DECL_GEN_SIGNAL(_001OnMouseMove);
      DECL_GEN_SIGNAL(_001OnMouseEnter);
      DECL_GEN_SIGNAL(_001OnMouseLeave);
      DECL_GEN_SIGNAL(_001OnKeyDown);
      DECL_GEN_SIGNAL(_001OnKeyUp);
      virtual void _001OnTimer(::timer * ptimer) override;
      DECL_GEN_SIGNAL(_001OnChar);
      DECL_GEN_SIGNAL(_001OnDestroy);
      DECL_GEN_SIGNAL(_001OnSize);
      DECL_GEN_SIGNAL(_001OnMove);
      DECL_GEN_SIGNAL(_001OnCreate);
      DECL_GEN_SIGNAL(_001OnUser184);
      DECL_GEN_SIGNAL(_001OnNcCalcSize);
      DECL_GEN_SIGNAL(_001OnClose);
      DECL_GEN_SIGNAL(_001OnCommand);
      DECL_GEN_SIGNAL(_001OnSimpleCommand);
      //DECL_GEN_SIGNAL(_001OnSetSchema);



      virtual DECL_GEN_SIGNAL(_002OnLButtonDown);
      virtual DECL_GEN_SIGNAL(_002OnLButtonUp);
      virtual DECL_GEN_SIGNAL(_002OnMouseMove);
      virtual DECL_GEN_SIGNAL(_002OnMouseEnter);
      virtual DECL_GEN_SIGNAL(_002OnMouseLeave);
      virtual DECL_GEN_SIGNAL(_002OnKeyDown);
      virtual DECL_GEN_SIGNAL(_002OnKeyUp);
      virtual DECL_GEN_SIGNAL(_002OnTimer);


      DECL_GEN_SIGNAL(_001OnBaseWndGetProperty);


      virtual LRESULT _001BaseWndGetProperty(EProperty eproperty,LPARAM lparam);



      virtual bool _001IsPointInside(point64 pt) override;
      virtual ::user::interaction * _001FromPoint(point64 pt,bool bTestedIfParentVisible = false) override;

      virtual void OnLinkClick(const char * psz,const char * pszTarget = NULL) override;

      virtual void pre_translate_message(signal_details * pobj) override;


      ::user::interaction * get_child_by_name(const char * pszName,int32_t iLevel = -1);
      ::user::interaction * get_child_by_id(id id,int32_t iLevel = -1);


      virtual bool IsAscendant(const interaction * puiIsAscendant) const override;
      virtual bool IsParent(const interaction * puiIsParent) const override;
      virtual bool IsChild(const interaction * puiIsChild) const override;
      virtual bool IsDescendant(const interaction * puiIsDescendant) const override;

      virtual ::user::elemental * get_wnd_elemental() const override;
      virtual ::user::interaction * get_wnd() const override;
      virtual ::user::interaction * get_wnd(UINT nCmd) const override;

      virtual ::user::interaction * SetParent(::user::interaction * pui) override;
      virtual ::user::interaction * SetOwner(::user::interaction * pui) override;

      virtual ::user::elemental * get_parent() const override;

      virtual ::user::interaction * GetTopWindow() const override;
      virtual ::user::interaction * GetParent() const override;
      virtual ::user::interaction * GetTopLevel() const override;
      virtual ::user::interaction * GetParentTopLevel() const override;
      virtual ::user::interaction * EnsureTopLevel() override;
      virtual ::user::interaction * EnsureParentTopLevel() override;
      virtual ::user::interaction * GetOwner() const override;
      virtual ::user::interaction * GetParentOwner() const override;
      virtual ::user::interaction * GetTopLevelOwner() const override;
      virtual ::user::frame_window * GetFrame() const override;
      virtual ::user::frame_window * GetParentFrame() const override;
      virtual ::user::frame_window * GetTopLevelFrame() const override;
      virtual ::user::frame_window * GetParentTopLevelFrame() const override;
      virtual ::user::frame_window * EnsureParentFrame() override;

      virtual void SendMessageToDescendants(UINT message,WPARAM wParam = 0,lparam lParam = 0,bool bDeep = TRUE,bool bOnlyPerm = FALSE) override;


      virtual int32_t get_descendant_level(::user::interaction * pui) override;
      virtual bool is_descendant(::user::interaction * pui,bool bIncludeSelf = false) override;



      virtual oswindow GetParentHandle() const;


      virtual ::user::interaction * get_focusable_descendant(::user::interaction * pui = NULL) override;



      virtual void RepositionBars(UINT nIDFirst,UINT nIDLast,id nIDLeftOver,UINT nFlag = reposDefault,LPRECT lpRectParam = NULL,LPCRECT lpRectClient = NULL,bool bStretch = TRUE) override;


      virtual ::user::interaction * ChildWindowFromPoint(POINT point) override;
      virtual ::user::interaction * ChildWindowFromPoint(POINT point,UINT nFlags) override;


#ifdef WINDOWSEX
      virtual ::user::interaction * get_next_window(UINT nFlag = GW_HWNDNEXT);
#else
      virtual ::user::interaction * get_next_window(UINT nFlag = 0) override;
#endif

      virtual ::user::interaction * GetTopWindow();

      virtual ::user::interaction * get_next(bool bIgnoreChildren = false,int32_t * piLevel = NULL) override;

      virtual ::user::interaction * GetLastActivePopup() override;

      virtual bool is_message_only_window() const override;

      virtual void pre_subclass_window() override;

      // for custom cleanup after WM_NCDESTROY
      virtual void PostNcDestroy() override;

      virtual LRESULT DefWindowProc(UINT uiMessage,WPARAM wparam,lparam lparam) override;

      virtual LRESULT call_message_handler(UINT message,WPARAM wparam,LPARAM lparam) override;

      virtual void message_handler(signal_details * pobj) override;
      virtual LRESULT message_handler(LPMESSAGE lpmessage) override;
      virtual void GuieProc(signal_details * pobj) override;

      virtual void _001DeferPaintLayeredWindowBackground(::draw2d::graphics * pgraphics) override;

      virtual void _001OnDeferPaintLayeredWindowBackground(::draw2d::graphics * pgraphics) override;


      virtual oswindow get_handle() const override;
      virtual bool attach(oswindow oswindow_New) override;
      virtual oswindow detach() override;

      virtual void * get_os_data() const;


      template < class T >
      T * GetTypedParent() const
      {

         ::user::interaction * pthis = (::user::interaction *)this;

         ASSERT_VALID(pthis);

         ::user::interaction * pParentWnd = GetParent();  // start with one parent up
         while(pParentWnd != NULL)
         {
            if(dynamic_cast <T *> (pParentWnd) != NULL)
            {
               return dynamic_cast <T *> (pParentWnd);
            }
            pParentWnd = pParentWnd->GetParent();
         }
         return NULL;
      }


      virtual bool can_merge(::user::interaction * pui) override;
      virtual bool merge(::user::interaction * pui) override;







      virtual void _001OnTriggerMouseInside() override;

      //#ifdef METROWIN
      //      Agile<Windows::UI::Core::CoreWindow> get_os_window();
      //#endif

      virtual bool IsNotifyIconEnabled();
      virtual void set_viewport_org(::draw2d::graphics * pgraphics) override;

      virtual void viewport_screen_to_client(POINT * ppt) override;
      virtual void viewport_client_to_screen(POINT * ppt) override;
      virtual void viewport_client_to_screen(RECT * ppt) override;
      virtual void viewport_screen_to_client(RECT * ppt) override;


      virtual string get_window_default_matter() override;
      virtual string get_window_icon_matter() override;
      virtual uint32_t get_window_default_style() override;
      virtual e_type get_window_type() override;


      virtual bool on_simple_command(e_simple_command ecommand,lparam lparam,LRESULT & lresult) override;


      // Window-Management message handler member functions
      virtual bool OnCommand(::message::base * pbase) override;
      virtual bool OnNotify(::message::base * pbase) override;
      virtual bool OnChildNotify(::message::base * pbase) override;



      virtual bool is_selected(::data::item * pitem) override;

      virtual sp(place_holder) place(::user::interaction * pui) override;

#if defined(METROWIN) && defined(__cplusplus_winrt)
      static_function Agile<Windows::UI::Core::CoreWindow>(*s_get_os_window)(interaction * pui);
      Agile<Windows::UI::Core::CoreWindow> get_os_window()
      {
         return get_os_window(this);
      }
      static_function Agile<Windows::UI::Core::CoreWindow> get_os_window(interaction * pui)
      {
         return (*s_get_os_window)(pui);
      }
      static_function Agile<Windows::UI::Core::CoreWindow> get_os_window_default(interaction * pui)
      {
         UNREFERENCED_PARAMETER(pui);
         return nullptr;
      }
#endif
      virtual bool _001HasCommandHandler(id id) override;



      virtual bool track_popup_menu(::user::menu_base_item * pitem,int32_t iFlags, POINT pt) override;
      virtual bool track_popup_menu(::xml::node * lpnode,int32_t iFlags, POINT pt) override;
      virtual bool track_popup_xml_matter_menu(const char * pszMatter,int32_t iFlags,POINT pt) override;

      virtual bool track_popup_menu(::user::menu_base_item * pitem,int32_t iFlags,signal_details * pobj) override;
      virtual bool track_popup_menu(::xml::node * lpnode,int32_t iFlags,signal_details * pobj) override;
      virtual bool track_popup_xml_matter_menu(const char * pszMatter,int32_t iFlags,signal_details * pobj) override;

      virtual bool track_popup_menu(::user::menu_base_item * pitem,int32_t iFlags) override;
      virtual bool track_popup_menu(::xml::node * lpnode,int32_t iFlags) override;
      virtual bool track_popup_xml_matter_menu(const char * pszMatter,int32_t iFlags) override;


      virtual void WfiEnableFullScreen(bool bEnable = true) override;
      virtual bool WfiIsFullScreen() override;
      virtual bool WfiIsFullScreenEnabled() override;
      virtual bool WfiIsZoomed() override;
      virtual bool WfiIsIconic() override;


      virtual bool Wfi(EAppearance eapperance = AppearanceCurrent) override;

      virtual bool WfiDock(EAppearance eapperance) override;
      virtual bool WfiClose() override;
      virtual bool WfiRestore(bool bForceNormal = false) override;
      virtual bool WfiMinimize() override;
      virtual bool WfiMaximize() override;
      virtual bool WfiFullScreen() override;
      virtual bool WfiUp() override;
      virtual bool WfiDown() override;
      virtual bool WfiNotifyIcon() override;
      virtual bool WfiToggleTransparentFrame();

      virtual bool WfiIsMoving();
      virtual bool WfiIsSizing();

      virtual EAppearance get_appearance() override;
      virtual EAppearance get_appearance_before() override;

      virtual bool set_appearance(EAppearance eappearance) override;
      virtual bool set_appearance_before(EAppearance eappearance) override;


      virtual void show_keyboard(bool bShow = true) override;

      virtual void keep_alive(::object * pliveobject = NULL) override;

      virtual ::user::interaction * best_top_level_parent(LPRECT lprect);

      virtual index make_zoneing(LPRECT lprect,const RECT & rect=::null_rect(),bool bSet = false,::user::EAppearance * peappearance = NULL,UINT uiSwpFlags = SWP_SHOWWINDOW | SWP_FRAMECHANGED,int_ptr iZOrder = ZORDER_TOP);
      virtual index best_zoneing(LPRECT lprect,const RECT & rect=::null_rect(),bool bSet = false,::user::EAppearance * peappearance = NULL,UINT uiSwpFlags = SWP_SHOWWINDOW | SWP_FRAMECHANGED,int_ptr iZOrder = ZORDER_TOP);
      virtual index best_monitor(LPRECT lprect,const RECT & rect=::null_rect(),bool bSet = false,UINT uiSwpFlags = SWP_SHOWWINDOW | SWP_FRAMECHANGED,int_ptr iZOrder = ZORDER_TOP);
      virtual index best_wkspace(LPRECT lprect,const RECT & rect=::null_rect(),bool bSet = false,UINT uiSwpFlags = SWP_SHOWWINDOW | SWP_FRAMECHANGED,int_ptr iZOrder = ZORDER_TOP);
      virtual index good_restore(LPRECT lprect,const RECT & rect=::null_rect(),bool bSet = false,UINT uiSwpFlags = SWP_SHOWWINDOW | SWP_FRAMECHANGED,int_ptr iZOrder = ZORDER_TOP);
      virtual index good_iconify(LPRECT lprect,const RECT & rect=::null_rect(),bool bSet = false,UINT uiSwpFlags = SWP_NOACTIVATE | SWP_FRAMECHANGED,int_ptr iZOrder = ZORDER_TOP);

      virtual index good_move(LPRECT lprect,const RECT & rect=::null_rect(),UINT uiSwpFlags = SWP_SHOWWINDOW | SWP_FRAMECHANGED,int_ptr iZOrder = ZORDER_TOP);

      virtual bool get_rect_normal(LPRECT lprect);


      virtual void set_viewport_offset_x(int x);
      virtual void set_viewport_offset_y(int y);
      virtual void set_viewport_offset(int x,int y);
      virtual void offset_viewport_offset_x(int x);
      virtual void offset_viewport_offset_y(int y);
      virtual void offset_viewport_offset(int x, int y);
      virtual void on_change_viewport_offset();
      virtual void on_viewport_offset(::draw2d::graphics * pgraphics);
      virtual point get_viewport_offset();
      virtual size get_total_size();
      virtual void on_change_view_size();
      virtual size get_page_size();
      virtual point get_parent_viewport_offset();
      virtual point get_ascendant_viewport_offset();
      virtual void get_margin_rect(LPRECT lprectMargin);
      virtual int get_final_x_scroll_bar_width();
      virtual int get_final_y_scroll_bar_width();



      virtual ::user::schema * get_parent_user_schema() override;


      /*

      static_function sp(::message::base) peek_message(LPMESSAGE lpmsg,::user::interaction * pwnd,UINT wMsgFilterMin,UINT wMsgFilterMax,UINT wRemoveMsg);
      static_function sp(::message::base) get_message(LPMESSAGE lpmsg,::user::interaction * pwnd,UINT wMsgFilterMin,UINT wMsgFilterMax);
      static_function sp(::message::base) peek_message(::user::interaction * pwnd,UINT wMsgFilterMin,UINT wMsgFilterMax,UINT wRemoveMsg);
      static_function sp(::message::base) get_message(::user::interaction * pwnd,UINT wMsgFilterMin,UINT wMsgFilterMax);

      virtual sp(::message::base) peek_message(LPMESSAGE lpmsg,UINT wMsgFilterMin,UINT wMsgFilterMax,UINT wRemoveMsg);
      virtual sp(::message::base) get_message(LPMESSAGE lpmsg,UINT wMsgFilterMin,UINT wMsgFilterMax);
      virtual sp(::message::base) peek_message(UINT wMsgFilterMin,UINT wMsgFilterMax,UINT wRemoveMsg);
      virtual sp(::message::base) get_message(UINT wMsgFilterMin,UINT wMsgFilterMax);

      */


      sp(::message::base) get_base(UINT uiMessage,WPARAM wparam,LPARAM lparam);


      //void transfer_from(::aura::timer_array & ta, interaction * pui);

      virtual window_graphics * get_window_graphics() override;

      virtual bool is_composite() override;

      virtual bool get_window_minimum_size(::size & sizeMin);



      // Up Down Target();


      virtual void UpDownTargetAttach(::user::wndfrm::frame::WorkSetUpDownInterface * pupdown);
      virtual void UpDownTargetDetach(::user::wndfrm::frame::WorkSetUpDownInterface * pupdown);

      virtual bool OnUpDownTargetAttach(::user::wndfrm::frame::WorkSetUpDownInterface * pupdown);
      virtual bool OnUpDownTargetDetach(::user::wndfrm::frame::WorkSetUpDownInterface * pupdown);



      virtual void user_interaction_on_destroy();
      virtual void user_interaction_on_hide();


      virtual string calc_data_id();


      virtual void _001GetXScrollInfo(scroll_info & info);
      virtual void _001GetYScrollInfo(scroll_info & info);
      virtual void layout_scroll_bar();

      // view support
      virtual void on_update(::user::impact * pSender,LPARAM lHint,::object* pHint);

      virtual void keyboard_focus_OnKeyDown(signal_details * pobj) override;
      virtual bool keyboard_focus_OnKillFocus() override;
      virtual bool keyboard_focus_OnChildKillFocus() override;

      virtual bool get_child(sp(::user::interaction) & pui);
      virtual bool rget_child(sp(::user::interaction) & pui);


      virtual ::user::interaction * get_focus_ui();
      virtual void set_focus_guie(::user::interaction * pguie);


      virtual bool is_descendant_of(::user::interaction * puiAscendantCandidate);


      virtual bool show_tooltip(const string & str,bool bError);

      virtual bool layout_tooltip(bool bForceShow = false);

      virtual bool hide_tooltip();

      virtual bool frame_is_transparent();

      virtual double get_alpha();


      //virtual void delete_this();

      virtual void defer_notify_mouse_move();

      virtual void defer_notify_mouse_move(point & ptLast);

      virtual bool has_pending_graphical_update() override;

      virtual void transparent_mouse_events();

      template < typename T >
      inline void redraw_add(T * p);

      template < typename T >
      inline void redraw_remove(T * p);

      inline bool has_redraw();

      virtual void on_after_graphical_update();


      virtual void _001OnDeiconify(::user::EAppearance eappearance);
      
      virtual void on_setting_changed(::aura::e_setting esetting);
      
      virtual bool is_this_visible();

      virtual bool _001OnClick(uint_ptr nFlag, point point);
      virtual bool _001OnRightClick(uint_ptr nFlag, point point);

   };


} // namespace user

// timer_ui works correctly when timer is originated from SetTimer call
inline ::user::interaction * timer_ui(::timer * ptimer)
{

   return (::user::interaction *) ptimer->m_pvoidData;

}


CLASS_DECL_BASE ::user::interaction * oswindow_interaction(oswindow oswindow);


