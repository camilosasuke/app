#include "framework.h"
//#include "base/user/user.h"
#include "metrowin.h"

namespace metrowin
{

   namespace base
   {

      factory_exchange::factory_exchange(::aura::application * papp):
         ::object(papp)
      {

         System.factory().creatable       < window_double_buffer     >(System.template type_info < window_graphics         >(),1);
         System.factory().creatable_large < interaction_impl         >(System.template type_info < ::user::interaction_impl    >());
         //System.factory().creatable_small < copydesk                 >(System.template type_info < ::base::copydesk            >(),1);
         ////      System.factory().creatable       < printer               >  (System.template type_info < ::user::printer             > ()  , 1);

         //System.factory().creatable          < ::windows::message_queue      >(System.type_info < ::user::message_queue       >(),32);


      }


      factory_exchange::~factory_exchange()
      {
      }


   } // namespace base


} // namespace metrowin


void __node_base_factory_exchange(::aura::application * papp)
{
   ::metrowin::base::factory_exchange factoryexchange(papp);
}



