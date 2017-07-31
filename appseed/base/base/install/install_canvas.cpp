#include "framework.h"


#if defined(INSTALL_SUBSYSTEM)


#include "install_canvas.h"
#include <stdio.h>


namespace install
{

   canvas::canvas(::aura::application * papp) :
      ::object(papp)
   {

      m_iMode = 0;

#ifdef SUPORTA_TELA_AVANCADA

      m_iModeCount = 7;

#else

      m_iModeCount = 5;

#endif

   }

   canvas::~canvas()
   {
   }

//#define SUPORTA_TELA_AVANCADA

#ifdef SUPORTA_TELA_AVANCADA
class canvas_zero
{
public:
   uint32_t m_dwCurZero;
   uint32_t m_dwNextZero;
   int32_t m_iMaxSize;
   int32_t m_iLast;
   array < POINT > m_pta;
   int_array m_ia;
   RECT m_rect;
   HBITMAP m_hbm;
   HDC m_hdc;
   HBITMAP m_hbmZero;
   HBITMAP m_hbmZeroOld;
   HDC m_hdcZero;
   COLORREF * m_pdata;
   COLORREF * m_pdataZero;
   canvas_zero();
   void prepare(HDC hdc, const RECT & rect);
   void on_paint(::draw2d::graphics * pgraphics, const RECT & rect);
   virtual void zero(HDC hdc, POINT pt, int32_t iSize, int32_t iStep);
};

#endif

int32_t canvas::increment_mode()
{

   m_iMode++;

   if(m_iMode >= m_iModeCount)
      m_iMode = 0;

   return m_iMode;

}

void canvas::on_paint(::draw2d::graphics * pgraphics, const RECT & rectParam)
{
   int32_t iMode = m_iMode;
#ifdef SUPORTA_TELA_AVANCADA
   //static canvas_zero czero;
   #endif

   //static string s_strLastStatus;
   RECT rect = rectParam;
//   int32_t cx = lpcrect->right - lpcrect->left;
//   int32_t cy = lpcrect->bottom - lpcrect->top;
//   int32_t iThankYouHeight = 30;

   ::draw2d::font_sp font(allocer());

   font->create_point_font(FONT_SANS, 10);

   ::draw2d::font_sp fontBold(allocer());


   fontBold->create_point_font(FONT_SANS, 10);

   fontBold->m_iFontWeight = FW_BOLD;

   pgraphics->SelectObject(font);

   ::draw2d::brush_sp brush(allocer());

   if(iMode == 1 || iMode == 2 || iMode == 3 || iMode == 4)
   {

      ::draw2d::pen_sp pen(allocer());

      pen->create_solid(1.0, ARGB(255, 84, 84, 77));

      if(iMode == 1 || iMode == 2)
      {
         brush->m_etype = ::draw2d::brush::type_null;
      }
      else
      {
         brush->create_solid(ARGB(255, 84, 84, 84));
      }

      pgraphics->SelectObject(pen);

      pgraphics->SelectObject(brush);

      pgraphics->Rectangle(rectParam);

   }
   SIZED size = pgraphics->GetTextExtent("CCpp");
//   int32_t iLineCount = (rect.bottom - 30 - iThankYouHeight) / size.cy;
   if(iMode == 6) // if(m_bHealingSurface)
   {
#ifdef SUPORTA_TELA_AVANCADA
      int32_t iCount = MAX(1, cx / 25);
      int32_t jCount = MAX(1, cy / 25);

      for(int32_t i = 0; i < iCount; i++)
      {
         for(int32_t j = 0; j < jCount; j++)
         {
            //int32_t iPhase =
            // 400
            //* (0.5 + (sin_dup(((double) ::get_tick_count() * 3.1415 * 2.0  * 0.084 / 1000.0) + ((double) (i) * 3.1415 * 2.0 / (double) (iCount))) / 2.0));
            //* (0.5 + (cos_dup(((double) ::get_tick_count() * 3.1415 * 2.0  * 0.084 / 1000.0) + ((double) (0) * 3.1415 * 2.0 / (double) (jCount))) / 2.0));
            double dPhase = fmod_dup((((double) ::get_tick_count() * 360  * 0.5984 / 1000.0) + (i * 360.0 / (double) iCount)  + (j * 360.0 / (double) jCount) + ((double) (sin_dup(((double) ::get_tick_count() * 3.1415 * 2.0  *0.0484 / 1000.0)+i * 3.1415 * 2.0 * 2.0 / (double) (iCount)) * sin_dup(((double) ::get_tick_count() * 3.1415 * 2.0  * 0.0484 / 1000.0)+j * 3.1415 * 2.0 * 2.0 / (double) (jCount)) * 360))), 360.0);
            int32_t iR;
            int32_t iG;
            int32_t iB;
            double dRate = fmod_dup(dPhase, 60.0) / 60.0;
            int32_t iColor = (int32_t) (dRate * 155.0);
            if(dPhase < 60)
            {
               // purple to blue
               iR = 255 - iColor;
               iG = 100;
               iB = 255;
            }
            else if(dPhase < (120))
            {
               // blue to cyan
               iR = 100;
               iG = 100 + iColor;
               iB = 255;
            }
            else if(dPhase < (180))
            {
               // cyan to green
               iR = 100;
               iG = 255;
               iB = 255 - iColor;
            }
            else if(dPhase < (240))
            {
               // green to yellow
               iR = 100 + iColor;
               iG = 255;
               iB = 100;
            }
            else if(dPhase < (300))
            {
               // yellow to red
               iR = 255;
               iG = 255 - iColor;
               iB = 100;
            }
            else
            {
               // red to purple
               iR = 255;
               iG = 100;
               iB = 100 + iColor;
            }

            double z = ((int32_t) (360 - (dPhase - 240 + ::get_tick_count() * 360  * 0.1984 / 1000.0))) % 360;
            if(dPhase == 240)
               z = 360;

            int32_t iInflate = (int32_t) (z * 33.0 / 360.0);

            rect.left = cx * i / iCount + 8 - iInflate;
            rect.right = cx * (i + 1) / iCount - 8 + iInflate;
            rect.top = cy * j / jCount + 8 - iInflate;
            rect.bottom = cy * (j + 1) / jCount - 8 + iInflate;

            // ca colors: green and yellow and black and white
            ::FillSolidRect_dup(hdc, &rect, RGB(iR, iG, iB));
         }
      }
#endif
   }
   else if(iMode == 3) // else // !m_bHealingSurface => "Surgery Internals"
   {

      brush->create_solid(ARGB(255, 0xCC, 0xCC, 0xCC));

      pgraphics->SelectObject(brush);

      size_t iRefresh = 1000;
      size_t iEat = 8;
      const char * psz = "development message so international english file \"C:\\core\\install.log\" excerpt  ::::::::";
      pgraphics->text_out(10.0, 10 + size.cy * 2, psz, (int32_t) (strlen_dup(psz) - iEat + 1 + ((::get_tick_count() / (iRefresh - 277) % iEat))));
      DWORD dwRead;
//      int32_t iLineMin = 5;
  //    int32_t iLine = ((rect.bottom - 10) / size.cy) - 1;
      if(rect.bottom - rect.top >= size.cy)
      {

         HANDLE hfile = ::create_file(::path::install_log(process_platform_dir_name2()),GENERIC_READ,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

         if(hfile != INVALID_HANDLE_VALUE)
         {
            int32_t iTell = ::SetFilePointer(hfile, 0, NULL, SEEK_END);
            iTell--;
            string strLine;
            int32_t iSkip = 0;
//            ::SetBkMode(hdc, TRANSPARENT);
            bool bNormal = false;
            bool bBold = false;
            bool bPreNormal = false;
            bool bStart = false;
            while(iTell > 0 && !bStart && !(bNormal && bBold))
            {
               ::SetFilePointer(hfile, iTell, NULL, SEEK_SET);
               char ch;
               if(!ReadFile(hfile, &ch,  1, &dwRead, NULL))
                  break;
               if(dwRead <= 0)
                  break;

               if(ch == '\r')
               {
                  iSkip++;
               }
               else if(ch == '\n')
               {
                  iSkip++;
               }
               else if(iSkip > 0)
               {
                  iSkip = 0;
                  strLine.trim();
                  if(strLine == "--")
                  {
                     bStart = true;
                  }
                  else if(strLine.begins_ci("***") && !bBold && strLine.length() > 0 && bNormal && bPreNormal)
                  {
                     bBold = true;
                     strLine = strLine.substr(3);
                     pgraphics->SelectObject(fontBold);
                     pgraphics->text_out(10.0, 10 + size.cy * 3, strLine);
                  }
                  else if(!strLine.begins_ci("***") && strLine.length() > 0 && !bNormal && !bBold && bPreNormal)
                  {
                     bNormal = true;
                     pgraphics->SelectObject(font);
                     pgraphics->text_out(10.0, 10 + size.cy * 4, strLine);
                  }
                  else if(strLine.length() > 0 && !bPreNormal && !bBold && !bNormal)
                  {
                     bPreNormal = true;
                     //::SelectObject(hdc, hfont);
                     //::TextOutU_dup(hdc, 10, 10 + size.cy * 4, strLine, strLine.length());
                  }
                  strLine = ch;
               }
               else
               {
                  strLine = ch + strLine;
               }
               iTell--;
            }
            ::CloseHandle(hfile);
         }
      }
   }
   else if(iMode == 4) // else // !m_bHealingSurface => "Surgery Internals"
   {

      brush->create_solid(ARGB(255, 0xCC, 0xCC, 0xCC));

      pgraphics->SelectObject(brush);

      size_t iRefresh = 1000;
      size_t iEat = 8;
      const char * psz = "development message so international english last lines of file \"C:\\core\\install.log\" ::::::::";
      pgraphics->text_out(10.0, 10 + size.cy * 2, psz, (int32_t) (strlen_dup(psz) - iEat + 1 + ((::get_tick_count() / (iRefresh - 277) % iEat))));
      pgraphics->SelectObject(fontBold);
      //pgraphics->text_out( 10, 10 + size.cy * 3, s_strLastStatus);
      DWORD dwRead;
      int32_t iLineMin = 5;
      int32_t iLine = ((rect.bottom - 10) / size.cy) - 1;
      if(rect.bottom - rect.top >= size.cy)
      {

         HANDLE hfile = ::create_file(::path::install_log(process_platform_dir_name2()),GENERIC_READ,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

         if(hfile != INVALID_HANDLE_VALUE)
         {
            int32_t iTell = ::SetFilePointer(hfile, 0, NULL, SEEK_END);
            iTell--;
            string strLine;
            int32_t iSkip = 0;
            while(iTell > 0 && iLine >= iLineMin)
            {
               ::SetFilePointer(hfile, iTell, NULL, SEEK_SET);
               char ch;
               if(!ReadFile(hfile, &ch,  1, &dwRead, NULL))
                  break;
               if(dwRead <= 0)
                  break;
               if(ch == '\r')
               {
                  if(iSkip >= 2)
                  {
                     iSkip = 0;
                     iLine--;
                  }
                  iSkip++;
               }
               else if(ch == '\n')
               {
                  if(iSkip >= 2)
                  {
                     iSkip = 0;
                     iLine--;
                  }
                  iSkip++;
               }
               else if(iSkip > 0)
               {
                  iSkip = 0;
                  if(strLine.begins_ci("***"))
                  {
                     strLine = strLine.substr(3);
                     //if(strLine == s_strLastStatus)
                     //{
                     //   goto skip_text_out1;
                     //}
                     //s_strLastStatus = strLine;
                     pgraphics->SelectObject(fontBold);
                  }
                  else
                  {
                     index iFind = strLine.find('\t');
                     if(iFind >= 0)
                     {
                        strLine = strLine.substr(iFind + 1);
                     }
                     pgraphics->SelectObject(font);
                  }
                  iLine--;
                  pgraphics->text_out(10.0, 10 + iLine * size.cy, strLine);
//                  skip_text_out1:
                  strLine = ch;
               }
               else
               {
                  strLine = ch + strLine;
               }
               iTell--;
            }
            ::CloseHandle(hfile);
            if(iLine >= iLineMin && strLine.length() > 0)
            {
               iLine--;
               pgraphics->text_out(10.0, 10 + iLine * size.cy, strLine);
            }
         }
      }
   }
   else if(iMode == 5) // if(m_bHealingSurface)
   {
#ifdef SUPORT_TELA_AVANCADA
      czero.on_paint(g, lpcrect);
#endif
   }

}












   #ifdef SUPORTA_TELA_AVANCADA


canvas_zero::canvas_zero()
{
   m_rect.left = 0;
   m_rect.top = 0;
   m_rect.right = 0;
   m_rect.bottom = 0;
   m_dwCurZero = ::get_tick_count();
   m_dwNextZero = ::get_tick_count();
   m_iMaxSize = 33;
   m_iLast = 0;
}

void canvas_zero::prepare(HDC hdc, const RECT & rect)
{
   m_rect.left    = lpcrect->left;
   m_rect.top     = lpcrect->top;
   m_rect.right   = lpcrect->right;
   m_rect.bottom  = lpcrect->bottom;

   int32_t cx = m_rect.right - m_rect.left;
   int32_t cy = m_rect.bottom - m_rect.top;

   #ifdef WINDOWSEX

   BITMAPINFO m_Info;

	m_Info.bmiHeader.biSize=sizeof (BITMAPINFOHEADER);
	m_Info.bmiHeader.biWidth=cx;
	m_Info.bmiHeader.biHeight=cy;
	m_Info.bmiHeader.biPlanes=1;
	m_Info.bmiHeader.biBitCount=32;
	m_Info.bmiHeader.biCompression=BI_RGB;
	m_Info.bmiHeader.biSizeImage=cx*cy*4;

   LPDWORD lpdata;

	m_hbm = CreateDIBSection ( NULL, &m_Info, DIB_RGB_COLORS, (void **)&m_pdata, NULL, NULL );
   m_hdc = ::CreateCompatibleDC(NULL);
   ::SelectObject(m_hdc, m_hbm);
   FillSolidRect_dup(m_hdc, lpcrect, RGB(0, 0, 0));

   #endif

   /*int32_t iMid = (m_rect.bottom + m_rect.top) / 2;

   HPEN hpen = ::create_solid(1, RGB(0x7c, 0x7c, 0x7c));

   ::SelectObject(m_hdc, hpen);

   MoveToEx(m_hdc, m_rect.left, iMid, NULL);
   LineTo(m_hdc, m_rect.right, iMid);
   ::DeleteObject(hpen);*/

}

void canvas_zero::on_paint(simple_graphics & gPaint, const RECT & rect)
{
   HDC hdc = m_hdc;

   if(m_rect.left != lpcrect->left
   || m_rect.top != lpcrect->top
   || m_rect.right != lpcrect->right
   || m_rect.bottom != lpcrect->bottom)
   {
      prepare(hdc, lpcrect);
   }

   int32_t cx = m_rect.right - m_rect.left;
   int32_t cy = m_rect.bottom - m_rect.top;


   int32_t iTimeFactor = 100; // greater faster

   if(::get_tick_count() >= m_dwNextZero)
   {
      POINT pt;
      pt.x = (rand_dup() * cx / RAND_MAX) + lpcrect->left;
      pt.y = (rand_dup() * cy / RAND_MAX) + lpcrect->top;
      m_pta.insert_at(0, pt);
      m_ia.insert_at(0, ((rand_dup() * MAX(cx, cy) / 4 / RAND_MAX) + 23));
      m_dwCurZero = ::get_tick_count();
      m_dwNextZero = m_dwCurZero + m_ia[m_ia.get_count() - 1] * 1000 / iTimeFactor + 5000;
      m_iLast = 0;
      int32_t iSize = m_ia[0];
      int32_t w = iSize * 2 + 1;
      int32_t h = iSize * 2 + 1;

      #ifdef WINDOWSEX
      BITMAPINFO m_Info;

      ZeroMemory(&m_Info, sizeof (BITMAPINFO));

	   m_Info.bmiHeader.biSize=sizeof (BITMAPINFOHEADER);
	   m_Info.bmiHeader.biWidth=w;
	   m_Info.bmiHeader.biHeight=-h;
	   m_Info.bmiHeader.biPlanes=1;
	   m_Info.bmiHeader.biBitCount=32;
	   m_Info.bmiHeader.biCompression=BI_RGB;
	   m_Info.bmiHeader.biSizeImage=w*h*4;

      LPDWORD lpdata;

	   m_hbmZero = CreateDIBSection ( NULL, &m_Info, DIB_RGB_COLORS, (void **)&m_pdataZero, NULL, NULL );
      m_hdcZero = ::CreateCompatibleDC(NULL);
      m_hbmZeroOld = (HBITMAP) ::SelectObject(m_hdcZero, m_hbmZero);
      FillSolidRect_dup(m_hdcZero, lpcrect, RGB(0, 0, 0));
      #endif
   }





   int32_t i = 1;
/*   for(; i < m_pta.size(); i++)
   {
      POINT pt = m_pta[i];
      int32_t iSize = m_ia[i];
      zero(hdc, pt, iSize, iSize);
   }*/

         BLENDFUNCTION bf;
         bf.BlendOp     = AC_SRC_OVER;
         bf.BlendFlags  = 0;
         bf.SourceConstantAlpha = 255;
         bf.AlphaFormat = AC_SRC_ALPHA;

   bool bReady = false;
   POINT pt;
   int32_t iSize;
   int32_t iCurStep;
   if(m_pta.get_count() > 0)
   {
      pt = m_pta[0];
      iSize = m_ia[0];
      iCurStep = (::get_tick_count() - m_dwCurZero) * iTimeFactor / 1000;
      iCurStep = MAX(2, MIN(iCurStep, iSize));
      zero(hdc, pt, iSize, iCurStep);
      if(iCurStep == iSize)
      {
         if(m_iLast <= iCurStep)
         {
            ::AlphaBlend(m_hdc, pt.x - iSize / 2, pt.y - iSize / 2, iSize * 2 + 1, iSize * 2 + 1, m_hdcZero, 0, 0, iSize * 2 + 1, iSize * 2 + 1, bf);
         }
         bReady = true;
      }
      else
      {
      }
      m_iLast = iCurStep + 1;
   }
   else
   {
   }

   ::BitBlt(hdcPaint, 0, 0, cx, cy, m_hdc, 0, 0, SRCCOPY);

   if(!bReady)
   {
      ::AlphaBlend(hdcPaint, pt.x - iSize / 2, pt.y - iSize / 2, iSize * 2 + 1, iSize * 2 + 1, m_hdcZero, 0, 0, iSize * 2 + 1, iSize * 2 + 1, bf);
      //::AlphaBlend(hdcPaint, 0, 0, iSize, iSize, m_hdcZero, 0, 0, iSize, iSize, bf);
   }

}


void canvas_zero::zero(HDC hdc, POINT pt, int32_t iSize, int32_t iStep)
{
   int32_t cx = m_rect.right - m_rect.left;
   int32_t cy = m_rect.bottom - m_rect.top;

   int32_t w = iSize * 2 + 1;
   int32_t radius = iStep - 1;
   for(int32_t j = -iSize; j < iSize; j++)
   {
      for(int32_t k = -iSize; k < iSize; k++)
      {
         double dj = j;
         double dk = k;
         double distance = sqrt_dup((dj * dj + dk * dk));
         if(distance < radius)
         {
            double dLum = (double) (distance) / (double) radius;
            dLum = pow_dup(dLum, 0.2);
            int32_t x = j + iSize;
            int32_t y = k + iSize;
            int32_t iAlpha = dLum * 255.0;
            if(iAlpha > 255)
               iAlpha = 255;
            if(iAlpha < 0)
               iAlpha = 0;
            int32_t iLum = 255 - iAlpha;
            m_pdataZero[x + y * w] = RGB(iLum, iLum, iLum) | (iLum << 24);
         }
      }
   }

}

#endif


} // namespace install



//namespace base
//{ 
//
//
//   ::install::canvas * system::install_create_canvas()
//   {
//
//
//      return new ::install::canvas(this);
//
//
//   }
//
//
//   void system::install_canvas_on_paint(::draw2d::graphics * pgraphics, const RECT & rect)
//   {
//
//      install().m_pcanvas->on_paint(pgraphics, rect);
//
//   }
//
//
//   int system::install_canvas_increment_mode()
//   {
//
//      return install().m_pcanvas->increment_mode();
//
//   }
//
//
//
//} // namespace base


#endif




