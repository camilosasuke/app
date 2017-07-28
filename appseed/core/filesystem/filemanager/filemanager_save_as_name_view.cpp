//#include "framework.h"


namespace filemanager
{



   save_as_edit_view::save_as_edit_view(::aura::application * papp) :
      object(papp),
      ::user::plain_edit(papp),
      ::user::interaction(papp)
   {

      m_bVoidSync = false;

   }

   void save_as_edit_view::on_update(::user::impact * pSender, LPARAM lHint, object* phint)
   {
      ::user::impact::on_update(pSender, lHint, phint);
      if (phint != NULL)
      {
         if (base_class < ::filemanager::update_hint >::bases(phint))
         {
            ::filemanager::update_hint * puh = (::filemanager::update_hint *) phint;
            if (puh->m_pview == this &&
               puh->is_type_of(::filemanager::update_hint::TypeInitialize))
            {
               //            get_filemanager_manager() = puh->get_filemanager_manager();
               /*            m_pserverNext = simpledb::AppGet()->GetDataServer();
               AddClient(this);
               SetDataInterface(&m_datainterface);
               AddClient(&m_datainterface);
               string str;
               str.Format("file_list(%d,%d)", get_filemanager_data()->m_iTemplate, get_filemanager_data()->m_iDocument);
               if(get_filemanager_data()->m_bTransparentBackground)
               {
               ::user::list::m_etranslucency = ::user::list::TranslucencyPresent;
               }
               DISetSection(str);
               _001UpdateColumns();*/
            }
            else if (puh->is_type_of(::filemanager::update_hint::TypeFilter))
            {
               /*if(puh->m_wstrFilter.is_empty())
               {
               FilterClose();
               }
               else
               {
               FilterBegin();
               Filter1(puh->m_wstrFilter);
               FilterApply();
               }*/
            }
            else if (base_class < ::user::form_update_hint >::bases(phint))
            {
            }
         }
      }
   }


   void save_as_edit_view::browse_sync(::action::context actioncontext)
   {
      if (m_bVoidSync)
         return;
      string strText;
      _001GetText(strText);
      if (strText != get_filemanager_data()->m_pmanager->m_strTopic.title())
      {
         _001SetText(get_filemanager_data()->m_pmanager->m_strTopic.title(), actioncontext);
      }
   }

   void save_as_edit_view::_001OnAfterChangeText(::action::context actioncontext)
   {

      string str;

      _001GetText(str);

      if (Application.dir().is(str))
      {
         get_filemanager_manager()->FileManagerBrowse(str, ::action::source::sync(actioncontext));
      }
      else if (str.find("/") >= 0 || str.find("\\") >= 0)
      {

         ::file::path strName = str;

         while (true)
         {

            strName = strName.folder();

            if (Application.dir().is(strName))
            {

               if (get_filemanager_item().m_filepath != strName)
               {

                  get_filemanager_manager()->FileManagerBrowse(strName, ::action::source::sync(actioncontext));

               }

               break;

            }

            if (strName.is_empty())
            {

               break;

            }

         }

      }

      if (get_filemanager_manager() != NULL)
      {

         get_filemanager_manager()->m_strTopic = str;

      }

   }


   save_as_button::save_as_button(::aura::application * papp) :
      object(papp),
      ::user::interaction(papp)
   {



   }

   void save_as_button::on_update(user::impact *p, LPARAM l , object * o)
   {

      ::filemanager::impact::on_update(p, l, o);

   }

   bool save_as_button::_001OnClick(uint_ptr nFlag, point point)
   {


      ASSERT(get_filemanager_data()->m_pdocumentTopic != NULL);

      ::file::path strTitle;

      ::file::path strPath;

      GetTypedParent<save_as_view>()->m_pedit->_001GetText(strTitle);

      if (strTitle.folder().has_char() && get_filemanager_manager()->get_fs_data()->is_dir(strTitle.folder()))
      {

         strPath = strTitle;

      }
      else if (get_filemanager_manager()->get_fs_data()->is_dir(get_filemanager_item().m_filepath))
      {

         strPath = get_filemanager_item().m_filepath / strTitle;

      }
      else if (strTitle.has_char())
      {

         strPath = get_filemanager_item().m_filepath / strTitle;

      }
      else
      {

         strPath = get_filemanager_item().m_filepath;

      }

      if (get_filemanager_manager()->m_emode == manager::mode_import)
      {

         get_filemanager_data()->m_pdocumentTopic->open_document(strPath);

      }
      else
      {

         bool bSave = !Application.dir().is(strPath);

         if (bSave && get_filemanager_manager()->get_fs_data()->file_exists(strPath))
         {
            if (System.simple_message_box(Session.get_view(), "Do you want to replace the existing file " + strPath + "?", MB_YESNO) == IDNO)
            {
               bSave = false;
            }
         }

         update_hint uh;

         if (bSave)
         {
            if (get_filemanager_data()->m_pdocumentTopic->on_filemanager_save(m_pmanager, strPath))
            {
               uh.set_type(update_hint::TypeTopicSaved);
               uh.m_filepath = strPath;
            }
            else
            {
               uh.set_type(update_hint::TypeTopicSaveFailed);
            }
         }
         else
         {
            uh.set_type(update_hint::TypeTopicCancel);
         }
         
         get_document()->update_all_views(NULL, 0, &uh);

         get_filemanager_data()->m_pdocumentTopic = NULL;

      }

      return true;

   }


   save_as_view::save_as_view(::aura::application * papp) :
      object(papp),
      ::user::split_view(papp),
      ::user::interaction(papp)
   {



   }

   void save_as_view::on_update(::user::impact * pSender, LPARAM lHint, object* phint)
   {
      ::user::impact::on_update(pSender, lHint, phint);
      if (phint != NULL)
      {
         if (base_class < ::filemanager::update_hint >::bases(phint))
         {
            ::filemanager::update_hint * puh = (::filemanager::update_hint *) phint;
            if (puh->m_pview == this &&
               puh->is_type_of(::filemanager::update_hint::TypeInitialize))
            {
               //            get_filemanager_manager() = puh->get_filemanager_manager();
               /*            m_pserverNext = simpledb::AppGet()->GetDataServer();
                           AddClient(this);
                           SetDataInterface(&m_datainterface);
                           AddClient(&m_datainterface);
                           string str;
                           str.Format("file_list(%d,%d)", get_filemanager_data()->m_iTemplate, get_filemanager_data()->m_iDocument);
                           if(get_filemanager_data()->m_bTransparentBackground)
                           {
                           ::user::list::m_etranslucency = ::user::list::TranslucencyPresent;
                           }
                           DISetSection(str);
                           _001UpdateColumns();*/
            }
            else if (puh->is_type_of(::filemanager::update_hint::TypeFilter))
            {
               /*if(puh->m_wstrFilter.is_empty())
               {
               FilterClose();
               }
               else
               {
               FilterBegin();
               Filter1(puh->m_wstrFilter);
               FilterApply();
               }*/
            }
            else if (base_class < ::user::form_update_hint >::bases(phint))
            {
            }
         }
      }
   }


   void save_as_view::on_create_views()
   {

      if (get_pane_count() > 0)
         return;

      m_pmanager = dynamic_cast < ::filemanager::manager * > (get_document());

      SetPaneCount(2);

      SetSplitOrientation(orientation_vertical);

      set_position_rate(0, 0.8);

      initialize_split_layout();

      m_pedit = create_pane_view < save_as_edit_view >(0);

      m_pedit->m_pmanager = m_pmanager;

      if (m_pedit == NULL)
      {

         System.simple_message_box(NULL, "Could not create file list ::user::impact");

      }

      m_pbutton = create_pane_view < save_as_button >(1);

      m_pbutton->set_window_text("Save");

      m_pbutton->m_pmanager = m_pmanager;

   }


} // namespace filemanager



