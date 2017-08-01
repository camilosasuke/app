//#include "framework.h" // from "base/user/user.h"
//#include "base/user/user.h"


namespace user
{


   ::user::document * server::open_new_document()
   {

      return open_document_file(var::type_null);

   }


   void server::on_request(::create * pcreate)
   {

      sp(::create) cc(canew(::create(Application.creation(), var(var::type_empty), true, NULL)));

      if (pcreate == NULL)
      {
         pcreate = cc;
      }

      request_create(pcreate);

//      return pcreate->m_spCommandLine->m_varQuery["document"].cast < ::user::document >();

   }


   ::user::document * server::open_document_file(var varFile, bool bMakeVisible, ::user::interaction * puiParent)
   {

      sp(::create) cc(canew(::create(Application.creation(), varFile, bMakeVisible, puiParent)));

      request_create(cc);

      return ::user::get_document(cc);

   }


   ::user::document * server::create_subdocument(::user::view_creator_data * pcreatordata)
   {

      return pcreatordata->m_pdoc = open_document_file(NULL, true, pcreatordata->m_pholder);

   }


} // namespace user



