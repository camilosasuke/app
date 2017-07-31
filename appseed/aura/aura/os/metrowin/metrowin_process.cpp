//#include "framework.h"
//#include "metrowin.h"
//#include <Shellapi.h>
//#include <string.h>

#ifdef WINDOWSEX

CLASS_DECL_BOOT int call_async(
   const char * pszPath, 
   const char * pszParam, 
   const char * pszDir,
   int iShow)
{

   SHELLEXECUTEINFOA infoa;

   memset_dup(&infoa, 0, sizeof(infoa));

   infoa.cbSize         = sizeof(infoa);
   infoa.lpFile         = pszPath;
   infoa.lpParameters   = pszParam;
   infoa.lpDirectory    = pszDir;
   infoa.nShow          = iShow;

   int iOk = ::ShellExecuteExA(&infoa);

   return iOk;

}

#endif

#ifdef WINDOWSEX
CLASS_DECL_BOOT DWORD call_sync(
   const char * pszPath, 
   const char * pszParam, 
   const char * pszDir,
   int iShow,
   int iRetry, 
   int iSleep, 
   int (* pfnOnRetry)(int iTry, dword_ptr dwParam),
   dword_ptr dwParam)
{

   SHELLEXECUTEINFOA infoa;

   memset_dup(&infoa, 0, sizeof(infoa));

   infoa.cbSize         = sizeof(infoa);
   infoa.lpFile         = pszPath;
   infoa.lpParameters   = pszParam;
   infoa.lpDirectory    = pszDir;
   infoa.nShow          = iShow;

   infoa.fMask |= SEE_MASK_NOCLOSEPROCESS | SEE_MASK_NOASYNC | SEE_MASK_FLAG_NO_UI;


   ::ShellExecuteExA(&infoa);

   DWORD dwExitCode;
   
   int iTry = 0;

   while(iRetry < 0 || iTry <= iRetry)
   {
      if(!GetExitCodeProcess(infoa.hProcess, &dwExitCode))
         break;
      if(dwExitCode != STILL_ACTIVE)
         break;
      Sleep(84);
      if(pfnOnRetry != NULL)
      {
         if(!pfnOnRetry(iTry, dwParam))
            break;
      }
      iTry++;
   }

   return dwExitCode;

}

#endif

#ifdef WINDOWSEX
int get_current_processor_index()
{


   return ::GetCurrentProcessorNumber();


}

#endif


#ifdef WINDOWSEX

int get_current_process_maximum_affinity()
{
   
   dword_ptr dwProcessAffinityMask;
   dword_ptr dwSystemAffinityMask;
   if(!GetProcessAffinityMask(::GetCurrentProcess(), &dwProcessAffinityMask, & dwSystemAffinityMask))
   {
      return 0;
   }
   int iMax = -1;
   dword_ptr dwMask = 1;
   for(int i = 0; i < sizeof(dwProcessAffinityMask) * 8; i++)
   {
      if((dwMask & dwProcessAffinityMask) != 0)
      {
         iMax = i;
      }
      dwMask = dwMask << 1;
   }

   return iMax;

}

#endif


#ifdef WINDOWSEX

int get_current_process_affinity_order()
{
   
   
   dword_ptr dwProcessAffinityMask;
   dword_ptr dwSystemAffinityMask;
   if(!GetProcessAffinityMask(::GetCurrentProcess(), &dwProcessAffinityMask, & dwSystemAffinityMask))
   {
      return 0;
   }
   int iCount = 0;
   dword_ptr dwMask = 1;
   for(int i = 0; i < sizeof(dwProcessAffinityMask) * 8; i++)
   {
      if((dwMask & dwProcessAffinityMask) != 0)
      {
         iCount++;
      }
      dwMask = dwMask << 1;
   }

   return iCount;


}


#endif


#ifdef WINDOWSEX

bool process_modules(stringa & stra, DWORD processID)
{
   
   HANDLE hProcess;
   
   DWORD cbNeeded;
   
   unsigned int i;

   hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID );

   if (NULL == hProcess)
      return false;

   const int iMaxModuleCount = 1024 * 8;
   
   HMODULE * hMods = new HMODULE[iMaxModuleCount];

   const int iImageSize = MAX_PATH * 8;

   char * szImage = (char *) _ca_alloc(iImageSize);

   if(EnumProcessModules(hProcess, hMods, sizeof(HMODULE) * iMaxModuleCount, &cbNeeded))
   {

      for ( i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
      {

         if(GetModuleFileNameEx( hProcess, hMods[i], szImage, iImageSize / sizeof(char)))
         {
            
            stra.add(szImage);

         }

      }

   }

   _ca_free(szImage, 0);

   delete hMods;

   CloseHandle( hProcess );

   return true;

}

#endif


#ifdef WINDOWSEX

bool load_modules_diff(stringa & straOld, stringa & straNew, const char * pszExceptDir)
{

   bool bFound;

   HMODULE hmodule;

   size_t iLenExcept;

   if(pszExceptDir != NULL)
   {
      
      iLenExcept = strlen_dup(pszExceptDir);

   }
   else
   {

      iLenExcept = 0;

   }

   for(int i = 0; i < straOld.get_count(); i++)
   {

      bFound = false;

      if(iLenExcept > 0)
      {
         
         if(strlen_dup(straOld[i]) > iLenExcept)
         {
         
            if(_strnicmp(straOld[i], pszExceptDir, iLenExcept) == 0)
            {
            
               continue;

            }

         }

      }

      for(int j = 0; j < straNew.get_count(); j++)
      {

         if(stricmp(straOld[i], straNew[j]) == 0)
         {
         
            bFound = true;

            break;

         }

      }

      if(!bFound)
      {
         
         hmodule = NULL;

         // double check, ensure, that the module has not been already loaded
         // it may happen by loading a missing module that loads dependencies that satisfies straOld modules state.

         if(::GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, straOld[i], &hmodule) == FALSE || hmodule == NULL)
         {

            try
            {

               ::LoadLibrary(straOld[i]);

            }
            catch(...)
            {

            }

         }

      }

   }

   return true;

}


#endif



int get_current_process_affinity_order()
{

   return 1;


}


unsigned long translate_processor_affinity(int i)
{

   return 1;


}




CLASS_DECL_AURA int ui_open_url(const char * pszUrl)
{

   string strUrl(pszUrl);

   auto uri = ref new Windows::Foundation::Uri(strUrl);

   Windows::System::Launcher::LaunchUriAsync(uri);

   return 0;

}