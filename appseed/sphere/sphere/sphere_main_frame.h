#pragma once


namespace sphere
{


   class CLASS_DECL_SPHERE main_frame :
      virtual public simple_frame_window
   {
   public:


      main_frame(::aura::application * papp);
      virtual ~main_frame();


      virtual bool get_translucency(::user::ETranslucency & etranslucency) override;

      virtual bool get_color(COLORREF & cr, ::user::e_color ecolor) override;


   };


} // namespace sphere



















