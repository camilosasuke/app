#include "framework.h"
#include <math.h>


#define d2d1_fax_options D2D1_FACTORY_OPTIONS // fax of merde
#define multi_threaded D2D1_FACTORY_TYPE_MULTI_THREADED // ???? muliple performance multi thread hidden option there exists cost uses?

#ifdef WINDOWSEX

void my_debug();

#endif

namespace draw2d_direct2d
{

   
   graphics::graphics(::aura::application * papp) :
      ::object(papp),
      ::draw2d::graphics(papp)
   {
      m_bSaveClip = false;

      m_hdcAttach = NULL;

      m_pmutex                = new mutex(papp);


//      &draw2d_mutex() = &draw2d_mutex();

      m_sppen.alloc(allocer());


      m_iType     = 0;

      m_interpolationmode = D2D1_INTERPOLATION_MODE_HIGH_QUALITY_CUBIC;

      m_bitmapinterpolationmode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;


      

      /*m_bPrinting       = FALSE;
      m_pdibAlphaBlend  = NULL;
      m_prendertarget       = NULL;
      m_hdc             = NULL;
      m_ppath           = NULL;
      m_ppathPaint      = NULL;
      m_etextrendering  = ::draw2d::text_rendering_anti_alias_grid_fit;*/

   }

   graphics::~graphics()
   {

      destroy();

   }


   void graphics::assert_valid() const
   {

      object::assert_valid();

   }


   void graphics::dump(dump_context & dumpcontext) const
   {
      object::dump(dumpcontext);

      dumpcontext << "get_handle1() = " << get_handle1();
      dumpcontext << "\nm_hAttribDC = " << get_handle2();
      dumpcontext << "\nm_bPrinting = " << m_bPrinting;

      dumpcontext << "\n";
   }




   bool graphics::IsPrinting() const
   {

      return m_bPrinting; 

   }


   bool graphics::CreateDC(const char * lpszDriverName, const char * lpszDeviceName, const char * lpszOutput, const void * lpInitData)
   {
      throw todo(get_app());
      //return Attach(::CreateDC(lpszDriverName, lpszDeviceName, lpszOutput, (const DEVMODE*)lpInitData)); 
   }

   bool graphics::CreateIC(const char * lpszDriverName, const char * lpszDeviceName, const char * lpszOutput, const void * lpInitData)
   { 
      throw todo(get_app());
      //return Attach(::CreateIC(lpszDriverName, lpszDeviceName, lpszOutput, (const DEVMODE*) lpInitData)); 
   }

   bool graphics::CreateCompatibleDC(::draw2d::graphics * pgraphics)
   { 

      //single_lock sl(System.m_pmutexDc, true);

      synch_lock ml(&draw2d_mutex());

      if(m_iType != 0)
         destroy();

      /*if(pgraphics == NULL)
      {
         
         GetD2D1Factory1()->CreateDevice(TlsGetDXGIDevice(), &m_pdevice);

         m_pdevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_pdevicecontext);

         if(m_pdevicecontext == NULL)
         {
            m_pdevice->Release();
            m_pdevice = NULL;
            return false;
         }

         m_prendertarget = NULL;

         HRESULT hr = m_pdevicecontext->QueryInterface(IID_ID2D1RenderTarget, (void **) &m_prendertarget);

         if(FAILED(hr) || m_prendertarget == NULL)
         {
            m_pdevice->Release();
            m_pdevice = NULL;
            m_pdevicecontext->Release();
            m_pdevicecontext = NULL;
            return false;
         }


         m_iType = 3;

         return true;         
      }*/
      //else



      //Microsoft::WRL::ComPtr<ID2D1RenderTarget> prendertarget = pgraphics->get_typed_os_data < ID2D1RenderTarget >(::draw2d_direct2d::graphics::data_render_target);

      Microsoft::WRL::ComPtr<ID2D1RenderTarget> prendertarget;

      HRESULT hr;

      if(pgraphics == NULL || pgraphics->get_os_data() == NULL)
      {
         if(System.m_pdevicecontext == NULL)
         {
            prendertarget = nullptr;
         }
         else
         {
            hr = System.m_pdevicecontext->QueryInterface(IID_ID2D1RenderTarget,(void **)&prendertarget);
            if(FAILED(hr))
            {

               trace_hr("graphics::CreateCompatibleDC, QueryInterface (1) ",hr);

            }
         }
      }
      else
      {
         prendertarget = pgraphics->get_typed_os_data < ID2D1RenderTarget > (data_render_target);
      }



      if(prendertarget == NULL)
      {

         GetD2D1Factory1()->CreateDevice(TlsGetDXGIDevice(), &m_pdevice);

         m_pdevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_pdevicecontext);

         if(m_pdevicecontext == NULL)
         {
            m_pdevice = nullptr;
            return false;
         }

         m_prendertarget = nullptr;

         HRESULT hr = m_pdevicecontext.As(&m_prendertarget);

         if(FAILED(hr) || m_prendertarget == NULL)
         {
            m_pdevice = nullptr;
            m_pdevicecontext = nullptr;
            return false;
         }

         prendertarget = m_prendertarget.Get();


         m_iType = 3;

         return true;         

      }

      D2D1_SIZE_U sizeu = D2D1::SizeU(1, 1);
      D2D1_PIXEL_FORMAT pixelformat;

      pixelformat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
      pixelformat.format = DXGI_FORMAT_B8G8R8A8_UNORM;

      hr = prendertarget->CreateCompatibleRenderTarget(NULL, &sizeu, &pixelformat, D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE, &m_pbitmaprendertarget);

      if(FAILED(hr))
      {

         trace_hr("graphics::CreateCompatibleDC, CreateCompatibleRenderTarget (1) ",hr);

      }

      if(m_pbitmaprendertarget == NULL)
      {
         return false;
      }

      hr = m_pbitmaprendertarget.As(&m_prendertarget);

      if(FAILED(hr))
      {
         m_pbitmaprendertarget = nullptr;
         return false;
      }

      hr = m_pbitmaprendertarget.As(&m_pdevicecontext);

      if(FAILED(hr))
      {
         m_prendertarget = nullptr;
         m_pbitmaprendertarget = nullptr;
         return false;
      }


      if(m_spbitmap.is_null())
         m_spbitmap.alloc(allocer());


      ID2D1Bitmap * pbitmap;

      hr = m_pbitmaprendertarget->GetBitmap(&pbitmap);

      if(FAILED(hr))
      {
         m_pbitmaprendertarget = nullptr;
         return false;
      }

      m_spbitmap->attach(pbitmap);

      m_iType = 3;

      return true;


   }


   int graphics::ExcludeUpdateRgn(::user::primitive * pwindow)
   { 
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL); 

      //return ::ExcludeUpdateRgn(get_handle1(), WIN_WINDOW(pwindow)->get_handle()); 

   }


   int graphics::GetDeviceCaps(int nIndex) const
   { 
      throw todo(get_app());
      //ASSERT(get_handle2() != NULL); 
      //return ::GetDeviceCaps(get_handle2(), nIndex); 
   }

   point graphics::GetBrushOrg() const
   {
      throw todo(get_app());
      //ASSERT(get_handle1() != NULL);
      //POINT point;
      //VERIFY(::GetBrushOrgEx(get_handle1(), &point));
      //return point;
   }

   point graphics::SetBrushOrg(int x, int y)
   {
      throw todo(get_app());
      //ASSERT(get_handle1() != NULL);
      //POINT point;
      //VERIFY(::SetBrushOrgEx(get_handle1(), x, y, &point));
      //return point;
   }

   point graphics::SetBrushOrg(POINT point)
   {
      throw todo(get_app());
      //ASSERT(get_handle1() != NULL);
      //VERIFY(::SetBrushOrgEx(get_handle1(), point.x, point.y, &point));
      //return point;
   }

   int graphics::EnumObjects(int nObjectType, int (CALLBACK* lpfn)(LPVOID, LPARAM), LPARAM lpData)
   {
      throw todo(get_app());
      //ASSERT(get_handle2() != NULL);
      //return ::EnumObjects(get_handle2(), nObjectType, (GOBJENUMPROC)lpfn, lpData); 
   }

   ::draw2d::bitmap* graphics::SelectObject(::draw2d::bitmap* pBitmap)
   { 


      if(pBitmap == NULL)
         return NULL;

//      if(m_pdevicecontext == NULL)
  //       return NULL;

      /*      if(get_handle1() == NULL)
      return NULL;
      if(pBitmap == NULL)
      return NULL;
      return dynamic_cast < ::draw2d::bitmap* > (SelectGdiObject(get_app(), get_handle1(), pBitmap->get_os_data()));*/
      if(m_prendertarget == NULL)
      {
         CreateCompatibleDC(NULL);
      }

      m_pdevicecontext->SetTarget((ID2D1Bitmap *)pBitmap->get_os_data());

      m_spbitmap = pBitmap;

      

      m_iType = 3;


      //set_text_rendering(::draw2d::text_rendering_anti_alias_grid_fit);

      //m_spbitmap = pBitmap;

      return m_spbitmap;
   }


   ::draw2d::object* graphics::SelectObject(::draw2d::object* pObject)
   {
      /*      ASSERT(get_handle1() != NULL); 
      if(pObject == NULL)
      return NULL;
      return SelectGdiObject(get_app(), get_handle1(), pObject->get_os_data()); */
      return NULL;
   }

/*   HGDIOBJ graphics::SelectObject(HGDIOBJ hObject) // Safe for NULL handles
   {
      return hObject;
   }*/

   COLORREF graphics::GetNearestColor(COLORREF crColor) const
   { 
      throw todo(get_app());
      //return ::GetNearestColor(get_handle2(), crColor); 
   }

   UINT graphics::RealizePalette()
   { 
      throw todo(get_app());
      //return ::RealizePalette(get_handle1()); 
   }

   void graphics::UpdateColors()
   { 
      throw todo(get_app());
      //::UpdateColors(get_handle1()); 
   }


   int graphics::GetPolyFillMode() const
   { 
      throw todo(get_app());
      //return ::GetPolyFillMode(get_handle2()); 
   }

   int graphics::GetROP2() const
   { 
      throw todo(get_app());
      //return ::GetROP2(get_handle2()); 
   }

   int graphics::GetStretchBltMode() const
   { 
      throw todo(get_app());
      //return ::GetStretchBltMode(get_handle2()); 
   }

   int graphics::GetMapMode() const
   { 
      throw todo(get_app());
      //return ::GetMapMode(get_handle2()); 
   }

   int graphics::GetGraphicsMode() const
   { 
      throw todo(get_app());
      //return ::GetGraphicsMode(get_handle2()); 
   }

   bool graphics::GetWorldTransform(XFORM* pXform) const
   { 
      throw todo(get_app());

      //return ::GetWorldTransform(get_handle2(),pXform) != FALSE; 

   }

   point graphics::GetViewportOrg() const
   {
      //POINT point;
      //::GetViewportOrgEx(get_handle2(), &point);

      D2D1::Matrix3x2F m;

      m_prendertarget->GetTransform(&m);

      D2D1_POINT_2F origin = m.TransformPoint(D2D1::Point2F(0.0f, 0.0f));

      return point((int64_t) origin.x, (int64_t) origin.y);

   }

   size graphics::GetViewportExt() const
   {
      throw todo(get_app());
      //SIZE size;
      //::GetViewportExtEx(get_handle2(), &size);
      //return size;
   }

   point graphics::GetWindowOrg() const
   {
      throw todo(get_app());
      //POINT point;
      //::GetWindowOrgEx(get_handle2(), &point);
      //return point;
   }

   size graphics::GetWindowExt() const
   {
      throw todo(get_app());
      //SIZE size;
      //::GetWindowExtEx(get_handle2(), &size);
      //return size;
   }

   // non-virtual helpers calling virtual mapping functions
   point graphics::SetViewportOrg(POINT point)
   { 
      return SetViewportOrg(point.x, point.y); 
   }

   size graphics::SetViewportExt(SIZE size)
   {
      return SetViewportExt(size.cx, size.cy); 
   }

   point graphics::SetWindowOrg(POINT point)
   {
      return SetWindowOrg(point.x, point.y); 
   }

   size graphics::SetWindowExt(SIZE size)
   {
      return SetWindowExt(size.cx, size.cy); 
   }


   void graphics::DPtoLP(LPPOINT lpPoints, count nCount) const
   { 

      throw todo(get_app());
      //::DPtoLP(get_handle2(), lpPoints, nCount); 

   }


   void graphics::DPtoLP(LPRECT lpRect) const
   { 

      throw todo(get_app());
      //::DPtoLP(get_handle2(), (LPPOINT)lpRect, 2); 

   }


   void graphics::LPtoDP(LPPOINT lpPoints, count nCount) const
   {

      throw todo(get_app());
      //::LPtoDP(get_handle2(), lpPoints, nCount); 

   }


   void graphics::LPtoDP(LPRECT lpRect) const
   {

      throw todo(get_app());
      //::LPtoDP(get_handle2(), (LPPOINT)lpRect, 2);

   }


   bool graphics::FillRgn(::draw2d::region* pRgn, ::draw2d::brush* pBrush)
   { 

      throw todo(get_app());
      //return ::FillRgn(get_handle1(), (HRGN)pRgn->get_os_data(), (HBRUSH)pBrush->get_os_data()) != FALSE;

   }


   bool graphics::FrameRgn(::draw2d::region* pRgn, ::draw2d::brush* pBrush, int nWidth, int nHeight)
   { 
   
      throw todo(get_app());
      //return ::FrameRgn(get_handle1(), (HRGN)pRgn->get_os_data(), (HBRUSH)pBrush->get_os_data(), nWidth, nHeight) != FALSE; 

   }


   bool graphics::InvertRgn(::draw2d::region* pRgn)
   {
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::InvertRgn(get_handle1(), (HRGN)pRgn->get_os_data()) != FALSE;

   }

   bool graphics::PaintRgn(::draw2d::region* pRgn)
   {

      throw todo(get_app());
      //ASSERT(get_handle1() != NULL);

      //return ::PaintRgn(get_handle1(), (HRGN)pRgn->get_os_data())  != FALSE;

   }

   bool graphics::PtVisible(int x, int y) const
   {

      throw todo(get_app());
      //ASSERT(get_handle1() != NULL); 

      //return ::PtVisible(get_handle1(), x, y) != FALSE;

   }

   bool graphics::PtVisible(POINT point) const
   { 
   
      ASSERT(get_handle1() != NULL);

      return PtVisible(point.x, point.y); 

   } // call virtual


   bool graphics::RectVisible(const RECT & lpcrect) const
   {

      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::RectVisible(get_handle1(), lpRect) != FALSE;

   }

   point graphics::GetCurrentPosition() const
   {
      throw todo(get_app());
      //ASSERT(get_handle2() != NULL);
      //POINT point;
      //VERIFY(::GetCurrentPositionEx(get_handle2(), &point));
      //return point;
   }

   bool graphics::Arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
   { 

      ::draw2d::path_sp path(allocer());

      double pi = 3.1415927f;

      rect rect(x1, y1, x2, y2);

      double centerx    = (x2 + x1) / 2.0;
      double centery    = (y2 + y1) / 2.0;

      double start      = atan2(y3 - centery, x3 - centerx) * 180.0 / pi;
      double end        = atan2(y4 - centery, x4 - centerx) * 180.0 / pi;

      path->begin_figure(false, ::draw2d::fill_mode_winding);
      path->add_arc(rect, (int) start, (int) fmod(end + 360.0 - start, 360.0));
      path->end_figure(false);

      return this->path(path);

   }

   bool graphics::Arc(double x1, double y1, double w, double h, double start, double extends)
   {

      ::draw2d::path_sp path(allocer());

      rect rect((int64_t) x1, (int64_t)y1, (int64_t)(x1 + w), (int64_t)(y1 + h));

      path->begin_figure(false, ::draw2d::fill_mode_winding);
      path->add_arc(rect, start, extends);
      path->end_figure(false);

      return this->path(path);

   }


   bool graphics::Arc(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
   {

      ::draw2d::path_sp path(allocer());

      double pi = 3.1415927f;

      rect rect((int64_t)x1, (int64_t)y1, (int64_t)x2, (int64_t)y2);

      double centerx = (x2 + x1) / 2.0;
      double centery = (y2 + y1) / 2.0;

      double start = atan2(y3 - centery, x3 - centerx) * 180.0 / pi;
      double end = atan2(y4 - centery, x4 - centerx) * 180.0 / pi;

      path->begin_figure(false, ::draw2d::fill_mode_winding);
      path->add_arc(rect, start, end - start);
      path->end_figure(false);

      return this->path(path);

   }

   bool graphics::Arc(const RECT & lpRect, POINT ptStart, POINT ptEnd)
   {

      ASSERT(get_handle1() != NULL);

      return Arc(lpRect.left, lpRect.top, lpRect.right, lpRect.bottom, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y)  != FALSE; 

   }

   bool graphics::Polyline(const POINT* lpPoints, count nCount)
   {

      //throw todo(get_app());
      //ASSERT(get_handle1() != NULL);

      //return ::Polyline(get_handle1(), lpPoints, nCount) != FALSE;
      return true;

   }

   void graphics::FillRect(const RECT & lpRect, ::draw2d::brush* pbrush)
   { 
      //g.SetCompositingMode(Gdiplus::CompositingModeSourceOver);
      //g().SetCompositingMode(Gdiplus::CompositingModeSourceOver);
      //g().SetCompositingQuality(Gdiplus::CompositingQualityGammaCorrected);

      if (m_prendertarget == NULL)
         return;

      D2D1_RECT_F r;

      r.left = (FLOAT)lpRect.left;
      r.top = (FLOAT)lpRect.top;
      r.right = (FLOAT)lpRect.right;
      r.bottom = (FLOAT)lpRect.bottom;

      m_prendertarget->FillRectangle(&r, (dynamic_cast < ::draw2d_direct2d::brush * > (pbrush))->get_os_brush((graphics *) this));

   }


   bool graphics::DrawRect(const RECT & lpRect, ::draw2d::pen* ppen)
   {
      //g.SetCompositingMode(Gdiplus::CompositingModeSourceOver);
      //g().SetCompositingMode(Gdiplus::CompositingModeSourceOver);
      //g().SetCompositingQuality(Gdiplus::CompositingQualityGammaCorrected);

      if (m_prendertarget == NULL)
         return false;

      SelectObject(m_spbrush);

      D2D1_RECT_F r;

      r.left = (FLOAT)lpRect.left;
      r.top = (FLOAT)lpRect.top;
      r.right = (FLOAT)lpRect.right;
      r.bottom = (FLOAT)lpRect.bottom;

      m_prendertarget->DrawRectangle(&r, (dynamic_cast < ::draw2d_direct2d::pen * > (ppen))->get_os_pen_brush((graphics *) this), (FLOAT)ppen->m_dWidth);

      return true;

   }


   void graphics::FrameRect(const RECT & lpRect, ::draw2d::brush* pBrush)
   { 
      throw todo(get_app());
      //ASSERT(get_handle1() != NULL); 
      //      ::FrameRect(get_handle1(), lpRect, (HBRUSH)pBrush->get_os_data()); 

   }
   void graphics::InvertRect(const RECT & lpRect)
   { 
      throw todo(get_app());
      //ASSERT(get_handle1() != NULL); ::InvertRect(get_handle1(), lpRect); 
   }

   bool graphics::DrawIcon(int x, int y, ::visual::icon * picon)
   {
      throw todo(get_app());
      //ASSERT(get_handle1() != NULL); 

      //if(picon == NULL)
      //   return false;

      //return ::DrawIcon(get_handle1(), x, y, picon->m_hicon) != FALSE; 

   }

   bool graphics::DrawIcon(POINT point, ::visual::icon * picon)
   { 
      throw todo(get_app());
      //ASSERT(get_handle1() != NULL); 

      //if(picon == NULL)
      //   return false;

      //return ::DrawIcon(get_handle1(), point.x, point.y, picon->m_hicon) != FALSE; 

   }

   bool graphics::DrawIcon(int x, int y, ::visual::icon * picon, int cx, int cy, UINT istepIfAniCur, HBRUSH hbrFlickerFreeDraw, UINT diFlags)
   { 

#ifdef METROWIN

      //::exception::throw_not_implemented(get_app());
      return false;

#else

      try
      {
      
         if(picon == NULL)
            return FALSE;

         if(m_prendertarget == NULL)
            return FALSE;

         bool bOk = FALSE;

         BITMAPINFO info;
         COLORREF * pcolorref;

         ZeroMemory(&info, sizeof (BITMAPINFO));

         info.bmiHeader.biSize          = sizeof (BITMAPINFOHEADER);
         info.bmiHeader.biWidth         = cx;
         info.bmiHeader.biHeight        = - cy;
         info.bmiHeader.biPlanes        = 1;
         info.bmiHeader.biBitCount      = 32; 
         info.bmiHeader.biCompression   = BI_RGB;
         info.bmiHeader.biSizeImage     = cx * cy * 4;

         HBITMAP hbitmap = ::CreateDIBSection(NULL, &info, DIB_RGB_COLORS, (void **) &pcolorref, NULL, 0);

         HDC hdc = ::CreateCompatibleDC(NULL);

         HBITMAP hbitmapOld = (HBITMAP) ::SelectObject(hdc, hbitmap);

         if(::DrawIconEx(hdc, 0, 0, (HICON) picon->m_picon, cx, cy, istepIfAniCur, NULL, DI_IMAGE | DI_MASK))
         {

            ::SelectObject(hdc, hbitmapOld);

            try
            {

               //Gdiplus::Bitmap b(cx, cy, cx * 4 , PixelFormat32bppARGB, (BYTE *) pcolorref);

               ::draw2d::bitmap_sp b(allocer());

               b->CreateBitmap(this, cx, cy, 1, 32, pcolorref, cx * sizeof(COLORREF));

               D2D1_RECT_F r;

               r.left   = (FLOAT) x;
               r.top    = (FLOAT) y;
               r.right  = r.left    + cx;
               r.bottom = r.top     + cy;

               m_prendertarget->DrawBitmap((ID2D1Bitmap *) b->get_os_data(), &r);

            }
            catch(...)
            {
            }

         }

         ::DeleteDC(hdc);

         ::DeleteObject(hbitmap);

         return bOk;

      }
      catch(...)
      {
      }

      return false;

#endif

   }

   bool graphics::DrawState(point pt, size size, HBITMAP hBitmap, UINT nFlags, HBRUSH hBrush)
   { 

      throw todo(get_app());
      //ASSERT(get_handle1() != NULL); 
      //return ::DrawState(get_handle1(), hBrush, NULL, (LPARAM)hBitmap, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_BITMAP) != FALSE;

   }

   bool graphics::DrawState(point pt, size size, ::draw2d::bitmap* pBitmap, UINT nFlags, ::draw2d::brush* pBrush)
   {

      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);
      //return ::DrawState(get_handle1(), (HBRUSH)pBrush->get_os_data(), NULL, (LPARAM)pBitmap->get_os_data(), 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_BITMAP) != FALSE;

   }

   bool graphics::DrawState(point pt, size size, HICON hIcon, UINT nFlags, HBRUSH hBrush)
   {


      throw todo(get_app());

      //ASSERT(get_handle1() != NULL); 
      //return ::DrawState(get_handle1(), hBrush, NULL, (LPARAM)hIcon, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_ICON) != FALSE; 

   }


   bool graphics::DrawState(point pt, size size, HICON hIcon, UINT nFlags, ::draw2d::brush* pBrush)
   {


      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);
      //return ::DrawState(get_handle1(), (HBRUSH)pBrush->get_os_data(), NULL, (LPARAM)hIcon, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_ICON) != FALSE;

   }

   bool graphics::DrawState(point pt, size size, const char * lpszText, UINT nFlags, bool bPrefixText, int nTextLen, HBRUSH hBrush)
   {

      throw todo(get_app());

      //ASSERT(get_handle1() != NULL); 
      //return ::DrawState(get_handle1(), hBrush,  NULL, (LPARAM)lpszText, (WPARAM)nTextLen, pt.x, pt.y, size.cx, size.cy, nFlags|(bPrefixText ? DST_PREFIXTEXT : DST_TEXT)) != FALSE;

   }

   bool graphics::DrawState(point pt, size size, const char * lpszText, UINT nFlags, bool bPrefixText, int nTextLen, ::draw2d::brush* pBrush)
   {

      throw todo(get_app());

      //ASSERT(get_handle1() != NULL); 
      //return ::DrawState(get_handle1(), (HBRUSH)pBrush->get_os_data(), NULL, (LPARAM)lpszText, (WPARAM)nTextLen, pt.x, pt.y, size.cx, size.cy, nFlags|(bPrefixText ? DST_PREFIXTEXT : DST_TEXT)) != FALSE;

   }

#ifdef WINDOWSEX

   /*
   bool graphics::DrawState(point pt, size size, DRAWSTATEPROC lpDrawProc, LPARAM lData, UINT nFlags, HBRUSH hBrush)
   {

      throw todo(get_app());

      //ASSERT(get_handle1() != NULL); return ::DrawState(get_handle1(), hBrush, lpDrawProc, lData, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_COMPLEX) != FALSE; 

   }

   bool graphics::DrawState(point pt, size size, DRAWSTATEPROC lpDrawProc, LPARAM lData, UINT nFlags, ::draw2d::brush* pBrush)
   {

      throw todo(get_app());

      //ASSERT(get_handle1() != NULL); return ::DrawState(get_handle1(), (HBRUSH)pBrush->get_os_data(), lpDrawProc, lData, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_COMPLEX) != FALSE; 

   }
   */

#endif

   bool graphics::DrawEdge(LPRECT lpRect, UINT nEdge, UINT nFlags)
   {

      throw todo(get_app());

      ASSERT(get_handle1() != NULL); 

      //return ::DrawEdge(get_handle1(), lpRect, nEdge, nFlags) != FALSE; 

   }


   bool graphics::DrawFrameControl(LPRECT lpRect, UINT nType, UINT nState)
   { 

      throw todo(get_app());

      //ASSERT(get_handle1() != NULL); return ::DrawFrameControl(get_handle1(), lpRect, nType, nState) != FALSE; 

   }

   bool graphics::Chord(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
   {

      throw todo(get_app());

      ASSERT(get_handle1() != NULL); 

      //return ::Chord(get_handle1(), x1, y1, x2, y2, x3, y3, x4, y4) != FALSE;

   }

   bool graphics::Chord(const RECT & lpRect, POINT ptStart, POINT ptEnd)
   {

      throw todo(get_app());

      ASSERT(get_handle1() != NULL);

      //return ::Chord(get_handle1(), lpRect.left, lpRect.top, lpRect.right, lpRect.bottom, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y) != FALSE; 

   }


   void graphics::DrawFocusRect(const RECT & lpRect)
   {

      throw todo(get_app());

      ASSERT(get_handle1() != NULL); 

      //::DrawFocusRect(get_handle1(), lpRect);

   }

   bool graphics::DrawEllipse(int x1, int y1, int x2, int y2)
   {

      if(m_sppen.is_null())
      {
         
         ((graphics *) this)->m_sppen.alloc(((graphics *) this)->allocer());
         ((graphics *) this)->m_sppen->m_powner = (void *) (graphics *) this;

      }

      if(m_sppen.is_null())
         return false;

      D2D1_ELLIPSE ellipse;

      ellipse.point.x = ((FLOAT) x1 + (FLOAT) x2) / ((FLOAT) 2.0);
      ellipse.point.y = ((FLOAT) y1 + (FLOAT) y2) / ((FLOAT) 2.0);
      ellipse.radiusX = ((FLOAT) (x2 - x1)) / ((FLOAT) 2.0);
      ellipse.radiusY = ((FLOAT) (y2 - y1)) / ((FLOAT) 2.0);

      m_pdevicecontext->DrawEllipse(&ellipse, (dynamic_cast < ::draw2d_direct2d::pen * > (m_sppen.m_p))->get_os_pen_brush(this), (FLOAT) m_sppen->m_dWidth);


      return true;

   }

   bool graphics::DrawEllipse(const RECT & lpRect)
   { 

      return DrawEllipse(lpRect.left, lpRect.top, lpRect.right, lpRect.bottom);

   }

   bool graphics::FillEllipse(int x1, int y1, int x2, int y2)
   {

      if(m_spbrush.is_null())
      {
         
         ((graphics *) this)->m_spbrush.alloc(((graphics *) this)->allocer());
         ((graphics *) this)->m_spbrush->m_powner = (void *) (graphics *) this;

      }

      if(m_spbrush.is_null())
         return false;

      D2D1_ELLIPSE ellipse;

      ellipse.point.x = ((FLOAT) x1 + (FLOAT) x2) / ((FLOAT) 2.0);
      ellipse.point.y = ((FLOAT) y1 + (FLOAT) y2) / ((FLOAT) 2.0);
      ellipse.radiusX = ((FLOAT) (x2 - x1)) / ((FLOAT) 2.0);
      ellipse.radiusY = ((FLOAT) (y2 - y1)) / ((FLOAT) 2.0);

      m_pdevicecontext->FillEllipse(&ellipse, (dynamic_cast < ::draw2d_direct2d::brush * > (m_spbrush.m_p))->get_os_brush((graphics *) this));

      return true;

   }

   bool graphics::FillEllipse(const RECT & lprect)
   { 

      return FillEllipse(lprect.left,lprect.top,lprect.right,lprect.bottom);

   }


   bool graphics::FillEllipse(const RECTD & r)
   {

      if(m_spbrush.is_null())
      {

         ((graphics *) this)->m_spbrush.alloc(((graphics *) this)->allocer());
         ((graphics *) this)->m_spbrush->m_powner = (void *)(graphics *) this;

      }

      if(m_spbrush.is_null())
         return false;

      D2D1_ELLIPSE ellipse;

      ellipse.point.x = (float)(r.right + r.left) / 2.f;
      ellipse.point.y = (float)(r.bottom + r.top) / 2.f;
      ellipse.radiusX = (float)(r.right - r.left) / 2.f;
      ellipse.radiusY = (float)(r.bottom - r.top) / 2.f;


      m_pdevicecontext->FillEllipse(&ellipse,(dynamic_cast < ::draw2d_direct2d::brush * > (m_spbrush.m_p))->get_os_brush((graphics *) this));


      return true;

   }

   bool graphics::DrawEllipse(const RECTD & r)
   {

      if(m_spbrush.is_null())
      {

         ((graphics *) this)->m_spbrush.alloc(((graphics *) this)->allocer());
         ((graphics *) this)->m_spbrush->m_powner = (void *)(graphics *) this;

      }

      if(m_spbrush.is_null())
         return false;

      D2D1_ELLIPSE ellipse;

      ellipse.point.x = (float)(r.right + r.left) / 2.f;
      ellipse.point.y = (float)(r.bottom + r.top) / 2.f;
      ellipse.radiusX = (float)(r.right - r.left) / 2.f;
      ellipse.radiusY = (float)(r.bottom - r.top) / 2.f;


      m_pdevicecontext->DrawEllipse(&ellipse,(dynamic_cast < ::draw2d_direct2d::brush * > (m_spbrush.m_p))->get_os_brush((graphics *) this));


      return true;

   }

   bool graphics::Pie(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
   {


      throw todo(get_app());

      //ASSERT(get_handle1() != NULL); 
      //
      //return ::Pie(get_handle1(), x1, y1, x2, y2, x3, y3, x4, y4) != FALSE;

   }


   bool graphics::Pie(const RECT & lpRect, POINT ptStart, POINT ptEnd)
   {


      throw todo(get_app());

      //ASSERT(get_handle1() != NULL); 
      //
      //return ::Pie(get_handle1(), lpRect.left, lpRect.top, lpRect.right, lpRect.bottom, ptStart.x, ptStart.y,  ptEnd.x, ptEnd.y) != FALSE;

   }

   bool graphics::Polygon(const POINT* lpPoints, count nCount)
   {

      ::draw2d::path_sp path(allocer());

      path->begin_figure(get_os_brush(m_spbrush) != NULL, ::draw2d::fill_mode_winding);

      path->add_lines(lpPoints, nCount);

      path->end_figure(true);

      return this->path(path);


   }


   bool graphics::fill_polygon(const POINTD * lpPoints,count nCount)
   {
      
      ::draw2d::path_sp path(allocer());

      path->begin_figure(get_os_brush(m_spbrush) != NULL,::draw2d::fill_mode_winding);

      path->add_lines(lpPoints,nCount);

      path->end_figure(true);

      return this->fill_path(path);

   }


   bool graphics::Polygon(const POINTD* lpPoints, count nCount)
   {

      ::draw2d::path_sp path(allocer());

      path->begin_figure(get_os_brush(m_spbrush) != NULL,::draw2d::fill_mode_winding);

      path->add_lines(lpPoints,nCount);

      path->end_figure(true);

      return this->path(path);


   }


   bool graphics::PolyPolygon(const POINT* lpPoints,const INT* lpPolyCounts,count nCount)
   {

      throw todo(get_app());

      //ASSERT(get_handle1() != NULL); 
      //
      //return ::PolyPolygon(get_handle1(), lpPoints, lpPolyCounts, nCount) != FALSE;

   }

   bool graphics::Rectangle(int x1, int y1, int x2, int y2)
   { 

      bool bOk1 = true;

      //if(m_spbrush->get_os_data() != NULL)
      {

         bOk1 = FillRectangle(x1, y1, x2, y2);

      }

      bool bOk2 = true;

      //if(m_sppen->get_os_data() != NULL)
      {

         bOk2 = DrawRectangle(x1, y1, x2, y2);

      }

      return bOk1 && bOk2;

   }

   bool graphics::Rectangle(const RECT & lpRect)
   { 

      return Rectangle(lpRect.left, lpRect.top, lpRect.right, lpRect.bottom);

   }

   bool graphics::DrawRectangle(int x1, int y1, int x2, int y2)
   { 

      if (m_sppen->m_etype == ::draw2d::pen::type_null)
         return true;

      D2D1_RECT_F r;

      r.left      = (FLOAT) x1;
      r.top       = (FLOAT) y1;
      r.right     = (FLOAT) x2;
      r.bottom    = (FLOAT) y2;

      m_prendertarget->DrawRectangle(&r, (dynamic_cast < ::draw2d_direct2d::pen * > (m_sppen.m_p))->get_os_pen_brush((graphics *) this), (FLOAT)m_sppen->m_dWidth);

      return true;

   }

   bool graphics::DrawRectangle(const RECT & lpRect)
   { 

      return DrawRectangle(lpRect.left, lpRect.top, lpRect.right, lpRect.bottom);

   }

   bool graphics::FillRectangle(int x1, int y1, int x2, int y2)
   { 

      if(get_os_brush(m_spbrush) == NULL)
         return true;

      D2D1_RECT_F r;

      r.left      = (FLOAT) x1;
      r.top       = (FLOAT) y1;
      r.right     = (FLOAT) x2;
      r.bottom    = (FLOAT) y2;

      m_prendertarget->FillRectangle(r, (ID2D1Brush *) get_os_brush(m_spbrush));

      return true;

   }

   bool graphics::FillRectangle(const RECT & lpRect)
   { 

      return FillRectangle(lpRect.left, lpRect.top, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top);

   }

   bool graphics::RoundRect(int x1, int y1, int x2, int y2, int x3, int y3)
   {

      throw todo(get_app());

      //ASSERT(get_handle1() != NULL); 

      //return ::RoundRect(get_handle1(), x1, y1, x2, y2, x3, y3) != FALSE; 

   }

   bool graphics::RoundRect(const RECT & lpRect, POINT point)
   {

      throw todo(get_app());

      //ASSERT(get_handle1() != NULL); 
      //
      //return ::RoundRect(get_handle1(), lpRect.left, lpRect.top, lpRect.right, lpRect.bottom, point.x, point.y) != FALSE; 

   }


   bool graphics::PatBlt(int x, int y, int nWidth, int nHeight, uint32_t dwRop)
   {

      throw todo(get_app());

      //ASSERT(get_handle1() != NULL); 
      //
      //return ::PatBlt(get_handle1(), x, y, nWidth, nHeight, dwRop) != FALSE; 

   }


   bool graphics::BitBltRaw(int x, int y, int nWidth, int nHeight, ::draw2d::graphics * pgraphicsSrc, int xSrc, int ySrc, uint32_t dwRop)
   { 

      synch_lock sl(&draw2d_mutex());

      try
      {

         if(pgraphicsSrc == NULL)
            return FALSE;

         if(pgraphicsSrc->get_current_bitmap() == NULL)
            return FALSE;

         if(pgraphicsSrc->get_current_bitmap()->get_os_data() == NULL)
            return FALSE;

         if (get_current_bitmap() != NULL && get_current_bitmap()->get_os_data() != NULL)
         {

            D2D1_SIZE_U sz = ((ID2D1Bitmap *)get_current_bitmap()->get_os_data())->GetPixelSize();

            if (natural(nWidth + x) > sz.width)
               nWidth = sz.width - x;

            if (natural(nHeight + y) > sz.height)
               nHeight = sz.height - y;

         }

         {

            D2D1_SIZE_U sz = ((ID2D1Bitmap *)pgraphicsSrc->get_current_bitmap()->get_os_data())->GetPixelSize();

            if (natural(nWidth + xSrc) > sz.width)
               nWidth = sz.width - xSrc;

            if (natural(nHeight + ySrc) > sz.height)
               nHeight = sz.height - ySrc;

         }

         D2D1_RECT_F rectDst = D2D1::RectF((float) x, (float) y, (float) (x + nWidth), (float) (y + nHeight));
         D2D1_RECT_F rectSrc = D2D1::RectF((float) xSrc, (float) ySrc, (float) (xSrc + nWidth), (float) (ySrc + nHeight));

         dynamic_cast <::draw2d_direct2d::graphics *> (pgraphicsSrc)->SaveClip();

         HRESULT hr = ((ID2D1DeviceContext *) pgraphicsSrc->get_os_data())->EndDraw();

         if(m_prendertarget != NULL)
         {
            m_prendertarget->DrawBitmap((ID2D1Bitmap *)pgraphicsSrc->get_current_bitmap()->get_os_data(),&rectDst,1.0,m_bitmapinterpolationmode,&rectSrc);
         }
         else
         {
            m_pdevicecontext->DrawBitmap((ID2D1Bitmap *) pgraphicsSrc->get_current_bitmap()->get_os_data(), rectDst, 1.0, m_interpolationmode, rectSrc);
         }

         //hr = m_prendertarget->Flush();
         flush();

         if(SUCCEEDED(hr))
         {
          
            ((ID2D1DeviceContext *)pgraphicsSrc->get_os_data())->BeginDraw();

            dynamic_cast <::draw2d_direct2d::graphics *> (pgraphicsSrc)->RestoreClip();

         }

         return true;

      }
      catch(...)
      {
         return FALSE;
      }

      //return ::BitBlt(get_handle1(), x, y, nWidth, nHeight, WIN_HDC(pgraphicsSrc), xSrc, ySrc, dwRop); 

   }


   bool graphics::StretchBltRaw(int xDst, int yDst, int nDstWidth, int nDstHeight, ::draw2d::graphics * pgraphicsSrc, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, uint32_t dwRop)
   { 

      try
      {

         if(pgraphicsSrc == NULL)
            return FALSE;

         if(pgraphicsSrc->get_current_bitmap() == NULL)
            return FALSE;

         if(pgraphicsSrc->get_current_bitmap()->get_os_data() == NULL)
            return FALSE;

         D2D1_RECT_F rectDst = D2D1::RectF((float) xDst, (float) yDst, (float) (xDst + nDstWidth), (float) (yDst + nDstHeight));
         D2D1_RECT_F rectSrc = D2D1::RectF((float) xSrc, (float) ySrc, (float) (xSrc + nSrcWidth), (float) (ySrc + nSrcHeight));

         dynamic_cast <::draw2d_direct2d::graphics *> (pgraphicsSrc)->SaveClip();

         HRESULT hr = ((ID2D1DeviceContext *) pgraphicsSrc->get_os_data())->EndDraw();

         if(m_prendertarget != NULL)
         {
            m_prendertarget->DrawBitmap((ID2D1Bitmap *)pgraphicsSrc->get_current_bitmap()->get_os_data(),&rectDst,1.0,m_bitmapinterpolationmode,& rectSrc);
         }
         else
         {
            m_pdevicecontext->DrawBitmap((ID2D1Bitmap *)pgraphicsSrc->get_current_bitmap()->get_os_data(),rectDst,1.0,m_interpolationmode,rectSrc);
         }

         flush();

         if(SUCCEEDED(hr))
         {

            ((ID2D1DeviceContext *)pgraphicsSrc->get_os_data())->BeginDraw();

            dynamic_cast <::draw2d_direct2d::graphics *> (pgraphicsSrc)->RestoreClip();

         }

         //hr = m_prendertarget->Flush();

         //((ID2D1DeviceContext *) pgraphicsSrc->get_os_data())->BeginDraw();

         return true;

      }
      catch(...)
      {
         return FALSE;
      }


      //return ::StretchBlt(get_handle1(), x, y, nWidth, nHeight, WIN_HDC(pgraphicsSrc), xSrc, ySrc, nSrcWidth, nSrcHeight, dwRop); 

   }


   COLORREF graphics::GetPixel(int x, int y) const
   { 

      throw todo(get_app());

      //ASSERT(get_handle1() != NULL); 
      //return ::GetPixel(get_handle1(), x, y); 

   }


   COLORREF graphics::GetPixel(POINT point) const
   {

      throw todo(get_app());

      //ASSERT(get_handle1() != NULL); 
      //return ::GetPixel(get_handle1(), point.x, point.y); 

   }


   COLORREF graphics::SetPixel(int x, int y, COLORREF crColor)
   {

      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);
      //return ::SetPixel(get_handle1(), x, y, crColor); 

   }


   COLORREF graphics::SetPixel(POINT point, COLORREF crColor)
   {

      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);
      //return ::SetPixel(get_handle1(), point.x, point.y, crColor); 

   }


   bool graphics::FloodFill(int x, int y, COLORREF crColor)
   { 

      throw todo(get_app());

      //ASSERT(get_handle1() != NULL); 
      //return ::FloodFill(get_handle1(), x, y, crColor) != FALSE; 

   }


   bool graphics::ExtFloodFill(int x, int y, COLORREF crColor, UINT nFillType)
   { 

      throw todo(get_app());

      //ASSERT(get_handle1() != NULL); 
      //return ::ExtFloodFill(get_handle1(), x, y, crColor, nFillType) != FALSE; 

   }


   // true blend
   // COLOR_DEST = SRC_ALPHA * COLOR_SRC  + (1 - SRC_ALPHA) * COLOR_DST

   // double blend
   //// COLOR_DEST = SRC_ALPHA * BLEND_ALPHA * COLOR_SRC  + (1 - SRC_ALPHA * BLEND_ALPHA) * COLOR_DST

   
   bool graphics::text_out(int x, int y, const string & str)
   { 

      return text_out((double)x, (double)y, str);
      
   }


   bool graphics::text_out(double x, double y, const string & str)
   { 

      return text_out((int) x, (int) y, str, (int) str.get_length()); 

   }


   bool graphics::ExtTextOut(int x, int y, UINT nOptions, const RECT & lpRect, const char * lpszString, strsize nCount, LPINT lpDxWidths)
   { 

      throw todo(get_app());

      //ASSERT(get_handle1() != NULL); 
      //return ::ExtTextOut(get_handle1(), x, y, nOptions, lpRect, lpszString, nCount, lpDxWidths) != FALSE; 
   }

   bool graphics::ExtTextOut(int x, int y, UINT nOptions, const RECT & lpRect, const string & str, LPINT lpDxWidths)
   {

      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      ////return ::ExtTextOut(get_handle1(), x, y, nOptions, lpRect, str, (UINT)str.get_length(), lpDxWidths) != FALSE;

   }


   size graphics::TabbedTextOut(int x, int y, const char * lpszString, strsize nCount, count nTabPositions, LPINT lpnTabStopPositions, int32_t nTabOrigin)
   {

      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::TabbedTextOut(get_handle1(), x, y, lpszString, nCount, nTabPositions, lpnTabStopPositions, nTabOrigin);

   }


   size graphics::TabbedTextOut(int x, int y, const string & str, count nTabPositions, LPINT lpnTabStopPositions, int32_t nTabOrigin)
   {

      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::TabbedTextOut(get_handle1(), x, y, str, (int)str.get_length(), nTabPositions, lpnTabStopPositions, nTabOrigin);

   }


   size graphics::GetTabbedTextExtent(const char * lpszString, strsize nCount, count nTabPositions, LPINT lpnTabStopPositions) const
   { 

      throw todo(get_app());

      //ASSERT(get_handle2() != NULL); 

      //return ::GetTabbedTextExtent(get_handle2(), lpszString, (int) nCount, nTabPositions, lpnTabStopPositions);

   }

   size graphics::GetTabbedTextExtent(const string & str, count nTabPositions, LPINT lpnTabStopPositions) const
   {

      throw todo(get_app());

      //ASSERT(get_handle2() != NULL); 

      //return ::GetTabbedTextExtent(get_handle2(), str, (int) str.get_length(), nTabPositions, lpnTabStopPositions);

   }


   size graphics::GetOutputTabbedTextExtent(const char * lpszString, strsize nCount, count nTabPositions, LPINT lpnTabStopPositions) const
   {

      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::GetTabbedTextExtent(get_handle1(), lpszString, (int) nCount, nTabPositions, lpnTabStopPositions);

   }

   size graphics::GetOutputTabbedTextExtent(const string & str, count nTabPositions, LPINT lpnTabStopPositions) const
   {

      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::GetTabbedTextExtent(get_handle1(), str, (int) str.get_length(), nTabPositions, lpnTabStopPositions);

   }

   bool graphics::GrayString(::draw2d::brush* pBrush, bool (CALLBACK* lpfnOutput)(HDC, LPARAM, int), LPARAM lpData, int nCount,int x, int y, int nWidth, int nHeight)
   {

      throw todo(get_app());

      //ASSERT(get_handle1() != NULL); 

      //return ::GrayString(get_handle1(), (HBRUSH)pBrush->get_os_data(),(GRAYSTRINGPROC)lpfnOutput, lpData, nCount, x, y, nWidth, nHeight) != FALSE;


   }

   UINT graphics::GetTextAlign() const
   {

      throw todo(get_app());

      //ASSERT(get_handle2() != NULL); 
      //
      //return ::GetTextAlign(get_handle2());

   }

   int graphics::GetTextFace(count nCount, LPTSTR lpszFacename) const
   {

      throw todo(get_app());

      //ASSERT(get_handle2() != NULL); 

      //return ::GetTextFace(get_handle2(), nCount, lpszFacename);

   }

   int graphics::GetTextFace(string & rString) const
   {

      throw todo(get_app());

      //ASSERT(get_handle2() != NULL);

      //int nResult = ::GetTextFace(get_handle2(), 256, rString.GetBuffer(256)); rString.ReleaseBuffer();
      //return nResult;

   }

   bool graphics::get_text_metrics(::draw2d::text_metric * lpMetrics) const
   { 


      if(m_spfont.is_null())
         ((graphics *) this)->m_spfont.alloc(((graphics *) this)->allocer());

      if(m_spfont.is_null())
         return false;

      IDWriteFontCollection * pcollection = NULL;

      WCHAR name[256]; 
      UINT32 findex; 
      BOOL exists;

      if (get_os_font(m_spfont) == NULL)
      {
         lpMetrics->tmAveCharWidth = 0;
         lpMetrics->tmAscent = 0;
         lpMetrics->tmDescent = 0;
         lpMetrics->tmHeight = 0;

         return true;
      }
      get_os_font(m_spfont)->GetFontFamilyName(name, 256);

      get_os_font(m_spfont)->GetFontCollection(&pcollection);

      if (pcollection == NULL)
      {
         
         lpMetrics->tmAveCharWidth = 0;
         lpMetrics->tmAscent = 0;
         lpMetrics->tmDescent = 0;
         lpMetrics->tmHeight = 0;

         return true;

      }

      pcollection->FindFamilyName(name, &findex, &exists);

      if (!exists)
      {

         lpMetrics->tmAveCharWidth = 0;
         lpMetrics->tmAscent = 0;
         lpMetrics->tmDescent = 0;
         lpMetrics->tmHeight = 0;

         return true;


      }

      IDWriteFontFamily *ffamily;

      pcollection->GetFontFamily(findex, &ffamily);

      if (ffamily == NULL)
      {

         lpMetrics->tmAveCharWidth = 0;
         lpMetrics->tmAscent = 0;
         lpMetrics->tmDescent = 0;
         lpMetrics->tmHeight = 0;
         pcollection->Release();
         return true;

      }
      IDWriteFont * pfont;

      ffamily->GetFirstMatchingFont(get_os_font(m_spfont)->GetFontWeight(), get_os_font(m_spfont)->GetFontStretch(), get_os_font(m_spfont)->GetFontStyle(), &pfont);

      if (pfont == NULL)
      {

         lpMetrics->tmAveCharWidth = 0;
         lpMetrics->tmAscent = 0;
         lpMetrics->tmDescent = 0;
         lpMetrics->tmHeight = 0;
         ffamily->Release();
         pcollection->Release();
         return true;

      }

      DWRITE_FONT_METRICS metrics;

      pfont->GetMetrics(&metrics);

      double ratio = get_os_font(m_spfont)->GetFontSize() / (float)metrics.designUnitsPerEm;

      string str("APC1pgfditflmnopw");

      ::size sz = GetTextExtent(str);

      lpMetrics->tmAveCharWidth = (LONG) (sz.cx / str.get_length());
      lpMetrics->tmAscent = (LONG) ( metrics.ascent * ratio);
      lpMetrics->tmDescent = (LONG)( metrics.descent * ratio);
      lpMetrics->tmInternalLeading = (LONG)0;
      lpMetrics->tmExternalLeading = (LONG)( metrics.lineGap * ratio);
      lpMetrics->tmHeight = (LONG) ((metrics.ascent + metrics.descent + metrics.lineGap) * ratio);

      pfont->Release();
      ffamily->Release();
      pcollection->Release();

      return true;

   }


   bool graphics::get_output_text_metrics(::draw2d::text_metric * lpMetrics) const
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::GetTextMetrics(get_handle1(), lpMetrics) != FALSE;

   }


   int graphics::GetTextCharacterExtra() const
   {
      
      throw todo(get_app());

      //ASSERT(get_handle2() != NULL);

      //return ::GetTextCharacterExtra(get_handle2());

   }


   bool graphics::GetCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const
   {
      
      throw todo(get_app());

      //ASSERT(get_handle2() != NULL);

      //return ::GetCharWidth(get_handle2(), nFirstChar, nLastChar, lpBuffer) != FALSE;

   }

   bool graphics::GetOutputCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::GetCharWidth(get_handle1(), nFirstChar, nLastChar, lpBuffer) != FALSE;

   }

   uint32_t graphics::GetFontLanguageInfo() const
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::GetFontLanguageInfo(get_handle1());

   }

#ifdef WINDOWSEX

/*   uint32_t graphics::GetCharacterPlacement(const char * lpString, int nCount, int nMaxExtent, LPGCP_RESULTS lpResults, uint32_t dwFlags) const
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::GetCharacterPlacement(get_handle1(), lpString, nCount, nMaxExtent, lpResults, dwFlags);

   }

   uint32_t graphics::GetCharacterPlacement(string & str, int nMaxExtent, LPGCP_RESULTS lpResults, uint32_t dwFlags) const
   { 
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL); 

      //return ::GetCharacterPlacement(get_handle1(), (const char *)str, (int) str.get_length(), nMaxExtent, lpResults, dwFlags);

   }
   */
#endif

   size graphics::GetAspectRatioFilter() const
   {
      
      throw todo(get_app());

      //ASSERT(get_handle2() != NULL);
      //SIZE size;
      //VERIFY(::GetAspectRatioFilterEx(get_handle2(), &size));
      //return size;

   }

   bool graphics::ScrollDC(int dx, int dy, const RECT & lpRectScroll, const RECT & lpRectClip, ::draw2d::region* pRgnUpdate, LPRECT lpRectUpdate)
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::ScrollDC(get_handle1(), dx, dy, lpRectScroll, lpRectClip, (HRGN)pRgnUpdate->get_os_data(), lpRectUpdate) != FALSE;

   }

   // Printer Escape Functions
   int graphics::Escape(int nEscape, int nCount, const char * lpszInData, LPVOID lpOutData)
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::Escape(get_handle1(), nEscape, nCount, lpszInData, lpOutData);
   
   }

   // graphics 3.1 Specific functions
   UINT graphics::SetBoundsRect(const RECT & lpRectBounds, UINT flags)
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::SetBoundsRect(get_handle1(), lpRectBounds, flags);

   }


   UINT graphics::GetBoundsRect(LPRECT lpRectBounds, UINT flags)
   {
      
      throw todo(get_app());

      //ASSERT(get_handle2() != NULL);

      //return ::GetBoundsRect(get_handle2(), lpRectBounds, flags);

   }

   bool graphics::ResetDC(const DEVMODE* lpDevMode)
   {
      
      throw todo(get_app());

      //ASSERT(get_handle2() != NULL);

      //return ::ResetDC(get_handle2(), lpDevMode) != NULL;

   }

#ifdef WINDOWSEX

/*   UINT graphics::GetOutlineTextMetrics(UINT cbData, LPOUTLINETEXTMETRICW lpotm) const
   {
      
      throw todo(get_app());

      //ASSERT(get_handle2() != NULL);

      //return ::GetOutlineTextMetrics(get_handle2(), cbData, lpotm);

   }

   bool graphics::GetCharABCWidths(UINT nFirstChar, UINT nLastChar, LPABC lpabc) const
   {
      
      throw todo(get_app());

      //ASSERT(get_handle2() != NULL);

      //return ::GetCharABCWidths(get_handle2(), nFirstChar, nLastChar, lpabc) != FALSE;

   }
   */
#endif

   uint32_t graphics::GetFontData(uint32_t dwTable, uint32_t dwOffset, LPVOID lpData, uint32_t cbData) const
   {
      
      throw todo(get_app());

      //ASSERT(get_handle2() != NULL);

      //return ::GetFontData(get_handle2(), dwTable, dwOffset, lpData, cbData);

   }

#ifdef WINDOWSEX

   /*
   int graphics::GetKerningPairs(int nPairs, LPKERNINGPAIR lpkrnpair) const
   {
      
      throw todo(get_app());

      //ASSERT(get_handle2() != NULL);

      //return ::GetKerningPairs(get_handle2(), nPairs, lpkrnpair);

   }

   uint32_t graphics::GetGlyphOutline(UINT nChar, UINT nFormat, LPGLYPHMETRICS lpgm,  uint32_t cbBuffer, LPVOID lpBuffer, const MAT2* lpmat2) const
   {
      
      throw todo(get_app());

      //ASSERT(get_handle2() != NULL);

      //return ::GetGlyphOutline(get_handle2(), nChar, nFormat, lpgm, cbBuffer, lpBuffer, lpmat2);

   }

   // ::userbase::document handling functions
   int graphics::StartDoc(LPDOCINFO lpDocInfo)
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::StartDoc(get_handle1(), lpDocInfo);

   }
   */
#endif

   int graphics::StartPage()
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::StartPage(get_handle1());

   }

   int graphics::EndPage()
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::EndPage(get_handle1());

   }


   int graphics::SetAbortProc(bool (CALLBACK* lpfn)(HDC, int))
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::SetAbortProc(get_handle1(), (ABORTPROC)lpfn);

   }


   int graphics::AbortDoc()
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::AbortDoc(get_handle1());

   }


   int graphics::EndDoc()
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::EndDoc(get_handle1());

   }


   bool graphics::MaskBlt(int x, int y, int nWidth, int nHeight, ::draw2d::graphics * pgraphicsSrc, int xSrc, int ySrc, ::draw2d::bitmap& maskBitmap, int xMask, int yMask, uint32_t dwRop)
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::MaskBlt(get_handle1(), x, y, nWidth, nHeight, WIN_HDC(pgraphicsSrc), xSrc, ySrc,  (HBITMAP)maskBitmap.get_os_data(), xMask, yMask, dwRop) != FALSE;

   }


   bool graphics::PlgBlt(LPPOINT lpPoint, ::draw2d::graphics * pgraphicsSrc, int xSrc, int ySrc, int nWidth, int nHeight, ::draw2d::bitmap& maskBitmap, int xMask, int yMask)
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::PlgBlt(get_handle1(), lpPoint, WIN_HDC(pgraphicsSrc), xSrc, ySrc, nWidth, nHeight, (HBITMAP)maskBitmap.get_os_data(), xMask, yMask) != FALSE;

   }


   bool graphics::SetPixelV(int x, int y, COLORREF crColor)
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::SetPixelV(get_handle1(), x, y, crColor) != FALSE;

   }

   bool graphics::SetPixelV(POINT point, COLORREF crColor)
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::SetPixelV(get_handle1(), point.x, point.y, crColor) != FALSE;

   }

   bool graphics::AngleArc(int x, int y, int nRadius, float fStartAngle, float fSweepAngle)
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::AngleArc(get_handle1(), x, y, nRadius, fStartAngle, fSweepAngle) != FALSE;

   }


   bool graphics::ArcTo(const RECT & lpRect, POINT ptStart, POINT ptEnd)
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ArcTo(lpRect.left, lpRect.top, lpRect.right, lpRect.bottom, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);

   }

   int graphics::GetArcDirection() const
   {
      
      throw todo(get_app());

      //ASSERT(get_handle2() != NULL);

      //return ::GetArcDirection(get_handle2());

   }

   bool graphics::PolyPolyline(const POINT* lpPoints, const INT * lpPolyPoints, count nCount)
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::PolyPolyline(get_handle1(), lpPoints, lpPolyPoints, nCount) != FALSE;

   }

#ifdef WINDOWSEX
/*
   bool graphics::GetColorAdjustment(LPCOLORADJUSTMENT lpColorAdjust) const
   {
      
      throw todo(get_app());

      //ASSERT(get_handle2() != NULL);

      //return ::GetColorAdjustment(get_handle2(), lpColorAdjust) != FALSE;

   }
   */
#endif

   ::draw2d::pen_sp graphics::get_current_pen() const
   {

      return m_sppen;

   }

   ::draw2d::brush_sp graphics::get_current_brush() const
   {

      return m_spbrush;

   }

   ::draw2d::palette_sp graphics::get_current_palette() const
   {

      return (::draw2d::palette *)NULL;

   }

   ::draw2d::font_sp graphics::get_current_font() const
   {

      return m_spfont;

   }

   ::draw2d::bitmap_sp graphics::get_current_bitmap() const
   {

      return m_spbitmap;

   }

   bool graphics::PolyBezier(const POINT* lpPoints, count nCount)
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL); 

      //return ::PolyBezier(get_handle1(), lpPoints, nCount) != FALSE; 

   }

   int graphics::DrawEscape(int nEscape, int nInputSize, const char * lpszInputData)
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL); 

      //return ::DrawEscape(get_handle1(), nEscape, nInputSize, lpszInputData); 

   }


   int graphics::Escape(__in int nEscape, __in int nInputSize, __in_bcount(nInputSize) const char * lpszInputData,  __in int nOutputSize, __out_bcount(nOutputSize) char * lpszOutputData)
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL); 

      //return ::ExtEscape(get_handle1(), nEscape, nInputSize, lpszInputData, nOutputSize, lpszOutputData); 

   }


#ifdef WINDOWSEX
/*
   bool graphics::GetCharABCWidths(UINT nFirstChar, UINT nLastChar, LPABCFLOAT lpABCF) const
   { 
      
      throw todo(get_app());

      //ASSERT(get_handle2() != NULL); 
      //
      //return ::GetCharABCWidthsFloat(get_handle2(), nFirstChar, nLastChar, lpABCF) != FALSE; 

   }
   */
#endif

   bool graphics::GetCharWidth(UINT nFirstChar, UINT nLastChar, float* lpFloatBuffer) const
   {
      
      throw todo(get_app());

      //ASSERT(get_handle2() != NULL); 

      //return ::GetCharWidthFloat(get_handle2(), nFirstChar, nLastChar, lpFloatBuffer) != FALSE; 

   }

   bool graphics::AbortPath()
   {
      
      throw todo(get_app());

      //if(m_ppath != NULL)
      //{
      //   delete m_ppath;
      //   m_ppath = NULL;
      //}
      //return true;


   }

   bool graphics::BeginPath()
   { 
      
      throw todo(get_app());

   }

   bool graphics::CloseFigure()
   {
      
      throw todo(get_app());

      //ASSERT(m_ppath != NULL); 
      //return m_ppath->CloseFigure() == Gdiplus::Status::Ok;

   }

   bool graphics::EndPath()
   { 
      
      throw todo(get_app());

      //if(m_ppath == NULL)
      //   return FALSE;

      //m_ppathPaint   = m_ppath;
      //m_ppath        = NULL;

      //return true;

   }

   bool graphics::FillPath()
   { 
      
      throw todo(get_app());

      //return m_prendertarget->FillPath(direct2d_brush(), m_ppath) == Gdiplus::Status::Ok;

   }

   bool graphics::FlattenPath()
   {
      
      throw todo(get_app());

      //return m_ppath->Flatten() == Gdiplus::Status::Ok;

   }

   float graphics::GetMiterLimit() const
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL); 
      //float fMiterLimit;
      //VERIFY(::GetMiterLimit(get_handle1(), &fMiterLimit)); 
      //return fMiterLimit; 

   }

   int graphics::GetPath(LPPOINT lpPoints, LPBYTE lpTypes, int nCount) const
   { 
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL); 
      //return ::GetPath(get_handle1(), lpPoints, lpTypes, nCount); 


   }
   bool graphics::SetMiterLimit(float fMiterLimit)
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL); 
      //return ::SetMiterLimit(get_handle1(), fMiterLimit, NULL) != FALSE; 

   }

   bool graphics::StrokeAndFillPath()
   { 
      
      throw todo(get_app());

      //bool bOk1 = m_prendertarget->FillPath(direct2d_brush(), m_ppathPaint) == Gdiplus::Status::Ok;

      //bool bOk2 = m_prendertarget->DrawPath(direct2d_pen(), m_ppathPaint) == Gdiplus::Status::Ok;

      //return bOk1 && bOk2;

   }

   bool graphics::StrokePath()
   {
      
      throw todo(get_app());

//      return m_prendertarget->DrawPath(direct2d_pen(), m_ppathPaint) == Gdiplus::Status::Ok;
   }

   bool graphics::WidenPath()
   {
      
      throw todo(get_app());


//      return m_ppath->Widen(direct2d_pen()) == Gdiplus::Status::Ok;
   }

   bool graphics::AddMetaFileComment(UINT nDataSize, const BYTE* pCommentData)
   { 
      
      throw todo(get_app());


      //ASSERT(get_handle1() != NULL); 
      //return ::GdiComment(get_handle1(), nDataSize, pCommentData) != FALSE; 
   }


   /*bool CALLBACK metaCallback(
   EmfPlusRecordType recordType, 
   unsigned int flags, 
   unsigned int dataSize, 
   const unsigned char* pStr, 
   void* callbackData)
   { 
   // Play only EmfPlusRecordTypeFillEllipse records.
   if (recordType == EmfPlusRecordTypeFillEllipse)
   {
   // Explicitly cast callbackData as a metafile pointer, and use it to call
   // the PlayRecord method.
   static_cast < Metafile* > (callbackData)->PlayRecord(recordType, flags, dataSize, pStr);
   }
   return TRUE; 
   }

   VOID Example_EnumerateMetafile9(HDC hdc)
   {   
   Graphics graphics(hdc);
   // Create a Metafile object from an existing disk metafile.
   Metafile* pMeta = new Metafile(L"SampleMetafile.emf", hdc);
   {
   // Fill a rectangle and an ellipse in pMeta.
   Graphics metaGraphics(pMeta);
   metaGraphics.FillRectangle(&SolidBrush(Color(255, 0, 0, 0)), 0, 0, 100, 100);
   metaGraphics.FillEllipse(&SolidBrush(Color(255, 255, 0, 0)), 100, 0, 200, 100);
   }
   // Enumerate pMeta to the destination rectangle, passing pMeta as the callback data. 
   graphics.EnumerateMetafile(
   pMeta,
   Rect(0, 0, 300, 50),
   metaCallback,
   pMeta);
   // Draw pMeta as an image.
   graphics.DrawImage(pMeta, Point(0, 150));
   delete pMeta;;
   }*/
   bool graphics::PlayMetaFile(HENHMETAFILE hEnhMF, const RECT & lpBounds)
   { 
      
      throw todo(get_app());

      //Gdiplus::RectF rect((Gdiplus::REAL) lpBounds->left, (Gdiplus::REAL) lpBounds->top, (Gdiplus::REAL) width(lpBounds), (Gdiplus::REAL) height(lpBounds));

      //Gdiplus::Metafile* pMeta = new Gdiplus::Metafile(hEnhMF, false);

      ////m_pgraphcis->EnumerateMetafile(pMeta, rect, metaCallback, PMETAHEADER);

      //bool bOk = m_prendertarget->DrawImage(pMeta, rect) == Gdiplus::Status::Ok;

      //delete pMeta;

      //return bOk ? TRUE : FALSE;
      ////return ::PlayEnhMetaFile(get_handle1(), hEnhMF, lpBounds); 

   }

   // true blend
   // COLOR_DEST = SRC_ALPHA * COLOR_SRC  + (1 - SRC_ALPHA) * COLOR_DST

   // double blend
   // COLOR_DEST = SRC_ALPHA * BLEND_ALPHA * COLOR_SRC  + (1 - SRC_ALPHA * BLEND_ALPHA) * COLOR_DST

   // Thank you
   // Jiju George T
   // Web Developer
   // India India
   // Member

   bool graphics::alpha_blendRaw(int xDst, int yDst, int nDstWidth, int nDstHeight, ::draw2d::graphics * pgraphicsSrc, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, double dRate)
   {

      synch_lock sl(&draw2d_mutex());

/*      float fA = (float) dRate;

      Gdiplus::ColorMatrix matrix = { 
         1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
         0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.0f, 0.0f, 0.0f, fA  , 0.0f,
         0.0f, 0.0f, 0.0f, 0.0f, 1.0f
      };

      Gdiplus::ImageAttributes attributes;

      attributes.SetColorMatrix(&matrix, Gdiplus::ColorMatrixFlagsDefault, Gdiplus::ColorAdjustTypeBitmap);

      Gdiplus::RectF dstRect((Gdiplus::REAL) xDest, (Gdiplus::REAL) yDest, (Gdiplus::REAL) nDestWidth, (Gdiplus::REAL) nDestHeight);

      m_prendertarget->DrawImage((Gdiplus::Bitmap *) pgraphicsSrc->get_current_bitmap()->get_os_data(), dstRect, 
         (Gdiplus::REAL) xSrc, (Gdiplus::REAL) ySrc, (Gdiplus::REAL) nSrcWidth, (Gdiplus::REAL) nSrcHeight, Gdiplus::UnitPixel, &attributes);*/

      try
      {

         if(pgraphicsSrc == NULL)
            return FALSE;

         if(pgraphicsSrc->get_current_bitmap() == NULL)
            return FALSE;

         if(pgraphicsSrc->get_current_bitmap()->get_os_data() == NULL)
            return FALSE;

         //D2D1_RECT_F rectDst = D2D1::RectF((float) xDst, (float) yDst, (float) (xDst + nDstWidth), (float) (yDst + nDstHeight));
         //D2D1_RECT_F rectSrc = D2D1::RectF((float) xSrc, (float) ySrc, (float) (xSrc + nSrcWidth), (float) (ySrc + nSrcHeight));
/*
         if (get_current_bitmap() != NULL && get_current_bitmap()->get_os_data() != NULL)
         {

            D2D1_SIZE_U sz = ((ID2D1Bitmap *)get_current_bitmap()->get_os_data())->GetPixelSize();

            if (natural(nDstWidth + xDst) > sz.width)
               nDstWidth = sz.width - xDst;

            if (natural(nDstHeight + yDst) > sz.height)
               nDstHeight = sz.height - yDst;

         }

         {

            D2D1_SIZE_U sz = ((ID2D1Bitmap *)pgraphicsSrc->get_current_bitmap()->get_os_data())->GetPixelSize();

            if (natural(nSrcWidth + xSrc) > sz.width)
               nSrcWidth = sz.width - xSrc;

            if (natural(nSrcHeight + ySrc) > sz.height)
               nSrcHeight = sz.height - ySrc;

         }*/

         D2D1_RECT_F rectDst = D2D1::RectF((float)xDst, (float)yDst, (float)(xDst + nDstWidth), (float)(yDst + nDstHeight));
         D2D1_RECT_F rectSrc = D2D1::RectF((float)xSrc, (float)ySrc, (float)(xSrc + nSrcWidth), (float)(ySrc + nSrcHeight));


         dynamic_cast <::draw2d_direct2d::graphics *> (pgraphicsSrc)->SaveClip();

         HRESULT hr = ((ID2D1DeviceContext *) pgraphicsSrc->get_os_data())->EndDraw();

         if(m_pdevicecontext != NULL)
         {
            m_prendertarget->DrawBitmap((ID2D1Bitmap *)pgraphicsSrc->get_current_bitmap()->get_os_data(),rectDst,(float)dRate,m_bitmapinterpolationmode,rectSrc);
         }
         else
         {
            m_pdevicecontext->DrawBitmap((ID2D1Bitmap *)pgraphicsSrc->get_current_bitmap()->get_os_data(),rectDst,(float)dRate,m_interpolationmode,rectSrc);
         }

         //hr = m_prendertarget->Flush();
         flush();

         if(SUCCEEDED(hr))
         {

            ((ID2D1DeviceContext *)pgraphicsSrc->get_os_data())->BeginDraw();

            dynamic_cast <::draw2d_direct2d::graphics *> (pgraphicsSrc)->RestoreClip();

         }

         return true;

      }
      catch(...)
      {
         return FALSE;
      }



      return true;

   }


   /*bool graphics::alpha_blend(int xDest, int yDest, int nDestWidth, int nDestHeight,
   ::draw2d::graphics * pgraphicsSrc, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, BLENDFUNCTION blend)
   { 

   throw not_implemented_exception();
   //if(get_handle1() == NULL)
   // return false;


   if(m_pdibAlphaBlend != NULL)
   {


   rect rectIntersect(m_ptAlphaBlend, m_pdibAlphaBlend->size());


   ::draw2d::dib * pdibWork = NULL;
   ::draw2d::dib * pdibWork2 = NULL;
   //         ::draw2d::dib * pdibWork3 = NULL;
   ::draw2d::dib * pdibWork4 = NULL;


   class point ptSrc(xSrc, ySrc);
   class point ptDest(xDest, yDest);
   class size size(nDestWidth, nDestHeight);



   ::draw2d::dib_sp spdib;
   if(pdibWork == NULL)
   {
   spdib.create(get_app());
   pdibWork = spdib;
   }
   if(pdibWork == NULL)
   return false;
   if(!pdibWork->create(size))
   return false;
   if(!pdibWork->from(null_point(), pgraphicsSrc, ptSrc, size))
   return false;




   ::draw2d::dib_sp spdib2;
   if(pdibWork2 == NULL)
   {
   spdib2.create(get_app());
   pdibWork2 = spdib2;
   }


   ::draw2d::dib_sp spdib4;
   if(pdibWork4 == NULL)
   {
   spdib4.create(get_app());
   pdibWork4 = spdib4;
   }
   if(pdibWork4 == NULL)
   return false;
   if(!pdibWork4->create(size))
   return false;


   pdibWork4->Fill(255, 0, 0, 0);

   pdibWork4->from(point(MAX(0, m_ptAlphaBlend.x - xDest), MAX(0, m_ptAlphaBlend.y - yDest)),
   m_pdibAlphaBlend->get_graphics(), point(MAX(0, xDest - m_ptAlphaBlend.x), MAX(0, yDest - m_ptAlphaBlend.y)), size);

   pdibWork->channel_multiply(visual::rgba::channel_alpha, pdibWork4);


   keep < ::draw2d::dib * > keep(&m_pdibAlphaBlend, NULL, m_pdibAlphaBlend, true);


   return Application.m_visual.imaging().true_blend(this, ptDest, size, pdibWork->get_graphics(), ptSrc); 


   }

   return ::AlphaBlend(get_handle1(), xDest, yDest, 
   nDestWidth, nDestHeight, WIN_HDC(pgraphicsSrc), xSrc, ySrc, nSrcWidth, 
   nSrcHeight, blend) != FALSE; 
   }*/


   bool graphics::TransparentBlt(int xDest, int yDest, int nDestWidth, int nDestHeight, ::draw2d::graphics * pgraphicsSrc, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, UINT crTransparent)
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::TransparentBlt(get_handle1(), xDest, yDest, nDestWidth, nDestHeight, WIN_HDC(pgraphicsSrc), xSrc, ySrc, nSrcWidth, nSrcHeight, crTransparent) != FALSE;

   }

   bool graphics::GradientFill(TRIVERTEX* pVertices, ULONG nVertices, void * pMesh, ULONG nMeshElements, uint32_t dwMode)
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL); 
      //
      //return ::GradientFill(get_handle1(), pVertices, nVertices, pMesh, nMeshElements, dwMode) != FALSE;

   }

   // This is core API library.
   // 
   // 
   //
   // 
   // 
   // 
   // 
   // 

   // Always Inline. Functions only in Win98/Win2K or later

   inline COLORREF graphics::GetDCBrushColor() const
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::GetDCBrushColor(get_handle1());

   }
   inline COLORREF graphics::SetDCBrushColor(COLORREF crColor)
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::SetDCBrushColor(get_handle1(), crColor);

   }

   inline COLORREF graphics::GetDCPenColor() const
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::GetDCPenColor(get_handle1());

   }
   inline COLORREF graphics::SetDCPenColor(COLORREF crColor)
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::SetDCPenColor(get_handle1(), crColor);

   }

#ifdef WINDOWSEX
/*
   inline bool graphics::GetCharABCWidthsI(UINT giFirst, UINT cgi, LPWORD pgi, LPABC lpabc) const
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::GetCharABCWidthsI(get_handle1(), giFirst, cgi, pgi, lpabc) != FALSE;

   }
   */
#endif

   inline bool graphics::GetCharWidthI(UINT giFirst, UINT cgi, LPWORD pgi, LPINT lpBuffer) const
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //return ::GetCharWidthI(get_handle1(), giFirst, cgi, pgi, lpBuffer) != FALSE;

   }

   inline bool graphics::GetTextExtentExPointI(LPWORD pgiIn, int cgi, int nMaxExtent, LPINT lpnFit, LPINT alpDx, LPSIZE lpSize) const
   { 
      
      throw todo(get_app());

      //ENSURE(lpSize != NULL); 
      //ASSERT(get_handle1() != NULL); 
      //return ::GetTextExtentExPointI(get_handle1(), pgiIn, cgi, nMaxExtent, lpnFit, alpDx, lpSize) != FALSE; 

   }


   inline bool graphics::GetTextExtentPointI(LPWORD pgiIn, int cgi, LPSIZE lpSize) const
   {
      
      throw todo(get_app());

      //ENSURE(lpSize != NULL); 
      //ASSERT(get_handle1() != NULL); 
      //return ::GetTextExtentPointI(get_handle1(), pgiIn, cgi, lpSize) != FALSE; 

   }






   

#define HIMETRIC_INCH   2540    // HIMETRIC units per inch

   void graphics::DPtoHIMETRIC(LPSIZE lpSize) const
   {
#ifdef WINDOWSEX
      ASSERT(__is_valid_address(lpSize, sizeof(SIZE)));

      int nMapMode;
      if (this != NULL && (nMapMode = GetMapMode()) < MM_ISOTROPIC &&
         nMapMode != MM_TEXT)
      {
         // when using a constrained ::map mode, ::map against physical inch
         ((::draw2d::graphics *)this)->SetMapMode(MM_HIMETRIC);
         DPtoLP(lpSize);
         ((::draw2d::graphics *)this)->SetMapMode(nMapMode);
      }
      else
      {
         // ::map against logical inch for non-constrained mapping modes
         int cxPerInch, cyPerInch;
         if (this != NULL)
         {
            ASSERT_VALID(this);
            ASSERT(get_handle1() != NULL);  // no HDC attached or created?
            cxPerInch = GetDeviceCaps(LOGPIXELSX);
            cyPerInch = GetDeviceCaps(LOGPIXELSY);
         }
         else
         {
            //            cxPerInch = afxData.cxPixelsPerInch;
            //          cyPerInch = afxData.cyPixelsPerInch;
         }
         ASSERT(cxPerInch != 0 && cyPerInch != 0);
         lpSize->cx = MulDiv(lpSize->cx, HIMETRIC_INCH, cxPerInch);
         lpSize->cy = MulDiv(lpSize->cy, HIMETRIC_INCH, cyPerInch);
      }
#endif
   }

   void graphics::HIMETRICtoDP(LPSIZE lpSize) const
   {
#ifdef WINDOWSEX
      ASSERT(__is_valid_address(lpSize, sizeof(SIZE)));

      int nMapMode;
      if (this != NULL && (nMapMode = GetMapMode()) < MM_ISOTROPIC &&
         nMapMode != MM_TEXT)
      {
         // when using a constrained ::map mode, ::map against physical inch
         ((::draw2d::graphics *)this)->SetMapMode(MM_HIMETRIC);
         LPtoDP(lpSize);
         ((::draw2d::graphics *)this)->SetMapMode(nMapMode);
      }
      else
      {
         // ::map against logical inch for non-constrained mapping modes
         int cxPerInch, cyPerInch;
         if (this != NULL)
         {
            ASSERT_VALID(this);
            ASSERT(get_handle1() != NULL);  // no HDC attached or created?
            cxPerInch = GetDeviceCaps(LOGPIXELSX);
            cyPerInch = GetDeviceCaps(LOGPIXELSY);
         }
         else
         {
            //            cxPerInch = afxData.cxPixelsPerInch;
            //          cyPerInch = afxData.cyPixelsPerInch;
         }
         ASSERT(cxPerInch != 0 && cyPerInch != 0);
         lpSize->cx = MulDiv(lpSize->cx, cxPerInch, HIMETRIC_INCH);
         lpSize->cy = MulDiv(lpSize->cy, cyPerInch, HIMETRIC_INCH);
      }
#endif
   }

   void graphics::LPtoHIMETRIC(LPSIZE lpSize) const
   {
      ASSERT(__is_valid_address(lpSize, sizeof(SIZE)));

      LPtoDP(lpSize);
      DPtoHIMETRIC(lpSize);
   }

   void graphics::HIMETRICtoLP(LPSIZE lpSize) const
   {
      ASSERT(__is_valid_address(lpSize, sizeof(SIZE)));

      HIMETRICtoDP(lpSize);
      DPtoLP(lpSize);
   }

   /////////////////////////////////////////////////////////////////////////////
   // special graphics drawing primitives/helpers

   ::draw2d::brush* graphics::GetHalftoneBrush(::aura::application * papp)
   {
      /*
      ::core::LockGlobals(CRIT_HALFTONEBRUSH);
      if (gen_HalftoneBrush == NULL)
      {
      WORD grayPattern[8];
      for (int i = 0; i < 8; i++)
      grayPattern[i] = (WORD)(0x5555 << (i & 1));
      HBITMAP grayBitmap = CreateBitmap(8, 8, 1, 1, grayPattern);
      if (grayBitmap != NULL)
      {
      gen_HalftoneBrush = ::CreatePatternBrush(grayBitmap);
      ::DeleteObject(grayBitmap);
      }
      }
      if (!gen_WingdixTerm)
      gen_WingdixTerm = (char)!atexit(&__win_gdi_x_term);
      ::core::UnlockGlobals(CRIT_HALFTONEBRUSH);
      */

      return NULL;
   }


   void graphics::DrawDragRect(const RECT & lpRect, SIZE size, const RECT & lpRectLast, SIZE sizeLast, ::draw2d::brush* pBrush, ::draw2d::brush* pBrushLast)
   {

      ::exception::throw_not_implemented(get_app());

      /*
      ASSERT(__is_valid_address(lpRect, sizeof(RECT), FALSE));
      ASSERT(lpRectLast == NULL ||
      __is_valid_address(lpRectLast, sizeof(RECT), FALSE));

      // first, determine the update region and select it
      ::draw2d::region rgnNew;
      ::draw2d::region rgnOutside, rgnInside;
      rgnOutside.CreateRectRgnIndirect(lpRect);
      rect rect = *lpRect;
      rect.inflate(-size.cx, -size.cy);
      rect.intersect(rect, lpRect);
      rgnInside.CreateRectRgnIndirect(rect);
      rgnNew.CreateRectRgn(0, 0, 0, 0);
      rgnNew.CombineRgn(&rgnOutside, &rgnInside, RGN_XOR);

      ::draw2d::brush* pBrushOld = NULL;
      if (pBrush == NULL)
      {
      pBrush = graphics::GetHalftoneBrush(get_app());
      }

      ENSURE(pBrush);

      if (pBrushLast == NULL)
      {
      pBrushLast = pBrush;
      }

      ::draw2d::region rgnLast, rgnUpdate;
      if (lpRectLast != NULL)
      {
      // find difference between new region and old region
      rgnLast.CreateRectRgn(0, 0, 0, 0);
      rgnOutside.SetRectRgn(lpRectLast);
      rect = *lpRectLast;
      rect.inflate(-sizeLast.cx, -sizeLast.cy);
      rect.intersect(rect, lpRectLast);
      rgnInside.SetRectRgn(rect);
      rgnLast.CombineRgn(&rgnOutside, &rgnInside, RGN_XOR);

      // only diff them if brushes are the same
      if (pBrush->get_os_data() == pBrushLast->get_os_data())
      {
      rgnUpdate.CreateRectRgn(0, 0, 0, 0);
      rgnUpdate.CombineRgn(&rgnLast, &rgnNew, RGN_XOR);
      }
      }
      if (pBrush->get_os_data() != pBrushLast->get_os_data() && lpRectLast != NULL)
      {
      // brushes are different -- erase old region first
      SelectClipRgn(&rgnLast);
      GetClipBox(&rect);
      pBrushOld = SelectObject(pBrushLast);
      PatBlt(rect.left, rect.top, rect.width(), rect.height(), PATINVERT);
      SelectObject(pBrushOld);
      pBrushOld = NULL;
      }

      // draw into the update/new region
      SelectClipRgn(rgnUpdate.get_os_data() != NULL ? &rgnUpdate : &rgnNew);
      GetClipBox(&rect);
      pBrushOld = SelectObject(pBrush);
      PatBlt(rect.left, rect.top, rect.width(), rect.height(), PATINVERT);

      // cleanup DC
      if (pBrushOld != NULL)
      SelectObject(pBrushOld);
      SelectClipRgn(NULL);

      */

   }

   void graphics::FillSolidRect(const RECT64 * lpRect, COLORREF clr)
   {
      rect rect32;
      ::copy(rect32, lpRect);
      FillSolidRect(rect32, clr);
   }


   void graphics::Draw3dRect(const RECT & lpRect, COLORREF clrTopLeft, COLORREF clrBottomRight)
   {
      Draw3dRect(lpRect.left, lpRect.top, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, clrTopLeft, clrBottomRight);
   }

   void graphics::Draw3dRect(int x, int y, int cx, int cy,  COLORREF clrTopLeft, COLORREF clrBottomRight)
   {
      FillSolidRect(x, y, cx - 1, 1, clrTopLeft);
      FillSolidRect(x, y, 1, cy - 1, clrTopLeft);
      FillSolidRect(x + cx, y, -1, cy, clrBottomRight);
      FillSolidRect(x, y + cy, cx, -1, clrBottomRight);
   }






/*   bool graphics::Attach(HDC hdc)
   {

      if(hdc == NULL)
         return FALSE;

      if(m_hdc == hdc)
      {

         if(m_prendertarget != NULL)
            return TRUE;

      }


      /*ASSERT(get_handle1() == NULL);      // only attach once, detach on destroy
      ASSERT(get_handle2() == NULL);    // only attach to an is_empty DC

      if (hDC == NULL)
      {
      return FALSE;
      }
      // remember early to avoid leak
      set_handle1(hDC);
      hdc_map* pMap = afxMapHDC(TRUE); // create ::map if not exist
      ASSERT(pMap != NULL);
      pMap->set_permanent(get_handle1(), this);

      SetAttribDC(get_handle1());     // Default to same as output
      return TRUE;*/
  /* }

   HDC graphics::Detach()
   {

      if(m_hdc == NULL)
         return NULL;

      if(m_prendertarget == NULL)
         return NULL;

      delete m_prendertarget;
      m_prendertarget = NULL;

      HDC hdc = m_hdc;
      m_hdc = NULL;

      return hdc;

   }
   */
   bool graphics::DeleteDC()
   {

      return destroy();

     /* if(get_handle() == NULL)
         return FALSE;

      return ::DeleteDC(Detach()) != FALSE;*/
   }

   /*

   void graphics::SetAttribDC(HDC hDC)  // Set the Attribute DC
   {
      //      set_handle2(hDC);
   }

   void graphics::SetOutputDC(HDC hDC)  // Set the Output DC
   {
#ifdef DEBUG
      /*      hdc_map* pMap = afxMapHDC();
      if (pMap != NULL && pMap->lookup_permanent(get_handle1()) == this)
      {
      TRACE(::aura::trace::category_AppMsg, 0, "Cannot Set Output hDC on Attached graphics.\n");
      ASSERT(FALSE);
      }*/
//#endif
      //    set_handle1(hDC);
/*   }

   void graphics::ReleaseAttribDC()     // Release the Attribute DC
   {
      //      set_handle2(NULL);
   }

   void graphics::ReleaseOutputDC()     // Release the Output DC
   {
#ifdef DEBUG
      /*      hdc_map* pMap = afxMapHDC();
      if (pMap != NULL && pMap->lookup_permanent(get_handle1()) == this)
      {
      TRACE(::aura::trace::category_AppMsg, 0, "Cannot Release Output hDC on Attached graphics.\n");
      ASSERT(FALSE);
      }*/
//#endif
  //    //set_handle1(NULL);
//   }

   /////////////////////////////////////////////////////////////////////////////
   // Out-of-line routines

   int graphics::StartDoc(const char * lpszDocName)
   {
      
      throw todo(get_app());

      //DOCINFO di;
      //memset(&di, 0, sizeof(DOCINFO));
      //di.cbSize = sizeof(DOCINFO);
      //di.lpszDocName = lpszDocName;
      //return StartDoc(&di);
   }

   int graphics::SaveDC()
   {

      m_prendertarget->GetTransform(&m_state.m_m);

      ::count iSaveDC = m_statea.get_size();

      m_statea.add(m_state);

      return (int) iSaveDC;
      //throw todo(get_app());

      //int nRetVal = 0;
      //if(get_handle2() != NULL)
      //   nRetVal = ::SaveDC(get_handle2());
      //if(get_handle1() != NULL && get_handle1() != get_handle2() && ::SaveDC(get_handle1()) != 0)
      //   nRetVal = -1;   // -1 is the only valid restore value for complex DCs
      //return nRetVal;
   }

   bool graphics::RestoreDC(int nSavedDC)
   {

      if (nSavedDC < 0)
      {

         return false;

      }

      if (nSavedDC >= m_statea.get_count())
      {

         return false;

      }

      ::count c;

      index iStart = 0;

      for (;
         iStart < m_state.m_sparegionClip.get_size()
         && iStart < m_statea[nSavedDC].m_sparegionClip.get_size()
         && m_state.m_sparegionClip[iStart] == m_statea[nSavedDC].m_sparegionClip[iStart]; )
      {
         iStart++;
      }

      for (index i = iStart; i < m_state.m_sparegionClip.get_size(); i++)
      {

         m_prendertarget->PopLayer();

         m_state.m_sparegionClip.remove_last();

         m_state.m_maRegion.remove_last();

      }

      

      for (index i = iStart;  i < m_statea[nSavedDC].m_sparegionClip.get_size(); i++)
      {

         m_prendertarget->SetTransform(&m_statea[nSavedDC].m_maRegion[i]);

         ID2D1Geometry * pgeometry = (ID2D1Geometry *)(dynamic_cast < region * > (m_statea[nSavedDC].m_sparegionClip[i].m_p))->get_os_data();

         m_prendertarget->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), pgeometry), NULL);

         m_state.m_sparegionClip.add(m_statea[nSavedDC].m_sparegionClip[i]);

         m_state.m_maRegion.add(m_statea[nSavedDC].m_maRegion[i]);

      }

      m_prendertarget->SetTransform(&m_statea[nSavedDC].m_m);

      m_statea.set_size(nSavedDC);

      return true;
      //throw todo(get_app());

   //   bool bRetVal = TRUE;
   //   if(get_handle1() != NULL && get_handle1() != get_handle2())
   //      bRetVal = ::RestoreDC(get_handle1(), nSavedDC) != FALSE;
   //   if(get_handle2() != NULL)
   //      bRetVal = (bRetVal && ::RestoreDC(get_handle2(), nSavedDC) != FALSE);
   //   return bRetVal;
   //
   }


   ::draw2d::object* graphics::SelectStockObject(int nIndex)
   {

      return NULL;

   }

   bool graphics::SelectFont(::draw2d::font * pfont)
   {
      // SIOOT - Should implemennt one of them
      // OASOWO - otherwise a stack overflow will occur
      // BTAIOM - because these are interface only methods

      m_spfont = pfont;

      //if((::draw2d_direct2d::graphics *) m_spfont->get_os_data_ex(::draw2d_direct2d::object::data_graphics) != this)
      //{
        // m_spfont->m_bUpdated = false;
      //}

      return true;

   }


   ::draw2d::pen* graphics::SelectObject(::draw2d::pen* pPen)
   {

      m_sppen = pPen;

      if((::draw2d_direct2d::graphics *) m_sppen->get_os_data_ex(::draw2d_direct2d::object::data_graphics) != this)
      {
         m_sppen->m_bUpdated = false;
      }

      return m_sppen;

   }


   ::draw2d::brush* graphics::SelectObject(::draw2d::brush* pBrush)
   {

      if(pBrush == NULL)
         return NULL;

      m_spbrush = pBrush;

      if((::draw2d_direct2d::graphics *) m_spbrush->get_os_data_ex(::draw2d_direct2d::object::data_graphics) != this)
      {
         m_spbrush->m_bUpdated = false;
      }

      return m_spbrush;

   }




   ::draw2d::font* graphics::SelectObject(::draw2d::font* pfont)
   {

      if(!select_font(pfont))
         return NULL;

      return m_spfont;

   }

   int graphics::SelectObject(::draw2d::region* pRgn)
   {
      
      throw todo(get_app());

      //int nRetVal = GDI_ERROR;
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //   nRetVal = (int)(int_ptr)::SelectObject(get_handle1(), (HGDIOBJ) pRgn->get_os_data());
      //if(get_handle2() != NULL)
      //   nRetVal = (int)(int_ptr)::SelectObject(get_handle2(), (HGDIOBJ) pRgn->get_os_data());
      //return nRetVal;
   }

   ::draw2d::palette* graphics::SelectPalette(::draw2d::palette* pPalette, bool bForceBackground)
   {
      return NULL;
   }

/*   COLORREF graphics::SetBkColor(COLORREF crColor)
   {

      
      return 0;
      //throw todo(get_app());



      //COLORREF crRetVal = CLR_INVALID;
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //   crRetVal = ::SetBkColor(get_handle1(), crColor);
      //if(get_handle2() != NULL)
      //   crRetVal = ::SetBkColor(get_handle2(), crColor);
      //return crRetVal;
   }*/

   //int graphics::SetBkMode(int nBkMode)
   //{

     // return 0;

      /*      int nRetVal = 0;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
      nRetVal = ::SetBkMode(get_handle1(), nBkMode);
      if(get_handle2() != NULL)
      nRetVal = ::SetBkMode(get_handle2(), nBkMode);
      return nRetVal;*/
   //}

   int graphics::SetPolyFillMode(int nPolyFillMode)
   {
      
      throw todo(get_app());

      //int nRetVal = 0;
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //   nRetVal = ::SetPolyFillMode(get_handle1(), nPolyFillMode);
      //if(get_handle2() != NULL)
      //   nRetVal = ::SetPolyFillMode(get_handle2(), nPolyFillMode);
      //return nRetVal;
   }

   int graphics::SetROP2(int nDrawMode)
   {
      
      throw todo(get_app());

      //int nRetVal = 0;
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //   nRetVal = ::SetROP2(get_handle1(), nDrawMode);
      //if(get_handle2() != NULL)
      //   nRetVal = ::SetROP2(get_handle2(), nDrawMode);
      //return nRetVal;
   }

   int graphics::SetStretchBltMode(int nStretchMode)
   {
      if(nStretchMode == 0)
      {
         m_bitmapinterpolationmode     = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR;
         m_interpolationmode           = D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR;;
      }
      else if(nStretchMode == HALFTONE)
      {
         m_bitmapinterpolationmode     = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;
         m_interpolationmode           = D2D1_INTERPOLATION_MODE_HIGH_QUALITY_CUBIC;
      }
      else
      {
         m_bitmapinterpolationmode     = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;
         m_interpolationmode           = D2D1_INTERPOLATION_MODE_LINEAR;
      }
      return 1;
      /*int nRetVal = 0;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
      nRetVal = ::SetStretchBltMode(get_handle1(), nStretchMode);
      if(get_handle2() != NULL)
      nRetVal = ::SetStretchBltMode(get_handle2(), nStretchMode);
      return nRetVal;*/
   }


  // COLORREF graphics::SetTextColor(COLORREF crColor)
//   {
    //  return set_color(crColor);
      //COLORREF crRetVal = m_crColor;
      //m_crColor = crColor;
      /*      COLORREF crRetVal = CLR_INVALID;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
      crRetVal = ::SetTextColor(get_handle1(), crColor);
      if(get_handle2() != NULL)
      crRetVal = ::SetTextColor(get_handle2(), crColor);*/
      //return crRetVal;
   //}

   int graphics::SetGraphicsMode(int iMode)
   {
      
      throw todo(get_app());

      //int nRetVal = 0;
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //{
      //   nRetVal = ::SetGraphicsMode(get_handle1(), iMode);
      //}
      //if(get_handle2() != NULL)
      //{
      //   nRetVal = ::SetGraphicsMode(get_handle2(), iMode);
      //}
      //return nRetVal;
   }

   bool graphics::SetWorldTransform(const XFORM* pXform)
   {
      
      throw todo(get_app());

      //bool nRetVal = 0;
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //{
      //   nRetVal = ::SetWorldTransform(get_handle1(), pXform) != FALSE;
      //}
      //if(get_handle2() != NULL)
      //{
      //   nRetVal = ::SetWorldTransform(get_handle2(), pXform) != FALSE;
      //}
      //return nRetVal;
   }

   bool graphics::ModifyWorldTransform(const XFORM* pXform,uint32_t iMode)
   {
      
      throw todo(get_app());

      //bool nRetVal = 0;
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //{
      //   nRetVal = ::ModifyWorldTransform(get_handle1(), pXform, iMode) != FALSE;
      //}
      //if(get_handle2() != NULL)
      //{
      //   nRetVal = ::ModifyWorldTransform(get_handle2(), pXform, iMode) != FALSE;
      //}
      //return nRetVal;
   }

   int graphics::SetMapMode(int nMapMode)
   {
      return 0;
      //throw todo(get_app());

      //int nRetVal = 0;
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //   nRetVal = ::SetMapMode(get_handle1(), nMapMode);
      //if(get_handle2() != NULL)
      //   nRetVal = ::SetMapMode(get_handle2(), nMapMode);
      //return nRetVal;
   }

   point graphics::SetViewportOrg(int x, int y)
   {

      D2D1::Matrix3x2F m;

      m_prendertarget->GetTransform(&m);

      m._31 = (FLOAT)x;

      m._32 = (FLOAT)y;

      m_prendertarget->SetTransform(m);

      return point(x, y);

   }

   point graphics::OffsetViewportOrg(int nWidth, int nHeight)
   {

      point point = GetViewportOrg();

      point.offset(nWidth, nHeight);

      return SetViewportOrg(point.x, point.y);

   }

   size graphics::SetViewportExt(int x, int y)
   {
      
      throw todo(get_app());

      //size size(0, 0);
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //   ::SetViewportExtEx(get_handle1(), x, y, &size);
      //if(get_handle2() != NULL)
      //   ::SetViewportExtEx(get_handle2(), x, y, &size);
      //return size;
   }

   size graphics::ScaleViewportExt(int xNum, int xDenom, int yNum, int yDenom)
   {
      
      throw todo(get_app());

      //size size(0, 0);
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //   ::ScaleViewportExtEx(get_handle1(), xNum, xDenom, yNum, yDenom, &size);
      //if(get_handle2() != NULL)
      //   ::ScaleViewportExtEx(get_handle2(), xNum, xDenom, yNum, yDenom, &size);
      //return size;
   }

   point graphics::SetWindowOrg(int x, int y)
   {
      
      throw todo(get_app());

      //point point(0, 0);
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //   ::SetWindowOrgEx(get_handle1(), x, y, &point);
      //if(get_handle2() != NULL)
      //   ::SetWindowOrgEx(get_handle2(), x, y, &point);
      //return point;
   }

   point graphics::OffsetWindowOrg(int nWidth, int nHeight)
   {
      
      throw todo(get_app());

      //point point(0, 0);
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //   ::OffsetWindowOrgEx(get_handle1(), nWidth, nHeight, &point);
      //if(get_handle2() != NULL)
      //   ::OffsetWindowOrgEx(get_handle2(), nWidth, nHeight, &point);
      //return point;
   }

   size graphics::SetWindowExt(int x, int y)
   {
      
      throw todo(get_app());

      //size size(0, 0);
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //   ::SetWindowExtEx(get_handle1(), x, y, &size);
      //if(get_handle2() != NULL)
      //   ::SetWindowExtEx(get_handle2(), x, y, &size);
      //return size;
   }

   size graphics::ScaleWindowExt(int xNum, int xDenom, int yNum, int yDenom)
   {
      
      throw todo(get_app());

      //size size(0, 0);
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //   ::ScaleWindowExtEx(get_handle1(), xNum, xDenom, yNum, yDenom, &size);
      //if(get_handle2() != NULL)
      //   ::ScaleWindowExtEx(get_handle2(), xNum, xDenom, yNum, yDenom, &size);
      //return size;
   }

   int graphics::GetClipBox(LPRECT lpRect) const
   {
      return 0;
      //throw todo(get_app());

      //return ::GetClipBox(get_handle1(), lpRect);
   }

   int graphics::SelectClipRgn(::draw2d::region * pregion)
   {

      synch_lock sl(m_pmutex);

      if (pregion == NULL)
      {

         for (index i = 0; i < m_state.m_sparegionClip.get_size(); i++)
         {

            m_prendertarget->PopLayer();

         }

         m_state.m_sparegionClip.remove_all();

      }
      else
      {

         D2D1::Matrix3x2F m;

         m_prendertarget->GetTransform(&m);

         m_state.m_sparegionClip.add(pregion);

         m_state.m_maRegion.add(m);

         ID2D1Geometry * pgeometry = (ID2D1Geometry *) (dynamic_cast < region * > (pregion))->get_os_data();

         m_prendertarget->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(),pgeometry),NULL);

      }
      
      return 0;

      //throw todo(get_app());


      //if(pregion == NULL)
      //{
      //   m_prendertarget->ResetClip();
      //}
      //else
      //{
      //   m_prendertarget->SetClip((Gdiplus::Region *) pregion->get_os_data());
      //}

      //return 0;

      ///*      int nRetVal = ERROR;
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //nRetVal = ::SelectClipRgn(get_handle1(), pRgn == NULL ? NULL : (HRGN) pRgn->get_os_data());
      //if(get_handle2() != NULL)
      //nRetVal = ::SelectClipRgn(get_handle2(), pRgn == NULL ? NULL : (HRGN) pRgn->get_os_data());
      //return nRetVal;*/
   }

   int graphics::ExcludeClipRect(int x1, int y1, int x2, int y2)
   {
      
      throw todo(get_app());

      //int nRetVal = ERROR;
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //   nRetVal = ::ExcludeClipRect(get_handle1(), x1, y1, x2, y2);
      //if(get_handle2() != NULL)
      //   nRetVal = ::ExcludeClipRect(get_handle2(), x1, y1, x2, y2);
      //return nRetVal;
   }

   int graphics::ExcludeClipRect(const RECT & lpRect)
   {
      
      throw todo(get_app());

      //int nRetVal = ERROR;
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //   nRetVal = ::ExcludeClipRect(get_handle1(), lpRect.left, lpRect.top,
      //   lpRect.right, lpRect.bottom);
      //if(get_handle2() != NULL)
      //   nRetVal = ::ExcludeClipRect(get_handle2(), lpRect.left, lpRect.top,
      //   lpRect.right, lpRect.bottom);
      //return nRetVal;
   }

   int graphics::IntersectClipRect(int x1, int y1, int x2, int y2)
   {
      
      synch_lock sl(m_pmutex);

      {

         ::draw2d::region_sp pregion(allocer());

         pregion->create_rect(x1, y1, x2, y2);

         D2D1::Matrix3x2F m;

         m_prendertarget->GetTransform(&m);

         m_state.m_sparegionClip.add(pregion);

         m_state.m_maRegion.add(m);

         ID2D1Geometry * pgeometry = (ID2D1Geometry *)(dynamic_cast < region * > (pregion.m_p))->get_os_data();

         m_prendertarget->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), pgeometry), NULL);

      }

      return 0;

   }

   int graphics::IntersectClipRect(const RECT & lpRect)
   {
      
      return IntersectClipRect(lpRect.left, lpRect.top, lpRect.right, lpRect.bottom);
      
   }

   int graphics::OffsetClipRgn(int x, int y)
   {
      
      throw todo(get_app());

      //int nRetVal = ERROR;
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //   nRetVal = ::OffsetClipRgn(get_handle1(), x, y);
      //if(get_handle2() != NULL)
      //   nRetVal = ::OffsetClipRgn(get_handle2(), x, y);
      //return nRetVal;
   }

   int graphics::OffsetClipRgn(SIZE size)
   {
      
      throw todo(get_app());

      //int nRetVal = ERROR;
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //   nRetVal = ::OffsetClipRgn(get_handle1(), size.cx, size.cy);
      //if(get_handle2() != NULL)
      //   nRetVal = ::OffsetClipRgn(get_handle2(), size.cx, size.cy);
      //return nRetVal;
   }

   /*point graphics::MoveTo(int x, int y)
   {
   point point(0, 0);
   if(get_handle1() != NULL && get_handle1() != get_handle2())
   ::MoveToEx(get_handle1(), x, y, &point);
   if(get_handle2() != NULL)
   ::MoveToEx(get_handle2(), x, y, &point);
   return point;
   }*/

   UINT graphics::SetTextAlign(UINT nFlags)
   {
      
      throw todo(get_app());

      //UINT nRetVal = GDI_ERROR;
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //   ::SetTextAlign(get_handle1(), nFlags);
      //if(get_handle2() != NULL)
      //   nRetVal = ::SetTextAlign(get_handle2(), nFlags);
      //return nRetVal;
   }

   int graphics::SetTextJustification(int nBreakExtra, int nBreakCount)
   {
      
      throw todo(get_app());

      //int nRetVal = 0;
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //   nRetVal = ::SetTextJustification(get_handle1(), nBreakExtra, nBreakCount);
      //if(get_handle2() != NULL)
      //   nRetVal = ::SetTextJustification(get_handle2(), nBreakExtra, nBreakCount);
      //return nRetVal;
   }

   int graphics::SetTextCharacterExtra(int nCharExtra)
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);
      //int nRetVal = 0x8000000;
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //   nRetVal = ::SetTextCharacterExtra(get_handle1(), nCharExtra);
      //if(get_handle2() != NULL)
      //   nRetVal = ::SetTextCharacterExtra(get_handle2(), nCharExtra);
      //return nRetVal;
   }

   uint32_t graphics::SetMapperFlags(uint32_t dwFlag)
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);
      //uint32_t dwRetVal = GDI_ERROR;
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //   dwRetVal = ::SetMapperFlags(get_handle1(), dwFlag);
      //if(get_handle2() != NULL)
      //   dwRetVal = ::SetMapperFlags(get_handle2(), dwFlag);
      //return dwRetVal;
   }

   typedef uint32_t (CALLBACK* __GDIGETLAYOUTPROC)(HDC);
   typedef uint32_t (CALLBACK* __GDISETLAYOUTPROC)(HDC, uint32_t);

   uint32_t graphics::GetLayout() const
   {
      
      throw todo(get_app());

      //HINSTANCE hInst = ::GetModuleHandleA("GDI32.DLL");
      //ASSERT(hInst != NULL);
      //uint32_t dwGetLayout = LAYOUT_LTR;
      //__GDIGETLAYOUTPROC pfn;
      //pfn = (__GDIGETLAYOUTPROC) GetProcAddress(hInst, "GetLayout");
      //// if they API is available, just call it. If it is not
      //// available, indicate an error.
      //if (pfn != NULL)
      //   dwGetLayout = (*pfn)(get_handle1());
      //else
      //{
      //   dwGetLayout = GDI_ERROR;
      //   SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
      //}
      //return dwGetLayout;
   }

   uint32_t graphics::SetLayout(uint32_t dwSetLayout)
   {
      
      throw todo(get_app());

      //HINSTANCE hInst = ::GetModuleHandleA("GDI32.DLL");
      //ASSERT(hInst != NULL);
      //uint32_t dwGetLayout = LAYOUT_LTR;
      //__GDISETLAYOUTPROC pfn;
      //pfn = (__GDISETLAYOUTPROC) GetProcAddress(hInst, "SetLayout");
      //// If the API is availalbe, just call it. If it's not available,
      //// setting anything other than LAYOUT_LTR is an error.
      //if (pfn != NULL)
      //   dwGetLayout = (*pfn)(get_handle1(), dwSetLayout);
      //else if (dwSetLayout != LAYOUT_LTR)
      //{
      //   dwGetLayout = GDI_ERROR;
      //   SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
      //}
      //return dwGetLayout;
   }
   /*
   void window::ScreenToClient(LPRECT lpRect)
   {
   ASSERT(::IsWindow(get_handle1()));
   ::ScreenToClient(get_handle1(), (LPPOINT)lpRect);
   ::ScreenToClient(get_handle1(), ((LPPOINT)lpRect)+1);
   if (GetExStyle() & WS_EX_LAYOUTRTL)
   rect::swap_left_right(lpRect);
   }

   void window::ClientToScreen(LPRECT lpRect)
   {
   ASSERT(::IsWindow(get_handle1()));
   ::ClientToScreen(get_handle1(), (LPPOINT)lpRect);
   ::ClientToScreen(get_handle1(), ((LPPOINT)lpRect)+1);
   if (GetExStyle() & WS_EX_LAYOUTRTL)
   rect::swap_left_right(lpRect);
   }*/


   /////////////////////////////////////////////////////////////////////////////
   // Advanced Win32 GDI functions

   bool graphics::ArcTo(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);
      //bool bResult = ::ArcTo(get_handle1(), x1, y1, x2, y2, x3, y3, x4, y4) != FALSE;
      //if (get_handle1() != get_handle2())
      //{
      //   point pt;
      //   VERIFY(::GetCurrentPositionEx(get_handle1(), &pt));
      //   VERIFY(::MoveToEx(get_handle2(), pt.x, pt.y, NULL));
      //}
      //return bResult;
   }

   int graphics::SetArcDirection(int nArcDirection)
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);
      //int nResult = 0;
      //if (get_handle1() != get_handle2())
      //   nResult = ::SetArcDirection(get_handle1(), nArcDirection);
      //if (get_handle2() != NULL)
      //   nResult = ::SetArcDirection(get_handle2(), nArcDirection);
      //return nResult;
   }

   bool graphics::PolyDraw(const POINT* lpPoints, const BYTE* lpTypes, count nCount)
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);
      //bool bResult = ::PolyDraw(get_handle1(), lpPoints, lpTypes, nCount) != FALSE;
      //if (get_handle1() != get_handle2())
      //{
      //   point pt;
      //   VERIFY(::GetCurrentPositionEx(get_handle1(), &pt));
      //   VERIFY(::MoveToEx(get_handle2(), pt.x, pt.y, NULL));
      //}
      //return bResult;
   }

   bool graphics::PolylineTo(const POINT* lpPoints, count nCount)
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);
      //bool bResult = ::PolylineTo(get_handle1(), lpPoints, nCount) != FALSE;
      //if (get_handle1() != get_handle2())
      //{
      //   point pt;
      //   VERIFY(::GetCurrentPositionEx(get_handle1(), &pt));
      //   VERIFY(::MoveToEx(get_handle2(), pt.x, pt.y, NULL));
      //}
      //return bResult;
   }

#ifdef WINDOWSEX
/*
   bool graphics::SetColorAdjustment(const COLORADJUSTMENT* lpColorAdjust)
   {
      ASSERT(get_handle1() != NULL);
      bool bResult = FALSE;
      if (get_handle1() != get_handle2())
         bResult = ::SetColorAdjustment(get_handle1(), lpColorAdjust) != FALSE;
      if (get_handle2() != NULL)
         bResult = ::SetColorAdjustment(get_handle2(), lpColorAdjust) != FALSE;
      return bResult;
   }
   */
#endif

   bool graphics::PolyBezierTo(const POINT* lpPoints, count nCount)
   {
            
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);
      //bool bResult = ::PolyBezierTo(get_handle1(), lpPoints, nCount) != FALSE;
      //if (get_handle1() != get_handle2())
      //{
      //   point pt;
      //   VERIFY(::GetCurrentPositionEx(get_handle1(), &pt));
      //   VERIFY(::MoveToEx(get_handle2(), pt.x, pt.y, NULL));
      //}
      //return bResult;
   }

   bool graphics::SelectClipPath(int nMode)
   {
      
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);

      //// output DC always holds the current path
      //if (!::SelectClipPath(get_handle1(), nMode))
      //   return FALSE;

      //// transfer clipping region into the attribute DC
      //bool bResult = TRUE;
      //if (get_handle1() != get_handle2())
      //{
      //   HRGN hRgn = ::CreateRectRgn(0, 0, 0, 0);
      //   if (::GetClipRgn(get_handle1(), hRgn) < 0 || !::SelectClipRgn(get_handle2(), hRgn))
      //   {
      //      TRACE(::aura::trace::category_AppMsg, 0, "Error: unable to transfer clip region in graphics::SelectClipPath!\n");
      //      bResult = FALSE;
      //   }
      //   ::DeleteObject(hRgn);
      //}
      //return bResult;
   }

   int graphics::SelectClipRgn(::draw2d::region* pRgn, int nMode)
   {

      return 0;
      
      //throw todo(get_app());

      //ASSERT(get_handle1() != NULL);
      //int nRetVal = ERROR;
      //if (get_handle1() != get_handle2())
      //   nRetVal = ::ExtSelectClipRgn(get_handle1(), (HRGN)pRgn->get_os_data(), nMode);
      //if (get_handle2() != NULL)
      //   nRetVal = ::ExtSelectClipRgn(get_handle2(), (HRGN)pRgn->get_os_data(), nMode);
      //return nRetVal;
   }

   /////////////////////////////////////////////////////////////////////////////
   // Special handling for metafile playback
#ifdef WINDOWSEX
   int CALLBACK __enum_meta_file_procedure(HDC hDC,
      HANDLETABLE* pHandleTable, METARECORD* pMetaRec, int nHandles, LPARAM lParam)
   {
      ::draw2d::graphics * pgraphics = (::draw2d::graphics *)lParam;
      ASSERT_VALID(pgraphics);

      switch (pMetaRec->rdFunction)
      {
         // these records have effects different for each graphics derived class
      case META_SETMAPMODE:
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->SetMapMode((int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SETWINDOWEXT:
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->SetWindowExt(
            (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SETWINDOWORG:
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->SetWindowOrg(
            (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SETVIEWPORTEXT:
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->SetViewportExt(
            (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SETVIEWPORTORG:
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->SetViewportOrg(
            (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SCALEWINDOWEXT:
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->ScaleWindowExt(
            (int)(short)pMetaRec->rdParm[3], (int)(short)pMetaRec->rdParm[2],
            (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SCALEVIEWPORTEXT:
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->ScaleViewportExt(
            (int)(short)pMetaRec->rdParm[3], (int)(short)pMetaRec->rdParm[2],
            (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_OFFSETVIEWPORTORG:
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->OffsetViewportOrg(
            (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SAVEDC:
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->SaveDC();
         break;
      case META_RESTOREDC:
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->RestoreDC((int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SETBKCOLOR:
         {
            ::draw2d::brush_sp brush(pgraphics->allocer(), *(UNALIGNED COLORREF*)&pMetaRec->rdParm[0]);
            (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->SelectObject(brush);
         }
         break;
      case META_SETTEXTCOLOR:
         {
            ::draw2d::brush_sp brush(pgraphics->allocer(), *(UNALIGNED COLORREF*)&pMetaRec->rdParm[0]);
            (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->SelectObject(brush);
         }
         break;

         // need to watch out for SelectObject(HFONT), for custom font mapping
      case META_SELECTOBJECT:
         {
            HGDIOBJ hObject = pHandleTable->objectHandle[pMetaRec->rdParm[0]];
            UINT nObjType = GetObjectType(hObject);
            if (nObjType == 0)
            {
               // object type is unknown, determine if it is a font
               HFONT hStockFont = (HFONT)::GetStockObject(SYSTEM_FONT);
               HFONT hFontOld = (HFONT)::SelectObject((dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->get_handle1(), hStockFont);
               HGDIOBJ hObjOld = ::SelectObject((dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->get_handle1(), hObject);
               if (hObjOld == hStockFont)
               {
                  // got the stock object back, so must be selecting a font
                  throw not_implemented(pgraphics->get_app());
                  //                  (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->SelectObject(::draw2d_direct2d::font::from_handle_dup(pgraphics->get_app(), (HFONT)hObject));
                  break;  // don't play the default record
               }
               else
               {
                  // didn't get the stock object back, so restore everything
                  ::SelectObject((dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->get_handle1(), hFontOld);
                  ::SelectObject((dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->get_handle1(), hObjOld);
               }
               // and fall through to PlayMetaFileRecord...
            }
            else if (nObjType == OBJ_FONT)
            {
               // play back as graphics::SelectObject(::draw2d::font*)
               //               (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->SelectObject(::draw2d_direct2d::font::from_handle_dup(pgraphics->get_app(), (HFONT)hObject));
               throw not_implemented(pgraphics->get_app());
               break;  // don't play the default record
            }
         }
         // fall through...

      default:
         ::PlayMetaFileRecord(hDC, pHandleTable, pMetaRec, nHandles);
         break;
      }

      return 1;
   }
#endif


   bool graphics::PlayMetaFile(HMETAFILE hMF)
   {
      
      throw todo(get_app());

      //if (::GetDeviceCaps(get_handle1(), TECHNOLOGY) == DT_METAFILE)
      //{
      //   // playing metafile in metafile, just use core windows API
      //   return ::PlayMetaFile(get_handle1(), hMF) != FALSE;
      //}

      //// for special playback, lParam == pgraphics
      //return ::EnumMetaFile(get_handle1(), hMF, __enum_meta_file_procedure, (LPARAM)this) != FALSE;
   }

   /////////////////////////////////////////////////////////////////////////////
   // Coordinate transforms

   void graphics::LPtoDP(LPSIZE lpSize) const
   {
      ASSERT(__is_valid_address(lpSize, sizeof(SIZE)));

      size sizeWinExt = GetWindowExt();
      size sizeVpExt = GetViewportExt();
      lpSize->cx = MulDiv(lpSize->cx, abs(sizeVpExt.cx), abs(sizeWinExt.cx));
      lpSize->cy = MulDiv(lpSize->cy, abs(sizeVpExt.cy), abs(sizeWinExt.cy));
   }

   void graphics::DPtoLP(LPSIZE lpSize) const
   {
      ASSERT(__is_valid_address(lpSize, sizeof(SIZE)));

      size sizeWinExt = GetWindowExt();
      size sizeVpExt = GetViewportExt();
      lpSize->cx = MulDiv(lpSize->cx, abs(sizeWinExt.cx), abs(sizeVpExt.cx));
      lpSize->cy = MulDiv(lpSize->cy, abs(sizeWinExt.cy), abs(sizeVpExt.cy));
   }



   int graphics::draw_text(const char * lpszString, strsize nCount, const RECT & lpRect, UINT nFormat)
   { 

      return draw_text(string(lpszString, nCount), lpRect, nFormat);

   }

   int graphics::draw_text(const string & str,const RECT & lpRect,UINT nFormat)
   { 

      if(str.is_empty())
         return TRUE;

      try
      {

         if(m_prendertarget == NULL)
            return FALSE;

         switch(m_etextrendering)
         {
         case ::draw2d::text_rendering_anti_alias:
            if(m_pdevicecontext) m_pdevicecontext->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_SOURCE_OVER);
            m_prendertarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
            break;
         case ::draw2d::text_rendering_anti_alias_grid_fit:
            if(m_pdevicecontext) m_pdevicecontext->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_SOURCE_OVER);
            m_prendertarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
            break;
         case ::draw2d::text_rendering_single_bit_per_pixel:
            if(m_pdevicecontext) m_pdevicecontext->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_SOURCE_OVER);
            m_prendertarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
            break;
         case ::draw2d::text_rendering_clear_type_grid_fit:
            if(m_pdevicecontext) m_pdevicecontext->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_SOURCE_OVER);
            m_prendertarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);
            break;
         }

      }
      catch(...)
      {
      }


      if (m_spfont.is_null())
         return false;

      if (get_os_font(m_spfont) == NULL)
         return false;

      if (nFormat & DT_RIGHT)
      {

         get_os_font(m_spfont)->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);

      }
      else if (nFormat & DT_CENTER)
      {

         get_os_font(m_spfont)->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

      }
      else
      {

         get_os_font(m_spfont)->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

      }

      if (nFormat & DT_BOTTOM)
      {

         get_os_font(m_spfont)->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);

      }
      else if (nFormat & DT_VCENTER)
      {

         get_os_font(m_spfont)->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

      }
      else
      {

         get_os_font(m_spfont)->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

      }

      D2D1::Matrix3x2F m;

      m_prendertarget->GetTransform(&m);

      D2D1::Matrix3x2F mOriginal(m);

      D2D1_RECT_F rectf = D2D1::RectF((FLOAT) 0, (FLOAT) 0, (FLOAT) lpRect.right, (FLOAT) lpRect.bottom);

      wstring wstr(str);

      m = m * D2D1::Matrix3x2F::Translation((FLOAT)lpRect.left, (FLOAT)lpRect.top);

      m = m * D2D1::Matrix3x2F::Scale((FLOAT)m_spfont->m_dFontWidth, 1.f);

      m_prendertarget->SetTransform(&m);

      IDWriteTextFormat * pformat = get_os_font(m_spfont);

      ID2D1Brush * pbrush = get_os_brush(m_spbrush);

      m_prendertarget->DrawText(wstr, (UINT32) wstr.get_length(), pformat, &rectf, pbrush);

      m_prendertarget->SetTransform(&mOriginal);
      
      return 1;

   }

#ifdef WINDOWSEX

   /*
   int graphics::draw_text_ex(LPTSTR lpszString, int nCount, LPRECT lpRect, UINT nFormat, LPDRAWTEXTPARAMS lpDTParams)
   { 
      if(get_handle1() == NULL)
         return -1;
      // these flags would modify the string
      ASSERT((nFormat & (DT_END_ELLIPSIS | DT_MODIFYSTRING)) != (DT_END_ELLIPSIS | DT_MODIFYSTRING));
      ASSERT((nFormat & (DT_PATH_ELLIPSIS | DT_MODIFYSTRING)) != (DT_PATH_ELLIPSIS | DT_MODIFYSTRING));
      wstring wstr = ::str::international::utf8_to_unicode(string(lpszString, nCount));
      return ::DrawTextExW(get_handle1(), const_cast<unichar *>((const unichar *)wstr), (int)wcslen(wstr), lpRect, nFormat, lpDTParams); 
   }

   int graphics::draw_text_ex(const string & str, LPRECT lpRect, UINT nFormat, LPDRAWTEXTPARAMS lpDTParams)
   { 
      ASSERT(get_handle1() != NULL);
      // these flags would modify the string
      ASSERT((nFormat & (DT_END_ELLIPSIS | DT_MODIFYSTRING)) != (DT_END_ELLIPSIS | DT_MODIFYSTRING));
      ASSERT((nFormat & (DT_PATH_ELLIPSIS | DT_MODIFYSTRING)) != (DT_PATH_ELLIPSIS | DT_MODIFYSTRING));
      wstring wstr = ::str::international::utf8_to_unicode(str);
      return ::DrawTextExW(get_handle1(), const_cast<unichar *>((const unichar *)wstr), (int)wcslen(wstr), lpRect, nFormat, lpDTParams); 
   }
   */

#endif


   sized graphics::GetTextExtent(const char * lpszString, strsize nCount, strsize iIndex) const
   {

      sized sz;

      bool bOk = GetTextExtent(sz, lpszString, nCount, iIndex);

      if (!bOk)
      {

         return ::size(0, 0);

      }
      else
      {

         return ::size((int32_t) sz.cx, (int32_t) sz.cy);

      }

   }

   sized graphics::GetTextExtent(const char * lpszString, strsize nCount) const
   {

      return GetTextExtent(lpszString, (int32_t) nCount, (int32_t) nCount);

   }


   sized graphics::GetTextExtent(const string & str) const
   {

      return GetTextExtent(str, str.get_length());
   
   }

   size graphics::GetOutputTextExtent(const char * lpszString, strsize nCount) const
   {
            
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);
      //SIZE size;
      //string str(lpszString, nCount);
      //wstring wstr = ::str::international::utf8_to_unicode(str);
      //VERIFY(::GetTextExtentPoint32W(get_handle1(), wstr, (int)wstr.get_length(), &size));
      //return size;
   }

   size graphics::GetOutputTextExtent(const string & str) const
   {
            
      throw todo(get_app());

      //ASSERT(get_handle1() != NULL);
      //SIZE size;
      //wstring wstr = ::str::international::utf8_to_unicode(str);
      //VERIFY(::GetTextExtentPoint32W(get_handle1(), wstr, (int)wstr.get_length(), &size));
      //return size;
   }

   bool graphics::GetTextExtent(sized & size, const char * lpszString, strsize nCount, strsize iIndex) const
   {

      string str;

      if (nCount < 0)
      {
       
         nCount = strlen(lpszString);

      }

      if (iIndex < 0)
      {

         iIndex = nCount;

      }

      if (iIndex > nCount)
      {

         iIndex = nCount;

      }

      if (iIndex < nCount)
      {

         str = string(lpszString, iIndex + ::str::get_utf8_char_length(&lpszString[iIndex]));

      }
      else
      {

         str = string(lpszString, nCount);

      }

      if (m_spfont.is_null())
      {

         return false;

      }

      if (get_os_font(m_spfont) == NULL)
      {

         return false;

      }

      wstring wstr(str);

      IDWriteTextLayout * playout1 = NULL;

      HRESULT hr;

      if (iIndex < nCount)
      {
         
         wstring wstrChar = str.Mid(iIndex, ::str::get_utf8_char_length(&lpszString[iIndex]));
         
         hr = TlsGetWriteFactory()->CreateTextLayout(
            wstrChar,                // The string to be laid out and formatted.
            (UINT32)1,   // The length of the string.
            get_os_font(m_spfont),    // The text format to apply to the string (contains font information, etc).
            1024.f * 1024.f,               // The width of the on_layout box.
            1024.f * 1024.f,        // The height of the on_layout box.
            &playout1  // The IDWriteTextLayout interface pointer.
         );

      }

      IDWriteTextLayout * playout = NULL;

      hr = TlsGetWriteFactory()->CreateTextLayout(
         wstr,                // The string to be laid out and formatted.
         (UINT32) wstr.get_length(),   // The length of the string.
         get_os_font(m_spfont),    // The text format to apply to the string (contains font information, etc).
         1024.f * 1024.f,               // The width of the on_layout box.
         1024.f * 1024.f,        // The height of the on_layout box.
         &playout  // The IDWriteTextLayout interface pointer.
         );

      if(playout == NULL || FAILED(hr))
      {

         size.cx = 0;

         size.cy = 0;
         return false;

      }
      else if (playout1 != NULL)
      {
         DWRITE_TEXT_METRICS m;

         playout->GetMetrics(&m);

         DWRITE_TEXT_METRICS m1;

         playout1->GetMetrics(&m1);

         size.cx = (LONG)((m.width - m1.width)  * m_spfont->m_dFontWidth);

         size.cy = (LONG)m.height;

      }
      else
      {
      
         DWRITE_TEXT_METRICS m;
      
         playout->GetMetrics(&m);

         size.cx = (LONG) (m.width  * m_spfont->m_dFontWidth);

         size.cy = (LONG) m.height;

      }

      return true;

   }

   bool graphics::GetTextExtent(sized & size, const char * lpszString, strsize nCount) const
   {

      return GetTextExtent(size, lpszString, nCount, 0);


   }

   bool graphics::GetTextExtent(sized & size, const string & str) const
   {

      return GetTextExtent(size, str, str.get_length());

   }










   // IMPLEMENT_DYNAMIC(resource_exception, ::exception::base)
   //resource_exception _simpleResourceException(FALSE, __IDS_RESOURCE_EXCEPTION);

   // IMPLEMENT_DYNAMIC(user_exception, ::exception::base)
   //user_exception _simpleUserException(FALSE, __IDS_USER_EXCEPTION);

   // IMPLEMENT_DYNCREATE(graphics, object)
   // IMPLEMENT_DYNAMIC(CClientDC, graphics)
   // IMPLEMENT_DYNAMIC(CWindowDC, graphics)
   // IMPLEMENT_DYNAMIC(CPaintDC, graphics)
   // IMPLEMENT_DYNCREATE(::draw2d::object, object)

   // IMPLEMENT_DYNAMIC(pen, ::draw2d::object)
   // IMPLEMENT_DYNAMIC(::draw2d::brush, ::draw2d::object)
   // IMPLEMENT_DYNAMIC(::draw2d::font, ::draw2d::object)
   // IMPLEMENT_DYNAMIC(::draw2d::bitmap, ::draw2d::object)
   // IMPLEMENT_DYNAMIC(::draw2d::palette, ::draw2d::object)
   // IMPLEMENT_DYNAMIC(::draw2d::region, ::draw2d::object)


} // namespace draw2d_direct2d






namespace draw2d_direct2d
{

   void graphics::FillSolidRect(const RECT & lpRect, COLORREF clr)
   {

      //g.SetCompositingMode(Gdiplus::CompositingModeSourceCopy);
      //g().SetCompositingMode(Gdiplus::CompositingModeSourceOver);
      //g().SetCompositingQuality(Gdiplus::CompositingQualityGammaCorrected);

      FillSolidRect(lpRect.left, lpRect.top, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, clr);
      //::SetBkColor(get_handle1(), clr);
      //::ExtTextOut(get_handle1(), 0, 0, ETO_OPAQUE, lpRect, NULL, 0, NULL);
   }

   void graphics::FillSolidRect(int x, int y, int cx, int cy, COLORREF clr)
   {
      //g.SetCompositingMode(Gdiplus::CompositingModeSourceOver);
      //g().SetCompositingMode(Gdiplus::CompositingModeSourceOver);
      //g().SetCompositingQuality(Gdiplus::CompositingQualityGammaCorrected);

      if(m_prendertarget == NULL)
         return;

      ::draw2d::brush_sp brPrevious(m_spbrush);

      ::draw2d::brush_sp br;

      if(brPrevious.is_null() || brPrevious->m_powner != this)
      {

         br.alloc(allocer());

         br->m_powner = this;

         br->create_solid(clr);

         SelectObject(br);

      }
      else
      {

         br = brPrevious;

         br->create_solid(clr);

         SelectObject(br);

      }

      D2D1_RECT_F r;

      r.left      = (FLOAT) x;
      r.top       = (FLOAT) y;
      r.right     = (FLOAT) x + cx;
      r.bottom    = (FLOAT) y + cy;

      m_prendertarget->FillRectangle(r, get_os_brush(m_spbrush));


   }


   bool graphics::text_out(int x, int y, const char * lpszString, strsize nCount)
   {

      return ::draw2d::graphics::text_out((double)x, (double)y, lpszString, nCount);

   }


   bool graphics::TextOutRaw(double x, double y, const char * lpszString, strsize nCount)
   {

      //if (::draw2d::graphics::text_out(x, y, lpszString, nCount))
      //   return true;

      try
      {

         if(m_prendertarget == NULL)
            return FALSE;

         switch(m_etextrendering)
         {
         case ::draw2d::text_rendering_anti_alias:
            if(m_pdevicecontext) m_pdevicecontext->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_SOURCE_OVER);
            m_prendertarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
            break;
         case ::draw2d::text_rendering_anti_alias_grid_fit:
            if(m_pdevicecontext) m_pdevicecontext->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_SOURCE_OVER);
            m_prendertarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
            break;
         case ::draw2d::text_rendering_single_bit_per_pixel:
            if(m_pdevicecontext) m_pdevicecontext->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_SOURCE_OVER);
            m_prendertarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
            break;
         case ::draw2d::text_rendering_clear_type_grid_fit:
            if(m_pdevicecontext) m_pdevicecontext->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_SOURCE_OVER);
            m_prendertarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);
            break;
         }

      }
      catch(...)
      {

      }

      if (m_spfont.is_null())
         return false;

      if (get_os_font(m_spfont) == NULL)
         return false;

      D2D1::Matrix3x2F m;

      m_prendertarget->GetTransform(&m);

      D2D1::Matrix3x2F mOriginal(m);

      //size sd = GetTextExtent(string(lpszString,nCount));

      //string strTest1 = string(lpszString, nCount);

      //strTest1.trim_left();

      //size sd1 = GetTextExtent(strTest1);

      D2D1_RECT_F rectf = D2D1::RectF((FLOAT) 0, (FLOAT)0, (FLOAT)(0 + 1024 * 1024) , (FLOAT)(0 + 1024 * 1024));

      HRESULT  hr = get_os_font(m_spfont)->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

      if(FAILED(hr))
      {
         trace_hr("text_out, SetTextAlignment",hr);
      }

      hr = get_os_font(m_spfont)->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

      if(FAILED(hr))
      {
         trace_hr("text_out, SetTextAlignment",hr);
      }

      DWRITE_TRIMMING trim;

      ZERO(trim);

      trim.granularity = DWRITE_TRIMMING_GRANULARITY_NONE;

      hr = get_os_font(m_spfont)->SetTrimming(&trim, NULL);

      if (FAILED(hr))
      {
         trace_hr("text_out, SetTextAlignment", hr);
      }

      hr = get_os_font(m_spfont)->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
      

      string str(lpszString, nCount);

      wstring wstr(str);

      

      m._11 *= (FLOAT) m_spfont->m_dFontWidth;
      m._31 += (FLOAT) x;
      m._32 += (FLOAT) y;

      m_prendertarget->SetTransform(&m);

      LPCWSTR lpcwsz = wstr;

      strsize uiLen = wstr.get_length();

      IDWriteTextFormat * pfont = get_os_font(m_spfont);

      ID2D1Brush * pbrush = get_os_brush(m_spbrush);

      if(lpcwsz != NULL && uiLen > 0)
      {

         m_prendertarget->DrawText(lpcwsz,(int) uiLen,pfont,&rectf,pbrush);

      }

      m_prendertarget->SetTransform(mOriginal);

      return true;

   }


   bool graphics::LineTo(double x, double y)
   {

      //      ::Gdiplus::Pen pen(::Gdiplus::Color(argb_get_a_value(m_crColor), argb_get_r_value(m_crColor), argb_get_g_value(m_crColor), argb_get_b_value(m_crColor)), m_dPenWidth);

      //m_prendertarget->DrawLine(direct2d_pen(), Gdiplus::Point((INT) m_x, (INT) m_y), Gdiplus::Point((INT) x,(INT) y));
      D2D1_POINT_2F p1;
      p1.x = (FLOAT) m_x;
      p1.y = (FLOAT) m_y;

      D2D1_POINT_2F p2;
      p2.x = (FLOAT) x;
      p2.y = (FLOAT) y;

      ID2D1Brush * pbrush =  get_os_pen_brush(m_sppen);

      if(pbrush == NULL)
         return false;


      m_prendertarget->DrawLine(p1, p2, pbrush, (FLOAT) (dynamic_cast < ::draw2d_direct2d::pen * > (m_sppen.m_p))->m_dWidth);

      m_x = x;
      m_y = y;

      return TRUE;

   }


   void graphics::set_alpha_mode(::draw2d::e_alpha_mode ealphamode)
   {

      try
      {

         if(m_prendertarget == NULL)
            return;

         ::draw2d::graphics::set_alpha_mode(ealphamode);
         if(m_ealphamode == ::draw2d::alpha_mode_blend)
         {
            if(m_pdevicecontext) m_pdevicecontext->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_SOURCE_OVER);
         }
         else if(m_ealphamode == ::draw2d::alpha_mode_set)
         {
            if(m_pdevicecontext) m_pdevicecontext->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_COPY);
         }

      }
      catch(...)
      {
         return;
      }

   }


   void graphics::set_text_rendering(::draw2d::e_text_rendering etextrendering)
   {
      m_etextrendering = etextrendering;

   }


   void * graphics::get_os_data() const
   {

      return (ID2D1DeviceContext *) m_pdevicecontext.Get();

   }

   
   void * graphics::get_os_data_ex(int i) const
   {

      if(i == data_device_context)
      {
         
         return (void *) m_pdevicecontext.Get();

      }
      else if(i == data_render_target)
      {

         return (void *) m_prendertarget.Get();

      }
      return NULL;
   }


   HDC graphics::get_handle() const
   {

      throw todo(get_app());

      //return m_hdc;
   }

   HDC graphics::get_handle1() const
   {
      return get_handle();
   }

   HDC graphics::get_handle2() const
   {
      return get_handle();
   }


   bool graphics::attach(void * pdata)
   {
   
      if(m_pdevicecontext != NULL)
      {

         m_pdevicecontext = nullptr;

      }

      if(m_prendertarget != NULL)
      {

         m_prendertarget = nullptr;

      }

      if(m_pbitmaprendertarget != NULL)
      {

         m_pbitmaprendertarget = nullptr;

      }

      m_pdevicecontext = (ID2D1DeviceContext *) pdata;

      HRESULT hr = m_pdevicecontext.As(&m_prendertarget);

      if(FAILED(hr))
      {
         m_pdevicecontext = nullptr;
         m_prendertarget = nullptr;
         return false;
      }

      hr = m_prendertarget.As(&m_pbitmaprendertarget);

      if(FAILED(hr))
      {
         m_pbitmaprendertarget = nullptr;
      }

      return true;

   }


   void * graphics::detach()
   {

      m_prendertarget = nullptr;

      m_pbitmaprendertarget = nullptr;

      return m_pdevicecontext.Detach();

   }

#ifdef WINDOWSEX

   bool graphics::Attach(HDC hdc)
   {

      HWND hwnd = ::WindowFromDC(hdc);

      if(hwnd == NULL)
         return false;

      ::rect rectClient;

      ::GetClientRect(hwnd, rectClient);

      // Create a DC render target.
      D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
         D2D1_RENDER_TARGET_TYPE_DEFAULT,
         D2D1::PixelFormat(
         DXGI_FORMAT_B8G8R8A8_UNORM,
         D2D1_ALPHA_MODE_IGNORE),
         0,
         0,
         D2D1_RENDER_TARGET_USAGE_NONE,
         D2D1_FEATURE_LEVEL_DEFAULT
         );

      HRESULT hr = GetD2D1Factory1()->CreateDCRenderTarget(&props,&m_pdcrendertarget);

      if(FAILED(hr))
         return false;

      hr = m_pdcrendertarget->BindDC(m_hdcAttach, rectClient);

      m_pdcrendertarget.As(&m_prendertarget);

      m_hdcAttach = hdc;



      return true;

   }

   HDC graphics::Detach()
   {
      if(m_hdcAttach == NULL)
         return NULL;

      //hr = m_pdcrendertarget->BindDC(m_hdcAttach,rectClient);

      HDC hdc = m_hdcAttach;

      m_hdcAttach = NULL;

      return hdc;
   }

#endif

   /*
   Gdiplus::Font * graphics::direct2d_font()
   {
      if(m_spfont.is_null())
      {
         m_spfont.create(get_app());
         m_spfont->operator=(m_fontxyz);
      }
      else if(!m_fontxyz.m_bUpdated)
      {
         m_fontxyz.m_bUpdated = true;
         m_spfont->operator=(m_fontxyz);
      }
      return (Gdiplus::Font *) m_spfont->get_os_data();      
   }

   Gdiplus::Brush * graphics::direct2d_brush()
   {
      if(m_spbrush.is_null())
      {
         m_spbrush.create(get_app());
         m_spbrush->operator=(m_brushxyz);
      }
      else if(!m_brushxyz.m_bUpdated)
      {
         m_brushxyz.m_bUpdated = true;
         m_spbrush->operator=(m_brushxyz);
      }
      return (Gdiplus::Brush *) m_spbrush->get_os_data();      
   }

   Gdiplus::Pen * graphics::direct2d_pen()
   {
      if(m_sppen.is_null())
      {
         m_sppen.create(get_app());
         m_sppen->operator=(m_penxyz);
      }
      else if(!m_penxyz.m_bUpdated)
      {
         m_penxyz.m_bUpdated = true;
         m_sppen->operator=(m_penxyz);
      }
      return (Gdiplus::Pen *) m_sppen->get_os_data();      
   }

   Gdiplus::FillMode graphics::direct2d_get_fill_mode()
   {
      return Gdiplus::FillModeWinding;
   }
   */

   bool graphics::blur(bool bExpand, double dRadius, const RECT & lpcrect)
   {

/*
   if(m_spbitmap.is_null() || m_spbitmap->get_os_data() == NULL)
         return false;

      Gdiplus::BlurParams myBlurParams;

      myBlurParams.expandEdge    = bExpand ? 1 : 0;
      myBlurParams.radius        = (float) dRadius;

      Gdiplus::Blur myBlur;
      myBlur.SetParameters(&myBlurParams);



      Gdiplus::Matrix m;
      m_prendertarget->GetTransform(&m);

      Gdiplus::PointF points[2];

      points[0].X    = (Gdiplus::REAL) lpcrect->left;
      points[0].Y    = (Gdiplus::REAL) lpcrect->top;
      points[1].X    = (Gdiplus::REAL) lpcrect->right;
      points[1].Y    = (Gdiplus::REAL) lpcrect->bottom;

      m.TransformPoints(points, 2);

      //Gdiplus::RectF rectf(points[0].X, points[0].Y, points[1].X - points[0].X, points[1].Y - points[0].Y);

      RECT rect;

      rect.left      = (LONG) points[0].X;
      rect.top       = (LONG) points[0].Y;
      rect.right     = (LONG) points[1].X;
      rect.bottom    = (LONG) points[1].Y;

      Gdiplus::Bitmap * pbitmap = ((Gdiplus::Bitmap *) m_spbitmap->get_os_data());

      pbitmap->ApplyEffect(&myBlur, &rect);

      return true;
      */
      throw todo(get_app());
   }


   double graphics::get_dpix() const
   {
      //return m_prendertarget->GetDpiX();
      return 72.0;
   }

   bool graphics::destroy()
   {

      synch_lock ml(&draw2d_mutex());

      single_lock sl(&System.m_mutexDc, true);

      if(m_player != NULL)
      {
         
         m_prendertarget->PopLayer();

         m_player = nullptr;

      }

      m_ppathgeometryClip = nullptr;
 
      m_prendertarget = nullptr;
   
      m_pdevice = nullptr;

      m_pdevicecontext = nullptr;

      m_pbitmaprendertarget = nullptr;

      return true;

   }


   bool graphics::draw_path(::draw2d::path * ppath)
   {

      ::ID2D1Brush * pbrush = get_os_pen_brush(m_sppen);

      if (pbrush == NULL)
         return false;

      //m_prendertarget->SetTransform(D2D1::Matrix3x2F::Identity());

      m_prendertarget->DrawGeometry((ID2D1PathGeometry *)(dynamic_cast < ::draw2d_direct2d::graphics_path * > (ppath))->get_os_path(this),pbrush,(FLOAT)m_sppen->m_dWidth);

      //HRESULT hr = m_prendertarget->Flush();

      return true;

   }



   bool graphics::draw_path(::draw2d::path * ppath, ::draw2d::pen * ppen)
   {

      ::ID2D1Brush * pbrush = get_os_pen_brush(ppen);

      if(pbrush == NULL)
         return false;

      ID2D1PathGeometry * pg = (ID2D1PathGeometry *)(dynamic_cast < ::draw2d_direct2d::graphics_path * > (ppath))->get_os_path(this);
      if(pg != NULL)
      {
         m_prendertarget->DrawGeometry(pg,pbrush,(FLOAT)ppen->m_dWidth);
      }

      for(index i = 0; i < ppath->m_elementa.get_size(); i++)
      {

         if(ppath->m_elementa[i]->m_etype == ::draw2d::path::element::type_string)
         {

            draw(ppath->m_elementa[i]->m_stringpath, ppen);

         }

      }

      return true;

   }


   bool graphics::fill_path(::draw2d::path * ppath)
   {

      if (ppath == NULL)
         return false;

      keep < bool > keepPreviousFill(&ppath->m_bFill, true, ppath->m_bFill, true);

      ppath->m_bFill = true;

      ppath->m_bUpdated = false;

      ID2D1Brush * pbrush = get_os_brush(m_spbrush);

      if(pbrush == NULL)
         return false;

      ID2D1PathGeometry * pgeometry = (ID2D1PathGeometry *)(dynamic_cast < ::draw2d_direct2d::graphics_path * > (ppath))->get_os_path(this);
      
      if (pgeometry != NULL)
      {

         m_prendertarget->FillGeometry(pgeometry, pbrush);

      }


      for (index i = 0; i < ppath->m_elementa.get_size(); i++)
      {
         
         if (ppath->m_elementa[i]->m_etype == ::draw2d::path::element::type_string)
         {
            
            fill(ppath->m_elementa[i]->m_stringpath, m_spbrush);

         }

      }

      //HRESULT hr = m_prendertarget->Flush();

      return true;

   }


   bool graphics::fill_path(::draw2d::path * ppath, ::draw2d::brush * pbrushParam)
   {

      if(ppath == NULL)
         return false;

      keep < bool > keepPreviousFill(&ppath->m_bFill,true,ppath->m_bFill,true);

      ppath->m_bFill = true;

      ppath->m_bUpdated = false;

      ID2D1Brush * pbrush = get_os_brush(pbrushParam);

      if(pbrush == NULL)
         return false;

      ID2D1PathGeometry * pgeometry = (ID2D1PathGeometry *)(dynamic_cast < ::draw2d_direct2d::graphics_path * > (ppath))->get_os_path(this);

      if (pgeometry != NULL)
      {

         m_prendertarget->FillGeometry(pgeometry, pbrush);

      }


      for (index i = 0; i < ppath->m_elementa.get_size(); i++)
      {

         if (ppath->m_elementa[i]->m_etype == ::draw2d::path::element::type_string)
         {

            fill(ppath->m_elementa[i]->m_stringpath, pbrushParam);

         }

      }

      //HRESULT hr = m_prendertarget->Flush();

      return true;

   }

   bool graphics::path(::draw2d::path * ppath)
   {

      if (ppath == NULL)
         return false;

      bool bOk1 = fill_path(ppath);

      bool bOk2 = draw_path(ppath);

      return bOk1 && bOk2;

   }


   IDWriteTextFormat * graphics::get_os_font(::draw2d::font * pfont) const
   {

      pfont->m_powner = (void *) (graphics *) this;

      return (dynamic_cast < ::draw2d_direct2d::font * > (pfont))->get_os_font((graphics *) this);

   }


   ID2D1Brush * graphics::get_os_brush(::draw2d::brush * pbrush) const
   {

      pbrush->m_powner = (void *) (graphics *) this;

      return (dynamic_cast < ::draw2d_direct2d::brush * > (pbrush))->get_os_brush((graphics *) this);

   }

   ID2D1Brush * graphics::get_os_pen_brush(::draw2d::pen * ppen) const
   {

      ppen->m_powner = (void *) (graphics *) this;

      return (dynamic_cast < ::draw2d_direct2d::pen * > (ppen))->get_os_pen_brush((graphics *) this);

   }

   bool graphics::draw(const ::draw2d::path::string_path & path, ::draw2d::pen * ppen)
   {

      wstring szOutline(path.m_strText);

      IDWriteTextFormat * pformat =(IDWriteTextFormat *) get_os_font(path.m_spfont);

      IDWriteFactory * pfactory = TlsGetWriteFactory();

      IDWriteTextLayout * playout = NULL;

      HRESULT hr = pfactory->CreateTextLayout(
         szOutline,      // The string to be laid out and formatted.
         (UINT32) szOutline.length(),  // The length of the string.
         pformat,  // The text format to apply to the string (contains font information, etc).
         4096,         // The width of the on_layout box.
         4096,        // The height of the on_layout box.
         &playout  // The IDWriteTextLayout interface pointer.
         );

      if(playout == NULL)
      {
         return false;
      }

      CustomTextRenderer * pr = new CustomTextRenderer(GetD2D1Factory1(),m_prendertarget.Get(),get_os_pen_brush(ppen));

      playout->Draw(NULL, pr, (FLOAT) path.m_x, (FLOAT) path.m_y);

      delete pr;

      return true;

   }


   bool graphics::fill(const ::draw2d::path::string_path & path, ::draw2d::brush * pbrush)
   {

      wstring szOutline(path.m_strText);

      IDWriteTextFormat * pformat = (IDWriteTextFormat *)get_os_font(path.m_spfont);

      IDWriteFactory * pfactory = TlsGetWriteFactory();

      IDWriteTextLayout * playout = NULL;

      HRESULT hr = pfactory->CreateTextLayout(
         szOutline,      // The string to be laid out and formatted.
         (UINT32) szOutline.length(),  // The length of the string.
         pformat,  // The text format to apply to the string (contains font information, etc).
         4096,         // The width of the on_layout box.
         4096,        // The height of the on_layout box.
         &playout  // The IDWriteTextLayout interface pointer.
      );

      if (playout == NULL)
      {
         return false;
      }

      CustomTextRenderer * pr = new CustomTextRenderer(GetD2D1Factory1(), m_prendertarget.Get(), NULL, get_os_brush(pbrush));

      playout->Draw(NULL, pr, (FLOAT)path.m_x, (FLOAT)path.m_y);

      delete pr;

      return true;

   }


   bool graphics::flush()
   {

      HRESULT hr = m_prendertarget->Flush();

      return SUCCEEDED(hr);
      
   }

   void graphics::debug()
   {

#ifdef WINDOWSEX
      my_debug();
#endif

   }

   bool graphics::SaveClip()
   {

      synch_lock sl(m_pmutex);

      //if(m_bSaveClip)
      //   return true;

      //if(m_sparegionClip.get_count() > 0)
      //{
      //   
      //   m_bSaveClip = true;
      //   for(index i = m_sparegionClip.get_upper_bound(); i >= 0; i--)
      //   {

      //      try
      //      {

      //         m_prendertarget->PopLayer();

      //      }
      //      catch(...)
      //      {

      //      }


      //   }


      //}

      return true;

   }


   bool graphics::RestoreClip()
   {

      synch_lock sl(m_pmutex);

      //if(!m_bSaveClip)
      //   return true;

      //if(m_sparegionClip.get_count() > 0)
      //{
      //   m_bSaveClip = false;

      //   for(auto pregion : m_sparegionClip)
      //   {

      //      ID2D1Geometry * pgeometry = (ID2D1Geometry *)pregion->get_os_data();

      //      m_prendertarget->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(),pgeometry),NULL);

      //   }

      //}

      return true;

   }


   void graphics::enum_fonts(::draw2d::font::enum_item_array & itema)
   {

      ::windows::comptr<IDWriteFontCollection> pFontCollection;

      // Get the system font collection. 
      HRESULT hr = TlsGetWriteFactory()->GetSystemFontCollection(&pFontCollection);

      UINT32 familyCount = 0;

      // Get the number of font families in the collection. 
      if(SUCCEEDED(hr))
      {
         familyCount = pFontCollection->GetFontFamilyCount();
      }

      uint32_t index = 0;
      BOOL exists = false;
      wchar_t localeName[LOCALE_NAME_MAX_LENGTH];

      // Get the default locale for this user. 
      int defaultLocaleSuccess = GetUserDefaultLocaleName(localeName,LOCALE_NAME_MAX_LENGTH);

      for(UINT32 i = 0; i < familyCount; ++i)
      {
         ::windows::comptr<IDWriteFontFamily> pFontFamily;

         // Get the font family. 
         if(SUCCEEDED(hr))
         {
            hr = pFontCollection->GetFontFamily(i,&pFontFamily);
         }

         ::windows::comptr<IDWriteLocalizedStrings> pFamilyNames;

         // Get a list of localized strings for the family name. 
         if(SUCCEEDED(hr))
         {
            hr = pFontFamily->GetFamilyNames(&pFamilyNames);
         }

         if(SUCCEEDED(hr))
         {
            // If the default locale is returned, find that locale name. 
            if(defaultLocaleSuccess)
            {
               hr = pFamilyNames->FindLocaleName(localeName,&index,&exists);
            }
            if(SUCCEEDED(hr) && !exists) // if the above find did not find a match, retry with US English 
            {
               hr = pFamilyNames->FindLocaleName(L"en-us",&index,&exists);
            }
         }

         // If the specified locale doesn't exist, select the first on the list. 
         if(!exists)
         {
            index = 0;
         }

         UINT32 length = 0;

         // Get the string length. 
         if(SUCCEEDED(hr))
         {
            hr = pFamilyNames->GetStringLength(index,&length);
         }

         // Allocate a string big enough to hold the name. 
         hwstring name(length + 1);

         if(name == nullptr)
         {
            hr = E_OUTOFMEMORY;
         }

         // Get the family name. 
         if(SUCCEEDED(hr))
         {
            hr = pFamilyNames->GetString(index,name,length + 1);
         }

         // Add the family name to the String Array. 
         if(SUCCEEDED(hr))
         {
            
            string strName = string((LPCWSTR)(name));

            itema.add(::draw2d::font::enum_item(strName, strName, ::draw2d::font::cs_default));

         }


      }

   }



} // namespace draw2d_direct2d






