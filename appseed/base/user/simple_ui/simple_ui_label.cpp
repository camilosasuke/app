//#include "framework.h" // from "base/user/user.h"
//#include "base/user/user.h"


namespace simple_ui
{


   label::label(::aura::application * papp) :
      ::object(papp)
   {

   }

   label::~label()
   {
   }




   void label::_001OnDraw(::draw2d::graphics * pgraphics)
   {

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);


      COLORREF c;

      //if(get_style()->m_eschema == ::hotplugin::plugin::schema_darker)
      {

#if CA2_PLATFORM_VERSION == CA2_BASIS
         c = ARGB(255, 184 + 49, 149 + 49, 177 + 49);
#else
         c = ARGB(255, 149 + 49, 184 + 49, 123 + 49);
#endif

      }
      //else
      {

#if CA2_PLATFORM_VERSION == CA2_BASIS
         c = ARGB(255, 84, 49, 77);
#else
         c = ARGB(255, 49, 84, 23);
#endif

      }

      rect rectClient;

      GetClientRect(rectClient);

      ::draw2d::brush_sp b(allocer());

      b->create_solid(c);

      ::draw2d::font_sp f(allocer());

      /*f->create_pixel_font(FONT_SANS_EX, (int32_t)height(rectClient) * 0.7);

      float fMargin = (height(rectClient) * ((1.0f - 0.7f) / 2.0f));*/

      f->create_pixel_font(FONT_SANS, (int32_t)height(rectClient) * .84);

      float fMargin = 0.0;

      pgraphics->SelectObject(f);

      pgraphics->SelectObject(b);

      string strText;

      get_window_text(strText);

      pgraphics->text_out(rectClient.left, (int32_t)(rectClient.top + fMargin), strText);

   }


} // namespace simple_ui
