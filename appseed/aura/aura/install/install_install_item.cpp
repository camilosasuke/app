#include "framework.h"


#if defined(INSTALL_SUBSYSTEM)


namespace install
{


   install_item::install_item(bootstrap * pbootstrap, string strFile, string strPlatform, install_status * pstatus) :
      object(pbootstrap->get_app()),
      m_pbootstrap(pbootstrap),
      m_strFile(strFile),
      m_strPlatform(strPlatform),
      m_pstatus(pstatus)
   {

      m_eitemtype = item_type_spa;

      __begin_thread(get_app(), &install_item::proc, this, 50, 0, 0, NULL, &m_dwThreadId);

   }





   UINT c_cdecl install_item::proc(LPVOID lp)
   {

      install_item * pitem = (install_item *)lp;

      try
      {

         pitem->run();

      }
      catch (...)
      {

         _gen_InterlockedDecrement(&pitem->m_pstatus->m_lProcessing);

      }

      delete pitem;

      return 0;

   }


   void install_item::progress(double dRate)
   {

      if (Application.get_admin())
      {

         if (m_eitemtype == item_type_spa)
         {

            Application.trace(0.05 + ((((double)m_pstatus->m_lTotal - (double)(m_pstatus->m_lProcessing)) * (0.25 - 0.05)) / ((double)m_pstatus->m_lTotal)));

         }

      }

   }


   void install_item::op_spa()
   {

      string strPlatform = m_strPlatform;

      if (m_strFile == "spaadmin")
      {

         if (m_pbootstrap->check_app_app_admin_bin(strPlatform))
         {

            output_debug_string("op_spa spaadmin Success\r\n");
            _gen_InterlockedIncrement(&m_pstatus->m_lOk);

         }
         else
         {

            output_debug_string("op_spa spaadmin Failed\r\n");
            _gen_InterlockedIncrement(&m_pstatus->m_lBad);

         }

      }
      else if (m_strFile == "install")
      {

         if (m_pbootstrap->check_app_app_bin(strPlatform))
         {

            output_debug_string("op_spa install Success\r\n");
            _gen_InterlockedIncrement(&m_pstatus->m_lOk);

         }
         else
         {
            output_debug_string("op_spa install Failed\r\n");
            _gen_InterlockedIncrement(&m_pstatus->m_lBad);

         }

      }
#ifdef WINDOWS
      else if (m_strFile == "vcredist")
      {

         if (m_pbootstrap->check_vcredist(strPlatform))
         {

            output_debug_string("op_spa vcredist Success\r\n");
            InterlockedIncrement(&m_pstatus->m_lOk);

         }
         else
         {
            output_debug_string("op_spa vcredist Failed\r\n");
            InterlockedIncrement(&m_pstatus->m_lBad);

         }

      }
#endif



   }


   void install_item::op_set()
   {

      string strUrlPrefix = m_strUrlPrefix;

      string strPath = m_strPath;

      string strFile = m_strFile;

      string strMd5 = m_strMd5;

      string strPlatform = m_strPlatform;

      string strDownload = dir::name(strPath) / strFile;

      if (!file_exists_dup(strDownload) || _stricmp(file_md5_dup(strDownload).c_str(), strMd5) != 0)
      {

         if (strFile.compare_ci("app_app_admin.exe") == 0)
         {

            System.os().terminate_processes_by_title(strFile);

         }

         string strUrl;

         int iRetry;

         bool bFileNice;

         iRetry = 0;

         strUrl = strUrlPrefix + strFile + ".bz";

         bFileNice = false;

         while (iRetry < 8 && !bFileNice)
         {

            if (Application.http_download(strUrl, strDownload + ".bz"))
            {

               System.compress().unbz(get_app(), strDownload, strDownload + ".bz");

               if (file_exists_dup(strDownload) && _stricmp(file_md5_dup(strDownload), strMd5) == 0)
               {

                  bFileNice = true;

               }


            }

            iRetry++;

         }

         if (!bFileNice)
         {

            // failed by too much retry in any number of the files already downloaded :
            // so, return failure (no eligible app_app_admin.exe file).
            //return "";

         }

      }

   }

   void install_item::run()
   {

      if (m_eitemtype == item_type_spa)
      {

         op_spa();

      }

      _gen_InterlockedDecrement(&m_pstatus->m_lProcessing);

      progress();

   }


} // namespace install


#endif



