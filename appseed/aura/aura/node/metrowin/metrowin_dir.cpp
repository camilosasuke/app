#include "framework.h"
#include "metrowin.h"


namespace metrowin
{


   dir::dir(::aura::application *  papp):
      ::object(papp),
      ::file::dir::system(papp),
      file_watcher::file_watcher(papp),
      ::file_watcher::listener_thread(papp)
   {

   }

   inline bool myspace(char ch)
   {
      return ch == ' ' ||
             ch == '\t' ||
             ch == '\r' ||
             ch == '\n';
   }


   ::file::listing & dir::root_ones(::file::listing & listing,::aura::application *  papp)
   {

      listing.add("winmetro-Pictures://");
      listing.last().m_iDir = 1;
      listing.add("winmetro-Music://");
      listing.last().m_iDir = 1;
      listing.add("winmetro-Videos://");

      listing.last().m_iDir = 1;

      listing.m_straTitle.add("Picture");
      listing.m_straTitle.add("Music");
      listing.m_straTitle.add("Video");

      return listing;

   }





   ::file::listing & dir::ls(::aura::application *  papp, ::file::listing & listing)
   {

      if(listing.m_bRecursive)
      {


         index iStart = listing.get_size();


         {

            RESTORE(listing.m_path);

            RESTORE(listing.m_eextract);

            if(::file::dir::system::ls(papp,listing))
            {

               listing.m_cres = cres(failure);

               return listing;

            }


            ::file::listing dira(papp);

            dira.ls_dir(listing.m_path);

            for(int32_t i = 0; i < dira.get_count(); i++)
            {

               ::file::path dir = dira[i];

               if(dir == listing.m_path)
                  continue;

               listing.m_path = dir;

               if(listing.m_eextract != extract_all)
               {

                  listing.m_eextract = extract_none;

               }

               listing.ls();

            }

         }

         ::file::patha stra;

         ::dir::ls(stra,listing.m_path);

         for(int i = 0; i < stra.get_count(); i++)
         {

            ::file::path strPath = stra[i];

            string strName = strPath.name();

            if(!matches_wildcard_criteria(listing.m_straPattern,strName))
               continue;

            bool bDir = is(strPath, papp);

            if((listing.m_bFile && !bDir) || (listing.m_bDir && bDir))
            {

               listing.add(strPath);

               listing.last().m_iDir = bDir ? 1 : 0;

            }

         }

         for(index i = iStart; i < listing.get_size(); i++)
         {

            listing[i].m_iRelative = listing.m_path.get_length() + 1;

         }

      }
      else
      {

         if(::file::dir::system::ls(papp,listing))
         {

            return listing;

         }

         ::file::patha stra;

         ::dir::ls(stra,listing.m_path);

         for(int i = 0; i < stra.get_count(); i++)
         {

            ::file::path strPath = stra[i];

            string strName = strPath.name();

            if(matches_wildcard_criteria(listing.m_straPattern,strName))
               continue;

            bool bDir = strPath.m_iDir < 0 ? is(strPath, papp) : strPath.m_iDir != 0;

            if((listing.m_bFile && !bDir) || (listing.m_bDir && bDir))
            {

               listing.add(strPath);

               listing.last().m_iDir = bDir ? 1 : 0;

            }

         }

      }

      return listing;

   }






   bool dir::is(const ::file::path & lpcszPath,::aura::application *  papp)
   {

      if(lpcszPath.compare_ci("winmetro-Pictures:") == 0)
         return true;
      if (lpcszPath.compare_ci("winmetro-Music:") == 0)
         return true;
      if (lpcszPath.compare_ci("winmetro-Videos:") == 0)
         return true;

      bool bIsDir;

      uint32_t dwLastError;

      if(m_isdirmap.lookup(lpcszPath,bIsDir,dwLastError))
      {
         if(!bIsDir)
         {
            ::SetLastError(dwLastError);
         }
         return bIsDir;
      }

      bool bIs;

      if(::file::dir::system::is_or_definitively_not(bIs,lpcszPath,papp))
         return bIs;


      string strPath(lpcszPath);
      if(strPath.get_length() >= MAX_PATH)
      {
         if(::str::begins(strPath,"\\\\"))
         {
            strPath = "\\\\?\\UNC" + strPath.Mid(1);
         }
         else
         {
            strPath = "\\\\?\\" + strPath;
         }
      }
      /*
      uint32_t dwAttrib;
      dwAttrib = GetFileAttributesW(::str::international::utf8_to_unicode(strPath));
      /*if(dwAttrib == INVALID_FILE_ATTRIBUTES)
      {
      dwAttrib = GetFileAttributes(lpcszPath);
      }*/

      //bIsDir = (dwAttrib != INVALID_FILE_ATTRIBUTES) && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);

      bIsDir = ::dir::is(strPath);

      m_isdirmap.set(lpcszPath,bIsDir,::GetLastError());

      return bIsDir;
   }

   bool dir::name_is(const ::file::path & str,::aura::application *  papp)
   {
      //OutputDebugString(str);
      strsize iLast = str.get_length() - 1;
      while(iLast >= 0)
      {
         if(str.m_pszData[iLast] != '\\' && str.m_pszData[iLast] != '/' && str.m_pszData[iLast] != ':')
            break;
         iLast--;
      }
      while(iLast >= 0)
      {
         if(str.m_pszData[iLast] == '\\' || str.m_pszData[iLast] == '/' || str.m_pszData[iLast] == ':')
            break;
         iLast--;
      }
      if(iLast >= 0)
      {
         while(iLast >= 0)
         {
            if(str.m_pszData[iLast] != '\\' && str.m_pszData[iLast] != '/' && str.m_pszData[iLast] != ':')
            {
               iLast++;
               break;
            }
            iLast--;
         }
      }
      else
      {
         return true; // assume empty string is root_ones directory
      }


      bool bIsDir;
      uint32_t dwLastError;


      if(m_isdirmap.lookup(str,bIsDir,dwLastError,(int)iLast))
         return bIsDir;


      if(papp->m_bZipIsDir && iLast >= 3 && !strnicmp_dup(&((const char *)str)[iLast - 3],".zip",4))
      {
         m_isdirmap.set(str.Left(iLast + 1),true,0);
         return true;
      }

      strsize iFind = ::str::find_ci(".zip:",str);

      //if(papp->m_bZipIsDir && iFind >= 0 && iFind < iLast)
      //{
      //   bool bHasSubFolder;
      //   uint32_t dwLastError;
      //   if(m_isdirmap.lookup(str,bHasSubFolder,dwLastError))
      //      return bHasSubFolder;
      //   bHasSubFolder = m_pziputil->HasSubFolder(papp,str);
      //   m_isdirmap.set(str.Left(iLast + 1),bHasSubFolder,::GetLastError());
      //   return bHasSubFolder;
      //}


      wstring wstrPath;

      //strsize iLen = ::str::international::utf8_to_unicode_count(str, iLast + 1);

      //wstrPath.alloc(iLen + 32);

      wstrPath = ::str::international::utf8_to_unicode(str,iLast + 1);

      //OutputDebugStringW(wstrPath);

      if(wstrPath.get_length() >= MAX_PATH)
      {
         if(::str::begins(wstrPath,L"\\\\"))
         {
            ::str::begin(wstrPath,L"\\\\?\\UNC");
         }
         else
         {
            ::str::begin(wstrPath,L"\\\\?\\");
         }
      }
      //      uint32_t dwAttrib;
      ///      dwAttrib = GetFileAttributesW(wstrPath);
      /*if(dwAttrib == INVALID_FILE_ATTRIBUTES)
      {
      dwAttrib = GetFileAttributes(strPath);
      }*/

      //   bIsDir = (dwAttrib != INVALID_FILE_ATTRIBUTES) && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);

      bIsDir = ::dir::is(::str::international::unicode_to_utf8(wstrPath));

      m_isdirmap.set(str.Left(iLast + 1),bIsDir,::GetLastError());

      return bIsDir;
   }

   ::file::path dir::votagus()
   {
      return System.dir().ca2module();
   }

   ::file::path dir::time()
   {
      return m_strTimeFolder;
   }

   ::file::path dir::stage()
   {
      return element() / "stage";
   }

   ::file::path dir::stageapp()
   {
      return stage()/ "basis";
   }

   ::file::path dir::netseed()
   {
      return m_strNetSeedFolder;
   }

   // stage in ccvotagus spalib
   ::file::path dir::element()
   {

      single_lock sl(&m_mutex,true);

      return m_strCa2;

   }

   ::file::path dir::time_square(::aura::application * papp, const string & strPrefix, const string & strSuffix)
   {

      UNREFERENCED_PARAMETER(papp);
      UNREFERENCED_PARAMETER(strPrefix);
      UNREFERENCED_PARAMETER(strSuffix);
      return time() / "time";

   }

   ::file::path dir::time_square()
   {
      return time() / "time";
   }


   ::file::path dir::time_log()
   {
      return appdata() / "log";
   }


   bool dir::mk(const ::file::path & lpcsz,::aura::application *  papp)
   {

      if(is(lpcsz,papp))
         return true;

      ::file::patha stra;

      lpcsz.ascendants_path(stra);

      for(int i = 0; i < stra.get_size(); i++)
      {

         ::file::path pathDir = stra[i];

         if(!is(pathDir,papp) && ::GetLastError() != ERROR_ACCESS_DENIED)
         {

            if (!::CreateDirectoryW(::str::international::utf8_to_unicode(pathDir), NULL))
            {

               if (!::CreateDirectoryW(::str::international::utf8_to_unicode("\\\\?\\" + pathDir), NULL))
               {

                  uint32_t dwError = ::GetLastError();

                  if (dwError == ERROR_ALREADY_EXISTS)
                  {
                     string str;
                     str = "\\\\?\\" + stra[i];
                     str.trim_right("\\/");
                     try
                     {
                        Application.file().del(str);
                     }
                     catch (...)
                     {
                     }
                     str = stra[i];
                     str.trim_right("\\/");
                     try
                     {
                        Application.file().del(str);
                     }
                     catch (...)
                     {
                     }
                     if (::CreateDirectoryW(::str::international::utf8_to_unicode("\\\\?\\" + stra[i]), NULL))
                     {
                        m_isdirmap.set(stra[i], true, 0);
                        goto try1;
                     }
                     else
                     {
                        dwError = ::GetLastError();
                     }
                  }
                  string strError = get_system_error_message(dwError);

                  APPTRACE("dir::mk CreateDirectoryW last error(%d)=%s", dwError, strError);
                  //m_isdirmap.set(stra[i], false);
               }
               else
               {
                  m_isdirmap.set(stra[i], true, 0);
               }
            }
            else
            {
               m_isdirmap.set(stra[i],true,0);
            }
try1:

            if(!is(stra[i],papp))
            {
               return false;
            }

         }
      }
      return true;
   }


   bool dir::rm(::aura::application *  papp,const ::file::path & psz,bool bRecursive)
   {

      if(bRecursive)
      {

         ::file::listing straPath(papp);

         straPath.ls(psz);

         for(int i = 0; i < straPath.get_count(); i++)
         {

            if(App(papp).dir().is(straPath[i]))
            {

               rm(papp, psz / straPath[i].name(),true);

            }
            else
            {

               ::DeleteFileW(::str::international::utf8_to_unicode(straPath[i]));

            }

         }

      }

      return RemoveDirectoryW(::str::international::utf8_to_unicode(psz)) != FALSE;

   }




   bool dir::initialize()
   {

      update_module_path();

      string strCa2Module = ca2module();

      m_strCa2 = strCa2Module;

//#if !defined(CUBE) && !defined(VSNORD)
//
//      m_strCa2 -= 2;
//
//#endif


      xml::document doc(get_app());

      doc.load(Application.file().as_string(appdata() / "configuration\\directory.xml"));

      if(doc.get_root()->get_name() == "directory_configuration")
      {

         m_strTimeFolder = doc.get_root()->get_child_value("time");

         m_strNetSeedFolder = doc.get_root()->get_child_value("netseed");

      }
      if(m_strTimeFolder.is_empty())
         m_strTimeFolder = appdata() / "time";

      if(m_strNetSeedFolder.is_empty())
         m_strNetSeedFolder = element() / "net/netseed";

      mk(m_strTimeFolder,get_app());

      if(!is(m_strTimeFolder,get_app()))
         return false;

      mk(m_strTimeFolder / "time",get_app());

      return true;

   }


   ::file::path dir::trash_that_is_not_trash(const ::file::path & psz)
   {

      if(psz == NULL)
         return "";

      if(psz[1] == ':')
      {

         string strDir = psz.folder();

         string str;
         str = strDir.Left(2);
         str += "\\trash_that_is_not_trash\\";
         string strFormat;
         ::datetime::time time;
         time = ::datetime::time::get_current_time();
         strFormat.Format("%04d-%02d-%02d %02d-%02d-%02d\\",time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());
         str += strFormat;
         if(strDir.m_pszData[2] == '\\')
         {
            str += strDir.Mid(3);
         }
         else
         {
            str += strDir.Mid(2);
         }
         return str;
      }

      return "";
   }


   ::file::path dir::appdata()
   {

      return ::file::path(::Windows::Storage::ApplicationData::Current->LocalFolder->Path);

   }


   ::file::path dir::usersystemappdata(::aura::application *  papp, const string & strPrefix)
   {

      UNREFERENCED_PARAMETER(papp);

      return appdata() / strPrefix;

   }


   ::file::path dir::userappdata(::aura::application *  papp)
   {

      return userfolder(papp) / "appdata";

   }


   ::file::path dir::userdata(::aura::application *  papp)
   {

      return userfolder(papp) / "data";

   }


   ::file::path dir::userfolder(::aura::application *  papp)
   {

      string str = appdata();

      string strUserFolderShift;

      if(App(papp).directrix()->m_varTopicQuery.has_property("user_folder_relative_path"))
      {
         strUserFolderShift = App(papp).directrix()->m_varTopicQuery["user_folder_relative_path"].get_string();
      }

      return str / "ca2" / strUserFolderShift;

   }


   ::file::path dir::default_os_user_path_prefix(::aura::application *  papp)
   {

      UNREFERENCED_PARAMETER(papp);

      return "CurrentUser";

   }


   ::file::path dir::default_userappdata(::aura::application *  papp,const string & lpcszPrefix,const string & lpcszLogin)
   {

      return default_userfolder(papp,lpcszPrefix,lpcszLogin) / "appdata";

   }


   ::file::path dir::default_userdata(::aura::application *  papp,const string & lpcszPrefix,const string & lpcszLogin)
   {

      return default_userfolder(papp,lpcszPrefix,lpcszLogin) / "data";

   }


   ::file::path dir::default_userfolder(::aura::application *  papp,const string & strPrefix,const string & strLogin)
   {

      return userfolder(papp) / strPrefix / strLogin;

   }


   ::file::path dir::userquicklaunch(::aura::application *  papp)
   {

      throw todo(get_app());

   }


   ::file::path dir::userprograms(::aura::application *  papp)
   {

      throw todo(get_app());

   }


   ::file::path dir::commonprograms()
   {

      throw todo(get_app());

   }


   bool dir::is_inside_time(const ::file::path & pszPath,::aura::application *  papp)
   {

      return is_inside(time(),pszPath,papp);

   }


   bool dir::is_inside(const ::file::path & pszDir,const ::file::path & strPath,::aura::application *  papp)
   {

      return ::str::begins_ci(pszDir,strPath);

   }


   bool dir::has_subdir(::aura::application *  papp,const ::file::path & pszDir)
   {

      return ::file::dir::system::has_subdir(papp,pszDir);

   }


   ::file::path dir::commonappdata()
   {

      return ::Windows::Storage::ApplicationData::Current->LocalFolder->Path / "commonappdata";

   }

   bool dir::update_module_path()
   {

      m_pathModule = ::path::module();

      m_pathCa2Module = ::dir::ca2_module();

      return true;

   }

   ::file::path dir::get_music_folder()
   {

      return "winmetro-Music://";

   }


   ::file::path dir::get_video_folder()
   {

      return "winmetro-Videos://";

   }


   ::file::path dir::get_image_folder()
   {

      return "winmetro-Pictures://";

   }


} // namespace metrowin












