#include "framework.h"


namespace user
{


   split_view::split_view(::aura::application * papp) :
      object(papp),

      split_layout(papp),
      place_holder_container(papp)
   {

   }


   split_view::~split_view()
   {

   }


   void split_view::install_message_handling(::message::dispatch * pinterface)
   {
      
      split_layout::install_message_handling(pinterface);
      impact::install_message_handling(pinterface);
      
      USER_MESSAGE_LINK(message_create, pinterface, this, &split_view::_001OnCreate);

      //IGUI_WIN_MSG_LINK(WM_SIZE, pinterface, this, &split_view::_001OnSize);
      //IGUI_WIN_MSG_LINK(WM_SHOWWINDOW, pinterface, this, &split_view::_001OnShowWindow);

   }



   void split_view::assert_valid() const
   {
      impact::assert_valid();
   }

   void split_view::dump(dump_context & dumpcontext) const
   {
      impact::dump(dumpcontext);
   }

   void split_view::_001OnCreate(signal_details * pobj)
   {

      SCAST_PTR(::message::create, pcreate, pobj);

      pcreate->previous();

      create_views();

   }


   bool split_view::create_views()
   {


      bool bOk = true;

      try
      {

         on_create_views();

      }
      catch (...)
      {

         bOk = false;

         string strMessage;
#if defined(APPLEOS) || defined(LINUX) || defined(ANDROID)
         strMessage.Format("split_view::on_create_views failed to create views for split view %s", typeid(this).name());
#else
         strMessage.Format("split_view::on_create_views failed to create views for split view %s", typeid(this).raw_name());
#endif

         Application.simple_message_box_timeout(this, strMessage, seconds(10), MB_ICONEXCLAMATION);

      }

      on_layout();

      return bOk;

   }


   void split_view::on_update(::user::impact * pSender, LPARAM lHint, object* phint)
   {

      if (phint != NULL)
      {

         view_update_hint * pupdatehint = dynamic_cast < view_update_hint * > (phint);

         if (pupdatehint != NULL)
         {

            if (pupdatehint->is_type_of(view_update_hint::hint_open_document) || pupdatehint->is_type_of(view_update_hint::hint_create_views))
            {

               //pupdatehint->m_bOk = create_views();

            }

         }

      }


   }


   void split_view::_001OnSize(signal_details * pobj)
   {
      SCAST_PTR(::message::size, psize, pobj);
      psize->previous();
      //on_layout();
   }

   bool split_view::pre_create_window(::user::create_struct& cs)
   {
      cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

      return impact::pre_create_window(cs);
   }


   void split_view::on_create_views()
   {

      Application.on_create_split_view(this);

   }

   void split_view::_001OnNcDraw(::draw2d::graphics * pgraphics)
   {

      UNREFERENCED_PARAMETER(pgraphics);

   }

   void split_view::_001OnDraw(::draw2d::graphics * pgraphics)
   {

      ::user::split_layout::_001OnDraw(pgraphics);


//      int32_t i;
      
      //rect rect;
      
      //COLORREF cr = ARGB(184, 92, 92, 80);

      //for(i = 0; i < get_pane_count(); i++)
      //{
      //  
      //   CalcPaneRect(i, &rect);

      //   // Top
      //   pgraphics->FillSolidRect(
      //      rect.left,
      //      rect.top,
      //      rect.width(),
      //      m_cyBorder,
      //      cr);

      //   // Right
      //   pgraphics->FillSolidRect(
      //      rect.right - m_cxBorder,
      //      rect.top,
      //      m_cxBorder,
      //      rect.height(),
      //      cr);

      //   // Bottom
      //   pgraphics->FillSolidRect(
      //      rect.left,
      //      rect.bottom - m_cyBorder,
      //      rect.width(),
      //      m_cyBorder,
      //      cr);

      //   // Left
      //   pgraphics->FillSolidRect(
      //      rect.left,
      //      rect.top,
      //      m_cxBorder,
      //      rect.height(),
      //      cr);

      //}

   }


   void split_view::_001OnShowWindow(signal_details * pobj)
   {
      SCAST_PTR(::message::show_window, pshowwindow, pobj);
      pshowwindow->previous();
      //on_layout();
   }


} // namespace user




