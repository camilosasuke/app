#include "framework.h"


namespace user
{


   orto_button::orto_button(::aura::application * papp) :
      object(papp),
      ::user::button(papp),
      ::user::wndfrm::frame::control_box_button(papp),
      m_spregion(allocer())
   {


   }

   orto_button::~orto_button()
   {
   }


   bool orto_button::pre_create_window(::user::create_struct& cs)
   {
      //   cs.style |= BS_OWNERDRAW;
      return ::user::button::pre_create_window(cs);
   }



   void orto_button::_001OnDraw(::draw2d::graphics * pgraphics)
   {



      rect rectClient;

      ::user::interaction::GetClientRect(rectClient);

      if (rectClient.area() <= 0)
         return;

      COLORREF crText;

      if (!is_window_enabled())
      {

         pgraphics->FillSolidRect(rectClient, ARGB(255, 84, 84, 77));

         crText = ARGB(255, 49, 49, 23);

      }
      else if (m_iHover >= 0)
      {

         pgraphics->FillSolidRect(rectClient, ARGB(184, 49, 49, 23));

         crText = ARGB(255, 255, 255, 255);

      }
      else if (Session.get_keyboard_focus() == this)
      {

         pgraphics->FillSolidRect(rectClient, ARGB(255, 255, 250, 184));

         crText = ARGB(255, 255, 255, 255);

      }
      else
      {

         crText = ARGB(255, 243, 243, 233);

      }

      if (m_estockicon == stock_icon_none)
      {

         string str;

         get_window_text(str);

         select_font(pgraphics, font_button, this);

         pgraphics->set_text_color(crText);

         pgraphics->draw_text(str, rectClient, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

      }
      else
      {

         ::draw2d::brush_sp brush(allocer());

         brush->create_solid(crText);

         pgraphics->SelectObject(brush);

         ::draw2d::pen_sp pen(allocer());

         pen->m_dWidth = 1.0;

         pen->m_cr = crText;

         pen->m_bUpdated = false;

         pgraphics->SelectObject(pen);

         class rect rectIcon(rectClient);

         rectIcon.deflate(rectIcon.width() / 6, rectIcon.height() / 6);

         pgraphics->draw_stock_icon(rectIcon, m_estockicon);


      }

   }


   void orto_button::_001OnShowWindow(signal_details * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

   }

   void orto_button::_001OnLButtonDown(signal_details * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

   }

   void orto_button::_001OnLButtonUp(signal_details * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

      //RedrawWindow();

   }

   LRESULT orto_button::OnAppForwardSyncMessage(WPARAM wParam, LPARAM lParam)
   {
      LPMESSAGE lpmsg = (LPMESSAGE)lParam;
      if (wParam == WM_MOUSEMOVE)
      {
         point ptCursor = lpmsg->pt;
         rect rectClient;
         ::user::interaction::GetClientRect(rectClient);
         ::user::interaction::ClientToScreen(rectClient);

         if (rectClient.contains(ptCursor))
         {
            RedrawWindow();
         }
         else
         {
            RedrawWindow();
         }
      }
      return 0;
   }

   void orto_button::on_layout()
   {

      UpdateWndRgn();

   }


   void orto_button::_001OnTimer(::timer * ptimer)
   {

      ::user::wndfrm::frame::control_box_button::_001OnTimer(ptimer);
      // TODO: add your message handler code here and/or call default
      if (ptimer->m_nIDEvent == TimerCheckFocus)
      {
         /*if(m_bFocus)
         {
         point ptCursor;
         Session.get_cursor_pos(&ptCursor);
         rect rectClient;
         ::user::interaction::GetClientRect(rectClient);
         ::user::interaction::ClientToScreen(rectClient);

         if(!rectClient.contains(ptCursor))
         {
         KillTimer(ptimer->m_nIDEvent);
         m_bFocus = false;
         RedrawWindow();
         }
         }
         else
         {
         KillTimer(ptimer->m_nIDEvent);
         }*/
      }
      else
      {

         //      sp(::user::interaction) pwnd = ::user::interaction::GetParent();


         rect rect;
         ::user::interaction::GetClientRect(rect);
      }


   }

   void orto_button::UpdateWndRgn()
   {

      rect rectClient;

      ::user::interaction::GetClientRect(rectClient);

      m_spregion->create_oval(rectClient);


   }

   void orto_button::install_message_handling(::message::dispatch *pinterface)
   {
      ::user::button::install_message_handling(pinterface);
      IGUI_WIN_MSG_LINK(WM_SHOWWINDOW, pinterface, this, &orto_button::_001OnShowWindow);
      IGUI_WIN_MSG_LINK(WM_LBUTTONDOWN, pinterface, this, &orto_button::_001OnLButtonDown);
      IGUI_WIN_MSG_LINK(WM_LBUTTONUP, pinterface, this, &orto_button::_001OnLButtonUp);
      IGUI_WIN_MSG_LINK(WM_SIZE, pinterface, this, &orto_button::_001OnSize);
      //   //IGUI_WIN_MSG_LINK(WM_TIMER,pinterface,this,&orto_button::_001OnTimer);
   }



   index orto_button::hit_test(point point, e_element & eelement)
   {

      if (m_spregion.is_null())
      {

         eelement = element_none;
         return -1;

      }

      if (m_spregion->get_os_data() == NULL)
      {

         eelement = element_none;
         return -1;

      }

      ScreenToClient(&point);

      if (!m_spregion->contains(point))
      {

         eelement = element_none;
         return -1;

      }

      eelement = element_client;
      return 0;

   }


} // namespace user













