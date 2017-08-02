#pragma once


namespace metrowin
{


   CLASS_DECL_BASE LRESULT CALLBACK __send_message_hook(int, WPARAM, LPARAM);
   //CLASS_DECL_BASE void _gen::StandardSubclass(oswindow);
   CLASS_DECL_BASE LRESULT CALLBACK __cbt_filter_hook(int, WPARAM, LPARAM);
   CLASS_DECL_BASE LRESULT __call_window_procedure(::user::interaction * pWnd, oswindow hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

   ref class directx_application;

   class CLASS_DECL_BASE interaction_impl :
      virtual public ::user::interaction_impl
   {
   public:

      directx_application ^                     m_xapp;
      ::user::message_queue_listener *          m_plistener;
      string                                    m_strWindowText;
      //visual::dib_sp              m_spdib;
      //visual::dib_sp              m_spdibMultAlphaWork;

      Agile < Windows::UI::Core::CoreWindow >   m_window;
      ::thread *                                m_pthreadDraw;

      int_ptr_to_int_ptr                        m_mapLong;

      interaction_impl();
      interaction_impl(::aura::application * papp);
      virtual void construct(oswindow hwnd);

      virtual bool has_pending_graphical_update();
      virtual void on_after_graphical_update();

      //virtual void on_delete(element * poc);

      //virtual oswindow get_handle() const;

      virtual ::user::interaction * get_wnd() const;

      bool create_message_queue(::user::interaction * pui, const char * pszName);

      virtual bool initialize_native_window(::user::native_window_initialize * pinitialize) override;


      LONG_PTR get_window_long_ptr(int32_t nIndex) const;
      LONG_PTR set_window_long_ptr(int32_t nIndex, LONG_PTR lValue);

      static_function const MSG* GetCurrentMessage();

      virtual void install_message_handling(::message::dispatch * pinterface);

      bool operator==(const ::user::interaction_impl& wnd) const;
      bool operator!=(const ::user::interaction_impl& wnd) const;

      uint32_t GetStyle() const;
      uint32_t GetExStyle() const;
      bool ModifyStyle(uint32_t dwRemove, uint32_t dwAdd, UINT nFlags = 0);
      bool ModifyStyleEx(uint32_t dwRemove, uint32_t dwAdd, UINT nFlags = 0);

      //virtual ::user::interaction * GetOwner();
      //virtual ::user::interaction * SetOwner(::user::interaction * pOwnerWnd);

      virtual oswindow _get_handle();

      virtual bool _001OnCmdMsg(::aura::cmd_msg * pcmdmsg);

      virtual bool BaseOnControlEvent(::user::control_event * pevent);

      void _002OnDraw(::draw2d::dib * pdib);

      DECL_GEN_SIGNAL(_001OnEraseBkgnd);
      DECL_GEN_SIGNAL(_001OnMove);
      DECL_GEN_SIGNAL(_001OnSize);
      // DECL_GEN_SIGNAL(_001OnShowWindow);
      DECL_GEN_SIGNAL(_001OnProdevianSynch);

      ::user::interaction_base *      m_pbasewnd;
      ::user::interaction *        m_pguieCapture;

#if(WINVER >= 0x0500)

      //      bool GetWindowInfo(PWINDOWINFO pwi) const;
      //    bool GetTitleBarInfo(PTITLEBARINFO pti) const;

#endif   // WINVER >= 0x0500

      //      virtual ::user::interaction_impl * from_os_data(void * pdata);
      //    virtual void * get_os_data() const;

      //      static_function interaction_impl * from_handle(oswindow hWnd);
      //    static_function interaction_impl * FromHandlePermanent(oswindow hWnd);
      //static_function void DeleteTempMap();
      //      bool Attach(oswindow hWndNew);
      //    oswindow Detach();

      // subclassing/unsubclassing functions
      virtual void pre_subclass_window();
      bool SubclassWindow(oswindow hWnd);
      bool SubclassDlgItem(UINT nID, ::user::interaction_impl * pParent);
      oswindow UnsubclassWindow();

      // handling of RT_DLGINIT resource (extension to RT_DIALOG)
      bool ExecuteDlgInit(const char * lpszResourceName);
      bool ExecuteDlgInit(LPVOID lpResource);

      using ::user::interaction_impl::create;
      // for child windows, views, panes etc
      virtual bool create(const char * lpszClassName,
                          const char * lpszWindowName, uint32_t dwStyle,
                          const RECT& rect,
                          ::user::interaction * pParentWnd, id id,
                          ::create * pContext = NULL);

      // advanced creation (allows access to extended styles)
      virtual bool CreateEx(uint32_t dwExStyle, const char * lpszClassName,
                            const char * lpszWindowName, uint32_t dwStyle,
                            int x, int y, int nWidth, int nHeight,
                            oswindow hWndParent, id id, LPVOID lpParam = NULL);

      virtual bool CreateEx(uint32_t dwExStyle, const char * lpszClassName,
                            const char * lpszWindowName, uint32_t dwStyle,
                            const RECT& rect,
                            ::user::interaction* pParentWnd, id id,
                            LPVOID lpParam = NULL);

      virtual bool DestroyWindow();

      // special pre-creation and ::user::interaction_impl rect adjustment hooks
      virtual bool pre_create_window(::user::create_struct& cs);

      // Advanced: virtual AdjustWindowRect
      enum AdjustType { adjustBorder = 0, adjustOutside = 1 };
      virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);


      // get immediate child with given ID
      //      using ::user::interaction::GetDlgItem;
      void GetDlgItem(id id, oswindow* phWnd) const;
      // as above, but returns oswindow
      //::user::interaction *  GetDescendantWindow(id id);
      // like GetDlgItem but recursive
      void SendMessageToDescendants(UINT message, WPARAM wParam = 0, lparam lParam = 0, bool bDeep = TRUE, bool bOnlyPerm = FALSE);
      //::user::frame_window * GetParentFrame();
      //::user::frame_window * EnsureParentFrame();
      //::user::interaction *  GetTopLevelParent();
      //::user::interaction *  EnsureTopLevelParent();
      //::user::interaction *  GetTopLevelOwner();
      //::user::interaction *  GetParentOwner();
      //sp(::user::frame_window) GetTopLevelFrame();
      //static_function ::user::interaction_impl * GetSafeOwner(::user::interaction_impl * pParent = NULL, oswindow* pWndTop = NULL);

      virtual bool IsWindow() const;

#if(WINVER >= 0x0500)

      sp(::user::interaction_impl) GetAncestor(UINT gaFlags) const;

#endif   // WINVER >= 0x0500

      LRESULT send_message(UINT message, WPARAM wParam = 0, lparam lParam = NULL);
      bool post_message(UINT message, WPARAM wParam = 0, lparam lParam = NULL);

      bool SendNotifyMessage(UINT message, WPARAM wParam, LPARAM lParam);
      bool SendChildNotifyLastMsg(LRESULT* pResult = NULL);

      bool DragDetect(POINT pt) const;



      // Window Text Functions
      void set_window_text(const char * lpszString) override;
      //strsize get_window_text(char * lpszStringBuf, strsize nMaxCount) override;
      void get_window_text(string & rString) override;
      //strsize get_window_text_length() override;
      void SetFont(::draw2d::font* pFont, bool bRedraw = TRUE);
      ::draw2d::font* GetFont();


      // Window size and position Functions
      //virtual bool IsIconic();
      //virtual bool IsZoomed();
      void MoveWindow(int x, int y, int nWidth, int nHeight, bool bRepaint = TRUE);
      void MoveWindow(LPCRECT lpRect, bool bRepaint = TRUE);
      int SetWindowRgn(HRGN hRgn, bool bRedraw);
      int GetWindowRgn(HRGN hRgn);


      virtual bool SetWindowPos(int_ptr z, int x, int y, int cx, int cy, UINT nFlags);
      virtual UINT ArrangeIconicWindows();
      virtual bool BringWindowToTop();
      using ::user::interaction_impl::GetWindowRect;
      virtual bool GetWindowRect(RECT64 * lpRect);
      using ::user::interaction_impl::GetClientRect;
      virtual bool GetClientRect(RECT64 * lpRect);

      virtual bool ClientToScreen(LPRECT lprect);
      virtual bool ClientToScreen(LPPOINT lppoint);
      virtual bool ClientToScreen(RECT64 * lprect);
      virtual bool ClientToScreen(POINT64 * lppoint);
      virtual bool ScreenToClient(LPRECT lprect);
      virtual bool ScreenToClient(LPPOINT lppoint);
      virtual bool ScreenToClient(RECT64 * lprect);
      virtual bool ScreenToClient(POINT64 * lppoint);

      //      virtual bool GetWindowPlacement(WINDOWPLACEMENT* lpwndpl);
      //    virtual bool SetWindowPlacement(const WINDOWPLACEMENT* lpwndpl);

      // Coordinate Mapping Functions
      virtual void MapWindowPoints(::user::interaction_impl * pwndTo, LPPOINT lpPoint, UINT nCount);
      virtual void MapWindowPoints(::user::interaction_impl * pwndTo, LPRECT lpRect);

      // Update/Painting Functions
      virtual ::draw2d::graphics * GetDC();
      virtual ::draw2d::graphics * GetWindowDC();
      virtual bool ReleaseDC(::draw2d::dib * pdib);
      virtual void Print(::draw2d::graphics * pgraphics, uint32_t dwFlags) const;
      virtual void PrintClient(::draw2d::graphics * pgraphics, uint32_t dwFlags) const;

      virtual void UpdateWindow();
      virtual void SetRedraw(bool bRedraw = TRUE);
      virtual bool GetUpdateRect(LPRECT lpRect, bool bErase = FALSE);
      virtual int GetUpdateRgn(::draw2d::region* pRgn, bool bErase = FALSE);
      virtual void Invalidate(bool bErase = TRUE);
      virtual void InvalidateRect(LPCRECT lpRect, bool bErase = TRUE);
      virtual void InvalidateRgn(::draw2d::region* pRgn, bool bErase = TRUE);
      virtual void ValidateRect(LPCRECT lpRect);
      virtual void ValidateRgn(::draw2d::region* pRgn);
      virtual bool ShowWindow(int nCmdShow);
      virtual void _001WindowMaximize();
      virtual void _001WindowRestore();
      virtual bool IsWindowVisible();
      virtual void ShowOwnedPopups(bool bShow = TRUE);

      virtual ::draw2d::graphics * GetDCEx(::draw2d::region* prgnClip, uint32_t flags);
      virtual bool LockWindowUpdate();
      virtual void UnlockWindowUpdate();
      virtual bool RedrawWindow(LPCRECT lpRectUpdate = NULL,
                                ::draw2d::region* prgnUpdate = NULL,
                                UINT flags = RDW_INVALIDATE | RDW_ERASE);
      //      virtual bool EnableScrollBar(int nSBFlags, UINT nArrowFlags = ESB_ENABLE_BOTH);

      virtual bool DrawAnimatedRects(int idAni, CONST RECT *lprcFrom, CONST RECT *lprcTo);
      virtual bool DrawCaption(::draw2d::graphics * pgraphics, LPCRECT lprc, UINT uFlags);

#if(WINVER >= 0x0500)

      virtual bool AnimateWindow(uint32_t dwTime, uint32_t dwFlags);

#endif   // WINVER >= 0x0500

#if(_WIN32_WINNT >= 0x0501)

      virtual bool PrintWindow(::draw2d::graphics * pgraphics, UINT nFlags) const;

#endif   // _WIN32_WINNT >= 0x0501

      // Layered Window

#if(_WIN32_WINNT >= 0x0500)

      virtual bool SetLayeredWindowAttributes(COLORREF crKey, BYTE bAlpha, uint32_t dwFlags);
      virtual bool UpdateLayeredWindow(::draw2d::graphics * pDCDst, POINT *pptDst, SIZE *psize,
                                       ::draw2d::graphics * pDCSrc, POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, uint32_t dwFlags);

#endif   // _WIN32_WINNT >= 0x0500

#if(_WIN32_WINNT >= 0x0501)

      virtual bool GetLayeredWindowAttributes(COLORREF *pcrKey, BYTE *pbAlpha, uint32_t *pdwFlags) const;

#endif   // _WIN32_WINNT >= 0x0501


      // Timer Functions
      virtual bool SetTimer(UINT_PTR nIDEvent, UINT nElapse, PFN_TIMER pfnTimer);
      virtual bool KillTimer(uint_ptr nIDEvent);

      // Window State Functions
      virtual bool IsWindowEnabled();
      virtual bool EnableWindow(bool bEnable = TRUE);

      // the active ::user::interaction_impl applies only to top-level (frame windows)
      virtual ::user::interaction *  GetActiveWindow();
      virtual ::user::interaction *  SetActiveWindow();

      // the foreground ::user::interaction_impl applies only to top-level windows (frame windows)
      virtual bool SetForegroundWindow();
      static_function sp(::user::interaction_impl) GetForegroundWindow();

      virtual id SetDlgCtrlId(id id);
      virtual id GetDlgCtrlId();



      // capture and focus apply to all windows
      //virtual ::user::interaction *  GetCapture();
      //virtual ::user::interaction *  SetCapture(::user::interaction *  pinterface = NULL);
      //virtual ::user::interaction *  ReleaseCapture();
      //virtual ::user::interaction *  GetCapture();
      virtual ::user::interaction *  GetFocus();
      virtual bool  SetFocus() override;

      static_function ::user::interaction *  GetDesktopWindow();

      // Obsolete and non-portable APIs - not recommended for new code
      virtual void CloseWindow();
      virtual bool OpenIcon();

      // Dialog-Box Item Functions
      // (NOTE: Dialog-Box Items/Controls are not necessarily in dialog boxes!)
      virtual void CheckDlgButton(int nIDButton, UINT nCheck);
      virtual void CheckRadioButton(int nIDFirstButton, int nIDLastButton,
                                    int nIDCheckButton);
      virtual int GetCheckedRadioButton(int nIDFirstButton, int nIDLastButton);
      /*virtual int DlgDirList(__inout_z char * lpPathSpec, __in int nIDListBox,
      __in int nIDStaticPath, __in UINT nFileType);
      virtual int DlgDirListComboBox(__inout_z char * lpPathSpec, __in int nIDComboBox,
      __in int nIDStaticPath, __in UINT nFileType);
      virtual bool DlgDirSelect(__out_ecount_z(nSize) char * lpString, __in int nSize, __in int nIDListBox);
      virtual bool DlgDirSelectComboBox(__out_ecount_z(nSize) char * lpString, __in int nSize, __in int nIDComboBox);*/

      virtual UINT GetDlgItemInt(int nID, BOOL * lpTrans = NULL, bool bSigned = TRUE) const;
      //      virtual int GetDlgItemText(__in int nID, __out_ecount_part_z(nMaxCount, return + 1) char * lpStr, __in int nMaxCount) const;
      //    virtual int GetDlgItemText(int nID, string & rString) const;
      virtual ::user::interaction_impl * GetNextDlgGroupItem(::user::interaction_impl * pWndCtl, bool bPrevious = FALSE) const;
      virtual ::user::interaction_impl * GetNextDlgTabItem(::user::interaction_impl * pWndCtl, bool bPrevious = FALSE) const;
      virtual UINT IsDlgButtonChecked(int nIDButton) const;
      virtual LRESULT SendDlgItemMessage(int nID, UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
      virtual void SetDlgItemInt(int nID, UINT nValue, bool bSigned = TRUE);
      virtual void SetDlgItemText(int nID, const char * lpszString);

      // Scrolling Functions
      virtual int GetScrollPos(int nBar) const;
      virtual void GetScrollRange(int nBar, LPINT lpMinPos, LPINT lpMaxPos) const;
      virtual void ScrollWindow(int xAmount, int yAmount,
                                LPCRECT lpRect = NULL,
                                LPCRECT lpClipRect = NULL);
      virtual int SetScrollPos(int nBar, int nPos, bool bRedraw = TRUE);
      virtual void SetScrollRange(int nBar, int nMinPos, int nMaxPos,
                                  bool bRedraw = TRUE);
      virtual void ShowScrollBar(UINT nBar, bool bShow = TRUE);
      virtual void EnableScrollBarCtrl(int nBar, bool bEnable = TRUE);
//      virtual CScrollBar* GetScrollBarCtrl(int nBar) const;
      // return sibling scrollbar control (or NULL if none)

      virtual int ScrollWindowEx(int dx, int dy,
                                 LPCRECT lpRectScroll, LPCRECT lpRectClip,
                                 ::draw2d::region* prgnUpdate, LPRECT lpRectUpdate, UINT flags);
      //      virtual bool SetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo,
      //       bool bRedraw = TRUE);
      //      virtual bool GetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, UINT nMask = SIF_ALL);
      virtual int GetScrollLimit(int nBar);

      //#if defined(WINDOWSEX) && (WINVER >= 0x0500)

      // virtual bool GetScrollBarInfo(LONG idObject, PSCROLLBARINFO psbi) const;

      //#endif   // WINVER >= 0x0500

      // Window Access Functions
      virtual ::user::interaction *  ChildWindowFromPoint(POINT point);
      virtual ::user::interaction *  ChildWindowFromPoint(POINT point, UINT nFlags);
      static_function sp(::user::interaction_impl) FindWindow(const char * lpszClassName, const char * lpszWindowName);
      static_function sp(::user::interaction_impl) FindWindowEx(oswindow hwndParent, oswindow hwndChildAfter, const char * lpszClass, const char * lpszWindow);

      //      virtual ::user::interaction * GetNextWindow(UINT nFlag = GW_HWNDNEXT);
      virtual ::user::interaction *  GetTopWindow();

      virtual ::user::interaction *  GetWindow(UINT nCmd);
      virtual ::user::interaction *  GetLastActivePopup();

      virtual bool IsChild(::user::interaction *   pWnd);
      virtual ::user::interaction *  GetParent();
      using ::user::interaction_impl::SetParent;
//      ::user::interaction *  SetParent(::user::interaction *  pWndNewParent);
      static_function sp(::user::interaction_impl) WindowFromPoint(POINT point);

      // Alert Functions

      bool FlashWindow(bool bInvert);

      virtual int message_box(const char * lpszText, const char * lpszCaption = NULL, UINT nType = MB_OK);


#if(WINVER >= 0x0500)

      virtual bool FlashWindowEx(uint32_t dwFlags, UINT  uCount, uint32_t dwTimeout);

#endif   // WINVER >= 0x0500

      // Clipboard Functions
      virtual bool ChangeClipboardChain(oswindow hWndNext);
      virtual oswindow SetClipboardViewer();
      virtual bool OpenClipboard();
      static_function ::user::interaction_impl * GetClipboardOwner();
      static_function ::user::interaction_impl * GetClipboardViewer();
      static_function ::user::interaction_impl * GetOpenClipboardWindow();

      // Caret Functions
      virtual void CreateCaret(::draw2d::bitmap* pBitmap);
      virtual void CreateSolidCaret(int nWidth, int nHeight);
      virtual void CreateGrayCaret(int nWidth, int nHeight);
      static_function point GetCaretPos();
      static_function void SetCaretPos(POINT point);
      virtual void HideCaret();
      virtual void ShowCaret();

      // Shell Interaction Functions
      virtual void DragAcceptFiles(bool bAccept = TRUE);

      // icon Functions
      virtual HICON SetIcon(HICON hIcon, bool bBigIcon);
      virtual HICON GetIcon(bool bBigIcon) const;

      // Context Help Functions
      virtual bool SetWindowContextHelpId(uint32_t dwContextHelpId);
      virtual uint32_t GetWindowContextHelpId() const;

      // Dialog Data support
      //virtual bool UpdateData(bool bSaveAndValidate = TRUE);
      // data wnd must be same type as this

      // Help Command Handlers
      void OnHelp();          // F1 (uses current context)
      void OnHelpIndex();     // ID_HELP_INDEX
      void OnHelpFinder();    // ID_HELP_FINDER, ID_DEFAULT_HELP
      void OnHelpUsing();     // ID_HELP_USING
      //      virtual void WinHelp(dword_ptr dwData, UINT nCmd = HELP_CONTEXT);
      //virtual void HtmlHelp(dword_ptr dwData, UINT nCmd = 0x000F);
      //    virtual void WinHelpInternal(dword_ptr dwData, UINT nCmd = HELP_CONTEXT);

      // on_layout and other functions
      /*   void RepositionBars(const char * pszPrefix, const char * pszIdLeftOver,
      UINT nFlag = reposDefault, LPRECT lpRectParam = NULL,
      LPCRECT lpRectClient = NULL, bool bStretch = TRUE);*/

      //
      //void RepositionBars(UINT nIDFirst, UINT nIDLast, id nIDLeftOver,
      // UINT nFlags = reposDefault, LPRECT lpRectParam = NULL,
      //LPCRECT lpRectClient = NULL, bool bStretch = TRUE);


      // dialog support
      void UpdateDialogControls(command_target* pTarget, bool bDisableIfNoHndler);
      void CenterWindow(::user::interaction * pAlternateOwner = NULL);
      virtual id   RunModalLoop(uint32_t dwFlags = 0, ::object * pliveobject = NULL);
      virtual bool ContinueModal(int iLevel);
      virtual void EndModalLoop(id nResult);
      virtual void EndAllModalLoops(id nResult);

      // Window-Management message handler member functions
      virtual bool OnCommand(WPARAM wParam, LPARAM lParam);
      virtual bool OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

      void OnActivate(UINT nState, ::user::interaction_impl * pWndOther, bool bMinimized);
      void OnActivateApp(bool bActive, uint32_t dwThreadID);
      LRESULT OnActivateTopLevel(WPARAM, LPARAM);
      void OnCancelMode();
      void OnChildActivate();
      void OnClose();
      void OnContextMenu(::user::interaction_impl * pWnd, point pos);
      //      bool OnCopyData(::user::interaction_impl * pWnd, COPYDATASTRUCT* pCopyDataStruct);
      DECL_GEN_SIGNAL(_001OnCreate);


      HBRUSH OnCtlColor(::draw2d::graphics * pgraphics, ::user::interaction_impl * pWnd, UINT nCtlColor);

      DECL_GEN_SIGNAL(_001OnDestroy);
      void OnEnable(bool bEnable);
      void OnEndSession(bool bEnding);
      void OnEnterIdle(UINT nWhy, ::user::interaction_impl * pWho);
      bool OnEraseBkgnd(::draw2d::graphics * pgraphics);
      //      void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
      //    bool OnHelpInfo(HELPINFO* lpHelpInfo);
      void OnIconEraseBkgnd(::draw2d::graphics * pgraphics);
      void OnKillFocus(::user::interaction_impl * pNewWnd);
      LRESULT OnMenuChar(UINT nChar, UINT nFlags, ::user::menu* pMenu);
      void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
      void OnMove(int x, int y);
      DECL_GEN_SIGNAL(_001OnPaint);
      DECL_GEN_SIGNAL(_001OnPrint);
      void OnParentNotify(UINT message, LPARAM lParam);
      HCURSOR OnQueryDragIcon();
      bool OnQueryEndSession();
      bool OnQueryNewPalette();
      bool OnQueryOpen();
      void OnSetFocus(::user::interaction_impl * pOldWnd);
      void OnShowWindow(bool bShow, UINT nStatus);
      void OnSize(UINT nType, int cx, int cy);
      void OnTCard(UINT idAction, uint32_t dwActionData);
      //      void OnWindowPosChanging(WINDOWPOS* lpwndpos);
      //    void OnWindowPosChanged(WINDOWPOS* lpwndpos);

      void OnChangeUIState(UINT nAction, UINT nUIElement);
      void OnUpdateUIState(UINT nAction, UINT nUIElement);
      UINT OnQueryUIState();

      // Nonclient-Area message handler member functions
      bool OnNcActivate(bool bActive);
      //      void OnNcCalcSize(bool bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
      bool OnNcCreate(LPCREATESTRUCT lpCreateStruct);
      DECL_GEN_SIGNAL(_001OnNcDestroy);
      LRESULT OnNcHitTest(point point);
      void OnNcLButtonDblClk(UINT nHitTest, point point);
      void OnNcLButtonDown(UINT nHitTest, point point);
      void OnNcLButtonUp(UINT nHitTest, point point);
      void OnNcMButtonDblClk(UINT nHitTest, point point);
      void OnNcMButtonDown(UINT nHitTest, point point);
      void OnNcMButtonUp(UINT nHitTest, point point);
      void OnNcMouseMove(UINT nHitTest, point point);
      void OnNcPaint();
      void OnNcRButtonDblClk(UINT nHitTest, point point);
      void OnNcRButtonDown(UINT nHitTest, point point);
      void OnNcRButtonUp(UINT nHitTest, point point);

      // System message handler member functions
      //      void OnDropFiles(HDROP hDropInfo);
      void OnPaletteIsChanging(::user::interaction_impl * pRealizeWnd);
      void OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags);
      void OnSysCommand(UINT nID, LPARAM lParam);
      void OnSysDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags);
      void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
      void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
      void OnCompacting(UINT nCpuTime);
      void OnDevModeChange(char * lpDeviceName);
      void OnFontChange();
      void OnPaletteChanged(::user::interaction_impl * pFocusWnd);
      void OnSpoolerStatus(UINT nStatus, UINT nJobs);
      void OnSysColorChange();
      void OnTimeChange();
      void OnSettingChange(UINT uFlags, const char * lpszSection);
      void OnWinIniChange(const char * lpszSection);

      // Input message handler member functions
      void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
      void OnDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags);
//      void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
      //    void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
      void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
      void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
      void OnLButtonDblClk(UINT nFlags, point point);
      void OnLButtonDown(UINT nFlags, point point);
      void OnLButtonUp(UINT nFlags, point point);
      void OnMButtonDblClk(UINT nFlags, point point);
      void OnMButtonDown(UINT nFlags, point point);
      void OnMButtonUp(UINT nFlags, point point);
      int OnMouseActivate(::user::interaction_impl * pDesktopWnd, UINT nHitTest, UINT message);
      void OnMouseMove(UINT nFlags, point point);
      bool OnMouseWheel(UINT nFlags, short zDelta, point pt);
      LRESULT OnRegisteredMouseWheel(WPARAM wParam, LPARAM lParam);
      void OnRButtonDblClk(UINT nFlags, point point);
      void OnRButtonDown(UINT nFlags, point point);
      void OnRButtonUp(UINT nFlags, point point);
      DECL_GEN_SIGNAL(_001OnSetCursor);
      void OnTimer(uint_ptr nIDEvent);

      // Initialization message handler member functions
      void OnInitMenu(::user::menu* pMenu);
      void OnInitMenuPopup(::user::menu* pPopupMenu, UINT nIndex, bool bSysMenu);

      // Clipboard message handler member functions
      void OnAskCbFormatName(__in UINT nMaxCount, __out_ecount_z(nMaxCount) char * lpszString);
      void OnChangeCbChain(oswindow hWndRemove, oswindow hWndAfter);
      void OnDestroyClipboard();
      void OnDrawClipboard();
      void OnHScrollClipboard(::user::interaction_impl * pClipAppWnd, UINT nSBCode, UINT nPos);
      void OnPaintClipboard(::user::interaction_impl * pClipAppWnd, HGLOBAL hPaintStruct);
      void OnRenderAllFormats();
      void OnRenderFormat(UINT nFormat);
      void OnSizeClipboard(::user::interaction_impl * pClipAppWnd, HGLOBAL hRect);
      void OnVScrollClipboard(::user::interaction_impl * pClipAppWnd, UINT nSBCode, UINT nPos);

      // control message handler member functions
      //      int OnCompareItem(int nIDCtl, LPCOMPAREITEMSTRUCT lpCompareItemStruct);
      //    void OnDeleteItem(int nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct);
      //  void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
      UINT OnGetDlgCode();
      //void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);

      // MDI message handler member functions
      void OnMDIActivate(bool bActivate,
                         ::user::interaction_impl * pActivateWnd, ::user::interaction_impl * pDeactivateWnd);

      // menu loop notification messages
      void OnEnterMenuLoop(bool bIsTrackPopupMenu);
      void OnExitMenuLoop(bool bIsTrackPopupMenu);

      // Win4 messages
      //      void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
      //    void OnStyleChanging(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
      void OnSizing(UINT nSide, LPRECT lpRect);
      void OnMoving(UINT nSide, LPRECT lpRect);
      void OnCaptureChanged(::user::interaction_impl * pWnd);
      bool OnDeviceChange(UINT nEventType, dword_ptr dwData);

      // Overridables and other helpers (for implementation of derived classes)
      // for deriving from a standard control
      //      virtual WNDPROC* GetSuperWndProcAddr();

      // for dialog data exchange and validation
      //      virtual void DoDataExchange(CDataExchange* pDX);

      // for modality
      virtual void BeginModalState();
      virtual void EndModalState();

      // for translating Windows messages in main message pump
      virtual void pre_translate_message(signal_details * pobj);


      // for processing Windows messages
      virtual void message_handler(signal_details * pobj);
      //virtual bool OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

      // for handling default processing
      LRESULT Default();
      virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, lparam lParam);

      // for custom cleanup after WM_NCDESTROY
      virtual void PostNcDestroy();

      // for notifications from parent
      virtual bool OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
      // return TRUE if parent should not process this message
      bool ReflectChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
      static_function bool ReflectLastMsg(oswindow hWndChild, LRESULT* pResult = NULL);

      // Implementation
      virtual ~interaction_impl();
      virtual bool CheckAutoCenter();
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;
      static_function bool GrayCtlColor(HDC hDC, oswindow hWnd, UINT nCtlColor,
                                        HBRUSH hbrGray, COLORREF clrText);


      // helper routines for implementation
      bool HandleFloatingSysCommand(UINT nID, LPARAM lParam);
      //bool IsTopParentActive();
      void ActivateTopParent();
      virtual void WalkPreTranslateTree(::user::interaction * puiStop, signal_details * pobj);
      static_function ::user::interaction * GetDescendantWindow(::user::interaction * hWnd, id id);
      static_function void SendMessageToDescendants(oswindow hWnd, UINT message,
            WPARAM wParam, LPARAM lParam, bool bDeep, bool bOnlyPerm);
      virtual bool is_frame_window(); // is_kind_of(System.template type_info < frame_window > ()))
      virtual void on_final_release();
      static_function bool ModifyStyle(oswindow hWnd, uint32_t dwRemove, uint32_t dwAdd,
                                       UINT nFlags);
      static_function bool ModifyStyleEx(oswindow hWnd, uint32_t dwRemove, uint32_t dwAdd,
                                         UINT nFlags);
      static_function void _FilterToolTipMessage(MSG* pMsg, ::user::interaction_impl * pWnd);
      bool _EnableToolTips(bool bEnable, UINT nFlag);
      static_function oswindow GetSafeOwner_(oswindow hWnd, oswindow* pWndTop);
      void PrepareForHelp();

      //UINT m_nFlags;      // see WF_ flags above

      //      WNDPROC m_pfnSuper; // for subclassing of controls
      static const UINT m_nMsgDragList;
      int m_nModalResult; // for return values from ::user::interaction_impl::RunModalLoop

      ::draw2d::font * m_pfont;

      friend class frame_window;

      // for creating dialogs and dialog-like windows
      bool CreateDlg(const char * lpszTemplateName, ::user::interaction_impl * pParentWnd);
      //bool CreateDlgIndirect(LPCDLGTEMPLATE lpDialogTemplate, ::user::interaction_impl * pParentWnd,
      // HINSTANCE hInst);


      // implementation of message dispatch/hooking
      CLASS_DECL_BASE friend LRESULT CALLBACK __send_message_hook(int, WPARAM, LPARAM);
      //CLASS_DECL_BASE friend void _gen::StandardSubclass(oswindow);
      CLASS_DECL_BASE friend LRESULT CALLBACK __cbt_filter_hook(int, WPARAM, LPARAM);
      CLASS_DECL_BASE friend LRESULT __call_window_procedure(::user::interaction * pWnd, oswindow hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

      // standard message implementation
      LRESULT OnNTCtlColor(WPARAM wParam, LPARAM lParam);
      LRESULT OnDisplayChange(WPARAM, LPARAM);
      LRESULT OnDragList(WPARAM, LPARAM);

      static_function BOOL CALLBACK GetAppsEnumWindowsProc(oswindow hwnd, LPARAM lParam);


      static_function void get_app_wnda(user::oswindow_array & wnda);

      virtual void _001DeferPaintLayeredWindowBackground(HDC hdc);

      virtual LONG GetWindowLong(int nIndex);
      virtual LONG SetWindowLong(int nIndex, LONG lValue);

      virtual void _001BaseWndInterfaceMap();


      // void _001UpdateWindow();


      void _001OnTriggerMouseInside();


      Agile < Windows::UI::Core::CoreWindow > get_os_window();

      void set_view_port_org(::draw2d::dib * pdib);

      void offset_view_port_org(LPRECT lprectScreen);


   };


} // namespace metrowin



