#pragma once


namespace draw2d_direct2d
{


   class CLASS_DECL_DRAW2D_DIRECT2D graphics : 
      virtual public ::draw2d::graphics
   {
   public:

      class state
      {
      public:

         D2D1::Matrix3x2F              m_m;
         array < D2D1::Matrix3x2F >    m_maRegion;
         spa(::draw2d::region)         m_sparegionClip;

      };


      Microsoft::WRL::ComPtr<ID2D1Device>                m_pdevice;
      Microsoft::WRL::ComPtr<ID2D1RenderTarget>          m_prendertarget;
      Microsoft::WRL::ComPtr<ID2D1DeviceContext>         m_pdevicecontext;
      Microsoft::WRL::ComPtr<ID2D1BitmapRenderTarget>    m_pbitmaprendertarget;
      Microsoft::WRL::ComPtr<ID2D1DCRenderTarget>        m_pdcrendertarget;

      Microsoft::WRL::ComPtr<IDXGIAdapter>               m_padapter;
      Microsoft::WRL::ComPtr<IDXGIFactory2>              m_pfactory2;
      Microsoft::WRL::ComPtr<ID2D1Layer>                 m_player;
      Microsoft::WRL::ComPtr<ID2D1PathGeometry>          m_ppathgeometryClip;

      array < state >                                    m_statea;
      state                                              m_state;

      bool m_bSaveClip;


      //array <ID2D1Layer * >       m_layerptra;


      int                              m_iType;

      D2D1_BITMAP_INTERPOLATION_MODE   m_bitmapinterpolationmode;
      D2D1_INTERPOLATION_MODE          m_interpolationmode;

/*      ::Gdiplus::Graphics *         m_pgraphics;
      ::Gdiplus::GraphicsPath *     m_ppath;
      ::Gdiplus::GraphicsPath *     m_ppathPaint;
      HDC                           m_hdc;*/

      // advanced use and implementation
      bool                             m_bPrinting;

      HDC                              m_hdcAttach;

      enum e_data
      {
         data_device_context,
         data_render_target,
      };

      graphics();
      graphics(::aura::application * papp);
      virtual ~graphics();


      ID2D1Device & d()
      {
         return *m_pdevice.Get();
      }

      bool IsPrinting() const;            // TRUE if being used for printing

      ::draw2d::pen_sp     get_current_pen() const;
      ::draw2d::brush_sp   get_current_brush() const;
      ::draw2d::palette_sp get_current_palette() const;
      ::draw2d::font_sp    get_current_font() const;
      ::draw2d::bitmap_sp  get_current_bitmap() const;


      /*
      Gdiplus::Pen *       direct2d_pen();
      Gdiplus::Brush *     direct2d_brush();
      Gdiplus::Font *      direct2d_font();
      */

      // for bidi and mirrored localization
      uint32_t GetLayout() const;
      uint32_t SetLayout(uint32_t dwLayout);


      virtual bool SaveClip();
      virtual bool RestoreClip();

      virtual double get_dpix() const;

   // Constructors
      bool CreateDC(const char * lpszDriverName, const char * lpszDeviceName,
         const char * lpszOutput, const void * lpInitData);
      bool CreateIC(const char * lpszDriverName, const char * lpszDeviceName,
         const char * lpszOutput, const void * lpInitData);
      bool CreateCompatibleDC(::draw2d::graphics * pgraphics);

      bool DeleteDC();

   // Device-Context Functions
      virtual int SaveDC();
      virtual bool RestoreDC(int nSavedDC);
      int GetDeviceCaps(int nIndex) const;
      UINT SetBoundsRect(const RECT & lpRectBounds, UINT flags);
      UINT GetBoundsRect(LPRECT lpRectBounds, UINT flags);
      bool ResetDC(const DEVMODE* lpDevMode);

   // Drawing-Tool Functions
      point GetBrushOrg() const;
      point SetBrushOrg(int x, int y);
      point SetBrushOrg(POINT point);
      int EnumObjects(int nObjectType,
            int (CALLBACK* lpfn)(LPVOID, LPARAM), LPARAM lpData);

   // Type-safe selection helpers
   public:
      virtual ::draw2d::object* SelectStockObject(int nIndex);
      bool SelectFont(::draw2d::font * pfont);
      ::draw2d::pen* SelectObject(::draw2d::pen* pPen);
      ::draw2d::brush* SelectObject(::draw2d::brush* pBrush);
      virtual ::draw2d::font* SelectObject(::draw2d::font* pFont);
      ::draw2d::bitmap* SelectObject(::draw2d::bitmap* pBitmap);
      int SelectObject(::draw2d::region* pRgn);       // special return for regions
      ::draw2d::object* SelectObject(::draw2d::object* pObject);
         // ::draw2d::object* provided so compiler doesn't use SelectObject(HGDIOBJ)

   // color and color Palette Functions
      COLORREF GetNearestColor(COLORREF crColor) const;
      ::draw2d::palette* SelectPalette(::draw2d::palette* pPalette, bool bForceBackground);
      UINT RealizePalette();
      void UpdateColors();

      int GetPolyFillMode() const;
      int GetROP2() const;
      int GetStretchBltMode() const;

      int SetPolyFillMode(int nPolyFillMode);
      int SetROP2(int nDrawMode);
      int SetStretchBltMode(int nStretchMode);


   #if (_WIN32_WINNT >= 0x0500)

      COLORREF GetDCBrushColor() const;
      COLORREF SetDCBrushColor(COLORREF crColor);

      COLORREF GetDCPenColor() const;
      COLORREF SetDCPenColor(COLORREF crColor);

   #endif

      // Graphics mode
      int SetGraphicsMode(int iMode);
      int GetGraphicsMode() const;


      virtual bool draw_path(::draw2d::path * ppath);
      virtual bool fill_path(::draw2d::path * ppath);
      virtual bool path(::draw2d::path * ppath);

      virtual bool draw_path(::draw2d::path * ppath, ::draw2d::pen * ppen);
      virtual bool fill_path(::draw2d::path * ppath, ::draw2d::brush * pbrush);

      // World transform
      bool SetWorldTransform(const XFORM* pXform);
      bool ModifyWorldTransform(const XFORM* pXform,uint32_t iMode);
      bool GetWorldTransform(XFORM* pXform) const;

      // Mapping Functions
      virtual int GetMapMode() const;
      virtual point GetViewportOrg() const;
      virtual int SetMapMode(int nMapMode);
      // Viewport Origin
      virtual point SetViewportOrg(int x, int y);
      virtual point SetViewportOrg(POINT point);
      virtual point OffsetViewportOrg(int nWidth, int nHeight);

      // Viewport Extent
      virtual size GetViewportExt() const;
      virtual size SetViewportExt(int cx, int cy);
      virtual size SetViewportExt(SIZE size);
      virtual size ScaleViewportExt(int xNum, int xDenom, int yNum, int yDenom);

      // Window Origin
      point GetWindowOrg() const;
      point SetWindowOrg(int x, int y);
      point SetWindowOrg(POINT point);
      point OffsetWindowOrg(int nWidth, int nHeight);

      // Window extent
      size GetWindowExt() const;
      virtual size SetWindowExt(int cx, int cy);
            size SetWindowExt(SIZE size);
      virtual size ScaleWindowExt(int xNum, int xDenom, int yNum, int yDenom);

   // Coordinate Functions
      void DPtoLP(LPPOINT lpPoints,count nCount = 1) const;
      void DPtoLP(LPRECT lpRect) const;
      void DPtoLP(LPSIZE lpSize) const;
      void LPtoDP(LPPOINT lpPoints,count nCount = 1) const;
      void LPtoDP(LPRECT lpRect) const;
      void LPtoDP(LPSIZE lpSize) const;

   // Special Coordinate Functions (useful for dealing with metafiles and OLE)
      void DPtoHIMETRIC(LPSIZE lpSize) const;
      void LPtoHIMETRIC(LPSIZE lpSize) const;
      void HIMETRICtoDP(LPSIZE lpSize) const;
      void HIMETRICtoLP(LPSIZE lpSize) const;

   // Region Functions
      bool FillRgn(::draw2d::region* pRgn, ::draw2d::brush* pBrush);
      bool FrameRgn(::draw2d::region* pRgn, ::draw2d::brush* pBrush, int nWidth, int nHeight);
      bool InvertRgn(::draw2d::region* pRgn);
      bool PaintRgn(::draw2d::region* pRgn);

   // Clipping Functions
      virtual int GetClipBox(LPRECT lpRect) const;
      virtual bool PtVisible(int x, int y) const;
            bool PtVisible(POINT point) const;
      virtual bool RectVisible(const RECT & lpRect) const;
            int SelectClipRgn(::draw2d::region* pRgn);
            int ExcludeClipRect(int x1, int y1, int x2, int y2);
            int ExcludeClipRect(const RECT & lpRect);
            int ExcludeUpdateRgn(::user::primitive * pwindow);
            int IntersectClipRect(int x1, int y1, int x2, int y2);
            int IntersectClipRect(const RECT & lpRect);
            int OffsetClipRgn(int x, int y);
            int OffsetClipRgn(SIZE size);
      int SelectClipRgn(::draw2d::region* pRgn, int nMode);

   // Line-Output Functions
      point GetCurrentPosition() const;
//      point MoveTo(int x, int y);
  //    point MoveTo(POINT point);
      bool LineTo(double x, double y);
    //  bool LineTo(POINT point);
      bool Arc(double x1, double y1, double w, double h, double start, double extends);
      bool Arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
      bool Arc(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
      bool Arc(const RECT & lpRect, POINT ptStart, POINT ptEnd);
      bool Polyline(const POINT* lpPoints,count nCount);

      bool AngleArc(int x, int y, int nRadius, float fStartAngle, float fSweepAngle);
      bool ArcTo(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
      bool ArcTo(const RECT & lpRect, POINT ptStart, POINT ptEnd);
      int GetArcDirection() const;
      int SetArcDirection(int nArcDirection);

      bool PolyDraw(const POINT* lpPoints, const BYTE* lpTypes,count nCount);
      bool PolylineTo(const POINT* lpPoints,count nCount);
      bool PolyPolyline(const POINT* lpPoints, const INT * lpPolyPoints,count nCount);

      bool PolyBezier(const POINT* lpPoints,count nCount);
      bool PolyBezierTo(const POINT* lpPoints,count nCount);

   // Simple Drawing Functions
      virtual void FillRect(const RECT & lpRect, ::draw2d::brush* pBrush);
      virtual void FrameRect(const RECT & lpRect, ::draw2d::brush* pBrush);
      virtual bool DrawRect(const RECT & lpRect, ::draw2d::pen* ppen);


      void InvertRect(const RECT & lpRect);
      bool DrawIcon(int x, int y, ::visual::icon * picon);
      bool DrawIcon(POINT point, ::visual::icon * picon);
      bool DrawIcon(int x, int y, ::visual::icon * picon, int cx, int cy, UINT istepIfAniCur, HBRUSH hbrFlickerFreeDraw, UINT diFlags);
      bool DrawState(point pt, size size, HBITMAP hBitmap, UINT nFlags,
         HBRUSH hBrush = NULL);
      bool DrawState(point pt, size size, ::draw2d::bitmap* pBitmap, UINT nFlags,
         ::draw2d::brush* pBrush = NULL);
      bool DrawState(point pt, size size, HICON hIcon, UINT nFlags,
         HBRUSH hBrush = NULL);
      bool DrawState(point pt, size size, HICON hIcon, UINT nFlags,
         ::draw2d::brush* pBrush = NULL);
      bool DrawState(point pt, size size, const char * lpszText, UINT nFlags,
         bool bPrefixText = TRUE, int nTextLen = 0, HBRUSH hBrush = NULL);
      bool DrawState(point pt, size size, const char * lpszText, UINT nFlags,
         bool bPrefixText = TRUE, int nTextLen = 0, ::draw2d::brush* pBrush = NULL);
//      bool DrawState(point pt, size size, DRAWSTATEPROC lpDrawProc,
  //       LPARAM lData, UINT nFlags, HBRUSH hBrush = NULL);
    //  bool DrawState(point pt, size size, DRAWSTATEPROC lpDrawProc,
      //   LPARAM lData, UINT nFlags, ::draw2d::brush* pBrush = NULL);

   // Ellipse and Polygon Functions
      bool Chord(int x1, int y1, int x2, int y2, int x3, int y3,
         int x4, int y4);
      bool Chord(const RECT & lpRect, POINT ptStart, POINT ptEnd);
      void DrawFocusRect(const RECT & lpRect);
      bool DrawEllipse(int x1, int y1, int x2, int y2);
      bool DrawEllipse(const RECT & lpRect);
      bool DrawEllipse(const RECTD & lpRect);
      bool FillEllipse(int x1, int y1, int x2, int y2);
      bool FillEllipse(const RECT & lpRect);
      bool FillEllipse(const RECTD & lpRect);
      bool Pie(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
      bool Pie(const RECT & lpRect, POINT ptStart, POINT ptEnd);
      bool Polygon(const POINT* lpPoints,count nCount);
      bool Polygon(const POINTD* lpPoints,count nCount);
      bool PolyPolygon(const POINT* lpPoints, const INT * lpPolyCounts,count nCount);

      virtual bool fill_polygon(const POINTD * lpPoints,count nCount);

      bool Rectangle(int x1, int y1, int x2, int y2);
      bool Rectangle(const RECT & lpRect);
      virtual bool DrawRectangle(int x1, int y1, int x2, int y2);
      virtual bool DrawRectangle(const RECT & lpRect);
      virtual bool FillRectangle(int x1, int y1, int x2, int y2);
      virtual bool FillRectangle(const RECT & lpRect);
      bool RoundRect(int x1, int y1, int x2, int y2, int x3, int y3);
      bool RoundRect(const RECT & lpRect, POINT point);

   // Bitmap Functions
      bool PatBlt(int x, int y, int nWidth, int nHeight, uint32_t dwRop);
      bool BitBltRaw(int x, int y, int nWidth, int nHeight, ::draw2d::graphics * pgraphicsSrc,
         int xSrc, int ySrc, uint32_t dwRop);
      bool StretchBltRaw(int x, int y, int nWidth, int nHeight, ::draw2d::graphics * pgraphicsSrc,
         int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, uint32_t dwRop);
      COLORREF GetPixel(int x, int y) const;
      COLORREF GetPixel(POINT point) const;
      COLORREF SetPixel(int x, int y, COLORREF crColor);
      COLORREF SetPixel(POINT point, COLORREF crColor);
      bool FloodFill(int x, int y, COLORREF crColor);
      bool ExtFloodFill(int x, int y, COLORREF crColor, UINT nFillType);
      bool MaskBlt(int x, int y, int nWidth, int nHeight, ::draw2d::graphics * pgraphicsSrc,
         int xSrc, int ySrc, ::draw2d::bitmap& maskBitmap, int xMask, int yMask,
         uint32_t dwRop);
      bool PlgBlt(LPPOINT lpPoint, ::draw2d::graphics * pgraphicsSrc, int xSrc, int ySrc,
         int nWidth, int nHeight, ::draw2d::bitmap& maskBitmap, int xMask, int yMask);
      bool SetPixelV(int x, int y, COLORREF crColor);
      bool SetPixelV(POINT point, COLORREF crColor);
      bool GradientFill(TRIVERTEX* pVertices, ULONG nVertices, 
        void * pMesh, ULONG nMeshElements, uint32_t dwMode);
      bool TransparentBlt(int xDest, int yDest, int nDestWidth, int nDestHeight,
        ::draw2d::graphics * pgraphicsSrc, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, 
        UINT clrTransparent);

      virtual bool alpha_blendRaw(int xDest, int yDest, int nDestWidth, int nDestHeight, ::draw2d::graphics * pgraphicsSrc, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, double dOpacity);

      /*bool alpha_blend(int xDest, int yDest, int nDestWidth, int nDestHeight,
        ::draw2d::graphics * pgraphicsSrc, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, 
        BLENDFUNCTION blend);*/

   // Text Functions
      virtual bool text_out(int x, int y, const char * lpszString, strsize nCount);
      virtual bool text_out(int x, int y, const string & str);
      virtual bool TextOutRaw(double x, double y, const char * lpszString, strsize nCount);
      virtual bool text_out(double x, double y, const string & str);
      virtual bool ExtTextOut(int x, int y, UINT nOptions, const RECT & lpRect, const char * lpszString, strsize nCount, LPINT lpDxWidths);
      virtual bool ExtTextOut(int x, int y, UINT nOptions, const RECT & lpRect, const string & str, LPINT lpDxWidths);
      virtual size TabbedTextOut(int x, int y, const char * lpszString, strsize nCount, count nTabPositions, LPINT lpnTabStopPositions, int32_t nTabOrigin);
      virtual size TabbedTextOut(int x, int y, const string & str, count nTabPositions, LPINT lpnTabStopPositions, int32_t nTabOrigin);

      virtual int draw_text(const char * lpszString, strsize nCount, const RECT & lpRect, UINT nFormat);
      virtual int draw_text(const string & str,const RECT & lpRect,UINT nFormat);

//      virtual int draw_text_ex(LPTSTR lpszString, int nCount, LPRECT lpRect, UINT nFormat, LPDRAWTEXTPARAMS lpDTParams);
  //    virtual int draw_text_ex(const string & str, LPRECT lpRect, UINT nFormat, LPDRAWTEXTPARAMS lpDTParams);

      virtual sized GetTextExtent(const char * lpszString, strsize nCount, strsize iIndex) const override;
      virtual sized GetTextExtent(const char * lpszString, strsize nCount) const override;
      virtual sized GetTextExtent(const string & str) const override;
      virtual bool GetTextExtent(sized & size, const char * lpszString, strsize nCount, strsize iIndex) const;
      virtual bool GetTextExtent(sized & size, const char * lpszString, strsize nCount) const;
      virtual bool GetTextExtent(sized & size, const string & str) const;
      virtual size GetOutputTextExtent(const char * lpszString, strsize nCount) const;
      virtual size GetOutputTextExtent(const string & str) const;
      virtual size GetTabbedTextExtent(const char * lpszString, strsize nCount, count nTabPositions, LPINT lpnTabStopPositions) const;
      virtual size GetTabbedTextExtent(const string & str, count nTabPositions, LPINT lpnTabStopPositions) const;
      virtual size GetOutputTabbedTextExtent(const char * lpszString, strsize nCount, count nTabPositions, LPINT lpnTabStopPositions) const;
      virtual size GetOutputTabbedTextExtent(const string & str, count  nTabPositions, LPINT lpnTabStopPositions) const;
      virtual bool GrayString(::draw2d::brush* pBrush, bool (CALLBACK* lpfnOutput)(HDC, LPARAM, int), LPARAM lpData, int nCount, int x, int y, int nWidth, int nHeight);
      virtual UINT GetTextAlign() const;
      virtual UINT SetTextAlign(UINT nFlags);
      virtual int GetTextFace(count nCount, LPTSTR lpszFacename) const;
      virtual int GetTextFace(string & rString) const;
      virtual bool get_text_metrics(::draw2d::text_metric * lpMetrics) const;
      virtual bool get_output_text_metrics(::draw2d::text_metric * lpMetrics) const;
      virtual int SetTextJustification(int nBreakExtra, int nBreakCount);
      virtual int GetTextCharacterExtra() const;
      virtual int SetTextCharacterExtra(int nCharExtra);

//      uint32_t GetCharacterPlacement(const char * lpString, int nCount, int nMaxExtent, LPGCP_RESULTS lpResults, uint32_t dwFlags) const;
  //    uint32_t GetCharacterPlacement(string & str, int nMaxExtent, LPGCP_RESULTS lpResults, uint32_t dwFlags) const;

   #if (_WIN32_WINNT >= 0x0500)

      bool GetTextExtentExPointI(LPWORD pgiIn, int cgi, int nMaxExtent, LPINT lpnFit, LPINT alpDx, __out_opt LPSIZE lpSize) const;
      bool GetTextExtentPointI(LPWORD pgiIn, int cgi, __out_opt LPSIZE lpSize) const;

   #endif



   // Advanced Drawing
      bool DrawEdge(LPRECT lpRect, UINT nEdge, UINT nFlags);
      bool DrawFrameControl(LPRECT lpRect, UINT nType, UINT nState);

   // Scrolling Functions
      bool ScrollDC(int dx, int dy, const RECT & lpRectScroll, const RECT & lpRectClip,
         ::draw2d::region* pRgnUpdate, LPRECT lpRectUpdate);

   // font Functions
      bool GetCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const;
      bool GetOutputCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const;
      uint32_t SetMapperFlags(uint32_t dwFlag);
      size GetAspectRatioFilter() const;

//      bool GetCharABCWidths(UINT nFirstChar, UINT nLastChar, LPABC lpabc) const;
      uint32_t GetFontData(uint32_t dwTable, uint32_t dwOffset, LPVOID lpData, uint32_t cbData) const;
//      int GetKerningPairs(int nPairs, LPKERNINGPAIR lpkrnpair) const;
//      UINT GetOutlineTextMetrics(UINT cbData, LPOUTLINETEXTMETRICW lpotm) const;
//      uint32_t GetGlyphOutline(UINT nChar, UINT nFormat, LPGLYPHMETRICS lpgm,
//         uint32_t cbBuffer, LPVOID lpBuffer, const MAT2* lpmat2) const;

//      bool GetCharABCWidths(UINT nFirstChar, UINT nLastChar,
//         LPABCFLOAT lpABCF) const;
      bool GetCharWidth(UINT nFirstChar, UINT nLastChar,
         float* lpFloatBuffer) const;

      uint32_t GetFontLanguageInfo() const;

   #if (_WIN32_WINNT >= 0x0500)

//      bool GetCharABCWidthsI(UINT giFirst, UINT cgi, LPWORD pgi, LPABC lpabc) const;
      bool GetCharWidthI(UINT giFirst, UINT cgi, LPWORD pgi, LPINT lpBuffer) const;

   #endif

   // Printer/Device Escape Functions
      virtual int Escape(int nEscape, int nCount, const char * lpszInData, LPVOID lpOutData);
      virtual int Escape(int nEscape, int nInputSize, const char * lpszInputData, int nOutputSize, char * lpszOutputData);
      virtual int DrawEscape(int nEscape, int nInputSize, const char * lpszInputData);

      // Escape helpers
      int StartDoc(const char * lpszDocName);  // old Win3.0 version
//      int StartDoc(LPDOCINFO lpDocInfo);
      int StartPage();
      int EndPage();
      int SetAbortProc(bool (CALLBACK* lpfn)(HDC, int));
      int AbortDoc();
      int EndDoc();

   // MetaFile Functions
      bool PlayMetaFile(HMETAFILE hMF);
      bool PlayMetaFile(HENHMETAFILE hEnhMetaFile, const RECT & lpBounds);
      bool AddMetaFileComment(UINT nDataSize, const BYTE* pCommentData);
         // can be used for enhanced metafiles only

   // Path Functions
      bool AbortPath();
      bool BeginPath();
      bool CloseFigure();
      bool EndPath();
      bool FillPath();
      bool FlattenPath();
      bool StrokeAndFillPath();
      bool StrokePath();
      bool WidenPath();
      float GetMiterLimit() const;
      bool SetMiterLimit(float fMiterLimit);
      int GetPath(LPPOINT lpPoints, LPBYTE lpTypes, int nCount) const;
      bool SelectClipPath(int nMode);

   // Misc Helper Functions
      static ::draw2d::brush* GetHalftoneBrush(::aura::application * papp);
      void DrawDragRect(const RECT & lpRect, SIZE size,
         const RECT & lpRectLast, SIZE sizeLast,
         ::draw2d::brush* pBrush = NULL, ::draw2d::brush* pBrushLast = NULL);
      void FillSolidRect(const RECT64 * lpRect, COLORREF clr);
      void FillSolidRect(const RECT & lpRect, COLORREF clr);
      void FillSolidRect(int x, int y, int cx, int cy, COLORREF clr);
      void Draw3dRect(const RECT & lpRect, COLORREF clrTopLeft, COLORREF clrBottomRight);
      void Draw3dRect(int x, int y, int cx, int cy,
         COLORREF clrTopLeft, COLORREF clrBottomRight);




   // Implementation
   public:
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;

      //HGDIOBJ SelectObject(HGDIOBJ);      // do not use for regions

      virtual void set_alpha_mode(::draw2d::e_alpha_mode ealphamode);

      virtual void set_text_rendering(::draw2d::e_text_rendering etextrendering);

      virtual void * get_os_data() const;
      virtual void * get_os_data_ex(int i) const;
      virtual HDC get_handle() const;
      virtual HDC get_handle1() const;
      virtual HDC get_handle2() const;

      virtual bool attach(void * pdata) override;
      virtual void * detach() override;

#ifdef WINDOWSEX
      virtual bool Attach(HDC hdc) override;
      virtual HDC Detach() override;
#endif

//      virtual Gdiplus::FillMode direct2d_get_fill_mode();

      bool blur(bool bExpand, double dRadius, const RECT & lpcrect);


      bool destroy();

      IDWriteTextFormat * get_os_font(::draw2d::font * pfont) const;
      ID2D1Brush * get_os_brush(::draw2d::brush * pbrush) const;
      ID2D1Brush * get_os_pen_brush(::draw2d::pen * ppen) const;

      virtual bool draw(const ::draw2d::path::string_path & path, ::draw2d::pen * ppen);

      virtual bool fill(const ::draw2d::path::string_path & path, ::draw2d::brush * pbrush);

      virtual bool flush() override;

      virtual void debug() override;
   //protected:
      // used for implementation of non-virtual SelectObject calls
      //static ::draw2d::object* SelectGdiObject(application * papp, HDC hDC, HGDIOBJ h);

      virtual void enum_fonts(::draw2d::font::enum_item_array & itema) override;


   };


} // namespace draw2d_direct2d










