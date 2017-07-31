//#include "framework.h"
//#include "base/user/user.h"


#ifdef HOTPLUGIN_SUBSYSTEM


namespace hotplugin
{


   void plugin::on_bare_paint_full_screen(::draw2d::graphics * pgraphics,const RECT & lprect)
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

      string str;

      str = m_strStatus;

      string strProgress;

      strProgress.Format("%0.3f%%", dRate * 100.0);

      ::draw2d::brush_sp br(allocer());

      

      {

         BYTE uchR, uchG, uchB;
         ::rect r;
         get_progress_color(uchR, uchG, uchB, dRate, 0);
         br->create_solid(ARGB(255, uchR, uchG, uchB));
         r = rect_dim(lprect.left, lprect.top, cx, cy);
         pgraphics->FillRect(r, br);

      }

      br->create_solid(ARGB(255, 255, 255, 255));

      pgraphics->SelectObject(br);

      ::draw2d::font_sp f(allocer());

      f->create_pixel_font(FONT_SANS_FX, 49);

      pgraphics->SelectObject(f);

      string strStatus;

      strStatus = str;

      //on_paint_progress(pgraphics, m_rect);

      pgraphics->text_out(lprect.left + 84, lprect.top + 84, strStatus);

      f->create_pixel_font(FONT_SANS_FX, 84, true);

      pgraphics->SelectObject(f);

      pgraphics->text_out(lprect.left + 84, (int32_t)(lprect.top + 133 + 49 * 0.2), strProgress);

      f->create_pixel_font(FONT_SANS_FX, 23);

      pgraphics->SelectObject(f);

      pgraphics->text_out(lprect.left + 84, (int32_t)(lprect.top + 133 + 49 * 0.2 + 133 * 0.2), m_strStatus2);

   }


} // namespace hotplugin


#endif



