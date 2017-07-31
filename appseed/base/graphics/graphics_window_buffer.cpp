#include "framework.h"
#include "base/user/core_user.h"
#include "graphics_window_buffer.h"


window_buffer::window_buffer(::aura::application * papp) :
   object(papp),
   window_graphics(papp)
{

}


window_buffer::~window_buffer()
{

}


void window_buffer::update_window()
{

   update_window(m_spdibBuffer);

}


void window_buffer::update_window(::draw2d::dib * pdib)
{

   UNREFERENCED_PARAMETER(pdib);

}

::draw2d::dib_sp & window_buffer::get_buffer()
{
   
   return m_spdibBuffer;
   
}


::draw2d::graphics * window_buffer::on_begin_draw()
{

   synch_lock sl(m_pmutex);

   if (m_spdibBuffer.is_null())
   {

      //output_debug_string("window_buffer m_spdibBuffer NULL");

      m_spdibBuffer.alloc(allocer());

   }

   if (!(m_pimpl->GetExStyle() & WS_EX_LAYERED))
   {
      rect r;
      m_pimpl->GetWindowRect(&r);
      m_pimpl->m_rectParentClientRequest = r;

   }

   if (m_cx != m_pimpl->m_rectParentClientRequest.size().cx || m_cy != m_pimpl->m_rectParentClientRequest.size().cy)
   {

      create_buffer(m_pimpl->m_rectParentClientRequest.size().cx, m_pimpl->m_rectParentClientRequest.size().cy, m_spdibBuffer->m_iScan);

   }

   if (m_spdibBuffer->host(m_pcolorref, m_iScan, abs(m_bitmapinfo.bmiHeader.biWidth), abs(m_bitmapinfo.bmiHeader.biHeight)))
   {

      m_bDibIsHostingBuffer = true;

   }
   else if(m_spdibBuffer->create(m_pimpl->m_rectParentClientRequest.size()))
   {

      m_bDibIsHostingBuffer = false;

   }
   else
   {

      return NULL;

   }

   return m_spdibBuffer->get_graphics();

}







window_double_buffer::window_double_buffer(::aura::application * papp) :
   object(papp),
   window_graphics(papp),
   window_buffer(papp),
   m_mutex2(papp),
   m_spdibBuffer2(allocer())
{
   
}


window_double_buffer::~window_double_buffer()
{

}


void window_double_buffer::update_window()
{

   synch_lock sl1(m_pmutex);

   synch_lock sl(&m_mutex2);

   m_spdibBuffer2->from(m_spdibBuffer);

}
