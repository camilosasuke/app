#pragma once


namespace android
{

   class CLASS_DECL_BASE window_buffer :
      virtual public ::window_buffer
   {
   public:


      window_buffer(::aura::application * papp);
      virtual ~window_buffer();


      virtual void create_window_graphics_(int64_t cx, int64_t cy, int iStride = -1) override;
      virtual void destroy_window_graphics_() override;

      virtual void update_window(::draw2d::dib * pdib) override;

      //virtual ::draw2d::graphics * on_begin_draw();

   };


} // namespace android


