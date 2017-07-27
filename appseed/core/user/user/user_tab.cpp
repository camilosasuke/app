#include "framework.h"

//#ifdef WINDOWS
//#undef new
////#include <gdiplus.h>
//#endif


extern CLASS_DECL_CORE thread_int_ptr < DWORD_PTR > t_time1;

// pgraphics->GetTextExtent("->:<-"); // oh no!! omg!! The size is the size of the alien!!
#define MAGIC_PALACE_TAB_SPLT "->:<-"
#define MAGIC_PALACE_TAB_SIZE "-/-"
#define MAGIC_PALACE_TAB_TEXT "/"


namespace user
{


   tab::data::data(::aura::application * papp) :
      object(papp),
      ::data::data(papp),
      m_imagelist(papp),
      m_pen(allocer()),
      m_font(allocer()),
      m_fontUnderline(allocer()),
      m_fontBigBold(allocer()),
      m_fontBold(allocer()),
      m_brushTextHover(allocer()),
      m_brushTextSel(allocer()),
      m_brushText(allocer()),
      m_brushCloseHover(allocer()),
      m_brushCloseSel(allocer()),
      m_brushClose(allocer()),
      m_penBorder(allocer()),
      m_penBorderSel(allocer()),
      m_penBorderHover(allocer()),
      m_dcextension(papp),
      m_panea(papp)
   {

      m_brushTextHover->create_solid(ARGB(255, 0, 127, 255));
      m_brushTextSel->create_solid(ARGB(255, 0, 0, 0));
      m_brushText->create_solid(ARGB(163, 0, 0, 0));
      m_brushCloseHover->create_solid(ARGB(255, 255, 127, 0));
      m_brushCloseSel->create_solid(ARGB(255, 0, 0, 0));
      m_brushClose->create_solid(ARGB(163, 0, 0, 0));
      m_penBorderHover->create_solid(1.0, ARGB(255, 0, 0, 0));
      m_penBorderSel->create_solid(1.0, ARGB(255, 0, 0, 0));
      m_penBorder->create_solid(1.0, ARGB(163, 84, 84, 77));

      m_bEnableCloseAll = false;
      m_rectTabClient.set(0,0,0,0);

   }

   tab::data::~data()
   {
   }


   tab_pane * tab::data::get_pane_by_id(id id)
   {

      return m_panea.get_by_id(id);

   }


   ::count tab::data::get_visible_tab_count()
   {

      return m_panea.get_visible_count();

   }

   tab::tab(::aura::application * papp) :
      object(papp),
      place_holder_container(papp),
      m_dcextension(papp)
   {

      m_iRestoredTabCount = 0;

      m_spdata = canew(data(papp));
      get_data()->m_panea.set_app(papp);
      get_data()->m_iHeightAddUp = 0;
      get_data()->m_pcallback    = NULL;
      get_data()->m_bCreated     = false;
      get_data()->m_iTabHeight   = 16;
      get_data()->m_iTabWidth    = 48;
      get_data()->m_imagelist.create(16, 16, 0, 0, 0);

      get_data()->m_iDragTab     = -1;

      m_bDisableSavingRestorableTabs           = true;

      m_bShowTabs                = true;

      m_bHidingTabs              = false;

      m_bDrawTabAtBackground = false;



      m_bNoTabs = System.directrix()->m_varTopicQuery.has_property("no_tabs");


      //m_rectBorder.set(7, 1, 7, 0);


      ::draw2d::graphics_sp spgraphics(allocer());
      spgraphics->CreateCompatibleDC(NULL);



      get_data()->m_font->create_point_font(FONT_SANS, 10);

      get_data()->m_fontBold->create_point_font(FONT_SANS, 10, FW_BOLD);

      get_data()->m_fontBigBold->create_point_font(FONT_SANS,12,FW_EXTRABOLD);

      get_data()->m_fontUnderline->create_point_font(FONT_SANS, 10, FW_NORMAL, false, true);

      _001SetVertical(false);

   }

   tab::~tab()
   {
   }

   /*bool tab::add_tab(UINT uiIdTitle, int32_t iId)
   {
      tab_pane tab_pane;
      tab_pane.m_uiId = uiIdTitle;
      tab_pane.m_istrTitleEx = L"";
      tab_pane.m_iId = iId == -1 ? get_data()->m_panea.get_size() : iId;
      if(!tab_pane.m_wstrTitle.load_string(uiIdTitle))
      {
         return false;
      }*/
   //   tab_pane.m_pcontainer = pcontainer;
     // tab_pane.m_typeinfo = NULL;
     /* tab_pane.m_iImage = -1;

      get_data()->m_panea.add(tab_pane);
      return true;
   }*/


   bool tab::set_title(::index iPane, const char * lpcsz)
   {

      if (iPane < 0)
      {

         return false;

      }

      if (iPane >= get_data()->m_panea.get_size())
      {

         return false;

      }

      get_data()->m_panea[iPane]->m_istrTitleEx = lpcsz;

      return true;

   }

   
   bool tab::SetTitleById(id id, const char * lpcsz)
   {

      index iPane = id_pane(id);

      if (iPane < 0)
      {

         return false;

      }
      
      get_data()->m_panea[iPane]->m_istrTitleEx = lpcsz;

      return true;

   }


   bool tab::set_tab(const char * lpcsz, id id, bool bVisible)
   {

      return add_tab(lpcsz, id, bVisible, true);

   }


   bool tab::add_tab(const char * lpcsz, id id, bool bVisible, bool bPermanent, ::user::place_holder * pholder)
   {

      sp(::user::tab_pane) ppane = canew(::user::tab_pane(get_app()));

      ppane->m_istrTitleEx       = lpcsz;
      ppane->m_bTabPaneVisible   = bVisible;
      ppane->m_bPermanent        = bPermanent;
      ppane->m_pholder           = pholder;

      if (id.is_empty())
      {

         id = get_data()->m_panea.get_size();

      }

      ppane->m_id               = id;
      ppane->m_dib.release();
      ppane->m_pholder          = NULL;

      get_data()->m_panea.add(ppane);

      on_change_pane_count({ ppane });

      return true;

   }

   bool tab::remove_tab_by_id(id id)
   {

      bool bRestorableMatch = false;

      for(int32_t i = 0; i < get_data()->m_panea.get_count(); i++)
      {

         if(get_data()->m_panea[i]->m_id == id)
         {

            if (!bRestorableMatch && matches_restorable_tab(get_data()->m_panea[i]->m_id, get_data()->m_panea[i]->m_pholder))
            {

               bRestorableMatch = true;

            }

            get_data()->m_panea.remove_at(i);

         }

      }

      if(bRestorableMatch)
      {

         on_change_pane_count({ NULL });

      }

      return true;
   }


   bool tab::set_image_tab(const char * lpcszTitle, const char * pszImage, id id, bool bVisible)
   {

      return add_image_tab(lpcszTitle, pszImage, id, bVisible, true);

   }

   bool tab::add_image_tab(const char * lpcszTitle, const char * pszImage, id id, bool bVisible, bool bPermanent)
   {

      sp(::user::tab_pane) ppane = canew(::user::tab_pane(get_app()));

      if (ppane == NULL)
      {

         return false;

      }

      ppane->m_bTabPaneVisible = bVisible;
      ppane->m_bPermanent  = bPermanent;
      ppane->m_istrTitleEx  = lpcszTitle;

      if(id.is_empty())
         id = get_data()->m_panea.get_size();

      ppane->m_id          = id;
      ppane->m_pholder     = NULL;
      ppane->m_dib.alloc(allocer());
      ppane->m_dib.load_from_file(pszImage);

      get_data()->m_panea.add(ppane);

      on_change_pane_count({ ppane });

      return true;

   }


   void tab::remove_tab(::index iPane, bool bVisible)
   {

      if(iPane < 0 || iPane >= get_data()->m_panea.get_size())
         return;

      if(bVisible)
      {
         for(int32_t i = 0; iPane >= 0 && i < get_data()->m_panea.get_count(); i++)
         {
            if(get_data()->m_panea[i]->m_bTabPaneVisible)
            {
               if(iPane <= 0)
               {
                  get_data()->m_panea.remove_at(i);
                  on_change_pane_count();
                  break;
               }
               else
               {
                  iPane--;
               }
            }
         }
      }
      else
      {
         get_data()->m_panea.remove_at(iPane);
         on_change_pane_count();
      }
   }


   bool tab::defer_handle_auto_hide_tabs(bool bLayout)
   {

      //sp(::user::wndfrm::frame::WorkSetClientInterface) pinterface = GetTopLevelFrame();
      //
      //if (pinterface != NULL && pinterface->m_bInitialFramePosition)
      //{
      //   return false;
      //}

      bool bNeedLayout = false;

      ::user::interaction * puiTopLevel = GetTopLevel();

      if (puiTopLevel != NULL)
      {

         if (puiTopLevel->frame_is_transparent())
         {

            if (m_bShowTabs)
            {

               m_bShowTabs = false;

               if (bLayout)
               {

                  on_layout();

               }

               bNeedLayout = true;

            }

            return bNeedLayout;

         }

      }

      if(m_bNoTabs)
      {

         if(m_bShowTabs)
         {

            m_bShowTabs = false;

            if (bLayout)
            {

               on_layout();

            }

            bNeedLayout = true;

         }

         return bNeedLayout;

      }

      if(m_bShowTabs)
      {

         if(GetParentFrame()->WfiIsFullScreen())
         {

            rect rectTab(get_data()->m_rectTab);

            ClientToScreen(rectTab);

            point ptCursor;

            Session.get_cursor_pos(&ptCursor);

            bool bShowTabs = rectTab.contains(ptCursor);

            if(bShowTabs)
            {

               m_bHidingTabs = false;

            }
            else if(m_bHidingTabs)
            {

               if(get_tick_count() - m_dwHidingTabs > 884)
               {

                  m_bHidingTabs = false;

                  m_bShowTabs = false;

                  if (bLayout)
                  {

                     on_layout();

                  }

                  bNeedLayout = true;

               }

            }
            else
            {

               m_bHidingTabs = true;

               m_dwHidingTabs = get_tick_count();

            }

         }

      }
      else
      {

         if(!GetParentFrame()->WfiIsFullScreen())
         {

            m_bShowTabs = true;

            if (bLayout)
            {

               on_layout();

            }

            bNeedLayout = true;

         }
         else
         {

            rect rectWindow;

            GetWindowRect(rectWindow);

            bool bShowTabs;

            point ptCursor;

            Session.get_cursor_pos(&ptCursor);

            if(get_data()->m_bVertical)
            {
               bShowTabs = ptCursor.x <= rectWindow.left;
            }
            else
            {
               bShowTabs = ptCursor.y <= rectWindow.top;
            }

            m_bShowTabs = bShowTabs;

            if(bShowTabs)
            {

               if (bLayout)
               {

                  on_layout();

               }

               bNeedLayout = true;

            }

         }

      }

      return bNeedLayout;

   }


   void tab::_001OnDraw(::draw2d::graphics * pgraphics)
   {

      //

      defer_handle_auto_hide_tabs();

      if(m_bNoTabs || !m_bShowTabs)
         return;

//      if(get_data()->is_locked())
//         return;

      if(GetTopLevel()->frame_is_transparent())
         return;

      if(m_puserschemaSchema == NULL)
      {

         _001OnDrawStandard(pgraphics);

         return;

      }
      

      if (!m_puserschemaSchema->_001TabOnDrawSchema01(pgraphics, this))
      {

         _001OnDrawSchema01(pgraphics);

      }

   }


   void tab::_001OnDrawSchema01(::draw2d::graphics * pgraphics)
   {


      class rect rect;
      class rect rectBorder;
      class rect rectText;
      class rect rectClient;
      class rect rectIcon;
      class rect rectClose;

      get_data()->m_pen->create_solid(1, RGB(32, 32, 32));

      pgraphics->set_text_rendering(::draw2d::text_rendering_anti_alias_grid_fit);

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

      int32_t iTab = -1;

      ::draw2d::brush_sp brushText;

      for (int32_t iPane = 0; iPane < get_data()->m_panea.get_size(); iPane++)
      {

         ::user::tab_pane & pane = get_data()->m_panea(iPane);

         if (!pane.m_bTabPaneVisible)
            continue;

         iTab++;

         if (!get_element_rect(iTab, rect, ::user::element_tab))
            continue;

         if (!get_element_rect(iTab, rectBorder, ::user::element_border))
            continue;

         if (!get_element_rect(iTab, rectClient, ::user::element_client))
            continue;

         if (get_data()->m_bVertical)
         {

            if (get_element_rect(iTab, rectIcon, ::user::element_icon))
            {

               pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);
               pane.m_dib->bitmap_blend(pgraphics, rectIcon);

            }

            ::draw2d::path_sp path(allocer());

            if (true)
            {

               if (get_data()->m_idaSel.contains(pane.m_id))
               {

                  //path->start_figure();

                  path->add_line(rectBorder.right, rectBorder.bottom, rectBorder.left + 1, rectBorder.bottom);
                  //path->add_line(rectClient.right, rectBorder.top);
                  path->add_line(rectBorder.left, rectBorder.top - (rectBorder.left - rectClient.left));
                  path->add_line(rectClient.left, rectBorder.top);
                  path->add_line(rectBorder.right, rectBorder.top);

                  path->end_figure(false);

                  pane.m_brushFillSel->CreateLinearGradientBrush(rectBorder.top_left(), rectBorder.bottom_left(), ARGB(230, 235, 235, 230), ARGB(250, 255, 255, 250));

                  pgraphics->SelectObject(pane.m_brushFillSel);

                  pgraphics->fill_path(path);

                  pgraphics->SelectObject(get_data()->m_penBorderSel);

                  pgraphics->draw_path(path);

                  pgraphics->set_font(get_data()->m_font);

                  brushText = get_data()->m_brushTextSel;

               }
               else
               {

                  //path->start_figure();

                  path->add_line(rectBorder.right, rectBorder.bottom, rectBorder.left + 1, rectBorder.bottom);
                  path->add_line(rectBorder.left, rectBorder.top - (rectBorder.left - rectClient.left));
                  path->add_line(rectText.left, rectBorder.top);
                  path->add_line(rectBorder.right, rectBorder.top);
                  path->add_line(rectBorder.right, rectBorder.bottom);

                  path->end_figure(true);

                  if (iTab == m_iHover && m_eelementHover != ::user::element_close_tab_button && m_eelementHover < ::user::element_split && m_eelementHover >(::user::element_split + 100))
                  {

                     pane.m_brushFillHover->CreateLinearGradientBrush(rectBorder.top_left(), rectBorder.bottom_left(), ARGB(230, 215, 215, 210), ARGB(250, 235, 235, 230));

                     pgraphics->SelectObject(pane.m_brushFillHover);

                     pgraphics->fill_path(path);

                     pgraphics->SelectObject(get_data()->m_penBorderHover);

                     pgraphics->draw_path(path);

                     pgraphics->set_font(get_data()->m_fontUnderline);

                     brushText = get_data()->m_brushTextHover;

                  }
                  else
                  {

                     pane.m_brushFill->CreateLinearGradientBrush(rectBorder.top_left(), rectBorder.bottom_left(), ARGB(230, 175, 175, 170), ARGB(250, 195, 195, 190));

                     pgraphics->SelectObject(pane.m_brushFill);

                     pgraphics->fill_path(path);

                     pgraphics->SelectObject(get_data()->m_penBorder);

                     pgraphics->draw_path(path);

                     pgraphics->set_font(get_data()->m_font);

                     brushText = get_data()->m_brushText;

                  }

               }

            }

         }
         else
         {

            if (get_element_rect(iTab, rectIcon, ::user::element_icon))
            {

               pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

               pane.m_dib->bitmap_blend(pgraphics, rectIcon);

            }

            if (true)
            {

               ::draw2d::path_sp path(allocer());

               if (get_data()->m_idaSel.contains(pane.m_id))
               {

                  path->add_line(rectBorder.left, rectClient.bottom, rectBorder.left, rectBorder.top);

                  path->add_line(rectClient.right, rectBorder.top);

                  path->add_line(rectBorder.right, rectBorder.top + (rectBorder.right - rectClient.right));

                  path->add_line(rectBorder.right - 1, rectClient.bottom);

                  path->end_figure(false);

                  pane.m_brushFillSel->CreateLinearGradientBrush(rectBorder.top_left(), rectBorder.bottom_left(), ARGB(230, 235, 235, 230), ARGB(250, 255, 255, 250));

                  pgraphics->SelectObject(pane.m_brushFillSel);

                  pgraphics->fill_path(path);

                  get_data()->m_penBorderSel->create_solid(1.0, ARGB(255, 0, 0, 0));

                  pgraphics->SelectObject(get_data()->m_penBorderSel);

                  pgraphics->draw_path(path);

                  pgraphics->set_font(get_data()->m_font);

                  brushText = get_data()->m_brushTextSel;

               }
               else
               {

                  //path->begin_figure(true, ::draw2d::fill_mode_winding);

                  path->add_line(rectBorder.left, rectClient.bottom, rectBorder.left, rectBorder.top);

                  path->add_line(rectClient.right, rectBorder.top);

                  path->add_line(rectBorder.right, rectBorder.top + (rectBorder.right - rectClient.right));

                  path->add_line(rectBorder.right - 1, rectClient.bottom);

                  path->end_figure(true);

                  if (iTab == m_iHover && m_eelementHover != ::user::element_close_tab_button && (m_eelementHover < ::user::element_split || m_eelementHover >(::user::element_split + 100)))
                  {

                     pane.m_brushFillHover->CreateLinearGradientBrush(rectBorder.top_left(), rectBorder.bottom_left(), ARGB(230, 215, 215, 210), ARGB(250, 235, 235, 230));

                     pgraphics->SelectObject(pane.m_brushFillHover);

                     pgraphics->fill_path(path);

                     pgraphics->SelectObject(get_data()->m_penBorderHover);

                     pgraphics->draw_path(path);

                     pgraphics->set_font(get_data()->m_fontUnderline);

                     brushText = get_data()->m_brushTextHover;

                  }
                  else
                  {

                     pane.m_brushFill->CreateLinearGradientBrush(rectBorder.top_left(), rectBorder.bottom_left(), ARGB(230, 175, 175, 170), ARGB(250, 195, 195, 190));

                     pgraphics->SelectObject(pane.m_brushFill);

                     pgraphics->fill_path(path);

                     pgraphics->SelectObject(get_data()->m_penBorder);

                     pgraphics->draw_path(path);

                     pgraphics->set_font(get_data()->m_font);

                     brushText = get_data()->m_brushTextSel;

                  }

               }

            }

         }

         if (true)
         {

            if (get_element_rect(iTab, rectText, ::user::element_text))
            {

               _001OnTabPaneDrawTitle(pane, this, pgraphics, rectText, brushText);

            }

         }

         if (true)
         {

            if (get_element_rect(iTab, rectClose, ::user::element_close_tab_button))
            {

               pgraphics->set_font(get_data()->m_fontBold);

               if (iTab == m_iHover && m_eelementHover == ::user::element_close_tab_button)
               {

                  brushText = get_data()->m_brushCloseHover;

               }
               else
               {

                  brushText = get_data()->m_brushClose;

               }

               pgraphics->SelectObject(brushText);

               pgraphics->draw_text("x", rectClose, DT_CENTER | DT_VCENTER);

            }

         }

      }

   }


   void tab::_001OnDrawStandard(::draw2d::graphics * pgraphics)
   {

      

      class rect rect;
      class rect rectBorder;
      class rect rectText;
      class rect rectClient;
      class rect rectIcon;
      class rect rectClose;

      get_data()->m_pen->create_solid(1,RGB(32,32,32));

      pgraphics->set_text_rendering(::draw2d::text_rendering_anti_alias_grid_fit);

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

      pgraphics->FillSolidRect(get_data()->m_rectTab, ARGB(0xc0, 250, 255, 255));

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_set);

      int32_t iVisiblePane = 0;

      ::draw2d::brush_sp brushText(allocer());

      for(int32_t iPane = 0; iPane < get_data()->m_panea.get_size(); iPane++)
      {

         ::user::tab_pane & tab_pane = get_data()->m_panea(iPane);

         if(!tab_pane.m_bTabPaneVisible)
            continue;



         if(!get_element_rect(iVisiblePane, rect, element_tab))
            continue;

         if(!get_element_rect(iVisiblePane, rectBorder, element_border))
            continue;

         if(!get_element_rect(iVisiblePane, rectClient, element_client))
            continue;


         if(get_data()->m_bVertical)
         {
            if(get_element_rect(iVisiblePane, rectIcon, element_icon))
            {
               pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);
               tab_pane.m_dib->bitmap_blend(pgraphics, rectIcon);
            }

            if(get_data()->m_idaSel.contains(iPane))
            {

               ::draw2d::pen_sp pen(allocer());

               pen->create_solid(1.0, ARGB(255, 0, 0, 0));

               pgraphics->SelectObject(pen);

               pgraphics->MoveTo(rectBorder.right, rectBorder.bottom);
               pgraphics->LineTo(rectBorder.left + 1, rectBorder.bottom);
               pgraphics->LineTo(rectBorder.left, rectBorder.top - (rectBorder.left - rectClient.left));
               pgraphics->LineTo(rectClient.left, rectBorder.top);
               pgraphics->LineTo(rectBorder.right, rectBorder.top);

               pgraphics->set_font(get_data()->m_fontBold);

               brushText = get_data()->m_brushTextSel;

            }
            else
            {

               ::draw2d::pen_sp pen(allocer());

               pen->create_solid(1.0, ARGB(255, 0, 0, 0));

               pgraphics->SelectObject(pen);

               pgraphics->MoveTo(rectBorder.right, rectBorder.bottom);
               pgraphics->LineTo(rectBorder.left + 1, rectBorder.bottom);
               pgraphics->LineTo(rectBorder.left, rectBorder.top - (rectBorder.left - rectClient.left));
               pgraphics->LineTo(rectText.left, rectBorder.top);
               pgraphics->LineTo(rectBorder.right, rectBorder.top);
               pgraphics->LineTo(rectBorder.right, rectBorder.bottom);
               if(iVisiblePane == m_iHover && m_eelementHover != element_close_tab_button)
               {
                  pgraphics->set_font(get_data()->m_fontUnderline);
                  brushText = get_data()->m_brushClose;
               }
               else
               {
                  pgraphics->set_font(get_data()->m_font);
                  brushText = get_data()->m_brushText;
               }
            }


         }
         else
         {

            if(get_element_rect(iVisiblePane, rectIcon, element_icon))
            {
               
               pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);
               
               tab_pane.m_dib->bitmap_blend(pgraphics, rectIcon);

            }

            if(get_data()->m_idaSel.contains(tab_pane.m_id))
            {

               ::draw2d::pen_sp pen(allocer());

               pen->create_solid(1.0, ARGB(255, 0, 0, 0));

               pgraphics->SelectObject(pen);

               pgraphics->MoveTo(rectBorder.left, rectClient.bottom);
               //pgraphics->LineTo(rectBorder.left, rectText.bottom);
               pgraphics->LineTo(rectBorder.left, rectBorder.top);
               pgraphics->LineTo(rectClient.right, rectBorder.top);
               pgraphics->LineTo(rectBorder.right, rectBorder.top + (rectBorder.right - rectClient.right));
               pgraphics->LineTo(rectBorder.right - 1, rectClient.bottom);
               //pgraphics->LineTo(rect.right, rectText.bottom);
               pgraphics->set_font(get_data()->m_fontBold);
               brushText->create_solid(ARGB(255, 0, 0, 0));
            }
            else
            {

::draw2d::pen_sp pen(allocer());

pen->create_solid(1.0, ARGB(255, 0, 0, 0));

pgraphics->SelectObject(pen);

//pgraphics->MoveTo(rect.left, rectBorder.bottom);
//pgraphics->LineTo(rect.right, rectBorder.bottom);
pgraphics->MoveTo(rectBorder.left, rectClient.bottom);
pgraphics->LineTo(rectBorder.left, rectBorder.top);
pgraphics->LineTo(rectClient.right, rectBorder.top);
pgraphics->LineTo(rectBorder.right, rectBorder.top + (rectBorder.right - rectClient.right));
pgraphics->LineTo(rectBorder.right - 1, rectClient.bottom);
pgraphics->LineTo(rectBorder.left, rectClient.bottom);

if (iVisiblePane == m_iHover && m_eelementHover != element_close_tab_button)
{
   pgraphics->set_font(get_data()->m_fontUnderline);
   brushText->create_solid(ARGB(255, 0, 127, 255));
}
else
{
   pgraphics->set_font(get_data()->m_font);
   brushText = get_data()->m_brushText;
}
            }

         }

         if (get_element_rect(iVisiblePane, rectText, element_text))
         {

            pgraphics->SelectObject(brushText);

            pgraphics->_DrawText(tab_pane.get_title(), tab_pane.get_title().get_length(), rectText, DT_LEFT | DT_BOTTOM);

         }

         if (get_element_rect(iVisiblePane, rectClose, element_close_tab_button))
         {
            pgraphics->set_font(get_data()->m_fontBold);
            if (iVisiblePane == m_iHover && m_eelementHover == element_close_tab_button)
            {
               brushText = get_data()->m_brushCloseSel;
               brushText->create_solid(ARGB(0xff, 255, 127, 0));
            }
            else
            {
               brushText->create_solid(ARGB(0xff, 0, 0, 0));
            }
            pgraphics->SelectObject(brushText);
            pgraphics->draw_text("x", rectClose, DT_CENTER | DT_VCENTER);
         }


         iVisiblePane++;

      }
      //pgraphics->SelectObject(hOldPen);

   }


   void tab::get_title(int iPane, stringa & stra)
   {

      ::user::tab_pane & tab_pane = get_data()->m_panea(iPane);

      stra = tab_pane.m_straTitle;

   }


   void tab::GetTabClientRect(LPRECT lprect)
   {

      *lprect = get_data()->m_rectTabClient;

   }


   void tab::GetTabClientRect(RECT64 * lprect)
   {

      rect rectClient;
      GetClientRect(rectClient);
      rect64 rect64(rectClient);
      *lprect = rect64;

   }


   void tab::on_layout()
   {

      rect r1;

      GetClientRect(r1);

      if (r1.is_empty())
      {


         return;

      }




      if (m_puserschemaSchema == NULL)
      {

         m_puserschemaSchema = GetTopLevelFrame();

      }

      if (m_puserschemaSchema == NULL)
      {

         m_puserschemaSchema = Application.userschema();

      }

      if(m_puserschemaSchema != NULL)
      {

         if (m_puserschemaSchema->_001OnTabLayout(this))
         {

            return;

         }

      }

      {

//         DWORD dwTime2 = ::get_tick_count();

         //TRACE("message_handler call time0= %d ms",dwTime2 - t_time1.operator DWORD_PTR());
         //TRACE("usertab::on_layout call time1= %d ms",dwTime2 - t_time1.operator DWORD_PTR());
      }
      if(!get_data()->m_bCreated)
         return;

      defer_handle_auto_hide_tabs(false);

      ::draw2d::memory_graphics pgraphics(allocer());
      pgraphics->SelectObject(get_data()->m_fontBold);

      m_dcextension.GetTextExtent(pgraphics,MAGIC_PALACE_TAB_SIZE,get_data()->m_sizeSep);



      if(get_data()->m_bVertical)
      {
         int32_t iTabWidth = 16;
         int32_t iTabHeight = 8;
         int32_t cx;
         int32_t cy;
         for(int32_t iPane = 0; iPane < get_data()->m_panea.get_size(); iPane++)
         {

            ::user::tab_pane & tab_pane = get_data()->m_panea(iPane);

            if(!tab_pane.m_bTabPaneVisible)
               continue;


            string str = tab_pane.get_title();

            tab_pane.do_split_layout(m_dcextension, pgraphics);

            ::size size;

            m_dcextension.GetTextExtent(pgraphics, str, size);



            if(tab_pane.m_dib.is_set())
            {
               size.cx += tab_pane.m_dib->m_size.cx+ 2;
               size.cy = MAX(size.cy, tab_pane.m_dib->m_size.cy);
            }
            cx = size.cx + 2;

            if(!tab_pane.m_bPermanent)
            {
               cx += 2 + 16 + 2;
            }

            if(cx > iTabWidth)
            {
               iTabWidth = cx;
            }
            cy = size.cy + 2;
            if(cy > iTabHeight)
            {
               iTabHeight = cy;
            }
         }

         // close tab button
         cy = 2 + 16 + 2;
         if(cy > iTabHeight)
         {
            iTabHeight = cy;
         }

         iTabWidth += get_data()->m_rectBorder.left + get_data()->m_rectBorder.right +
               get_data()->m_rectMargin.left + get_data()->m_rectMargin.right +
               get_data()->m_rectTextMargin.left + get_data()->m_rectTextMargin.right;

         get_data()->m_iTabWidth = iTabWidth;

         iTabHeight += get_data()->m_rectBorder.top + get_data()->m_rectBorder.bottom +
               get_data()->m_rectMargin.top + get_data()->m_rectMargin.bottom +
               get_data()->m_rectTextMargin.top + get_data()->m_rectTextMargin.bottom;

         get_data()->m_iTabHeight = iTabHeight;

         rect rectClient;
         GetClientRect(rectClient);

         get_data()->m_rectTab.left       = rectClient.left;
         get_data()->m_rectTab.top        = rectClient.top;
         get_data()->m_rectTab.right      = get_data()->m_rectTab.left + get_data()->m_iTabWidth;
         get_data()->m_rectTab.bottom     = rectClient.bottom;

   /*      m_pui->SetWindowPos(
            ZORDER_TOP,
            m_rectTab.left,
            m_rectTab.top,
            m_rectTab.width(),
            m_rectTab.height(),
            0);*/

         get_data()->m_rectTabClient.left       = m_bShowTabs ? get_data()->m_rectTab.right : rectClient.left;
         get_data()->m_rectTabClient.top        = get_data()->m_rectTab.top;
         get_data()->m_rectTabClient.right      = rectClient.right;
         get_data()->m_rectTabClient.bottom     = get_data()->m_rectTab.bottom;

      }
      else
      {
         int32_t iTabHeight = 16;
         int32_t cy;
         ::draw2d::graphics_sp graphics(allocer());
         graphics->CreateCompatibleDC(NULL);
         ::draw2d::graphics * pgraphics = graphics;
         pgraphics->SelectObject(get_data()->m_fontBold);

         rect rectClient;
         GetClientRect(rectClient);
         int x = rectClient.left;

         int32_t ixAdd;
         for(int32_t iPane = 0; iPane < get_data()->m_panea.get_size(); iPane++)
         {

            ::user::tab_pane & tab_pane = get_data()->m_panea(iPane);

            if(!tab_pane.m_bTabPaneVisible)
               return;

            string str = tab_pane.get_title();

            tab_pane.do_split_layout(m_dcextension,pgraphics);

            size size;

            m_dcextension.GetTextExtent(pgraphics, str, size);

            if(tab_pane.m_dib.m_p != NULL)
            {
               size.cy = MAX(size.cy, tab_pane.m_dib->size().cy);
            }
            cy = size.cy + 2;

            if(cy > iTabHeight)
            {
               iTabHeight = cy;
            }

            tab_pane.m_pt.x = x;
            tab_pane.m_pt.y = rectClient.top;


//            string str = tab_pane.get_title();

//            size size;

            ixAdd = 5;

            if (tab_pane.m_dib.is_set())
            {
               //::image_list::info ii;
               ixAdd += tab_pane.m_dib->m_size.cx + 2;
            }

            if (!tab_pane.m_bPermanent)
            {
               ixAdd += 2 + 16 + 2;
            }




            tab_pane.m_size.cx = size.cx + ixAdd
                                     + get_data()->m_rectBorder.left + get_data()->m_rectBorder.right
                                     + get_data()->m_rectMargin.left + get_data()->m_rectMargin.right
                                     + get_data()->m_rectTextMargin.left + get_data()->m_rectTextMargin.right;

            x += tab_pane.m_size.cx;
         }

         // close tab button
         cy = 2 + 16 + 2;
         if(cy > iTabHeight)
         {
            iTabHeight = cy;
         }

         iTabHeight += get_data()->m_rectBorder.top + get_data()->m_rectBorder.bottom +
               get_data()->m_rectMargin.top + get_data()->m_rectMargin.bottom + get_data()->m_iHeightAddUp;

         get_data()->m_iTabHeight = iTabHeight;

         for (int32_t iPane = 0; iPane < get_data()->m_panea.get_size(); iPane++)
         {

            ::user::tab_pane & tab_pane = get_data()->m_panea(iPane);

            tab_pane.m_size.cy = iTabHeight;

         }




         get_data()->m_rectTab.left       = rectClient.left;
         get_data()->m_rectTab.top        = rectClient.top;
         get_data()->m_rectTab.right      = rectClient.right;
         get_data()->m_rectTab.bottom     = get_data()->m_rectTab.top + get_data()->m_iTabHeight;

   /*      SetWindowPos(
            ZORDER_TOP,
            m_rectTab.left,
            m_rectTab.top,
            m_rectTab.width(),
            m_rectTab.height(),
            0);*/

         rect & rectTabClient = get_data()->m_rectTabClient;

         rectTabClient.left       = get_data()->m_rectTab.left;
         rectTabClient.top        = m_bShowTabs ? get_data()->m_rectTab.bottom : rectClient.top;
         rectTabClient.right      = get_data()->m_rectTab.right;
         rectTabClient.bottom     = rectClient.bottom;

         //TRACE0("rectTabClient");
      }

            {

//               DWORD dwTime2 = ::get_tick_count();

               //TRACE("message_handler call time0= %d ms",dwTime2 - t_time1.operator DWORD_PTR());
               //TRACE("usertab::on_layout call time2= %d ms",dwTime2 - t_time1.operator DWORD_PTR());
      }


      for(int32_t iPane = 0; iPane < get_data()->m_panea.get_size(); iPane++)
      {

         if(iPane != _001GetSel())
         {

            layout_pane(iPane);

         }

      }

      layout_pane(_001GetSel(), IsWindowVisible());

   }


   void tab::layout_pane(index iPane, bool bDisplay)
   {

      sp(place_holder) pholder = pane_holder(iPane);

      if (pholder.is_null())
      {

         return;

      }

      rect rectChild;

      GetTabClientRect(rectChild);

      rect rectWindow;

      pholder->GetWindowRect(rectWindow);

      ScreenToClient(rectWindow);

      pholder->SetWindowPos(ZORDER_TOP, rectChild.left, rectChild.top, rectChild.width(), rectChild.height(), 
         (bDisplay && !pholder->is_this_visible() ? 0 : SWP_NOZORDER ));

      if (bDisplay)
      {

         pholder->ShowWindow(SW_SHOW);

         RedrawWindow();

      }

   }


   void tab::_001OnLButtonDown(signal_details * pobj)
   {
      SCAST_PTR(::message::mouse, pmouse, pobj);
      class point point = pmouse->m_pt;

      index iPane = hit_test(point, m_eelement);

      get_data()->m_bDrag = false;
      if(iPane >= 0)
      {
         index iSel = _001GetSel();
         if(m_eelement == element_close_tab_button)
         {
            get_data()->m_iDragTab = iPane;
            pmouse->m_bRet = true;
            pmouse->set_lresult(1);
         }
         else if(iPane != iSel)
         {
            get_data()->m_iDragTab = iPane;
            SetTimer(5432187, 840, NULL);
            pmouse->m_bRet = true;
            pmouse->set_lresult(1);
         }
      }
   }


   void tab::_001OnLButtonUp(signal_details * pobj)
   {
      SCAST_PTR(::message::mouse, pmouse, pobj);
      class point point = pmouse->m_pt;

      e_element eelement;

      index iPane = hit_test(point, eelement);

      KillTimer(5432187);

      index iDragTab = get_data()->m_iDragTab;

      if(iPane >= 0 && iDragTab == iPane && m_eelement == eelement)
      {
         if(eelement == element_close_tab_button)
         {
            _001OnTabClose(iPane);
         }
         else
         {
            _001OnTabClick(iPane);
         }
         pmouse->m_bRet = true;
         pmouse->set_lresult(1);
      }
      else
      {
         get_data()->m_iDragTab = -1;
      }
   }

   void tab::_001OnMouseMove(signal_details * pobj)
   {
//      SCAST_PTR(::message::mouse, pmouse, pobj);
//      class point point = pmouse->m_pt;
      if(get_data()->m_iDragTab >= 0)
      {
         if(get_data()->m_pcallback != NULL)
         {
            get_data()->m_pcallback->_001DropTargetWindowRelay(this);
         }
      }
/*      if(m_iHover < 0)
      {
         track_mouse_hover();
      }

      index iHover = hit_test(point, m_eelementHover);

      if(iHover != m_iHover)
      {
         m_iHover = iHover;
         RedrawWindow();
      }*/

   }


   void tab::_001OnMouseLeave(signal_details * pobj)
   {
      SCAST_PTR(::message::base, pbase, pobj);
      m_iHover = -1;
      //GetParent()->RedrawWindow();
      pbase->set_lresult(0);
      pobj->m_bRet = true;
   }


   bool tab::get_element_rect(::index iTab, LPRECT lprect, e_element eelement)
   {
      
      if (iTab < 0)
      {

         return false;

      }

      index iPane = tab_pane(iTab);

      if(eelement == element_border)
      {
       
         if (!get_element_rect(iTab, lprect, element_tab))
         {

            return false;

         }

         deflate(lprect, get_data()->m_rectMargin);

         return true;

      }
      
      if(eelement == element_client)
      {

         if (!get_element_rect(iTab, lprect, element_border))
         {

            return false;

         }

         deflate(lprect, get_data()->m_rectBorder);

         return true;

      }

      if(eelement == element_icon)
      {

         if (get_data()->m_panea[iPane]->m_dib.is_null())
         {

            return false;

         }

         if (!get_element_rect(iTab, lprect, element_client))
         {

            return false;

         }

         lprect->right = lprect->left + get_data()->m_panea[iPane]->m_dib->m_size.cx;

         lprect->bottom = lprect->top + get_data()->m_panea[iPane]->m_dib->m_size.cy;

         return true;

      }
      else if(eelement == element_text)
      {

         if (!get_element_rect(iTab, lprect, element_client))
         {
          
            return false;

         }

         if(get_data()->m_panea[iPane]->m_dib.is_set())
         {

            lprect->left += get_data()->m_panea[iPane]->m_dib->m_size.cx + 2;

         }

         if(!get_data()->m_panea[iPane]->m_bPermanent)
         {

            lprect->right -= 2 + 16 + 2;

         }

         deflate(lprect, get_data()->m_rectTextMargin);

         return true;

      }
      else if(eelement == element_close_tab_button)
      {

         if (iPane < 0)
         {

            return false;

         }

         if (get_data()->m_panea[iPane]->m_bPermanent)
         {
          
            return false;

         }
         
         if (get_data()->get_visible_tab_count() <= 1 && !get_data()->m_bEnableCloseAll)
         {

            return false;

         }

         if (!get_element_rect(iTab, lprect, element_client))
         {

            return false;

         }

         lprect->right  = lprect->right;

         lprect->left   = lprect->right - 20;

         lprect->top    = lprect->bottom - 20;

         return true;

      }

      if (eelement != element_tab)
      {

         return false;

      }

      if(get_data()->m_bVertical)
      {

         rect rect = get_data()->m_rectTab;
         rect.bottom = rect.top;

         lprect->left   = rect.left;
         lprect->top    = (LONG) (rect.top + iTab * get_data()->m_iTabHeight);
         lprect->right  = rect.right;
         lprect->bottom = lprect->top + get_data()->m_iTabHeight;

      }
      else
      {

         ::user::tab_pane & tab_pane = get_data()->m_panea(iTab);
         lprect->left = tab_pane.m_pt.x;
         lprect->top = tab_pane.m_pt.y;
         lprect->right = tab_pane.m_pt.x + tab_pane.m_size.cx;
         lprect->bottom = tab_pane.m_pt.y + tab_pane.m_size.cy;
         
      }

      return true;

   }


   ::count tab::get_pane_count()
   {

      return get_data()->m_panea.get_size();

   }

   ::count tab::get_tab_count()
   {

      return get_data()->m_panea.pred_get_count([](auto & pane) {return pane->m_bTabPaneVisible; });

   }


   index tab::hit_test(point pt, e_element & eelement)
   {
      ScreenToClient(&pt);
      rect rect;
      for(int32_t iPane = 0; iPane < get_data()->m_panea.get_size(); iPane++)
      {
         ::user::tab_pane & p = *get_data()->m_panea[iPane];

         if(p.m_straTitle.get_size() > 1)
         {

            ::rect rectText;

            if(get_element_rect(iPane,rectText,element_text) && rectText.contains(pt))
            {
               for(int iTitle = 0; iTitle < p.m_straTitle.get_size(); iTitle++)
               {
                  rectText.left += p.m_sizeaText[iTitle].cx;
                  rectText.right = rectText.left + get_data()->m_sizeSep.cx;
                  if(rectText.contains(pt))
                  {
                     eelement = (e_element)((int)element_split + iTitle);
                     return iPane;
                  }
                  rectText.left += get_data()->m_sizeSep.cx;
               }
            }

         }

         if(get_element_rect(iPane, rect, element_close_tab_button) && rect.contains(pt))
         {
            eelement = element_close_tab_button;
            return iPane;
         }
         if(get_element_rect(iPane, rect, element_tab) && rect.contains(pt))
         {
            eelement = element_tab;
            return iPane;
         }
      }
      return -1;
   }

   ::window_sp tab::GetNotifyWnd()
   {
#ifdef METROWIN
      return NULL;

#else
      ::window_sp pwnd;
   //   if((pwnd = m_pui->GetOwner()) != NULL)
     //    return pwnd;
      if((pwnd = GetParent()->get_wnd()) != NULL)
         return pwnd;
      return NULL;
#endif
   }

   /*
   bool tab::create(::user::interaction * pinterface, UINT uiId)
   {
      if(!m_pui->create(
         pinterface,
         uiId))
         return false;
      m_bCreated = true;
      on_layout();
      return true;
   }
   */

   void tab::_001OnCreate(signal_details * pobj)
   {

      SCAST_PTR(::message::base, pbase, pobj);

      if(pobj->previous())
         return;

      //m_puserschemaSchema = Application.userschema();
   //  m_pimagelist = new image_list(get_app());

      get_data()->m_bCreated = true;

      pbase->set_lresult(0);

      //post_message(WM_USER + 1342);

      m_iRestoredTabCount = restore_tabs();


      on_create_tabs();


   }


   void tab::on_create_tabs()
   {

      {

         ::user::control_event ev;

         ev.m_puie      = this;

         ev.m_ptab      = this;

         ev.m_eevent    = ::user::event_on_create_tab;

         BaseOnControlEvent(&ev);

      }


   }


   ::count tab::restore_tabs()
   {

      ::count c = 0;

      keep_bool keepDisableSavingRestorableTabs(&m_bDisableSavingRestorableTabs);

      if(get_data()->m_matchanyRestore.get_count() > 0)
      {

         var_array vara;

         if(data_load("restore_tab",vara))
         {

            c = open_tabs(vara);

         }

      }

      return c;

   }


   void tab::install_message_handling(::message::dispatch *pinterface)
   {

      ::user::control::install_message_handling(pinterface);

      IGUI_WIN_MSG_LINK(WM_LBUTTONDOWN , pinterface, this, &tab::_001OnLButtonDown);
      IGUI_WIN_MSG_LINK(WM_LBUTTONUP   , pinterface, this, &tab::_001OnLButtonUp);
      IGUI_WIN_MSG_LINK(WM_MOUSEMOVE   , pinterface, this, &tab::_001OnMouseMove);
      IGUI_WIN_MSG_LINK(WM_MOUSELEAVE  , pinterface, this, &tab::_001OnMouseLeave);
      IGUI_WIN_MSG_LINK(WM_CREATE      , pinterface, this, &tab::_001OnCreate);
      IGUI_WIN_MSG_LINK(WM_SHOWWINDOW  , pinterface, this, &tab::_001OnShowWindow);
      IGUI_WIN_MSG_LINK(::base::application::APPM_LANGUAGE, pinterface, this, &tab::_001OnAppLanguage);
      IGUI_WIN_MSG_LINK(message_start_tab_drag,pinterface,this,&tab::_001OnStartTabDrag);


      ////IGUI_WIN_MSG_LINK(WM_TIMER, pinterface, this, &tab::_001OnTimer);

   }


   void tab::_001SetSel(::index iSel)
   {

      {

         synch_lock lock(get_data()->m_pmutex);

         get_data()->m_idaSel.remove_all();

         id idTab = tab_id(iSel);

         if (!idTab.is_empty())
         {

            get_data()->m_idaSel.add(idTab);

         }

      }

      //on_change_pane_count();

      on_show_view();

      on_stage_view();

   }


   tab_pane::tab_pane(::aura::application * papp) :
      object(papp),
      m_brushFill(allocer()),
      m_brushFillSel(allocer()),
      m_brushFillHover(allocer()),
      m_istrTitleEx(papp)
   {
      m_bTabPaneVisible = true;
      m_bPermanent   = false;
      m_pholder      = NULL;
   }

   tab_pane::tab_pane(const ::user::tab_pane & tab_pane) :
      m_istrTitleEx(tab_pane.get_app())
   {
      operator = (tab_pane);
   }

   tab_pane::~tab_pane()
   {
   }

   tab_pane & tab_pane::operator = (const tab_pane & tab_pane)
   {

      if(this != &tab_pane)
      {
         m_id              = tab_pane.m_id;
         m_istrTitleEx     = tab_pane.m_istrTitleEx;
         m_dib             = tab_pane.m_dib;
         m_pholder         = tab_pane.m_pholder;
         m_bTabPaneVisible = tab_pane.m_bTabPaneVisible;
         m_bPermanent      = tab_pane.m_bPermanent;
      }

      return *this;

   }

   string tab_pane::get_title()
   {

      return m_istrTitleEx;

   }

   void tab_pane::do_split_layout(::visual::graphics_extension & dc, ::draw2d::graphics * pgraphics)
   {

      stringa & straTitle = m_straTitle;

      straTitle.explode(MAGIC_PALACE_TAB_SPLT,get_title());

      m_sizeaText.set_size(straTitle.get_count());

      for(int iTitle = 0; iTitle < straTitle.get_count(); iTitle++)
      {

         dc.GetTextExtent(pgraphics,straTitle[iTitle],m_sizeaText[iTitle]);

      }

   }




   tab_pane_array::tab_pane_array(::aura::application * papp) :
      object(papp)
   {
   }

   tab_pane_array::~tab_pane_array()
   {
   }



   tab_pane * tab_pane_array::get_by_id(id id)
   {
      for(int32_t i = 0; i < this->get_count(); i++)
      {
         if(this->element_at(i)->m_id == id)
         {
            return element_at(i);
         }
      }
      return NULL;
   }

   ::count tab_pane_array::get_visible_count()
   {

      ::count count = 0;

      for(int32_t i = 0; i < this->get_count(); i++)
      {
         if(this->element_at(i)->m_bTabPaneVisible)
         {
            count++;
         }
      }

      return count;
   }

   void tab::_001OnAppLanguage(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      on_layout();
   }



   void tab::_001SetTabCallback(tab_callback * pcallback)
   {
      if(get_data()->m_pcallback != NULL)
      {
         delete get_data()->m_pcallback;
      }
      get_data()->m_pcallback = pcallback;;

   }


   ::index tab::_001GetSel()
   {

      if(get_data()->m_idaSel.get_size() == 1)
      {

         return id_tab(get_data()->m_idaSel[0]);

      }
      else
      {

         return -1;

      }

   }


   ::count tab::_001GetPaneCount()
   {
      return get_data()->m_panea.get_count();
   }


   bool tab::_001IsAddTab(::index iPane)
   {
      if(get_data()->m_pcallback != NULL)
      {
         return get_data()->m_pcallback->_001IsAddTab(this, iPane);
      }
      else
      {
         return false;
      }
   }


   void tab::_001OnTabClick(::index iPane)
   {

      if(get_data()->m_pcallback != NULL)
      {

         get_data()->m_pcallback->_001OnTabClick(this, iPane);

      }
      else
      {

         _001SelectTab(iPane);

      }

   }


   void tab::_001OnTabClose(::index iPane)
   {

      if(get_data()->m_pcallback != NULL)
      {

         get_data()->m_pcallback->_001OnTabClose(this, iPane);

      }
      else
      {

         _001CloseTab(iPane);

      }

   }


   bool tab::show_tab_by_id(id id, bool bShow)
   {

      ::user::tab_pane * ppane = get_pane_by_id(id);

      if (ppane == NULL)
      {
       
         return false;

      }

      ppane->m_bTabPaneVisible = bShow;

      on_layout();

      return true;

   }


   bool tab::show_pane(::index iPane, bool bShow)
   {
      
      ::user::tab_pane * ppane = get_pane(iPane);

      if (ppane == NULL)
      {

         return !bShow;

      }

      ppane->m_bTabPaneVisible = bShow;

      on_layout();

      return true;

   }

   
   bool tab::hide_tab(::index iTab)
   {

      index iPane = tab_pane(iTab);

      if (iPane < 0)
      {

         return false;

      }

      if (!show_pane(iPane, false))
      {

         return false;

      }

      return true;

   }


   ::user::interaction * tab::tab_window(::index iPane)
   {
      
      sp(place_holder) pholder = tab_holder(iPane);

      if (pholder.is_null())
      {

         return NULL;

      }

      return pholder->get_hold();

   }

   
   ::user::place_holder * tab::tab_holder(::index iTab)
   {

      ::user::tab_pane * ppane = get_tab(iTab);

      if (ppane == NULL)
      {

         return NULL;

      }

      return ppane->m_pholder;

   }


   ::user::interaction * tab::pane_window(::index iPane)
   {

      sp(place_holder) pholder = pane_holder(iPane);

      if (pholder.is_null())
      {

         return NULL;

      }

      return pholder->get_hold();

   }


   ::user::place_holder * tab::pane_holder(::index iPane)
   {

      ::user::tab_pane * ppane = get_pane(iPane);

      if (ppane == NULL)
      {

         return NULL;

      }

      return ppane->m_pholder;

   }


   tab_pane * tab::get_pane(::index iPane)
   {

      if (iPane < 0)
      {

         return NULL;

      }

      if (iPane >= get_data()->m_panea.get_count())
      {

         return NULL;

      }

      return get_data()->m_panea.element_at(iPane);

   }


   tab_pane * tab::get_tab(::index iTab)
   {

      index iPane = tab_pane(iTab);

      if(iPane < 0)
      {

         return NULL;

      }

      return get_data()->m_panea.element_at(iPane);
      
   }


   void tab::_001SelectTab(::index iPane)
   {
//      if(get_tab_holder(_001GetSel()) != NULL)
  //    {
    //     get_tab_holder(_001GetSel())->ShowWindow(SW_HIDE);
     // }
      _001SetSel(iPane);
     // if(get_tab_holder(_001GetSel()) != NULL)
      //{
        // get_tab_holder(_001GetSel())->ShowWindow(SW_SHOW);
     // }

      on_layout();

   }


   void tab::on_show_view()
   {
      
      if(get_data()->m_pcallback != NULL && dynamic_cast < ::user::tab * > (get_data()->m_pcallback) != this)
      {

         get_data()->m_pcallback->_001OnShowTab(this);

      }

      Application.on_show_view(this);

   }


   void tab::on_stage_view()
   {


   }


   void tab:: _001SetVertical(bool bVertical)
   {
      get_data()->m_bVertical = bVertical;

      if(get_data()->m_bVertical)
      {
         get_data()->m_rectMargin.set(2, 3, 1, 0);
         get_data()->m_rectBorder.set(11, 1, 6, 0);
         get_data()->m_rectTextMargin.set(3, 0, 1, 0);

         get_data()->m_iTabHeight += get_data()->m_rectBorder.top + get_data()->m_rectBorder.bottom +
               get_data()->m_rectMargin.top + get_data()->m_rectMargin.bottom;

         get_data()->m_iTabWidth += get_data()->m_rectBorder.left + get_data()->m_rectBorder.right +
               get_data()->m_rectMargin.left + get_data()->m_rectMargin.right;
      }
      else
      {
         get_data()->m_rectMargin.set(3, 2, 0, 1);
         get_data()->m_rectBorder.set(0, 1, 11, 6);
         get_data()->m_rectTextMargin.set(3, 0, 1, 0);

         get_data()->m_iTabHeight += get_data()->m_rectBorder.top + get_data()->m_rectBorder.bottom +
               get_data()->m_rectMargin.top + get_data()->m_rectMargin.bottom;

         get_data()->m_iTabWidth += get_data()->m_rectBorder.left + get_data()->m_rectBorder.right +
               get_data()->m_rectMargin.left + get_data()->m_rectMargin.right;
      }
   }

   void tab::_001ConnectParent(::message::dispatch * pinterface)
   {
      UNREFERENCED_PARAMETER(pinterface);
   }

   
   id tab::get_cur_tab_id()
   {

      return tab_id(_001GetSel());

   }


   ::index tab::id_tab(id id)
   {

      index iTab = -1;

      for(::index iPane = 0; iPane < get_data()->m_panea.get_size(); iPane++)
      {

         if (get_data()->m_panea[iPane]->m_bTabPaneVisible)
         {

            iTab++;

         }

         if (get_data()->m_panea[iPane]->m_id == id)
         {

            return iTab;

         }

      }

      return -1;

   }

   ::index tab::id_pane(id id)
   {

      for (::index iPane = 0; iPane < get_data()->m_panea.get_size(); iPane++)
      {


         if (get_data()->m_panea[iPane]->m_id == id)
         {

            return iPane;

         }

      }

      return -1;

   }


   id tab::tab_id(::index iTab)
   {

      for(int32_t iPane = 0; iPane >= 0 && iPane < get_data()->m_panea.get_count(); iPane++)
      {

         if(get_data()->m_panea[iPane]->m_bTabPaneVisible)
         {

            if(iTab <= 0)
            {

               return get_data()->m_panea[iPane]->m_id;

            }
            else
            {

               iTab--;

            }

         }

      }

      return id();

   }


   id tab::pane_id(::index iPane)
   {

      if (iPane < 0)
      {

         return id();

      }

      if (iPane >= get_data()->m_panea.get_size())
      {

         return id();

      }
         
      return get_data()->m_panea[iPane]->m_id;

   }


   ::index tab::tab_pane(index iTab)
   {

      if (iTab < 0)
      {

         return -1;

      }

      for (::index iPane = 0; iPane < get_data()->m_panea.get_size(); iPane++)
      {

         if (!get_data()->m_panea[iPane]->m_bTabPaneVisible)
         {

            continue;

         }

         if(iTab <= 0)
         {

            return iPane;

         }

         iTab--;



      }

      return -1;

   }


   ::index tab::pane_tab(index iPaneParam)
   {

      if (iPaneParam < 0)
      {

         return -1;

      }

      index iTab = 0;

      for (::index iPane = 0; iPane < get_data()->m_panea.get_size(); iPane++)
      {

         if (iPaneParam == iPane)
         {

            if (get_data()->m_panea[iPane]->m_bTabPaneVisible)
            {

               return iTab;

            }
            else
            {

               return -1;

            }

         }


         if (get_data()->m_panea[iPane]->m_bTabPaneVisible)
         {

            iTab++;

         }


      }

      return -1;

   }


   bool tab::set_cur_tab_by_id(id id, ::create * pcreate)
   {

      try
      {

         m_spcreate = pcreate;

         ::index iTab = id_tab(id);

         if(iTab == -1)
         {

            iTab = create_tab_by_id(id);

            if (iTab == -1)
            {

               return false;

            }

         }

         _001SetSel(iTab);

      }
      catch(::exit_exception & e)
      {

         throw e;

      }
      catch(::exception::exception &)
      {

         return false;

      }
      catch(...)
      {

         return false;

      }

      m_spcreate = (sp(::create)) NULL;

      RedrawWindow();

      return true;

   }



   e_position tab::DragHitTest(point pt)
   {
      rect rect;
      GetDragRect(rect, position_top);
      if(rect.contains(pt))
      {
         return position_top;
      }
      GetDragRect(rect, position_bottom);
      if(rect.contains(pt))
      {
         return position_bottom;
      }
      GetDragRect(rect, position_left);
      if(rect.contains(pt))
      {
         return position_left;
      }
      GetDragRect(rect, position_right);
      if(rect.contains(pt))
      {
         return position_right;
      }
      return position_none;
   }

   void tab::GetDragRect(LPRECT lprect, e_position eposition)
   {
      switch(eposition)
      {
      case position_top:
         {
            lprect->top = get_data()->m_rectTabClient.top;
            lprect->left = get_data()->m_rectTabClient.left + get_data()->m_rectTabClient.width() / 3;
            lprect->right = lprect->left + get_data()->m_rectTabClient.width() / 3;
            lprect->bottom = lprect->top + get_data()->m_rectTabClient.height() / 3;
         }
         break;
      case position_left:
         {
            lprect->top = get_data()->m_rectTabClient.top + get_data()->m_rectTabClient.height() / 3;
            lprect->left = get_data()->m_rectTabClient.left;
            lprect->right = lprect->left + get_data()->m_rectTabClient.width() / 3;
            lprect->bottom = lprect->top + get_data()->m_rectTabClient.height() / 3;
         }
         break;
      case position_right:
         {
            lprect->top = get_data()->m_rectTabClient.top + get_data()->m_rectTabClient.height() / 3;
            lprect->left = get_data()->m_rectTabClient.left + get_data()->m_rectTabClient.width() * 2 / 3;
            lprect->right = lprect->left + get_data()->m_rectTabClient.width() / 3;
            lprect->bottom = lprect->top + get_data()->m_rectTabClient.height() / 3;
         }
         break;
      case position_bottom:
         {
            lprect->top = get_data()->m_rectTabClient.top + get_data()->m_rectTabClient.height() * 2 / 3;
            lprect->left = get_data()->m_rectTabClient.left + get_data()->m_rectTabClient.width() / 3;
            lprect->right = lprect->left + get_data()->m_rectTabClient.width() / 3;
            lprect->bottom = lprect->top + get_data()->m_rectTabClient.height() / 3;
         }
         break;
         default:
            break;
      }
   }



   void tab::_001OnDropTab(::index iPane, e_position eposition)
   {
      if(get_data()->m_pcallback != NULL)
      {
         get_data()->m_pcallback->_001OnDropTab(iPane, eposition);
      }
   }


   ::user::interaction * tab::get_view_uie()
   {
      return NULL;
   }


   id tab::get_current_id()
   {
      return get_cur_tab_id();
   }


   void tab::_001OnTimer(::timer * ptimer)
   {

      control::_001OnTimer(ptimer);;

      if(ptimer->m_nIDEvent == 5432187)
      {
         get_data()->m_bDrag = true;
         KillTimer(5432187);
         post_message(message_start_tab_drag);

      }

   }


   void tab::_001OnStartTabDrag(::signal_details * pobj)
   {

      if(get_data()->m_pcallback != NULL)
      {

         get_data()->m_pcallback->_001DropTargetWindowInitialize(this);

      }

   }


   void tab::_000OnMouse(::message::mouse * pmouse)
   {
      
      if(m_bShowTabs)
      {
         
         // these try catchs are needed for multi threading supporting: multi threaded windows: an endeavour
         // Now I understand why Microsoft (TM) Windows (R) windows are single threaded.
         // to debug, enable catch exceptions in debugger
         try
         {

            (m_pimpl->*m_pimpl->m_pfnDispatchWindowProc)(dynamic_cast < signal_details * > (pmouse));

            if (pmouse->get_lresult() != 0)
            {

               return;

            }

         }
         catch(...)
         {

         }

      }
      else if(pmouse->m_uiMessage == WM_MOUSEMOVE)
      {

      }
      
      sp(::user::interaction) pui;

      
      while(rget_child(pui))
      {

         try
         {

            if(pui->IsWindowVisible() && pui->_001IsPointInside(pmouse->m_pt))
            {

               try
               {

                  pui->_000OnMouse(pmouse);

                  if (pmouse->m_bRet)
                  {

                     return;

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

      }

   }


   tab::data * tab::get_data()
   {

      return m_spdata;

   }


   tab_pane * tab::get_pane_by_id(id id)
   {

      return get_data()->get_pane_by_id(id);

   }


   tab_pane * tab::create_pane_by_id(id id)
   {

      create_tab_by_id(id);

      return get_data()->get_pane_by_id(id);

   }


   ::index tab::create_tab_by_id(id id)
   {
      
      if (!add_tab("", id))
      {

         return -1;

      }

      index iTab = id_tab(id);

      if (iTab < 0)
      {

         return -1;

      }

      return iTab;
      
   }

   /// array - if you want, you can hint on_change_pane_count
   /// with added panes info. Pass array with NULL value to force save restorable tabs
   void tab::on_change_pane_count(array < ::user::tab_pane * > array)
   {

      bool bAny = false;

      for (auto ppane : array)
      {

         if (ppane == NULL || matches_restorable_tab(ppane->m_id, ppane->m_pholder))
         {

            bAny = true;

            break;

         }

      }

      if (array.is_empty() || bAny)
      {

         save_restorable_tabs();

      }
      else
      {

         TRACE("Yup! Optimized Out a save_restorable_tabs {?A*dev}op! ...... . .\n");

      }

      set_need_layout();

   }


   void tab::save_restorable_tabs()
   {

      if (m_bDisableSavingRestorableTabs)
         return;

         if (get_data()->m_matchanyRestore.get_count() > 0)
         {

            var_array vara;

            get_restore_tab(vara);

            //if (vara.get_size() > 0)
            //{
            //   TRACE(".");
            //   for (index i = 0; i < 200; i++)
            //   {


                  data_save("restore_tab", vara);
            //   }
            //}
            //else
            //{
            //   TRACE("o");
            //   for (index i = 0; i < 200; i++)
            //   {


            //      data_save("restore_tab", vara);
            //   }

            //}

         }


   }


   void tab::get_text_id(stringa & stra)
   {
      get_presuffixed_ci_id(stra, NULL, NULL);
   }

   void tab::get_prefixed_ci_id(stringa & stra, const char * pszPrefix)
   {
      get_presuffixed_ci_id(stra, pszPrefix, NULL);
   }


   void tab::get_suffixed_ci_id(stringa & stra, const char * pszSuffix)
   {
      
      get_presuffixed_ci_id(stra, NULL, pszSuffix);

   }


   void tab::get_presuffixed_ci_id(stringa & stra, const char * pszPrefix, const char * pszSuffix)
   {
   
      string strPrefix(pszPrefix);

      string strSuffix(pszSuffix);

      string strPath;

      tab_pane_array & panea = get_data()->m_panea;

      for(int32_t i = 0; i < panea.get_count(); i++)
      {

         ::user::tab_pane & tab_pane = panea(i);
         
         strPath = tab_pane.m_id;

         if(strPrefix.is_empty() || ::str::begins_ci(strPath, strPrefix))
         {

            if(strSuffix.is_empty() || ::str::ends_ci(strPath, strSuffix))
            {

               stra.add(strPath);

            }

         }

      }

   }


   void tab::get_begins_ci_eat_id(stringa & stra, const char * pszPrefix)
   {

      get_begins_ends_ci_eat_id(stra, pszPrefix, NULL);

   }


   void tab::get_ends_ci_eat_id(stringa & stra, const char * pszSuffix)
   {

      get_begins_ends_ci_eat_id(stra, NULL, pszSuffix);

   }


   void tab::get_begins_ends_ci_eat_id(stringa & stra, const char * pszPrefix, const char * pszSuffix)
   {

      string strPrefix(pszPrefix);

      string strSuffix(pszSuffix);

      string strPath;

      tab_pane_array & panea = get_data()->m_panea;

      for(int32_t i = 0; i < panea.get_count(); i++)
      {
         
         ::user::tab_pane & tab_pane = panea(i);

         strPath = tab_pane.m_id;

         if(strPrefix.is_empty() || ::str::begins_ci(strPath, strPrefix))
         {

            if(strSuffix.is_empty() || ::str::ends_ci(strPath, strSuffix))
            {

               stra.add(strPath);

            }

         }

      }

   }


   bool tab::matches_restorable_tab(const var & varId, ::user::place_holder * pholder)
   {

      ::core::match::any  & matchany = get_data()->m_matchanyRestore;

      if (pholder != NULL && (bool) pholder->oprop("void_restore"))
      {

         return false;

      }

      if (matchany.get_count() == 0)
      {

         return false;

      }

      if (!matchany.matches(varId))
      {

         return false;

      }

      return true;

   }
   
   void tab::get_restore_tab(var_array & vara)
   {

      var varId;

      tab_pane_array & panea = get_data()->m_panea;

      for(int32_t i = 0; i < panea.get_count(); i++)
      {

         varId = panea[i]->m_id;
         
         if(matches_restorable_tab(varId, panea[i]->m_pholder))
         {

            vara.add(varId);

         }

      }

   }


   bool tab::has_restore_tab()
   {

      ::core::match::any  & matchany = get_data()->m_matchanyRestore;

      if(matchany.get_count() == 0)
         return false;
      var varId;
      tab_pane_array & panea = get_data()->m_panea;
      for(int32_t i = 0; i < panea.get_count(); i++)
      {
         varId = panea[i]->m_id;
         if(matchany.matches(varId))
         {
            return true;
         }
      }

      return false;

   }


   ::count tab::open_tabs(const var_array & vara)
   {

      ::count c = 0;

      stringa stra;

      for(int32_t i = 0; i < vara.get_count(); i++)
      {

         // ODOW : TODO : should create bergedgewrapper to open bergedge inside a window.

         if(vara[i].get_type() == var::type_string && vara[i].get_string() == "app:bergedge")
            continue;

         if(vara[i].get_type() == var::type_string && vara[i].get_string() == "app:")
            continue;

         //if(stra.add_unique(vara[i]) >= 0)
         //{
            //create_tab_by_id(stra.last());

         set_cur_tab_by_id(vara[i].get_id());

         c++;

         //}

      }

      return c;

   }

   
   void tab::_001CloseTab(::index iTab)
   {

      remove_tab_by_id(tab_id(iTab));

   }


   void tab::_001OnShowWindow(signal_details * pobj)
   {

      SCAST_PTR(::message::show_window, pshowwindow, pobj);

      ::user::place_holder * pholder = tab_holder(_001GetSel());

      if (pholder != NULL)
      {

         if (pshowwindow->m_bShow)
         {

            pholder->ShowWindow(SW_SHOW);

         }
         else
         {

            pholder->ShowWindow(SW_HIDE);

         }

      }

   }


} // namespace core


