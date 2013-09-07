#pragma once


namespace core
{

   class CLASS_DECL_ca2 print_job :
      virtual public ::core::job
   {
   public:

      int32_t      m_iPageStart;
      int32_t      m_iPageCount;

      int32_t      m_iPrintingPage;

      print_job(sp(base_application) papp);
      virtual ~print_job();


      virtual int32_t run() = 0;

   };

} // namespace core
