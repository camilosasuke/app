#pragma once


namespace base
{

   class CLASS_DECL_BASE app:
      virtual public ::base::system
   {
   public:


      int32_t                       m_argc;
      TCHAR **                      m_argv;

      MESSAGE                       m_msg;


      app();
      virtual ~app();

      virtual int32_t main();

      virtual void body();

      virtual int32_t simple_app_pre_run();

      virtual bool intro();
      virtual int32_t refrain();
      virtual bool end();


   };


} // namespace base




