//#include "framework.h" // from "base/user/user.h"


namespace user
{


   schema::schema()
   {

      m_plibrary = NULL;
      m_puserschemaSchema = NULL;

   }


   schema::~schema()
   {
      
      ::aura::del(m_plibrary);

   }


   bool schema::get_color(COLORREF & cr, e_color ecolor)
   {

      if (m_puserschemaSchema != NULL && m_puserschemaSchema != this)
      {

         if (m_puserschemaSchema->get_color(cr, ecolor))
            return true;

      }

      if (get_parent_user_schema() != NULL)
      {

         if (get_parent_user_schema()->get_color(cr, ecolor))
            return true;

      }

      if (m_pauraapp != NULL && this != m_pauraapp->m_pbasesession &&m_pauraapp->m_pbasesession != NULL && m_pauraapp->m_pbasesession != m_puserschemaSchema && m_pauraapp->m_pbasesession->m_puserschemasimple != m_puserschemaSchema)
      {

         if (m_pauraapp->m_pbasesession->get_color(cr, ecolor))
            return true;

      }

      return false;

   }

   bool schema::get_font(::draw2d::font_sp & spfont, e_font efont, ::user::interaction * pui)
   {

      if (get_parent_user_schema() != NULL)
      {

         if (get_parent_user_schema()->get_font(spfont, efont, pui))
            return true;

      }

      if (m_puserschemaSchema != NULL && m_puserschemaSchema != this)
      {

         if (m_puserschemaSchema->get_font(spfont, efont, pui))
            return true;

      }

      if (m_pauraapp != NULL && this != m_pauraapp->m_pbasesession && m_pauraapp->m_pbasesession != NULL && m_pauraapp->m_pbasesession != m_puserschemaSchema && m_pauraapp->m_pbasesession->m_puserschemasimple != m_puserschemaSchema)
      {

         if (m_pauraapp->m_pbasesession->get_font(spfont, efont, pui))
            return true;

      }

      return false;

   }


   bool schema::get_translucency(ETranslucency & etranslucency)
   {

      if (m_puserschemaSchema != NULL && m_puserschemaSchema != this)
      {

         if (m_puserschemaSchema->get_translucency(etranslucency))
            return true;

      }

      if (get_parent_user_schema() != NULL)
      {

         if (get_parent_user_schema()->get_translucency(etranslucency))
            return true;

      }

      if (m_pauraapp != NULL && this != m_pauraapp->m_pbasesession && m_pauraapp->m_pbasesession != NULL && m_pauraapp->m_pbasesession != m_puserschemaSchema && m_pauraapp->m_pbasesession->m_puserschemasimple != m_puserschemaSchema)
      {

         if (m_pauraapp->m_pbasesession->get_translucency(etranslucency))
            return true;

      }

      return false;

   }


   bool schema::on_ui_event(e_event eevent, e_object eobject, ::user::interaction * pui)
   {

      if (m_puserschemaSchema != NULL && m_puserschemaSchema != this)
      {

         if (m_puserschemaSchema->on_ui_event(eevent, eobject, pui))
            return true;

      }

      if (get_parent_user_schema() != NULL)
      {

         if (get_parent_user_schema()->on_ui_event(eevent, eobject, pui))
            return true;

      }

      if (m_pauraapp != NULL && this != m_pauraapp->m_pbasesession && m_pauraapp->m_pbasesession != NULL && m_pauraapp->m_pbasesession != m_puserschemaSchema && m_pauraapp->m_pbasesession->m_puserschemasimple != m_puserschemaSchema)
      {

         if (m_pauraapp->m_pbasesession->on_ui_event(eevent, eobject, pui))
            return true;

      }

      return false;
   }

   COLORREF schema::_001GetColor(e_color ecolor, COLORREF crDefault)
   {

      COLORREF cr;

      if (get_color(cr, ecolor))
         return cr;

      return crDefault;

   }

   COLORREF schema::_001GetColor(e_color ecolor)
   {

      COLORREF cr;

      if (get_color(cr, ecolor))
         return cr;

      if (ecolor == color_text)
      {

         return ARGB(255, 0, 0, 0);

      }
      else if (ecolor == color_text_selected)
      {

         return ARGB(255, 255, 255, 255);

      }
      else if (ecolor == color_text_highlight)
      {

         return ARGB(255, 55, 105, 255);

      }
      else if (ecolor == color_text_selected_highlight)
      {

         return ARGB(255, 102, 153, 255);

      }
      else if (ecolor == color_background)
      {

         if (_001IsTranslucent())
         {

            return ARGB(60, 255, 255, 255);

         }
         else
         {

            return ARGB(255, 255, 255, 255);

         }

      }
      else if (ecolor == color_face)
      {

         if (_001IsTranslucent())
         {

            return ARGB(120, 0, 0, 0);

         }
         else
         {

            return ARGB(255,80, 80, 80);

         }

      }
      else if (ecolor == color_face_lite)
      {

         if (_001IsTranslucent())
         {

            return ARGB(80, 0, 0, 0);

         }
         else
         {

            return ARGB(255, 120, 120, 120);

         }

      }
      else if (ecolor == color_background_selected)
      {

         if (_001IsTranslucent())
         {

            return ARGB(184, 0, 0, 127);

         }
         else
         {

            return ARGB(255, 0, 0, 127);

         }

      }
      else if (ecolor == color_background_selected)
      {

         if (_001IsTranslucent())
         {

            return ARGB(184, 0, 0, 127);

         }
         else
         {

            return ARGB(255, 0, 0, 127);

         }

      }
      else if (ecolor == color_border_normal)
      {

         return ARGB(255, 84, 84, 77);

      }
      else if (ecolor == color_border_hover)
      {

         return ARGB(255, 84, 184, 77);

      }
      else if (ecolor == color_text_hover)
      {

         return ARGB(255, 84, 184, 255);

      }
      else if (ecolor == color_list_header)
      {

         return ARGB(255, 80, 80, 80);

      }
      else if (ecolor == color_background_normal)
      {

         return ARGB(255, 255, 255, 255);

      }
      else if (ecolor == color_text_normal)
      {

         return ARGB(255, 0, 0, 0);

      }
      else if (ecolor == color_background_hover)
      {

         return ARGB(255, 255, 255, 255);

      }
      else if (ecolor == color_text_hover)
      {

         return ARGB(255, 0, 0, 0);

      }
      else if (ecolor == color_list_header_background)
      {

         return ARGB(80, 0, 0, 0);

      }
      else if (ecolor == color_list_item_background)
      {

         return ARGB(127, 0, 0, 0);

      }
      else if (ecolor == color_button_background_disabled)
      {

         return _001GetColor(color_background_disabled);

      }
      else if (ecolor == color_button_background_hover)
      {

         return _001GetColor(color_background_hover);

      }
      else if (ecolor == color_button_background_normal)
      {

         return _001GetColor(color_background_normal);

      }
      else if (ecolor == color_button_background_press)
      {

         return _001GetColor(color_background_press);

      }
      else if (ecolor == color_button_text_disabled)
      {

         return _001GetColor(color_text_disabled);

      }
      else if (ecolor == color_button_text_hover)
      {

         return _001GetColor(color_text_hover);

      }
      else if (ecolor == color_button_text_normal)
      {

         return _001GetColor(color_text_normal);

      }
      else if (ecolor == color_button_text_press)
      {

         return _001GetColor(color_text_press);

      }
      else if (ecolor == color_edit_background)
      {

         return ARGB(128, 255, 255, 255);

      }
      else if (ecolor == color_edit_text)
      {

         return ARGB(128, 0, 0, 0);

      }
      else if (ecolor == color_edit_text_selected)
      {

         return ARGB(128, 255, 255, 255);

      }
      else if (ecolor == color_edit_background_selected)
      {

         return ARGB(128, 0, 0, 128);

      }
      else if(ecolor == color_split_layout_background)
      {

         return ARGB(128, 255, 255, 255);

      }
      else if (ecolor == color_list_background)
      {

         return ARGB(128, 255, 255, 255);

      }
      else if (ecolor == color_toolbar_background)
      {

         return ARGB(128, 255, 255, 255);

      }
      else if (ecolor == color_list_header_separator)
      {

         return ARGB(128, 0, 0, 0);

      }

      throw invalid_argument_exception(::get_thread_app());

   }


   ETranslucency schema::_001GetTranslucency(ETranslucency etranslucencyDefault)
   {

      ETranslucency etranslucency = TranslucencyUndefined;

      if (get_translucency(etranslucency))
      {

         if (etranslucency != TranslucencyUndefined)
         {

            return etranslucency;

         }

      }

      return etranslucencyDefault;

   }



   bool schema::_001IsTranslucent()
   {

      ETranslucency etranslucency = _001GetTranslucency();

      return etranslucency == TranslucencyPresent || etranslucency == TranslucencyTotal;

   }


   bool schema::_001IsBackgroundBypass()
   {

      return _001GetTranslucency() == TranslucencyTotal;

   }


   bool schema::_001HasTranslucency()
   {

      ETranslucency etranslucency = _001GetTranslucency();

      return etranslucency == TranslucencyPresent || etranslucency == TranslucencyTotal;

   }


   bool schema::_001IsTransparent()
   {

      return _001GetTranslucency() == TranslucencyTotal;

   }


   ::user::schema * schema::get_user_schema()
   {

      if (m_puserschemaSchema != NULL)
      {

         return m_puserschemaSchema;

      }

      return get_parent_user_schema();

   }


   ::user::schema * schema::get_parent_user_schema()
   {

      return NULL;

   }


   bool schema::select_layered_frame()
   {



      m_puserschemaSchema = System.m_pschemaLayeredFrame;

      return true;

   }



   bool schema::select_text_color(::draw2d::graphics * pgraphics, e_color ecolor)
   {

      COLORREF crText;

      if (!get_color(crText, ecolor))
         return false;

      return pgraphics->set_text_color(crText);

   }


   bool schema::select_font(::draw2d::graphics * pgraphics, e_font efont, ::user::interaction * pui)
   {

      ::draw2d::font_sp spfont;

      if (!get_font(spfont, efont, pui))
      {

         spfont.alloc(allocer());

         if (!spfont->create_point_font(FONT_SANS, 12.0))
         {

            return false;

         }

      }

      if (spfont.is_null())
         return false;

      return pgraphics->set_font(spfont);

   }


   bool schema::simple_ui_draw_focus_rect(::user::interaction * pui, ::draw2d::graphics * pgraphics)
   {

      if (m_puserschemaSchema != NULL && m_puserschemaSchema != this)
      {

         if (m_puserschemaSchema->simple_ui_draw_focus_rect(pui, pgraphics))
            return true;

      }



      bool bError = pui->m_ptooltip.is_set()
         && pui->m_ptooltip->IsWindowVisible()
         && pui->get_tooltip()->m_bError;

      rect rectClient;

      pui->::user::interaction::GetClientRect(rectClient);

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

      int32_t iStyle = 1;

      rectClient.left--;
      rectClient.top--;

      if (m_pauraapp != NULL && (Session.get_keyboard_focus() == pui || pui->has_action_hover()))
      {

         ::draw2d::brush_sp brush(allocer());

         if (pui->has_action_hover() && !pui->has_text_input())
         {

            brush->create_solid(ARGB(184, 150, 230, 180));

            pgraphics->SelectObject(brush);

            pgraphics->FillRectangle(rectClient);

         }

         if (iStyle == 1)
         {

            bool bHover = pui->has_action_hover();

            if (pui->has_action_hover())
            {

               ::draw2d::pen_sp pen(pgraphics, 1.0, pui->get_action_hover_border_color());

               pgraphics->DrawRect(rectClient, pen);

            }
            else
            {

               //::draw2d::pen_sp pen(pgraphics, 1.0, ARGB(255, 108, 149, 255));
               ::draw2d::pen_sp pen(pgraphics, 1.0, ARGB(184, 84, 84, 77));

               pgraphics->DrawRect(rectClient, pen);

            }


            if (Session.get_keyboard_focus() == pui)
            {

               rectClient.left--;
               rectClient.right++;
               rectClient.top--;
               rectClient.bottom++;

               {

                  //::draw2d::pen_sp pen(pgraphics, 1.0, ARGB(255, 108, 149, 255));
                  ::draw2d::pen_sp pen(pgraphics, 1.0, bError ? ARGB(195, 255, 130, 120) : bHover? ARGB(220, 120, 190, 255) : ARGB(220, 150, 190, 235));

                  pgraphics->DrawRect(rectClient, pen);

               }

               rectClient.left--;
               rectClient.right++;
               rectClient.top--;
               rectClient.bottom++;

               {

                  /*::draw2d::path_sp pathRound(allocer());

                  pathRound->begin_figure(true, ::draw2d::fill_mode_winding);

                  pathRound->add_round_rect(rectClient, 1 * 2);

                  pathRound->end_figure(true);*/

                  //::draw2d::pen_sp pen(pgraphics, 1.0, ARGB(84, 108, 149, 255));
                  ::draw2d::pen_sp pen(pgraphics, 1.0, bError ? ARGB(155, 255, 150, 140) : bHover?ARGB(200, 140, 200, 255 ) : ARGB(200, 150, 210, 235));

                  pgraphics->SelectObject(pen);

                  pgraphics->DrawRect(rectClient, pen);

               }

               rectClient.left--;
               rectClient.right++;
               rectClient.top--;
               rectClient.bottom++;

               {

                  ::draw2d::path_sp pathRound(allocer());

                  pathRound->begin_figure(true, ::draw2d::fill_mode_winding);

                  pathRound->add_round_rect(rectClient, 1 * 2);

                  pathRound->end_figure(true);

                  //::draw2d::pen_sp pen(pgraphics, 1.0, ARGB(72, 108, 149, 255));
                  ::draw2d::pen_sp pen(pgraphics, 1.0, bError ? ARGB(135, 255, 170, 160) : bHover ? ARGB(160, 160, 210, 255) : ARGB(160, 180, 220, 255));

                  pgraphics->SelectObject(pen);

                  pgraphics->DrawRect(rectClient, pen);

               }

               rectClient.left--;
               rectClient.right++;
               rectClient.top--;
               rectClient.bottom++;

               {

                  ::draw2d::path_sp pathRound(allocer());

                  pathRound->begin_figure(true, ::draw2d::fill_mode_winding);

                  pathRound->add_round_rect(rectClient, 1 * 2);

                  pathRound->end_figure(true);

                  //::draw2d::pen_sp pen(pgraphics, 1.0, ARGB(60, 108, 149, 255));
                  ::draw2d::pen_sp pen(pgraphics, 1.0, bError ? ARGB(105, 255, 190, 180) : bHover ? ARGB(120, 180, 220, 255):  ARGB(120, 200, 230, 235));

                  pgraphics->SelectObject(pen);

                  pgraphics->DrawRect(rectClient, pen);

               }


               rectClient.left--;
               rectClient.right++;
               rectClient.top--;
               rectClient.bottom++;

               {

                  ::draw2d::path_sp pathRound(allocer());

                  pathRound->begin_figure(true, ::draw2d::fill_mode_winding);

                  pathRound->add_round_rect(rectClient, 1 * 2);

                  pathRound->end_figure(true);

                  //::draw2d::pen_sp pen(pgraphics, 1.0, ARGB(48, 108, 149, 255));
                  ::draw2d::pen_sp pen(pgraphics, 1.0, bError ? ARGB(75, 255, 210, 200) : bHover ? ARGB(80, 200, 230, 255) : ARGB(80, 220, 231, 235));

                  pgraphics->SelectObject(pen);

                  pgraphics->DrawRect(rectClient, pen);

               }



               rectClient.left--;
               rectClient.right++;
               rectClient.top--;
               rectClient.bottom++;

               {

                  ::draw2d::path_sp pathRound(allocer());

                  pathRound->begin_figure(true, ::draw2d::fill_mode_winding);

                  pathRound->add_round_rect(rectClient, 1 * 2);

                  pathRound->end_figure(true);

                  //::draw2d::pen_sp pen(pgraphics, 1.0, ARGB(36, 108, 149, 255));
                  ::draw2d::pen_sp pen(pgraphics, 1.0, bError ? ARGB(45, 255, 230, 220) : bHover ? ARGB(40, 220, 240, 255) : ARGB(40, 230, 235, 240));

                  pgraphics->SelectObject(pen);

                  pgraphics->DrawRect(rectClient, pen);

               }




               /*
               rectClient.left--;
               rectClient.right++;
               rectClient.top--;
               rectClient.bottom++;

               {

               ::draw2d::path_sp pathRound(allocer());

               pathRound->begin_figure(true, ::draw2d::fill_mode_winding);

               pathRound->add_round_rect(rectClient, 1 * 2);

               pathRound->end_figure(true);

               //::draw2d::pen_sp pen(pgraphics, 1.0, ARGB(24, 108, 149, 255));

               ::draw2d::pen_sp pen(pgraphics, 1.0, ARGB(25, 240, 250, 255));

               pgraphics->SelectObject(pen);

               pgraphics->DrawRect(rectClient, pen);

               }
               */

            }

         }
         else
         {

            ::draw2d::pen_sp pen(pgraphics, 3.0, ARGB(255, 84, 77, 255));

            pgraphics->DrawRect(rectClient, pen);

         }

      }
      else
      {

         ::draw2d::pen_sp pen(pgraphics, 1.0, ARGB(255, 149, 149, 123));

         pgraphics->DrawRect(rectClient, pen);

      }

      return true;

   }


   bool schema::_001TabOnDrawSchema01(::draw2d::graphics * pgraphics, tab * ptab)
   {

      return false;

   }


   void schema::_001OnTabPaneDrawTitle(::user::tab_pane & pane, ::user::tab * ptab, ::draw2d::graphics * pgraphics, LPCRECT lpcrect, ::draw2d::brush_sp & brushText)
   {

      if (Session.userschema() != this)
      {

         Session.userschema()->_001OnTabPaneDrawTitle(pane, ptab, pgraphics, lpcrect, brushText);

      }

   }


   bool schema::_001OnTabLayout(::user::tab * ptab)
   {

      return false;

   }


   bool schema::_001GetMainFrameTranslucency(::user::ETranslucency & etranslucency)
   {

      return false;

   }


   bool schema::_001OnDrawMainFrameBackground(::draw2d::graphics * pgraphics, ::user::frame_window * pframe)
   {

      return false;

   }


   void schema::_001DrawCheckBox(::draw2d::graphics * pgraphics, LPCRECT lpcrectClient, ::check::e_check echeck)
   {


      int w = width(lpcrectClient);
      
      int h = height(lpcrectClient);

      if (w <= 0 || h <= 0)
      {

         return;

      }

      pgraphics->OffsetViewportOrg(lpcrectClient->left, lpcrectClient->top);

      // if no image
      {
         rect rectCheckBox;
         rectCheckBox.left = 0;
         rectCheckBox.top = 0;
         rectCheckBox.right = 15 * w / 15;
         rectCheckBox.bottom = 15 * h / 15;
         if (echeck == check::tristate)
         {
            pgraphics->FillSolidRect(rectCheckBox, ARGB(255, 220, 220, 220));
         }
         pgraphics->Draw3dRect(rectCheckBox, ARGB(255, 128, 128, 128), ARGB(255, 128, 128, 128));
         if (echeck == check::tristate
            || echeck == check::checked)
         {
            ::draw2d::pen_sp pen(allocer());
            pen->create_solid(1 * (w + h) / 30, echeck == check::checked ? ARGB(255, 0, 0, 0) : ARGB(255, 96, 96, 96));
            pgraphics->SelectObject(pen);
            pgraphics->MoveTo(2 * w / 15, 8 * h / 15);
            pgraphics->LineTo(6 * w / 15, 12 * h / 15);
            pgraphics->LineTo(13 * w / 15, 5 * h / 15);
            pgraphics->MoveTo(2 * w / 15, 9 * h / 15);
            pgraphics->LineTo(6 * w / 15, 13 * h / 15);
            pgraphics->LineTo(13 * w / 15, 6 * h / 15);
         }

      }

      pgraphics->OffsetViewportOrg(-lpcrectClient->left, -lpcrectClient->top);

   }


   bool schema::_001DrawToolbarItem(::draw2d::graphics * pgraphics, int32_t iItem, ::user::toolbar * ptoolbar)
   {

      return false;

   }


   bool schema::_001DrawSimpleScrollBar(::draw2d::graphics * pgraphics, simple_scroll_bar * pbar)
   {

      return false;

   }


   bool schema::_001DrawScrollBar(::draw2d::graphics * pgraphics, ::user::scroll_bar * pbar)
   {

      return false;

   }


} // namespace user





