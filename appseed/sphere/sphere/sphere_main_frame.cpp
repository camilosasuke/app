#include "framework.h"


namespace sphere
{


   main_frame::main_frame(::aura::application * papp) :
      ::object(papp),
      simple_frame_window(papp)
   {

      WfiEnableFullScreen();

   }


   main_frame::~main_frame()
   {

   }


   bool main_frame::get_translucency(::user::ETranslucency & etranslucency)
   {

      etranslucency = ::user::TranslucencyPresent;

      return true;

   }


   bool main_frame::get_color(COLORREF & cr, ::user::e_color ecolor)
   {

      bool bOk = true;

      if (ecolor == ::user::color_text || ecolor == ::user::color_edit_text)
      {

         cr = Application.get_theme_fg_color();
         //if (Application.get_theme() == theme_dark)
         //{

         //   cr = ARGB(255, 250, 250, 250);

         //}
         //else if (Application.get_theme() == theme_blue)
         //{

         //   cr = ARGB(255, 0, 0, 0);

         //}
         //else
         //{

         //   cr = ARGB(255, 0, 0, 0);

         //}

      }
      else if (ecolor == ::user::color_text_selected)
      {

         cr = ARGB(255, 255, 255, 255);

      }
      else if (ecolor == ::user::color_text_highlight)
      {

         cr = ARGB(255, 55, 105, 255);

      }
      else if (ecolor == ::user::color_text_selected_highlight)
      {

         cr = ARGB(255, 102, 153, 255);

      }
      else if (ecolor == ::user::color_background 
         || ecolor == ::user::color_edit_background
         || ecolor == ::user::color_toolbar_background)
      {

         //if (Application.get_theme() == theme_dark)
         //{

         //   if (_001IsTranslucent())
         //   {

         //      cr = ARGB(184, 55, 55, 55);

         //   }
         //   else
         //   {

         //      cr = ARGB(255, 55, 55, 55);

         //   }

         //}
         //else if (Application.get_theme() == theme_blue)
         //{

         //   if (_001IsTranslucent())
         //   {

         //      cr = ARGB(184, 55, 155, 255);

         //   }
         //   else
         //   {

         //      cr = ARGB(255, 55, 155, 255);

         //   }

         //}
         //else
         //{

         //   if (_001IsTranslucent())
         //   {

         //      cr = ARGB(184, 255, 255, 255);

         //   }
         //   else
         //   {

         //      cr = ARGB(255, 255, 255, 255);

         //   }


         //}

         cr = Application.get_theme_bg_color();

      }
      else if (ecolor == ::user::color_background_selected)
      {

         if (_001IsTranslucent())
         {

            cr = ARGB(184, 0, 0, 127);

         }
         else
         {

            cr = ARGB(255, 0, 0, 127);

         }

      }
      else if (ecolor == ::user::color_background_selected)
      {

         if (_001IsTranslucent())
         {

            cr = ARGB(184, 0, 0, 127);

         }
         else
         {

            cr = ARGB(255, 0, 0, 127);

         }

      }
      else if (ecolor == ::user::color_border_normal)
      {

         cr = ARGB(255, 84, 84, 77);

      }
      else if (ecolor == ::user::color_border_hover)
      {

         cr = ARGB(255, 84, 184, 77);

      }
      else if (ecolor == ::user::color_text_hover)
      {

         cr = ARGB(255, 84, 184, 255);

      }
      else
      {

         bOk = false;

      }

      return bOk;

   }



} // namespace asphere
































