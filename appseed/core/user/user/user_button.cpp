#include "framework.h"


namespace user
{


   button::button():
      button(get_app())
   {

   }


   button::button(::aura::application * papp) :
      object(papp),
      ::user::interaction(papp)
   {

      m_estockicon      = stock_icon_none;

      m_estyle          = style_none;

      m_pbitmap         = NULL;
      m_plist           = NULL;
      m_iHover          = -1;
      m_echeck          = check::unchecked;
      m_pschema         = NULL;
      m_iClick          = 0;

   }

   button::~button()
   {
      ::aura::del(m_pbitmap);
      ::aura::del(m_plist);
      set_button_style(style_none);

   }


   void button::install_message_handling(::message::dispatch * pinterface)
   {

      ::user::control::install_message_handling(pinterface);

      USER_MESSAGE_LINK(message_create, pinterface, this, &button::_001OnCreate);

      IGUI_WIN_MSG_LINK(WM_LBUTTONDOWN, pinterface, this, &button::_001OnLButtonDown);
      IGUI_WIN_MSG_LINK(WM_LBUTTONUP, pinterface, this, &button::_001OnLButtonUp);
      IGUI_WIN_MSG_LINK(WM_MBUTTONDOWN, pinterface, this, &button::_001OnMButtonDown);
      IGUI_WIN_MSG_LINK(WM_MBUTTONUP, pinterface, this, &button::_001OnMButtonUp);
      IGUI_WIN_MSG_LINK(WM_MOUSEMOVE, pinterface, this, &button::_001OnMouseMove);
      IGUI_WIN_MSG_LINK(WM_MOUSELEAVE, pinterface, this, &button::_001OnMouseLeave);
      IGUI_WIN_MSG_LINK(WM_KEYDOWN,pinterface,this,&button::_001OnKeyDown);

   }






   void button::_001OnDraw(::draw2d::graphics * pgraphics)
   {

      if(m_estyle == style_push)
      {

         _001OnDrawPush(pgraphics);

      }
      else if(m_estyle == style_list)
      {

         _001OnDrawList(pgraphics);

      }
      else if(m_estyle == style_bitmap)
      {

         _001OnDrawBitmap(pgraphics);

      }
      else
      {

         

         string strText(m_strWindowText);

         rect rectClient;
         GetClientRect(rectClient);


         if(m_puserschemaSchema == NULL)
         {

            if(m_iHover == 0 || Session.m_puiLastLButtonDown == this)
            {

               pgraphics->FillSolidRect(rectClient,ARGB(255,127,127,127));

               pgraphics->set_text_color(ARGB(255,0,100,255));

            }
            else
            {

               pgraphics->FillSolidRect(rectClient,ARGB(255,127,127,127));

               pgraphics->set_text_color(ARGB(255,0,0,0));

            }

         }
         else
         {
            if(m_iHover == 0 || Session.m_puiLastLButtonDown == this)
            {

               //pgraphics->Draw3dRect(rectClient,m_pschema->_001GetColor(color_border_hover),m_pschema->_001GetColor(color_border_hover));

               //rectClient.deflate(1,1);

               pgraphics->FillSolidRect(rectClient, m_puserschemaSchema->_001GetColor(color_button_background_hover));

               pgraphics->set_text_color(m_puserschemaSchema->_001GetColor(color_button_text_hover));

            }
            else
            {

               //pgraphics->Draw3dRect(rectClient,m_pschema->_001GetColor(color_border_normal),m_pschema->_001GetColor(color_border_normal));

               //rectClient.deflate(1,1);

               pgraphics->FillSolidRect(rectClient, m_puserschemaSchema->_001GetColor(color_button_background_normal));

               pgraphics->set_text_color(m_puserschemaSchema->_001GetColor(color_button_text_normal));

            }

         }

         if(m_estockicon == stock_icon_none)
         {

            select_font(pgraphics, font_button, this);

            pgraphics->text_out(m_rectText.left,m_rectText.top,strText);

         }
         else
         {

            //::draw2d::brush_sp brush(allocer());

            //brush->create_solid(pgraphics->get_current_pen()->m_cr);

            //pgraphics->SelectObject(brush);

            ::draw2d::pen_sp pen(allocer());

            pen->m_cr = pgraphics->get_current_brush()->m_cr;

            pen->m_dWidth = 1.0;

            pgraphics->SelectObject(pen);

            class rect rectIcon(rectClient);

            rectIcon.deflate(rectIcon.width() / 4,rectIcon.height() / 4);

            pgraphics->draw_stock_icon(rectIcon,m_estockicon);


         }

      }

   }


   void button::_001OnLButtonDown(signal_details * pobj)
   {
      SCAST_PTR(::message::mouse,pmouse,pobj);

         pobj->previous();

         e_element eelement;

      if(hit_test(pmouse->m_pt, eelement) >= 0)
      {

         if(!simple_process_system_message(pobj,::user::event_button_down))
         {

            Session.m_puiLastLButtonDown = this;

            pmouse->m_bRet = true;

         }



      }

   }

   void button::_001OnMButtonDown(signal_details * pobj)
   {
      SCAST_PTR(::message::mouse, pmouse, pobj);

         pobj->previous();

      e_element eelement;

      if (hit_test(pmouse->m_pt, eelement) >= 0)
      {

         if (!simple_process_system_message(pobj, ::user::event_m_button_down))
         {

            //Session.m_puiLastLButtonDown = this;

            pmouse->m_bRet = true;

         }



      }

   }
   void button::_001OnMButtonUp(signal_details * pobj)
   {
      SCAST_PTR(::message::mouse, pmouse, pobj);

         pobj->previous();

      e_element eelement;

      if (hit_test(pmouse->m_pt, eelement) >= 0)
      {

         if (!simple_process_system_message(pobj, ::user::event_m_button_up))
         {

            //Session.m_puiLastLButtonDown = this;

            pmouse->m_bRet = true;

         }



      }

   }

   bool button::is_pressed()
   {
      return Session.m_puiLastLButtonDown == this;
   }

   void button::_001OnLButtonUp(signal_details * pobj)
   {

      SCAST_PTR(::message::mouse, pmouse, pobj);

      e_element eelement;

      point pt = pmouse->m_pt;

      ScreenToClient(&pt);

      if(hit_test(pmouse->m_pt, eelement) >= 0 && Session.m_puiLastLButtonDown == this)
      {
         Session.m_puiLastLButtonDown = NULL;


         pobj->m_bRet = _001OnClick(pmouse->m_ulFlags, pt);
         if (pobj->m_bRet)
         {
            pmouse->set_lresult(1);
         }
         else
         {

            ::user::control_event ev;
            ev.m_puie = this;
            ev.m_eevent = ::user::event_button_clicked;
            BaseOnControlEvent(&ev);
            pobj->m_bRet = ev.m_bRet;
            if (pobj->m_bRet)
            {
               pmouse->set_lresult(1);
            }
         }

      }

   }

   void button::_001OnMouseMove(signal_details * pobj)
   {

      SCAST_PTR(::message::mouse, pmouse, pobj);

      e_element eelement;

      index iHover = hit_test(pmouse->m_pt, eelement);
      if(iHover != m_iHover)
      {
         index iOldHover = m_iHover;
         m_iHover = iHover;
         RedrawWindow();

         if (iHover >= 0)
         {

            track_mouse_hover();

         }


         if(iOldHover == -1)
         {
            ::user::control_event ev;
            ev.m_puie = this;
            ev.m_eevent = ::user::event_mouse_enter;
            GetParent()->send_message(
               ::message::message_event, 0, (LPARAM)&ev);
//               m_bActionHover = true;
         }
         else if(iHover == -1)
         {
            ::user::control_event ev;
            ev.m_puie = this;
            ev.m_eevent = ::user::event_mouse_leave;
            GetParent()->send_message(
               ::message::message_event, 0, (LPARAM)&ev);
//             m_bActionHover = false;
         }
      }
      pobj->m_bRet = false;

   }


   void button::_001OnMouseLeave(signal_details * pobj)
   {

      SCAST_PTR(::message::base, pbase, pobj);
      index iOldHover = m_iHover;
      m_iHover = -1;
      if(iOldHover >= 0)
      {
         RedrawWindow();
         ::user::control_event ev;
         ev.m_puie = this;
         ev.m_eevent = ::user::event_mouse_leave;
         if(GetParent() != NULL)
         {
            GetParent()->send_message(::message::message_event, 0, (LPARAM)&ev);
         }
      }

      pbase->m_bRet = false;

   }

   index button::hit_test(point pt, e_element & eelement)
   {
      rect rectWindow;
      GetWindowRect(rectWindow);
      if(rectWindow.contains(pt))
      {
         eelement = element_area;
         return 0;
      }
      else
      {
         eelement = element_none;
         return -1;
      }
   }


   ::size button::calc_text_size()
   {

      ::draw2d::memory_graphics pgraphics(allocer());

      if(pgraphics.is_null())
         return size(0, 0);

      select_font(pgraphics, font_button, this);

      string strText(m_strWindowText);

      size size = pgraphics->GetTextExtent(strText);

      ::draw2d::text_metric tm;

      pgraphics->get_text_metrics(&tm);

      ::size sizeTotal;

      sizeTotal.cx = size.cx;

      sizeTotal.cy = tm.tmHeight;

      return sizeTotal;

   }


   void button::ResizeToFit()
   {

      if(m_estyle == style_simple)
      {

            ::draw2d::memory_graphics pgraphics(allocer());

            select_font(pgraphics, font_button, this);

            string str;
            GetWindowText(str);
            size size = pgraphics->GetTextExtent(str);

            rect rect(0,0,0,0);
            rect.right = size.cx + 4;
            rect.bottom = size.cy + 4;

            SetWindowPos(0,0,0,rect.width(),rect.height(),SWP_NOMOVE);


      }
      else if(m_estyle == style_bitmap)
      {

         class size size = m_pbitmap->m_dib->size();

         SetWindowPos(0,0,0,size.cx,size.cy,SWP_NOMOVE | SWP_NOZORDER);

      }
      else
      {

         ::size sizeTotal = calc_text_size();

         sizeTotal.cx += 10 * 2;

         sizeTotal.cy += 5 * 2;

         SetWindowPos(0,0,0,sizeTotal.cx,sizeTotal.cy,SWP_NOMOVE);

      }

   }

   //void button::_001SetButtonText(const char * lpcszText)
   //{
   //   m_istrButtonText = lpcszText;
   //}



   void button::_001SetCheck(check::e_check echeck, ::action::context actioncontext)
   {

      UNREFERENCED_PARAMETER(actioncontext);

      m_echeck = echeck;

   }


   check::e_check button::_001GetCheck()
   {

      return m_echeck;

   }


   void button::_001OnCreate(signal_details * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

      if(m_estyle == style_none)
      {

         set_button_style(style_normal);

      }


   }


   void button::on_layout()
   {

      rect rectClient;

      GetClientRect(rectClient);


      ::user::front_end_schema * pschema = NULL;

      if (m_puserschemaSchema == NULL)
      {

         m_puserschemaSchema = GetTopLevel();

      }

      sp(::user::frame_window) pframewindow = GetTypedParent < ::user::frame_window >();

      if (m_puserschemaSchema == NULL)
      {

         m_puserschemaSchema = Application.userschema();

      }

      if (pschema == NULL)
      {

         pschema = Session.user()->GetUfeSchema();

      }

      if (pschema != NULL)
      {

         m_pschema = &pschema->m_button;

      }


      ::size sizeText = calc_text_size();

      ::rect rect;

      rect.left = rectClient.left + (rectClient.width() - sizeText.cx) / 2;

      rect.top = rectClient.top + (rectClient.height() - sizeText.cy) / 2;

      rect.right = rect.left + sizeText.cx;

      rect.bottom = rect.top + sizeText.cy;

      m_rectText = rect;

   }


   bool button::create_control(class control::descriptor * pdescriptor, index iItem)
   {

      if(!create_window(
         NULL,
         NULL,
         WS_VISIBLE | WS_CHILD,
         pdescriptor->m_rect,
         pdescriptor->m_pform,
         pdescriptor->m_id))
      {
         TRACE("Failed to create control");
         return false;
      }

      return control::create_control(pdescriptor, iItem);


   }


   void button::_002OnDraw(::draw2d::graphics * pgraphics)
   {

      if(m_pschema == NULL)
         return;

      

      rect rectClient;

      GetClientRect(rectClient);

      COLORREF crBk;
      if(!is_window_enabled())
      {
         crBk = m_puserschemaSchema->_001GetColor(color_button_background_disabled);
      }
      else if(is_pressed())
      {
         crBk = m_puserschemaSchema->_001GetColor(color_button_background_press);
      }
      else if(m_iHover >= 0)
      {
         crBk = m_puserschemaSchema->_001GetColor(color_button_background_hover);
      }
      else
      {
         crBk = m_puserschemaSchema->_001GetColor(color_button_background_normal);
      }


      if(::user::button::_001IsTranslucent())
      {
         class imaging & imaging = System.visual().imaging();
         imaging.color_blend(
            pgraphics,
            rectClient,
            crBk,
            127);
      }
      else
      {
         pgraphics->FillSolidRect(rectClient, crBk);
      }


      COLORREF crBorder;
      if(!is_window_enabled())
      {
         crBorder = ARGB(255, 127, 127, 127);
      }
      else if(is_pressed())
      {
         crBorder = ARGB(255, 255, 255, 255);
      }
      else if(m_iHover >= 0)
      {
         crBorder = ARGB(255, 100, 100, 200);
      }
      else
      {
         crBorder = ARGB(255, 10, 10, 100);
      }

      if(m_pschema->m_bBorder)
      {
         pgraphics->Draw3dRect(rectClient, crBorder, crBorder);
      }

//      pgraphics->SetBkMode(TRANSPARENT);

      rectClient.left   += 3;
      rectClient.top    += 3;
      rect rectText = m_rectText;
//      string str = ::str::international::utf8_to_unicode(str);
      if(m_pbitmap->m_dib.is_set())
      {
         if(m_pbitmap->m_dib->m_size.cx > 0 && m_pbitmap->m_dib->m_size.cy > 0)
         {
            rect rectDib;
            rectDib = m_rectText;
            rectDib.bottom = MIN(rectText.top + m_pbitmap->m_dib->m_size.cy,rectText.bottom);
            rectDib.right = MIN(rectText.left + m_pbitmap->m_dib->m_size.cx,rectText.right);
            //m_dib->to(pgraphics, rectDib);
            m_pbitmap->m_dib->bitmap_blend(pgraphics,rectDib);
            rectText.left += m_pbitmap->m_dib->m_size.cx;
         }
      }


      ::draw2d::brush_sp brushText(allocer());


      if(!is_window_enabled())
      {
//         pgraphics->set_text_color(m_pschema->m_crTextDisabled);
         brushText->create_solid(m_puserschemaSchema->_001GetColor(color_button_text_disabled));
      }
      else if(is_pressed())
      {
//         pgraphics->set_text_color(m_pschema->m_crTextPress);
         brushText->create_solid(m_puserschemaSchema->_001GetColor(color_button_text_press));
      }
      else if(m_iHover >= 0)
      {
//         pgraphics->set_text_color(m_pschema->m_crTextHover);
         brushText->create_solid(m_puserschemaSchema->_001GetColor(color_button_text_hover));
      }
      else
      {
//         pgraphics->set_text_color(m_pschema->m_crTextNormal);
         brushText->create_solid(m_puserschemaSchema->_001GetColor(color_button_text_normal));
      }

      pgraphics->SelectObject(brushText);

      string strText(GetWindowText());

      select_font(pgraphics, font_button, this);

      pgraphics->draw_text(strText, rectText, DT_LEFT | DT_TOP);

   }

   bool button::keyboard_focus_is_focusable()
   {

      return is_window_enabled() && IsWindowVisible();

   }


   void button::_001OnKeyDown(signal_details * pobj)
   {

      SCAST_PTR(::message::key,pkey,pobj);

      ::user::e_key iKey = pkey->m_ekey;

      if(iKey == ::user::key_return || iKey == ::user::key_space)
      {

         ::user::control_event ev;
         ev.m_puie = this;
         ev.m_eevent = ::user::event_button_clicked;
         ev.m_pobj = pobj;
         BaseOnControlEvent(&ev);
         pobj->m_bRet = ev.m_bRet;
         if(pobj->m_bRet)
         {
            pkey->set_lresult(1);
         }

      }

   }



   void button::_001OnDrawBitmap(::draw2d::graphics * pgraphics)
   {

      if(!(m_pbitmap->m_dib.is_set() && m_pbitmap->m_dib->area() > 0))
         return;

      ASSERT(m_pbitmap->m_dib.is_set() && m_pbitmap->m_dib->area() > 0); // required

      // use the main bitmap for up, the selected bitmap for down
      ::draw2d::dib * pdib = m_pbitmap->m_dib;

      if(_001GetCheck() == ::check::checked && m_pbitmap->m_dibSel.is_set() && m_pbitmap->m_dibSel->area() > 0)
         pdib = m_pbitmap->m_dibSel;
      else if(m_iHover >= 0 && is_window_enabled() && m_pbitmap->m_dibHover.is_set() && m_pbitmap->m_dibHover->area() > 0)
         pdib = m_pbitmap->m_dibHover;
      else if(Session.get_focus_ui() == this && m_pbitmap->m_dibFocus.is_set() && m_pbitmap->m_dibFocus->area() > 0)
         pdib = m_pbitmap->m_dibFocus;   // third image for focused
      else if(!is_window_enabled() && m_pbitmap->m_dibDisabled.is_set() && m_pbitmap->m_dibDisabled->area() > 0)
         pdib = m_pbitmap->m_dibDisabled;   // last image for disabled

      pgraphics->from(pdib->m_size, pdib->get_graphics(), SRCCOPY);

   }


   void button::_001OnDrawPush(::draw2d::graphics * pgraphics)
   {

      

      //   int32_t iOriginalBkMode = pgraphics->GetBkMode();
      //   pgraphics->SetBkMode(TRANSPARENT);
      rect rectClient;
      GetClientRect(rectClient);

      //   state::state state = _001GetState();
      //   const int32_t ihilite = 0x0004; // hilighted
      //   const int32_t ifocus = 0x0008; // focus
      //   const int32_t iPushState = state & 3;

      color color;

      if(m_pschema == NULL)
      {

         color.set_rgb(ARGB(255,127,127,127));

      }
      else
      {

         color.set_rgb(m_pschema->_001GetColor(::user::color_button_background_normal));

      }

      COLORREF cr;

      //if(m_pcolorschema != NULL)
      //{
      //   if(is_pushed())
      //   {
      //      color.hls_rate(m_pcolorschema->m_hlsRatePush);
      //   }
      //   if(m_bHover)
      //   {
      //      color.hls_rate(m_pcolorschema->m_hlsRateHover);
      //   }
      //}
      //else
      {
         if(_001GetCheck() == ::check::checked)
         {
            color.hls_rate(0.0,- 0.2,- 0.3);
         }
         if(m_iHover >= 0)
         {
            color.hls_rate(0.0,0.2,0.0);
         }
      }
      cr = color;
      //if(m_bTransitionEffect)
      //{
      //   class color color;
      //   color.set_rgb(cr);
      //   cr = color;

      //   double dH,dL,dS;
      //   color.get_hls(dH,dL,dS);
      //   color.set_hls(dH,dL,dS);
      //   cr = color;
      //   double dRate;
      //   if(m_iStep <= m_iStepCount / 2)
      //   {
      //      dRate = m_iStep * 2.0 / m_iStepCount;
      //   }
      //   else
      //   {
      //      dRate = (m_iStepCount - m_iStep) * 2.0 / m_iStepCount;
      //   }
      //   if(is_pushed())
      //   {
      //      dL = dL * (1.0 - dRate);
      //   }
      //   else
      //   {
      //      dL = (1.0 - dL) * dRate + dL;
      //   }
      //   color.set_hls(dH,dL,dS);
      //   cr = color.get_rgb();
      //}

      class color colorExt1TL;
      class color colorExt2TL;
      class color colorExt1BR;
      class color colorExt2BR;

      colorExt1TL.set_rgb(cr);
      colorExt2TL.set_rgb(cr);
      colorExt1BR.set_rgb(cr);
      colorExt2BR.set_rgb(cr);
      if(_001GetCheck() == ::check::checked)
      {
         colorExt1TL.hls_rate(0.0,- 0.5,0.0);
         colorExt2TL.hls_rate(0.0,- 0.2,0.0);
         colorExt2BR.hls_rate(0.0,0.2,0.0);
         colorExt1BR.hls_rate(0.0,0.85,0.0);
      }
      else
      {
         colorExt1TL.hls_rate(0.0,0.85,0.0);
         colorExt2TL.hls_rate(0.0,0.2,0.0);
         colorExt2BR.hls_rate(0.0,- 0.2,0.0);
         colorExt1BR.hls_rate(0.0,- 0.5,0.0);
      }

      class imaging & imaging = System.visual().imaging();

      rect rect = rectClient;
      imaging.color_blend_3dRect(pgraphics,rect,colorExt1TL,215,colorExt1BR,215);
      rect.deflate(1,1,1,1);
      imaging.color_blend_3dRect(pgraphics,rect,colorExt1TL,210,colorExt1BR,210);
      rect.deflate(1,1,1,1);
      imaging.color_blend_3dRect(pgraphics,rect,colorExt2TL,205,colorExt2BR,205);
      rect.deflate(1,1,1,1);
      imaging.color_blend_3dRect(pgraphics,rect,colorExt2TL,200,colorExt2BR,200);
      rect.deflate(1,1,1,1);
      imaging.color_blend(pgraphics,rect.left,rect.top,rect.width(),rect.height(),cr,200);
      rect.deflate(1,1,1,1);
      int32_t x1 = rect.left;
      int32_t x2 = x1 + rect.width() / 3;
      rect.left = x1;
      rect.right = x2;
      rect.bottom = rect.top + 5;
      ::draw2d::pen_sp pen(pgraphics,1,colorExt1TL);
      pgraphics->SelectObject(pen);
      imaging.color_blend_3dRect(pgraphics,rect,colorExt1TL,220,colorExt1BR,220);




      //   pgraphics->SetBkMode(iOriginalBkMode);

   }


   void button::set_button_style(e_style estyle)
   {

      if(m_estyle == estyle)
         return;

      on_exit_button_style(m_estyle);

      m_estyle = estyle;

      on_enter_button_style(estyle);

   }


   void button::on_enter_button_style(e_style estyle)
   {

      if(estyle == style_bitmap)
      {

         m_pbitmap = new bitmap();

      }
      else if(estyle == style_list)
      {

         m_plist = new list();

         m_plist->m_pimagelistNormal         = NULL;
         m_plist->m_pimagelistItemHover      = NULL;
         m_plist->m_pimagelistSubItemHover   = NULL;

      }
      else if(estyle == style_push)
      {

         SetTimer(16384,100,NULL);

      }


   }


   void button::on_exit_button_style(e_style estyle)
   {

      if(estyle == style_bitmap)
      {

         ::aura::del(m_pbitmap);

      }
      else if(estyle == style_bitmap)
      {

         ::aura::del(m_plist);

      }
      else if(estyle == style_push)
      {

         KillTimer(16384);

      }

   }





   bool button::LoadBitmaps(::var var,::var varSel,::var varFocus,::var varDisabled,::var varHover)
   {

      set_button_style(style_bitmap);

      if(!var.is_empty())
      {

         m_pbitmap->m_dib.alloc(allocer());

         m_pbitmap->m_dib.load_from_file(var);

      }

      if(!varSel.is_empty())
      {

         m_pbitmap->m_dibSel.alloc(allocer());

         m_pbitmap->m_dibSel.load_from_file(varSel);

      }


      if(!varFocus.is_empty())
      {

         m_pbitmap->m_dibFocus.alloc(allocer());

         m_pbitmap->m_dibFocus.load_from_file(varFocus);

      }


      if(!varDisabled.is_empty())
      {

         m_pbitmap->m_dibDisabled.alloc(allocer());

         m_pbitmap->m_dibDisabled.load_from_file(varDisabled);

      }


      if(!varHover.is_empty())
      {

         m_pbitmap->m_dibHover.alloc(allocer());

         m_pbitmap->m_dibHover.load_from_file(varHover);

      }

      return m_pbitmap->m_dib.is_set() && m_pbitmap->m_dib->area() > 0;

   }






   void button::pre_translate_message(signal_details * pobj)
      {

         // Relay events from this button to the tool tip tool handler
         BaseToolTipRelayEvent(pobj);

         return ::user::control::pre_translate_message(pobj);

      }


      void button::BaseToolTipGetRect(LPRECT lprect)
      {
         // use window client rect as the tool rect
         GetClientRect(lprect);
      }

      int32_t button::BaseToolTipGetIndex()
      {
         // use window dialog control id as the index
         return (int32_t)GetDlgCtrlId();
      }






   void button::_001OnDrawList(::draw2d::graphics * pgraphics)
   {
      rect rectClient;
      bool bItemHover;
      bool bSubItemHover;

      

      ::aura::draw_context * pdrawcontext = pgraphics->::core::simple_chain < ::aura::draw_context >::get_last();

      if(pdrawcontext != NULL)
      {
         rectClient     = pdrawcontext->m_rectClient;
         bItemHover     = pdrawcontext->m_bListItemHover;
         bSubItemHover  = pdrawcontext->m_bListSubItemHover;
      }
      else
      {
         if(get_form() != NULL)
            return;
         GetClientRect(rectClient);
         bItemHover     = get_hover() >= 0;
         bSubItemHover  = get_hover() >= 0;
      }

      point pt = rectClient.top_left();
      pt += size(1,1);

      if(bSubItemHover)
      {
         pgraphics->Draw3dRect(
            rectClient,
            RGB(255,255,255),
            RGB(155,155,105));

         if(m_plist->m_pimagelistSubItemHover != NULL)
         {
            m_plist->m_pimagelistSubItemHover->draw(
               pgraphics,
               m_plist->m_iImageSubItemHover,
               pt,
               0);
         }
         else if(m_plist->m_pimagelistItemHover != NULL)
         {
            m_plist->m_pimagelistItemHover->draw(
               pgraphics,
               m_plist->m_iImageItemHover,
               pt,
               0);
         }
         else if(m_plist->m_pimagelistNormal != NULL)
         {
            m_plist->m_pimagelistNormal->draw(
               pgraphics,
               m_plist->m_iImageNormal,
               pt,
               0);
         }
      }
      else if(bItemHover)
      {
         if(m_plist->m_pimagelistItemHover != NULL)
         {
            m_plist->m_pimagelistItemHover->draw(
               pgraphics,
               m_plist->m_iImageItemHover,
               pt,
               0);
         }
         else if(m_plist->m_pimagelistSubItemHover != NULL)
         {
            m_plist->m_pimagelistSubItemHover->draw(
               pgraphics,
               m_plist->m_iImageSubItemHover,
               pt,
               0);
         }
         else if(m_plist->m_pimagelistNormal != NULL)
         {
            m_plist->m_pimagelistNormal->draw(
               pgraphics,
               m_plist->m_iImageNormal,
               pt,
               0);
         }
      }
      else
      {
         if(m_plist->m_pimagelistNormal != NULL)
         {
            m_plist->m_pimagelistNormal->draw(
               pgraphics,
               m_plist->m_iImageNormal,
               pt,
               0);
         }
         else if(m_plist->m_pimagelistItemHover != NULL)
         {
            m_plist->m_pimagelistItemHover->draw(
               pgraphics,
               m_plist->m_iImageItemHover,
               pt,
               0);
         }
         else if(m_plist->m_pimagelistSubItemHover != NULL)
         {
            m_plist->m_pimagelistSubItemHover->draw(
               pgraphics,
               m_plist->m_iImageSubItemHover,
               pt,
               0);
         }
      }

   }


   void button::BaseToolTipRelayEvent(class signal_details *)
   {
   }


   bool button::is_custom_draw()
   {

      return m_estyle == style_list;

   }


   bool button::has_action_hover()
   {

      return m_iHover >= 0;

   }

   void button::set_stock_icon(e_stock_icon eicon)
   {

      m_estockicon = eicon;

   }


   e_stock_icon button::get_stock_icon()
   {

      return m_estockicon;

   }


} // namespace user




