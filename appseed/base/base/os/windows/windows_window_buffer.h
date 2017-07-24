#pragma once


#include "base/graphics/graphics_window_buffer.h"


namespace windows
{


   class CLASS_DECL_BASE window_buffer :
      virtual public ::window_buffer
   {
      HBITMAP                       m_hbitmap;
      HDC                           m_hdc;
   public:


      HBITMAP                       m_hbitmapOld;
      HDC                           m_hdcScreen;
      bool                          m_bOwnDC;
      HANDLE                        m_hmutex;
      rect                          m_rectLast;


      window_buffer(::aura::application * papp);
      virtual ~window_buffer();


      virtual void create_window_graphics_(int64_t cx, int64_t cy, int iStride = -1);
      virtual void destroy_window_graphics_();

      virtual void create_buffer(int64_t cx, int64_t cy, int iStride = -1);
      virtual void destroy_buffer();

      virtual void on_create_window(::user::interaction_impl * pimpl);

      virtual void update_window(::draw2d::dib * pdib) override;

      virtual void ipc_copy(int cx, int cy);

   };


} // namespace windows


extern CLASS_DECL_BASE bool g_bCreateWindowScreen;


//CLASS_DECL_BASE string expand_env(string str);

