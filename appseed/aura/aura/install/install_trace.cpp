//#include "framework.h"


#if defined(INSTALL_SUBSYSTEM)


namespace install
{


   trace::trace(::aura::application * papp) :
      ::object(papp)
   {

      defer_create_mutex();

      m_hfile           = INVALID_HANDLE_VALUE;
      m_iLastStatus     = 0;
      m_iLastGlsStatus  = 0;


   }



   trace::~trace()
   {

      finalize();

   }




   void trace::ensure_trace_file()
   {
      dir::mk(dir::element());
      if (m_hfile != INVALID_HANDLE_VALUE)
      {
         // best really determination that m_hfile is valid, if it is valid, it is not necessary to create or open it
         string str2 = "ensure_trace_file";
         DWORD dwWritten;
         if (WriteFile(m_hfile, str2, (uint32_t)str2.length(), &dwWritten, NULL))
         {
            ::FlushFileBuffers(m_hfile);
            return;
         }
      }

      m_hfile = ::create_file(path::install_log(process_platform_dir_name2()),GENERIC_WRITE,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

      ::SetFilePointer(m_hfile, 0, NULL, FILE_END);

   }


   void trace::rich_trace(const char * psz)
   {
      if (str_begins_ci_dup(psz, "***"))
      {
         m_iLastStatus = 0;
         if (m_strLastStatus != psz)
         {
            m_strLastStatus = psz;
         }
      }
      else if (str_begins_ci_dup(psz, ":::::"))
      {
         m_iLastGlsStatus = 0;
         if (m_strLastGlsStatus != psz)
         {
            m_strLastGlsStatus = psz;
         }
      }
      else
      {
         if (m_strLastStatus.begins_ci("***"))
         {
            m_iLastStatus++;
            if (m_iLastStatus >= 23)
            {
               rich_trace(m_strLastStatus);
            }
         }
         if (m_strLastGlsStatus.begins_ci(":::::"))
         {
            m_iLastGlsStatus++;
            if (m_iLastGlsStatus >= 23)
            {
               rich_trace(m_strLastGlsStatus);
            }
         }
      }
      string str;
      {
         synch_lock lockTrace(m_pmutex);
         m_stra.add(psz);
         str = m_stra.last();
      }
      string str2(str);
      str2 = "\r\n" + str2;
      on_trace(str, str2);
   }

   void trace::trace_add(const char * psz)
   {
      string str;
      {
         synch_lock lockTrace(m_pmutex);
         if (m_stra.is_empty())
            m_stra.add(psz);
         else
            m_stra.element_at(m_stra.get_count() - 1) += psz;
         str = m_stra.element_at(m_stra.get_count() - 1);
      }
      string str2(psz);
      on_trace(str, str2);
   }

   bool trace::isspace_dup(char ch)
   {
      if (ch == '\t')
         return true;
      if (ch == ' ')
         return true;
      if (ch == '\r')
         return true;
      if (ch == '\n')
         return true;
      return false;
   }

   void trace::on_trace(string & str, string & str2)
   {

      if (m_hfile != NULL && str2.length() > 0)
      {

         DWORD dwWritten;
         ::SetFilePointer(m_hfile, 0, NULL, SEEK_END);
         WriteFile(m_hfile, str2, (uint32_t)str2.length(), &dwWritten, NULL);
         ::FlushFileBuffers(m_hfile);

      }

   }


   void trace::on_progress(double dRate)
   {

      trace_progress(dRate);

   }


   void trace::trace_progress(double dRate)
   {
      if (dRate < 0.0)
         dRate = 0.0;
      if (dRate > 1.0)
         dRate = 1.0;
      dRate = dRate * 1000.0 * 1000.0 * 1000.0;
      //int32_t i = ftol(dRate);
      int32_t i = (int32_t)dRate;
      string str;
      str = "|||";
      str += itoa_dup(i);
      rich_trace(str);
   }




   bool trace::initialize()
   {

      ensure_trace_file();

      return true;

   }


   void trace::finalize()
   {

   }



} // namespace install



#endif


