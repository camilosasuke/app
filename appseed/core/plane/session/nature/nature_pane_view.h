#pragma once


namespace nature
{


   class CLASS_DECL_ca2 pane_view : 
      virtual public ::userex::pane_tab_view
   {
   public:

      
      sp(::user::single_document_template) m_pdoctemplateAppearance;
      sp(::userex::keyboard_layout)             m_pkeyboardlayout;


      pane_view(sp(base_application) papp);
      virtual ~pane_view();


      using ::user::tab_view::on_show_view;
      virtual void on_create_view(::user::view_creator_data * pdata);
      virtual void on_show_view() ;

      virtual void install_message_handling(message::dispatch * pinterface);

      void on_update(sp(::user::view) pview, LPARAM lHint, object* pHint);

      
   #ifdef DEBUG
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;
   #endif

      DECL_GEN_SIGNAL(_001OnCreate)


   };


} // namespace nature





