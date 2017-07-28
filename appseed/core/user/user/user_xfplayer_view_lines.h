#pragma once


class CLASS_DECL_CORE xfplayer_view_linea :
   virtual public spa(xfplayer_view_line),
   virtual public signalizable
{
public:


   XfplayerViewLineSelection     m_selection;
   ::user::interaction *         m_pinteraction;
   index                         m_iFirstVisible;
   index                         m_iLastVisible;


   xfplayer_view_linea(::aura::application * papp);


   void SetBlend(double dBlend);
   index FindLine(xfplayer_view_line * pline);
   void SetRenderWindow(::window_sp pwindow);
   void SetEffect(int32_t iEffect);
   void set_user_interaction(sp(::user::interaction) pinteraction);
   void Prepare(xfplayer_view_line * lpViewLine);
   void Prepare();

   index GetFirstVisibleLineIndex();
   index GetLastVisibleLineIndex();
   void OnChildSetVisible(xfplayer_view_line * lpViewLine, bool bVisible);

   user::e_line_hit hit_test(POINT &ptCursor, strsize &iLine, strsize &iChar);

   void InstallMessageHandling(::message::dispatch *pinterface);

   virtual ~xfplayer_view_linea();

   DECL_GEN_SIGNAL(OnMouseMove);
   DECL_GEN_SIGNAL(OnLButtonDown);
   DECL_GEN_SIGNAL(OnLButtonUp);
   virtual void _001OnTimer(::timer * ptimer);
   DECL_GEN_SIGNAL(OnSetCursor);

   XfplayerViewLineSelection & GetSelection();

   void get_sel_text(string & strSelText, const char * pszLineSeparator = "\r\n");
   string get_sel_text(const char * pszLineSeparator = "\r\n");

   void get_text(string & strSelText, const char * pszLineSeparator = "\r\n");
   string get_text(const char * pszLineSeparator = "\r\n");

};



