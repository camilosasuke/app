//#include "framework.h" // from "base/user/user.h"
//#include "base/user/user.h"


namespace user
{


#ifdef METROWIN


   Agile<Windows::UI::Core::CoreWindow>(*interaction::s_get_os_window)(interaction * pui) = &interaction::get_os_window_default;


#endif


   interaction::interaction()
   {

      user_interaction_common_construct();

   }

   interaction::interaction(::aura::application * papp):
      ::object(papp),
      ::user::interaction_base(papp)
   {

      user_interaction_common_construct();

   }


   void interaction::user_interaction_common_construct()
   {
      m_iItemHeight = -1;
      m_flagNonClient.signalize(non_client_background);
      m_flagNonClient.signalize(non_client_focus_rect);

      m_bMouseHover = false;
      m_bTransparentMouseEvents = false;
      m_bRedraw = false;

      m_bRedraw = false;

      m_bCursorRedraw = false;


      m_bRedrawOnVisible = false;

      m_bNeedLayout = false;

      m_bCreated = false;

      m_bLockWindowUpdate = false;

      m_bRedrawing = false;
      m_pparent = NULL;
      m_bWorkspaceFullScreen = false;
      m_bHideOnTransparentFrame = false;
      m_pvoidUserInteraction = this;

      m_bMayProDevian = true;
      //m_pmutex                   = NULL;
      m_eappearance = AppearanceNormal;
      m_bCursorInside = false;
      m_nFlags = 0;
      m_puiOwner = NULL;
      //m_pimpl                    = NULL;
      m_ecursor = ::visual::cursor_default;
      m_iModal = 0;
      m_iModalCount = 0;
      m_bRectOk = false;
      m_bLayoutEnable = true;
      //m_psession                 = NULL;
      m_bMessageWindow = false;

      m_bVoidPaint = false;
      //m_pparent                  = NULL;
      m_bBackgroundBypass = false;
      m_bEnableSaveWindowRect = false;

      m_puserschemaSchema = NULL;
      m_bLockWindowUpdate = false;


      m_bDefaultWalkPreTranslateParentTree = false;

      m_bMoving = false;
      m_bMoveWindow = false;

      m_bSizeMove = false;

      m_ptScrollPassword1.x = 0;
      m_ptScrollPassword1.y = 0;
      m_palphasource = NULL;

   }


   interaction::~interaction()
   {

      TRACE("::user::interaction::~interaction interaction=0x%016x %s", this, typeid(*this).name());

      m_uiptraChild.m_pmutex = NULL;

   }
   tooltip *    interaction::get_tooltip()
   {

      return m_ptooltip.cast<tooltip>();

   }

   bool interaction::defer_check_layout()
   {

      if (!check_need_layout() || !m_bLayoutEnable)
      {

         if (m_bLayoutEnable)
         {

            return defer_check_translation();

         }
         else
         {

            return false;

         }

      }
      
      do_show_flags();

      layout();

      clear_need_layout();

      return true;

   }


   bool interaction::check_need_layout()
   {

      if (m_pimpl.is_null())
      {

         return false;

      }

      return m_pimpl->check_need_layout();

   }


   void interaction::clear_need_layout()
   {

      if (m_pimpl.is_null())
      {

         return;

      }

      return m_pimpl->clear_need_layout();

   }


   void interaction::set_need_layout(bool bAscendants)
   {

      m_bNeedLayout = true;

      if(bAscendants && m_pparent != NULL)
      {

         m_pparent->set_need_layout(true);

      }

   }


   bool interaction::defer_check_translation()
   {

      if (!check_need_translation() || !m_bLayoutEnable)
      {

         return defer_check_show_flags();

      }
      
      do_show_flags();
      
      translate();

      clear_need_translation();

      return true;

   }


   bool interaction::check_need_translation()
   {

      if (m_pimpl.is_null())
      {

         return false;

      }

      return m_pimpl->check_need_translation();

   }


   void interaction::clear_need_translation()
   {

      if (m_pimpl.is_null())
      {

         return;

      }

      return m_pimpl->clear_need_translation();

   }


   bool interaction::defer_check_show_flags()
   {

      if (!check_show_flags() || !m_bLayoutEnable)
      {

         return false;

      }

      do_show_flags();

      clear_show_flags();

      return true;

   }


   bool interaction::check_show_flags()
   {

      if (m_pimpl.is_null())
      {

         return false;

      }

      return m_pimpl->check_show_flags();

   }


   void interaction::clear_show_flags()
   {

      if (m_pimpl.is_null())
      {

         return;

      }

      return m_pimpl->clear_show_flags();

   }


   bool interaction::defer_check_zorder()
   {

      if (!check_need_zorder() || !m_bLayoutEnable)
      {

         return false;

      }

      zorder();

      clear_need_zorder();

      return true;

   }


   bool interaction::check_need_zorder()
   {

      if (m_pimpl.is_null())
      {

         return false;

      }

      return m_pimpl->check_need_zorder();

   }


   void interaction::clear_need_zorder()
   {

      if (m_pimpl.is_null())
      {

         return;

      }

      return m_pimpl->clear_need_zorder();

   }



   ::user::interaction * interaction::GetTopWindow() const
   {

      if(m_pimpl != NULL)
      {

         return m_pimpl->GetTopWindow();

      }

      sp(interaction) pui = get_wnd();

      if(pui.is_null())
         return NULL;

      return pui->GetTopWindow();

   }

   elemental * interaction::get_parent() const
   {

      return GetParent();

   }


   interaction * interaction::GetParent() const
   {

//#if defined(METROWIN) || defined(APPLE_IOS) || defined(ANDROID)
//
//      if(m_pparent == System.m_possystemwindow->m_pui)
//      {
//
//         return NULL;
//
//      }
//
//#endif

      return m_pparent;

   }


   oswindow interaction::GetParentHandle() const
   {

      interaction * puiParent = GetParent();

      if(puiParent == NULL)
         return NULL;

      return puiParent->get_safe_handle();

   }


   sp(place_holder) interaction::place(::user::interaction * pui)
   {

      UNREFERENCED_PARAMETER(pui);

      return NULL;

   }



   bool interaction::on_before_set_parent(::user::interaction * puiParent)
   {

      return true;

   }



   ::user::interaction * interaction::SetParent(::user::interaction * puiParent)
   {

      if(puiParent != NULL && (puiParent == this || puiParent == GetParent() || IsDescendant(puiParent)))
      {

         return GetParent();

      }

      if(!on_before_set_parent(puiParent))
      {

         return GetParent();

      }

      sp(::user::interaction_impl_base) pimplOld = m_pimpl;

      sp(interaction) pparentOld = GetParent();

      if(pparentOld != NULL)
      {

         if(puiParent == NULL)
         {

            {

            restart:

               sp(::user::interaction) pui;

               while (get_impl()->m_guieptraMouseHover.get_child(pui))
               {

                  if (IsDescendant(pui) || pui == this)
                  {

                     try
                     {

                        pui->send_message(WM_MOUSELEAVE);

                     }
                     catch (...)
                     {

                     }

                     get_impl()->m_guieptraMouseHover.remove(pui);

                     goto restart;

                  }

               }

            }

            sp(::user::interaction_impl) pimplNew = Application.alloc(System.type_info < ::user::interaction_impl >());

            string strName;

            get_window_text(strName);

            int32_t iStyle = get_window_long(GWL_STYLE);

            iStyle &= ~WS_CHILD;

            rect rectWindow;

            GetWindowRect(rectWindow);

            if(!pimplNew->create_window_ex(this, 0,NULL,strName,iStyle,rectWindow,NULL,GetDlgCtrlId()))
            {

               pimplNew.release();

               pimplNew = NULL;

               m_pimpl = pimplOld;

            }
            else
            {

               {

                  single_lock sl(m_pmutex);

                  pimplNew->m_pui = this;

                  m_pimpl = pimplNew;

                  pimplNew->install_message_handling(pimplNew);

                  on_set_parent(puiParent);

               }

               if(pimplOld != NULL)
               {

                  try
                  {

                     pimplOld->filter_target(pimplOld);

                     pimplOld->m_pui = NULL;

                     pimplOld->DestroyWindow();

                     pimplOld.release();

                  }
                  catch(...)
                  {

                  }

               }



            }

         }
         else
         {

            on_set_parent(puiParent);

         }

      }
      else
      {

         if(puiParent != NULL)
         {

            ::multithreading::post_quit(m_pimpl.cast < ::user::interaction_impl>()->m_pthreadUpdateWindow);

            sp(::user::interaction_child) pimplNew = canew(::user::interaction_child(get_app()));

            pimplNew->m_pui = this;

            System.remove_frame(this); // no more a top level frame if it were one

            m_pimpl = pimplNew;

            string strName;

            int32_t iStyle = get_window_long(GWL_STYLE);

            iStyle |= WS_CHILD;

            rect rectWindow;

            GetWindowRect(rectWindow);

            puiParent->ScreenToClient(rectWindow);

            if(!pimplNew->create_window(this, NULL,strName,iStyle,rectWindow,puiParent,GetDlgCtrlId()))
            {

               m_pimpl = pimplOld;

               System.add_frame(this);

               pimplNew.release();

            }
            else
            {

               if(pimplOld != NULL)
               {

                  ::user::interaction_impl * pimpl = pimplOld.cast < ::user::interaction_impl > ();

                  if (pimpl != NULL)
                  {

                     try
                     {

                        oswindow oswindow = oswindow_remove(pimpl);

                        pimpl->m_pui = NULL;

                        pimplOld.release();

                        if (oswindow != NULL)
                        {

                           ::DestroyWindow(oswindow);

                        }

                     }
                     catch (...)
                     {

                     }

                  }
                  else
                  {

                     pimplOld.release();

                  }

               }

               on_set_parent(puiParent);

            }

         }
         else
         {

         }

      }

      return pparentOld;

   }


   void interaction::set_timer(smart_pointer_array < ::aura::timer_item > timera)
   {

      for(int32_t i = 0; i < timera.get_count(); i++)
      {

         SetTimer(timera[i]->m_uiId,timera[i]->m_uiElapse,NULL);

      }

   }


   bool interaction::GetClientRect(LPRECT lprect)
   {

      RECT64 rect;

      if(!GetClientRect(&rect))
      {

         return false;

      }

      lprect->left = (LONG)rect.left;
      lprect->top = (LONG)rect.top;
      lprect->right = (LONG)rect.right;
      lprect->bottom = (LONG)rect.bottom;

      return true;

   }


   bool interaction::GetWindowRect(LPRECT lprect)
   {

      rect64 rectWindow;

      if(!GetWindowRect(rectWindow))
      {

         return false;

      }

      lprect->left = (LONG)rectWindow.left;
      lprect->top = (LONG)rectWindow.top;
      lprect->right = (LONG)rectWindow.right;
      lprect->bottom = (LONG)rectWindow.bottom;

      return true;

   }


   bool interaction::GetWindowRect(RECT64 * lprect)
   {

      if(m_pimpl == NULL)
      {

         return false;

      }

      if(!m_pimpl->GetWindowRect(lprect))
      {

         return false;
      }

      return true;

   }


   bool interaction::SetPlacement(const RECT & lpcrect,UINT nFlags)
   {

      return RepositionWindow(lpcrect.left,lpcrect.top,width(lpcrect),height(lpcrect),nFlags);

   }

   bool interaction::RepositionWindow(const RECT & lpcrect,UINT nFlags)
   {

      return SetPlacement(lpcrect,nFlags);

   }

   bool interaction::RepositionWindow(int32_t x,int32_t y,int32_t cx,int32_t cy,UINT nFlags)
   {

      return SetWindowPos(0,x,y,cx,cy,nFlags);

   }

   bool interaction::MoveWindow(int32_t x,int32_t y,UINT nFlags)
   {

      return SetWindowPos(0,x,y,0,0,nFlags | SWP_NOSIZE | SWP_NOZORDER);

   }

   bool interaction::MoveWindow(POINT pt,UINT nFlags)
   {

      return MoveWindow(pt.x,pt.y,nFlags);

   }

   bool interaction::SizeWindow(int32_t cx,int32_t cy,UINT nFlags)
   {

      return SetWindowPos(0,0,0,cx,cy,nFlags | SWP_NOMOVE | SWP_NOZORDER);

   }

   bool interaction::SizeWindow(SIZE sz,UINT nFlags)
   {

      return SizeWindow(sz.cx,sz.cy,nFlags);

   }

   bool interaction::ResizeWindow(int32_t cx,int32_t cy,UINT nFlags)
   {

      return SetWindowPos(0,0,0,cx,cy,nFlags | SWP_NOMOVE | SWP_NOZORDER);

   }

   bool interaction::ResizeWindow(SIZE sz,UINT nFlags)
   {

      return ResizeWindow(sz.cx,sz.cy,nFlags);

   }

   bool interaction::SetWindowPos(int_ptr z,const RECT & rect,UINT nFlags)
   {

      return SetWindowPos(z,rect.left,rect.top,width(rect),height(rect),nFlags);

   }

   bool interaction::defer_set_window_pos(int32_t z,int32_t x,int32_t y,int32_t cx,int32_t cy,UINT nFlags) // only set_windows_pos if GetParent()->ScreenToClient(get_window_rect) different of rect(x, y, cx, cy)
   {

      rect rectWindow;

      GetWindowRect(rectWindow);

      if(GetParent() != NULL)
      {

         GetParent()->ScreenToClient(rectWindow);

      }

      if(!(nFlags & SWP_NOMOVE))
      {

         if(rectWindow.left == x && rectWindow.top == y)
         {
            nFlags |= SWP_NOMOVE;

         }

      }

      if(!(nFlags & SWP_NOSIZE))
      {

         if(rectWindow.width() == cx && rectWindow.height() == cy)
         {

            nFlags |= SWP_NOSIZE;

         }

      }

      bool bOk = SetWindowPos(z,x,y,cx,cy,nFlags);


      return bOk;

   }




   void interaction::install_message_handling(::message::dispatch * pinterface)
   {

      IGUI_WIN_MSG_LINK(WM_CREATE,pinterface,this,&interaction::_001OnCreate);

      if(m_bMessageWindow)
      {

         //IGUI_WIN_MSG_LINK(WM_DESTROY              , pinterface, this, &interaction::_001OnDestroyMessageWindow);

      }
      else
      {

         IGUI_WIN_MSG_LINK(WM_CLOSE,pinterface,this,&interaction::_001OnClose);
         IGUI_WIN_MSG_LINK(WM_DESTROY,pinterface,this,&interaction::_001OnDestroy);
         IGUI_WIN_MSG_LINK(WM_SIZE,pinterface,this,&interaction::_001OnSize);
         IGUI_WIN_MSG_LINK(WM_MOVE,pinterface,this,&interaction::_001OnMove);
         IGUI_WIN_MSG_LINK(WM_USER + 184,pinterface,this,&interaction::_001OnUser184);
         IGUI_WIN_MSG_LINK(WM_NCCALCSIZE,pinterface,this,&interaction::_001OnNcCalcSize);
         IGUI_WIN_MSG_LINK(WM_SHOWWINDOW,pinterface,this,&interaction::_001OnShowWindow);
         IGUI_WIN_MSG_LINK(WM_LBUTTONDOWN,pinterface,this,&interaction::_001OnLButtonDown);

      }

      IGUI_WIN_MSG_LINK(WM_COMMAND,pinterface,this,&interaction::_001OnCommand);
      IGUI_WIN_MSG_LINK(message_simple_command,pinterface,this,&interaction::_001OnSimpleCommand);
      //      IGUI_WIN_MSG_LINK(message_set_schema,pinterface,this,&interaction::_001OnSetSchema);


   }

   void interaction::_001OnNcCalcSize(signal_details * pobj)
   {
      pobj->m_bRet = true; // avoid any Microsoft-Window-concept-of-non-client-area
   }

   void interaction::_001OnDestroy(signal_details * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

      user_interaction_on_destroy();

      pobj->previous();


   }

   void interaction::user_interaction_on_hide()
   {


      //if(System.get_active_ui() == this)
      //{
      //   // TODO : to be possibly done by camilo : instead of setting active guie to null
      //   // you could relinquish (I don't know what it is it) the active window status
      //   // to another window by using a function like kill_focus or relinquish_focus.
      //   // But, if you set active guie to NULL, Session could ignore the NULL assignment,
      //   // and use as a tip to set to a proper alive windows/guie.
      //   System.set_active_guie(NULL);
      //}

      //if(Session.get_focus_ui() == this)
      //{

      //   System.set_focus_guie(NULL);

      //}


      //if(Session.get_keyboard_focus() == this)
      //{

      //   Session.set_keyboard_focus(NULL);

      //}

      try
      {

         for (index i = 0; i < m_uiptraChild.get_size(); i++)
         {

            try
            {

               m_uiptraChild[i]->user_interaction_on_hide();

            }
            catch (...)
            {

            }

         }

      }
      catch (...)
      {

      }

      try
      {

         if (m_pauraapp != NULL && m_pauraapp->m_pbasesession != NULL)
         {

            ::user::interaction * puiCapture = m_pauraapp->m_pbasesession->GetCapture();

            if (puiCapture != NULL && puiCapture == this)
            {

               ReleaseCapture();

            }

         }

      }
      catch (...)
      {

      }


      try
      {

         mouse_hover_remove(this);

      }
      catch(...)
      {

      }

      try
      {

         if(m_pauraapp!= NULL && m_pauraapp->m_pbasesession!= NULL && m_pauraapp->m_pbasesession->m_pkeyboardfocus == this)
         {

            if(GetParent() == NULL || GetParent()->m_bDestroying || !GetParent()->IsWindowVisible())
            {

               m_pauraapp->m_pbasesession->m_pkeyboardfocus = NULL;

            }
            else
            {

               ::user::elemental * pnext = NULL;

               try
               {

                  pnext = keyboard_get_next_focusable();

               }
               catch(...)
               {

               }

               if(pnext != NULL && pnext != this)
               {

                  try
                  {

                     pnext->keyboard_set_focus();

                  }
                  catch(...)
                  {

                  }

               }

               if(Session.m_pkeyboardfocus == this)
               {

                  Session.m_pkeyboardfocus = NULL;

               }

            }

         }

      }
      catch(...)
      {

      }

      if(GetCapture() == this)
      {

         SetCapture();

      }


      if(m_pauraapp != NULL && &System != NULL && System.get_active_ui() == this)
      {

         #if defined(METROWIN)
         ::WinSetActiveWindow(NULL);
         #else
         ::SetActiveWindow(NULL);
         #endif

      }



   }

   void interaction::_001GetXScrollInfo(scroll_info & info)
   {

      size sizeTotal = get_total_size();
      size sizePage = get_page_size();
      point ptOffset = get_viewport_offset();

      info.nMin = 0;
      info.nMax = sizeTotal.cx;
      info.nPage = sizePage.cx;
      info.nPos = ptOffset.x;
      info.nTrackPos = ptOffset.x;
   }

   void interaction::_001GetYScrollInfo(scroll_info & info)
   {
      size sizeTotal = get_total_size();
      size sizePage = get_page_size();
      point ptOffset = get_viewport_offset();

      info.nMin = 0;
      info.nMax = sizeTotal.cy;
      info.nPage = sizePage.cy;
      info.nPos = ptOffset.y;
      info.nTrackPos = ptOffset.y;
   }


   void interaction::layout_scroll_bar()
   {

   }


   void interaction::user_interaction_on_destroy()
   {

      single_lock sl(get_wnd() == NULL || get_wnd()->m_pimpl.is_null()
                     || get_wnd()->m_pimpl.cast < ::user::interaction_impl >()==NULL ? NULL : get_wnd()->m_pimpl.cast < ::user::interaction_impl >()->draw_mutex(), true);

      //single_lock sl(m_pmutex,true);

      m_bCreated = false;

      user_interaction_on_hide();

      try
      {

         if(m_pimpl != NULL)
         {

            m_pimpl->delete_all_timers();

         }

      }
      catch(...)
      {

      }


      try
      {

         ShowWindow(SW_HIDE);

      }
      catch(...)
      {

      }

      ref_array <  ::user::interaction  > uiptra;

      {

         if (GetParent() != NULL)
         {

            try
            {

               single_lock sl(GetParent()->m_pmutex, true);

               GetParent()->m_uiptraChild.remove(this);

            }
            catch (...)
            {

            }

         }

      }

      {

         synch_lock sl(m_pmutex);

         m_uiptraChild.slice(uiptra);

      }


      for (int32_t i = 0; i < uiptra.get_count(); i++)
      {

         ::user::interaction * pui = uiptra[i];

         try
         {

            pui->DestroyWindow();

         }
         catch (...)
         {

         }

      }



      if (m_pauraapp == NULL)
      {

         return;

      }

      if(GetParent() == NULL && !is_message_only_window())
      {

         try
         {

            Application.remove_frame(this); // guess this may be a frame, it doesn't hurt to remove if this is not there

         }
         catch(...)
         {

         }

         try
         {

            Session.remove_frame(this); // guess this may be a frame, it doesn't hurt to remove if this is not there

         }
         catch(...)
         {

         }

         try
         {

            System.remove_frame(this); // guess this may be a frame, it doesn't hurt to remove if this is not there

         }
         catch(...)
         {
         }

      }

      //try
      //{
      //
      //    m_pauraapp->remove(this);

      //}
      //catch(...)
      //{

      //}


      //try
      //{

      //   while(m_threadptra.has_elements())
      //   {
      //      ::thread * pthread = m_threadptra[0];
      //      if (pthread == NULL)
      //      {
      //         m_threadptra.remove_at(0);
      //      }
      //      else
      //      {
      //         {
      //            synch_lock sl(&pthread->m_mutexUiPtra);
      //            if (pthread->m_spuiptra.is_set())
      //            {
      //               pthread->m_spuiptra->remove(this);
      //            }
      //         }
      //         m_threadptra.remove(pthread);
      //      }

      //   }

      //}
      //catch(...)
      //{

      //}


   }


   void interaction::_001OnSize(signal_details * pobj)
   {

      SCAST_PTR(::message::size,psize,pobj);

      pobj->previous();

      //      keep<bool> lockWindowUpdate(&get_wnd()->m_bLockWindowUpdate, true, get_wnd()->m_bLockWindowUpdate, true);

      if(psize->m_nType == SIZE_MINIMIZED)
      {

         TRACE("::user::interaction::_001OnSize SIZE_MINIMIZED - ignoring event");

      }
      else if(m_pimpl->m_bIgnoreSizeEvent)
      {

         TRACE("::user::interaction::_001OnSize instructed to m_bIgnoreSizeEvent");

      }
      else
      {

         //layout();

      }

   }

   void interaction::layout()
   {

      m_pimpl->on_layout();

      single_lock sl(m_pmutex, true);

      on_layout();

   }


   void interaction::translate()
   {

      m_pimpl->on_translate();

      single_lock sl(m_pmutex, true);

      on_translate();

   }


   void interaction::do_show_flags()
   {

      m_pimpl->on_do_show_flags();

      single_lock sl(m_pmutex, true);

      on_do_show_flags();

   }


   void interaction::zorder()
   {

      if (GetParent() == NULL)
      {

         m_pimpl->on_zorder();

         return;

      }

      single_lock sl(GetParent()->m_pmutex);

      int_ptr z = m_pimpl->m_iZ;

      if (z == ZORDER_TOP || z == ZORDER_TOPMOST)
      {

         if (GetParent()->m_uiptraChild.find_first(this) != GetParent()->m_uiptraChild.get_upper_bound())
         {

            try
            {

               GetParent()->m_uiptraChild.remove(this);

               GetParent()->m_uiptraChild.add_unique(this);

            }
            catch (...)
            {

            }

         }

      }
      else if (z == ZORDER_BOTTOM)
      {

         if (GetParent()->m_uiptraChild.find_first(this) != 0)
         {

            try
            {

               GetParent()->m_uiptraChild.remove(this);

               GetParent()->m_uiptraChild.insert_at(0, this);

            }
            catch (...)
            {

            }

         }

      }

   }


   void interaction::_001OnMove(signal_details * pobj)
   {

      pobj->previous();

//      layout_tooltip();



      //synch_lock sl(m_pmutex);

      //for(index i = 0; i < m_uiptraChild.get_size(); i++)
      //{
      //
      //   sp(::user::interaction) pui = m_uiptraChild[i];
      //
      //   sl.unlock();

      //   pui->send_message(WM_MOVE);
      //
      //   sl.lock();

      //}

   }


   void interaction::set_viewport_org(::draw2d::graphics * pgraphics)
   {

      if(m_pimpl == NULL)
         return;

      m_pimpl->set_viewport_org(pgraphics);

      /*      rect64 rectWindow;
      GetWindowRect(rectWindow);
      get_wnd()->ScreenToClient(rectWindow);
      pgraphics->SetViewportOrg(point(rectWindow.top_left()));
      pgraphics->SelectClipRgn(NULL);
      */

   }


   void interaction::_001OnClip(::draw2d::graphics * pgraphics)
   {

      //return;



      try
      {

         rect rectClip;

         ::aura::draw_context * pdrawcontext = pgraphics->::core::simple_chain < ::aura::draw_context >::get_last();

         rect rectClient;

         bool bFirst = true;

         if(pdrawcontext != NULL)
         {

            rectClient     = pdrawcontext->m_rectWindow;

            ScreenToClient(rectClient);

            rectClient.bottom++;
            rectClient.right++;

            rectClip       = rectClient;

            bFirst         = false;

         }

         ::user::interaction * pui = this;

         ::rect rectFocus;

         ::rect rectIntersect;

         index i = 0;

         while(pui != NULL)
         {

            pui->GetWindowRect(rectClient);

            pui->GetClientRect(rectFocus);

            rectFocus.offset(rectClient.top_left());

            ScreenToClient(rectFocus);

            rectFocus.bottom++;
            rectFocus.right++;

            if (i == 0)
            {

               rectIntersect = rectFocus;

            }
            else
            {

               rectIntersect.intersect(rectFocus);

            }

            i++;

            pui = pui->GetParent();

         }

         pgraphics->IntersectClipRect(rectIntersect);

      }
      catch(...)
      {

         throw simple_exception(::get_thread_app(),"no more a window");

      }

   }


   void interaction::_001DrawThis(::draw2d::graphics * pgraphics)
   {

      if (pgraphics == NULL)
      {

         throw invalid_argument_exception(get_app());

      }

      ::draw2d::keep k(pgraphics);

      try
      {

         if(!is_custom_draw() && pgraphics != NULL && pgraphics->m_pnext == NULL)
         {

            set_viewport_org(pgraphics);

         }

         pgraphics->m_dFontFactor = 1.0;

         //try
         //{

         //   pgraphics->SelectClipRgn(NULL);

         //}
         //catch(...)
         //{

         //   throw simple_exception(::get_thread_app(), "no more a window");

         //}

         {

            synch_lock sl(m_pmutex);

            _001OnNcDraw(pgraphics);

         }

         _001OnClip(pgraphics);

         _001CallOnDraw(pgraphics);


      }
      catch(...)
      {

         TRACE("Exception: interaction::_001DrawThis %s", typeid(*this).name());

      }

      if (m_pparent != NULL)
      {

         on_after_graphical_update();

      }

   }

   void interaction::_001CallOnDraw(::draw2d::graphics * pgraphics)
   {

      on_viewport_offset(pgraphics);

      synch_lock sl(m_pmutex);

      _001OnDraw(pgraphics);

   }


   void interaction::_008CallOnDraw(::draw2d::graphics * pgraphics)
   {

      ::draw2d::keep k(pgraphics);

      try
      {

         set_viewport_org(pgraphics);

         synch_lock sl(m_pmutex);

         _008OnDraw(pgraphics);

      }
      catch (...)
      {

      }

   }

   void interaction::on_viewport_offset(::draw2d::graphics * pgraphics)
   {



      point ptViewportOffset = get_viewport_offset();

      pgraphics->OffsetViewportOrg(-ptViewportOffset.x,-ptViewportOffset.y);

   }


   void interaction::_001DrawChildren(::draw2d::graphics * pgraphics)
   {

      ::draw2d::keep k(pgraphics);

      // while drawing layout can occur and change z-order.
      // keep this past z-order
      interaction_spa uia = m_uiptraChild;

      for(auto & pui : uia)
      {

         try
         {

            if (!pui->is_custom_draw())
            {

               pui->_000OnDraw(pgraphics);

            }

         }
         catch (...)
         {

            TRACE("\n\nException thrown while drawing user::interaction\n\n");

         }

      }



   }


   void interaction::_001Print(::draw2d::graphics * pgraphics)
   {

      {

         ::draw2d::keep k(pgraphics);

         try
         {

            _001OnDeferPaintLayeredWindowBackground(pgraphics);

         }
         catch (...)
         {

         }

      }

      {

         ::draw2d::keep k(pgraphics);

         try
         {

            _000OnDraw(pgraphics);

         }
         catch (...)
         {

         }

      }

      if(&Session != NULL && Session.m_bDrawCursor)
      {

         {

            ::draw2d::keep k(pgraphics);

            try
            {

               point ptCursor;

               Session.get_cursor_pos(&ptCursor);

               ScreenToClient(&ptCursor);

               ::visual::cursor * pcursor = Session.get_cursor();

               if (pcursor != NULL && pgraphics != NULL)
               {

                  pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

                  pcursor->to(pgraphics, ptCursor);

               }

            }
            catch (...)
            {

            }

         }

      }

   }
   
   
   bool interaction::is_this_visible()
   {

      return (::user::interaction_base::is_this_visible() || 
         (m_pimpl.is_set() && (m_pimpl->m_bShowFlags && (m_pimpl->m_iShowFlags & SWP_SHOWWINDOW)))
         && !(m_pimpl->m_bShowFlags && (m_pimpl->m_iShowFlags & SWP_HIDEWINDOW)));

   }


   void interaction::_000OnDraw(::draw2d::graphics * pgraphics)
   {

      defer_check_layout();

      defer_check_zorder();

      if(!is_this_visible())
         return;

      try
      {

         _001DrawThis(pgraphics);

      }
      catch (...)
      {

         TRACE("Exception: interaction::_000OnDraw _001DrawThis %s", typeid(*this).name());

      }


      if (m_uiptraChild.has_elements())
      {

         try
         {

            _001DrawChildren(pgraphics);

         }
         catch (...)
         {

            TRACE("Exception: interaction::_000OnDraw _001DrawChildren %s", typeid(*this).name());

         }

      }

      try
      {

         _008CallOnDraw(pgraphics);

      }
      catch (...)
      {

         TRACE("Exception: interaction::_000OnDraw _008CallOnDraw %s", typeid(*this).name());

      }

   }


   void interaction::_001OnNcDraw(::draw2d::graphics * pgraphics)
   {

      if (m_flagNonClient.is_signalized(non_client_background))
      {

         draw_control_background(pgraphics);

      }

   }


   void interaction::_001OnDraw(::draw2d::graphics * pgraphics)
   {


   }

   void interaction::_008OnDraw(::draw2d::graphics * pgraphics)
   {

   }


   void interaction::draw_control_background(::draw2d::graphics * pgraphics)
   {



      ::aura::draw_context * pdrawcontext = pgraphics->::core::simple_chain < ::aura::draw_context >::get_last();

      rect rectClient;

      if(pdrawcontext != NULL)
      {

         rectClient     = pdrawcontext->m_rectClient;

      }
      else
      {

         GetClientRect(rectClient);

      }

      if(_001IsBackgroundBypass())
      {
      }
      else if(_001IsTranslucent())
      {


         pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

         COLORREF cr = ARGB(184,255,255,255);

         get_color(cr, ::user::color_background);

         if (cr != 0)

         {

            pgraphics->FillSolidRect(rectClient, cr);


         }

      }
      else
      {

         pgraphics->set_alpha_mode(::draw2d::alpha_mode_set);

         COLORREF cr = ARGB(255,255,255,255);

         get_color(cr,::user::color_background);

         cr |= (255 << 24);

         pgraphics->FillSolidRect(rectClient,cr);

      }

   }

   bool interaction::is_custom_draw()
   {

      return false;

   }


   bool interaction::set_may_pro_devian(bool bSet)
   {

      m_bMayProDevian = bSet;

      return true;

   }


   void interaction::on_set_may_pro_devian()
   {

      if(!m_bMayProDevian && GetParent() == NULL)
      {

         SetTimer(1984 + 77 + 3,250,NULL);

      }
      else
      {

         KillTimer(1984 + 77 + 3);
      }

   }


   void interaction::_001OnCreate(signal_details * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

      m_bCreated = true;

      if(m_pauraapp == NULL)
         throw simple_exception(get_app(), "m_pauraapp cannot be null");

      on_set_may_pro_devian();

      {

         //m_pauraapp->add(this);


         if((GetParent() == NULL
#if defined(METROWIN) || defined(VSNORD)
               || GetParent() == System.m_possystemwindow->m_pui
#endif
            )
               && !is_message_only_window()
#if !defined(LINUX) && !defined(METROWIN) && !defined(APPLEOS) && !defined(VSNORD)
            && (::user::interaction *) System.m_psystemwindow != this
#endif
            )
         {

//            synch_lock slUser(m_pmutex);

            synch_lock sl(m_pmutex);

            if(m_pauraapp->m_pbasesystem != NULL)
            {

               System.add_frame(this);

            }

            if(m_pauraapp->m_pbasesession != NULL)
            {

               Session.add_frame(this);

            }

            if(m_pauraapp != NULL)
            {

               Application.add_frame(this);

            }

         }

      }

      defer_create_mutex();

      m_uiptraChild.m_pmutex = m_pmutex;

      try
      {
         if ((GetParent() != NULL
#if defined(METROWIN) || defined(VSNORD)
            && GetParent() != System.m_possystemwindow->m_pui
#endif
            )
            && !is_message_only_window()
#if !defined(LINUX) && !defined(METROWIN) && !defined(APPLEOS) && !defined(VSNORD)
            && (::user::interaction *) System.m_psystemwindow != this
#endif
            )
         {

            sp(place_holder) pholder = GetParent();

            if(pholder.is_set())
            {

               // A Copy Paste error (the commented out code below)?
               //single_lock sl(puiParent->m_pmutex,TRUE);
               //single_lock sl2(m_pmutex,TRUE);

               if(!pholder->is_holding(this))
               {

                  pholder->hold(this);

               }

            }

         }

      }
      catch(...)
      {

      }



   }


   void interaction::_000OnDrag(::message::drag_and_drop * pdrag)
   {
#ifdef WINDOWS
      if(pdrag->m_uiMessage != MESSAGE_OLE_DRAGLEAVE)
      {

         try
         {
            if(!IsWindowVisible())
               return;
            if(!_001IsPointInside(point(pdrag->pt.x, pdrag->pt.y)))
               return;
         }
         catch(...)
         {
            return;
         }

      }
      // these try catchs are needed for multi threading : multi threaded windows: the hell
      // Now I understand why Microsoft (TM) Windows (R) windows are single threaded.
      sp(::user::interaction) pui = top_child();
      //      int32_t iSize;
      try
      {
         while(pui != NULL)
         {
            try
            {
               if(pui->IsWindowVisible() && (pdrag->m_uiMessage == MESSAGE_OLE_DRAGLEAVE || pui->_001IsPointInside(point(pdrag->pt.x,pdrag->pt.y))))
               {
                  try
                  {
                     pui->_000OnDrag(pdrag);
                     if(pdrag->m_bRet)
                        return;
                  }
                  catch(...)
                  {
                  }
               }
               pui = pui->under_sibling();
            }
            catch(...)
            {
            }
         }
      }
      catch(...)
      {
      }
      try
      {
         if(m_pimpl == NULL)
            return;
         (m_pimpl->*m_pimpl->m_pfnDispatchWindowProc)(dynamic_cast <signal_details *> (pdrag));
         if(pdrag->m_bRet)
            return;
      }
      catch(...)
      {
      }
#endif
   }


   void interaction::_000OnMouse(::message::mouse * pmouse)
   {

      try
      {

         if(!is_this_visible()) // assume parent visibility already checked
         {

            return;

         }

         if(!_001IsPointInside(pmouse->m_pt)
               && !(Session.GetCapture() == this ||
                    is_descendant(Session.GetCapture())))
         {

            return;

         }

      }
      catch(...)
      {

         return;

      }
      // these try catchs are needed for multi threading : multi threaded windows: the hell
      // Now I understand why Microsoft (TM) Windows (R) windows are single threaded.
      sp(::user::interaction) pui;
      //      int32_t iSize;
      try
      {
         while(rget_child(pui))
         {
            try
            {
               if (pui.is_null())
               {
                  continue;
               }
               if(pui->IsWindowVisible() && pui->_001IsPointInside(pmouse->m_pt))
               {
                  try
                  {
                     pui->_000OnMouse(pmouse);
                     if(pmouse->m_bRet)
                        return;
                  }
                  catch(...)
                  {
                  }
               }
            }
            catch(...)
            {
            }
         }
      }
      catch(...)
      {
      }
      try
      {
         if(m_pimpl == NULL)
            return;
         (m_pimpl->*m_pimpl->m_pfnDispatchWindowProc)(dynamic_cast <signal_details *> (pmouse));
         if(pmouse->get_lresult() != 0)
            return;
      }
      catch(...)
      {
      }
   }

   void interaction::_000OnKey(::message::key * pkey)
   {
      point ptCursor;
      Session.get_cursor_pos(&ptCursor);
      if(!pkey->m_bRet)
      {
         // these try catchs are needed for multi threading : multi threaded windows: the hell
         // Now I understand why Microsoft (TM) Windows (R) windows are single threaded.
         ::user::interaction * pui = top_child();
         //         int32_t iSize;
         try
         {
            while(pui != NULL)
            {
               try
               {
                  if(pui->IsWindowVisible())
                  {
                     try
                     {
                        pui->_000OnKey(pkey);
                        if(pkey->m_bRet)
                           return;
                     }
                     catch(...)
                     {
                     }
                  }
                  pui = pui->under_sibling();
               }
               catch(...)
               {
               }
            }
         }
         catch(...)
         {
         }
         try
         {
            (m_pimpl->*m_pimpl->m_pfnDispatchWindowProc)(dynamic_cast <signal_details *> (pkey));
            if(pkey->get_lresult() != 0)
               return;
         }
         catch(...)
         {
         }
      }
   }

   void interaction::_001OnMouseEnter(signal_details * pobj)
   {
      /*
      for(int32_t i = 0; i < m_uiptra.get_size(); i++)
      {
      if(m_uiptra[i]->_001IsVisible()
      && m_uiptra[i]->_001IsPointInside(point)
      && !m_uiptra[i]->m_bCursorInside)
      {
      m_uiptra[i]->m_bCursorInside = true;
      m_uiptra[i]->_001OnMouseEnter(wparam, lparam, lresult);
      }
      }
      */
      pobj->m_bRet = false;
   }


   void interaction::_001OnMouseLeave(signal_details * pobj)
   {

      sp(interaction) pui = top_child();
      while(pui != NULL)
      {
         if(pui->m_bCursorInside)
         {
            pui->m_bCursorInside = false;
            pui->_001OnMouseLeave(pobj);
            pui->_002OnMouseLeave(pobj);
         }
         pui = pui->under_sibling();
      }
      pobj->m_bRet = false;
   }


   ::user::interaction * interaction::_001FromPoint(point64 pt,bool bTestedIfParentVisible)
   {

      if(bTestedIfParentVisible)
      {

         if(!is_this_visible() || !_001IsPointInside(pt)) // inline version - do not need pointer to the function
         {

            return NULL;

         }

      }
      else
      {

         if(!IsWindowVisible() || !_001IsPointInside(pt)) // inline version - do not need pointer to the function
         {

            return NULL;

         }

      }

      sp(interaction) pui = top_child();

      while(pui != NULL)
      {

         sp(interaction) puie = pui->_001FromPoint(pt,true);

         if(puie != NULL)
         {

            return puie;

         }

         pui = pui->under_sibling();

      }

      return this;

   }


   bool interaction::_001IsPointInside(point64 pt)
   {

      if (m_pimpl.is_null())
      {

         return false;

      }

      if (m_pimpl != get_wnd())
      {
       
         if (get_wnd()->WfiIsIconic() || !IsWindowVisible())
         {

            return false;

         }

      }

      rect64 rect;

      GetWindowRect(rect);

      return rect.contains(pt);

   }


   void interaction::_001OnKeyDown(signal_details * pobj)
   {
      //if(Session.get_keyboard_focus() != this
      //   && Session.get_keyboard_focus() != NULL)
      //{
      // Session.get_keyboard_focus()->keyboard_focus_OnKeyDown(pobj);
      //}
   }

   void interaction::_001OnKeyUp(signal_details * pobj)
   {
      //if(Session.get_keyboard_focus() != this
      ///&& Session.get_keyboard_focus() != NULL)
      //{
      // Session.get_keyboard_focus()->keyboard_focus_OnKeyUp(pobj);
      //}
   }

   void interaction::_001OnChar(signal_details * pobj)
   {
      //if(Session.get_keyboard_focus() != this
      // && Session.get_keyboard_focus() != NULL)
      //{
      // Session.get_keyboard_focus()->keyboard_focus_OnChar(pobj);
      //}
   }

   void interaction::_001OnTimer(::timer * ptimer)
   {

      if(ptimer->m_nIDEvent == 1984 + 77 + 3)
      {

         if(!m_bMayProDevian && GetParent() == NULL)
         {

            //TRACE("Redraw !m_bMayProDevian");

            RedrawWindow();

         }

      }

   }


   ::user::interaction * interaction::get_child_by_name(const char * pszName,int32_t iLevel)
   {
      sp(interaction) pui = top_child();
      while(pui != NULL)
      {
         if(pui->m_strName == pszName)
         {
            return pui;
         }
         pui = pui->under_sibling();
      }
      sp(interaction) pchild;
      if(iLevel > 0 || iLevel == -1)
      {
         if(iLevel > 0)
         {
            iLevel--;
         }
         sp(interaction) pui = top_child();
         while(pui != NULL)
         {
            pchild = pui->get_child_by_name(pszName,iLevel);
            if(pchild != NULL)
               return pchild;
            pui = pui->under_sibling();
         }
      }
      return NULL;
   }


   ::user::interaction * interaction::get_child_by_id(id id,int32_t iLevel)
   {
      interaction * pui = top_child();
      while(pui != NULL)
      {
         if(pui->m_id == id)
         {
            return pui;
         }
         pui = pui->under_sibling();
      }
      interaction * pchild;
      if(iLevel > 0 || iLevel == -1)
      {
         if(iLevel > 0)
         {
            iLevel--;
         }
         interaction * pui = top_child();
         while(pui != NULL)
         {
            pchild = pui->get_child_by_id(id,iLevel);
            if(pchild != NULL)
               return pchild;
            pui = pui->under_sibling();
         }
      }
      return NULL;
   }



   void interaction::_002OnLButtonDown(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void interaction::_002OnLButtonUp(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void interaction::_002OnMouseMove(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void interaction::_002OnMouseEnter(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void interaction::_002OnMouseLeave(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void interaction::_002OnKeyDown(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void interaction::_002OnKeyUp(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void interaction::_002OnTimer(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }


   // pbase object should be allocated with new in
   // base or derived object and will be delete after
   // handling
   LRESULT interaction::send(::message::base * pbase)
   {

      message_handler(pbase);

      return pbase->get_lresult();

   }


   // pbase object should be allocated with new in
   // base or derived object and will be delete after
   // handling
   bool interaction::post(::message::base * pbase)
   {

      return post_message(MESSAGE_POST_MESSAGE,1,pbase );

   }


   LRESULT interaction::send_message(UINT uiMessage,WPARAM wparam,lparam lparam)
   {

      if(m_pimpl == NULL)
      {

         return 0;

      }

      return m_pimpl->send_message(uiMessage,wparam,lparam);

   }


   LRESULT interaction::message_call(UINT uiMessage, WPARAM wparam, lparam lparam)
   {

      if (m_pimpl == NULL)
      {

         return 0;

      }

      return m_pimpl->message_call(uiMessage, wparam, lparam);

   }


#ifdef LINUX

   LRESULT interaction::send_message(XEvent * pevent)
   {
      if (m_pimpl == NULL)
         return FALSE;
      else
         return m_pimpl->send_message(pevent);
   }

#endif


   bool interaction::IsWindowVisible()
   {

      if(m_pimpl.m_p == NULL)
      {

         return false;

      }

      if(m_bHideOnTransparentFrame)
      {

         ::user::interaction * puiTopLevel = GetTopLevel();

         if(puiTopLevel != NULL)
         {

            if(puiTopLevel->frame_is_transparent())
            {

               return false;

            }

         }

      }

      return m_pimpl.m_p->IsWindowVisible();

   }


   bool interaction::enable_window(bool bEnable)
   {

      if(m_pimpl == NULL)
         return FALSE;
      else
         return m_pimpl->enable_window(bEnable);

   }


   bool interaction::ShowWindow(int32_t nCmdShow)
   {

      if(m_pimpl.is_set())
      {

         m_pimpl->ShowWindow(nCmdShow);

      }

      return IsWindowVisible();

   }


   bool interaction::is_window_enabled()
   {
      if(m_pimpl == NULL)
         return FALSE;
      else
         return m_pimpl->is_window_enabled();
   }



   void interaction::SendMessageToDescendants(UINT message,WPARAM wparam,lparam lparam,bool bDeep,bool bOnlyPerm)
   {
      if(m_pimpl == NULL)
         return;
      else
         return m_pimpl->SendMessageToDescendants(message,wparam,lparam,bDeep,bOnlyPerm);
   }


   void interaction::pre_translate_message(signal_details * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

      SCAST_PTR(::message::base,pbase,pobj);

      //if(pbase->m_uiMessage == WM_KEYDOWN)
      //{

      //   SCAST_PTR(::message::key,pkey,pobj);

      //   if(pkey->m_ekey == ::user::key_tab)
      //   {

      //      ::user::control_event ev;

      //      ev.m_puie         = this;

      //      ev.m_eevent       = ::user::event_tab_key;

      //      ev.m_actioncontext        = ::action::source_user;

      //      if(!BaseOnControlEvent(&ev))
      //      {

      //         sp(::user::interaction) pui = Session.get_keyboard_focus();

      //         pui =  pui.is_set() ? pui->keyboard_get_next_focusable() : keyboard_get_next_focusable();

      //         if(pui != NULL)
      //         {

      //            if (!pui->does_consume_key(::user::key_tab))
      //            {

      //               pui->keyboard_set_focus();

      //               pkey->m_bRet = true;

      //            }

      //         }

      //      }

      //   }

      //}


   }


   oswindow interaction::get_handle() const
   {

#if defined(METROWIN)

      sp(::user::interaction) pwnd = NULL;

      try
      {

         pwnd = get_wnd();

         if (pwnd == NULL)
            return NULL;

         return oswindow_get(pwnd->m_pimpl.cast < ::user::interaction_impl >());

      }
      catch (...)
      {

      }

      return NULL;

#else

      ::user::interaction * pui;

      ::user::interaction_impl * pwnd;

      try
      {

         pui = get_wnd();

         if(pui == NULL)
            return NULL;

         if(pui->m_pimpl == NULL)
            return NULL;

         pwnd = pui->m_pimpl->get_user_interaction_impl();

         if(pwnd == NULL)
            return NULL;

         return pwnd->get_handle();

      }
      catch(...)
      {

      }

      return NULL;

#endif


   }



   bool interaction::subclass_window(oswindow posdata)
   {

      if(IsWindow())
      {

         DestroyWindow();

      }

      m_signalptra.remove_all();

      sp(interaction_impl_base) pimplOld = m_pimpl;

      ::window_sp pimplNew = NULL;

      pimplNew = (Application.alloc(System.type_info < interaction_impl >()));

      pimplNew->m_pui = this;

      if(!pimplNew->subclass_window(posdata))
      {

         pimplNew.release();

      }

      if(pimplNew != NULL)
      {

         if(pimplOld != NULL)
         {

            pimplOld->m_pui = NULL;

            pimplOld->_001ClearMessageHandling();

            ::window_sp pwindowOld = pimplOld;

            if(pwindowOld != NULL)
            {

               pwindowOld->install_message_handling(pimplOld);

            }

            pimplOld.release();

         }

         m_pimpl = pimplNew;

         return true;

      }
      else
      {

         return false;

      }

   }

   oswindow interaction::unsubclass_window()
   {

      ::window_sp pwindow = m_pimpl;

      if(pwindow != NULL)
      {

         return pwindow->unsubclass_window();
      }
      return NULL;
   }

#if defined(METROWIN) || defined(APPLE_IOS) || defined(ANDROID)

   bool interaction::initialize_native_window(::user::native_window_initialize * pinitialize)
   {

      if (IsWindow())
      {

         DestroyWindow();

      }

      m_signalptra.remove_all();

      m_pimpl = (Application.alloc(System.type_info < interaction_impl >()));

      m_pimpl->m_pui = this;

      if (!m_pimpl->initialize_native_window(pinitialize))
      {

         m_pimpl.release();

         return false;

      }

      return true;

   }


#endif


   bool interaction::create_window(const RECT & rect, ::user::interaction *pparent,id id)
   {

      if(IsWindow())
      {

         DestroyWindow();

      }

      m_signalptra.remove_all();

      m_pimpl = canew(::user::interaction_child(get_app()));

      if(!m_pimpl->create_window(this, rect, pparent,id))
      {

         m_pimpl.release();

         return false;

      }

      return true;

   }


   bool interaction::create_window(const char * lpszClassName,const char * lpszWindowName,uint32_t dwStyle,const RECT & rectParam,::user::interaction * pParentWnd,id id, ::create * pcreate)
   {

      if(IsWindow())
      {

         DestroyWindow();

      }

      m_signalptra.remove_all();

      sp(interaction_impl_base) pimplNew;

      ::rect rect(rectParam);

      if(rect.area() <= 0)
      {

         if(pParentWnd != NULL && dynamic_cast <::user::place_holder *> (pParentWnd) != NULL)
         {

            pParentWnd->GetClientRect(rect);

         }

      }


#if defined(APPLE_IOS) || defined(VSNORD) || defined(METROWIN)

      if(pParentWnd == NULL || pParentWnd == System.m_possystemwindow->m_pui)
      {

         pParentWnd = System.m_possystemwindow->m_pui;

         //if(!Application.defer_initialize_twf())
         //{

         //   //m_threadptra.remove_all();

         //   return false;

         //}

      }

#endif


#if defined(WINDOWSEX) || defined(LINUX)

      if(pParentWnd == NULL || pParentWnd->get_safe_handle() == (oswindow)HWND_MESSAGE)

#else

      if(pParentWnd == NULL)

#endif
      {

         pimplNew = (Application.alloc(System.type_info < interaction_impl >()));

         if(!pimplNew->create_window(this, lpszClassName,lpszWindowName,dwStyle,rect,pParentWnd,id,pcreate))
         {

            m_pimpl.release();

            pimplNew.release();

         }

      }
      else
      {

         pimplNew = canew(::user::interaction_child(get_app()));

         if(!pimplNew->create_window(this, lpszClassName,lpszWindowName,dwStyle,rect,pParentWnd,id, pcreate))
         {

            pimplNew.release();

         }

      }

      if(m_pimpl.is_null())
      {

         return false;

      }

      return true;

   }


   bool interaction::create_window_ex(uint32_t dwExStyle,const char * lpszClassName,const char * lpszWindowName,uint32_t dwStyle,const RECT & rect,::user::interaction * pParentWnd,id id,LPVOID lpParam)
   {

      if(IsWindow())
      {

         DestroyWindow();

      }

      m_signalptra.remove_all();

      //::thread * pthread = ::get_thread();

      //if(pthread != NULL)
      //{

      //   m_threadptra.add(pthread);

      //}

#if defined(APPLE_IOS) || defined(VSNORD) || defined(METROWIN)

      if(pParentWnd == NULL || pParentWnd == System.m_possystemwindow->m_pui)
      {

         pParentWnd = System.m_possystemwindow->m_pui;

//         if(!Application.defer_initialize_twf())
//         {
//
////            m_threadptra.remove_all();
//
//            return false;
//
//         }

      }

#endif

      //if(m_threadptra.get_count() <= 0)
      //{

      //   m_threadptra.add(get_app());

      //}
#if !defined(METROWIN)
      if(pParentWnd == NULL)
      {

         m_pimpl = Application.alloc(System.type_info < interaction_impl >());
         //oswindow window = m_pimpl->m_pui->get_handle();
         dwStyle &= ~WS_CHILD;
         if(!m_pimpl->create_window_ex(this, dwExStyle,lpszClassName,lpszWindowName,dwStyle,rect,pParentWnd,id,lpParam))
         {

            //m_threadptra.remove_all();

            m_pimpl.release();

            return false;
         }
         //install_message_handling(this);
         return true;
      }
      else
#endif
      {
#if defined(METROWIN) || defined(APPLE_IOS) || defined(VSNORD)
         if(pParentWnd == NULL)
            pParentWnd = System.m_possystemwindow->m_pui;
#endif

         ::rect rectFrame(0, 0, 0, 0);

         if(IsRectEmpty(&rect))
         {

            if(rectFrame.is_null() && pParentWnd->is_place_holder())
            {

               pParentWnd->GetClientRect(rectFrame);

            }

         }
         else
         {

            rectFrame = rect;

         }

         //synch_lock sl(pParentWnd == NULL ? NULL : pParentWnd->m_pmutex);

         m_pimpl = canew(::user::interaction_child(get_app()));
         m_pimpl->m_pui = this;
         if(!m_pimpl->create_window_ex(this, dwExStyle,lpszClassName,lpszWindowName,dwStyle,rectFrame,pParentWnd,id,lpParam))
         {

            //m_threadptra.remove_all();

            m_pimpl.release();

            return false;
         }
         //install_message_handling(this);
         return true;
      }

   }


   void interaction::BringToTop(int nCmdShow)
   {

      if(m_pimpl != NULL)
         return m_pimpl->BringToTop(nCmdShow);

   }


   bool interaction::BringWindowToTop()
   {

      if(m_pimpl == NULL)
         return false;
      else
         return m_pimpl->BringWindowToTop();

   }


   bool interaction::IsWindow() const
   {

      if(m_pimpl == NULL)
      {

         return false;

      }

      return m_pimpl->IsWindow();

   }


   LONG interaction::get_window_long(int32_t nIndex) const
   {
      if(m_pimpl == NULL)
         return 0;
      else
         return m_pimpl->get_window_long(nIndex);
   }

   LONG interaction::set_window_long(int32_t nIndex,LONG lValue)
   {
      if(m_pimpl == NULL)
         return 0;
      else
         return m_pimpl->set_window_long(nIndex,lValue);
   }

   LONG_PTR interaction::get_window_long_ptr(int32_t nIndex) const
   {
      if(m_pimpl == NULL)
         return 0;
      else
         return m_pimpl->get_window_long_ptr(nIndex);
   }

   LONG_PTR interaction::set_window_long_ptr(int32_t nIndex,LONG_PTR lValue)
   {
      if(m_pimpl == NULL)
         return 0;
      else
         return m_pimpl->set_window_long_ptr(nIndex,lValue);
   }


   bool interaction::RedrawWindow(LPCRECT lpRectUpdate, ::draw2d::region* prgnUpdate, UINT flags)
   {

      if (m_pimpl == NULL)
      {

         return false;

      }

      return m_pimpl->RedrawWindow(lpRectUpdate,prgnUpdate,flags);

   }


   ::user::interaction * interaction::ChildWindowFromPoint(POINT point)
   {
      if(m_pimpl == NULL)
         return NULL;
      else
         return m_pimpl->ChildWindowFromPoint(point);
   }

   ::user::interaction * interaction::ChildWindowFromPoint(POINT point,UINT nFlags)
   {
      if(m_pimpl == NULL)
         return NULL;
      else
         return m_pimpl->ChildWindowFromPoint(point,nFlags);
   }

   ::user::interaction * interaction::get_next_window(UINT nFlag)
   {
      if(m_pimpl == NULL)
         return NULL;
      else
         return m_pimpl->get_next_window(nFlag);
   }

   ::user::interaction * interaction::get_next(bool bIgnoreChildren,int32_t * piLevel)
   {

      if(!bIgnoreChildren)
      {
         if(m_uiptraChild.has_elements())
         {
            if(piLevel != NULL)
               (*piLevel)++;
            return m_uiptraChild[0];
         }
      }
      if(GetParent() == NULL)
      {
         // todo, reached desktop or similar very top system interaction_impl
         return NULL;
      }

      synch_lock slParent(GetParent()->m_pmutex);
      synch_lock sl(m_pmutex);


      index iFind = GetParent()->m_uiptraChild.find_first(this);

      if(iFind < 0)
         throw "not expected situation";

      if(iFind < GetParent()->m_uiptraChild.get_upper_bound())
      {
         return GetParent()->m_uiptraChild[iFind + 1];
      }

      if(GetParent()->GetParent() == NULL)
      {
         // todo, reached desktop or similar very top system interaction_impl
         return NULL;
      }

      if(piLevel != NULL)
         (*piLevel)--;

      return GetParent()->GetParent()->get_next(true,piLevel);

   }

   bool interaction::is_message_only_window() const
   {

      return m_bMessageWindow;

   }

   ::user::interaction * interaction::GetTopWindow()
   {
      if(m_pimpl == NULL)
         return NULL;
      else
         return m_pimpl->GetTopWindow();
   }

   ::user::interaction * interaction::get_wnd(UINT nCmd) const
   {
      if(m_pimpl == NULL)
         return NULL;
      else
         return m_pimpl->get_wnd(nCmd);
   }

   ::user::interaction * interaction::GetActiveWindow()
   {
      if(m_pimpl == NULL)
         return NULL;
      else
         return m_pimpl->GetActiveWindow();
   }


   bool interaction::SetFocus()
   {

      if (m_pimpl == NULL)
      {

         return false;

      }
      else
      {

         return m_pimpl->SetFocus();

      }

   }


   ::user::interaction * interaction::GetFocus()
   {

      if(m_pimpl == NULL)
         return NULL;
      else
         return m_pimpl->GetFocus();

   }



   ::user::interaction * interaction::SetActiveWindow()
   {
      if(m_pimpl == NULL)
         return NULL;
      else
         return m_pimpl->SetActiveWindow();
   }

   bool interaction::SetForegroundWindow()
   {
      if(m_pimpl == NULL)
         return FALSE;
      else
         return m_pimpl->SetForegroundWindow();
   }

   ::user::interaction * interaction::GetLastActivePopup()
   {
      if(m_pimpl == NULL)
         return NULL;
      else
         return m_pimpl->GetLastActivePopup();
   }


   void interaction::set_window_text(const char * lpszString)
   {

      m_strWindowText = lpszString;

      if(m_pimpl != NULL)
      {

         m_pimpl->set_window_text(lpszString);

      }

   }


   strsize interaction::get_window_text(char * lpszStringBuf, strsize nMaxCount)
   {

      strsize n = MIN(nMaxCount,m_strWindowText.get_length());

      strncpy(lpszStringBuf,m_strWindowText, n);

      return n;

   }


   string interaction::get_window_text()
   {

      string str;

      get_window_text(str);

      return str;

   }


   void interaction::get_window_text(string & rString)
   {

      synch_lock sl(m_pmutex);

      rString = m_strWindowText;

   }


   strsize interaction::get_window_text_length()
   {

      synch_lock sl(m_pmutex);

      if (m_pimpl == NULL)
      {
       
         return 0;

      }
      
      return m_pimpl->get_window_text_length();

   }


   ::user::interaction * interaction::GetParentTopLevel() const
   {

      sp(::user::interaction) puiParent = GetParent();

      sp(::user::interaction) puiParentTopLevel;

      if(puiParent.is_null())
         return NULL;

      do
      {

         puiParentTopLevel = puiParent;

         puiParent = puiParent->GetParent();

      } while(puiParent.is_set());

      return puiParentTopLevel;

   }


   ::user::interaction * interaction::EnsureParentTopLevel()
   {

      sp(interaction) pwindow=GetParentTopLevel();

      ENSURE_VALID(pwindow);

      return pwindow;

   }


   ::user::interaction * interaction::GetTopLevel() const
   {

      ::user::interaction * puiParent = (::user::interaction *) this;

      ::user::interaction * puiTopLevelParent;

      if(puiParent == NULL)
         return NULL;

      do
      {

         puiTopLevelParent = puiParent;

         puiParent = puiParent->GetParent();

      } while(puiParent != NULL);

      return puiTopLevelParent;

   }


   ::user::interaction * interaction::EnsureTopLevel()
   {

      sp(interaction) pwindow=GetTopLevel();

      ENSURE_VALID(pwindow);

      return pwindow;

   }

   ::user::frame_window * interaction::GetFrame() const
   {

      ::user::interaction * pui = (::user::interaction *) this;

      if (pui == NULL)
      {

         return NULL;

      }

      ::user::frame_window * pframeParent;

      do
      {

         pframeParent = dynamic_cast < ::user::frame_window * > (pui);

         if(pframeParent != NULL)
            return pframeParent;

         pui = pui->GetParent();

      } while(pui != NULL);

      return NULL;

   }


   ::user::frame_window * interaction::GetParentFrame() const
   {

      ::user::interaction * pui = GetParent();

      if(pui == NULL)
         return NULL;

      return pui->GetFrame();

   }


   ::user::frame_window * interaction::GetTopLevelFrame() const
   {

      sp(::user::frame_window) pframe = GetFrame();

      if(pframe.is_null())
         return NULL;

      sp(::user::frame_window) pframeTopLevel;

      do
      {

         pframeTopLevel = pframe;

         pframe = pframe->GetParentFrame();

      } while(pframe.is_set());

      return pframeTopLevel;

   }


   ::user::frame_window * interaction::GetParentTopLevelFrame() const
   {

      sp(::user::frame_window) pframe = GetParentFrame();

      if(pframe.is_null())
         return NULL;

      return pframe->GetTopLevelFrame();

   }


   ::user::frame_window * interaction::EnsureParentFrame()
   {

      ::user::frame_window * pFrameWnd = GetParentFrame();

      ENSURE_VALID(pFrameWnd);

      return pFrameWnd;

   }


   LRESULT interaction::Default()
   {

      if(m_pimpl == NULL)
         return 0;
      else
         return m_pimpl->Default();

   }


   uint32_t interaction::GetStyle() const
   {

      if(m_pimpl == NULL)
         return 0;
      else
         return m_pimpl->GetStyle();

   }


   uint32_t interaction::GetExStyle() const
   {

      if(m_pimpl == NULL)
         return 0;
      else
         return m_pimpl->GetExStyle();

   }


   bool interaction::ModifyStyle(uint32_t dwRemove,uint32_t dwAdd,UINT nFlags)
   {

      if(m_pimpl == NULL)
         return false;

      return m_pimpl->ModifyStyle(dwRemove,dwAdd,nFlags);

   }


   bool interaction::ModifyStyleEx(uint32_t dwRemove,uint32_t dwAdd,UINT nFlags)
   {

      if(m_pimpl == NULL)
         return false;

      return m_pimpl->ModifyStyleEx(dwRemove,dwAdd,nFlags);

   }


   bool interaction::DestroyWindow()
   {

///      single_lock slTwf(System.wait_twf(), true);

      single_lock sl1(get_wnd() != NULL && get_wnd() != this ? get_wnd()->m_pmutex : NULL,true);

      //single_lock sl(m_pmutex,true);

      m_bCreated = false;

      if(!IsWindow())
      {


         user_interaction_on_destroy();

         return true;

      }

      m_bDestroying = true;

      sp(::user::interaction) pui = this; // keep a reference to this while destroying

      //{

      //   synch_lock sl(get_ui_destroyed_mutex());

      m_bDestroying = true;

      //   try
      //   {

      //      for(index i = 0; i < m_bptraTellMeDestroyed.get_count(); i++)
      //      {

      //         *m_bptraTellMeDestroyed[i] = true;

      //      }

      //   }
      //   catch(...)
      //   {

      //   }

      //}

      if(m_pimpl == NULL)
         return FALSE;
      else
         return m_pimpl->DestroyWindow();
   }



   // for custom cleanup after WM_NCDESTROY
   void interaction::PostNcDestroy()
   {

      synch_lock sl(m_pmutex);

      try
      {

         signalizable_disconnect_all();

      }
      catch(...)
      {

      }


      try
      {

         m_signala.remove_all();

      }
      catch(...)
      {

      }

      try
      {

         m_dispatchUpdateCmdUi.m_signala.remove_all();

      }
      catch(...)
      {

      }


      try
      {

         m_dispatchCommand.m_signala.remove_all();

      }
      catch(...)
      {

      }


      try
      {

         m_signalidaCommand.remove_all();

      }
      catch(...)
      {

      }

      try
      {

         ::signalizable::m_signalptra.remove_all();

      }
      catch(...)
      {

      }


      //try
      //{

      //   m_pauraapp = NULL;

      //}
      //catch (...)
      //{

      //}


      {

         m_pimpl.release();

      }

   }





   void interaction::CalcWindowRect(LPRECT lprect,UINT nAdjustType)
   {
      if(m_pimpl == NULL)
         return;
      else
         return m_pimpl->CalcWindowRect(lprect,nAdjustType);
   }


   void interaction::RepositionBars(UINT nIDFirst,UINT nIDLast,id nIDLeftOver,
                                    UINT nFlag,LPRECT lpRectParam,
                                    LPCRECT lpRectClient,bool bStretch)
   {
      if(m_pimpl == NULL)
         return;
      else
         return m_pimpl->RepositionBars(nIDFirst,nIDLast,nIDLeftOver,nFlag,lpRectParam,lpRectClient,bStretch);
   }


   ::user::interaction * interaction::SetOwner(::user::interaction * pui)
   {

      if(m_pimpl == NULL)
         return NULL;

      return m_pimpl->SetOwner(pui);

   }


   ::user::interaction * interaction::GetOwner() const
   {

      if(m_pimpl == NULL)
         return NULL;

      return m_pimpl->GetOwner();

   }

   ::user::interaction * interaction::GetParentOwner() const
   {

      sp(::user::interaction) puiParent = GetParent();

      if(puiParent.is_null())
         return NULL;

      return puiParent->GetOwner();

   }


   ::user::interaction * interaction::GetTopLevelOwner() const
   {

      sp(::user::interaction) puiOwner = GetOwner();

      sp(::user::interaction) puiTopLevelOwner;

      if(puiOwner.is_null())
         return NULL;

      do
      {

         puiTopLevelOwner = puiOwner;

         puiOwner = puiOwner->GetParent();

      } while(puiOwner.is_set());

      return puiTopLevelOwner;

   }




   interaction * interaction::GetDescendantWindow(id iId) const
   {
      if(m_pimpl == NULL)
         return NULL;
      else
         return m_pimpl->GetDescendantWindow(iId);
   }

   void interaction::viewport_client_to_screen(POINT * ppt)
   {

      m_pimpl->viewport_client_to_screen(ppt);

   }

   bool interaction::IsNotifyIconEnabled()
   {

      return false;

   }

   void interaction::viewport_screen_to_client(POINT * ppt)
   {

      if(m_pimpl.is_null())
         return;

      m_pimpl->viewport_screen_to_client(ppt);

   }


   void interaction::viewport_client_to_screen(RECT * prect)
   {

      viewport_client_to_screen((POINT *)&prect->left);
      viewport_client_to_screen((POINT *)&prect->right);

   }


   void interaction::viewport_screen_to_client(RECT * prect)
   {

      viewport_screen_to_client((POINT *)&prect->left);
      viewport_screen_to_client((POINT *)&prect->right);

   }




   //int32_t interaction::SetWindowRgn(HRGN hRgn,bool bRedraw)
   //{

   //   if(m_pimpl == NULL)
   //      return 0;
   //   else
   //      return m_pimpl->SetWindowRgn(hRgn,bRedraw);

   //}


   //int32_t interaction::GetWindowRgn(HRGN hRgn)
   //{
   //   if(m_pimpl == NULL)
   //      return 0;
   //   else
   //      return m_pimpl->GetWindowRgn(hRgn);
   //}


   bool interaction::WfiIsZoomed()
   {
      if(m_pimpl == NULL)
         return false;
      else
         return m_pimpl->WfiIsZoomed();
   }

   bool interaction::WfiIsFullScreen()
   {
      if(m_pimpl == NULL)
         return false;
      else
         return m_pimpl->WfiIsFullScreen();
   }

   bool interaction::WfiIsIconic()
   {
      if(m_pimpl == NULL)
         return false;
      else
         return m_pimpl->WfiIsIconic();
   }


   bool interaction::CheckAutoCenter()
   {
      if(m_pimpl == NULL)
         return TRUE;
      else
         return m_pimpl->CheckAutoCenter();
   }

   void interaction::CenterWindow(::user::interaction * pAlternateOwner)
   {
      if(m_pimpl == NULL)
         return;
      else
         m_pimpl->CenterWindow(pAlternateOwner);
   }


   LRESULT interaction::DefWindowProc(UINT uiMessage,WPARAM wparam,lparam lparam)
   {

      if(m_pimpl == NULL)
         return 0;
      else
         return m_pimpl->DefWindowProc(uiMessage,wparam,lparam);

   }


   void interaction::message_handler(signal_details * pobj)
   {
      if(m_pimpl == NULL)
         return;
      else
      {
         interaction_impl_base * pimpl = m_pimpl;
         return pimpl->message_handler(pobj);
      }
   }


   LRESULT interaction::message_handler(LPMESSAGE lpmessage)
   {

      return send_message(lpmessage->message,lpmessage->wParam,lpmessage->lParam);

   }

#ifdef WINDOWSEX

   bool interaction::GetWindowPlacement(WINDOWPLACEMENT* lpwndpl)
   {
      if(m_pimpl == NULL)
         return FALSE;
      else
         return m_pimpl->GetWindowPlacement(lpwndpl);
   }

   bool interaction::SetWindowPlacement(const WINDOWPLACEMENT* lpwndpl)
   {
      if(m_pimpl == NULL)
         return FALSE;
      else
         return m_pimpl->SetWindowPlacement(lpwndpl);
   }

#endif


   bool interaction::pre_create_window(::user::create_struct& cs)
   {

      return true;

   }


   bool interaction::IsTopParentActive()
   {
      if(m_pimpl == NULL)
         return FALSE;
      else
         return m_pimpl->IsTopParentActive();
   }

   void interaction::ActivateTopParent()
   {
      if(m_pimpl == NULL)
         return;
      else
         m_pimpl->ActivateTopParent();
   }

   void interaction::UpdateDialogControls(command_target* pTarget,bool bDisableIfNoHandler)
   {
      if(m_pimpl == NULL)
         return;
      else
         m_pimpl->UpdateDialogControls(pTarget,bDisableIfNoHandler);
   }

   void interaction::UpdateWindow()
   {
      if(m_pimpl == NULL)
         return;
      else
         m_pimpl->UpdateWindow();
   }

   void interaction::register_drop_target()
   {
      if(m_pimpl == NULL)
         return;
      else
         m_pimpl->register_drop_target();
   }

   void interaction::SetRedraw(bool bRedraw)
   {
      if(m_pimpl == NULL)
         return;
      else
         m_pimpl->SetRedraw(bRedraw);
   }

   bool interaction::GetUpdateRect(LPRECT lpRect,bool bErase)
   {
      if(m_pimpl == NULL)
         return FALSE;
      else
         return m_pimpl->GetUpdateRect(lpRect,bErase);
   }

   int32_t interaction::GetUpdateRgn(::draw2d::region* pRgn,bool bErase)
   {
      if(m_pimpl == NULL)
         return 0;
      else
         return m_pimpl->GetUpdateRgn(pRgn,bErase);

   }

   void interaction::Invalidate(bool bErase)
   {
      if(m_pimpl == NULL)
         return;
      else
         m_pimpl->Invalidate(bErase);
   }

   void interaction::InvalidateRect(LPCRECT lpRect,bool bErase)
   {
      if(m_pimpl == NULL)
         return;
      else
         m_pimpl->InvalidateRect(lpRect,bErase);
   }

   void interaction::InvalidateRgn(::draw2d::region* pRgn,bool bErase)
   {
      if(m_pimpl == NULL)
         return;
      else
         m_pimpl->InvalidateRgn(pRgn,bErase);
   }

   void interaction::ValidateRect(LPCRECT lpRect)
   {
      if(m_pimpl == NULL)
         return;
      else
         m_pimpl->ValidateRect(lpRect);
   }
   void interaction::ValidateRgn(::draw2d::region* pRgn)
   {
      if(m_pimpl == NULL)
         return;
      else
         m_pimpl->ValidateRgn(pRgn);
   }


   void interaction::on_layout()
   {

      layout_tooltip();

      on_change_view_size();

   }


   void interaction::on_set_window_pos()
   {

   }


   void interaction::on_translate()
   {

      layout_tooltip();

   }


   void interaction::on_do_show_flags()
   {

      layout_tooltip();

   }



   void interaction::ShowOwnedPopups(bool bShow)
   {
      if(m_pimpl == NULL)
         return;
      else
         m_pimpl->ShowOwnedPopups(bShow);
   }

   void * interaction::get_os_data() const
   {

      return get_handle();

   }

   bool interaction::attach(oswindow oswindow_New)
   {
      if(m_pimpl == NULL)
         return FALSE;
      else
         return m_pimpl->attach(oswindow_New);
   }

   oswindow interaction::detach()
   {
      if(m_pimpl == NULL)
         return NULL;
      else
         return m_pimpl->detach();
   }

   void interaction::pre_subclass_window()
   {
      if(m_pimpl == NULL)
         return;
      else
         m_pimpl->pre_subclass_window();
   }


   id interaction::run_modal_loop(::user::interaction * pui,uint32_t dwFlags,::object * pliveobject)
   {

      return pui->_001RunModalLoop(dwFlags,pliveobject);

   }


   id interaction::RunModalLoop(uint32_t dwFlags,::object * pliveobject)
   {

      return _001RunModalLoop(dwFlags,pliveobject);

   }


   id interaction::_001RunModalLoop(uint32_t dwFlags,::object * pliveobject)
   {

      // for tracking the idle time state
      bool bIdle = TRUE;
      LONG lIdleCount = 0;
      bool bShowIdle = (dwFlags & MLF_SHOWONIDLE) && !(GetStyle() & WS_VISIBLE);
      //      oswindow oswindow_Parent = ::GetParent(get_handle());
      m_iModal = m_iModalCount;
      int32_t iLevel = m_iModal;
      sp(::user::interaction) puieParent = GetParent();
      oprop(string("RunModalLoop.thread(") + ::str::from(iLevel) + ")") = ::get_thread();
      m_iModalCount++;

      //bool bAttach = AttachThreadInput(get_wnd()->get_os_int(), ::GetCurrentThreadId(), TRUE);

      m_threadptraModal.add(::get_thread());
      sp(::aura::application) pappThis1 = (m_pimpl);
      sp(::aura::application) pappThis2 = (::get_thread());
      // acquire and dispatch messages until the modal state is done
      MESSAGE msg;

      ::thread * pthread = ::get_thread();

      //if(pthread == NULL)
      //{

      //   pthread = m_threadptra.is_empty() ? NULL : m_threadptra[0];

      //}

      for(;;)
      {
         if(!ContinueModal(iLevel))
            goto ExitModal;

         // phase1: check to see if we can do idle work
         while(bIdle && !::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
         {
            if(!ContinueModal(iLevel))
               goto ExitModal;

            // show the dialog when the message queue goes idle
            if(bShowIdle)
            {
               ShowWindow(SW_SHOWNORMAL);
               UpdateWindow();
               bShowIdle = FALSE;
            }

//            pthread->step_timer();

            // call on_idle while in bIdle state
            if(!(dwFlags & MLF_NOIDLEMSG) && puieParent != NULL && lIdleCount == 0)
            {
               // send WM_ENTERIDLE to the parent
               //puieParent->send_message(WM_ENTERIDLE,MSGF_DIALOGBOX,(LPARAM)(DWORD_PTR)NULL);
            }
            /*if ((dwFlags & MLF_NOKICKIDLE) ||
            !__call_window_procedure(this, get_handle(), WM_KICKIDLE, MESSAGEF_DIALOGBOX, lIdleCount++))
            {
            // stop idle processing next time
            bIdle = FALSE;
            }*/

            pthread->m_dwAlive = ::get_tick_count();
            if(pappThis1 != NULL)
            {
               pappThis1->m_dwAlive = pthread->m_dwAlive;
            }
            if(pappThis2 != NULL)
            {
               pappThis2->m_dwAlive = pthread->m_dwAlive;
            }
            if(pliveobject != NULL)
            {
               pliveobject->keep_alive();
            }
         }


         // phase2: pump messages while available
         do
         {

            pthread->pump_message();

            if(!ContinueModal(iLevel))
               goto ExitModal;


            if(::get_thread() != NULL && !::get_thread()->pump_message())
            {
               __post_quit_message(0);
               return -1;
            }

            // show the interaction_impl when certain special messages rec'd
            //if(bShowIdle &&
            // (msg.message == 0x118 || msg.message == WM_SYSKEYDOWN))
            //{
            // ShowWindow(SW_SHOWNORMAL);
            //UpdateWindow();
            //bShowIdle = FALSE;
            //}

            if(!ContinueModal(iLevel))
               goto ExitModal;


            keep_alive(pliveobject);

         } while(::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE) != FALSE);


//         pthread->step_timer();

         if(!ContinueModal(iLevel))
            goto ExitModal;


      }



ExitModal:

      //#ifdef WINDOWS

      m_threadptraModal.remove_first(::get_thread());

      //#else

      //    m_iaModalThread.remove_first(::pthread_self());

      //#endif

      m_iModal = m_iModalCount;

      return m_idModalResult;

   }

   bool interaction::ContinueModal(int32_t iLevel)
   {
      return iLevel < m_iModalCount && (::get_thread() == NULL || ::get_thread_run()) && m_pauraapp->get_run_thread();
   }

   void interaction::EndModalLoop(id nResult)
   {
      ASSERT(IsWindow());

      // this result will be returned from interaction_impl::RunModalLoop
      m_idModalResult = nResult;

      // make sure a message goes through to exit the modal loop
      if(m_iModalCount > 0)
      {
         m_iModalCount--;
         for (index i = 0; i < m_threadptraModal.get_count(); i++)
         {

            m_threadptraModal[i]->kick_thread();

         }


         kick_queue();

         try
         {

            sp(::thread) pthread = ::get_thread();

            if(pthread.is_set())
            {

               pthread->kick_thread();

            }

         }
         catch(...)
         {

         }

         try
         {

            sp(::thread) pthread = ::get_thread();

            if(pthread.is_set())
            {

               pthread->kick_thread();

            }

         }
         catch(...)
         {

         }

      }

   }


   void interaction::EndAllModalLoops(id nResult)
   {

      ASSERT(IsWindow());

      // this result will be returned from interaction_impl::RunModalLoop
      m_idModalResult = nResult;

      // make sure a message goes through to exit the modal loop
      if(m_iModalCount > 0)
      {

         int32_t iLevel = m_iModalCount - 1;

         m_iModalCount = 0;

         kick_queue();

         ::get_thread()->kick_thread();

         for(int32_t i = iLevel; i >= 0; i--)
         {

            thread * pthread = oprop(string("RunModalLoop.thread(") + ::str::from(i) + ")").cast < thread >();

            try
            {

               pthread->kick_thread();

            }
            catch(...)
            {

            }

         }

      }

   }


   bool interaction::BaseOnControlEvent(control_event * pevent)
   {

      ::user::interaction * puiParent = GetParent();

      if(puiParent != NULL)
      {

         return puiParent->BaseOnControlEvent(pevent);

      }
      else
      {

         return false;

      }

   }


   bool interaction::post_message(UINT uiMessage,WPARAM wparam,lparam lparam)
   {

      if(m_pimpl == NULL)
         return FALSE;
      else
         return m_pimpl->post_message(uiMessage,wparam,lparam);

   }


   bool interaction::post_object(UINT uiMessage, WPARAM wparam, lparam lparam)
   {

      bool bIsWindow = m_pimpl.is_set() && IsWindow();

      if (uiMessage == WM_QUIT || !bIsWindow)
      {

         {

            // discards object
            sp(::object) spo(lparam);

         }

         if (!bIsWindow)
         {

            return false;

         }

         return m_pimpl->post_message(uiMessage);

      }

      return m_pimpl->post_message(uiMessage, wparam, lparam);

   }


   // timer Functions
   bool interaction::SetTimer(uint_ptr nIDEvent,UINT nElapse, PFN_TIMER pfnTimer)
   {

      if(m_pimpl == NULL)
         return false;

      return m_pimpl->SetTimer(nIDEvent,nElapse,pfnTimer);

   }


   bool interaction::KillTimer(uint_ptr nIDEvent)
   {

      if(m_pimpl == NULL)
         return false;

      return m_pimpl->KillTimer(nIDEvent);

   }


   bool interaction::has_focus()
   {

      return Session.get_keyboard_focus() == this;

   }


   bool interaction::SetCapture(::user::interaction * pui)
   {

      if(m_pimpl == NULL)
      {

         return false;

      }

      if (pui == NULL)
      {

         pui = this;

      }

      return m_pimpl->SetCapture(pui);

   }


   ::user::interaction * interaction::GetCapture()
   {

      if(m_pimpl != NULL)
      {

         return m_pimpl->GetCapture();

      }

      ::user::interaction * pwnd = get_wnd();

      if(pwnd != NULL)
      {

         return pwnd->GetCapture();

      }

      return NULL;

   }


   bool interaction::ReleaseCapture()
   {

      if (m_pimpl == NULL)
      {

         return false;

      }

      return m_pimpl->ReleaseCapture();

   }


   void interaction::track_mouse_leave()
   {

      track_mouse_hover();

   }


   void interaction::track_mouse_hover()
   {

      ::user::interaction * pui = GetTopLevel();

      if(pui == NULL)
         return;

      if(pui->get_wnd() == NULL)
         return;

      pui->get_wnd()->mouse_hover_add(this);

   }




   void interaction::_001UpdateWindow(bool bUpdateBuffer)
   {

      if (m_bLockWindowUpdate)
         return;

      if (m_pimpl == NULL)
         return;

      m_pimpl->_001UpdateWindow(bUpdateBuffer);

   }





   void interaction::_001WindowMinimize(bool bNoActivate)
   {

      m_pimpl->_001WindowMinimize(bNoActivate);

   }


   void interaction::_001WindowMaximize()
   {

      m_pimpl->_001WindowMaximize();

   }


   void interaction::_001WindowFullScreen()
   {

      m_pimpl->_001WindowFullScreen();

   }


   void interaction::_001WindowRestore()
   {

      m_pimpl->_001WindowRestore();

   }


   void interaction::_001WindowDock(::user::EAppearance eappearance)
   {

      m_pimpl->_001WindowDock(eappearance);

   }


   void interaction::GuieProc(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void interaction::_001DeferPaintLayeredWindowBackground(::draw2d::graphics * pgraphics)
   {
      if(m_pimpl != NULL)
      {
         //         m_pimpl->_001DeferPaintLayeredWindowBackground(pgraphics);
      }
   }

   void interaction::_001OnDeferPaintLayeredWindowBackground(::draw2d::graphics * pgraphics)
   {

      _001DeferPaintLayeredWindowBackground(pgraphics);

   }







   void interaction::OnLinkClick(const char * psz,const char * pszTarget)
   {

      System.open_link(psz,"",pszTarget);

   }


   void interaction::on_set_parent(::user::interaction * puiParent)
   {

      if (m_pparent == puiParent)
      {

         return;

      }

      try
      {
         if(m_pparent != NULL)
         {

            single_lock sl(m_pparent->m_pmutex,TRUE);

            single_lock sl2(m_pmutex,TRUE);

            m_pparent->m_uiptraChild.remove(this);

         }
      }
      catch(...)
      {
      }

      m_pparent = puiParent;

      try
      {
         if(puiParent != NULL)
         {

            single_lock sl(puiParent->m_pmutex,TRUE);

            single_lock sl2(m_pmutex,TRUE);

            puiParent->m_uiptraChild.add_unique(this);

         }
      }
      catch(...)
      {
      }

   }


   //bool interaction::SetTimer(uint_ptr nIDEvent,UINT nEllapse, PFN_TIMER pfnTimer)
   //{
   //
   //   return ::aura::timer_array::SetTimer(nIDEvent,nEllapse, pfnTimer);

   //}


   //bool interaction::KillTimer(uint_ptr nIDEvent)
   //{

   //   return ::aura::timer_array::KillTimer(nIDEvent);

   //}



   bool interaction::create_message_queue(const char * pszName)
   {

      if(IsWindow())
      {

         DestroyWindow();

      }

      m_signalptra.remove_all();

      m_pimpl = Application.alloc(System.type_info < interaction_impl >());

      if(m_pimpl == NULL)
         return false;

      m_bMessageWindow = true;

      m_pimpl->m_pui = this;

      //::thread * pthread = ::get_thread();

      //if(pthread != NULL)
      //{

      //   m_threadptra.add(pthread);

      //}

      //if(m_threadptra.get_count() <= 0)
      //{

      //   m_threadptra.add(get_app());

      //}

      if(!m_pimpl->create_message_queue(this, pszName))
      {

         //m_threadptra.remove_all();

         m_pimpl.release();

         return false;

      }

      return true;

   }



   void interaction::walk_pre_translate_tree(signal_details * pobj, ::user::interaction * puiStop)
   {

      if(puiStop == this)
      {

         return;

      }

      try
      {

         pre_translate_message(pobj);

      }
      catch(...)
      {

      }

      if(!m_bDefaultWalkPreTranslateParentTree)
         return;

      if(pobj->m_bRet)
         return;

      ::user::interaction * puiParent = GetParent();

      if(puiParent != NULL)
         return;

      if(puiParent == puiStop)
         return;

      try
      {

         puiParent->pre_translate_message(pobj);

      }
      catch(...)
      {

      }



   }

   void interaction::on_select()
   {
   }

   bool interaction::is_place_holder()
   {
      return false;
   }

   ::visual::e_cursor interaction::get_cursor()
   {
      return m_ecursor;
   }

   void interaction::set_cursor(::visual::e_cursor ecursor)
   {
      m_ecursor = ecursor;
   }

   void interaction::_001OnMouseMove(signal_details * pobj)
   {
      SCAST_PTR(::message::mouse,pmouse,pobj);
      pmouse->m_ecursor = get_cursor();
   }





   bool interaction::can_merge(::user::interaction * pui)
   {
      UNREFERENCED_PARAMETER(pui);
      return false;
   }

   bool interaction::merge(::user::interaction * pui)
   {
      UNREFERENCED_PARAMETER(pui);
      return false;
   }

   ::user::elemental * interaction::first_child_elemental()
   {

      return first_child();

   }


   ::user::elemental * interaction::top_elemental()
   {

      return interaction_base::top_elemental();

   }


   ::user::elemental * interaction::under_elemental()
   {

      return under_sibling();

   }


   ::user::elemental * interaction::above_elemental()
   {

      return above_sibling();

   }


   ::user::elemental * interaction::next_elemental()
   {

      return next_sibling();

   }


   ::user::elemental * interaction::previous_elemental()
   {

      return previous_sibling();

   }



   ::user::interaction * interaction::first_child()
   {

      try
      {
         if(m_uiptraChild.get_count() <= 0)
            return NULL;
         else
            return m_uiptraChild.first_sp();

      }
      catch(...)
      {
      }
      return NULL;
   }

   ::user::interaction * interaction::last_child()
   {
      try
      {
         if(m_uiptraChild.get_count() <= 0)
            return NULL;
         else
            return m_uiptraChild.last_sp();
      }
      catch(...)
      {
      }
      return NULL;
   }



   ::user::interaction * interaction::first_sibling()
   {
      try
      {

         if(GetParent() == NULL)
            return NULL;

         return GetParent()->first_child();

      }
      catch(...)
      {
      }
      return NULL;

   }


   ::user::interaction * interaction::next_sibling()
   {
      try
      {

         ::user::interaction * pui = NULL;
         try
         {
            pui = GetParent();
         }
         catch(...)
         {
            return NULL;
         }
         if(pui == NULL)
            return NULL;
         index i = pui->m_uiptraChild.find_first(this);
         if(i < 0)
            return NULL;
         i++;
         if(i >= pui->m_uiptraChild.get_count())
            return NULL;
         else
            return pui->m_uiptraChild[i];

      }
      catch(...)
      {
      }
      return NULL;


   }

   ::user::interaction * interaction::next_sibling(::user::interaction * pui)
   {
      synch_lock sl(m_pmutex);
      try
      {

         index i = m_uiptraChild.find_first(pui);
         if(i < 0)
            return NULL;
         i++;
restart:
         if(i >= m_uiptraChild.get_count())
            return NULL;
         else
         {
            try
            {
               return m_uiptraChild[i];
            }
            catch(...)
            {
               goto restart;
            }
         }
      }
      catch(...)
      {

      }

      return NULL;

   }

   ::user::interaction * interaction::previous_sibling()
   {

      try
      {

         ::user::interaction * pui = NULL;
         try
         {
            pui = GetParent();
         }
         catch(...)
         {
            return NULL;
         }
         if(pui == NULL)
            return NULL;
         index i = pui->m_uiptraChild.find_first(this);
         if(i < 0)
            return NULL;
         i--;
         if(i < 0)
            return NULL;
         else
            return pui->m_uiptraChild[i];

      }
      catch(...)
      {

      }

      return NULL;

   }

   ::user::interaction * interaction::last_sibling()
   {

      try
      {

         if(GetParent() == NULL)
            return NULL;

         return GetParent()->last_child();

      }
      catch(...)
      {

      }

      return NULL;


   }


   void interaction::mouse_hover_add(::user::interaction * pinterface)
   {

      if(m_pimpl == NULL)
         return;

      m_pimpl->mouse_hover_add(pinterface);

   }

   void interaction::mouse_hover_remove(::user::interaction * pinterface)
   {

      if(m_pimpl == NULL)
         return;

      m_pimpl->mouse_hover_remove(pinterface);

   }


   ::user::interaction * interaction::previous_sibling(::user::interaction * pui)
   {
      single_lock sl(m_pmutex,TRUE);
      index i = m_uiptraChild.find_first(pui);
      if(i < 0)
         return NULL;
restart:
      i--;
      if(i < 0)
         return NULL;
      else
      {
         try
         {
            return m_uiptraChild[i];
         }
         catch(...)
         {
            goto restart;
         }
      }
   }

   void interaction::_001OnUser184(signal_details * pobj)
   {
      SCAST_PTR(::message::base,pbase,pobj);
      if(pbase->m_wparam == 0 &&
            pbase->m_lparam == 0)
      {
         class rect rect;
         ::exception::throw_not_implemented(get_app());
         /*         System.get_monitor_rect(0, &rect);
         rect.deflate(rect.width() / 4, rect.height() / 4);
         SetWindowPos(ZORDER_TOP, rect.left, rect.top, rect.width(), rect.height(), 0);
         pbase->m_bRet = true;*/
      }
   }

   bool interaction::on_keyboard_focus(::user::elemental * pfocus)
   {

      if(m_pimpl == NULL)
         return true;


      return m_pimpl->on_keyboard_focus(pfocus);

   }


   bool interaction::keyboard_focus_OnKillFocus()
   {

      if(m_pimpl == NULL)
         return true;


      return m_pimpl->keyboard_focus_OnKillFocus();

   }


   bool interaction::keyboard_focus_OnChildKillFocus()
   {

      if(m_pimpl == NULL)
         return true;


      return m_pimpl->keyboard_focus_OnChildKillFocus();

   }


   ::user::interaction * interaction::get_os_focus_uie()
   {

      return NULL;

   }


   bool interaction::IsAscendant(const interaction * puiIsAscendant) const
   {

      return m_pimpl->IsAscendant(puiIsAscendant);

   }


   bool interaction::IsParent(const interaction * puiIsParent) const
   {

      return m_pimpl->IsParent(puiIsParent);

   }


   bool interaction::IsChild(const interaction * puiIsChild) const
   {

      return m_pimpl->IsChild(puiIsChild);

   }


   bool interaction::IsDescendant(const interaction * puiIsDescendant) const
   {

      return m_pimpl->IsDescendant(puiIsDescendant);

   }


   ::user::elemental * interaction::get_wnd_elemental() const
   {

      return get_wnd();

   }


   ::user::interaction * interaction::get_wnd() const
   {

      if(m_pimpl != NULL)
      {

         ::user::interaction_impl * pwnd = m_pimpl->get_user_interaction_impl();

         if(pwnd != NULL)
            return (::user::interaction *) this;

      }

      if(m_pparent == NULL)
         return NULL;

      return m_pparent->get_wnd();

   }


   // returns -1 if not descendant
   int32_t interaction::get_descendant_level(::user::interaction * pui)
   {
      int32_t iLevel = 0;
      while(pui != NULL)
      {
         if(pui == this)
            return iLevel;
         pui = pui->GetParent();
         iLevel++;
      }
      return -1;
   }

   bool interaction::is_descendant(::user::interaction * pui,bool bIncludeSelf)
   {
      if(bIncludeSelf)
      {
         return get_descendant_level(pui) >= 0;
      }
      else
      {
         return get_descendant_level(pui) > 0;
      }
   }

   ::user::interaction * interaction::get_focusable_descendant(::user::interaction * pui)
   {
      int32_t iLevel = 0;
      if(pui == NULL)
      {
         pui = this;
      }
      else if(pui != this)
      {
         iLevel = get_descendant_level(pui);
         if(iLevel < 0)
         {
            return NULL;
         }
      }
      sp(::user::interaction) puiFocusable = NULL;
      int32_t iPreviousLevel = iLevel;
      while(true)
      {
         iPreviousLevel = iLevel;
         pui = pui->get_next(false,&iLevel);
         if(iLevel == 0)
            break;
         if(iLevel <= iPreviousLevel && puiFocusable != NULL)
            break;
         if(pui == NULL)
            break;
         if(pui->keyboard_focus_is_focusable())
            puiFocusable = pui;

      }
      return puiFocusable;
   }


   void interaction::_001OnTriggerMouseInside()
   {

      if(m_pimpl != NULL)
      {

         m_pimpl->_001OnTriggerMouseInside();

      }

   }


   bool interaction::update_data(bool bSaveAndValidate)
   {

      return true;

   }


   void interaction::_001OnClose(signal_details * pobj)
   {

      pobj->m_bRet = true;

      ShowWindow(SW_HIDE);

      DestroyWindow();

   }


   /*
   #ifdef METROWIN

   #ifdef METROWIN

   Agile<Windows::UI::Core::CoreWindow> interaction::get_os_window()
   {
   if (m_pimpl == NULL)
   {
   return System.m_window;
   }
   else
   return m_pimpl->get_os_window();
   }

   #endif

   #endif
   */


   string interaction::get_window_default_matter()
   {

      return "";

   }

   string interaction::get_window_icon_matter()
   {

      return get_window_default_matter();

   }

   uint32_t interaction::get_window_default_style()
   {
      return 0;
   }

   ::user::interaction::e_type interaction::get_window_type()
   {
      return type_window;
   }


   bool interaction::post_simple_command(e_simple_command ecommand,lparam lparam)
   {

      post_message(message_simple_command,(WPARAM)ecommand,lparam);

      return true;

   }

   void interaction::_001OnCommand(signal_details * pobj)
   {

      SCAST_PTR(::message::base,pbase,pobj);

      LRESULT lresult = 0;

      pbase->m_bRet = OnCommand(pbase);

      pbase->set_lresult(lresult);

   }


   void interaction::_001OnSimpleCommand(signal_details * pobj)
   {

      SCAST_PTR(::message::base,pbase,pobj);

      LRESULT lresult = 0;

      pbase->m_bRet = on_simple_command((e_simple_command)pbase->m_wparam,pbase->m_lparam,pbase->get_lresult());

      pbase->set_lresult(lresult);

   }


   /*

      bool interaction::_001SetSchema(::user::schema * pschema)
      {

         m_pschema = pschema;

         return true ;

      }


      bool interaction::_008SetSchema(::user::schema * pschema)
      {

         SendMessageToDescendants(message_set_schema,0,pschema,true,true);

         return true;

      }


      bool interaction::_009SetSchema(::user::schema * pschema)
      {

         _001SetSchema(pschema);

         _008SetSchema(pschema);

         return true;

      }


   */


   /*

      void interaction::_001OnSetSchema(signal_details * pobj)
      {

         SCAST_PTR(::message::base,pbase,pobj);

         LRESULT lresult = 0;

         _001SetSchema((::user::schema *) pbase->m_lparam);

         pbase->set_lresult(lresult);

      }

   */


   bool interaction::OnCommand(::message::base * pbase)
   {

      if(m_pimpl != NULL)
         return m_pimpl->OnCommand(pbase);

      return false;

   }

   bool interaction::OnNotify(::message::base * pbase)
   {

      if(m_pimpl != NULL)
         return m_pimpl->OnNotify(pbase);

      return false;

   }


   bool interaction::OnChildNotify(::message::base * pbase)
   {

      if(m_pimpl != NULL)
         return m_pimpl->OnChildNotify(pbase);

      return false;

   }


   bool interaction::on_simple_command(e_simple_command ecommand,lparam lparam,LRESULT & lresult)
   {

      UNREFERENCED_PARAMETER(lparam);
      UNREFERENCED_PARAMETER(lresult);

      if(::user::interaction_base::on_simple_command(ecommand,lparam,lresult))
         return true;

      switch(ecommand)
      {
      case simple_command_layout:
         {
            on_layout();
         }
         return true;
      default:
         break;
      }

      return false;

   }


   bool interaction::is_selected(::data::item * pitem)
   {

      UNREFERENCED_PARAMETER(pitem);

      return false;

   }



   bool interaction::_001HasCommandHandler(id id)
   {

      if(command_target_interface::_001HasCommandHandler(id))
         return true;

      if(GetParent() != NULL)
      {

         if(GetParent()->_001HasCommandHandler(id))
            return true;

      }

      return false;

   }

   bool interaction::track_popup_menu(::user::menu_base_item * pitem,int32_t iFlags)
   {

      point pt;

      Session.get_cursor_pos(&pt);

      return track_popup_menu(pitem,iFlags,pt);

   }


   bool interaction::track_popup_menu(::xml::node * lpnode,int32_t iFlags)
   {

      point pt;

      Session.get_cursor_pos(&pt);

      return track_popup_menu(lpnode,iFlags,pt);

   }


   bool interaction::track_popup_xml_matter_menu(const char * pszMatter,int32_t iFlags)
   {

      point pt;

      Session.get_cursor_pos(&pt);

      return track_popup_xml_matter_menu(pszMatter,iFlags,pt);

   }



   bool interaction::track_popup_menu(::user::menu_base_item * pitem,int32_t iFlags,signal_details * pobj)
   {

      SCAST_PTR(::message::mouse,pmouse,pobj);

      point pt = pmouse->m_pt;

      ScreenToClient(&pt);

      return track_popup_menu(pitem,iFlags,pt);

   }


   bool interaction::track_popup_menu(::xml::node * lpnode,int32_t iFlags,signal_details * pobj)
   {

      SCAST_PTR(::message::mouse,pmouse,pobj);

      point pt = pmouse->m_pt;

      ScreenToClient(&pt);

      return track_popup_menu(lpnode,iFlags,pt);

   }


   bool interaction::track_popup_xml_matter_menu(const char * pszMatter,int32_t iFlags,signal_details * pobj)
   {

      SCAST_PTR(::message::mouse,pmouse,pobj);

      point pt = pmouse->m_pt;

      //ScreenToClient(&pt);

      return track_popup_xml_matter_menu(pszMatter,iFlags,pt);

   }


   bool interaction::track_popup_menu(::user::menu_base_item * pitem,int32_t iFlags, POINT pt)
   {

      sp(::user::menu_base) pmenu = Application.alloc(System.type_info < ::user::menu_base >());

      pmenu->m_pitem = pitem;

      if(!pmenu->TrackPopupMenu(iFlags,pt,this))
      {

         pmenu.release();

         return false;

      }

      return true;

   }

   bool interaction::track_popup_menu(::xml::node * lpnode,int32_t iFlags, POINT pt)
   {

      sp(::user::menu_base) pmenu = Application.alloc(System.type_info < ::user::menu_base >());

      if(!pmenu->LoadMenu(lpnode))
      {

         pmenu.release();

         return false;

      }

      if(!pmenu->TrackPopupMenu(iFlags,pt,this))
      {

         pmenu.release();

         return false;

      }

      return true;

   }

   bool interaction::track_popup_xml_matter_menu(const char * pszMatter,int32_t iFlags, POINT pt)
   {

      sp(::user::menu_base) pmenu = Application.alloc(System.type_info < ::user::menu_base >());

      if(!pmenu->LoadXmlMenu(pszMatter))
      {

         pmenu.release();

         return false;

      }

      if(!pmenu->TrackPopupMenu(iFlags,pt,this))
      {

         pmenu.release();

         return false;

      }

      return true;

   }


   void interaction::WfiEnableFullScreen(bool bEnable)
   {

   }


   bool interaction::WfiIsFullScreenEnabled()
   {

      return false;

   }


   bool interaction::WfiClose()
   {

      return false;

   }


   bool interaction::Wfi(EAppearance eapperance)
   {

      return false;

   }


   bool interaction::WfiDock(EAppearance eapperance)
   {

      return false;

   }

   bool interaction::WfiRestore(bool bForceNormal)
   {

      UNREFERENCED_PARAMETER(bForceNormal);

      return false;

   }


   bool interaction::WfiMinimize()
   {

      return false;

   }


   bool interaction::WfiMaximize()
   {

      return false;

   }


   bool interaction::WfiFullScreen()
   {

      return false;

   }


   bool interaction::WfiUp()
   {

      return false;

   }


   bool interaction::WfiDown()
   {

      return false;

   }


   bool interaction::WfiNotifyIcon()
   {

      return false;

   }


   bool interaction::WfiIsMoving()
   {

      return false;

   }


   bool interaction::WfiIsSizing()
   {

      return false;

   }



   EAppearance interaction::get_appearance()
   {

      return m_eappearance;

   }


   bool interaction::set_appearance(EAppearance eappearance)
   {

      if(get_appearance() != get_appearance_before())
      {

         set_appearance_before(get_appearance());

      }

      m_eappearance = eappearance;

      return true;

   }


   bool interaction::set_appearance_before(EAppearance eappearance)
   {

      m_eappearanceBefore = eappearance;

      return true;

   }


   EAppearance interaction::get_appearance_before()
   {

      return m_eappearanceBefore;

   }


   void interaction::show_keyboard(bool bShow)
   {

      if(m_pimpl == NULL)
         return;

      m_pimpl->show_keyboard(bShow);

   }


   LRESULT interaction::call_message_handler(UINT message,WPARAM wparam,LPARAM lparam)
   {

      smart_pointer < ::message::base > spbase;

      spbase = get_base(message,wparam,lparam);

      //__trace_message("window_procedure",spbase);

      try
      {

         message_handler(spbase);

      }
      catch(::exit_exception &)
      {

         ::multithreading::post_quit(&System);

         return -1;

      }
      catch(const ::exception::exception & e)
      {

         if(!Application.on_run_exception((::exception::exception &) e))
         {

            ::multithreading::post_quit(&System);

            return -1;

         }

         return -1;

      }
      catch(::exception::base * pe)
      {

         m_pauraapp->process_window_procedure_exception(pe,spbase);

         pe->Delete();

      }
      catch(...)
      {

      }

      try
      {
         LRESULT lresult = spbase->get_lresult();

         return lresult;

      }
      catch(...)
      {

         return 0;

      }

   }


   void interaction::keep_alive(::object * pliveobject)
   {

      m_pauraapp->keep_alive();

      if(::get_thread() != NULL)
      {

         ::get_thread()->keep_alive();

      }

      if(pliveobject != NULL)
      {

         pliveobject->keep_alive();

      }

   }


   ::user::interaction * interaction::best_top_level_parent(LPRECT lprect)
   {

      sp(::user::interaction) pui = GetTopLevel();

      if(pui.is_null() || pui == this)
      {

         best_monitor(lprect);

      }
      else
      {

         pui->GetWindowRect(lprect);

      }

      return pui;

   }


   index interaction::best_monitor(LPRECT lprect,const RECT & rect,bool bSet,UINT uiSwpFlags,int_ptr iZOrder)
   {

      ::rect rectWindow;

      if(!::IsRectEmpty(&rect))
      {

         rectWindow = rect;

      }
      else
      {

         GetWindowRect(rectWindow);

      }

      ::rect rectNew;

      index iMatchingMonitor = -1;

      if(GetParent() != NULL)
      {

         GetParent()->GetClientRect(rectNew);

         iMatchingMonitor = 0;

      }
      else
      {

         iMatchingMonitor = Session.get_best_monitor(rectNew,rectWindow);

      }

      if(bSet && (!::IsRectEmpty(&rect) || iMatchingMonitor >= 0))
      {

#ifdef WINDOWSEX

         if(m_bWorkspaceFullScreen)
         {

            if(iMatchingMonitor >= 0 && rectNew.bottom > 2)
            {

               rectNew.bottom -= 2;

            }

         }


         synch_lock slUserMutex(m_pmutex);

         {

            keep < bool > keepLockWindowUpdate(&m_bLockWindowUpdate,true,false,true);

            keep < bool > keepIgnoreSizeEvent(&m_pimpl->m_bIgnoreSizeEvent,true,false,true);

            keep < bool > keepIgnoreMoveEvent(&m_pimpl->m_bIgnoreMoveEvent,true,false,true);

            keep < bool > keepDisableSaveWindowRect(&m_bEnableSaveWindowRect,false,m_bEnableSaveWindowRect,true);

            ::ShowWindow(get_handle(),SW_RESTORE);

         }

         SetWindowPos(iZOrder,rectNew,uiSwpFlags);

#elif defined WINDOWSEX

         WINDOWPLACEMENT wp;

         GetWindowPlacement(&wp);

         if(::IsZoomed(get_handle()) || ::IsIconic(get_handle()))
         {

            wp.showCmd = SW_RESTORE;

         }
         else
         {

            wp.showCmd = SW_SHOW;

         }

         wp.flags = 0;

         Session.monitor_to_wkspace(rect);

         wp.rcNormalPosition = rect;

         SetWindowPlacement(&wp);

#else


         SetWindowPos(iZOrder,rectNew,uiSwpFlags);

#endif

      }

      if(lprect != NULL)
      {

         *lprect = rectNew;

      }

      return iMatchingMonitor;

   }


   index interaction::best_wkspace(LPRECT lprect,const RECT & rect,bool bSet,UINT uiSwpFlags,int_ptr iZOrder)
   {

      ::rect rectWindow;

      if(!::IsRectEmpty(&rect))
      {

         rectWindow = rect;

      }
      else
      {

         GetWindowRect(rectWindow);

      }

      ::rect rectNew;

      index iMatchingMonitor = Session.get_best_monitor(rectNew,rectWindow);

      ::rect rectWkspace;

      Session.get_wkspace_rect(iMatchingMonitor,rectWkspace);

      if(bSet && (!::IsRectEmpty(&rect) || iMatchingMonitor >= 0))
      {
#ifdef WINDOWSEX

         if(iMatchingMonitor >= 0 && rectNew.bottom > 0)
         {

            if(rectWkspace.bottom > rectNew.bottom - 2)
            {

               rectWkspace.bottom = rectNew.bottom - 2;

            }

         }


         synch_lock slUserMutex(m_pmutex);

         {

            keep < bool > keepLockWindowUpdate(&m_bLockWindowUpdate,true,false,true);

            keep < bool > keepIgnoreSizeEvent(&m_pimpl->m_bIgnoreSizeEvent,true,false,true);

            keep < bool > keepIgnoreMoveEvent(&m_pimpl->m_bIgnoreMoveEvent,true,false,true);

            keep < bool > keepDisableSaveWindowRect(&m_bEnableSaveWindowRect,false,m_bEnableSaveWindowRect,true);

            ::ShowWindow(get_handle(),SW_MAXIMIZE);

            SetWindowPos(iZOrder,rectWkspace,uiSwpFlags);

         }

         //send_message(WM_SIZE);

         //send_message(WM_MOVE);

#elif defined WINDOWSEX

         ::rect rectWkspace;

         Session.get_wkspace_rect(iMatchingMonitor,rectWkspace);

         if(!::IsRectEmpty(&rect))
         {

            rect.intersect(lpcrect,rectWkspace);

         }
         else
         {

            rect = rectWkspace;

         }


         WINDOWPLACEMENT wp;

         GetWindowPlacement(&wp);

         wp.showCmd = SW_MAXIMIZE;

         wp.flags = 0;

         Session.monitor_to_wkspace(rect);

         wp.rcNormalPosition = rectWkspace;

         //wp.ptMaxPosition = rectWkspace.top_left();

         SetWindowPlacement(&wp);

#else


         SetWindowPos(iZOrder,rectWkspace,uiSwpFlags);

#endif

      }

      if(lprect != NULL)
      {

         *lprect = rectWkspace;

      }

      return iMatchingMonitor;

   }


   index interaction::make_zoneing(LPRECT lprect,const RECT & rect,bool bSet,::user::EAppearance * peappearance,UINT uiSwpFlags,int_ptr iZOrder)
   {

      if(peappearance == NULL || !::user::is_docking_appearance(*peappearance))
      {

         return best_zoneing(lprect,rect,bSet,peappearance,uiSwpFlags,iZOrder);

      }

      ::rect rectWindow;

      if(IsRectEmpty(&rect))
      {

         GetWindowRect(rectWindow);

      }
      else
      {

         rectWindow = rect;

      }

      ::rect rectNew;

      index iMatchingMonitor = Session.get_zoneing(rectNew,rectWindow,*peappearance);

      if(bSet & (!::IsRectEmpty(&rect) || iMatchingMonitor >= 0))
      {
#ifdef WINDOWSEX


         synch_lock slUserMutex(m_pmutex);

         set_appearance(*peappearance);

         {

            keep < bool > keepLockWindowUpdate(&m_bLockWindowUpdate,true,false,true);

            keep < bool > keepIgnoreSizeEvent(&m_pimpl->m_bIgnoreSizeEvent,true,false,true);

            keep < bool > keepIgnoreMoveEvent(&m_pimpl->m_bIgnoreMoveEvent,true,false,true);

            keep < bool > keepDisableSaveWindowRect(&m_bEnableSaveWindowRect,false,m_bEnableSaveWindowRect,true);

            ::ShowWindow(get_handle(),SW_RESTORE);

            SetWindowPos(iZOrder,rectNew,uiSwpFlags);

         }

         //send_message(WM_SIZE);

         //send_message(WM_MOVE);

#elif defined WINDOWSEX

         ::rect rectWkspace;

         Session.get_wkspace_rect(iMatchingMonitor,rectWkspace);

         if(!::IsRectEmpty(&rect))
         {

            rect.intersect(lpcrect,rectWkspace);

         }
         else
         {

            rect = rectWkspace;

         }


         WINDOWPLACEMENT wp;

         GetWindowPlacement(&wp);

         wp.showCmd = SW_MAXIMIZE;

         wp.flags = 0;

         Session.monitor_to_wkspace(rect);

         wp.rcNormalPosition = rectWkspace;

         //wp.ptMaxPosition = rectWkspace.top_left();

         SetWindowPlacement(&wp);

#else


         SetWindowPos(iZOrder,rectNew,uiSwpFlags);

#endif

      }

      if(lprect != NULL)
      {

         *lprect = rectNew;

      }

      return iMatchingMonitor;

   }

   index interaction::best_zoneing(LPRECT lprect,const RECT & rect,bool bSet,::user::EAppearance * peappearance, UINT uiSwpFlags,int_ptr iZOrder)
   {

      ::user::EAppearance eappearance;

      if(peappearance == NULL)
      {

         peappearance = &eappearance;

      }

      *peappearance = get_appearance();

      ::rect rectWindow;

      if(!::IsRectEmpty(&rect))
      {

         rectWindow = rect;

      }
      else
      {

         GetWindowRect(rectWindow);

      }

      ::rect rectNew;

      index iMatchingMonitor = Session.get_best_zoneing(peappearance,rectNew,rectWindow);

      if(bSet && (!::IsRectEmpty(&rect) || iMatchingMonitor >= 0))
      {
#ifdef WINDOWSEX


         synch_lock slUserMutex(m_pmutex);

         set_appearance(*peappearance);

         {

            keep < bool > keepLockWindowUpdate(&m_bLockWindowUpdate,true,false,true);

            keep < bool > keepIgnoreSizeEvent(&m_pimpl->m_bIgnoreSizeEvent,true,false,true);

            keep < bool > keepIgnoreMoveEvent(&m_pimpl->m_bIgnoreMoveEvent,true,false,true);

            keep < bool > keepDisableSaveWindowRect(&m_bEnableSaveWindowRect,false,m_bEnableSaveWindowRect,true);

            ::ShowWindow(get_handle(),SW_RESTORE);

            SetWindowPos(iZOrder,rectNew,uiSwpFlags);

         }

         //send_message(WM_SIZE);

         //send_message(WM_MOVE);

#elif defined WINDOWSEX

         ::rect rectWkspace;

         Session.get_wkspace_rect(iMatchingMonitor,rectWkspace);

         if(!::IsRectEmpty(&rect))
         {

            rect.intersect(lpcrect,rectWkspace);

         }
         else
         {

            rect = rectWkspace;

         }


         WINDOWPLACEMENT wp;

         GetWindowPlacement(&wp);

         wp.showCmd = SW_MAXIMIZE;

         wp.flags = 0;

         Session.monitor_to_wkspace(rect);

         wp.rcNormalPosition = rectWkspace;

         //wp.ptMaxPosition = rectWkspace.top_left();

         SetWindowPlacement(&wp);

#else


         SetWindowPos(iZOrder,rectNew,uiSwpFlags);

#endif

      }

      if(lprect != NULL)
      {

         *lprect = rectNew;

      }

      return iMatchingMonitor;

   }


   index interaction::good_restore(LPRECT lprect,const RECT & rect,bool bSet,UINT uiSwpFlags,int_ptr iZOrder)
   {

      ::rect rectWindow;

      if(!::IsRectEmpty(&rect))
      {

         rectWindow = rect;

      }
      else
      {

         GetWindowRect(rectWindow);

      }

      ::rect rectNew;

      index iMatchingMonitor = Session.get_good_restore(rectNew,rectWindow, this);

      if(bSet && (!::IsRectEmpty(&rect) || iMatchingMonitor >= 0))
      {
#ifdef WINDOWSEX

         //synch_lock slUserMutex(m_pmutex);

         {

            if (GetExStyle() & WS_EX_LAYERED)
            {

               m_pimpl->m_bShowWindow = true;

               m_pimpl->m_iShowWindow = SW_RESTORE;

               SetWindowPos(iZOrder, rectNew, uiSwpFlags);

            }
            else
            {

               keep < bool > keepLockWindowUpdate(&m_bLockWindowUpdate, true, false, true);

               keep < bool > keepIgnoreSizeEvent(&m_pimpl->m_bIgnoreSizeEvent, true, false, true);

               keep < bool > keepIgnoreMoveEvent(&m_pimpl->m_bIgnoreMoveEvent, true, false, true);

               keep < bool > keepDisableSaveWindowRect(&m_bEnableSaveWindowRect, false, m_bEnableSaveWindowRect, true);

               ::ShowWindow(get_handle(), SW_RESTORE);

               SetWindowPos(iZOrder, rectNew, uiSwpFlags);

            }


         }

         //send_message(WM_SIZE);

         //send_message(WM_MOVE);


#elif defined WINDOWSEX

         WINDOWPLACEMENT wp;

         GetWindowPlacement(&wp);

         if(::IsZoomed(get_handle()) || ::IsIconic(get_handle()))
         {

            wp.showCmd = SW_RESTORE;

         }
         else
         {

            wp.showCmd = SW_SHOW;

         }

         wp.flags = 0;

         Session.monitor_to_wkspace(rect);

         wp.rcNormalPosition = rect;

         SetWindowPlacement(&wp);

#else


         SetWindowPos(iZOrder,rectNew,uiSwpFlags);

#endif

      }

      if(lprect != NULL)
      {

         *lprect = rectNew;

      }

      return iMatchingMonitor;

   }


   index interaction::good_iconify(LPRECT lprect,const RECT & rect,bool bSet,UINT uiSwpFlags,int_ptr iZOrder)
   {

      ::rect rectWindow;

      if(!::IsRectEmpty(&rect))
      {

         rectWindow = rect;

      }
      else
      {

         GetWindowRect(rectWindow);

      }

      ::rect rectNew;

      index iMatchingMonitor = Session.get_good_iconify(&rectNew,rectWindow);

      if(bSet && (!::IsRectEmpty(&rect) || iMatchingMonitor >= 0))
      {

#ifdef WINDOWSEX

         synch_lock slUserMutex(m_pmutex);

         {

            keep < bool > keepLockWindowUpdate(&m_bLockWindowUpdate,true,false,true);

            keep < bool > keepIgnoreSizeEvent(&m_pimpl->m_bIgnoreSizeEvent,true,false,true);

            keep < bool > keepIgnoreMoveEvent(&m_pimpl->m_bIgnoreMoveEvent,true,false,true);

            keep < bool > keepDisableSaveWindowRect(&m_bEnableSaveWindowRect,false,m_bEnableSaveWindowRect,true);
            uiSwpFlags &= ~SWP_NOZORDER;
            if (GetExStyle() & WS_EX_LAYERED)
            {
               
               m_pimpl->m_bShowWindow = true;

               //if (uiSwpFlags & SWP_NOACTIVATE)
               //{
               //   uiSwpFlags &= ~SWP_FRAMECHANGED;
               //   m_pimpl->m_iShowWindow = SW_SHOWMINNOACTIVE;

               //}
               //else
               {
                  uiSwpFlags &= ~SWP_FRAMECHANGED;
                  uiSwpFlags &= ~SWP_NOACTIVATE;
                  uiSwpFlags &= ~SWP_NOZORDER;
                  m_pimpl->m_iShowWindow = SW_MINIMIZE;

               }

            }
            else
            {

               ::ShowWindow(get_handle(), SW_MINIMIZE);

            }

         }

         //SetWindowPos(iZOrder,rectNew,uiSwpFlags);


#elif defined WINDOWSEX

         WINDOWPLACEMENT wp;

         GetWindowPlacement(&wp);

         wp.showCmd = SW_MINIMIZE;

         wp.flags = WPF_SETMINPOSITION;

         Session.monitor_to_wkspace(rect);

         //wp.rcNormalPosition = rect;

         wp.ptMinPosition = rect.top_left();

         SetWindowPlacement(&wp);

#else


         SetWindowPos(iZOrder,rectNew,uiSwpFlags);

#endif

      }

      if(lprect != NULL)
      {

         *lprect = rectNew;

      }

      return iMatchingMonitor;

   }


   index interaction::good_move(LPRECT lprect,const RECT & rect,UINT uiSwpFlags,int_ptr iZOrder)
   {

      ::rect rectWindow;

      if(!::IsRectEmpty(&rect))
      {

         rectWindow = rect;

      }
      else
      {

         GetWindowRect(rectWindow);

      }

      ::rect rectNew;

      index iMatchingMonitor = Session.get_good_move(rectNew,rectWindow, this);

      if(!::IsRectEmpty(&rect) || iMatchingMonitor >= 0)
      {

         SetWindowPos(iZOrder,rectNew,uiSwpFlags);

      }

      if(lprect != NULL)
      {

         *lprect = rectNew;

      }

      return iMatchingMonitor;

   }

   bool interaction::SetWindowPos(int_ptr z,int32_t x,int32_t y,int32_t cx,int32_t cy,UINT nFlags)
   {

      if (m_pimpl.is_null())
      {

         return false;

      }

      return m_pimpl->SetWindowPos(z, x, y, cx, cy, nFlags);

   }


   bool interaction::ClientToScreen(LPRECT lprect)
   {

      if(m_pimpl == NULL)
      {

         return false;

      }

      if(!m_pimpl->ClientToScreen(lprect))
      {

         return false;

      }

      return true;

   }


   bool interaction::ClientToScreen(LPPOINT lppoint)
   {

      if(m_pimpl == NULL)
      {

         return false;

      }

      if(!m_pimpl->ClientToScreen(lppoint))
      {

         return false;

      }

      return true;

   }


   bool interaction::ClientToScreen(RECT64 * lprect)
   {

      if(m_pimpl == NULL)
      {

         return false;

      }

      if(!m_pimpl->ClientToScreen(lprect))
      {

         return false;

      }

      return true;

   }


   bool interaction::ClientToScreen(POINT64 * lppoint)
   {

      if(m_pimpl == NULL)
      {

         return false;

      }

      if(!m_pimpl->ClientToScreen(lppoint))
      {

         return false;

      }

      return true;

   }


   bool interaction::ScreenToClient(LPRECT lprect)
   {

      if(m_pimpl == NULL)
      {

         return false;

      }

      if(!m_pimpl->ScreenToClient(lprect))
      {

         return false;

      }

      return true;

   }


   bool interaction::ScreenToClient(LPPOINT lppoint)
   {

      if(m_pimpl == NULL)
      {

         return false;

      }

      if(!m_pimpl->ScreenToClient(lppoint))
      {

         return false;

      }

      return true;

   }


   bool interaction::ScreenToClient(RECT64 * lprect)
   {

      if(m_pimpl == NULL)
      {

         return false;

      }

      if(!m_pimpl->ScreenToClient(lprect))
      {

         return false;

      }

      return true;

   }


   bool interaction::ScreenToClient(POINT64 * lppoint)
   {

      if(m_pimpl == NULL)
      {

         return false;

      }

      if(!m_pimpl->ScreenToClient(lppoint))
      {

         return false;

      }

      return true;

   }


   bool interaction::GetClientRect(RECT64 * lprect)
   {

      if(m_pimpl == NULL)
      {

         return false;

      }

      if(!m_pimpl->GetClientRect(lprect))
      {

         return false;

      }

      return true;

   }


   id interaction::GetDlgCtrlId() const
   {

      return m_id;

   }


   id interaction::SetDlgCtrlId(id id)
   {

      m_id = id;

      return m_id;

   }


   void interaction::_001OnBaseWndGetProperty(signal_details * pobj)
   {
      SCAST_PTR(::message::base,pbase,pobj);
      pbase->set_lresult(_001BaseWndGetProperty((EProperty)pbase->m_wparam,pbase->m_lparam));
   }


   LRESULT interaction::_001BaseWndGetProperty(EProperty eprop,LPARAM lparam)
   {
      switch(eprop)
      {
      case PropertyBaseWndInterface:
      case PropertyDrawBaseWndInterface:
         {
            const interaction_base ** ppinterface = (const interaction_base **)lparam;
            *ppinterface = this;
         }
         return 1;
      default:
         break;
      }
      return 0;
   }



   bool interaction::get_rect_normal(LPRECT lprect)
   {

      return m_pimpl->get_rect_normal(lprect);

   }

   void interaction::offset_viewport_offset(int x,int y)
   {

      point ptOffset = get_viewport_offset();

      set_viewport_offset(ptOffset.x + x,ptOffset.y + y);

   }


   void interaction::offset_viewport_offset_x(int x)
   {

      offset_viewport_offset(x,0);

   }


   void interaction::offset_viewport_offset_y(int y)
   {

      offset_viewport_offset(0,y);

   }

   void interaction::set_viewport_offset(int x,int y)
   {
      m_ptScrollPassword1.x = x;

      m_ptScrollPassword1.y = y;

      on_change_viewport_offset();

   }


   void interaction::set_viewport_offset_x(int x)
   {

      set_viewport_offset(x,get_viewport_offset().y);

   }


   void interaction::set_viewport_offset_y(int y)
   {

      set_viewport_offset(get_viewport_offset().x, y);

   }


   void interaction::on_change_viewport_offset()
   {

      RedrawWindow();

   }


   point interaction::get_viewport_offset()
   {

      return m_ptScrollPassword1;

   }


   size interaction::get_total_size()
   {

      ::rect rectClient;

      GetClientRect(rectClient);

      return rectClient.size();

   }


   void interaction::on_change_view_size()
   {

   }


   size interaction::get_page_size()
   {

      ::rect rectClient;

      GetClientRect(rectClient);

      return rectClient.size();

   }

   point interaction::get_ascendant_viewport_offset()
   {

      sp(::user::interaction) puser = GetParent();

      point pt(0,0);

      while(puser.is_set())
      {

         pt += puser->get_viewport_offset();

         puser = puser->GetParent();

      }

      return pt;

   }


   void interaction::get_margin_rect(LPRECT lprectMargin)
   {

      lprectMargin->left = 0;
      lprectMargin->top = 0;
      lprectMargin->right = 0;
      lprectMargin->bottom = 0;

   }

   int interaction::get_final_x_scroll_bar_width()
   {

      return 0;

   }

   int interaction::get_final_y_scroll_bar_width()
   {

      return 0;

   }


   point interaction::get_parent_viewport_offset()
   {

      ::user::interaction * puser = GetParent();

      if (puser == NULL)
      {

         return point(0, 0);

      }

      return puser->get_viewport_offset();

   }





   ::user::schema * interaction::get_parent_user_schema()
   {

      ::user::interaction * puiParent = GetParent();

      if(puiParent == NULL)
         return NULL;

      return puiParent;

   }


   CLASS_DECL_BASE bool is_docking_appearance(::user::EAppearance eappearance)
   {
      return eappearance == ::user::AppearanceLeft
             || eappearance == ::user::AppearanceTop
             || eappearance == ::user::AppearanceRight
             || eappearance == ::user::AppearanceBottom
             || eappearance == ::user::AppearanceTopLeft
             || eappearance == ::user::AppearanceTopRight
             || eappearance == ::user::AppearanceBottomRight
             || eappearance == ::user::AppearanceBottomLeft;
   }

   void interaction::_001OnShowWindow(signal_details * pobj)
   {

      SCAST_PTR(::message::show_window, pshowwindow, pobj);

      if(!pshowwindow->m_bShow)
      {

         user_interaction_on_hide();

      }

      RedrawWindow();

   }


   void interaction::_001OnLButtonDown(signal_details * pobj)
   {

//      SCAST_PTR(::message::mouse,pmouse,pobj);

      pobj->previous();

      try
      {

         if(keyboard_focus_is_focusable())
         {

            Session.set_keyboard_focus(this);

            Session.user()->set_mouse_focus_LButtonDown(this);

         }
         else
         {

            output_debug_string("::user::interaction::_001OnLButtonDown Not Focusable\n");

            Session.set_keyboard_focus(NULL);

            Session.user()->set_mouse_focus_LButtonDown(NULL);

         }


      }
      catch(...)
      {

      }

   }



//   void interaction::transfer_from(::aura::timer_array & ta, interaction * pui)
//   {
//
//
//      single_lock sl(&ta.m_mutex,TRUE);
//
//      smart_pointer_array < ::aura::timer_item > timera;
//
////      ta.detach(timera,this);
//
//      set_timer(timera);
//
//   }

   /*

      sp(::message::base) interaction::peek_message(LPMESSAGE lpmsg,::user::interaction * pwnd,UINT wMsgFilterMin,UINT wMsgFilterMax,UINT wRemoveMsg)
      {

         if(!::PeekMessage(lpmsg,pwnd->get_safe_handle(),wMsgFilterMin,wMsgFilterMax,wRemoveMsg))
            return NULL;

         return get_base(lpmsg,pwnd);

      }


      sp(::message::base) interaction::get_message(LPMESSAGE lpmsg,::user::interaction * pwnd,UINT wMsgFilterMin,UINT wMsgFilterMax)
      {

         if(!::GetMessage(lpmsg,pwnd->get_safe_handle(),wMsgFilterMin,wMsgFilterMax))
            return NULL;

         return get_base(lpmsg,pwnd);

      }


      sp(::message::base) interaction::peek_message(::user::interaction * pwnd,UINT wMsgFilterMin,UINT wMsgFilterMax,UINT wRemoveMsg)
      {
         MESSAGE msg;
         return peek_message(&msg,pwnd,wMsgFilterMin,wMsgFilterMax,wRemoveMsg);
      }


      sp(::message::base) interaction::get_message(::user::interaction * pwnd,UINT wMsgFilterMin,UINT wMsgFilterMax)
      {
         MESSAGE msg;
         return get_message(&msg,pwnd,wMsgFilterMin,wMsgFilterMax);
      }


      sp(::message::base) interaction::peek_message(LPMESSAGE lpmsg,::user::interaction * pwnd,UINT wMsgFilterMin,UINT wMsgFilterMax,UINT wRemoveMsg)
      {
         if(!::PeekMessage(lpmsg,pwnd->get_safe_handle(),wMsgFilterMin,wMsgFilterMax,wRemoveMsg))
            return NULL;
         return get_base(lpmsg,pwnd);
      }

      sp(::message::base) interaction::get_message(LPMESSAGE lpmsg,::user::interaction * pwnd,UINT wMsgFilterMin,UINT wMsgFilterMax)
      {
         if(!::GetMessage(lpmsg,pwnd->get_safe_handle(),wMsgFilterMin,wMsgFilterMax))
            return NULL;
         return get_base(lpmsg,pwnd);
      }

      sp(::message::base) interaction::peek_message(::user::interaction * pwnd,UINT wMsgFilterMin,UINT wMsgFilterMax,UINT wRemoveMsg)
      {
         MESSAGE msg;
         return peek_message(&msg,pwnd,wMsgFilterMin,wMsgFilterMax,wRemoveMsg);
      }


      sp(::message::base) interaction::get_message(::user::interaction * pwnd,UINT wMsgFilterMin,UINT wMsgFilterMax)
      {
         MESSAGE msg;
         return get_message(&msg,pwnd,wMsgFilterMin,wMsgFilterMax);
      }

      */

   sp(::message::base) interaction::get_base(UINT uiMessage,WPARAM wparam,LPARAM lparam)
   {

//      ::user::interaction * pwnd = this;
      sp(::message::base) pbase;
      ::message::e_prototype eprototype = ::message::PrototypeNone;
      //if(oswindow != NULL)
      {
         eprototype = ::message::dispatch::GetMessagePrototype(uiMessage,0);
      }
      switch(eprototype)
      {
      case ::message::PrototypeNone:
         {
            pbase = canew(::message::base(get_app()));
         }
         break;
      case ::message::PrototypeCreate:
         {
            pbase = canew(::message::create(get_app()));
         }
         break;
      case ::message::PrototypeNcActivate:
         {
            pbase = canew(::message::nc_activate(get_app()));
         }
         break;
      case ::message::PrototypeKey:
         {
            pbase = canew(::message::key(get_app()));
         }
         break;
      case ::message::PrototypeTimer:
         {
            throw simple_exception(get_app(),"do not use WM_TIMER or Windows SetTimer/KillTimer");
            //           pbase = canew(::message::timer(get_app()));
         }
         break;
      case ::message::PrototypeShowWindow:
         {
            pbase = canew(::message::show_window(get_app()));
         }
         break;
      case ::message::PrototypeSetCursor:
         {
            pbase = canew(::message::set_cursor(get_app()));
         }
         break;
      case ::message::PrototypeNcHitTest:
         {
            pbase = canew(::message::nchittest(get_app()));
         }
         break;
      case ::message::PrototypeMove:
         {
            pbase = canew(::message::move(get_app()));
         }
         break;
      case ::message::PrototypeEraseBkgnd:
         {
            pbase = canew(::message::erase_bkgnd(get_app()));
         }
         break;
      case ::message::PrototypeScroll:
         {
            pbase = canew(::message::scroll(get_app()));
         }
         break;
      case ::message::PrototypeSetFocus:
         {
            pbase = canew(::message::set_focus(get_app()));
         }
         break;
#if !defined(METROWIN) && !defined(LINUX) && !defined(APPLEOS)
      case ::message::PrototypeWindowPos:
         {
            pbase = canew(::message::window_pos(get_app()));
         }
         break;
      case ::message::PrototypeNcCalcSize:
         {
            pbase = canew(::message::nc_calc_size(get_app()));
         }
         break;
#endif
      case ::message::PrototypeMouse:
         {
            pbase = canew(::message::mouse(get_app()));
         }
         break;
      case ::message::PrototypeMouseWheel:
         {
            pbase = canew(::message::mouse_wheel(get_app()));
         }
         break;
      case ::message::PrototypeSize:
         {
            pbase = canew(::message::size(get_app()));
         }
         break;
      case ::message::PrototypeActivate:
         {
            pbase = canew(::message::activate(get_app()));
         }
         break;
      default:
         {
            pbase = canew(::message::base(get_app()));
         }
         break;
      }
      if(pbase == NULL)
         return NULL;

      pbase->set(this,uiMessage,wparam,lparam);

      return pbase;

   }


   window_graphics * interaction::get_window_graphics()
   {

      if(m_pimpl == NULL)
         return ::user::interaction_base::get_window_graphics();

      return m_pimpl->get_window_graphics();

   }


   bool interaction::is_composite()
   {

      if(m_pimpl == NULL)
         return true; // optimistic response, assume always true alpha blendable

      return m_pimpl->is_composite();

   }

   bool interaction::get_window_minimum_size(::size & sizeMin)
   {

      sizeMin.cx = 0;
      sizeMin.cy = 0;

      return true;

   }

   // up down target

   //UpDownTarget::UpDownTarget()
   //{
   //   m_eupdowntargettype = type_normal_frame;
   //}

   void interaction::UpDownTargetAttach(::user::wndfrm::frame::WorkSetUpDownInterface * pupdown)
   {

      bool bAttached = false;

      pupdown->m_eupdown = updown_down;

      try
      {

         if (OnUpDownTargetAttach(pupdown))
         {

            bAttached = true;

         }

      }
      catch(...)
      {

      }

      if(!bAttached)
      {

         pupdown->m_eupdown = updown_none;

      }

   }


   void interaction::UpDownTargetDetach(::user::wndfrm::frame::WorkSetUpDownInterface * pupdown)
   {

      bool bDetached = false;

      pupdown->m_eupdown = updown_up;

      try
      {

         if (OnUpDownTargetDetach(pupdown))
         {

            bDetached = true;

         }

      }
      catch(...)
      {

      }

      if(!bDetached)
      {

         pupdown->m_eupdown = updown_none;

      }

   }


   bool interaction::OnUpDownTargetAttach(::user::wndfrm::frame::WorkSetUpDownInterface * pupdown)
   {

      return false;

   }


   bool interaction::OnUpDownTargetDetach(::user::wndfrm::frame::WorkSetUpDownInterface * pupdown)
   {

      return false;

   }


   string interaction::calc_data_id()
   {

      string str;

      if(Application.m_dataid.m_id.is_empty())
      {

         str = Application.m_strAppId;

      }
      else
      {

         str = Application.m_dataid.m_id;

      }

      if(str.has_char())
      {

         str += ".";

      }

      return str + m_id;

   }


   void interaction::on_update(::user::impact * pSender,LPARAM lHint,::object* pHint)
   {

      UNREFERENCED_PARAMETER(pSender);
      UNREFERENCED_PARAMETER(lHint);
      UNREFERENCED_PARAMETER(pHint);

   }


   void interaction::keyboard_focus_OnKeyDown(signal_details * pobj)
   {

      SCAST_PTR(::message::key,pkey,pobj);

      if(pkey->m_ekey == ::user::key_tab)
      {

         control_event ev;

         ev.m_puie                  = dynamic_cast <::user::interaction * > (this);
         ev.m_eevent                = ::user::event_tab_key;
         ev.m_actioncontext         = ::action::source_user;

         GetParent()->BaseOnControlEvent(&ev);

         BaseOnControlEvent(&ev);

      }

   }



   bool interaction::get_child(sp(::user::interaction) & pui)
   {

      synch_lock sl(m_pmutex);

      return m_uiptraChild.get_child(pui);

   }

   bool interaction::rget_child(sp(::user::interaction) & pui)
   {

      synch_lock sl(m_pmutex);

      return m_uiptraChild.rget_child(pui);

   }



   ::user::interaction * interaction::get_focus_ui()
   {

      if(m_pimpl == NULL)
         return NULL;

      return m_pimpl->get_focus_ui();

   }


   void interaction::set_focus_guie(::user::interaction * pguie)
   {

      if(m_pimpl == NULL)
         return;

      m_pimpl->set_focus_guie(pguie);

   }


   bool interaction::is_descendant_of(::user::interaction * puiAscendantCandidate)
   {

      ::user::interaction * pui = GetParent();

      while(pui != NULL)
      {

         if(pui == puiAscendantCandidate)
            return true;

         pui = pui->GetParent();

      }

      return false;

   }


   bool interaction::kick_queue()
   {

      post_message(::message::message_null);

      return true;

   }


   bool interaction::has_action_hover()
   {

      return false;

   }


   bool interaction::has_text_input()
   {

      return false;

   }


   COLORREF interaction::get_action_hover_border_color()
   {

      return 0;

   }


   bool interaction::show_tooltip(const string & str,bool bError)
   {

      if(m_ptooltip.is_null())
      {

         m_ptooltip = canew(tooltip(get_app()));

         m_ptooltip->create_window_ex(WS_EX_LAYERED | WS_EX_TOOLWINDOW,NULL,"tooltip",0,::null_rect(),NULL);

      }

      get_tooltip()->m_str = str;

      get_tooltip()->m_bError = bError;

      get_tooltip()->CalcSize();

      if(!layout_tooltip(true))
      {

         return false;

      }

      return true;

   }

   void interaction::ExitHelpMode()
   {

   }


   void interaction::BaseOnControlEvent(::message::base * pbase)
   {

      ((::user::control_event *) pbase->m_lparam.m_lparam)->m_bRet = BaseOnControlEvent((::user::control_event *) pbase->m_lparam.m_lparam);

   }


   bool interaction::layout_tooltip(bool bForceShow)
   {

      if(m_ptooltip.is_null())
      {

         return true;

      }

      if(m_ptooltip->IsWindowVisible() && get_wnd() != GetActiveWindow())
      {

         hide_tooltip();

         return false;

      }

      if(!bForceShow && !m_ptooltip->IsWindowVisible())
      {

         return true;

      }

      ::size sizeTooltip = get_tooltip()->m_size;

      ::rect rectThisWindow;

      GetWindowRect(rectThisWindow);

      ::rect rectWindow;

      ::rect rectMonitor;

      Session.get_best_monitor(rectMonitor,rectThisWindow);

      rectWindow = rectThisWindow;

      rectWindow.SetBottomRightSize(sizeTooltip);

      rectWindow.offset(0,- sizeTooltip.cy);

      if(rectWindow.top < rectMonitor.top)
      {

         rectWindow.move_to_y(rectThisWindow.bottom);

      }

      if(rectWindow.right > rectMonitor.right)
      {

         rectWindow.move_to_x(rectMonitor.right - sizeTooltip.cx - rectThisWindow.height());

      }

      m_ptooltip->SetWindowPos(ZORDER_TOPMOST,rectWindow,SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOACTIVATE);

      return true;

   }


   bool interaction::hide_tooltip()
   {

      if(m_ptooltip.is_null())
      {

         return true;

      }

      m_ptooltip->ShowWindow(SW_HIDE);

      return true;

   }


   bool interaction::frame_is_transparent()
   {

      return false;

   }

   bool interaction::WfiToggleTransparentFrame()
   {

      return false;

   }


   double interaction::get_alpha()
   {

      if(m_palphasource == NULL)
      {

         return 1.0;

      }
      else
      {

         return m_palphasource->get_alpha(this);

      }

   }


   bool interaction::has_pending_graphical_update()
   {

#ifdef APPLE_IOS

      {

      synch_lock sl(m_pmutex);

      for(index i = 0; i < m_uiptraChild.get_size(); i++)
      {

         ::user::interaction * pui = m_uiptraChild[i];

         sl.unlock();

         if(pui->has_pending_graphical_update())
         {

            return true;

         }

         sl.lock();


      }

      }

#endif

      if (m_bRedrawOnVisible && IsWindowVisible())
      {

         return true;

      }

      if (m_bRedraw)
      {

         return true;

      }

      if (m_bCursorRedraw)
      {

         point ptCursor;

         Session.get_cursor_pos(ptCursor);

         if (m_ptCursor != ptCursor)
         {

            if (_001IsPointInside(ptCursor))
            {

               return true;

            }

         }

      }

      if (m_pimpl->has_pending_graphical_update())
      {

         return true;

      }

      return false;

   }


   void interaction::transparent_mouse_events()
   {

      m_bTransparentMouseEvents = true;

      ::fork(get_app(), [=]()
      {

         point pt;

         while (get_thread_run())
         {

            defer_notify_mouse_move(pt);

            Sleep(5);

         }

      });

   }


   void interaction::defer_notify_mouse_move(point & ptLast)
   {

      if (Session.get_capture() != NULL)
      {

         return;

      }

      point ptCurrent;

      Session.get_cursor_pos(ptCurrent);

      if (ptCurrent != ptLast)
      {

         ptLast = ptCurrent;

         bool bPointInside = _001IsPointInside(ptCurrent);

         if (bPointInside || m_bMouseHover)
         {

#if !defined(LINUX)

            get_wnd()->ScreenToClient(ptCurrent);

#endif

            if (bPointInside)
            {

               m_bMouseHover = true;

               get_wnd()->message_call(WM_MOUSEMOVE, 0, ptCurrent);

            }
            else
            {

               m_bMouseHover = false;

               get_impl()->mouse_hover_remove(this);

               send_message(WM_MOUSELEAVE);

               RedrawWindow();

            }


         }

      }

   }


   void interaction::defer_notify_mouse_move()
   {

      if (Session.get_capture() != NULL)
      {

         return;

      }

      point ptCurrent;

      Session.get_cursor_pos(ptCurrent);

#if !defined(LINUX)

      get_wnd()->ScreenToClient(ptCurrent);

#endif

      get_wnd()->send_message(WM_MOUSEMOVE, 0, ptCurrent);

   }


   double alpha_source::get_alpha(::user::interaction * puiTarget)
   {

      return 1.0;

   }


   void alpha_source::on_alpha_target_initial_frame_position()
   {


   }


   void interaction::on_after_graphical_update()
   {

      ::user::interaction_base::on_after_graphical_update();

      m_bRedraw = false;

      Session.get_cursor_pos(m_ptCursor);

   }


   void interaction::_001OnDeiconify(::user::EAppearance eappearance)
   {

      WfiRestore();

   }
   
   
   void interaction::on_setting_changed(::aura::e_setting esetting)
   {
      
      
   }

   
   bool interaction::_001OnClick(uint_ptr nFlag, point point)
   {

      return false;

   }


   bool interaction::_001OnRightClick(uint_ptr nFlag, point point)
   {

      return false;

   }


} // namespace user

















