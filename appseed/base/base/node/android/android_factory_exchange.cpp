#include "framework.h"
#include "axis/user/user.h"
#include "android.h"


namespace android
{


   namespace base
   {


      factory_exchange::factory_exchange(::aura::application * papp):
         ::object(papp)
      {
         System.factory().cloneable_large < ::window_double_buffer        >(System.type_info <::window_graphics                >());
         

         //System.factory().creatable < window_draw                 >(System. type_info < ::user::window_draw          >(),1);
         //System.factory().creatable < ::android::window_buffer                 >(System.type_info < ::window_graphics          >(), 1);
         System.factory().creatable_large < interaction_impl      >(System. type_info < ::user::interaction_impl     >());
         //System.factory().creatable < message_queue               >(System. type_info < ::user::message_queue        >(),1);
         //System.factory().creatable_small < copydesk              >(System. type_info < ::base::copydesk             >(),1);

      }


      factory_exchange::~factory_exchange()
      {

      }


   } // namespace base


} // namespace android



void __node_base_factory_exchange(::aura::application * papp)
{
   ::android::base::factory_exchange factoryexchange(papp);
}


