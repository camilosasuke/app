#include "framework.h"


MetaControlBox::MetaControlBox(::aura::application * papp) :
object(papp),
::user::interaction(papp),
::user::wndfrm::frame::control_box(papp)
{
   

}


MetaControlBox::~MetaControlBox()
{

}


void MetaControlBox::_001OnNcDraw(::draw2d::graphics * pgraphics)
{


}

void MetaControlBox::_001OnDraw(::draw2d::graphics * pgraphics)
{

   if(GetTopLevel()->frame_is_transparent() && GetTopLevel() != GetActiveWindow())
   {

      return;

   }

   rect rectClient;

   GetClientRect(rectClient);

   if (rectClient.area() <= 0)
      return;

   

   pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

   COLORREF crBackground;

   if(GetTopLevel()->frame_is_transparent())
   {

      crBackground = ARGB(84,argb_get_r_value(m_crBackground),argb_get_g_value(m_crBackground),argb_get_b_value(m_crBackground));

   }
   else
   {

      crBackground = m_crBackground;

   }

   pgraphics->FillSolidRect(rectClient, crBackground);

}


void MetaControlBox::_001OnShowWindow(signal_details * pobj)
{

   UNREFERENCED_PARAMETER(pobj);

}

void MetaControlBox::_001OnLButtonDown(signal_details * pobj)
{

   UNREFERENCED_PARAMETER(pobj);

}

void MetaControlBox::_001OnLButtonUp(signal_details * pobj)
{

   UNREFERENCED_PARAMETER(pobj);

   RedrawWindow();

}



void MetaControlBox::_001OnTimer(::timer * ptimer)
{

   ::user::wndfrm::frame::control_box::_001OnTimer(ptimer);

}


void MetaControlBox::install_message_handling(::message::dispatch *pinterface)
{
   
   ::user::wndfrm::frame::control_box::install_message_handling(pinterface);

   IGUI_WIN_MSG_LINK(WM_SHOWWINDOW, pinterface, this, &MetaControlBox::_001OnShowWindow);
   IGUI_WIN_MSG_LINK(WM_LBUTTONDOWN, pinterface, this, &MetaControlBox::_001OnLButtonDown);
   IGUI_WIN_MSG_LINK(WM_LBUTTONUP, pinterface, this, &MetaControlBox::_001OnLButtonUp);
   IGUI_WIN_MSG_LINK(WM_SIZE, pinterface, this, &MetaControlBox::_001OnSize);
//   //IGUI_WIN_MSG_LINK(WM_TIMER, pinterface, this, &MetaControlBox::_001OnTimer);

}



