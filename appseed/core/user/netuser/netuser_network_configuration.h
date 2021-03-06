#pragma once


namespace usernet // ca8 + cube
{


   class CLASS_DECL_CORE network_configuration :
      public ::user::form_callback,
      public ::user::view_creator
   {
   public:


      sp(::user::document)               m_pdoc;
      sp(::user::form)                   m_pview;


      network_configuration(::aura::application * papp);
      virtual ~network_configuration();


      bool initialize(sp(::user::interaction) puiParent);
      bool initialize_child(sp(::user::interaction) puiParent);

      void on_show();

      bool BaseOnControlEvent(::user::form_window * pview, ::user::control_event * pevent);

   };


} // namespace core // ca8 + cube


