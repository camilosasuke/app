#include "framework.h"
#include <stdio.h>


#include "zlib.h"
#include "zutil.h"
#include "aura/aura/compress/zip/zip.h"

#undef local

//#ifndef METROWIN
//#include <openssl/ssl.h>
//#endif
//
//#include <openssl/md5.h>
//#include <openssl/whrlpool.h>
//
#ifndef WINDOWS
#include <sys/stat.h>
//#include <ctype.h>
#endif

//CLASS_DECL_AURA void NESSIEinit(struct NESSIEstruct * const structpointer);
//CLASS_DECL_AURA void NESSIEadd(const uchar * const source, uint_ptr sourceBits, struct NESSIEstruct * const structpointer);
//CLASS_DECL_AURA void NESSIEfinalize(struct NESSIEstruct * const structpointer, uchar * const result);




//#ifndef METROWIN
//#include <openssl/ssl.h>
//#endif


#include "aura/aura/crypto/crypto_openssl.h"


#ifndef WINDOWS
#include <sys/stat.h>
//#include <ctype.h>
#endif





namespace file
{


   system::system(::aura::application * papp) :
      object(papp)
   {
      //m_path.m_pfile = this;

   }

   system::~system()
   {

   }

   

   bool system::exists(const ::file::path & pszPath, ::aura::application * papp)
   {

      return exists(pszPath, NULL, papp);

   }


   bool system::exists(const ::file::path & strPath,var * pvarQuery,::aura::application * papp)
   {



      if (::str::begins(strPath, astr.strHttpProtocol) || ::str::begins(strPath, astr.strHttpsProtocol))
      {

         property_set set(papp);

         /*         if(papp->m_paxissession != NULL)
         {

         set["user"] = papp->m_paxissession->safe_get_user();

         }*/

         return App(papp).http().exists(strPath, pvarQuery, set);

      }


      if (::get_thread() != NULL && ::get_thread()->m_bZipIsDir)
      {

         strsize iFind = ::str::find_file_extension("zip:", strPath);

         ::zip::util ziputil;

         if (iFind >= 0)
         {

            if (!exists(strPath.Mid(0, iFind + 4), papp))
               return false;

            return ziputil.exists(papp, strPath);

         }


      }




      //if (!papp->m_paurasystem->dir().name_is(strPath, papp))
      //   return false;

#ifdef WINDOWS


      return file_exists_dup(strPath) != FALSE;

#else

      struct stat st;

      if (stat(strPath, &st) != 0)
         return false;

      return S_ISREG(st.st_mode) || S_ISDIR(st.st_mode);

#endif

   }

   var system::length(const ::file::path & pszPath,::aura::application * papp)
   {

      return length(pszPath, NULL, papp);

   }


   var system::length(const ::file::path & strPath,var * pvarQuery,::aura::application * papp)
   {

      var varRet;

#ifdef WINDOWS

      WIN32_FILE_ATTRIBUTE_DATA data;

      if(!GetFileAttributesExW(::str::international::utf8_to_unicode(strPath), GetFileExInfoStandard, &data))
      {

         varRet.set_type(var::type_null);

      }
      else
      {

         varRet = (uint32_t) data.nFileSizeLow;

      }

#else

      struct stat stat;

      if(::stat(strPath, &stat)  == -1)
      {

         varRet.set_type(var::type_null);

      }
      else
      {

         varRet = stat.st_size;

      }

#endif

      return varRet;

   }


   ::file::path system::module()
   {

      return m_pathModule;

   }


   ::file::path system::ca2module()
   {

      return m_pathCa2Module;

   }


   ::file::path system::time_square(::aura::application * papp,const string & pszPrefix,const string & pszSuffix)
   {

      return time(papp, System.dir().time_square(papp), 25, pszPrefix, pszSuffix);

   }


   ::file::path system::time_log(::aura::application * papp,const string & pszId)
   {
      return time(papp, System.dir().time_log(pszId), 9);
   }


   ::file::path system::time(::aura::application * papp,const ::file::path & psz,int32_t iMaxLevel,const string & pszPrefix,const string & pszSuffix)
   {

      synch_lock lockMachineEvent(!is_null(&System.machine_event_central()) ? &System.machine_event_central().m_machineevent.m_mutex : ((mutex *) NULL));

      int32_t iIncLevel = -1;

      ::file::path str;

      string strPrefix(pszPrefix);

      string strSuffix(pszSuffix);

restart:

      str.Empty();

      str = psz;

      System.dir().mk(str, papp);

      listing ls(papp);

      string strFormat;

      for(int32_t i = 1; i <= iMaxLevel;)
      {

         System.dir().mk(str, papp);

         if (!System.dir().is(str, papp))
         {

            throw "time square dir does not exist";

         }

         ls.ls(str);

         if(i < iMaxLevel)
         {

            int32_t iMax = filterex_time_square("", ls);

            if(iMax == -1)
            {

               str /= "00";

               System.dir().mk(str, papp);

            }
            else if(iMax == 99)
            {

               iIncLevel = i - 1;

               goto restart;

            }
            else
            {

               if(i == iIncLevel)
               {

                  iMax++;

               }

               strFormat.Format("%02d", iMax);

               str /= strFormat;

               if(i == iIncLevel)
               {

                  System.dir().mk(str, papp);

               }

            }

            i++;

         }
         else // if i == iMaxLevel
         {

            ls.ls(str);

            int32_t iMax = filterex_time_square(pszPrefix, ls);

            if(iMax == -1)
            {

               str = str / (strPrefix+"00"+strSuffix);

               if (system::mk_time(str))
               {

                  break;

               }

            }
            else if(iMax == 99)
            {
               
               iIncLevel = iMaxLevel - 1;

               goto restart;

            }
            else
            {

               iMax++;

               strFormat.Format("%02d", iMax);

               str = str / (strPrefix+strFormat+strSuffix);

               if (system::mk_time(str))
               {

                  break;

               }

            }

            return "";

         }

      }

      return str;

   }


   int32_t system::filterex_time_square(const char * pszPrefix, ::file::patha & stra)
   {

      int32_t iMax = -1;

      int32_t iIndex;

      for(int32_t i = 0; i < stra.get_size(); i++)
      {

         string str = stra[i].name();

         if(::str::begins_eat_ci(str, pszPrefix))
         {

            if(str.get_length() < 2)
            {

               stra.remove_at(i);

               i--;

               continue;

            }
            
            if(!isdigit((uchar) str[0]) || !isdigit((uchar) str[1]))
            {

               stra.remove_at(i);

               i--;

               continue;

            }

            iIndex = atoi(str.Mid(0, 2));

            if (iIndex > iMax)
            {

               iMax = iIndex;

            }

         }

      }

      return iMax;

   }


   // fail if exists, create if not exists
   bool system::mk_time(const ::file::path & pathCandidate)
   {

      ::file::file_sp spfile(allocer());

      if (System.file().exists(pathCandidate, get_app()))
      {

         return false;

      }

      try
      {

         if (spfile->open(pathCandidate, ::file::mode_create | ::file::type_binary).failed())
         {

            return false;

         }

      }
      catch(...)
      {

         return false;

      }

      return true;

   }


   var system::as_json(var varFile, ::aura::application * papp)
   {

      string str = as_string(varFile, papp);

      if (str.is_empty())
      {

         return var::type_new;

      }

      const char * pszJson = str;

      var v;

      try
      {

         v.parse_json(pszJson);

      }
      catch (::exception::exception & e)
      {

         TRACE(e.what());

         v = var::type_new;

      }
      catch (...)
      {

         TRACE("GENERAL Exception parsing json system::as_json : \"" + str + "\"");

         v = var::type_new;

      }

      return v;

   }


   string system::as_string(var varFile, ::aura::application * papp)
   {

      var varQuery;

      return as_string(varFile, varQuery, papp);

   }


   string system::as_string(var varFile, var & varQuery, ::aura::application * papp)
   {

      memory storage;

      if (varFile.cast < ::file::file >() != NULL)
      {

         ::file::byte_istream is(varFile.cast < ::file::file >());

         storage.read(is);

      }
      else
      {

         string strFilePath(varFile.get_file_path());

         if (::get_thread() != NULL && ::get_thread()->m_bZipIsDir && (::str::find_file_extension("zip:", strFilePath) >= 0))
         {

            if (!exists(strFilePath, papp))
            {

               return "";

            }

            ::memory_file memfile(papp, &storage);
            
            zip::in_file infile(papp);

            if (!infile.unzip_open(strFilePath, 0))
            {

               return "";

            }

            if (!infile.dump(&memfile))
            {

               return "";

            }

         }
         else if (::str::begins_eat(strFilePath, "file:///"))
         {

            if (!exists(strFilePath, papp))
            {

               return "";

            }

            as_memory(strFilePath, storage, papp);

         }
         else if (::str::begins_eat(strFilePath, "file:\\\\\\"))
         {

            if (!exists(strFilePath, papp))
            {

               return "";

            }

            as_memory(strFilePath, storage, papp);

         }
         else if (::str::begins(strFilePath, astr.strHttpProtocol)
            || ::str::begins(strFilePath, astr.strHttpsProtocol))
         {

            if (!exists(strFilePath, &varQuery, papp))
            {

               return "";

            }

            ::url_domain domain;

            domain.create(System.url().get_server(strFilePath));

            if (domain.m_strRadix == "ca2" && strFilePath.contains("/matter/"))
            {

               try
               {

                  ::file::byte_istream is(App(papp).file().get_file(strFilePath, ::file::type_binary | ::file::mode_read));

                  is >> storage;

               }
               catch (...)
               {

               }

            }
            else
            {

               varQuery["raw_http"] = true;

               //if (!strFilePath.find_ci("sessid=noauth"))
               //{
               //   varQuery["user"] = &AppUser(papp);
               //}

               App(papp).http().get(strFilePath, storage, varQuery.propset());

            }

         }
         else
         {

            as_memory(strFilePath, storage, papp);

         }

      }

      string strResult;

      if (storage.get_size() >= 2
         && storage.get_data()[0] == 255
         && storage.get_data()[1] == 60)
      {

         ::str::international::unicode_to_utf8(strResult, (const unichar *)&storage.get_data()[2], (int32_t)(storage.get_size() - 2));

      }
      else if (storage.get_size() >= 2
         && storage.get_data()[0] == 255
         && storage.get_data()[1] == 254)
      {

#ifdef VSNORD
         //for (index i = 2; i < storage.get_size(); i += 2)
         //{
         //   byte b = storage.get_data()[i];
         //   storage.get_data()[i] = storage.get_data()[i + 1];
         //   storage.get_data()[i + 1] = b;
         //}
#endif
         ::str::international::unicode_to_utf8(strResult, (const unichar *)&storage.get_data()[2], (int32_t)(storage.get_size() - 2));

      }
      else if (storage.get_size() >= 3
         && storage.get_data()[0] == 0xef
         && storage.get_data()[1] == 0xbb
         && storage.get_data()[2] == 0xbf)
      {
         
         strResult = string((const char *)(const unichar *)&storage.get_data()[3], (int32_t)(storage.get_size() - 3));

      }
      else
      {
         
         strResult = string((const char *)storage.get_data(), (int32_t)storage.get_size());

      }

      return strResult;

   }


   void system::as_memory(var varFile, primitive::memory_base & mem, ::aura::application * papp)
   {

      mem.allocate(0);

      if(varFile.get_type() == var::type_string)
      {

         string strPath = varFile.get_string();

         if(strPath.is_empty())
         {

            TRACE("::file::binary_file::system::as_memory varFile is a empty file name!!");

            return;

         }

         strPath.trim("\"'");

         if ((::str::begins(strPath, astr.strHttpProtocol) || ::str::begins(strPath, astr.strHttpsProtocol)))
         {

            property_set set(papp);

            //set["user"] = &AppUser(papp);

            App(papp).http().get(strPath, mem, set);

            return;

         }

#if MEMDLEAK

         mem.m_strTag = "memory://" + strPath;

#endif

      }

      ::file::file_sp spfile;

      try
      {

         cres res;

         spfile = App(papp).file().get_file(varFile, ::file::type_binary | ::file::mode_read | ::file::share_deny_none | ::file::no_call_stack, &res);

         if (res.failed())
         {

            thiswarn << res->element_at(0)->what();

            return;

         }

         if (spfile.is_null())
         {

            return;

         }

         file_size_t filesize = spfile->get_length();

         if (filesize > ::numeric_info < memory_size_t> ::max())
         {

            throw memory_exception(papp);

         }

         mem.allocate((memory_size_t)filesize);

         byte * pdata = mem.get_data();

         memory_size_t memsize = mem.get_size();

         memory_size_t uiRead = (memory_size_t)spfile->read(pdata, memsize);

         mem.allocate(uiRead);

      }
      catch(...)
      {

      }

   }


   void system::lines(stringa & stra, var varFile, ::aura::application * papp)
   {

      UNREFERENCED_PARAMETER(papp);

      ::file::file_sp spfile;

      try
      {

         spfile = App(papp).file().get_file(varFile,::file::type_text | ::file::mode_read);

      }
      catch(...)
      {

         return;

      }

      string strLine;

      while(spfile->read_string(strLine))
      {

         stra.add(strLine);

      }

   }

   bool system::put_contents(var varFile, const void * pvoidContents, ::count count, ::aura::application * papp)
   {

      ::file::file_sp spfile;

      try
      {

         spfile = App(papp).file().get_file(varFile, ::file::type_binary | ::file::mode_write | ::file::mode_create | ::file::share_deny_write | ::file::defer_create_directory);

      }
      catch(...)
      {

         return false;

      }

      if (spfile.is_null())
      {

         return false;

      }

      spfile->write(pvoidContents, count);

      return true;

   }

   bool system::add_contents(var varFile,const void * pvoidContents,::count count,::aura::application * papp)
   {

      ::file::file_sp spfile;

      try
      {

         spfile = App(papp).file().get_file(varFile,::file::type_binary | ::file::mode_write | ::file::mode_create | ::file::mode_no_truncate | ::file::share_exclusive | ::file::defer_create_directory);

      }
      catch(...)
      {

         return false;

      }

      if(spfile.is_null())
         return false;


      spfile->seek_to_end();

      spfile->write(pvoidContents,count);

      return true;

   }


   bool system::put_contents(var varFile, const char * lpcszContents, ::aura::application * papp)
   {

      if(lpcszContents == NULL)
      {

         return put_contents(varFile, "", 0, papp);

      }
      else
      {

         return put_contents(varFile, lpcszContents, strlen(lpcszContents), papp);

      }

   }


   bool system::add_contents(var varFile,const char * lpcszContents,::aura::application * papp)
   {

      if(lpcszContents != NULL)
      {

         return add_contents(varFile,lpcszContents,strlen(lpcszContents),papp);

      }

      return false;

   }


   bool system::put_contents(var varFile, ::file::reader & reader, ::aura::application * papp)
   {

      ::file::file_sp spfile;

      spfile = App(papp).file().get_file(varFile, ::file::type_binary | ::file::mode_write | ::file::mode_create | ::file::share_deny_write | ::file::defer_create_directory);

      if (spfile.is_null())
      {

         return false;

      }

      memory mem;

      mem.allocate(1024 * 1024 * 8);

      memory_size_t uiRead;

      while((uiRead = reader.read(mem.get_data(), mem.get_size())) > 0)
      {

         spfile->write(mem.get_data(), uiRead);

      }

      return true;

   }


   bool system::put_contents(var varFile, memory & mem, ::aura::application * papp)
   {

      return put_contents(varFile, mem.get_data(), (count) mem.get_size(), papp);

   }


   bool system::put_contents_utf8(var varFile, const char * lpcszContents, ::aura::application * papp)
   {

      ::file::file_sp spfile;

      spfile = App(papp).file().get_file(varFile, ::file::type_binary | ::file::mode_write | ::file::mode_create | ::file::share_deny_write | ::file::defer_create_directory);

      if (spfile.is_null())
      {

         return false;

      }

      spfile->write("\xef\xbb\xbf", 3);

      spfile->write(lpcszContents, strlen(lpcszContents));

      return true;

   }




   ::cres system::copy(const ::file::path & pszNew,const ::file::path & psz,bool bFailIfExists,e_extract eextract,::aura::application * papp)
   {

      //if(!papp->m_spdir->is(pszNew.folder()))
      //{

      //   papp->m_spdir->mk(pszNew.folder());

      //}

      //if(!::file_copy_dup(pszNew,psz,!bFailIfExists))
      //{

      //   return ::failure;

      //}

      //return ::no_exception;

      if (System.dir().is(psz, papp) && (eextract == extract_first || eextract == extract_all || !(::str::ends_ci(psz, ".zip"))))
      {
         ::file::listing patha(papp);
         patha.rls(psz);
         ::file::path strDst;
         ::file::path strSrc;
         ::file::path strDirSrc(psz);
         ::file::path strDirDst(pszNew);
         if (App(papp).m_bZipIsDir && (::str::ends(strDirSrc, ".zip")))
         {
            strDirSrc += ":";
         }
         for (int32_t i = 0; i < patha.get_size(); i++)
         {
            strSrc = patha[i];
            strDst = strSrc;
            ::str::begins_eat_ci(strDst, strDirSrc);
            strDst = strDirDst / strDst;
            if (System.dir().is(strSrc, papp))
            {
               if ((eextract == extract_first || eextract == extract_none) && (::str::ends_ci(psz, ".zip")))
               {
               }
               else
               {
                  System.dir().mk(strDst, papp);
               }
            }
            else
            {
               if (!System.dir().is(strDst.folder(), papp))
               {
                  System.dir().mk(strDst.folder(), papp);
               }
               copy(strDst, strSrc, bFailIfExists, eextract == extract_all ? extract_all : extract_none, papp);
            }
         }
      }
      else
      {

         if (pszNew.m_epath == ::file::path_file && psz.m_epath == ::file::path_file)
         {

            if (!papp->m_spdir->is(pszNew.folder()))
            {

               papp->m_spdir->mk(pszNew.folder());

            }

            if (!::file_copy_dup(pszNew, psz, !bFailIfExists))
            {

               return ::failure;

            }

            return ::no_exception;
         }

         if (bFailIfExists)
         {

            if (exists(pszNew, papp))
            {
               return failure;

            }

         }

         ::file::path strNew;

         if (System.dir().is(pszNew, papp))
         {
            strNew = ::file::path(pszNew) / psz.name();
         }
         else
         {
            strNew = pszNew;
         }

         ::file::file_sp ofile;
         ofile = App(papp).file().get_file(strNew, ::file::mode_write | ::file::type_binary | ::file::mode_create | ::file::defer_create_directory | ::file::share_deny_write);
         if (ofile.is_null())
         {
            string strError;
            strError.Format("Failed to copy file \"%s\" to \"%s\" bFailIfExists=%d error=could not open output file", psz, pszNew, bFailIfExists);
            throw strError;
         }

         ::file::file_sp ifile;
         ifile = App(papp).file().get_file(psz, ::file::mode_read | ::file::type_binary | ::file::share_deny_none);
         if (ifile.is_null())
         {
            string strError;
            strError.Format("Failed to copy file \"%s\" to \"%s\" bFailIfExists=%d error=could not open input file", psz, pszNew, bFailIfExists);
            throw strError;
         }

         ::file::ostream ostream(ofile);

         ::file::istream istream(ifile);

         System.file().transfer(ostream, istream);



         bool bOutputFail = false;
         bool bInputFail = false;
         bool bStatusFail = false;
         ::file::file_status st;

         try
         {

            ifile->GetStatus(st);

         }
         catch (...)
         {

            bStatusFail = true;
            TRACE("During copy, failed to get status from input file \"%s\" bFailIfExists=%d", psz, bFailIfExists);
         }

         if (!bStatusFail)
         {
            try
            {

               ifile->GetStatus(st);

            }
            catch (...)
            {

               bStatusFail = true;
               TRACE("During copy, failed to set status to output file \"%s\" bFailIfExists=%d", pszNew, bFailIfExists);

            }

         }

         try
         {
            ofile->flush();
         }
         catch (...)
         {
         }

         try
         {
            ofile->close();
         }
         catch (...)
         {
            bOutputFail = true;
         }

         try
         {
            ifile->close();
         }
         catch (...)
         {
            bInputFail = true;
         }

         if (bInputFail)
         {
            if (bOutputFail)
            {
               string strError;
               strError.Format("During copy, failed to close both input file \"%s\" and output file \"%s\" bFailIfExists=%d", psz, pszNew, bFailIfExists);
               throw strError;
            }
            else
            {
               string strError;
               strError.Format("During copy, failed to close input file \"%s\" bFailIfExists=%d", psz, bFailIfExists);
               throw strError;
            }
         }
         else if (bOutputFail)
         {
            string strError;
            strError.Format("During copy, failed to close output file \"%s\" bFailIfExists=%d", pszNew, bFailIfExists);
            throw strError;
         }

      }

      return no_exception;

   }











   ::cres system::move(const ::file::path & pszNew,const ::file::path & psz,::aura::application * papp)
   {

#ifdef WINDOWSEX

      if (!::MoveFileW(
         ::str::international::utf8_to_unicode(psz),
         ::str::international::utf8_to_unicode(pszNew)))
      {

         uint32_t dwError = ::GetLastError();

         if (dwError == ERROR_ALREADY_EXISTS)
         {

            if (::CopyFileW(
               ::str::international::utf8_to_unicode(psz),
               ::str::international::utf8_to_unicode(pszNew),
               FALSE))
            {

               if (!::DeleteFileW(::str::international::utf8_to_unicode(psz)))
               {

                  dwError = ::GetLastError();

                  string strError;

                  strError.Format("Failed to delete the file to move \"%s\" error=%d", psz, dwError);

                  TRACE0(strError);

               }

               return no_exception;

            }

            dwError = ::GetLastError();

         }

         string strError;

         strError.Format("Failed to move file \"%s\" to \"%s\" error=%d", psz, pszNew, dwError);

         throw strError;

      }

#elif defined(METROWIN)

      ::Windows::Storage::StorageFile ^ file = get_os_file(psz,  0, 0, NULL, OPEN_EXISTING, 0, NULL);

      if (file == nullptr)
      {

         output_debug_string("test");

         throw "file::system::move Could not move file, could not open source file";

      }

      string strDirOld     = psz.folder();
      string strDirNew     = pszNew.folder();
      string strNameOld    = psz.name();
      string strNameNew    = pszNew.name();

      if(strDirOld == strDirNew)
      {
         if(strNameOld == strNameNew)
         {
            return no_exception;
         }
         else
         {
            ::wait(file->RenameAsync(strNameNew));
         }
      }
      else
      {
         ::Windows::Storage::StorageFolder ^ folder = get_os_folder(strDirNew);
         if(strNameOld == strNameNew)
         {
            ::wait(file->MoveAsync(folder));
         }
         else
         {
            ::wait(file->MoveAsync(folder, strNameNew));
         }
      }


#else
      if(::rename(psz, pszNew) != 0)
      {
         int32_t err = errno;
         string strError;
         strError.Format("Failed to delete file error=%d", err);
         throw strError;
      }
#endif

      return ::no_exception;

   }










   ::cres system::del(const ::file::path & psz,::aura::application * papp)
   {


#ifdef WINDOWSEX

      HANDLE h = ::CreateFileW(::str::international::utf8_to_unicode(string("\\\\?\\") + psz), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_DELETE_ON_CLOSE, NULL);

      if (h == INVALID_HANDLE_VALUE)
      {
         uint32_t dwError = ::GetLastError();
         if (dwError == 2) // the file does not exist, so delete "failed"
            return no_exception;
         string strError;
         strError.Format("Failed to delete file \"%s\" error=%d", psz, dwError);
         return failure;
      }
      else
      {
         ::FlushFileBuffers(h);
         ::CloseHandle(h);
      }

      /*      if(!::DeleteFileW(::str::international::utf8_to_unicode(string("\\\\?\\") + psz)))
      {
      uint32_t dwError = ::GetLastError();
      if(dwError == 2) // the file does not exist, so delete "failed"
      return;
      string strError;
      strError.Format("Failed to delete file \"%s\" error=%d", psz, dwError);
      throw io_exception(papp, strError);
      }*/


#else

      if (remove(psz) != 0)
      {
         int32_t err = errno;
         if (err != ENOENT) // already does not exist - consider removal successful - does not issue an exception
         {
            string strError;
            strError.Format("Failed to delete file error=%d", err);
            throw strError;
         }
      }
#endif

      return no_exception;

   }


   ::file::path system::duplicate(const ::file::path & psz,::aura::application * papp)
   {
      string strCopy("copy");
      string strNew;
      if(System.dir().is(psz, papp))
      {
         int32_t i = 1;
         while( i <= 100)
         {
            strNew.Format("%s-%s-%d", psz, strCopy.c_str(), i);
            if(!exists(strNew, papp))
            {
               copy(strNew, psz, false, extract_all, papp);
               return strNew;
            }
            i++;
         }
      }
      else
      {
         string strExt = psz.ext();
         if(!strExt.is_empty())
         {
            strExt = "-" + strExt;
         }
         int32_t i = 1;
         while( i <= 100)
         {
            strNew.Format("%s-%s-%d%s", psz, strCopy.c_str(), i, strExt.c_str());
            if(!exists(strNew, papp))
            {
               copy(strNew, psz, false, extract_all, papp);
               return strNew;
            }
            i++;
         }
      }
      return "";
   }



   ::file::path system::paste(const ::file::path & pathLocation,const ::file::path & path,::aura::application * papp)
   {
      
      ::file::path pathDir = path.folder();
      
      ::file::path pathDst = pathLocation;
      
      ::file::path pathSrc = pathDir;
      
      if (pathDst == pathSrc)
      {
         
         return duplicate(path, papp);

      }
      else
      {
         
         ::file::path pathNew = pathDst / path.name();

         copy(pathNew, path, false, extract_all, papp);

         return pathNew;

      }

   }



   void system::trash_that_is_not_trash(::file::patha & stra, ::aura::application * papp)
   {

      if (stra.get_size() <= 0)
      {

         return;

      }

      ::file::path strDir = System.dir().trash_that_is_not_trash(stra[0]);

      System.dir().mk(strDir, papp);

      for (int32_t i = 0; i < stra.get_size(); i++)
      {

         move(strDir / stra[i].name(), stra[i], papp);

      }

   }




   void system::trash_that_is_not_trash(const ::file::path & psz,::aura::application * papp)
   {

      ::file::path strDir = System.dir().trash_that_is_not_trash(psz);

      System.dir().mk(strDir, papp);

      move(strDir / psz.name(), psz, papp);

   }




   bool system::get_status(const ::file::path & path, ::file::file_status & status, ::aura::application * papp)
   {

      UNREFERENCED_PARAMETER(path);
      UNREFERENCED_PARAMETER(status);
      throw interface_only_exception(papp);

   }


   cres system::set_status(const ::file::path & path, const ::file::file_status & status, ::aura::application * papp)
   {

      UNREFERENCED_PARAMETER(path);
      UNREFERENCED_PARAMETER(status);
      throw interface_only_exception(papp);

   }


   ::cres system::replace(const ::file::path & pszContext,const string & pszFind,const string & pszReplace,::aura::application * papp)
   {

      cres e;

      ::file::listing straTitle(papp);
      straTitle.ls(pszContext);
      string strOld;
      string strNew;
      //string strFail;
      ::file::path pathContext = pszContext;
      for (int32_t i = 0; i < straTitle.get_size(); i++)
      {
         strOld = straTitle[i].name();
         strNew = strOld;
         strNew.replace(pszFind, pszReplace);
         if (strNew != strOld)
         {
            // TODO(camilo) : should there be a way to chain or return multiple exceptions... instead of cres specifile file smart...
            // why not a super smart chained super hand heroe smarter pointerer that can contain vector of any excepction based on
            // ::exception like (::file::exception) (I supposed ::file::exception is already based on ::exception OMG CAMILO!!!)
            // and may be then replace could do replace for example on HTTP servers and return may io_exception and not tighted
            // to a barely translated io exception into a empty ::file::exception with improper filled members....
            e.add(move(pathContext / strNew, pathContext / strOld, papp));

         }

      }
      //if (strFail.has_char())
      //{
      //   App(papp).simple_message_box(NULL, strFail, MB_ICONEXCLAMATION);
      //}

      return e;


   }


   bool system::is_read_only(const ::file::path & psz)
   {

#ifdef WINDOWSEX

      uint32_t dwAttrib = GetFileAttributesW(::str::international::utf8_to_unicode(psz));
      if(dwAttrib & FILE_ATTRIBUTE_READONLY)
         return true;
      return false;

#elif defined(METROWIN)

      throw todo(get_app());

#else

      struct stat st;

      if(stat(psz, &st) != 0)
         return true;

      return !(st.st_mode & S_IWUSR);

#endif

   }


   ::file::path system::sys_temp(const ::file::path & pszName, const char * pszExtension, ::aura::application * papp)
   {


      string strTempDir = ::dir::sys_temp();

      if (!::str::ends(strTempDir, "\\") && !::str::ends(strTempDir, "/"))
      {

         strTempDir += "\\";

      }

      string str;

      char buf[30];

      for (int32_t i = 0; i < 1000; i++)
      {

         sprintf(buf, "%d", i);

         str = strTempDir;
         str += pszName;
         str += "-";
         str += buf;
         str += ".";
         str += pszExtension;
         if (!exists(str, papp))
            return str;
      }

      return "";

   }


   ::file::path system::sys_temp_unique(const ::file::path & pszName)
   {

      return ::dir::sys_temp() / pszName;

   }


   ::file::file_sp system::time_square_file(::aura::application * papp, const string & pszPrefix, const string & pszSuffix)
   {

      return get(time_square(papp, pszPrefix, pszSuffix), papp);

   }


   ::file::file_sp system::get(const ::file::path & name,::aura::application * papp)
   {

      System.dir().mk(name.name(), papp);

      ::file::file_sp fileOut = App(papp).file().get_file(name, ::file::mode_create | ::file::type_binary | ::file::mode_write);

      if(fileOut.is_null())
         throw ::file::exception(papp, ::file::exception::none,-1L, name);

      return fileOut;

   }


   ::file::path system::replace_extension(const ::file::path & pszFile, const char * pszExtension)
   {

      ::file::path strFile(pszFile);

      set_extension(strFile, pszExtension);

      return strFile;

   }


   void system::set_extension(::file::path & strFile, const char * pszExtension)
   {

      strsize iEnd = strFile.reverse_find('.');

      if(iEnd < 0)
      {

         iEnd = strFile.get_length();

      }

      strFile = strFile.Left(iEnd) + ::str::has_char(pszExtension, ".");

   }


   void system::normalize(string & str)
   {
      if (str.is_empty())
      {

         return;

      }

      while(str.has_char() &&
         (str.Right(1) == "\\" ||
         str.Right(1) == "/"))
      {
         str = str.Left(str.get_length() - 1);
      }
   }


   int32_t system::cmp(const ::file::path & psz1, const ::file::path & psz2)
   {
      string str1(psz1);
      normalize(str1);
      string str2(psz2);
      normalize(str2);
      return str1.compare_ci(str2);
   }



   cres system::rename(const ::file::path & pszNew,const ::file::path & psz,::aura::application * papp)
   {

      ::file::path strDir = psz.folder();

      ::file::path strDirNew = pszNew.folder();

      if(strDir != strDirNew)
      {

         // rename should work only on files in the same directory/folder

         return failure;

      }

      if(move(pszNew, psz, papp).failed())
      {

         return failure;

      }

      return no_exception;

   }




   void system::dtf(const ::file::path  & pszFile,const ::file::path & pszDir,::aura::application * papp)
   {
      listing ls(papp);
      ls.rls( pszDir);
      dtf(pszFile, ls, papp);
   }

   void system::dtf(const ::file::path & pszFile,::file::patha & stra,::aura::application * papp)
   {

      ::file::file_sp spfile = App(papp).file().get_file(pszFile, ::file::mode_create | ::file::mode_write | ::file::type_binary);

      if (spfile.is_null())
         throw "failed";

      string strVersion;

      strVersion = "fileset v1";

      MD5_CTX ctx;

      write_gen_string(spfile, NULL, strVersion);

      ::file::file_sp file2(allocer());

      memory_size_t iBufSize = 1024 * 1024;

      memory_size_t uiRead;

      memory buf;

      buf.allocate(iBufSize);

      string strMd5 = "01234567012345670123456701234567";

      uint64_t iPos;

      for (int32_t i = 0; i < stra.get_size(); i++)
      {
         if (::str::ends_ci(stra[i], ".zip"))
         {
         }
         else if (System.dir().is(stra[i], papp))
            continue;
         write_n_number(spfile, NULL, 1);
         iPos = spfile->get_position();
         write_gen_string(spfile, NULL, strMd5);
         MD5_Init(&ctx);
         string strRelative = stra[i].relative();
         write_gen_string(spfile, &ctx, strRelative);
         if (file2->open(stra[i], ::file::mode_read | ::file::type_binary).failed())
            throw "failed";
         write_n_number(spfile, &ctx, (int32_t)file2->get_length());
         while ((uiRead = file2->read(buf, iBufSize)) > 0)
         {
            spfile->write(buf, uiRead);
            MD5_Update(&ctx, buf, uiRead);
         }
         spfile->seek(iPos, ::file::seek_begin);
         strMd5 = ::str::from(ctx);
         write_gen_string(spfile, NULL, strMd5);
         spfile->seek_to_end();

      }

      write_n_number(spfile, NULL, 2);


   }

   void system::ftd(const ::file::path & pszDir,const ::file::path & pszFile,::aura::application * papp)
   {

      string strVersion;

      ::file::file_sp spfile = App(papp).file().get_file(pszFile, ::file::mode_read | ::file::type_binary);

      if (spfile.is_null())
         throw "failed";

      read_gen_string(spfile, NULL, strVersion);

      int64_t n;

      string strRelative;
      string strMd5;
      string strMd5New;
      int32_t iBufSize = 1024 * 1024;
      memory buf;
      buf.allocate(iBufSize);
      int64_t iLen;
      MD5_CTX ctx;
      ::file::file_sp file2(allocer());
      memory_size_t uiRead;
      if (strVersion == "fileset v1")
      {
         while (true)
         {
            read_n_number(spfile, NULL, n);
            if (n == 2)
               break;
            read_gen_string(spfile, NULL, strMd5);
            MD5_Init(&ctx);
            read_gen_string(spfile, &ctx, strRelative);
            ::file::path strPath = ::file::path(pszDir) / strRelative;
            App(papp).dir().mk(strPath.folder());
            if (file2->open(strPath, ::file::mode_create | ::file::type_binary | ::file::mode_write).failed())
               throw "failed";
            read_n_number(spfile, &ctx, iLen);
            while (iLen > 0)
            {
               uiRead = spfile->read(buf, (UINT)(MIN(iBufSize, iLen)));
               if (uiRead == 0)
                  break;
               file2->write(buf, uiRead);
               MD5_Update(&ctx, buf, uiRead);
               iLen -= uiRead;
            }
            file2->close();
            strMd5New = ::str::from(ctx);
            if (strMd5 != strMd5New)
               throw "failed";
         }
      }

   }




   void system::write_n_number(::file::file *  pfile, void * pctx, int64_t iNumber)
   {

      string str;

      str.Format("%I64dn", iNumber);

      pfile->write((const char *)str, str.get_length());

      if (pctx != NULL)
      {

         MD5_Update((MD5_CTX *)pctx, (const char *)str, (int32_t)str.get_length());

      }

   }

   void system::read_n_number(::file::file *  pfile, void * pctx, int64_t & iNumber)
   {

      uint64_t uiRead;

      string str;

      char ch;

      while ((uiRead = pfile->read(&ch, 1)) == 1)
      {

         if (ch >= '0' && ch <= '9')
            str += ch;
         else
            break;

         if (pctx != NULL)
         {
            MD5_Update((MD5_CTX *)pctx, &ch, 1);
         }

      }

      if (ch != 'n')
         throw "failed";

      if (pctx != NULL)
      {
         MD5_Update((MD5_CTX *)pctx, &ch, 1);
      }

      iNumber = ::str::to_int64(str);

   }

   void system::write_gen_string(::file::file *  pfile, void * pctx, string & str)
   {
      ::count iLen = str.get_length();
      write_n_number(pfile, pctx, iLen);
      pfile->write((const char *)str, str.get_length());
      if (pctx != NULL)
      {
         MD5_Update((MD5_CTX *)pctx, (const char *)str, (int32_t)str.get_length());
      }
   }

   void system::read_gen_string(::file::file * pfile, void * pctx, string & str)
   {
      int64_t iLen;
      read_n_number(pfile, pctx, iLen);
      LPSTR lpsz = str.GetBufferSetLength((strsize)(iLen + 1));
      pfile->read(lpsz, (memory_size_t)iLen);
      if (pctx != NULL)
      {
         int64_t iProcessed = 0;
         while (iLen - iProcessed > 0)
         {
            int32_t iProcess = (int32_t)MIN(1024 * 1024, iLen - iProcessed);
            MD5_Update((MD5_CTX *)pctx, &lpsz[iProcessed], iProcess);
            iProcessed += iProcess;
         }
      }
      lpsz[iLen] = '\0';
      str.ReleaseBuffer();
   }




   bool system::resolve_link(string & strTarget, string & strFolder, string & strParams, const string & pszSource, ::user::primitive * puiMessageParentOptional)
   {

      return System.os().resolve_link(strTarget, strFolder, strParams, pszSource, puiMessageParentOptional);

   }



   string system::md5(var varFile, ::aura::application * papp)
   {

      ::cres cres;

      ::file::file_sp pfile;

      try
      {

         pfile = App(papp).file().get_file(varFile, ::file::type_binary | ::file::mode_read);

      }
      catch (::file::exception &)
      {

         return "";

      }

      if (pfile.is_null())
      {

         return "";

      }

      return md5(pfile);

   }


   string system::md5(file * pfile)
   {

      memory mem(get_app());

      mem.allocate(1024 * 256);

      MD5_CTX ctx;

      MD5_Init(&ctx);

      memory_size_t iRead;

      while ((iRead = (memory_size_t)pfile->read(mem.get_data(), mem.get_size())) > 0)
      {

         MD5_Update(&ctx, mem.get_data(), iRead);

      }

      return ::str::from(ctx);

   }


   string system::nessie(var varFile, ::aura::application * papp)
   {

      ::cres cres;

      ::file::file_sp pfile;

      try
      {

         pfile = App(papp).file().get_file(varFile, ::file::type_binary | ::file::mode_read);

      }
      catch(::file::exception &)
      {

         return "";

      }

      if (pfile.is_null())
      {

         return "";

      }

      return nessie(pfile);

   }


   string system::nessie(file * pfile)
   {

      memory mem(get_app());

      mem.allocate(1024 * 256);

      WHIRLPOOL_CTX ns;

      WHIRLPOOL_Init(&ns);

      memory_size_t iRead;

      while ((iRead = (memory_size_t)pfile->read(mem.get_data(), mem.get_size())) > 0)
      {

         WHIRLPOOL_Update(&ns, mem.get_data(), iRead);

      }

      return ::str::from(ns);

   }


   bool system::get_last_write_time(FILETIME * pfiletime,const string & strFilename)
   {

      throw interface_only_exception(get_app());

   }


   bool system::update_module_path()
   {

      m_pathModule = "";
      m_pathCa2Module = "";


      return true;

   }


   bool system::initialize()
   {

      return true;

   }

   bool system::prepare_output(::aura::application * papp, path & pathDownloading, const path & pathOut, ostream & os)
   {

      Sys(papp).dir().mk(pathOut.folder(), papp);

      ::file::file_sp fileOut;

      int64_t iTry = 0;

      while (true)
      {

         pathDownloading = pathOut + ".downloading." + ::str::zero_pad(::str::from(iTry), 20);

         fileOut = papp->file().get_file(pathDownloading, ::file::mode_create | ::file::type_binary | ::file::mode_write);

         if (fileOut.is_set())
         {

            break;

         }

         iTry++;

      }

      os.m_spfile = fileOut;

      if (os.m_spfile.is_null())
      {

         return false;

      }

      return true;

   }

   bool system::prepare_input(::aura::application * papp, istream & is, const path & pathIn)
   {

      ::file::file_sp fileIn = Sess(papp).file().get_file(pathIn, ::file::type_binary | ::file::mode_read);

      if (fileIn.is_null())
      {

         return false;

      }

      return prepare_input(papp, is, fileIn);

   }

   bool system::prepare_input(::aura::application * papp, istream & is, ::file::file * pfileIn)
   {

      is.m_spfile = pfileIn;

      if (is.m_spfile.is_null())
      {

         return false;

      }

      return prepare_input(papp, is);

   }

   bool system::prepare_input(::aura::application * papp, istream & is)
   {

      if (is.m_spfile.is_null())
      {

         return false;

      }

      return true;

   }


   bool system::post_output(::aura::application * papp, ::file::path pathOut, ::file::path pathDownloading)
   {

      try
      {

         if (rename(pathOut, pathDownloading, papp))
         {

            TRACE("Failed to rename \"downloading\" file from %s to %s", pathDownloading, pathOut);

            del(pathDownloading, papp);

            return false;

         }

      }
      catch (...)
      {

         return false;

      }

      return true;

   }


   ::file::file_sp system::get_file(var varFile,UINT nOpenFlags,cres * pfesp,::aura::application * papp)
   {

      if(pfesp != NULL)
      {
         ::release(pfesp->m_p);
      }



      ::cres cres;

      ::file::file_sp spfile;

      ::file::path strPath;

      if(varFile.get_type() == var::type_element)
      {

         spfile = varFile.cast < ::file::file >();

         if(spfile.is_set())
            return spfile;

      }
      else if(varFile.get_type() == var::type_string)
      {

         strPath = varFile;

         //strPath.trim("\"'");

      }
      else if(varFile.get_type() == var::type_stra)
      {

         if(varFile.stra().get_count() > 0)
         {

            strPath = varFile.stra()[0];

         }

         //strPath.trim("\"'");

      }
      else if(varFile.get_type() == var::type_propset)
      {

         if(varFile.has_property("url"))
         {

            strPath = varFile["url"];

            //strPath.trim("\"'");

         }

      }

      if(varFile.get_type() == var::type_propset && varFile.propset()["file"].cast < ::file::binary_file >() != NULL)
      {

         spfile = varFile.propset()["file"].cast < ::file::file >();

      }
      else
      {



         if(strPath.is_empty())
         {
            TRACE("::application::get_file file with empty name!!");
            return spfile;
         }

         if (pfesp != NULL)
         {
            ::release(pfesp->m_p);
         }

         //::cres cres;

         //::file::file_sp spfile;

         //::file::path strPath;

         //if (varFile.get_type() == var::type_element)
         //{

         //   spfile = varFile.cast < ::file::file >();

         //   if (spfile.is_set())
         //      return spfile;

         //}
         //else
         //if (varFile.get_type() == var::type_string)
         //{

         //   strPath = varFile;

         //   //strPath.trim("\"'");

         //}
         //else if (varFile.get_type() == var::type_stra)
         //{

         //   if (varFile.stra().get_count() > 0)
         //   {

         //      strPath = varFile.stra()[0];

         //   }

         //   //strPath.trim("\"'");

         //}
         //else if (varFile.get_type() == var::type_propset)
         //{

         //   if (varFile.has_property("url"))
         //   {

         //      strPath = varFile["url"];

         //      //strPath.trim("\"'");

         //   }

         //}

         //if (varFile.get_type() == var::type_propset && varFile.propset()["file"].cast < ::file::binary_file >() != NULL)
         //{

         //   spfile = varFile.propset()["file"].cast < ::file::binary_file >();

         //}
         //else
         if (::str::find_file_extension("zip:", strPath) >= 0)
         {

            sp(zip::in_file) pinfile = canew(zip::in_file(papp));

            if (pinfile != NULL)
            {

               if (!pinfile->unzip_open(strPath, 0))
               {

                  pinfile.release();

               }

            }

            spfile = pinfile;

         }
         else if (::str::begins(strPath, astr.strHttpProtocol) || ::str::begins(strPath, astr.strHttpsProtocol))
         {

            ::url_domain domain;

            domain.create(System.url().get_server(strPath));

            if (domain.m_strRadix == "ca2" && ::str::begins(System.url().get_object(strPath), astr.strMatterUri))
            {

               ::file::path strFile(strPath);

               if (::str::ends(strPath, "en_us_international.xml"))
               {
                  TRACE("Debug Here");
               }

               if (::str::ends(strPath, "text_select.xml"))
               {
                  TRACE("Debug Here");
               }

               if (::str::ends(strPath, "arialuni.ttf"))
               {
                  TRACE("Debug Here : arialuni.ttf");
               }

#ifdef WINDOWS
               strFile.replace("://", "_\\");
#else
               strFile.replace("://", "_/");
#endif
               strFile = System.dir().appdata() / "cache" / strFile + ".local_copy";

               single_lock sl(System.http().m_pmutexDownload, true);

               if (App(papp).file().exists(strFile) && !(System.http().m_straDownloading.contains(strPath) || System.http().m_straExists.contains(strPath)))
               {

                  sl.unlock();

                  spfile = App(papp).alloc(System.type_info < ::file::binary_file >());

                  try
                  {

                     if (spfile->open(strFile, nOpenFlags).succeeded())
                     {
                        TRACE("from_exist_cache:\"" + strPath + "\"");
                        return spfile;

                     }
                  }
                  catch (...)
                  {

                  }

                  try
                  {

                     spfile.release();

                  }
                  catch (...)
                  {
                  }

               }


               var varQuery;

               varQuery["raw_http"] = true;

               property_set set(papp);

               set["raw_http"] = true;
               set["disable_common_name_cert_check"] = true;

               //            bool bOk = true;

               sl.lock();

               while (System.http().m_straDownloading.contains(strPath) || System.http().m_straExists.contains(strPath))
               {
                  sl.unlock();
                  Sleep(100);
                  sl.lock();
               }

               if (!Sys(papp).http().m_straDownloading.contains(strPath) && Sess(papp).m_http.exists(strPath, &varQuery, set))
               {

                  System.http().m_straDownloading.add(strPath);

                  sl.unlock();

                  auto pfile = canew(::memory_file(papp));

                  spfile = pfile;

                  if (!Application.http().get(strPath, *pfile->get_memory(), set))
                  {

                     sl.lock();

                     System.http().m_straDownloading.remove(strPath);

                     sl.unlock();

                  }
                  else
                  {

                     try
                     {

                        App(papp).file().transfer(strFile, spfile);

                     }
                     catch (...)
                     {

                     }

                     sl.lock();

                     System.http().m_straDownloading.remove(strPath);

                     sl.unlock();

                     spfile->seek_to_begin();

                  }

               }


            }
            else
            {

               auto pfile = canew(memory_file(papp));

               spfile = pfile;

               property_set set;

               set = varFile["http_set"].propset();

               bool bRawHttp1 = (bool)varFile["http_set"]["raw_http"];

               UNUSED(bRawHttp1);

               bool bRawHttp2 = (bool)spfile->oprop("http_set")["raw_http"];

               UNUSED(bRawHttp2);

               if (Application.http().get(strPath, *pfile->get_memory(), set))
               {

                  cres = no_exception;

               }
               else
               {

                  cres = failure;

               }

            }

         }
         /* xxx      else if(::str::begins(strPath, "fs://"))
         {

         if(&Session == NULL)
         {

         spfile = NULL;

         }
         else
         {

         spfile = Session.m_prfs->get_file(varFile, nOpenFlags);

         }

         } */
         else if (::str::begins_eat_ci(strPath, "matter://"))
         {

            ::aura::application * pappLookup = NULL;

            if (System.url().get_server("matter://" + strPath) == papp->m_strAppName)
            {

               strPath = System.url().get_object("matter://" + strPath).Mid(1);

               spfile = App(papp).alloc(System.type_info < ::file::binary_file >());

               cres = spfile->open(App(papp).dir().matter(strPath), nOpenFlags);

            }
            else if (&Session != NULL && Session.m_mapApplication.Lookup(System.url().get_server("matter://" + strPath), pappLookup) && App(pappLookup).m_strAppName.has_char())
            {

               spfile = App(pappLookup).file().get_file("matter://" + strPath, nOpenFlags, &cres);

            }
            else
            {

               ::file::path pathUrl = App(papp).dir().matter(strPath);

               spfile = get_file(pathUrl, nOpenFlags, &cres, papp);

            }

         }
         else
         {

          //  return NULL;




            /*            if((nOpenFlags & ::file::mode_create) == 0 && !exists(strPath))
            {
            TRACE("::application::file does not exist!! : \"%s\"",strPath);
            return spfile;
            }
            */

            spfile = App(papp).alloc(System.type_info < ::file::file >());

            cres = spfile->open(strPath, nOpenFlags);

         }


      }

      if(cres.failed())
      {

         spfile.release();

         if(pfesp != NULL)
         {

            *pfesp = cres;

         }

      }

      return spfile;


   }


   bool system::transfer(::aura::application * papp, const path & pszOutput, const path & lpszSource)
   {

      return output(papp, pszOutput, this, &system::transfer, lpszSource);

   }


   bool system::transfer(::aura::application * papp, const path & pszOutput, ::file::file * pfileIn)
   {

      return output(papp, pszOutput, this, &system::transfer, pfileIn);

   }


   bool system::transfer(::aura::application * papp, const path & pszOutput, ::file::istream & istream)
   {

      return output(papp, pszOutput, this, &system::transfer, istream);

   }


   bool system::transfer(::aura::application * papp, file * pfileOut, const path & lpszSource)
   {

      return output(papp, pfileOut, this, &system::transfer, lpszSource);

   }


   bool system::transfer(::aura::application * papp, file * pfileOut, ::file::file * pfileIn)
   {

      return output(papp, pfileOut, this, &system::transfer, pfileIn);

   }


   bool system::transfer(::aura::application * papp, file * pfileOut, ::file::istream & istream)
   {

      return output(papp, pfileOut, this, &system::transfer, istream);

   }


   bool system::transfer(::aura::application * papp, ostream & os, const path & lpszSource)
   {

      return output(papp, os, this, &system::transfer, lpszSource);

   }


   bool system::transfer(::aura::application * papp, ostream & os, ::file::file * pfileIn)
   {

      return output(papp, os, this, &system::transfer, pfileIn);

   }


   bool system::transfer(::aura::application * papp, ostream & os, ::file::istream & istream)
   {

      return output(papp, os, this, &system::transfer, istream);

   }

   bool system::transfer(::file::ostream & ostream, ::file::istream & istream)
   {
      class memory memory;
      memory.allocate(1024 * 256);
      memory_size_t  uiRead;
      while ((uiRead = istream.read(memory, memory.get_size())) > 0)
      {
         ostream.write(memory, uiRead);
      }
      return true;
   }


   bool system::is_link(string strPath)
   {

      return false;

   }


} // namespace file






