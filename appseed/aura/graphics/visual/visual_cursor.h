#pragma once


namespace visual
{


   class CLASS_DECL_AURA cursor :
      virtual public ::object
   {
   public:


      e_cursor                m_ecursor;
      ::draw2d::dib_sp        m_dib;
      size                    m_szHotspotOffset;



#ifdef WINDOWSEX

      HCURSOR                 m_hcursor;

#endif


      cursor(::aura::application * papp);

      bool to(::draw2d::graphics * pgraphics, point pt);

      bool initialize_system_default();

#ifdef WINDOWSEX

      HCURSOR get_HCURSOR();

#endif


   };


   typedef sp(cursor) cursor_sp;


   CLASS_DECL_AURA void cursor_alloc(::aura::application * papp,cursor * & pdib,int xHotspot,int yHotspot);
   CLASS_DECL_AURA ::draw2d::dib * cursor_get_dib(cursor * pcursor);


} // namespace visual



#ifdef WINDOWSEX

// Derived from Microsoft KB318876!!
// on 2014-06-29!! During World Cup Games 2014!! Oitavas de Final, third game between Nederland and Mexico!!
// Thank you God!!
// Thank you Microsoft!!
// Thank you Pappi!! Pelo passeio no Mercado Municipal de Curitiba com a Mummi e pelo almoço com o-bento de sushi e onigiri delicioso!!
// Obrigado Mummi por tudo!! E por hoje também e por propiciar isso tudo!! Por falar tão profundamente do Chico Xavier mesmo com muitas dúvidas minhas!!
// Obrigado Carlos!! Você poderia colaborar, não é!!
// Meu Deus me ajude!!

CLASS_DECL_AURA HBITMAP CreateAlphaBitmapV5(::draw2d::dib * pdib);
CLASS_DECL_AURA HICON CreateAlphaIcon(::draw2d::dib * pdib,bool bIcon = true,int xHotSpot = 0,int yHotSpot = 0);
CLASS_DECL_AURA HCURSOR CreateAlphaCursor(::draw2d::dib * pdib,int xHotSpot,int yHotSpot);

#endif