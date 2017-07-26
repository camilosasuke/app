#include "framework.h"
#include "android.h"
#include "android_window_android.h"

namespace android
{

   window_buffer::window_buffer(::aura::application * papp) :
      object(papp),
      window_graphics(papp),
      ::window_buffer(papp)
   {

      //m_cairoSource = NULL;
      //m_cairosurfaceSource = NULL;

   }


   window_buffer::~window_buffer()
   {

   }


   void window_buffer::create_window_graphics_(int64_t cxParam, int64_t cyParam, int iStrideParam)
   {

      destroy_window_graphics_();

      synch_lock sl(m_pimpl->m_pui->m_pmutex);

      int w;

      int h;

      w = ANativeWindow_getWidth(m_pimpl->m_oswindow->m_engine.app->window);

      h = ANativeWindow_getHeight(m_pimpl->m_oswindow->m_engine.app->window);

      ANativeWindow_setBuffersGeometry(m_pimpl->m_oswindow->m_engine.app->window, w, h, WINDOW_FORMAT_RGBA_8888);

      ::window_graphics::create_window_graphics_(cxParam, cyParam, w * 4);

   }





   void window_buffer::destroy_window_graphics_()
   {

      ::window_graphics::destroy_window_graphics_();


   }


   void window_buffer::update_window(::draw2d::dib * pdib)
   {

      ANativeWindow_Buffer buffer;

      ARect r;

      r.left = 0;
      r.top = 0;
      r.right = pdib->m_size.cx;
      r.bottom = pdib->m_size.cy;

      ZERO(buffer);

      ANativeWindow_lock(m_pimpl->m_oswindow->m_engine.app->window, &buffer, &r);

      /**
      * Unlock the window's drawing surface after previously locking it,
      * posting the new buffer to the display.
      */


      ::draw2d::copy_colorref(pdib->m_size.cx, pdib->m_size.cy, (COLORREF *)buffer.bits, pdib->m_size.cx * sizeof(COLORREF), pdib->m_pcolorref, pdib->m_iScan);

      ANativeWindow_unlockAndPost(m_pimpl->m_oswindow->m_engine.app->window);

   }


} // namespace android




