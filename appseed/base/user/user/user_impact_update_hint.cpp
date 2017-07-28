//#include "framework.h" // from "base/user/user.h"
//#include "base/user/user.h"


namespace user
{


   view_update_hint::view_update_hint(::aura::application * papp) :
      ::object(papp)
   {

      m_ehint              = hint_none;
      m_bOk                = true;
      m_pusercontrolevent  = NULL;

   }


   view_update_hint::~view_update_hint()
   {

   }


   bool view_update_hint::is_type_of(e_hint ehint) const
   {

      return m_ehint == ehint;

   }


} // namespace user









