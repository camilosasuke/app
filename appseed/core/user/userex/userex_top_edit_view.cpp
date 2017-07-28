#include "framework.h"


namespace userex
{


   top_edit_view::top_edit_view(::aura::application * papp) :
      object(papp),
      ::user::interaction(papp),
      ::user::plain_edit(papp)
   {

      m_ptopview = NULL;

   }

   top_edit_view::~top_edit_view()
   {

   }
   void top_edit_view::install_message_handling(::message::dispatch * pdispatch)
   {

      ::user::show < ::user::plain_edit >::install_message_handling(pdispatch);

      IGUI_WIN_MSG_LINK(WM_CREATE, pdispatch, this, &top_edit_view::_001OnCreate);

   }


   void top_edit_view::_001OnCreate(signal_details * pobj)
   {
      SCAST_PTR(::message::create, pcreate, pobj);

      pcreate->previous();

      if (pcreate->m_bRet)
         return;


      m_ptopview = GetTypedParent < top_view >();


   }


   void top_edit_view::on_update(::user::impact * pSender, LPARAM lHint, object* phint)
   {

   }


   void top_edit_view::_001OnAfterChangeText(::action::context actioncontext)
   {

      if (actioncontext.is_source(::action::source_sync))
         return;

      if (m_ptopview == NULL)
         return;



      //string strText;

      //_001GetText(strText);

      ::user::view_update_hint uh(get_app());

      uh.m_ehint = ::user::view_update_hint::hint_after_change_text;
      uh.m_pui = this;

      GetTypedParent<::userex::pane_tab_view>()->get_document()->update_all_views(this, 0, &uh);


   }


   bool top_edit_view::keyboard_focus_is_focusable()
   {

      return is_window_enabled() && IsWindowVisible();

   }

   bool top_edit_view::IsWindowVisible()
   {

      return ::user::show < ::user::plain_edit >::IsWindowVisible() && !GetTopLevel()->frame_is_transparent();

   }

   /*
   int64_t top_edit_view::add_ref()
   {
   return ::object::add_ref();
   }
   int64_t top_edit_view::dec_ref()
   {
   return ::object::dec_ref();
   }


   */
} // namespace userex




