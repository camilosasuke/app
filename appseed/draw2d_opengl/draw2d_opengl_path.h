#pragma once


namespace draw2d_opengl
{


   class CLASS_DECL_DRAW2D_OPENGL path :
      virtual public ::draw2d::path
   {
   public:


      //plusplus::GraphicsPath *    m_ppath;
      //plusplus::PointF            m_ptInternal;
      bool                       m_bHasPointInternal;



      path(::aura::application * papp);
      virtual ~path();


      //virtual plusplus::GraphicsPath * get_os_path(plusplus::Graphics * pgraphics);

      virtual bool internal_add_arc(const RECT & rect, double iStart, double iAngle);

      virtual pointd internal_last_point();

      virtual pointd internal_current_point();


      virtual bool internal_begin_figure(bool bFill, ::draw2d::e_fill_mode efillmode);
      virtual bool internal_end_figure(bool bClose);

      virtual bool internal_add_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
      virtual bool internal_add_rect(int32_t x,int32_t y,int32_t cx,int32_t cy);
      virtual bool internal_add_line(int32_t x, int32_t y);
      virtual bool internal_add_move(int32_t x,int32_t y);

      virtual bool internal_add_rect(double x,double y,double cx,double cy);
      virtual bool internal_add_move(double x,double y);

      //virtual bool internal_add_string(plusplus::Graphics * pgraphics, int32_t x,int32_t y,const string & strText,::draw2d::font_sp spfont);

//      virtual bool create(plusplus::Graphics * pgraphics);
      virtual bool destroy();

//      virtual bool set(plusplus::Graphics * pgraphics, const ::draw2d::path::element & e);

      virtual bool set(const ::draw2d::path::arc & a);

      virtual bool set(const ::draw2d::path::rect & r);

      virtual bool set(const ::draw2d::path::line & l);

      virtual bool set(const ::draw2d::path::move & p);

//      virtual bool set(plusplus::Graphics * pgraphics, const ::draw2d::path::string_path & p);


   };


} // namespace draw2d_opengl




