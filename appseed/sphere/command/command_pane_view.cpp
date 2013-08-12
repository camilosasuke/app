#include "framework.h"


namespace command
{


   pane_view::pane_view(sp(::ca2::application) papp) :
      ca2(papp),
      ::user::tab(papp),

      ::user::tab_view(papp),
      ::userex::pane_tab_view(papp),
      place_holder_container(papp)
   {

      m_etranslucency      = TranslucencyPresent;



   }

   pane_view::~pane_view()
   {
   }


   #ifdef DEBUG
   void pane_view::assert_valid() const
   {
	   ::user::view::assert_valid();
   }

   void pane_view::dump(dump_context & dumpcontext) const
   {
	   ::user::view::dump(dumpcontext);
   }
   #endif //DEBUG

   /////////////////////////////////////////////////////////////////////////////
   // pane_view message handlers

   void pane_view::_001OnCreate(::ca2::signal_object * pobj)
   {
//      SCAST_PTR(::ca2::message::create, pcreate, pobj)
      if(pobj->previous())
         return;

      add_tab("3-action-launch", command::PaneViewThreeActionLaunch);
      add_tab("menu", command::PaneViewContextMenu);
      add_tab("primary verbing", command::PaneViewPrimaryCommand);
      add_tab("options", command::PaneViewConfiguration);
      add_tab("file manager", command::PaneViewFileManager);

      set_cur_tab_by_id(command::PaneViewPrimaryCommand);

   }

   void pane_view::on_update(sp(::user::view) pSender, LPARAM lHint, ::ca2::object* pHint)
   {
      ::user::tab_view::on_update(pSender, lHint, pHint);
      if(lHint == 543218)
      {
         set_cur_tab_by_id(command::PaneViewPrimaryCommand);
      }
      if(pHint != NULL)
      {
         if(base < pane_view_update_hint >::bases(pHint))
         {
            pane_view_update_hint * puh = (pane_view_update_hint *) pHint;
            if(puh->is_type_of(pane_view_update_hint::TypeGetView))
            {
            }
            else if(puh->is_type_of(pane_view_update_hint::TypeSetView))
            {
               set_cur_tab_by_id(puh->m_eview);
            }
         }
      }
	      if(pHint != NULL)
      {
   	   if(base < pane_view_update_hint >::bases(pHint))
         {
            pane_view_update_hint * puh = (pane_view_update_hint * ) pHint;
            if(puh->is_type_of(pane_view_update_hint::TypeOnShowKaraoke))
            {
               ( (GetTopLevelFrame()))->SetActiveView(this);
            }
            else if(puh->is_type_of(pane_view_update_hint::TypeOnShowView))
            {

            }
         }
      }


   }

   void pane_view::on_show_view()
   {
      ::userex::pane_tab_view::on_show_view();
      sp(frame) pframe = GetTypedParent < frame > ();

      if(get_view_id() == command::PaneViewFileManager)
      {
         pframe->m_bAutoHideOnOutClick = false;
         pframe->ShowWindow(SW_MAXIMIZE);
      }
      else if(get_view_id() == command::PaneViewContextMenu)
      {
         sp(::filemanager::document) pdoc =  (get_view_uie());
         pdoc->FileManagerBrowse(Application.dir().userappdata("command\\menu"));
      }
      else
      {
         pframe->m_bAutoHideOnOutClick = true;
         //pframe->OnHoverAction(false);
      }
   }


   void pane_view::_001OnSize(::ca2::signal_object * pobj)
   {
	   pobj->previous();

   }


   bool pane_view::pre_create_window(CREATESTRUCT& cs)
   {
      cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	   return ::user::view::pre_create_window(cs);
   }


   void pane_view::on_create_view(::user::view_creator_data * pcreatordata)
   {
      sp(application) papp =  (( (GetParentFrame()))->get_app());
      switch(pcreatordata->m_id)
      {
      case PaneViewContextMenu:
         {
            sp(::filemanager::document) pdoc = papp->filemanager().std().open_child_list(false, true);
            if(pdoc != NULL)
            {
               pdoc->get_filemanager_data()->m_iIconSize = 16;
               pdoc->get_filemanager_data()->m_bListText = true;
               pdoc->get_filemanager_data()->m_bListSelection = false;
               pdoc->get_filemanager_data()->m_ptemplate->m_strFilePopup = "filemanager\\file_popup.xml";
               pdoc->get_filemanager_data()->m_strDISection = "winactionarea_menu";
               pdoc->get_filemanager_data()->m_pcallback = this;
               pdoc->Initialize(true);
               pdoc->update_all_views(NULL, 1234);
               pdoc->update_all_views(NULL, 123458);
               sp(::user::view) pview = pdoc->get_view();
               pdoc->FileManagerBrowse(Application.dir().userappdata("command\\menu"));
               if(pview != NULL)
               {
                  sp(::user::frame_window) pframe =  (pview->GetParentFrame());
                  if(pframe != NULL)
                  {
                  #ifdef WINDOWSEX
                     pframe->ModifyStyle(WS_CAPTION, WS_CHILD, 0);
                     #endif
                     pframe->set_parent(this);
                     pcreatordata->m_pdoc = pdoc;
                     pcreatordata->m_pwnd = pframe;
                  }
               }
            }
         }
         break;
      case PaneViewPrimaryCommand:
         {
            sp(::user::view) pview = create_view < primary_view > ();
            if(pview != NULL)
            {
               pcreatordata->m_pdoc = get_document();
               pcreatordata->m_pwnd = pview;
               pcreatordata->m_iExtendOnParent = 0;
            }
         }
         break;
      case PaneViewFileManager:
         {
            sp(::filemanager::document) pdoc = papp->filemanager().std().OpenChild(false, true);
            if(pdoc != NULL)
            {
               pdoc->get_filemanager_data()->m_strDISection = "winactionarea_filemanager";
               pdoc->Initialize(true);
               pdoc->update_all_views(NULL, 1234);
               pdoc->update_all_views(NULL, 123458);
               sp(::user::view) pview = pdoc->get_view();
               if(pview != NULL)
               {
                  sp(::user::frame_window) pframe =  (pview->GetParentFrame());
                  if(pframe != NULL)
                  {
                  #ifdef WINDOWSEX
                     pframe->ModifyStyle(WS_CAPTION, WS_CHILD, 0);
                     #endif
                     pframe->set_parent(this);
                     pcreatordata->m_pdoc = pdoc;
                     pcreatordata->m_pwnd = pframe;


                  }
               }
            }
         }
         break;
      case PaneViewThreeActionLaunch:
         {
            sp(::filemanager::document) pdoc = papp->filemanager().std().open_child_list(false, true);
            if(pdoc != NULL)
            {
               pdoc->get_filemanager_data()->m_iIconSize = 48;
               pdoc->get_filemanager_data()->m_bListText = false;
               pdoc->get_filemanager_data()->m_bListSelection = false;
               pdoc->get_filemanager_data()->m_pcallback = this;
               pdoc->get_filemanager_data()->m_strDISection = "winactionarea_3-action-launch";
               pdoc->Initialize(true);
               pdoc->update_all_views(NULL, 1234);
               pdoc->update_all_views(NULL, 123458);
               sp(::user::view) pview = pdoc->get_view();
               pdoc->FileManagerBrowse(Application.dir().userappdata("command\\3-action-launch"));
               if(pview != NULL)
               {
                  sp(::user::frame_window) pframe =  (pview->GetParentFrame());
                  if(pframe != NULL)
                  {
                  #ifdef WINDOWSEX
                     pframe->ModifyStyle(WS_CAPTION, WS_CHILD, 0);
                     #endif
                     pframe->set_parent(this);
                     pcreatordata->m_pdoc = pdoc;
                     pcreatordata->m_pwnd = pframe;


                  }
               }
            }
         }
         break;
      case PaneViewConfiguration:
      {
         sp(form_document) pdoc = System.user()->create_form(this, this);
         if(pdoc == NULL)
            return;
         ::user::view_creator_data * pcreatordata = new ::user::view_creator_data;
         sp(::user::view) pview = pdoc->get_typed_view < ::user::view > ();
         form_update_hint uh;
         uh.m_etype = form_update_hint::type_browse;
         uh.m_strForm = "filemanager\\replace_name_in_file_system.xhtml";
         pdoc->update_all_views(NULL, 0, &uh);

         uh.m_etype = form_update_hint::type_get_form_view;
         pdoc->update_all_views(NULL, 0, &uh);

         uh.m_etype = form_update_hint::type_after_browse;
         pdoc->update_all_views(NULL, 0, &uh);


         pcreatordata->m_pwnd = (pview->GetParentFrame());
//         sp(form_child_frame) pframe = (pcreatordata->m_pwnd);
         //pframe->m_iTabId = iId;
         pcreatordata->m_pdoc = pdoc;
      }
      break;
      default:
         ASSERT(FALSE);
         break;
      }
   }

   void pane_view::_001OnMenuMessage(::ca2::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      set_cur_tab_by_id(m_pviewdataOld->m_id);
   }

   void pane_view::install_message_handling(::ca2::message::dispatch * pinterface)
   {
      ::user::view::install_message_handling(pinterface);

	   IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &pane_view::_001OnCreate);
	   IGUI_WIN_MSG_LINK(WM_SIZE, pinterface, this, &pane_view::_001OnSize);
      IGUI_WIN_MSG_LINK(WM_USER + 1122  , this, this, &pane_view::_001OnMenuMessage);

   }

   void pane_view::rotate()
   {
      command::EPaneView eviewNew;
      switch(get_view_id())
      {
      case command::PaneViewContextMenu:
         eviewNew = command::PaneViewPrimaryCommand;
         break;
      case command::PaneViewPrimaryCommand:
         eviewNew = command::PaneViewFileManager;
         break;
      case command::PaneViewFileManager:
         eviewNew = command::PaneViewContextMenu;
         break;
      default:
         eviewNew = command::PaneViewPrimaryCommand;
      }

      set_cur_tab_by_id(eviewNew);
   }

   void pane_view::OnFileManagerOpenFile(::filemanager::data * pdata, ::fs::item_array & itema)
   {

      UNREFERENCED_PARAMETER(pdata);

      if(itema.get_size() > 0)
      {

         int32_t i = 0;

#ifdef WINDOWSEX

         i = (int32_t) ::ShellExecuteW(
            GetTopLevelParent()->get_handle(),
            NULL,
            ::ca2::international::utf8_to_unicode("\"" + itema[0].m_strPath + "\""),
            NULL,
            ::ca2::international::utf8_to_unicode("\"" +  System.dir().name(itema[0].m_strPath) + "\""),
            SW_SHOWNORMAL);

#else

         throw todo(get_app());

#endif

         string str;

         str.Format("%d", i);

         //Application.simple_message_box(str);

         if(i == ERROR_FILE_NOT_FOUND)
         {

         }

      }

      GetParentFrame()->ShowWindow(SW_HIDE);

   }


} // namespace command


