
namespace str
{

template < typename T >
inline string  from(const T & t)
{
   
   string str;
   
   to_string(str, t);
   
   return str;
   
}

template < typename T >
inline string  from_int(const T & t)
{
   
   string str;
   
   to_string(str, (int64_t) t);
   
   return str;
   
}

template < typename T >
inline string  from_uint(const T & t)
{
   
   string str;
   
   to_string(str, (uint64_t)t);
   
   return str;
   
}

} // namespace str

inline void simple_string::construct(string_manager * pstringmanager)
   {
      ENSURE( pstringmanager != NULL );
      string_data * pData = pstringmanager->GetNilString();
      attach( pData );
   }

   inline simple_string::simple_string(const simple_string & strSrc, string_manager * pstringmanager  )
   {
      //if(is_null(&strSrc))
      //{
      //   ENSURE( pstringmanager != NULL );

      //   string_data* pData = pstringmanager->allocate( 0 );
      //   attach( pData );
      //   set_length( 0 );
      //   return;
      //}
      string_data* pSrcData = strSrc.get_data();
      string_data* pNewData = CloneData( pSrcData );
      attach( pNewData );
   }

inline simple_string::simple_string(const string_data  * pdata, string_manager * pstringmanager  )
{
   if(pdata == NULL)
   {
      ENSURE( pstringmanager != NULL );

      string_data* pData = pstringmanager->allocate( 0);
      if( pData == NULL )
      {
         throw_memory_exception();
      }
      attach( pData );
      set_length( 0 );
      return;
   }
   string_data* pSrcData = const_cast < string_data * > (pdata);
   string_data* pNewData = CloneData( pSrcData );
   attach( pNewData );
}


   inline simple_string::simple_string(const char * pszSrc,string_manager * pstringmanager )
   {
      ENSURE( pstringmanager != NULL );

      strsize nLength = StringLength( pszSrc );
      string_data* pData = pstringmanager->allocate( nLength);
      if( pData == NULL )
      {
         throw_memory_exception();
      }
      attach( pData );
      set_length( nLength );
#if _SECURE_TEMPLATE
      CopyChars( m_pszData, nLength, pszSrc, nLength );
#else
      CopyChars( m_pszData, pszSrc, nLength );
#endif
   }
   inline simple_string::simple_string(const char* pchSrc,strsize nLength,string_manager * pstringmanager)
   {
      ENSURE( pstringmanager != NULL );

      if(pchSrc == NULL && nLength != 0)
         throw invalid_argument_exception(get_thread_app());

      if(nLength < 0)
         nLength = (strsize) strlen(pchSrc);

      string_data * pData = pstringmanager->allocate( nLength);
      if( pData == NULL )
      {
         throw_memory_exception();
      }
      attach( pData );
      set_length( nLength );
#if _SECURE_TEMPLATE
      CopyChars( m_pszData, nLength, pchSrc, nLength );
#else
      CopyChars( m_pszData, pchSrc, nLength );
#endif
   }

   // besides returning a reference (and const does not really impedes changing), do not change a simple_string (or string) directly,
   // there may be multiple instances of a string (all referencing the same pointer).
   inline const char & simple_string::operator [](strsize iChar ) const
   {

      //ASSERT( (iChar >= 0) && (iChar <= get_length()) );  // Indexing the '\0' is OK

      //if( (iChar < 0) || (iChar > get_length()) )
        // throw invalid_argument_exception(get_thread_app());

      return m_pszData[iChar];

   }
   inline char simple_string::get_at(strsize iChar ) const
   {
      //ASSERT( (iChar >= 0) && (iChar <= get_length()) );  // Indexing the '\0' is OK
      //if( (iChar < 0) || (iChar > get_length()) )
        // throw invalid_argument_exception(get_thread_app());

      return m_pszData[iChar];
   }


   inline   void simple_string::set_at(strsize iChar,char ch )
   {
      ASSERT( (iChar >= 0) && (iChar < get_length()) );

      if( (iChar < 0) || (iChar >= get_length()) )
         throw invalid_argument_exception(get_thread_app());

      strsize nLength = get_length();
      char * pszBuffer = GetBuffer();
      pszBuffer[iChar] = ch;
      ReleaseBufferSetLength( nLength );

   }


   inline void simple_string::SetString(const char * pszSrc,strsize nLength )
   {
      if( nLength == 0 )
      {
         Empty();
      }
      else
      {
         // It is possible that pszSrc points to a location inside of our
         // buffer.  GetBuffer() might change m_pszData if (1) the buffer
         // is shared or (2) the buffer is too small to hold the new
         // string.  We detect this aliasing, and modify pszSrc to point
         // into the newly allocated buffer instead.

         if(pszSrc == NULL)
            throw invalid_argument_exception(get_thread_app());

         uint_ptr nOldLength = (uint_ptr) get_length();
         uint_ptr nOffset = (uint_ptr) (pszSrc - GetString());
         // If 0 <= nOffset <= nOldLength, then pszSrc points into our
         // buffer

         char * pszBuffer = GetBuffer( nLength );
         if( nOffset <= nOldLength )
         {
#if _SECURE_TEMPLATE
            CopyCharsOverlapped( pszBuffer, nLength,
               pszBuffer+nOffset, nLength );
#else
            CopyCharsOverlapped( pszBuffer, pszBuffer+nOffset, nLength );
#endif
         }
         else
         {
#if _SECURE_TEMPLATE
            CopyChars( pszBuffer, nLength, pszSrc, nLength );
#else
            CopyChars( pszBuffer, pszSrc, nLength );
#endif
         }
         ReleaseBufferSetLength( nLength );
      }
   }


   inline void string_file::set_length(strsize nLength )
   {
      ASSERT( nLength >= 0 );
      ASSERT( nLength <= m_nBufferLength );

      if( nLength < 0 )
         throw invalid_argument_exception(get_thread_app());

      m_nLength = nLength;
   }



   inline void simple_string::ReleaseBufferSetLength(strsize nNewLength )
   {
      ASSERT( nNewLength >= 0 );
      set_length( nNewLength );
   }
   inline void simple_string::Truncate(strsize nNewLength )
   {
      ASSERT( nNewLength <= get_length() );
      GetBuffer( nNewLength );
      ReleaseBufferSetLength( nNewLength );
   }

   inline void simple_string::SetManager(string_manager * pstringmanager )
   {
      ASSERT( is_empty() );

      string_data * pData = get_data();
      pData->Release();
      pData = pstringmanager->GetNilString();
      attach( pData );
   }

   inline void simple_string::Reallocate(strsize nLength )
   {

      attach(get_data()->Reallocate(nLength));

   }

   inline string_data * string_data::Reallocate(strsize nLength) // current data will be invalid after function call
   {

      ASSERT(nAllocLength < nLength);

      return pstringmanager->Reallocate(this,nLength);

   }


   inline void string_data::AddRef() RELEASENOTHROW
{
   ASSERT(nRefs > 0);
   _gen_InterlockedIncrement(&nRefs);
}
inline bool string_data::IsLocked() const NOTHROW
{
   return nRefs < 0;
}
inline bool string_data::IsShared() const NOTHROW
{
   return( nRefs > 1 );
}
inline void string_data::lock() RELEASENOTHROW
{
   ASSERT( nRefs <= 1 );
   nRefs--;  // Locked buffers can't be shared, so no interlocked operation necessary
   if( nRefs == 0 )
   {
      nRefs = -1;
   }
}
inline void string_data::Release() RELEASENOTHROW
{
   ASSERT( nRefs != 0 );

   _gen_InterlockedDecrement( &nRefs );
   if(nRefs <= 0 )
   {
      pstringmanager->Free( this );
   }
}
inline void string_data::unlock() RELEASENOTHROW
{
   ASSERT( IsLocked() );

   if(IsLocked())
   {
      nRefs++;  // Locked buffers can't be shared, so no interlocked operation necessary
      if( nRefs == 0 )
      {
         nRefs = 1;
      }
   }
}


inline strsize string::replace_ci(const char * pszOld,const char * pszNew, strsize iStart)
{
   
   *this = ::str::replace_ci(pszOld, pszNew, *this, iStart);
   
   return get_length();
   
}


inline ::count string::utf8_replace(const char * pszOld,const char * pszNew, strsize iStart)
{
   
   return ::str::utf8_replace(*this, pszOld, pszNew, iStart);
   
}


inline bool string::ends_ci(const string & strSuffixCandidate) const
{

   return ::str::ends_ci(*this, strSuffixCandidate);

}

inline bool string::ensure_ends_ci(const string & strSuffix)
{

   if (!::str::ends_ci(*this, strSuffix))
   {

      operator +=(strSuffix);

      return false;

   }

   return true;

}










template < class BASE >
strsize stdstring < BASE >::copy(typename BASE::value_type * s,strsize len,strsize pos) const
{

   strsize thislen = this->get_length();

   if(pos < 0)
   {

      pos = thislen + pos + 1;

   }

   if(pos < 0)
   {

      len += pos;

      pos = 0;

   }

   if(pos >= thislen)
   {

      return 0;

   }

   len = MIN(len,thislen - pos);

   if(len <= 0)
   {

      return 0;

   }

   typedef typename BASE::value_type char_type;

   typedef const char_type const_char_type;


   memcpy(s,((const_char_type *)*this) + (pos * sizeof(char_type)),(len * sizeof(char_type)));

   return len;

}





inline verisimple_wstring & verisimple_wstring::operator = (const string & str)
{

   operator = (::str::international::utf8_to_unicode(str));

   return *this;
}





inline strsize strlen_s_dup(const char * psz,strsize nsize)
{
   if(psz == NULL)
      return 0;
   strsize len = 0;
   while(*psz != '\0' && nsize > 0)
   {
      psz++;
      nsize--;
      len++;
   }
   return len;
}


template < >
inline void std_string_assign(stdstring < simple_string > & t,const char * psz)
{
   t.assign(psz);
}

template < >
inline void std_string_assign(stdstring < simple_string > & t,const unichar * psz)
{
   t = ::str::international::unicode_to_utf8(psz);
}

template < >
inline void std_string_bassign(stdstring < simple_string > & t,const byte * psz,strsize nsize)
{
   t = (const char *) string((const char *) psz, MIN(nsize, strlen_s_dup((const char *) psz, nsize)));
}

template < >
inline void std_string_assign(stdstring < simple_string > & t,const string * pstr)
{
   t = *pstr;
}

template < >
inline void std_string_assign(stdstring < simple_string > & t,const wstring * pwstr)
{
   t = ::str::international::unicode_to_utf8(*pwstr);
}

template < >
inline void std_string_assign(stdstring < simple_string > & t,const bstring * pbstr)
{
   t = string((const char *)pbstr->get_data(),MIN(pbstr->get_length(),natural(strlen_s_dup((const char *)pbstr->get_data(),pbstr->get_length()))));
}

template < >
inline void std_string_assign(stdstring < verisimple_wstring > & t,const char * psz)
{
   t.assign(psz);
}


template < >
inline void std_string_assign(stdstring < verisimple_wstring > & t,const unichar * psz)
{

   t.assign(psz);

}


template < >
inline void std_string_bassign(stdstring < verisimple_wstring > & t,const byte * psz,strsize nsize)
{
   t = ::str::international::utf8_to_unicode(string((const char *) psz,MIN(nsize,strlen_s_dup((const char *) psz,nsize))));
}

template < >
inline void std_string_assign(stdstring < verisimple_wstring > & t,const string * pstr)
{
   t = ::str::international::utf8_to_unicode(*pstr);
}

template < >
inline void std_string_assign(stdstring < verisimple_wstring > & t,const stdstring < simple_string > * pstr)
{
   t = ::str::international::utf8_to_unicode(*pstr);
}

template < >
inline void std_string_assign(stdstring < verisimple_wstring > & t,const wstring * pwstr)
{
   t = *pwstr;
}

template < >
inline void std_string_assign(stdstring < verisimple_wstring > & t,const bstring * pbstr)
{
   t = ::str::international::utf8_to_unicode(string((const char *)pbstr->get_data(),MIN(pbstr->get_length(),natural(strlen_s_dup((const char *)pbstr->get_data(),pbstr->get_length())))));
}


template < >
inline void std_string_assign(stdstring < memory > & t,const char * psz)
{
   t.assign(psz);
}

template < >
inline void std_string_assign(stdstring < memory > & t,const unichar * psz)
{
   t.assign(::str::international::unicode_to_utf8(psz));
}

template < >
inline void std_string_bassign(stdstring < memory > & t,const byte * psz,strsize nsize)
{
   t.assign(string((const char *)psz,MIN(nsize,strlen_s_dup((const char *)psz,nsize))));
}

template < >
inline void std_string_assign(stdstring < memory > & t,const string * pstr)
{
   t = (const char *) *pstr;
}

template < >
inline void std_string_assign(stdstring < memory > & t,const wstring * pwstr)
{
   t = (const char *) ::str::international::unicode_to_utf8(*pwstr);
}

template < >
inline void std_string_assign(stdstring < memory > & t,const bstring * pbstr)
{
   t = *pbstr;
}















namespace str
{


   ///
   /// \param[in] psz Parsed string.
   /// \param[in] strPrefix Candidate prefix string.
   /// \return True if \p psz is prefixed with \p strPrefix.
   /// \note Binary comparison.
   ///
   inline bool begins(const char * psz,const string & strPrefix)
   {

      return psz != NULL && strncmp(psz,strPrefix,strPrefix.get_length()) == 0;

   }


   inline bool begins(const string & str,const string & strPrefix)
   {

      return strncmp(str,strPrefix,strPrefix.get_length()) == 0;

   }

   inline bool begins_get_prefix(string & strGetPrefix, const string & str, const string & strPrefix)
   {

      if(strncmp(str, strPrefix, strPrefix.get_length()) == 0)
      {

         strGetPrefix = strPrefix;

         return true;

      }

      return false;

   }


   inline bool begins(const id & id,const string & strPrefix)
   {

      return id.is_text() && id.m_psz != NULL && strncmp(id.m_psz,strPrefix,strPrefix.get_length()) == 0;

   }



   inline bool begins(const char * psz,string & strFed,const string & strPrefix)
   {

      if(!begins(psz,strPrefix))
      {

         return false;

      }

      strFed = strPrefix;

      return true;

   }


   inline bool begins(const string & str,string & strFed,const string & strPrefix)
   {

      if(!begins(str,strPrefix))
      {

         return false;

      }

      strFed = strPrefix;

      return true;

   }


   inline bool begins(const id & id,string & strFed,const string & strPrefix)
   {

      if(!begins(id,strPrefix))
      {

         return false;

      }

      strFed = strPrefix;

      return true;

   }

   inline bool begins_with(const char * psz,const string & strPrefix)
   {

      return psz != NULL && strncmp(psz,strPrefix,strPrefix.get_length()) == 0;

   }


   inline bool begins_with(const string & str,const string & strPrefix)
   {

      return strncmp(str,strPrefix,strPrefix.get_length()) == 0;

   }

   /// If \p str is prefixed with \p strPrefix, remove \p strPrefix from \p str.
   /// \param[in,out] str Parsed string.
   /// \param[in] strPrefix Candidate prefix string.
   /// \return True if \p psz is prefixed with \p strPrefix.
   /// \note Binary comparison.
   inline bool begins_eat(string & str,const string & strPrefix)
   {

      if(strncmp(str,strPrefix,strPrefix.get_length()) == 0)
      {

         str = str.Mid(strPrefix.get_length());

         return true;

      }

      return false;

   }



} // namespace str










class CLASS_DECL_AURA aura_str_pool
{
public:

   const string strUifsProtocol = "uifs://";
   const string strIfsProtocol = "ifs://";
   const string strHttpProtocol = "http://";
   const string strHttpsProtocol = "https://";
   const string strFsProtocol = "fs://";
   const string strSlash = "/";
   const string strDoubleBackSlash = "\\\\";
   const string strSystem = "system";
   const string strAppCoreDeepfish = "app-core/deepfish_";
   const string stringSysCommand = "syscommand::";
   const string strPassthroughUri = "/passthrough/";
   const string strMatterUri = "/matter/";
   const string str200Space = "200 ";
   const string strWwwDot = "www.";

   const string szXMLPIOpen = "<?";
   const string szXMLPIClose = "?>";
   const string szXMLDOCTYPEOpen = "<!DOCTYPE";
   const string szXMLDOCTYPEClose = ">";
   const string szXMLCommentOpen = "<!--";
   const string szXMLCommentClose = "-->";
   const string szXMLCDATAOpen = "<![CDATA[";
   const string szXMLCDATAClose = "]]>";




};



extern CLASS_DECL_AURA aura_str_pool * s_paurastrpool;

#define astr (*s_paurastrpool)

//#ifndef __GNUC__
//inline id::operator string() const
//{
//
//   return str();
//
//}
//#endif


//
//
//template < >
//inline string & to_string(string & str, unichar * pwsz)
//{
//
//   ::str::international::unicode_to_utf8(str,pwsz);
//
//   return str;
//
//}
//
//
template < >
inline void to_string(string & str, const verisimple_wstring & wstr)
{

   ::str::international::unicode_to_utf8(str, wstr, wstr.get_length());

}




template < >
inline void to_string(string & str, const stdstring<verisimple_wstring> & wstr)
{

   ::str::international::unicode_to_utf8(str, wstr, wstr.get_length());

}



//#ifdef __GNUC__

inline string::string(const var & var):stdstring< simple_string> (string_trait::GetDefaultManager()) { var.to_string(*this);}
#ifdef __GNUC__
inline string::string(const property & prop):stdstring< simple_string> (string_trait::GetDefaultManager()) { prop.to_string(*this);}
#endif
inline string::string(const id & id):stdstring< simple_string> (string_trait::GetDefaultManager()) { id.to_string(*this);}


//template < typename Type, typename RawType >
//memory string_array < Type, RawType > ::GetFormatV004()
//{
//
//   strsize iTotalLength = 0;
//
//   strsize i;
//
//   for(i = 0; i < this->m_nSize; i++)
//   {
//      iTotalLength += this->element_at(i).get_length();
//      iTotalLength++;
//   }
//
//   if(iTotalLength == 0)
//   {
//      iTotalLength = 2;
//   }
//   else
//   {
//      iTotalLength++;
//   }
//
//   memory mem;
//
//   mem.allocate(iTotalLength * sizeof(char));
//
//   char * lpsz = (char *)mem.get_data();
//
//   memset(lpsz,0x00,iTotalLength * sizeof(char));
//
//   char * lpszN = lpsz;
//
//   strsize iLength;
//
//   for(i = 0; i < this->m_nSize; i++)
//   {
//
//      Type * pstr = &this->element_at(i);
//
//      iLength = pstr->get_length();
//
//#ifdef WINDOWS
//      strcat_s(lpszN,iLength,*pstr);
//#else
//      strcat(lpszN,*pstr);
//#endif
//
//#ifdef WINDOWSEX
//      iLength = _tcsnbcnt(lpszN,iLength);
//#elif defined(METROWINDOW)
//      iLength = wcsnbcnt(lpszN,iLength);
//#else
//      iLength = strlen(lpszN);
//#endif
//
//      lpszN = (char *)((LPBYTE)lpszN) + iLength + sizeof(char);
//
//   }
//
//   if(iTotalLength == 2)
//   {
//      memset(lpsz,0x00,iTotalLength * sizeof(char));
//   }
//   else
//   {
//      memset(lpszN,0x00,sizeof(char));
//   }
//
//   return mem;
//
//}

inline string::string(const istring & istr) :
stdstring< simple_string>(istr.m_str,string_trait::GetDefaultManager())
{

}




inline string & string::operator += (const id & id)
{

   return operator += (id.str());

}




inline string string_format::to_string()
{
   
   return string(m_pszBuffer, m_iLength);

}

template < >
inline void to_string(string & str, const int16_t & i)
{

   str = i64toa_dup(i);

}


template < >
inline void to_string(string & str, const uint16_t & ui)
{

   str = ui64toa_dup(ui);

}


template < >
inline void to_string(string & str, const int32_t & i)
{

   str = i64toa_dup(i);

}

template < >
inline void to_string(string & str, const uint32_t & ui)
{

   str = ui64toa_dup(ui);

}

template < >
inline void to_string(string & str, const int64_t & i)
{

   str = i64toa_dup(i);

}

template < >
inline void to_string(string & str, const uint64_t & ui)
{

   str = ui64toa_dup(ui, 10);

}


template < >
CLASS_DECL_AURA void to_string(string & str, const float & f);


template < >
CLASS_DECL_AURA void to_string(string & str, const double & d);


#ifdef ANDROID

template < >
inline void to_string(string & str, const long int & i)
{

   str = i64toa_dup(i);

}

#endif



#ifdef WINDOWS


template < >
inline void to_string(string & str, const LONG & i)
{

   str = i64toa_dup(i);

}

template < >
inline void to_string(string & str, const DWORD & ui)
{

   str = ui64toa_dup(ui);

}


#endif



#if defined(METROWIN) && defined(__cplusplus_winrt)


inline string & string::operator = (String ^ & str)
{


   return operator = (begin(str));


}


#endif
