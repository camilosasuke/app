//#include "framework.h"
//#include "base/user/user.h"


#ifdef HOTPLUGIN_SUBSYSTEM

#include <math.h>


namespace hotplugin
{


   void plugin::on_bare_paint_discreet(::draw2d::graphics * pgraphics,const RECT & lprect)
   {

      double dRate = get_progress_rate();

      if (dRate <= 0.0)
      {

         return;

      }

      RECT rectWindow;
      GetWindowRect(&rectWindow);
      int32_t cx = rectWindow.right - rectWindow.left;
      int32_t cy = rectWindow.bottom - rectWindow.top;
      RECT rect;
      rect.left = 0;
      rect.top = 0;
      rect.bottom = cy;
      rect.right = cx;

      ::draw2d::pen_sp pen(allocer());

      pen->create_solid(1.0, RGB(84, 84, 77));

      ::draw2d::brush_sp brush(allocer());

      brush->m_etype = ::draw2d::brush::type_null;

      

      pgraphics->SelectObject(pen);

      pgraphics->SelectObject(brush);

      ::rect rectClient;

      GetClientRect(rectClient);

      pgraphics->Rectangle(rectClient);



      int32_t iDelta = m_iDelta;
      int32_t iRadius = 8;
      int32_t iPhase = ((((get_tick_count() - m_dwSync) % iDelta)) * iRadius * 2) / iDelta;

      if (iPhase == 0 && (get_tick_count() - m_dwSync) > 1977)
      {

         m_iDelta = 1984 + (get_tick_count() % ((5000) * 2));
         m_dwSync = get_tick_count();

      }

      int32_t iStep = iPhase;

      if (iStep > iRadius)
         iStep = iRadius * 2 - iStep;

      wstring wstr;

      wstr = m_strStatus;

      int32_t iRate1 = 25;

      BYTE bA;

      {

         //int32_t iARange = (184 - 23) * 2;
         //int32_t iAClip = iARange / 2;
         double period = 8.4; // seconds
         double frequency = 1.0 / period;
         //int32_t iA = (iARange * ::get_tick_count() / 8000) % iARange;
         double w = 2.0 * 3.1415 * frequency;
         double t = get_tick_count() / 1000.0;


         /*if(iA < iAClip)
         bA = iA + 23;
         else
         bA = 184 * 2 - iA;*/

         bA = MIN(255, MAX(0, (BYTE)((184.0 * ((sin(w * t) + 1.0) / 2.0)) + 23.0)));



      }

      BYTE uchR;
      BYTE uchG;
      BYTE uchB;

      //graphics2.SetCompositingMode(Gdiplus::CompositingModeSourceOver);

      int32_t iRate = 10;

      const int32_t iRowCount = cx - cx / (iRate / 2);
      int32_t iProgressCount = MAX(MIN((int32_t)(iRowCount * dRate), iRowCount), 0);



      int32_t iBorder1 = MAX(cx / iRate1, cy / iRate1);

      ::draw2d::path_sp pathClip1(allocer());

      RECT rectClip1;

      rectClip1.left = lprect.left + iBorder1;
      rectClip1.top = lprect.top + iBorder1;
      rectClip1.right = rectClip1.left + cx - iBorder1 * 2;
      rectClip1.bottom = rectClip1.top + cy - iBorder1 * 2;

      pathClip1->begin_figure(true, ::draw2d::fill_mode_winding);

      pathClip1->add_round_rect(rectClip1, iBorder1 * 2);

      pathClip1->end_figure(true);

      //      pgraphics->replace_clip(pathClip1);



      int32_t iRatePercentMillis = ((int32_t)(dRate * 100.0 * 1000.0)) % 1000;
      int32_t iRatePercent = ((int32_t)(dRate * 100.0));

      wstring wstrProgress;

      {

         stringtow strProgress(wstrProgress);

         string strDecimal = itoa_dup(iRatePercentMillis);

         zero_pad(strDecimal, 3);

         strProgress = itoa_dup(iRatePercent) + "." + strDecimal + "%";

      }

      int32_t iBarHeight = 23;



      int32_t iBorder = 16;

      ::draw2d::path_sp pathClip(allocer());

      RECT rectClip;

      rectClip.left = lprect.left + cx / iRate - iBorder;
      rectClip.top = lprect.top + (cy - iBarHeight) / 2 - iBorder;
      rectClip.right = rectClip.left + iRowCount + iBorder * 2;
      rectClip.bottom = rectClip.top + iBarHeight + iBorder * 2;


      pathClip->begin_figure(true, ::draw2d::fill_mode_winding);

      pathClip->add_round_rect(rectClip, iBorder);

      pathClip->end_figure(true);

      //pgraphics->exclude_clip(pathClip);


      POINT pa[4];

      //Gdiplus::SolidBrush * pbr = new Gdiplus::SolidBrush(Gdiplus::Color(49, 177 + iBarHeight, 177 + iBarHeight, 177 + 19));
      //graphics2.FillRectangle(pbr, lprect.left , lprect.top, lprect.left + cx, lprect.top + cy);
      //delete pbr;

      ::draw2d::brush_sp br(allocer());

      br->create_solid(ARGB(49, 184 + 23, 184 + 23, 184 + 19));

      int32_t mcy = cy / 2;

      if (m_iHealingSurface == 1)
      {

         pgraphics->SelectObject(br);

         for (int32_t x = 0; x < (cx + cy); x += 46)
         {

            pa[0].x = lprect.left + x;
            pa[0].y = lprect.top;

            pa[1].x = lprect.left + x + iBarHeight;
            pa[1].y = lprect.top;

            pa[2].x = lprect.left + x - mcy + iBarHeight;
            pa[2].y = lprect.top + mcy;

            pa[3].x = lprect.left + x - mcy;
            pa[3].y = lprect.top + mcy;

            pgraphics->fill_polygon(pa, 4);

            pa[0].x = lprect.left + x - mcy - iBarHeight;
            pa[0].y = lprect.top + mcy;

            pa[1].x = lprect.left + x - mcy;
            pa[1].y = lprect.top + mcy;

            pa[2].x = lprect.left + x - cy;
            pa[2].y = lprect.top + cy;

            pa[3].x = lprect.left + x - cy - iBarHeight;
            pa[3].y = lprect.top + cy;

            pgraphics->fill_polygon(pa, 4);


         }

      }


      inflate_rect(rectClip, 3);

      //pgraphics->replace_clip(rectClip);

      br->create_solid(ARGB(84, 84, 84, 77));

      RECT r1;

      r1.left = lprect.left + cx / iRate - 1;
      r1.top = lprect.top + (cy - iBarHeight) / 2 - 1;
      r1.right = r1.left + iRowCount + 4;
      r1.bottom = r1.top + iBarHeight + 2;

      pgraphics->FillRect(r1, br);

      /*for(iRow = 0; iRow < iProgressCount; iRow++)
      {
      {
      get_progress_color(uchR, uchG, uchB, (double) iRow / (double) iRowCount, 0);
      Gdiplus::SolidBrush * pbr = new Gdiplus::SolidBrush(Gdiplus::Color(bA, uchR, uchG, uchB));
      graphics2.FillRectangle(pbr, lprect.left + iRow + cx / iRate , lprect.top + (cy - iBarHeight) / 2, 1, 5);
      delete pbr;
      }
      {
      get_progress_color(uchR, uchG, uchB, (double) iRow / (double) iRowCount, 1);
      Gdiplus::SolidBrush * pbr = new Gdiplus::SolidBrush(Gdiplus::Color(bA, uchR, uchG, uchB));
      graphics2.FillRectangle(pbr, lprect.left + iRow + cx / iRate , lprect.top + (cy - iBarHeight) / 2 + 5, 1, 5);
      delete pbr;
      }
      {
      get_progress_color(uchR, uchG, uchB, (double) iRow / (double) iRowCount, 2);
      Gdiplus::SolidBrush * pbr = new Gdiplus::SolidBrush(Gdiplus::Color(bA, uchR, uchG, uchB));
      graphics2.FillRectangle(pbr, lprect.left + iRow + cx / iRate , lprect.top + (cy - iBarHeight) / 2 + 10, 1, 13);
      delete pbr;
      }
      }*/
      ::rect r;
      {
         get_progress_color(uchR, uchG, uchB, dRate, 0);
         br->create_solid(ARGB(bA, uchR, uchG, uchB));
         r = rect_dim(lprect.left + cx / iRate, lprect.top + (cy - iBarHeight) / 2, iProgressCount, 5);
         pgraphics->FillRect(r, br);
      }
      {
         get_progress_color(uchR, uchG, uchB, dRate, 1);
         br->create_solid(ARGB(bA, uchR, uchG, uchB));
         r = rect_dim(lprect.left + cx / iRate, lprect.top + (cy - iBarHeight) / 2 + 5, iProgressCount, 5);
         pgraphics->FillRect(r, br);
      }
      {
         get_progress_color(uchR, uchG, uchB, dRate, 2);
         br->create_solid(ARGB(bA, uchR, uchG, uchB));
         r = rect_dim(lprect.left + cx / iRate, lprect.top + (cy - iBarHeight) / 2 + 10, iProgressCount, 13);
         pgraphics->FillRect(r, br);
      }

      int32_t iOffset = 3;

      pen->create_solid(1.0, ARGB(220, 180, 180, 180));
      pgraphics->draw_line(lprect.left + cx / iRate - iOffset, lprect.top + (cy - iBarHeight) / 2 - iOffset, lprect.left + cx - cx / iRate + iOffset, lprect.top + (cy - iBarHeight) / 2 - iOffset, pen);
      pgraphics->draw_line(lprect.left + cx / iRate - iOffset, lprect.top + (cy - iBarHeight) / 2 - iOffset, lprect.left + cx / iRate - iOffset, lprect.top + (cy + iBarHeight) / 2 + iOffset, pen);

      pen->create_solid(1.0, ARGB(220, 77, 77, 77));
      pgraphics->draw_line(lprect.left + cx / iRate - iOffset, lprect.top + (cy + iBarHeight) / 2 + iOffset, lprect.left + cx - cx / iRate + iOffset, lprect.top + (cy + iBarHeight) / 2 + iOffset, pen);
      pgraphics->draw_line(lprect.left + cx - cx / iRate + iOffset, lprect.top + (cy - iBarHeight) / 2 - iOffset, lprect.left + cx - cx / iRate + iOffset, lprect.top + (cy + iBarHeight) / 2 + iOffset, pen);

      iOffset = 2;
      pen->create_solid(1.0, ARGB(220, 84, 84, 84));
      pgraphics->draw_line(lprect.left + cx / iRate - iOffset, lprect.top + (cy - iBarHeight) / 2 - iOffset, lprect.left + cx - cx / iRate + iOffset, lprect.top + (cy - iBarHeight) / 2 - iOffset, pen);
      pgraphics->draw_line(lprect.left + cx / iRate - iOffset, lprect.top + (cy - iBarHeight) / 2 - iOffset, lprect.left + cx / iRate - iOffset, lprect.top + (cy + iBarHeight) / 2 + iOffset, pen);

      pen->create_solid(1.0, ARGB(220, 170, 170, 170));
      pgraphics->draw_line(lprect.left + cx / iRate - iOffset, lprect.top + (cy + iBarHeight) / 2 + iOffset, lprect.left + cx - cx / iRate + iOffset, lprect.top + (cy + iBarHeight) / 2 + iOffset, pen);
      pgraphics->draw_line(lprect.left + cx - cx / iRate + iOffset, lprect.top + (cy - iBarHeight) / 2 - iOffset, lprect.left + cx - cx / iRate + iOffset, lprect.top + (cy + iBarHeight) / 2 + iOffset, pen);


      br->create_solid(ARGB(127, 255, 255, 255));

      pgraphics->SelectObject(br);

      ::draw2d::font_sp f(allocer());

      f->create_pixel_font("Calibri", iBarHeight * 0.7);

      pgraphics->SelectObject(f);

      wstring wstrStatus;

      wstrStatus = wstr;
      wstrStatus = wstrStatus + unitext(" : ");
      wstrStatus = wstrStatus + wstrProgress;


      pgraphics->text_out(lprect.left + cx / iRate - 1 + 18, lprect.top + (cy - iBarHeight) / 2 - 1 + 1, string(wstrStatus));

   }


} // namespace hotplugin


#endif


