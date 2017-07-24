#pragma once


namespace user
{


   namespace wndfrm
   {

      namespace wndfrm_core
      {


         class CLASS_DECL_APP_CORE_WNDFRM_CORE library:
            virtual public ::aura::library
         {
         public:


            library(::aura::application * papp);
            virtual ~library();


//            virtual bool is_uinteraction_library() override;
            virtual sp(::object) create_object(::aura::application * papp,const char * pszClass, object * p) override;


         };


      } // namespace wndfrm_core


   } // namespace wndfrm


} // namespace user






