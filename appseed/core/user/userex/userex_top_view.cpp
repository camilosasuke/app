#include "framework.h"


namespace userex
{


   top_view::top_view(::aura::application * papp) :
      object(papp),
      ::user::split_layout(papp),
      ::user::split_view(papp),
      place_holder_container(papp)
   {
      m_peditview = NULL;
      m_ptoggleview = NULL;
   }


   top_view::~top_view()
   {

   }


   void top_view::assert_valid() const
   {

      ::user::split_view::assert_valid();

   }


   void top_view::dump(dump_context & dumpcontext) const
   {

      ::user::split_view::dump(dumpcontext);

   }


   void top_view::on_update(::user::impact * pSender, LPARAM lHint, object* phint)
   {
      ::user::split_view::on_update(pSender, lHint, phint);
   }


   void top_view::on_create_views()
   {

      if (get_pane_count() > 0)
         return;

      SetPaneCount(2);

      SetSplitOrientation(orientation_vertical);

      set_position_rate(0, 0.84);

      initialize_split_layout();

      m_peditview = create_view < top_edit_view >(NULL, ::null_rect(), get_pane_holder(0), "top_edit_view");

      if (m_peditview == NULL)
      {

         System.simple_message_box(NULL, "Could not create folder edit view");

      }

      m_peditview->m_ptopview = this;

      m_ptoggleview = create_view < top_toggle_view >(NULL, ::null_rect(), get_pane_holder(1), "top_toggle_view");

      if (m_ptoggleview == NULL)
      {

         System.simple_message_box(NULL, "Could not create file list ::user::impact");

      }

      m_ptoggleview->m_ptopview = this;



   }

   bool top_view::is_this_visible()
   {

      return ::user::split_view::is_this_visible() && !GetTopLevel()->frame_is_transparent();

   }



} // namespace userex



