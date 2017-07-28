#include "framework.h"


namespace user
{


   namespace wndfrm
   {


      namespace frame
      {


         namespace wndfrm_core
         {


            frame_schema::frame_schema(::aura::application * papp) :
               object(papp),
               ::user::wndfrm::frame::frame(papp),
               m_schema(papp),
               m_penText1(allocer()),
               m_penFace1(allocer()),
               m_penHilight1(allocer()),
               m_penShadow1(allocer()),
               m_penDkShadow1(allocer())
            {



                  m_rectClient = null_rect();

                  //               m_iMargin = 7;


               }

            frame_schema::~frame_schema()
            {

            }

            /*

            EHitTest frame_schema::_000HitTest(point pt)
            {
               UNREFERENCED_PARAMETER(pt);
               return HitTestClient;
            }

            */

            /*

            bool frame_schema::_000OnLButtonDown(::message::mouse * pmouse)
            {

               if (m_pworkset->GetAppearance() != ::user::AppearanceZoomed && m_pworkset->GetAppearance() != ::user::AppearanceFullScreen)
               {

                  if (m_pworkset->GetSizingManager()->_000OnLButtonDown(pmouse))
                     return true;

                  if (m_pworkset->GetMovingManager()->_000OnLButtonDown(pmouse))
                     return true;

               }

               return false;

            }

            bool frame_schema::_000OnLButtonUp(::message::mouse * pmouse)
            {

               if (m_pworkset->GetAppearance() != ::user::AppearanceZoomed && m_pworkset->GetAppearance() != ::user::AppearanceFullScreen)
               {

                  if (m_pworkset->GetSizingManager()->_000OnLButtonUp(pmouse))
                     return true;

                  if (m_pworkset->GetMovingManager()->_000OnLButtonUp(pmouse))
                     return true;

               }

               return false;
            }

            bool frame_schema::_000OnMouseMove(::message::mouse * pmouse)
            {

               if (m_pworkset->GetAppearance() != ::user::AppearanceZoomed && m_pworkset->GetAppearance() != ::user::AppearanceFullScreen)
               {

                  if (m_pworkset->GetSizingManager()->_000OnMouseMove(pmouse))
                     return true;

                  if (m_pworkset->GetMovingManager()->_000OnMouseMove(pmouse))
                     return true;

               }

               return false;
            }

            bool frame_schema::_000OnNcLButtonDown(::message::mouse * pmouse)
            {

               if (m_pworkset->GetAppearance() != ::user::AppearanceZoomed && m_pworkset->GetAppearance() != ::user::AppearanceFullScreen)
               {

                  if (m_pworkset->GetSizingManager()->_000OnLButtonDown(pmouse))
                     return true;

                  if (m_pworkset->GetMovingManager()->_000OnLButtonDown(pmouse))
                     return true;

               }

               return false;
            }

            bool frame_schema::_000OnNcLButtonUp(::message::mouse * pmouse)
            {

               if (m_pworkset->GetAppearance() != ::user::AppearanceZoomed && m_pworkset->GetAppearance() != ::user::AppearanceFullScreen)
               {

                  if (m_pworkset->GetSizingManager()->Relay(pmouse))
                     return true;

                  if (m_pworkset->GetMovingManager()->Relay(pmouse))
                     return true;

               }

               return false;
            }

            bool frame_schema::_000OnNcMouseMove(::message::mouse * pmouse)
            {

               if (m_pworkset->GetAppearance() != ::user::AppearanceZoomed && m_pworkset->GetAppearance() != ::user::AppearanceFullScreen)
               {

                  if (m_pworkset->GetSizingManager()->Relay(pmouse))
                     return true;

                  if (m_pworkset->GetMovingManager()->Relay(pmouse))
                     return true;

               }

               return false;
            }

            bool frame_schema::_000OnNcHitTest(point pt, LRESULT & nHitTest)
            {
               UNREFERENCED_PARAMETER(pt);
               UNREFERENCED_PARAMETER(nHitTest);
               return false;
            }
            */

/*            bool frame_schema::_000OnTimer(uint32_t nIDEvent)
            {
               UNREFERENCED_PARAMETER(nIDEvent);
               return false;
            }


            bool frame_schema::_000OnSize(uint32_t nType, int32_t cx, int32_t cy)
            {

               UNREFERENCED_PARAMETER(cx);
               UNREFERENCED_PARAMETER(cy);

               sp(::user::interaction) pwnd = get_window();

               if(pwnd == NULL || pwnd->m_pimpl->m_bIgnoreSizeEvent)
                  return false;

               on_layout();

               return false;

            }


            bool frame_schema::_000OnMove(int32_t x, int32_t y)
            {
               UNREFERENCED_PARAMETER(x);
               UNREFERENCED_PARAMETER(y);
               return false;
            }

            bool frame_schema::_000OnCommand(WPARAM wparam, LPARAM lparam, LRESULT & lresult)
            {
               UNREFERENCED_PARAMETER(wparam);
               UNREFERENCED_PARAMETER(lparam);
               UNREFERENCED_PARAMETER(lresult);
               return false;
            }

            bool frame_schema::_000OnDisplayChange(int32_t iBitsPerPixel, size sizeScreen)
            {
               UNREFERENCED_PARAMETER(iBitsPerPixel);
               UNREFERENCED_PARAMETER(sizeScreen);
               update_drawing_objects();
               return false;
            }*/

            /*
            void frame_schema::OnNcCalcSize(LPRECT lprect)
            {

               if(get_appearance()->m_bUseNc)
               {

                  calc_window_client_rect(lprect, lprect);

               }

            }


            appearance * frame_schema::get_appearance()
            {
               ASSERT(m_pworkset != NULL);
               return m_pworkset->get_appearance();
            }
            */



            void frame_schema::_000OnBeforeMove(const RECT & rect)
            {

               update_window_region(rect);

            }


            void frame_schema::_000OnBeforeSize(const RECT & rect)
            {

               UNREFERENCED_PARAMETER(rect);

            }





            void frame_schema::OnMove(sp(::user::interaction) pwnd)
            {

               UNREFERENCED_PARAMETER(pwnd);

            }


            void frame_schema::Glass(::draw2d::graphics * pgraphics, const RECT & lprect)
            {

               sp(::user::interaction) pwndiDraw = m_pworkset->get_draw_window();

               class imaging & imaging = System.visual().imaging();

               class rect rectClient(lprect);

               class rect rectInflate;

               if(rectClient.area() <= 0)
                  return;

               

               int32_t iInflate = 5; // raio 2 pixels + centro 1 pixel

               rectInflate = rectClient;
               rectInflate.inflate(iInflate, iInflate);

               //sp(::user::interaction) pwndDesktop = System.get_desktop_window();

               class rect rectScreen;

               pwndiDraw->best_monitor(rectScreen);

               ::draw2d::dib_sp spdib(allocer());

               ::draw2d::dib_sp spdib2(allocer());

               spdib->create(rectClient.width() + iInflate * 2, rectClient.height() + iInflate * 2);

               spdib2->create(rectClient.width() + iInflate * 2, rectClient.height() + iInflate * 2);
               class rect rectWindow = rectClient;
               pwndiDraw->ClientToScreen(rectWindow);
               //spdib->create(rectClient.width(), rectClient.height());
               bool b = spdib2->get_graphics()->BitBlt(0, 0, rectClient.width() + iInflate * 2, rectClient.height() + iInflate * 2, pgraphics, rectClient.left - iInflate, rectClient.top - iInflate, SRCCOPY);
               //bool b = ::BitBlt(dc2, 0, 0, rectClient.width() + iInflate * 2, rectClient.height() + iInflate * 2, hdcScreen, rectClient.left - iInflate, rectClient.top - iInflate, SRCCOPY);
               b = imaging.blur(spdib->get_graphics(), point(0, 0),
                  size(rectClient.width() + iInflate * 2, rectClient.height() + iInflate * 2),
                  spdib2->get_graphics(), point(0, 0), 2);

               //spgraphics->Draw3dRect(rectClient, 127 << 24, 127 << 24);
               //rectClient.deflate(1, 1);
               //spgraphics->Draw3dRect(rectClient, 64 << 24, 64 << 24);
               /*b = imaging.bitmap_blend(pgraphics, lprect->left, lprect->top,
               rectWindow.width(),
               rectWindow.height(),
               &spgraphics,
               iInflate, iInflate, 128);*/
               /*BLENDFUNCTION bf;
               bf.AlphaFormat = AC_SRC_ALPHA;
               bf.BlendOp     = AC_SRC_OVER;
               bf.BlendFlags = 0;
               bf.SourceConstantAlpha = 255;
               ::alpha_blend(pgraphics->get_handle1(),
               lprect->left, lprect->top,
               rectWindow.width(),
               rectWindow.height(),
               spgraphics->get_handle1(),
               0, 0,
               rectWindow.width(),
               rectWindow.height(),
               bf);*/

               pgraphics->BitBlt(rectClient.left, rectClient.top, rectClient.width(), rectClient.height(), spdib->get_graphics(), iInflate, iInflate, SRCCOPY);

            }

            COLORREF frame_schema::get_border_main_body_color()
            {
               return RGB(63, 150, 106);
            }

            ::user::front_end_schema * frame_schema::get_user_front_end_schema()
            {

               return &m_schema;

            }


            void frame_schema::ColorGlass(::draw2d::graphics * pgraphics, const RECT & lprect, COLORREF cr, BYTE bAlpha)
            {

               /*Gdiplus::Graphics g((HDC) pgraphics->get_os_data());
               g.SetCompositingMode(Gdiplus::CompositingModeSourceOver);
               Gdiplus::SolidBrush solidBrush(Gdiplus::Color(bAlpha, argb_get_r_value(cr), argb_get_g_value(cr), argb_get_b_value(cr)));
               g.FillRectangle(&solidBrush, lprect->left, lprect->top, lprect->right - lprect->left, lprect->bottom - lprect->top);*/
               
               System.visual().imaging().color_blend(pgraphics, lprect, cr, bAlpha);

            }




            void frame_schema::set_style(e_style estyle)
            {

               m_estyle = estyle;

               if(get_control_box().is_null())
                  return;

               on_style_change();

            }


            void frame_schema::set_style(const char * pszStyle)
            {

               string strStyle(pszStyle);

               if(strStyle == "TranslucidWarmGray")
               {
                  set_style(StyleTranslucidWarmGray);
               }
               else if(strStyle == "DarkWarmBlue")
               {
                  set_style(StyleDarkWarmBlue);
               }
               else if(strStyle == "WarmGray")
               {
                  set_style(StyleTranslucidWarmGray);
               }
               else if(strStyle == "BlueRedPurple")
               {
                  set_style(StyleBlueRedPurple);
               }
               else if(strStyle == "RedOrange")
               {
                  set_style(StyleRedOrange);
               }
               else if(strStyle == "LightBlue")
               {
                  set_style(StyleLightBlue);
               }


            }


            frame_schema::e_element operator++(frame_schema::e_element & eelement, int32_t i)
            {
               UNREFERENCED_PARAMETER(i);
               return (frame_schema::e_element) (*((int32_t*)&eelement))++;
            }

            bool frame_schema::get_element_rect(LPRECT lprect, e_element eelement)
            {
               switch(eelement)
               {
               case ElementTopLeftIcon:
                  
                  if(m_pworkset->m_pappearance == NULL || m_pworkset->m_pappearance->m_picon == NULL)
                     return false;

                  lprect->left = m_pointWindowIcon.x;
                  lprect->top = m_pointWindowIcon.y;
                  lprect->right = lprect->left + m_pworkset->m_pappearance->m_picon->get_size().cx;
                  lprect->bottom = lprect->top + m_pworkset->m_pappearance->m_picon->get_size().cy;

                  return true;

               case ElementMoveGripMinimal:
                  
                  if(m_pworkset->m_pappearance == NULL || m_pworkset->m_pappearance->GetAppearance() != ::user::AppearanceMinimal)
                     return false;

                  lprect->left = m_pointMoveGripMinimal.x + 2;
                  lprect->top = m_pointMoveGripMinimal.y + 2;
                  lprect->right = lprect->left + get_control_box_rect()->height() - 4;
                  lprect->bottom = lprect->top + get_control_box_rect()->height() - 4;

                  return true;

               default:
                  break;
               }
               return false;

            }

            bool frame_schema::hit_test(const POINT &point, e_element &eelementParam)
            {
               rect rect;
               for(e_element eelement = (e_element)(ElementNone + 1);
                  eelement < ElementEnd;
                  eelement++)
               {
                  get_element_rect(rect, eelement);
                  if(rect.contains(point))
                  {
                     eelementParam = eelement;
                     return true;
                  }
               }
               return false;
            }


            void frame_schema::set_moveable_border_color(COLORREF cr)
            {
               m_crMoveableBorder = cr;

               color color;

               color.set_rgb(cr);
               color.hls_rate(0.0, 0.5, 0.0);
               m_crMoveableBorderHilight = color.get_rgb() | (0xff << 24);

               color.set_rgb(cr);
               color.hls_rate(0.0, -0.3, 0.0);
               m_crMoveableBorderShadow = color.get_rgb() | (0xff << 24);

               color.set_rgb(cr);
               color.hls_rate(8.0, -0.8, 0.0);
               m_crMoveableBorderDkShadow = color.get_rgb() | (0xff << 24);


               m_crCaptionTextBk = m_crMoveableBorderShadow;

            }


            void frame_schema::set_button_color_schema_001(COLORREF crMoveableBorder)
            {

               COLORREF crBase = crMoveableBorder;

               color ca;

               ca.set_rgb(crBase);
               ca.hls_rate(0.0, -0.49, -0.49);
               m_schema.m_button.set_color(color_border_normal, ca.get_rgb() | (0xff << 24));

               ca.set_rgb(crBase);
               ca.hls_rate(0.0, -0.11, -0.11);
               m_schema.m_button.set_color(color_background_normal, ca.get_rgb() | (0xff << 24));

               ca.set_rgb(crBase);
               ca.hls_rate(0.0, -0.49, -0.49);
               m_schema.m_button.set_color(color_text_normal, ca.get_rgb() | (0xff << 24));




               ca.set_rgb(crBase);
               ca.hls_rate(0.0, -0.45, 0.11);
               m_schema.m_button.set_color(color_border_hover, ca.get_rgb() | (0xff << 24));

               ca.set_rgb(crBase);
               ca.hls_rate(0.0, 0.33, 0.0);
               m_schema.m_button.set_color(color_background_hover,ca.get_rgb() | (0xff << 24));

               ca.set_rgb(crBase);
               ca.hls_rate(0.0, -0.55, 0.11);
               m_schema.m_button.set_color(color_text_hover, ca.get_rgb() | (0xff << 24));

            }



            void frame_schema::set_frame_schema_color_system_default_001()
            {

               m_penText1->create_solid(1, Session.get_default_color(COLOR_BTNTEXT) | 0xff000000);
               m_penFace1->create_solid(1, Session.get_default_color(COLOR_BTNFACE) | 0xff000000);
               m_penHilight1->create_solid(1, Session.get_default_color(COLOR_BTNHILIGHT) | 0xff000000);
               m_penShadow1->create_solid(1, Session.get_default_color(COLOR_BTNSHADOW) | 0xff000000);
               m_penDkShadow1->create_solid(1, Session.get_default_color(COLOR_3DDKSHADOW) | 0xff000000);
               m_crDkShadow = Session.get_default_color(COLOR_3DDKSHADOW);
               m_crFrameBorder = RGB(0, 0, 0) | 0xff000000;


            }


            void frame_schema::on_style_change_001_and_002()
            {


               control_box * pcontrolbox = get_control_box();


               m_minSize = size(144, 48);
               m_minSize.cy = 48;
               pcontrolbox->set_button_color_system_default_001();
               set_frame_schema_color_system_default_001();
               set_moveable_border_color(get_style_moveable_border_color(m_estyle));

               set_button_color_schema_001(m_crMoveableBorder);

               if(m_estyle == StyleTranslucidWarmGray || m_estyle == StyleDarkWarmBlue)
               {
                  pcontrolbox->m_crBackground = m_crCaptionTextBk;
               }



            }

            COLORREF frame_schema::get_style_moveable_border_color(e_style estyle)
            {
               switch(m_estyle)
               {
               case StyleLightBlue:
               case StyleTranslucidLightBlue:
                  return RGB(116, 160, 220);
               case StyleTranslucidWarmGray:
                  return ARGB(255, 184, 184, 177);
                  break;
               case StyleDarkWarmBlue:
                  return ARGB(255,34,54,75);
                  break;
               case StyleBlackBorder:
                  return RGB(116, 160, 220) | 0xff000000;
               case StyleLightGreen:
               case StyleTranslucidLightGreen:
                  return RGB(116, 220, 160) | 0xff000000;
               case StyleRedOrange:
                  return RGB(255, 170, 136) | 0xff000000;
               case StyleBlueRedPurple:
                  return RGB(200, 100, 220) | 0xff000000;
               case StyleEveningSun:
                  return RGB(255, 210, 100) | 0xff000000;
               case StyleTranslucidWarmLiteGray:
                  return RGB(239, 230, 219) | 0xff000000;
               }


               // Light Green/Translucid Light Green
               return RGB(116, 220, 160) | 0xff000000;

            }


            bool frame_schema::is_translucid_style(e_style estyle)
            {

               return
                  estyle == StyleTranslucidWarmGray
                  || estyle == StyleTranslucidLightBlue
                  || estyle == StyleTranslucidLightGreen
                  || estyle == StyleTranslucidWarmLiteGray;


            }


            void frame_schema::Draw3dRectSide(::draw2d::graphics * pgraphics,const RECT & rectParam,EBorder eborder,COLORREF crTopLeft,COLORREF crBottomRight)
            {

               rect rect(rectParam);
               int32_t x = rect.left;
               int32_t y = rect.top;
               int32_t cx = rect.width();
               int32_t cy = rect.height();

               

               pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

               ::draw2d::pen_sp pen;

               if(eborder & BorderTop || eborder & BorderLeft)
               {

                  pen.alloc(allocer());

                  pen->create_solid(1.0,crTopLeft);

                  pgraphics->SelectObject(pen);

               }

               if(eborder & BorderTop)
               {

                  pgraphics->MoveTo(x,y);

                  pgraphics->LineTo(x + cx,y);

               }

               if(eborder & BorderLeft)
               {

                  pgraphics->MoveTo(x,y);

                  pgraphics->LineTo(x,y+cy);

               }

               if((eborder & BorderRight || eborder & BorderBottom) && (pen.is_null() || pen->m_cr != crBottomRight))
               {

                  pen.alloc(allocer());

                  pen->create_solid(1.0,crBottomRight);

                  pgraphics->SelectObject(pen);

               }

               if(eborder & BorderRight)
               {

                  pgraphics->MoveTo(x + cx,y);

                  pgraphics->LineTo(x + cx,y + cy);

               }

               if(eborder & BorderBottom)
               {

                  pgraphics->MoveTo(x,y + cy);

                  pgraphics->LineTo(x + cx,y + cy);

               }

            }



            void frame_schema::on_style_change()
            {


            }


            void frame_schema::_001OnDraw(::draw2d::graphics * pgraphics)
            {

               if(!m_pworkset->IsAppearanceEnabled())
                  return;

               

               pgraphics->set_text_rendering(::draw2d::text_rendering_anti_alias);

               appearance * pappearance = m_pworkset->get_appearance();

               if(!pappearance->IsFullScreen() && !pappearance->IsZoomed() && !pappearance->IsIconic() && !m_pworkset->GetWndDraw()->frame_is_transparent())
               {

                  on_draw_frame(pgraphics);

               }

               if(pappearance->IsMinimal())
               {

                  class rect rectIcon;

                  if(get_element_rect(rectIcon,ElementTopLeftIcon))
                  {

                     ::visual::icon * picon = m_pworkset->m_pappearance->m_picon;

                     if(picon != NULL)
                     {

                        pgraphics->DrawIcon(rectIcon.left,rectIcon.top,picon,rectIcon.width(),rectIcon.height(),0,NULL,DI_NORMAL);

                     }

                  }

                  class rect rectGrip;

                  if(get_element_rect(rectGrip,ElementMoveGripMinimal))
                  {

                     int i = 0;

                     while(i < rectGrip.width() - 5 + 1)
                     {

                        pgraphics->Draw3dRect(rectGrip.left + i,rectGrip.top,3,rectGrip.height(),ARGB(84 + 23,230,230,230),ARGB(84 + 23,108 + 23,108 + 23,108 + 23));

                        i += 5;
   
                     }

                     //pgraphics->Draw3dRect(rectGrip.left + 12,rectGrip.top,3,rectGrip.height(),ARGB(184,255,255,255),ARGB(184,84,84,84));

                  }

               }
               else if(!pappearance->IsFullScreen() && !m_pworkset->GetWndDraw()->frame_is_transparent())
               {

                  pgraphics->FillSolidRect(m_rectCaptionTextBk, m_crCaptionTextBk);

                  class rect rectIcon;

                  sp(::user::interaction) pwndiDraw = m_pworkset->get_draw_window();

                  COLORREF    crMoveableBorder;

                  COLORREF    crMoveableBorderHilight;

                  COLORREF    crMoveableBorderShadow;

                  string str;

                  pwndiDraw->get_window_text(str);

                  if(pappearance->m_fActive)
                  {

                     crMoveableBorder = m_crMoveableBorder;

                     crMoveableBorderHilight = m_crMoveableBorderHilight;

                     crMoveableBorderShadow = m_crMoveableBorderShadow;

                  }
                  else
                  {

                     crMoveableBorder = Session.get_default_color(COLOR_BTNFACE);

                     crMoveableBorderHilight = Session.get_default_color(COLOR_BTNHILIGHT);

                     crMoveableBorderShadow = Session.get_default_color(COLOR_BTNSHADOW);

                  }

                  rect rect;

                  sp(::user::interaction) pwnd = get_window();

                  pwnd->GetWindowRect(rect);

                  rect -= rect.top_left();

                  if(get_element_rect(rectIcon, ElementTopLeftIcon))
                  {

                     ::visual::icon * picon = m_pworkset->m_pappearance->m_picon;

                     if(picon != NULL)
                     {

                        pgraphics->DrawIcon(rectIcon.left, rectIcon.top, picon, rectIcon.width(), rectIcon.height(), 0, NULL, DI_NORMAL);

                     }

                  }

                  class font_department & fonts = System.visual().fonts();

                  string wstrWindowText;

                  pwndiDraw->get_window_text(wstrWindowText);

                  ::draw2d::brush_sp brushText(allocer());

                  brushText->create_solid(Session.get_default_color(COLOR_CAPTIONTEXT));

                  pgraphics->SelectObject(brushText);

                  pgraphics->SelectObject(fonts.GetCaptionFont());

                  pgraphics->_DrawText(wstrWindowText, m_rectWindowText, DT_LEFT | DT_VCENTER | DT_NOPREFIX);

               }


            }


            void frame_schema::on_draw_frame(::draw2d::graphics * pgraphics)
            {

               UNREFERENCED_PARAMETER(pgraphics);

            }


         } // namespace wndfrm_core


      } // namespace frame


   } // namespace wndfrm


} // namespace user












