#pragma once


namespace wndfrm_core
{


   class CLASS_DECL_APP_CORE_WNDFRM_CORE user_schema:
      virtual public ::user::schema_simple_impl
   {
   public:



      user_schema(::aura::application * papp);
      virtual ~user_schema();

      virtual int64_t add_ref()
      {

         return ::object::add_ref();

      }

      virtual int64_t dec_ref()
      {

         return ::object::dec_ref();

      }

      bool _001TabOnDrawSchema01(::draw2d::graphics * pgraphics,::user::tab * ptab);
      void _001OnTabPaneDrawTitle(::user::tab_pane & pane,::user::tab * ptab,::draw2d::graphics * pgraphics,LPCRECT lpcrect,::draw2d::brush_sp & brushText);
      bool _001OnTabLayout(::user::tab * ptab);

      virtual bool get_color(COLORREF & cr, ::user::e_color ecolor);

      virtual bool _001DrawSimpleScrollBar(::draw2d::graphics * pgraphics, simple_scroll_bar * pbar);

   };


} // namespace wndfrm_rootkiller


