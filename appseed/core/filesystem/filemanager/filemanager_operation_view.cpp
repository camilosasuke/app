//#include "framework.h"


namespace filemanager
{


   operation_view::operation_view(::aura::application * papp):
      object(papp),
      ::user::split_layout(papp),

      ::user::split_view(papp),
      place_holder_container(papp)
   {
      }

   operation_view::~operation_view()
   {
   }


   void operation_view::_001OnCreate(signal_details * pobj)
   {
      pobj->previous();

      get_document()->m_thread.m_pview = this;

      m_pviewcreator = canew(::user::view_creator());

      SetPaneCount(2);

      SetSplitOrientation(orientation_horizontal);

      set_position_rate(0,0.3);

      initialize_split_layout();

      m_plistview = create_view < operation_list_view >();
      if(m_plistview == NULL)
      {
         System.simple_message_box(NULL,"Could not create transfer list ::user::impact");
      }
      SetPane(0,m_plistview,false);


      m_pinfoview = create_view < operation_info_view >();
      if(m_pinfoview == NULL)
      {
         System.simple_message_box(NULL,"Could not create transfer information ::user::impact");
      }
      SetPane(1,m_pinfoview,false);

   }

   void operation_view::install_message_handling(::message::dispatch * pinterface)
   {
      ::user::split_view::install_message_handling(pinterface);
      IGUI_WIN_MSG_LINK(WM_CREATE,pinterface,this,&operation_view::_001OnCreate);
      IGUI_WIN_MSG_LINK(MessageMainPost,pinterface,this,&operation_view::_001OnMainPostMessage);
      IGUI_WIN_MSG_LINK(WM_DESTROY,pinterface,this,&operation_view::_001OnDestroy);
   }

   sp(operation_document) operation_view::get_document()
   {
      return  (::user::impact::get_document());
   }

   void operation_view::_001OnMainPostMessage(signal_details * pobj)
   {
      SCAST_PTR(::message::base,pbase,pobj);
      if(pbase->m_wparam == MessageMainPostFileOperation)
      {
         m_pinfoview->OnFileOperationStep((int32_t)pbase->m_lparam,false);
         m_plistview->OnFileOperationStep((int32_t)pbase->m_lparam,false);
      }
      else if(pbase->m_wparam == MessageMainPostFileOperationFinal)
      {
         m_pinfoview->OnFileOperationStep((int32_t)pbase->m_lparam,true);
         m_plistview->OnFileOperationStep((int32_t)pbase->m_lparam,true);
      }
   }

   void operation_view::_001OnDestroy(signal_details *pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }


} // namespace filemanager
























