#include "framework.h"
//#include <math.h>


namespace user
{

   tree::tree() :
      object(get_app()),
      m_dcextension(get_app())
   {

      user_tree_common_construct();

   }


   tree::tree(::aura::application * papp) :
      object(papp),
      m_dcextension(papp)
   {

      user_tree_common_construct();

   }


   tree::~tree()
   {

   }


   void tree::user_tree_common_construct()
   {

      ASSERT(get_app() != NULL);

      m_bHoverStart = false;

      m_pitemFirstVisible        = NULL;
      m_pitemHover               = NULL;
      m_iItemCount               = 0;
      m_iItemHeight              = 18;
      m_iImageExpand             = -1;
      m_iImageCollapse           = -1;
      m_pimagelist               = NULL;
      m_uchHoverAlphaInit        = 0;
      m_iDrawTextFlags = DT_LEFT | DT_VCENTER;

   }


   void tree::_001OnCreate(signal_details * pobj)
   {

      if(pobj->previous())
         return;

      
      m_pimagelist = canew(image_list(get_app()));

      m_pimagelist->create(16, 16, 0, 10, 10);
      
      ::fork(get_app(), [this]()
      {

         _001SetCollapseImage("list/collapse.png");

         _001SetExpandImage("list/expand.png");
         
      });


   }


   void tree::_001OnDraw(::draw2d::graphics * pgraphics)
   {

      ::user::interaction::_001OnDraw(pgraphics);

      

      rect rectClient;

      GetClientRect(rectClient);

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

      pgraphics->FillSolidRect(rectClient, _001GetColor(color_list_background));


      point ptCursor;

      Session.get_cursor_pos(&ptCursor);
      DWORD dwHoverIn = 384;
      DWORD dwHoverOut = 1284;
      ScreenToClient(&ptCursor);
      bool bTreeHover = rectClient.contains(ptCursor);
      if(bTreeHover)
      {
         if(!m_bHoverStart)
         {
            m_bHoverStart = true;
            m_uchHoverAlphaInit = m_uchHoverAlpha;
            m_dwHoverStart = get_tick_count();
         }
         if(get_tick_count() - m_dwHoverStart > dwHoverIn)
         {
            m_uchHoverAlpha = 255;
         }
         else
         {
            DWORD dwCurve =  (DWORD) (255.0 * (1.0 - pow(2.718281, -3.3 * (get_tick_count() - m_dwHoverStart) / dwHoverIn)));
            if(m_uchHoverAlphaInit + dwCurve > 255)
               m_uchHoverAlpha = 255;
            else
               m_uchHoverAlpha =  (BYTE) (m_uchHoverAlphaInit + dwCurve);
         }
      }
      else
      {
         if(m_bHoverStart)
         {
            m_bHoverStart = false;
            m_uchHoverAlphaInit = m_uchHoverAlpha;
            m_dwHoverEnd = get_tick_count();
         }

         if(get_tick_count() - m_dwHoverEnd  > dwHoverOut)
         {
            m_uchHoverAlpha = 0;
         }
         else
         {
            DWORD dwCurve =  (DWORD) (255.0 * (1.0 - pow(2.718281, -3.3 * (get_tick_count() - m_dwHoverEnd) / dwHoverOut)));
            if(m_uchHoverAlphaInit < dwCurve)
               m_uchHoverAlpha = 0;
            else
               m_uchHoverAlpha = (BYTE) (m_uchHoverAlphaInit - dwCurve);
         }
      }

      point ptOffset = get_viewport_offset();

      //pgraphics->OffsetViewportOrg((int32_t)-ptOffset.x,(int32_t)-(ptOffset.y % _001GetItemHeight()));
      pgraphics->OffsetViewportOrg(-ptOffset.x, -ptOffset.y);

      ::user::tree_draw_item drawitemdata;
      
      drawitemdata.m_pdc = pgraphics;
      
      drawitemdata.m_iIndentation = _001GetIndentation();
      
      drawitemdata.m_iItemHeight = _001GetItemHeight();
      
      GetClientRect(drawitemdata.m_rectClient);

      sp(::data::tree_item) pitem = m_pitemFirstVisible;

      synch_lock sl(pitem.is_null()? NULL :pitem->m_ptree->m_pmutex);

      index iItem = m_iFirstVisibleItemProperIndex;

      int iVisibleCount = _001GetVisibleItemCount();

      index iIndex = 0;

      for(;pitem != NULL; pitem = pitem->get_item(::data::TreeNavigationProperForward))
      {

         drawitemdata.m_pitem = pitem;

         drawitemdata.m_iItem = iItem;

         drawitemdata.m_rect = drawitemdata.m_rectClient;

         drawitemdata.m_rect.left = (LONG) (drawitemdata.m_iIndentation * pitem->m_iLevel);

         drawitemdata.m_rect.top = (LONG) (iItem * drawitemdata.m_iItemHeight);

         drawitemdata.m_rect.bottom = (LONG) (drawitemdata.m_rect.top + drawitemdata.m_iItemHeight);

         drawitemdata.m_rect.right = m_iCurrentViewWidth;

         _001DrawItem(drawitemdata);

         iItem++;

         iIndex++;

         if (iIndex > iVisibleCount)
         {

            break;

         }

      }

   }


   void tree::_001DrawItem(tree_draw_item & data)
   {
      rect rect;

      sp(tree) ptree = this;
      sp(::data::tree_item) pitem = data.m_pitem;

      sp(image_list) pimagelistItem = pitem->get_image_list();
      sp(image_list) pimagelistTree = get_image_list();

      bool bSelected    = ptree->is_selected(pitem.m_p);
      bool bHover       = ptree->is_hover(pitem);

      data.m_pdc->set_alpha_mode(::draw2d::alpha_mode_blend);

      if(m_uchHoverAlpha > 0)
      {

         if(ptree != NULL && pimagelistTree != NULL && data.m_pitem->m_dwState & ::data::tree_item_state_expandable)
         {

            _001GetItemElementRect(rect, data, tree_element_expand_box);

            int32_t iImage;
            if(data.m_pitem->m_dwState & ::data::tree_item_state_expanded)
            {
               iImage = (int32_t) ptree->m_iImageCollapse;
            }
            else
            {
               iImage = (int32_t) ptree->m_iImageExpand;
            }
            pimagelistTree->draw(data.m_pdc, iImage, rect.top_left(), 0, m_uchHoverAlpha);
         }
      }


      //      ::aura::savings & savings = Session.savings();
      if(bSelected) // selected
      {
         if(Session.savings().is_trying_to_save(::aura::resource_processing))
         {
            data.m_pdc->FillSolidRect(
               data.m_rect,
               RGB(96,96,96));
         }
         else
         {
            class rect rectUnion;
            _001GetItemElementRect(
               rect,
               data,
               tree_element_image);
            rectUnion = rect;
            _001GetItemElementRect(
               rect,
               data,
               tree_element_text);
            rectUnion.unite(rect, rectUnion);
            class imaging & imaging = System.visual().imaging();
            COLORREF crTranslucid = RGB(0, 0, 0);
            imaging.color_blend(
               data.m_pdc,
               rectUnion.left, rectUnion.top,
               rectUnion.width(), rectUnion.height(),
               crTranslucid, 127);
         }
      }

      if(pimagelistItem != NULL)
      {
         int32_t iImage = (int32_t) data.m_pitem->get_image();
         if(iImage >= 0)
         {
            _001GetItemElementRect(
               rect,
               data,
               tree_element_image);
            pimagelistItem->draw(
               data.m_pdc,
               iImage,
               rect.top_left(),
               0);
         }
      }


      string strItem = data.m_pitem->get_text();

      if(strItem.has_char() && _001GetItemElementRect(rect, data, tree_element_text))
      {

         ::draw2d::brush_sp brushText(allocer());

         COLORREF crText = ARGB(255, 0, 0, 0);

         get_color(crText, color_text);

         if(bSelected) // selected
         {
            if(bHover)
            {
               brushText->create_solid(_001GetColor(color_text_selected_highlight));
            }
            else
            {
               brushText->create_solid(_001GetColor(color_text_selected));
            }
         }
         else
         {
            if(bHover)
            {
               brushText->create_solid(_001GetColor(color_text_highlight));
            }
            else
            {
               brushText->create_solid(_001GetColor(color_text));
            }
         }


         data.m_pdc->SelectObject(brushText);


         //::draw2d::font_sp font(allocer());


         //font->operator=(*System.visual().fonts().GetListCtrlFont());
         //font->set_bold();
         //data.m_pdc->set_font(font);
         select_font(data.m_pdc, font_tree, this);
         data.m_pdc->_DrawText(strItem, strItem.get_length(), rect, m_iDrawTextFlags);

      }

   }


   index tree::_001GetIndentation()
   {
      return 24;
   }




   void tree::_001OnMouseMove(signal_details * pobj)
   {
      track_mouse_leave();

      UpdateHover();
      pobj->m_bRet = true;
   }

   void tree::_001OnMouseLeave(signal_details * pobj)
   {
      m_pitemHover = NULL;
      RedrawWindow();
      pobj->m_bRet = true;
   }

   void tree::_001OnLButtonDblClk(signal_details * pobj)
   {
      
      SCAST_PTR(::message::mouse, pmouse, pobj);
      
      _001OnClick((UINT) pmouse->m_nFlags, pmouse->m_pt);

      sp(::data::tree_item) pitem;

      ::user::e_tree_element eelement;

      pitem = _001HitTest(pmouse->m_pt, eelement);

      if(pitem != NULL)
      {
      
         if(eelement == tree_element_expand_box || eelement == tree_element_image || eelement == tree_element_text)
         {

            _001ExpandItem(pitem, ::action::source_user, !(pitem->m_dwState & ::data::tree_item_state_expanded));

         }

      }

   }


   void tree::_001OnLButtonDown(signal_details * pobj)
   {

      SCAST_PTR(::message::mouse, pmouse, pobj);

      pmouse->previous();
      
      pobj->m_bRet = true;
   
      pmouse->set_lresult(1);

   }


   void tree::_001OnLButtonUp(signal_details * pobj)
   {
      
      SCAST_PTR(::message::mouse, pmouse, pobj);

      m_uiLButtonUpFlags = (UINT) pmouse->m_nFlags;

      m_ptLButtonUp = pmouse->m_pt;

      perform_click();
      
      pobj->m_bRet = true;

      pmouse->set_lresult(1);

   }

   void tree::perform_click()
   {
      
      UINT nFlags = m_uiLButtonUpFlags;

      point point = m_ptLButtonUp;

      _001OnClick(nFlags, point);

      sp(::data::tree_item) pitem;
      
      ::user::e_tree_element eelement;
      
      ScreenToClient(&point);

      pitem = _001HitTest(point, eelement);

      if(pitem != NULL)
      {
         
         if(eelement == tree_element_expand_box)
         {

            _001ExpandItem(pitem, ::action::source_user, !(pitem->m_dwState & ::data::tree_item_state_expanded));

         }
         else if(eelement == tree_element_image || eelement == tree_element_text)
         {

            _001OnOpenItem(pitem, ::action::source_user);

         }

      }

   }


   void tree::perform_right_click(uint_ptr nFlags, point pointCursor)
   {

      _001OnRightClick(nFlags, pointCursor);

      point point(pointCursor);

      sp(::data::tree_item) pitem;

      ::user::e_tree_element eelement;

      ScreenToClient(&point);

      pitem = _001HitTest(point, eelement);

      if (pitem != NULL)
      {

         if (eelement == tree_element_image || eelement == tree_element_text)
         {

            _001OnItemContextMenu(pitem, ::action::source_user, this, pointCursor);

         }

      }

   }


   void tree::_001OnRButtonDown(signal_details * pobj)
   {

      SCAST_PTR(::message::mouse, pmouse, pobj);

      pmouse->previous();

      pobj->m_bRet = true;

      pmouse->set_lresult(1);

   }


   void tree::_001OnRButtonUp(signal_details * pobj)
   {

      SCAST_PTR(::message::mouse, pmouse, pobj);

      perform_right_click(pmouse->m_nFlags, pmouse->m_pt);

      pobj->m_bRet = true;

      pmouse->set_lresult(1);

   }


   void tree::_001OnTimer(::timer * ptimer)
   {
      ::user::control::_001OnTimer(ptimer);

         //         return; //xxxtimer

         if(ptimer->m_nIDEvent == TimerHover)
         {
            UpdateHover();
         }
         else
         {


         }
   }


   bool tree::_001OnClick(uint_ptr uiFlags, point point)
   {

      return false;

   }


   bool tree::_001OnRightClick(uint_ptr uiFlags, point point)
   {

      return false;

   }
   


   sp(::data::tree_item) tree::_001HitTest(POINT pt, ::user::e_tree_element & eelement)
   {
      index iy = pt.y;

      index iItem = -1;

      index iItemHeight = _001GetItemHeight();

      point ptOffset = get_viewport_offset();

      if(iItemHeight != 0)
      {
         iItem = (int32_t) ((iy + ptOffset.y) / iItemHeight);
      }

      if(iItem < 0)
         return NULL;


      //   if(iItem >= _001StaticGetItemCount())
      //      return false;

      synch_lock sl(m_treeptra.has_elements() ? m_treeptra[0]->m_pmutex : NULL);
      
      sp(::data::tree_item) pitem = get_proper_item(iItem);

      if(pitem == NULL)
         return NULL;

      


      index iLevel = pitem->m_iLevel;

      index x = (int32_t) (pt. x - _001GetIndentation() * (iLevel) + ptOffset.x);
      if(x >= 0 && x < 16)
         eelement = tree_element_expand_box;
      if(x >= 18 && x < 34)
         eelement = tree_element_image;
      if(x >= 36)
         eelement = tree_element_text;

      return pitem;


   }

   int32_t tree::_001GetItemHeight()
   {
      return m_iItemHeight;
   }

   void tree::install_message_handling(::message::dispatch * pdispatch)
   {
      
      ::user::control::install_message_handling(pdispatch);

      IGUI_WIN_MSG_LINK(WM_CREATE        , pdispatch, this, &tree::_001OnCreate);
      IGUI_WIN_MSG_LINK(WM_LBUTTONDBLCLK , pdispatch, this, &tree::_001OnLButtonDblClk);
      IGUI_WIN_MSG_LINK(WM_LBUTTONUP     , pdispatch, this, &tree::_001OnLButtonUp);
      IGUI_WIN_MSG_LINK(WM_LBUTTONDOWN   , pdispatch, this, &tree::_001OnLButtonDown);
      IGUI_WIN_MSG_LINK(WM_RBUTTONUP     , pdispatch, this, &tree::_001OnRButtonUp);
      IGUI_WIN_MSG_LINK(WM_RBUTTONDOWN   , pdispatch, this, &tree::_001OnRButtonDown);
      IGUI_WIN_MSG_LINK(WM_MOUSEMOVE     , pdispatch, this, &tree::_001OnMouseMove);
      IGUI_WIN_MSG_LINK(WM_MOUSELEAVE    , pdispatch, this, &tree::_001OnMouseLeave);
      IGUI_WIN_MSG_LINK(WM_HSCROLL       , pdispatch, this, &tree::_001OnHScroll);
      IGUI_WIN_MSG_LINK(WM_VSCROLL       , pdispatch, this, &tree::_001OnVScroll);
//      //IGUI_WIN_MSG_LINK(WM_TIMER         , pdispatch, this, &tree::_001OnTimer);

   }


   void tree::on_change_view_size()
   {

      m_sizeTotal.cx = m_iCurrentViewWidth;

      m_sizeTotal.cy = (LONG)(get_proper_item_count() * _001GetItemHeight());

      ::user::control::on_change_view_size();

   }


   bool tree::_001GetItemElementRect(
      LPRECT lprect,
      ::user::tree_draw_item &drawitem,
      ::user::e_tree_element eelement)
   {
      switch(eelement)
      {
      case tree_element_expand_box:
         {
            lprect->left   = drawitem.m_rect.left;
            lprect->right  = MIN(lprect->left + 16, drawitem.m_rect.right);
            lprect->top    = drawitem.m_rect.top;
            lprect->bottom = drawitem.m_rect.bottom;
         }
         break;
      case tree_element_image:
         {
            lprect->left   = drawitem.m_rect.left + 18;
            lprect->right  = MIN(lprect->left + 16, drawitem.m_rect.right);

            int iHDiff = 0;
            if (m_pimagelist != NULL)
            {

               iHDiff = drawitem.m_rect.height() - m_pimagelist->m_size.cy;

            }

            lprect->top    = drawitem.m_rect.top +iHDiff/2;
            lprect->bottom = drawitem.m_rect.bottom - iHDiff / 2;
         }
         break;
      case tree_element_text:
         {
            lprect->left   = drawitem.m_rect.left + 38;
            lprect->right  = drawitem.m_rect.right;
            lprect->top    = drawitem.m_rect.top;
            lprect->bottom = drawitem.m_rect.bottom;
         }
         break;
      }
      return true;
   }

   void tree::_001SelectItem(::data::tree_item * pitem)
   {

      if (is_selected(pitem))
         return;

      selection_set(pitem);

   }

   void tree::_001ExpandItem(::data::tree_item * pitem, ::action::context actioncontext, bool bExpand, /* = true */ bool bRedraw, /*=true*/ bool bLayout /*=true*/)
   {
      
      //::data::simple_lock lock(pitem->m_pitem);

      UNREFERENCED_PARAMETER(bLayout);

      point ptOffset = get_viewport_offset();

      if(bExpand)
      {

         if(!(pitem->m_dwState & ::data::tree_item_state_expanded))
         {

            pitem->m_dwState |= ::data::tree_item_state_expanded;

            _001OnItemExpand(pitem, actioncontext);


            // scroll properly to show the highest possible number
            // of children while trying to preserve the old position and
            // never passing
            index iLevel = 0;
            index iParentIndex = get_proper_item_index(pitem, &iLevel);

            index iLastChildIndex = iParentIndex + pitem->get_proper_descendant_count();

            index iLastVisibleIndex = (index) (ptOffset.y / _001GetItemHeight() + _001GetVisibleItemCount()) + 1;

            

            index iObscured; // obscured proper descendants
            iObscured = iLastChildIndex  - iLastVisibleIndex;

            if(iObscured > 0)
            {
               
               index iNewScroll = (int32_t) (ptOffset.y + iObscured * _001GetItemHeight());
               
               if(iNewScroll > (iParentIndex * _001GetItemHeight()))
                  iNewScroll = (iParentIndex * _001GetItemHeight());

               set_viewport_offset_y((int) MAX(iNewScroll, 0));
               //            _001SetYScroll(MAX(iNewScroll, 0), false);
               //m_pscrollbarVert->_001SetScrollPos(ptOffset.y);
            }
         }
      }
      else
      {
         if ((pitem->m_dwState & ::data::tree_item_state_expanded))
         {
            
            pitem->m_dwState &= ~::data::tree_item_state_expanded;

            _001OnItemCollapse(pitem, actioncontext);

         }
      }

      set_need_layout();
      if(bRedraw)
      {
         RedrawWindow();
      }

   }

   sp(::data::tree) tree::find_tree(::data::tree_item * pitem) const
   {

      for (int i = 0; i < m_treeptra.get_count(); i++)
      {

         if (m_treeptra[i]->contains(pitem))
            return m_treeptra[i];

      }


      return NULL;

   }

   sp(::data::tree_item) tree::find(::data::item * pitem, index * piIndex)
   {

      sp(::data::tree_item) pitemFound;

      for (int iTree = 0; iTree < m_treeptra.get_count(); iTree++)
      {

         pitemFound = m_treeptra[iTree]->find(pitem, piIndex);

         if (pitemFound.is_set())
            return pitemFound;

      }

      return NULL;

   }

   bool tree::contains(::data::item * pitem)
   {

      return find(pitem).is_set();

   }

   bool tree::contains(::data::tree_item * pitem)
   {

      for (int iTree = 0; iTree < m_treeptra.get_count(); iTree++)
      {

         if (m_treeptra[iTree]->contains(pitem))
            return true;

      }

      return false;

   }
   void tree::_001OnItemExpand(::data::tree_item * pitem, ::action::context actioncontext)
   {
      
      sp(::data::tree) ptree = find_tree(pitem);

      if (ptree.is_set())
      {

         ptree->_001OnItemExpand(pitem, actioncontext);

      }

   }

   void tree::_001OnItemCollapse(::data::tree_item * pitem, ::action::context actioncontext)
   {

      sp(::data::tree) ptree = find_tree(pitem);

      if (ptree.is_set())
      {

         ptree->_001OnItemCollapse(pitem, actioncontext);

      }

   }

   void tree::_001OnVScroll(signal_details * pobj)
   {

      pobj->previous();


   }


   void tree::_001OnHScroll(signal_details * pobj)
   {

      pobj->previous();

   }

   void tree::on_change_viewport_offset()
   {

      m_pitemFirstVisible = CalcFirstVisibleItem(m_iFirstVisibleItemProperIndex);

      UpdateHover();

   }


   /*   void tree::on_change_view_size()
   {
   rect64 rectTotal;

   _001GetViewRect(&rectTotal);

   size sizeTotal = rectTotal.size();

   m_sizeTotal = sizeTotal;

   rect rectViewClient;
   _001GetViewClientRect(&rectViewClient);

   m_sizeTotal = sizeTotal;
   m_scrolldata.m_sizePage = rectViewClient.size();


   if(ptOffset.y > (m_sizeTotal.cy - m_scrolldata.m_sizePage.cy))
   {
   ptOffset.y = (m_sizeTotal.cy - m_scrolldata.m_sizePage.cy);
   }


   }*/

   void tree::on_layout()
   {

      ::user::control::on_layout();

      ::rect rectClient;

      GetClientRect(rectClient);

      if(rectClient.area() <= 0)
         return;


      if (m_puserschemaSchema == NULL)
      {

         m_puserschemaSchema = GetTopLevel();

      }

      if (m_puserschemaSchema == NULL)
      {

         m_puserschemaSchema = Application.userschema();

      }

      m_pitemFirstVisible = CalcFirstVisibleItem(m_iFirstVisibleItemProperIndex);

      m_iCurrentViewWidth = _001CalcTotalViewWidth();

      m_sizeTotal.cx = m_iCurrentViewWidth;

      m_sizeTotal.cy = _001CalcTotalViewHeight();

      on_change_view_size();

      on_change_viewport_offset();

   }


   void tree::_001OnOpenItem(::data::tree_item * pitem, ::action::context actioncontext)
   {

      if (actioncontext.contains(this))
         return;

      actioncontext.add(this);

      // TODO: if(m_bSelectOnClik) if(m_bInAJungleFeel)
      // {
      //_001SelectItem(pitem);
      // } END TODO

      pitem->m_ptree->_001OnOpenItem(pitem, actioncontext);

      // TODO: if(m_bSelectOnClik) if(m_bGlueFeel)
      // {
      _001SelectItem(pitem);
      // } END TODO

   }


   void tree::_001OnItemContextMenu(::data::tree_item * pitem, ::action::context actioncontext, ::user::tree * ptree, point pt)
   {

      if (actioncontext.contains(this))
         return;

      actioncontext.add(this);

      pitem->m_ptree->_001OnItemContextMenu(pitem, actioncontext, ptree, pt);

   }


   void tree::UpdateHover()
   {
      point pt;
      Session.get_cursor_pos(&pt);
      ScreenToClient(&pt);
      ::user::e_tree_element eelement;
      sp(::data::tree_item) pitem = _001HitTest(pt, eelement);
      if(eelement != tree_element_image &&
         eelement != tree_element_text)
      {
         pitem = NULL;
      }
      if(pitem != m_pitemHover)
      {
         m_pitemHover = pitem;
         RedrawWindow();
      }
      /*if(pitem != NULL)
      {
      SetTimer(TimerHover, 250, NULL);
      }
      else
      {
      KillTimer(TimerHover);
      }*/
   }

   void tree::_001SetExpandImage(const char * pszMatter)
   {
      m_iImageExpand = m_pimagelist->add_matter(pszMatter);
   }

   void tree::_001SetCollapseImage(const char * pszMatter)
   {
      m_iImageCollapse = m_pimagelist->add_matter(pszMatter);
   }

   ::count tree::_001GetVisibleItemCount()
   {
      if(_001GetItemHeight() == 0)
         return 0;

      rect rect;
      GetClientRect(rect);

      return rect.height() / _001GetItemHeight() - 1;
   }

   int32_t tree::_001CalcCurrentViewWidth()
   {
      rect rectClient;
      GetClientRect(rectClient);

      ::count iCount = _001GetVisibleItemCount();
      int32_t iMaxWidth = rectClient.width();
      int32_t iWidth;
      index iIndent  = _001GetIndentation();
      sp(::data::tree_item) pitem = m_pitemFirstVisible;
      if(pitem == NULL)
         return iMaxWidth;
      for(int32_t i = 0; i < iCount; i++)
      {
         iWidth = (int32_t) (200 + iIndent * pitem->m_iLevel);
         if(iWidth > iMaxWidth)
         {
            iMaxWidth = iWidth;
         }
         pitem = pitem->get_item(::data::TreeNavigationProperForward);
         if(pitem == NULL)
            break;
      }

      return iMaxWidth;

   }


   sp(::data::tree_item) tree::CalcFirstVisibleItem(index & iProperIndex)
   {
      
      synch_lock sl(m_treeptra.has_elements() ? m_treeptra[0]->m_pmutex : NULL);

      index nOffset;
      
      if(_001GetItemHeight() == 0)
         return NULL;

      point ptOffset = get_viewport_offset();

      int iItemHeight = _001GetItemHeight();

      nOffset = ptOffset.y / iItemHeight;
      
      ::data::tree_item * pitem = NULL;

      iProperIndex = 0;

      for (index i = 0; i < m_treeptra.get_count(); i++)
      {

         pitem = m_treeptra[i]->get_base_item();

         for (;;)
         {

            ::data::tree_item * pitemNext = pitem->get_item(::data::TreeNavigationProperForward);

            if (pitemNext == NULL)
            {

               break;

            }

            pitem = pitemNext;

            if (nOffset <= 0)
            {
               
               return pitem;

            }

            nOffset--;

            iProperIndex++;

         }

      }

      return pitem;

   }


   int32_t tree::_001CalcTotalViewWidth()
   {

      index nOffset;

      if (_001GetItemHeight() == 0)
         return NULL;

      point ptOffset = get_viewport_offset();

      nOffset = ptOffset.y / _001GetItemHeight();

      nOffset = INT_MAX;

      ::data::tree_item * pitem = NULL;

      int iWidth;
      int iMaxWidth = 0;
      index iLevel = 0;
      int iIndent = _001GetIndentation();

      ::draw2d::graphics_sp g(allocer());

      g->CreateCompatibleDC(NULL);

      //::draw2d::font_sp font(allocer());


      //font->operator=(*System.visual().fonts().GetListCtrlFont());
      //font->set_bold();
      //g->set_font(font);

      select_font(g, font_tree, this);


      size size;
      size = g->GetTextExtent(unitext("Ág"));

      int iItemHeight = 1;

      if (size.cy > iItemHeight)
      {
         iItemHeight = MAX(size.cy, iItemHeight);
      }

      m_iItemHeight = iItemHeight;

      on_ui_event(event_calc_item_height, object_tree, this);

      spa(::data::tree_item) spitema;
      
      for (index i = 0; i < m_treeptra.get_count(); i++)
      {

         pitem = m_treeptra[i]->get_base_item();
         iLevel = 0;

         for (;;)
         {
            pitem = pitem->get_item(::data::TreeNavigationProperForward, &iLevel);
            if (pitem == NULL)
               break;
            if (spitema.find_first(pitem)>= 0)
            {

               output_debug_string("what!?!");

               break;

            }
            spitema.add(pitem);
            if (nOffset <= 0)
            {
               break;
            }
            nOffset--;
            string strText = pitem->get_text();
            sized s = g->GetTextExtent(strText);
            iWidth = (int32_t)(48 + s.cx + iIndent * (iLevel + 1));
            if (iWidth > iMaxWidth)
            {
               iMaxWidth = iWidth;
            }
         }


      }

      return iMaxWidth;

   }

   int tree::_001CalcTotalViewHeight()
   {

      return _001GetProperItemCount() & _001GetItemHeight();

   }

   count tree::_001GetProperItemCount()
   {

      ::count iProperCount = 0;

      index nOffset;

      index iLevel;

      ::data::tree_item * pitem = NULL;
      spa(::data::tree_item) spitema;
      for (index i = 0; i < m_treeptra.get_count(); i++)
      {

         pitem = m_treeptra[i]->get_base_item();

         for (;;)
         {
            pitem = pitem->get_item(::data::TreeNavigationProperForward, &iLevel);
            if (pitem == NULL)
               break;
            if (pitem == m_treeptra[i]->get_base_item())
               break;
            if (spitema.find_first(pitem) >= 0)
            {
               output_debug_string("what!!?");
               break;

            }
            spitema.add(pitem);
            iProperCount++;
         }

      }

      return iProperCount;

   }

   void tree::_001OnTreeDataChange()
   {
      on_layout();
   }

   sp(image_list) tree::get_image_list()
   {
      return m_pimagelist;
   }

 

   int32_t tree::get_wheel_scroll_delta()
   {


      return 3 * m_iItemHeight;


   }


   bool tree::selection_add(::data::item * pitemdata, index i)
   {
      sp(::data::tree_item) pitem = find(pitemdata, &i);
      if (pitem == NULL)
         return false;
      return selection_add(pitem);
   }

   ::count tree::selection_add(::data::tree_item_ptr_array & itemptra)
   {
      ::count count = 0;
      for (int32_t i = 0; i < itemptra.get_count(); i++)
      {
         if (selection_add(itemptra[i]))
            count++;
      }
      return count;
   }

   bool tree::selection_add(::data::tree_item * pitem)
   {
      if (!contains(pitem))
         return false;
      return m_itemptraSelected.add_unique(pitem);
   }

   bool tree::selection_set(::data::item * pitemdata, bool bIfNotInSelection, bool bIfParentInSelection)
   {

      return selection_set(0, pitemdata, bIfNotInSelection, bIfParentInSelection);

   }

   bool tree::selection_set(index i, ::data::item * pitemdata, bool bIfNotInSelection, bool bIfParentInSelection)
   {
      sp(::data::tree_item) pitem = find(pitemdata, &i);
      if (pitem == NULL)
         return false;
      return selection_set(pitem, bIfNotInSelection, bIfParentInSelection);
   }

   ::count tree::selection_set(::data::tree_item_ptr_array & itemptra)
   {
      ::count count = 0;
      for (int32_t i = 0; i < itemptra.get_count(); i++)
      {
         if (contains(itemptra[i]) && m_itemptraSelected.add_unique(itemptra[i]))
            count++;
      }
      return count;
   }

   bool tree::selection_set(::data::tree_item * pitem, bool bIfNotInSelection, bool bIfParentInSelection)
   {
      
      if (!contains(pitem))
         return false;
      
      if (bIfNotInSelection && is_selected(pitem))
         return true;
   
      if (bIfParentInSelection && !is_selected(pitem->m_pparent))
         return true;

      bool bContains = m_itemptraSelected.contains(pitem);

      m_itemptraSelected.remove_all();

      m_itemptraSelected.add(pitem);

      return bContains;

   }


   bool tree::selection_remove(::data::item * pitemdata, index i)
   {
      sp(::data::tree_item) pitem = find(pitemdata, &i);
      if (pitem == NULL)
         return false;
      return selection_remove(pitem);
   }

   ::count tree::selection_remove(::data::tree_item_ptr_array & itemptra)
   {
      ::count count = 0;
      for (int32_t i = 0; i < itemptra.get_count(); i++)
      {
         if (m_itemptraSelected.remove(itemptra[i]))
            count++;
      }
      return count;
   }

   bool tree::selection_remove(::data::tree_item * pitem)
   {
      return m_itemptraSelected.remove(pitem) >= 0;
   }


   ::count tree::clear_selection()
   {
      return m_itemptraSelected.remove_all();
   }

   bool tree::hover(::data::item * pitemdata, index i)
   {
      sp(::data::tree_item) pitem = find(pitemdata, &i);
      return hover(pitem);
   }

   bool tree::hover(::data::tree_item * pitem)
   {
      if (pitem == NULL)
      {
         m_pitemHover = NULL;
         return true;
      }
      if (!contains(pitem))
         return false;
      return m_pitemHover == pitem;
   }


   bool tree::is_selected(const ::data::tree_item * pitem) const
   {

      return m_itemptraSelected.contains(pitem);

   }


   bool tree::is_selected(const ::data::item * pitemdata) const
   {

      if (pitemdata == NULL)
         return false;

      for (int32_t i = 0; i < m_itemptraSelected.get_count(); i++)
      {

         if (m_itemptraSelected[i]->m_pitem == pitemdata)
            return true;

      }

      return false;

   }


   bool tree::is_hover(const ::data::tree_item * pitem) const
   {
      return pitem != NULL && m_pitemHover == pitem;
   }

   bool tree::is_hover(const ::data::item * pitem) const
   {
      if (pitem == NULL)
         return false;
      if (m_pitemHover == NULL)
         return false;
      if (m_pitemHover->m_pitem != pitem)
         return false;
      return true;
   }



   bool tree::can_merge(const ::data::tree * ptree) const
   {

      return !m_treeptra.contains(ptree);

   }


   bool tree::merge(::data::tree * pdatatree, bool bBind)
   {

      if(pdatatree == NULL)
         return false;

      ::user::tree_data * ptree = dynamic_cast <::user::tree_data *>  (pdatatree);

      if (ptree == NULL)
         return false;

      if (!can_merge(ptree))
         return false;

      m_treeptra.add(ptree);

      ptree->m_treeptra.add(this);

      ptree->on_merge_user_tree(this);

      if (bBind)
      {

         m_treeptraBound.add(ptree);

         ptree->m_treeptraBound.add(this);

         ptree->on_bind_user_tree(this);

      }

      sp(::user::impact) pview = this;

      if(pview.is_set())
      {

         ptree->m_pcontainerbase = (::data::data_container_base *) pview->get_document();

      }

      ptree->install_message_handling(m_pimpl);

      return true;


   }

   sp(::data::tree_item) tree::get_proper_item(index i, index * piLevel, index * piCount)
   {

      index iCount = 0;

      sp(::data::tree_item) pitem;

      for (index iTree = 0; iTree < m_treeptra.get_count(); iTree++)
      {

         if (piLevel != NULL)
            *piLevel = 1;

         pitem = m_treeptra[iTree]->get_proper_item(i - iCount, piLevel, &iCount);

         if (pitem.is_set())
            return pitem;

      }


      if (piCount != NULL)
         *piCount += iCount;

      return NULL;

   }


   index tree::get_proper_item_index(::data::tree_item *pitemParam, index * piLevel)
   {

      index iCount = 0;

      index iFound;

      for (index i = 0; i < m_treeptra.get_count(); i++)
      {

         if (piLevel != NULL) 
            *piLevel = 1;

         iFound = m_treeptra[i]->get_proper_item_index(pitemParam, piLevel, &iCount);

         if (iFound >= 0)
            return iCount;

      }

      return -1;

   }


   index tree::get_proper_item_count()
   {
      synch_lock sl(m_pmutex);
      index iCount = 0;

      for (index i = 0; i < m_treeptra.get_count(); i++)
      {

         iCount++; // per tree

         iCount += m_treeptra[i]->get_proper_item_count();

      }

      return iCount;

   }


   void tree::_001EnsureVisible(::data::tree_item * pitem)
   {

      if (pitem == NULL)
         return;

      ::data::tree_item * pitemExpand = pitem;

      while(pitemExpand != NULL)
      {
         pitemExpand->m_dwState |=  ::data::tree_item_state_expanded;

         pitemExpand = pitemExpand->m_pparent;
      }

      index iLevel = 0;

      index iIndex = get_proper_item_index(pitem, &iLevel);

      point ptOffset = get_viewport_offset();

      index iMinVisibleIndex = (index)(ptOffset.y / m_iItemHeight);
      index iMaxVisibleIndex = (index)(iMinVisibleIndex + _001GetVisibleItemCount());


      if (iIndex < iMinVisibleIndex || iIndex > iMaxVisibleIndex)
      {
         
         index iNewScrollIndex = iIndex;

         set_viewport_offset_y((int) MAX(iNewScrollIndex,0) * m_iItemHeight);

         on_layout();

         RedrawWindow();

      }


   }

   void tree::on_update(::user::impact * pSender, LPARAM lHint, ::object* pHint)
   {

      if (pSender != NULL)
         pSender = dynamic_cast < ::user::impact * > (this);

      for (index iTree = 0; iTree < m_treeptra.get_count(); iTree++)
      {

         m_treeptra[iTree]->on_update(pSender, lHint, pHint);

      }

   }


   bool tree::keyboard_focus_is_focusable()
   {

      return true;

   }

   size tree::get_total_size()
   {

      return m_sizeTotal;

   }


   void tree::on_viewport_offset(::draw2d::graphics * pgraphics)
   {

   }


} // namespace core


