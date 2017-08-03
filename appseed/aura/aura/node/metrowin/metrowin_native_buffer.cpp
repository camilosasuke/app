//#include "framework.h"
//#include "metrowin.h"


#pragma once


CLASS_DECL_AURA ::Windows::Storage::StorageFolder ^ winrt_folder(string & strPath, string & strPrefix)
{

   if (str::begins_eat_ci(strPath, "winmetro-Pictures://"))
   {

      strPrefix = "winmetro-Pictures://";

      return ::Windows::Storage::KnownFolders::PicturesLibrary;

   }
   else if (str::begins_eat_ci(strPath, "winmetro-Music://"))
   {

      strPrefix = "winmetro-Music://";

      return ::Windows::Storage::KnownFolders::MusicLibrary;

   }
   else if (str::begins_eat_ci(strPath, "winmetro-Videos://"))
   {

      strPrefix = "winmetro-Videos://";

      return ::Windows::Storage::KnownFolders::VideosLibrary;

   }
   else if (str::begins_eat_ci(strPath, "winmetro-Document://"))
   {

      strPrefix = "winmetro-Document://";

      return ::Windows::Storage::KnownFolders::DocumentsLibrary;

   }
   else
   {

      return nullptr;

   }

}


CLASS_DECL_AURA ::Windows::Storage::StorageFolder ^ winrt_get_folder(const string & strFolder, string & strPrefix)
{

   string strPath = strFolder;

   ::Windows::Storage::StorageFolder ^ folder = winrt_folder(strPath, strPrefix);

   if (folder == nullptr)
   {

      return nullptr;

   }

   return wait(folder->GetFolderAsync(strPath));

}


CLASS_DECL_AURA::Windows::Storage::StorageFolder ^ winrt_get_folder(const string & strFolder)
{

   string strPrefix;

   return winrt_get_folder(strFolder, strPrefix);

}



namespace metrowin
{


   //namespace WinFileException
   //{

   //   ::file::exception::e_cause OsErrorToException(LONG lOsError);
   //   ::file::exception::e_cause ErrnoToException(int nErrno);
   //   void ThrowOsError(::aura::application * papp,LONG lOsError,const char * lpszfileName = NULL);
   //   void ThrowErrno(::aura::application * papp,int nErrno,const char * lpszfileName = NULL);

   //}




   //__STATIC inline bool IsDirSep(WCHAR ch)
   //{
   //   return (ch == '\\' || ch == '/');
   //}


   native_buffer::native_buffer(::aura::application * papp):
      ::object(papp)
   {

      m_bCloseOnDelete = TRUE;

   }

   native_buffer::native_buffer(::aura::application * papp,StorageFile ^ file):
      ::object(papp)
   {

      m_file = file;

      m_bCloseOnDelete = TRUE;

   }

   native_buffer::native_buffer(::aura::application * papp,const char * lpszfileName,UINT nOpenFlags):
      ::object(papp)
   {

      ASSERT(__is_valid_string(lpszfileName));

      if(!open(lpszfileName,nOpenFlags))
         throw ::file::exception(papp,::file::exception::none,-1,lpszfileName);

   }

   native_buffer::~native_buffer()
   {

      m_file = nullptr;

   }

   sp(::file::file) native_buffer::Duplicate() const
   {
      //ASSERT_VALID(this);
      //ASSERT(m_hnative_buffer != (UINT)hnative_bufferNull);

      //sp(native_buffer) pnative_buffer = canew(native_buffer(get_app(),hnative_bufferNull));
      //HANDLE hnative_buffer;
      //if(!::DuplicateHandle(::GetCurrentProcess(),(HANDLE)m_hnative_buffer,
      //   ::GetCurrentProcess(),&hnative_buffer,0,FALSE,DUPLICATE_SAME_ACCESS))
      //{
      //   delete pnative_buffer;
      //   //xxx      Ex1WinFileException::ThrowOsError(get_app(), (LONG)::GetLastError());
      //   throw 0;
      //}
      //pnative_buffer->m_hnative_buffer = (UINT)hnative_buffer;
      //ASSERT(pnative_buffer->m_hnative_buffer != (UINT)hnative_bufferNull);
      //pnative_buffer->m_bCloseOnDelete = m_bCloseOnDelete;
      //return pnative_buffer;
      return NULL;
   }


   ::cres native_buffer::open(const ::file::path & path,UINT nOpenFlags)
   {

      m_file = nullptr;
      m_folder = nullptr;

      ASSERT_VALID(this);
      ASSERT(__is_valid_string(path));
      ASSERT((nOpenFlags & ::file::type_text) == 0);   // text mode not supported

      // native_buffer objects are always binary and Createnative_buffer does not need flag
      nOpenFlags &= ~(UINT)::file::type_binary;


      string strPath = path;

      string strPrefix;

      ::Windows::Storage::StorageFolder ^ folder = winrt_folder(strPath, strPrefix);

      if(folder == nullptr)
      {

         return failure;

      }

      ::file::path pathFolder = strPath;

      string strName = pathFolder.name();

      pathFolder -= 1;

      string strRelative = pathFolder;

      ::str::begins_eat_ci(strRelative, strPrefix);

      if (strRelative.is_empty())
      {

         m_folder = folder;

      }
      else
      {

         m_folder = wait(folder->GetFolderAsync(strRelative));

      }

      if (m_folder == nullptr)
      {

         return failure;

      }

      ::file::path lpszfileName(strPath);
      if(nOpenFlags & ::file::defer_create_directory)
      {

         Application.dir().mk(lpszfileName.folder());

      }

      m_bCloseOnDelete = FALSE;
      //m_hnative_buffer = (UINT)hnative_bufferNull;
      m_strFileName.Empty();

      m_strFileName     = lpszfileName;
      //      m_wstrnative_bufferName    = ::str::international::utf8_to_unicode(m_strFileName);

      ASSERT(sizeof(HANDLE) == sizeof(uint_ptr));
      ASSERT(::file::share_compat == 0);

      // ::map read/write mode
      ASSERT((::file::mode_read | ::file::mode_write | ::file::mode_read_write) == 3);
      DWORD dwAccess = 0;
      switch(nOpenFlags & 3)
      {
      case ::file::mode_read:
         dwAccess = GENERIC_READ;
         break;
      case ::file::mode_write:
         dwAccess = GENERIC_WRITE;
         break;
      case ::file::mode_read_write:
         dwAccess = GENERIC_READ | GENERIC_WRITE;
         break;
      default:
         dwAccess = GENERIC_READ;
         break;
      }

      // ::map share mode
      //DWORD dwShareMode = 0;
      //switch(nOpenFlags & 0x70)    // ::map compatibility mode to exclusive
      //{
      //default:
      //   ASSERT(FALSE);  // invalid share mode?
      //case ::file::share_compat:
      //case ::file::share_exclusive:
      //   dwShareMode = 0;
      //   break;
      //case ::file::share_deny_write:
      //   dwShareMode = FILEative_buffer_SHARE_READ;
      //   break;
      //case ::file::share_deny_read:
      //   dwShareMode = native_buffer_SHARE_WRITE;
      //   break;
      //case ::file::share_deny_none:
      //   dwShareMode = native_buffer_SHARE_WRITE | native_buffer_SHARE_READ;
      //   break;
      //}

      // Note: type_text and type_binary are used in derived classes only.

      // ::map mode_no_inherit flag
      SECURITY_ATTRIBUTES sa;
      sa.nLength = sizeof(sa);
      sa.lpSecurityDescriptor = NULL;
      sa.bInheritHandle = (nOpenFlags & ::file::mode_no_inherit) == 0;

      // ::map creation flags
      DWORD dwCreateFlag;
      if(nOpenFlags & ::file::mode_create)
      {
         if(nOpenFlags & ::file::mode_no_truncate)
            dwCreateFlag = OPEN_ALWAYS;
         else
            dwCreateFlag = CREATE_ALWAYS;
      }
      else
         dwCreateFlag = OPEN_EXISTING;

      // attempt native_buffer creation
      //HANDLE hnative_buffer = shell::Createnative_buffer(::str::international::utf8_to_unicode(m_strFileName), dwAccess, dwShareMode, &sa, dwCreateFlag, native_buffer_ATTRIBUTE_NORMAL, NULL);

      folder = m_folder;

      m_file = ::wait(folder->GetFileAsync(strName));


      if(m_file == nullptr)
      {
         m_folder = nullptr;
         return failure;
      }

      //m_file = ::wait(Storage::GetFileFromPathAsync(m_strFileName));
      //if(m_file == nullptr)
      //{
      //   m_folder = nullptr;
      //   return failure;
      //}

      m_stream = ::wait(m_file->OpenAsync(::Windows::Storage::FileAccessMode::Read));
      if(m_stream == nullptr)
      {
         m_file = nullptr;
         m_folder = nullptr;
         return failure;
      }


      m_bCloseOnDelete = TRUE;

      return no_exception;

   }


   memory_size_t native_buffer::read(void *  lpBuf,memory_size_t nCount)
   {

      ::Windows::Storage::Streams::IBuffer ^ buffer = ref new ::Windows::Storage::Streams::Buffer(nCount);

      ::Windows::Storage::Streams::IBuffer ^ buffer2 = ::wait(m_stream->ReadAsync(buffer,nCount,::Windows::Storage::Streams::InputStreamOptions::None));

      memory memory(get_app());

      memory.set_os_buffer(buffer2);

      memcpy(lpBuf,memory.get_data(), memory.get_size());

      return memory.get_size();

   }

   void native_buffer::write(const void * lpBuf,memory_size_t nCount)
   {
      //ASSERT_VALID(this);
      //ASSERT(m_hnative_buffer != (UINT)hnative_bufferNull);

      //if(nCount == 0)
      //   return;     // avoid Win32 "NULL-write" option

      //ASSERT(lpBuf != NULL);
      //ASSERT(__is_valid_address(lpBuf,nCount,FALSE));

      //DWORD nWritten;
      //if(!::Writenative_buffer((HANDLE)m_hnative_buffer,lpBuf,(DWORD)nCount,&nWritten,NULL))
      //   WinFileException::ThrowOsError(get_app(),(LONG)::GetLastError(),m_strFileName);

      //// Win32s will not return an error all the time (usually DISK_FULL)
      //if(nWritten != nCount)
      //   ::file::throw_exception(get_app(),::file::exception::diskFull,-1,m_strFileName);
   }

   file_position_t native_buffer::seek(file_offset_t lOff,::file::e_seek nFrom)
   {
      if(nFrom == ::file::seek_begin)
      {
         m_stream->Seek(lOff);
      }
      else if(nFrom == ::file::seek_current)
      {
         m_stream->Seek(m_stream->Position+lOff);
      }
      else
      {
         m_stream->Seek(m_stream->Size + lOff);
      }

      return m_stream->Position;
   }

   file_position_t native_buffer::get_position() const
   {

      return m_stream->Position;
   }

   void native_buffer::Flush()
   {
      //m_stream->Flush();
   }

   void native_buffer::close()
   {
      //m_file->Close();
      m_stream = nullptr;
      m_file = nullptr;
      m_folder = nullptr;
   }

   void native_buffer::Abort()
   {
      //ASSERT_VALID(this);
      //if(m_hnative_buffer != (UINT)hnative_bufferNull)
      //{
      //   // close but ignore errors
      //   ::CloseHandle((HANDLE)m_hnative_buffer);
      //   m_hnative_buffer = (UINT)hnative_bufferNull;
      //}
      //m_strFileName.Empty();
   }

   void native_buffer::LockRange(file_position_t dwPos,file_size_t dwCount)
   {
      //ASSERT_VALID(this);
      //ASSERT(m_hnative_buffer != (UINT)hnative_bufferNull);

      ////      if (!::Locknative_buffer((HANDLE)m_hnative_buffer, LODWORD(dwPos), HIDWORD(dwPos), LODWORD(dwCount), HIDWORD(dwCount)))
      ////       WinFileException::ThrowOsError(get_app(), (LONG)::GetLastError());
   }

   void native_buffer::UnlockRange(file_position_t dwPos,file_size_t dwCount)
   {
      //ASSERT_VALID(this);
      //ASSERT(m_hnative_buffer != (UINT)hnative_bufferNull);

      ////  if (!::Unlocknative_buffer((HANDLE)m_hnative_buffer,  LODWORD(dwPos), HIDWORD(dwPos), LODWORD(dwCount), HIDWORD(dwCount)))
      ////   WinFileException::ThrowOsError(get_app(), (LONG)::GetLastError());
   }

   void native_buffer::set_length(file_size_t dwNewLen)
   {
      //ASSERT_VALID(this);
      //ASSERT(m_hnative_buffer != (UINT)hnative_bufferNull);

      //seek((LONG)dwNewLen,(::file::e_seek)::file::seek_begin);

      //if(!::SetEndOfnative_buffer((HANDLE)m_hnative_buffer))
      //   WinFileException::ThrowOsError(get_app(),(LONG)::GetLastError());
   }

   file_size_t native_buffer::get_length() const
   {
      ASSERT_VALID(this);

      file_position_t dwLen,dwCur;

      // seek is a non const operation
      native_buffer* pnative_buffer = (native_buffer*)this;
      dwCur = pnative_buffer->seek(0L,::file::seek_current);
      dwLen = pnative_buffer->seek_to_end();
      VERIFY(dwCur == (uint64_t)pnative_buffer->seek((file_offset_t)dwCur,::file::seek_begin));

      return (file_size_t)dwLen;
   }

   // native_buffer does not support direct buffering (CMemnative_buffer does)
   uint64_t native_buffer::GetBufferPtr(UINT nCommand,uint64_t /*nCount*/,
                                        void ** /*ppBufStart*/,void ** /*ppBufMax*/)
   {
      ASSERT(nCommand == bufferCheck);
      UNUSED(nCommand);    // not used in retail build

      return 0;   // no support
   }

   /*
   void native_buffer::Rename(const char * lpszOldName, const char * lpszNewName)
   {
   if (!::Movenative_buffer((LPTSTR)lpszOldName, (LPTSTR)lpszNewName))
   WinFileException::ThrowOsError(get_app(), (LONG)::GetLastError());
   }

   void native_buffer::remove(const char * lpszfileName)
   {
   if (!::Deletenative_buffer((LPTSTR)lpszfileName))
   WinFileException::ThrowOsError(get_app(), (LONG)::GetLastError());
   }
   */



   void native_buffer::assert_valid() const
   {
      ::object::assert_valid();
      // we permit the descriptor m_hnative_buffer to be any value for derived classes
   }

   void native_buffer::dump(dump_context & dumpcontext) const
   {
      ::object::dump(dumpcontext);

//      dumpcontext << "with handle " << (UINT)m_hnative_buffer;
      //    dumpcontext << " and name \"" << m_strFileName << "\"";
      //  dumpcontext << "\n";
   }




   string native_buffer::GetFileName() const
   {

      ASSERT_VALID(this);

      ::file::file_status status;

      GetStatus(status);

      string wstrResult;

      wstrResult = status.m_strFullName.name();

      return wstrResult;

   }


   string native_buffer::GetFileTitle() const
   {

      ASSERT_VALID(this);

      ::file::file_status status;

      GetStatus(status);

      string wstrResult;

      wstrResult = status.m_strFullName.title();

      return wstrResult;

   }


   string native_buffer::GetFilePath() const
   {
      ASSERT_VALID(this);

      ::file::file_status status;
      GetStatus(status);
      return status.m_strFullName;
   }




   bool native_buffer::GetStatus(::file::file_status& rStatus) const
   {
      ASSERT_VALID(this);

      //memset(&rStatus, 0, sizeof(::file::file_status));

      // copy File name from cached m_strFileName
      rStatus.m_strFullName = m_strFileName;

#ifdef WINDOWSEX
      if(m_hnative_buffer != hnative_bufferNull)
      {
         // get time ::file::seek_current native_buffer size
         native_bufferTIME ftCreate,ftAccess,ftModify;
         if(!::Getnative_bufferTime((HANDLE)m_hnative_buffer,&ftCreate,&ftAccess,&ftModify))
            return FALSE;

         if((rStatus.m_size = ::Getnative_bufferSize((HANDLE)m_hnative_buffer,NULL)) == (DWORD)-1L)
            return FALSE;

         if(m_strFileName.is_empty())
            rStatus.m_attribute = 0;
         else
         {
            DWORD dwAttribute = ::Getnative_bufferAttributesW(::str::international::utf8_to_unicode(m_strFileName));

            // don't return an error for this because previous versions of ca2 API didn't
            if(dwAttribute == 0xFFFFFFFF)
               rStatus.m_attribute = 0;
            else
            {
               rStatus.m_attribute = (BYTE)dwAttribute;
#ifdef DEBUG
               // ca2 API BUG: m_attribute is only a BYTE wide
               if(dwAttribute & ~0xFF)
                  TRACE0("Warning: native_buffer::GetStatus() returns m_attribute without high-order flags.\n");
#endif
            }
         }

         // convert times as appropriate
         rStatus.m_ctime = ::datetime::time(ftCreate);
         rStatus.m_atime = ::datetime::time(ftAccess);
         rStatus.m_mtime = ::datetime::time(ftModify);

         if(rStatus.m_ctime.get_time() == 0)
            rStatus.m_ctime = rStatus.m_mtime;

         if(rStatus.m_atime.get_time() == 0)
            rStatus.m_atime = rStatus.m_mtime;
      }
#endif
      return TRUE;
   }


   bool native_buffer::GetStatus(const ::file::path & lpszfileName,::file::file_status& rStatus)
   {

#ifdef WINDOWSEX
      // attempt to fully qualify path first
      wstring wstrFullName;
      wstring wstrnative_bufferName;
      wstrnative_bufferName = ::str::international::utf8_to_unicode(lpszfileName);
      if(!vfxFullPath(wstrFullName,wstrnative_bufferName))
      {
         rStatus.m_strFullName.Empty();
         return FALSE;
      }
      ::str::international::unicode_to_utf8(rStatus.m_strFullName,wstrFullName);

      WIN32_FIND_DATA findnative_bufferData;
      HANDLE hFind = FindFirstnative_buffer((LPTSTR)lpszfileName,&findnative_bufferData);
      if(hFind == INVALID_HANDLE_VALUE)
         return FALSE;
      VERIFY(FindClose(hFind));

      // strip attribute of NORMAL bit, our API doesn't have a "normal" bit.
      rStatus.m_attribute = (BYTE)(findnative_bufferData.dwnative_bufferAttributes & ~native_buffer_ATTRIBUTE_NORMAL);

      // get just the low DWORD of the native_buffer size
      ASSERT(findnative_bufferData.nnative_bufferSizeHigh == 0);
      rStatus.m_size = (LONG)findnative_bufferData.nnative_bufferSizeLow;

      // convert times as appropriate
      rStatus.m_ctime = ::datetime::time(findnative_bufferData.ftCreationTime);
      rStatus.m_atime = ::datetime::time(findnative_bufferData.ftLastAccessTime);
      rStatus.m_mtime = ::datetime::time(findnative_bufferData.ftLastWriteTime);

      if(rStatus.m_ctime.get_time() == 0)
         rStatus.m_ctime = rStatus.m_mtime;

      if(rStatus.m_atime.get_time() == 0)
         rStatus.m_atime = rStatus.m_mtime;
#endif
      return TRUE;
   }


   /*
   UINT CLASS_DECL_AURA vfxGetnative_bufferTitle(const unichar * lpszPathName, unichar * lpszTitle, UINT nMax)
   {
   ASSERT(lpszTitle == NULL ||
   __is_valid_address(lpszTitle, _MAX_FNAME));
   ASSERT(__is_valid_string(lpszPathName));

   // use a temporary to avoid bugs in ::Getnative_bufferTitle when lpszTitle is NULL
   WCHAR szTemp[_MAX_PATH];
   unichar * lpszTemp = lpszTitle;
   if (lpszTemp == NULL)
   {
   lpszTemp = szTemp;
   nMax = _countof(szTemp);
   }
   if (::Getnative_bufferTitleW(lpszPathName, lpszTemp, (WORD)nMax) != 0)
   {
   // when ::Getnative_bufferTitle fails, use cheap imitation
   return vfxGetnative_bufferName(lpszPathName, lpszTitle, nMax);
   }
   return lpszTitle == NULL ? lstrlenW(lpszTemp)+1 : 0;
   }


   bool vfxComparePath(const unichar * lpszPath1, const unichar * lpszPath2)
   {
   // use case insensitive compare as a starter
   if (lstrcmpiW(lpszPath1, lpszPath2) != 0)
   return FALSE;

   // on non-DBCS systems, we are done
   if (!GetSystemMetrics(SM_DBCSENABLED))
   return TRUE;

   // on DBCS systems, the native_buffer name may not actually be the same
   // in particular, the native_buffer system is case sensitive with respect to
   // "full width" roman characters.
   // (ie. fullwidth-R is different from fullwidth-r).
   int nLen = lstrlenW(lpszPath1);
   if (nLen != lstrlenW(lpszPath2))
   return FALSE;
   ASSERT(nLen < _MAX_PATH);

   // need to get both CT_CTYPE1 and CT_CTYPE3 for each native_buffername
   LCID lcid = GetThreadLocale();
   WORD aCharType11[_MAX_PATH];
   VERIFY(GetStringTypeExW(lcid, CT_CTYPE1, lpszPath1, -1, aCharType11));
   WORD aCharType13[_MAX_PATH];
   VERIFY(GetStringTypeExW(lcid, CT_CTYPE3, lpszPath1, -1, aCharType13));
   WORD aCharType21[_MAX_PATH];
   VERIFY(GetStringTypeExW(lcid, CT_CTYPE1, lpszPath2, -1, aCharType21));
   #ifdef DEBUG
   WORD aCharType23[_MAX_PATH];
   VERIFY(GetStringTypeExW(lcid, CT_CTYPE3, lpszPath2, -1, aCharType23));
   #endif

   // for every C3_FULLWIDTH character, make sure it has same C1 value
   int i = 0;
   for (const unichar * lpsz = lpszPath1; *lpsz != 0; lpsz = _wcsinc(lpsz))
   {
   // check for C3_FULLWIDTH characters only
   if (aCharType13[i] & C3_FULLWIDTH)
   {
   #ifdef DEBUG
   ASSERT(aCharType23[i] & C3_FULLWIDTH); // should always match!
   #endif

   // if CT_CTYPE1 is different then native_buffer system considers these
   // native_buffer names different.
   if (aCharType11[i] != aCharType21[i])
   return FALSE;
   }
   ++i; // look at next character type
   }
   return TRUE; // otherwise native_buffer name is truly the same
   }
   */

   /*
   void native_buffer::SetStatus(const char * lpszfileName, const ::file::file_status& status)
   {
   DWORD wAttr;
   native_bufferTIME creationTime;
   native_bufferTIME lastAccessTime;
   native_bufferTIME lastWriteTime;
   LPnative_bufferTIME lpCreationTime = NULL;
   LPnative_bufferTIME lpLastAccessTime = NULL;
   LPnative_bufferTIME lpLastWriteTime = NULL;

   if ((wAttr = Getnative_bufferAttributes((LPTSTR)lpszfileName)) == (DWORD)-1L)
   WinFileException::ThrowOsError(get_app(), (LONG)GetLastError());

   if ((DWORD)status.m_attribute != wAttr && (wAttr & readOnly))
   {
   // Set native_buffer attribute, only if currently readonly.
   // This way we will be able to modify the time assuming the
   // caller changed the native_buffer from readonly.

   if (!Setnative_bufferAttributes((LPTSTR)lpszfileName, (DWORD)status.m_attribute))
   WinFileException::ThrowOsError(get_app(), (LONG)GetLastError());
   }

   // last modification time
   if (status.m_mtime.get_time() != 0)
   {
   ::ca2::TimeTonative_bufferTime(status.m_mtime, &lastWriteTime);
   lpLastWriteTime = &lastWriteTime;

   // last access time
   if (status.m_atime.get_time() != 0)
   {
   ::ca2::TimeTonative_bufferTime(status.m_atime, &lastAccessTime);
   lpLastAccessTime = &lastAccessTime;
   }

   // create time
   if (status.m_ctime.get_time() != 0)
   {
   ::ca2::TimeTonative_bufferTime(status.m_ctime, &creationTime);
   lpCreationTime = &creationTime;
   }

   HANDLE hnative_buffer = ::Createnative_buffer(lpszfileName, GENERIC_READ|GENERIC_WRITE,
   native_buffer_SHARE_READ, NULL, OPEN_EXISTING, native_buffer_ATTRIBUTE_NORMAL,
   NULL);

   if (hnative_buffer == INVALID_HANDLE_VALUE)
   WinFileException::ThrowOsError(get_app(), (LONG)::GetLastError());

   if (!Setnative_bufferTime((HANDLE)hnative_buffer, lpCreationTime, lpLastAccessTime, lpLastWriteTime))
   WinFileException::ThrowOsError(get_app(), (LONG)::GetLastError());

   if (!::CloseHandle(hnative_buffer))
   WinFileException::ThrowOsError(get_app(), (LONG)::GetLastError());
   }

   if ((DWORD)status.m_attribute != wAttr && !(wAttr & readOnly))
   {
   if (!Setnative_bufferAttributes((LPTSTR)lpszfileName, (DWORD)status.m_attribute))
   WinFileException::ThrowOsError(get_app(), (LONG)GetLastError());
   }
   }
   */

   bool native_buffer::IsOpened()
   {
      return m_file != nullptr;
   }


   void native_buffer::SetFilePath(const char * lpszNewName)
   {
      ASSERT_VALID(this);
      ASSERT(__is_valid_string(lpszNewName));
      m_strFileName = lpszNewName;
   }



   //// ::file::file_sp
   //native_buffer::operator HFILE() const
   //{
   //   //return m_hnative_buffer;
   //   return NULL;

   //}

   uint64_t native_buffer::ReadHuge(void * lpBuffer,uint64_t dwCount)
   {
      return (uint64_t)read(lpBuffer,(UINT)dwCount);
   }
   void native_buffer::WriteHuge(const void * lpBuffer,uint64_t dwCount)
   {
      write(lpBuffer,(UINT)dwCount);
   }




} // namespace metrowin






