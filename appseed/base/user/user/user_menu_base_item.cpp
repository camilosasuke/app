//#include "framework.h" // from "base/user/user.h"
//#include "base/user/user.h"


namespace user
{


   menu_base_item::menu_base_item(::aura::application * papp) :
      ::object(papp)
   {

   }


   menu_base_item::~menu_base_item()
   {

   }


   bool menu_base_item::create_menu(const stringa & straCommand, const stringa & straCommandTitle)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }

   bool menu_base_item::load_menu(::xml::node * pnode)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


} // namespace user




