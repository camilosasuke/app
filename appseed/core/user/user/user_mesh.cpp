#include "framework.h"


CLASS_DECL_AURA index array_translate_a(index_biunique & ia, index aNew, index aOld);
CLASS_DECL_AURA index_array array_translate_a(index_biunique & ia, index_array iaNew, index_array iaOld);


namespace user
{

   UINT c_cdecl data_update_mesh_visible_subitem_proc(LPVOID pparam);



   const UINT mesh::MESSAGE_ENDCOLUMNHEADERDRAG = WM_USER + 25;
   const UINT mesh::MESSAGE_COLUMNHEADERTRACK = WM_USER + 26;
   const UINT mesh::MESSAGE_ENDCOLUMNHEADERTRACK = WM_USER + 27;

   mesh::mesh():
      m_penFocused(allocer()),
      m_penHighlight(allocer()),
      m_dcextension(get_app())
   {

      m_iItemHeight = 32;
      m_plist = NULL;
      m_piaFilterIcon = new index_biunique();
      m_piaFilterMesh = new index_array();
      m_iconlayout.m_iaDisplayToStrict.set_empty_a(-1);
      m_iconlayout.m_iaDisplayToStrict.set_empty_b(-1);

      m_bDrag = false;
      m_eview = view_grid;
      m_iItemFocus = -1;
      m_iItemHover = -1;
      m_iSubItemHover = -2;
      m_bSortEnable              = true;
      m_bFilter1                 = false;
      m_nColumnCount             = 1;

      m_pcache                   = NULL;
      m_bTopText                 = false;

      m_bEmboss                  = true;
#if defined(VSNORD)
      m_bHoverSelect = false;
#else
      m_bHoverSelect = true;
#endif
      m_bMultiSelect             = true;
      m_iLateralGroupWidth       = 200;

      m_pdrawmeshitem            = NULL;

      m_bGroup                   = false;
      m_bLateralGroup            = false;

      // Simple Filter
      m_efilterstate             = FilterStateNoFilter;

      m_iGroupMinHeight          = 0;
      m_nGroupCount              = 0;
      m_nItemCount               = 0;
      m_iTopIndex                = -1;
      m_nDisplayCount            = 0;

      m_iClick = 0;

      m_iDefaultColumnWidth      = 200;

      m_bLockViewUpdate = false;


      //m_iItemHeight = 0;
      m_iItemWidth = 0;

      m_iLastItemSel = 0;
      m_iLastSubItemSel = 0;
      m_iItemSel = 0;
      m_iSubItemSel = 0;
      m_iSubItemEnter = -1;
      m_iItemEnter = -1;

      //   m_iItemCount = 0;

      m_pilGroup        = NULL;
      m_pilGroupHover   = NULL;


      m_iLeftMargin                       = 0;
      m_iTopMargin                        = 0;

      m_bAutoCreateMeshHeader = true;
      m_bAutoCreateMeshData = true;

      m_nGridItemCount = 0;
      m_nGridColumnCount = 0;

      m_sizeTotal.cx = 0;
      m_sizeTotal.cy = 0;

   }

   mesh::~mesh()
   {
      ::aura::del(m_piaFilterIcon);
      ::aura::del(m_piaFilterMesh);
   }


   void mesh::install_message_handling(::message::dispatch * pinterface)
   {

      ::user::control::install_message_handling(pinterface);

      IGUI_WIN_MSG_LINK(WM_SIZE,pinterface,this,&mesh::_001OnSize);
      IGUI_WIN_MSG_LINK(WM_VSCROLL,pinterface,this,&mesh::_001OnVScroll);
      IGUI_WIN_MSG_LINK(WM_HSCROLL,pinterface,this,&mesh::_001OnHScroll);
      IGUI_WIN_MSG_LINK(WM_MOUSELEAVE,pinterface,this,&mesh::_001OnMouseLeave);

      IGUI_WIN_MSG_LINK(WM_LBUTTONDOWN,pinterface,this,&mesh::_001OnLButtonDown);
      IGUI_WIN_MSG_LINK(WM_LBUTTONUP,pinterface,this,&mesh::_001OnLButtonUp);
      IGUI_WIN_MSG_LINK(WM_LBUTTONDBLCLK,pinterface,this,&mesh::_001OnLButtonDblClk);
      IGUI_WIN_MSG_LINK(WM_RBUTTONDOWN,pinterface,this,&mesh::_001OnRButtonDown);
      IGUI_WIN_MSG_LINK(WM_RBUTTONDOWN, pinterface, this, &mesh::_001OnRButtonUp);

      IGUI_WIN_MSG_LINK(WM_MOUSEMOVE,pinterface,this,&mesh::_001OnMouseMove);

      IGUI_WIN_MSG_LINK(WM_KEYDOWN,pinterface,this,&mesh::_001OnKeyDown);

      IGUI_WIN_MSG_LINK(WM_CREATE,pinterface,this,&mesh::_001OnCreate);
      ////IGUI_WIN_MSG_LINK(WM_TIMER,pinterface,this,&mesh::_001OnTimer);
      connect_command("mesh_view_auto_arrange",&mesh::_001OnMeshViewAutoArrange);
      connect_update_cmd_ui("mesh_view_auto_arrange",&mesh::_001OnUpdateMeshViewAutoArrange);
   }


   void mesh::_001OnDraw(::draw2d::graphics * pgraphics)
   {

      

      //single_lock sl(&m_mutex,true);

      m_penFocused->create_solid(2,ARGB(255,0,255,255));

      m_penHighlight->create_solid(2,ARGB(255,0,255,255));

      pgraphics->set_text_rendering(::draw2d::text_rendering_anti_alias_grid_fit);

      if(m_bLockViewUpdate)
         return;

      ::user::control::_001OnDraw(pgraphics);

      draw_framing(pgraphics);

      rect rectClient;

      GetClientRect(rectClient);


      point ptScroll = get_viewport_offset();


      //      pgraphics->SetBkMode(TRANSPARENT);

      if(m_bTopText)
      {

         ::draw2d::brush_sp brushText(allocer());

         brushText->create_solid(_001GetColor(color_text));

         point ptViewportOrg = pgraphics->GetViewportOrg();
         pgraphics->SelectObject(brushText);
         array < size > sizea;
         m_dcextension.GetTextExtent(pgraphics,m_strTopText,sizea);
         index x = 0;
         index right = (index)rectClient.right;
         index y = m_iItemHeight;
         index iStart = 0;
         index iNewStart = 0;
         index w;
         for(index i = 0; i < sizea.get_size(); i++)
         {

            if((sizea[i].cx - x > right)
               || i == sizea.get_upper_bound())
            {
               rect rect;
               rect.top = LONG(y - ptScroll.y);
               if(i == 0)
               {
                  w = sizea[0].cx - x      ;
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
               rect.left = LONG(- ptScroll.x);
               rect.right = rectClient.right;
               rect.bottom = LONG(y - ptScroll.y);

               pgraphics->_DrawText(m_strTopText.Mid(iStart,i - iStart),rect,DT_LEFT);
               iStart = iNewStart;
            }
         }
         pgraphics->SetViewportOrg(ptViewportOrg);
      }


//      if(m_pmeshdata != NULL)
//      {
//         if(m_pmeshdata->is_locked())
//         {
//            return;
//         }
//      }




      m_pdrawmeshitem->m_pgraphics              = pgraphics;
      m_pdrawmeshitem->m_iItemRectItem          = -1;
      m_pdrawmeshitem->m_iSubItemRectOrder      = -1;
      m_pdrawmeshitem->m_iSubItemRectSubItem    = -1;
      m_pdrawmeshitem->m_iSubItemRectItem       = -1;
      m_pdrawmeshitem->m_iSubItemRectColumn     = -1;
//      m_pdrawmeshitem->m_iWidthColumn           = -1;
      m_pdrawmeshitem->m_iColumnWidth           = 0;

      rect rectItem;
      rect rectIntersect;
      int_ptr iItemCount = m_nDisplayCount;
      int_ptr iItemFirst = m_iTopIndex;
      int_ptr iItemLast;
      if(m_eview == view_icon)
      {
         iItemLast = iItemCount;
      }
      else
      {
         iItemLast = MIN(m_nItemCount - 1,iItemFirst + m_nDisplayCount - 1);
      }

      if(iItemFirst < 0)
         return;

      _001DrawItems(m_pdrawmeshitem,iItemFirst,iItemLast);

      if(m_bGroup && m_bLateralGroup)
      {

         int32_t iGroupFirst = 0;
         int32_t iGroupFirstTopIndex = 0;
         for(; iGroupFirst < m_nGroupCount; iGroupFirst++)
         {
            if(iItemFirst >= iGroupFirstTopIndex && iItemFirst < (iGroupFirstTopIndex + _001GetGroupItemCount(iGroupFirst)))
               break;
         }
         int32_t iGroupLast = iGroupFirst;
         int32_t iGroupLastTopIndex = iGroupFirstTopIndex;
         for(; iGroupLast < m_nGroupCount; iGroupLast++)
         {
            if(iItemLast >= iGroupLastTopIndex && iItemLast < (iGroupLastTopIndex + _001GetGroupItemCount(iGroupLast)))
               break;
         }

         _001DrawGroups(m_pdrawmeshitem,iGroupFirst,iGroupLast,iItemFirst,iItemLast);
      }


   }



















   void mesh::_001DrawGroups(draw_mesh_item * pdrawitem,index iGroupFirst,index iGroupLast,index iItemFirst,index iItemLast)
   {

      UNREFERENCED_PARAMETER(iItemFirst);
      UNREFERENCED_PARAMETER(iItemLast);

      index iGroup;

      rect rectClient;

      rect rectIntersect;

      GetClientRect(rectClient);

      bool bHoverFont = false;
      
      select_font(pdrawitem->m_pgraphics, font_list_item, this);

      m_pdrawmeshitem->m_pfont = pdrawitem->m_pgraphics->m_spfont;

      for(iGroup = iGroupFirst; iGroup <= iGroupLast; iGroup++)
      {

         m_pdrawmeshitem->m_iGroup          = iGroup;

         _001GetGroupRect(m_pdrawmeshitem);

         if(!m_pdrawmeshitem->m_bOk)
            continue;

         if(!rectIntersect.intersect(m_pdrawmeshitem->m_rectItem,rectClient))
            continue;

         if(iGroup == m_iGroupHover)
         {
            if(!bHoverFont)
            {
               select_font(pdrawitem->m_pgraphics, font_list_hover, this);
            }
         }
         else
         {
            if(bHoverFont)
            {
               select_font(pdrawitem->m_pgraphics, font_list_item, this);
            }
         }

         _001DrawGroup(m_pdrawmeshitem);

      }

   }



















   void mesh::_001DrawGroup(draw_mesh_item * pdrawitem)
   {

      /*if(m_bGroupCustomDraw)
      {
      pdrawitem->m_bMeshSubItemHover = pdrawitem->m_bListItemHover && (pdrawitem->m_iSubItem == m_iSubItemHover);
      pdrawitem->m_bFocus = false;
      return;
      }*/

      _001GetElementRect(pdrawitem,::user::mesh::element_group_image);
      if(pdrawitem->m_bOk)
      {
         pdrawitem->draw_group_image();
      }

      ::count nItem = _001GetGroupMetaItemCount(pdrawitem->m_iGroup);

      for(pdrawitem->m_iItem = 0; pdrawitem->m_iItem < nItem; pdrawitem->m_iItem++)
      {
         _001GetElementRect(pdrawitem,::user::mesh::element_group_item_text);
         if(pdrawitem->m_bOk)
         {
            _001GetGroupText(pdrawitem);
            pdrawitem->draw_text();
         }
      }



   }
















   void mesh::_001DrawItems(draw_mesh_item * pdrawitem,index iItemFirst,index iItemLast)
   {

      rect rectClient;

      GetClientRect(rectClient);

      rect rectVisible(rectClient);

      //rectVisible.deflate(2,2);

      rect rectIntersect;

      index iItem;

      if(m_eview == view_icon)
      {
         m_pdrawmeshitem->m_iDrawTextFlags = DT_TOP | DT_CENTER | DT_END_ELLIPSIS | DT_WORDBREAK;
      }
      else
      {
         m_pdrawmeshitem->m_iDrawTextFlags = DT_VCENTER | DT_LEFT | DT_END_ELLIPSIS | DT_SINGLELINE;
      }

      bool bHoverFont = false;
      
      select_font(pdrawitem->m_pgraphics, font_list_item, this);

      pdrawitem->m_pfont = pdrawitem->m_pgraphics->m_spfont;

      for(iItem = iItemFirst; iItem <= iItemLast; iItem++)
      {
         if(m_eview == view_icon
            && (m_iconlayout.m_iaDisplayToStrict.get_b(iItem) == -1
            && iItem != m_iItemDrop))
            continue;

         m_pdrawmeshitem->m_iItem           = DisplayToStrict(iItem);
         m_pdrawmeshitem->m_iDisplayItem    = iItem;

         if(m_bGroup)
         {
            m_pdrawmeshitem->m_iGroupTopIndex = 0;
            //            int32_t igroup;
            for(m_pdrawmeshitem->m_iGroup = 0; m_pdrawmeshitem->m_iGroup < m_nGroupCount; m_pdrawmeshitem->m_iGroup++)
            {
               m_pdrawmeshitem->m_iGroupCount = _001GetGroupItemCount(m_pdrawmeshitem->m_iGroup);
               if(iItem >= m_pdrawmeshitem->m_iGroupTopIndex && iItem < (m_pdrawmeshitem->m_iGroupTopIndex + m_pdrawmeshitem->m_iGroupCount))
                  break;
            }
         }

         _001GetItemRect(m_pdrawmeshitem);

         if(!m_pdrawmeshitem->m_bOk)
            continue;

         if(!rectIntersect.intersect(m_pdrawmeshitem->m_rectItem,rectVisible))
            continue;

         if(iItem == m_iItemHover)
         {
            if(!bHoverFont)
            {
               bHoverFont = true;
               select_font(pdrawitem->m_pgraphics, font_list_hover, this);
            }
         }
         else
         {
            if(bHoverFont)
            {
               bHoverFont = false;
               select_font(pdrawitem->m_pgraphics, font_list_item, this);
            }
         }

         _001DrawItem(m_pdrawmeshitem);

      }
   }



   void mesh::_001DrawItem(draw_mesh_item * pdrawitem)
   {

      pdrawitem->m_iState = 0;

      range & rangeSelection = m_rangeSelection;
      range & rangeHighlight = m_rangeHighlight;

      rect rectClient;

      GetClientRect(rectClient);

      if(pdrawitem->m_iItem < 0)
         return;

      pdrawitem->m_bListItemHover = pdrawitem->m_iDisplayItem == m_iItemHover &&
         (m_eview != view_icon ||
         ((m_iconlayout.m_iaDisplayToStrict.get_a(m_iItemHover) >= 0 && m_iconlayout.m_iaDisplayToStrict.get_a(m_iItemHover) < m_nItemCount)));

      ::draw2d::font * pfont;
      if(pdrawitem->m_bListItemHover)
      {
         pdrawitem->m_pgraphics->FillSolidRect(pdrawitem->m_rectItem,ARGB(128,255,255,255));
         select_font(pdrawitem->m_pgraphics, font_list_hover, this);
      }
      else
      {
         select_font(pdrawitem->m_pgraphics, font_list_item, this);
      }
//      pdrawitem->m_pgraphics->set_font(pfont);

      pdrawitem->m_bListItemSelected = (m_eview != view_icon || is_valid_display_item(pdrawitem->m_iItem)) && rangeSelection.has_item(pdrawitem->m_iDisplayItem);

      if(pdrawitem->m_bListItemHover)
         pdrawitem->m_iState |= ItemStateHover;
      if(pdrawitem->m_bListItemSelected)
         pdrawitem->m_iState |= ItemStateSelected;




      if(pdrawitem->m_bListItemSelected)
      {
         if(Session.savings().is_trying_to_save(::aura::resource_processing))
         {
            pdrawitem->m_pgraphics->FillSolidRect(pdrawitem->m_rectItem,ARGB(255,96,96,96));
         }
         else
         {
            COLORREF crTranslucid = RGB(0,0,0);
            ::rect r = pdrawitem->m_rectItem;
            r.inflate(8,0,8,-1);
            System.visual().imaging().color_blend(pdrawitem->m_pgraphics,r,crTranslucid,127);
         }
      }

      pdrawitem->update_item_color();
      pdrawitem->set_text_color();

      string str;

      index iColumnCount;

      if(m_eview == view_icon)
      {
         iColumnCount = 1;
      }
      else
      {
         iColumnCount = m_nColumnCount;
      }

      pdrawitem->m_iSubItemRectOrder      = -1;
      pdrawitem->m_iSubItemRectSubItem    = -1;
      pdrawitem->m_iSubItemRectColumn     = -1;

      ::rect rectIntersect;

      if(m_eview == view_grid)
      {

         pdrawitem->m_iOrder = MAX(get_viewport_offset().x, 0);

      }
      else
      {

         pdrawitem->m_iOrder = 0;

      }

      for(; pdrawitem->m_iOrder < iColumnCount; pdrawitem->m_iOrder++)
      {


         pdrawitem->m_iColumn    = _001MapOrderToColumn(pdrawitem->m_iOrder);


         if(pdrawitem->m_iColumn < 0)
            continue;


         _001GetSubItemRect(pdrawitem);

         if(!rectIntersect.intersect(m_pdrawmeshitem->m_rectSubItem,rectClient))
            break;

         if(!pdrawitem->m_bOk)
            continue;


         _001DrawSubItem(pdrawitem);

      }
      if(rangeHighlight.has_item(pdrawitem->m_iDisplayItem))
      {
         ::draw2d::pen_sp penHighlight(allocer());
         ::draw2d::pen * ppenHighlight = _001GetPenHighlight();
         class rect rectHighlight(pdrawitem->m_rectItem);
         rectHighlight.inflate(8,0,8,-1);
         pdrawitem->m_pgraphics->SelectObject(ppenHighlight);
         pdrawitem->m_pgraphics->DrawRectangle(rectHighlight);
      }



   }

   void mesh::draw_framing(::draw2d::graphics * pgraphics)
   {

   }


   void   mesh::_001DrawSubItem(draw_mesh_item * pdrawitem)
   {


      pdrawitem->m_iListItem = -1;
      _001GetElementRect(pdrawitem,::user::mesh::element_image);
      if(pdrawitem->m_bOk)
      {
         try
         {
            _001GetItemImage(pdrawitem);
         }
         catch(...)
         {
            pdrawitem->m_bOk = false;
         }
         pdrawitem->draw_image();
      }
      pdrawitem->m_iListItem = -1;
      _001GetElementRect(pdrawitem,::user::mesh::element_text);
      if(pdrawitem->m_bOk)
      {
         try
         {
            _001GetItemText(pdrawitem);
         }
         catch(...)
         {
            pdrawitem->m_bOk = false;
         }
         if(pdrawitem->m_bOk)
         {
            pdrawitem->draw_text();
         }
      }

   }




   void mesh::_001GetItemImage(::user::mesh_item * pitem)
   {
      
      if(m_pmeshdata != NULL)
      {

         pitem->m_pmesh = this;
         try
         {
            return m_pmeshdata->_001GetItemImage(pitem);
         }
         catch(...)
         {
            TRACE("_001GetItemImage exception");
         }
      }
      /*LVITEM lvi;
      lvi.mask = LVIF_IMAGE;
      lvi.iItem = iItem;
      lvi.iSubItem = 0;
      get_item(&lvi);
      return lvi.iImage;*/
      pitem->m_bOk = false;
   }

   void mesh::_001SetItemText(::user::mesh_item * pitem)
   {

      pitem->m_bOk = false;

   }

   void mesh::_001GetItemText(::user::mesh_item * pitem)
   {
      pitem->m_bOk = false;
      if(m_pcache != NULL)
      {
         m_pcache->_001GetItemText(pitem);
         if(pitem->m_bOk)
            return;
      }
      if(m_pmeshdata != NULL)
      {
         m_pmeshdata->_001GetItemText(pitem);
      }
   }

   // Purpose:
   // Provide Item Text for Search Purposes.
   void mesh::_001SearchGetItemText(::user::mesh_item * pitem)
   {
      _001GetItemText(pitem);
   }




   /*bool mesh::pre_create_window(::user::create_struct& cs)
   {

   //cs.style |= LVS_NOSCROLL;

   return window::pre_create_window(cs);
   }*/

   /*void mesh::OnSize(UINT nType, index cx, index cy)
   {
   window::OnSize(nType, cx, cy);

   on_layout();
   }*/

   void mesh::_001OnSize(signal_details * pobj)
   {
      SCAST_PTR(::message::size,psize,pobj);
      //on_layout();
      //psize->m_bRet = false;
   }

   void mesh::on_layout()
   {

      synch_lock sl(m_pmutex);

      if(m_bTopText)
         _001LayoutTopText();

      ::count iCount = m_nItemCount;
      int32_t iMaxWidth = 0;
      //int32_t iWidth;

      m_iTopIndex       = _001CalcDisplayTopIndex();

      if(m_eview == view_grid)
      {

         m_iItemWidth      = m_nColumnCount * m_iDefaultColumnWidth;

      }

      index iLow = 0;
      for(m_iTopGroup = 0; m_iTopGroup < m_nGroupCount; m_iTopGroup++)
      {

         if(m_iTopIndex >= iLow && m_iTopIndex < (iLow + _001GetGroupItemCount(m_iTopGroup)))
            break;

      }

      m_nDisplayCount   = _001CalcDisplayItemCount();

      on_change_view_size();


      //if (m_eview == view_icon)
      //{
      //   //            if(m_iconlayout.m_iWidth <= 0)
      //   {
      //      rect rectClient;
      //      GetClientRect(rectClient);
      //      index iIconSize;
      //      if (m_nColumnCount > 0)
      //         //                  iIconSize = MAX(32,m_columna[0]->m_sizeIcon.cy);
      //         iIconSize = 32;
      //      else
      //         iIconSize = 32;
      //      index iItemSize = iIconSize * 2;
      //      m_iconlayout.m_iWidth = (int32_t)(MAX(1, rectClient.width() / iItemSize));
      //   }
      //}


   }



   bool mesh::_001OnUpdateColumnCount(uint32_t dwFlags)
   {

      UNREFERENCED_PARAMETER(dwFlags);

      if(m_eview == view_grid)
      {

         ::rect rectClient;

         GetClientRect(rectClient);

         size sizePage = rectClient.size();

         m_nGridColumnCount = _001GetColumnCount();

         m_nColumnCount = MAX(m_nColumnCount,sizePage.cx * 2);

         m_nColumnCount = MIN(m_nColumnCount,m_nGridColumnCount);

      }
      else
      {

         m_nColumnCount = _001GetColumnCount();

      }

      if(m_nColumnCount < 0)
      {

         return false;

      }

      on_layout();

      return true;

   }

   bool mesh::_001OnUpdateItemCount(uint32_t dwFlags)
   {

      synch_lock sl(m_pmutex);

      UNREFERENCED_PARAMETER(dwFlags);

      if(m_eview == view_grid)
      {

         ::rect rectClient;

         GetClientRect(rectClient);

         size sizePage = rectClient.size();

         m_nGridItemCount = _001GetItemCount();

         m_nItemCount = MAX(m_nItemCount, sizePage.cy * 2);

         m_nItemCount = MIN(m_nItemCount,m_nGridItemCount);

      }
      else
      {

         m_nItemCount = _001GetItemCount();

      }

      if(m_nItemCount < 0)
      {

         return false;

      }

      if(m_bGroup)
      {

         m_nGroupCount = _001GetGroupCount();

         if(m_nGroupCount < 0)
         {

            return false;

         }

      }

      if(m_eview == view_icon)
      {
         for(index iStrict = 0; iStrict < m_nItemCount; iStrict++)
         {
            if(m_iconlayout.m_iaDisplayToStrict.get_a(iStrict) == -1)
            {
               m_iconlayout.m_iaDisplayToStrict.set(m_iconlayout.m_iaDisplayToStrict.get_free_a(),iStrict);
            }
         }
      }
      else
      {
         index iStart = m_meshlayout.m_iaDisplayToStrict.get_count();
         index iEnd = m_nItemCount - 1;
         m_meshlayout.m_iaDisplayToStrict.allocate(m_nItemCount);
         for(index iStrict = iStart; iStrict <= iEnd; iStrict++)
         {
            m_meshlayout.m_iaDisplayToStrict.set_at(iStrict,iStrict);
         }
      }

      if(m_bFilter1)
      {
         FilterApply();
      }

      CacheHint();
      
      on_change_view_size();

      TRACE("mesh::_001OnUpdateItemCount ItemCount %d\n",m_nItemCount);
      if(m_bGroup)
      {
         TRACE("mesh::_001OnUpdateItemCount GroupCount %d\n",m_nGroupCount);
      }

      return true;
   }

   void mesh::on_change_view_size()
   {

      size & sizeTotal = m_sizeTotal;

      if(m_eview == view_grid)
      {

         sizeTotal.cy = (LONG) m_nColumnCount;
         sizeTotal.cx = (LONG) m_nItemCount;

      }
      else if(m_eview == view_list)
      {
         if(m_nItemCount == 0)
         {
            sizeTotal.cy = 0;
            sizeTotal.cx = 0;
         }
         else
         {

            ::rect rectClient;

            GetClientRect(&rectClient);

            sizeTotal.cy = rectClient.height();

            draw_mesh_item itemFirst(this);

            itemFirst.m_iItem          = 0;
            itemFirst.m_iDisplayItem   = 0;

            _001GetItemRect(&itemFirst);


            if(m_iItemHeight <= 0)
               sizeTotal.cx = rectClient.right;
            else
               sizeTotal.cx  = (LONG)MIN(
               m_nItemCount * itemFirst.m_rectItem.width() * m_iItemHeight /
               rectClient.height()
               + itemFirst.m_rectItem.width(),MAXLONG);
         }
      }
      else if(m_eview == view_report)
      {
         if(m_nItemCount == 0)
         {
            sizeTotal.cy = 0;
            sizeTotal.cx = 0;
         }
         else
         {

            ::rect rect;

            draw_mesh_item itemFirst(this);

            itemFirst.m_iItem             = 0;
            itemFirst.m_iDisplayItem      = 0;
            itemFirst.m_iGroup            = 0;
            _001GetItemRect(&itemFirst);

            draw_mesh_item itemLast(this);

            if(m_bFilter1)
            {
               itemLast.m_iDisplayItem       = m_piaFilterMesh->get_count() - 1;
               itemLast.m_iItem              = m_piaFilterMesh->get_count() - 1;
            }
            else
            {
               itemLast.m_iDisplayItem       = m_nItemCount - 1;
               itemLast.m_iItem              = m_nItemCount - 1;
            }
            if(m_bGroup)
            {
               itemLast.m_iGroupTopIndex = 0;
               //               int32_t igroup;
               for(itemLast.m_iGroup = 0; itemLast.m_iGroup < m_nGroupCount; itemLast.m_iGroup++)
               {
                  itemLast.m_iGroupCount = _001GetGroupItemCount(itemLast.m_iGroup);
                  if(itemLast.m_iItem >= itemLast.m_iGroupTopIndex && itemLast.m_iItem < (itemLast.m_iGroupTopIndex + itemLast.m_iGroupCount))
                     break;
               }
            }
            _001GetItemRect(&itemLast);

//            itemLast.m_rectItem.right     -= (m_scrolldata.m_rectMargin.left + m_scrolldata.m_rectMargin.right);
  //          itemLast.m_rectItem.bottom    -= (m_scrolldata.m_rectMargin.top + m_scrolldata.m_rectMargin.bottom);

            rect.unite(itemFirst.m_rectItem,itemLast.m_rectItem);

            sizeTotal = rect.size();

         }

      }
      else if(m_eview == view_icon)
      {

         ::rect rect;

         draw_mesh_item itemFirst(this);

         itemFirst.m_iItem             = 0;
         itemFirst.m_iDisplayItem      = 0;

         _001GetItemRect(&itemFirst);

         draw_mesh_item itemLast(this);

         itemLast.m_iDisplayItem       = m_nItemCount - 1;
         itemLast.m_iItem              = m_nItemCount - 1;

         _001GetItemRect(&itemLast);

         draw_mesh_item itemTopRight(this);
//         if(m_flags.is_signalized(flag_auto_arrange) || m_iconlayout.m_iWidth <= 0)
         {
            class rect rectClient;

            GetClientRect(&rectClient);

            itemTopRight.m_iItem = (index)MAX(1,rectClient.width() / get_item_size().cx) - 1;
         }
  /*       else
         {
            itemTopRight.m_iItem = MAX(1,m_iconlayout.m_iWidth) - 1;
         }
  */       itemTopRight.m_iDisplayItem = itemTopRight.m_iDisplayItem;
         _001GetItemRect(&itemTopRight);

         rect.unite(itemFirst.m_rectItem,itemLast.m_rectItem);

         rect.unite(rect,itemTopRight.m_rectItem);

         sizeTotal = rect.size();

      }

//      ::user::box::on_change_view_size();

   }


   void mesh::_001OnInitialize()
   {

   }





   void mesh::data_update_visible_subitem()
   {

      return;

      synch_lock sl(&m_mutexData);

//      index iColumn;

      index_array iaVisible;


   }



   void mesh::_001GetColumnWidth(draw_mesh_item * pitem)
   {

      int iWidth;

      if(!m_mapColumnWidth.Lookup(pitem->m_iColumn,iWidth))
      {

         iWidth = m_iDefaultColumnWidth;

      }

      pitem->m_iColumnWidth   = iWidth;

      pitem->m_bOk            = true;

   }


   index mesh::_001MapSubItemToOrder(index iSubItem)
   {
      return _001MapColumnToOrder(_001MapSubItemToColumn(iSubItem));
   }

   index mesh::_001MapOrderToSubItem(index iOrder)
   {
      return _001MapColumnToSubItem(_001MapOrderToColumn(iOrder));
   }


   index mesh::_001MapOrderToColumn(index iOrder)
   {

      if(iOrder >= _001GetColumnCount())
      {

         return -1;

      }

      if(iOrder < 0)
      {

         return -1;

      }

      return iOrder;

   }


   index mesh::_001MapColumnToOrder(index iColumn)
   {

      if(iColumn >= _001GetColumnCount())
      {

         return -1;

      }

      if(iColumn < 0)
      {

         return -1;

      }

      return iColumn;

   }

   index mesh::_001MapSubItemToColumn(index iSubItem)
   {

      if(iSubItem >= _001GetColumnCount())
      {

         return -1;

      }

      if(iSubItem < 0)
      {

         return -1;

      }

      return iSubItem;

   }

   index mesh::_001MapColumnToSubItem(index iColumn)
   {

      if(iColumn >= _001GetColumnCount())
      {

         return -1;

      }

      if(iColumn < 0)
      {

         return -1;

      }

      return iColumn;
   }


   ::count mesh::_001GetItemCount()
   {

      if(m_pmeshdata.is_null())
         return -1;

      return m_pmeshdata->_001GetItemCount();

   }


   ::count mesh::_001GetGroupCount()
   {

      if(m_pmeshdata.is_null())
         return -1;

      return m_pmeshdata->_001GetGroupCount();

   }

   /////////////////////////////////////////////////////////////////
   //
   // Function: _001CalcDisplayTopIndex
   // Old Name: _001GetTopIndex
   //
   // Purpose:
   // Return the index of the first visible item in the mesh
   //
   // Output:
   // The index of the first visible item in the mesh
   //
   //
   /////////////////////////////////////////////////////////////////
   index mesh::_001CalcDisplayTopIndex()
   {


      if(m_eview == view_grid)
      {

         return MIN(MAX(0,get_viewport_offset().y),m_nGridItemCount);

      }
      else
      {

         point ptScroll = get_viewport_offset();

         index iItem;

         if(_001DisplayHitTest(point(ptScroll.x < 0 ? -ptScroll.x : 0,m_iItemHeight + (ptScroll.y < -0 ? -ptScroll.y : 0)),iItem))
         {

            return iItem;

         }
         else
         {

            if(m_eview == view_report || m_eview == view_grid)
            {

               if(ptScroll.y < 0)
               {

                  return 0;

               }

            }

            return -1;

         }

      }

   }

   ::count mesh::_001CalcDisplayItemCount()
   {

      if(m_eview == view_grid)
      {

         rect rectScroll;

         GetClientRect(&rectScroll);

         if(m_iItemHeight == 0)
         {

            return 0;

         }
         else
         {

            if(m_bTopText)
            {
               return rectScroll.height() - m_rectTopText.height() / m_iItemHeight;
            }
            else
            {
               return rectScroll.height();
            }

         }

      }
      if(m_eview == view_icon)
      {
         rect rectView;
         GetClientRect(&rectView);
         class size sizeItem = get_item_size();
         return MAX((rectView.width() / sizeItem.cx) * (rectView.height() / sizeItem.cy),
            m_iconlayout.m_iaDisplayToStrict.get_max_a() + 1);
      }
      else if(m_eview == view_report || m_eview == view_grid)
      {
         rect rectView;
         GetClientRect(&rectView);
         if(m_iItemHeight == 0)
         {
            return 0;
         }
         else
         {
            if(m_bTopText)
            {
               return (rectView.height() - m_rectTopText.height()) / m_iItemHeight;
            }
            else
            {
               return rectView.height() / m_iItemHeight;
            }
         }
      }

      int_ptr iItemCount;
      if(m_bFilter1 && (m_eview == view_list || m_eview == view_report || m_eview == view_grid))
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
      draw_mesh_item item(this);
      if(iItemFirst >= 0)
      {
         iItemLast = -1;
         for(int_ptr i = iItemFirst + 1; i < iItemCount; i++)
         {
            item.m_iItem         = i;
            item.m_iDisplayItem  = i;
            if(m_bGroup)
            {
               item.m_iGroupTopIndex = 0;
               for(item.m_iGroup = 0; item.m_iGroup < m_nGroupCount; item.m_iGroup++)
               {
                  item.m_iGroupCount = _001GetGroupItemCount(item.m_iGroup);
                  if(i >= item.m_iGroupTopIndex && i < (item.m_iGroupTopIndex + item.m_iGroupCount))
                     break;
               }
            }

            _001GetItemRect(&item);
            if(item.m_bOk)
            {
               if(!rectIntersect.intersect(item.m_rectItem,rectUpdate))
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
         if(iItemLast < 0)
            iItemLast = iItemCount - 1;
         return iItemLast - iItemFirst + 1;
      }
      else
      {
         return 0;
      }

   }


   bool mesh::_001HitTest_(point point,index &iItem,index &iSubItem,index &iListItem,::user::mesh::e_element &eelement)
   {
      UNREFERENCED_PARAMETER(point);
      UNREFERENCED_PARAMETER(iItem);
      UNREFERENCED_PARAMETER(iSubItem);
      UNREFERENCED_PARAMETER(iListItem);
      UNREFERENCED_PARAMETER(eelement);
      return false;
   }

   bool mesh::_001HitTest_(point point,index &iItem,index &iSubItem)
   {
      if(!_001DisplayHitTest(point,iItem,iSubItem))
         return false;
      iItem = DisplayToStrict(iItem);
      return true;
   }

   bool mesh::_001HitTest_(POINT pt,index & iItem)
   {
      if(!_001DisplayHitTest(pt,iItem))
         return false;
      iItem = DisplayToStrict(iItem);
      return true;
   }

   bool mesh::_001DisplayHitTest(point point,index & iItem,index & iSubItem,index & iListItem,::user::mesh::e_element &eelement)
   {
      UNREFERENCED_PARAMETER(point);
      UNREFERENCED_PARAMETER(iItem);
      UNREFERENCED_PARAMETER(iSubItem);
      UNREFERENCED_PARAMETER(iListItem);
      UNREFERENCED_PARAMETER(eelement);
      return false;
   }

   bool mesh::_001DisplayHitTest(point point,int_ptr &iItemParam,int_ptr &iSubItemParam)
   {

      int_ptr iItem;

      if(!_001DisplayHitTest(point,iItem))
      {

         return false;

      }

      ::point ptScroll = get_viewport_offset();

      if(m_eview == view_icon || m_eview == view_list)
      {

         iItemParam = iItem;
         if(iItem == -1)
         {
            iSubItemParam = -1;
         }
         else
         {
            iSubItemParam = 0;
         }
         return true;
      }
      int_ptr iColumnCount = m_nColumnCount;
      int_ptr iLeft =(index)ptScroll.x;
      if(m_bGroup && m_bLateralGroup)
         iLeft += m_iLateralGroupWidth;
      int_ptr iRight;
      draw_mesh_item item(this);
      if(point.x < iLeft)
         return false;
      for(item.m_iColumn = 0; item.m_iColumn < iColumnCount; item.m_iColumn++)
      {
         _001GetColumnWidth(&item);
         if(!item.m_bOk)
            continue;
         iRight = iLeft + item.m_iColumnWidth;
         if(iLeft <= point.x && point.x < iRight)
         {
            iItemParam = iItem;
            iSubItemParam = _001MapColumnToSubItem(item.m_iColumn);
            return true;
         }
         iLeft = iRight;
      }
      return false;
   }

   bool mesh::_001DisplayHitTest(POINT pt,int_ptr &iItemParam)
   {

      {

         class rect rectClient;

         GetClientRect(&rectClient);

         if(pt.x < 0
            || pt.x > rectClient.right
            || pt.y < 0
            || pt.x > rectClient.bottom)
         {
            return false;
         }


      }

      ::point ptScroll = get_viewport_offset();

      if(m_eview == view_report || m_eview == view_grid)
      {
         index iy = pt.y + ptScroll.y;

         index iItem = -1;

         if(m_iItemHeight != 0)
            iItem = iy / m_iItemHeight;

         if(iItem < 0)
            return false;

         //if(m_bHeaderCtrl)
         //{
            iItem--;

            if(iItem < 0)
               return false;
         //}

         if(m_bFilter1)
         {
            if(iItem >= m_piaFilterMesh->get_count())
               return false;
         }

         if(iItem >= m_nItemCount)
            return false;

         iItemParam = (index)iItem;

         return true;
      }
      else if(m_eview == view_list)
      {
         if(m_iItemHeight <= 0)
            return false;

         class rect rectClient;
         GetClientRect(&rectClient);
         if(m_bTopText)
         {
            rectClient.top += m_rectTopText.height();
         }
         //if(m_bHeaderCtrl)
         //{
         //   rectClient.top += m_iItemHeight;
         //}
         index iRoundHeight = (index)((rectClient.height() / m_iItemHeight) * m_iItemHeight);

         index iy = (index)((pt.y + ptScroll.y) + (((pt.x + ptScroll.x) / m_iItemWidth)) * iRoundHeight);

         index iItem = -1;

         if(m_iItemHeight != 0)
            iItem = iy / m_iItemHeight;

         if(iItem < 0)
            return false;

         //if(m_bHeaderCtrl)
         //{
         //   iItem--;

         //   if(iItem < 0)
         //      return false;
         //}

         if(m_bFilter1)
         {
            if(iItem >= m_piaFilterMesh->get_count())
               return false;
         }

         if(iItem >= m_nItemCount)
            return false;

         iItemParam = iItem;

         return true;

      }
      else if(m_eview == view_icon)
      {
         /*if(_001GetColumnCount() == 0)
            return false;
         */class rect rectClient;
         GetClientRect(&rectClient);
         if(m_bTopText)
         {
            rectClient.top += m_rectTopText.height();
         }
//         index iIconSize = MAX(32,m_columna[0]->m_sizeIcon.cy);
         index iIconSize = 32;
         index iItemSize = iIconSize * 2;

         index ix = (index)(pt.x + ptScroll.x);
         ix = (index)MAX(ptScroll.x,ix);
         ix = (index)MIN(rectClient.right,ix);
         ix = (index)MAX(rectClient.left,ix);
         ix /= iItemSize;

         index iy = pt.y + ptScroll.y;
         iy = MAX(ptScroll.y,iy);
         iy = MAX(rectClient.top,iy);
         iy /= iItemSize;

         //if(m_flags.is_signalized(flag_auto_arrange))
         {
            iItemParam = iy * (MAX(1,rectClient.width() / iItemSize)) + ix;
         }
         //else
         {
           // iItemParam = iy * (MAX(1,m_iconlayout.m_iWidth)) + ix;
         }


         return true;
      }

      return false;
   }

   void mesh::_001GetGroupRect(::user::draw_mesh_item * pdrawitem)
   {
      if(pdrawitem->m_iGroup < 0)
         return_(pdrawitem->m_bOk,false);
      if(pdrawitem->m_iGroup >= m_nGroupCount)
         return_(pdrawitem->m_bOk,false);
      index iItemFirst = 0;
      index iItemLast = _001GetGroupItemCount(0) - 1;
      pdrawitem->m_iGroupRectGroup = 0;
      while(pdrawitem->m_iGroupRectGroup < pdrawitem->m_iGroup)
      {
         pdrawitem->m_iGroupRectGroup++;
         iItemFirst = iItemLast + 1;
         iItemLast = iItemFirst + _001GetGroupItemCount(pdrawitem->m_iGroupRectGroup) - 1;
      }

      draw_mesh_item itemFirst(this);

      itemFirst.m_iItem             = DisplayToStrict(iItemFirst);
      itemFirst.m_iDisplayItem      = iItemFirst;
      itemFirst.m_iGroup            = pdrawitem->m_iGroupRectGroup;

      _001GetItemRect(&itemFirst);

      draw_mesh_item itemLast(this);

      itemLast.m_iDisplayItem       = DisplayToStrict(iItemLast);
      itemLast.m_iItem              = iItemLast;
      itemLast.m_iGroup             = pdrawitem->m_iGroupRectGroup;

      _001GetItemRect(&itemLast);

      pdrawitem->m_rectGroup.unite(itemFirst.m_rectItem,itemLast.m_rectItem);
      pdrawitem->m_rectGroup.bottom = MAX(itemLast.m_rectItem.bottom,itemFirst.m_rectItem.top + m_iGroupMinHeight);
      pdrawitem->m_rectGroup.left = 0;
      pdrawitem->m_rectGroup.right = m_iLateralGroupWidth;
      pdrawitem->m_bOk = true;
   }

   void mesh::_001GetItemRect(::user::draw_mesh_item * pdrawitem)
   {

      if(pdrawitem->m_iDisplayItem == (-1 - pdrawitem->m_iItemRectItem) && pdrawitem->m_iItemRectItem >= 0)
      {
         return_(pdrawitem->m_bOk,false);
      }

      if(pdrawitem->m_iDisplayItem == pdrawitem->m_iItemRectItem)
      {
         return_(pdrawitem->m_bOk,true);
      }

      if(pdrawitem->m_iDisplayItem < 0)
      {
         pdrawitem->m_rectItem.left = 0;
         pdrawitem->m_rectItem.top = 0;
         pdrawitem->m_rectItem.right = 0;
         pdrawitem->m_rectItem.bottom = 0;
         pdrawitem->m_iItemRectItem = (-1 - pdrawitem->m_iDisplayItem);
         return_(pdrawitem->m_bOk,false);
      }
      if(m_eview == view_icon && pdrawitem->m_iDisplayItem >= m_nDisplayCount)
      {
         pdrawitem->m_rectItem.left = 0;
         pdrawitem->m_rectItem.top = 0;
         pdrawitem->m_rectItem.right = 0;
         pdrawitem->m_rectItem.bottom = 0;
         pdrawitem->m_iItemRectItem = (-1 - pdrawitem->m_iDisplayItem);
         return_(pdrawitem->m_bOk,false);
      }

      ::point ptScroll = get_viewport_offset();

      if(m_eview == view_grid)
      {

         if(m_bGroup)
         {

            if(m_bLateralGroup)
            {

               if(pdrawitem->m_iItemRectItem < 0)
               {

                  pdrawitem->m_rectItem.left    = (LONG) m_iLateralGroupWidth;
                  pdrawitem->m_rectItem.right   = (LONG)(pdrawitem->m_rectItem.left + m_iItemWidth);
                  pdrawitem->m_iItemRectItem = 0;
                  pdrawitem->m_rectItem.top  = 0;
                  //if(m_bHeaderCtrl)
                  //{
                  //   pdrawitem->m_rectItem.top += m_iItemHeight;
                  //}
                  if(m_bTopText)
                  {
                     pdrawitem->m_rectItem.top += m_rectTopText.height();
                  }
                  pdrawitem->m_rectItem.bottom = pdrawitem->m_rectItem.top + m_iItemHeight;
                  pdrawitem->m_rectItem.offset(-ptScroll.x,-ptScroll.y);
               }

               if(pdrawitem->m_iDisplayItem > pdrawitem->m_iItemRectItem)
               {
                  int32_t iOffset                               = (int32_t)((pdrawitem->m_iItemRectItem - pdrawitem->m_iGroupTopIndex) * m_iItemHeight);
                  pdrawitem->m_rectItem.top                -= iOffset;
                  pdrawitem->m_rectItem.bottom              = pdrawitem->m_rectItem.top + m_iItemHeight;
                  while(pdrawitem->m_iGroupTopIndex + pdrawitem->m_iGroupCount < pdrawitem->m_iDisplayItem)
                  {
                     //                     int32_t iHeight                            = _001GetGroupHeight(pdrawitem->m_iGroup);
                     pdrawitem->m_rectItem.top             += iOffset;
                     pdrawitem->m_rectItem.bottom           = pdrawitem->m_rectItem.top + m_iItemHeight;
                     if((pdrawitem->m_iGroup + 1) >= m_nGroupCount)
                     {
                        break;
                     }
                     pdrawitem->m_iGroup++;
                     pdrawitem->m_iGroupTopIndex            += pdrawitem->m_iGroupCount;
                     pdrawitem->m_iGroupCount               = _001GetGroupItemCount(pdrawitem->m_iGroup);
                     pdrawitem->m_iItemRectItem             = pdrawitem->m_iGroupTopIndex;
                  }
               }
               else
               {
                  int32_t iOffset                               = (int32_t)((pdrawitem->m_iItemRectItem - pdrawitem->m_iGroupTopIndex) * m_iItemHeight);
                  pdrawitem->m_rectItem.top                -= iOffset;
                  pdrawitem->m_rectItem.bottom              = pdrawitem->m_rectItem.top + m_iItemHeight;
                  while(pdrawitem->m_iGroupTopIndex + pdrawitem->m_iGroupCount > pdrawitem->m_iDisplayItem)
                  {
                     if((pdrawitem->m_iGroup - 1) < 0)
                     {
                        break;
                     }
                     pdrawitem->m_iGroup--;
                     int32_t iHeight                         = _001GetGroupHeight(pdrawitem->m_iGroup);
                     pdrawitem->m_rectItem.top          -= iHeight;
                     pdrawitem->m_rectItem.bottom        = pdrawitem->m_rectItem.top + m_iItemHeight;
                     pdrawitem->m_iGroupCount            = _001GetGroupItemCount(pdrawitem->m_iGroup);
                     pdrawitem->m_iGroupTopIndex        -= pdrawitem->m_iGroupCount;
                     pdrawitem->m_iItemRectItem          = pdrawitem->m_iGroupTopIndex;
                  }
               }
               if(pdrawitem->m_iGroup < m_nGroupCount && pdrawitem->m_iGroup >= 0)
               {
                  pdrawitem->m_iItemRectItem       = pdrawitem->m_iDisplayItem;
                  pdrawitem->m_rectItem.top       += (LONG)((pdrawitem->m_iItemRectItem - pdrawitem->m_iGroupTopIndex) * m_iItemHeight);
                  pdrawitem->m_rectItem.bottom     =  pdrawitem->m_rectItem.top + m_iItemHeight;
               }
            }
            else
            {
               ::exception::throw_not_implemented(get_app());
            }
         }
         else
         {

            pdrawitem->m_rectItem.left    = m_iLeftMargin;

            pdrawitem->m_rectItem.right   = (LONG)(m_iLeftMargin + m_iItemWidth);

            pdrawitem->m_rectItem.top = 0;
            //if(m_bHeaderCtrl)
            //{
            //   pdrawitem->m_rectItem.top += m_iItemHeight;
            //}
            pdrawitem->m_rectItem.top += (LONG)(m_iItemHeight * pdrawitem->m_iDisplayItem);
            if(m_bTopText)
            {
               pdrawitem->m_rectItem.top += m_rectTopText.height();
            }
            pdrawitem->m_rectItem.bottom = pdrawitem->m_rectItem.top + m_iItemHeight;
            pdrawitem->m_rectItem.offset(0,m_iTopMargin -ptScroll.y * m_iItemHeight);
            pdrawitem->m_iItemRectItem = pdrawitem->m_iDisplayItem;
         }
      }
      else if(m_eview == view_report)
      {

         if(m_bGroup)
         {

            if(m_bLateralGroup)
            {

               if(pdrawitem->m_iItemRectItem < 0)
               {

                  pdrawitem->m_rectItem.left    = (LONG)m_iLateralGroupWidth;
                  pdrawitem->m_rectItem.right   = (LONG)(pdrawitem->m_rectItem.left + m_iItemWidth);
                  pdrawitem->m_iItemRectItem = 0;
                  pdrawitem->m_rectItem.top  = 0;
                  //if(m_bHeaderCtrl)
                  //{
                  //   pdrawitem->m_rectItem.top += m_iItemHeight;
                  //}
                  if(m_bTopText)
                  {
                     pdrawitem->m_rectItem.top += m_rectTopText.height();
                  }
                  pdrawitem->m_rectItem.bottom = pdrawitem->m_rectItem.top + m_iItemHeight;
                  pdrawitem->m_rectItem.offset(-ptScroll.x,-ptScroll.y);
               }

               if(pdrawitem->m_iDisplayItem > pdrawitem->m_iItemRectItem)
               {
                  int32_t iOffset                               = (int32_t)((pdrawitem->m_iItemRectItem - pdrawitem->m_iGroupTopIndex) * m_iItemHeight);
                  pdrawitem->m_rectItem.top                -= iOffset;
                  pdrawitem->m_rectItem.bottom              = pdrawitem->m_rectItem.top + m_iItemHeight;
                  while(pdrawitem->m_iGroupTopIndex + pdrawitem->m_iGroupCount < pdrawitem->m_iDisplayItem)
                  {
                     //                     int32_t iHeight                            = _001GetGroupHeight(pdrawitem->m_iGroup);
                     pdrawitem->m_rectItem.top             += iOffset;
                     pdrawitem->m_rectItem.bottom           = pdrawitem->m_rectItem.top + m_iItemHeight;
                     if((pdrawitem->m_iGroup + 1) >= m_nGroupCount)
                     {
                        break;
                     }
                     pdrawitem->m_iGroup++;
                     pdrawitem->m_iGroupTopIndex            += pdrawitem->m_iGroupCount;
                     pdrawitem->m_iGroupCount               = _001GetGroupItemCount(pdrawitem->m_iGroup);
                     pdrawitem->m_iItemRectItem             = pdrawitem->m_iGroupTopIndex;
                  }
               }
               else
               {
                  int32_t iOffset                               = (int32_t)((pdrawitem->m_iItemRectItem - pdrawitem->m_iGroupTopIndex) * m_iItemHeight);
                  pdrawitem->m_rectItem.top                -= iOffset;
                  pdrawitem->m_rectItem.bottom              = pdrawitem->m_rectItem.top + m_iItemHeight;
                  while(pdrawitem->m_iGroupTopIndex + pdrawitem->m_iGroupCount > pdrawitem->m_iDisplayItem)
                  {
                     if((pdrawitem->m_iGroup - 1) < 0)
                     {
                        break;
                     }
                     pdrawitem->m_iGroup--;
                     int32_t iHeight                         = _001GetGroupHeight(pdrawitem->m_iGroup);
                     pdrawitem->m_rectItem.top          -= iHeight;
                     pdrawitem->m_rectItem.bottom        = pdrawitem->m_rectItem.top + m_iItemHeight;
                     pdrawitem->m_iGroupCount            = _001GetGroupItemCount(pdrawitem->m_iGroup);
                     pdrawitem->m_iGroupTopIndex        -= pdrawitem->m_iGroupCount;
                     pdrawitem->m_iItemRectItem          = pdrawitem->m_iGroupTopIndex;
                  }
               }
               if(pdrawitem->m_iGroup < m_nGroupCount && pdrawitem->m_iGroup >= 0)
               {
                  pdrawitem->m_iItemRectItem       = pdrawitem->m_iDisplayItem;
                  pdrawitem->m_rectItem.top       += (LONG)((pdrawitem->m_iItemRectItem - pdrawitem->m_iGroupTopIndex) * m_iItemHeight);
                  pdrawitem->m_rectItem.bottom     =  pdrawitem->m_rectItem.top + m_iItemHeight;
               }
            }
            else
            {
               ::exception::throw_not_implemented(get_app());
            }
         }
         else
         {
            pdrawitem->m_rectItem.left    = 0;
            pdrawitem->m_rectItem.right   = (LONG)m_iItemWidth;

            pdrawitem->m_rectItem.top = 0;
            //if(m_bHeaderCtrl)
            //{
            //   pdrawitem->m_rectItem.top += m_iItemHeight;
            //}
            pdrawitem->m_rectItem.top += (LONG)(m_iItemHeight * pdrawitem->m_iDisplayItem);
            if(m_bTopText)
            {
               pdrawitem->m_rectItem.top += m_rectTopText.height();
            }
            pdrawitem->m_rectItem.bottom = pdrawitem->m_rectItem.top + m_iItemHeight;
            pdrawitem->m_rectItem.offset(-ptScroll.x,-ptScroll.y);
            pdrawitem->m_iItemRectItem   = pdrawitem->m_iDisplayItem;
         }
      }
      else if(m_eview == view_list)
      {
         class rect rectClient;
         GetClientRect(&rectClient);
         if(m_bTopText)
         {
            rectClient.top += m_rectTopText.height();
         }
         //if(m_bHeaderCtrl)
         //{
         //   rectClient.top += m_iItemHeight;
         //}
         if(m_iItemHeight <= 0)
            return_(pdrawitem->m_bOk,false);
         index iRoundHeight = (rectClient.height() / m_iItemHeight) * m_iItemHeight;
         if(iRoundHeight != 0)
         {
            pdrawitem->m_rectItem.left = (LONG)(((pdrawitem->m_iItem * m_iItemHeight) / iRoundHeight) * m_iItemWidth);
            pdrawitem->m_rectItem.top = (LONG)((pdrawitem->m_iItem * m_iItemHeight) % iRoundHeight);
         }
         if(m_bTopText)
         {
            pdrawitem->m_rectItem.top += m_rectTopText.height();
         }
         //if(m_bHeaderCtrl)
         //{
         //   pdrawitem->m_rectItem.top += m_iItemHeight;
         //}
         pdrawitem->m_rectItem.bottom = (LONG)(pdrawitem->m_rectItem.top + m_iItemHeight);
         pdrawitem->m_rectItem.right = (LONG)(pdrawitem->m_rectItem.left + m_iItemWidth);
         pdrawitem->m_rectItem.offset(-ptScroll.x,-ptScroll.y);
      }
      else if(m_eview == view_icon)
      {
//         if(m_flags.is_signalized(flag_auto_arrange) || m_iconlayout.m_iWidth <= 0)
         {
            class rect rectClient;
            GetClientRect(&rectClient);
            if(m_bTopText)
            {
               rectClient.top += m_rectTopText.height();
            }
            //index iIconSize = MAX(32,m_columna[0]->m_sizeIcon.cy);
            index iIconSize = 32;
            index iItemSize = iIconSize * 2;
            pdrawitem->m_rectItem.left = (LONG)(iItemSize * (pdrawitem->m_iItem % (MAX(1,rectClient.width() / iItemSize))));
            pdrawitem->m_rectItem.top = (LONG)(iItemSize * (pdrawitem->m_iItem / (MAX(1,rectClient.width() / iItemSize))));
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
         //   pdrawitem->m_rectItem.left = (LONG)(iItemSize * (pdrawitem->m_iDisplayItem % m_iconlayout.m_iWidth));
         //   pdrawitem->m_rectItem.top = (LONG)(rectClient.top + iItemSize * (pdrawitem->m_iDisplayItem / m_iconlayout.m_iWidth));
         //   pdrawitem->m_rectItem.bottom = (LONG)(pdrawitem->m_rectItem.top + iItemSize);
         //   pdrawitem->m_rectItem.right = (LONG)(pdrawitem->m_rectItem.left + iItemSize);
         //}
         pdrawitem->m_rectItem.offset(-ptScroll.x,-ptScroll.y);
      }

      pdrawitem->m_bOk = true;

   }

   void mesh::_001GetSubItemRect(::user::draw_mesh_item * pdrawitem)
   {

      if(pdrawitem->m_iDisplayItem != pdrawitem->m_iItemRectItem)
      {
         _001GetItemRect(pdrawitem);
         if(!pdrawitem->m_bOk)
            return;
      }

      if(pdrawitem->m_iSubItemRectOrder == pdrawitem->m_iOrder)
      {
         if(pdrawitem->m_iSubItemRectItem != pdrawitem->m_iItemRectItem)
         {
            pdrawitem->m_rectSubItem.top     = pdrawitem->m_rectItem.top;
            pdrawitem->m_rectSubItem.bottom  = pdrawitem->m_rectItem.bottom;
         }
         return_(pdrawitem->m_bOk,true);
      }

      if(m_eview == view_icon)
      {
         pdrawitem->m_rectSubItem     = pdrawitem->m_rectItem;
         return_(pdrawitem->m_bOk,true);
         return;
         throw "subitem rectangle on icon view? why are you asking for that now?";
      }

      pdrawitem->m_bOk = false;

      pdrawitem->m_rectSubItem.top     = pdrawitem->m_rectItem.top;
      pdrawitem->m_rectSubItem.bottom  = pdrawitem->m_rectItem.bottom;

      index iLastOrder = m_nColumnCount - 1;

      if(iLastOrder < 0)
         return_(pdrawitem->m_bOk,false);



      if(pdrawitem->m_iSubItemRectOrder < 0 || pdrawitem->m_iOrder == 0 || pdrawitem->m_iSubItemRectOrder > pdrawitem->m_iOrder)
      {

         if(m_eview == view_grid)
         {

            pdrawitem->m_iSubItemRectOrder         = pdrawitem->m_iOrder;

         }
         else
         {

            pdrawitem->m_iSubItemRectOrder         = 0;

         }

         pdrawitem->m_rectSubItem.left          = pdrawitem->m_rectItem.left;
         pdrawitem->m_iSubItemRectColumn        = _001MapOrderToColumn(0);
         pdrawitem->m_iColumn                    = pdrawitem->m_iSubItemRectColumn;
         _001GetColumnWidth(pdrawitem);
         pdrawitem->m_rectSubItem.right         = pdrawitem->m_rectItem.left + pdrawitem->m_iColumnWidth;
         pdrawitem->m_iSubItemRectItem          = pdrawitem->m_iItemRectItem;
         pdrawitem->m_iSubItemRectSubItem       = pdrawitem->m_iSubItem;
         if(pdrawitem->m_iOrder == 0)
         {
            pdrawitem->m_bOk = true;
            return;
         }
      }

      if(pdrawitem->m_iOrder <= iLastOrder)
      {
         while(pdrawitem->m_iSubItemRectOrder < pdrawitem->m_iOrder)
         {
            pdrawitem->m_iColumn           = _001MapOrderToColumn(pdrawitem->m_iSubItemRectOrder);
            _001GetColumnWidth(pdrawitem);
            pdrawitem->m_rectSubItem.left       += pdrawitem->m_iColumnWidth;
            pdrawitem->m_iSubItemRectOrder++;
         }
         pdrawitem->m_iColumn = _001MapOrderToColumn(pdrawitem->m_iSubItemRectOrder);
         pdrawitem->m_iSubItemRectColumn        = pdrawitem->m_iColumn;
         _001GetColumnWidth(pdrawitem);
         pdrawitem->m_rectSubItem.right         = pdrawitem->m_rectSubItem.left + pdrawitem->m_iColumnWidth;
         pdrawitem->m_iSubItemRectOrder         = pdrawitem->m_iOrder;
         pdrawitem->m_iSubItemRectItem          = pdrawitem->m_iItemRectItem;
         pdrawitem->m_iSubItemRectSubItem       = pdrawitem->m_iSubItem;
         pdrawitem->m_bOk = true;
      }

   }

   void mesh::_001GetElementRect(::user::draw_mesh_item * pdrawitem,::user::mesh::e_element eelement)
   {

      if(m_bGroup && m_bLateralGroup &&
         (eelement == ::user::mesh::element_group_image
         || eelement == ::user::mesh::element_group_item_text))
      {
         int32_t x = pdrawitem->m_rectGroup.left;
         int32_t iImageBottom = pdrawitem->m_rectGroup.top;

         if(m_pilGroup != NULL)
         {


            ::image_list::info ii;

            _001GetGroupImage(pdrawitem);
            if(pdrawitem->m_bOk && pdrawitem->m_iImage >= 0)
            {
               m_pilGroup->get_image_info((int32_t)pdrawitem->m_iImage,&ii);
               if(eelement == ::user::mesh::element_group_image)
               {
                  pdrawitem->m_rectImage.left      = x;
                  pdrawitem->m_rectImage.right     = x + width(&ii.m_rect);
                  pdrawitem->m_rectImage.top       = pdrawitem->m_rectGroup.top;
                  pdrawitem->m_rectImage.bottom    = pdrawitem->m_rectImage.top + height(&ii.m_rect);
                  return_(pdrawitem->m_bOk,true);
               }
               else
               {
                  x += width(&ii.m_rect);
                  x += 2;
                  iImageBottom += height(&ii.m_rect) + 2;
               }
            }
            else if(eelement == ::user::mesh::element_group_image)
            {
               return_(pdrawitem->m_bOk,false);
            }

         }
         else if(eelement == ::user::mesh::element_group_image)
         {
            return_(pdrawitem->m_bOk,false);
         }
         if(eelement == ::user::mesh::element_group_item_text)
         {
            pdrawitem->m_rectText.top       = (LONG)(pdrawitem->m_rectGroup.top + m_iItemHeight * pdrawitem->m_iItem);
            if(pdrawitem->m_rectText.top >= iImageBottom)
            {
               pdrawitem->m_rectText.left      = pdrawitem->m_rectGroup.left;
            }
            else
            {
               pdrawitem->m_rectText.left      = x;
            }
            pdrawitem->m_rectText.right     = pdrawitem->m_rectGroup.right;
            pdrawitem->m_rectText.bottom    = pdrawitem->m_rectText.top + m_iItemHeight;
            return_(pdrawitem->m_bOk,true);
         }
         pdrawitem->m_bOk = false;
      }
      else
      {
         if(pdrawitem->m_iDisplayItem != pdrawitem->m_iItemRectItem)
         {
            _001GetItemRect(pdrawitem);
            if(!pdrawitem->m_bOk)
               return;
         }
         if(eelement == ::user::mesh::element_item)
         {
            return_(pdrawitem->m_bOk,true);
         }
         if(m_eview == view_icon)
         {
            if(eelement == ::user::mesh::element_image)
            {
               int32_t iIconSize                  = 32;
               pdrawitem->m_rectImage.left      = pdrawitem->m_rectItem.left + iIconSize / 2;
               pdrawitem->m_rectImage.top       = pdrawitem->m_rectItem.top;
               pdrawitem->m_rectImage.right     = pdrawitem->m_rectImage.left + iIconSize;
               pdrawitem->m_rectImage.bottom    = pdrawitem->m_rectImage.top + iIconSize;
               return_(pdrawitem->m_bOk,true);
            }
            else if(eelement == ::user::mesh::element_text)
            {
               int32_t iIconSize               = 32;
               pdrawitem->m_rectText.left    = pdrawitem->m_rectItem.left;
               pdrawitem->m_rectText.top     = pdrawitem->m_rectItem.top + iIconSize;
               pdrawitem->m_rectText.right   = pdrawitem->m_rectText.left + iIconSize * 2;
               pdrawitem->m_rectText.bottom  = pdrawitem->m_rectText.top + iIconSize;
               return_(pdrawitem->m_bOk,true);
            }
            return_(pdrawitem->m_bOk,false);
         }

         _001GetSubItemRect(pdrawitem);

         if(!pdrawitem->m_bOk)
            return;

         int32_t x = pdrawitem->m_rectSubItem.left;

         if(pdrawitem->m_iListItem == -1)
         {
            if(eelement == ::user::mesh::element_sub_item)
            {
               return_(pdrawitem->m_bOk,true);
            }
            //if(pdrawitem->m_pcolumnSubItemRect->m_bIcon)
            //{
            //   _001GetItemImage(pdrawitem);
            //   if(pdrawitem->m_bOk && pdrawitem->m_iImage >= 0)
            //   {
            //      if(eelement == ::user::mesh::element_image)
            //      {
            //         pdrawitem->m_rectImage.left      = x;
            //         pdrawitem->m_rectImage.right     = x + pdrawitem->m_pcolumnSubItemRect->m_sizeIcon.cx;
            //         pdrawitem->m_rectImage.bottom    = pdrawitem->m_rectSubItem.bottom;
            //         pdrawitem->m_rectImage.top       = pdrawitem->m_rectImage.bottom - pdrawitem->m_pcolumnSubItemRect->m_sizeIcon.cx;
            //         return_(pdrawitem->m_bOk,true);
            //      }
            //      else
            //      {
            //         x += pdrawitem->m_pcolumnSubItemRect->m_sizeIcon.cx;
            //         x += 2;
            //      }
            //   }
            //   else if(eelement == ::user::mesh::element_image)
            //   {
            //      return_(pdrawitem->m_bOk,false);
            //   }
            //}
            //else if(pdrawitem->m_pcolumnSubItemRect->m_pil != NULL)
            //{

            //   ::image_list::info ii;

            //   _001GetItemImage(pdrawitem);
            //   if(pdrawitem->m_bOk && pdrawitem->m_iImage >= 0)
            //   {
            //      pdrawitem->m_pcolumnSubItemRect->m_pil->get_image_info((int32_t)pdrawitem->m_iImage,&ii);
            //      if(eelement == ::user::mesh::element_image)
            //      {
            //         pdrawitem->m_rectImage.left      = x;
            //         pdrawitem->m_rectImage.right     = x + width(&ii.m_rect);
            //         pdrawitem->m_rectImage.bottom    = pdrawitem->m_rectSubItem.bottom;
            //         pdrawitem->m_rectImage.top       = pdrawitem->m_rectImage.bottom - height(&ii.m_rect);
            //         return_(pdrawitem->m_bOk,true);
            //      }
            //      else
            //      {
            //         x += width(&ii.m_rect);
            //         x += 2;
            //      }
            //   }
            //   else if(eelement == ::user::mesh::element_image)
            //   {
            //      return_(pdrawitem->m_bOk,false);
            //   }

            //}
            else if(eelement == ::user::mesh::element_image)
            {
               return_(pdrawitem->m_bOk,false);
            }
            if(eelement == ::user::mesh::element_text)
            {
               pdrawitem->m_rectText.left      = x;
               pdrawitem->m_rectText.right     = pdrawitem->m_rectSubItem.right;
               pdrawitem->m_rectText.top       = pdrawitem->m_rectSubItem.top;
               pdrawitem->m_rectText.bottom    = pdrawitem->m_rectSubItem.bottom;
               return_(pdrawitem->m_bOk,true);
            }
         }
         pdrawitem->m_bOk = false;
      }

   }


   bool mesh::range::has_item(index iItem) const
   {
      try
      {
         if (!is_ptr_null(m_piLink, 4096))
         {
            return *m_piLink == iItem;
         }
      }
      catch (...)
      {
      }
      for(index i = 0; i < m_itemrangea.get_size(); i++)
      {
         const item_range & itemrange = m_itemrangea[i];
         if(itemrange.has_item(iItem))
            return true;
      }
      return false;
   }

   ::count mesh::range::get_item_indexes(index_array & ia) const
   {

      for(index i = 0; i < m_itemrangea.get_size(); i++)
      {

         const item_range & itemrange = m_itemrangea[i];

         itemrange.get_item_indexes(ia);

      }

      return ia.get_size();

   }


   bool mesh::range::has_sub_item(int_ptr iItem,int_ptr iSubItem) const
   {
      for(index i = 0; i < m_itemrangea.get_size(); i++)
      {
         const item_range & itemrange = m_itemrangea[i];
         if(itemrange.has_item(iItem))
            if(itemrange.has_sub_item(iSubItem))
               return true;
      }
      return false;
   }

   // remove the specified item
   bool mesh::range::remove_item(index iItem)
   {
      for(index i = 0; i < m_itemrangea.get_size();)
      {
         item_range & itemrange = m_itemrangea[i];
         if(itemrange.has_item(iItem))
         {
            if(itemrange.get_lower_bound() == iItem)
            {
               itemrange.set_lower_bound(itemrange.get_lower_bound() + 1);
               if(itemrange.get_lower_bound() > itemrange.get_upper_bound())
               {
                  m_itemrangea.remove_at(i);
                  continue;
               }
            }
            else if(itemrange.get_upper_bound() == iItem)
            {
               itemrange.set_upper_bound(itemrange.get_upper_bound() - 1);
            }
            else
            {
               // split the current range as it will be segmented
               item_range itemrangeL;
               item_range itemrangeU;
               itemrangeL = itemrange;
               itemrangeU = itemrange;
               itemrangeL.set_upper_bound(iItem - 1);
               itemrangeU.set_lower_bound(iItem + 1);
               m_itemrangea.remove_at(i);
               m_itemrangea.add(itemrangeL);
               m_itemrangea.add(itemrangeU);
               continue;
            }
         }
         i++;
      }
      return false;
   }

   // remove the specified item and offset remaining items.
   bool mesh::range::OnRemoveItem(index iItem)
   {
      remove_item(iItem);
      // All ranges that has item "iItem + 1",
      // must have all items greater or equal "iItem + 1"
      // decremented by one
      iItem++;
      for(index i = 0; i < m_itemrangea.get_size();)
      {
         item_range & itemrange = m_itemrangea[i];
         if(itemrange.has_item(iItem))
         {
            if(itemrange.get_lower_bound() >= iItem)
            {
               itemrange.offset(-1);
            }
            else if(itemrange.get_upper_bound() >= iItem)
            {
               itemrange.set_upper_bound(itemrange.get_upper_bound() - 1);
            }
         }
         i++;
      }
      return false;
   }

   void mesh::item_range::get_item_indexes(index_array & ia) const
   {

      if(m_iLowerBound == -1 || m_iUpperBound == -1)
         return;

      for(index iItem = m_iLowerBound; iItem <= m_iUpperBound; iItem++)
      {

         ia.add_unique(iItem);

      }

   }

   bool mesh::item_range::has_item(index iItem) const
   {
      if(m_iLowerBound == -1 || m_iUpperBound == -1)
         return false;
      if(iItem >= m_iLowerBound
         && iItem <= m_iUpperBound)
         return true;
      else
         return false;
   }

   bool mesh::item_range::has_sub_item(index iSubItem) const
   {
      return m_subitemrange.has_sub_item(iSubItem);
   }

   //void mesh::LayoutHeaderCtrl()
   //{

   //   if(m_pmeshheader == NULL)
   //      return;

   //   if(m_bHeaderCtrl)
   //   {
   //      rect rectClient;

   //      GetClientRect(&rectClient);

   //      m_pmeshheader->SetWindowPos(
   //         ZORDER_TOP,
   //         0,0,
   //         MAX(m_iItemWidth + 10,rectClient.width()),
   //         m_iItemHeight,
   //         SWP_SHOWWINDOW);
   //   }
   //   else
   //   {
   //      m_pmeshheader->ShowWindow(SW_HIDE);
   //   }
   //}

   void mesh::_001OnKeyDown(signal_details * pobj)
   {
      SCAST_PTR(::message::key,pkey,pobj);
         if(pkey->previous()) // give chance to child
            return;
      if(pkey->m_ekey == ::user::key_down || pkey->m_ekey == ::user::key_up ||
         pkey->m_ekey == ::user::key_next || pkey->m_ekey == ::user::key_prior)
      {
         if(m_nItemCount > 0)
         {
            range range = m_rangeSelection;
            m_rangeSelection.clear();

            int_ptr iItem = m_iItemFocus;

            if(iItem < 0)
            {
               if(pkey->m_ekey == ::user::key_down || pkey->m_ekey == ::user::key_next)
               {
                  iItem = 0;
               }
               else if(pkey->m_ekey == ::user::key_up || pkey->m_ekey == ::user::key_prior)
               {
                  iItem = m_nItemCount - 1;
               }
            }
            else
            {
               if(pkey->m_ekey == ::user::key_down)
               {
                  iItem++;
               }
               else if(pkey->m_ekey == ::user::key_next)
               {
                  iItem += m_nDisplayCount;
               }
               else if(pkey->m_ekey == ::user::key_up)
               {
                  iItem--;
               }
               else if(pkey->m_ekey == ::user::key_prior)
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

            if(iItem < 0)
            {
               iItem = m_nItemCount - 1;
            }
            else if(iItem >= m_nItemCount)
            {
               iItem = 0;
            }

            m_iShiftFirstSelection = iItem;
            m_iItemFocus = iItem;

            item_range itemrange;
//            itemrange.set(iItem,iItem,0,m_nColumnCount - 1,- 1,-1);
            itemrange.set(iItem,iItem,0,m_nColumnCount - 1,- 1,-1);
            m_rangeSelection.add_item(itemrange);

            _001EnsureVisible(iItem,range);

            //range.add_item(itemrange);

            RedrawWindow();

            _001OnSelectionChange();
         }
      }
      else if(pkey->m_ekey == ::user::key_delete)
      {

         range range;

         _001GetSelection(range);

         _001DeleteRange(range);

         pobj->m_bRet = true;

      }

      pobj->m_bRet = false;
   }

   void mesh::_001OnMouseMove(signal_details * pobj)
   {

      SCAST_PTR(::message::mouse, pmouse, pobj);

      pmouse->set_lresult(1);

      point pt = pmouse->m_pt;

      ScreenToClient(&pt);

      pmouse->previous(); // give chance to child control

      synch_lock sl(m_pmutex);

      if (m_bDrag)
      {

         if (!m_bLButtonDown)
         {

            m_bDrag = false;

         }
         else if (pmouse->m_nFlags == 0)
         {

            m_bLButtonDown = false;
            m_bDrag = false;

         }

      }

      if (m_bDrag)
      {

         if (m_iItemMouseDown >= 0)
         {
            //index iItemOld = m_iItemDrop;
            m_ptLButtonUp = pt;
            //if (!_001DisplayHitTest(pt, m_iItemDrop))
            {
            }
            //            if (iItemOld != m_iItemDrop)
            {
               //             if (m_eview == view_icon)
               {
                  //if (iItemOld != m_iItemDrag)
                  {
                     //                  m_iconlayout.m_iaDisplayToStrict.array_remove_a(iItemOld);
                  }
                  //             m_iconlayout.m_iaDisplayToStrict.array_insert(m_iItemDrop, m_iStrictItemDrag);
                  //array_translate_a(m_iconlayout.m_iaDisplayToStrict, m_iItemDrop, iItemOld);

               }
               RedrawWindow();
            }
         }
      }
      else if(m_bHoverSelect)
      {

         if (m_bLButtonDown)
         {

            bool bShouldStartDrag = d_distance(pt, m_ptLButtonDown) > m_iItemHeight;

            if (!bShouldStartDrag)
            {

               index iItem = -1;

               if (_001DisplayHitTest(pt, iItem))
               {

                  if (iItem != m_iDisplayItemLButtonDown)
                  {

                     bShouldStartDrag = true;

                  }

               }

            }

            if (bShouldStartDrag)
            {

               m_bDrag = true;

            }


         }

      }

      track_mouse_leave();

      //if(m_spmenuPopup.is_null())
      {

         UpdateHover();
         pobj->m_bRet = true;


         index iItemEnter;
         index iSubItemEnter;
         point point;
         //Session.get_cursor_pos(&point);
         //ScreenToClient(&point);

         if (_001DisplayHitTest(pt, iItemEnter, iSubItemEnter))
         {
            if (m_bSelect && m_bHoverSelect &&
               (m_iSubItemEnter != iSubItemEnter ||
                  m_iItemEnter != iItemEnter)
               && !m_rangeSelection.has_item(iItemEnter))
            {
               m_iMouseFlagEnter = pmouse->m_nFlags;
               m_iItemEnter = iItemEnter;
               m_iSubItemEnter = iSubItemEnter;
               //SetTimer(12321, 840, NULL);
               SetTimer(12321, 184 + 177 + 151, NULL);
               //track_mouse_hover();
            }
         }

      }


   }

   DWORD g_dwStartLDown;

   void mesh::_001OnLButtonDown(signal_details * pobj)
   {
      
      SCAST_PTR(::message::mouse,pmouse,pobj);
      
      pmouse->previous(); // give chance to child control and to base views

      m_bLButtonDown = true;

      SetCapture();

      int_ptr iItem;
      
      point pt = pmouse->m_pt;
      
      ScreenToClient(&pt);

      m_ptLButtonDown = pt;

      m_dwLButtonDownStart = get_tick_count();

      if (!has_focus())
      {
         
         SetFocus();

      }

      Session.set_keyboard_focus(this);

      Session.user()->set_mouse_focus_LButtonDown(this);

      if (!_001DisplayHitTest(pt, m_iDisplayItemLButtonDown))
      {

         m_iDisplayItemLButtonDown = -1;

      }

      if(m_bSelect)
      {

         if(m_bHoverSelect)
         {

            if(_001DisplayHitTest(pt,iItem))
            {

               // In "Hover Select"/"Single Click to Open" mode
               // the "Last Button UP flag" from a double-click
               // doesn't exist! So cannot "magically" take a
               // possibly quite old or inexisting "Last Button UP flag" (filled
               // with garbage in this later inexisting case),
               // and use as parameter for _001OnClick.
               // And if it is actually from a double-click,
               // because "Hover Select"/"Single Click to Open"
               // mode is active, the "Last Button UP flag"
               // shouldn't be regarded, as it is not directly
               // related with further click other than the
               // current click, i.e., there is no interpretation
               // for double-click in "Hover Select"/"Single Click to Open" 
               // mode, and every click must be regarded as single
               // independent click.
               // Action. Remove.
               // uint_ptr nFlags = m_uiLButtonUpFlags;

               // uint_ptr nFlags = 0;

               // point point = pt;

               // _001OnClick(nFlags, point);

               // Redraw();

               // m_iClick = 0;

            }
            else
            {

               m_rangeSelection.clear();
            }
         }
         else
         {
            if(m_bMultiSelect && Session.is_key_pressed(::user::key_shift))
            {
               if(_001DisplayHitTest(pt,iItem))
               {
                  item_range itemrange;
                  int_ptr iLItem = MIN(m_iShiftFirstSelection,iItem);
                  int_ptr iUItem = MAX(m_iShiftFirstSelection,iItem);
                  itemrange.set(iLItem,iUItem,0,m_nColumnCount - 1,- 1,-1);
                  _001AddSelection(itemrange);
                  m_iShiftFirstSelection = iItem;
               }
            }
            else if(m_bMultiSelect && Session.is_key_pressed(::user::key_control))
            {
               if(_001DisplayHitTest(pt,iItem))
               {
                  item_range itemrange;
                  int_ptr iLItem = MIN(m_iShiftFirstSelection,iItem);
                  int_ptr iUItem = MAX(m_iShiftFirstSelection,iItem);
                  itemrange.set(iLItem,iUItem,0,m_nColumnCount - 1,- 1,-1);
                  _001AddSelection(itemrange);
                  m_iShiftFirstSelection = iItem;
               }
            }
            else
            {
               m_rangeSelection.clear();
               index iItem;
               if(_001DisplayHitTest(pt,iItem))
               {
                  g_dwStartLDown = get_tick_count();
                  m_iShiftFirstSelection = iItem;
                  m_iItemFocus = iItem;
                  _001DisplayHitTest(pt,m_iItemDrag);
                  m_iItemDrop = m_iItemDrag;
                  m_uiLButtonDownFlags = pmouse->m_nFlags;
                  m_ptLButtonDown = pt;
                  SetTimer(12345678,800,NULL);
                  item_range itemrange;
                  itemrange.set(iItem,iItem,0,m_nColumnCount - 1,- 1,-1);
                  _001AddSelection(itemrange);
               }
            }
         }
      }

      RedrawWindow();
      pobj->m_bRet = true;
      pmouse->set_lresult(1);
   }


   void mesh::_001OnLButtonUp(signal_details * pobj)
   {

      SCAST_PTR(::message::mouse,pmouse,pobj);
      
      point pt = pmouse->m_pt;

      ScreenToClient(&pt);

      ReleaseCapture();

      if (m_bDrag)
      {

         m_bDrag = false;

         if (m_eview != view_icon)
         {

            index iItemOld = m_iItemDrop;
            if (_001DisplayHitTest(pt, m_iItemDrop))
            {
               if (m_iItemDrag != m_iItemDrop && m_iItemDrop != -1)
               {
                  // swap
                  index i = m_meshlayout.m_iaDisplayToStrict[m_iItemDrag];
                  m_meshlayout.m_iaDisplayToStrict[m_iItemDrag] = m_meshlayout.m_iaDisplayToStrict[m_iItemDrop];
                  m_meshlayout.m_iaDisplayToStrict[m_iItemDrop] = i;
                  _001OnAfterSort();
               }
            }
         }

      }
      else if (m_bHoverSelect || (get_tick_count() - g_dwStartLDown > 800))
      {

         if (m_bLButtonDown)
         {

            index iDisplayItemLButtonUp = -1;

            if (_001DisplayHitTest(pt, iDisplayItemLButtonUp) && iDisplayItemLButtonUp >= 0)
            {

               if (iDisplayItemLButtonUp == m_iDisplayItemLButtonDown)
               {

                  if (!m_rangeSelection.has_item(iDisplayItemLButtonUp))
                  {

                     m_rangeSelection.clear();

                     item_range itemrange;

                     itemrange.set(iDisplayItemLButtonUp, iDisplayItemLButtonUp, 0, m_nColumnCount - 1, -1, -1);

                     _001AddSelection(itemrange);

                  }

                  _001OnClick(pmouse->m_nFlags, pt);

               }


            }

         }

      }
      else
      {
//         if ()
         {

         }
      }
      //else
      //{
      // commented on 2017-03-28
      //   m_iClick++;
      //   m_uiLButtonUpFlags = (UINT)pmouse->m_nFlags;
      //   m_ptLButtonUp = pt;
      //   SetTimer(12345679, 500, NULL);
      //   KillTimer(12345678);
      //}
      pobj->m_bRet = true;
      pmouse->set_lresult(1);

      m_bLButtonDown = false;


   }

   void mesh::_001OnRButtonDown(signal_details * pobj)
   {

      SCAST_PTR(::message::mouse,pmouse,pobj);

      pmouse->previous();

      point pt = pmouse->m_pt;

      ScreenToClient(&pt);

      if(!has_focus())
      {

         SetFocus();

      }

      if(Session.is_key_pressed(::user::key_shift))
      {

      }
      else if(Session.is_key_pressed(::user::key_control))
      {

      }
      else
      {

         index iItem;
         if(_001DisplayHitTest(pt,iItem))
         {
            if(!m_rangeSelection.has_item(iItem))
            {
               m_rangeSelection.clear();
               m_iShiftFirstSelection = iItem;
               item_range itemrange;
               itemrange.set(iItem,iItem,0,m_nColumnCount - 1,- 1,-1);
               _001AddSelection(itemrange);
               RedrawWindow();
            }
         }
      }


      pobj->m_bRet = true;
   }

   
   void mesh::_001OnRButtonUp(signal_details * pobj)
   {

      SCAST_PTR(::message::mouse, pmouse, pobj);

      pmouse->previous();

      point pt = pmouse->m_pt;

      ScreenToClient(&pt);

      if (!has_focus())
      {

         SetFocus();

      }

      _001OnRightClick(pmouse->m_ulFlags, pt);


      pobj->m_bRet = true;

   }


   bool mesh::_001OnClick(uint_ptr nFlag,point point)
   {
      UNREFERENCED_PARAMETER(nFlag);
      UNREFERENCED_PARAMETER(point);
      ::user::control_event ev;
      ev.m_puie = this;
      ev.m_eevent = ::user::event_list_clicked;
      if(m_pformcallback != NULL)
      {
         m_pformcallback->BaseOnControlEvent(NULL,&ev);
      }
      else if(get_form() != NULL)
      {
         get_form()->send_message(
            ::message::message_event,0,(LPARAM)&ev);
      }
      else
      {
         GetParent()->send_message(
            ::message::message_event,0,(LPARAM)&ev);
      }
      return true;
   }


   bool mesh::_001OnRightClick(uint_ptr nFlag,point point)
   {

      UNREFERENCED_PARAMETER(nFlag);
      UNREFERENCED_PARAMETER(point);
      return false;

   }


   void mesh::range::clear()
   {
      m_itemrangea.remove_all();
   }

   void mesh::range::add_item(const item_range & itemrange)
   {
      m_itemrangea.add(itemrange);
   }

   mesh::item_range::item_range()
   {
      m_iLowerBound = -1;
      m_iUpperBound = -1;
   }

   index mesh::item_range::get_lower_bound() const
   {
      return m_iLowerBound;
   }

   index mesh::item_range::get_upper_bound() const
   {
      return m_iUpperBound;
   }



   mesh::item_range::item_range(const item_range & itemrange)
   {
      operator =(itemrange);
   }

   mesh::item_range & mesh::item_range::operator =(const item_range & itemrange)
   {
      m_iLowerBound         = itemrange.m_iLowerBound;
      m_iUpperBound         = itemrange.m_iUpperBound;
      m_subitemrange    = itemrange.m_subitemrange;
      return *this;
   }



   mesh::sub_item_range::sub_item_range()
   {
      m_iLowerBound = -1;
      m_iUpperBound = -1;
   }

   mesh::sub_item_range::sub_item_range(const sub_item_range & subitemrange)
   {
      operator =(subitemrange);
   }

   mesh::sub_item_range & mesh::sub_item_range::operator =(const sub_item_range & subitemrange)
   {
      m_iLowerBound         = subitemrange.m_iLowerBound;
      m_iUpperBound         = subitemrange.m_iUpperBound;
      m_meshitemrange   = subitemrange.m_meshitemrange;
      return *this;
   }

   bool mesh::sub_item_range::has_sub_item(index iSubItem) const
   {
      if(m_iLowerBound == -1 || m_iUpperBound == -1)
         return false;
      if(iSubItem >= m_iLowerBound
         && iSubItem <= m_iUpperBound)
         return true;
      else
         return false;
   }


   mesh::range & mesh::range::operator = (const range & range)
   {
      m_itemrangea.copy(range.m_itemrangea);
      return *this;
   }

   void mesh::_001GetSelection(range &range)
   {
      range = m_rangeSelection;
   }

   void mesh::_001GetSelection(
      ::database::id & key,
      ::database::selection &selection)
   {
      if(!_001HasConfigId(key))
         return;
      int_ptr iFilterSubItem = _001ConfigIdToColumnKey(key);
      range & range = m_rangeSelection;
      for(index i = 0; i < range.get_item_count(); i++)
      {
         item_range & itemrange = range.ItemAt(i);
         if(itemrange.has_sub_item(iFilterSubItem))
         {
            for(index iLine = itemrange.get_lower_bound(); iLine <= itemrange.get_upper_bound(); iLine++)
            {
               selection.add_item(key.m_id + "." + ::str::from(iLine));
            }
         }
      }
   }


   /*index mesh::range::get_item(index iItemIndex)
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

   /*index mesh::item_range::get_count()
   {
   return m_iItemEnd - m_iItemStart + 1;
   }*/

   /*index mesh::item_range::get_item(index iItemIndex)
   {
   return m_iItemStart + iItemIndex;
   }*/

   ::count mesh::range::get_item_count() const
   {
      return m_itemrangea.get_size();
      /*   index iCount = 0;

      for(index i = 0; i < m_itemrangea.get_size(); i++)
      {
      item_range & itemrange = m_itemrangea[i];
      iCount += itemrange.get_count();
      }

      return iCount;*/
   }

   ::count mesh::range::get_items(index_array & ia) const
   {
      return get_item_indexes(ia);
      /*   index iCount = 0;

      for(index i = 0; i < m_itemrangea.get_size(); i++)
      {
      item_range & itemrange = m_itemrangea[i];
      iCount += itemrange.get_count();
      }

      return iCount;*/
   }

   mesh::item_range & mesh::range::ItemAt(index iItem)
   {
      return m_itemrangea.element_at(iItem);
   }

   ::count mesh::_001GetSelectedItemCount()
   {
      return m_rangeSelection.get_item_count();
   }

   id mesh::_001GetColumnTextId(index iColumn)
   {
      UNREFERENCED_PARAMETER(iColumn);
      return -1;
   }

   void mesh::_001OnLButtonDblClk(signal_details * pobj)
   {
      SCAST_PTR(::message::mouse,pmouse,pobj);
         m_iClick = 2;
      point pt = pmouse->m_pt;
      ScreenToClient(&pt);

      _001OnClick(pmouse->m_nFlags,pt);


      //   _001OnClick(nFlags, point);
      Redraw();


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

      if(lresult == 0)
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

   ///*void mesh::HeaderCtrlLayout()
   //{
   //   m_pmeshheader->SetWindowPos(
   //      ZORDER_TOP,
   //      -ptScroll.x,
   //      0,
   //      0,0,
   //      SWP_NOSIZE
   //      | SWP_SHOWWINDOW);
   //*/}

   //index mesh::HeaderCtrlMapColumnToOrder(index iColumn)
   //{

   //   if(m_pmeshheader == NULL)
   //      return iColumn;

   //   return m_pmeshheader->MapItemToOrder(iColumn);

   //}

   //bool mesh::_001OnHeaderCtrlEndDrag(WPARAM wparam,LPARAM lparam)
   //{
   //   UNREFERENCED_PARAMETER(wparam);
   //   UNREFERENCED_PARAMETER(lparam);
   //   for(index iColumn = 0; iColumn < m_columna.VisibleGetCount(); iColumn++)
   //   {
   //      mesh_column * pcolumn = m_columna._001GetVisible(iColumn);
   //      pcolumn->m_iOrder = HeaderCtrlMapColumnToOrder(iColumn);
   //   }

   //   _001OnColumnChange();

   //   DISaveOrder();

   //   Redraw();

   //   return true;

   //}

   ///*LRESULT mesh::OnEndColumnHeaderDragMessage(WPARAM wparam, LPARAM lparam)
   //{
   //HeaderCtrlOnEndDrag();
   //return 0;
   //}*/

   /*LRESULT mesh::OnEndColumnHeaderTrackMessage(WPARAM wparam, LPARAM lparam)
   {
   HeaderCtrlOnEndTrack();
   return 0;
   }*/

   /*LRESULT mesh::OnColumnHeaderTrackMessage(WPARAM wparam, LPARAM lparam)
   {
   HeaderCtrlOnTrack();
   return 0;
   }*/

   //bool mesh::_001OnHeaderCtrlEndTrack(WPARAM wparam,LPARAM lparam)
   //{
   //   UNREFERENCED_PARAMETER(wparam);
   //   UNREFERENCED_PARAMETER(lparam);
   //   _001OnHeaderCtrlTrack(0,0);

   //   string str;
   //   index i;
   //   index width;

   //   for(i = 0; i < m_nColumnCount; i++)
   //   {
   //      str.Format("mesh_column[%d].width",i);
   //      width = m_columna.element_at(i)->m_iWidth;
   //      data_set(str,width);
   //   }

   //   return true;
   //}

   //bool mesh::_001OnHeaderCtrlTrack(WPARAM wparam,LPARAM lparam)
   //{
   //   UNREFERENCED_PARAMETER(wparam);
   //   UNREFERENCED_PARAMETER(lparam);
   //   //    for(index iColumn = 0; iColumn < m_columna.VisibleGetCount(); iColumn++)
   //   //  {
   //   //         mesh_column & column = m_columna._001GetVisible(iColumn);
   //   //column.m_iWidth = m_pmeshheader->GetItemWidth(iColumn);
   //   //}

   //   _001OnColumnChange();

   //   Redraw();

   //   return true;
   //}

   //void mesh::_001ShowSubItem(index iSubItem,bool bShow)
   //{
   //   string str;
   //   str.Format("SubItem[%d].Visible",iSubItem);
   //   data_set(str,bShow ? 1 : 0);
   //   m_columna.ShowSubItem(iSubItem,bShow);
   //   _001OnColumnChange();
   //   Redraw();

   //}

   //mesh_column * mesh_column_array::_001GetByKey(index iKey)
   //{

   //   for(index i = 0; i < this->get_size(); i++)
   //   {
   //      mesh_column * pcolumn = element_at(i);
   //      if(pcolumn->m_iKey == iKey)
   //      {
   //         return pcolumn;
   //      }
   //   }

   //   return NULL;

   //}

   //mesh_column * mesh_column_array::_001GetBySubItem(index iSubItem)
   //{
   //   for(index i = 0; i < this->get_size(); i++)
   //   {
   //      mesh_column * pcolumn = element_at(i);
   //      if(pcolumn == NULL)
   //         continue;
   //      if(pcolumn->m_iSubItem == iSubItem)
   //      {
   //         if(pcolumn->m_bVisible)
   //         {
   //            return pcolumn;
   //         }
   //         else
   //         {
   //            return NULL;
   //         }
   //      }
   //   }
   //   return NULL;

   //}

   //mesh_column * mesh_column_array::GlobalOrderGetPrevious(index iKey)
   //{
   //   for(index i = 0; i < this->get_size(); i++)
   //   {
   //      mesh_column * pcolumn = element_at(i);
   //      if(pcolumn->m_iNextGlobalOrderKey == iKey)
   //      {
   //         return pcolumn;
   //      }
   //   }
   //   return NULL;
   //}

   //mesh_column * mesh_column_array::GlobalOrderGetNext(index iKey)
   //{
   //   mesh_column * pcolumn = _001GetByKey(iKey);
   //   return _001GetByKey(pcolumn->m_iNextGlobalOrderKey);
   //}

   //mesh_column * mesh_column_array::_001GetVisible(index iKeyVisible)
   //{
   //   for(index i = 0; i < this->get_size(); i++)
   //   {
   //      mesh_column * pcolumn = element_at(i);
   //      if(pcolumn->m_iKeyVisible == iKeyVisible)
   //      {
   //         return pcolumn;
   //      }
   //   }
   //   return NULL;

   //}
   //mesh_column * mesh_column_array::_001GetNonVisible(index iKeyNonVisible)
   //{
   //   for(index i = 0; i < this->get_size(); i++)
   //   {
   //      mesh_column * pcolumn = element_at(i);
   //      if(pcolumn->m_iKeyNonVisible == iKeyNonVisible)
   //      {
   //         return pcolumn;
   //      }
   //   }
   //   return NULL;

   //}


   //index mesh_column_array::add(mesh_column &column)
   //{

   //   column.m_iKey = this->get_size();
   //   column.m_iOrder = this->get_size();
   //   column.m_pcontainer = this;

   //   smart_pointer_array < mesh_column >::add(new mesh_column(column));

   //   OnChange();

   //   return smart_pointer_array < mesh_column >::get_upper_bound();

   //}


   //void mesh_column_array::remove_all()
   //{

   //   smart_pointer_array < mesh_column >::remove_all();

   //   OnChange();

   //}

   //::count mesh_column_array::get_count()
   //{
   //   return this->get_size();
   //}

   //mesh_column_array::mesh_column_array(::aura::application * papp):
   //   object(papp)
   //{
   //   m_pmesh = NULL;
   //}

   //void mesh_column_array::Initialize(mesh * pmesh)
   //{
   //   ASSERT(pmesh != NULL);
   //   m_pmesh = pmesh;
   //}

   //void mesh_column_array::OnChange()
   //{
   //   sort::array::quick_sort(*this,mesh_column::CompareKey);

   //   index iKeyVisible = 0;
   //   index iKeyNonVisible = 0;

   //   for(index i = 0; i < this->get_size(); i++)
   //   {
   //      mesh_column * pcolumn = element_at(i);
   //      if(pcolumn->m_bVisible)
   //      {
   //         pcolumn->m_iKeyVisible = iKeyVisible;
   //         pcolumn->m_iKeyNonVisible = -1;
   //         iKeyVisible++;
   //      }
   //      else
   //      {
   //         pcolumn->m_iKeyNonVisible = iKeyNonVisible;
   //         pcolumn->m_iKeyVisible = -1;
   //         iKeyNonVisible++;
   //      }
   //   }

   //   sort::array::quick_sort(*this,mesh_column::CompareOrderSectEndNonVisible);

   //   for(index iOrder = 0; iOrder < this->get_size(); iOrder++)
   //   {
   //      mesh_column & column = *this->element_at(iOrder);
   //      column.m_iOrder = iOrder;
   //   }



   //}

   //void mesh_column_array::remove(index iColumn)
   //{
   //   ASSERT(iColumn >= 0);
   //   ASSERT(iColumn < this->get_size());

   //   remove_at(iColumn);

   //   OnChange();
   //}

   //::count mesh_column_array::VisibleGetCount()
   //{
   //   index iCount = 0;
   //   for(index i = 0; i < this->get_size(); i++)
   //   {
   //      mesh_column * pcolumn = element_at(i);
   //      if(pcolumn->m_bVisible)
   //         iCount++;
   //   }
   //   return iCount;
   //}

   //::count mesh_column_array::NonVisibleGetCount()
   //{
   //   index iCount = 0;
   //   for(index i = 0; i < this->get_size(); i++)
   //   {
   //      mesh_column * pcolumn = element_at(i);
   //      if(!pcolumn->m_bVisible)
   //         iCount++;
   //   }
   //   return iCount;
   //}

   //void mesh_column_array::ShowSubItem(index iSubItem,bool bShow)
   //{
   //   if(bShow)
   //   {
   //      index iColumn = NonVisibleMapSubItemToColumn(iSubItem);
   //      if(iColumn >= 0)
   //      {
   //         mesh_column * pcolumn = _001GetNonVisible(iColumn);
   //         pcolumn->m_bVisible = true;
   //         OnChange();
   //      }
   //   }
   //   else
   //   {
   //      index iColumn = VisibleMapSubItemToColumn(iSubItem);
   //      if(iColumn >= 0)
   //      {
   //         mesh_column * pcolumn = _001GetVisible(iColumn);
   //         pcolumn->m_bVisible = false;
   //         OnChange();
   //      }
   //   }
   //}

   //index mesh_column_array::_001GetSubItemKey(index iSubItem)
   //{
   //   mesh_column * pcolumn = _001GetBySubItem(iSubItem);
   //   if(pcolumn == NULL)
   //      return -1;
   //   return pcolumn->m_iKey;
   //}

   //index mesh_column_array::VisibleMapSubItemToColumn(index iSubItem)
   //{
   //   int32_t iVisible = 0;
   //   for(index iColumn = 0; iColumn < this->get_count(); iColumn++)
   //   {
   //      mesh_column * pcolumn = element_at(iColumn);
   //      if(pcolumn->m_bVisible)
   //      {
   //         if(pcolumn->m_iSubItem == iSubItem)
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

   //index mesh_column_array::NonVisibleMapSubItemToColumn(index iSubItem)
   //{
   //   int32_t iNonVisible = 0;
   //   for(index iColumn = 0; iColumn < this->get_count(); iColumn++)
   //   {
   //      mesh_column * pcolumn = element_at(iColumn);
   //      if(!pcolumn->m_bVisible)
   //      {
   //         if(pcolumn->m_iSubItem == iSubItem)
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



   //void mesh_column_array::GlobalToVisibleOrder()
   //{
   //   int_ptr iVisibleCount = VisibleGetCount();
   //   for(index iVisibleKey = 0; iVisibleKey < iVisibleCount; iVisibleKey++)
   //   {
   //      mesh_column * pcolumn = _001GetVisible(iVisibleKey);
   //      pcolumn->m_iOrder = VisibleGetOrderFromKey(pcolumn->m_iKey);
   //   }


   //}

   //void mesh_column_array::VisibleToGlobalOrder(index iKeyA,index iKeyB)
   //{
   //   if(iKeyA == iKeyB)
   //      return;
   //   mesh_column * columnA = _001GetByKey(iKeyA);
   //   mesh_column * columnAPrevious = GlobalOrderGetPrevious(iKeyA);
   //   mesh_column * columnANext = GlobalOrderGetNext(iKeyA);
   //   mesh_column * columnB = _001GetByKey(iKeyB);
   //   mesh_column * columnBPrevious = GlobalOrderGetPrevious(iKeyB);
   //   mesh_column * columnBNext = GlobalOrderGetNext(iKeyB);

   //   if(columnA == NULL)
   //      return;

   //   if(columnB == NULL)
   //      return;

   //   if(columnAPrevious == NULL)
   //   {
   //      m_iFirstGlobalOrderKey = iKeyB;
   //   }
   //   else
   //   {
   //      if(columnAPrevious->m_iKey != iKeyB)
   //      {
   //         columnAPrevious->m_iNextGlobalOrderKey = iKeyB;
   //      }
   //   }


   //   if(columnBPrevious == NULL)
   //   {
   //      m_iFirstGlobalOrderKey = iKeyA;
   //   }
   //   else
   //   {
   //      if(columnBPrevious->m_iKey != iKeyA)
   //      {
   //         columnBPrevious->m_iNextGlobalOrderKey = iKeyA;
   //      }
   //   }

   //   if(columnANext == NULL)
   //   {
   //      columnB->m_iNextGlobalOrderKey = -1;
   //   }
   //   else
   //   {
   //      if(columnANext->m_iKey == columnB->m_iKey)
   //      {
   //         columnB->m_iNextGlobalOrderKey = iKeyA;
   //      }
   //      else
   //      {
   //         columnB->m_iNextGlobalOrderKey = columnANext->m_iKey;
   //      }
   //   }

   //   if(columnBNext == NULL)
   //   {
   //      columnA->m_iNextGlobalOrderKey = -1;
   //   }
   //   else
   //   {
   //      if(columnBNext->m_iKey == columnA->m_iKey)
   //      {
   //         columnB->m_iNextGlobalOrderKey = iKeyB;
   //      }
   //      else
   //      {
   //         columnA->m_iNextGlobalOrderKey = columnBNext->m_iKey;
   //      }
   //   }



   //}

   //void mesh_column_array::VisibleToGlobalOrder()
   //{
   //   mesh * pmesh = m_pmesh;
   //   //detects change
   //   int_ptr iVisibleCount = VisibleGetCount();
   //   int_ptr iChangeCount = 0;
   //   int_ptr iNew = 0;
   //   int_ptr iOld = 0;
   //   for(int_ptr iVisibleKey = 0; iVisibleKey < iVisibleCount; iVisibleKey++)
   //   {
   //      mesh_column * column = _001GetVisible(iVisibleKey);
   //      if(VisibleGetOrderFromKey(column->m_iKey) != pmesh->HeaderCtrlMapColumnToOrder(iVisibleKey))
   //      {
   //         iChangeCount++;
   //         if(iChangeCount == 1)
   //         {
   //            iNew = iVisibleKey;
   //         }
   //         else if(iChangeCount == 2)
   //         {
   //            iOld = iVisibleKey;
   //         }
   //      }
   //   }
   //   if(iChangeCount == 2)
   //   {
   //      VisibleToGlobalOrder(iNew,iOld);
   //   }
   //   else
   //   {
   //      if(this->get_size() == VisibleGetCount())
   //      {
   //         for(index iColumn = 0; iColumn < VisibleGetCount(); iColumn++)
   //         {
   //            mesh_column * column = _001GetVisible(iColumn);
   //            column->m_iOrder = pmesh->HeaderCtrlMapColumnToOrder(iColumn);
   //         }

   //         m_iFirstGlobalOrderKey = OrderToKey(0);
   //         if(m_iFirstGlobalOrderKey >= 0)
   //         {
   //            index iKey = m_iFirstGlobalOrderKey;
   //            index iNextKey;
   //            index iOrder = 1;

   //            while(true)
   //            {
   //               iNextKey = OrderToKey(iOrder);
   //               _001GetByKey(iKey)->m_iNextGlobalOrderKey = iNextKey;
   //               if(iNextKey < 0)
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

   //void mesh_column_array::DISaveOrder()
   //{
   //   int_ptr iCount = this->get_size();
   //   string str;
   //   for(index iKey = 0; iKey < iCount; iKey++)
   //   {
   //      mesh_column * column = _001GetByKey(iKey);
   //      str.Format("mesh_column[%d].Next",iKey);
   //      m_pmesh->data_set(
   //         str,
   //         column->m_iNextGlobalOrderKey);
   //   }
   //   str.Format("mesh_column[-1].Next");
   //   m_pmesh->data_set(
   //      str,
   //      m_iFirstGlobalOrderKey);


   //}

   //void mesh_column_array::DILoadOrder()
   //{
   //   int_ptr iCount = this->get_size();
   //   string str;
   //   for(index iKey = 0; iKey < iCount; iKey++)
   //   {
   //      mesh_column * column = _001GetByKey(iKey);
   //      str.Format("mesh_column[%d].Next",iKey);
   //      m_pmesh->data_get(
   //         str,
   //         column->m_iNextGlobalOrderKey);
   //   }
   //   str.Format("mesh_column[-1].Next");
   //   m_pmesh->data_get(
   //      str,
   //      m_iFirstGlobalOrderKey);

   //   GlobalToVisibleOrder();
   //   OnChange();

   //}

   //index mesh_column_array::VisibleGetOrderFromKey(index iKey)
   //{
   //   index iNextKey = m_iFirstGlobalOrderKey;
   //   index iOrder = 0;
   //   while(true)
   //   {
   //      mesh_column * column = _001GetByKey(iNextKey);
   //      if(column == NULL)
   //         return -1;
   //      if(column->m_bVisible)
   //      {
   //         if(iKey == iNextKey)
   //            return iOrder;
   //         iOrder++;
   //      }
   //      else
   //      {
   //         if(iKey == iNextKey)
   //            return -1;
   //      }
   //      if(iOrder > VisibleGetCount())
   //         return -1;
   //      iNextKey = column->m_iNextGlobalOrderKey;
   //   }
   //   ASSERT(FALSE);
   //   return -1;
   //}

   //index mesh_column_array::OrderToKey(index iOrder)
   //{
   //   for(index iKey = 0; iKey < this->get_count(); iKey++)
   //   {
   //      mesh_column * column = _001GetByKey(iKey);
   //      if(column->m_iOrder == iOrder)
   //         return iKey;
   //   }
   //   return -1;

   //   /*   index iNextKey = m_iFirstGlobalOrderKey;
   //   index iOrder = 0;
   //   while(true)
   //   {
   //   mesh_column & column = get(iNextKey);
   //   if(column.IsNull())
   //   return -1;
   //   if(column.IsVisible())
   //   {
   //   if(iOrderParam == iOrder)
   //   {
   //   return iNextKey;
   //   }
   //   iOrder++;
   //   }
   //   else
   //   {
   //   if(iOrderParam == iOrder)
   //   {
   //   return -1;
   //   }
   //   }
   //   if(iOrder > VisibleGetCount())
   //   return -1;
   //   iNextKey = column.m_iNextGlobalOrderKey;
   //   }
   //   ASSERT(FALSE);
   //   return -1;*/
   //}

   void mesh::DISaveOrder()
   {
      //m_columna.VisibleToGlobalOrder();
      //m_columna.DISaveOrder();

   }

   void mesh::DILoadOrder()
   {
      //m_columna.DILoadOrder();
      //_001OnColumnChange();
   }









   //void mesh::_001UpdateColumns()
   //{
   //   _001RemoveAllColumns();
   //   keep < bool > keepLockViewUpdate(&m_bLockViewUpdate,true,false,true);
   //   _001InsertColumns();
   //   keepLockViewUpdate.KeepAway();
   //   DIDDXHeaderLayout(false);
   //   _001OnColumnChange();
   //   on_layout();
   //   RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
   //}

   //void mesh::_001InsertColumns()
   //{


   //}

   //void mesh::_001RemoveAllColumns()
   //{
   //   m_columna.remove_all();
   //   _001OnColumnChange();
   //}




//   bool mesh::TwiHasTranslucency()
//   {
////      return !m_scrolldata.m_bVScroll;
//   }

   //void mesh::_001SetBackBuffer(visual::CBuffer *ptwb)
   //{
   //   UNREFERENCED_PARAMETER(ptwb);
   //   //   m_pgdibuffer = ptwb;
   //}


   void mesh::PreSubClassWindow()
   {
      //   ::user::interaction_base::PreSubClassWindow();
      if(IsWindowVisible())
      {
         RedrawWindow(NULL,NULL,RDW_INVALIDATE);
      }
   }

   void mesh::_001OnCreate(signal_details * pobj)
   {

      SCAST_PTR(::message::create,pcreate,pobj);


      pobj->previous();


      on_create_draw_item();


      if (m_eview == ::user::mesh::view_icon)
      {

         m_pdrawmeshitem->m_iDrawTextFlags = DT_TOP | DT_CENTER | DT_END_ELLIPSIS | DT_WORDBREAK | DT_NOPREFIX;

      }
      else
      {

         m_pdrawmeshitem->m_iDrawTextFlags = DT_VCENTER | DT_LEFT | DT_END_ELLIPSIS | DT_SINGLELINE | DT_NOPREFIX;

      }


      //m_font->operator=(*System.visual().fonts().GetMeshCtrlFont());

      //m_fontHover->operator=(*System.visual().fonts().GetMeshCtrlFont());

      //m_fontHover->set_underline();
      ////m_fontHover->set_bold();

      if(pcreate->get_lresult() == -1)
      {
         pobj->m_bRet = false;
         return;
      }


      //if(m_bAutoCreateMeshHeader)
      //{

      //   if(m_pmeshheader.is_null())
      //   {

      //      m_pmeshheader = create_mesh_header();

      //      if(m_pmeshheader.is_set())
      //      {

      //         m_pmeshheader->SetBaseMeshCtrlInterface(this);

      //      }

      //   }

      //   if(m_pmeshheader != NULL)
      //   {

      //      if(!CreateHeaderCtrl())
      //      {
      //         pcreate->set_lresult(-1);
      //         pobj->m_bRet = true;
      //         return;
      //      }
      //   }

      //}

      if(m_pmeshdata.is_null() && m_bAutoCreateMeshData)
      {

         defer_create_mesh_data();

      }

      class rect rect;

      rect.null();

      //on_layout();

      pcreate->set_lresult(0);

      if(IsWindowVisible())
      {
         RedrawWindow();
      }

      pobj->m_bRet = false;

   }


   //void mesh::_001CreateImageMesh(mesh_column * pcolumn)
   //{

   //   if(pcolumn == NULL)
   //      return;

   //   if(pcolumn->m_uiSmallBitmap == (UINT)-1)
   //   {
   //      return;
   //   }
   //   if(pcolumn->m_pil == NULL)
   //   {
   //      pcolumn->m_pil = new image_list(get_app());
   //   }
   //   //      sp(image_list) pil = column.m_pil;
   //   //   if(pil != NULL)
   //   //      pil->DeleteImageMesh();
   //   ::exception::throw_not_implemented(get_app());
   //   /*if(!pil->create(
   //   MAKEINTRESOURCE(column.m_uiSmallBitmap),
   //   column.m_iSmallImageWidth,
   //   0,
   //   column.m_crSmallMask))
   //   {
   //   }*/
   //}


   void mesh::_001OnTimer(::timer * ptimer)
   {
      //      return; //xxxtimer
      ::user::control::_001OnTimer(ptimer);
      if(ptimer->m_nIDEvent == 12345679) // left click
      {
         KillTimer(12345679);
         if(m_bSelect)
         {
            if(m_bHoverSelect)
            {
            }

         }
      }
      else if(ptimer->m_nIDEvent == 8477) // right click
      {
         KillTimer(8477);
         //if(!_001IsEditing())
         {
            uint_ptr nFlags = m_uiRButtonUpFlags;
            point point = m_ptRButtonUp;
            _001OnRightClick(nFlags,point);
            Redraw();


            /* trans
            window_id wndidNotify = pwnd->GetOwner()->GetSafeoswindow_();
            if(wndidNotify == NULL)
            wndidNotify = pwnd->GetParent()->GetSafeoswindow_(); */

            //            LRESULT lresult = 0;

            /* trans            if(wndidNotify)
            {
            NMLISTVIEW nm;
            nm.hdr.idFrom = pwnd->GetDlgCtrlId();
            nm.hdr.code =   NM_CLICK;
            nm.hdr.oswindowFrom = pwnd->GetSafeoswindow_();
            lresult = ::SendMessage(
            wndidNotify,
            WM_NOTIFY,
            nm.hdr.idFrom,
            (LPARAM) &nm);
            }*/
         }
      }
      else if(ptimer->m_nIDEvent == 0xfffffffe)
      {
         if(!Filter1Step())
            KillTimer(ptimer->m_nIDEvent);
      }
      else if (ptimer->m_nIDEvent == 224455)
      {

         KillTimer(ptimer->m_nIDEvent);
         
         m_iItemDrop = -1;

         if (m_eview == view_icon)
         {

            m_iStrictItemDrag = m_iconlayout.m_iaDisplayToStrict.get_b(m_iItemMouseDown);

            if (m_iStrictItemDrag >= 0)
            {

               //if (iItemOld >= 0)
               {

                  //m_iconlayout.m_iaDisplayToStrict.remove_a(m_iItemMouseDown);

               }

               m_iItemDrag = m_iItemMouseDown;

               m_iItemDrop = m_iItemDrag;

               m_bDrag = true;

            }
            else
            {

               m_iItemMouseDown = -1;

               m_iItemDrag = -1;

               m_iItemDrop = -1;

               m_bDrag = true;

            }

         }
         else
         {
            
            m_iStrictItemDrag = m_meshlayout.m_iaDisplayToStrict.find_first(m_iItemDrag);

            if (m_iStrictItemDrag >= 0)
            {

               m_meshlayout.m_iaDisplayToStrict.remove_at(m_iStrictItemDrag);

               m_iItemDrop = -1;

               m_iItemDrag = m_iItemMouseDown;

               m_bDrag = true;

            }
            
         }

      }
      else if(ptimer->m_nIDEvent == 12345678)
      {
         KillTimer(ptimer->m_nIDEvent);
         if(!m_bHoverSelect)
         {
         }
         else
         {
            m_bDrag = true;
         }
      }
      else if(ptimer->m_nIDEvent == 12321)
      {


         KillTimer(ptimer->m_nIDEvent);


         index iItemSel;
         index iSubItemSel;
         point point;
         Session.get_cursor_pos(&point);
         ScreenToClient(&point);
         try
         {
            if(_001DisplayHitTest(point,iItemSel,iSubItemSel))
            {
               if(m_iSubItemEnter == iSubItemSel &&
                  m_iItemEnter == iItemSel)
               {
                  m_iSubItemEnter = -1;
                  m_iItemEnter = -1;
                  bool bLShiftKeyDown     = Session.is_key_pressed(::user::key_lshift);
                  bool bRShiftKeyDown     = Session.is_key_pressed(::user::key_rshift);
                  bool bLControlKeyDown   = Session.is_key_pressed(::user::key_lcontrol);
                  bool bRControlKeyDown   = Session.is_key_pressed(::user::key_rcontrol);
                  bool bShiftKeyDown      = bLShiftKeyDown || bRShiftKeyDown;
                  bool bControlKeyDown    = bLControlKeyDown || bRControlKeyDown;

                  if(m_bMultiSelect && bShiftKeyDown)
                  {
                     if(bControlKeyDown)
                     {
                        item_range itemrange;
                        itemrange.set(
                           MIN(iItemSel,m_iItemSel),
                           MAX(iItemSel,m_iItemSel),
                           MIN(iSubItemSel,m_iSubItemSel),
                           MAX(iSubItemSel,m_iSubItemSel),
                           -1,
                           -1);
                        _001AddSelection(itemrange);
                     }
                     else
                     {
                        item_range itemrange;
                        itemrange.set(
                           MIN(iItemSel,m_iItemSel),
                           MAX(iItemSel,m_iItemSel),
                           MIN(iSubItemSel,m_iSubItemSel),
                           MAX(iSubItemSel,m_iSubItemSel),
                           -1,
                           -1);
                        range range;
                        range.add_item(itemrange);
                        _001SetSelection(range);
                     }
                  }
                  else if(m_bMultiSelect && bControlKeyDown)
                  {
                     m_iLastItemSel = m_iItemSel;
                     m_iLastSubItemSel = m_iSubItemSel;
                     m_iItemSel = iItemSel;
                     m_iSubItemSel = iSubItemSel;
                     item_range itemrange;
                     itemrange.set(
                        m_iItemSel,
                        m_iItemSel,
                        m_iSubItemSel,
                        m_iSubItemSel,
                        -1,
                        -1);
                     _001AddSelection(itemrange);
                  }
                  else
                  {
                     m_iLastItemSel = m_iItemSel;
                     m_iLastSubItemSel = m_iSubItemSel;
                     m_iItemSel = iItemSel;
                     m_iSubItemSel = iSubItemSel;
                     item_range itemrange;
                     itemrange.set(
                        DisplayToStrict(m_iItemSel),
                        DisplayToStrict(m_iItemSel),
                        m_iSubItemSel,
                        m_iSubItemSel,
                        -1,
                        -1);
                     range range;
                     range.add_item(itemrange);
                     _001SetSelection(range);
                  }
               }
            }
         }
         catch(...)
         {
         }
         m_iSubItemEnter = -1;
         m_iItemEnter = -1;

      }

//      ptimer->m_bRet = false;
   }

   bool mesh::_001IsItemVisible(index iItem)
   {
      draw_mesh_item item(this);
      item.m_iItem         = iItem;
      item.m_iDisplayItem  = iItem;
      _001GetItemRect(&item);
      if(!item.m_bOk)
      {
         return false;
      }
      rect rectClient;
      GetClientRect(&rectClient);
      return rectClient.intersect(rectClient,item.m_rectItem) != 0;
   }

   void mesh::_001ClearSelection()
   {
      m_rangeSelection.clear();
      _001OnSelectionChange();
   }

   void mesh::_001SetSelection(const range &range)
   {
      m_rangeSelection = range;
      on_select();
      _001OnSelectionChange();
   }

   void mesh::_001AddSelection(const item_range & itemrange)
   {
      m_rangeSelection.add_item(itemrange);
      on_select();
      _001OnSelectionChange();
   }

   void mesh::_001SetHighlightRange(range & range)
   {
      m_rangeHighlight = range;
   }

   void mesh::DIOnSectionSet()
   {
      string str;
      str = m_dataid.get_id();
      str += ".headerctrl";
      //if(m_pmeshheader != NULL)
      //{
      //   m_pmeshheader->m_dataid = str;
      //}
   }

   bool mesh::DIDDXHeaderLayout(bool bSave)
   {

      //if(m_pmeshheader == NULL)
      //   return false;

      //return m_pmeshheader->DIDDXLayout(bSave);
      return false;

   }

   void mesh::_001SetTopText(const unichar * lpcwsz)
   {
      m_strTopText = lpcwsz;
      _001LayoutTopText();
   }

   void mesh::_001LayoutTopText()
   {
      ::draw2d::memory_graphics pgraphics(allocer());
      select_font(pgraphics, font_list_item, this);
      array < size > sizea;
      m_dcextension.GetTextExtent(pgraphics,m_strTopText,sizea);
      rect rectClient;
      GetClientRect(rectClient);
      index x = 0;
      index right = rectClient.right;
      index y = 0;
      for(index i = 0; i < sizea.get_size(); i++)
      {
         if((sizea[i].cx - x > right)
            || i == sizea.get_upper_bound())
         {
            if(i == 0)
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

   void mesh::_001ShowTopText(bool bShow)
   {
      m_bTopText = bShow;
   }




   void mesh::SetCacheInterface(mesh_cache_interface * pinterface)
   {
      m_pcache = pinterface;
   }

   void mesh::CacheHint()
   {
      if(m_pcache != NULL)
      {
         int_ptr iItemCount = m_nDisplayCount;
         int_ptr iItemFirst = m_iTopIndex;
         m_pcache->_001CacheHint(
            this,
            iItemFirst,
            iItemCount);
      }

   }


   void mesh::SetDataInterface(::user::mesh_data *pinterface)
   {

      m_pmeshdata = pinterface;

      m_psimplemeshdata = m_pmeshdata;

   }


   //bool mesh::_001InsertColumn(mesh_column &column)
   //{

   //   _001AddColumn(column);
   //   _001OnColumnChange();
   //   return true;
   //}

   //void mesh::_001SetSingleColumnMode(bool bHeaderCtrl)
   //{

   //   //m_bSingleColumnMode = true;

   //   //m_bHeaderCtrl = bHeaderCtrl;
   //   //_001RemoveAllColumns();

   //   //mesh_column column;
   //   //column.m_iWidth = -1;
   //   //column.m_iSubItem = 0;
   //   //column.m_iSmallImageWidth = 16;
   //   //column.m_crSmallMask = ARGB(255,255,0,255);


   //   //_001InsertColumn(column);

   //   on_layout();



   //}

   int32_t mesh::_001CalcItemWidth(index iItem,index iSubItem)
   {
      ::draw2d::memory_graphics pgraphics(allocer());
      select_font(pgraphics, font_list_item, this);
      index cx = _001CalcItemWidth(pgraphics,iItem,iSubItem);

      return (int32_t)cx;

   }

   //int32_t mesh::_001CalcItemWidth(::draw2d::graphics * pgraphics,::draw2d::font * pfont,index iItem,index iSubItem)
   //{
   //   pgraphics->SelectObject(pfont);
   //   return _001CalcItemWidth(pgraphics,iItem,iSubItem);
   //}

   int32_t mesh::_001CalcItemWidth(::draw2d::graphics * pgraphics,index iItem,index iSubItem)
   {
#ifdef WINDOWSEX
      select_font(pgraphics, font_list_item, this);
      ::image_list::info ii;
      rect rect;
      size size;
      index cx = 0;
      //mesh_column * pcolumn = m_columna._001GetByKey(iSubItem);
      draw_mesh_item item(this);
      item.m_iItem = iItem;
      item.m_iSubItem = iSubItem;
      item.m_iListItem = -1;
      //if(pcolumn->m_pil != NULL)
      //{
      //   _001GetItemImage(&item);
      //   if(item.m_bOk && item.m_iImage >= 0)
      //   {
      //      pcolumn->m_pil->get_image_info((int32_t)item.m_iImage,&ii);
      //      rect = ii.m_rect;
      //      cx += rect.width();
      //      cx += 2;
      //   }
      //}
      _001GetItemText(&item);
      if(item.m_bOk)
      {
         m_dcextension.GetTextExtent(pgraphics,item.m_strText,size);
         cx += size.cx;
      }

      return (int32_t)cx;
#else
      throw todo(get_app());
#endif
   }




   bool mesh::_001HasConfigId(const ::database::id & key)
   {
      return _001ConfigIdToColumnKey(key) >= 0;
   }

   index mesh::_001ConfigIdToSubItem(const ::database::id & key)
   {
      //mesh_column * column = m_columna._001GetByConfigId(key);
      //if(column == NULL)
      //   return -1;
      //return column->m_iSubItem;
      return -1;
   }

   index mesh::_001ConfigIdToColumnKey(const ::database::id & key)
   {
      //mesh_column * column = m_columna._001GetByConfigId(key);
      //if(column == NULL)
      //   return -1;
      //return column->m_iKey;
      return -1;
   }

   //mesh_column * mesh_column_array::_001GetByConfigId(const ::database::id & key)
   //{
   //   //index iKey = MapConfigIdToKey(key);
   //   //if(iKey >= 0)
   //   //   return element_at(iKey);
   //   //else
   //   //   return NULL;



   //}

   //index mesh_column_array::MapConfigIdToKey(const ::database::id & key)
   //{
   //   for(index iKey = 0; iKey < this->get_size(); iKey++)
   //   {
   //      if(this->element_at(iKey)->m_datakey == key)
   //         return iKey;
   //   }
   //   return -1;
   //}





   void mesh::item_range::set(index iLowerBoundItem,index iUpperBoundItem,index iLowerBoundSubItem,index iUpperBoundSubItem,index iLowerBoundListItem,index iUpperBoundListItem)
   {

      m_iLowerBound = iLowerBoundItem;
      m_iUpperBound = iUpperBoundItem;
      m_subitemrange.set(
         iLowerBoundSubItem,
         iUpperBoundSubItem,
         iLowerBoundListItem,
         iUpperBoundListItem);
   }

   void mesh::item_range::set_lower_bound(index iLowerBoundItem)
   {
      m_iLowerBound = iLowerBoundItem;
   }

   void mesh::item_range::set_upper_bound(index iUpperBoundItem)
   {
      m_iUpperBound = iUpperBoundItem;
   }

   void mesh::sub_item_range::set(index iLowerBoundSubItem,index iUpperBoundSubItem,index iLowerBoundListItem,index iUpperBoundListItem)
   {
      m_iLowerBound = iLowerBoundSubItem;
      m_iUpperBound = iUpperBoundSubItem;
      m_meshitemrange.set(
         iLowerBoundListItem,
         iUpperBoundListItem);
   }

   void mesh::mesh_item_range::set(index iLowerBoundListItem,index iUpperBoundListItem)
   {
      m_iLowerBound = iLowerBoundListItem;
      m_iUpperBound = iUpperBoundListItem;

   }


   void mesh::_001EnsureVisible(index iItem,bool bRedraw)
   {

      ::point ptScroll = get_viewport_offset();

      if(iItem < ptScroll.y || (m_iItemHeight > 0 && iItem >= ptScroll.y / m_iItemHeight + m_nDisplayCount))
      {

         set_viewport_offset_y((int) (iItem * m_iItemHeight));

         if(bRedraw)
         {

            RedrawWindow();

         }

      }

   }


   void mesh::_001ItemScroll(index iItem,bool bRedraw)
   {

      ::point ptScroll = get_viewport_offset();

      if(iItem < m_nItemCount)
      {

         set_viewport_offset_y((int)(iItem * m_iItemHeight));

         if(bRedraw)
         {

            RedrawWindow();

         }

      }

   }


   void mesh::_001EnsureVisible(index iItem,range & range)
   {

      ::point ptScroll = get_viewport_offset();

      index iyScroll = ptScroll.y / MAX(1,m_iItemHeight);
      if(iItem < iyScroll)
      {
         iyScroll = iItem - m_nDisplayCount + 1;
      }
      else if(iItem >= iyScroll + m_nDisplayCount)
      {
         iyScroll = iItem;
      }
      if(ptScroll.y / MAX(1,m_iItemHeight) != iyScroll)
      {
         item_range item;
         set_viewport_offset_y((int)(iyScroll * m_iItemHeight));
         on_change_viewport_offset();
         item.set_lower_bound(iyScroll);
         item.set_upper_bound(MIN(iyScroll + m_nDisplayCount - 1,m_nItemCount - 1));
         range.add_item(item);
      }
   }

   void mesh::_001Highlight(index iItem,bool bRedraw)
   {
      m_rangeHighlight.clear();
      item_range itemrange;
      itemrange.set(iItem,iItem,0,m_nColumnCount - 1,- 1,-1);
      m_rangeHighlight.add_item(itemrange);
      if(bRedraw)
      {
         RedrawWindow();
      }
   }


   bool mesh::_001OnRemoveItem(index iItem)
   {
      UNREFERENCED_PARAMETER(iItem);
      return false;
   }

   bool mesh::_001RemoveItem(index iItem,bool bRedraw)
   {
      if(!_001OnRemoveItem(iItem))
         return false;
      m_rangeSelection.OnRemoveItem(iItem);
      if(bRedraw)
      {
         RedrawWindow();
      }
      return true;
   }

   void mesh::item_range::offset(index iOffset)
   {
      m_iLowerBound += iOffset;
      m_iUpperBound += iOffset;
   }

   void mesh::_001RemoveSelection()
   {
      range range;

      _001GetSelection(range);


      while(range.get_item_count() > 0)
      {
         index iItem = range.ItemAt(0).get_lower_bound();
         if(!_001RemoveItem(iItem,false))
            break;
         _001GetSelection(range);
      }

      RedrawWindow();
   }


   void mesh::_001Select(index iItem,index iSubItem)
   {
      m_rangeSelection.clear();
      item_range itemrange;
      itemrange.set(iItem,iItem,iSubItem,iSubItem,- 1,-1);
      _001AddSelection(itemrange);

   }

   index mesh::StrictToDisplay(index iStrict)
   {
      UNREFERENCED_PARAMETER(iStrict);
      return -1;
   }

   index mesh::DisplayToStrict(index iDisplay)
   {
      if(iDisplay < 0)
         return -1;
      if(m_bDrag)
      {
         if(m_eview == view_icon)
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
            //     // return m_iconlayout.m_iaDisplayToStrict[m_iItemDrag];
            //   //}
            //   //else if(iDisplay == m_iItemDrag)
            //   //{
            //      //return -1;
            //   //}
            //   //else
            //   {
            //      return m_iconlayout.m_iaDisplayToStrict[iDisplay];
            //   }
            //}
         }
         else
         {
            if(iDisplay < m_iItemDrag && iDisplay < m_iItemDrop)
            {
               return m_iconlayout.m_iaDisplayToStrict[iDisplay];
            }
            else if(iDisplay > m_iItemDrag && iDisplay > m_iItemDrop)
            {
               return m_iconlayout.m_iaDisplayToStrict[iDisplay];
            }
            else
            {
               if(iDisplay == m_iItemDrop)
               {
                  return m_iconlayout.m_iaDisplayToStrict[m_iItemDrag];
               }
               else if(m_iItemDrop > m_iItemDrag)
               {
                  return m_iconlayout.m_iaDisplayToStrict[iDisplay + 1];
               }
               else if(m_iItemDrag > m_iItemDrop)
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
      else if(m_eview == view_icon)
      {
         return m_iconlayout.m_iaDisplayToStrict[iDisplay];
      }
      else
      {
         if(iDisplay < 0)
            return -1;
         if(m_meshlayout.m_iaDisplayToStrict.get_count() <= 0)
         {
            if(iDisplay < m_nItemCount)
               return iDisplay;
         }
         //if(m_meshlayout.m_iaDisplayToStrict.get_count() != m_nItemCount)
         // return iDisplay;
         if(iDisplay >= m_meshlayout.m_iaDisplayToStrict.get_count())
         {
            return -1;
         }
         return m_meshlayout.m_iaDisplayToStrict[iDisplay];
      }
   }

   void mesh::FilterBegin()
   {
      m_efilterstate = FilterStateSetup;

      index iItemCount = m_nItemCount;

      if(m_eview == view_icon)
      {
         m_piaFilterIcon->remove_all();

         for(index i = 0; i < iItemCount; i++)
         {
            m_piaFilterIcon->add(i);
         }
      }
      else
      {
         m_piaFilterMesh->remove_all();

         for(index i = 0; i < iItemCount; i++)
         {
            m_piaFilterMesh->add(i);
         }
      }

   }

   void mesh::FilterApply()
   {

      //      ASSERT(m_efilterstate == FilterStateSetup);

      m_iFilter1Step = 0;

      SetTimer(0xfffffffe,50,NULL);

      set_viewport_offset_x(0);

      set_viewport_offset_y(0);

      m_efilterstate = FilterStateFilter;

      on_change_view_size();

      on_layout();

      RedrawWindow();

   }

   void mesh::FilterClose()
   {
      m_bFilter1 = false;

      KillTimer(0xfffffffe);

      ASSERT(m_efilterstate == FilterStateSetup
         || m_efilterstate == FilterStateFilter);

      index iItemCount = m_nItemCount;

      if(m_eview == view_icon)
      {
         m_iconlayout.m_iaDisplayToStrict.remove_all();

         for(index i = 0; i < iItemCount; i++)
         {
            m_iconlayout.m_iaDisplayToStrict.add(i);
         }
      }
      else
      {
         m_meshlayout.m_iaDisplayToStrict.remove_all();

         for(index i = 0; i < iItemCount; i++)
         {
            m_meshlayout.m_iaDisplayToStrict.add(i);
         }
      }

      on_change_view_size();

      on_layout();

      RedrawWindow();

   }

   void mesh::FilterExcludeAll()
   {
      if(m_eview == view_icon)
      {
         m_piaFilterIcon->remove_all();
      }
      else
      {
         m_piaFilterMesh->remove_all();
      }
   }

   void mesh::FilterInclude(index iItem)
   {
      ASSERT(m_efilterstate == FilterStateSetup);
      if(m_eview == view_icon)
      {
         m_piaFilterIcon->add_unique(iItem);
      }
      else
      {
         m_piaFilterMesh->add_unique(iItem);
      }
   }

   void mesh::FilterInclude(int_array & array)
   {
      ASSERT(m_efilterstate == FilterStateSetup);
      for(index i = 0; i < array.get_size() ; i++)
      {
         FilterInclude(array[i]);
      }
   }


   bool mesh::Filter1Step()
   {
      uint32_t dwIn = get_tick_count();
      uint32_t dwOut;
      TRACE("mesh::Filter1Step\n");
      TRACE("dwIn = %d\n",dwIn);
      if(!m_bFilter1)
         return false;

      string wstrItem;

      index iItemCount = MIN(m_nItemCount,m_iFilter1Step + 1000);

      index iFilter1Step;

      draw_mesh_item item(this);

      for(
         iFilter1Step =  m_iFilter1Step;
         iFilter1Step < iItemCount;
      iFilter1Step++)
      {
         //for(index j = 0; j < m_nColumnCount; j++)
         /*{
            mesh_column * pcolumn = m_columna._001GetByKey(j);
            item.m_strText.Empty();
            item.m_iItem = iFilter1Step;
            item.m_iSubItem = pcolumn->m_iSubItem;
            item.m_iListItem = -1;
            _001SearchGetItemText(&item);
            if(item.m_bOk)
            {
               item.m_strText.make_lower();
               item.m_strText.replace("_"," ");
               if(m_reFilter1.match(item.m_strText))
               {
                  if(m_eview == view_icon)
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
         }*/
      }

      m_iFilter1Step = (int32_t)iFilter1Step;

      if(m_eview == view_icon)
      {
         m_iconlayout.m_iaDisplayToStrict = (*m_piaFilterIcon);
      }
      else
      {
         m_meshlayout.m_iaDisplayToStrict = (*m_piaFilterMesh);
      }

      set_viewport_offset_x(0);

      set_viewport_offset_y(0);

      m_efilterstate = FilterStateFilter;

      on_change_view_size();

      on_layout();

      RedrawWindow();


      dwOut = get_tick_count();
      TRACE("dwOut = %d\n",dwOut);
      TRACE("(delta) = %d\n",dwOut - dwIn);

      return m_nItemCount != iItemCount;
   }


   void mesh::Filter1(string & strFilter)
   {

      if(m_eview == view_icon)
      {
         m_piaFilterIcon->remove_all();
      }
      else
      {
         m_piaFilterMesh->remove_all();
      }

      stringa stra;

      stra.add_tokens(strFilter," ",false);

      m_reFilter1.setPositionMoves(1);

      m_bFilter1 = m_reFilter1.setRE("/.*" + stra.implode(".*") + ".*/i");

      if(m_bFilter1)
      {
         FilterApply();
      }
      else
      {
         FilterClose();
      }

   }

   void mesh::_001OnMeshHeaderItemClick(index iHeaderItem)
   {
      if(!m_bSortEnable)
         return;
      _001OnSort(_001MapColumnToSubItem(iHeaderItem));
   }

   void mesh::_001OnMeshHeaderItemDblClk(index iHeaderItem)
   {
      _001MaximizeColumnWidth(iHeaderItem);
   }


   bool mesh::does_drag_reorder()
   {

      return false;

   }


   void mesh::_001OnSort()
   {
      if(!m_bSortEnable)
         return;
      if(m_eview != view_icon)
      {
         ::sort::quick_sort(m_meshlayout.m_iaDisplayToStrict,this,(::sort::compare_interface::_FUNCTION_Compare) &::user::mesh::_001Compare);
      }
      else
      {
      }
   }

   void mesh::_001OnSort(index iSubItem)
   {

      index iFound = -1;
      for(index i = 0; i < m_sortinfo.m_itema.get_size(); i++)
      {
         CSortInfoItem & item = m_sortinfo.m_itema[i];
         if(item.m_iSubItem == iSubItem)
         {
            iFound = i;
            break;
         }
      }
      if(iFound >= 0)
      {
         CSortInfoItem newItem = m_sortinfo.m_itema[iFound];
         m_sortinfo.m_itema.remove_at(iFound);
         newItem.m_bAscendent = !newItem.m_bAscendent;
         m_sortinfo.m_itema.insert_at(0,newItem);
      }
      else
      {
         CSortInfoItem newItem;
         newItem.m_iSubItem = iSubItem;
         newItem.m_bAscendent = true;
         m_sortinfo.m_itema.insert_at(0,newItem);
      }

      _001OnSort();

   }




   index mesh::_002Compare(index iItem1,index iItem2,index iSubItem)
   {
      draw_mesh_item item1(this);
      draw_mesh_item item2(this);

      item1.m_iItem     = iItem1;
      item2.m_iItem     = iItem2;
      item1.m_iSubItem  = iSubItem;
      item2.m_iSubItem  = iSubItem;

      _001GetItemText(&item1);
      _001GetItemText(&item2);

      if(item1.m_bOk && item2.m_bOk)
      {
         return item1.m_strText.compare_ci(item2.m_strText);
      }
      else
      {
         if(item1.m_bOk)
            return 1;
         else if(item2.m_bOk)
            return -1;
         else
            return 0;
      }

   }

   index mesh::_001Compare(index iItem1,index iItem2)
   {
      CSortInfoItem * pitem = NULL;
      index iCompare = 0;
      for(index i = 0; i < m_sortinfo.m_itema.get_size(); i++)
      {
         pitem = &m_sortinfo.m_itema[i];
         iCompare = _002Compare(iItem1,iItem2,pitem->m_iSubItem);
         if(iCompare != 0)
            break;
      }
      if(pitem == NULL)
         return 0;
      else
      {
         if(pitem->m_bAscendent)
            return iCompare;
         else
            return -iCompare;
      }
   }


   index mesh::_001DisplayCompare(index iDisplayItem1,index iDisplayItem2)
   {
      return _001Compare(DisplayToStrict(iDisplayItem1),DisplayToStrict(iDisplayItem2));
   }


   int32_t mesh::_001CalcMeshWidth()
   {
      ASSERT(FALSE);
      return -1;
   }

   int32_t mesh::_001CalcColumnWidth(index iColumn)
   {
      UNREFERENCED_PARAMETER(iColumn);
      ::draw2d::memory_graphics pgraphics(allocer());
      select_font(pgraphics, font_list_item, this);
      //pgraphics->SelectObject(pfont);
      int32_t iMaxWidth = 0;
      ::count iCount = m_nItemCount;
      int32_t iWidth;
      for(index i = 0; i < iCount; i++)
      {
         iWidth = _001CalcItemWidth(pgraphics,i,0);
         if(iWidth > iMaxWidth)
         {
            iMaxWidth = iWidth;
         }
      }

      return iMaxWidth;

   }


   bool mesh::_001SetColumnWidth(index iColumn,int32_t iWidth)
   {

      m_mapColumnWidth[iColumn] = iWidth;

      return true;

   }



   void mesh::_001MaximizeColumnWidth(index iColumn)
   {
      _001SetColumnWidth(iColumn,_001CalcColumnWidth(iColumn));
   }


   void mesh::_OnDraw(::draw2d::graphics * pgraphics)
   {

      UNREFERENCED_PARAMETER(pgraphics);

   }


   int32_t mesh::get_wheel_scroll_delta()
   {

      if(m_eview == view_grid)
      {

         return 1;

      }
      else
      {

         return m_iItemHeight;

      }

   }


   //rect mesh::get_scroll_margin()
   //{

   //   if(m_eview == view_grid && m_iItemHeight > 0)
   //   {

   //      return rect(m_scrolldata.m_rectMargin.left,m_scrolldata.m_rectMargin.top/m_iItemHeight, m_scrolldata.m_rectMargin.right,m_scrolldata.m_rectMargin.bottom / m_iItemHeight);

   //   }
   //   else
   //   {

   //      return scroll_control::get_scroll_margin();

   //   }

   //}


   void mesh::on_change_viewport_offset()
   {

//      ::user::control::on_change_viewport_offset();

      m_iTopIndex = _001CalcDisplayTopIndex();
      index iLow = 0;
      for(m_iTopGroup = 0; m_iTopGroup < m_nGroupCount; m_iTopGroup++)
      {
         if(m_iTopIndex >= iLow && m_iTopIndex < (iLow + _001GetGroupItemCount(m_iTopGroup)))
            break;
      }
      m_nDisplayCount = _001CalcDisplayItemCount();

//      HeaderCtrlLayout();

      CacheHint();

      UpdateHover();

   }



   ::draw2d::pen * mesh::_001GetPenHighlight()
   {
      return m_penHighlight;
   }

   ::draw2d::pen * mesh::_001GetPenFocused()
   {
      return m_penFocused;

   }

   //::draw2d::font * mesh::_001GetFont()
   //{
   //   return m_font;
   //}

   //::draw2d::font * mesh::_001GetFontHover()
   //{
   //   return m_fontHover;
   //}

   void mesh::_001OnMouseLeave(signal_details * pobj)
   {
      m_iItemHover = -1;
      m_iSubItemHover = -1;
      RedrawWindow();
      pobj->m_bRet = true;
   }



   void mesh::UpdateHover()
   {
      index iItemHover;
      index iSubItemHover;
      point point;
      Session.get_cursor_pos(&point);
      ScreenToClient(&point);

      if(_001DisplayHitTest(point,iItemHover,iSubItemHover))
      {
         if(m_iSubItemHover != iSubItemHover ||
            m_iItemHover != iItemHover)
         {
            m_iItemHover = iItemHover;
            m_iSubItemHover = iSubItemHover;
            RedrawWindow();
         }
      }

   }


   mesh::range::range()
   {

      m_piLink = NULL;

   }


   mesh::range::range(const range & range)
   {

      m_piLink = NULL;
      m_itemrangea = range.m_itemrangea;

   }


   ::user::mesh_data * mesh::GetDataInterface()
   {

      return m_pmeshdata;

   }


   void mesh::_001SetView(EView eview, bool bLayout)
   {

      m_eview = eview;

      if (m_eview == ::user::mesh::view_icon)
      {

         m_pdrawmeshitem->m_iDrawTextFlags = DT_TOP | DT_CENTER | DT_END_ELLIPSIS | DT_WORDBREAK | DT_NOPREFIX;

      }
      else
      {

         m_pdrawmeshitem->m_iDrawTextFlags = DT_VCENTER | DT_LEFT | DT_END_ELLIPSIS | DT_SINGLELINE | DT_NOPREFIX;

      }


      on_ui_event(event_change_view_style, object_list, this);

      data_get_DisplayToStrict();

      if (bLayout)
      {

         on_layout();

      }

   }


   void mesh::data_get_DisplayToStrict()
   {

      if(m_eview == view_icon)
      {

         data_get(data_get_current_sort_id() + "." + data_get_current_mesh_layout_id(),m_iconlayout);

      }
      else
      {

         data_get(data_get_current_sort_id() + "." + data_get_current_mesh_layout_id(),m_meshlayout);

      }

   }


   void mesh::data_set_DisplayToStrict()
   {
      if(m_eview == view_icon)
      {
         data_set(data_get_current_sort_id() + "." + data_get_current_mesh_layout_id(),m_iconlayout);
      }
      else
      {
         data_set(data_get_current_sort_id() + "." + data_get_current_mesh_layout_id(),m_meshlayout);
      }
   }

   id mesh::data_get_current_sort_id()
   {
      return data_get_sort_id(m_eview);
   }

   id mesh::data_get_sort_id(EView eview)
   {
      UNREFERENCED_PARAMETER(eview);
      switch(m_eview)
      {
      case view_grid:
         return "sort-grid";
         break;
      case view_report:
         return "sort-report";
         break;
      case view_list:
         return "sort-mesh";
         break;
      case view_icon:
         return "sort-icon";
         break;
      default:
         ASSERT(FALSE);
         return "sort";
      }
   }

   mesh::EView mesh::_001GetView()
   {
      return m_eview;
   }


   void mesh::_001OnAfterSort()
   {
      data_set_DisplayToStrict();
   }

   bool mesh::_001IsEditing()
   {
      return false;
   }

   void mesh::_001GetItemColor(::user::mesh_item * pitem)
   {

      if (m_pmeshdata != NULL)
      {

         m_pmeshdata->_001GetItemColor(pitem);

         if (pitem->m_bOk)
         {

            return;

         }

      }
      pitem->m_bOk = false;
   }


   void mesh::_001OnVScroll(signal_details * pobj)
   {

      SCAST_PTR(::message::scroll, pscroll, pobj);

      pobj->previous();

      synch_lock sl(m_pmutex);

      if(m_eview == view_grid)
      {

         if(pscroll->m_nSBCode != SB_THUMBTRACK)
         {

            point ptScroll = get_viewport_offset();

            size sizePage = get_page_size();

            if((m_sizeTotal.cy - ptScroll.y - sizePage.cy) <= 1)
            {

               m_nItemCount = MIN(m_nGridItemCount,m_nItemCount + sizePage.cy);

               on_change_view_size();

            }

         }

      }

   }

   void mesh::_001OnHScroll(signal_details * pobj)
   {

      SCAST_PTR(::message::scroll, pscroll, pobj);

      pobj->previous();

      if(m_eview == view_grid)
      {

         if(pscroll->m_nSBCode != SB_THUMBTRACK)
         {

            point ptScroll = get_viewport_offset();

            size sizePage = get_page_size();

            if((m_sizeTotal.cx - ptScroll.x - sizePage.cx) <= 1)
            {

               m_nColumnCount = MIN(m_nGridColumnCount,m_nColumnCount + sizePage.cx);

               on_change_view_size();

            }

         }

      }

      m_iTopIndex = _001CalcDisplayTopIndex();
      index iLow = 0;
      for(m_iTopGroup = 0; m_iTopGroup < m_nGroupCount; m_iTopGroup++)
      {
         if(m_iTopIndex >= iLow && m_iTopIndex < (iLow + _001GetGroupItemCount(m_iTopGroup)))
            break;
      }
      m_nDisplayCount = _001CalcDisplayItemCount();

//      HeaderCtrlLayout();

      CacheHint();

   }


   id mesh::data_get_current_mesh_layout_id()
   {

      return "mesh." + m_dataid.m_id.str();

   }


   bool mesh::query_drop(index iDisplayDrop,index iDisplayDrag)
   {
      if(iDisplayDrag < 0)
         return false;
      if(iDisplayDrop < 0)
         return false;
      if(m_iItemDrag != m_iItemDrop)
      {
         if(m_eview == view_icon)
         {
            if(m_iconlayout.m_iaDisplayToStrict[m_iItemDrop] == -1 || m_iconlayout.m_iaDisplayToStrict[m_iItemDrop] >= m_nItemCount)
            {
               return true;
            }
         }
         else
         {
            if(m_meshlayout.m_iaDisplayToStrict[m_iItemDrop] == -1 || m_meshlayout.m_iaDisplayToStrict[m_iItemDrop] >= m_nItemCount)
            {
               return true;
            }
         }
      }
      return false;
   }


   bool mesh::do_drop(index iDisplayDrop,index iDisplayDrag)
   {
      UNREFERENCED_PARAMETER(iDisplayDrop);
      UNREFERENCED_PARAMETER(iDisplayDrag);
      if(m_eview == view_icon)
      {
         m_iconlayout.m_iaDisplayToStrict.translate_a(m_iItemDrop,m_iItemDrag);
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

   bool mesh::defer_drop(index iDisplayDrop,index iDisplayDrag)
   {
      if(query_drop(iDisplayDrop,iDisplayDrag))
      {
         if(m_eview == view_icon)
         {
//            if(m_iconlayout.m_iWidth <= 0)
//            {
//               rect rectClient;
//               GetClientRect(rectClient);
//               index iIconSize;
//               if(m_nColumnCount > 0)
////                  iIconSize = MAX(32,m_columna[0]->m_sizeIcon.cy);
//                     iIconSize = 32;
//               else
//                  iIconSize = 32;
//               index iItemSize = iIconSize * 2;
//               m_iconlayout.m_iWidth = (int32_t)(MAX(1,rectClient.width() / iItemSize));
//            }
            return do_drop(iDisplayDrop,iDisplayDrag);
         }
         else
         {
            return do_drop(iDisplayDrop,iDisplayDrag);
         }
      }
      else
      {
         return false;
      }
   }

   mesh::mesh_layout::mesh_layout()
   {
   }

   mesh::mesh_layout::~mesh_layout()
   {
   }

   void mesh::mesh_layout::write(::file::ostream & ostream) const
   {
      //ostream << m_iaDisplayToStrict;
      ostream << m_iWidth;
   }

   void mesh::mesh_layout::read(::file::istream & istream)
   {
      //istream >> m_iaDisplayToStrict;
      istream >> m_iWidth;
   }

   mesh::icon_layout::icon_layout()
   {
//      m_iWidth = -1;
   }

   mesh::icon_layout::~icon_layout()
   {
   }

   void mesh::icon_layout::write(::file::ostream & ostream) const
   {
      ostream << m_iaDisplayToStrict;
//      ostream << m_iWidth;
   }

   void mesh::icon_layout::read(::file::istream & istream)
   {
      istream >> m_iaDisplayToStrict;
//      istream >> m_iWidth;
   }

   class size mesh::get_item_size()
   {
      if(m_eview == view_icon)
      {
         if(m_nColumnCount == 0)
         {
            return size(32,32);
         }
//         index iIconSize = MAX(32,m_columna[0]->m_sizeIcon.cy);
         index iIconSize = 32;
         index iItemSize = iIconSize * 2;
         return size(iItemSize,iItemSize);
      }
      else
      {
         // not implemented
         ASSERT(FALSE);
         return size(0,0);
      }
   }

   void mesh::auto_arrange(bool bAutoArrange)
   {
      if(bAutoArrange)
      {
         m_flags.signalize(flag_auto_arrange);
      }
      else
      {
         m_flags.unsignalize(flag_auto_arrange);
      }
      if(bAutoArrange)
      {
         index iMaxStrict = m_iconlayout.m_iaDisplayToStrict.get_max_b();
         for(index iStrict = 0; iStrict <= iMaxStrict; iStrict++)
         {
            m_iconlayout.m_iaDisplayToStrict.set(iStrict,iStrict);
         }
         on_layout();
      }
   }

   bool mesh::get_auto_arrange()
   {
      return m_flags.is_signalized(flag_auto_arrange);
   }

   void mesh::_001OnMeshViewAutoArrange(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      auto_arrange(!get_auto_arrange());
   }

   void mesh::_001OnUpdateMeshViewAutoArrange(signal_details * pobj)
   {
      SCAST_PTR(::aura::cmd_ui,pcmdui,pobj);
         pcmdui->m_pcmdui->_001SetCheck(get_auto_arrange());
      pcmdui->m_pcmdui->Enable();
   }

   bool mesh::is_valid_display_item(index iDisplayItem)
   {
      if(iDisplayItem < 0)
         return false;
      if(iDisplayItem >= m_nItemCount)
         return false;
      return true;
   }

   bool mesh::is_valid_strict_item(index iStrictItem)
   {
      if(iStrictItem < 0)
         return false;
      if(iStrictItem >= m_nItemCount)
         return false;
      return true;

   }


   bool mesh::keyboard_focus_is_focusable()
   {
      return true;
   }

   void mesh::_001OnSelectionChange()
   {
   }

   index mesh::set_cur_sel(index iSel)
   {
      index iOld = get_cur_sel();
      m_rangeSelection.clear();
      if(iSel >= 0)
      {
         item_range itemrange;
         itemrange.set_lower_bound(iSel);
         itemrange.set_upper_bound(iSel);
         _001AddSelection(itemrange);
      }
      return iOld;
   }

   void mesh::set_cur_sel(const index_array & iaSel)
   {
      m_rangeSelection.clear();
      for(index i = 0; i < iaSel.get_count(); i++)
      {
         item_range itemrange;
         itemrange.set_lower_bound(iaSel[i]);
         itemrange.set_upper_bound(iaSel[i]);
         m_rangeSelection.add_item(itemrange);
      }

   }

   index mesh::get_cur_sel()
   {
      if(m_rangeSelection.get_item_count() != 1)
         return -1;
      if(m_rangeSelection.ItemAt(0).get_lower_bound() == m_rangeSelection.ItemAt(0).get_upper_bound() && m_rangeSelection.ItemAt(0).get_lower_bound() >= 0)
         return m_rangeSelection.ItemAt(0).get_lower_bound();
      return -1;
   }


   void mesh::get_cur_sel(index_array & iaSel)
   {

      for(index i = 0; i < m_rangeSelection.get_item_count(); i++)
      {

         for(index iItem = m_rangeSelection.ItemAt(i).get_lower_bound(); iItem <= m_rangeSelection.ItemAt(i).get_upper_bound(); iItem++)
         {

            iaSel.add_unique(iItem);

         }

      }

   }


   ::count mesh::_001GetGroupItemCount(index iGroup)
   {
      UNREFERENCED_PARAMETER(iGroup);
      return -1;
   }

   int32_t mesh::_001GetGroupHeight(index iGroup)
   {
      int32_t iMeshHeight = (int32_t)(_001GetGroupItemCount(iGroup) * m_iItemHeight);
      return MAX(m_iGroupMinHeight,iMeshHeight);
   }

   mesh_item::mesh_item(mesh * pmesh):
      ::object(pmesh->get_app())
   {
      m_plistitem       = NULL;
      m_pmesh           = pmesh;
      m_iGroup          = -1;
      m_iItem           = -1;
      m_iDisplayItem    = -1;
      m_iColumn         = -1;
      m_iOrder          = -1;
      m_iSubItem        = -1;
      m_iListItem       = -1;
      m_crText              = (COLORREF)-1;
      m_crTextBackground    = ARGB(255, 0, 0, 0);
      m_crItemBackground = 0;
      m_iState          = -1;
      m_iImage          = -1;
      m_bOk             = false;

      m_iGroupTopIndex  = -1;
      m_iGroupCount     = -1;

   }

   draw_mesh_item::draw_mesh_item(mesh * pmesh):
      ::object(pmesh->get_app()),
      mesh_item(pmesh)
   {

      m_prectClient              = NULL;

      m_iColumn = -1;
      m_iColumnWidth             = 0;

      m_iItemRectItem            = -1;

      m_iSubItemRectItem         = -1;
      m_iSubItemRectSubItem      = -1;
      m_iSubItemRectOrder        = -1;
      m_iSubItemRectColumn       = -1;

      m_iListItemRectItem        = -1;
      m_iListItemRectSubItem     = -1;
      m_iListItemRectListItem    = -1;

   }


   sp(image_list) draw_mesh_item::get_image_list()
   {
      //if(m_bListItemHover && m_pcolumn->m_pilHover != NULL)
      //if(m_bListItemHover)
      //{
      //   return m_pcolumn->m_pilHover;
      //}
      //else
      //{
      //   return m_pcolumn->m_pil;
      //}

      return NULL;

   }


   bool draw_mesh_item::draw_image()
   {
      //if(m_pcolumn->m_bIcon)
      //{
      //   visual::icon * picon;
      //   if(m_pcolumn->m_mapIcon.Lookup((int32_t)m_iImage,picon))
      //   {
      //      m_pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);
      //      return m_pgraphics->DrawIcon(m_rectImage.top_left(),picon) != FALSE;
      //   }
      //}
      //else
      //{
      //   m_pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);
      //   return get_image_list()->draw(m_pgraphics,(int32_t)m_iImage,m_rectImage.top_left(),m_rectImage.size(),point(0,0),0);
      //}
      return false;
   }


   bool draw_mesh_item::draw_group_image()
   {
      return m_pmesh->m_pilGroup->draw(m_pgraphics,(int32_t)m_iImage,m_rectImage.top_left(),m_rectImage.size(),point(0,0),0);
   }


   void draw_mesh_item::update_item_color()
   {
      m_pmesh->_001GetItemColor(this);

      if(!m_bOk)
      {
         if(m_bListItemSelected)
         {
            if(m_bListItemHover)
            {
               m_crText =m_pmesh->_001GetColor(::user::color_text_selected_highlight);
            }
            else
            {
               m_crText =m_pmesh->_001GetColor(::user::color_text_selected);
            }
         }
         else
         {
            if(m_bListItemHover)
            {
               m_crText =m_pmesh->_001GetColor(::user::color_text_highlight);
            }
            else
            {
               m_crText =m_pmesh->_001GetColor(::user::color_text);
            }
         }
      }
   }

   void draw_mesh_item::set_text_color()
   {
      ::draw2d::brush_sp brushText(allocer());
      brushText->create_solid(m_crText);
      m_pgraphics->SelectObject(brushText);
   }


   void draw_mesh_item::draw_text()
   {

      if(m_bOk)
      {

         if(m_pmesh->m_eview == mesh::view_icon && m_pmesh->m_bEmboss)
         {

            class size size = m_rectText.size();
            size.cx += 4;
            size.cy += 4;

            ::draw2d::dib_sp dib(m_pmesh->allocer());
            dib->create(size.cx,size.cy);
            dib->Fill(0,0,0,0);
            ::draw2d::brush_sp brushText(allocer());
            brushText->create_solid(ARGB(255,255,255,255));
            dib->get_graphics()->SelectObject(brushText);
            ::draw2d::dib_sp dib2(m_pmesh->allocer());
            dib2->create(size.cx,size.cy);
            dib2->Fill(0,0,0,0);

            class rect rectCache;
            rectCache.left = 2;
            rectCache.top = 2;
            rectCache.right = rectCache.left + (int32_t)m_rectText.width();
            rectCache.bottom = rectCache.top + (int32_t)m_rectText.height();
            dib->get_graphics()->SelectObject(m_pfont);
            dib->get_graphics()->_DrawText(m_strText,rectCache,m_iDrawTextFlags);

            Sys(m_pmesh->get_app()).visual().imaging().channel_spread_set_color(dib2->get_graphics(),null_point(),size,dib->get_graphics(),null_point(),0,2,ARGB(184,184,184,184));
            dib->Fill(0,0,0,0);
            Sys(m_pmesh->get_app()).visual().imaging().channel_alpha_gray_blur(dib->get_graphics(),null_point(),size,dib2->get_graphics(),null_point(),0,1);
            dib2->Fill(0,0,0,0);
            Sys(m_pmesh->get_app()).visual().imaging().channel_alpha_gray_blur(dib2->get_graphics(),null_point(),size,dib->get_graphics(),null_point(),0,1);
            dib2->set(0,0,0);


            Sys(m_pmesh->get_app()).visual().imaging().color_blend(m_pgraphics,m_rectText,dib2->get_graphics(),point(1,1),0.50);


            brushText->create_solid(ARGB(255,255,255,255));
            m_pgraphics->SelectObject(brushText);
            m_pgraphics->SelectObject(m_pfont);
            m_pgraphics->_DrawText(m_strText,m_rectText,m_iDrawTextFlags);
         }
         else
         {
            ::draw2d::brush_sp brushText(allocer());
            brushText->create_solid(m_crText);
            m_pgraphics->SelectObject(brushText);
            m_pgraphics->_DrawText(m_strText,m_rectText,m_iDrawTextFlags);
         }
      }
   }

   ::count mesh::_001GetGroupMetaItemCount(index iGroup)
   {
      if(m_pmeshdata != NULL)
      {
         return m_pmeshdata->_001GetGroupMetaItemCount(iGroup);
      }
      return 0;
   }

   void mesh::_001GetGroupText(mesh_item * pitem)
   {
      pitem->m_bOk = false;
      if(m_pcache != NULL)
      {
         m_pcache->_001GetGroupText(pitem);
         if(pitem->m_bOk)
            return;
      }
      if(m_pmeshdata != NULL)
      {
         m_pmeshdata->_001GetGroupText(pitem);
      }
   }

   void mesh::_001GetGroupImage(mesh_item * pitem)
   {
      pitem->m_bOk = false;
      pitem->m_iImage = -1;
   }




   void mesh::_001OnBeforeDeleteRange(range & range)
   {
   }

   void mesh::_001OnDeleteRange(range & range)
   {

      if(m_pmeshdata != NULL)
      {

         m_pmeshdata->_001OnDeleteRange(range);

      }

   }


   void mesh::_001DeleteRange(range & range)
   {

      _001OnBeforeDeleteRange(range);

      _001OnDeleteRange(range);

   }


   void mesh::defer_create_mesh_data()
   {

      sp(::user::list_data) pmeshdata = create_mesh_data();

      if(pmeshdata.is_null())
         return;

      SetDataInterface(pmeshdata);

   }


   ::count mesh::_001GetColumnCount()
   {

      if(m_pmeshdata.is_null())
         return -1;

      return m_pmeshdata->_001GetColumnCount(this);

   }


   point mesh::get_viewport_offset()
   {

      return ::user::control::get_viewport_offset();

   }


   //void mesh::on_change_view_size()
   //{

   //   if(m_eview == view_grid && m_iItemHeight > 0)
   //   {

   //      rect rectTotal;

   //      _001GetViewRect(&rectTotal);

   //      size sizeTotal = rectTotal.size();

   //      m_sizeTotal.cx = sizeTotal.cx;

   //      m_sizeTotal.cy = sizeTotal.cy / m_iItemHeight;

   //      rect rectViewClient;

   //      GetClientRect(&rectViewClient);

   //      m_scrolldata.m_sizePage.cx = rectViewClient.size().cx;

   //      m_scrolldata.m_sizePage.cy = rectViewClient.size().cy / m_iItemHeight;

   //      if(m_scrolldata.m_ptScroll.y > (m_sizeTotal.cy - m_scrolldata.m_sizePage.cy))
   //      {

   //         m_scrolldata.m_ptScroll.y = (m_sizeTotal.cy - m_scrolldata.m_sizePage.cy);

   //      }

   //   }
   //   else
   //   {
   //
   //      ::user::box::on_change_view_size();

   //   }


   //}


   size mesh::get_total_size()
   {

      return m_sizeTotal;

   }


   size mesh::get_page_size()
   {

      if(m_eview == view_grid && m_iItemHeight > 0)
      {

         rect rectClient;

         GetClientRect(rectClient);

         point ptScroll = get_viewport_offset();

         size sizePage;

         sizePage.cx = rectClient.width() / m_iDefaultColumnWidth;

         sizePage.cy = rectClient.width() / m_iItemHeight;

         return sizePage;

      }
      else
      {

         return ::user::control::get_page_size();

      }

   }


   bool mesh::GetClientRect(LPRECT lprect)
   {

      if(m_eview == view_grid && m_iItemHeight > 0)
      {

         if(!::user::control::GetClientRect(lprect))
         {

            return false;

         }

      }
      else
      {

         if(!::user::control::GetClientRect(lprect))
         {

            return false;

         }

      }

      return true;

   }



   /*int mesh::get_scroll_bar_width()
   {

      if(m_eview == view_grid)
      {

         return 0;

      }
      else
      {

         return ::user::control::get_scroll_bar_width();

      }

   }


   int mesh::get_scroll_bar_height()
   {

      if(m_eview == view_grid)
      {

         return 0;

      }
      else
      {

         return ::user::control::get_scroll_bar_height();

      }

   }*/

   void mesh::on_create_draw_item()
   {

      m_pdrawmeshitem            = new draw_mesh_item(this);


   }

   string mesh::_001GetItemId(index iStrictItem)
   {

      return ::str::from(iStrictItem);

   }

   index mesh::_001GetItemById(const char * pszChar)
   {

      ::count c = _001GetItemCount();

      string strId(pszChar);

      for (index iItem = 0; iItem < c; iItem++)
      {

         if (strId == _001GetItemId(iItem))
         {

            return iItem;

         }

      }

      return -1;

   }



} // namespace user



