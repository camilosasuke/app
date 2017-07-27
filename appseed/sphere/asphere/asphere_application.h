#pragma once


namespace user
{

   class printer;


} // namespace user


namespace asphere
{


   class CLASS_DECL_SPHERE application : 
      virtual public ::core::application
   {
   public:


      
      stringa                                      m_straTheme;
      string                                       m_strTheme;
      ::file::path                                 m_pathTheme;
      string_map < COLORREF >                      m_mapBg;
      string_map < COLORREF >                      m_mapIconGlow;
      string_map < COLORREF >                      m_mapFg;
      string_map < COLORREF >                      m_mapMg;
      string_map < COLORREF >                      m_mapBack;
      string_map < COLORREF >                      m_mapText;
      COLORREF                                     m_crBg;
      COLORREF                                     m_crFg;
      COLORREF                                     m_crMg;
      COLORREF                                     m_crBack;
      COLORREF                                     m_crText;
      COLORREF                                     m_crIconGlow;


      application();
      virtual ~application();


      virtual bool initialize_application() override;
      virtual bool initialize1();
      virtual bool initialize();


      virtual void get_theme_text_color(COLORREF & crText, COLORREF & crBack);

      virtual void calc_theme(string strTheme = "");

      virtual ::file::path   get_theme_path();
      inline COLORREF get_theme_bg_color() { return m_crBg; }
      inline COLORREF get_theme_fg_color() { return m_crFg; }
      inline COLORREF get_theme_mg_color() { return m_crMg; }
      inline COLORREF get_theme_icon_glow_color() { return m_crIconGlow; }
      virtual void on_change_theme();


      virtual void _001OnTheme(signal_details * pobj);

      virtual void set_theme(string strTheme);

      virtual string get_theme();


   };



} // namespace sphere


