//#include "framework.h"


namespace visual
{


   font_enumeration::font_enumeration(::aura::application * papp) :
      object(papp)
   {

      defer_create_mutex();

   }


   font_enumeration::~font_enumeration()
   {

   }




   void font_enumeration::update()
   {

      ::draw2d::graphics_sp g(allocer());

      g->CreateCompatibleDC(NULL);

      {

         synch_lock sl(m_pmutex);

         g->enum_fonts(m_itema);

      }

   }


} // namespace visual





