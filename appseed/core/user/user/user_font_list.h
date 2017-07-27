#pragma once


namespace user
{


   class font_list;


   class CLASS_DECL_CORE font_list :
      virtual public control
   {
   public:


      rect                             m_rectMargin;
      int                              m_iSel;
      ::visual::font_list_data *       m_pfontlistdata;


      font_list();
      font_list(::aura::application * papp);
      virtual ~font_list();




      void attach_font_list_data(::visual::font_list_data * pdata);
      //virtual void on_update_data(::visual::font_list_data * pdata, int32_t iHint);

      void font_list_common_construct();

      //virtual void assert_valid() const;
      //virtual void dump(dump_context & dumpcontext) const;

      virtual bool get_color(COLORREF & cr, e_color ecolor) override;


      virtual void install_message_handling(::message::dispatch * pdispatch);

      virtual void _001OnDraw(::draw2d::graphics * pgraphics);

      virtual void on_update(::user::impact * pSender, LPARAM lHint, object* pHint);


      string      get_cur_sel_face_name();
      int         get_cur_sel();

      string      get_cur_hover_face_name();
      int         get_cur_hover();

      DECL_GEN_SIGNAL(_001OnCreate);

      DECL_GEN_SIGNAL(_001OnLButtonDown);
      DECL_GEN_SIGNAL(_001OnMouseMove);

      virtual void _001OnTimer(::timer * ptimer) override;

      int hit_test(point pt);

      virtual void on_layout();

      bool set_sel_by_name(string str);

   };


   typedef ::user::show < ::user::font_list > font_list_view;


} // namespace user




















