#pragma once


namespace user
{


   namespace wndfrm
   {


      namespace wndfrm_core
      {


         class CLASS_DECL_APP_CORE_WNDFRM_CORE interaction:
            public ::user::wndfrm::interaction
         {
         public:


            


            interaction(::aura::application * papp);
            virtual ~interaction();

            virtual void update();


            virtual void get_frame_schema_list(stringa & stra);


            virtual sp(::user::wndfrm::frame::frame) get_frame_schema(const char * pszSchema);


         };


      } // namespace wndfrm_core


   } // namespace wndfrm


} // namespace user





