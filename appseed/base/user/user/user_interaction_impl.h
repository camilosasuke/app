#pragma once


#include "user_interaction_impl_base.h"


#if defined METROWIN && defined(__cplusplus_winrt)


namespace user
{


   class CLASS_DECL_BASE native_window
   {
   public:


      ::axis::system_window ^ m_pwindow;


   };


} // namespace user



//#define interaction_impl ::user::interaction

#elif defined(APPLE_IOS)

class CLASS_DECL_BASE native_window
{
public:

};

//#define interaction_impl ::user::interaction

#else

class native_window;

#endif


namespace user
{


   class CLASS_DECL_BASE interaction_impl:
      virtual public ::user::interaction_impl_base
   {
   public:

      double                        m_dFps;
      double                        m_dUpdateScreenFps;
      bool                          m_bIpcCopy;
      bool                          m_bLockWindowUpdate;

      ref_array < object >          m_ptraRedraw;

      ::user::interaction *         m_puiFocus;

      sp(::user::interaction)       m_puiThis;

      oswindow                      m_oswindow;
      bool                          m_bScreenRelativeMouseMessagePosition;
      bool                          m_bTranslateMouseMessageCursor;
      bool                          m_bComposite;
      bool                          m_bUpdateGraphics;
      
      point                         m_pt;
      size                          m_size;

      native_window *               m_pwindow;

      sp(window_graphics)           m_spgraphics;

      pointer < mutex >             m_apmutexDraw; // auto pointer
      pointer < mutex >             m_apmutexRedraw; // auto pointer

      ::user::interaction_ptra      m_guieptraMouseHover;

      sp(::thread)                  m_pthreadUpdateWindow;


      interaction_impl();


      void user_common_construct();


      virtual void install_message_handling(::message::dispatch * pdispatch) override;


      virtual mutex * draw_mutex();

      virtual ::user::interaction_impl * get_user_interaction_impl() override;


      virtual bool check_need_layout();
      virtual void clear_need_layout();
      virtual void set_need_layout();

      virtual bool create_message_queue(::user::interaction * pui, const char * pszName);

#if defined(METROWIN) || defined(APPLE_IOS) || defined(ANDROID)
      virtual bool initialize_native_window(::user::native_window_initialize * pinitialize) = 0;
#endif


      virtual void _000OnMouseLeave(signal_details * pobj);
      virtual void _008OnMouse(::message::mouse * pmouse);
      virtual void _001BaseWndInterfaceMap();

      virtual void prio_install_message_handling(::message::dispatch * pinterface);
      virtual void last_install_message_handling(::message::dispatch * pinterface);

      bool operator==(const interaction_impl& wnd) const;
      bool operator!=(const interaction_impl& wnd) const;

      virtual uint32_t GetStyle() const;
      virtual uint32_t GetExStyle() const;
      virtual bool ModifyStyle(uint32_t dwRemove,uint32_t dwAdd,UINT nFlags = 0);
      virtual bool ModifyStyleEx(uint32_t dwRemove,uint32_t dwAdd,UINT nFlags = 0);


      virtual void mouse_hover_add(::user::interaction * pinterface);
      virtual void mouse_hover_remove(::user::interaction * pinterface);

      //virtual ::user::interaction * get_owner();
      virtual void set_owner(::user::interaction * pOwnerWnd);



      virtual ::user::interaction_impl * from_os_data(void * pdata);
      virtual void * get_os_data() const;

      bool attach(oswindow oswindow_New);
      oswindow detach();

      virtual bool _001OnCmdMsg(::aura::cmd_msg * pcmdmsg);

      DECL_GEN_SIGNAL(_002OnDraw);
      DECL_GEN_SIGNAL(_001OnShowWindow);


#if (WINVER >= 0x0500) && defined(WINDOWSEX)

      bool GetWindowInfo(PWINDOWINFO pwi) const;
      bool GetTitleBarInfo(PTITLEBARINFO pti) const;

#endif   // WINVER >= 0x0500

      // subclassing/unsubclassing functions
      virtual void pre_subclass_window();
      virtual bool subclass_window(oswindow oswindow);
      virtual bool SubclassDlgItem(UINT nID,::window_sp pParent);
      virtual oswindow unsubclass_window();

      // handling of RT_DLGINIT resource (extension to RT_DIALOG)
      virtual bool ExecuteDlgInit(const char * lpszResourceName);
      virtual bool ExecuteDlgInit(LPVOID lpResource);




      using ::user::interaction_base::create_window;
      using ::user::interaction_base::create_window_ex;
      // for child windows, views, panes etc
      virtual bool create_window(::user::interaction * pui, const char * lpszClassName,const char * lpszWindowName,uint32_t dwStyle,const RECT & rect,::user::interaction * pParentWnd,id id, ::create * pcreate = NULL);

      virtual bool create_window_ex(::user::interaction * pui, uint32_t dwExStyle,const char * lpszClassName,const char * lpszWindowName,uint32_t dwStyle,const RECT & rect,::user::interaction * pParentWnd,id id,LPVOID lpParam = NULL);

      virtual bool DestroyWindow();

      // special pre-creation and interaction_impl rect adjustment hooks
      virtual bool pre_create_window(::user::create_struct& cs);

      // Advanced: virtual AdjustWindowRect
      enum AdjustType { adjustBorder = 0,adjustOutside = 1 };
      virtual void CalcWindowRect(LPRECT lpClientRect,UINT nAdjustType = adjustBorder);


      // get immediate child with given ID
      //void get_child_by_id(id id, int32_t iLevelHWND* poswindow_) const;
      // as above, but returns oswindow
      virtual ::user::interaction * GetDescendantWindow(id id) const;
      // like get_child_by_id but recursive
      void SendMessageToDescendants(UINT message,WPARAM wParam = 0,lparam lParam = 0,bool bDeep = TRUE,bool bOnlyPerm = FALSE);

      virtual bool IsWindow() const;

#if(WINVER >= 0x0500)

      virtual ::window_sp GetAncestor(UINT gaFlags) const;

#endif   // WINVER >= 0x0500

      virtual LRESULT send_message(UINT message, WPARAM wParam = 0, lparam lParam = 0);

#ifdef LINUX

      virtual LRESULT send_message(XEvent * pevent);

#endif


      virtual bool post_message(UINT message,WPARAM wParam = 0,lparam lParam = 0);

      virtual bool SendNotifyMessage(UINT message,WPARAM wParam,lparam lParam);
      //virtual bool SendChildNotifyLastMsg(LRESULT* pResult = NULL);

      virtual bool DragDetect(POINT pt) const;



      // Window Text Functions
      virtual void set_window_text(const char * lpszString);
      virtual strsize GetWindowText(char * lpszStringBuf,int32_t nMaxCount);
      virtual void GetWindowText(string & rString);
      virtual strsize GetWindowTextLength();


      // Window size and position Functions
      virtual bool WfiIsIconic();
      virtual bool WfiIsZoomed();
      //virtual int32_t SetWindowRgn(HRGN hRgn,bool bRedraw);
      //virtual int32_t GetWindowRgn(HRGN hRgn);

      using ::user::interaction_base::SetWindowPos;
      virtual bool SetWindowPos(int_ptr z,int32_t x,int32_t y,int32_t cx,int32_t cy,UINT nFlags =  SWP_SHOWWINDOW);
      virtual UINT ArrangeIconicWindows();
      virtual bool BringWindowToTop();

#ifdef WINDOWSEX
      virtual bool GetWindowPlacement(WINDOWPLACEMENT* lpwndpl);
      virtual bool SetWindowPlacement(const WINDOWPLACEMENT* lpwndpl);
#endif

      // Coordinate Mapping Functions
      virtual void MapWindowPoints(::window_sp pwndTo,LPPOINT lpPoint,UINT nCount);
      virtual void MapWindowPoints(::window_sp pwndTo,LPRECT lpRect);

      virtual void Print(::draw2d::graphics * pgraphics,uint32_t dwFlags) const;
      virtual void PrintClient(::draw2d::graphics * pgraphics,uint32_t dwFlags) const;

      virtual void UpdateWindow();
      virtual void SetRedraw(bool bRedraw = TRUE);
      virtual bool GetUpdateRect(LPRECT lpRect,bool bErase = FALSE);
      virtual int32_t GetUpdateRgn(::draw2d::region* pRgn,bool bErase = FALSE);
      virtual void Invalidate(bool bErase = TRUE);
      virtual void InvalidateRect(LPCRECT lpRect,bool bErase = TRUE);
      virtual void InvalidateRgn(::draw2d::region* pRgn,bool bErase = TRUE);
      virtual void ValidateRect(LPCRECT lpRect);
      virtual void ValidateRgn(::draw2d::region* pRgn);
      virtual bool ShowWindow(int32_t nCmdShow);
      virtual bool IsWindowVisible();
      virtual void ShowOwnedPopups(bool bShow = TRUE);

      virtual ::draw2d::graphics * GetDCEx(::draw2d::region* prgnClip,uint32_t flags);
      virtual bool LockWindowUpdate();
      virtual void UnlockWindowUpdate();


#ifdef WINDOWS

      virtual bool RedrawWindow(LPCRECT lpRectUpdate = NULL,::draw2d::region* prgnUpdate = NULL,UINT flags = RDW_INVALIDATE | RDW_ERASE);

#else

      virtual bool RedrawWindow(LPCRECT lpRectUpdate = NULL,::draw2d::region* prgnUpdate = NULL,UINT flags = 0);

#endif


#ifdef WINDOWSEX

      virtual bool EnableScrollBar(int32_t nSBFlags,UINT nArrowFlags = ESB_ENABLE_BOTH);

#else

      virtual bool EnableScrollBar(int32_t nSBFlags,UINT nArrowFlags = 3);

#endif


      virtual bool DrawAnimatedRects(int32_t idAni,CONST RECT *lprcFrom,CONST RECT *lprcTo);

      virtual bool DrawCaption(::draw2d::graphics * pgraphics,LPCRECT lprc,UINT uFlags);

#if(WINVER >= 0x0500)

      virtual bool AnimateWindow(uint32_t dwTime,uint32_t dwFlags);

#endif


#if(_WIN32_WINNT >= 0x0501)

      virtual bool PrintWindow(::draw2d::graphics * pgraphics,UINT nFlags) const;

#endif


#if(_WIN32_WINNT >= 0x0500)

      virtual bool SetLayeredWindowAttributes(COLORREF crKey,BYTE bAlpha,uint32_t dwFlags);
      virtual bool UpdateLayeredWindow(::draw2d::graphics * pDCDst,POINT *pptDst,SIZE *psize,::draw2d::graphics * pDCSrc,POINT *pptSrc,COLORREF crKey,BLENDFUNCTION *pblend,uint32_t dwFlags);

#endif   // _WIN32_WINNT >= 0x0500

#if(_WIN32_WINNT >= 0x0501)

      virtual bool GetLayeredWindowAttributes(COLORREF *pcrKey,BYTE *pbAlpha,uint32_t *pdwFlags) const;

#endif   // _WIN32_WINNT >= 0x0501


      // timer Functions
      //virtual bool SetTimer(uint_ptr nIDEvent,UINT nElapse);
      //virtual bool KillTimer(uint_ptr nIDEvent);

      // Window State Functions
      virtual bool is_window_enabled();
      virtual bool enable_window(bool bEnable = TRUE);

      // the active interaction_impl applies only to top-level (frame windows)
      virtual ::user::interaction * GetActiveWindow();
      virtual ::user::interaction * SetActiveWindow();

      // the foreground interaction_impl applies only to top-level windows (frame windows)
      virtual bool SetForegroundWindow();

      virtual id SetDlgCtrlId(id id);
      virtual id GetDlgCtrlId() const;



      // capture and focus apply to all windows
      virtual bool SetCapture(::user::interaction * pui = NULL);
      virtual bool ReleaseCapture();
      virtual ::user::interaction * GetCapture();
      virtual bool SetFocus();

      // Obsolete and non-portable APIs - not recommended for new code
      virtual void CloseWindow();
      virtual bool OpenIcon();

      // Dialog-Box Item Functions
      // (NOTE: Dialog-Box Items/Controls are not necessarily in dialog boxes!)
      virtual void CheckDlgButton(int32_t nIDButton,UINT nCheck);
      virtual void CheckRadioButton(int32_t nIDFirstButton,int32_t nIDLastButton,
         int32_t nIDCheckButton);
      virtual int32_t GetCheckedRadioButton(int32_t nIDFirstButton,int32_t nIDLastButton);
      virtual int32_t DlgDirList(LPTSTR lpPathSpec,int32_t nIDListBox,
         int32_t nIDStaticPath,UINT nFileType);
      virtual int32_t DlgDirListComboBox(LPTSTR lpPathSpec,int32_t nIDComboBox,
         int32_t nIDStaticPath,UINT nFileType);
      virtual bool DlgDirSelect(LPTSTR lpString,int32_t nSize,int32_t nIDListBox);
      virtual bool DlgDirSelectComboBox(LPTSTR lpString,int32_t nSize,int32_t nIDComboBox);

      virtual UINT GetChildByIdInt(int32_t nID,bool* lpTrans = NULL,bool bSigned = TRUE) const;
      virtual int32_t GetChildByIdText(int32_t nID,LPTSTR lpStr,int32_t nMaxCount) const;
      virtual int32_t GetChildByIdText(int32_t nID,string & rString) const;
      virtual ::window_sp GetNextDlgGroupItem(::window_sp pWndCtl,bool bPrevious = FALSE) const;
      virtual ::window_sp GetNextDlgTabItem(::window_sp pWndCtl,bool bPrevious = FALSE) const;
      virtual UINT IsDlgButtonChecked(int32_t nIDButton) const;
      virtual LRESULT SendDlgItemMessage(int32_t nID,UINT message,WPARAM wParam = 0,LPARAM lParam = 0);
      virtual void SetDlgItemInt(int32_t nID,UINT nValue,bool bSigned = TRUE);
      virtual void SetDlgItemText(int32_t nID,const char * lpszString);

      // Scrolling Functions
      virtual int32_t GetScrollPos(int32_t nBar) const;
      virtual void GetScrollRange(int32_t nBar,LPINT lpMinPos,LPINT lpMaxPos) const;
      virtual void ScrollWindow(int32_t xAmount,int32_t yAmount,
         LPCRECT lpRect = NULL,
         LPCRECT lpClipRect = NULL);
      virtual int32_t SetScrollPos(int32_t nBar,int32_t nPos,bool bRedraw = TRUE);
      virtual void SetScrollRange(int32_t nBar,int32_t nMinPos,int32_t nMaxPos,
         bool bRedraw = TRUE);
      virtual void ShowScrollBar(UINT nBar,bool bShow = TRUE);
      virtual void EnableScrollBarCtrl(int32_t nBar,bool bEnable = TRUE);
      //virtual CScrollBar* GetScrollBarCtrl(int32_t nBar) const;
      // return sibling scrollbar control (or NULL if none)

      virtual int32_t ScrollWindowEx(int32_t dx,int32_t dy,LPCRECT lpRectScroll,LPCRECT lpRectClip,::draw2d::region* prgnUpdate,LPRECT lpRectUpdate,UINT flags);


#ifdef WINDOWSEX
      virtual bool SetScrollInfo(int32_t nBar,LPSCROLLINFO lpScrollInfo,bool bRedraw = TRUE);
      virtual bool GetScrollInfo(int32_t nBar,LPSCROLLINFO lpScrollInfo,UINT nMask = SIF_ALL);
#endif


      virtual int32_t GetScrollLimit(int32_t nBar);

#if (WINVER >= 0x0500) && defined(WINDOWSEX)

      virtual bool GetScrollBarInfo(LONG idObject,PSCROLLBARINFO psbi) const;

#endif   // WINVER >= 0x0500

      // Window Access Functions
      virtual ::user::interaction * ChildWindowFromPoint(POINT point);
      virtual ::user::interaction * ChildWindowFromPoint(POINT point,UINT nFlags);

#ifdef WINDOWSEX
      virtual ::user::interaction * get_next_window(UINT nFlag = GW_HWNDNEXT);
#else
      virtual ::user::interaction * get_next_window(UINT nFlag = 0);
#endif
      virtual ::user::interaction * GetTopWindow();

      //      virtual ::user::interaction * GetWindow(UINT nCmd);
      virtual ::user::interaction * GetLastActivePopup();

      //virtual bool IsChild(::user::interaction * pwindow) const;
      //virtual ::user::interaction * get_parent() const;
      //::user::interaction * set_parent(::user::interaction * pWndNewParent);

      // Alert Functions
      bool FlashWindow(bool bInvert);

      virtual int32_t message_box(const char * lpszText,const char * lpszCaption = NULL,UINT nType = MB_OK);

#if(WINVER >= 0x0500)

      virtual bool FlashWindowEx(uint32_t dwFlags,UINT  uCount,uint32_t dwTimeout);

#endif   // WINVER >= 0x0500

      // Clipboard Functions
      virtual bool ChangeClipboardChain(oswindow oswindow_Next);
      virtual oswindow SetClipboardViewer();
      virtual bool OpenClipboard();

      // Caret Functions
      virtual void CreateCaret(::draw2d::bitmap* pBitmap);
      virtual void CreateSolidCaret(int32_t nWidth,int32_t nHeight);
      virtual void CreateGrayCaret(int32_t nWidth,int32_t nHeight);
      virtual void HideCaret();
      virtual void ShowCaret();

      // Shell Interaction Functions
      virtual void DragAcceptFiles(bool bAccept = TRUE);

      // icon Functions
      // virtual ::visual::icon * SetIcon(::visual::icon * picon, bool bBigIcon);
      // virtual ::visual::icon * GetIcon(bool bBigIcon) const;

      // Context Help Functions
      virtual bool SetWindowContextHelpId(uint32_t dwContextHelpId);
      virtual uint32_t GetWindowContextHelpId() const;


//      // Help Command Handlers
//      void OnHelp();          // F1 (uses current context)
//      void OnHelpIndex();     // ID_HELP_INDEX
//      void OnHelpFinder();    // ID_HELP_FINDER, ID_DEFAULT_HELP
//      void OnHelpUsing();     // ID_HELP_USING
//#ifdef WINDOWSEX
//      virtual void WinHelp(uint_ptr dwData,UINT nCmd = HELP_CONTEXT);
//      virtual void HtmlHelp(uint_ptr dwData,UINT nCmd = 0x000F);
//      virtual void WinHelpInternal(uint_ptr dwData,UINT nCmd = HELP_CONTEXT);
//#endif


      // Dialog Data support
      virtual bool update_data(bool bSaveAndValidate = true);
      // data wnd must be same type as this


      // dialog support
      virtual void UpdateDialogControls(command_target* pTarget,bool bDisableIfNoHndler);
      virtual void CenterWindow(::user::interaction * pAlternateOwner = NULL);


//      virtual bool OnCommand(::message::base * pbase);
//      virtual bool OnNotify(::message::base * pbase);
//
//      void OnActivate(UINT nState,::window_sp pWndOther,bool bMinimized);
//      void OnActivateApp(bool bActive,uint32_t dwThreadID);
//      LRESULT OnActivateTopLevel(WPARAM,LPARAM);
//      void OnCancelMode();
//      void OnChildActivate();
//      void OnClose();
//      void OnContextMenu(::window_sp pwindow,point pos);
//
//#ifdef WINDOWSEX
//      bool OnCopyData(::window_sp pwindow,COPYDATASTRUCT* pCopyDataStruct);
//#endif
      DECL_GEN_SIGNAL(_001OnCreate);


//      void * OnCtlColor(::draw2d::graphics * pgraphics,::window_sp pwindow,UINT nCtlColor);
//
        DECL_GEN_SIGNAL(_001OnDestroy);
//      void OnEnable(bool bEnable);
//      void OnEndSession(bool bEnding);
//      void OnEnterIdle(UINT nWhy,::window_sp pWho);
//      bool OnEraseBkgnd(::draw2d::dib * pdib);
//
//
//#ifdef WINDOWSEX
//      void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
//      bool OnHelpInfo(HELPINFO* lpHelpInfo);
//#endif
//
//
//      void OnIconEraseBkgnd(::draw2d::dib * pdib);
//      void OnKillFocus(::window_sp pNewWnd);
//      LRESULT OnMenuChar(UINT nChar,UINT nFlags,::user::menu* pMenu);
//      void OnMenuSelect(UINT nItemID,UINT nFlags,HMENU hSysMenu);
//      void OnMove(int32_t x,int32_t y);
      DECL_GEN_SIGNAL(_001OnPaint);
      DECL_GEN_SIGNAL(_001OnPrint);
      DECL_GEN_SIGNAL(_001OnCaptureChanged);
      //void OnParentNotify(UINT message,LPARAM lParam);
      //bool OnQueryEndSession();
      //bool OnQueryNewPalette();
      //bool OnQueryOpen();
      //void OnSetFocus(::window_sp pOldWnd);
      //void OnShowWindow(bool bShow,UINT nStatus);
      //void OnSize(UINT nType,int32_t cx,int32_t cy);


      //void OnTCard(UINT idAction,uint32_t dwActionData);

#ifdef WINDOWSEX
      //void OnWindowPosChanging(WINDOWPOS* lpwndpos);
      DECL_GEN_SIGNAL(_001OnWindowPosChanged);
#endif

//      void OnChangeUIState(UINT nAction,UINT nUIElement);
//      void OnUpdateUIState(UINT nAction,UINT nUIElement);
//      UINT OnQueryUIState();
//
//      // Nonclient-Area message handler member functions
//      bool OnNcActivate(bool bActive);
//
//#ifdef WINDOWSEX
//      void OnNcCalcSize(bool bCalcValidRects,NCCALCSIZE_PARAMS* lpncsp);
//#endif
//      bool OnNcCreate(::user::create_struct * lpCreateStruct);
//      LRESULT OnNcHitTest(point point);
//      void OnNcLButtonDblClk(UINT nHitTest,point point);
//      void OnNcLButtonDown(UINT nHitTest,point point);
//      void OnNcLButtonUp(UINT nHitTest,point point);
//      void OnNcMButtonDblClk(UINT nHitTest,point point);
//      void OnNcMButtonDown(UINT nHitTest,point point);
//      void OnNcMButtonUp(UINT nHitTest,point point);
//      void OnNcMouseMove(UINT nHitTest,point point);
//      void OnNcPaint();
//      void OnNcRButtonDblClk(UINT nHitTest,point point);
//      void OnNcRButtonDown(UINT nHitTest,point point);
//      void OnNcRButtonUp(UINT nHitTest,point point);
//
//      // System message handler member functions
//#ifdef WINDOWSEX
//      void OnDropFiles(HDROP hDropInfo);
//#endif
      //void OnPaletteIsChanging(::window_sp pRealizeWnd);
      //void OnSysChar(UINT nChar,UINT nRepCnt,UINT nFlags);
      //void OnSysCommand(UINT nID,LPARAM lParam);
      //void OnSysDeadChar(UINT nChar,UINT nRepCnt,UINT nFlags);
      //void OnSysKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags);
      //void OnSysKeyUp(UINT nChar,UINT nRepCnt,UINT nFlags);
      //void OnCompacting(UINT nCpuTime);
      //void OnDevModeChange(LPTSTR lpDeviceName);
      //void OnFontChange();
      //void OnPaletteChanged(::window_sp pFocusWnd);
      //void OnSpoolerStatus(UINT nStatus,UINT nJobs);
      //void OnSysColorChange();
      //void OnTimeChange();
      //void OnSettingChange(UINT uFlags,const char * lpszSection);
      //void OnWinIniChange(const char * lpszSection);

      //// Input message handler member functions
      //void OnChar(UINT nChar,UINT nRepCnt,UINT nFlags);
      //void OnDeadChar(UINT nChar,UINT nRepCnt,UINT nFlags);
      ////      void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
      ////    void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
      //void OnKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags);
      //void OnKeyUp(UINT nChar,UINT nRepCnt,UINT nFlags);
      //void OnLButtonDblClk(UINT nFlags,point point);
      //void OnLButtonDown(UINT nFlags,point point);
      //void OnLButtonUp(UINT nFlags,point point);
      //void OnMButtonDblClk(UINT nFlags,point point);
      //void OnMButtonDown(UINT nFlags,point point);
      //void OnMButtonUp(UINT nFlags,point point);
      //int32_t OnMouseActivate(::window_sp pDesktopWnd,UINT nHitTest,UINT message);
      //void OnMouseMove(UINT nFlags,point point);
      //bool OnMouseWheel(UINT nFlags,int16_t zDelta,point pt);
      //LRESULT OnRegisteredMouseWheel(WPARAM wParam,LPARAM lParam);
      //void OnRButtonDblClk(UINT nFlags,point point);
      //void OnRButtonDown(UINT nFlags,point point);
      //void OnRButtonUp(UINT nFlags,point point);
      //bool OnSetCursor(::window_sp pwindow,UINT nHitTest,UINT message);
      //void OnTimer(uint_ptr nIDEvent);

      //// Initialization message handler member functions
      //void OnInitMenu(::user::menu* pMenu);
      //void OnInitMenuPopup(::user::menu* pPopupMenu,UINT nIndex,bool bSysMenu);


//#ifdef WINDOWS
//
//
//      // Clipboard message handler member functions
//      void OnAskCbFormatName(UINT nMaxCount,LPTSTR lpszString);
//      void OnChangeCbChain(oswindow oswindow_Remove,oswindow oswindow_After);
//      void OnDestroyClipboard();
//      void OnDrawClipboard();
//      void OnHScrollClipboard(::window_sp pClipAppWnd,UINT nSBCode,UINT nPos);
//      void OnPaintClipboard(::window_sp pClipAppWnd,HGLOBAL hPaintStruct);
//      void OnRenderAllFormats();
//      void OnRenderFormat(UINT nFormat);
//      void OnSizeClipboard(::window_sp pClipAppWnd,HGLOBAL hRect);
//      void OnVScrollClipboard(::window_sp pClipAppWnd,UINT nSBCode,UINT nPos);
//
//
//#endif
//

//#ifdef WINDOWSEX
//
//
//      // control message handler member functions
//      int32_t  OnCompareItem(int32_t nIDCtl,LPCOMPAREITEMSTRUCT    lpCompareItemStruct);
//      void OnDeleteItem(int32_t nIDCtl,LPDELETEITEMSTRUCT     lpDeleteItemStruct);
//      void OnDrawItem(int32_t nIDCtl,LPDRAWITEMSTRUCT       lpDrawItemStruct);
//      void OnMeasureItem(int32_t nIDCtl,LPMEASUREITEMSTRUCT    lpMeasureItemStruct);
//
//
//#endif
//

//      UINT OnGetDlgCode();
//
//
//      // MDI message handler member functions
//      void OnMDIActivate(bool bActivate,::window_sp pActivateWnd,::window_sp pDeactivateWnd);
//
//      // menu loop notification messages
//      void OnEnterMenuLoop(bool bIsTrackPopupMenu);
//      void OnExitMenuLoop(bool bIsTrackPopupMenu);
//
//      // Win4 messages
//#ifdef WINDOWSEX
//      void OnStyleChanged(int32_t nStyleType,LPSTYLESTRUCT lpStyleStruct);
//      void OnStyleChanging(int32_t nStyleType,LPSTYLESTRUCT lpStyleStruct);
//#endif
//      void OnSizing(UINT nSide,LPRECT lpRect);
//      void OnMoving(UINT nSide,LPRECT lpRect);
//      void OnCaptureChanged(::window_sp pwindow);
//      bool OnDeviceChange(UINT nEventType,uint_ptr dwData);


      // for dialog data exchange and validation
      // virtual void do_data_exchange(CDataExchange* pDX);

      // for modality
      virtual void BeginModalState();
      virtual void EndModalState();

      // for translating Windows messages in main message pump
      virtual void pre_translate_message(signal_details * pobj);


      // for processing Windows messages
      using ::user::interaction_base::message_handler;
      virtual void message_handler(signal_details * pobj);
      //virtual bool OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

      // for handling default processing
      virtual LRESULT Default();
      virtual LRESULT DefWindowProc(UINT message,WPARAM wParam,lparam lParam);

      // for custom cleanup after WM_NCDESTROY
      virtual void PostNcDestroy();

      // for notifications from parent
      virtual bool OnChildNotify(::message::base * pbase);
      // return TRUE if parent should not process this message
      virtual bool ReflectChildNotify(::message::base * pbase);

      // Implementation
      virtual ~interaction_impl();
      virtual bool CheckAutoCenter();
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;


      // helper routines for implementation
      virtual bool HandleFloatingSysCommand(UINT nID,LPARAM lParam);
      virtual bool IsTopParentActive();
      virtual void ActivateTopParent();
      virtual void on_final_release();


      virtual bool _EnableToolTips(bool bEnable,UINT nFlag);
      virtual void PrepareForHelp();

      // for creating dialogs and dialog-like windows
      //virtual bool CreateDlg(const char * lpszTemplateName, ::window_sp pParentWnd);
      //virtual bool CreateDlgIndirect(LPCDLGTEMPLATE lpDialogTemplate, ::window_sp pParentWnd,
      // HINSTANCE hInst);



      //// standard message implementation
      //LRESULT OnNTCtlColor(WPARAM wParam,LPARAM lParam);
      //LRESULT OnDisplayChange(WPARAM,LPARAM);
      //LRESULT OnDragList(WPARAM,LPARAM);

      virtual void _001DeferPaintLayeredWindowBackground(::draw2d::dib * pdib);

      virtual void _001OnDeferPaintLayeredWindowBackground(::draw2d::dib * pdib);

      virtual LONG get_window_long(int32_t nIndex) const;
      virtual LONG set_window_long(int32_t nIndex,LONG lValue);
      virtual LONG_PTR get_window_long_ptr(int32_t nIndex) const;
      virtual LONG_PTR set_window_long_ptr(int32_t nIndex,LONG_PTR lValue);



      virtual interaction_impl * get_impl() const;

      virtual void _001UpdateScreen();
      virtual void _001UpdateBuffer();

      //virtual void _001UpdateWindow(bool bUpdateBuffer = true);


      virtual void update_graphics_resources();

      


      virtual void set_handle(oswindow oswindow);
      virtual oswindow get_handle() const;

      virtual window_graphics * get_window_graphics();

      virtual bool is_composite();


      virtual ::user::interaction * get_focus_ui() override;
      virtual void set_focus_guie(::user::interaction * pguie) override;

      virtual void redraw_add(::object * p);
      virtual void redraw_remove(::object * p);
      virtual bool has_redraw();
      virtual mutex * mutex_redraw();

      virtual void _001OnTriggerMouseInside();

      virtual bool has_pending_graphical_update();

   };






   class CLASS_DECL_BASE guie_message_wnd:
      virtual public interaction_impl
   {
   public:


      ::user::interaction * m_puiForward;


      guie_message_wnd(::aura::application * papp);


      using interaction_impl::message_handler;
      virtual void message_handler(signal_details * pobj);


   }; // guie_message_wnd


} // namespace user




