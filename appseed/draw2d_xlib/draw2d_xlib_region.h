#pragma once


namespace draw2d_xlib
{


   class CLASS_DECL_DRAW2D_XLIB region :
      virtual public ::draw2d::region
   {
   public:

//      xlib_surface_t *       m_psurface;
      rect                    m_rectBoundingBoxInternal;


// xxx     Gdiplus::Region *       m_pregion;

      //static region * PASCAL from_handle(::aura::application * papp, HRGN hRgn);
      //operator HRGN() const;

      region(::aura::application * papp);
      virtual ~region();


      //virtual void * get_os_data() const;


      /*bool CreateRectRgn(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
      bool CreateRectRgnIndirect(LPCRECT lpRect);
      bool CreateEllipticRgn(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
      bool CreateEllipticRgnIndirect(LPCRECT lpRect);
      bool CreatePolygonRgn(LPPOINT lpPoints, int32_t nCount, int32_t nMode);
      bool CreatePolyPolygonRgn(LPPOINT lpPoints, LPINT lpPolyCounts,
            int32_t nCount, int32_t nPolyFillMode);
      bool CreateRoundRectRgn(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3);
      bool CreateFromPath(::draw2d::dib * pdib);
      bool CreateFromData(const XFORM* lpXForm, int32_t nCount,
         const RGNDATA* pRgnData);*/

   // Operations
      //void SetRectRgn(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
      //void SetRectRgn(LPCRECT lpRect);
      //int32_t CombineRgn(const ::draw2d::region* pRgn1, const ::draw2d::region* pRgn2, int32_t nCombineMode);
      //int32_t CopyRgn(const ::draw2d::region* pRgnSrc);
      //bool EqualRgn(const ::draw2d::region* pRgn) const;
      //int32_t OffsetRgn(int32_t x, int32_t y);
      //int32_t OffsetRgn(POINT point);
      //int32_t GetRgnBox(LPRECT lpRect) const;
      //bool PtInRegion(int32_t x, int32_t y) const;
      //bool PtInRegion(POINT point) const;
      //bool RectInRegion(LPCRECT lpRect) const;
// xxx      int32_t GetRegionData(LPRGNDATA lpRgnData, int32_t nCount) const;

/*      bool get(xlib_t * pdc);
      bool get_rect(xlib_t * pdc);
      bool get_oval(xlib_t * pdc);
      bool get_polygon(xlib_t * pdc);
      bool get_poly_polygon(xlib_t * pdc);
      bool get_combine(xlib_t * pdc);


      virtual void * get_os_data() const;*/



   };


} // namespace draw2d_xlib


