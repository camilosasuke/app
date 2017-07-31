//#include "framework.h"
//#include "base/user/user.h"


#ifdef HOTPLUGIN_SUBSYSTEM


namespace hotplugin
{


   composer::composer()
   {

      m_bFocus                   = false;

      m_bActive                  = false;

      m_bActivationStateSent     = false;

      m_bSendActivationState     = false;

      m_bRectSent                = false;

      m_bInit                 = true; // default implemenation assume initialized on construction, derivations may change it

      m_estatus               = status_start_axis_system;
      m_bBaseSystemOk         = false;
      m_bComposerSystemOk     = false;
      m_bTryInitHost          = false;
      m_bHostOk               = false;
      m_bTryInitWindow        = false;
      m_bWindowOk             = false;
      m_bOk                   = false;
      m_bResponsive           = true;
      m_bWrite                = false;
      m_bEntryHallTextStarted = false;
      
      m_pcomposersystem       = NULL;

      m_paxishost             = NULL;

      m_bHostInit             = false;

   }
   
   
   composer::~composer()
   {

   }


   ::hotplugin::host * composer::create_host(sp(::axis::system) psystem)
   {

      return NULL;

   }


   void composer::hotplugin_composer_on_timer()
   {

      if(m_estatus == status_start_axis_system)
      {

         if(m_bBaseSystemOk)
         {

            m_estatus = status_start_composer_system;

         }
         else
         {

            if(::hotplugin::get_axis_system() == NULL)
            {

               ::hotplugin::defer_start_base_system();

            }
            else
            {


               if(::hotplugin::get_axis_system()->m_bReady)
               {

                  if(::hotplugin::get_axis_system()->m_iReturnCode != 0)
                  {

                     string str;
                     
                     str.Format("::hotplugin::g_pbasesystem initialization error %d",::hotplugin::get_axis_system()->m_iReturnCode);

                     ::output_debug_string(str);

                  }
                  else
                  {

                     m_bBaseSystemOk = true;

                  }

               }

            }


         }


      }
      else if(m_estatus == status_start_composer_system)
      {

         if(m_bComposerSystemOk)
         {

            m_estatus = status_start_host;

         }
         else
         {

            if(get_composer_system() == NULL)
            {

               defer_start_composer_system();

            }
            else
            {


               if(get_composer_system()->m_bReady)
               {

                  if(get_composer_system()->m_iReturnCode != 0)
                  {

                     string str;

                     str.Format("m_pcomposersystem initialization error %d",get_composer_system()->m_iReturnCode);

                     ::output_debug_string(str);

                  }
                  else
                  {

                     m_bComposerSystemOk = true;

                  }

               }

            }


         }


      }
      else if(m_estatus == status_start_host)
      {

         if(m_bHostOk)
         {

            m_estatus = status_init_host;

         }
         else if(m_bInit)
         {

            if(m_paxishost == NULL)
            {

               m_paxishost = create_host(get_composer_system());

               m_paxishost->m_pbasecomposer = this;

               m_paxishost->hotplugin_host_begin();

            }
            else
            {

               if(m_paxishost->m_bReady)
               {

                  if(m_paxishost->m_iReturnCode != 0)
                  {

                     string str;

                     str.Format("::hotplugin::composer::m_paxishost initialization error %d",::hotplugin::get_axis_system()->m_iReturnCode);

                     ::output_debug_string(str);

                  }
                  else
                  {

                     m_bHostOk = true;

                  }

               }

            }

         }

      }
      else if(m_estatus == status_init_host)
      {

         if(m_bHostInit)
         {

            m_estatus = status_start_window;

         }
         else
         {

            m_bTryInitHost = true;

            if(!m_paxishost->hotplugin_host_is_initialized())
            {

               if(m_paxishost->hotplugin_host_initialize())
               {

                  m_bHostInit = true;

               }

            }

         }

      }
      else if(m_estatus == status_start_window)
      {

         if(m_bWindowOk)
         {

            m_estatus = status_ok;

         }
         else if(!m_bTryInitWindow)
         {

            m_bTryInitWindow = true;

            if(on_composer_init_window())
            {
               
               m_bWindowOk = true;

            }

         }

      }
      else if(m_estatus == status_ok)
      {

         if(!m_bOk)
         {

            m_bOk = true;

         }
      }


      if(m_paxishost != NULL && m_paxishost->hotplugin_host_is_initialized())
      {

         if(m_bWrite)
         {

            m_bWrite = false;

            m_paxishost->hotplugin_host_on_write(); // at least m_strPluginUrl is ready

         }

      }


      if(m_bOpenUrl)
      {

         m_bOpenUrl = false;

         if(!_open_link(m_strOpenUrl, m_strOpenTarget))
         {

            m_bOpenUrl = true;

         }

      }

   }


   bool composer::on_composer_init_window()
   {

      return true;

   }


   bool composer::open_link(const string & strLink,const string & pszTarget)
   {

      m_strOpenUrl = strLink;

      m_strOpenTarget = pszTarget;

      m_bOpenUrl = true;

      return true;

   }


   bool composer::_open_link(const string & strLink,const string & pszTarget)
   {

      return true;

   }


   // if composer on paint returns (returns true), it has painted something meaningful : no other painting is needed or even desired (finally when system, and host are ok, 
   // if host returns in a fashion-timed way the response for bitmap, it draw this bitmap, and not the default waiting [hall] screen painted by this composer).

#ifdef WINDOWS

   bool composer::windows_on_paint(HDC hdc)
   {

      if((!m_bOk || !m_bResponsive) || m_paxishost == NULL || !m_paxishost->hotplugin_host_is_initialized() || m_strEntryHallText.has_char())
      {

         if(!m_bEntryHallTextStarted)
         {

            m_bEntryHallTextStarted = true;

            m_dwEntryHallTextStart = get_tick_count();

         }

         if(get_tick_count() - m_dwEntryHallTextStart > ((1984 + 1951 + 1977) / 3))
         {

            ::rect rect = m_rect;

//            POINT pt;

            //::SetViewportOrgEx(hdc, rect.left, rect.top, &pt);

            ::hotplugin::entry_hall_windows_on_paint(hdc,m_rect,m_strEntryHallText);

         }

         return true;

      }

      m_bEntryHallTextStarted = false;

      /*else 
      {

         m_paxishost->m_bOnPaint = true;

         ::draw2d::graphics_sp g(m_paxishost->allocer());

         g->Attach((HDC)hdc);

         ::rect rect;

         m_paxishost->GetWindowRect(rect);

         m_paxishost->on_paint(g,rect);

         g->Detach();

         m_paxishost->m_bOnPaint = false;

      }*/
      
      return false;


   }

#endif

   void composer::deferred_prodevian_redraw()
   {

      if(m_paxishost != NULL && m_paxishost->hotplugin_host_is_initialized())
      {

         m_paxishost->deferred_prodevian_redraw();

      }

   }

   
   bool composer::is_active()
   {

      return true;

   }


   ::axis::system * composer::get_composer_system()
   {

      return m_pcomposersystem;

   }


   bool composer::defer_start_composer_system()
   {

      if(m_pcomposersystem != NULL)
         return true;

      try
      {

         m_pcomposersystem = new ::axis::system(NULL);

         ::axis::system * paxissystem = m_pcomposersystem;

         paxissystem->m_bMatterFromHttpCache = true;

         paxissystem->m_bSystemSynchronizedCursor = false;

         paxissystem->construct(NULL);

#ifdef WINDOWS

         paxissystem->m_hinstance = (HINSTANCE)get_hinstance();

#endif

         xxdebug_box("box1","box1",MB_ICONINFORMATION);

         paxissystem->m_bReady = false;

         ::create_thread(NULL,0,&::hotplugin::composer::composer_system_main,paxissystem,0,NULL);

      }
      catch(...)
      {

         return false;

      }

      return true;

   }


   uint32_t c_cdecl composer::composer_system_main(LPVOID lpVoid)
   {

      //int32_t iReturnCode = 0;

      ::axis::system * paxissystem = (::axis::system *) lpVoid;

      try
      {

         if(!paxissystem->pre_run())
         {

            if(paxissystem->m_iReturnCode == 0)
            {

               paxissystem->m_iReturnCode = -1;

            }

            paxissystem->m_bReady = true;

            return -1;

         }

      }
      catch(...)
      {

         if(paxissystem->m_iReturnCode == 0)
         {

            paxissystem->m_iReturnCode = -1;

         }

         paxissystem->m_bReady = true;

         return -1;

      }

      return paxissystem->main();

   }


   void composer::defer_stop_composer_system()
   {

      if(m_pcomposersystem != NULL)
      {

         m_pcomposersystem->post_quit();

         m_pcomposersystem = NULL;

      }

   }


} // namespace hotplugin


#endif


