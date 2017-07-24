#pragma once


namespace user
{


   class CLASS_DECL_APP_CORE_WNDFRM_CORE meta_button :
      virtual public ::user::wndfrm::frame::control_box_button
   {
   public:


      ::draw2d::region_sp                 m_spregion;


      meta_button(::aura::application * papp);
      virtual ~meta_button();


      virtual index hit_test(point point, ::user::e_element & eelement);

      virtual void _001OnNcDraw(::draw2d::graphics * pgraphics) override;
      virtual void _001OnDraw(::draw2d::graphics * pgraphics);

      virtual void install_message_handling(::message::dispatch *pinterface);



      virtual void on_layout();



      virtual bool keyboard_focus_is_focusable();

   };



} // namespace user




