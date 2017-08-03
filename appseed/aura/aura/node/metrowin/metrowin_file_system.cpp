#include "framework.h"
#include "metrowin.h"


namespace metrowin
{


   file_system::file_system(::aura::application * papp) :
      ::object(papp),
      ::file::system(papp)
   {

   }


   file_system::~file_system()
   {

   }

   bool file_system::FullPath(string &str, const char * lpszFileIn)
   {
      /*      if(::file::system::FullPath(str, lpszFileIn))
      return true;*/
      if(::str::begins_ci(lpszFileIn, "http://"))
      {
         str = lpszFileIn;
         return true;
      }
      else if(::str::begins_ci(lpszFileIn, "https://"))
      {
         str = lpszFileIn;
         return true;
      }
      wstring wstrFileIn;
      wstrFileIn = ::str::international::utf8_to_unicode(lpszFileIn);
      wstring wstrFileOut;
      //      bool b = vfxFullPath(wstrFileOut.alloc(MAX_PATH * 8), wstrFileIn) != FALSE;
      /*      if(b)
      {
      ::str::international::unicode_to_utf8(str, wstrFileOut);
      }
      return b;*/

      return false;
   }

   bool file_system::FullPath(wstring & wstrFullPath, const wstring & wstrPath)
   {

      //if(::file::system::FullPath(wstrFullPath, wstrPath))
      // return true;

      if(::str::begins_ci(wstrPath, L"http://"))
      {
         wstrFullPath = wstrPath;
         return true;
      }
      else if(::str::begins_ci(wstrPath, L"https://"))
      {
         wstrFullPath = wstrPath;
         return true;
      }

      //return vfxFullPath(wstrFullPath, wstrPath) != FALSE;
      return false;

   }


   UINT file_system::GetFileName(const char * lpszPathName, string & str)
   {
      int nMax = MAX_PATH * 8;
      wstring wstrPathName;
      wstrPathName = ::str::international::utf8_to_unicode(lpszPathName);
      wstring wstrTitle;
      //UINT user = vfxGetFileName(wstrPathName, wstrTitle.alloc(nMax), nMax);
      str = ::str::international::unicode_to_utf8(wstrTitle);
      //return user;
      return 0;
   }

   void file_system::GetModuleShortFileName(HINSTANCE hInst, string & strShortName)
   {
      //vfxGetModuleShortFileName(hInst, strShortName);
   }


   var file_system::length(const char * pszPath)
   {

      var varRet;

      WIN32_FILE_ATTRIBUTE_DATA data;

      if(!GetFileAttributesExW(::str::international::utf8_to_unicode(pszPath), GetFileExInfoStandard, &data))
      {
         varRet.set_type(var::type_null);
      }
      else
      {
         varRet = (uint32_t) data.nFileSizeLow;
      }


      return varRet;

   }

   ::file::file_sp file_system::get_file(var varFile,UINT nOpenFlags,cres * pfesp,::aura::application * papp)
   {

      ::file::file_sp spfile;

      ::file::path strPath;

      if (varFile.get_type() == var::type_string)
      {

         strPath = varFile.get_string();

      }
      else if (varFile.get_type() == var::type_propset)
      {

         if (varFile.has_property("url"))
         {

            strPath = varFile["url"];

            //strPath.trim("\"'");

         }

      }

      if(::str::begins_ci(strPath, "winmetro-Document://") || ::str::begins_ci(strPath,"winmetro-Pictures://") || ::str::begins_ci(strPath, "winmetro-Music://") || ::str::begins_ci(strPath, "winmetro-Videos://"))
      {

         spfile = canew(::metrowin::native_buffer(papp));

         cres cres = spfile->open(strPath,nOpenFlags);

         if (cres.failed())
         {

            return NULL;

         }

         if(pfesp != NULL)
         {

            *pfesp = cres;

         }

         return spfile;

      }

      spfile = ::file::system::get_file(varFile,nOpenFlags,pfesp,papp);

      return spfile;

   }


} // namespace metrowin




