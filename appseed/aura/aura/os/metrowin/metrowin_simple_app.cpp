//#include "framework.h"
//#include "metrowin.h"

namespace aura
{

   app::app()
   {

      m_iReturnCode = 0;

   }

   app::~app()
   {

   }


   int app::main()
   {

      //Sleep(15 * 1000);

#ifdef WINDOWSEX

      __argc = _init_args();
      __targv = _argv;
      _init_file();

      TCHAR *cmd = GetCommandLine();

      // Skip program name
      if (*cmd == _T('"'))
      {
         while (*cmd && *cmd != _T('"'))
            cmd++;
         if (*cmd == _T('"'))
            cmd++;
      }
      else
      {
         while (*cmd > _T(' '))
            cmd++;
      }

      // Skip any white space
      while (*cmd && *cmd <= _T(' '))
         cmd++;

      STARTUPINFO si;
      si.dwFlags = 0;
      GetStartupInfo(&si);

      initialize_primitive_heap();


      initialize_primitive_trace();

      if(!os_initialize())
         return -1;

      if(!main_initialize()) 
         return -1;

      body();

      finalize_primitive_trace();

      main_finalize();

      os_finalize(); 

      finalize_primitive_heap(); 

      //_doexit();
      _term_args();

#endif

      return m_iReturnCode;

   }


   void app::body()
   {


      try
      {

         if(!initialize())
         {

            if(m_iReturnCode > 0)
               m_iReturnCode = -1;

            return;

         }

      }
      catch(...)
      {

         if(m_iReturnCode > 0)
            m_iReturnCode = -1;

         return;

      }

      try
      {

         m_iReturnCode = run();

      }
      catch(...)
      {

         if(m_iReturnCode > 0)
            m_iReturnCode = -1;

         return;

      }

      try
      {

         finalize();

      }
      catch(...)
      {
      }

   }

   bool app::intro()
   {
      return true;
   }

   int app::refrain()
   {

#ifdef WINDOWSEX
      while(true)
      {
         GetMessage(&m_msg, NULL, 0, 0xffffffffu);
         TranslateMessage(&m_msg);
         DispatchMessage(&m_msg);
}
#endif

      return 0;
}

   bool app::end()
   {
      return true;
   }


   int app::simple_app_pre_run()
   {

      return 0;

   }


} // namespace aura



