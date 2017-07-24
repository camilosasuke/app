//#include "framework.h"
//#include "windows.h"


namespace windows
{


   namespace axis
   {


   dir::dir(::aura::application * papp) :
      ::object(papp),
      ::file::dir::system(papp),
      ::file::dir::axis::system(papp),
      ::windows::dir(papp),
      file_watcher(papp),
      listener_thread(papp)
   {
      

   }

   //path::path(::aura::application * papp) :
   //   ::object(papp)
   //{
   //}


   //inline bool myspace(char ch)
   //{
   //   return ch == ' ' ||
   //          ch == '\t' ||
   //          ch == '\r' ||
   //          ch == '\n';
   //}



   //string dir::path(const char * pszFolder, strsize iLenFolder, const char * pszRelative, strsize iLenRelative, const char * psz2, strsize iLen2, bool bUrl)
   //{

   //   bool bEmptyRelative = iLenRelative == 0 || pszRelative == NULL || *pszRelative == '\0';
   //   bool bEmpty2 = iLen2 == 0 || psz2 == NULL || *psz2 == '\0';

   //   if(bEmptyRelative && bEmpty2)
   //      return pszFolder;

   //   string strPath;
   //   LPSTR lpsz;

   //   if(bEmptyRelative)
   //   {
   //      pszRelative = psz2;
   //      iLenRelative = iLen2;
   //   }

   //   while((pszFolder[iLenFolder - 1] == '\\' || pszFolder[iLenFolder - 1] == '/') && iLenFolder > 0)
   //   {
   //      if(bUrl)
   //      {
   //         if((iLenFolder - 2) >= 0 && (pszFolder[iLenFolder - 2] == '\\' || pszFolder[iLenFolder - 2] == '/' || pszFolder[iLenFolder - 2] == ':'))
   //         {
   //            if(pszFolder[iLenFolder - 2] == ':')
   //            {
   //               break;
   //            }
   //            else
   //            {
   //               if((iLenFolder - 3) >= 0 && (pszFolder[iLenFolder - 3] == ':'))
   //               {
   //                  iLenFolder--;
   //                  break;
   //               }
   //            }
   //         }
   //         else
   //         {
   //            iLenFolder--;
   //         }
   //      }
   //      else
   //      {
   //         iLenFolder--;
   //      }
   //   }

   //   while(*pszRelative != '\0' && (*pszRelative == '\\' || *pszRelative == '/') && iLenRelative > 0)
   //   {
   //      pszRelative++;
   //      iLenRelative--;
   //   }

   //   if(bEmptyRelative || bEmpty2)
   //   {
   //      lpsz = strPath.GetBufferSetLength(iLenFolder + 1 + iLenRelative);
   //      strncpy(lpsz, pszFolder, iLenFolder);
   //      if(strnicmp(&lpsz[iLenFolder - 5], ".zip:", 5) == 0)
   //      {
   //         iLenFolder--;
   //      }
   //      else
   //      {
   //         lpsz[iLenFolder] = '/';
   //      }
   //      strncpy(&lpsz[iLenFolder + 1], pszRelative, iLenRelative);
   //      lpsz[iLenFolder + 1 + iLenRelative] = '\0';
   //      {
   //         if(bUrl)
   //         {
   //            while(*lpsz++ != '\0')
   //               if(*lpsz == '\\') *lpsz = '/';
   //         }
   //         else
   //         {
   //            while(*lpsz++ != '\0')
   //               if(*lpsz == '/') *lpsz = '\\';
   //         }
   //      }
   //      strPath.ReleaseBuffer(iLenFolder + 1 + iLenRelative);
   //      return strPath;
   //   }

   //   while((pszRelative[iLenRelative - 1] == '\\' || pszRelative[iLenRelative - 1] == '/') && iLenRelative > 0)
   //   {
   //      iLenRelative--;
   //   }

   //   while(*psz2 != '\0' && (*psz2 == '\\' || *psz2 == '/') && iLen2 > 0)
   //   {
   //      psz2++;
   //      iLen2--;
   //   }

   //   lpsz = strPath.GetBufferSetLength(iLenFolder + 1 + iLenRelative + 1 + iLen2);
   //   strncpy(lpsz, pszFolder, iLenFolder);
   //   lpsz[iLenFolder] = '/';
   //   strncpy(&lpsz[iLenFolder + 1], pszRelative, iLenRelative);
   //   lpsz[iLenFolder + 1 + iLenRelative] = '/';
   //   strncpy(&lpsz[iLenFolder + 1 + iLenRelative + 1], psz2, iLen2);
   //   lpsz[iLenFolder + 1 + iLenRelative + 1 + iLen2] = '\0';
   //   {
   //      if(bUrl)
   //      {
   //         while(*lpsz++ != '\0')
   //            if(*lpsz == '\\') *lpsz = '/';
   //      }
   //      else
   //      {
   //         while(*lpsz++ != '\0')
   //            if(*lpsz == '/') *lpsz = '\\';
   //      }
   //   }
   //   strPath.ReleaseBuffer(iLenFolder + 1 + iLenRelative + 1 + iLen2);
   //   return strPath;
   //}



   //string dir::relpath(const char * lpcszSource, const char * lpcszRelative, const char * psz2)
   //{
   //   const char * pszRequest;
   //   if(::url::is_url(lpcszSource, &pszRequest))
   //   {
   //      if(::str::begins(lpcszRelative, "/"))
   //      {
   //         return path((const char *) string(lpcszSource, pszRequest - lpcszSource), lpcszRelative);
   //      }
   //      else if(*pszRequest == '\0' || ::str::ends(lpcszSource, "/"))
   //      {
   //         return path(lpcszSource, lpcszRelative, psz2);
   //      }
   //      else
   //      {
   //         return path((const char *) name(lpcszSource), lpcszRelative, psz2);
   //      }
   //   }
   //   else
   //   {
   //      if(::str::ends(lpcszSource, "\\") || ::str::ends(lpcszSource, "/"))
   //      {
   //         return path(lpcszSource, lpcszRelative, psz2);
   //      }
   //      else
   //      {
   //         return path((const char *) name(lpcszSource), lpcszRelative, psz2);
   //      }
   //   }
   //}

   //bool path::is_equal(const char * lpszPath1, const char * lpszPath2)
   //{
   //   // use case insensitive compare as a starter
   //   if (lstrcmpi(lpszPath1, lpszPath2) != 0)
   //   return FALSE;

   //   // on non-DBCS systems, we are done
   //   if (!GetSystemMetrics(SM_DBCSENABLED))
   //   return TRUE;

   //   // on DBCS systems, the file name may not actually be the same
   //   // in particular, the file system is case sensitive with respect to
   //   // "full width" roman characters.
   //   // (ie. fullwidth-R is different from fullwidth-r).
   //   int32_t nLen = lstrlen(lpszPath1);
   //   if (nLen != lstrlen(lpszPath2))
   //   return FALSE;
   //   ASSERT(nLen < _MAX_PATH);

   //   // need to get both CT_CTYPE1 and CT_CTYPE3 for each filename
   //   LCID lcid = GetThreadLocale();
   //   WORD aCharType11[_MAX_PATH];
   //   VERIFY(GetStringTypeEx(lcid, CT_CTYPE1, lpszPath1, -1, aCharType11));
   //   WORD aCharType13[_MAX_PATH];
   //   VERIFY(GetStringTypeEx(lcid, CT_CTYPE3, lpszPath1, -1, aCharType13));
   //   WORD aCharType21[_MAX_PATH];
   //   VERIFY(GetStringTypeEx(lcid, CT_CTYPE1, lpszPath2, -1, aCharType21));
   //   #ifdef DEBUG
   //   WORD aCharType23[_MAX_PATH];
   //   VERIFY(GetStringTypeEx(lcid, CT_CTYPE3, lpszPath2, -1, aCharType23));
   //   #endif

   //   // for every C3_FULLWIDTH character, make sure it has same C1 value
   //   int32_t i = 0;
   //   for (const char * lpsz = lpszPath1; *lpsz != 0; lpsz = _tcsinc(lpsz))
   //   {
   //   // check for C3_FULLWIDTH characters only
   //   if (aCharType13[i] & C3_FULLWIDTH)
   //   {
   //   #ifdef DEBUG
   //   ASSERT(aCharType23[i] & C3_FULLWIDTH); // should always match!
   //   #endif

   //   // if CT_CTYPE1 is different then file system considers these
   //   // file names different.
   //   if (aCharType11[i] != aCharType21[i])
   //   return FALSE;
   //   }
   //   ++i; // look at next character type
   //   }
   //   return TRUE; // otherwise file name is truly the same
   //}

   //void dir::root_ones(stringa & patha, stringa & straTitle, ::aura::application * papp)
   //{
   //   DWORD dwSize = ::GetLogicalDriveStrings(0, NULL);
   //   LPTSTR lpszAlloc = (LPTSTR) malloc(dwSize + 1);
   //   LPTSTR lpsz = lpszAlloc;
   //   dwSize = ::GetLogicalDriveStrings(dwSize + 1, lpsz);

   //   string str;
   //   while(*lpsz)
   //   {
   //      str.Empty();
   //      while(*lpsz)
   //      {
   //         str += *lpsz;  
   //         lpsz++;
   //      }
   //      patha.add(str);
   //      str.trim(":/\\");
   //      straTitle.add("Drive " + str);
   //      lpsz++;
   //   }

   //   free(lpszAlloc);
   //}

   
   ::file::listing & dir::ls(::aura::application * papp, ::file::listing & listing)
   {
      
      if(::file::dir::axis::system::ls(papp,listing).succeeded())
      {

         return listing;
         
      }

      return ::windows::dir::ls(papp,listing);

   }


   ::file::listing & dir::ls_relative_name(::aura::application * papp, ::file::listing & listing)
   {

      if (::file::dir::axis::system::ls_relative_name(papp, listing).succeeded())
      {

         return listing;

      }

      return ::windows::dir::ls_relative_name(papp, listing);

   }




   //bool dir::is(const char * lpcszPath, ::aura::application * papp)
   //{
   //   
   //   if(::windows::dir::is(lpcszPath,papp))
   //      return true;

   //   if(::file::dir::axis::system::is(lpcszPath, papp))
   //      return true;

   //   return false;
   //   
   //}
   //   
   bool dir::is(const ::file::path & strPath,::aura::application * papp)
   {
      
      if(::windows::dir::is(strPath,papp))
         return true;

      if(::file::dir::axis::system::is(strPath,papp))
         return true;

      return false;

   }

   bool dir::name_is(const ::file::path & str,::aura::application * papp)
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


      uint32_t uiLastError;

      if(m_isdirmap.lookup(str, bIsDir, uiLastError, (int32_t) iLast))
      {
         if(!bIsDir)
         {
            ::SetLastError(uiLastError);
         }
         return bIsDir;
      }


      if(::get_thread() != NULL && ::get_thread()->m_bZipIsDir && iLast >= 3 && !strnicmp_dup(&((const char *)str)[iLast - 3],".zip",4))
      {
         m_isdirmap.set(str.Left(iLast + 1), true, 0);
         return true;
      }
      
      strsize iFind = ::str::find_file_extension("zip:", str, 0, iLast);

      if(::get_thread() != NULL && ::get_thread()->m_bZipIsDir && iFind >= 0 && iFind < iLast)
      {
         bool bHasSubFolder;
         if(m_isdirmap.lookup(str, bHasSubFolder, uiLastError))
         {
            if(!bHasSubFolder)
            {
               ::SetLastError(uiLastError);
            }
            return bHasSubFolder;
         }
         bHasSubFolder = m_pziputil->has_sub_folder(papp, str);
         m_isdirmap.set(str.Left(iLast + 1), bHasSubFolder, bHasSubFolder ? 0 : ::GetLastError());
         return bHasSubFolder;
      }


      wstring wstrPath;
      
      //strsize iLen = ::str::international::utf8_to_unicode_count(str, iLast + 1);

      //wstrPath.alloc(iLen + 32);

      wstrPath = ::str::international::utf8_to_unicode(str, iLast + 1);

      //OutputDebugStringW(wstrPath);

      if(wstrPath.get_length() >= MAX_PATH)
      {
         if(::str::begins(wstrPath, L"\\\\"))
         {
            ::str::begin(wstrPath, L"\\\\?\\UNC");
         }
         else
         {
            ::str::begin(wstrPath, L"\\\\?\\");
         }
      }
      DWORD dwAttrib;
      dwAttrib = GetFileAttributesW(wstrPath);
      /*if(dwAttrib == INVALID_FILE_ATTRIBUTES)
      {
         dwAttrib = GetFileAttributes(strPath);
      }*/
      
      bIsDir = (dwAttrib != INVALID_FILE_ATTRIBUTES) && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
      
      m_isdirmap.set(str.Left(iLast + 1), bIsDir, bIsDir ? 0 : ::GetLastError());

      return bIsDir;
   }


   //string dir::time(const char * lpcsz, const char * lpcsz2)
   //{
   //   return path(m_strTimeFolder, lpcsz, lpcsz2);
   //}

   //string dir::stage(const char * lpcsz, const char * lpcsz2)
   //{
   //   return path(element("stage"), lpcsz, lpcsz2);
   //}

   //string dir::stageapp(const char * lpcsz, const char * lpcsz2)
   //{
   //   return path(stage("basis", lpcsz), lpcsz2);
   //}

   //string dir::netseed(const char * lpcsz, const char * lpcsz2)
   //{
   //   return path(m_strNetSeedFolder, lpcsz, lpcsz2);
   //}

   //// stage in ca2os spalib
   //string dir::element(const char * lpcsz, const char * lpcsz2)
   //{
   //   
   //   single_lock sl(&m_mutex, true);

   //   return path(m_strCa2, lpcsz, lpcsz2);

   //}

   //string dir::element(const string & str, const char * lpcsz2)
   //{
   //   
   //   single_lock sl(&m_mutex, true);

   //   return path(m_strCa2, str, lpcsz2);

   //}

   //string dir::element(const char * lpcsz, const string & str2)
   //{
   //   
   //   single_lock sl(&m_mutex, true);

   //   return path(m_strCa2, lpcsz, str2);

   //}

   //string dir::element(const string & str, const string & str2)
   //{
   //   
   //   single_lock sl(&m_mutex, true);

   //   return path(m_strCa2, str, str2);

   //}

   //string dir::element(const string & str)
   //{
   //   
   //   single_lock sl(&m_mutex, true);

   //   return path(m_strCa2, str);

   //}

   //string dir::element()
   //{
   //   
   //   single_lock sl(&m_mutex, true);

   //   return m_strCa2;

   //}


   //string dir::module(const char * lpcsz, const char * lpcsz2)
   //{
   //   string str = System.get_module_folder();
   //   return path(str, lpcsz, lpcsz2);
   //}

   //string dir::ca2module(const char * lpcsz, const char * lpcsz2)
   //{
   //   string str = System.get_ca2_module_folder();
   //   return path(str, lpcsz, lpcsz2);
   //}


   //void dir::time_square(string &str)
   //{
   //   str = time("time");
   //}

   //string dir::time_log(const char * pszId)
   //{
   //   string strLogBaseDir;
   //   strLogBaseDir = appdata("log");
   //   return path(strLogBaseDir, pszId);
   //}

   //bool dir::mk(const char * lpcsz, ::aura::application * papp)
   //{

   //   if(is(lpcsz, papp))
   //      return true;

   //   stringa stra;
   //   System.file().get_ascendants_path(lpcsz, stra);
   //   for(int32_t i = 0; i < stra.get_size(); i++)
   //   {
   //      
   //      string strDir = stra[i];

   //      if(!is(strDir, papp))
   //      {
   //         
   //         if(::CreateDirectoryW(::str::international::utf8_to_unicode("\\\\?\\" + strDir), NULL))
   //         {

   //            m_isdirmap.set(strDir, true, 0);

   //         }
   //         else
   //         {

   //            DWORD dwError = ::GetLastError();

   //            if (dwError == ERROR_ALREADY_EXISTS)
   //            {

   //               if (::dir::is(strDir))
   //               {
   //                  m_isdirmap.set(strDir, true, 0);
   //               }
   //               else
   //               {
   //                  string str;
   //                  str = "\\\\?\\" + strDir;
   //                  str.trim_right("\\/");
   //                  try
   //                  {
   //                     System.file().del(str);
   //                  }
   //                  catch (...)
   //                  {
   //                  }
   //                  str = stra[i];
   //                  str.trim_right("\\/");
   //                  try
   //                  {
   //                     System.file().del(str);
   //                  }
   //                  catch (...)
   //                  {
   //                  }
   //                  if (::CreateDirectoryW(::str::international::utf8_to_unicode("\\\\?\\" + stra[i]), NULL))
   //                  {
   //                     m_isdirmap.set(stra[i], true, 0);
   //                     goto try1;
   //                  }
   //                  else
   //                  {
   //                     dwError = ::GetLastError();
   //                  }
   //               }
   //               char * pszError;
   //               FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, 0, (LPTSTR)&pszError, 8, NULL);

   //               //TRACE("dir::mk CreateDirectoryW last error(%d)=%s", dwError, pszError);
   //               ::LocalFree(pszError);
   //               //m_isdirmap.set(stra[i], false);
   //            }
   //         }
   //         try1:
   //         
   //         if(!is(stra[i], papp))
   //         {
   //            return false;
   //         }
   //         
   //      }
   //   }
   //   return true;
   //}

   //bool dir::rm(::aura::application * papp, const char * psz, bool bRecursive)
   //{
   //   if(bRecursive)
   //   {
   //      ::file::patha patha;
   //      ::file::patha straTitle;
   //      ls(papp, psz, &patha, &straTitle);         
   //      for(int32_t i = 0; i < patha.get_count(); i++)
   //      {
   //         if(is(patha[i], papp))
   //         {
   //            rm(papp, path(psz, straTitle[i]), true);
   //         }
   //         else
   //         {
   //            ::DeleteFile(patha[i]);
   //         }
   //      }
   //   }
   //   return RemoveDirectory(psz) != FALSE;
   //}


   //string dir::name(const char * path1)
   //{
   //   const char * psz = path1 + strlen(path1) - 1;
   //   while(psz >= path1)
   //   {
   //      if(*psz != '\\' && *psz != '/' && *psz != ':')
   //         break;
   //      psz--;
   //   }
   //   while(psz >= path1)
   //   {
   //      if(*psz == '\\' || *psz == '/' || *psz == ':')
   //         break;
   //      psz--;
   //   }
   //   if(psz >= path1) // strChar == "\\" || strChar == "/"
   //   {
   //      const char * pszEnd = psz;
   //      /*while(psz >= path1)
   //      {
   //         if(*psz != '\\' && *psz != '/' && *psz != ':')
   //            break;
   //         psz--;
   //      }*/
   //      return string(path1, pszEnd - path1 + 1);
   //   }
   //   else
   //   {
   //      return "";
   //   }
   //}

   //string dir::name(const string & str)
   //{
   //   
   //   strsize iLast = str.get_length() - 1;

   //   while(iLast >= 0)
   //   {
   //      if(str.m_pszData[iLast] != '\\' && str.m_pszData[iLast] != '/' && str.m_pszData[iLast] != ':')
   //         break;
   //      iLast--;
   //   }
   //   while(iLast >= 0)
   //   {
   //      if(str.m_pszData[iLast] == '\\' || str.m_pszData[iLast] == '/' || str.m_pszData[iLast] == ':')
   //         break;
   //      iLast--;
   //   }
   //   if(iLast >= 0)
   //   {
   //      while(iLast >= 0)
   //      {
   //         if(str.m_pszData[iLast] != '\\' && str.m_pszData[iLast] != '/' && str.m_pszData[iLast] != ':')
   //            break;
   //         iLast--;
   //      }
   //      return str.Left(iLast + 1);
   //   }
   //   else
   //   {
   //      return "";
   //   }
   //}


   //class ::file::path & dir::path()
   //{
   //   return m_path;
   //}


   bool dir::initialize()
   {

      if(!::windows::dir::initialize())
         return false;
      

//         string strCa2Module = ca2module();
//
//         m_strCa2 = strCa2Module;
//
//#ifndef CUBE
//
//         System.file().path().eat_end_level(m_strCa2, 2, "\\");
//
//#endif
//
//         SHGetSpecialFolderPath(
//            NULL,
//            m_strCommonAppData,
//            CSIDL_COMMON_APPDATA,
//            FALSE);
//         SHGetSpecialFolderPath(
//            NULL,
//            m_strProfile,
//            CSIDL_PROFILE,
//            FALSE);
//         SHGetSpecialFolderPath(
//            NULL,
//            m_strAppData,
//            CSIDL_APPDATA,
//            FALSE);
//         SHGetSpecialFolderPath(
//            NULL,
//            m_strPrograms,
//            CSIDL_PROGRAMS,
//            FALSE);
//         SHGetSpecialFolderPath(
//            NULL,
//            m_strCommonPrograms,
//            CSIDL_COMMON_PROGRAMS,
//            FALSE);
//
//      xml::document doc(get_app());
//      
//      doc.load(System.file().as_string(appdata("configuration\\directory.xml"),get_app()));
//      
//      xxdebug_box("win_dir::initialize (configuration)", "win_dir::initialize", 0);
//      if(doc.get_root()->get_name() == "directory_configuration")
//      {
//
//         m_strTimeFolder = doc.get_root()->get_child_value("time"); 
//
//         m_strNetSeedFolder = doc.get_root()->get_child_value("netseed"); 
//
//      }
//      if(m_strTimeFolder.is_empty())
//         m_strTimeFolder = appdata("time");
//
//      if(m_strNetSeedFolder.is_empty())
//         m_strNetSeedFolder = element("net/netseed");
//
//      mk(m_strTimeFolder, get_app());
//      xxdebug_box("win_dir::initialize (m_strTimeFolder)", "win_dir::initialize", 0);
//
//      if(!is(m_strTimeFolder, get_app()))
//         return false;
//
//      mk(path(m_strTimeFolder, "time"), get_app());
//      xxdebug_box("win_dir::initialize", "win_dir::initialize", 0);
      return true;

   }

//   string dir::trash_that_is_not_trash(const char * psz)
//   {
//      if(psz == NULL)
//         return "";
//
//      if(psz[1] == ':')
//      {
//         string strDir = name(psz);
//         string str;
//         str = strDir.Left(2);
//         str += "\\trash_that_is_not_trash\\";
//         string strFormat;
//         ::datetime::time time;
//         time = ::datetime::time::get_current_time();
//         strFormat.Format("%04d-%02d-%02d %02d-%02d-%02d\\", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
//         str += strFormat;
//         if(strDir.m_pszData[2] == '\\')
//         {
//            str += strDir.Mid(3);
//         }
//         else
//         {
//            str += strDir.Mid(2);
//         }
//         return str;
//      }
//
//      return "";
//   }
//
//   string dir::appdata(const char * lpcsz, const char * lpcsz2)
//   {
//      string str;
//      str = m_strAppData;
//
//      string strRelative;
//      strRelative = element();
//      index iFind = strRelative.find(':');
//      if(iFind >= 0)
//      {
//         strsize iFind1 = strRelative.reverse_find("\\", iFind);
//         strsize iFind2 = strRelative.reverse_find("/", iFind);
//         strsize iStart = MAX(iFind1 + 1, iFind2 + 1);
//         strRelative = strRelative.Left(iFind - 1) + "_" + strRelative.Mid(iStart, iFind - iStart) + strRelative.Mid(iFind + 1);
//      }
//      return path(path(str, "ca2", strRelative), lpcsz, lpcsz2);
//   }
//
//   
//   string dir::commonappdata(const char * lpcsz,const char * lpcsz2)
//   {
//      
//      string str;
//      
//      str = m_strCommonAppData;
//
//      return element_commonappdata(element(), lpcsz, lpcsz2);
//
//   }
//
//
//   string dir::element_commonappdata(const string & strElement, const char * lpcsz,const char * lpcsz2)
//   {
//
//      string strRelative;
//
//      strRelative = strElement;
//
//      index iFind = strRelative.find(':');
//
//      if(iFind >= 0)
//      {
//
//         strsize iFind1 = strRelative.reverse_find("\\",iFind);
//
//         strsize iFind2 = strRelative.reverse_find("/",iFind);
//
//         strsize iStart = MAX(iFind1 + 1,iFind2 + 1);
//
//         strRelative = strRelative.Left(iFind - 1) + "_" + strRelative.Mid(iStart,iFind - iStart) + strRelative.Mid(iFind + 1);
//
//      }
//
//      return path(path(m_strCommonAppData,"ca2",strRelative),lpcsz,lpcsz2);
//
//   }
//
//
//   string dir::usersystemappdata(::aura::application * papp, const char * lpcszPrefix, const char * lpcsz, const char * lpcsz2)
//   {
//      UNREFERENCED_PARAMETER(papp);
//      return path(appdata(lpcszPrefix), lpcsz, lpcsz2);
//   }
//
//   string dir::userappdata(::aura::application * papp, const char * lpcsz, const char * lpcsz2)
//   {
//      return path(userfolder(papp, "appdata"), lpcsz, lpcsz2);
//   }
//
//   string dir::userdata(::aura::application * papp, const char * lpcsz, const char * lpcsz2)
//   {
//      return path(userfolder(papp, "data"), lpcsz, lpcsz2);
//   }
//
//   string dir::userfolder(::aura::application * papp, const char * lpcsz, const char * lpcsz2)
//   {
//
//      string str;
//      str = m_strProfile;
//
//
//      string strRelative;
//      strRelative = element();
//      index iFind = strRelative.find(':');
//      if(iFind >= 0)
//      {
//         strsize iFind1 = strRelative.reverse_find("\\", iFind);
//         strsize iFind2 = strRelative.reverse_find("/", iFind);
//         strsize iStart = MAX(iFind1 + 1, iFind2 + 1);
//         strRelative = strRelative.Left(iFind - 1) + "_" + strRelative.Mid(iStart, iFind - iStart) + strRelative.Mid(iFind + 1);
//      }
//
//      string strUserFolderShift;
//
//      if(App(papp).directrix()->m_varTopicQuery.has_property("user_folder_relative_path"))
//      {
//         strUserFolderShift = path(strRelative, App(papp).directrix()->m_varTopicQuery["user_folder_relative_path"].get_string());
//      }
//      else
//      {
//         strUserFolderShift = strRelative;
//      }
//
//      return path(path(str, "ca2", strUserFolderShift), lpcsz, lpcsz2);
//
////      return path(path(str, "ca2"), lpcsz);
///*      if(&AppUser(papp) == NULL)
//      {
//         string str;
//         SHGetSpecialFolderPath(
//            NULL,
//            str,
//            CSIDL_PROFILE,
//            FALSE);
//         return path(path(str, "core\\_____default"), lpcsz);
//      }
//      else
//      {
//         return path(AppUser(papp).m_strPath, lpcsz, lpcsz2);
//      }*/
//   }
//
//   string dir::default_os_user_path_prefix(::aura::application * papp)
//   {
//      UNREFERENCED_PARAMETER(papp);
//      unichar buf[MAX_PATH];
//      ULONG ulSize = sizeof(buf) / sizeof(unichar);
//      if(!::GetUserNameExW(NameCanonical, buf, &ulSize))
//      {
//         if(!::GetUserNameW(buf, &ulSize))
//         {
//            memset(buf, 0, sizeof(buf));
//         }
//      }
//      return ::str::international::unicode_to_utf8(buf);
//   }
//
//   string dir::default_userappdata(::aura::application * papp, const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath)
//   {
//      return path(default_userfolder(papp, lpcszPrefix, lpcszLogin, "appdata"), pszRelativePath);
//   }
//
//   string dir::default_userdata(::aura::application * papp, const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath)
//   {
//      return path(default_userfolder(papp, lpcszPrefix, lpcszLogin, "data"), pszRelativePath);
//   }
//
//   string dir::default_userfolder(::aura::application * papp, const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath)
//   {
//
//      return userfolder(papp, pszRelativePath);
//
///*      UNREFERENCED_PARAMETER(papp);
//      string str;
//      SHGetSpecialFolderPath(
//         NULL,
//         str,
//         CSIDL_APPDATA,
//         FALSE);
//      return path(path(str, "core\\user", lpcszPrefix), lpcszLogin, pszRelativePath);*/
//   }
//
//   string dir::userquicklaunch(::aura::application * papp, const char * lpcszRelativePath, const char * lpcsz2)
//   {
//      UNREFERENCED_PARAMETER(papp);
//      string str;
//      str = m_strAppData;
//      str = path(str, "Microsoft\\Internet Explorer\\Quick Launch");
//      return path(str, lpcszRelativePath, lpcsz2);
//   }
//
//   string dir::userprograms(::aura::application * papp, const char * lpcszRelativePath, const char * lpcsz2)
//   {
//      UNREFERENCED_PARAMETER(papp);
//      string str;
//      str = m_strPrograms;
//      return path(str, lpcszRelativePath, lpcsz2);
//   }
//
//   string dir::commonprograms(const char * lpcszRelativePath, const char * lpcsz2)
//   {
//      string str;
//      str = m_strCommonPrograms;
//      return path(str, lpcszRelativePath, lpcsz2);
//   }

   //bool dir::is_inside_time(const char * pszPath, ::aura::application * papp)
   //{
   //   return is_inside(time(), pszPath, papp);
   //}


   //bool dir::is_inside(const char * pszDir, const char * pszPath, ::aura::application * papp)
   //{
   //   
   //   return ::str::begins_ci(pszDir, pszPath);

   //}

   
   //bool dir::has_subdir(::aura::application * papp, const char * pszDir)
   //{
   //   
   //   return ::windows::dir::has_subdir(papp,pszDir);

   //}

   } // namespace axis

} // namespace windows
























































