#pragma once


#include "user_xfplayer_view_line_selection.h"


class xfplayer_view_linea;


class CLASS_DECL_CORE xfplayer_view_line  :
   virtual public ::object
{
public:


   enum EAnimate
   {
      AnimateNoAnimate = 0,
      AnimateRHL = 1 // Rotates Horizontally Left
   };


   enum EEffect
   {
      EffectSimple = 0,
      EffectEmbossed = 1
   };


   bool                             m_bColonPrefix;


   XfplayerViewLineSelection        m_selection;
   LPBYTE                           m_lpBitmapData;
   ::visual::graphics_extension     m_dcextension;

   index                            m_iLinkHoverIndex;
   index                            m_iIndex;

   string                           m_str;
   string                           m_strPrefix;
   string                           m_strRoot;
   int_array                        m_iaPosition;

   rect                                m_rect;

   int32_t                              m_iIndent;
   bool                             m_bPendingLayoutUpdate;
   bool                             m_bVisible;
   bool                             m_bAutoSizeX;
   bool                             m_bAutoSizeY;
   int32_t                              m_iAlign;
   ::user::interaction *            m_oswindow;
   xfplayer_view_linea *              m_pContainer;

   // 08, May 2004 attributes
   ::draw2d::graphics_sp                m_dc1;
   ::draw2d::bitmap_sp                  m_bmp1;
   ::draw2d::font_sp                    m_font;
   ::draw2d::font_sp                    m_fontPrefix;
   ::draw2d::font_sp                    m_fontLink;
   COLORREF                         m_cr;
   COLORREF                         m_crOutline;
   rect                             m_rectClient;
   double                           m_floatRateX;
   LOGFONTW                          m_logfont;


   // 09, Sept 2004 attributes with change inApril 2010 from dcMain/bmpMain to dibMain
   ::draw2d::dib_sp                     m_dibMain;
   bool                             m_bEnhancedEmboss;
   bool                             m_bCacheEmboss;
   string                           m_strCache;

   // Link
   LOGFONTW                          m_logfontLink;
   stringa                          m_straLink;
   index_array                      m_iaLinkStart;
   index_array                      m_iaLinkEnd;


   double                           m_dXfplayerViewLineBlend;
   int32_t                              m_nFont;
   rect                             m_rectInvalidate;
   int32_t                              m_iAnimateType;
   double                           m_dAnimateProgress;
   double                           m_dAnimateProgressIncrement;
   int32_t                              m_iTextEffect;
   ::draw2d::pen *                      m_lpPenEmboss;
   COLORREF                         m_crForeground;
   ::draw2d::pen *                      m_ppenLyricLeft;
   ::draw2d::pen *                      m_ppenLyricRight;
   size                             m_sizeLyricMargin;
   COLORREF                         m_crLyricLeft;
   COLORREF                         m_crLyricRight;

   xfplayer_view_line(::aura::application * papp);
   xfplayer_view_line(const xfplayer_view_line & line);
   xfplayer_view_line(xfplayer_view_linea * pContainer);
   virtual ~xfplayer_view_line();


   void SetBlend(double d);
   ::draw2d::font * GetFont();
   bool CalcChar(point pt, strsize &iChar);
   index GetCharLink(strsize iChar);
   bool GetCharLink(string & str, strsize iChar);
   bool CharHasLink(strsize iChar);
   user::e_line_hit GetLink(string & strUrl, point pt);
   index GetLinkIndex(index iLine, strsize iChar);
   bool HasLink();
   bool IsInHover();
   void UpdateHover(point & ptCursor);
   void PrepareURLLinks();
   void SetFont(visual::font * pfont);
   void SetFont(::draw2d::font * pfont);
   void GetLogFont(LOGFONTW & lf);

   user::e_line_hit hit_test(const POINT &ptCursorParam, strsize &iChar);


/*   void CalcCharsPositions(
      ::draw2d::graphics * pdcForeground,
      visual::font * pFont,
      const RECT & rect);*/

   void CalcCharsPositions(
      ::draw2d::graphics * pdcForeground,
      const RECT & rect);

   void SetColors(COLORREF cr, COLORREF crOutline);

   void EmbossedTextOut(
      ::draw2d::graphics * pgraphics,
      const char * lpcsz,
      int32_t left,
      int32_t top,
      int32_t ignore1,
      COLORREF cr,
      COLORREF crOutline,
      strsize iLen,
      double dBlend);

   void EmbossedTextOut(
      ::draw2d::graphics * pgraphics,
      ::draw2d::dib * pdibCache,
      const char * lpcsz,
      int32_t iLeft,
      int32_t iTop,
      int32_t iWidth,
      COLORREF cr,
      COLORREF crOutline,
      strsize iLen,
      double dBlend);


   void CacheEmboss(
      ::draw2d::graphics * pgraphics,
      const char * lpcsz,
      strsize iLen,
      ::draw2d::dib * pdibCache);


   bool IsVisible();
   void Validate(const RECT & rect = ::null_rect());
   void Invalidate(const RECT & rect = ::null_rect());
   //void AddVmsFont(visual::font * pfont);
   //int32_t GetVmsFontCount();
   void SetPlacement(const RECT & rect);
   int32_t SetLyricColors(COLORREF crLeft, COLORREF crRight);
   int32_t SetLyricPens(::draw2d::pen * ppenLeft, ::draw2d::pen * ppenRight);
   void SetRenderCriticalSection(::critical_section *pcs);
   void SetAnimateIncrement(double dIncrement);
   int32_t MapToFontEffect(int32_t iLineEffect);


   void SetForegroundColor(COLORREF cr);
   void SetEmbossPen(::draw2d::pen * lpPen);
   void SetTextEffect(int32_t iTextEffect);

   void SetAnimateType(int32_t iAnimateType);

   void OnTimerAnimate(::draw2d::graphics * pgraphics, rect_array &   rectaModified);

   void Show(bool bShow = true);
   virtual xfplayer_view_line & operator = (const xfplayer_view_line & src);
   void GetPlacement(LPRECT lpRect);
   void SetPlaement(const RECT & rect);
   void SetAlign(int32_t iAlign);
   static const int32_t AlignLeft;
   static const int32_t AlignRight;

   void SetAutoSize(bool bAutoSize);



   bool PrepareLine(::draw2d::graphics * pgraphics, const string & str, int32_t flags, const RECT & rect);

   void AddChar(
      WCHAR wch,
      strsize &index);

   void AddChar(
      WCHAR wch,
      strsize &index,
      visual::font * pFont);

   bool to(::draw2d::graphics * pgraphics, bool bDraw, const RECT & rect, rect_array & rectaModified, bool bRecalcLayout);

   bool to(::draw2d::graphics * pgraphics, bool bDraw, const RECT & rect, rect_array & rectaModified, strsize * count, bool bRecalcLayout, COLORREF crColor, ::draw2d::pen_sp sppen);


   DECL_GEN_SIGNAL(OnMouseMove);
   DECL_GEN_SIGNAL(OnLButtonDown);
   DECL_GEN_SIGNAL(OnLButtonUp);
   virtual void _001OnTimer(::timer * ptimer);
   DECL_GEN_SIGNAL(OnSetCursor);

   XfplayerViewLineSelection::e_state GetSelectionState();
   XfplayerViewLineSelection & GetSelection();
   ::user::interaction * get_interaction();


};



