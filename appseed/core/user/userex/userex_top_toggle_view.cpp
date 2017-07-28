#include "framework.h"


namespace userex
{


   top_toggle_view::top_toggle_view(::aura::application * papp) :
      object(papp),
      ::user::interaction(papp),
      ::user::button(papp)
   {

      m_ptopview = NULL;

   }


   top_toggle_view::~top_toggle_view()
   {


   }


   void top_toggle_view::install_message_handling(::message::dispatch * pdispatch)
   {

      ::user::impact::install_message_handling(pdispatch);
      ::user::button::install_message_handling(pdispatch);

   }


   void top_toggle_view::on_update(::user::impact * pSender, LPARAM lHint, object* phint)
   {

      ::user::impact::on_update(pSender, lHint, phint);

   }


   bool top_toggle_view::keyboard_focus_is_focusable()
   {

      return ::user::button::keyboard_focus_is_focusable();

   }


} // namespace hellomultiverse













