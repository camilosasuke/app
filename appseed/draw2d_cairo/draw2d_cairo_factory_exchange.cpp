#include "framework.h"
#if defined(LINUX)
#include "base/os/linux/linux_window_xlib.h"
#elif defined(VSNORD)
//#include "base/os/android/android_window_anative.h"
#endif

namespace draw2d_cairo
{


   factory_exchange::factory_exchange(::aura::application * papp) :
      ::object(papp)
   {

      init_cairo_mutex(papp);

      System.factory().cloneable_large < dib                   >  (System.type_info < ::draw2d::dib                 > ());
      System.factory().cloneable_large < graphics              >  (System.type_info < ::draw2d::graphics            > ());
      System.factory().cloneable_large < bitmap                >  (System.type_info < ::draw2d::bitmap              > ());
      System.factory().cloneable_large < pen                   >  (System.type_info < ::draw2d::pen                 > ());
      System.factory().cloneable_large < brush                 >  (System.type_info < ::draw2d::brush               > ());
      System.factory().cloneable_large < palette               >  (System.type_info < ::draw2d::palette             > ());
      System.factory().cloneable_large < region                >  (System.type_info < ::draw2d::region              > ());
      System.factory().cloneable_large < font                  >  (System.type_info < ::draw2d::font                > ());
      System.factory().cloneable_large < path                  >  (System.type_info < ::draw2d::path                > ());

  //    #ifdef WINDOWSEX

//      System.factory().cloneable_large < window_gdi                  >  (System.type_info <window_graphics                > ());

      #if defined(LINUX)

      System.factory().cloneable_large < window_xlib                 >  (System.type_info <window_graphics                > ());

      //#elif defined(ANDROID)

      //System.factory().cloneable_large < window_double_buffer        >  (System.type_info <window_graphics                >());

      #endif

   }


   factory_exchange::~factory_exchange()
   {

   }


} // namespace win


extern "C"
void ca2_factory_exchange(::aura::application * papp)
{
   draw2d_cairo::factory_exchange factoryexchange(papp);
}

extern "C"
void draw2d_factory_exchange(::aura::application * papp)
{

   draw2d_cairo::factory_exchange factoryexchange(papp);

}


