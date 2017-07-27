//#include "framework.h"


namespace sphere
{


   application::application()
   {

      m_bLicense = false;

   }


   application::application(const char * pszId)
   {
      construct(pszId);
   }


   application::~application()
   {
   }

   sp(::aura::application) application::get_system()
   {
      return new application();
   }

   void application::construct(const char * pszAppId)
   {
      string strId = pszAppId;
      m_strId = pszAppId;
      char chFirst = '\0';
      if(strId.get_length() > 0)
      {
         chFirst = strId[0];
      }
      if(chFirst == 'd')
      {
         if(strId == "default_file_handler")
         {
            return ::filehandler::application::construct();
         }
      }
      else if(chFirst == 'u')
      {
         if(strId == "userstack")
         {
            return ::userstack::application::construct();
         }
      }


   }


   bool application::initialize_application()
   {


      System.factory().creatable_small < main_frame >();


      string strId = m_strId;
      char chFirst = '\0';
      if(strId.get_length() > 0)
      {
         chFirst = strId[0];
      }
      if(chFirst == 'd')
      {
         if(strId == "default_file_handler")
         {
            return ::filehandler::application::initialize_application();
         }
      }
      else if(chFirst == 'u')
      {
         if(strId == "userstack")
         {
            return ::userstack::application::initialize_application();
         }
      }



      return asphere::application::initialize_application();
   }

   int32_t application::exit_application()
   {


      System.factory().creatable_small < main_frame >();

      string strId = m_strId;
      char chFirst = '\0';
      if(strId.get_length() > 0)
      {
         chFirst = strId[0];
      }
      if(chFirst == 'd')
      {
         if(strId == "default_file_handler")
         {
            return ::filehandler::application::exit_application();
         }
      }
      else if(chFirst == 'u')
      {
         if(strId == "userstack")
         {
            return ::userstack::application::exit_application();
         }
      }


      return asphere::application::exit_application();
   }

   void application::_001OnFileNew()
   {
      string strId = m_strId;
      char chFirst = '\0';
      if(strId.get_length() > 0)
      {
         chFirst = strId[0];
      }
      ::asphere::application::_001OnFileNew();
   }


   bool application::bergedge_start()
   {
      string strId = m_strId;
      char chFirst = '\0';
      if(strId.get_length() > 0)
      {
         chFirst = strId[0];
      }
      if(chFirst == 'd')
      {
         if(strId == "default_file_handler")
         {
            return ::filehandler::application::bergedge_start();
         }
      }
      else if(chFirst == 'u')
      {
         if(strId == "userstack")
         {
            return ::userstack::application::bergedge_start();
         }
      }

      return ::asphere::application::bergedge_start();
   }

   bool application::on_install()
   {
      string strId = m_strId;
      char chFirst = '\0';
      if(strId.get_length() > 0)
      {
         chFirst = strId[0];
      }
      if(chFirst == 'd')
      {
         if(strId == "default_file_handler")
         {
            return ::filehandler::application::on_install();
         }
      }
      else if(chFirst == 'u')
      {
         if(strId == "userstack")
         {
            return ::userstack::application::on_install();
         }
      }

      return asphere::application::on_install();
   }

   bool application::on_uninstall()
   {
      string strId = m_strId;
      char chFirst = '\0';
      if(strId.get_length() > 0)
      {
         chFirst = strId[0];
      }
      if(chFirst == 'd')
      {
         if(strId == "default_file_handler")
         {
            return ::filehandler::application::on_uninstall();
         }
      }
      else if(chFirst == 'u')
      {
         if(strId == "userstack")
         {
            return ::userstack::application::on_uninstall();
         }
      }

      return asphere::application::on_uninstall();
   }


   void application::on_request(::create * pcreate)
   {
      string strId = m_strId;
      char chFirst = '\0';
      if(strId.get_length() > 0)
      {
         chFirst = strId[0];
      }
      if(chFirst == 'd')
      {
         if(strId == "default_file_handler")
         {
            return ::filehandler::application::on_request(pcreate);
         }
      }
      else if(chFirst == 'u')
      {
         if(strId == "userstack")
         {
            return ::userstack::application::on_request(pcreate);
         }
      }
      return ::asphere::application::on_request(pcreate);


   }

   bool application::is_serviceable()
   {


      string strId = m_strId;

      char chFirst = '\0';

      if(strId.get_length() > 0)
      {

         chFirst = strId[0];

      }

      if(chFirst == 'd')
      {

         if(strId == "default_file_handler")
         {

            return ::filehandler::application::is_serviceable();

         }

      }
      
      return ::asphere::application::is_serviceable();

   }

   service_base * application::allocate_new_service()
   {
      string strId = m_strId;
      char chFirst = '\0';
      if(strId.get_length() > 0)
      {
         chFirst = strId[0];
      }
      if(chFirst == 'd')
      {
         if(strId == "default_file_handler")
         {
            return ::filehandler::application::allocate_new_service();
         }
      }
      
      return ::asphere::application::allocate_new_service();
   }


   ::user::document *  application::_001OpenDocumentFile(var varFile)
   {
      string strId = m_strId;
      char chFirst = '\0';
      if(strId.get_length() > 0)
      {
         chFirst = strId[0];
      }
      if(chFirst == 'd')
      {
         if(strId == "default_file_handler")
         {
            return ::filehandler::application::_001OpenDocumentFile(varFile);
         }
      }
      else if(chFirst == 'r')
      {
      }

      return ::asphere::application::_001OpenDocumentFile(varFile);

   }


   int32_t application::run()
   {

      return ::asphere::application::run();

   }


} //namespace sphere



