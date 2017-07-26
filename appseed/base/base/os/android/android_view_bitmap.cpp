#include <android/log.h>
#include <android/bitmap.h>

// fazer o que, por enquanto global....
//::android::view_bitmap * g_pandroidbitmapview = NULL;
//
////#include "framework.h"
////#include "windows_window_gdi.h"
//namespace android
//{
//
//
//
//   view_bitmap::view_bitmap(::aura::application * papp) :
//      object(papp),
//      window_graphics(papp),
//      m_spdibBuffer(allocer()),
//      m_mutex(papp)
//   {
//
//      ::g_pandroidbitmapview = this;
//
//   }
//
//
//   view_bitmap::~view_bitmap()
//   {
//
//   }
//
//
//   void view_bitmap::create_window_graphics(oswindow interaction_impl, int64_t cxParam, int64_t cyParam, int iStrideParam)
//   {
//
//      synch_lock sl(&m_mutex);
//
//      m_spdibBuffer->create(cxParam, cyParam);
//
//      window_graphics::create_window_graphics(interaction_impl, cxParam, cyParam, m_dib->m_iScan);
//
//   }
//
//
//
//
//   void view_bitmap::destroy_window_graphics()
//   {
//
//      m_spdibBuffer->destroy();
//
//      window_graphics::destroy_window_graphics();
//
//
//   }
//
//
//   ::draw2d::graphics * view_bitmap::on_begin_draw()
//   {
//
//
//
//   }
//
//   //void view_bitmap::update_window(COLORREF * pcolorref, const RECT & rect, int cxParam, int cyParam, int iStride, bool bTransferBuffer)
//   //{
//
//   //   if (width(rect) <= 0 || height(rect) <= 0)
//   //      return;
//
//   //   synch_lock sl(&m_mutex);
//
//   //   ::draw2d::copy_colorref(
//   //      MIN(cxParam, m_spdibBuffer->m_size.cx),
//   //      MIN(cyParam, m_spdibBuffer->m_size.cy), 
//   //      m_spdibBuffer->m_pcolorref,
//   //      m_spdibBuffer->m_iScan, pcolorref, iStride);
//
//   //}
//
//
//} // namespace android


extern "C"
void android_fill_plasma(AndroidBitmapInfo * info, void * pixels, double  t)
{

   if (::aura::system::g_p == NULL)
   {

      output_debug_string("android_fill_plasma : aura_system is null");

      return;

   }

   if (::aura::system::g_p->m_pbasesystem == NULL)
   {
      
      output_debug_string("android_fill_plasma : base_system is null");

      return;

   }

   if (::aura::system::g_p->m_pbasesystem->m_possystemwindow == NULL)
   {

      output_debug_string("android_fill_plasma : os_data is null");

      return;

   }

   if (::aura::system::g_p->m_pbasesystem->m_possystemwindow->m_pui.is_null())
   {

      output_debug_string("android_fill_plasma : window is null");

      return;

   }

   if (::aura::system::g_p->m_pbasesystem->m_possystemwindow->m_pui->m_pimpl == NULL)
   {

      output_debug_string("android_fill_plasma : impl is null");

      return;

   }

   if (::aura::system::g_p->m_pbasesystem->m_possystemwindow->m_pui->m_pimpl->get_window_graphics() == NULL)
   {

      //   output_debug_string("android_fill_plasma : get_window_graphics returned null");

      return;

   }

   auto pbuffer = dynamic_cast <::window_double_buffer *> (::aura::system::g_p->m_pbasesystem->m_possystemwindow->m_pui->m_pimpl->get_window_graphics());

   if(pbuffer == NULL)
   {

      output_debug_string("android_fill_plasma : double_buffer is null");

      return;

   }

   if (pbuffer->m_spdibBuffer2.is_null())
   {

      output_debug_string("android_fill_plasma : dib is null");

      return;

   }

   if (pbuffer->m_spdibBuffer2->m_pcolorref == NULL || pbuffer->m_spdibBuffer2->area() <= 0)
   {

      output_debug_string("android_fill_plasma : dib data is null or empty");

      return;

   }


   synch_lock sl(&pbuffer->m_mutex2);

   ::draw2d::copy_colorref(
      MIN(pbuffer->m_spdibBuffer2->m_size.cx, info->width),
      MIN(pbuffer->m_spdibBuffer2->m_size.cy, info->height),
      (COLORREF *) pixels, 
      info->stride, 
      pbuffer->m_spdibBuffer2->m_pcolorref,
      pbuffer->m_spdibBuffer2->m_iScan);
   
   //output_debug_string("android_fill_plasma OK (area="+::str::from(pbuffer->m_spdibBuffer->area())+")");

}



