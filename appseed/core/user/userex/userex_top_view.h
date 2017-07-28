#pragma once


namespace userex
{


   class CLASS_DECL_CORE top_view :
      virtual public ::user::split_view
   {
   public:


      top_edit_view *                     m_peditview;
      top_toggle_view *                   m_ptoggleview;


      top_view(::aura::application * papp);
      virtual ~top_view();


      virtual void assert_valid() const;

      virtual void dump(dump_context & dumpcontext) const;

      void on_create_views();

      virtual void on_update(::user::impact * pSender, LPARAM lHint, object* pHint);

      virtual bool is_this_visible() override;


   };


} // namespace userex







