//#include "framework.h"
//#include "framework.h"

#include "base/database/simpledb/simpledb.h"


namespace filemanager
{


   file_list::file_list(::aura::application * papp) :
      object(papp),
      ::user::interaction(papp),
      ::userfs::list(papp)
   {

      m_iAnimate = 0;
      m_bRestartCreateImageList = false;
      m_bStatic = false;
      m_bPendingSize = false;
//      m_pcreateimagelistthread = NULL;

      m_bFileSize = false;
      m_bShow = false;
      m_dwLastFileSize = ::get_tick_count();

      bool bDoubleClickInWebView = true;

#ifdef WINDOWS
      
      SHELLSTATE shellstate;

      SHGetSetSettings(&shellstate, SSF_DOUBLECLICKINWEBVIEW, false);

      bDoubleClickInWebView = shellstate.fDoubleClickInWebView != FALSE;

#endif

#ifdef LINUX
      m_bHoverSelect = true;
#else
      m_bHoverSelect = !bDoubleClickInWebView;
#endif


      connect_update_cmd_ui("edit_copy", &file_list::_001OnUpdateEditCopy);
      connect_command("edit_copy", &file_list::_001OnEditCopy);
      connect_update_cmd_ui("trash_that_is_not_trash", &file_list::_001OnUpdateTrashThatIsNotTrash);
      connect_command("trash_that_is_not_trash", &file_list::_001OnTrashThatIsNotTrash);
      connect_update_cmd_ui("open_with", &file_list::_001OnUpdateOpenWith);
      connect_update_cmd_ui("spafy", &file_list::_001OnUpdateSpafy);
      connect_command("spafy", &file_list::_001OnSpafy);
      connect_update_cmd_ui("spafy2", &file_list::_001OnUpdateSpafy2);
      connect_command("spafy2", &file_list::_001OnSpafy2);
      connect_update_cmd_ui("file_rename", &file_list::_001OnUpdateFileRename);
      connect_command("file_rename", &file_list::_001OnFileRename);

   }


   file_list::~file_list()
   {

   }


   void file_list::install_message_handling(::message::dispatch * pinterface)
   {

      ::user::impact::install_message_handling(pinterface);
      ::user::form_list::install_message_handling(pinterface);
      ::userfs::list::install_message_handling(pinterface);
      IGUI_WIN_MSG_LINK(MessageMainPost, pinterface, this, &file_list::_001OnMainPostMessage);
      IGUI_WIN_MSG_LINK(WM_HSCROLL, pinterface, this, &file_list::_001OnHScroll);
      IGUI_WIN_MSG_LINK(WM_VSCROLL, pinterface, this, &file_list::_001OnVScroll);
      IGUI_WIN_MSG_LINK(WM_RBUTTONUP, pinterface, this, &file_list::_001OnContextMenu);
      connect_command_range(FILEMANAGER_SHELL_COMMAND_FIRST, FILEMANAGER_SHELL_COMMAND_LAST, &file_list::_001OnShellCommand);
      IGUI_WIN_MSG_LINK(WM_SHOWWINDOW, pinterface, this, &file_list::_001OnShowWindow);

   }


   #ifdef DEBUG
   void file_list::assert_valid() const
   {
      ::user::impact::assert_valid();
   }

   void file_list::dump(dump_context & dumpcontext) const
   {
      ::user::impact::dump(dumpcontext);
   }
   #endif //DEBUG


   void file_list::on_update(::user::impact * pSender, LPARAM lHint, object* phint)
   {

      ::filemanager::impact::on_update(pSender, lHint, phint);

      ::userfs::list::on_update(pSender, lHint, phint);

      if(m_bStatic && lHint == hint_add_location)
      {

         ::file::patha filepatha;

         get_filemanager_manager()->data_load(get_filemanager_template()->m_dataidStatic, filepatha);

         ::file::path filepath = get_filemanager_item().m_filepath;

         filepath.trim();

         if(filepath.has_char() && get_filemanager_manager()->get_fs_data()->is_dir(filepath))
         {

            if(filepatha.add_unique(filepath) >= 0)
            {

               get_filemanager_manager()->data_save(get_filemanager_template()->m_dataidStatic, filepatha);

               add_item(get_filemanager_item().m_filepath, get_filemanager_item().m_filepath.name());

               _001OnUpdateItemCount();

            }

         }

      }
      else if(lHint == 123458)
      {
         _001SetView(view_list);
      }
      else if(lHint == 1234511)
      {
         _001SetView(view_report);
      }
      else if(lHint == 1234525)
      {
         _001SetView(view_icon);
      }
      if(phint != NULL)
      {
         if(base_class < update_hint >::bases(phint))
         {
            update_hint * puh = (update_hint *) phint;
            if(puh->is_type_of(update_hint::TypeInitialize))
            {


               m_pauraapp = get_app()->m_pcoreapp;
               db_server * pcentral = dynamic_cast < db_server * > (&System.m_simpledb.db());
               if(pcentral == NULL)
                  return;
               string str;
               str.Format(".local://file_list(%s)", get_filemanager_data()->m_strDISection);
               if(get_filemanager_data()->m_bPassBk)
               {
                  ::user::list::m_bBackgroundBypass = true;
               }
               m_dataid = str;
               _001UpdateColumns();
               _001OnUpdateItemCount();

            }
            else if(!m_bStatic && puh->is_type_of(update_hint::TypeSynchronizePath))
            {

               if(puh->m_filepath != get_filemanager_item().m_filepath)
                  return;

               if(get_filemanager_data()->m_pholderFileList != NULL)
               {
                  if(get_filemanager_data()->m_pholderFileList->m_uiptraChild.get_size() > 0)
                  {
                     get_filemanager_data()->m_pholderFileList->m_uiptraChild[0]->ShowWindow(SW_HIDE);
                  }
                  get_filemanager_data()->m_pholderFileList->hold(this);
                  get_filemanager_data()->m_pholderFileList->on_layout();
               }

               data_get_DisplayToStrict();
               _001OnUpdateItemCount();
               /*string str;
               if(data_get("sort-" + get_filemanager_item().m_strPath, str))
               {
                  stringa stra;
                  stra.add_tokens(str, ";", true);
                  if(stra.get_size() == m_iaDisplayToStrict.get_size())
                  {
                     for(int32_t i = 0; i < m_iaDisplayToStrict.get_size(); i++)
                     {
                        m_iaDisplayToStrict.set(i, atoi(stra[i]));
                     }
                  }
               }*/
            }
            else if(puh->is_type_of(update_hint::TypeFilter))
            {
               if(puh->m_wstrFilter.is_empty())
               {
                  FilterClose();
               }
               else
               {
                  FilterBegin();
                  Filter1(puh->m_wstrFilter);
//                  FilterApply();
               }
            }
            else if(puh->is_type_of(update_hint::TypeGetActiveViewSelection))
            {
               if(GetParentFrame()->GetActiveView() ==  (this))
               {
                  GetSelected(puh->m_itemaSelected);
               }
            }
         }
         else if(base_class < form_update_hint > :: bases(phint))
         {
            form_update_hint * puh = dynamic_cast<form_update_hint * > (phint);
            if(puh->m_etype == form_update_hint::type_after_browse)
            {
               if(puh->m_strForm == "filemanager\\replace_name_in_file_system.xhtml")
               {
                  //html::elemental * pelemental = dynamic_cast < html::elemental * > (puh->m_pformview->get_html_data()->get_element_by_name("encontrar"));
                  //html::impl::input_text * pinput = dynamic_cast < html::impl::input_text * > (pelemental->m_pimpl);
                  sp(::user::interaction) ptext = puh->m_pform->get_child_by_id("encontrar");
                  range range;
                  _001GetSelection(range);
                  if(range.get_item_count() > 0)
                  {
                     ptext->_001SetText(get_fs_mesh_data()->m_itema.get_item(range.ItemAt(0).get_lower_bound()).m_strName, puh->m_actioncontext);
                  }
               }
            }

            form_update_hint * pmanageruh = dynamic_cast<form_update_hint * > (phint);

            if(pmanageruh != NULL)
            {

               if (pmanageruh->m_ehint == manager::hint_replace_name)
               {

                  if (pmanageruh->m_strFind.has_char())
                  {

                     ::file::path pathFolder = get_filemanager_item().m_filepath;

                     Application.file().replace(pathFolder, pmanageruh->m_strFind, pmanageruh->m_strReplace);



                  }

               }
               else if (pmanageruh->m_ehint == manager::hint_new_folder)
               {

                  if (pmanageruh->m_str.has_char())
                  {

                     ::file::path pathFolder = get_filemanager_item().m_filepath / pmanageruh->m_str;

                     Application.dir().mk(pathFolder);

                  }

               }

            }

         }

      }
   }


  bool file_list::_001OnClick(uint_ptr nFlags, point point)
   {
      UNREFERENCED_PARAMETER(nFlags);
      index iItem;
      index iSubItem;
      if(_001HitTest_(point, iItem, iSubItem))
      {
         if(iSubItem == m_iNameSubItem || (m_eview == view_list && iSubItem == 0))
         {
           _017OpenSelected(true, ::action::source_user);
         }
         return true;
      }

      return ::user::list::_001OnClick(nFlags, point);


   }


   bool file_list::_001OnRightClick(uint_ptr nFlags, point point)
   {

      UNREFERENCED_PARAMETER(nFlags);
      index iItem;
      index iSubItem;
      if(_001HitTest_(point, iItem, iSubItem))
      {
         _017OpenContextMenuSelected(::action::source_user);
      }
      else
      {
         _017OpenContextMenu(::action::source_user);
      }
      return true;
   }


   void file_list::RenameFile(int32_t iLine, string &wstrNameNew, ::action::context actioncontext)
   {

      synch_lock sl(get_fs_mesh_data()->m_pmutex);

      ::file::path filepath = get_fs_mesh_data()->m_itema.get_item(iLine).m_filepath;

      ::file::path filepathNew = filepath.folder() / wstrNameNew;

      System.file().rename(filepathNew, filepath, get_app());

      browse_sync(actioncontext);

   }


   void file_list::_001OnContextMenu(signal_details * pobj)
   {


      //SCAST_PTR(::message::context_menu, pcontextmenu, pobj);
      SCAST_PTR(::message::mouse, pcontextmenu, pobj);

      synch_lock sl(get_fs_mesh_data()->m_pmutex);

      index iItem;
//      HRESULT hr;
      point point = pcontextmenu->m_pt;
      class point ptClient = point;
      ::user::list::ScreenToClient(&ptClient);
        if(_001HitTest_(ptClient, iItem))
      {
         ::user::menu menu(get_app());
         if(get_fs_mesh_data()->m_itema.get_item(iItem).IsFolder())
         {
            _017OpenContextMenuFolder(canew(::fs::item(get_fs_mesh_data()->m_itema.get_item(iItem))), ::action::source_user);
            /*if (menu.LoadXmlMenu(get_filemanager_template()->m_strFolderPopup))
            {
               ::user::menu menuPopup(get_app(), menu.GetSubMenu(0));
               //SimpleMenu* pPopup = (SimpleMenu *) menu.GetSubMenu(0);
               //ASSERT(pPopup != NULL);
               sp(::user::frame_window) pframe = ( (GetParentFrame()));
               pframe->SetActiveView(this);
               menuPopup.set_app(get_app());
               menuPopup.TrackPopupMenu(
                  point.x, point.y,
                  pframe);
            }*/
         }
         else
         {

            track_popup_xml_matter_menu(get_filemanager_template()->m_strFilePopup, 0, pobj);

         }

      }
      else
      {

         track_popup_xml_matter_menu(get_filemanager_template()->m_strPopup, 0, pobj);

      }

   }


   bool file_list::pre_create_window(::user::create_struct& cs)
   {

      cs.style |= WS_CLIPCHILDREN;

      return ::user::impact::pre_create_window(cs);
   }

   UINT c_cdecl file_list::ThreadProcFileSize(LPVOID lpparam)
   {
      file_size * psize = (file_size *) lpparam;
      db_server * pcentral = dynamic_cast < db_server * > (&App(psize->m_pview->m_pauraapp).simpledb().db());
      if(pcentral == NULL)
         return 0;
      DBFileSystemSizeSet * pset = pcentral->m_pfilesystemsizeset;
      bool bPendingSize;
      int64_t i64Size;
      while(true)
      {
         bPendingSize = false;
         pset->get_fs_size(i64Size, psize->m_str, bPendingSize);
         if(!bPendingSize)
            break;
      }
      //psize->m_pview->RedrawWindow();
      delete psize;
      //file_list * pview = (file_list *) lpparam;
      //pview->FileSize();
      return 0;
   }

   void file_list::FileSize()
   {
      if(m_bFileSize)
         return;
      m_bFileSize = true;

      db_server * pcentral = dynamic_cast < db_server * > (&System.m_simpledb.db());
      if(pcentral == NULL)
         return;
      DBFileSystemSizeSet * pset = pcentral->m_pfilesystemsizeset;

      int32_t i;
      while(true)
      {
         i = 0;
         while(i < get_fs_mesh_data()->m_itema.get_count() || IsWindowVisible())
         {
            int64_t i64Size;
            bool bPendingSize;
            single_lock lock(m_pauraapp->m_pmutex);
            if(!lock.lock(millis(1984)))
               return;
            if(i >= get_fs_mesh_data()->m_itema.get_count())
               i = 0;
            bPendingSize = false;

            try
            {

               pset->get_fs_size(i64Size, get_fs_mesh_data()->m_itema.get_item(i).m_filepath, bPendingSize);

            }
            catch(...)
            {

            }

            lock.unlock();
            i++;
            Sleep(23);
         }
         Sleep(100);
      }
      m_bFileSize = false;
   }


   void file_list::_001OnTimer(::timer * ptimer)
   {

      ::userfs::list::_001OnTimer(ptimer);

      if(ptimer->m_nIDEvent == 888888)
      {
         if(get_filemanager_data()->m_bSetBergedgeTopicFile)
         {
            stringa stra;
            GetSelectedFilePath(stra);
            if(stra.get_count() <= 0)
            {
               Session.m_varTopicFile.unset();
            }
            else if(stra.get_count() == 1)
            {
               Session.m_varTopicFile = stra[0];
            }
            else
            {
               Session.m_varTopicFile = stra;
            }

         }
         else
         {
            KillTimer(888888);
         }
      }

   }

   void file_list::StartAnimation()
   {
      m_iAnimate = 1;

      //SetTimer(1234567, 50, NULL);
   }

   bool file_list::_001OnCmdMsg(::aura::cmd_msg * pcmdmsg)
   {
      ::fs::item_array itema;
      GetSelected(itema);
      if(get_filemanager_manager()->HandleDefaultFileManagerItemCmdMsg(pcmdmsg, itema))
         return TRUE;
      return ::user::impact::_001OnCmdMsg(pcmdmsg);
   }

   void file_list::_001OnShellCommand(signal_details * pobj)
   {
      SCAST_PTR(::message::command, pcommand, pobj);
      m_contextmenu.OnCommand(pcommand->GetId());
   }

   void file_list::_001OnFileManagerItemCommand(signal_details * pobj)
   {
      SCAST_PTR(::aura::command, pcommand, pobj);
      ::fs::item_array itema;
      index iItemRange, iItem;
      range range;
      _001GetSelection(range);
      for(iItemRange = 0;
          iItemRange < range.get_item_count();
          iItemRange++)
      {
         item_range itemrange = range.ItemAt(iItemRange);
         for(iItem = itemrange.get_lower_bound() ; iItem <= itemrange.get_upper_bound(); iItem ++)
         {
            itema.add(canew(::fs::item  (get_fs_mesh_data()->m_itema.get_item(iItem))));
         }
      }
      get_filemanager_data()->OnFileManagerItemCommand(
         pcommand->m_id,
         itema);
   }

   void file_list::_001OnFileManagerItemUpdate(signal_details * pobj)
   {

      SCAST_PTR(::message::update_cmd_ui, pupdatecmdui, pobj);

      synch_lock sl(get_fs_mesh_data()->m_pmutex);

      ::fs::item_array itema;
      index iItemRange, iItem;
      range range;
      _001GetSelection(range);
      for(iItemRange = 0;
          iItemRange < range.get_item_count();
          iItemRange++)
      {
         item_range itemrange = range.ItemAt(iItemRange);
         for(iItem = itemrange.get_lower_bound() ; iItem <= itemrange.get_upper_bound(); iItem ++)
         {
            itema.add(canew ( ::fs::item  (get_fs_mesh_data()->m_itema.get_item(iItem))));
         }
      }
      get_filemanager_data()->OnFileManagerItemUpdate(
         pupdatecmdui->m_pcmdui,
         itema);
      pobj->m_bRet = true;
   }

   void file_list::_017OpenContextMenuFolder(sp(::fs::item)  item, ::action::context actioncontext)
   {

      stringa straCommand;

      stringa straCommandTitle;

      get_filemanager_data()->OnFileManagerOpenContextMenuFolder(item, straCommand, straCommandTitle, actioncontext);

      if(straCommand.get_size() > 0)
      {

         sp(::user::menu_base) pmenu = canew(::user::menu(get_app()));

         point ptCursor;

         Session.get_cursor_pos(&ptCursor);

         if(pmenu->create_menu(straCommand, straCommandTitle))
         {

            pmenu->TrackPopupMenu(0, ptCursor, GetParentFrame());

         }

      }

   }

   void file_list::_017OpenContextMenuFile(const ::fs::item_array & itema, ::action::context actioncontext)
   {

      get_filemanager_data()->OnFileManagerOpenContextMenuFile(itema, actioncontext);

   }


   void file_list::_017OpenContextMenu(::action::context actioncontext)
   {

      get_filemanager_data()->OnFileManagerOpenContextMenu(actioncontext);

   }


   void file_list::_017OpenFolder(sp(::fs::item) item, ::action::context actioncontext)
   {

      get_filemanager_manager()->FileManagerBrowse(item, actioncontext);

   }


   void file_list::_017OpenFile(const ::fs::item_array &itema, ::action::context actioncontext)
   {

      get_filemanager_data()->OnFileManagerOpenFile(itema, actioncontext);

   }


   void file_list::_001OnFileRename(signal_details * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

      range range;

      _001GetSelection(range);

      if (range.get_item_count() == 1 && range.ItemAt(0).get_lower_bound() == range.ItemAt(0).get_upper_bound())
      {

         index iEditItem = range.ItemAt(0).get_lower_bound();

         sp(::user::control) pcontrol = _001GetControl(iEditItem, m_iNameSubItem);

         _001PlaceControl(pcontrol, iEditItem);

      }

   }


   void file_list::_001OnUpdateFileRename(signal_details * pobj)
   {
//      SCAST_PTR(::aura::cmd_ui, pcmdui, pobj);
  //    pcmdui->m_pcmdui->Enable(_001GetSelectedItemCount() == 1);
    //  pobj->m_bRet = true;

      SCAST_PTR(::aura::cmd_ui, pcmdui, pobj);
         range range;
      _001GetSelection(range);
      pcmdui->m_pcmdui->Enable(
         range.get_item_count() == 1
         && range.ItemAt(0).get_lower_bound() == range.ItemAt(0).get_upper_bound());
      pobj->m_bRet = true;

   }


   void file_list::_001OnUpdateEditCopy(signal_details * pobj)
   {
      SCAST_PTR(::aura::cmd_ui, pcmdui, pobj);
      range range;
      _001GetSelection(range);
      pcmdui->m_pcmdui->Enable(range.get_item_count() > 0);
      pobj->m_bRet = true;
   }

   void file_list::_001OnEditCopy(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      ::fs::item_array itema;
      GetSelected(itema);
      stringa stra;

      for(int32_t i = 0; i < itema.get_size(); i++)
      {

         stra.add(itema[i]->m_filepath);

      }

      /*
      string str;
      if(itema.get_size() > 0)
      {
         str = itema[0]->m_strPath;
      }
      for(int32_t i = 1; i < itema.get_size(); i++)
      {
         str += "|" + itema[i]->m_strPath;
      }
      System.m_strCopy = str;
   */


      Session.copydesk().set_filea(stra);
      pobj->m_bRet = true;

   }

   void file_list::_001OnUpdateTrashThatIsNotTrash(signal_details * pobj)
   {
      SCAST_PTR(::aura::cmd_ui, pcmdui, pobj);
      range range;
      _001GetSelection(range);
      pcmdui->m_pcmdui->Enable(range.get_item_count() > 0);
      pobj->m_bRet = true;
   }


   void file_list::_001OnTrashThatIsNotTrash(signal_details * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

      ::fs::item_array itema;

      GetSelected(itema);

      ::file::patha stra;

      for(int32_t i = 0; i < itema.get_size(); i++)
      {

         stra.add(itema[i]->m_filepath);

      }

      Application.file().trash_that_is_not_trash(stra);

      _001Refresh();

      pobj->m_bRet = true;

   }


   void file_list::_001OnUpdateOpenWith(signal_details * pobj)
   {
      SCAST_PTR(::aura::cmd_ui, pcmdui, pobj);

         ::user::menu_button_cmd_ui * pcmdui1 = dynamic_cast < ::user::menu_button_cmd_ui * > (pcmdui->m_pcmdui);
         if(pcmdui1 != NULL)
         {
            ::user::menu_item_ptra * pitema = pcmdui1->m_pitema;

            ::user::menu_base * pbase = pitema->element_at(pcmdui->m_pcmdui->m_iIndex)->m_pbase;
            pitema->remove_at(pcmdui->m_pcmdui->m_iIndex);


            index iStartIndex = pcmdui->m_pcmdui->m_iIndex;
            index iIndex = iStartIndex;


            ::fs::item_array itema;
            GetSelected(itema);

            ::file::path strPath = itema[0]->m_filepath;

            string strExt = strPath.extension();

            stringa stra;
            System.os().file_extension_get_open_with_list_keys(stra, strExt);

            m_straOpenWith = stra;
            ::count iCount = stra.get_size();

            sp(::user::menu_item) pmenuitem(canew(::user::menu_item(get_app())));
            string str;
            for(int32_t i = 0; i < iCount; i++)
            {
               pmenuitem->m_id = "open with" + stra[i];
               pmenuitem->m_button.set_window_text(stra[i]);
               pmenuitem->m_iLevel = pitema->m_pitemContainer != NULL ? pitema->m_pitemContainer->m_iLevel + 1 : 0;
               pmenuitem->m_pbase = pbase;
               pitema->insert_at(iIndex, pmenuitem);
               iIndex++;
            }
            pcmdui->m_pcmdui->m_iIndex = iStartIndex;

            pcmdui->m_pcmdui->m_iCount = iIndex;


            pbase->on_layout();

         }
      pobj->m_bRet = true;
   }


   bool file_list::on_simple_update(cmd_ui * pcmdui)
   {
      int32_t iPos = -1;
      for(int32_t i = 0; i < m_straOpenWith.get_size(); i++)
      {
         string strId = "open with" + m_straOpenWith[i];
         if(pcmdui->m_id == strId)
         {
            iPos = i;
            break;
         }
      }
      if(iPos >= 0)
      {
         pcmdui->Enable(TRUE);
         return true;
      }
      else
      {
         return ::user::impact::on_simple_update(pcmdui);
      }
   }

   bool file_list::on_simple_action(id id)
   {
      if(id == "1000")
      {
   //      _017OpenSelected(true, ::action::source_user);
         return true;
      }
      int32_t iPos = -1;
      for(int32_t i = 0; i < m_straOpenWith.get_size(); i++)
      {
         string strId = "open with" + m_straOpenWith[i];
         if(id == strId)
         {
            iPos = i;
            break;
         }
      }
      if(iPos >= 0)
      {

         ::fs::item_array itema;

         GetSelected(itema);

         ::file::path filepath = itema[0]->m_filepath;

#ifdef WINDOWSEX

         ::aura::shell_launcher launcher(NULL, "open", m_straOpenWith[iPos], filepath, filepath.name(), SW_SHOW);

         launcher.execute();

#else

         throw todo(get_app());

#endif

         return true;

      }
      else
      {
         return ::user::impact::on_simple_action(id);
      }

   }

   void file_list::_001OnUpdateSpafy(signal_details * pobj)
   {
      SCAST_PTR(::aura::cmd_ui, pcmdui, pobj);
      range range;
      _001GetSelection(range);
      pcmdui->m_pcmdui->Enable(range.get_item_count() > 0);
      pobj->m_bRet = true;
   }

   void file_list::_001OnSpafy(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      ::fs::item_array itema;
      GetSelected(itema);
      stringa stra;
      ::file::listing straSub;

      string strFileList;
      string strFileCheck;
      for(int32_t i = 0; i < itema.get_size(); i++)
      {

         if(Application.dir().is(itema[i]->m_filepath) && itema[i]->m_filepath.name() != ".svn")
         {

            straSub.rls(itema[i]->m_filepath);

            for(int32_t j = 0; j < straSub.get_size(); j++)
            {
               if(!Application.dir().is(straSub[j]) && straSub[j].find(".svn") < 0)
               {
                  strFileList += straSub[j] + "\n";
                  strFileCheck += straSub[j] + ",";
                  strFileCheck += Application.file().length(straSub[j]).get_string() + ",";
                  strFileCheck += Application.file().md5(straSub[j]) +"\n";
               }
            }
         }
         else
         {
            strFileList += itema[i]->m_filepath + "\n";
            strFileCheck += itema[i]->m_filepath + ",";
            strFileCheck += Application.file().length(itema[i]->m_filepath).get_string() + ",";
            strFileCheck += Application.file().md5(itema[i]->m_filepath) +"\n";
         }
      }

      ::datetime::time time = ::datetime::time::get_current_time();

      string strTime;

      strTime.Format("%04d-%02d-%02d %02d-%02d",
         time.GetYear(),
         time.GetMonth(),
         time.GetDay(),
         time.GetHour(),
         time.GetMinute());

      string strBase = get_filemanager_item().m_filepath / "spafy_";

      string strList = strBase + "list_" + strTime + ".txt";
      string strCheck = strBase + "check_" + strTime + ".txt";


      Application.file().put_contents(strList, strFileList);
      Application.file().put_contents(strCheck, strFileCheck);

   }

   void file_list::_001OnUpdateSpafy2(signal_details * pobj)
   {
      SCAST_PTR(::aura::cmd_ui, pcmdui, pobj);
      pcmdui->m_pcmdui->Enable(TRUE);
      pobj->m_bRet = true;
   }

   void file_list::_001OnSpafy2(signal_details * pobj)
   {

      synch_lock sl(get_fs_mesh_data()->m_pmutex);

      sp(::userfs::list_data) pdata = get_fs_mesh_data();
      UNREFERENCED_PARAMETER(pobj);
      stringa stra;
      ::file::listing straSub(get_app());

      string strFileList;
      string strFileCheck;
      for(int32_t i = 0; i < pdata->m_itema.get_count(); i++)
      {
         if(::userfs::list::get_document()->get_fs_data()->is_dir(pdata->m_itema.get_item(i).m_filepath)
            && pdata->m_itema.get_item(i).m_filepath.name() != ".svn")
         {
            straSub.rls(pdata->m_itema.get_item(i).m_filepath);
            for(int32_t j = 0; j < straSub.get_size(); j++)
            {
             string strExtension = straSub[j].extension();

               if(!Application.dir().is(straSub[j])
         && (strExtension == "exe" || strExtension == "dll" || strExtension == "dll.manifest"
             || strExtension == "exe.manifest"))
               {
                  strFileList += straSub[j] + "\n";
                  strFileCheck += straSub[j] + ",";
                  strFileCheck += Application.file().length(straSub[j]).get_string() + ",";
                  strFileCheck += Application.file().md5(straSub[j]) +"\n";
               }
            }
         }
         else
         {
             string strExtension = pdata->m_itema.get_item(i).m_filepath.extension();
         if(strExtension == "exe" || strExtension == "dll" || strExtension == "dll.manifest"
             || strExtension == "exe.manifest")
         {

            strFileList += pdata->m_itema.get_item(i).m_filepath + "\n";
            strFileCheck += pdata->m_itema.get_item(i).m_filepath + ",";
            strFileCheck += Application.file().length(pdata->m_itema.get_item(i).m_filepath).get_string() + ",";
            strFileCheck += Application.file().md5(pdata->m_itema.get_item(i).m_filepath) +"\n";
         }
         }
      }

      ::datetime::time time = ::datetime::time::get_current_time();

      string strTime;
      strTime.Format("%04d-%02d-%02d %02d-%02d",
         time.GetYear(),
         time.GetMonth(),
         time.GetDay(),
         time.GetHour(),
         time.GetMinute());
      string strBase = get_filemanager_item().m_filepath /  "spafy_";
      string strList = strBase + "list_" + strTime + ".txt";
      string strCheck = strBase + "check_" + strTime + ".txt";


      Application.file().put_contents(strList, strFileList);
      Application.file().put_contents(strCheck, strFileCheck);

   }


   void file_list::_001OnAfterSort()
   {
      data_set_DisplayToStrict();
   }


   void file_list::schedule_file_size(const char * psz)
   {

      UNREFERENCED_PARAMETER(psz);

      if(!IsWindowVisible())
         return;

   }


   void file_list::_001OnShowWindow(signal_details * pobj)
   {
      SCAST_PTR(::message::show_window, pshowwindow, pobj);

      if(!pshowwindow->m_bShow)
      {
   //      System.simple_message_box("hide");
      }

      UNREFERENCED_PARAMETER(pobj);
      //      SCAST_PTR(::message::show_window, pshow, pobj);

      db_server * pcentral = dynamic_cast < db_server * > (&System.m_simpledb.db());
      if (pcentral == NULL)
         return;
      //DBFileSystemSizeSet * pset = pcentral->m_pfilesystemsizeset;
      /*if(pshow->m_bShow)
      {
      for(int32_t i = 0; i < m_itema.get_item_count(); i++)
      {
      pset->m_table.add_request(m_itema.get_item(i).m_strPath);
      }
      }
      else
      {
      for(int32_t i = 0; i < m_itema.get_item_count(); i++)
      {
      pset->m_table.remove_request(m_itema.get_item(i).m_strPath);
      }
      }*/

   }

   id file_list::data_get_current_list_layout_id()
   {
      return get_filemanager_item().m_filepath;
   }




   void file_list::browse_sync(::action::context actioncontext)
   {

      if(m_bStatic)
      {

         ::userfs::list_item item(get_app());

         stringa stra;

         get_filemanager_manager()->data_load(get_filemanager_template()->m_dataidStatic,stra);

         synch_lock lock(get_fs_mesh_data()->m_pmutex);

         get_fs_mesh_data()->m_itema.m_itema.remove_all();

         for(int32_t i = 0; i < stra.get_size(); i++)
         {

            item.m_flags.unsignalize_all();

            if(Application.dir().is(stra[i]))
            {
               item.m_flags.signalize(::fs::FlagFolder);
            }
            else
            {
            }

            ::file::path strPath = stra[i];

            string strName = strPath.name();

            item.m_filepath = strPath;

            //item.m_iImage = Session.userex()->shell().get_image(
            //   get_handle(),
            //   item.filepath,
            //   get_document()->get_fs_data()->is_dir(item.m_filepath) ?
            //      ::user::shell::file_attribute_directory : ::user::shell::file_attribute_normal,
            //      ::user::shell::icon_normal);

            item.m_strName = strName;

            get_fs_mesh_data()->m_itema.add_item(item);

         }

         _001OnUpdateItemCount();

         return;

      }

      if(get_filemanager_data()->m_bSetBergedgeTopicFile)
      {
         SetTimer(888888,230,NULL);
      }


      stringa straStrictOrder;

      data_load(     data_get_current_sort_id() + "." + data_get_current_list_layout_id() + ".straStrictOrder",      straStrictOrder);
      index_biunique iaDisplayToStrict;
      icon_layout iconlayout;
      data_get(data_get_current_sort_id() + "." + data_get_current_list_layout_id(),     iconlayout);
      iaDisplayToStrict = iconlayout.m_iaDisplayToStrict;
      index_biunique iaDisplayToStrictNew;

      ::userfs::list_item item(get_app());

      string strParent = get_filemanager_item().m_filepath;

      int32_t iMaxSize;
      iMaxSize = 1000;

      int32_t iSize;
      iSize = 0;

      //{

      //   synch_lock lock(get_fs_mesh_data()->m_pmutex);

      //   get_fs_mesh_data()->m_itema.m_itema.remove_all();

      //}

      m_pathaStrictOrder.remove_all();

      _001OnUpdateItemCount();

      {

         mutex *pm = get_fs_mesh_data()->m_pmutex;

         synch_lock lock(pm);

         ::file::listing & listing = get_document()->m_listing;

         get_fs_mesh_data()->m_itema.m_itema.set_size(listing.get_size());

         m_pathaStrictOrder = listing;

         for (int32_t i = 0; i < listing.get_size(); i++)
         {

            auto & spitem = get_fs_mesh_data()->m_itema.m_itema[i];

            if (spitem.is_null())
            {
               spitem.alloc(allocer());
            }
            else
            {
               spitem->m_flags.unsignalize_all();

            }

            ::file::path & path = listing[i];

            ::file::path fullpath = path;

            if (path.m_iDir < 0)
            {

               path.m_iDir = get_document()->get_fs_data()->is_dir(fullpath) ? 1 : 0;

            }

            if (path.m_iDir == 1)
            {

               spitem->m_flags.signalize(::fs::FlagFolder);

            }

            spitem->m_filepath = fullpath;

            //spitem->m_iImage = Session.userex()->shell().get_image(
            //   get_handle(),
            //   path,
            //   path.m_iDir == 1 ? 
            //   ::user::shell::file_attribute_directory : ::user::shell::file_attribute_normal,
            //   ::user::shell::icon_normal);

            spitem->m_strName = listing.name(i);


         }

      }

      _001OnUpdateItemCount();

      {

         synch_lock lock(get_fs_mesh_data()->m_pmutex);

         if (m_eview == view_icon)
         {
            /*   // primeiro, todos System arquivos que foram removidos
            // ou seja, que existem no array antigo,
            // mas não existe no novo.
            for(index strictOld = 0; strictOld < straStrictOrder.get_count(); strictOld++)
            {
            string str = straStrictOrder[strictOld];
            index find = m_pathaStrictOrder.find_first(str);
            if(find < 0)
            {
            iaDisplayToStrictNew.remove_b(strictOld);
            }
            }*/
            // segundo, reordena conforme a
            // ordem que a listagem de arquivos fornecida pelo
            // sistema operacional pode ser fornecida.
            for (index strictNew = 0; strictNew < m_pathaStrictOrder.get_count(); strictNew++)
            {
               string str = m_pathaStrictOrder[strictNew];
               index strictOld = straStrictOrder.find_first(str);
               if (strictOld >= 0)
               {
                  index iDisplay = iaDisplayToStrict.get_a(strictOld);
                  iaDisplayToStrictNew.set(iDisplay, strictNew);
               }
            }
            // terceiro, adiciona System novos arquivos nos primeiros espaços
            // vazios
            for (index strictNew = 0; strictNew < m_pathaStrictOrder.get_count(); strictNew++)
            {
               string str = m_pathaStrictOrder[strictNew];
               index strictOld = straStrictOrder.find_first(str);
               if (strictOld < 0)
               {
                  iaDisplayToStrictNew.add_b_in_first_free_a(strictNew);
               }
            }
            m_iconlayout.m_iaDisplayToStrict = iaDisplayToStrictNew;
         }
         else
         {
            get_fs_mesh_data()->m_itema.arrange(::fs::arrange_by_name);
         }

      }

///      _001CreateImageList();

      //file_size_add_request(true);
      /*   for(int32_t i = 0; i < m_itema.get_item_count(); i++)
      {
      pset->m_table.add_request(m_itema.get_item(i).m_strPath);
      }*/
      if(m_eview == view_icon)
      {
         data_save(     data_get_current_sort_id() + "."+data_get_current_list_layout_id() + ".straStrictOrder",          m_pathaStrictOrder);
         m_iconlayout.m_iaDisplayToStrict = iaDisplayToStrictNew;
         data_set_DisplayToStrict();
      }

      _001ClearSelection();

      set_viewport_offset(0,0);

      RedrawWindow();


   }


//   void file_list::_001CreateImageList()
//   {
//
//      icon_key iconkey;
//      icon icon;
//
//#ifdef WINDOWSEX
//      for (POSITION pos = m_iconmap.get_start_position(); pos != NULL; m_iconmap.get_next_assoc(pos, iconkey, icon))
//      {
//         DestroyIcon((HICON) *icon.m_picon);
//      }
//#endif
//
//      m_iCreateImageListStep = 0;
//      m_bCreateImageList = true;
//      //if (m_pcreateimagelistthread == NULL)
//      //{
//      //   m_pcreateimagelistthread = new create_image_list_thread(get_app());
//      //   m_pcreateimagelistthread->m_plist = this;
//      //   m_pcreateimagelistthread->begin();
//      //}
//   }
//
//   file_list::create_image_list_thread::create_image_list_thread(::aura::application * papp) :
//      object(papp),
//      thread(papp)
//   {
//   }
//
//   int32_t file_list::create_image_list_thread::run()
//   {
//
//      int32_t iStepSetCount = 84;
//
//      ::file::path path = m_plist->get_filemanager_path();
//
//      Sess(get_app()).userex()->shell().open_folder(m_plist->get_handle(), path);
//
//      try
//      {
//
//         //      int32_t iStepSetSleep = 23;
//         while (get_run_thread())
//         {
//            int32_t i = iStepSetCount;
//            while (i > 0 && get_run_thread())
//            {
//               if (!m_plist->_001CreateImageListStep())
//                  goto endloop;
//               i--;
//            }
//            //m_plist->post_message(MessageMainPost, MessageMainPostCreateImageListItemStepSetRedraw);
//            //Sleep(iStepSetSleep);
//         }
//      endloop:
//         m_plist->post_message(MessageMainPost, MessageMainPostCreateImageListItemRedraw);
//         //synch_lock lock(m_plist->m_pauraapp);
//         m_plist->m_pcreateimagelistthread = NULL;
//         return 0;
//
//      }
//      catch (...)
//      {
//
//
//      }
//
//      Sess(get_app()).userex()->shell().close_folder(path);
//
//   }


   //bool file_list::_001CreateImageListStep()
   //{

   //   synch_lock sl(get_fs_mesh_data()->m_pmutex);

   //   if (m_iCreateImageListStep < 0 || m_iCreateImageListStep >= get_fs_mesh_data()->m_itema.get_count())
   //   {
   //      if (m_bRestartCreateImageList)
   //      {
   //         m_bRestartCreateImageList = false;
   //         m_iCreateImageListStep = 0;
   //         if (m_iCreateImageListStep >= get_fs_mesh_data()->m_itema.get_count())
   //         {
   //            return false;
   //         }
   //      }
   //      else
   //      {
   //         return false;
   //      }
   //   }

   //   ::file::path path;

   //   {

   //      ::userfs::list_item & item = get_fs_mesh_data()->m_itema.get_item((int32_t)m_iCreateImageListStep);
   //      if (&item == NULL)
   //      {

   //         return true;

   //      }
   //      ::file::path & p = item.m_filepath;
   //      if (p.m_iDir < 0)
   //      {

   //         p.m_iDir = get_document()->get_fs_data()->is_dir(p) ? 1 : 0;

   //      }

   //      if (p.m_iDir == 1)
   //      {

   //         item.m_flags.signalize(::fs::FlagFolder);

   //      }
   //      path = p;
   //   }
   //   sl.unlock();

   //   int iImage = Session.userex()->shell().get_image(
   //      get_handle(),
   //      path,
   //      path.m_iDir == 1 ? ::user::shell::file_attribute_directory : ::user::shell::file_attribute_normal,
   //      ::user::shell::icon_normal);

   //   sl.lock();
   //   {

   //      //single_lock sl(m_pmutex, true);

   //      if (m_iCreateImageListStep < 0 || m_iCreateImageListStep >= get_fs_mesh_data()->m_itema.get_count())
   //      {
   //         return true;
   //      }
   //      ::userfs::list_item & item = get_fs_mesh_data()->m_itema.get_item((int32_t)m_iCreateImageListStep);

   //      if (&item == NULL)
   //      {

   //         return true;

   //      }

   //      if (path == item.m_filepath)
   //      {

   //         item.m_iImage = iImage;

   //      }

   //   }


   //   m_iCreateImageListStep++;

   //   return true;

   //}


   void file_list::_001InsertColumns()
   {
      class user::control::descriptor control;


      if (get_filemanager_data()->m_bIconView)
      {
         m_eview = view_icon;
      }

      ::user::list_column column;

      int32_t iCount = 0;

      file_list_callback * pcallback =
         get_filemanager_template()->m_pfilelistcallback;

      if (pcallback != NULL)
      {
         iCount = pcallback->GetActionButtonCount();
      }

      index i;
      for (i = 0; i < iCount; i++)
      {
         control.m_bTransparent = true;
         control.set_type(user::control_type_button);
         control.m_typeinfo = System.type_info < ::user::button >();
         control.m_id = 1000 + i;
         control.add_function(user::control::function_action);
         index iControl = _001AddControl(control);

         column.m_iWidth = 18;
         column.m_iSubItem = i;
         column.m_iControl = iControl;
         column.m_bCustomDraw = true;
         column.m_bEditOnSecondClick = true;
         column.m_uiText = "";
         column.m_pil = pcallback->GetActionButtonImageList(i);
         _001AddColumn(column);
      }


      if (get_filemanager_data()->m_bListSelection)
      {
         column.m_iWidth = get_filemanager_data()->m_iIconSize;
         column.m_iSubItem = i;
         //column.m_bIcon                = true;
         column.m_sizeIcon.cx = get_filemanager_data()->m_iIconSize;
         column.m_sizeIcon.cy = get_filemanager_data()->m_iIconSize;
         column.m_iControl = -1;
         column.m_uiText = "";
         column.m_datakey = "FILE_MANAGER_ID_FILE_NAME";
         column.m_bEditOnSecondClick = false;
         column.m_pil = Session.userex()->shell().GetImageList(get_filemanager_data()->m_iIconSize);
         column.m_pilHover = Session.userex()->shell().GetImageListHover(get_filemanager_data()->m_iIconSize);
         _001AddColumn(column);
         m_iSelectionSubItem = i;

         i++;
      }
      else
      {
         m_iSelectionSubItem = -1;
      }

      control.set_type(user::control_type_edit_plain_text);
      control.m_dataid = "FILE_MANAGER_ID_FILE_NAME";
      //pcontrol->descriptor().m_id = _vms::FILE_MANAGER_ID_FILE_NAME;
      control.set_data_type(user::control::DataTypeString);
      control.add_function(user::control::function_vms_data_edit);
      control.m_typeinfo = System.type_info < ::user::plain_edit > ();
      //control.m_typeinfo = sp(type)();
      control.m_iSubItem = i;
      control.m_id = 1000 + i;
      index iControl = _001AddControl(control);



      column.m_iSubItem = i;
      m_iNameSubItem = i;
      if (get_filemanager_data()->m_bListText)
      {
         m_iNameSubItemText = i;
         column.m_iWidth = 500;
      }
      else
      {
         m_iNameSubItemText = i;
         column.m_iWidth = get_filemanager_data()->m_iIconSize;
      }
      get_fs_mesh_data()->m_iNameSubItemText = m_iNameSubItemText;
      //column.m_bIcon                = true;
      column.m_sizeIcon.cx = get_filemanager_data()->m_iIconSize;
      column.m_sizeIcon.cy = get_filemanager_data()->m_iIconSize;
      column.m_iControl = iControl;
      column.m_uiText = "Name";
      column.m_datakey = "FILE_MANAGER_ID_FILE_NAME";
      column.m_bEditOnSecondClick = true;
      column.m_pil = Session.userex()->shell().GetImageList(get_filemanager_data()->m_iIconSize);
      column.m_pilHover = Session.userex()->shell().GetImageListHover(get_filemanager_data()->m_iIconSize);
      _001AddColumn(column);

      i++;

      if (get_filemanager_data()->m_bFileSize)
      {
         // file/directory size
         column.m_iWidth = 100;
         column.m_iSubItem = i;
         m_iSizeSubItem = i;
         column.m_sizeIcon.cx = 0;
         column.m_sizeIcon.cy = 0;
         column.m_pilHover = NULL;
         column.m_pil = NULL;
         _001AddColumn(column);
      }

   }


   void file_list::_001GetItemText(::user::mesh_item * pitem)
   {

      if (m_bStatic)
         return ::user::list::_001GetItemText(pitem);

      return ::userfs::list::_001GetItemText(pitem);

      /*UNREFERENCED_PARAMETER(iListItem);
      if(iSubItem == m_iNameSubItemText)
      {
      str = get_fs_lim_itema.get_item(iItem).m_strName;
      return true;
      }
      else if(iSubItem == m_iSizeSubItem)
      {
      bool bPendingSize = false;

      bool bGet;
      bGet = System.get_fs_size(str, m_itema.get_item(iItem).m_strPath, bPendingSize);
      if(bGet)
      {
      m_straFileSize.set_at_grow(iItem, str);
      }
      else
      {
      if(iItem < m_straFileSize.get_size())
      {
      str = m_straFileSize[iItem];
      }
      }
      if(bPendingSize)
      {
      schedule_file_size(m_itema.get_item(iItem).m_strPath);
      m_bPendingSize = true;
      }
      return true;
      }
      //else if(m_bStatic)
      //{
      // return ::user::list::_001GetItemText(str, iItem, iSubItem, iListItem);
      //}
      else
      return false;*/
   }

   void file_list::_001GetItemImage(::user::mesh_item * pitem)
   {
      return ::userfs::list::_001GetItemImage(pitem);
      /*      if(iSubItem == m_iSelectionSubItem)
      {
      if(m_rangeSelection.has_item(iItem))
      {
      return 1;
      }
      else
      {
      return 0;
      }
      }
      else if(iSubItem == m_iNameSubItem)
      {
      return m_itema.get_item(iItem).m_iImage;
      }
      else
      return ::user::list::_001GetItemImage(iItem, iSubItem, iListItem);*/

   }


   //bool file_list::TwiHasTranslucency()
   //{
   //   return ::user::list::TwiHasTranslucency() && !m_bCreateImageListRedraw;
   //}


   void file_list::GetSelectedFilePath(stringa & array)
   {
      range range;

      _001GetSelection(range);

      string str;
      //      HRESULT hr;
      for (index i = 0; i < range.get_item_count(); i++)
      {
         item_range & itemrange = range.ItemAt(i);
         for (index iItem = itemrange.get_lower_bound(); iItem <= itemrange.get_upper_bound(); iItem++)
         {
            index iStrict;
            if (m_eview == view_icon)
            {
               iStrict = m_iconlayout.m_iaDisplayToStrict.get_b(iItem);
            }
            else
            {
               iStrict = m_meshlayout.m_iaDisplayToStrict[iItem];
            }
            ::userfs::list_item & item = get_fs_mesh_data()->m_itema.get_item(iStrict);
            if (!item.IsFolder())
            {
               array.add(item.m_filepath);
            }
         }
      }

   }


   void file_list::_001OnMainPostMessage(signal_details * pobj)
   {
      SCAST_PTR(::message::base, pbase, pobj);
         switch (pbase->m_wparam)
      {
         case MessageMainPostCreateImageListItemStepSetRedraw:
         {
                                                                uint32_t dwNow = get_tick_count();
                                                                if (dwNow - m_dwLastRedraw > 784)
                                                                {
                                                                   m_dwLastRedraw = dwNow;
                                                                   RedrawWindow();
                                                                }
         }
            break;
         case MessageMainPostCreateImageListItemRedraw:
         {
                                                         RedrawWindow();
         }
            break;
      }
      pbase->set_lresult(0);
      pbase->m_bRet = true;
   }





   void file_list::_001OnDraw(::draw2d::graphics * pgraphics)
   {

      if (m_iAnimate <= 0)
      {

         ::user::form_list::_001OnDraw(pgraphics);

      }
      else
      {



         // Animation Drawing
         rect rectClipBox;
         pgraphics->GetClipBox(rectClipBox);
         class imaging & imaging = System.visual().imaging();
         imaging.color_blend(
            pgraphics,
            rectClipBox,
            RGB(255, 255, 255),
            127);

         //imaging.bitmap_blend(
         //   pgraphics,
         //   null_point(),
         //   rectClipBox.size(),
         //   m_gdibuffer.GetBuffer(),
         //   null_point(),
         //   (byte)(255 - m_iAnimate * 25));

      }

   }


   void file_list::TakeAnimationSnapshot()
   {
      m_iAnimate = 1;
//      ::user::list::_001OnDraw(m_gdibuffer.GetBuffer());
   }

   void file_list::_017OpenSelected(bool bOpenFile, ::action::context actioncontext)
   {
      ::fs::item_array itema;
      index iItemRange, iItem;
      range range;
      _001GetSelection(range);
      for (iItemRange = 0;
         iItemRange < range.get_item_count();
         iItemRange++)
      {
         item_range itemrange = range.ItemAt(iItemRange);
         for (iItem = itemrange.get_lower_bound();
            iItem <= itemrange.get_upper_bound();
            iItem++)
         {
            if (iItem < 0)
               continue;
            if (iItem >= _001GetItemCount())
               continue;
            index iStrict;
            if (m_eview == view_icon)
            {
               iStrict = m_iconlayout.m_iaDisplayToStrict.get_b(iItem);
            }
            else
            {
               iStrict = m_meshlayout.m_iaDisplayToStrict[iItem];
            }
            ::userfs::list_item & item = get_fs_mesh_data()->m_itema.get_item(iStrict);
            if (item.IsFolder())
            {
               _017OpenFolder(canew (::fs::item(item)), ::action::source::sel(actioncontext));
               break;
            }
            else
            {
               itema.add(canew( ::fs::item(item)));
            }
         }
      }
      if (bOpenFile && itema.get_size() > 0)
      {
         _017OpenFile(itema, ::action::source::sel(actioncontext));
      }
      _001ClearSelection();
   }

   void file_list::_017OpenContextMenuSelected(::action::context actioncontext)
   {

      synch_lock sl(get_fs_mesh_data()->m_pmutex);


      ::fs::item_array itema;
      index iItemRange, iItem;
      range range;
      _001GetSelection(range);
      for (iItemRange = 0;
         iItemRange < range.get_item_count();
         iItemRange++)
      {
         item_range itemrange = range.ItemAt(iItemRange);
         for (iItem = itemrange.get_lower_bound();
            iItem <= itemrange.get_upper_bound();
            iItem++)
         {
            if (iItem < 0)
               continue;
            if (iItem >= get_fs_mesh_data()->m_itema.get_count())
               continue;
            index iStrict;
            if (m_eview == view_icon)
            {
               iStrict = m_iconlayout.m_iaDisplayToStrict.get_b(iItem);
            }
            else
            {
               iStrict = m_meshlayout.m_iaDisplayToStrict[iItem];
            }
            if (get_fs_mesh_data()->m_itema.get_item(iStrict).IsFolder())
            {
               _017OpenContextMenuFolder(canew ( ::fs::item(get_fs_mesh_data()->m_itema.get_item(iStrict))), actioncontext);
               break;
            }
            else
            {
               itema.add(canew(::fs::item(get_fs_mesh_data()->m_itema.get_item(iStrict))));
            }
         }
      }
      if (itema.get_size() > 0)
      {
         _017OpenContextMenuFile(itema, actioncontext);
      }
      else
      {
         _017OpenContextMenu(actioncontext);
      }
      _001ClearSelection();
   }


   void file_list::_001OnInitializeForm(sp(::user::control) pcontrol)
   {
      ASSERT(pcontrol != NULL);
      if (pcontrol == NULL)
         return;

      file_list_callback * pcallback =
         get_filemanager_template()->m_pfilelistcallback;

      sp(::user::button) pbutton = (pcontrol);
      if (pcallback != NULL && pbutton != NULL)
      {
         pbutton->set_button_style(::user::button::style_list);
         pcallback->InitializeActionButton(((int32_t)pcontrol->descriptor().m_id) - 1000, pbutton);
      }
   }

   void file_list::_001OnButtonAction(
      sp(::user::control) pcontrol)
   {
      file_list_callback * pcallback =
         get_filemanager_template()->m_pfilelistcallback;

      if (pcallback != NULL)
      {
         ::fs::item item;
         index iItem = pcontrol->GetEditItem();
         index iStrict;
         if (m_eview == view_icon)
         {
            iStrict = m_iconlayout.m_iaDisplayToStrict.get_b(iItem);
         }
         else
         {
            iStrict = m_meshlayout.m_iaDisplayToStrict[iItem];
         }
         pcallback->OnButtonAction((int32_t)pcontrol->descriptor().m_id - 1000,canew(::fs::item(get_fs_mesh_data()->m_itema.get_item(iStrict))));
      }
   }

   void file_list::GetSelected(::fs::item_array &itema)
   {

      synch_lock sl(get_fs_mesh_data()->m_pmutex);

      index iItemRange, iItem;
      range range;
      _001GetSelection(range);
      index_array iaItem;
      for (iItemRange = 0;
         iItemRange < range.get_item_count();
         iItemRange++)
      {
         item_range itemrange = range.ItemAt(iItemRange);
         for (iItem = MAX(0, itemrange.get_lower_bound());
            iItem <= itemrange.get_upper_bound();
            iItem++)
         {

            if (iItem < get_fs_mesh_data()->m_itema.get_count() && !iaItem.contains(iItem))
            {

               iaItem.add(iItem);

               index iStrict;

               if (m_eview == view_icon)
               {

                  iStrict = m_iconlayout.m_iaDisplayToStrict.get_b(iItem);

               }
               else
               {

                  if (iItem >= m_meshlayout.m_iaDisplayToStrict.get_count())
                     continue;

                  iStrict = m_meshlayout.m_iaDisplayToStrict[iItem];

               }

               sp(::fs::item) spitem(canew(::fs::item));

               spitem->m_filepath = get_fs_mesh_data()->m_itema.get_item(iStrict).m_filepath;

               spitem->m_flags = get_fs_mesh_data()->m_itema.get_item(iStrict).m_flags;

               itema.add(spitem);

            }
         }
      }
   }

   void file_list::_001OnVScroll(signal_details * pobj)
   {
      SCAST_PTR(::message::scroll, pscroll, pobj);
         m_iCreateImageListStep = pscroll->m_nPos;
      m_bRestartCreateImageList = true;
      pobj->m_bRet = false;
   }

   void file_list::_001OnHScroll(signal_details * pobj)
   {
      pobj->m_bRet = false;
   }

   ::count file_list::_001GetItemCount()
   {
      return get_fs_mesh_data()->m_itema.get_count();
   }


   bool file_list::add_item(const char * pszPath, const char * pszTitle)
   {

      synch_lock sl(get_fs_mesh_data()->m_pmutex);

      ::userfs::list_item item(get_app());

      item.m_filepath = pszPath;

      item.m_strName = pszTitle;

      if (Application.dir().is(pszPath))
      {

         item.m_flags.signalize(::fs::FlagFolder);

      }

      get_fs_mesh_data()->m_itema.add_item(item);

      _001OnUpdateItemCount();

      return true;

   }


   sp(image_list) file_list::GetActionButtonImageList(index i)
   {
      if (i == 0)
      {
         return   Session.userex()->shell().GetImageList(get_filemanager_data()->m_iIconSize);

      }
      return NULL;
   }



   void file_list::file_size_add_request(bool bClear)
   {
      UNREFERENCED_PARAMETER(bClear);
      db_server * pcentral = dynamic_cast < db_server * > (&System.m_simpledb.db());
      if (pcentral == NULL)
         return;
      DBFileSystemSizeSet * pset = pcentral->m_pfilesystemsizeset;
      single_lock sl(pset->m_table.m_pmutex, TRUE);

      //      int64_t iSize;
      //      bool bPending;
      for (int32_t i = 0; i < get_fs_mesh_data()->m_itema.get_count(); i++)
      {
         //pset->get_cache_fs_size(iSize, m_itema.get_item(i).m_strPath, bPending);
      }
   }


   void file_list::_001InitializeFormPreData()
   {
      ::filemanager::data * pdata = get_filemanager_data();
      pdata->m_pcallback->OnFileManagerInitializeFormPreData(pdata, GetDlgCtrlId(), this);
   }


   bool file_list::query_drop(index iDisplayDrop, index iDisplayDrag)
   {

      synch_lock sl(get_fs_mesh_data()->m_pmutex);

      if (iDisplayDrag < 0)
         return false;
      if (iDisplayDrop < 0)
         return false;
      if (m_iItemDrag != m_iItemDrop)
      {
         index strict;
         if (m_eview == view_icon)
            strict = m_iconlayout.m_iaDisplayToStrict[iDisplayDrop];
         else
            strict = m_meshlayout.m_iaDisplayToStrict[iDisplayDrop];
         if (strict <= -1)
            return true; // can drop in is_empty place
         else if (strict >= _001GetItemCount())
            return true; // can drop if destination is invalid
         else
         {
            // can drop if destination is folder
            return get_fs_mesh_data()->m_itema.get_item(strict).IsFolder();
         }
      }
      return false;
   }


   bool file_list::do_drop(index iDisplayDrop, index iDisplayDrag)
   {

      synch_lock sl(get_fs_mesh_data()->m_pmutex);

      index strict;

      index strictDrag;

      if (m_eview == view_icon)
      {

         strict = m_iconlayout.m_iaDisplayToStrict[iDisplayDrop];

         strictDrag = m_iconlayout.m_iaDisplayToStrict[iDisplayDrag];

      }
      else
      {

         strict = m_meshlayout.m_iaDisplayToStrict[iDisplayDrop];

         strictDrag = m_meshlayout.m_iaDisplayToStrict[iDisplayDrag];

      }

      if (strict >= 0 && get_fs_mesh_data()->m_itema.get_item(strict).IsFolder())
      {

         ::file::path strPath = get_fs_mesh_data()->m_itema.get_item(strictDrag).m_filepath;

         string strName = strPath.name();

         Application.file().move(get_fs_mesh_data()->m_itema.get_item(strict).m_filepath, strPath);

      }
      else
      {

         ::user::list::do_drop(iDisplayDrop, iDisplayDrag);

      }

      return true;

   }


   COLORREF file_list::get_background_color()
   {
      if (get_filemanager_template() != NULL && get_filemanager_data()->is_topic())
      {
         return ARGB(255, 255, 255, 255);
         //COLORREF cr;
         //if (m_pmanager->m_emode == manager::mode_saving)
         //{

         //   cr = ARGB(255, 255, 210, 180);

         //}
         //else if (m_pmanager->m_emode == manager::mode_import)
         //{

         //   cr = ARGB(255, 180, 210, 255);

         //}
         //else if (m_pmanager->m_emode == manager::mode_export)
         //{

         //   cr = ARGB(255, 255, 250, 210);

         //}
         //else
         //{

         //   cr = ARGB(184, 210, 255, 180);

         //}
         //return cr;
      }
      else
      {
         return ARGB(255, 255, 255, 255);
      }
   }

} // namespace filemanager

