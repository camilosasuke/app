#include "framework.h" // from "base/user/user.h"
#include "base/user/core_user.h"
#include "windows.h"


namespace base
{


   namespace windows
   {


      factory_exchange::factory_exchange(::aura::application * papp):
         object(papp)
      {

         System.factory().creatable_large    < ::windows::print_job          >(System.type_info < ::user::print_job           >());
         System.factory().creatable_small    < ::windows::copydesk           >(System.type_info < ::user::copydesk            >(), 1);
         System.factory().cloneable_large    < ::windows::window_buffer      >(System.type_info < ::window_graphics                > ());
         System.factory().creatable_large    < ::windows::interaction_impl   >(System.type_info < ::user::interaction_impl    >());

      }


      factory_exchange::~factory_exchange()
      {

      }


   } // namespace windows


} // namespace base


void __node_base_factory_exchange(::aura::application * papp)
{

   ::base::windows::factory_exchange factoryexchange(papp);

}




