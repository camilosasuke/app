#include "framework.h"


namespace userex
{


   font_view::font_view(::aura::application * papp) :
      object(papp),
      ::user::split_layout(papp),
      ::user::split_view(papp),
      place_holder_container(papp)
   {

      m_pview = NULL;

      m_ptopview = NULL;

   }


   font_view::~font_view()
   {

   }


   void font_view::assert_valid() const
   {

      ::user::split_view::assert_valid();

   }


   void font_view::dump(dump_context & dumpcontext) const
   {

      ::user::split_view::dump(dumpcontext);

   }


   void font_view::on_update(::user::impact * pSender, LPARAM lHint, object* phint)
   {

      ::user::split_view::on_update(pSender, lHint, phint);

      ::user::view_update_hint * puh = dynamic_cast <::user::view_update_hint *>(phint);

      if (puh != NULL)
      {

         if (puh->m_ehint == ::user::view_update_hint::hint_after_change_text)
         {

            if (puh->m_pui == m_ptopview->m_peditview)
            {
               
               synch_lock sl(m_pview->m_pfontlistdata->m_pmutex);

               string strText;

               m_ptopview->m_peditview->_001GetText(strText);

               m_pview->m_pfontlistdata->m_strText = strText;

            }

         }

      }

   }


   void font_view::on_create_views()
   {

      if (get_pane_count() > 0)
         return;

      SetPaneCount(2);

      SetSplitOrientation(orientation_horizontal);

      set_position(0, 24);

      initialize_split_layout();

      m_ptopview = create_view < top_view >(NULL, ::null_rect(), get_pane_holder(0), "top_view");

      if (m_ptopview == NULL)
      {

         System.simple_message_box(NULL, "Could not create folder edit view");

      }

      //SetPane(0,m_ptopview,false);

      m_pview = create_view < ::user::font_list_view >(NULL, ::null_rect(), get_pane_holder(1), "font_list_view");

      if (m_pview == NULL)
      {

         System.simple_message_box(NULL, "Could not create file list ::user::impact");

      }


   }


   bool font_view::BaseOnControlEvent(::user::control_event * pevent)
   {


      if (pevent->m_eevent == ::user::event_after_change_cur_sel
         || pevent->m_eevent == ::user::event_after_change_cur_hover)
      {

         if (m_pview == pevent->m_puie)
         {

            ::user::view_update_hint uh(get_app());

            uh.m_ehint = ::user::view_update_hint::hint_control_event;
            uh.m_pusercontrolevent = pevent;
            uh.m_pui =this;

            GetTypedParent<::userex::pane_tab_view>()->get_document()->update_all_views(this, 0, &uh);

         }

      }
      return true;

   }


} // namespace userex



