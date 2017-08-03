#include "framework.h"


namespace user
{


   form_list::form_list()
   {


      m_iControlItem = -1;
      m_iControlSubItem = -1;
      m_pcontrolEdit = NULL;


      m_iOnClickClickCount    = 0;
      m_iOnClickClickItem     = -1;
      m_iOnClickClickSubItem  = -1;

   }

   form_list::~form_list()
   {

   }

   void form_list::_001GetSelection(
      ::database::id & key,::database::selection &selection)
   {
      list::_001GetSelection(key,selection);
   }

   void form_list::install_message_handling(::message::dispatch *pinterface)
   {
      IGUI_WIN_MSG_LINK(WM_KEYDOWN,pinterface,this,&form_list::_001OnKeyDown);

      form_mesh::install_message_handling(pinterface);
      list::install_message_handling(pinterface);

      IGUI_WIN_MSG_LINK(WM_VSCROLL, pinterface, this, &form_list::_001OnVScroll);
      IGUI_WIN_MSG_LINK(WM_HSCROLL, pinterface, this, &form_list::_001OnHScroll);

   }


   bool form_list::_001OnRightClick(uint_ptr uiFlags, point point)
   {

      list::_001OnClick(uiFlags, point);

      index iItem;

      index iSubItem;

      if (_001HitTest_(point, iItem, iSubItem))
      {

         _001OnRightClick(uiFlags, point, iItem, iSubItem);

      }
      else
      {

         // Clicked in empty area

         _001HideEditingControls();

      }

      return true;

   }


   bool form_list::_001OnRightClick(uint_ptr uiFlags, point point, index iItem, index iSubItem)
   {

      sp(control) pcontrol = _001GetControl(iItem, iSubItem);

      if (pcontrol != NULL)
      {

         if (pcontrol->descriptor().has_function(::user::control::function_action))
         {

            if (pcontrol->descriptor().get_type() == ::user::control_type_button)
            {


               Session.m_puiLastLButtonDown = NULL;

               ::user::control_event ev;

               ev.m_puie = pcontrol;

               ev.m_eevent = ::user::event_button_clicked;

               m_iControlItem = iItem;

               send_message(::message::message_event, 0, (LPARAM)&ev);

            }

         }
         else if (m_columna.get_by_subitem(iSubItem)->m_bEditOnSecondClick)
         {

            if (m_iOnClickClickItem == iItem && m_iOnClickClickSubItem == iSubItem)
            {

               m_iOnClickClickCount++;

            }
            else
            {

               m_iOnClickClickCount = 1;

            }

            m_iOnClickClickItem = iItem;

            m_iOnClickClickSubItem = iSubItem;

            if (m_iOnClickClickCount == 2)
            {

               m_iOnClickClickCount = 0;

               _001PlaceControl(pcontrol, iItem);

            }

         }
         else
         {

            _001PlaceControl(pcontrol, iItem, true);

         }

      }
      else
      {

         // Click in subitem without editing controls.

         _001HideEditingControls();

      }

      return true;

   }





   bool form_list::_001OnClick(uint_ptr uiFlags,point point)
   {
      
      list::_001OnClick(uiFlags,point);

      index iItem;
      
      index iSubItem;

      if(_001HitTest_(point,iItem,iSubItem))
      {

         _001OnClick(uiFlags,point,iItem,iSubItem);

      }
      else
      {

         // Clicked in empty area

         _001HideEditingControls();

      }

      return true;

   }


   bool form_list::_001OnClick(uint_ptr uiFlags,point point,index iItem,index iSubItem)
   {

      sp(control) pcontrol = _001GetControl(iItem, iSubItem);

      if(pcontrol != NULL)
      {

         if(pcontrol->descriptor().has_function(::user::control::function_action))
         {

            if(pcontrol->descriptor().get_type() == ::user::control_type_button)
            {


               Session.m_puiLastLButtonDown      = NULL;

               ::user::control_event ev;
            
               ev.m_puie                  = pcontrol;
               
               ev.m_eevent                = ::user::event_button_clicked;

               m_iControlItem             = iItem;

               send_message(::message::message_event,0,(LPARAM)&ev);

            }

         }
         else if(m_columna.get_by_subitem(iSubItem)->m_bEditOnSecondClick)
         {
            
            if(m_iOnClickClickItem == iItem && m_iOnClickClickSubItem == iSubItem)
            {

               m_iOnClickClickCount++;

            }
            else
            {

               m_iOnClickClickCount = 1;

            }

            m_iOnClickClickItem = iItem;

            m_iOnClickClickSubItem = iSubItem;

            if(m_iOnClickClickCount == 2)
            {

               m_iOnClickClickCount = 0;

               _001PlaceControl(pcontrol, iItem);

            }

         }
         else
         {
      
            _001PlaceControl(pcontrol, iItem, true);

         }

      }
      else
      {

         // Click in subitem without editing controls.

         _001HideEditingControls();

         sp(::user::list_column) pcolumn = m_columna.get_by_subitem(iSubItem);

         if (pcolumn.is_set() && pcolumn->m_iControl >= 0)
         {

            sp(class ::user::control::descriptor) pdescriptor = m_controldescriptorset.sp_at(pcolumn->m_iControl);

            if (pdescriptor.is_set())
            {

               if (pdescriptor->has_function(::user::control::function_check_box))
               {

                  ::check::e_check echeck = _001GetSubItemCheck(iItem, iSubItem);

                  if(echeck == ::check::checked)
                  {

                     echeck = ::check::unchecked;

                  }
                  else
                  {

                     echeck = ::check::checked;

                  }

                  _001SetSubItemCheck(iItem, iSubItem, echeck);

                  if (echeck == ::check::checked)
                  {

                     for (auto & pitem : m_controldescriptorset)
                     {

                        if (pitem->has_function(::user::control::function_duplicate_on_check_box))
                        {

                           if (pitem->m_iSubItemDuplicateCheckBox == iSubItem)
                           {

                              for (auto iSubItemTarget : pitem->m_iaSubItemDuplicate)
                              {

                                 ::user::mesh_item itemSource(this);

                                 itemSource.m_iItem = iItem;
                                 itemSource.m_iSubItem = pitem->m_iSubItem;

                                 _001GetItemText(&itemSource);

                                 if (itemSource.m_bOk)
                                 {

                                    ::user::mesh_item itemTarget(this);

                                    itemTarget.m_iItem = iItem;
                                    itemTarget.m_iSubItem = iSubItemTarget;
                                    itemTarget.m_strText = itemSource.m_strText;

                                    _001SetItemText(&itemTarget);

                                 }


                              }

                           }

                        }

                     }

                  }


                  return true;

               }

            }

         }
         else
         {

            if (m_rangeSelection.has_item(iItem))
            {

               m_rangeSelection.clear();

            }
            else
            {

               m_rangeSelection.clear();

               ::user::mesh::item_range r;

               r.m_iLowerBound = iItem;

               r.m_iUpperBound = iItem;

               m_rangeSelection.add_item(r);

            }
            // return ::user::list::_001OnClick(uiFlags, point, iItem, iSubItem);

         }


      }

      return true;

   }


   sp(control) form_list::_001GetControl(index iItem, index iSubItem)
   {

      synch_lock sl(m_pmutex);

      ::user::list_column * pcolumn = m_columna.get_by_subitem(iSubItem);

      if(pcolumn == NULL || pcolumn->m_iControl < 0)
      {

         return NULL;

      }

      sp(class ::user::control::descriptor) pdescriptor = m_controldescriptorset[pcolumn->m_iControl];

      if(pdescriptor.is_null())
      {

         return NULL;

      }

      return pdescriptor->get_control(this, iItem);

   }
   void form_list::_001OnShowControl(sp(control) pcontrol)
   {


   }

   void form_list::_001PlaceControl(sp(control) pcontrol, index iEditItem, bool bClick, bool bOnlySizeAndPosition)
   {
      
      if (!bOnlySizeAndPosition && _001GetEditControl() != NULL)
      {

         _001SaveEdit(_001GetEditControl());

      }

      if (!bOnlySizeAndPosition)
      {

         _001HideEditingControls();

      }

      draw_list_item item(this);

      pcontrol->m_iEditItem = iEditItem;

      item.m_iDisplayItem = DisplayToStrict(iEditItem);

      item.m_iItem = iEditItem;

      if (!bOnlySizeAndPosition)
      {

         _001EnsureVisible(item.m_iItem);

      }

      item.m_iSubItem = pcontrol->descriptor().m_iSubItem;

      item.m_iOrder = _001MapSubItemToOrder(item.m_iSubItem);

      item.m_iListItem = -1;

      _001GetElementRect(&item,::user::mesh::element_text);

      if(item.m_bOk)
      {

         rect rectControl(item.m_rectSubItem);

         rectControl.offset(get_viewport_offset());

         if (!bOnlySizeAndPosition)
         {

            _001Update(pcontrol);

         }

         pcontrol->SetWindowPos(
            ZORDER_TOP,
            rectControl.left,
            rectControl.top,
            rectControl.width(),
            rectControl.height(),
            SWP_SHOWWINDOW | (bOnlySizeAndPosition ? SWP_CHANGEVIEWPORTOFFSET : 0));

         if (bOnlySizeAndPosition)
         {

            pcontrol->set_need_layout();

         }
         else
         {

            _001SetEditControl(pcontrol);

            pcontrol->SetFocus();

            if (bClick)
            {

               sp(::user::plain_edit) pedit = pcontrol;

               if (pedit.is_set())
               {

                  pedit->_001SetSel(0, pedit->_001GetTextLength());

               }

            }

            if (_001IsSubItemEnabled(iEditItem, item.m_iSubItem))
            {

               pcontrol->enable_window();

            }
            else
            {

               pcontrol->enable_window(false);

            }

            _001OnShowControl(pcontrol);

         }

      }

   }

   bool form_list::_001OnUpdateItemCount(uint32_t dwFlags)
   {

      _001HideEditingControls();

      bool bOk = ::user::list::_001OnUpdateItemCount(dwFlags);

      for (auto & pitem : m_controldescriptorset)
      {

         if (pitem->has_function(::user::control::function_duplicate_on_check_box))
         {

            ::count iItemCount = _001GetItemCount();

            for(index iItem = 0; iItem < iItemCount; iItem++)
            {
               
               if (_001GetSubItemCheck(iItem, pitem->m_iSubItemDuplicateCheckBox) == ::check::checked)
               {

                  for (auto iSubItemTarget : pitem->m_iaSubItemDuplicate)
                  {

                     ::user::mesh_item itemSource(this);

                     itemSource.m_iItem = iItem;
                     itemSource.m_iSubItem = pitem->m_iSubItem;

                     _001GetItemText(&itemSource);

                     if (itemSource.m_bOk)
                     {

                        ::user::mesh_item itemTarget(this);

                        itemTarget.m_iItem = iItem;
                        itemTarget.m_iSubItem = iSubItemTarget;
                        itemTarget.m_strText = itemSource.m_strText;

                        _001SetItemText(&itemTarget);

                     }

                  }

               }

            }

         }

      }


      return bOk;

   }


   void form_list::_001UpdateEdit(sp(control) pcontrol)
   {
      ASSERT(pcontrol != NULL);
      if (pcontrol == NULL)
         return;

      if (m_bOnEditUpdate)
         return;
      keep<bool> keepUpdateLock(&m_bOnEditUpdate, true, false, true);

      ASSERT(pcontrol->descriptor().get_type() == control_type_edit
         || pcontrol->descriptor().get_type() == control_type_edit_plain_text);

      sp(::user::plain_edit) pedit = pcontrol;

      //if (pedit.is_set())
      //{

      //   pedit->m_spfont = m_font;

      //}

      if (pcontrol->descriptor().has_function(control::function_vms_data_edit))
      {

         

         draw_list_item item(this);

         item.m_iItem = pcontrol->m_iEditItem;
         item.m_iSubItem = pcontrol->descriptor().m_iSubItem;

         _001GetItemText(&item);

         if (item.m_bOk)
         {

            pcontrol->_001SetText(item.m_strText, ::action::source_initialize);

         }

         //var var;
         //::database::selection selection;
         //_001GetSelection(pcontrol->descriptor().m_dataid, selection);
         //if (selection.get_item_count() > 0)
         //{
         //   ::database::selection_item & item = selection.get_item(0);
         //   sp(::user::elemental) ptext = NULL;
         //   if (get_child_by_id(pcontrol->m_id) != NULL)
         //   {
         //      ptext = get_child_by_id(pcontrol->m_id);
         //   }
         //   if (ptext == NULL && pcontrol != NULL)
         //   {
         //      ptext = pcontrol;
         //   }
         //   if (ptext == NULL)
         //      return;
         //   if (data_get(pcontrol->descriptor().m_dataid.m_id + "." + item.m_id.m_id, var))
         //   {
         //      switch (var.get_type())
         //      {
         //      case var::type_string:
         //      {
         //         string str;
         //         str = var.m_str;
         //         ptext->_001SetText(str, ::action::source_database);
         //      }
         //      break;
         //      case var::type_int32:
         //      {
         //         string str;
         //         str.Format("%d", var.int32());
         //         ptext->_001SetText(str, ::action::source_database);
         //      }
         //      break;
         //      default:
         //         ASSERT(FALSE);
         //         break;
         //      }
         //   }
         //}
      }
   }

   void form_list::_001UpdateComboBox(sp(control) pcontrol)
   {
      
      ASSERT(pcontrol != NULL);

      if (pcontrol == NULL)
         return;

      if (m_bOnEditUpdate)
         return;

      keep<bool> keepUpdateLock(&m_bOnEditUpdate, true, false, true);

      ASSERT(pcontrol->descriptor().get_type() == control_type_combo_box);

      sp(::user::combo_box) pcombo = pcontrol;

      //if (pcombo.is_set())
      //{

      //   pcombo->m_spfont = m_font;

      //}

      if (pcontrol->descriptor().has_function(control::function_data_selection))
      {



         draw_list_item item(this);

         item.m_iItem = pcontrol->m_iEditItem;
         item.m_iSubItem = pcontrol->descriptor().m_iSubItem;

         _001GetItemText(&item);

         if (item.m_bOk)
         {

            index iFind = pcombo->_001FindListText(item.m_strText);


            pcombo->_001SetCurSel(iFind, ::action::source_sync);



         }

      }
   }

   bool form_list::_001SaveEdit(sp(control) pcontrol)
   {

      if (pcontrol == NULL)
         return false;

      if (pcontrol->descriptor().get_type() == control_type_check_box)
      {
         
         sp(::user::elemental) pcheckbox = pcontrol;

         ::check::e_check echeck = pcheckbox->_001GetCheck();

         draw_list_item item(this);

         item.m_iItem = pcontrol->m_iEditItem;

         item.m_iSubItem = pcontrol->descriptor().m_iSubItem;

         if (echeck == ::check::checked)
         {

            string str = pcontrol->descriptor().m_setValue[::check::checked];
            
            if(str.has_char())
            {

               item.m_strText = str;

            }
            else
            {

               item.m_strText = "true";

            }

         }
         else if (echeck == ::check::unchecked)
         {

            string str = pcontrol->descriptor().m_setValue[::check::unchecked];

            if (str.has_char())
            {

               item.m_strText = str;

            }
            else
            {

               item.m_strText = "false";

            }

         }
         else
         {

            string str = pcontrol->descriptor().m_setValue[::check::tristate];

            if (str.has_char())
            {

               item.m_strText = str;

            }
            else
            {

               item.m_strText = "";

            }

         }

         _001SetItemText(&item);


         on_update(NULL, ::user::impact::hint_control_saved, pcontrol);

         return true;
      }

      ASSERT(pcontrol->descriptor().get_type() == control_type_edit || pcontrol->descriptor().get_type() == control_type_edit_plain_text
      || pcontrol->descriptor().get_type() == control_type_combo_box);

      sp(::user::elemental) pedit = get_child_by_id(pcontrol->m_id);

      string str;

      if (pedit == NULL)
      {

         sp(::user::elemental) ptext = pcontrol;

         if (ptext == NULL)
            return false;

         ptext->_001GetText(str);

      }
      else
      {

         pedit->_001GetText(str);

      }

      if (!pcontrol->Validate(str))
      {
         // que tal um balão para indicar o erro
         return false;
      }

      var var;
      if (!pcontrol->get_data(pedit, var))
      {
         return false;
      }

      if (!_001Validate(pcontrol, var))
      {
         return false;
      }

      if (pcontrol->descriptor().has_function(control::function_vms_data_edit)
         || pcontrol->descriptor().has_function(control::function_data_selection))
      {
         
         draw_list_item item(this);

         item.m_iItem = pcontrol->m_iEditItem;

         item.m_iSubItem = pcontrol->descriptor().m_iSubItem;

         item.m_strText = var.get_string();

         _001SetItemText(&item);

         on_update(NULL, ::user::impact::hint_control_saved, pcontrol);

         if (pcontrol->descriptor().has_function(::user::control::function_duplicate_on_check_box))
         {

            if (_001GetSubItemCheck(item.m_iItem, pcontrol->descriptor().m_iSubItemDuplicateCheckBox) == ::check::checked)
            {

               for (auto iSubItemTarget : pcontrol->descriptor().m_iaSubItemDuplicate)
               {

                  ::user::mesh_item itemTarget(this);

                  itemTarget.m_iItem = item.m_iItem;
                  itemTarget.m_iSubItem = iSubItemTarget;
                  itemTarget.m_strText = item.m_strText;

                  _001SetItemText(&itemTarget);

                  //sp(::list::column) pcolumn = m_columna.get_by_subitem(iSubItemTarget);
                  //
                  //if (pcolumn.is_set() && m_controldescriptorset.bounds(pcolumn->m_iControl))
                  //{

                  //   sp(class ::user::control::descriptor) pdescriptorTarget = m_controldescriptorset.sp_at(pcolumn->m_iControl);

                  //   if (pdescriptorTarget.is_set())
                  //   {

                  //      on_update(NULL, ::user::impact::hint_control_saved, pcontrol);
                  //   }

                  //}

               }

            }

         }

      }

      return true;

   }



   void form_list::_001SetEditControl(sp(control) pcontrol)
   {

      if(pcontrol == NULL)
      {
         if(m_pcontrolEdit != NULL)
         {
            sp(class control) pcontrolEdit = m_pcontrolEdit;
            _001HideControl(pcontrolEdit);
            m_pcontrolEdit = NULL;
         }
      }
      else
      {
         m_pcontrolEdit = pcontrol;
      }
   }

   sp(control) form_list::_001GetEditControl()
   {
      return m_pcontrolEdit;
   }

   void form_list::_001OnVScroll(signal_details * pobj)
   {
      //SCAST_PTR(::message::scroll, pscroll, pobj);
      pobj->previous();


      sp(control) pcontrol = _001GetEditControl();

      if(pcontrol != NULL)
      {

         _001PlaceControl(pcontrol, pcontrol->m_iEditItem, false, true);

      }

   }

   void form_list::_001OnHScroll(signal_details * pobj)
   {
      pobj->previous();
      if(pobj->m_bRet)
         return;

      sp(control) pcontrol = _001GetEditControl();

      if(pcontrol != NULL)
      {
         
         _001PlaceControl(pcontrol, pcontrol->m_iEditItem, false, true);

      }

   }

   bool form_list::_001OnMouseActivate(::window_sp pDesktopWnd,UINT nHitTest,UINT message,LRESULT & iResult)
   {
      UNREFERENCED_PARAMETER(pDesktopWnd);
      UNREFERENCED_PARAMETER(nHitTest);
      UNREFERENCED_PARAMETER(message);
      UNREFERENCED_PARAMETER(iResult);
      return false;
   }

   void form_list::_001OnNotify(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void form_list::_001OnTimer(::timer * ptimer)
   {
      list::_001OnTimer(ptimer);
   }

   void form_list::_001OnMessageNotify(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      // linux na verdade revamp
      /*
      lresult = user::NotifyRetContinue;

      sp(control) pcontrol = m_controldescriptorset.get_control_by_id(wparam);

      if(pcontrol == NULL)
      return true;

      user::Notify * pnotify = (user::Notify *) lparam;

      switch(pcontrol->descriptor().get_type())
      {
      case control_type_edit:
      switch(pnotify->m_uiCode)
      {
      case user::NotifyKeyDown:
      {
      user::NotifyKey * pkey = (user::NotifyKey *) pnotify;
      switch(pkey->m_uiVKey)
      {
      //             //  case VK_RETURN:
      _001SaveEdit(pcontrol);
      pcontrol->ShowWindow(SW_HIDE);
      lresult &= ~user::NotifyRetContinue;
      return true;
      }
      }
      break;
      }
      break;
      }*/
      //return true;
   }

   void form_list::_001UpdateColumns()
   {

      _001RemoveControls();

      list::_001UpdateColumns();

   }

   LRESULT form_list::_001BaseWndGetProperty(EProperty eprop,LPARAM lparam)
   {
      switch(eprop)
      {
      case list::PropertyBaseWndInterface:
      case list::PropertyDrawBaseWndInterface:
      {
         const ::user::interaction_base ** ppinterface = (const ::user::interaction_base **) lparam;
         *ppinterface = (list *)this;
      }
      return 1;
      default:
         return 0;
      }
   }


   void form_list::_001OnDraw(::draw2d::graphics * pgraphics)
   {

      list::_001OnDraw(pgraphics);

   }

   void form_list::_001DrawChildren(::draw2d::graphics * pgraphics)
   {

      form_mesh::_001DrawChildren(pgraphics);

   }


   void form_list::_001HideEditingControls()
   {

      form_mesh::_001HideEditingControls();

      //if(_001GetEditControl() != NULL)
      //{
      //   _001SetEditControl(NULL);
      //}
      //for(int32_t i = 0; i < m_controldescriptorset.get_count(); i++)
      //{
      //   class ::user::control::descriptor & descriptor = m_controldescriptorset(i);
      //   if(control.m_etype == control_type_edit
      //      || control.m_etype == control_type_edit_plain_text)
      //   {
      //      if(control.m_pcontrol != NULL)
      //      {
      //         control.m_pcontrol->ShowWindow(SW_HIDE);
      //      }
      //   }
      //}
   }


   void form_list::_001HideControl(sp(control) pcontrol)
   {
      pcontrol->ShowWindow(SW_HIDE);
   }


   void form_list::_001OnKeyDown(signal_details * pobj)
   {
      SCAST_PTR(::message::key,pkey,pobj);

         if(pkey->m_ekey == ::user::key_return)
         {
            _001SaveEdit(_001GetEditControl());
            _001HideEditingControls();
            pobj->m_bRet = true;
            return;
         }
      pkey->previous();
   }



   bool form_list::_001IsEditing()
   {
      return m_pcontrolEdit != NULL;
   }

   bool form_list::_001IsPointInside(sp(control) pcontrol,point64 point)
   {
      if(pcontrol->m_pdescriptor != NULL)
      {
         if(pcontrol->m_pdescriptor->m_etype == control_type_edit
            || pcontrol->m_pdescriptor->m_etype == control_type_edit_plain_text)
         {
            if(pcontrol == _001GetEditControl())
            {
               rect64 rectWindow;
               pcontrol->GetWindowRect(rectWindow);
               return rectWindow.contains(point) != FALSE;
            }
            else
            {
               return false;
            }
         }
      }
      rect rectControl;
      rect rect;
      draw_list_item item(this);


      return m_iControlItem >= 0 && m_iControlSubItem == pcontrol->descriptor().m_iSubItem;

      //int32_t iEditItem;
      //int32_t iEditSubItem;

      //_001DisplayHitTest(point, iEditItem, iEditSubItem);

      //      item.m_iDisplayItem = DisplayToStrict(pcontrol->m_iEditItem);
      //    item.m_iItem = pcontrol->m_iEditItem;
      item.m_iDisplayItem = DisplayToStrict(pcontrol->m_iEditItem);

      item.m_iSubItem = pcontrol->descriptor().m_iSubItem;
      item.m_iOrder = _001MapSubItemToOrder(item.m_iSubItem);
      item.m_iListItem = -1;
      //_001GetElementRect(&item, ::user::mesh::element_sub_item);
      rectControl = item.m_rectSubItem;
      ClientToScreen(rectControl);
      rect64 rectForm;
      GetWindowRect(rectForm);
      rect64 rectClient;
      rectClient.top = rectForm.top;
      rectClient.bottom = rectForm.bottom;
      rectClient.left = rectControl.left;
      rectClient.right = rectControl.right;
      return rectClient.contains(point) != FALSE;
   }


   void form_list::_001OnColumnChange()
   {

      ::user::list::_001OnColumnChange();


      {

         synch_lock sl(m_pmutex);

         for (int32_t i = 0; i < m_columna.get_size(); i++)
         {
            if (m_columna[i]->m_iControl >= 0 && m_columna[i]->m_iControl < m_controldescriptorset.get_size())
            {
               class control::descriptor * pdescriptor = m_controldescriptorset.element_at(m_columna[i]->m_iControl);
               if (pdescriptor != NULL)
               {
                  if (m_columna[i]->m_iSubItem >= 0)
                  {
                     pdescriptor->m_iSubItem = m_columna[i]->m_iSubItem;
                  }
                  else if (pdescriptor->m_iSubItem >= 0)
                  {
                     m_columna[i]->m_iSubItem = pdescriptor->m_iSubItem;
                  }
               }
            }

         }

      }

      if (m_pcontrolEdit != NULL && m_pcontrolEdit->IsWindowVisible())
      {

         _001PlaceControl(m_pcontrolEdit, m_pcontrolEdit->m_iEditItem);

      }

   }

   void form_list::_000OnMouse(::message::mouse * pmouse)
   {

      form_mesh::_000OnMouse(pmouse);

      //point pt = pmouse->m_pt;
      //ScreenToClient(&pt);
      ///*      if(uiMessage == WM_LBUTTONDOWN)
      //      {
      //      int32_t iItem;
      //      int32_t iSubItem;
      //      range range;
      //      _001GetSelection(range);
      //      if(_001DisplayHitTest(pt, iItem, iSubItem))
      //      {
      //      class ::user::control::descriptor * pcontrol = m_controldescriptorset.get_by_sub_item(iSubItem);
      //      if(pcontrol != NULL
      //      && pcontrol->m_pcontrol != NULL
      //      && (pcontrol->m_etype == type_edit
      //      || pcontrol->m_etype == type_edit_plain_text)
      //      && !range.has_sub_item(iItem, iSubItem))
      //      {
      //      _001HideEditingControls();
      //      return false;
      //      }
      //      }
      //      }
      //      else if(uiMessage == WM_LBUTTONUP)
      //      {
      //      int32_t iItem;
      //      int32_t iSubItem;
      //      range range;
      //      _001GetSelection(range);
      //      if(_001DisplayHitTest(pt, iItem, iSubItem))
      //      {
      //      class ::user::control::descriptor * pcontrol = m_controldescriptorset.get_by_sub_item(iSubItem);
      //      if(pcontrol != NULL
      //      && pcontrol->m_pcontrol != NULL
      //      && !pcontrol->m_pcontrol->IsWindowVisible()
      //      && (pcontrol->m_etype == type_edit
      //      || pcontrol->m_etype == type_edit_plain_text))
      //      {
      //      return false;
      //      }
      //      }
      //      }*/
      //control_keep controlkeep(this,pt);
      //sp(::user::interaction) pui = top_child();
      //sp(::user::interaction) puiBefore = NULL;
      //bool bError;
      //try
      //{
      //   while(pui != NULL)
      //   {
      //      bError = false;
      //      try
      //      {
      //         if(pui->IsWindowVisible() && pui->_001IsPointInside(pmouse->m_pt))
      //         {
      //            pui->_000OnMouse(pmouse);
      //            if(pmouse->m_bRet)
      //               return;
      //            pui->send(pmouse);
      //            if(pmouse->get_lresult() != 0)
      //               return;
      //         }
      //      }
      //      catch(...)
      //      {
      //         bError = true;
      //         puiBefore = pui;
      //      }
      //      pui = under_sibling(pui);
      //      if(bError)
      //      {
      //         m_uiptraChild.remove(puiBefore);
      //      }
      //   }
      //}
      //catch(...)
      //{
      //}
      //try
      //{
      //   (m_pimpl->*m_pimpl->m_pfnDispatchWindowProc)(dynamic_cast <signal_details *> (pmouse));
      //   if(pmouse->get_lresult() != 0)
      //      return;
      //}
      //catch(...)
      //{
      //}

   }

   void form_list::control_get_client_rect(sp(control) pcontrol,LPRECT lprect)
   {
      if(pcontrol == NULL)
      {
         SetRectEmpty(lprect);
         return;
      }
      rect rectControl;
      draw_list_item item(this);

      item.m_iDisplayItem = m_iItemHover;
      item.m_iItem = DisplayToStrict(m_iItemHover);

      if(m_bGroup)
      {
         item.m_iGroupTopIndex = 0;
         //            int32_t igroup;
         for(item.m_iGroup = 0; item.m_iGroup < m_nGroupCount; item.m_iGroup++)
         {
            item.m_iGroupCount = _001GetGroupItemCount(item.m_iGroup);
            if(item.m_iItem >= item.m_iGroupTopIndex && item.m_iItem < (item.m_iGroupTopIndex + item.m_iGroupCount))
               break;
         }
      }


      item.m_iSubItem = pcontrol->descriptor().m_iSubItem;
      item.m_iOrder = _001MapSubItemToOrder(item.m_iSubItem);
      item.m_iListItem = -1;
      _001GetElementRect(&item,::user::mesh::element_sub_item);
      rectControl = item.m_rectSubItem;
      class rect rect(rectControl);
      *lprect = rect;
   }

   void form_list::control_get_window_rect(sp(control) pcontrol,LPRECT lprect)
   {
      control_get_client_rect(pcontrol,lprect);
      ClientToScreen(lprect);
   }

   bool form_list::control_001DisplayHitTest(POINT pt)
   {
      return _001DisplayHitTest(pt,m_iControlItem,m_iControlSubItem);
   }


   bool form_list::_001PreviousEditableControl(int & iItem, int & iSubItem)
   {

      int iPreviousItem = iItem;

      int iColumnCount = _001GetColumnCount();

      index iSubItemColumn = _001MapSubItemToColumn(iSubItem);

      index iColumnPrevious = -1;

      for (index iColumn = iSubItemColumn - 1; iColumn >= 0; iColumn--)
      {

         if (m_columna.get_visible(iColumn)->m_iControl >= 0
            && _001GetControl(iItem, m_columna.get_visible(iColumn)->m_iSubItem) != NULL
            && _001IsSubItemEnabled(iItem, m_columna.get_visible(iColumn)->m_iSubItem))
         {

            iColumnPrevious = iColumn;

            break;

         }

      }

      if (iColumnPrevious < 0)
      {

         iPreviousItem--;

         for (index iColumn = iColumnCount - 1; iColumn >= iSubItemColumn; iColumn--)
         {

            if (m_columna.get_visible(iColumn)->m_iControl >= 0
               && _001GetControl(iItem, m_columna.get_visible(iColumn)->m_iSubItem) != NULL
               && _001IsSubItemEnabled(iItem, m_columna.get_visible(iColumn)->m_iSubItem))
            {

               iColumnPrevious = iColumn;

               break;

            }

         }

      }

      if (iColumnPrevious >= 0)
      {

         if (iPreviousItem < 0)
         {

            iPreviousItem = _001GetItemCount() - 1;

         }

         if (iPreviousItem < 0)
         {

            return false;

         }

         iItem = iPreviousItem;

         iSubItem = _001MapColumnToSubItem(iColumnPrevious);

         return true;

      }

      return false;

   }


   bool form_list::_001NextEditableControl(int & iItem, int & iSubItem)
   {

      int iNextItem = iItem;

      int iColumnCount = _001GetColumnCount();

      index iSubItemColumn = _001MapSubItemToColumn(iSubItem);

      index iColumnNext = -1;

      for (index iColumn = iSubItemColumn + 1; iColumn < iColumnCount; iColumn++)
      {

         if (m_columna.get_visible(iColumn)->m_iControl >= 0
            && _001GetControl(iItem, m_columna.get_visible(iColumn)->m_iSubItem) != NULL
            && _001IsSubItemEnabled(iItem, m_columna.get_visible(iColumn)->m_iSubItem))
         {

            iColumnNext = iColumn;

            break;

         }

      }

      if (iColumnNext < 0)
      {

         iNextItem++;

         for (index iColumn = 0; iColumn <= iSubItemColumn; iColumn++)
         {

            if (m_columna.get_visible(iColumn)->m_iControl >= 0
               && _001GetControl(iItem, m_columna.get_visible(iColumn)->m_iSubItem) != NULL
               && _001IsSubItemEnabled(iItem, m_columna.get_visible(iColumn)->m_iSubItem))
            {

               iColumnNext = iColumn;

               break;

            }

         }

      }

      if (iColumnNext >= 0)
      {

         if (iNextItem >= _001GetItemCount())
         {

            iNextItem = 0;

         }

         if (iNextItem >= _001GetItemCount())
         {

            return false;

         }

         iItem = iNextItem;

         iSubItem = _001MapColumnToOrder(iColumnNext);

         return true;

      }

      return false;

   }

   bool form_list::_001UpperEditableControl(int & iItem, int & iSubItem)
   {

      int iUpperItem = iItem - 1;

      for (; iUpperItem >= 0; iUpperItem--)
      {

         if (_001GetControl(iUpperItem, iSubItem) != NULL && _001IsSubItemEnabled(iUpperItem, iSubItem))
         {

            iItem = iUpperItem;

            return true;

         }

      }

      for (iUpperItem = _001GetItemCount() - 1; iUpperItem >= iItem; iUpperItem--)
      {

         if (_001GetControl(iUpperItem, iSubItem) != NULL && _001IsSubItemEnabled(iUpperItem, iSubItem))
         {

            iItem = iUpperItem;

            return true;

         }

      }

      iItem--;

      if (iItem < 0)
      {

         iItem = _001GetItemCount() - 1;

      }

      return _001PreviousEditableControl(iItem, iSubItem);

   }


   bool form_list::_001LowerEditableControl(int & iItem, int & iSubItem)
   {

      int iLowerItem = iItem + 1;

      for (; iLowerItem <  _001GetItemCount() - 1; iLowerItem++)
      {

         if (_001GetControl(iLowerItem, iSubItem) != NULL && _001IsSubItemEnabled(iLowerItem, iSubItem))
         {

            iItem = iLowerItem;

            return true;

         }

      }

      for (iLowerItem = 0; iLowerItem <= iItem; iLowerItem++)
      {

         if (_001GetControl(iLowerItem, iSubItem) != NULL && _001IsSubItemEnabled(iLowerItem, iSubItem))
         {

            iItem = iLowerItem;

            return true;

         }

      }

      iItem++;

      if (iItem >= _001GetItemCount())
      {

         iItem = 0;

      }

      return _001NextEditableControl(iItem, iSubItem);

   }



   bool form_list::BaseOnControlEvent(::user::control_event * pevent)
   {

      if (pevent->m_eevent == ::user::event_after_change_cur_sel)
      {

         if (m_pcontrolEdit == pevent->m_puie)
         {

            if (m_pcontrolEdit->descriptor().has_function(::user::control::function_data_selection))
            {
               _001SaveEdit(m_pcontrolEdit);
               pevent->m_bRet = true;
               pevent->m_bProcessed = true;

            }


         }

      }
      else if (pevent->m_eevent == ::user::event_enter_key)
      {

         if(m_pcontrolEdit != NULL)
         {

            _001SaveEdit(m_pcontrolEdit);
            _001HideControl(m_pcontrolEdit);

            pevent->m_bRet = true;
            pevent->m_bProcessed = true;

         }

      }
      else if (pevent->m_eevent == ::user::event_tab_key)
      {

         int iItem = 0;
         int iSubItem = 0;

         if (m_pcontrolEdit != NULL)
         {

            iItem = m_pcontrolEdit->m_iEditItem;

            iSubItem = m_pcontrolEdit->descriptor().m_iSubItem;

            _001SaveEdit(m_pcontrolEdit);
            _001HideControl(m_pcontrolEdit);

            pevent->m_bRet = true;
            pevent->m_bProcessed = true;

         }

         bool bOk;

         if (Session.is_key_pressed(::user::key_shift))
         {

            bOk = _001PreviousEditableControl(iItem, iSubItem);

         }
         else
         {

            bOk = _001NextEditableControl(iItem, iSubItem);

         }

         if(bOk)
         { 
         
            sp(control) pcontrol = _001GetControl(iItem, iSubItem);

            _001PlaceControl(pcontrol, iItem);

            pevent->m_bRet = true;

            pevent->m_bProcessed = true;  

         }

      }
      else if (pevent->m_eevent == ::user::event_key_down)
      {

         SCAST_PTR(::message::key, pkey, pevent->m_pobj);

         if (pkey->m_ekey == key_down || pkey->m_ekey == key_up
            || pkey->m_ekey == key_left || pkey->m_ekey == key_right)
         {

            int iItem = 0;

            int iSubItem = 0;

            if (m_pcontrolEdit != NULL)
            {

               sp(::user::plain_edit) pedit = m_pcontrolEdit;

               if (pedit.is_set())
               {

                  strsize iSel;

                  strsize iSelEnd;

                  pedit->_001GetSel(iSel, iSelEnd);

                  if (iSel != iSelEnd)
                  {

                     iSel = -1;

                  }

                  if (pkey->m_ekey == key_left && iSel != 0)
                  {

                     return false;

                  }
                  else if (pkey->m_ekey == key_right)
                  {

                     strsize iLen = pedit->_001GetTextLength();

                     if (iSel != iLen)
                     {

                        return false;

                     }

                  }

               }

               iItem = m_pcontrolEdit->m_iEditItem;

               iSubItem = m_pcontrolEdit->descriptor().m_iSubItem;

               _001SaveEdit(m_pcontrolEdit);
               _001HideControl(m_pcontrolEdit);

               pevent->m_bRet = true;
               pevent->m_bProcessed = true;

            }

            bool bOk;

            if (pkey->m_ekey == key_left)
            {

               bOk = _001PreviousEditableControl(iItem, iSubItem);

            }
            else if (pkey->m_ekey == key_right)
            {

               bOk = _001NextEditableControl(iItem, iSubItem);

            }
            else if (pkey->m_ekey == key_up)
            {

               bOk = _001UpperEditableControl(iItem, iSubItem);

            }
            else if (pkey->m_ekey == key_down)
            {

               bOk = _001LowerEditableControl(iItem, iSubItem);

            }

            if (bOk)
            {

               sp(control) pcontrol = _001GetControl(iItem, iSubItem);

               _001PlaceControl(pcontrol, iItem);


               sp(::user::plain_edit) pedit = m_pcontrolEdit;


               if(pedit.is_set())
               {

                  if (pkey->m_ekey == key_left)
                  {

                     strsize iLen = pedit->_001GetTextLength();

                     pedit->_001SetSel(iLen, iLen);


                  }
                  else if (pkey->m_ekey == key_right)
                  {

                     pedit->_001SetSel(0, 0);


                  }

               }

               pevent->m_bRet = true;

               pevent->m_bProcessed = true;

            }

         }

      }

      if (pevent->m_bRet)
         return pevent->m_bProcessed;

      return form_mesh::BaseOnControlEvent(pevent);
   }


   bool form_list::_001HitTest_(point point,index &iItem,index &iSubItem)
   {

      return ::user::list::_001HitTest_(point,iItem,iSubItem);

   }


   void form_list::_001DrawSubItem(draw_list_item * pdrawitem)
   {

      if (pdrawitem->m_pcolumn->m_iControl >= 0)
      {

         rect rScreen = pdrawitem->m_rectSubItem;

         ClientToScreen(rScreen);

         if (rScreen.contains(Session.m_ptCursor))
         {

            COLORREF crBackHover = ARGB(40, 0, 0, 0);

            get_color(crBackHover, ::user::color_background_hover);

            pdrawitem->m_pgraphics->FillSolidRect(pdrawitem->m_rectSubItem, crBackHover);

         }

         if (pdrawitem->m_pcolumn->m_iControl >= 0 && pdrawitem->m_pcolumn->m_iControl < m_controldescriptorset.get_count())
         {

            sp(class ::user::control::descriptor) pdescriptor = m_controldescriptorset.sp_at(pdrawitem->m_pcolumn->m_iControl);

            if (pdescriptor.is_set())
            {

               if (pdescriptor->has_function(::user::control::function_check_box))
               {

                  _001GetElementRect(pdrawitem, ::user::mesh::element_text);

                  if (pdrawitem->m_bOk)
                  {

                     rect r;

                     r.left = 0;
                     r.top = 0;
                     r.right = 15;
                     r.bottom = 15;

                     r.Align(::align_center, pdrawitem->m_rectSubItem);

                     _001GetItemText(pdrawitem);

                     ::check::e_check echeck;

                     if (pdrawitem->m_strText == pdescriptor->m_setValue[::check::checked])
                     {

                        echeck = ::check::checked;

                     }
                     else
                     {

                        echeck = ::check::unchecked;

                     }

                     if (m_puserschemaSchema != NULL)
                     {

                        m_puserschemaSchema->_001DrawCheckBox(pdrawitem->m_pgraphics, r, echeck);

                     }

                  }

                  return;

               }

            }

         }

      }

      ::user::list::_001DrawSubItem(pdrawitem);

      if (pdrawitem->m_pcolumn->m_bCustomDraw)
      {

         sp(control) pcontrol = _001GetControl(pdrawitem->m_iItem, pdrawitem->m_iSubItem);

         if (pcontrol != NULL)
         {

            pdrawitem->m_rectClient = pdrawitem->m_rectSubItem;

            pdrawitem->m_rectWindow = pdrawitem->m_rectClient;

            ClientToScreen(pdrawitem->m_rectWindow);

            ::rect rectWindow;

            pcontrol->GetWindowRect(rectWindow);

            ScreenToClient(rectWindow);

            if (rectWindow != pdrawitem->m_rectClient)
            {

               pcontrol->SetWindowPos(0, pdrawitem->m_rectClient, SWP_SHOWWINDOW | SWP_NOZORDER);

            }

            //control_keep controlkeep(this,pdrawitem->m_iItem,pdrawitem->m_iSubItem);

            if (pcontrol != Session.get_keyboard_focus())
            {

               sp(::user::plain_edit) pedit = pcontrol;

               if (pedit.is_set())
               {
                  pdrawitem->m_bOk = false;
                  _001GetItemText(pdrawitem);
                  if (pdrawitem->m_bOk)
                  {

                     string strText;

                     pedit->_001GetText(strText);


                     if (strText != pdrawitem->m_strText)
                     {

                        pedit->_001SetText(pdrawitem->m_strText, ::action::source_sync);

                     }

                  }

               }

            }

            //pcontrol->_003CallCustomDraw(pdrawitem->m_pgraphics,pdrawitem);
            //pdrawitem->m_pgraphics->SelectClipRgn(NULL);
            //_001OnClip(pdrawitem->m_pgraphics);
         }
      }

      ::user::list::_001DrawSubItem(pdrawitem);

   }


   ::check::e_check form_list::_001GetSubItemCheck(index iItem, index iSubItem)
   {

      sp(::user::list_column) pcolumn = m_columna.get_by_subitem(iSubItem);

      if (pcolumn.is_set() && pcolumn->m_iControl >= 0)
      {

         sp(class ::user::control::descriptor) pdescriptor = m_controldescriptorset.sp_at(pcolumn->m_iControl);

         if (pdescriptor.is_set())
         {

            if (pdescriptor->has_function(::user::control::function_check_box))
            {

               ::user::mesh_item item(this);

               item.m_iItem = iItem;
               item.m_iSubItem = iSubItem;

               _001GetItemText(&item);

               if (!item.m_bOk)
               {

                  return ::check::undefined;

               }

               ::check::e_check echeck;

               if (item.m_strText == pdescriptor->m_setValue[::check::checked])
               {

                  return ::check::checked;

               }
               else
               {

                  return ::check::unchecked;

               }


            }

         }

      }

      return ::check::undefined;

   }


   bool form_list::_001SetSubItemCheck(index iItem, index iSubItem, ::check::e_check echeck)
   {

      sp(::user::list_column) pcolumn = m_columna.get_by_subitem(iSubItem);

      if (pcolumn.is_set() && pcolumn->m_iControl >= 0)
      {

         sp(class ::user::control::descriptor) pdescriptor = m_controldescriptorset.sp_at(pcolumn->m_iControl);

         if (pdescriptor.is_set())
         {

            if (pdescriptor->has_function(::user::control::function_check_box))
            {

               ::user::mesh_item item(this);

               item.m_iItem = iItem;
               item.m_iSubItem = iSubItem;
               item.m_strText = pdescriptor->m_setValue[echeck];

               if (!item.m_strText.has_char())
               {

                  return false;

               }

               _001SetItemText(&item);

               return item.m_bOk;

            }

         }

      }

      return false;

   }


   bool form_list::_001IsSubItemEnabled(index iItem, index iSubItem)
   {

      sp(::user::list_column) pcolumn = m_columna.get_by_subitem(iSubItem);

      if (pcolumn.is_set() && pcolumn->m_iControl >= 0)
      {

         sp(class ::user::control::descriptor) pdescriptor = m_controldescriptorset.sp_at(pcolumn->m_iControl);

         if (pdescriptor.is_set())
         {

            if (pdescriptor->has_function(::user::control::function_disable_on_check_box))
            {

               if(_001GetSubItemCheck(iItem, pdescriptor->m_iSubItemDisableCheckBox) == ::check::checked)
               {

                  return false;

               }

            }

         }

      }

      return true;

   }




} // namespace user









