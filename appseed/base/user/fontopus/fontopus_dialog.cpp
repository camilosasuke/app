//#include "framework.h"
//#include "base/user/user.h"




namespace fontopus
{


   dialog::dialog(::aura::application * papp, const string & strRequestUrl) :
      ::object(papp),
      ::simple_ui::style(papp),
      m_login(papp, 0, 0, strRequestUrl)
   {

      ASSERT(m_login.m_strRequestUrl.has_char());
#ifdef METROWIN
      m_bMayProDevian = true;
#else
      m_bMayProDevian = false;
#endif
      m_eschema = schema_normal;
      m_login.m_pstyle = this;
      m_bLButtonDown = false;
      m_bFontopusSimpleUiLayout = false;


   }


   dialog::~dialog()
   {

   }


   void dialog::install_message_handling(::message::dispatch * pdispatch)
   {

      ::simple_ui::interaction::install_message_handling(pdispatch);

      IGUI_WIN_MSG_LINK(WM_CREATE,pdispatch,this,&dialog::_001OnCreate);
      IGUI_WIN_MSG_LINK(WM_CHAR,pdispatch,this,&dialog::_001OnChar);
      IGUI_WIN_MSG_LINK(WM_LBUTTONDOWN,pdispatch,this,&dialog::_001OnLButtonDown);
      IGUI_WIN_MSG_LINK(WM_LBUTTONUP,pdispatch,this,&dialog::_001OnLButtonUp);
      IGUI_WIN_MSG_LINK(WM_MOUSEMOVE,pdispatch,this,&dialog::_001OnMouseMove);

   }


   void dialog::_001OnCreate(signal_details * pobj)
   {

      SCAST_PTR(::message::create,pcreate,pobj);

      if(pcreate->previous())
         return;

      if(!m_login.create_window(null_rect(), this,"pane_first"))
      {

         pcreate->set_lresult(-1);

         pcreate->m_bRet = true;

         return;

      }

      string strUser;

      string strPass;

      string strOpen;

      strUser = lstr("fontopus::login::email","e-mail");

      strPass = lstr("fontopus::login::password","password");

      strOpen = lstr("fontopus::login::open","open");

      m_login.defer_translate(strUser,strPass,strOpen);

   }


   void dialog::_001OnChar(signal_details * pobj)
   {

      SCAST_PTR(::message::key, pkey, pobj);

      if(pkey->m_ekey == ::user::key_return)
      {

         m_login.on_action("submit");

         pobj->m_bRet = true;

      }
      else if(pkey->m_ekey == ::user::key_escape)
      {

         m_login.on_action("escape");

         pobj->m_bRet = true;

      }

   }


   //void dialog::GetWindowRect(LPRECT lprect)
   //{

   //   *lprect = m_rect;

   //}


   void dialog::_001OnTimer(::timer * ptimer)
   {

      ::simple_ui::interaction::_001OnTimer(ptimer);

      if(ptimer->m_nIDEvent == 1984 && !m_login.m_bCred)
      {

         try
         {

            string strUsername;
            string strPassword;

            string str = ::fontopus::get_cred(get_app(),strUsername,strPassword,"ca2");

            if(strUsername.has_char() && strPassword.has_char() && str == "ok")
            {
               KillTimer(1984);

               m_login.on_action("submit");



            }

         }
         catch(...)
         {

         }

      }

   }



   string dialog::do_fontopus(const RECT & rectParam)
   {

      ::user::interaction * puiParent = Session.oprop("plugin_parent").cast < ::user::interaction >();

      ::rect rectDesktop;

      if(puiParent != NULL)
      {

         puiParent->GetWindowRect(rectDesktop);

      }
      else if (IsRectEmpty(&rectParam))
      {

         Session.get_main_monitor(rectDesktop);

      }
      else
      {

         rectDesktop = rectParam;

      }

      rect rectFontopus;

      rect rectLogin;

      int stdw = 800;

      int stdh = 400;

      int w = stdw;

      int h = stdh;

      if(w > rectDesktop.width())
      {

         w = rectDesktop.width();

      }

      if(h > rectDesktop.height())
      {

         h = rectDesktop.height();

      }

      rectFontopus.left = rectDesktop.left + (width(rectDesktop) - w) / 2;
      rectFontopus.top = rectDesktop.top + (height(rectDesktop) - h) / 3;
      rectFontopus.right = rectFontopus.left + w;
      rectFontopus.bottom = rectFontopus.top + h;

      if(puiParent != NULL)
         puiParent->ScreenToClient(rectFontopus);

      if((rectFontopus.width() < 300 || rectFontopus.height() < 300) && puiParent != NULL)
      {
         sp(::aura::application) papp = puiParent->get_app();
         if(papp != NULL)
         {
            papp->open_link("ca2account:this", "", "");
            Sleep(1984 + 1977);
         }
         else
         {
            System.open_link("ca2account:this", "", "");
         }
         return "";
      }

      if(!create_window_ex(0,NULL,NULL,0,rectFontopus,puiParent,"fontopus"))
         return "";

      set_window_text( "fontopus Auth Windows");

      SetWindowPos(ZORDER_TOP,rectFontopus,SWP_SHOWWINDOW);

      m_login.m_peditUser->SetFocus();

      //m_login.on_layout();

      m_login.ShowWindow(SW_NORMAL);

      SetForegroundWindow();

      ShowWindow(SW_NORMAL);

      BringWindowToTop();

      SetTimer(1984,284,NULL);

      id idResult = RunModalLoop();

      return idResult;

   }


   string dialog::get_cred(const RECT & rect,string & strUsername,string & strPassword,string strToken,string strTitle)
   {

      if(strTitle == "ca2")
      {

         m_login.m_bCred = false;

         m_login.m_strCred = "";

      }
      else
      {

         m_login.m_bCred = true;

         stringa stra;
         stra.explode("|", strTitle);
         if (stra.get_size() >= 2)
         {
            m_login.m_dib.load_from_matter(stra[0]);
            m_login.m_strCred = stra.implode("|", 1);
         }
         else
         {
            m_login.m_strCred = strTitle;
         }

      }

      id idResult = do_fontopus(rect);

      if(idResult == "ok")
      {

         m_login.m_peditUser->_001GetText(strUsername);

         m_login.m_ppassword->_001GetText(strPassword);

         return "ok";

      }
      else if(idResult == "cancel")
      {

         return "cancel";

      }
      else
      {

         return "fail";

      }

   }


   void dialog::on_layout()
   {

      simple_ui::interaction::on_layout();

      /*

      if(!m_bFontopusSimpleUiLayout)
      {

         keep < bool > keepLayout(&m_bFontopusSimpleUiLayout,true,false,true);

         rect rectClient1;

         GetClientRect(rectClient1);

         bool bParentChange = false;

         if(GetParent() != NULL)
         {

            ::rect rectParent;

            GetParent()->GetWindowRect(rectParent);

            if(rectParent != m_rectParent)
            {

               bParentChange = true;

            }

         }


         if(rectClient1.area() < 100 * 100 || bParentChange)
         {

            ::rect rectDesktop;

            if(GetParent() != NULL)
            {

               GetParent()->GetWindowRect(rectDesktop);

            }
            else
            {

               Session.get_main_monitor(rectDesktop);

            }

            rect rectFontopus;

            rect rectLogin;

            int stdw = 800;

            int stdh = 184 + 23 + 184;

            int w = stdw;

            int h = stdh;

            if(w > rectDesktop.width())
            {

               w = rectDesktop.width();

            }

            if(h > rectDesktop.height())
            {

               h = rectDesktop.height();

            }

            rectFontopus.left = rectDesktop.left + (width(rectDesktop) - w) / 2;
            rectFontopus.top = rectDesktop.top + (height(rectDesktop) - h) / 3;
            rectFontopus.right = rectFontopus.left + w;
            rectFontopus.bottom = rectFontopus.top + h;


            if(GetParent() != NULL)
               GetParent()->ScreenToClient(rectFontopus);


            SetWindowPos(ZORDER_TOP,rectFontopus,SWP_SHOWWINDOW);

         }

      }
      */


      rect rectClient;

      GetClientRect(rectClient);

      m_login.SetPlacement(rectClient);

      //m_login.on_layout();

      

   }


   void dialog::_001OnLButtonDown(signal_details * pobj)
   {

      SCAST_PTR(::message::mouse,pmouse,pobj);

      if (pobj->previous())
         return;

      get_wnd()->show_keyboard(false);

      m_bLButtonDown = true;
      m_bDrag = false;

      m_ptLButtonDown = pmouse->m_pt;
      m_ptLButtonDownPos = m_ptLButtonDown;
      ScreenToClient(&m_ptLButtonDownPos);
      SetCapture();

      pmouse->m_bRet = true;

   }


   void dialog::_001OnLButtonUp(signal_details * pobj)
   {

      m_bLButtonDown = false;

      SCAST_PTR(::message::mouse,pmouse,pobj);

      if(pmouse->previous())
         return;

      ReleaseCapture();

      m_bDrag = false;

      pobj->m_bRet = true;

   }


   void dialog::_001OnMouseMove(signal_details * pobj)
   {

      SCAST_PTR(::message::mouse,pmouse,pobj);

      if (m_bLButtonDown)
      {
         if (!m_bDrag)
         {
            m_bDrag = true;
            POINT ptNow = pmouse->m_pt;
            point pt;
            pt.x = ptNow.x - m_ptLButtonDownPos.x;
            pt.y = ptNow.y - m_ptLButtonDownPos.y;
            MoveWindow(pt);
            m_bDrag = false;
         }
         pobj->m_bRet = true;

      }
      else
      {
         if(pobj->previous())
            return;

         pobj->m_bRet = true;
      }

   }









   void dialog::_000OnDraw(::draw2d::graphics * pgraphics)
   {

      //pgraphics->FillSolidRect(0, 0, 100, 100, ARGB(255, 255, 0, 0));

      ::simple_ui::interaction::_000OnDraw(pgraphics);

      //pgraphics->FillSolidRect(100, 100, 100, 100, ARGB(255, 0, 255, 0));

   }


   void dialog::_001DrawChildren(::draw2d::graphics * pgraphics)
   {

      ::simple_ui::interaction::_001DrawChildren(pgraphics);

   }

   
   bool dialog::has_pending_graphical_update()
   {
      
      return IsWindowVisible();
      
   }


} // namespace fontopus


