#pragma once


namespace windows
{


   CLASS_DECL_BASE LRESULT CALLBACK __send_message_hook(int32_t,WPARAM,LPARAM);
   CLASS_DECL_BASE LRESULT CALLBACK __cbt_filter_hook(int32_t,WPARAM,LPARAM);


   class CLASS_DECL_BASE interaction_impl:
      virtual public ::user::interaction_impl,
      virtual public IDropTarget
   {
   public:

      bool                                      m_bUpdateGraphics;

      size                                      m_size;

      point                                     m_pt;

      ::user::interaction_base *                m_pbasewnd;

      uint32_t                                  m_uiLastRedrawRequest;

      IDropTargetHelper *                       m_piDropHelper;

      bool                                      m_bUseDnDHelper;

      WNDPROC                                   m_pfnSuper;          // for subclassing of controls

      ::user::EAppearance                       m_eapperanceLayout;
      
      //::thread *                                m_pthreadDraw;
      
      rect64                                    m_rectLastPos;
      DWORD                                     m_dwLastPos;

      interaction_impl();
      interaction_impl(::aura::application * papp);
      virtual ~interaction_impl();


      void user_common_construct();

      bool SetWindowPos(int_ptr z, int32_t x, int32_t y, int32_t cx, int32_t cy, UINT nFlags);
      virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid,void **ppvObject);

      virtual ULONG STDMETHODCALLTYPE AddRef(void);

      virtual ULONG STDMETHODCALLTYPE Release(void);

      virtual HRESULT STDMETHODCALLTYPE DragEnter(
         /* [unique][in] */ __RPC__in_opt IDataObject *pDataObj,
         /* [in] */ DWORD grfKeyState,
         /* [in] */ POINTL pt,
         /* [out][in] */ __RPC__inout DWORD *pdwEffect);

      virtual HRESULT STDMETHODCALLTYPE DragOver(
         /* [in] */ DWORD grfKeyState,
         /* [in] */ POINTL pt,
         /* [out][in] */ __RPC__inout DWORD *pdwEffect);

      virtual HRESULT STDMETHODCALLTYPE DragLeave(void);

      virtual HRESULT STDMETHODCALLTYPE Drop(
         /* [unique][in] */ __RPC__in_opt IDataObject *pDataObj,
         /* [in] */ DWORD grfKeyState,
         /* [in] */ POINTL pt,
         /* [out][in] */ __RPC__inout DWORD *pdwEffect);


      virtual void construct(oswindow oswindow);


      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;


      bool create_message_queue(::user::interaction * pui, const char * pszName);

      //virtual const MSG* GetCurrentMessage();

      virtual void install_message_handling(::message::dispatch * pinterface);

      bool operator==(const interaction_impl& wnd) const;
      bool operator!=(const interaction_impl& wnd) const;

      virtual uint32_t GetStyle() const;
      virtual uint32_t GetExStyle() const;
      virtual bool ModifyStyle(uint32_t dwRemove,uint32_t dwAdd,UINT nFlags = 0);
      virtual bool ModifyStyleEx(uint32_t dwRemove,uint32_t dwAdd,UINT nFlags = 0);

      //virtual ::user::interaction * get_owner();
      //virtual void set_owner(::user::interaction * pOwnerWnd);

      virtual bool _001OnCmdMsg(::aura::cmd_msg * pcmdmsg);

      void _002OnDraw(::draw2d::dib * pdib);

      DECL_GEN_SIGNAL(_001OnEraseBkgnd);
      DECL_GEN_SIGNAL(_001OnSize);
      DECL_GEN_SIGNAL(_001OnShowWindow);
      DECL_GEN_SIGNAL(_001OnProdevianSynch);
      DECL_GEN_SIGNAL(_001OnWindowPosChanging);
      DECL_GEN_SIGNAL(_001OnWindowPosChanged);
      DECL_GEN_SIGNAL(_001OnGetMinMaxInfo);


      virtual void win_update_graphics();

#if(WINVER >= 0x0500)

      bool GetWindowInfo(PWINDOWINFO pwi) const;
      bool GetTitleBarInfo(PTITLEBARINFO pti) const;

#endif   // WINVER >= 0x0500

      //      virtual ::user::interaction * from_os_data(void * pdata);
      virtual void * get_os_data() const;

//      virtual ::user::interaction * from_handle(oswindow oswindow);

      // subclassing/unsubclassing functions
      virtual void pre_subclass_window();
      bool subclass_window(oswindow oswindow);
      bool SubclassDlgItem(UINT nID,::window_sp pParent);
      oswindow unsubclass_window();

      // handling of RT_DLGINIT resource (extension to RT_DIALOG)
      bool ExecuteDlgInit(const char * lpszResourceName);
      bool ExecuteDlgInit(LPVOID lpResource);

      // for child windows, views, panes etc
      virtual bool create_window(::user::interaction * pui, const char * lpszClassName,const char * lpszWindowName,uint32_t dwStyle,const RECT & rect,::user::interaction * puiParent,id id, ::create * pcreate = NULL) override;

      // advanced creation (allows access to extended styles)
      virtual bool create_window_ex(::user::interaction * pui, uint32_t dwExStyle,const char * lpszClassName,const char * lpszWindowName,uint32_t dwStyle,const RECT & rect,::user::interaction * puiParent,id id,LPVOID lpParam = NULL) override;

      // advanced creation (allows access to extended styles)
      virtual bool native_create_window_ex(::user::interaction * pui, uint32_t dwExStyle,const char * lpszClassName,const char * lpszWindowName,uint32_t dwStyle,const RECT & rect,oswindow oswindowParent,id id,LPVOID lpParam = NULL);

      virtual bool DestroyWindow();

      // special pre-creation and interaction_impl rect adjustment hooks
      virtual bool pre_create_window(::user::create_struct& cs);

      // Advanced: virtual AdjustWindowRect
      enum AdjustType { adjustBorder = 0,adjustOutside = 1 };
      virtual void CalcWindowRect(LPRECT lpClientRect,UINT nAdjustType = adjustBorder);


      // get immediate child with given ID
      using ::user::interaction_impl::get_child_by_id;
      void get_child_by_id(id id,oswindow* poswindow_) const;
      // as above, but returns oswindow
//      using ::user::interaction_impl::GetDescendantWindow;
  //    ::user::interaction * GetDescendantWindow(id id) const;
      // like get_child_by_id but recursive
      void SendMessageToDescendants(UINT message,WPARAM wParam = 0,lparam lParam = 0,bool bDeep = TRUE,bool bOnlyPerm = FALSE);
//      virtual ::window_sp get_safe_owner(::window_sp pParent = NULL,oswindow* pWndTop = NULL);

      virtual bool IsWindow() const;

#if(WINVER >= 0x0500)

      ::window_sp GetAncestor(UINT gaFlags) const;

#endif   // WINVER >= 0x0500

      LRESULT send_message(UINT message,WPARAM wParam = 0,lparam lParam = NULL);
      bool post_message(UINT message,WPARAM wParam = 0,lparam lParam = NULL);

      bool SendNotifyMessage(UINT message,WPARAM wParam,lparam lParam);
      //bool SendChildNotifyLastMsg(LRESULT* pResult = NULL);

      bool DragDetect(POINT pt) const;



      // Window Text Functions
      virtual void set_window_text(const char * lpszString) override;
      virtual strsize get_window_text(char * lpszStringBuf,strsize nMaxCount) override;
      virtual void get_window_text(string & rString) override;
      virtual strsize get_window_text_length() override;

      virtual void on_layout() override;
      virtual void on_translate() override;
      virtual void on_do_show_flags() override;
      virtual void on_zorder() override;

      virtual void clear_need_layout() override;
      virtual void clear_need_translation() override;
      virtual void clear_show_flags() override;
      virtual void clear_need_zorder() override;


      //virtual bool SetWindowPos(int_ptr z, int32_t x, int32_t y, int32_t cx, int32_t cy, UINT nFlags = SWP_SHOWWINDOW);


      // Window size and position Functions
      virtual bool WfiIsIconic();
      virtual bool WfiIsZoomed();
      //int32_t SetWindowRgn(HRGN hRgn,bool bRedraw);
      //int32_t GetWindowRgn(HRGN hRgn);

      virtual UINT ArrangeIconicWindows();
      virtual void BringToTop(int nCmdShow);
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

      virtual bool GetWindowPlacement(WINDOWPLACEMENT* lpwndpl);
      virtual bool SetWindowPlacement(const WINDOWPLACEMENT* lpwndpl);

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
      virtual void _001WindowMinimize(bool bNoActivate) override;
      virtual void _001WindowMaximize() override;
      virtual void _001WindowFullScreen() override;
      virtual void _001WindowRestore() override;
      virtual bool IsWindowVisible() override;
      virtual void ShowOwnedPopups(bool bShow = TRUE);

      virtual ::draw2d::graphics * GetDCEx(::draw2d::region* prgnClip,uint32_t flags);
      virtual bool LockWindowUpdate();
      virtual void UnlockWindowUpdate();

      virtual bool RedrawWindow(LPCRECT lpRectUpdate = NULL,
         ::draw2d::region* prgnUpdate = NULL,
         UINT flags = RDW_INVALIDATE | RDW_ERASE);

      virtual bool EnableScrollBar(int32_t nSBFlags,UINT nArrowFlags = ESB_ENABLE_BOTH);

      virtual bool DrawAnimatedRects(int32_t idAni,CONST RECT *lprcFrom,CONST RECT *lprcTo);
      virtual bool DrawCaption(::draw2d::graphics * pgraphics,LPCRECT lprc,UINT uFlags);

#if(WINVER >= 0x0500)

      virtual bool AnimateWindow(uint32_t dwTime,uint32_t dwFlags);

#endif   // WINVER >= 0x0500

#if(_WIN32_WINNT >= 0x0501)

      virtual bool PrintWindow(::draw2d::graphics * pgraphics,UINT nFlags) const;

#endif   // _WIN32_WINNT >= 0x0501

      // Layered Window

#if(_WIN32_WINNT >= 0x0500)

      virtual bool SetLayeredWindowAttributes(COLORREF crKey,BYTE bAlpha,uint32_t dwFlags);
      virtual bool UpdateLayeredWindow(::draw2d::graphics * pDCDst,POINT *pptDst,SIZE *psize,
         ::draw2d::graphics * pDCSrc,POINT *pptSrc,COLORREF crKey,BLENDFUNCTION *pblend,uint32_t dwFlags);

#endif   // _WIN32_WINNT >= 0x0500

#if(_WIN32_WINNT >= 0x0501)

      virtual bool GetLayeredWindowAttributes(COLORREF *pcrKey,BYTE *pbAlpha,uint32_t *pdwFlags) const;

#endif   // _WIN32_WINNT >= 0x0501


      // timer Functions
      virtual bool SetTimer(uint_ptr nIDEvent,UINT nElapse,PFN_TIMER pfnTimer);
      virtual bool KillTimer(uint_ptr nIDEvent);

      // Window State Functions
      virtual bool is_window_enabled();
      virtual bool enable_window(bool bEnable = TRUE);

      // the active interaction_impl applies only to top-level (frame windows)
      virtual ::user::interaction * GetActiveWindow();
      virtual ::user::interaction * SetActiveWindow();

      // the foreground interaction_impl applies only to top-level windows (frame windows)
      virtual bool SetForegroundWindow();
      virtual ::user::interaction * GetForegroundWindow();



      virtual bool SetFocus();

      virtual ::user::interaction * GetFocus();

      virtual ::user::interaction * GetDesktopWindow();

      // Obsolete and non-portable APIs - not recommended for new code
      virtual void CloseWindow();
      virtual bool OpenIcon();

      // Dialog-Box Item Functions
      // (NOTE: Dialog-Box Items/Controls are not necessarily in dialog boxes!)
      virtual void CheckDlgButton(int32_t nIDButton,UINT nCheck);
      virtual void CheckRadioButton(int32_t nIDFirstButton,int32_t nIDLastButton,  int32_t nIDCheckButton);
      virtual int32_t GetCheckedRadioButton(int32_t nIDFirstButton,int32_t nIDLastButton);
      virtual int32_t DlgDirList(__inout_z LPTSTR lpPathSpec,__in int32_t nIDListBox,
         __in int32_t nIDStaticPath,__in UINT nFileType);
      virtual int32_t DlgDirListComboBox(__inout_z LPTSTR lpPathSpec,__in int32_t nIDComboBox,
         __in int32_t nIDStaticPath,__in UINT nFileType);
      virtual bool DlgDirSelect(__out_ecount_z(nSize) LPTSTR lpString,__in int32_t nSize,__in int32_t nIDListBox);
      virtual bool DlgDirSelectComboBox(__out_ecount_z(nSize) LPTSTR lpString,__in int32_t nSize,__in int32_t nIDComboBox);

      virtual UINT GetChildByIdInt(int32_t nID,BOOL * lpTrans = NULL,bool bSigned = TRUE) const;
      virtual int32_t GetChildByIdText(__in int32_t nID,__out_ecount_part_z(nMaxCount,return + 1) LPTSTR lpStr,__in int32_t nMaxCount) const;
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

      virtual int32_t ScrollWindowEx(int32_t dx,int32_t dy,
         LPCRECT lpRectScroll,LPCRECT lpRectClip,
         ::draw2d::region* prgnUpdate,LPRECT lpRectUpdate,UINT flags);
      virtual bool SetScrollInfo(int32_t nBar,LPSCROLLINFO lpScrollInfo,
         bool bRedraw = TRUE);
      virtual bool GetScrollInfo(int32_t nBar,LPSCROLLINFO lpScrollInfo,UINT nMask = SIF_ALL);
      virtual int32_t GetScrollLimit(int32_t nBar);

#if(WINVER >= 0x0500)

      virtual bool GetScrollBarInfo(LONG idObject,PSCROLLBARINFO psbi) const;

#endif   // WINVER >= 0x0500

      // Window Access Functions
      virtual ::user::interaction * ChildWindowFromPoint(POINT point);
      virtual ::user::interaction * ChildWindowFromPoint(POINT point,UINT nFlags);
///      virtual ::window_sp FindWindow(const char * lpszClassName,const char * lpszWindowName);
      //   virtual ::window_sp FindWindowEx(oswindow oswindowParent,oswindow oswindowChildAfter,const char * lpszClass,const char * lpszWindow);

      virtual ::user::interaction * get_next_window(UINT nFlag = GW_HWNDNEXT);
      virtual ::user::interaction * GetTopWindow() const;

      virtual ::user::interaction * GetWindow(UINT nCmd) const;
      virtual ::user::interaction * GetLastActivePopup() const;

      virtual ::user::interaction * GetParent() const;
      virtual ::user::interaction * SetParent(::user::interaction * pWndNewParent);
//      virtual ::user::interaction * WindowFromPoint(POINT point);


      virtual ::user::interaction * SetOwner(::user::interaction * pWndNewParent);
      virtual ::user::interaction * GetOwner() const;

      virtual bool FlashWindow(bool bInvert);

      virtual int32_t message_box(const char * lpszText,const char * lpszCaption = NULL,UINT nType = MB_OK);


#if(WINVER >= 0x0500)

      virtual bool FlashWindowEx(uint32_t dwFlags,UINT  uCount,uint32_t dwTimeout);

#endif   // WINVER >= 0x0500

      // Clipboard Functions
      virtual bool ChangeClipboardChain(oswindow oswindow_Next);
      virtual ::oswindow  SetClipboardViewer();
      virtual bool OpenClipboard();
      virtual ::window_sp GetClipboardOwner();
      virtual ::window_sp GetClipboardViewer();
      virtual ::window_sp GetOpenClipboardWindow();

      // Caret Functions
      virtual void CreateCaret(::draw2d::bitmap* pBitmap);
      virtual void CreateSolidCaret(int32_t nWidth,int32_t nHeight);
      virtual void CreateGrayCaret(int32_t nWidth,int32_t nHeight);
      virtual point GetCaretPos();
      virtual void SetCaretPos(POINT point);
      virtual void HideCaret();
      virtual void ShowCaret();

      // Shell Interaction Functions
      virtual void DragAcceptFiles(bool bAccept = TRUE);

      // icon Functions
      virtual HICON SetIcon(HICON hIcon,bool bBigIcon);
      virtual HICON GetIcon(bool bBigIcon) const;

      // Context Help Functions
      virtual bool SetWindowContextHelpId(uint32_t dwContextHelpId);
      virtual uint32_t GetWindowContextHelpId() const;

      // Dialog Data support
      //virtual bool update_data(bool bSaveAndValidate = TRUE);
      // data wnd must be same type as this

      // Help Command Handlers
      void OnHelp();          // F1 (uses current context)
      void OnHelpIndex();     // ID_HELP_INDEX
      void OnHelpFinder();    // ID_HELP_FINDER, ID_DEFAULT_HELP
      void OnHelpUsing();     // ID_HELP_USING
      virtual void WinHelp(uint_ptr dwData,UINT nCmd = HELP_CONTEXT);
      //virtual void HtmlHelp(uint_ptr dwData, UINT nCmd = 0x000F);
      virtual void WinHelpInternal(uint_ptr dwData,UINT nCmd = HELP_CONTEXT);



      // dialog support
      void UpdateDialogControls(command_target* pTarget,bool bDisableIfNoHndler);
      void CenterWindow(::user::interaction * pAlternateOwner = NULL);
      //virtual id   RunModalLoop(uint32_t dwFlags = 0,::object * pliveobject = NULL);
      virtual bool ContinueModal(int32_t iLevel);
      virtual void EndModalLoop(id nResult);
      virtual void EndAllModalLoops(id nResult);

      // Window-Management message handler member functions
      virtual bool OnCommand(::message::base * pbase);
      virtual bool OnNotify(::message::base * pbase);

      void OnActivate(UINT nState,::window_sp pWndOther,bool bMinimized);
      void OnActivateApp(bool bActive,uint32_t dwThreadID);
      LRESULT OnActivateTopLevel(WPARAM,LPARAM);
      void OnCancelMode();
      void OnChildActivate();
      void OnClose();
      void OnContextMenu(::window_sp pwindow,point pos);
      bool OnCopyData(::window_sp pwindow,COPYDATASTRUCT* pCopyDataStruct);
      DECL_GEN_SIGNAL(_001OnCreate);


      HBRUSH OnCtlColor(::draw2d::graphics * pgraphics,::window_sp pwindow,UINT nCtlColor);

      DECL_GEN_SIGNAL(_001OnDestroy);
      void OnEnable(bool bEnable);
      void OnEndSession(bool bEnding);
      void OnEnterIdle(UINT nWhy,::window_sp pWho);
      bool OnEraseBkgnd(::draw2d::graphics * pgraphics);
      void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
      bool OnHelpInfo(HELPINFO* lpHelpInfo);
      void OnIconEraseBkgnd(::draw2d::graphics * pgraphics);
      DECL_GEN_SIGNAL(_001OnKillFocus);
      LRESULT OnMenuChar(UINT nChar,UINT nFlags,::user::menu* pMenu);
      void OnMenuSelect(UINT nItemID,UINT nFlags,HMENU hSysMenu);
      void OnMove(int32_t x,int32_t y);
      DECL_GEN_SIGNAL(_001OnPaint);
      DECL_GEN_SIGNAL(_001OnPrint);
      void OnParentNotify(UINT message,LPARAM lParam);
      HCURSOR OnQueryDragIcon();
      bool OnQueryEndSession();
      bool OnQueryNewPalette();
      bool OnQueryOpen();
      DECL_GEN_SIGNAL(_001OnSetFocus);
      void OnShowWindow(bool bShow,UINT nStatus);
      void OnSize(UINT nType,int32_t cx,int32_t cy);
      void OnTCard(UINT idAction,uint32_t dwActionData);

      void OnChangeUIState(UINT nAction,UINT nUIElement);
      void OnUpdateUIState(UINT nAction,UINT nUIElement);
      UINT OnQueryUIState();

      // Nonclient-Area message handler member functions
      bool OnNcActivate(bool bActive);
      void OnNcCalcSize(bool bCalcValidRects,NCCALCSIZE_PARAMS* lpncsp);
      bool OnNcCreate(::user::create_struct * lpCreateStruct);
      DECL_GEN_SIGNAL(_001OnNcDestroy);
      DECL_GEN_SIGNAL(_001OnNcCalcSize);
      LRESULT OnNcHitTest(point point);
      void OnNcLButtonDblClk(UINT nHitTest,point point);
      void OnNcLButtonDown(UINT nHitTest,point point);
      void OnNcLButtonUp(UINT nHitTest,point point);
      void OnNcMButtonDblClk(UINT nHitTest,point point);
      void OnNcMButtonDown(UINT nHitTest,point point);
      void OnNcMButtonUp(UINT nHitTest,point point);
      void OnNcMouseMove(UINT nHitTest,point point);
      void OnNcPaint();
      void OnNcRButtonDblClk(UINT nHitTest,point point);
      void OnNcRButtonDown(UINT nHitTest,point point);
      void OnNcRButtonUp(UINT nHitTest,point point);

      // System message handler member functions
      void OnDropFiles(HDROP hDropInfo);
      void OnPaletteIsChanging(::window_sp pRealizeWnd);
      void OnSysChar(UINT nChar,UINT nRepCnt,UINT nFlags);
      void OnSysCommand(UINT nID,LPARAM lParam);
      void OnSysDeadChar(UINT nChar,UINT nRepCnt,UINT nFlags);
      void OnSysKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags);
      void OnSysKeyUp(UINT nChar,UINT nRepCnt,UINT nFlags);
      void OnCompacting(UINT nCpuTime);
      void OnDevModeChange(__in_z LPTSTR lpDeviceName);
      void OnFontChange();
      void OnPaletteChanged(::window_sp pFocusWnd);
      void OnSpoolerStatus(UINT nStatus,UINT nJobs);
      void OnSysColorChange();
      void OnTimeChange();
      void OnSettingChange(UINT uFlags,const char * lpszSection);
      void OnWinIniChange(const char * lpszSection);

      // Input message handler member functions
      void OnChar(UINT nChar,UINT nRepCnt,UINT nFlags);
      void OnDeadChar(UINT nChar,UINT nRepCnt,UINT nFlags);
      //      void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
      //    void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
      void OnKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags);
      void OnKeyUp(UINT nChar,UINT nRepCnt,UINT nFlags);
      void OnLButtonDblClk(UINT nFlags,point point);
      void OnLButtonDown(UINT nFlags,point point);
      void OnLButtonUp(UINT nFlags,point point);
      void OnMButtonDblClk(UINT nFlags,point point);
      void OnMButtonDown(UINT nFlags,point point);
      void OnMButtonUp(UINT nFlags,point point);
      int32_t OnMouseActivate(::window_sp pDesktopWnd,UINT nHitTest,UINT message);
      void OnMouseMove(UINT nFlags,point point);
      bool OnMouseWheel(UINT nFlags,int16_t zDelta,point pt);
      LRESULT OnRegisteredMouseWheel(WPARAM wParam,LPARAM lParam);
      void OnRButtonDblClk(UINT nFlags,point point);
      void OnRButtonDown(UINT nFlags,point point);
      void OnRButtonUp(UINT nFlags,point point);
      DECL_GEN_SIGNAL(_001OnSetCursor);
      void OnTimer(uint_ptr nIDEvent);

      // Initialization message handler member functions
      void OnInitMenu(::user::menu* pMenu);
      void OnInitMenuPopup(::user::menu* pPopupMenu,UINT nIndex,bool bSysMenu);

      // Clipboard message handler member functions
      void OnAskCbFormatName(__in UINT nMaxCount,__out_ecount_z(nMaxCount) LPTSTR lpszString);
      void OnChangeCbChain(oswindow oswindow_Remove,oswindow oswindow_After);
      void OnDestroyClipboard();
      void OnDrawClipboard();
      void OnHScrollClipboard(::window_sp pClipAppWnd,UINT nSBCode,UINT nPos);
      void OnPaintClipboard(::window_sp pClipAppWnd,HGLOBAL hPaintStruct);
      void OnRenderAllFormats();
      void OnRenderFormat(UINT nFormat);
      void OnSizeClipboard(::window_sp pClipAppWnd,HGLOBAL hRect);
      void OnVScrollClipboard(::window_sp pClipAppWnd,UINT nSBCode,UINT nPos);

      // control message handler member functions
      int32_t OnCompareItem(int32_t nIDCtl,LPCOMPAREITEMSTRUCT lpCompareItemStruct);
      void OnDeleteItem(int32_t nIDCtl,LPDELETEITEMSTRUCT lpDeleteItemStruct);
      void OnDrawItem(int32_t nIDCtl,LPDRAWITEMSTRUCT lpDrawItemStruct);
      UINT OnGetDlgCode();
      void OnMeasureItem(int32_t nIDCtl,LPMEASUREITEMSTRUCT lpMeasureItemStruct);

      // MDI message handler member functions
      void OnMDIActivate(bool bActivate,
         ::window_sp pActivateWnd,::window_sp pDeactivateWnd);

      // menu loop notification messages
      void OnEnterMenuLoop(bool bIsTrackPopupMenu);
      void OnExitMenuLoop(bool bIsTrackPopupMenu);

      // Win4 messages
      void OnStyleChanged(int32_t nStyleType,LPSTYLESTRUCT lpStyleStruct);
      void OnStyleChanging(int32_t nStyleType,LPSTYLESTRUCT lpStyleStruct);
      void OnSizing(UINT nSide,LPRECT lpRect);
      void OnMoving(UINT nSide,LPRECT lpRect);
      void OnCaptureChanged(::window_sp pwindow);
      bool OnDeviceChange(UINT nEventType,uint_ptr dwData);

      // Overridables and other helpers (for implementation of derived classes)
      // for deriving from a standard control
      virtual WNDPROC* GetSuperWndProcAddr();

      // for dialog data exchange and validation
      //      virtual void do_data_exchange(CDataExchange* pDX);

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
      virtual LRESULT DefWindowProc(UINT message,WPARAM wParam,lparam lParam);

      // for custom cleanup after WM_NCDESTROY
      virtual void PostNcDestroy();

      // for notifications from parent
      virtual bool OnChildNotify(::message::base * pbase);
      // return TRUE if parent should not process this message
      virtual bool ReflectChildNotify(::message::base * pbase);
      virtual bool ReflectMessage(oswindow oswindow_Child,::message::base * pbase);

      // Implementation
      virtual bool CheckAutoCenter();
      virtual bool GrayCtlColor(HDC hDC,oswindow oswindow,UINT nCtlColor,
         HBRUSH hbrGray,COLORREF clrText);


      // helper routines for implementation
      bool HandleFloatingSysCommand(UINT nID,LPARAM lParam);
      bool IsTopParentActive();
      void ActivateTopParent();
      virtual ::user::interaction * GetDescendantWindow(::user::interaction * pui, id id);
      virtual void SendMessageToDescendants(oswindow  oswindow,UINT message,WPARAM wParam,lparam lParam,bool bDeep,bool bOnlyPerm);
      virtual void on_final_release();
      virtual bool ModifyStyle(oswindow oswindow,uint32_t dwRemove,uint32_t dwAdd,UINT nFlags);
      virtual bool ModifyStyleEx(oswindow oswindow,uint32_t dwRemove,uint32_t dwAdd,UINT nFlags);
      //virtual void _FilterToolTipMessage(MSG* pMsg,::window_sp pwindow);
      bool _EnableToolTips(bool bEnable,UINT nFlag);
      virtual oswindow get_safe_owner(::oswindow oswindow,::oswindow * pWndTop);
      void PrepareForHelp();


      friend class frame_window;

      bool CreateDlg(const char * lpszTemplateName,::window_sp pParentWnd);


      CLASS_DECL_BASE friend LRESULT CALLBACK __send_message_hook(int32_t,WPARAM,LPARAM);
      CLASS_DECL_BASE friend LRESULT CALLBACK __cbt_filter_hook(int32_t,WPARAM,LPARAM);

      // standard message implementation
      LRESULT OnNTCtlColor(WPARAM wParam,LPARAM lParam);
      LRESULT OnDisplayChange(WPARAM,LPARAM);
      LRESULT OnDragList(WPARAM,LPARAM);



      virtual void get_app_wnda(user::oswindow_array & wnda);

      virtual void _001DeferPaintLayeredWindowBackground(HDC hdc);

      virtual LONG get_window_long(int32_t nIndex) const;
      virtual LONG set_window_long(int32_t nIndex,LONG lValue);
      virtual LONG_PTR get_window_long_ptr(int32_t nIndex) const;
      virtual LONG_PTR set_window_long_ptr(int32_t nIndex,LONG_PTR lValue);


      void _001OnTriggerMouseInside();

      void set_viewport_org(::draw2d::graphics * pgraphics);

      wstring calc_icon_window_class(uint32_t dwDefaultStyle,const char * pszMatter);
      wstring calc_window_class();

      void on_set_parent(::user::interaction * pui);

      
      virtual bool get_rect_normal(LPRECT lprect);
      virtual void register_drop_target();

      virtual void show_task(bool bShow);


   };


} // namespace windows





