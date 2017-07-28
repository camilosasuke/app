

namespace fontopus
{


   //UINT c_cdecl thread_proc_pre_login(void * p);
   //   UINT c_cdecl thread_proc_defer_translate_login(void * p);


   view::view(::aura::application * papp) :
      ::object(papp),
      m_evSubmit(papp),
      m_mutexResponse(papp)
   {

      m_plabelUser = new ::simple_ui::label(papp);
      m_peditUser = new ::simple_ui::edit_box(papp);
      m_plabelPassword = new ::simple_ui::label(papp);
      m_ppassword = new ::simple_ui::password(papp);
      m_ptap = new ::simple_ui::tap(papp);
      m_ptapClose = new ::simple_ui::tap(papp);

      m_bSelfLayout = false;

      m_picon95 = NULL;

#if defined(WINDOWSEX)

      {

         HICON hicon95 = (HICON) ::LoadImage(::GetModuleHandle(NULL), MAKEINTRESOURCE(95), IMAGE_ICON, 95, 95, LR_VGACOLOR);

         if (hicon95 != NULL)
         {

            m_picon95 = new ::visual::icon(get_app(), hicon95);

         }

      }

#endif
      m_bCred = false;


   }

   view::~view()
   {
      /*
      if(m_pploginDeferTranslate != NULL)
      {

      try
      {

      *m_pploginDeferTranslate = NULL;

      }
      catch(...)
      {

      }

      }

      Sleep(49);*/

      ::aura::del(m_plabelUser);
      ::aura::del(m_peditUser);
      ::aura::del(m_plabelPassword);
      ::aura::del(m_ppassword);
      ::aura::del(m_ptap);
      ::aura::del(m_ptapClose);

   }


   void view::install_message_handling(::message::dispatch * pdispatch)
   {

      ::simple_ui::interaction::install_message_handling(pdispatch);

      IGUI_CREATE(view);

   }


   void view::defer_translate(const string & strUser, const string & strPass, const string & strOpen)
   {

      if (strUser.has_char())
         m_plabelUser->set_window_text(strUser);

      if (strPass.has_char())
         m_plabelPassword->set_window_text(strPass);

      if (strOpen.has_char())
         m_ptap->set_window_text(strOpen);

   }


   void view::initialize()
   {

   }


   void view::on_layout()
   {


      int stdw = 800;
      int stdh = 400;

      int h;
      int w;

      rect rectClient;

      GetClientRect(rectClient);

      /*
      if (m_bSelfLayout)
      {


      double dwh = (double)stdw / (double)stdh;

      int availw = (int) (rectClient.width() * (1.0 - 0.14));
      int availh = (int) (rectClient.height() * (1.0 - 0.14));

      double davailwh;

      if (availh == 0.0)
      {
      davailwh = 1.0;
      }
      else
      {
      davailwh = (double)availw / (double)availh;
      }


      if (davailwh > dwh) // remaining width
      {

      h = (int) MIN(stdh, availh);
      w = (int) MIN(stdw, h  * dwh);

      }
      else // remaining height
      {

      w = (int) MIN(stdw, availw);
      h = (int) MIN(stdh, w / dwh);

      }

      rectClient.left = (rectClient.width() - w) / 2;
      rectClient.top = (rectClient.height() - h) / 3;
      rectClient.right = rectClient.left + w;
      rectClient.bottom = rectClient.top + h;

      SetPlacement(rectClient);

      }
      else
      {

      GetClientRect(rectClient);

      }*/

      w = (int)(rectClient.width());

      h = (int)(rectClient.height());

      m_dRateX = (double)w / (double)stdw;

      m_dRateY = (double)h / (double)stdh;

      double rx = m_dRateX;

      double ry = m_dRateY;

      int32_t x1 = (int)(49 * rx);
      int32_t w2 = (int)((rectClient.width() - 49 * 2 * rx));
      int32_t h1 = (int)(23 * ry);
      int32_t pad = (int)(5 * ry);

      int32_t y = (int)((49 + 86) * ry);
      m_plabelUser->RepositionWindow(x1, y, w2, h1);
      y += h1 + pad;
      m_peditUser->RepositionWindow(x1, y, w2, h1);
      y += h1 + pad;
      m_plabelPassword->RepositionWindow(x1, y, w2, h1);
      y += h1 + pad;
      m_ppassword->RepositionWindow(x1, y, w2, h1);
      y += h1 + pad;
      y += pad + h1 + pad;
      m_ptap->RepositionWindow(x1, y, w2, h1 * 3);;

      m_ptapClose->RepositionWindow(w - 36, 12, 24, 24);

   }


   void view::_001OnDraw(::draw2d::graphics * pgraphics)
   {



      //return;

      simple_ui_draw_frame_window_rect(pgraphics);

      

      COLORREF crOut, crIn, crBorderOut, crBorderIn, cr, crBk;

      //       if (is_hover() || m_bDown || m_bMouseMove)
      /*       {

      #if CA2_PLATFORM_VERSION == CA2_BASIS

      crOut = ARGB(184 + 49, 255, 230, 255);

      crIn = ARGB(255, 255, 133, 255);

      crBorderOut = ARGB(184, 150, 100, 150);

      crBorderIn = ARGB(184, 255, 240, 255);

      #else

      crOut = ARGB(184 + 49, 230, 255, 225);

      crIn = ARGB(255, 133, 255, 77 + 49);

      crBorderOut = ARGB(184, 100, 150, 100);

      crBorderIn = ARGB(184, 240, 255, 235);

      #endif

      }
      else*/
      {

#if CA2_PLATFORM_VERSION == CA2_BASIS

         /*crOut = ARGB(184, 255, 210, 255);

         crIn = ARGB(255, 255, 184 + 49, 255);

         crBorderOut = ARGB(184, 90, 20, 90);

         crBorderIn = ARGB(184, 255, 240, 255);*/

         crOut = ARGB(255, 255, 210, 255);

         crIn = ARGB(255, 255, 184 + 49, 255);

         crBorderOut = ARGB(255, 90, 20, 90);

         crBorderIn = ARGB(255, 255, 255, 255);

#else

         crOut = ARGB(184, 210, 255, 205);

         crIn = ARGB(255, 133, 255, 77 + 49);

         crBorderOut = ARGB(184, 20, 90, 20);

         crBorderIn = ARGB(184, 240, 255, 235);

#endif

      }


#if CA2_PLATFORM_VERSION == CA2_BASIS

      //cr = ARGB(223, 84, 49, 77);
      cr = ARGB(255, 84, 49, 77);

#else

      //cr = ARGB(223, 49, 84, 23);
      cr = ARGB(255, 49, 84, 23);

#endif

      crBk = ARGB(
         (argb_get_a_value(crOut) + argb_get_a_value(crIn)) / 2,
         (argb_get_r_value(crOut) + argb_get_r_value(crIn)) / 2,
         (argb_get_g_value(crOut) + argb_get_g_value(crIn)) / 2,
         (argb_get_b_value(crOut) + argb_get_b_value(crIn)) / 2);

      double rx = m_dRateX;

      double ry = m_dRateY;

      if (m_bCred && m_strCred.has_char())
      {
         float fHeight = 18.0;

         ::draw2d::font_sp f(allocer());

         /*f->create_pixel_font(FONT_SANS_EX, (int32_t)height(rectClient) * 0.7);

         float fMargin = (height(rectClient) * ((1.0f - 0.7f) / 2.0f));*/

         f->create_point_font(FONT_SANS_EX, fHeight * 1.0);


         pgraphics->SelectObject(f);

         pgraphics->set_text_color(crBorderOut);
         pgraphics->text_out((int)(49 * rx), (int)(49 * ry), m_strCred);
      }
      else if (m_picon95)
      {

         pgraphics->draw_ca2_border2((int)(49 * rx), (int)(49 * ry) - 11, (int)((91 + 2 + 2) * ry), 1, 1, crBk, cr, crBorderOut, crBorderIn);

         pgraphics->DrawIcon((int)(49 * rx) + 2, (int)(49 * ry) + 2 - 11, m_picon95, (int)((91 + 2 + 2) * ry), (int)((91 + 2 + 2) * ry), 0, NULL, 0);

      }
      else
      {

         pgraphics->draw_ca2_with_border2((int)(49 * rx), (int)(49 * ry) - 23, (int)((91 + 2 + 2) * ry), 1, 1, crBk, cr, crBorderOut, crBorderIn);

      }


   }


   bool view::on_action(const char * pszId)
   {

      if (!strcmp(pszId, "submit"))
      {
       

         if (!m_bCred)
         {
            string strText;
            m_ppassword->_001GetText(strText);
            strText = System.crypto().nessie(strText);
            m_ppassword->_001SetText(strText, ::action::source::database());
         }
         
         m_strResponse = "ok";

         m_evSubmit.SetEvent();

      }
      else if (!strcmp(pszId, "escape"))
      {

         GetParent()->ShowWindow(SW_HIDE);
         m_ppassword->_001SetText("", ::action::source::database());
         GetParent()->EndModalLoop("cancel");

         return true;

      }

      return false;


   }



   void view::_001OnCreate(signal_details * pobj)
   {

      SCAST_PTR(::message::create, pcreate, pobj);

      if (pcreate->previous())
         return;

      if (!m_plabelUser->create_window(null_rect(), this, "label_user")
         || !m_peditUser->create_window(null_rect(), this, "edit_user")
         || !m_plabelPassword->create_window(null_rect(), this, "label_password")
         || !m_ppassword->create_window(null_rect(), this, "password")
         || !m_ptap->create_window(null_rect(), this, "submit")
         || !m_ptapClose->create_window(null_rect(), this, "escape"))
      {
         pcreate->set_lresult(-1);
         pcreate->m_bRet = true;
         return;
      }

      m_ptapClose->m_estockicon = stock_icon_close;

      //m_plabelUser->set_window_text("e-mail:");
      //m_plabelPassword->set_window_text("password:");
      //m_ptap->set_window_text("open");

      //m_plabelUser->set_window_text("@");
      //m_plabelPassword->set_window_text("********");
      //m_ptap->set_window_text("===>");

      m_plabelUser->set_window_text("");
      m_plabelPassword->set_window_text("");
      m_ptap->set_window_text("");

      int stdw = 800;
      int stdh = 177 + 23 + 184 + 49;

      RepositionWindow(0, 0, stdw, stdh);

      m_peditUser->keyboard_set_focus();


      //m_pploginDeferTranslate = new view *;

      //*m_pploginDeferTranslate = this;

      //add_ref();

      //__begin_thread(get_app(),thread_proc_pre_login,get_app());

   }



   //UINT c_cdecl thread_proc_pre_login(void * p)
   //{

   //   ::aura::application * papp = (::aura::application *) p;

   //   return 0;

   //   login_thread thread(papp);
   //   thread.m_strRequestingServer = "account.ca2.cc";
   //   thread.m_strUsername = "";
   //   thread.m_strPassword = "";
   //   thread.run();

   //   return 0;

   //}


   void view::_000OnDraw(::draw2d::graphics * pgraphics)
   {

      //simple_ui::interaction::_000OnDraw(pgraphics);


      if (!is_this_visible())
         return;

      _001DrawThis(pgraphics);

      try
      {

         _001DrawChildren(pgraphics);

      }
      catch (...)
      {

         throw simple_exception(::get_thread_app(), "no more a window");

      }

   }


   void view::_001DrawChildren(::draw2d::graphics * pgraphics)
   {

      //single_lock sl(m_pmutex, true);

      //int i = 5;

      sp(::user::interaction) pui;

      //while((pui = get_child(pui)).is_set() && i > 0)
      while (get_child(pui))
      {

         //i--;

         try
         {

            if (pui->is_this_visible() && !pui->is_custom_draw())
            {

               pui->_000OnDraw(pgraphics);

            }

         }
         catch (...)
         {

         }

      }

   }

   string view::get_cred(const string & strRequestUrl, const RECT & rect, string & strUsername, string & strPassword, string strToken, string strTitle, bool bInteractive)
   {

      synch_lock sl(&m_mutexResponse);

      m_strRequestUrl = strRequestUrl;

      m_strResponse = Application.get_cred(strUsername, strPassword, strToken);

      if (m_strResponse == "ok")
      {

         return m_strResponse;

      }

      m_peditUser->_001SetText(strUsername, ::action::source_initialize);

      m_ppassword->_001SetText(strPassword, ::action::source_initialize);

      m_strResponse.Empty();

      m_evSubmit.ResetEvent();

      m_evSubmit.wait();

      if (m_strResponse == "ok")
      {

         m_peditUser->_001GetText(strUsername);

         m_ppassword->_001GetText(strPassword);

      }

      return m_strResponse;

   }


} // namespace fontopus




