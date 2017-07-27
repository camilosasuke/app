#include "framework.h"

namespace user
{

   UINT c_cdecl data_update_list_visible_subitem_proc(LPVOID pparam);



   //const UINT list::MESSAGE_ENDCOLUMNHEADERDRAG = WM_USER + 25;
   //const UINT list::MESSAGE_COLUMNHEADERTRACK = WM_USER + 26;
   //const UINT list::MESSAGE_ENDCOLUMNHEADERTRACK = WM_USER + 27;

   list::list() :
      m_columna(get_app())
   {

      m_iImageSpacing = 2;
      m_dIconSaturation = 1.0;

      m_iIconBlur = 0;

      m_iIconBlurRadius = 0;

      m_iTextSpreadRadius = 0;

      m_iTextBlurRadius = 0;

      m_iTextBlur = 0;

      m_plist = this;

      m_eview = view_report;

      m_columna.Initialize(this);

      m_bHeaderCtrl = true;
      m_bSingleColumnMode = false;
      //m_psimplemeshdata          = NULL;

      m_pdrawlistitem = NULL;


      m_bAutoCreateListHeader = true;


      m_bMorePlain = false;

      m_bSelect = true;

   }

   list::~list()
   {
      ::aura::del(m_pdrawlistitem);
   }


   void list::install_message_handling(::message::dispatch * pinterface)
   {

      ::user::mesh::install_message_handling(pinterface);

      IGUI_WIN_MSG_LINK(WM_SIZE, pinterface, this, &list::_001OnSize);
      IGUI_WIN_MSG_LINK(WM_VSCROLL, pinterface, this, &list::_001OnVScroll);
      IGUI_WIN_MSG_LINK(WM_HSCROLL, pinterface, this, &list::_001OnHScroll);
      IGUI_WIN_MSG_LINK(WM_MOUSELEAVE, pinterface, this, &list::_001OnMouseLeave);

      IGUI_WIN_MSG_LINK(WM_LBUTTONDOWN, pinterface, this, &list::_001OnLButtonDown);
      IGUI_WIN_MSG_LINK(WM_LBUTTONUP, pinterface, this, &list::_001OnLButtonUp);
      IGUI_WIN_MSG_LINK(WM_LBUTTONDBLCLK, pinterface, this, &list::_001OnLButtonDblClk);
      IGUI_WIN_MSG_LINK(WM_RBUTTONDOWN, pinterface, this, &list::_001OnRButtonDown);

      IGUI_WIN_MSG_LINK(WM_MOUSEMOVE, pinterface, this, &list::_001OnMouseMove);

      IGUI_WIN_MSG_LINK(WM_KEYDOWN, pinterface, this, &list::_001OnKeyDown);

      IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &list::_001OnCreate);
      //      //IGUI_WIN_MSG_LINK(WM_TIMER,           pinterface, this, &list::_001OnTimer);
      connect_command("list_view_auto_arrange", &list::_001OnListViewAutoArrange);
      connect_update_cmd_ui("list_view_auto_arrange", &list::_001OnUpdateListViewAutoArrange);
   }

   bool list::CreateHeaderCtrl()
   {
      if (m_plistheader != NULL)
      {
         if (!m_plistheader->IsWindow())
         {
            bool bOk = m_plistheader->create_window(
               NULL,
               "",
               WS_CHILD
               | WS_VISIBLE
               | HDS_FULLDRAG
               | HDS_HOTTRACK
               | HDS_DRAGDROP,
               null_rect(),
               this,
               1023) != 0;
            //if (bOk)
            //{
            //   m_plistheader->m_font = m_font;
            //}
            return bOk;
         }
         else
            return true;
         return false;
      }
      return false;
   }

   void list::_001OnNcDraw(::draw2d::graphics * pgraphics)
   {

      ::user::mesh::_001OnNcDraw(pgraphics);

      defer_draw_scroll_gap(pgraphics);


   }


   void list::_001OnDraw(::draw2d::graphics * pgraphics)
   {

      //single_lock sl(&m_mutex, true);

      m_penFocused->create_solid(2, ARGB(255, 0, 255, 255));

      m_penHighlight->create_solid(2, ARGB(255, 0, 255, 255));

      pgraphics->set_text_rendering(::draw2d::text_rendering_anti_alias_grid_fit);

      if (m_bLockViewUpdate)
         return;

      ::user::interaction::_001OnDraw(pgraphics);

      synch_lock sl(m_pmeshdata->m_pmutex);

      rect rectClient;

      GetClientRect(rectClient);

      point ptOffset = get_viewport_offset();

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

     pgraphics->FillSolidRect(rectClient, _001GetColor(color_list_background));


      //      pgraphics->SetBkMode(TRANSPARENT);

      if (m_bTopText)
      {

         ::draw2d::brush_sp brushText(allocer());

         brushText->create_solid(_001GetColor(color_text));

         point ptViewportOrg = pgraphics->GetViewportOrg();
         pgraphics->SelectObject(brushText);
         array < size > sizea;
         m_dcextension.GetTextExtent(pgraphics, m_strTopText, sizea);
         index x = 0;
         index right = (index)rectClient.right;
         index y = m_iItemHeight;
         index iStart = 0;
         index iNewStart = 0;
         index w;
         for (index i = 0; i < sizea.get_size(); i++)
         {

            if ((sizea[i].cx - x > right)
               || i == sizea.get_upper_bound())
            {
               rect rect;
               rect.top = LONG(y - ptOffset.y);
               if (i == 0)
               {
                  w = sizea[0].cx - x;
                  x = sizea[0].cx;
                  y += sizea[0].cy;
                  iNewStart = 0;

               }
               else
               {
                  w = sizea[i - 1].cx - x;
                  x = sizea[i - 1].cx;
                  y += sizea[i - 1].cy;
                  iNewStart = i - 1;
               }
               rect.left = LONG(-ptOffset.x);
               rect.right = rectClient.right;
               rect.bottom = LONG(y - ptOffset.y);

               pgraphics->_DrawText(m_strTopText.Mid(iStart, i - iStart), rect, DT_LEFT);
               iStart = iNewStart;
            }
         }
         pgraphics->SetViewportOrg(ptViewportOrg);
      }


      //      if(m_pmeshdata != NULL)
      //      {
      //
      //         if(m_pmeshdata->is_locked())
      //         {
      //
      //            return;
      //
      //         }
      //
      //      }
      //

      if (m_pdrawlistitem == NULL)
      {

         return;

      }


      m_pdrawlistitem->m_pgraphics = pgraphics;
      m_pdrawlistitem->m_iItemRectItem = -1;
      m_pdrawlistitem->m_iSubItemRectOrder = -1;
      m_pdrawlistitem->m_iSubItemRectSubItem = -1;
      m_pdrawlistitem->m_iSubItemRectItem = -1;
      m_pdrawlistitem->m_iSubItemRectColumn = -1;
      //m_pdrawlistitem->m_pcolumnSubItemRect = NULL;
      //m_pdrawlistitem->m_pcolumnWidth = NULL;
      m_pdrawlistitem->m_iColumn = -1;
      m_pdrawlistitem->m_iColumnWidth = 0;

      rect rectItem;
      rect rectIntersect;
      int_ptr iItemCount = m_nDisplayCount;
      int_ptr iItemFirst = m_iTopIndex;
      int_ptr iItemLast;
      if (m_eview == view_icon)
      {
         iItemLast = iItemCount;
      }
      else
      {
         iItemLast = MIN(m_nItemCount - 1, iItemFirst + m_nDisplayCount - 1);
      }

      if (iItemFirst < 0)
         return;

      _001DrawItems(m_pdrawlistitem, iItemFirst, iItemLast);

      if (m_bGroup && m_bLateralGroup)
      {

         int32_t iGroupFirst = 0;
         int32_t iGroupFirstTopIndex = 0;
         for (; iGroupFirst < m_nGroupCount; iGroupFirst++)
         {
            if (iItemFirst >= iGroupFirstTopIndex && iItemFirst < (iGroupFirstTopIndex + _001GetGroupItemCount(iGroupFirst)))
               break;
         }
         int32_t iGroupLast = iGroupFirst;
         int32_t iGroupLastTopIndex = iGroupFirstTopIndex;
         for (; iGroupLast < m_nGroupCount; iGroupLast++)
         {
            if (iItemLast >= iGroupLastTopIndex && iItemLast < (iGroupLastTopIndex + _001GetGroupItemCount(iGroupLast)))
               break;
         }

         _001DrawGroups(m_pdrawlistitem, iGroupFirst, iGroupLast, iItemFirst, iItemLast);
      }

      if (m_eview == view_icon)
      {

         if (m_bDrag && m_iItemMouseDown < 0)
         {

            int x1 = m_ptLButtonDown.x;
            int x2 = m_ptLButtonUp.x;

            ::sort::sort(x1, x2);

            int y1 = m_ptLButtonDown.y;
            int y2 = m_ptLButtonUp.y;

            ::sort::sort(y1, y2);

            pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);
            pgraphics->Draw3dRect(x1, y1, x2 - x1, y2 - y1, ARGB(184, 184, 184, 208), ARGB(184, 184, 184, 208));
            pgraphics->FillSolidRect(x1, y1, x2 - x1, y2 - y1, ARGB(84, 250, 250, 255));

         }

      }

   }



















   void list::_001DrawGroups(draw_list_item * pdrawitem, index iGroupFirst, index iGroupLast, index iItemFirst, index iItemLast)
   {

      UNREFERENCED_PARAMETER(iItemFirst);
      UNREFERENCED_PARAMETER(iItemLast);

      index iGroup;

      rect rectClient;

      rect rectIntersect;

      GetClientRect(rectClient);

      bool bHoverFont = false;
      select_font(pdrawitem->m_pgraphics, font_list_item, this);
      //pdrawitem->m_pgraphics->SelectObject(pfont);

      m_pdrawlistitem->m_pfont = pdrawitem->m_pgraphics->m_spfont;

      for (iGroup = iGroupFirst; iGroup <= iGroupLast; iGroup++)
      {

         m_pdrawlistitem->m_iGroup = iGroup;

         _001GetGroupRect(m_pdrawlistitem);

         if (!m_pdrawlistitem->m_bOk)
            continue;

         if (!rectIntersect.intersect(m_pdrawlistitem->m_rectItem, rectClient))
            continue;

         if (iGroup == m_iGroupHover)
         {
            if (!bHoverFont)
            {
               select_font(pdrawitem->m_pgraphics, font_list_hover, this);
               //pdrawitem->m_pgraphics->SelectObject(m_pdrawlistitem->m_pfont);
            }
         }
         else
         {
            if (bHoverFont)
            {
               //m_pdrawlistitem->m_pfont = pfont;
               //pdrawitem->m_pgraphics->SelectObject(pfont);
               select_font(pdrawitem->m_pgraphics, font_list_item, this);
            }
         }

         _001DrawGroup(m_pdrawlistitem);

      }

   }



















   void list::_001DrawGroup(draw_list_item * pdrawitem)
   {

      /*if(m_bGroupCustomDraw)
      {
         pdrawitem->m_bListSubItemHover = pdrawitem->m_bListItemHover && (pdrawitem->m_iSubItem == m_iSubItemHover);
         pdrawitem->m_bFocus = false;
         return;
      }*/

      _001GetElementRect(pdrawitem, ::user::list::element_group_image);
      if (pdrawitem->m_bOk)
      {
         pdrawitem->draw_group_image();
      }

      ::count nItem = _001GetGroupMetaItemCount(pdrawitem->m_iGroup);

      for (pdrawitem->m_iItem = 0; pdrawitem->m_iItem < nItem; pdrawitem->m_iItem++)
      {
         _001GetElementRect(pdrawitem, ::user::list::element_group_item_text);
         if (pdrawitem->m_bOk)
         {
            _001GetGroupText(pdrawitem);
            pdrawitem->draw_text();
         }
      }



   }
















   void list::_001DrawItems(draw_list_item * pdrawitem, index iItemFirst, index iItemLast)
   {

      rect rectClient;

      GetClientRect(rectClient);

      rect rectIntersect;

      index iItem;

      m_pdrawlistitem->m_iDrawTextFlags = _001GetDrawTextFlags(m_eview);

      bool bHoverFont = false;
      select_font(pdrawitem->m_pgraphics, font_list_item, this);

      pdrawitem->m_pfont = pdrawitem->m_pgraphics->m_spfont;

      for (iItem = iItemFirst; iItem <= iItemLast; iItem++)
      {
         if (m_eview == view_icon
            && (m_iconlayout.m_iaDisplayToStrict.get_b(iItem) == -1
               && iItem != m_iItemDrop))
            continue;

         m_pdrawlistitem->m_iItem = DisplayToStrict(iItem);
         m_pdrawlistitem->m_iDisplayItem = iItem;

         if (m_bGroup)
         {
            m_pdrawlistitem->m_iGroupTopIndex = 0;
            //            int32_t igroup;
            for (m_pdrawlistitem->m_iGroup = 0; m_pdrawlistitem->m_iGroup < m_nGroupCount; m_pdrawlistitem->m_iGroup++)
            {
               m_pdrawlistitem->m_iGroupCount = _001GetGroupItemCount(m_pdrawlistitem->m_iGroup);
               if (iItem >= m_pdrawlistitem->m_iGroupTopIndex && iItem < (m_pdrawlistitem->m_iGroupTopIndex + m_pdrawlistitem->m_iGroupCount))
                  break;
            }
         }

         _001GetItemRect(m_pdrawlistitem);

         if (!m_pdrawlistitem->m_bOk)
            continue;

         if (!rectIntersect.intersect(m_pdrawlistitem->m_rectItem, rectClient))
            continue;

         if (iItem == m_iItemHover)
         {
            if (!bHoverFont)
            {
               bHoverFont = true;
               select_font(pdrawitem->m_pgraphics, font_list_hover, this);
            }
         }
         else
         {
            if (bHoverFont)
            {
               bHoverFont = false;
               select_font(pdrawitem->m_pgraphics, font_list_item, this);
            }
         }

         _001DrawItem(m_pdrawlistitem);

      }
   }



   void list::_001DrawItem(draw_list_item * pdrawitem)
   {

      pdrawitem->m_iState = 0;

      range & rangeSelection = m_rangeSelection;
      range & rangeHighlight = m_rangeHighlight;


      if (pdrawitem->m_iItem < 0)
         return;


      pdrawitem->m_bListItemHover = pdrawitem->m_iDisplayItem == m_iItemHover &&
         (m_eview != view_icon ||
         ((m_iconlayout.m_iaDisplayToStrict.get_b(m_iItemHover) >= 0 && m_iconlayout.m_iaDisplayToStrict.get_b(m_iItemHover) < m_nItemCount)));

      if (pdrawitem->m_bListItemHover)
      {

         if (!pdrawitem->m_plist->m_bMorePlain)
         {

            pdrawitem->m_pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);
            pdrawitem->m_pgraphics->Draw3dRect(pdrawitem->m_rectItem, ARGB(77, 235, 235, 255), ARGB(77, 235, 235, 255));
            pdrawitem->m_pgraphics->FillSolidRect(pdrawitem->m_rectItem, ARGB(40, 255, 255, 255));

         }

      }

      pdrawitem->m_bListItemSelected = (m_eview != view_icon || is_valid_display_item(pdrawitem->m_iItem)) && rangeSelection.has_item(pdrawitem->m_iItem);

      if (pdrawitem->m_bListItemHover)
         pdrawitem->m_iState |= ItemStateHover;
      if (pdrawitem->m_bListItemSelected)
         pdrawitem->m_iState |= ItemStateSelected;

      pdrawitem->m_crText = 0;
      pdrawitem->m_crTextBackground = 0;
      pdrawitem->m_crItemBackground = 0;
      pdrawitem->update_item_color();
      pdrawitem->set_text_color();


      if (pdrawitem->m_bListItemSelected)
      {
         if (pdrawitem->m_plist->m_bMorePlain)
         {

         }
         else if (Session.savings().is_trying_to_save(::aura::resource_processing))
         {
            pdrawitem->m_pgraphics->FillSolidRect(pdrawitem->m_rectItem, ARGB(255, 96, 96, 96));
         }
         else
         {
            rect rectClient;
            GetClientRect(rectClient);
            ::rect r = pdrawitem->m_rectItem;
            r.inflate(8, 0, 8, -1);
            r.right = rectClient.right;

            pdrawitem->m_pgraphics->FillSolidRect(r, _001GetColor(::user::color_list_item_background));
            //System.visual().imaging().color_blend(pdrawitem->m_pgraphics, r, crTranslucid, 127);
         }
      }
      else if (pdrawitem->m_crItemBackground != 0)
      {
         pdrawitem->m_pgraphics->FillSolidRect(pdrawitem->m_rectItem, pdrawitem->m_crItemBackground);
      }


      string str;

      index iColumnCount;

      if (m_eview == view_icon)
      {
         iColumnCount = 1;
      }
      else
      {
         iColumnCount = m_columna.VisibleGetCount();
      }

      pdrawitem->m_iSubItemRectOrder = -1;
      pdrawitem->m_iSubItemRectSubItem = -1;
      pdrawitem->m_iSubItemRectColumn = -1;
//      pdrawitem->m_pcolumnSubItemRect = NULL;

      for (index iVisible = 0; iVisible < iColumnCount; iVisible++)
      {

         pdrawitem->m_iColumn = iVisible;

         pdrawitem->m_iOrder = _001MapColumnToOrder(iVisible);

         if (pdrawitem->m_iOrder < 0)
            continue;


         pdrawitem->m_pcolumn = m_columna.get_visible(pdrawitem->m_iColumn);

         if (pdrawitem->m_pcolumn != NULL)
         {

            pdrawitem->m_iSubItem = pdrawitem->m_pcolumn->m_iSubItem;

         }

         _001GetSubItemRect(pdrawitem);

         if (!pdrawitem->m_bOk)
            continue;


         _001DrawSubItem(pdrawitem);

      }
      if (rangeHighlight.has_item(pdrawitem->m_iDisplayItem))
      {
         ::draw2d::pen_sp penHighlight(allocer());
         ::draw2d::pen * ppenHighlight = _001GetPenHighlight();
         class rect rectHighlight(pdrawitem->m_rectItem);
         rectHighlight.inflate(8, 0, 8, -1);
         pdrawitem->m_pgraphics->SelectObject(ppenHighlight);
         pdrawitem->m_pgraphics->DrawRectangle(rectHighlight);
      }


      if (pdrawitem->m_bListItemSelected)
      {
         if (pdrawitem->m_plist->m_bMorePlain)
         {
            ::rect r = pdrawitem->m_rectItem;
            COLORREF cr;
            if (!get_color(cr, color_text))
            {
               cr = ARGB(255, 96, 96, 96);
            }
            pdrawitem->m_pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);
            pdrawitem->m_pgraphics->FillSolidRect(pdrawitem->m_rectItem, ARGB(84, argb_get_r_value(cr), argb_get_g_value(cr), argb_get_b_value(cr)));

         }

      }

   }


   void   list::_001DrawSubItem(draw_list_item * pdrawitem)
   {

      if (pdrawitem->m_pcolumn->m_bCustomDraw)
      {
         pdrawitem->m_bListSubItemHover = pdrawitem->m_bListItemHover && (pdrawitem->m_iSubItem == m_iSubItemHover);
         pdrawitem->m_bFocus = false;
         return;
      }

      //::draw2d::font * pfont;
      if (pdrawitem->m_bListItemHover)
      {
         select_font(pdrawitem->m_pgraphics, font_list_hover, this);
      }
      else
      {
         select_font(pdrawitem->m_pgraphics, font_list_item, this);
      }
//      pdrawitem->m_pgraphics->set_font(pfont);


      pdrawitem->m_iListItem = -1;
      _001GetElementRect(pdrawitem, ::user::list::element_image);
      if (pdrawitem->m_bOk)
      {
         try
         {
            _001GetItemImage(pdrawitem);
         }
         catch (...)
         {
            pdrawitem->m_bOk = false;
         }
         pdrawitem->draw_image();
      }
      pdrawitem->m_iListItem = -1;
      _001GetElementRect(pdrawitem, ::user::mesh::element_text);

      if (pdrawitem->m_bOk)
      {
         try
         {
            _001GetItemText(pdrawitem);
         }
         catch (...)
         {
            pdrawitem->m_bOk = false;
         }
         if (pdrawitem->m_bOk)
         {
            pdrawitem->draw_text();
         }
      }

   }




   //void list::_001GetItemImage(::user::mesh_item * pitem)
   //{
   //
   //   if(m_pmeshdata != NULL)
   //   {

   //      pitem->m_plist = this;
   //      try
   //      {
   //         return m_pmeshdata->_001GetItemImage(pitem);
   //      }
   //      catch(...)
   //      {
   //         TRACE("_001GetItemImage exception");
   //      }
   //   }
   //   /*LVITEM lvi;
   //   lvi.mask = LVIF_IMAGE;
   //   lvi.iItem = iItem;
   //   lvi.iSubItem = 0;
   //   get_item(&lvi);
   //   return lvi.iImage;*/
   //   pitem->m_bOk = false;
   //}

   //void list::_001GetItemText(::user::mesh_item * pitem)
   //{
   //   pitem->m_bOk = false;
   //   if(m_pcache != NULL)
   //   {
   //      m_pcache->_001GetItemText(pitem);
   //      if(pitem->m_bOk)
   //         return;
   //   }
   //   if(m_pmeshdata != NULL)
   //   {
   //      m_pmeshdata->_001GetItemText(pitem);
   //   }
   //}

   //// Purpose:
   //// Provide Item Text for Search Purposes.
   //void list::_001SearchGetItemText(::user::mesh_item * pitem)
   //{
   //   _001GetItemText(pitem);
   //}


   ::count list::_001GetColumnCount()
   {

      return m_columna.VisibleGetCount();

   }




   void list::_001OnSize(signal_details * pobj)
   {

      SCAST_PTR(::message::size, psize, pobj);

   }


   void list::on_layout()
   {

      synch_lock sl(m_pmutex);

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

      if (m_bTopText)
      {

         _001LayoutTopText();

      }

      if (m_eview == view_icon)
      {

         {
            rect rectClient;
            GetClientRect(rectClient);
            index iIconSize;
            if (m_columna.get_count() > 0)
               iIconSize = MAX(32, m_columna[0]->m_sizeIcon.cy);
            else
               iIconSize = 32;
            index iItemSize = iIconSize * 2;

         }

      }



      ::count iCount = m_nItemCount;
      int32_t iMaxWidth = 0;
      int32_t iWidth;
      if (m_bSingleColumnMode)
      {
         for (index i = 0; i < iCount; i++)
         {
            iWidth = _001CalcItemWidth(i, 0);
            if (iWidth > iMaxWidth)
            {
               iMaxWidth = iWidth;
            }
         }
         m_iItemWidth = iMaxWidth;
         m_columna.get_visible(0)->m_iWidth = iMaxWidth;
      }

      on_change_view_size();

      LayoutHeaderCtrl();

      update_icon_list_view_sort();

   }


   bool list::_001OnUpdateItemCount(uint32_t dwFlags)
   {



      UNREFERENCED_PARAMETER(dwFlags);

      point ptOffset = get_viewport_offset();

      ::count nCount = _001GetItemCount();
      
      if (nCount < 0)
      {

         m_nItemCount = nCount;

         return false;

      }

      ::count nGroupCount = 1;

      if (m_bGroup)
      {
         nGroupCount = _001GetGroupCount();
         if (nGroupCount < 0)
         {
            m_nGroupCount = nGroupCount;
            return false;
         }
      }

      {

         synch_lock sl(m_pmutex);

         m_nItemCount = nCount;

         if (m_bGroup)
         {
            m_nGroupCount = nGroupCount;
         }

         if (m_eview == view_icon)
         {

            update_icon_list_view_sort();

         }
         else
         {

            index iStart = m_meshlayout.m_iaDisplayToStrict.get_count();

            index iEnd = m_nItemCount - 1;

            m_meshlayout.m_iaDisplayToStrict.allocate(m_nItemCount);

            for (index iStrict = iStart; iStrict <= iEnd; iStrict++)
            {

               m_meshlayout.m_iaDisplayToStrict.set_at(iStrict, iStrict);

            }

         }

      }

      if (m_bFilter1)
      {

         FilterApply();

      }

      CacheHint();

      //layout_scroll_bar();

      on_change_view_size();

      TRACE("list::_001OnUpdateItemCount ItemCount %d\n", m_nItemCount);

      if (m_bGroup)
      {

         TRACE("list::_001OnUpdateItemCount GroupCount %d\n", m_nGroupCount);

      }

      set_viewport_offset(ptOffset.x, ptOffset.y);



      RedrawWindow();

      return true;

   }


   void list::on_change_view_size()
   {

      m_iTopIndex = _001CalcDisplayTopIndex();

      index iLow = 0;
      for (m_iTopGroup = 0; m_iTopGroup < m_nGroupCount; m_iTopGroup++)
      {
         if (m_iTopIndex >= iLow && m_iTopIndex < (iLow + _001GetGroupItemCount(m_iTopGroup)))
            break;
      }

      m_nDisplayCount = _001CalcDisplayItemCount();


      rect rect;
      if (m_eview == view_list)
      {
         if (m_nItemCount == 0)
         {
            rect = ::rect(0, 0, 0, 0);
         }
         else
         {
            ::rect rectClient;

            GetClientRect(&rectClient);

            draw_list_item itemFirst(this);

            itemFirst.m_iItem = 0;
            itemFirst.m_iDisplayItem = 0;

            _001GetItemRect(&itemFirst);


            rect.top = rectClient.top;
            rect.bottom = rectClient.bottom;
            rect.left = rectClient.left;
            if (m_iItemHeight <= 0 || rectClient.height() <= 0)
               rect.right = rectClient.right;
            else
               rect.right = (LONG)MIN(
                  rectClient.left +
                  m_nItemCount * itemFirst.m_rectItem.width() * m_iItemHeight /
                  rectClient.height()
                  + itemFirst.m_rectItem.width(), MAXLONG);
         }
      }
      else if (m_eview == view_report)
      {
         if (m_nItemCount == 0)
         {
            rect = ::rect(0, 0, 0, 0);
         }
         else
         {
            draw_list_item itemFirst(this);

            itemFirst.m_iItem = 0;
            itemFirst.m_iDisplayItem = 0;
            itemFirst.m_iGroup = 0;
            _001GetItemRect(&itemFirst);

            rect = itemFirst.m_rectItem;

            rect.top = 0;

            rect.bottom = (_001GetItemCount() + (m_bHeaderCtrl ? 1 : 0)) * m_iItemHeight;

            //            draw_list_item itemLast(this);
            //
            //            if(m_bFilter1)
            //            {
            //               itemLast.m_iDisplayItem       = m_piaFilterMesh->get_count() - 1;
            //               itemLast.m_iItem              = m_piaFilterMesh->get_count() - 1;
            //            }
            //            else
            //            {
            //               itemLast.m_iDisplayItem       = m_nItemCount - 1;
            //               itemLast.m_iItem              = m_nItemCount - 1;
            //            }
            //            if(m_bGroup)
            //            {
            //               itemLast.m_iGroupTopIndex = 0;
            ////               int32_t igroup;
            //               for(itemLast.m_iGroup = 0; itemLast.m_iGroup < m_nGroupCount; itemLast.m_iGroup++)
            //               {
            //                  itemLast.m_iGroupCount = _001GetGroupItemCount(itemLast.m_iGroup);
            //                  if(itemLast.m_iItem >= itemLast.m_iGroupTopIndex && itemLast.m_iItem < (itemLast.m_iGroupTopIndex + itemLast.m_iGroupCount))
            //                     break;
            //               }
            //            }
            //            _001GetItemRect(&itemLast);

                        //::rect rectMargin;

                        //get_margin_rect(rectMargin);

                        //itemLast.m_rectItem.right     -= (rectMargin.left + rectMargin.right);
                        //itemLast.m_rectItem.bottom    -= (rectMargin.top  + rectMargin.bottom);
            //            itemFirst.m_rectItem.top = 0;
              //          rect.unite(itemFirst.m_rectItem, itemLast.m_rectItem);
         }
      }
      else if (m_eview == view_icon)
      {
         rect = ::rect(0, 0, 0, 0);
         draw_list_item itemFirst(this);

         itemFirst.m_iItem = 0;
         itemFirst.m_iDisplayItem = 0;

         _001GetItemRect(&itemFirst);

         if (itemFirst.m_bOk)
         {

            draw_list_item itemLast(this);

            if (m_nItemCount <= 0)
            {

               itemLast.m_iDisplayItem = 0;
               itemLast.m_iItem = 0;

            }
            else
            {

               itemLast.m_iDisplayItem = m_nItemCount - 1;
               itemLast.m_iItem = m_nItemCount - 1;

            }


            _001GetItemRect(&itemLast);

            if (itemLast.m_bOk)
            {

               draw_list_item itemTopRight(this);
               //               if (m_flags.is_signalized(flag_auto_arrange) || m_iconlayout.m_iWidth <= 0)
               {
                  class rect rectClient;

                  GetClientRect(&rectClient);

                  itemTopRight.m_iItem = (index)MAX(1, rectClient.width() / get_item_size().cx) - 1;
               }
               /*      else
                     {
                        itemTopRight.m_iItem = MAX(1, m_iconlayout.m_iWidth) - 1;
                     }*/
               itemTopRight.m_iDisplayItem = itemTopRight.m_iItem;
               _001GetItemRect(&itemTopRight);

               if (itemTopRight.m_bOk)
               {

                  rect.unite(itemFirst.m_rectItem, itemLast.m_rectItem);
                  rect.unite(rect, itemTopRight.m_rectItem);

               }
            }
         }
      }

      m_sizeTotal = rect.size();

      ::user::box::on_change_view_size();

   }


   void list::_001OnInitialize()
   {

   }




   void list::_001AddColumn(list_column & column)
   {

      m_columna.add(column);

      column.m_bCustomDraw = false;
      column.m_iControl = (UINT)-1;
      column.m_uiSmallBitmap = (UINT)-1;
      column.m_bIcon = false;
      column.m_pil = NULL;

   }

   //
   // list_column
   //

   list_column::list_column()
   {
      m_uiSmallBitmap = (UINT)-1;
      m_iOrder = -1;
      m_bVisible = true;
      m_pil = NULL;
      m_iControl = -1;
      m_bEditOnSecondClick = false;
      m_bCustomDraw = false;
      m_bIcon = false;
      m_pil = NULL;
      m_pilHover = NULL;
      m_sizeIcon.cx = 0;
      m_sizeIcon.cy = 0;
   }
   list_column::list_column(const list_column & column)
   {
      operator =(column);
   }
   list_column::~list_column()
   {
   }

   ::index list_column::get_index()
   {

      return m_pcontainer->get_index(this);

   }

   ::index list_column::get_visible_index()
   {

      return m_pcontainer->get_visible_index(this);

   }

   bool list_column::operator ==(const list_column & column) const
   {

      return m_iSubItem == column.m_iSubItem;

   }



   int_ptr list_column::CompareOrderSectEndNonVisible(const sp(list_column) & pcolumna, const sp(list_column) & pcolumnb)
   {

      if (pcolumna->m_bVisible && pcolumnb->m_bVisible)
      {

         return pcolumna->m_iOrder - pcolumnb->m_iOrder;

      }
      else if (pcolumna->m_bVisible)
      {

         return -1;

      }
      else if (pcolumnb->m_bVisible)
      {

         return 1;

      }
      else
      {

         return 0;

      }

   }


   int_ptr list_column::CompareOrder(const sp(list_column) & pcolumna, const sp(list_column) & pcolumnb)
   {

      return pcolumna->m_iOrder - pcolumnb->m_iOrder;

   }


   int_ptr list_column::CompareKey(const sp(list_column) & pcolumna, const sp(list_column) & pcolumnb)
   {
   
      return pcolumna->get_index() - pcolumnb->get_index();

   }


   list_column & list_column::operator = (const list_column & column)
   {
      m_iOrder = column.m_iOrder;
      m_iSubItem = column.m_iSubItem;
      m_uiText = column.m_uiText;
      m_iWidth = column.m_iWidth;
      m_uiSmallBitmap = column.m_uiSmallBitmap;
      m_crSmallMask = column.m_crSmallMask;
      m_iSmallImageWidth = column.m_iSmallImageWidth;
      m_pil = column.m_pil;
      m_pilHover = column.m_pilHover;
      m_bVisible = column.m_bVisible;
      m_iControl = column.m_iControl;
      m_datakey = column.m_datakey;
      m_bIcon = column.m_bIcon;
      m_sizeIcon = column.m_sizeIcon;
      m_bCustomDraw = column.m_bCustomDraw;
      m_mapIcon = column.m_mapIcon;
      m_bEditOnSecondClick = column.m_bEditOnSecondClick;
      m_dibHeader = column.m_dibHeader;
      return *this;
   }


   void list::data_update_visible_subitem()
   {

      return;

      synch_lock sl(&m_mutexData);

      index iColumn;

      index_array iaVisible;

      data_load("VisibleSubItem", iaVisible);

      for(index iColumn = 0; iColumn < m_columna.get_count(); iColumn++)
      {

         list_column * pcolumn = m_columna[iColumn];

         if (pcolumn != NULL)
         {

            pcolumn->m_bVisible = iaVisible.contains(iColumn);

         }

      }

   }


   UINT c_cdecl data_update_list_visible_subitem_proc(LPVOID pparam)
   {

      list * plist = (list *)pparam;

      plist->data_update_visible_subitem();

      return 0;

   }


   int32_t list::_001CalcItemHeight(int iBaseHeight)
   {

      return iBaseHeight;

   }


   void list::_001OnColumnChange()
   {

      index iItemHeight = 0;
      index iItemWidth = 0;

      rect rect;

      string str;
      index iColumn;
      int_ptr iColumnWidth;


      __begin_thread(get_app(), data_update_list_visible_subitem_proc, this);


      image_list::info ii;

      for (iColumn = 0; iColumn < m_columna.VisibleGetCount(); iColumn++)
      {
         list_column * pcolumn = m_columna.get_visible(iColumn);
         iColumnWidth = pcolumn->m_iWidth;
         _001CreateImageList(pcolumn);
         if (iColumnWidth >= 0)
         {
            iItemWidth += iColumnWidth;
         }
         if (pcolumn->m_pil != NULL
            && pcolumn->m_pil->get_image_count() > 0)
         {
            pcolumn->m_pil->get_image_info(0, &ii);
            rect = ii.m_rect;
            if (rect.height() + 2 > iItemHeight)
            {
               iItemHeight = (index)rect.height() + 2;
            }
         }
         if (pcolumn->m_sizeIcon.cy + 2 > iItemHeight)
         {
            iItemHeight = pcolumn->m_sizeIcon.cy + 2;
         }
      }

      ::draw2d::memory_graphics pgraphics(allocer());
      select_font(pgraphics, font_list_item, this);
      size size;
      size = pgraphics->GetTextExtent(unitext("Ág"));
      iItemHeight = 1;
      if (size.cy > iItemHeight)
      {
         iItemHeight = MAX(size.cy, iItemHeight);
      }

      m_iItemHeight = _001CalcItemHeight((int)iItemHeight);

      on_ui_event(event_calc_item_height, object_list, this);

//      m_iVScrollOffset = m_iItemHeight;

      m_iItemWidth = (int32_t)iItemWidth;

      if (m_bSingleColumnMode)
      {
         m_iItemWidth = _001CalcColumnWidth(0);
      }

      if (m_plistheader != NULL && m_plistheader->IsWindow())
      {
         //      while(m_plistheader->DeleteItem(0));

         ::user::list_header::item hditem;

         for (index iOrder = 0; iOrder < m_columna.VisibleGetCount(); iOrder++)
         {
            iColumn = _001MapOrderToColumn(iOrder);
            if (iColumn < 0)
               continue;
            list_column * pcolumn = m_columna.get_visible(iColumn);
            //hditem.mask = HDI_WIDTH | HDI_TEXT | HDI_LPARAM | HDI_ORDER;
            //str.load_string(_001GetColumnTextId(iColumn));
            //hditem.pszText = (LPTSTR) (const char *) str;
            //hditem.pszText = LPSTR_TEXTCALLBACK;
            //hditem.cchTextMax = str.get_length();
            hditem.cchTextMax = 0;
            hditem.cxy = pcolumn->m_iWidth;;
            hditem.lParam = iColumn;
            hditem.iOrder = (int32_t)iOrder;

         }

      }

   }


   bool list::_001SetColumnWidth(index iColumn, int32_t iWidth)
   {

      if (iColumn < 0)
      {

         return false;

      }

      if (iColumn >= m_columna.VisibleGetCount())
      {

         return false;

      }

      m_columna.get_visible(iColumn)->m_iWidth = iWidth;

      m_plistheader->DIDDXColumn(true);

      _001OnColumnChange();

      return true;

   }

   void list::_001GetColumnWidth(draw_list_item * pitem)
   {
      auto  * pcolumn = m_columna.get_visible(pitem->m_iColumn);
      if (pcolumn == NULL)
      {
         pitem->m_iColumnWidth = 0;
         pitem->m_bOk = false;
      }
      else
      {
         pitem->m_iSubItem = pcolumn->m_iSubItem;
         pitem->m_iColumnWidth = pcolumn->m_iWidth;
         pitem->m_bOk = true;
      }
   }


   index list::_001MapSubItemToOrder(index iSubItem)
   {
      return _001MapColumnToOrder(_001MapSubItemToColumn(iSubItem));
   }

   index list::_001MapOrderToSubItem(index iOrder)
   {
      return _001MapColumnToSubItem(_001MapOrderToColumn(iOrder));
   }

   index list::_001MapOrderToColumn(index iOrder)
   {
      
      for (index iColumn = 0; iColumn < m_columna.get_size(); iColumn++)
      {
         list_column * pcolumn = m_columna.element_at(iColumn);
         if (pcolumn->m_bVisible)
         {
            if (pcolumn->m_iOrder == iOrder)
            {
               return iColumn;
            }

         }
      }
      return -1;
   }

   index list::_001MapColumnToOrder(index iColumn)
   {
      if (iColumn < 0)
         return -1;
      if (iColumn >= m_columna.VisibleGetCount())
         return -1;
      auto p = m_columna.get_visible(iColumn);
      if (p == NULL)
         return -1;
      return p->m_iOrder;
   }

   
   index list::_001MapSubItemToColumn(index iSubItem)
   {
      
      return m_columna.subitem_visible_index(iSubItem);

   }


   index list::_001MapColumnToSubItem(index iColumn)
   {
      ASSERT(iColumn >= 0);
      ASSERT(iColumn < m_columna.VisibleGetCount());

      return m_columna.get_visible(iColumn)->m_iSubItem;
   }

   void list::_001DeleteColumn(index iColumn)
   {
      ASSERT(iColumn >= 0);
      ASSERT(iColumn < m_columna.VisibleGetCount());

      m_columna.remove(iColumn);

   }


   ::count list::_001GetItemCount()
   {

      if (m_pmeshdata.is_null())
         return -1;

      return m_pmeshdata->_001GetItemCount();

   }


   ::count list::_001GetGroupCount()
   {

      if (m_pmeshdata.is_null())
         return -1;

      return m_pmeshdata->_001GetGroupCount();

   }

   /////////////////////////////////////////////////////////////////
   //
   // Function: _001CalcDisplayTopIndex
   // Old Name: _001GetTopIndex
   //
   // Purpose:
   // Return the index of the first visible item in the list
   //
   // Output:
   // The index of the first visible item in the list
   //
   //
   /////////////////////////////////////////////////////////////////
   index list::_001CalcDisplayTopIndex()
   {

      point ptOffset = get_viewport_offset();

      index iItem;

      if (_001DisplayHitTest(point(0, m_bHeaderCtrl ? m_iItemHeight : 0), iItem))
         return iItem;
      else
      {
         if (m_eview == view_report)
         {
            if (ptOffset.y < 0)
               return 0;
         }
         return -1;
      }
   }

   ::count list::_001CalcDisplayItemCount()
   {
      if (m_eview == view_icon)
      {
         rect rectView;
         GetClientRect(&rectView);
         class size sizeItem = get_item_size();
         return MAX((rectView.width() / sizeItem.cx) * (rectView.height() / sizeItem.cy),
            m_iconlayout.m_iaDisplayToStrict.get_max_a() + 1);
      }
      else if (m_eview == view_report)
      {
         rect rectView;
         GetClientRect(&rectView);
         if (m_iItemHeight == 0)
         {
            return 0;
         }
         else
         {
            if (m_bTopText)
            {
               return (rectView.height() - m_rectTopText.height()) / m_iItemHeight + 1;
            }
            else
            {
               return rectView.height() / m_iItemHeight + 1;
            }
         }
      }

      int_ptr iItemCount;
      if (m_bFilter1 && (m_eview == view_list || m_eview == view_report))
      {
         iItemCount = m_piaFilterMesh->get_count();
      }
      else
      {
         iItemCount = m_nItemCount;
      }
      int_ptr iItemFirst = m_iTopIndex;
      int_ptr iItemLast = iItemFirst;
      rect rectItem;
      rect rectIntersect;
      rect rectUpdate;
      GetClientRect(&rectUpdate);
      draw_list_item item(this);
      if (iItemFirst >= 0)
      {
         iItemLast = -1;
         for (int_ptr i = iItemFirst + 1; i < iItemCount; i++)
         {
            item.m_iItem = i;
            item.m_iDisplayItem = i;
            if (m_bGroup)
            {
               item.m_iGroupTopIndex = 0;
               for (item.m_iGroup = 0; item.m_iGroup < m_nGroupCount; item.m_iGroup++)
               {
                  item.m_iGroupCount = _001GetGroupItemCount(item.m_iGroup);
                  if (i >= item.m_iGroupTopIndex && i < (item.m_iGroupTopIndex + item.m_iGroupCount))
                     break;
               }
            }

            _001GetItemRect(&item);
            if (item.m_bOk)
            {
               if (!rectIntersect.intersect(item.m_rectItem, rectUpdate))
               {
                  iItemLast = i - 1;
                  break;
               }
            }
            else
            {
               iItemLast = i - 1;
               break;
            }
         }
         if (iItemLast < 0)
            iItemLast = iItemCount - 1;
         return iItemLast - iItemFirst + 1;
      }
      else
      {
         return 0;
      }

   }


   bool list::_001HitTest_(point point, index &iItem, index &iSubItem, index &iListItem, ::user::mesh::e_element &eelement)
   {
      UNREFERENCED_PARAMETER(point);
      UNREFERENCED_PARAMETER(iItem);
      UNREFERENCED_PARAMETER(iSubItem);
      UNREFERENCED_PARAMETER(iListItem);
      UNREFERENCED_PARAMETER(eelement);
      return false;
   }

   bool list::_001HitTest_(point point, index &iItem, index &iSubItem)
   {
      if (!_001DisplayHitTest(point, iItem, iSubItem))
         return false;
      iItem = DisplayToStrict(iItem);
      return true;
   }

   bool list::_001HitTest_(POINT pt, index & iItem)
   {
      if (!_001DisplayHitTest(pt, iItem))
         return false;
      iItem = DisplayToStrict(iItem);
      return true;
   }

   bool list::_001DisplayHitTest(point point, index & iItem, index & iSubItem, index & iListItem, ::user::mesh::e_element &eelement)
   {
      UNREFERENCED_PARAMETER(point);
      UNREFERENCED_PARAMETER(iItem);
      UNREFERENCED_PARAMETER(iSubItem);
      UNREFERENCED_PARAMETER(iListItem);
      UNREFERENCED_PARAMETER(eelement);
      return false;
   }

   bool list::_001DisplayHitTest(point point, int_ptr &iItemParam, int_ptr &iSubItemParam)
   {

      int_ptr iItem;
      if (!_001DisplayHitTest(point, iItem))
      {
         return false;
      }
      if (m_eview == view_icon || m_eview == view_list)
      {
         iItemParam = iItem;
         if (iItem == -1)
         {
            iSubItemParam = -1;
         }
         else
         {
            iSubItemParam = 0;
         }
         return true;
      }
      int_ptr iColumnCount = _001GetColumnCount();

      //point ptOffset = get_viewport_offset();

      ::point ptOffset = get_viewport_offset();

      ::rect rectMargin;

      get_margin_rect(rectMargin);

      int_ptr iLeft = (index)-ptOffset.x - (rectMargin.left * 2);
      if (m_bGroup && m_bLateralGroup)
         iLeft += m_iLateralGroupWidth;
      int_ptr iRight;
      draw_list_item item(this);
      if (point.x < 0)
         return false;
      for (item.m_iColumn = 0; item.m_iColumn < iColumnCount; item.m_iColumn++)
      {
         _001GetColumnWidth(&item);
         if (!item.m_bOk)
            continue;
         iRight = iLeft + item.m_iColumnWidth;
         if (iLeft <= point.x && point.x < iRight)
         {
            iItemParam = iItem;
            iSubItemParam = item.m_iSubItem;
            return true;
         }
         iLeft = iRight;
      }
      return false;
   }

   bool list::_001DisplayHitTest(POINT pt, int_ptr &iItemParam)
   {

      if (m_eview == view_report)
      {

         point ptOffset = get_viewport_offset();

         index iy = pt.y + ptOffset.y + (m_bHeaderCtrl ? -m_iItemHeight : 0);

         if (iy < 0)
            iy = 0;

         index iItem = -1;

         if (m_iItemHeight != 0)
            iItem = iy / m_iItemHeight;

         if (iItem < 0)
            return false;

         //if(m_bHeaderCtrl)
         //{
           // iItem--;

            //if(iItem < 0)
              // return false;
         //}

         if (m_bFilter1)
         {
            if (iItem >= m_piaFilterMesh->get_count())
               return false;
         }

         if (iItem >= m_nItemCount)
            return false;

         iItemParam = (index)iItem;

         return true;
      }
      else if (m_eview == view_list)
      {
         if (m_iItemHeight <= 0)
            return false;

         class rect rectClient;
         GetClientRect(&rectClient);
         if (m_bTopText)
         {
            rectClient.top += m_rectTopText.height();
         }
         if (m_bHeaderCtrl)
         {
            rectClient.top += m_iItemHeight;
         }

         //         rectClient.inflate(m_scrolldata.m_rectMargin);


         index iRoundHeight = (index)((rectClient.height() / m_iItemHeight) * m_iItemHeight);

         point ptOffset = get_viewport_offset();

         index iy;

         if (m_iItemWidth <= 0)
         {

            iy = 0;

         }
         else
         {

            iy = (index)((pt.y + ptOffset.y) + (((pt.x + ptOffset.x) / m_iItemWidth)) * iRoundHeight);

         }

         index iItem = -1;

         if (m_iItemHeight != 0)
            iItem = iy / m_iItemHeight;

         if (iItem < 0)
            return false;

         if (m_bHeaderCtrl)
         {
            iItem--;

            if (iItem < 0)
               return false;
         }

         if (m_bFilter1)
         {
            if (iItem >= m_piaFilterMesh->get_count())
               return false;
         }

         if (iItem >= m_nItemCount)
            return false;

         iItemParam = iItem;

         return true;

      }
      else if (m_eview == view_icon)
      {
         if (m_columna.get_count() == 0)
            return false;
         class rect rectClient;
         GetClientRect(&rectClient);
         if (m_bTopText)
         {
            rectClient.top += m_rectTopText.height();
         }
         index iIconSize = MAX(32, m_columna[0]->m_sizeIcon.cy);
         index iItemSize = iIconSize * 2;


         point ptOffset = get_viewport_offset();

         index ix = (index)(pt.x + ptOffset.x);
         ix = (index)MAX(ptOffset.x, ix);
         ix = (index)MIN(rectClient.right, ix);
         ix = (index)MAX(rectClient.left, ix);
         ix /= iItemSize;

         index iy = pt.y + ptOffset.y;
         iy = MAX(ptOffset.y, iy);
         iy = MAX(rectClient.top, iy);
         iy /= iItemSize;

         //if(m_flags.is_signalized(flag_auto_arrange) || m_iconlayout.m_iWidth <= 0)
         {
            iItemParam = iy * (MAX(1, rectClient.width() / iItemSize)) + ix;
         }
         //else
         {
            //iItemParam = iy * (MAX(1, m_iconlayout.m_iWidth)) + ix;
         }


         return true;
      }

      return false;
   }

   void list::_001GetGroupRect(::user::draw_list_item * pdrawitem)
   {
      if (pdrawitem->m_iGroup < 0)
         return_(pdrawitem->m_bOk, false);
      if (pdrawitem->m_iGroup >= m_nGroupCount)
         return_(pdrawitem->m_bOk, false);
      index iItemFirst = 0;
      index iItemLast = _001GetGroupItemCount(0) - 1;
      pdrawitem->m_iGroupRectGroup = 0;
      while (pdrawitem->m_iGroupRectGroup < pdrawitem->m_iGroup)
      {
         pdrawitem->m_iGroupRectGroup++;
         iItemFirst = iItemLast + 1;
         iItemLast = iItemFirst + _001GetGroupItemCount(pdrawitem->m_iGroupRectGroup) - 1;
      }

      draw_list_item itemFirst(this);

      itemFirst.m_iItem = DisplayToStrict(iItemFirst);
      itemFirst.m_iDisplayItem = iItemFirst;
      itemFirst.m_iGroup = pdrawitem->m_iGroupRectGroup;

      _001GetItemRect(&itemFirst);

      draw_list_item itemLast(this);

      itemLast.m_iDisplayItem = DisplayToStrict(iItemLast);
      itemLast.m_iItem = iItemLast;
      itemLast.m_iGroup = pdrawitem->m_iGroupRectGroup;

      _001GetItemRect(&itemLast);

      pdrawitem->m_rectGroup.unite(itemFirst.m_rectItem, itemLast.m_rectItem);
      pdrawitem->m_rectGroup.bottom = MAX(itemLast.m_rectItem.bottom, itemFirst.m_rectItem.top + m_iGroupMinHeight);
      pdrawitem->m_rectGroup.left = 0;
      pdrawitem->m_rectGroup.right = m_iLateralGroupWidth;
      pdrawitem->m_bOk = true;
   }

   void list::_001GetItemRect(::user::draw_list_item * pdrawitem)
   {

      if (pdrawitem->m_iDisplayItem == (-1 - pdrawitem->m_iItemRectItem) && pdrawitem->m_iItemRectItem >= 0)
      {
         return_(pdrawitem->m_bOk, false);
      }

      if (pdrawitem->m_iDisplayItem == pdrawitem->m_iItemRectItem)
      {
         return_(pdrawitem->m_bOk, true);
      }

      if (pdrawitem->m_iDisplayItem < 0)
      {
         pdrawitem->m_rectItem.left = 0;
         pdrawitem->m_rectItem.top = 0;
         pdrawitem->m_rectItem.right = 0;
         pdrawitem->m_rectItem.bottom = 0;
         pdrawitem->m_iItemRectItem = (-1 - pdrawitem->m_iDisplayItem);
         return_(pdrawitem->m_bOk, false);
      }
      if (m_eview == view_icon && pdrawitem->m_iDisplayItem >= m_nDisplayCount)
      {
         pdrawitem->m_rectItem.left = 0;
         pdrawitem->m_rectItem.top = 0;
         pdrawitem->m_rectItem.right = 0;
         pdrawitem->m_rectItem.bottom = 0;
         pdrawitem->m_iItemRectItem = (-1 - pdrawitem->m_iDisplayItem);
         return_(pdrawitem->m_bOk, false);
      }

      point ptOffset = get_viewport_offset();

      if (m_eview == view_report)
      {

         if (m_bGroup)
         {
            if (m_bLateralGroup)
            {

               if (pdrawitem->m_iItemRectItem < 0)
               {
                  pdrawitem->m_rectItem.left = m_iLateralGroupWidth;
                  pdrawitem->m_rectItem.right = (LONG)(pdrawitem->m_rectItem.left + m_iItemWidth);
                  pdrawitem->m_iItemRectItem = 0;
                  pdrawitem->m_rectItem.top = 0;
                  if (m_bHeaderCtrl)
                  {
                     pdrawitem->m_rectItem.top += m_iItemHeight;
                  }
                  if (m_bTopText)
                  {
                     pdrawitem->m_rectItem.top += m_rectTopText.height();
                  }
                  pdrawitem->m_rectItem.bottom = pdrawitem->m_rectItem.top + m_iItemHeight;
                  pdrawitem->m_rectItem.offset(-ptOffset.x, -ptOffset.y);
               }

               if (pdrawitem->m_iDisplayItem > pdrawitem->m_iItemRectItem)
               {
                  int32_t iOffset = (int32_t)((pdrawitem->m_iItemRectItem - pdrawitem->m_iGroupTopIndex) * m_iItemHeight);
                  pdrawitem->m_rectItem.top -= iOffset;
                  pdrawitem->m_rectItem.bottom = pdrawitem->m_rectItem.top + m_iItemHeight;
                  while (pdrawitem->m_iGroupTopIndex + pdrawitem->m_iGroupCount < pdrawitem->m_iDisplayItem)
                  {
                     //                     int32_t iHeight                            = _001GetGroupHeight(pdrawitem->m_iGroup);
                     pdrawitem->m_rectItem.top += iOffset;
                     pdrawitem->m_rectItem.bottom = pdrawitem->m_rectItem.top + m_iItemHeight;
                     if ((pdrawitem->m_iGroup + 1) >= m_nGroupCount)
                     {
                        break;
                     }
                     pdrawitem->m_iGroup++;
                     pdrawitem->m_iGroupTopIndex += pdrawitem->m_iGroupCount;
                     pdrawitem->m_iGroupCount = _001GetGroupItemCount(pdrawitem->m_iGroup);
                     pdrawitem->m_iItemRectItem = pdrawitem->m_iGroupTopIndex;
                  }
               }
               else
               {
                  int32_t iOffset = (int32_t)((pdrawitem->m_iItemRectItem - pdrawitem->m_iGroupTopIndex) * m_iItemHeight);
                  pdrawitem->m_rectItem.top -= iOffset;
                  pdrawitem->m_rectItem.bottom = pdrawitem->m_rectItem.top + m_iItemHeight;
                  while (pdrawitem->m_iGroupTopIndex + pdrawitem->m_iGroupCount > pdrawitem->m_iDisplayItem)
                  {
                     if ((pdrawitem->m_iGroup - 1) < 0)
                     {
                        break;
                     }
                     pdrawitem->m_iGroup--;
                     int32_t iHeight = _001GetGroupHeight(pdrawitem->m_iGroup);
                     pdrawitem->m_rectItem.top -= iHeight;
                     pdrawitem->m_rectItem.bottom = pdrawitem->m_rectItem.top + m_iItemHeight;
                     pdrawitem->m_iGroupCount = _001GetGroupItemCount(pdrawitem->m_iGroup);
                     pdrawitem->m_iGroupTopIndex -= pdrawitem->m_iGroupCount;
                     pdrawitem->m_iItemRectItem = pdrawitem->m_iGroupTopIndex;
                  }
               }
               if (pdrawitem->m_iGroup < m_nGroupCount && pdrawitem->m_iGroup >= 0)
               {
                  pdrawitem->m_iItemRectItem = pdrawitem->m_iDisplayItem;
                  pdrawitem->m_rectItem.top += (LONG)((pdrawitem->m_iItemRectItem - pdrawitem->m_iGroupTopIndex) * m_iItemHeight);
                  pdrawitem->m_rectItem.bottom = pdrawitem->m_rectItem.top + m_iItemHeight;
               }
            }
            else
            {
               ::exception::throw_not_implemented(get_app());
            }
         }
         else
         {
            pdrawitem->m_rectItem.left = 0;
            pdrawitem->m_rectItem.right = (LONG)m_iItemWidth;

            pdrawitem->m_rectItem.top = 0;
            if (m_bHeaderCtrl)
            {
               pdrawitem->m_rectItem.top += m_iItemHeight;
            }
            pdrawitem->m_rectItem.top += (LONG)(m_iItemHeight * pdrawitem->m_iDisplayItem);
            if (m_bTopText)
            {
               pdrawitem->m_rectItem.top += m_rectTopText.height();
            }
            pdrawitem->m_rectItem.bottom = pdrawitem->m_rectItem.top + m_iItemHeight;
            pdrawitem->m_rectItem.offset(-ptOffset.x, -ptOffset.y);
            pdrawitem->m_iItemRectItem = pdrawitem->m_iDisplayItem;
         }
      }
      else if (m_eview == view_list)
      {

         class rect rectClient;
         GetClientRect(&rectClient);
         if (m_bTopText)
         {
            rectClient.top += m_rectTopText.height();
         }
         if (m_bHeaderCtrl)
         {
            rectClient.top += m_iItemHeight;
         }
         //         rectClient.inflate(m_scrolldata.m_rectMargin);
         if (m_iItemHeight <= 0)
            return_(pdrawitem->m_bOk, false);
         index iRoundHeight = (rectClient.height() / m_iItemHeight) * m_iItemHeight;
         if (iRoundHeight != 0)
         {
            pdrawitem->m_rectItem.left = (LONG)(((pdrawitem->m_iItem * m_iItemHeight) / iRoundHeight) * m_iItemWidth);
            pdrawitem->m_rectItem.top = (LONG)((pdrawitem->m_iItem * m_iItemHeight) % iRoundHeight);
         }
         if (m_bTopText)
         {
            pdrawitem->m_rectItem.top += m_rectTopText.height();
         }
         if (m_bHeaderCtrl)
         {
            pdrawitem->m_rectItem.top += m_iItemHeight;
         }
         pdrawitem->m_rectItem.bottom = pdrawitem->m_rectItem.top + m_iItemHeight;
         pdrawitem->m_rectItem.right = (LONG)(pdrawitem->m_rectItem.left + m_iItemWidth);
         pdrawitem->m_rectItem.offset(-ptOffset.x, -ptOffset.y);
      }
      else if (m_eview == view_icon)
      {
         //         if(m_flags.is_signalized(flag_auto_arrange) || m_iconlayout.m_iWidth <= 0)
         {
            class rect rectClient;
            GetClientRect(&rectClient);
            if (rectClient.is_empty())
            {
               return_(pdrawitem->m_bOk, false);
            }
            if (m_bTopText)
            {
               rectClient.top += m_rectTopText.height();
            }
            index iIconSize = MAX(32, m_columna[0]->m_sizeIcon.cy);
            index iItemSize = iIconSize * 2;
            pdrawitem->m_rectItem.left = (LONG)(iItemSize * (pdrawitem->m_iDisplayItem % (MAX(1, rectClient.width() / iItemSize))));
            pdrawitem->m_rectItem.top = (LONG)(iItemSize * (pdrawitem->m_iDisplayItem / (MAX(1, rectClient.width() / iItemSize))));
            pdrawitem->m_rectItem.bottom = (LONG)(pdrawitem->m_rectItem.top + iItemSize);
            pdrawitem->m_rectItem.right = (LONG)(pdrawitem->m_rectItem.left + iItemSize);
         }
         //else
         //{
         //   class rect rectClient;
         //   GetClientRect(&rectClient);
         //   if(m_bTopText)
         //   {
         //      rectClient.top += m_rectTopText.height();
         //   }
         //   index iItemSize = get_item_size().cy;
         //   pdrawitem->m_rectItem.left = (LONG) (iItemSize * (pdrawitem->m_iDisplayItem % m_iconlayout.m_iWidth));
         //   pdrawitem->m_rectItem.top = (LONG) (rectClient.top + iItemSize * (pdrawitem->m_iDisplayItem / m_iconlayout.m_iWidth));
         //   pdrawitem->m_rectItem.bottom = (LONG) (pdrawitem->m_rectItem.top + iItemSize);
         //   pdrawitem->m_rectItem.right = (LONG) (pdrawitem->m_rectItem.left + iItemSize);
         //}
         pdrawitem->m_rectItem.offset(-ptOffset.x, -ptOffset.y);

         //if (m_eview == view_icon)
         {

            if (m_bDrag)
            {
               if (m_iItemMouseDown >= 0 && m_iStrictItemDrag >= 0)
               {
                  if (m_rangeSelection.has_item(pdrawitem->m_iItem))
                  {
                     point pt = m_ptLButtonUp;
                     point p2 = m_ptLButtonDown;
                     //pt.x -= oprop("offx").int32();
                     //pt.y -= oprop("offy").int32();
                     pt -= p2;
                     pdrawitem->m_rectItem.offset(pt);
                  }
                  else if (pdrawitem->m_iItem == m_iStrictItemDrag)
                  {
                     point pt = m_ptLButtonUp;
                     pt.x -= oprop("offx").int32();
                     pt.y -= oprop("offy").int32();
                     pdrawitem->m_rectItem.move_to(pt);

                  }

               }

            }

         }

      }

      pdrawitem->m_bOk = true;

   }

   void list::_001GetSubItemRect(::user::draw_list_item * pdrawitem)
   {

      if (pdrawitem->m_iDisplayItem != pdrawitem->m_iItemRectItem)
      {
         _001GetItemRect(pdrawitem);
         if (!pdrawitem->m_bOk)
            return;
      }

      if (pdrawitem->m_iSubItemRectOrder == pdrawitem->m_iOrder)
      {
         if (pdrawitem->m_iSubItemRectItem != pdrawitem->m_iItemRectItem)
         {
            pdrawitem->m_rectSubItem.top = pdrawitem->m_rectItem.top;
            pdrawitem->m_rectSubItem.bottom = pdrawitem->m_rectItem.bottom;
         }
         return_(pdrawitem->m_bOk, true);
      }

      if (m_eview == view_icon)
      {
         pdrawitem->m_rectSubItem = pdrawitem->m_rectItem;
         return_(pdrawitem->m_bOk, true);
         return;
         throw "subitem rectangle on icon view? why are you asking for that now?";
      }

      pdrawitem->m_bOk = false;

      pdrawitem->m_rectSubItem.top = pdrawitem->m_rectItem.top;
      pdrawitem->m_rectSubItem.bottom = pdrawitem->m_rectItem.bottom;

      index iLastOrder = _001GetColumnCount() - 1;

      if (iLastOrder < 0)
         return_(pdrawitem->m_bOk, false);



      if (pdrawitem->m_iSubItemRectOrder < 0 || pdrawitem->m_iOrder == 0 || pdrawitem->m_iSubItemRectOrder > pdrawitem->m_iOrder)
      {
         pdrawitem->m_iSubItemRectOrder = pdrawitem->m_iOrder;
         pdrawitem->m_rectSubItem.left = pdrawitem->m_rectItem.left;
         pdrawitem->m_iSubItemRectColumn = _001MapOrderToColumn(pdrawitem->m_iOrder);
         pdrawitem->m_iColumn = pdrawitem->m_iSubItemRectColumn;
         _001GetColumnWidth(pdrawitem);
         pdrawitem->m_rectSubItem.right = pdrawitem->m_rectItem.left + pdrawitem->m_iColumnWidth;
         pdrawitem->m_iSubItemRectItem = pdrawitem->m_iItemRectItem;
         pdrawitem->m_iSubItemRectSubItem = pdrawitem->m_iSubItem;
         if (pdrawitem->m_iOrder == 0)
         {
            pdrawitem->m_bOk = true;
            return;
         }
      }

      if (pdrawitem->m_iOrder <= iLastOrder)
      {
         while (pdrawitem->m_iSubItemRectOrder < pdrawitem->m_iOrder)
         {
            pdrawitem->m_iColumn = _001MapOrderToColumn(pdrawitem->m_iSubItemRectOrder);
            _001GetColumnWidth(pdrawitem);
            pdrawitem->m_rectSubItem.left += pdrawitem->m_iColumnWidth;
            pdrawitem->m_iSubItemRectOrder++;
         }
         pdrawitem->m_iColumn = _001MapOrderToColumn(pdrawitem->m_iSubItemRectOrder);
         pdrawitem->m_iSubItemRectColumn = pdrawitem->m_iColumn;
         _001GetColumnWidth(pdrawitem);
         pdrawitem->m_rectSubItem.right = pdrawitem->m_rectSubItem.left + pdrawitem->m_iColumnWidth;
         pdrawitem->m_iSubItemRectOrder = pdrawitem->m_iOrder;
         pdrawitem->m_iSubItemRectItem = pdrawitem->m_iItemRectItem;
         pdrawitem->m_iSubItemRectSubItem = pdrawitem->m_iSubItem;
         pdrawitem->m_bOk = true;
      }

   }

   void list::_001GetElementRect(::user::draw_list_item * pdrawitem, ::user::mesh::e_element eelement)
   {

      if (m_bGroup && m_bLateralGroup &&
         (eelement == ::user::list::element_group_image
            || eelement == ::user::list::element_group_item_text))
      {
         int32_t x = pdrawitem->m_rectGroup.left;
         int32_t iImageBottom = pdrawitem->m_rectGroup.top;

         if (m_pilGroup != NULL)
         {


            ::image_list::info ii;

            _001GetGroupImage(pdrawitem);
            if (pdrawitem->m_bOk && pdrawitem->m_iImage >= 0)
            {
               m_pilGroup->get_image_info((int32_t)pdrawitem->m_iImage, &ii);
               if (eelement == ::user::list::element_group_image)
               {
                  pdrawitem->m_rectImage.left = x;
                  pdrawitem->m_rectImage.right = x + width(&ii.m_rect);
                  pdrawitem->m_rectImage.top = pdrawitem->m_rectGroup.top;
                  pdrawitem->m_rectImage.bottom = pdrawitem->m_rectImage.top + height(&ii.m_rect);
                  return_(pdrawitem->m_bOk, true);
               }
               else
               {
                  x += width(&ii.m_rect);
                  x += 2;
                  iImageBottom += height(&ii.m_rect) + 2;
               }
            }
            else if (eelement == ::user::list::element_group_image)
            {
               return_(pdrawitem->m_bOk, false);
            }

         }
         else if (eelement == ::user::list::element_group_image)
         {
            return_(pdrawitem->m_bOk, false);
         }
         if (eelement == ::user::list::element_group_item_text)
         {
            pdrawitem->m_rectText.top = (LONG)(pdrawitem->m_rectGroup.top + m_iItemHeight * pdrawitem->m_iItem);
            if (pdrawitem->m_rectText.top >= iImageBottom)
            {
               pdrawitem->m_rectText.left = pdrawitem->m_rectGroup.left;
            }
            else
            {
               pdrawitem->m_rectText.left = x;
            }
            pdrawitem->m_rectText.right = pdrawitem->m_rectGroup.right;
            pdrawitem->m_rectText.bottom = pdrawitem->m_rectText.top + m_iItemHeight;
            return_(pdrawitem->m_bOk, true);
         }
         pdrawitem->m_bOk = false;
      }
      else
      {
         if (pdrawitem->m_iDisplayItem != pdrawitem->m_iItemRectItem)
         {
            _001GetItemRect(pdrawitem);
            if (!pdrawitem->m_bOk)
               return;
         }
         if (eelement == ::user::list::element_item)
         {
            return_(pdrawitem->m_bOk, true);
         }
         if (m_eview == view_icon)
         {
            if (eelement == ::user::list::element_image)
            {
               int32_t iIconSize = m_columna[0]->m_sizeIcon.cy;
               pdrawitem->m_rectImage.left = pdrawitem->m_rectItem.left + iIconSize / 2;
               pdrawitem->m_rectImage.top = pdrawitem->m_rectItem.top;
               pdrawitem->m_rectImage.right = pdrawitem->m_rectImage.left + iIconSize;
               pdrawitem->m_rectImage.bottom = pdrawitem->m_rectImage.top + iIconSize;
               return_(pdrawitem->m_bOk, true);
            }
            else if (eelement == ::user::mesh::element_text)
            {
               int32_t iIconSize = m_columna[0]->m_sizeIcon.cy;
               pdrawitem->m_rectText.left = pdrawitem->m_rectItem.left;
               pdrawitem->m_rectText.top = pdrawitem->m_rectItem.top + iIconSize;
               pdrawitem->m_rectText.right = pdrawitem->m_rectText.left + iIconSize * 2;
               pdrawitem->m_rectText.bottom = pdrawitem->m_rectText.top + iIconSize;
               return_(pdrawitem->m_bOk, true);
            }
            return_(pdrawitem->m_bOk, false);
         }

         _001GetSubItemRect(pdrawitem);

         if (!pdrawitem->m_bOk)
            return;

         int32_t x = pdrawitem->m_rectSubItem.left;

         if (pdrawitem->m_iListItem == -1)
         {
            if (eelement == ::user::mesh::element_sub_item)
            {
               return_(pdrawitem->m_bOk, true);
            }
            if (pdrawitem->m_pcolumn->m_bIcon)
            {
               _001GetItemImage(pdrawitem);
               if (pdrawitem->m_bOk && pdrawitem->m_iImage >= 0)
               {
                  if (eelement == ::user::list::element_image)
                  {
                     rect rAlign(pdrawitem->m_rectSubItem);
                     rAlign.left = x;
                     rect rIcon;
                     rIcon.set(0, 0, pdrawitem->m_pcolumn->m_sizeIcon.cx, pdrawitem->m_pcolumn->m_sizeIcon.cy);
                     rIcon.Align(align_left_center, rAlign);
                     pdrawitem->m_rectImage = rIcon;
                     return_(pdrawitem->m_bOk, true);
                  }
                  else
                  {
                     x += pdrawitem->m_pcolumn->m_sizeIcon.cx;
                     x += m_iImageSpacing;
                  }
               }
               else if (eelement == ::user::list::element_image)
               {
                  return_(pdrawitem->m_bOk, false);
               }
            }
            else if (pdrawitem->m_pcolumn->m_pil != NULL)
            {

               ::image_list::info ii;

               _001GetItemImage(pdrawitem);
               if (pdrawitem->m_bOk && pdrawitem->m_iImage >= 0)
               {
                  pdrawitem->m_pcolumn->m_pil->get_image_info((int32_t)pdrawitem->m_iImage, &ii);
                  if (eelement == ::user::list::element_image)
                  {

                     rect rAlign(pdrawitem->m_rectSubItem);
                     rAlign.left = x;
                     rect rIcon;
                     rIcon.set(0, 0, width(&ii.m_rect), height(&ii.m_rect));
                     rIcon.Align(align_left_center, rAlign);
                     pdrawitem->m_rectImage = rIcon;

                 /*    pdrawitem->m_rectImage.left = x;
                     pdrawitem->m_rectImage.right = x + ;
                     pdrawitem->m_rectImage.bottom = pdrawitem->m_rectSubItem.bottom;
                     pdrawitem->m_rectImage.top = pdrawitem->m_rectImage.bottom - ;*/
                     return_(pdrawitem->m_bOk, true);
                  }
                  else
                  {
                     x += width(&ii.m_rect);
                     x += m_iImageSpacing;
                  }
               }
               else if (eelement == ::user::list::element_image)
               {
                  return_(pdrawitem->m_bOk, false);
               }

            }
            else if (eelement == ::user::list::element_image)
            {
               return_(pdrawitem->m_bOk, false);
            }
            if (eelement == ::user::mesh::element_text)
            {
               pdrawitem->m_rectText.left = x;
               pdrawitem->m_rectText.right = pdrawitem->m_rectSubItem.right;
               pdrawitem->m_rectText.top = pdrawitem->m_rectSubItem.top;
               pdrawitem->m_rectText.bottom = pdrawitem->m_rectSubItem.bottom;
               return_(pdrawitem->m_bOk, true);
            }
         }
         pdrawitem->m_bOk = false;
      }

   }


   //bool list::range::has_item(index iItem) const
   //{
   //   for(index i = 0; i < m_itemrangea.get_size(); i++)
   //   {
   //      const item_range & itemrange = m_itemrangea[i];
   //      if(itemrange.has_item(iItem))
   //         return true;
   //   }
   //   return false;
   //}

   //void list::range::get_item_indexes(index_array & ia) const
   //{

   //   for(index i = 0; i < m_itemrangea.get_size(); i++)
   //   {

   //      const item_range & itemrange = m_itemrangea[i];

   //      itemrange.get_item_indexes(ia);

   //   }

   //}


   //bool list::range::has_sub_item(int_ptr iItem, int_ptr iSubItem) const
   //{
   //   for(index i = 0; i < m_itemrangea.get_size(); i++)
   //   {
   //      const item_range & itemrange = m_itemrangea[i];
   //      if(itemrange.has_item(iItem))
   //         if(itemrange.has_sub_item(iSubItem))
   //            return true;
   //   }
   //   return false;
   //}

   //// remove the specified item
   //bool list::range::remove_item(index iItem)
   //{
   //   for(index i = 0; i < m_itemrangea.get_size();)
   //   {
   //      item_range & itemrange = m_itemrangea[i];
   //      if(itemrange.has_item(iItem))
   //      {
   //         if(itemrange.get_lower_bound() == iItem)
   //         {
   //            itemrange.set_lower_bound(itemrange.get_lower_bound() + 1);
   //            if(itemrange.get_lower_bound() > itemrange.get_upper_bound())
   //            {
   //               m_itemrangea.remove_at(i);
   //               continue;
   //            }
   //         }
   //         else if(itemrange.get_upper_bound() == iItem)
   //         {
   //            itemrange.set_upper_bound(itemrange.get_upper_bound() - 1);
   //         }
   //         else
   //         {
   //            // split the current range as it will be segmented
   //            item_range itemrangeL;
   //            item_range itemrangeU;
   //            itemrangeL = itemrange;
   //            itemrangeU = itemrange;
   //            itemrangeL.set_upper_bound(iItem - 1);
   //            itemrangeU.set_lower_bound(iItem + 1);
   //            m_itemrangea.remove_at(i);
   //            m_itemrangea.add(itemrangeL);
   //            m_itemrangea.add(itemrangeU);
   //            continue;
   //         }
   //      }
   //      i++;
   //   }
   //   return false;
   //}

   //// remove the specified item and offset remaining items.
   //bool list::range::OnRemoveItem(index iItem)
   //{
   //   remove_item(iItem);
   //   // All ranges that has item "iItem + 1",
   //   // must have all items greater or equal "iItem + 1"
   //   // decremented by one
   //   iItem++;
   //   for(index i = 0; i < m_itemrangea.get_size();)
   //   {
   //      item_range & itemrange = m_itemrangea[i];
   //      if(itemrange.has_item(iItem))
   //      {
   //         if(itemrange.get_lower_bound() >= iItem)
   //         {
   //            itemrange.offset(-1);
   //         }
   //         else if(itemrange.get_upper_bound() >= iItem)
   //         {
   //            itemrange.set_upper_bound(itemrange.get_upper_bound() - 1);
   //         }
   //      }
   //      i++;
   //   }
   //   return false;
   //}

   //void list::item_range::get_item_indexes(index_array & ia) const
   //{

   //   if(m_iLowerBound == -1 || m_iUpperBound == -1)
   //      return;

   //   for(index iItem = m_iLowerBound; iItem <= m_iUpperBound; iItem++)
   //   {

   //      ia.add_unique(iItem);

   //   }

   //}

   //bool list::item_range::has_item(index iItem) const
   //{
   //   if(m_iLowerBound == -1 || m_iUpperBound == -1)
   //      return false;
   //   if(iItem >= m_iLowerBound
   //      && iItem <= m_iUpperBound)
   //      return true;
   //   else
   //      return false;
   //}

   //bool list::item_range::has_sub_item(index iSubItem) const
   //{
   //   return m_subitemrange.has_sub_item(iSubItem);
   //}

   void list::LayoutHeaderCtrl()
   {

      synch_lock sl(m_pmutex);

      if (m_plistheader == NULL)
         return;

      if (m_bHeaderCtrl)
      {

         rect rectFocus;

         GetFocusRect(&rectFocus);

         m_plistheader->SetWindowPos(ZORDER_BOTTOM, 0, 0, (int)MAX(m_iItemWidth + 10, rectFocus.width()), (int)m_iItemHeight,
            SWP_SHOWWINDOW);

      }
      else
      {

         m_plistheader->ShowWindow(SW_HIDE);

      }

   }


   void list::_001OnKeyDown(signal_details * pobj)
   {

      SCAST_PTR(::message::key, pkey, pobj);

      if (pkey->previous()) // give chance to child
         return;

      synch_lock sl(m_pmutex);

      if (pkey->m_ekey == ::user::key_down || pkey->m_ekey == ::user::key_up ||
         pkey->m_ekey == ::user::key_next || pkey->m_ekey == ::user::key_prior)
      {
         if (m_nItemCount > 0)
         {
            range range = m_rangeSelection;
            m_rangeSelection.clear();

            int_ptr iItem = m_iItemFocus;

            if (iItem < 0)
            {
               if (pkey->m_ekey == ::user::key_down || pkey->m_ekey == ::user::key_next)
               {
                  iItem = 0;
               }
               else if (pkey->m_ekey == ::user::key_up || pkey->m_ekey == ::user::key_prior)
               {
                  iItem = m_nItemCount - 1;
               }
            }
            else
            {
               if (pkey->m_ekey == ::user::key_down)
               {
                  iItem++;
               }
               else if (pkey->m_ekey == ::user::key_next)
               {
                  iItem += m_nDisplayCount;
               }
               else if (pkey->m_ekey == ::user::key_up)
               {
                  iItem--;
               }
               else if (pkey->m_ekey == ::user::key_prior)
               {
                  iItem -= m_nDisplayCount;
               }
               else
               {
                  ASSERT(FALSE);
                  pobj->m_bRet = false;
                  return;
               }
            }

            if (iItem < 0)
            {
               iItem = m_nItemCount - 1;
            }
            else if (iItem >= m_nItemCount)
            {
               iItem = 0;
            }

            m_iShiftFirstSelection = iItem;
            m_iItemFocus = iItem;

            item_range itemrange;
            itemrange.set(iItem, iItem, 0, m_columna.get_count() - 1, -1, -1);
            m_rangeSelection.add_item(itemrange);

            _001EnsureVisible(iItem, range);

            range.add_item(itemrange);

            RedrawWindow();

            _001OnSelectionChange();

         }

      }
      else if (pkey->m_ekey == ::user::key_delete)
      {

         range range;

         _001GetSelection(range);

         _001DeleteRange(range);

         pobj->m_bRet = true;

      }

      pobj->m_bRet = false;

   }


   void list::_001OnMouseMove(signal_details * pobj)
   {

      SCAST_PTR(::message::mouse, pmouse, pobj);

      pmouse->set_lresult(1);

      point pt = pmouse->m_pt;

      ScreenToClient(&pt);

      pmouse->previous(); // give chance to child control

      synch_lock sl(m_pmutex);

      if (m_bDrag)
      {
         if (m_iItemMouseDown < 0)
         {

            if (m_eview == view_icon)
            {
               class rect rectClient;
               GetClientRect(&rectClient);
               index iIconSize = MAX(32, m_columna[0]->m_sizeIcon.cy);
               index iItemSize = iIconSize * 2;
               ::count iItemColumnCount = MAX(1, rectClient.width() / iItemSize);
               m_ptLButtonUp = pt;
               index iItemStart;
               index iItemEnd;
               if (_001DisplayHitTest(m_ptLButtonDown, iItemStart))
               {
                  if (_001DisplayHitTest(pt, iItemEnd))
                  {

                     index iCol1 = iItemStart % iItemColumnCount;
                     index iCol2 = iItemEnd % iItemColumnCount;

                     ::sort::sort(iCol1, iCol2);

                     index iRow1 = iItemStart / iItemColumnCount;
                     index iRow2 = iItemEnd / iItemColumnCount;

                     ::sort::sort(iRow1, iRow2);

                     m_rangeSelection.clear();

                     for (index i = iRow1; i <= iRow2; i++)
                     {
                        for (index j = iCol1; j <= iCol2; j++)
                        {
                           item_range itemrange;
                           itemrange.set_lower_bound(m_iconlayout.m_iaDisplayToStrict.get_b(i * iItemColumnCount + j));
                           itemrange.set_upper_bound(m_iconlayout.m_iaDisplayToStrict.get_b(i * iItemColumnCount + j));
                           m_rangeSelection.add_item(itemrange);
                        }
                     }

                  }

               }

            }

         }

      }
      //   index iItemOld = m_iItemDrop;
      //   if(!_001DisplayHitTest(pt, m_iItemDrop))
      //   {
      //      m_iItemDrop = m_iItemDrag;
      //   }
      //   if(iItemOld != m_iItemDrop)
      //   {
      //      RedrawWindow();
      //   }
      //}

      track_mouse_leave();

      //if (m_spmenuPopup.is_null())
      //{

      //   UpdateHover();
      //   pobj->m_bRet = true;


      //   index iItemEnter;
      //   index iSubItemEnter;
      //   point point;
      //   //Session.get_cursor_pos(&point);
      //   //ScreenToClient(&point);

      //   if (_001DisplayHitTest(pt, iItemEnter, iSubItemEnter))
      //   {
      //      if (m_bHoverSelect &&
      //         (m_iSubItemEnter != iSubItemEnter ||
      //         m_iItemEnter != iItemEnter)
      //         && !m_rangeSelection.has_item(iItemEnter))
      //      {
      //         m_iMouseFlagEnter = pmouse->m_nFlags;
      //         m_iItemEnter = iItemEnter;
      //         m_iSubItemEnter = iSubItemEnter;
      //         //SetTimer(12321, 840, NULL);
      //         SetTimer(12321, 184 + 177 + 151, NULL);
      //      }
      //   }

      //}


   }


   void list::_001OnLButtonDown(signal_details * pobj)
   {


      SCAST_PTR(::message::mouse, pmouse, pobj);

      pmouse->previous(); // give chance to base views

      int_ptr iItem;

      point pt = pmouse->m_pt;

      ScreenToClient(&pt);

      synch_lock sl(m_pmutex);

      if (m_bSelect)
      {

         if (m_bHoverSelect)
         {

         }
         else
         {
            if (m_bMultiSelect && Session.is_key_pressed(::user::key_shift))
            {
               if (_001DisplayHitTest(pt, iItem))
               {
                  item_range itemrange;
                  int_ptr iLItem = MIN(m_iShiftFirstSelection, iItem);
                  int_ptr iUItem = MAX(m_iShiftFirstSelection, iItem);
                  itemrange.set(iLItem, iUItem, 0, m_columna.get_count() - 1, -1, -1);
                  m_rangeSelection.add_item(itemrange);
                  m_iShiftFirstSelection = iItem;
               }
            }
            else if (m_bMultiSelect && Session.is_key_pressed(::user::key_control))
            {
               if (_001DisplayHitTest(pt, iItem))
               {
                  item_range itemrange;
                  int_ptr iLItem = MIN(m_iShiftFirstSelection, iItem);
                  int_ptr iUItem = MAX(m_iShiftFirstSelection, iItem);
                  itemrange.set(iLItem, iUItem, 0, m_columna.get_count() - 1, -1, -1);
                  m_rangeSelection.add_item(itemrange);
                  m_iShiftFirstSelection = iItem;
               }
            }
            else
            {
               m_rangeSelection.clear();
               index iItem;
               if (_001DisplayHitTest(pt, iItem))
               {
                  m_iShiftFirstSelection = iItem;
                  m_iItemFocus = iItem;
                  _001DisplayHitTest(pt, m_iItemDrag);
                  m_iItemDrop = m_iItemDrag;
                  SetTimer(12345678, 400, NULL);
                  item_range itemrange;
                  itemrange.set(iItem, iItem, 0, m_columna.get_count() - 1, -1, -1);
                  m_rangeSelection.add_item(itemrange);
               }
            }
         }

      }
      else
      {

         if (_001DisplayHitTest(pt, iItem))
         {

            m_iItemMouseDown = iItem;
            SetTimer(224455, 500, NULL);

            if (m_eview == view_icon)
            {



               point pt = pmouse->m_pt;
               ScreenToClient(pt);
               draw_list_item item(this);
               item.m_iItem = m_iconlayout.m_iaDisplayToStrict.get_b(iItem);
               item.m_iDisplayItem = iItem;
               _001GetItemRect(&item);
               oprop("offx") = pt.x - item.m_rectItem.left;
               oprop("offy") = pt.y - item.m_rectItem.top;
               if (!m_rangeSelection.has_item(item.m_iItem))
               {
                  m_rangeSelection.clear();
               }
            }

         }
         else
         {

            m_iItemMouseDown = -1;
            SetTimer(224455, 500, NULL);

         }
         m_ptLButtonDown = pt;
      }

      RedrawWindow();

      //if (!has_focus())
      //{

      //   SetFocus();

      //}

      //Session.set_keyboard_focus(this);
      //Session.user()->set_mouse_focus_LButtonDown(this);
      //GetParentFrame()->SetActiveView(this);
      pobj->m_bRet = true;
      pmouse->set_lresult(1);
   }

   void list::_001OnLButtonUp(signal_details * pobj)
   {

      SCAST_PTR(::message::mouse, pmouse, pobj);

      point pt = pmouse->m_pt;

      ScreenToClient(&pt);

      ReleaseCapture();

      KillTimer(12345678);
      KillTimer(224455);

      synch_lock sl(m_pmutex);

      if (m_bDrag)
      {

         m_bDrag = false;

         if (m_eview == view_icon)
         {

            if (m_iItemMouseDown >= 0)
            {

               index iItemOld = m_iItemDrop;

               if (_001DisplayHitTest(pt, m_iItemDrop))
               {

                  if (iItemOld != m_iItemDrop)
                  {

                     if (m_eview == view_icon)
                     {

                        if (m_rangeSelection.get_item_count() > 0)
                        {

                           class rect rectClient;

                           GetClientRect(&rectClient);

                           index iIconSize = MAX(32, m_columna[0]->m_sizeIcon.cy);

                           index iItemSize = iIconSize * 2;

                           int iItemColumnCount = (int)MAX(1, rectClient.width() / iItemSize);

                           index iCol1 = m_iItemDrag % iItemColumnCount;

                           index iRow1 = m_iItemDrag / iItemColumnCount;

                           index iCol2 = m_iItemDrop % iItemColumnCount;

                           index iRow2 = m_iItemDrop / iItemColumnCount;

                           index iColOffset = iCol2 - iCol1;

                           index iRowOffset = iRow2 - iRow1;

                           index_array iaNew;

                           index_array iaOld;

                           for (index i = 0; i < m_rangeSelection.get_item_count(); i++)
                           {

                              item_range & itemrange = m_rangeSelection.ItemAt(i);

                              for (index iItem = itemrange.m_iLowerBound; iItem <= itemrange.m_iUpperBound; iItem++)
                              {

                                 index iDisplay = m_iconlayout.m_iaDisplayToStrict.get_a(iItem);

                                 index iCol = (iDisplay + iColOffset) % iItemColumnCount;

                                 index iRow = (iDisplay + iColOffset) / iItemColumnCount + iRowOffset;

                                 iaNew.add(iRow * iItemColumnCount + iCol);

                                 iaOld.add(iDisplay);

                              }

                           }

                           array_translate_a(m_iconlayout.m_iaDisplayToStrict, iaNew, iaOld);

                        }
                        else
                        {
                           //if (iItemOld >= 0)
                           //{
                           //   m_iconlayout.m_iaDisplayToStrict.array_remove_a(iItemOld);
                           //}
                           array_translate_a(m_iconlayout.m_iaDisplayToStrict, m_iItemDrop, iItemOld);

                        }

                     }

                     RedrawWindow();

                  }

                  //if(m_iItemDrop != 0)
                  //{
                  //   if (!defer_drop(m_iItemDrop, m_iItemDrag))
                  //   {



                  //   }
                  //}

                  string strSort = oprop("list_sort");

                  if (strSort.has_char())
                  {

                     defer_update_display();

                     strSort += "-" + m_strDisplay + ".icon_list_view_sort";

                     stringa stra;

                     for (index a = 0; a <= m_plist->m_iconlayout.m_iaDisplayToStrict.m_iMaxA; a++)
                     {

                        index b = m_plist->m_iconlayout.m_iaDisplayToStrict.get_b(a);

                        stra.add(_001GetItemId(b));

                     }

                     Application.file().put_contents(strSort, stra.implode("\r\n"));

                     update_icon_list_view_sort();

                  }

               }

            }
         }
      }
      else if(m_bHoverSelect)
      {

         if (m_eview == view_report)
         {

            pmouse->previous();

         }
         else
         {

            index iItem = -1;

            if (_001DisplayHitTest(pt, iItem))
            {

               if (iItem >= 0 && m_iItemMouseDown == iItem)
               {

                  iItem = m_iconlayout.m_iaDisplayToStrict.get_b(iItem);

                  if (iItem >= 0)
                  {

                     _001OnItemClick(iItem);

                  }

               }

            }

         }
         
      }
      else if (!m_bSelect)
      {

         if (Session.user()->get_mouse_focus_LButtonDown() == this)
         {

            index iItem = -1;

            if (_001DisplayHitTest(pt, iItem))
            {

               if (iItem >= 0 && m_iItemMouseDown == iItem)
               {

                  iItem = m_iconlayout.m_iaDisplayToStrict.get_b(iItem);

                  if (iItem >= 0)
                  {

                     _001OnItemClick(iItem);

                  }

               }

            }

         }
         m_iItemMouseDown = -1;

         pobj->m_bRet = true;

         pmouse->set_lresult(1);

      }
      else
      {

         //if(m_iClick == 1)
         //{

         //   m_iClick = 0;

         //   if(!_001IsEditing())
         //   {
         //      //uint_ptr nFlags = m_uiLButtonUpFlags;
         //      //point point = m_ptLButtonUp;
         //      //_001OnClick(pmouse->m_nFlags,pt);
         //      Redraw();


         //      /* trans
         //      window_id wndidNotify = pwnd->GetOwner()->GetSafeoswindow_();
         //      if(wndidNotify == NULL)
         //      wndidNotify = pwnd->GetParent()->GetSafeoswindow_(); */

         //      //               LRESULT lresult = 0;

         //      /* trans            if(wndidNotify)
         //      {
         //      NMLISTVIEW nm;
         //      nm.hdr.idFrom = pwnd->GetDlgCtrlId();
         //      nm.hdr.code =   NM_CLICK;
         //      nm.hdr.oswindowFrom = pwnd->GetSafeoswindow_();
         //      lresult = ::SendMessage(
         //      wndidNotify,
         //      WM_NOTIFY,
         //      nm.hdr.idFrom,
         //      (LPARAM) &nm);
         //      }*/

         //   }

         //}
         //else
         //{
         //   output_debug_string(".");
         //   // commented on 2017-03-28
         //   //m_iClick++;
         //   //m_uiLButtonUpFlags = (UINT) pmouse->m_nFlags;
         //   //m_ptLButtonUp = pt;
         //   //SetTimer(12345679, 500, NULL);
            // commented on 2017-03-28
            //m_iClick++;
            //m_uiLButtonUpFlags = (UINT) pmouse->m_nFlags;
            //m_ptLButtonUp = pt;
            //SetTimer(12345679, 500, NULL);


         //}

      }

      m_iItemMouseDown = -1;

      pobj->m_bRet = true;

      pmouse->set_lresult(1);

      m_bLButtonDown = false;

   }


   void list::_001OnRButtonDown(signal_details * pobj)
   {

      SCAST_PTR(::message::mouse, pmouse, pobj);

      pmouse->previous();

      point pt = pmouse->m_pt;

      ScreenToClient(&pt);

      synch_lock sl(m_pmutex);

      if (!has_focus())
      {
         SetFocus();
      }
      if (Session.is_key_pressed(::user::key_shift))
      {
      }
      else if (Session.is_key_pressed(::user::key_control))
      {
      }
      else
      {
         //      m_rangeSelection.clear();
         index iItem;
         if (_001DisplayHitTest(pt, iItem))
         {
            if (!m_rangeSelection.has_item(iItem))
            {
               m_rangeSelection.clear();
               m_iShiftFirstSelection = iItem;
               item_range itemrange;
               itemrange.set(iItem, iItem, 0, m_columna.get_count() - 1, -1, -1);
               m_rangeSelection.add_item(itemrange);
               RedrawWindow();
            }
         }
         m_uiRButtonUpFlags = (UINT)pmouse->m_nFlags;
         m_ptRButtonUp = pmouse->m_pt;
         SetTimer(8477, 500, NULL);
      }


      pobj->m_bRet = true;
   }


   bool list::_001OnItemClick(index iItem)
   {

      UNREFERENCED_PARAMETER(iItem);
      return false;
   }


   bool list::_001OnClick(uint_ptr nFlag, point point)
   {
      UNREFERENCED_PARAMETER(nFlag);
      UNREFERENCED_PARAMETER(point);
      ::user::control_event ev;
      ev.m_puie = this;
      ev.m_eevent = ::user::event_list_clicked;
      if (m_pformcallback != NULL)
      {
         m_pformcallback->BaseOnControlEvent(NULL, &ev);
      }
      else if (get_form() != NULL)
      {
         get_form()->send_message(
            ::message::message_event, 0, (LPARAM)&ev);
      }
      else
      {
         GetParent()->send_message(
            ::message::message_event, 0, (LPARAM)&ev);
      }
      return false;
   }

   bool list::_001OnRightClick(uint_ptr nFlag, point point)
   {
      UNREFERENCED_PARAMETER(nFlag);
      UNREFERENCED_PARAMETER(point);
      return false;
   }

   //void list::range::clear()
   //{
   //   m_itemrangea.remove_all();
   //}

   //void list::range::add_item(const item_range & itemrange)
   //{
   //   m_itemrangea.add(itemrange);
   //}

   //list::item_range::item_range()
   //{
   //   m_iLowerBound = -1;
   //   m_iUpperBound = -1;
   //}

   //index list::item_range::get_lower_bound() const
   //{
   //   return m_iLowerBound;
   //}

   //index list::item_range::get_upper_bound() const
   //{
   //   return m_iUpperBound;
   //}



   //list::item_range::item_range(const item_range & itemrange)
   //{
   //   operator =(itemrange);
   //}

   //list::item_range & list::item_range::operator =(const item_range & itemrange)
   //{
   //   m_iLowerBound         = itemrange.m_iLowerBound;
   //   m_iUpperBound         = itemrange.m_iUpperBound;
   //   m_subitemrange    = itemrange.m_subitemrange;
   //   return *this;
   //}



   //list::sub_item_range::sub_item_range()
   //{
   //   m_iLowerBound = -1;
   //   m_iUpperBound = -1;
   //}

   //list::sub_item_range::sub_item_range(const sub_item_range & subitemrange)
   //{
   //   operator =(subitemrange);
   //}

   //list::sub_item_range & list::sub_item_range::operator =(const sub_item_range & subitemrange)
   //{
   //   m_iLowerBound         = subitemrange.m_iLowerBound;
   //   m_iUpperBound         = subitemrange.m_iUpperBound;
   //   m_meshitemrange   = subitemrange.m_meshitemrange;
   //   return *this;
   //}

   //bool list::sub_item_range::has_sub_item(index iSubItem) const
   //{
   //   if(m_iLowerBound == -1 || m_iUpperBound == -1)
   //      return false;
   //   if(iSubItem >= m_iLowerBound
   //      && iSubItem <= m_iUpperBound)
   //      return true;
   //   else
   //      return false;
   //}


   //list::range & list::range::operator = (const range & range)
   //{
   //   m_itemrangea.copy(range.m_itemrangea);
   //   return *this;
   //}

   void list::_001GetSelection(range &range)
   {
      range = m_rangeSelection;
   }

   void list::_001GetSelection(
      ::database::id & key,
      ::database::selection &selection)
   {
      if (!_001HasConfigId(key))
         return;
      int_ptr iFilterSubItem = _001ConfigIdToColumnKey(key);
      range & range = m_rangeSelection;
      for (index i = 0; i < range.get_item_count(); i++)
      {
         item_range & itemrange = range.ItemAt(i);
         if (itemrange.has_sub_item(iFilterSubItem))
         {
            for (index iLine = itemrange.get_lower_bound(); iLine <= itemrange.get_upper_bound(); iLine++)
            {
               selection.add_item(key.m_id + "." + ::str::from(iLine));
            }
         }
      }
   }


   /*index list::range::get_item(index iItemIndex)
   {
   index iFirst = 0;
   index i = 0;
   index iItem;

   while(true)
   {
   if(i >= m_itemrangea.get_size())
   return -1;
   iItem = iItemIndex - iFirst;
   item_range & itemrange = m_itemrangea[i];
   if(iItem < itemrange.get_count())
   {
   return itemrange.get_item(iItem);
   }
   iFirst += itemrange.get_count();
   i++;
   }



   }*/

   /*index list::item_range::get_count()
   {
   return m_iItemEnd - m_iItemStart + 1;
   }*/

   /*index list::item_range::get_item(index iItemIndex)
   {
   return m_iItemStart + iItemIndex;
   }*/

   //::count list::range::get_item_count() const
   //{
   //   return m_itemrangea.get_size();
   //   /*   index iCount = 0;

   //   for(index i = 0; i < m_itemrangea.get_size(); i++)
   //   {
   //   item_range & itemrange = m_itemrangea[i];
   //   iCount += itemrange.get_count();
   //   }

   //   return iCount;*/
   //}

   //list::item_range & list::range::ItemAt(index iItem)
   //{
   //   return m_itemrangea.element_at(iItem);
   //}

   ::count list::_001GetSelectedItemCount()
   {
      return m_rangeSelection.get_item_count();
   }

   ::count list::_001GetSelectedItems(index_array  & ia)
   {
      return m_rangeSelection.get_items(ia);
   }

   id list::_001GetColumnTextId(index iColumn)
   {

      list_column * pcolumn = m_columna.get_visible(iColumn);

      if (pcolumn == NULL)
         return id();

      return pcolumn->m_uiText;

   }

   void list::_001OnLButtonDblClk(signal_details * pobj)
   {
      SCAST_PTR(::message::mouse, pmouse, pobj);
      m_iClick = 2;
      
      point pt = pmouse->m_pt;
      
      ScreenToClient(&pt);


      //   _001OnClick(nFlags, point);
      //Redraw();
      
      index iItem = -1;
      
      if(!m_bHoverSelect)
      {
         
         if (_001DisplayHitTest(pt, iItem))
         {
            
            if (iItem >= 0 && m_iItemMouseDown == iItem)
            {
               
               iItem = m_iconlayout.m_iaDisplayToStrict.get_b(iItem);
               
               if (iItem >= 0)
               {
                  
                  _001OnItemClick(iItem);
                  
               }
               
            }
            
         }
         
      }


      /* trans window_id wndidNotify = pwnd->GetOwner()->GetSafeoswindow_();
      if(wndidNotify == NULL)
      wndidNotify = pwnd->GetParent()->GetSafeoswindow_();*/


      LRESULT lresult = 0;

      /* trans if(wndidNotify)
      {
      NMLISTVIEW nm;
      nm.hdr.idFrom = pwnd->GetDlgCtrlId();
      nm.hdr.code =   NM_DBLCLK;
      nm.hdr.oswindowFrom = pwnd->GetSafeoswindow_();
      lresult = ::SendMessage(
      wndidNotify,
      WM_NOTIFY,
      nm.hdr.idFrom,
      (LPARAM) &nm);


      }*/

      if (lresult == 0)
      {
         pmouse->m_bRet = false;
         return;
      }
      else
      {
         pmouse->m_bRet = true;
         return;
      }
   }

   void list::HeaderCtrlLayout()
   {

      if (m_plistheader == NULL)
         return;

      point ptOffset = get_viewport_offset();

      m_plistheader->SetWindowPos(ZORDER_TOP, -ptOffset.x, 0, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);

   }

   index list::HeaderCtrlMapColumnToOrder(index iColumn)
   {

      if (m_plistheader == NULL)
         return iColumn;

      return m_plistheader->MapItemToOrder(iColumn);

   }

   bool list::_001OnHeaderCtrlEndDrag(WPARAM wparam, LPARAM lparam)
   {
      UNREFERENCED_PARAMETER(wparam);
      UNREFERENCED_PARAMETER(lparam);
      for (index iColumn = 0; iColumn < m_columna.VisibleGetCount(); iColumn++)
      {
         list_column * pcolumn = m_columna.get_visible(iColumn);
         pcolumn->m_iOrder = HeaderCtrlMapColumnToOrder(iColumn);
      }

      _001OnColumnChange();

      DISaveOrder();

      Redraw();

      return true;

   }

   /*LRESULT list::OnEndColumnHeaderDragMessage(WPARAM wparam, LPARAM lparam)
   {
   HeaderCtrlOnEndDrag();
   return 0;
   }*/

   /*LRESULT list::OnEndColumnHeaderTrackMessage(WPARAM wparam, LPARAM lparam)
   {
   HeaderCtrlOnEndTrack();
   return 0;
   }*/

   /*LRESULT list::OnColumnHeaderTrackMessage(WPARAM wparam, LPARAM lparam)
   {
   HeaderCtrlOnTrack();
   return 0;
   }*/

   bool list::_001OnHeaderCtrlEndTrack(WPARAM wparam, LPARAM lparam)
   {
      UNREFERENCED_PARAMETER(wparam);
      UNREFERENCED_PARAMETER(lparam);
      _001OnHeaderCtrlTrack(0, 0);

      string str;
      index i;
      index width;

      for (i = 0; i < m_columna.get_count(); i++)
      {
         str.Format("list_column[%d].width", i);
         width = m_columna.element_at(i)->m_iWidth;
         data_set(str, width);
      }

      return true;
   }

   bool list::_001OnHeaderCtrlTrack(WPARAM wparam, LPARAM lparam)
   {
      UNREFERENCED_PARAMETER(wparam);
      UNREFERENCED_PARAMETER(lparam);
      //    for(index iColumn = 0; iColumn < m_columna.VisibleGetCount(); iColumn++)
        //  {
    //         list_column & column = m_columna.get_visible(iColumn);
             //column.m_iWidth = m_plistheader->GetItemWidth(iColumn);
          //}

      _001OnColumnChange();

      Redraw();

      return true;
   }

   void list::_001ShowSubItem(index iSubItem, bool bShow)
   {
      string str;
      str.Format("SubItem[%d].Visible", iSubItem);
      data_set(str, bShow ? 1 : 0);
      m_columna.ShowSubItem(iSubItem, bShow);
      _001OnColumnChange();
      Redraw();

   }

   //list_column * list_column_array::_001GetByKey(index iKey)
   //{

   //   for (index i = 0; i < this->get_size(); i++)
   //   {
   //      list_column * pcolumn = element_at(i);
   //      if (pcolumn->m_iKey == iKey)
   //      {
   //         return pcolumn;
   //      }
   //   }

   //   return NULL;

   //}

   ::index list_column_array::get_index(list_column * pcolumn)
   {

      return find_first(pcolumn);

   }

   ::index list_column_array::get_visible_index(list_column * pcolumn)
   {

      ::count cVisible = 0;

      for (index i = 0; i < this->get_size(); i++)
      {

         list_column * pcolumn = element_at(i);

         if (pcolumn->m_bVisible)
         {

            if (pcolumn == element_at(i))
            {

               return cVisible;

            }

            cVisible++;

         }

      }

      return -1;

   }


   list_column * list_column_array::get_visible(index iIndex)
   {

      ::count cVisible = 0;

      for (index i = 0; i < this->get_size(); i++)
      {

         list_column * pcolumn = element_at(i);

         if (pcolumn->m_bVisible)
         {

            if (iIndex == cVisible)
            {

               return pcolumn;

            }

            cVisible++;

         }

      }

      return NULL;

   }

   list_column * list_column_array::get_by_index(index iIndex)
   {

      if (iIndex < 0)
      {

         return NULL;

      }

      if (iIndex >= get_count())
      {

         return NULL;

      }

      return element_at(iIndex);

   }


   index list_column_array::subitem_index(index iSubItem)
   {

      for (index iIndex = 0; iIndex < this->get_size(); iIndex++)
      {

         list_column * pcolumn = element_at(iIndex);

         if (pcolumn != NULL && pcolumn->m_iSubItem == iSubItem)
         {

            return iIndex;

         }

      }

      return -1;

   }

   index list_column_array::subitem_visible_index(index iSubItem)
   {

      ::count cVisible = 0;

      for (index iIndex = 0; iIndex < this->get_size(); iIndex++)
      {

         list_column * pcolumn = element_at(iIndex);

         if (pcolumn == NULL)
         {

            continue;

         }

         if (pcolumn->m_bVisible)
         {

            if (pcolumn->m_iSubItem == iSubItem)
            {

               return cVisible;

            }

            cVisible++;

         }

      }

      return -1;

   }


   list_column * list_column_array::get_by_subitem(index iSubItem)
   {

      index iIndex = subitem_index(iSubItem);

      return get_by_index(iIndex);


   }

   //list_column * list_column_array::GlobalOrderGetPrevious(index iKey)
   //{
   //   for (index i = 0; i < this->get_size(); i++)
   //   {
   //      list_column * pcolumn = element_at(i);
   //      if (pcolumn->m_iNextGlobalOrderKey == iKey)
   //      {
   //         return pcolumn;
   //      }
   //   }
   //   return NULL;
   //}

   //list_column * list_column_array::GlobalOrderGetNext(index iIndex)
   //{
   //   list_column * pcolumn = element_at(iKey);
   //   return _001GetByKey(pcolumn->m_iNextGlobalOrderKey);
   //}


   //list_column * list_column_array::get_visible(index iKeyVisible)
   //{

   //   ::count cVisible = 0;

   //   for (index i = 0; i < this->get_size(); i++)
   //   {
   //      
   //      list_column * pcolumn = element_at(i);
   //      
   //      if(pcolumn->m_bVisible)
   //      {

   //         if (cVisible == iKeyVisible)
   //         {

   //            return pcolumn;

   //         }

   //         cVisible++;
   //      
   //      }

   //   }

   //   return NULL;

   //}


   //list_column * list_column_array::_001GetNonVisible(index iKeyNonVisible)
   //{

   //   if (iKeyNonVisible < 0)
   //   {

   //      return NULL;

   //   }

   //   if (iKeyNonVisible >= get_size())
   //   {

   //      return NULL;

   //   }

   //   return element_at(iKeyNonVisible);

   //}


   index list_column_array::add(list_column &column)
   {

      list_column * pcolumn = canew(list_column(column));

      ::smart_pointer_array < list_column >::add(pcolumn);

      pcolumn->m_iOrder = this->get_size();

      pcolumn->m_pcontainer = this;

      OnChange();

      return smart_pointer_array < list_column >::get_upper_bound();

   }


   void list_column_array::remove_all()
   {

      smart_pointer_array < list_column >::remove_all();

      OnChange();

   }

   ::count list_column_array::get_count()
   {
      return this->get_size();
   }

   list_column_array::list_column_array(::aura::application * papp) :
      object(papp)
   {
      m_plist = NULL;
   }

   void list_column_array::Initialize(list * plist)
   {
      ASSERT(plist != NULL);
      m_plist = plist;
   }

   void list_column_array::OnChange()
   {
      sort::array::quick_sort(*this, list_column::CompareKey);

      //index iKeyVisible = 0;
      //index iKeyNonVisible = 0;

      //for (index i = 0; i < this->get_size(); i++)
      //{
      //   list_column * pcolumn = element_at(i);
      //   if (pcolumn->m_bVisible)
      //   {
      //      pcolumn->m_iKeyVisible = iKeyVisible;
      //      pcolumn->m_iKeyNonVisible = -1;
      //      iKeyVisible++;
      //   }
      //   else
      //   {
      //      pcolumn->m_iKeyNonVisible = iKeyNonVisible;
      //      pcolumn->m_iKeyVisible = -1;
      //      iKeyNonVisible++;
      //   }
      //}

      sort::array::quick_sort(*this, list_column::CompareOrderSectEndNonVisible);

      for (index iOrder = 0; iOrder < this->get_size(); iOrder++)
      {
         list_column & column = *this->element_at(iOrder);
         column.m_iOrder = iOrder;
      }



   }

   void list_column_array::remove(index iColumn)
   {
      ASSERT(iColumn >= 0);
      ASSERT(iColumn < this->get_size());

      remove_at(iColumn);

      OnChange();
   }

   ::count list_column_array::VisibleGetCount()
   {
      index iCount = 0;
      for (index i = 0; i < this->get_size(); i++)
      {
         list_column * pcolumn = element_at(i);
         if (pcolumn->m_bVisible)
            iCount++;
      }
      return iCount;
   }

   //::count list_column_array::NonVisibleGetCount()
   //{
   //   index iCount = 0;
   //   for (index i = 0; i < this->get_size(); i++)
   //   {
   //      list_column * pcolumn = element_at(i);
   //      if (!pcolumn->m_bVisible)
   //         iCount++;
   //   }
   //   return iCount;
   //}

   void list_column_array::ShowSubItem(index iSubItem, bool bShow)
   {

      index iColumn = subitem_index(iSubItem);

      if (iColumn >= 0)
      {

         list_column * pcolumn = get_by_index(iColumn);

         pcolumn->m_bVisible = bShow;

         OnChange();

      }

   }


   //index list_column_array::subitem_index(index iSubItem)
   //{

   //   for (index iIndex = 0; iIndex < get_count(); iIndex++)
   //   {

   //      list_column * pcolumn = element_at(iIndex);

   //      if (pcolumn != NULL && pcolumn->m_iSubItem == iSubItem)
   //      {

   //         return iIndex;

   //      }

   //   }

   //   
   //}


   //index list_column_array::subitem_visible_index(index iSubItem)
   //{
   //   list_column * pcolumn = get_by_subitem(iSubItem);
   //   if (pcolumn == NULL)
   //      return -1;
   //   return pcolumn->get_visible_index();
   //}


   //index list_column_array::subitem_visible_index(index iSubItem)
   //{
   //   int32_t iVisible = 0;
   //   for (index iColumn = 0; iColumn < this->get_count(); iColumn++)
   //   {
   //      list_column * pcolumn = element_at(iColumn);
   //      if (pcolumn != NULL && pcolumn->m_bVisible)
   //      {
   //         if (pcolumn->m_iSubItem == iSubItem)
   //         {
   //            return iVisible;
   //         }
   //         else
   //         {
   //            iVisible++;
   //         }
   //      }
   //   }
   //   return -1;
   //}

   //index list_column_array::NonVisibleMapSubItemToColumn(index iSubItem)
   //{
   //   int32_t iNonVisible = 0;
   //   for (index iColumn = 0; iColumn < this->get_count(); iColumn++)
   //   {
   //      list_column * pcolumn = element_at(iColumn);
   //      if (!pcolumn->m_bVisible)
   //      {
   //         if (pcolumn->m_iSubItem == iSubItem)
   //         {
   //            return iNonVisible;
   //         }
   //         else
   //         {
   //            iNonVisible++;
   //         }
   //      }
   //   }
   //   return -1;

   //}



   //void list_column_array::GlobalToVisibleOrder()
   //{
   //   int_ptr iVisibleCount = VisibleGetCount();
   //   for (index iVisibleKey = 0; iVisibleKey < iVisibleCount; iVisibleKey++)
   //   {
   //      list_column * pcolumn = get_visible(iVisibleKey);
   //      pcolumn->m_iOrder = VisibleGetOrderFromKey(pcolumn->m_iKey);
   //   }


   //}

   //void list_column_array::VisibleToGlobalOrder(index iKeyA, index iKeyB)
   //{
   //   if (iKeyA == iKeyB)
   //      return;
   //   list_column * columnA = _001GetByKey(iKeyA);
   //   list_column * columnAPrevious = GlobalOrderGetPrevious(iKeyA);
   //   list_column * columnANext = GlobalOrderGetNext(iKeyA);
   //   list_column * columnB = _001GetByKey(iKeyB);
   //   list_column * columnBPrevious = GlobalOrderGetPrevious(iKeyB);
   //   list_column * columnBNext = GlobalOrderGetNext(iKeyB);

   //   if (columnA == NULL)
   //      return;

   //   if (columnB == NULL)
   //      return;

   //   if (columnAPrevious == NULL)
   //   {
   //      m_iFirstGlobalOrderKey = iKeyB;
   //   }
   //   else
   //   {
   //      if (columnAPrevious->m_iKey != iKeyB)
   //      {
   //         columnAPrevious->m_iNextGlobalOrderKey = iKeyB;
   //      }
   //   }


   //   if (columnBPrevious == NULL)
   //   {
   //      m_iFirstGlobalOrderKey = iKeyA;
   //   }
   //   else
   //   {
   //      if (columnBPrevious->m_iKey != iKeyA)
   //      {
   //         columnBPrevious->m_iNextGlobalOrderKey = iKeyA;
   //      }
   //   }

   //   if (columnANext == NULL)
   //   {
   //      columnB->m_iNextGlobalOrderKey = -1;
   //   }
   //   else
   //   {
   //      if (columnANext->m_iKey == columnB->m_iKey)
   //      {
   //         columnB->m_iNextGlobalOrderKey = iKeyA;
   //      }
   //      else
   //      {
   //         columnB->m_iNextGlobalOrderKey = columnANext->m_iKey;
   //      }
   //   }

   //   if (columnBNext == NULL)
   //   {
   //      columnA->m_iNextGlobalOrderKey = -1;
   //   }
   //   else
   //   {
   //      if (columnBNext->m_iKey == columnA->m_iKey)
   //      {
   //         columnB->m_iNextGlobalOrderKey = iKeyB;
   //      }
   //      else
   //      {
   //         columnA->m_iNextGlobalOrderKey = columnBNext->m_iKey;
   //      }
   //   }



   //}

   //void list_column_array::VisibleToGlobalOrder()
   //{
   //   list * plist = m_plist;
   //   //detects change
   //   int_ptr iVisibleCount = VisibleGetCount();
   //   int_ptr iChangeCount = 0;
   //   int_ptr iNew = 0;
   //   int_ptr iOld = 0;
   //   for (int_ptr iVisibleKey = 0; iVisibleKey < iVisibleCount; iVisibleKey++)
   //   {
   //      list_column * column = get_visible(iVisibleKey);
   //      if (VisibleGetOrderFromKey(column->m_iKey) != plist->HeaderCtrlMapColumnToOrder(iVisibleKey))
   //      {
   //         iChangeCount++;
   //         if (iChangeCount == 1)
   //         {
   //            iNew = iVisibleKey;
   //         }
   //         else if (iChangeCount == 2)
   //         {
   //            iOld = iVisibleKey;
   //         }
   //      }
   //   }
   //   if (iChangeCount == 2)
   //   {
   //      VisibleToGlobalOrder(iNew, iOld);
   //   }
   //   else
   //   {
   //      if (this->get_size() == VisibleGetCount())
   //      {
   //         for (index iColumn = 0; iColumn < VisibleGetCount(); iColumn++)
   //         {
   //            list_column * column = get_visible(iColumn);
   //            column->m_iOrder = plist->HeaderCtrlMapColumnToOrder(iColumn);
   //         }

   //         m_iFirstGlobalOrderKey = OrderToKey(0);
   //         if (m_iFirstGlobalOrderKey >= 0)
   //         {
   //            index iKey = m_iFirstGlobalOrderKey;
   //            index iNextKey;
   //            index iOrder = 1;

   //            while (true)
   //            {
   //               iNextKey = OrderToKey(iOrder);
   //               _001GetByKey(iKey)->m_iNextGlobalOrderKey = iNextKey;
   //               if (iNextKey < 0)
   //               {
   //                  break;
   //               }
   //               iOrder++;
   //               iKey = iNextKey;
   //            }
   //         }
   //      }

   //   }
   //}

   void list_column_array::DISaveOrder()
   {
      
      int_ptr iCount = this->get_size();

      string str;

      for (index i = 0; i < iCount; i++)
      {

         list_column * pcolumn = element_at(i);

         str.Format("list_column[%d].Next", i);

         m_plist->data_set(str, pcolumn->m_iOrder);

      }

   }


   void list_column_array::DILoadOrder()
   {

      int_ptr iCount = this->get_size();

      string str;

      for (index i = 0; i < iCount; i++)
      {

         list_column * pcolumn = element_at(i);

         str.Format("list_column[%d].Next", i);

         m_plist->data_get(str, pcolumn->m_iOrder);

      }

      OnChange();

   }


   //index list_column_array::VisibleIndexOrder(index iIndex)
   //{

   //   list_column * pcolumn = get_visible(iIndex);

   //   if (pcolumn == NULL)
   //   {

   //      return -1;

   //   }

   //   return pcolumn->m_iOrder;

   //}


   index list_column_array::order_index(index iOrder)
   {

      for (index iIndex = 0; iIndex < this->get_count(); iIndex++)
      {
         
         list_column * column = element_at(iIndex);
         
         if (column->m_iOrder == iOrder)
         {

            return iIndex;

         }

      }

      return -1;

   }


   void list::DISaveOrder()
   {

      m_columna.DISaveOrder();

   }

   void list::DILoadOrder()
   {
      m_columna.DILoadOrder();
      _001OnColumnChange();
   }









   void list::_001UpdateColumns()
   {

      synch_lock sl(m_pmutex);

      _001RemoveAllColumns();

      keep < bool > keepLockViewUpdate(&m_bLockViewUpdate, true, false, true);

      _001InsertColumns();

      keepLockViewUpdate.KeepAway();

      DIDDXHeaderLayout(false);

      _001OnColumnChange();

      RedrawWindow();

   }


   void list::_001InsertColumns()
   {


   }


   void list::_001RemoveAllColumns()
   {

      m_columna.remove_all();

      _001OnColumnChange();

   }


   void list::_001SetBackBuffer(visual::CBuffer *ptwb)
   {

   }


   void list::PreSubClassWindow()
   {

      if (IsWindowVisible())
      {

         RedrawWindow();

      }

   }


   void list::_001OnCreate(signal_details * pobj)
   {

      SCAST_PTR(::message::create, pcreate, pobj);

      pobj->previous();


      //m_font->operator=(*System.visual().fonts().GetListCtrlFont());

      //m_fontHover->operator=(*System.visual().fonts().GetListCtrlFont());

      //m_fontHover->set_underline();

      //get_font(m_font, font_list_item, this);

      //get_font(m_fontHover, font_list_hover, this);

      if (pcreate->get_lresult() == -1)
      {

         pobj->m_bRet = false;

         return;

      }

      if (m_bAutoCreateListHeader)
      {

         if (m_plistheader.is_null())
         {

            m_plistheader = create_list_header();

            if (m_plistheader.is_set())
            {

               m_plistheader->SetBaseListCtrlInterface(this);

            }

         }

         if (m_plistheader != NULL)
         {

            if (!CreateHeaderCtrl())
            {

               pcreate->set_lresult(-1);

               pobj->m_bRet = true;

               return;

            }

         }

      }

      class rect rect;

      rect.null();

      pcreate->set_lresult(0);

      on_ui_event(event_create, object_list, this);

      RedrawWindow();

      pobj->m_bRet = false;

   }


   void list::_001CreateImageList(list_column * pcolumn)
   {

      if (pcolumn == NULL)
         return;

      if (pcolumn->m_uiSmallBitmap == (UINT)-1)
      {
         return;
      }
      if (pcolumn->m_pil == NULL)
      {
         pcolumn->m_pil = canew(image_list(get_app()));
      }
      //      sp(image_list) pil = column.m_pil;
            //   if(pil != NULL)
            //      pil->DeleteImageList();
      ::exception::throw_not_implemented(get_app());
      /*if(!pil->create(
         MAKEINTRESOURCE(column.m_uiSmallBitmap),
         column.m_iSmallImageWidth,
         0,
         column.m_crSmallMask))
      {
      }*/
   }


   void list::_001OnTimer(::timer * ptimer)
   {
      //      return; //xxxtimer
      ::user::mesh::_001OnTimer(ptimer);;
      //      if(ptimer->m_nIDEvent == 12345679) // left click
      //      {
      //         KillTimer(12345679);
      //         if(m_iClick == 1)
      //         {
      //            m_iClick = 0;
      //            if(!_001IsEditing())
      //            {
      //               uint_ptr nFlags = m_uiLButtonUpFlags;
      //               point point = m_ptLButtonUp;
      //               _001OnClick(nFlags, point);
      //               Redraw();
      //
      //
      //               /* trans
      //               window_id wndidNotify = pwnd->GetOwner()->GetSafeoswindow_();
      //               if(wndidNotify == NULL)
      //               wndidNotify = pwnd->GetParent()->GetSafeoswindow_(); */
      //
      ////               LRESULT lresult = 0;
      //
      //               /* trans            if(wndidNotify)
      //               {
      //               NMLISTVIEW nm;
      //               nm.hdr.idFrom = pwnd->GetDlgCtrlId();
      //               nm.hdr.code =   NM_CLICK;
      //               nm.hdr.oswindowFrom = pwnd->GetSafeoswindow_();
      //               lresult = ::SendMessage(
      //               wndidNotify,
      //               WM_NOTIFY,
      //               nm.hdr.idFrom,
      //               (LPARAM) &nm);
      //               }*/
      //            }
      //         }
      //         else
      //         {
      //            m_iClick = 0;
      //         }
      //
      //
      //      }
      //      else if(ptimer->m_nIDEvent == 8477) // right click
      //      {
      //         KillTimer(8477);
      //         //if(!_001IsEditing())
      //         {
      //            uint_ptr nFlags = m_uiLButtonUpFlags;
      //            point point = m_ptLButtonUp;
      //            _001OnRightClick(nFlags, point);
      //            Redraw();
      //
      //
      //            /* trans
      //            window_id wndidNotify = pwnd->GetOwner()->GetSafeoswindow_();
      //            if(wndidNotify == NULL)
      //            wndidNotify = pwnd->GetParent()->GetSafeoswindow_(); */
      //
      ////            LRESULT lresult = 0;
      //
      //            /* trans            if(wndidNotify)
      //            {
      //            NMLISTVIEW nm;
      //            nm.hdr.idFrom = pwnd->GetDlgCtrlId();
      //            nm.hdr.code =   NM_CLICK;
      //            nm.hdr.oswindowFrom = pwnd->GetSafeoswindow_();
      //            lresult = ::SendMessage(
      //            wndidNotify,
      //            WM_NOTIFY,
      //            nm.hdr.idFrom,
      //            (LPARAM) &nm);
      //            }*/
      //         }
      //      }
      //      else if(ptimer->m_nIDEvent == 0xfffffffe)
      //      {
      //         if(!Filter1Step())
      //            KillTimer(ptimer->m_nIDEvent);
      //      }
      //      else if(ptimer->m_nIDEvent == 12345678)
      //      {
      //         KillTimer(ptimer->m_nIDEvent);
      //         m_bDrag = true;
      //      }
      //      else if(ptimer->m_nIDEvent == 12321)
      //      {
      //         KillTimer(ptimer->m_nIDEvent);
      //         index iItemSel;
      //         index iSubItemSel;
      //         point point;
      //         Session.get_cursor_pos(&point);
      //         ScreenToClient(&point);
      //         try
      //         {
      //            if(_001DisplayHitTest(point, iItemSel, iSubItemSel))
      //            {
      //               if(m_iSubItemEnter == iSubItemSel &&
      //                  m_iItemEnter == iItemSel)
      //               {
      //                  m_iSubItemEnter = -1;
      //                  m_iItemEnter = -1;
      //                  bool bLShiftKeyDown     = Session.is_key_pressed(::user::key_lshift);
      //                  bool bRShiftKeyDown     = Session.is_key_pressed(::user::key_rshift);
      //                  bool bLControlKeyDown   = Session.is_key_pressed(::user::key_lcontrol);
      //                  bool bRControlKeyDown   = Session.is_key_pressed(::user::key_rcontrol);
      //                  bool bShiftKeyDown      = bLShiftKeyDown     || bRShiftKeyDown;
      //                  bool bControlKeyDown    = bLControlKeyDown   || bRControlKeyDown;
      //
      //                  if(m_bMultiSelect && bShiftKeyDown)
      //                  {
      //                     if(bControlKeyDown)
      //                     {
      //                        item_range itemrange;
      //                        itemrange.set(
      //                           MIN(iItemSel, m_iItemSel),
      //                           MAX(iItemSel, m_iItemSel),
      //                           MIN(iSubItemSel, m_iSubItemSel),
      //                           MAX(iSubItemSel, m_iSubItemSel),
      //                           -1,
      //                           -1);
      //                        _001AddSelection(itemrange);
      //                     }
      //                     else
      //                     {
      //                        item_range itemrange;
      //                        itemrange.set(
      //                           MIN(iItemSel, m_iItemSel),
      //                           MAX(iItemSel, m_iItemSel),
      //                           MIN(iSubItemSel, m_iSubItemSel),
      //                           MAX(iSubItemSel, m_iSubItemSel),
      //                           -1,
      //                           -1);
      //                        range range;
      //                        range.add_item(itemrange);
      //                        _001SetSelection(range);
      //                     }
      //                  }
      //                  else if(m_bMultiSelect && bControlKeyDown)
      //                  {
      //                     m_iLastItemSel = m_iItemSel;
      //                     m_iLastSubItemSel = m_iSubItemSel;
      //                     m_iItemSel = iItemSel;
      //                     m_iSubItemSel = iSubItemSel;
      //                     item_range itemrange;
      //                     itemrange.set(
      //                        m_iItemSel,
      //                        m_iItemSel,
      //                        m_iSubItemSel,
      //                        m_iSubItemSel,
      //                        -1,
      //                        -1);
      //                     _001AddSelection(itemrange);
      //                  }
      //                  else
      //                  {
      //                     m_iLastItemSel = m_iItemSel;
      //                     m_iLastSubItemSel = m_iSubItemSel;
      //                     m_iItemSel = iItemSel;
      //                     m_iSubItemSel = iSubItemSel;
      //                     item_range itemrange;
      //                     itemrange.set(
      //                        m_iItemSel,
      //                        m_iItemSel,
      //                        m_iSubItemSel,
      //                        m_iSubItemSel,
      //                        -1,
      //                        -1);
      //                     range range;
      //                     range.add_item(itemrange);
      //                     _001SetSelection(range);
      //                  }
      //               }
      //            }
      //         }
      //         catch(...)
      //         {
      //         }
      //         m_iSubItemEnter = -1;
      //         m_iItemEnter = -1;
      //
      //      }
      //
      ptimer->m_bRet = false;
   }

   bool list::_001IsItemVisible(index iItem)
   {
      draw_list_item item(this);
      item.m_iItem = iItem;
      item.m_iDisplayItem = iItem;
      _001GetItemRect(&item);
      if (!item.m_bOk)
      {
         return false;
      }
      rect rectClient;
      GetClientRect(&rectClient);
      return rectClient.intersect(rectClient, item.m_rectItem) != 0;
   }

   void list::_001ClearSelection()
   {
      m_rangeSelection.clear();
      _001OnSelectionChange();
   }

   void list::_001SetSelection(const range &range)
   {
      m_rangeSelection = range;
      on_select();
      _001OnSelectionChange();
   }

   void list::_001AddSelection(const item_range & itemrange)
   {
      m_rangeSelection.add_item(itemrange);
      on_select();
      _001OnSelectionChange();
   }

   void list::_001SetHighlightRange(range & range)
   {
      m_rangeHighlight = range;
   }

   void list::DIOnSectionSet()
   {
      string str;
      str = m_dataid.get_id();
      str += ".headerctrl";
      if (m_plistheader != NULL)
      {
         m_plistheader->m_dataid = str;
      }
   }

   bool list::DIDDXHeaderLayout(bool bSave)
   {

      if (m_plistheader == NULL)
         return false;

      return m_plistheader->DIDDXLayout(bSave);
   }

   void list::_001SetTopText(const unichar * lpcwsz)
   {
      m_strTopText = lpcwsz;
      _001LayoutTopText();
   }

   void list::_001LayoutTopText()
   {

      ::draw2d::memory_graphics pgraphics(allocer());

      select_font(pgraphics, font_list_item, this);

      array < size > sizea;
      m_dcextension.GetTextExtent(pgraphics, m_strTopText, sizea);
      rect rectClient;
      GetClientRect(rectClient);
      index x = 0;
      index right = rectClient.right;
      index y = 0;
      for (index i = 0; i < sizea.get_size(); i++)
      {
         if ((sizea[i].cx - x > right)
            || i == sizea.get_upper_bound())
         {
            if (i == 0)
            {
               x = sizea[0].cx;
               y += sizea[0].cy;
            }
            else
            {
               x = sizea[i - 1].cx;
               y += sizea[i - 1].cy;
            }
         }
      }

      m_rectTopText.left = 0;
      m_rectTopText.top = 0;
      m_rectTopText.right = rectClient.right;
      m_rectTopText.bottom = (LONG)y;


   }

   void list::_001ShowTopText(bool bShow)
   {
      m_bTopText = bShow;
   }




   void list::CacheHint()
   {

      synch_lock sl(m_pmutex);

      if (m_pcache != NULL)
      {

         int_ptr iItemCount = m_nDisplayCount;

         int_ptr iItemFirst = m_iTopIndex;

         m_pcache->_001CacheHint(this, iItemFirst, iItemCount);

      }

   }


   void list::SetDataInterface(::user::mesh_data *pinterface)
   {

      mesh::SetDataInterface(pinterface);

      m_psimplelistdata = m_pmeshdata;

   }


   bool list::_001InsertColumn(list_column &column)
   {

      _001AddColumn(column);
      _001OnColumnChange();
      return true;
   }


   void list::_001SetSingleColumnMode(bool bHeaderCtrl)
   {

      m_bSingleColumnMode = true;

      m_bHeaderCtrl = bHeaderCtrl;
      _001RemoveAllColumns();

      list_column column;
      column.m_iWidth = -1;
      column.m_iSubItem = 0;
      column.m_iSmallImageWidth = 16;
      column.m_crSmallMask = ARGB(255, 255, 0, 255);


      _001InsertColumn(column);

      RedrawWindow();

   }


   int32_t list::_001CalcItemWidth(index iItem, index iSubItem)
   {

      ::draw2d::memory_graphics pgraphics(allocer());

      select_font(pgraphics, font_list_item, this);

      index cx = _001CalcItemWidth(pgraphics, iItem, iSubItem);

      return (int32_t)cx;

   }


   int32_t list::_001CalcItemWidth(::draw2d::graphics * pgraphics, ::draw2d::font * pfont, index iItem, index iSubItem)
   {
      pgraphics->SelectObject(pfont);
      return _001CalcItemWidth(pgraphics, iItem, iSubItem);
   }

   int32_t list::_001CalcItemWidth(::draw2d::graphics * pgraphics, index iItem, index iSubItem)
   {
#ifdef WINDOWSEX
      ::image_list::info ii;
      rect rect;
      size size;
      index cx = 0;
      list_column * pcolumn = m_columna.get_by_subitem(iSubItem);
      draw_list_item item(this);
      item.m_iItem = iItem;
      item.m_iSubItem = iSubItem;
      item.m_iListItem = -1;
      if (pcolumn->m_pil != NULL)
      {
         _001GetItemImage(&item);
         if (item.m_bOk && item.m_iImage >= 0)
         {
            pcolumn->m_pil->get_image_info((int32_t)item.m_iImage, &ii);
            rect = ii.m_rect;
            cx += rect.width();
            cx += 2;
         }
      }
      _001GetItemText(&item);
      if (item.m_bOk)
      {
         m_dcextension.GetTextExtent(pgraphics, item.m_strText, size);
         cx += size.cx;
      }

      return (int32_t)cx;
#else
      throw todo(get_app());
#endif
   }




   bool list::_001HasConfigId(const ::database::id & key)
   {
      return _001ConfigIdToColumnKey(key) >= 0;
   }

   index list::_001ConfigIdToSubItem(const ::database::id & key)
   {
      list_column * column = m_columna.get_by_config_id(key);
      if (column == NULL)
         return -1;
      return column->m_iSubItem;
   }

   
   index list::config_id_index(const ::database::id & key)
   {
      
      return m_columna.config_id_index(key);

   }


   list_column * list_column_array::get_by_config_id(const ::database::id & key)
   {
      index iKey = config_id_index(key);
      if (iKey >= 0)
         return element_at(iKey);
      else
         return NULL;

   }


   index list_column_array::config_id_index(const ::database::id & key)
   {

      for (index iIndex = 0; iIndex < this->get_size(); iIndex++)
      {

         if (this->element_at(iIndex)->m_datakey == key)
         {

            return iIndex;

         }

      }

      return -1;

   }


   index list_column_array::config_id_visible_index(const ::database::id & key)
   {

      ::count cVisible = 0;

      for (index iIndex = 0; iIndex < this->get_size(); iIndex++)
      {
         
         if (this->element_at(iIndex)->m_bVisible)
         {

            if (this->element_at(iIndex)->m_datakey == key)
            {

               return cVisible;

            }

            cVisible++;

         }

      }

      return -1;

   }



   //void list::item_range::set(index iLowerBoundItem, index iUpperBoundItem, index iLowerBoundSubItem, index iUpperBoundSubItem, index iLowerBoundListItem, index iUpperBoundListItem)
   //{

   //   m_iLowerBound = iLowerBoundItem;
   //   m_iUpperBound = iUpperBoundItem;
   //   m_subitemrange.set(
   //      iLowerBoundSubItem,
   //      iUpperBoundSubItem,
   //      iLowerBoundListItem,
   //      iUpperBoundListItem);
   //}

   //void list::item_range::set_lower_bound(index iLowerBoundItem)
   //{
   //   m_iLowerBound = iLowerBoundItem;
   //}

   //void list::item_range::set_upper_bound(index iUpperBoundItem)
   //{
   //   m_iUpperBound = iUpperBoundItem;
   //}

   //void list::sub_item_range::set(index iLowerBoundSubItem, index iUpperBoundSubItem, index iLowerBoundListItem, index iUpperBoundListItem)
   //{
   //   m_iLowerBound = iLowerBoundSubItem;
   //   m_iUpperBound = iUpperBoundSubItem;
   //   m_meshitemrange.set(
   //      iLowerBoundListItem,
   //      iUpperBoundListItem);
   //}

   //void list::list_item_range::set(index iLowerBoundListItem, index iUpperBoundListItem)
   //{
   //   m_iLowerBound = iLowerBoundListItem;
   //   m_iUpperBound = iUpperBoundListItem;

   //}


   void list::_001EnsureVisible(index iItem, bool bRedraw)
   {
      point ptOffset = get_viewport_offset();

      if (m_iItemHeight > 0
         &&
         (iItem < ptOffset.y / m_iItemHeight ||
            iItem >= ptOffset.y / m_iItemHeight + m_nDisplayCount))
      {
         ptOffset.y = (LONG)(iItem * m_iItemHeight);
         set_viewport_offset_y(ptOffset.y);
         on_change_viewport_offset();
         if (bRedraw)
         {
            RedrawWindow();
         }
      }
   }

   void list::_001ItemScroll(index iItem, bool bRedraw)
   {
      if (iItem < m_nItemCount)
      {

         point ptOffset = get_viewport_offset();


         ptOffset.y = (LONG)(iItem * m_iItemHeight);
         on_change_viewport_offset();
         if (bRedraw)
         {
            RedrawWindow();
         }
      }
   }

   void list::_001EnsureVisible(index iItem, range & range)
   {

      point ptOffset = get_viewport_offset();


      index iyScroll = ptOffset.y / MAX(1, m_iItemHeight);
      if (iItem < iyScroll)
      {
         iyScroll = iItem - m_nDisplayCount + 1;
      }
      else if (iItem >= iyScroll + m_nDisplayCount)
      {
         iyScroll = iItem;
      }
      if (ptOffset.y / MAX(1, m_iItemHeight) != iyScroll)
      {
         item_range item;
         ptOffset.y = (LONG)(iyScroll * m_iItemHeight);
         on_change_viewport_offset();
         item.set_lower_bound(iyScroll);
         item.set_upper_bound(MIN(iyScroll + m_nDisplayCount - 1, m_nItemCount - 1));
         range.add_item(item);
      }
   }

   void list::_001Highlight(index iItem, bool bRedraw)
   {
      m_rangeHighlight.clear();
      item_range itemrange;
      itemrange.set(iItem, iItem, 0, m_columna.get_count() - 1, -1, -1);
      m_rangeHighlight.add_item(itemrange);
      if (bRedraw)
      {
         RedrawWindow();
      }
   }


   bool list::_001OnRemoveItem(index iItem)
   {
      UNREFERENCED_PARAMETER(iItem);
      return false;
   }

   bool list::_001RemoveItem(index iItem, bool bRedraw)
   {
      if (!_001OnRemoveItem(iItem))
         return false;
      m_rangeSelection.OnRemoveItem(iItem);
      if (bRedraw)
      {
         RedrawWindow();
      }
      return true;
   }

   //void list::item_range::offset(index iOffset)
   //{
   //   m_iLowerBound += iOffset;
   //   m_iUpperBound += iOffset;
   //}

   void list::_001RemoveSelection()
   {
      range range;

      _001GetSelection(range);


      while (range.get_item_count() > 0)
      {
         index iItem = range.ItemAt(0).get_lower_bound();
         if (!_001RemoveItem(iItem, false))
            break;
         _001GetSelection(range);
      }

      RedrawWindow();
   }


   void list::_001Select(index iItem, index iSubItem)
   {
      m_rangeSelection.clear();
      item_range itemrange;
      itemrange.set(iItem, iItem, iSubItem, iSubItem, -1, -1);
      m_rangeSelection.add_item(itemrange);

   }

   index list::StrictToDisplay(index iStrict)
   {
      UNREFERENCED_PARAMETER(iStrict);
      return -1;
   }

   index list::DisplayToStrict(index iDisplay)
   {
      if (iDisplay < 0)
         return -1;
      if (m_bDrag && does_drag_reorder())
      {
         if (m_eview == view_icon)
         {

            return m_iconlayout.m_iaDisplayToStrict[iDisplay];
            //if(m_iconlayout.m_iaDisplayToStrict[m_iItemDrop] >= 0 && m_iconlayout.m_iaDisplayToStrict[m_iItemDrop] < m_nItemCount)
            //{
            //   return m_iconlayout.m_iaDisplayToStrict[iDisplay];
            //}
            //else
            //{
            //   //if(iDisplay == m_iItemDrop)
            //   //{
            //   //   return m_iconlayout.m_iaDisplayToStrict[m_iItemDrag];
            //   //}
            //   //else if(iDisplay == m_iItemDrag)
            //   //{
            //   //   return -1;
            //   //}
            //   //else
            //   {
            //      return m_iconlayout.m_iaDisplayToStrict[iDisplay];
            //   }
            //}
         }
         else
         {
            if (iDisplay < m_iItemDrag && iDisplay < m_iItemDrop)
            {
               return m_iconlayout.m_iaDisplayToStrict[iDisplay];
            }
            else if (iDisplay > m_iItemDrag && iDisplay > m_iItemDrop)
            {
               return m_iconlayout.m_iaDisplayToStrict[iDisplay];
            }
            else
            {
               if (iDisplay == m_iItemDrop)
               {
                  return m_iconlayout.m_iaDisplayToStrict[m_iItemDrag];
               }
               else if (m_iItemDrop > m_iItemDrag)
               {
                  return m_iconlayout.m_iaDisplayToStrict[iDisplay + 1];
               }
               else if (m_iItemDrag > m_iItemDrop)
               {
                  return m_iconlayout.m_iaDisplayToStrict[iDisplay - 1];
               }
               else
               {
                  return m_iconlayout.m_iaDisplayToStrict[iDisplay];
               }
            }
         }
      }
      else if (m_eview == view_icon)
      {
         return m_iconlayout.m_iaDisplayToStrict[iDisplay];
      }
      else
      {
         if (iDisplay < 0)
            return -1;
         if (m_meshlayout.m_iaDisplayToStrict.get_count() <= 0)
         {
            if (iDisplay < m_nItemCount)
               return iDisplay;
         }
         //if(m_meshlayout.m_iaDisplayToStrict.get_count() != m_nItemCount)
           // return iDisplay;
         if (iDisplay >= m_meshlayout.m_iaDisplayToStrict.get_count())
         {
            return -1;
         }
         return m_meshlayout.m_iaDisplayToStrict[iDisplay];
      }
   }

   void list::FilterBegin()
   {
      m_efilterstate = FilterStateSetup;

      index iItemCount = m_nItemCount;

      if (m_eview == view_icon)
      {
         m_piaFilterIcon->remove_all();

         for (index i = 0; i < iItemCount; i++)
         {
            m_piaFilterIcon->add(i);
         }
      }
      else
      {
         m_piaFilterMesh->remove_all();

         for (index i = 0; i < iItemCount; i++)
         {
            m_piaFilterMesh->add(i);
         }
      }

   }

   void list::FilterApply()
   {

      //      ASSERT(m_efilterstate == FilterStateSetup);

      m_iFilter1Step = 0;

      SetTimer(0xfffffffe, 50, NULL);

      set_viewport_offset(0, 0);

      m_efilterstate = FilterStateFilter;

      on_change_view_size();

      RedrawWindow();

   }


   void list::FilterClose()
   {

      m_bFilter1 = false;

      KillTimer(0xfffffffe);

      ASSERT(m_efilterstate == FilterStateSetup
         || m_efilterstate == FilterStateFilter);

      index iItemCount = m_nItemCount;

      if (m_eview == view_icon)
      {
         m_iconlayout.m_iaDisplayToStrict.remove_all();

         for (index i = 0; i < iItemCount; i++)
         {
            m_iconlayout.m_iaDisplayToStrict.add(i);
         }
      }
      else
      {
         m_meshlayout.m_iaDisplayToStrict.remove_all();

         for (index i = 0; i < iItemCount; i++)
         {
            m_meshlayout.m_iaDisplayToStrict.add(i);
         }
      }

      on_change_view_size();

      RedrawWindow();

   }


   void list::FilterExcludeAll()
   {

      if (m_eview == view_icon)
      {

         m_piaFilterIcon->remove_all();

      }
      else
      {

         m_piaFilterMesh->remove_all();

      }

   }


   void list::FilterInclude(index iItem)
   {
      ASSERT(m_efilterstate == FilterStateSetup);
      if (m_eview == view_icon)
      {
         m_piaFilterIcon->add_unique(iItem);
      }
      else
      {
         m_piaFilterMesh->add_unique(iItem);
      }
   }

   void list::FilterInclude(int_array & array)
   {
      ASSERT(m_efilterstate == FilterStateSetup);
      for (index i = 0; i < array.get_size(); i++)
      {
         FilterInclude(array[i]);
      }
   }


   bool list::Filter1Step()
   {
      uint32_t dwIn = get_tick_count();
      uint32_t dwOut;
      TRACE("list::Filter1Step\n");
      TRACE("dwIn = %d\n", dwIn);
      if (!m_bFilter1)
         return false;

      string wstrItem;

      index iItemCount = MIN(m_nItemCount, m_iFilter1Step + 1000);

      index iFilter1Step;

      draw_list_item item(this);

      for (
         iFilter1Step = m_iFilter1Step;
         iFilter1Step < iItemCount;
         iFilter1Step++)
      {
         for (index j = 0; j < m_columna.get_count(); j++)
         {
            list_column * pcolumn = m_columna.get_by_index(j);
            item.m_strText.Empty();
            item.m_iItem = iFilter1Step;
            item.m_iSubItem = pcolumn->m_iSubItem;
            item.m_iListItem = -1;
            _001SearchGetItemText(&item);
            if (item.m_bOk)
            {
               item.m_strText.make_lower();
               item.m_strText.replace("_", " ");
               if (m_reFilter1.match(item.m_strText))
               {
                  if (m_eview == view_icon)
                  {
                     m_piaFilterIcon->add_unique(iFilter1Step);
                  }
                  else
                  {
                     m_piaFilterMesh->add_unique(iFilter1Step);
                  }
                  break;
               }
            }
         }
      }

      m_iFilter1Step = (int32_t)iFilter1Step;

      if (m_eview == view_icon)
      {
         m_iconlayout.m_iaDisplayToStrict = (*m_piaFilterIcon);
      }
      else
      {
         m_meshlayout.m_iaDisplayToStrict = (*m_piaFilterMesh);
      }

      set_viewport_offset(0, 0);

      m_efilterstate = FilterStateFilter;

      on_change_view_size();

      RedrawWindow();


      dwOut = get_tick_count();
      TRACE("dwOut = %d\n", dwOut);
      TRACE("(delta) = %d\n", dwOut - dwIn);

      return m_nItemCount != iItemCount;

   }


   void list::Filter1(string & strFilter)
   {

      if (m_eview == view_icon)
      {
         m_piaFilterIcon->remove_all();
      }
      else
      {
         m_piaFilterMesh->remove_all();
      }

      stringa stra;

      stra.add_tokens(strFilter, " ", false);

      m_reFilter1.setPositionMoves(1);

      m_bFilter1 = m_reFilter1.setRE("/.*" + stra.implode(".*") + ".*/i");

      if (m_bFilter1)
      {
         FilterApply();
      }
      else
      {
         FilterClose();
      }

   }

   void list::_001OnListHeaderItemClick(index iHeaderItem)
   {
      if (!m_bSortEnable)
         return;
      _001OnSort(_001MapColumnToSubItem(iHeaderItem));
   }

   void list::_001OnListHeaderItemDblClk(index iHeaderItem)
   {
      _001MaximizeColumnWidth(iHeaderItem);
   }


   void list::_001OnSort()
   {
      if (!m_bSortEnable)
         return;
      if (m_eview != view_icon)
      {
         ::sort::quick_sort(m_meshlayout.m_iaDisplayToStrict, this, (::sort::compare_interface::_FUNCTION_Compare) &::user::mesh::_001Compare);
      }
      else
      {
      }
   }

   void list::_001OnSort(index iSubItem)
   {

      index iFound = -1;
      for (index i = 0; i < m_sortinfo.m_itema.get_size(); i++)
      {
         CSortInfoItem & item = m_sortinfo.m_itema[i];
         if (item.m_iSubItem == iSubItem)
         {
            iFound = i;
            break;
         }
      }
      if (iFound >= 0)
      {
         CSortInfoItem newItem = m_sortinfo.m_itema[iFound];
         m_sortinfo.m_itema.remove_at(iFound);
         newItem.m_bAscendent = !newItem.m_bAscendent;
         m_sortinfo.m_itema.insert_at(0, newItem);
      }
      else
      {
         CSortInfoItem newItem;
         newItem.m_iSubItem = iSubItem;
         newItem.m_bAscendent = true;
         m_sortinfo.m_itema.insert_at(0, newItem);
      }

      _001OnSort();

   }




   index list::_002Compare(index iItem1, index iItem2, index iSubItem)
   {
      draw_list_item item1(this);
      draw_list_item item2(this);

      item1.m_iItem = iItem1;
      item2.m_iItem = iItem2;
      item1.m_iSubItem = iSubItem;
      item2.m_iSubItem = iSubItem;

      _001GetItemText(&item1);
      _001GetItemText(&item2);

      if (item1.m_bOk && item2.m_bOk)
      {
         return item1.m_strText.compare_ci(item2.m_strText);
      }
      else
      {
         if (item1.m_bOk)
            return 1;
         else if (item2.m_bOk)
            return -1;
         else
            return 0;
      }

   }

   index list::_001Compare(index iItem1, index iItem2)
   {
      CSortInfoItem * pitem = NULL;
      index iCompare = 0;
      for (index i = 0; i < m_sortinfo.m_itema.get_size(); i++)
      {
         pitem = &m_sortinfo.m_itema[i];
         iCompare = _002Compare(iItem1, iItem2, pitem->m_iSubItem);
         if (iCompare != 0)
            break;
      }
      if (pitem == NULL)
         return 0;
      else
      {
         if (pitem->m_bAscendent)
            return iCompare;
         else
            return -iCompare;
      }
   }


   index list::_001DisplayCompare(index iDisplayItem1, index iDisplayItem2)
   {
      return _001Compare(DisplayToStrict(iDisplayItem1), DisplayToStrict(iDisplayItem2));
   }


   int32_t list::_001CalcListWidth()
   {
      ASSERT(FALSE);
      return -1;
   }

   int32_t list::_001CalcColumnWidth(index iColumn)
   {
      UNREFERENCED_PARAMETER(iColumn);
      ::draw2d::memory_graphics pgraphics(allocer());
      select_font(pgraphics, font_list_item, this);
      int32_t iMaxWidth = 0;
      ::count iCount = m_nItemCount;
      int32_t iWidth;
      for (index i = 0; i < iCount; i++)
      {
         iWidth = _001CalcItemWidth(pgraphics, i, 0);
         if (iWidth > iMaxWidth)
         {
            iMaxWidth = iWidth;
         }
      }

      return iMaxWidth;

   }


   void list::_001MaximizeColumnWidth(index iColumn)
   {
      _001SetColumnWidth(iColumn, _001CalcColumnWidth(iColumn));
   }


   void list::_OnDraw(::draw2d::graphics * pgraphics)
   {

      UNREFERENCED_PARAMETER(pgraphics);

   }


   int32_t list::get_wheel_scroll_delta()
   {
      return 3 * m_iItemHeight;
   }


   void list::on_change_viewport_offset()
   {

      synch_lock sl(m_pmutex);

      point pt = get_viewport_offset();

      m_iTopIndex = _001CalcDisplayTopIndex();

      index iLow = 0;

      for (m_iTopGroup = 0; m_iTopGroup < m_nGroupCount; m_iTopGroup++)
      {

         if (m_iTopIndex >= iLow && m_iTopIndex < (iLow + _001GetGroupItemCount(m_iTopGroup)))
         {

            break;

         }

      }

      m_nDisplayCount = _001CalcDisplayItemCount();


      if (m_iTopIndex < 0 && m_iItemHeight > 0 && m_eview == view_report && pt.y != 0 && m_nDisplayCount > 0)
      {

         rect rectScroll;

         scroll_x::m_pscrollbarHorz->GetWindowRect(rectScroll);

         if (pt.y > (_001GetItemCount() - m_nDisplayCount) * m_iItemHeight + (m_bHeaderCtrl ? m_iItemHeight : 0))
         {

            pt.y = (_001GetItemCount() - m_nDisplayCount) * m_iItemHeight + (m_bHeaderCtrl ? m_iItemHeight : 0);

         }

         if (pt.y < 0)
         {

            pt.y = 0;

         }

         m_ptScrollPassword1.y = pt.y;

         m_iTopIndex = _001CalcDisplayTopIndex();

      }

      HeaderCtrlLayout();

      CacheHint();

      UpdateHover();

      RedrawWindow();

   }



   //::draw2d::pen * list::_001GetPenHighlight()
   //{
   //   return m_penHighlight;
   //}

   //::draw2d::pen * list::_001GetPenFocused()
   //{
   //   return m_penFocused;

   //}

   //::draw2d::font * list::_001GetFont()
   //{
   //   return m_font;
   //}

   //::draw2d::font * list::_001GetFontHover()
   //{
   //   return m_fontHover;
   //}

   void list::_001OnMouseLeave(signal_details * pobj)
   {
      m_iItemHover = -1;
      m_iSubItemHover = -1;
      RedrawWindow();
      pobj->m_bRet = true;
   }



   void list::UpdateHover()
   {
      index iItemHover;
      index iSubItemHover;
      point point;
      Session.get_cursor_pos(&point);
      ScreenToClient(&point);

      if (_001DisplayHitTest(point, iItemHover, iSubItemHover))
      {
         if (m_iSubItemHover != iSubItemHover ||
            m_iItemHover != iItemHover)
         {
            m_iItemHover = iItemHover;
            m_iSubItemHover = iSubItemHover;
            RedrawWindow();
         }
      }

   }


   void list::data_get_DisplayToStrict()
   {

      if (m_eview == view_icon)
      {

         data_get(data_get_current_sort_id() + "." + data_get_current_list_layout_id(), m_iconlayout);

      }
      else
      {

         data_get(data_get_current_sort_id() + "." + data_get_current_list_layout_id(), m_meshlayout);

      }

   }


   void list::data_set_DisplayToStrict()
   {
      if (m_eview == view_icon)
      {
         data_set(data_get_current_sort_id() + "." + data_get_current_list_layout_id(), m_iconlayout);
      }
      else
      {
         data_set(data_get_current_sort_id() + "." + data_get_current_list_layout_id(), m_meshlayout);
      }
   }

   id list::data_get_current_sort_id()
   {
      return data_get_sort_id(m_eview);
   }

   id list::data_get_sort_id(EView eview)
   {

      return ::user::mesh::data_get_sort_id(eview);

      //UNREFERENCED_PARAMETER(eview);
      //switch(m_eview)
      //{
      //case view_grid:
      //   return "sort-grid";
      //   break;
      //case view_report:
      //   return "sort-report";
      //   break;
      //case view_list:
      //   return "sort-list";
      //   break;
      //case view_icon:
      //   return "sort-icon";
      //   break;
      //default:
      //   ASSERT(FALSE);
      //   return "sort";
      //}
   }

   list::EView list::_001GetView()
   {
      return m_eview;
   }


   void list::_001OnAfterSort()
   {
      data_set_DisplayToStrict();
   }

   bool list::_001IsEditing()
   {
      return false;
   }

   //void list::_001GetItemColor(::user::list_item * pitem)
   //{
   //   pitem->m_bOk = false;
   //}


   void list::_001OnVScroll(signal_details * pobj)
   {
      //      SCAST_PTR(::message::scroll, pscroll, pobj);

      pobj->previous();

   }

   void list::_001OnHScroll(signal_details * pobj)
   {
      //      SCAST_PTR(::message::scroll, pscroll, pobj);

      pobj->previous();

      m_iTopIndex = _001CalcDisplayTopIndex();
      index iLow = 0;
      for (m_iTopGroup = 0; m_iTopGroup < m_nGroupCount; m_iTopGroup++)
      {
         if (m_iTopIndex >= iLow && m_iTopIndex < (iLow + _001GetGroupItemCount(m_iTopGroup)))
            break;
      }
      m_nDisplayCount = _001CalcDisplayItemCount();

      HeaderCtrlLayout();

      CacheHint();

   }


   id list::data_get_current_list_layout_id()
   {

      return "list." + m_dataid.m_id.str();

   }


   bool list::query_drop(index iDisplayDrop, index iDisplayDrag)
   {
      if (iDisplayDrag < 0)
         return false;
      if (iDisplayDrop < 0)
         return false;
      if (m_iItemDrag != m_iItemDrop)
      {
         if (m_eview == view_icon)
         {
            return m_iconlayout.m_iaDisplayToStrict[m_iItemDrag] != -1;

            if (m_iconlayout.m_iaDisplayToStrict[m_iItemDrop] == -1 || m_iconlayout.m_iaDisplayToStrict[m_iItemDrop] >= m_nItemCount)
            {
               return true;
            }
         }
         else
         {
            if (m_meshlayout.m_iaDisplayToStrict[m_iItemDrop] == -1 || m_meshlayout.m_iaDisplayToStrict[m_iItemDrop] >= m_nItemCount)
            {
               return true;
            }
         }
      }
      return false;
   }


   bool list::do_drop(index iDisplayDrop, index iDisplayDrag)
   {
      UNREFERENCED_PARAMETER(iDisplayDrop);
      UNREFERENCED_PARAMETER(iDisplayDrag);
      if (m_eview == view_icon)
      {
         m_iconlayout.m_iaDisplayToStrict.swap(m_iItemDrag, m_iItemDrop);
      }
      else
      {
         index i = m_meshlayout.m_iaDisplayToStrict[m_iItemDrag];
         m_meshlayout.m_iaDisplayToStrict[m_iItemDrag] = m_meshlayout.m_iaDisplayToStrict[m_iItemDrop];
         m_meshlayout.m_iaDisplayToStrict[m_iItemDrop] = i;
      }
      _001OnAfterSort();
      return true;
   }

   bool list::defer_drop(index iDisplayDrop, index iDisplayDrag)
   {
      if (query_drop(iDisplayDrop, iDisplayDrag))
      {
         if (m_eview == view_icon)
         {
            return do_drop(iDisplayDrop, iDisplayDrag);
         }
         else
         {
            return do_drop(iDisplayDrop, iDisplayDrag);
         }
      }
      else
      {
         return false;
      }
   }

   //list::list_layout::list_layout()
   //{
   //}

   //list::list_layout::~list_layout()
   //{
   //}

   //void list::list_layout::write(::file::ostream & ostream) const
   //{
   //   //ostream << m_iaDisplayToStrict;
   //   ostream << m_iWidth;
   //}

   //void list::list_layout::read(::file::istream & istream)
   //{
   //   //istream >> m_iaDisplayToStrict;
   //   istream >> m_iWidth;
   //}

   //list::icon_layout::icon_layout()
   //{
   //}

   //list::icon_layout::~icon_layout()
   //{
   //}

   //void list::icon_layout::write(::file::ostream & ostream) const
   //{
   //   ostream << m_iaDisplayToStrict;
   //   ostream << m_iWidth;
   //}

   //void list::icon_layout::read(::file::istream & istream)
   //{
   //   istream >> m_iaDisplayToStrict;
   //   istream >> m_iWidth;
   //}

   class size list::get_item_size()
   {
      if (m_eview == view_icon)
      {
         if (m_columna.get_count() == 0)
         {
            return size(32, 32);
         }
         index iIconSize = MAX(32, m_columna[0]->m_sizeIcon.cy);
         index iItemSize = iIconSize * 2;
         return size(iItemSize, iItemSize);
      }
      else
      {
         // not implemented
         ASSERT(FALSE);
         return size(0, 0);
      }
   }

   void list::auto_arrange(bool bAutoArrange)
   {
      if (bAutoArrange)
      {
         m_flags.signalize(flag_auto_arrange);
      }
      else
      {
         m_flags.unsignalize(flag_auto_arrange);
      }
      if (bAutoArrange)
      {
         index iMaxStrict = m_iconlayout.m_iaDisplayToStrict.get_max_b();
         for (index iStrict = 0; iStrict <= iMaxStrict; iStrict++)
         {
            m_iconlayout.m_iaDisplayToStrict.set(iStrict, iStrict);
         }

         RedrawWindow();

      }

   }


   bool list::get_auto_arrange()
   {
      return m_flags.is_signalized(flag_auto_arrange);
   }

   void list::_001OnListViewAutoArrange(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      auto_arrange(!get_auto_arrange());
   }

   void list::_001OnUpdateListViewAutoArrange(signal_details * pobj)
   {
      SCAST_PTR(::aura::cmd_ui, pcmdui, pobj);
      pcmdui->m_pcmdui->_001SetCheck(get_auto_arrange());
      pcmdui->m_pcmdui->Enable();
   }

   bool list::is_valid_display_item(index iDisplayItem)
   {
      if (iDisplayItem < 0)
         return false;
      if (iDisplayItem >= m_nItemCount)
         return false;
      return true;
   }

   bool list::is_valid_strict_item(index iStrictItem)
   {
      if (iStrictItem < 0)
         return false;
      if (iStrictItem >= m_nItemCount)
         return false;
      return true;

   }


   bool list::keyboard_focus_is_focusable()
   {

      return true;

   }


   void list::_001OnSelectionChange()
   {

      RedrawWindow();

   }


   index list::set_cur_sel(index iSel)
   {
      index iOld = get_cur_sel();
      m_rangeSelection.clear();
      if (iSel >= 0)
      {
         item_range itemrange;
         itemrange.set_lower_bound(iSel);
         itemrange.set_upper_bound(iSel);
         m_rangeSelection.add_item(itemrange);
      }
      return iOld;
   }

   void list::set_cur_sel(const index_array & iaSel)
   {
      m_rangeSelection.clear();
      for (index i = 0; i < iaSel.get_count(); i++)
      {
         item_range itemrange;
         itemrange.set_lower_bound(iaSel[i]);
         itemrange.set_upper_bound(iaSel[i]);
         m_rangeSelection.add_item(itemrange);
      }

   }

   index list::get_cur_sel()
   {
      if (m_rangeSelection.get_item_count() != 1)
         return -1;
      if (m_rangeSelection.ItemAt(0).get_lower_bound() == m_rangeSelection.ItemAt(0).get_upper_bound() && m_rangeSelection.ItemAt(0).get_lower_bound() >= 0)
         return m_rangeSelection.ItemAt(0).get_lower_bound();
      return -1;
   }


   void list::get_cur_sel(index_array & iaSel)
   {

      for (index i = 0; i < m_rangeSelection.get_item_count(); i++)
      {

         for (index iItem = m_rangeSelection.ItemAt(i).get_lower_bound(); iItem <= m_rangeSelection.ItemAt(i).get_upper_bound(); iItem++)
         {

            iaSel.add_unique(iItem);

         }

      }

   }


   ::count list::_001GetGroupItemCount(index iGroup)
   {
      UNREFERENCED_PARAMETER(iGroup);
      return -1;
   }

   int32_t list::_001GetGroupHeight(index iGroup)
   {
      int32_t iListHeight = (int32_t)(_001GetGroupItemCount(iGroup) * m_iItemHeight);
      return MAX(m_iGroupMinHeight, iListHeight);
   }

   list_item::list_item(list * plist) :
      ::object(plist->get_app()),
      mesh_item(plist)
   {
      m_plistitem = this;
      m_plist = plist;
      m_iGroup = -1;
      m_iItem = -1;
      m_iDisplayItem = -1;
      m_iColumn = -1;
      //m_iColumnKey = -1;
      m_iOrder = -1;
      m_iSubItem = -1;
      m_iListItem = -1;
      m_crText = (COLORREF)-1;
      m_crTextBackground = ARGB(255, 0, 0, 0);
      m_crItemBackground = 0;
      m_iState = -1;
      m_iImage = -1;
      m_bOk = false;

      m_iGroupTopIndex = -1;
      m_iGroupCount = -1;

   }

   draw_list_item::draw_list_item(list * plist) :
      ::object(plist->get_app()),
      list_item(plist),
      mesh_item(plist),
      draw_mesh_item(plist)
   {

      m_prectClient = NULL;

      m_iColumn = -1;
      m_iColumnWidth = 0;
      m_pcolumn = NULL;

      m_iItemRectItem = -1;

      m_iSubItemRectItem = -1;
      m_iSubItemRectSubItem = -1;
      m_iSubItemRectOrder = -1;
      m_iSubItemRectColumn = -1;
      m_pcolumn = NULL;

      m_iListItemRectItem = -1;
      m_iListItemRectSubItem = -1;
      m_iListItemRectListItem = -1;

   }


   sp(image_list) draw_list_item::get_image_list()
   {
      if (m_bListItemHover && m_pcolumn->m_pilHover != NULL)
      {
         return m_pcolumn->m_pilHover;
      }
      else
      {
         return m_pcolumn->m_pil;
      }
   }

   bool draw_list_item::draw_image()
   {
      if (m_pcolumn->m_bIcon)
      {
         visual::icon * picon;
         if (m_pcolumn->m_mapIcon.Lookup((int32_t)m_iImage, picon))
         {
            m_pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);
            return m_pgraphics->DrawIcon(m_rectImage.top_left(), picon) != FALSE;
         }
      }
      else
      {
         synch_lock sl(get_image_list()->m_pmutex);

         if (m_plist->m_iIconBlur > 0 && m_plist->m_iIconBlurRadius > 0 || m_plist->m_dIconSaturation < 1.0)
         {

            auto & dib = m_plist->m_mapIconBlur[m_iImage];

            if (dib.is_null())
            {

               dib.alloc(allocer());

            }

            int iRate = 3;

            if (dib->area() <= 0)
            {

               if (m_plist->m_iIconBlur > 0 && m_plist->m_iIconBlurRadius > 0)
               {

                  dib.initialize(m_rectImage.size() + size(m_plist->m_iIconBlurRadius * iRate * 2, m_plist->m_iIconBlurRadius * iRate * 2), m_plist->m_iIconBlurRadius);

               }
               else
               {

                  dib->create(m_rectImage.size() + size(m_plist->m_iIconBlurRadius * iRate * 2, m_plist->m_iIconBlurRadius * iRate * 2));

               }

               dib->get_graphics()->set_alpha_mode(::draw2d::alpha_mode_set);

               dib->get_graphics()->FillSolidRect(0, 0, dib->m_size.cx, dib->m_size.cy, 0);

               get_image_list()->draw(dib->get_graphics(), (int32_t)m_iImage,
                  point(m_plist->m_iIconBlurRadius*iRate, m_plist->m_iIconBlurRadius *iRate), m_rectImage.size(), point(0, 0), 0);


               if (m_plist->m_iIconBlur > 0 && m_plist->m_iIconBlurRadius > 0)
               {

                  for (index i = 0; i < m_plist->m_iIconBlur; i++)
                  {

                     dib.blur();

                  }

               }

               if (m_plist->m_dIconSaturation < 1.0)
               {

                  dib->saturation(m_plist->m_dIconSaturation);

               }

            }

            m_pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

            rect rDib(m_rectImage.top_left() - size(m_plist->m_iIconBlurRadius *iRate, m_plist->m_iIconBlurRadius * iRate),
               m_rectImage.size() + size(m_plist->m_iIconBlurRadius *iRate * 2, m_plist->m_iIconBlurRadius * iRate * 2));

            m_pgraphics->BitBlt(rDib, dib->get_graphics());

            rect rI;

            if (m_plist->m_dibSpot.is_set() && m_plist->m_dibSpot->area() > 0 && rI.intersect(m_rectImage, m_plist->m_rectSpot))
            {

               if (m_plist->m_dibTime.is_null())
               {

                  m_plist->m_dibTime.alloc(allocer());

               }

               m_plist->m_dibTime->create(m_plist->m_dibSpot->size());

               rect r = rI;

               r.offset(-m_rectImage.top_left());

               //m_plist->m_dibTime->get_graphics()->set_alpha_mode(::draw2d::alpha_mode_set);

               //m_plist->m_dibTime->get_graphics()->FillSolidRect(r, 0);

               rect r2 = rI;

               r2.offset(-m_plist->m_rectSpot.top_left());

               m_plist->m_dibTime->get_graphics()->set_alpha_mode(::draw2d::alpha_mode_set);

               get_image_list()->draw(m_plist->m_dibTime->get_graphics(), (int32_t)m_iImage,
                  r2.top_left(),
                  r.size(), r.top_left(), 0);
               
               //m_plist->m_dibTime->div_alpha(r2.top_left(), r2.size());

               m_plist->m_dibTime->channel_multiply(visual::rgba::channel_alpha, m_plist->m_dibSpot, r2);

               //m_plist->m_dibTime->mult_alpha(r2.top_left(), r2.size());
               
               m_pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

               m_pgraphics->BitBlt(rI, m_plist->m_dibTime->get_graphics(), r2.top_left());

            }

            return true;

         }
         //if(m_plist->m_bMorePlain)
         //{

         //   m_pgraphics->set_alpha_mode(::draw2d::alpha_mode_set);
         //   return m_pgraphics->BitBlt(m_rectImage.left, m_rectImage.top, m_rectImage.width(), m_rectImage.height(),get_image_list()->m_spdib->get_graphics(), m_iImage * m_rectImage.width());

         //}
         else
         {
            m_pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);
            return get_image_list()->draw(m_pgraphics, (int32_t)m_iImage, m_rectImage.top_left(), m_rectImage.size(), point(0, 0), 0);
         }
      }
      return false;
   }


   bool draw_list_item::draw_group_image()
   {
      return m_plist->m_pilGroup->draw(m_pgraphics, (int32_t)m_iImage, m_rectImage.top_left(), m_rectImage.size(), point(0, 0), 0);
   }


   void draw_list_item::update_item_color()
   {

      m_plist->_001GetItemColor(this);

      if (!m_bOk)
      {
         if (m_bListItemSelected)
         {
            if (m_bListItemHover)
            {
               m_crText = m_plist->_001GetColor(::user::color_text_selected_highlight);
            }
            else
            {
               m_crText = m_plist->_001GetColor(::user::color_text_selected);
            }
         }
         else
         {
            if (m_bListItemHover)
            {
               m_crText = m_plist->_001GetColor(::user::color_text_highlight);
            }
            else
            {
               m_crText = m_plist->_001GetColor(::user::color_text);
            }
         }
      }
   }

   void draw_list_item::set_text_color()
   {
      ::draw2d::brush_sp brushText(allocer());
      brushText->create_solid(m_crText);
      m_pgraphics->SelectObject(brushText);
   }


   void draw_list_item::draw_text()
   {
      if (m_bOk)
      {
         if (m_plist->m_eview == list::view_icon && m_plist->m_bEmboss && ((m_plist->m_iTextBlurRadius > 0 && m_plist->m_iTextBlur > 0) || m_plist->m_iTextSpreadRadius > 0))
         {

            if (m_strText.has_char())
            {

               visual::fastblur & dib2 = m_plist->m_mapBlur[m_iItem];

               if (Sys(m_plist->get_app()).visual().embossed_text_out(
                  m_pgraphics,
                  m_rectText,
                  m_strText,
                  dib2,
                  m_pgraphics->m_spfont,
                  m_iDrawTextFlags,
                  m_crText,
                  m_crTextBackground,
                  m_plist->m_iTextSpreadRadius, m_plist->m_iTextBlurRadius,
                  m_plist->m_iTextBlur,
                  m_strText != m_plist->m_mapText[m_iItem] || m_crTextBackground != m_plist->m_mapBackColor[m_iItem]))
               {

                  m_plist->m_mapText[m_iItem] = m_strText;

                  m_plist->m_mapBackColor[m_iItem] = m_crTextBackground;

               }

            }

         }
         else
         {
            ::draw2d::brush_sp brushText(allocer());

            brushText->create_solid(m_crText);
            m_pgraphics->SelectObject(brushText);
            m_pgraphics->set_text_rendering(::draw2d::text_rendering_anti_alias);
            m_pgraphics->_DrawText(m_strText, m_rectText, m_iDrawTextFlags);
            if (m_strText == "LOVE OR NOTHING")
            {
               output_debug_string("LOVE OR NOTHING");
            }
         }
      }
   }

   ::count list::_001GetGroupMetaItemCount(index iGroup)
   {
      if (m_pmeshdata != NULL)
      {
         return m_pmeshdata->_001GetGroupMetaItemCount(iGroup);
      }
      return 0;
   }

   //void list::_001GetGroupText(list_item * pitem)
   //{
   //   pitem->m_bOk = false;
   //   if(m_pcache != NULL)
   //   {
   //      m_pcache->_001GetGroupText(pitem);
   //      if(pitem->m_bOk)
   //         return;
   //   }
   //   if(m_pmeshdata != NULL)
   //   {
   //      m_pmeshdata->_001GetGroupText(pitem);
   //   }
   //}

   //void list::_001GetGroupImage(list_item * pitem)
   //{
   //   pitem->m_bOk = false;
   //   pitem->m_iImage = -1;
   //}




   void list::_001OnBeforeDeleteRange(range & range)
   {
   }

   void list::_001OnDeleteRange(range & range)
   {

      if (m_pmeshdata != NULL)
      {

         m_pmeshdata->_001OnDeleteRange(range);

      }

   }


   void list::_001DeleteRange(range & range)
   {

      _001OnBeforeDeleteRange(range);

      _001OnDeleteRange(range);

   }


   void list::defer_create_mesh_data()
   {

      sp(::user::list_data) plistdata = create_mesh_data();

      if (plistdata.is_null())
         return;

      SetDataInterface(plistdata);

   }


   void list::on_create_draw_item()
   {

      m_pdrawlistitem = new draw_list_item(this);

      m_pdrawmeshitem = m_pdrawlistitem;


   }


   void list::on_viewport_offset(::draw2d::graphics * pgraphics)
   {

   }


   void list::_001OnClip(::draw2d::graphics * pgraphics)
   {

      mesh::_001OnClip(pgraphics);

      if (m_bHeaderCtrl && m_plistheader != NULL)
      {

         try
         {

            rect rectClient;

            GetClientRect(rectClient);

            rect rectClientHeader;

            m_plistheader->GetClientRect(rectClientHeader);

            m_plistheader->ClientToScreen(rectClientHeader);

            ScreenToClient(rectClientHeader);

            rectClient.top = rectClientHeader.bottom;

            pgraphics->IntersectClipRect(rectClient);

         }
         catch (...)
         {

            TRACE("Exception : ::list::_001OnClip");

         }

      }

   }


   int32_t list::_001GetDrawTextFlags(EView eview)
   {

      return m_pdrawmeshitem->m_iDrawTextFlags;

   }


   bool list::GetClientRect(LPRECT lprect)
   {

      mesh::GetClientRect(lprect);

      if (m_bHeaderCtrl && m_plistheader != NULL)
      {

         rect rectClient;

         m_plistheader->GetClientRect(rectClient);

         m_plistheader->ClientToScreen(rectClient);

         ScreenToClient(rectClient);

         lprect->top += rectClient.bottom;

      }

      return true;

   }




   void list::update_icon_list_view_sort()
   {

      if (m_eview != view_icon)
         return;


      synch_lock sl(m_pmutex);

      string strSort = oprop("list_sort");

      if (strSort.has_char())
      {

         defer_update_display();

         strSort += "-" + m_strDisplay + ".icon_list_view_sort";

         string str = Application.file().as_string(strSort);
         stringa stra;
         stra.add_lines(str);
         for (index a = 0; a < stra.get_size(); a++)
         {
            index b = _001GetItemById(stra[a]);
            if (b >= 0 && b < m_nItemCount)
            {
               m_iconlayout.m_iaDisplayToStrict.set(a, b);
            }
         }

      }


      for (index b = 0; b < m_nItemCount; b++)
      {

         if (m_iconlayout.m_iaDisplayToStrict.get_a(b) == -1)
         {

            m_iconlayout.m_iaDisplayToStrict.set(m_iconlayout.m_iaDisplayToStrict.get_free_a(), b);

         }

      }

   }


} // namespace user



