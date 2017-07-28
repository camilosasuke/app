//#include "framework.h" // from "base/user/user.h"
//#include "base/user/user.h"


#define SIZEX 584
#define SIZEY 384


namespace simple_ui
{


   message_box::message_box(::aura::application * papp,const char * pszMessage,const char * pszCaption, uint32_t uiFlags):
      ::object(papp),
      ::simple_ui::top(papp),
      ::simple_ui::style(papp),
      m_evReady(papp)
   {

      m_evReady.ResetEvent();
      m_bMayProDevian   = false;
      m_strMessage      = pszMessage;
      m_strTitle        = pszCaption;
      m_uiFlags         = uiFlags;
      m_iResult         = 0;


   }


   message_box::~message_box()
   {

   }


   void message_box::create_a_button(id id,const char * pszText)
   {

      create_button(m_tapaA,id,pszText);

   }


   void message_box::create_b_button(id id,const char * pszText)
   {

      create_button(m_tapaB,id,pszText);

   }


   void message_box::create_button(spa(tap) & tapa,id id,const char * pszText)
   {

      sp(::simple_ui::tap) ptap = canew(::simple_ui::tap(get_app()));
      ptap->create_window(null_rect(), this,id);
      ptap->set_window_text(pszText);

      tapa.add(ptap);

   }

   int32_t message_box::show()
   {



      ::rect rectDesktop;

      stringa stra;

      stra.add("\r");

      stra.add("\n");

      stra.add("\r\n");

      m_stra.add_smallest_tokens(m_strMessage,stra);

      ::user::interaction * puiParent = Session.oprop("plugin_parent").cast < ::user::interaction >();

      if(puiParent != NULL)
      {

         puiParent->GetWindowRect(rectDesktop);

      }
      else
      {

         Session.get_main_monitor(rectDesktop);

      }

      set_window_text(m_strTitle);

      rect rectFontopus;

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

      if(!create_window_ex(WS_EX_LAYERED,NULL,NULL,0,rectFontopus,puiParent,"fontopus"))
         throw simple_exception(get_app(),"not excepted! Failing Message box!!");

      TRACE("(1) Just after create_window_ex for simple_message_box (m_pimpl.m_p) : %d",m_pimpl.m_p);
      TRACE("(2) Just after create_window_ex for simple_message_box (m_pimpl->m_pui) : %d",m_pimpl->m_pui);

      uint32_t uiType = m_uiFlags & MB_TYPEMASK;

      switch(uiType)
      {
      case MB_OKCANCEL:
         create_a_button("ok","OK");
         create_a_button("cancel","Cancel");
         break;
      case MB_ABORTRETRYIGNORE:
         create_a_button("abort","Abort");
         create_a_button("retry","Retry");
         create_a_button("ignore","Ignore");
         break;
      case MB_YESNOCANCEL:
         create_a_button("yes","Yes");
         create_a_button("no","No");
         create_a_button("cancel","Cancel");
         break;
      case MB_YESNO:
         create_a_button("yes","Yes");
         create_a_button("no","No");
         break;
      case MB_RETRYCANCEL:
         create_a_button("retry","Retry");
         create_a_button("cancel","Cancel");
         break;
      case MB_CANCELTRYCONTINUE:
         create_a_button("cancel","Cancel");
         create_a_button("try","Try");
         create_a_button("continue","Continue");
         break;
      default:
         create_a_button("ok","OK");
         break;
      }

      TRACE("(3) Just after create_window_ex for simple_message_box (m_pimpl.m_p) : %d",m_pimpl.m_p);
      TRACE("(4) Just after create_window_ex for simple_message_box (m_pimpl->m_pui) : %d",m_pimpl->m_pui);


      //on_layout();

      SetWindowPos(ZORDER_TOP,rectFontopus,SWP_SHOWWINDOW);

      SetForegroundWindow();

      BringWindowToTop();

      BringToTop(SW_NORMAL);

      RunModalLoop();

      return m_iResult;

   }


   void message_box::_001OnDraw(::draw2d::graphics * pgraphics)
   {



      rect rectClient;

      GetClientRect(rectClient);

      COLORREF crBk;

      ::datetime::time timeNow = ::datetime::time::get_current_time();

      if (timeNow.GetHour() >= 6 && timeNow.GetHour() <= 17)
      {

         crBk = ARGB(255, 0xcd, 0xcd, 0xc8);

      }
      else
      {

         crBk = ARGB(255, 0x50, 0x50, 0x58);

      }

      pgraphics->FillSolidRect(rectClient,crBk);

      sp(::draw2d::font) font(allocer());

      font->create_point_font(FONT_SANS,12);

      pgraphics->selectFont(font);

      ::draw2d::text_metric tm;

      pgraphics->get_text_metrics(&tm);

      //int iHeight = tm.tmHeight;

      //int y = 10;

      COLORREF crText;


      if (timeNow.GetHour() >= 6 && timeNow.GetHour() <= 17)
      {

         crText = ARGB(255, 49, 49, 42);

      }
      else
      {

         crText = ARGB(255, 255, 255, 255);

      }


      pgraphics->set_text_color(crText);

      string strMessage = m_stra.implode("\n");

      ::rect r(rectClient);

      r.deflate(10, 10);

      pgraphics->draw_text(strMessage,r,DT_LEFT | DT_TOP | DT_EXPANDTABS);

      //for(int i = 0; i < m_stra.get_size(); i++)
      //{

      //   pgraphics->text_out(10,y,m_stra[i]);

      //   y+= iHeight;

      //}


   }


   void message_box::on_layout()
   {

      rect rectClient;

      GetClientRect(rectClient);

      // DESKTOP sizing
      int cx = (84 + 77) * 2 / 3;
      int cy = 49 / 2;
      int margin = 10;
      int x = rectClient.left + margin;
      int y = rectClient.bottom - cy - margin;


      for(index i = 0; i < m_tapaA.get_count(); i++)
      {

         sp(tap) ptap = m_tapaA[i];

         ptap->SetWindowPos(ZORDER_TOP,x,y,cx,cy,SWP_SHOWWINDOW);

         x += cx + margin;

      }

      x = rectClient.right - margin - cx;

      for(index i = m_tapaB.get_upper_bound(); i >= 0; i--)
      {

         sp(tap) ptap = m_tapaB[i];

         ptap->SetWindowPos(ZORDER_TOP,x,y,cx,cy,SWP_SHOWWINDOW);

         x -= cx + margin;

      }


   }


   bool message_box::on_action(const char * pszId)
   {

      if(stricmp_dup(pszId,"ok") == 0)
      {

         m_iResult = IDOK;

         EndModalLoop(IDOK);

         return true;

      }
      else if(stricmp_dup(pszId,"yes") == 0)
      {

         m_iResult = IDYES;

         EndModalLoop(IDOK);

         return true;

      }
      else if(stricmp_dup(pszId,"no") == 0)
      {

         m_iResult = IDNO;

         EndModalLoop(IDOK);

         return true;

      }
      else if(stricmp_dup(pszId,"cancel") == 0)
      {

         m_iResult = IDCANCEL;

         EndModalLoop(IDOK);

         return true;

      }
      else if(stricmp_dup(pszId,"abort") == 0)
      {

         m_iResult = IDABORT;

         EndModalLoop(IDOK);

         return true;

      }
      else if(stricmp_dup(pszId,"retry") == 0)
      {

         m_iResult = IDRETRY;

         EndModalLoop(IDOK);

         return true;

      }
      else if(stricmp_dup(pszId,"ignore") == 0)
      {

         m_iResult = IDIGNORE;

         EndModalLoop(IDOK);

         return true;

      }
      else if(stricmp_dup(pszId,"try") == 0)
      {

         m_iResult = IDTRYAGAIN;

         EndModalLoop(IDOK);

         return true;

      }
      else if(stricmp_dup(pszId,"continue") == 0)
      {

         m_iResult = IDCONTINUE;

         EndModalLoop(IDOK);

         return true;

      }


      return false;

   }


   void message_box::pre_translate_message(signal_details * pobj)
   {

      simple_ui::interaction::pre_translate_message(pobj);

   }


   bool message_box::has_pending_graphical_update()
   {

      return true;

   }


//   void message::EndModalLoop(id nResult)
//   {
//
//      simple_ui::interacion::EndModalLoop(nResult);
//
//   }



} // namespace simple_ui


extern "C"
CLASS_DECL_BASE int32_t system_message_box(oswindow interaction_impl,const char * lpText,const char * lpCaption,uint32_t uiFlags)
{

   return MessageBoxW(interaction_impl,wstring(lpText),wstring(lpCaption),uiFlags);

}

UINT c_cdecl thread_proc_simple_ui_message_box(LPVOID lpvoid)
{

#ifdef WINDOWSEX

   attach_thread_input_to_main_thread(false);

#endif

   ::simple_ui::message_box * pmessagebox = (::simple_ui::message_box *) lpvoid;

   try
   {

      pmessagebox->show();

   }
   catch(...)
   {

   }

   try
   {

      pmessagebox->DestroyWindow();

   }
   catch(...)
   {

   }


   pmessagebox->m_evReady.SetEvent();



   return 0;

}

int32_t simple_ui_message_box(oswindow interaction_impl,const char * lpText,const char * lpCaption,uint32_t uiFlags)
{


#ifdef WINDOWSEX

   MESSAGE msg;

   bool bQuit = PeekMessage(&msg,NULL,WM_QUIT,WM_QUIT,PM_REMOVE) != FALSE;

#endif

//   if(::get_thread_app() == NULL || ::get_thread_app()->m_pbasesession == NULL
  //    || &Sys(::get_thread_app()) == NULL || Sys(::get_thread_app()).get_twf() == NULL)
   if(::get_thread_app() == NULL || ::get_thread_app()->m_paxissession == NULL || ::user::g_pmapImpl == NULL)
   {

      return system_message_box(interaction_impl,lpText,lpCaption,uiFlags);

   }

   int32_t iResult = 0;

   {

      ::simple_ui::message_box * pmessagebox = new ::simple_ui::message_box(::get_thread_app(),lpText,lpCaption,uiFlags);

      try
      {


         //__begin_thread(::get_thread_app(),&thread_proc_simple_ui_message_box,pmessagebox);
         thread_proc_simple_ui_message_box(pmessagebox);
         //pmessagebox->show();

         //while(!pmessagebox->m_evReady.wait(millis(23)).signaled())
         //{

           // ::get_thread_app()->defer_pump_message();

         //}

         /*         if(bWasLocked)
                  {
                  ::get_thread()->m_pslUser->lock();
                  }*/

         iResult = pmessagebox->m_iResult;

      }
      catch(...)
      {

      }

      try
      {

         delete pmessagebox;

      }
      catch(...)
      {

      }

   }

#ifdef WINDOWSEX
   if(bQuit)
      PostQuitMessage((int32_t)msg.wParam);
#endif // WINDOWS
   return iResult;

}






