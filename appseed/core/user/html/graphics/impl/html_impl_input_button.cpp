#include "framework.h"


namespace html
{


   namespace impl
   {


      input_button::input_button(data * pdata)
      {

         m_iFont = -1;

         m_pbutton = canew(::user::button(pdata->get_app()));
         m_pbutton->m_ulFlags &= ~::object::flag_auto_delete;

      }


      input_button::~input_button()
      {
         
         try
         {

            if(m_pbutton->IsWindow())
            {

               m_pbutton->DestroyWindow();

            }

         }
         catch(...)
         {

         }

         try
         {

            m_pbutton.release();

         }
         catch(...)
         {

         }

      }


      void input_button::implement_phase1(data * pdata, ::html::elemental * pelemental)
      {

         m_bHasChar = true;

         if(!m_pbutton->IsWindow())
         {
            m_pbutton->create_window(null_rect(),pdata->m_pui,100);
            pdata->on_create_interaction(m_pbutton);
         }
         elemental::implement_phase1(pdata, pelemental);
         m_pbutton->set_window_text(pelemental->m_pbase->get_tag()->get_attr_value("value"));
         m_pbutton->m_id = pelemental->m_pbase->get_tag()->get_attr_value("id");
         m_pbutton->m_puserschemaSchema = pelemental;
         if(m_pbutton->m_id.is_empty())
         {
            m_pbutton->m_id = pelemental->m_pbase->get_tag()->get_attr_value("name");
         }
         m_cxMax = 200;
         m_cxMin = 200;
      }


      bool input_button::layout_phase1(data * pdata)
      {

         m_pbutton->ResizeToFit();

         rect rectClient = m_pbutton->m_pimpl->m_rectParentClientRequest;

         //m_pbutton->GetClientRect(rectClient);

         m_box.set_cxy((float) rectClient.width(), (float) rectClient.height());

         return true;

      }


      void input_button::layout_phase3(data * pdata)
      {

         elemental::layout_phase3(pdata);

         m_pbutton->SetWindowPos(0, (int32_t) m_box.left, (int32_t) m_box.top, (int32_t) m_box.get_cx(), (int32_t) m_box.get_cy(), SWP_NOREDRAW);

      }


      void input_button::_001OnDraw(data * pdata)
      {

         ::draw2d::graphics * pgraphics = pdata->m_pgraphics;

         //rect rectWindow;
         //m_pbutton->GetWindowRect(rectWindow);
         //m_pbutton->GetWindow()->ScreenToClient(rectWindow);
         ::point ptPreviousViewportOrg = pgraphics->GetViewportOrg();
         pgraphics->OffsetViewportOrg((int32_t) m_box.left, (int32_t) m_box.top);
         m_pbutton->_000OnDraw(pgraphics);
         pgraphics->SetViewportOrg(ptPreviousViewportOrg);
      }

      void input_button::on_change_layout(data * pdata)
      {
         
         UNREFERENCED_PARAMETER(pdata);

         m_pbutton->SetWindowPos(0, (int32_t) m_box.left, (int32_t) m_box.top, (int32_t) m_box.get_cx(), (int32_t) m_box.get_cy(), SWP_NOREDRAW);

      }


   } // namespace impl


} // namespace html





