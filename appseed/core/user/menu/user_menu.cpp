//#include "framework.h"
//#include "core/user/user/user.h"


namespace user
{


   const int32_t BaseWndMenuTimer = 500;
   const int32_t BaseWndMenuCmdUi = 117;


   menu::menu():
      menu(get_app())
   {

   }

   menu::menu(::aura::application * papp) :
      object(papp),
      menu_base(papp),
      m_itemClose(papp)
   {

      m_itemClose.m_pbase = this;
      m_bAutoDelete        = true;
      m_pschema            = NULL;
      //m_pitem              = new ::user::menu_item(papp);
      m_bOwnItem           = true;
      m_oswindowParent         = NULL;
      m_pmenuParent        = NULL;
      m_psubmenu           = NULL;
      m_iHoverSubMenu      = -1;

   }


   menu::menu(::aura::application * papp,sp(::user::menu_item) pitem):
      object(papp),
      menu_base(papp),
      m_itemClose(papp)
   {

      m_iHoverSubMenu      = -1;
      m_oswindowParent         = NULL;
      m_pmenuParent        = NULL;
      m_psubmenu           = NULL;
      m_bAutoDelete        = true;
      m_pschema            = NULL;
      m_pitem              = pitem;
      m_bOwnItem           = false;

   }


   menu::~menu()
   {

   }


   ::user::menu_item * menu::GetSubMenu(int32_t i)
   {

      return get_item()->m_spitema->element_at(i);

   }


   bool menu::TrackPopupMenu(int32_t iFlags, POINT pt, ::user::interaction * oswindowParent)
   {

      ASSERT(oswindowParent != NULL);

      if (!menu_base::TrackPopupMenu(iFlags, pt, oswindowParent))
         return false;

      return _TrackPopupMenu(iFlags, pt, oswindowParent, NULL);

   }


   bool menu::_TrackPopupMenu(int32_t iFlags, POINT pt, sp(::user::interaction) oswindowParent, menu * pmenuParent)
   {

      UNREFERENCED_PARAMETER(iFlags);

      m_oswindowParent   = oswindowParent;

      m_pmenuParent  = pmenuParent;

      LPVOID lpvoid = NULL;

      if (!create_window_ex(WS_EX_LAYERED | WS_EX_TOOLWINDOW, NULL, NULL, 0, rect(0, 0, 0, 0), Session.get_view(), id(), lpvoid))
      {

         return false;

      }

      SetOwner(oswindowParent);

      if (!m_itemClose.m_button.create_window(null_rect(), this, ChildIdClose))
      {

         return false;

      }

      m_itemClose.m_button.set_window_text("r");

      m_itemClose.m_button.set_stock_icon(stock_icon_close);

      m_itemClose.m_button.m_pschema = m_pschema->m_pschemaSysMenuCloseButton;

      layout_menu(pt);

      SetActiveWindow();

      SetForegroundWindow();

      BringWindowToTop();

      return true;

   }


   void menu::layout_menu(point pt)
   {

      if(GetParent() != NULL)
      {

         GetParent()->ScreenToClient(&pt);

      }

      m_ptTrack = pt;

      ::draw2d::memory_graphics pgraphics(allocer());

      pgraphics->SelectObject(m_pschema->m_pschemaItemButton->m_pfont);

      size size = pgraphics->GetTextExtent(unitext("XXXMMMÁÇg"));

      int32_t iMaxHeight = size.cy;

      int32_t iMaxWidth = size.cx;

      m_iCheckBoxSize = iMaxHeight;

      m_iHeaderHeight = size.cy;

      sp(::user::menu_item) pitem = get_item();

      sp(::user::menu_item_ptra) spitema = pitem->m_spitema;

      for (int32_t i = 0; i < spitema->get_size(); i++)
      {
         
         string strButtonText = spitema->element_at(i)->m_button.get_window_text();
         
         class size size = pgraphics->GetTextExtent(strButtonText);

         size.cx += m_pschema->m_rectItemMargin.left;

         size.cx += m_pschema->m_rectItemMargin.right;

         size.cx += m_pschema->m_iElementPadding;

         size.cx += m_iCheckBoxSize;
         
         if (spitema->element_at(i)->IsPopup())
         {

            size.cx += m_pschema->m_iElementPadding;

            size.cx += m_iCheckBoxSize;

         }

         size.cy += m_pschema->m_rectItemMargin.top;

         size.cy += m_pschema->m_rectItemMargin.bottom;

         if (size.cy > iMaxHeight)
         {

            iMaxHeight = size.cy;

         }
         
         if (size.cx > iMaxWidth)
         {

            iMaxWidth = size.cx;

         }

      }

      m_iItemHeight = iMaxHeight;

      m_size.cx = iMaxWidth + m_pschema->m_rectItemMargin.left + m_pschema->m_rectItemMargin.right;
       
      ::count iItemCount = spitema->get_size();

      int32_t iSeparatorCount = pitem->m_iSeparatorCount;

      int x = m_pschema->m_rectItemMargin.left;

      string str;

      int cx = iMaxWidth;

      int cy;

      int y = m_pschema->m_rectItemMargin.top;

      y += m_iItemHeight;

      y += m_pschema->m_iElementPadding;

      for(int32_t i = 0; i < iItemCount; i++)
      {

         ::user::menu_item * pitem = spitema->element_at(i);

         if(pitem->m_id == "separator")
         {
            
            cy = m_pschema->m_rectItemMargin.top;

            cy += m_pschema->m_rectItemMargin.bottom;
            
         }
         else
         {
            
            cy = m_iItemHeight;

         }
         
         pitem->m_button.create_window(null_rect(), this, pitem->m_id);
         
         pitem->m_button.SetWindowPos(0, x, y, cx,cy, SWP_SHOWWINDOW | SWP_NOZORDER);
         
         pitem->m_button.m_pschema = m_pschema->m_pschemaItemButton;
         
         pitem->m_button.m_pitem = pitem;
         
         pitem->m_pbase = this;

         pitem->m_button.m_rectCheckBox.left = m_pschema->m_rectItemMargin.left;
         pitem->m_button.m_rectCheckBox.top = m_pschema->m_rectItemMargin.top;
         pitem->m_button.m_rectCheckBox.bottom = cy - m_pschema->m_rectItemMargin.bottom;
         pitem->m_button.m_rectCheckBox.right = pitem->m_button.m_rectCheckBox.left + m_iCheckBoxSize;

         pitem->m_button.m_rectText.left = pitem->m_button.m_rectCheckBox.right + m_pschema->m_iElementPadding;
         pitem->m_button.m_rectText.top = m_pschema->m_rectItemMargin.top;
         pitem->m_button.m_rectText.bottom = cy - m_pschema->m_rectItemMargin.bottom;
         pitem->m_button.m_rectText.right = cx - m_pschema->m_rectItemMargin.right;

         y += cy;

         y += m_pschema->m_iElementPadding;

      }

      y -= m_pschema->m_iElementPadding;

      y += m_pschema->m_rectItemMargin.bottom;

      m_size.cy = y;


      m_itemClose.m_button.ResizeToFit();

      m_itemClose.m_button.SetWindowPos(0, 0, 0, 0, 0, SWP_NOSIZE);

      rect rectWindow;

      rectWindow.left = pt.x;
      rectWindow.top = pt.y;
      rectWindow.right = rectWindow.left + m_size.cx;
      rectWindow.bottom = rectWindow.top + m_size.cy;

      rect rectMonitor;

      if (Session.get_best_monitor(rectMonitor, rectWindow) >= 0)
      {

         rectMonitor.deflate(16, 16);

         if (rectWindow.left < rectMonitor.left)
         {

            rectWindow.offset(rectMonitor.left - rectWindow.left, 0);

         }
         else if (rectWindow.right > rectMonitor.right)
         {

            rectWindow.offset(rectMonitor.right - rectWindow.right, 0);

         }

         if (rectWindow.top < rectMonitor.top)
         {

            rectWindow.offset(0, rectMonitor.top - rectWindow.top);

         }
         else if (rectWindow.bottom > rectMonitor.bottom)
         {

            rectWindow.offset(0, rectMonitor.bottom - rectWindow.bottom);

         }

      }

      SetWindowPos(ZORDER_TOPMOST,rectWindow,SWP_FRAMECHANGED | SWP_SHOWWINDOW);

      SetTimer(::user::BaseWndMenuCmdUi,100,NULL);

   }


   void menu::_001OnDraw(::draw2d::graphics * pgraphics)
   {

      COLORREF crBackground = 0;

      get_color(crBackground, ::user::color_background);

      if ((crBackground & ARGB(255, 0, 0, 0)) != 0)
      {

         rect rectClient;

         GetClientRect(rectClient);

         pgraphics->FillSolidRect(rectClient, ARGB(255, 255, 255, 255));

      }

      ::user::interaction::_001OnDraw(pgraphics);

   }


   void menu::_001OnDestroy(signal_details * pobj)
   {

      if(m_pmenuParent != NULL)
      {

         m_pmenuParent->m_psubmenu = NULL;

         m_pmenuParent->m_idSubMenu.is_empty();

      }

      if(m_psubmenu != NULL)
      {

         m_psubmenu->m_pmenuParent = NULL;

      }

      pobj->m_bRet = false;

   }


   void menu::_001OnLButtonDown(signal_details * pobj)
   {

      pobj->previous();

   }


   void menu::OnMessageDestroyWindow(signal_details * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

      DestroyWindow();

   }


   bool menu::BaseOnControlEvent(::user::control_event * pevent)
   {

      if(pevent->m_eevent == ::user::event_button_clicked)
      {

         if(pevent->m_puie == &m_itemClose.m_button)
         {

            ::user::control_event ev;

            ev.m_eevent = ::user::event_context_menu_close;

            ::user::interaction * puiTarget = get_target_window();

            if (puiTarget != NULL)
            {

               m_oswindowParent->BaseOnControlEvent(&ev);

            }

            post_message(WM_CLOSE);
         }
         else
         {

            sp(::user::menu_item) pitem = get_item(pevent->m_puie);

            //sp(::user::menu_item) pitemThis = get_item();

            //sp(::user::menu_item_ptra) spitema = pitemThis->m_spitema;

            //sp(::user::menu_item) pitem = spitema->find(pevent->m_puie->m_id);

            if(pitem != NULL && !pitem->m_bPopup)
            {

               if(::str::begins((const char *) pevent->m_puie->m_id, astr.stringSysCommand))
               {

                  ::user::interaction * puiTarget = get_target_window();

                  if (puiTarget != NULL)
                  {

                     puiTarget->_001SendCommand(pevent->m_puie->m_id);

                  }

               }
               else
               {
                  
                  ::user::interaction * puiTarget = get_target_window();

                  id id = pevent->m_puie->m_id;
                  
                  send_message(WM_CLOSE);

                  // this may be destroyed by WM_CLOSE above

                  if (puiTarget != NULL)
                  {

                     puiTarget->_001SendCommand(id);

                  }

                  pevent->m_bProcessed = true;

               }
            }
         }
         return true;
      }
      else if(pevent->m_eevent == ::user::event_mouse_enter)
      {
         
         if(pevent->m_puie == &m_itemClose.m_button)
         {

         }
         else
         {
            if(pevent->m_puie->m_id != m_idSubMenu)
            {
               if(m_psubmenu != NULL
               && m_idSubMenu.has_char())
               {
                  m_psubmenu->send_message(WM_CLOSE);
                  m_psubmenu = NULL;
                  m_idSubMenu = "";
               }
   /*
                  SetTimer(BaseWndMenuTimer, BaseWndMenuTiming, NULL);
                  if(m_pitem->m_pitema->element_at(pevent->m_pcontrol->m_uiId)->m_bPopup)
                  {
                     m_iTimerMenu = pevent->m_pcontrol->m_uiId;
                  }
                  else
                  {
                     m_iTimerMenu = -1;
                  }
               }
               else*/
               {
                  sp(::user::menu_item) pitem = get_item(pevent->m_puie);

                  //sp(::user::menu_item) pitemThis = get_item();

                  //sp(::user::menu_item_ptra) spitema = pitemThis->m_spitema;

                  //sp(::user::menu_item) pitem = spitema->find(pevent->m_puie->m_id);

                  if (pitem.is_set())
                  {

                     if (pitem->m_bPopup)
                     {

                        m_idSubMenu = pevent->m_puie->m_id;

                        m_psubmenu = canew(menu(get_app(), pitem));

                        rect rect;

                        pevent->m_puie->GetWindowRect(rect);

                        m_psubmenu->_TrackPopupMenu(0, rect.top_right(), m_oswindowParent, this);

                     }
                     else
                     {

                        ::user::control_event ev;

                        ev.m_eevent = ::user::event_menu_hover;

                        ev.m_id = pitem->m_id;

                        ::user::interaction * puiTarget = get_target_window();

                        if (puiTarget == NULL)
                        {

                           return false;

                        }

                        return puiTarget->BaseOnControlEvent(&ev);

                     }

                  }

               }
            }
         }
         return true;
      }
      else if(pevent->m_eevent == ::user::event_mouse_leave)
      {
         if(pevent->m_puie->m_id == m_idTimerMenu)
         {
            KillTimer(BaseWndMenuTimer);
            m_idTimerMenu.is_empty();
         }
         return true;
      }

      return false;

   }


   void menu::_001OnTimer(::timer * ptimer)
   {

      ::user::control::_001OnTimer(ptimer);

      sp(::user::menu_item) pitemThis = get_item();

      if (pitemThis == NULL)
      {

         return;

      }

      sp(::user::menu_item_ptra) spitema = pitemThis->m_spitema;

      if(ptimer->m_nIDEvent == BaseWndMenuTimer)
      {
         KillTimer(BaseWndMenuTimer);
         if(m_idSubMenu.has_char())
         {
            m_psubmenu->send_message(WM_CLOSE);
            m_psubmenu = NULL;
            m_idSubMenu.is_empty();
         }
         if(m_idTimerMenu.has_char())
         {
            m_idSubMenu = m_idTimerMenu;
            m_psubmenu = canew(menu(get_app(), spitema->find(m_idTimerMenu)));
            rect rect;
            spitema->find(m_idTimerMenu)->m_button.GetWindowRect(rect);
            m_psubmenu->_TrackPopupMenu(0,
               rect.top_right(), m_oswindowParent, this);
         }
         m_idTimerMenu.is_empty();
      }
      else if(ptimer->m_nIDEvent == ::user::BaseWndMenuCmdUi)
      {
         if(spitema != NULL)
         {
            ::user::menu_button_cmd_ui cmdui(get_app());
            cmdui.m_pitema          = spitema;
            for(int32_t i = 0; i < spitema->get_size(); i++)
            {

               cmdui.m_iIndex    = i;
               cmdui.m_id        = spitema->element_at(i)->m_id;
               cmdui.m_pOther    = (sp(::user::interaction)) &spitema->element_at(i)->m_button;

               sp(::user::interaction) puiTarget = get_target_window();
               
               if(puiTarget != NULL)
               {
                 /* xxx if(pwndParent->on_simple_action(0,
                     MAKELONG((int32_t)CN_UPDATE_COMMAND_UI, WM_COMMAND+WM_REFLECT_BASE),
                     &cmdui, NULL))
                     continue;*/
                  if(puiTarget->_001SendUpdateCmdUi(&cmdui))
                     continue;
               }
            }
         }
      }
      

   }


   void menu::install_message_handling(::message::dispatch * pinterface)
   {
      
      ::user::control::install_message_handling(pinterface);
      ::user::menu_base::install_message_handling(pinterface);

      IGUI_WIN_MSG_LINK(MessageDestroyWindow, pinterface, this, &menu::OnMessageDestroyWindow);
      IGUI_WIN_MSG_LINK(WM_IDLEUPDATECMDUI  , pinterface, this, &menu::_001OnIdleUpdateCmdUI);
      IGUI_WIN_MSG_LINK(WM_CREATE           , pinterface, this, &menu::_001OnCreate);
      IGUI_WIN_MSG_LINK(WM_DESTROY          , pinterface, this, &menu::_001OnDestroy);
      IGUI_WIN_MSG_LINK(WM_NCACTIVATE       , pinterface, this, &menu::_001OnNcActivate);
      IGUI_WIN_MSG_LINK(WM_NCCALCSIZE       , pinterface, this, &menu::_001OnNcCalcSize);
      IGUI_WIN_MSG_LINK(WM_ENABLE           , pinterface, this, &menu::_001OnEnable);
      IGUI_WIN_MSG_LINK(WM_SHOWWINDOW       , pinterface, this, &menu::_001OnShowWindow);
   }


   void menu::_001OnCreate(signal_details * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

      m_pschema            = &::user::GetUfeSchema(get_app())->m_menu;

      
   }



   void menu::_001OnIdleUpdateCmdUI(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
//      SCAST_PTR(::message::base, pbase, pobj);

      sp(::user::menu_item) pitemThis = get_item();

      sp(::user::menu_item_ptra) spitema = pitemThis->m_spitema;

      if(spitema != NULL)
      {
         ::user::menu_button_cmd_ui cmdui(get_app());
         cmdui.m_pitema          = spitema;
         for(int32_t i = 0; i < spitema->get_size(); i++)
         {

            cmdui.m_iIndex    = i;
            cmdui.m_id        = spitema->element_at(i)->m_id;
            cmdui.m_pOther    = (sp(::user::interaction)) &spitema->element_at(i)->m_button;

            sp(::user::interaction) pwndParent = m_oswindowParent;
            if(pwndParent != NULL)
            {
               /*
               if(pwndParent->on_simple_action(0,
                  MAKELONG((int32_t)CN_UPDATE_COMMAND_UI, WM_COMMAND+WM_REFLECT_BASE),
                  &cmdui, NULL))
                  continue;
                  */
               
               if(pwndParent->_001SendUpdateCmdUi(&cmdui))
                  continue;
            }

         }
      }
   }


   void menu::_001OnEnable(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }


   void menu::_001OnNcActivate(signal_details * pobj)
   {
      SCAST_PTR(::message::base, pbase, pobj);
      pbase->m_bRet = true;
      pbase->set_lresult(DefWindowProc(WM_NCACTIVATE, pbase->m_wparam, -1));
   }

   void menu::_001OnNcCalcSize(signal_details * pobj)
   {
      SCAST_PTR(::message::base, pbase, pobj);

      if(pbase->m_wparam == TRUE)
      {
         pbase->m_bRet = true;
         pbase->set_lresult(0);
      }
      else
      {
         LPRECT lprect = (LPRECT) pbase->m_lparam.m_lparam;
         lprect->left      = m_ptTrack.x;
         lprect->top       = m_ptTrack.y;
         lprect->right     = lprect->left + m_size.cx;
         lprect->bottom    = lprect->left + m_size.cx;
         pbase->m_bRet = true;
         pbase->set_lresult(0);
      }
   }


   bool menu::pre_create_window(::user::create_struct& cs)
   {

      cs.dwExStyle = WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW;

      cs.style &= ~WS_VISIBLE;

      return TRUE;

   }


   void menu::_001OnShowWindow(signal_details * pobj)
   {
      
      SCAST_PTR(::message::show_window, pshow, pobj);
   
      TRACE("menu::_001OnShowWindow bShow = %d", pshow->m_bShow);

   }


   sp(::user::menu_item) menu::get_item()
   {

      return m_pitem;

   }


   sp(::user::menu_item) menu::get_item(::user::interaction * pui)
   {

      sp(::user::menu_button) pbutton = pui;

      if (pbutton.is_null())
         return NULL;

      return pbutton->m_pitem;

      //if(pbutton.)


      //sp(::user::menu_item) pitemThis = get_item();

      //sp(::user::menu_item_ptra) spitema = pitemThis->m_spitema;


      //for(auto & pitem : *spitema.m_p)
      //{
      //   
      //   if(&pitem->m_button == pbutton.m_p)
      //   {

      //      return pitem;

      //   }

      //}

      //return NULL;

   }

   bool menu::get_color(COLORREF & cr,::user::e_color ecolor)
   {

      if(ecolor == ::user::color_background)
      {

         cr = ARGB(84+77,185,184,177);

         return true;

      }
      else
      {

         return ::user::control::get_color(cr,ecolor);

      }

   }


   bool menu::get_translucency(::user::ETranslucency & etranslucency)
   {

      etranslucency = ::user::TranslucencyPresent;

      return true;
   }


   ::user::interaction * menu::get_target_window()
   {

      ::user::interaction * puiTarget = m_oswindowParent;

      if (puiTarget == NULL)
      {

         return NULL;

      }

      if (dynamic_cast < ::user::impact * > (puiTarget) != NULL)
      {

         puiTarget = puiTarget->GetParentFrame();

      }

      return puiTarget;

   }


   bool menu::has_pending_graphical_update()
   {

      return true;

   }


} // namespace user




