#include "framework.h"
#ifdef WINDOWS
#include <process.h>
#endif


CLASS_DECL_AURA int32_t FUNCTION_DEBUGBOX(const char * pszMessage, const char * pszTitle, int32_t iFlags)
{

   return ::MessageBoxA(NULL,pszMessage,pszTitle,iFlags);

}


void TRACELASTERROR()
{
   sp(::aura::application) m_pauraapp = get_thread_app();
   string strErrorMessage = ::get_system_error_message(::GetLastError());
   TRACE("Error Message :\n%s\n", strErrorMessage.c_str());
}


string FormatMessageFromSystem(uint32_t dwError)
{


   return ::get_system_error_message(dwError);


}


extern "C"
void o_debug_string(const char * psz)
{
   output_debug_string(psz);
}




CLASS_DECL_AURA void trace(e_level elevel, const char * pszTag, const char * pszText, const char * pszFile, int iLine)
{

   strsize iLen;

   iLen = strlen(pszText);

   if (pszFile != NULL)
   {

      iLen += strlen(pszFile);

      iLen += 30;

      if (iLine >= 1)
      {

         iLen += 30;

      }

   }

   hstring hstr(iLen + 8);

   char * psz = hstr;

   strcpy(psz, pszText);

   if (pszFile != NULL)
   {

      strcat(psz, ", \"");

      strcat(psz, pszFile);

      if (iLine >= 1)
      {

         char pszNum[30];

         itoa_dup(pszNum, iLine, 10);

         strcat(psz, "(");

         strcat(psz, pszNum);

         strcat(psz, ")");

      }

      strcat(psz, "\"");

   }

   os_trace(elevel, pszTag, psz);

}



int g_iMemoryCounters = -1;

CLASS_DECL_AURA mutex * g_pmutexMemoryCounters = NULL;

int g_iMemoryCountersStartable = 0;

CLASS_DECL_AURA bool memcnts()
{

   if (g_iMemoryCountersStartable && g_iMemoryCounters < 0)
   {

      g_iMemoryCounters = file_exists_dup(::dir::system() / "config/system/memory_counters.txt") ? 1 : 0;

      if (g_iMemoryCounters)
      {

         g_pmutexMemoryCounters =  new mutex(NULL, FALSE, "Global\\ca2_memory_counters");

      }

   }

   return g_iMemoryCountersStartable && g_iMemoryCounters;

}

::file::path * g_pMemoryCounters = NULL;

CLASS_DECL_AURA ::file::path memcnts_base_path()
{

   if (g_iMemoryCountersStartable && g_pMemoryCounters == NULL)
   {

      g_pMemoryCounters = new ::file::path();

#if defined(METROWIN)

      string strBasePath = ::dir::system() / "memory_counters";

#else

      ::file::path strModule = module_path_from_pid(getpid());

      string strBasePath = ::dir::system() / "memory_counters" / strModule.title() / ::str::from(getpid());

#endif

      *g_pMemoryCounters = strBasePath;

   }

   return *g_pMemoryCounters;

}

