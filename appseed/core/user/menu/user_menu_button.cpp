//#include "framework.h"

namespace user
{

   menu_button::menu_button(::aura::application * papp):
      object(papp),
      ::user::interaction(papp),
      ::user::button(papp)
   {
      m_pitem = NULL;
   }

   menu_button::~menu_button()
   {
   }



   void menu_button::install_message_handling(::message::dispatch * pinterface)
   {

      ::user::button::install_message_handling(pinterface);

      IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &menu_button::_001OnCreate);

   }



   void menu_button::_001OnDraw(::draw2d::graphics * pgraphics)
   {

      rect rectClient;

      GetClientRect(rectClient);

      if (m_id == "separator")
      {

         ::draw2d::pen_sp pen(allocer());

         pen->create_solid(2.0, ARGB(127, 80, 80, 80));

         pgraphics->SelectObject(pen);

         pgraphics->MoveTo(rectClient.left, (rectClient.top + rectClient.bottom) / 2);
         
         pgraphics->LineTo(rectClient.right, (rectClient.top + rectClient.bottom) / 2);

         return;

      }

      button::_001OnDraw(pgraphics);

      if(m_pitem != NULL && m_pitem->m_bPopup)
      {

         

         ::draw2d::brush_sp br(allocer(), RGB(0, 0, 0));

         ::draw2d::pen_sp pen(allocer());

         pen->create_solid(1, RGB(0, 0, 0));
         pgraphics->SelectObject(pen);
         pgraphics->SelectObject(br);
         rect rectPopupArrow;
         rectPopupArrow.left = rectClient.right - 9;
         rectPopupArrow.right = rectClient.right - 4;
         rectPopupArrow.top = ((rectClient.top + rectClient.bottom) / 2) - 4;
         rectPopupArrow.bottom = ((rectClient.top + rectClient.bottom) / 2) + 4;
         point_array pta;
         pta.add(point(rectPopupArrow.left, rectPopupArrow.bottom));
         pta.add(point(rectPopupArrow.right, (rectPopupArrow.bottom + rectPopupArrow.top) / 2));
         pta.add(point(rectPopupArrow.left, rectPopupArrow.top));
         pta.add(point(rectPopupArrow.left, rectPopupArrow.bottom));
         pgraphics->Polygon(pta.get_data(), (int32_t) pta.get_size());
      }

      _001DrawCheck(pgraphics);

   }


   void menu_button::on_layout()
   {

      if (m_puserschemaSchema == NULL)
      {

         if (m_pitem != NULL)
         {

            sp(::user::menu) pui = m_pitem->m_pbase;

            if (pui.is_set() && pui->m_oswindowParent != NULL)
            {

               m_puserschemaSchema = pui->m_oswindowParent;


            }

         }

      }

   }
   
   void menu_button::_001OnNcDraw(::draw2d::graphics * pgraphics)
   {

      if (m_id != "separator")
      {

         ::user::button::_001OnNcDraw(pgraphics);

      }

   }

   void menu_button::_001DrawCheck(::draw2d::graphics * pgraphics)
   {
      
      UINT uiImage = 0xffffffffu;

      if(m_pitem != NULL)
      {

         uiImage = BaseMenuCentral::GetMenuCentral(get_app())->CommandToImage(m_pitem->m_id);

      }

      if(uiImage != 0xffffffffu)
      {

         

         rect rectImage = m_rectCheckBox;
         rect rectImageBorder = rectImage;
         rectImageBorder.inflate(2, 2);
         ::image_list::info ii;
         sp(image_list) pimagelist;
         if(!is_window_enabled())
         {
            pimagelist = BaseMenuCentral::GetMenuCentral(get_app())->MenuV033GetImageListHueLight();
         }
         else
         {
            pimagelist = BaseMenuCentral::GetMenuCentral(get_app())->MenuV033GetImageList();
         }
         pimagelist->get_image_info(uiImage, &ii);
         rect & rectImageInfo(ii.m_rect);
         rectImage.offset(1, 1);
         rectImage.top     = rectImage.bottom - rectImageInfo.height();
         rectImage.right   = rectImage.left + rectImageInfo.width();
         if(m_echeck == check::checked)
         {

            
            pgraphics->FillSolidRect(rectImageBorder, RGB(127, 127, 127));
            pgraphics->Draw3dRect(rectImageBorder, Session.get_default_color(COLOR_3DSHADOW), Session.get_default_color(COLOR_3DHILIGHT));
         }
         pimagelist->draw(
            pgraphics,
            uiImage,
            rectImage.top_left(),
            0);
      }
      else
      {

         ::user::GetUfeSchema(get_app())->DrawCheck(m_echeck, m_rectCheckBox, pgraphics);

      }

   }
   void menu_button::_001OnCreate(::signal_details * pobj)
   {



      pobj->previous();

   }


   void menu_button::_001OnMouseMove(::signal_details * pobj)
   {

      SCAST_PTR(::message::mouse,pmouse,pobj);

      pmouse->previous();


   }

   menu_button_cmd_ui::menu_button_cmd_ui(::aura::application * papp) :
      object(papp),
      cmd_ui(papp)
   {
   }


   void menu_button_cmd_ui::Enable(bool bOn, ::action::context actioncontext)
   {

      m_bEnableChanged = TRUE;

      menu_button* pbutton = dynamic_cast < menu_button * > (m_pOther);

      pbutton->enable_window(bOn != FALSE);

   }


   void menu_button_cmd_ui::_001SetCheck(check::e_check echeck, ::action::context actioncontext)
   {
      
      ASSERT(echeck == check::checked || echeck == check::unchecked || echeck == check::tristate); // 0=>off, 1=>on, 2=>indeterminate
      
      menu_button* pbutton = dynamic_cast < menu_button *  > (m_pOther);

      pbutton->_001SetCheck(echeck, actioncontext);

   }


   void menu_button_cmd_ui::SetText(const char *, ::action::context)
   {
      

   }



} // namespace user
