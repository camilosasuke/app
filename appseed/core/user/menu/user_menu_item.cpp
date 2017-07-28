//#include "framework.h"


namespace user
{


   menu_item::menu_item(::aura::application * papp) :
      object(papp),
      menu_base_item(papp),
      m_button(papp)
   {
      m_spitema      = canew(menu_item_ptra(this));
      m_iLevel       = 0;
      m_bPopup       = false;
      m_button.m_pitem = this;
   }

   menu_item::~menu_item()
   {

      //m_button.DestroyWindow();

   }


   void menu_item::add_item(menu_item * pitem)
   {

      m_spitema->add(pitem);

   }




   bool menu_item::create_menu(const stringa & straCommand, const stringa & straCommandTitle)
   {

      ::count iItemCount = straCommand.get_count();
      
      if(straCommandTitle.get_size() < straCommand.get_size())
         return false;

      for(int32_t i = 0; i < iItemCount; i++)
      {
         string strCommand = straCommand[i];
         string strCommandTitle = straCommandTitle[i];
         sp(menu_item) pitemNewChild = canew(menu_item(get_app()));
         pitemNewChild->m_bPopup = false;
         if(strCommand.is_empty())
         {
            pitemNewChild->m_id = "separator";
            m_iSeparatorCount++;
         }
         else
         {
            m_iFullHeightItemCount++;
            pitemNewChild->m_id = strCommand;
            pitemNewChild->m_iLevel = 0;
            pitemNewChild->m_button.set_window_text(strCommandTitle);
         }
         add_item(pitemNewChild);
      }

      return iItemCount > 0;

   }


   bool menu_item::load_menu(::xml::node * pnode)
   {

      m_iSeparatorCount       = 0;
      m_iFullHeightItemCount  = 0;

      return load_menu(pnode, m_iLevel);

   }


   bool menu_item::load_menu(::xml::node * pnode, int32_t iLevel)
   {

      ::count iItemCount = pnode->get_children_count();

      for(int32_t i = 0; i < iItemCount; i++)
      {

         sp(::xml::node) pnodeChild = pnode->child_at(i);

         sp(menu_item) pitemNewChild = canew(menu_item(get_app()));

         pitemNewChild->m_bPopup = pnodeChild->get_children_count() > 0 && pnodeChild->get_name() == "menubar";

         if(pitemNewChild->m_bPopup)
         {
            pitemNewChild->m_iLevel = iLevel + 1;
            pitemNewChild->load_menu(pnodeChild);
         }
         if(pnodeChild->get_name() == "separator")
         {
            pitemNewChild->m_id = "separator";
            m_iSeparatorCount++;
         }
         else
         {
            m_iFullHeightItemCount++;
            pitemNewChild->m_id = pnodeChild->attr("id");
            pitemNewChild->m_iLevel = iLevel;

            string strText;

            if(pitemNewChild->m_bPopup)
            {

               strText = pnodeChild->attr("title");

            }
            else
            {

               strText = pnodeChild->get_value();

            }

            pitemNewChild->m_button.set_window_text(strText);

         }
         add_item(pitemNewChild);
      }

      return true;
   }

   bool menu_item::IsPopup()
   {
      return m_spitema.is_set() && m_spitema->get_count() > 0;
   }

   menu_item_ptra::menu_item_ptra(menu_item * pitemContainer) :
      ::object(pitemContainer->get_app())
   {
      m_pitemContainer = pitemContainer;
   }

   menu_item_ptra::~menu_item_ptra()
   {
   }

   menu_item * menu_item::find(id id)
   {
      if(!m_spitema.is_set())
         return NULL;
      return m_spitema->find(id);
   }

   void menu_item::OnAddRef()
   {
   }

   void menu_item::OnRelease()
   {
   }

   menu_item * menu_item_ptra::find(id id)
   {
      menu_item * pitemFind;
      for(int32_t i = 0; i < this->get_size(); i++)
      {
         menu_item * pitem = element_at(i);
         if(pitem->m_id  == id)
            return pitem;
         pitemFind = pitem->find(id);
         if(pitemFind != NULL)
            return pitemFind;
      }
      return NULL;
   }


} // namespace user

