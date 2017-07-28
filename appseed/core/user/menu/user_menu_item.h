#pragma once


namespace user
{

   
   class CLASS_DECL_CORE menu_item :
      virtual public ::user::menu_base_item
   {
   public:


      id                         m_id;
      sp(menu_item_ptra)         m_spitema;
      int32_t                    m_iFullHeightItemCount;
      int32_t                    m_iSeparatorCount;
      menu_button                m_button;
      bool                       m_bPopup;
      int32_t                    m_iLevel;
      ::user::menu_base *        m_pbase;


      menu_item(::aura::application * papp);
      virtual ~menu_item();


      void add_item(menu_item * pitem);
      bool create_menu(const stringa & straCommand, const stringa & straCommandTitle);
      bool load_menu(::xml::node * pxmlnode);
      bool load_menu(::xml::node * pxmlnode, int32_t iLevel);

      virtual void OnAddRef();
      virtual void OnRelease();

      bool IsPopup();
      menu_item * find(id id);


   };

   class CLASS_DECL_CORE menu_item_ptra :
      public spa(menu_item)
   {
   public:


      menu_item *    m_pitemContainer;


      menu_item_ptra(menu_item * pitemContainer);
      virtual ~menu_item_ptra();


      menu_item * find(id id);

   };


} // namespace user
















