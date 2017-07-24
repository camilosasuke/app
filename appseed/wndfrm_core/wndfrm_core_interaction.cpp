#include "framework.h"


namespace user
{


   namespace wndfrm
   {


      namespace wndfrm_core
      {


         interaction::interaction(::aura::application * papp) :
            object(papp),
            ::user::wndfrm::interaction(papp)
         {

            ::fork(get_app(), [&]()
            {

               System.visual().set_cursor_set_from_matter("cursor/antialiased-classic");

            });
         
         }


         interaction::~interaction()
         {
         }


         void interaction::update()
         {
         }

         void interaction::get_frame_schema_list(stringa & stra)
         {

            stra.add("001");
            stra.add("002");
            stra.add("005");
            stra.add("008");
            stra.add("011");
            stra.add("013");
            stra.add("Minimal001");

         }


         sp(::user::wndfrm::frame::frame) interaction::get_frame_schema(const char * pszSchema)
         {

            string strSchema(pszSchema);

            if(strSchema == "001")
            {
               return  canew(::user::wndfrm::frame::wndfrm_core::FrameSchemaHardCoded001(get_app()));
            }
            else if(strSchema == "002")
            {
               return  canew(::user::wndfrm::frame::wndfrm_core::FrameSchemaHardCoded002(get_app()));
            }
            else if(strSchema == "005")
            {
               return  canew(::user::wndfrm::frame::wndfrm_core::FrameSchemaHardCoded005(get_app()));
            }
            else if(strSchema == "008")
            {
               return  canew(::user::wndfrm::frame::wndfrm_core::FrameSchemaHardCoded008(get_app()));
            }
            else if(strSchema == "011")
            {
               return  canew(::user::wndfrm::frame::wndfrm_core::FrameSchemaHardCoded011(get_app()));
            }
            else if(strSchema == "013")
            {
               return  canew(::user::wndfrm::frame::wndfrm_core::FrameSchemaHardCoded013(get_app()));
            }
            else if(strSchema == "Minimal001")
            {
               return  canew( ::user::wndfrm::frame::wndfrm_core::FrameSchemaHardCodedMinimal001(get_app()));
            }

            return NULL;

         }


      } // namespace wndfrm


   } // namespace wndfrm


} // namespace user




