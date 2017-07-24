#pragma once


class string_manager;



struct CLASS_DECL_AURA string_data
{
   string_manager * pstringmanager;  // string manager for this string_data
   strsize nDataLength;  // Length of currently used data in XCHARs (not including terminating NULL)
   strsize nAllocLength;  // Length of allocated data in XCHARs (not including terminating NULL)
   long nRefs;     // Reference count: negative == locked
   // char data[nAllocLength+1]  // A string_data is always followed in memory by the actual array of character data

   inline char * data() NOTHROW;
   inline void AddRef() RELEASENOTHROW;
   inline bool IsLocked() const NOTHROW;
   inline bool IsShared() const NOTHROW;
   inline void lock() RELEASENOTHROW;
   inline void Release() RELEASENOTHROW;
   inline void unlock() RELEASENOTHROW;

   inline string_data * Reallocate(strsize nLength); // current data will be invalid after function call
};




class CLASS_DECL_AURA nil_string_data :
   public string_data
{
public:


   nil_string_data() NOTHROW
   {
      pstringmanager = NULL;
      nRefs = 2;  // Never gets freed by string_manager
      nDataLength = 0;
      nAllocLength = 0;
      achNil[0] = 0;
      achNil[1] = 0;
   }

   void SetManager(string_manager * pMgr ) NOTHROW
   {
//      ASSERT( pstringmanager == NULL );
      pstringmanager = pMgr;
   }



public:


   unichar achNil[2];


};










// on 2015-03-01 with mummi thinking on carlos
// with a lot of only my side friendship virtual friends...
// turning string manager into a hard-coded nCharSize == 1 (char string_manager).
// TIP you can create wstring_manager
class string_manager
{
protected:


   nil_string_data  m_nil;


public:


   string_manager();


   //inline string_data * allocate(strsize nChars, int32_t nCharSize);
   inline string_data * allocate(strsize nChars);
   inline void Free(string_data * pData);
   inline string_data * Reallocate(string_data * pData,strsize nChars);
   //inline string_data * Reallocate(string_data * pData, strsize nChars, int32_t nCharSize);
   inline string_data * GetNilString() ;
   inline string_manager * Clone() { return this; }

};

// TODO turn string manager into template so STRING_MANAGER_CHAR_SIZE
//inline string_data * string_manager::allocate(strsize nChars,int32_t nCharSize)
inline string_data * string_manager::allocate(strsize nChars)
{
   //size_t nTotalSize;
   string_data * pData;
   //size_t nDataBytes;

   //ASSERT(nCharSize > 0);

   //nDataBytes = (nChars+1)*nCharSize;
   //nTotalSize = sizeof( string_data  )+nDataBytes;

   //bool bEnable = __enable_memory_tracking(FALSE);

   //try
   //{
//      pData = (string_data *) m_palloca->alloc(nTotalSize);
      pData = (string_data *) memory_alloc_no_track(nChars + 1 + sizeof(string_data));
   //}
   //catch(...)
   //{
      //return NULL;
   //}

   //__enable_memory_tracking(bEnable);

   //if (pData == NULL)
     // return NULL;
   pData->pstringmanager = this;
   pData->nRefs = 1;
   pData->nAllocLength = nChars;
   pData->nDataLength = 0;

   return pData;
}

inline void string_manager::Free(string_data * pData)
{
//   size_t nTotalSize = sizeof( string_data  ) + pData->nAllocLength + 1;
//   m_palloca->free(pData, nTotalSize);
   memory_free(pData);
}
//inline string_data * string_manager::Reallocate(string_data * pOldData,strsize nChars,int32_t nCharSize)
inline string_data * string_manager::Reallocate(string_data * pOldData, strsize nChars)
{
   //string_data * pNewData = NULL;
   //size_t nNewTotalSize;
   //size_t nNewDataBytes;
   //size_t nOldTotalSize;
   //size_t nOldDataBytes;

////   ASSERT(nCharSize > 0);
//
//   if(nChars < 0)
//   {
////      ASSERT(FALSE);
//      return NULL;
//   }

   //nNewDataBytes = (nChars+1)*nCharSize;
   //nNewTotalSize = sizeof( string_data  )+nNewDataBytes;
   //nOldDataBytes = (pOldData->nAllocLength+1)*nCharSize;
   //nOldTotalSize = sizeof( string_data  ) + nOldDataBytes;

   //bool bEnable = __enable_memory_tracking(FALSE);

   //try
   //{

//      pNewData = (string_data *) m_palloca->realloc(pOldData, nOldTotalSize, nNewTotalSize);
      string_data *   pNewData = (string_data *)memory_realloc(pOldData,nChars + 1 + sizeof(string_data));

   //}
   //catch(...)
   //{
   //}

   //__enable_memory_tracking(bEnable);


   //if(pNewData == NULL)
   //{
     // return NULL;
   //}

   pNewData->nAllocLength = nChars;

   return pNewData;
}

inline string_data * string_manager::GetNilString()
{
   m_nil.AddRef();
   return &m_nil;
}







































namespace str
{

   CLASS_DECL_AURA inline void copy(char * pszDest, const char * pszSrc) { strcpy(pszDest, pszSrc); }
   CLASS_DECL_AURA inline void copy(unichar * pszDest, const unichar * pszSrc) { wcscpy_dup(pszDest, pszSrc); }

} // namespace str




/*namespace core
{

   namespace international
   {

      CLASS_DECL_AURA strsize          UnicodeToMultiByteCount(UINT uiCodePage, const unichar * lpcsz);
      CLASS_DECL_AURA bool         UnicodeToMultiByte(UINT uiCodePage, char * lpstrMultiByte, strsize nCount, const unichar * lpcsz);
      CLASS_DECL_AURA const char * UnicodeToMultiByte(UINT uiCodePage, const unichar * lpcsz);
      CLASS_DECL_AURA
      inline const char * _001GetUtf8(const char * psz)
      {
         return _strdup(psz);
      }
      inline const char * _001GetUtf8(const unichar * psz)
      {
         return UnicodeToMultiByte(CP_UTF8, psz);
      }
   }
}*/


      struct string_data;

#if defined(SOLARIS)
      #include <atomic.h>
#if defined(OS64BIT)
   #define _gen_InterlockedIncrement(ptr) atomic_inc_64(ptr)
   #define _gen_InterlockedDecrement(ptr) atomic_dec_64(ptr)
#else
   #define _gen_InterlockedIncrement(ptr) atomic_inc_32((volatile uint32_t *) ptr)
   #define _gen_InterlockedDecrement(ptr) atomic_dec_32((volatile uint32_t *) ptr)
#endif
#elif defined(LINUX) || defined(APPLEOS) || defined(ANDROID)
   #define _gen_InterlockedIncrement(ptr) __sync_add_and_fetch(ptr, 1)
   #define _gen_InterlockedDecrement(ptr) __sync_sub_and_fetch(ptr, 1)
#else
   #ifdef _M_IX86
      #ifndef _M_CEE
         #define _gen_InterlockedIncrement InterlockedIncrement
         #define _gen_InterlockedDecrement InterlockedDecrement
      #else
         #define _gen_InterlockedIncrement InterlockedIncrement
         #define _gen_InterlockedDecrement InterlockedDecrement
         /* managed code must use the non-intrinsics */
         #ifdef InterlockedIncrement
            #undef InterlockedIncrement
         #endif
         #ifdef InterlockedDecrement
            #undef InterlockedDecrement
         #endif
      #endif  // !_M_CEE
   #else
      #define _gen_InterlockedIncrement InterlockedIncrement
      #define _gen_InterlockedDecrement InterlockedDecrement
   #endif  // _M_IX86_
#endif // ! LINUX
















template< const strsize t_nSize >
   class static_string
{
public:
static_string(const char* psz ) :
   m_psz( psz )
   {
   }

   operator const char*() const
   {
      return m_psz;
   }

   static strsize __cdecl get_length()
   {
      return (t_nSize/sizeof( char ))-1;
   }

private:
const char* m_psz;

private:
static_string( const static_string& str ) NOTHROW;
static_string& operator=( const static_string& str ) NOTHROW;
};








#define _ST( psz ) ::core::static_string< char, sizeof( _T( psz ) ) >( _T( psz ) )
#define _SA( psz ) ::core::static_string< char, sizeof( psz ) >( psz )
#define _SW( psz ) ::core::static_string< unichar, sizeof( L##psz ) >( L##psz )
#define _SO( psz ) _SW( psz )

class CLASS_DECL_AURA char_traits_base
{
public:
};



























      class string_file;

class CLASS_DECL_AURA simple_string
{
public:

   typedef string_manager manager;

   typedef char value_type;

   typedef string_data data_type;


   simple_string(for_moving)
   {
   }

   explicit simple_string(string_manager * pstringmanager )
   {
      construct(pstringmanager);
   }

   void construct(string_manager * pstringmanager);
   simple_string(const simple_string & strSrc, string_manager * pstringmanager  );
   simple_string(const string_data * pdata, string_manager * pstringmanager  );
   simple_string(const char * pszSrc,string_manager * pstringmanager );
   simple_string(const unichar * pszSrc,string_manager * pstringmanager);
   simple_string(const byte * pszSrc,string_manager * pstringmanager);
   simple_string(const char* pchSrc,strsize nLength,string_manager * pstringmanager );
   ~simple_string() NOTHROW
   {
      if(m_pszData != NULL)
      {
         string_data * pData = get_data();
         pData->Release();
      }
   }

   simple_string& operator=(const simple_string& strSrc )
   {
      string_data * pSrcData = strSrc.get_data();
      string_data * pOldData = get_data();
      if( pSrcData != pOldData)
      {
         if( pOldData->IsLocked() || pSrcData->pstringmanager != pOldData->pstringmanager )
         {
            SetString( strSrc.GetString(), strSrc.get_length() );
         }
         else
         {
            string_data * pNewData = CloneData( pSrcData );
            pOldData->Release();
            attach( pNewData );
         }
      }

      return *this;
   }


   simple_string& operator=(const char * pszSrc )
   {
      SetString( pszSrc );

      return *this;
   }

   void assign(const char * pszSrc)
   {
      SetString(pszSrc);
   }

   simple_string& operator+=(const simple_string& strSrc )
   {
      append( strSrc );

      return *this;
   }

   simple_string& operator+=(const char * pszSrc )
   {
      append( pszSrc );

      return *this;
   }
   template< strsize t_nSize >
      simple_string& operator+=(const static_string< t_nSize >& strSrc )
   {
      append( static_cast<const char *>(strSrc), strSrc.get_length() );

      return *this;
   }


   simple_string& operator+=(char ch )
   {

      append_char(ch );

      return *this;

   }


   simple_string& operator+=(uchar ch )
   {

      append_char( char( ch ) );

      return *this;

   }


   simple_string& operator+=(unichar ch )
   {

      append_char( char( ch ) );

      return *this;

   }


   // besides returning a reference (and const does not really impedes changing), do not change a simple_string (or string) directly,
   // there may be multiple instances of a string (all referencing the same pointer).
   inline const char & operator [](strsize iChar) const;

   // non error at
   inline char s_at(strsize iChar) const
   {

      return m_pszData[iChar];

   }

   inline operator const char *() const NOTHROW
   {
      return( m_pszData );
   }

#if defined(METROWIN) && defined(__cplusplus_winrt)
   inline operator String ^() const
   {
      return ref new String(wstring(*this));
   }
#endif

   void append(const char * pszSrc )
   {
      append( pszSrc, StringLength( pszSrc ) );
   }
   void append(const char * pszSrc,strsize nLength )
   {
      // See comment in SetString() about why we do this
      uint_ptr nOffset = pszSrc-GetString();

      strsize nOldLength = get_length();
      if (nOldLength < 0)
      {
         // protects from underflow
         nOldLength = 0;
      }
      strsize nNewLength = nOldLength+nLength;
      char * pszBuffer = GetBuffer( nNewLength );
      if( nOffset <= (uint_ptr) nOldLength )
      {
         pszSrc = pszBuffer+nOffset;
         // No need to call CopyCharsOverlapped, since the destination is
         // beyond the end of the original buffer
      }
#if _SECURE_TEMPLATE
      CopyChars( pszBuffer+nOldLength, nLength, pszSrc, nLength );
#else
      CopyChars( pszBuffer+nOldLength, pszSrc, nLength );
#endif
      ReleaseBufferSetLength( nNewLength );
   }

   void append_char(char ch)
   {

      strsize nOldLength = get_length();

      strsize nNewLength = nOldLength+1;

      char * pszBuffer = GetBuffer( nNewLength );

      pszBuffer[nOldLength] = ch;

      ReleaseBufferSetLength( nNewLength );

   }

   void append(strsize i, char ch)
   {

      if(i <= 0)
         return;

      strsize nOldLength = get_length();

      strsize nNewLength = nOldLength + i;

      char * pszBuffer = GetBuffer( nNewLength );

      memset(&pszBuffer[nOldLength], (int) ch, i);

      ReleaseBufferSetLength( nNewLength );

   }

   void append(const simple_string& strSrc )
   {

      append( strSrc.GetString(), strSrc.get_length() );

   }

   void Empty() NOTHROW
   {

      string_data * pOldData = get_data();

      string_manager * pstringmanager = pOldData->pstringmanager;

      if(pOldData->nDataLength == 0)
         return;

      if(pOldData->IsLocked())
      {
         // Don't reallocate a locked buffer that's shrinking
         set_length(0);
      }
      else
      {

         pOldData->Release();
         string_data* pNewData = pstringmanager->GetNilString();
         attach( pNewData );

      }

   }
   void free_extra()
   {
      string_data* pOldData = get_data();
      strsize nLength = pOldData->nDataLength;
      string_manager* pstringmanager = pOldData->pstringmanager;
      if( pOldData->nAllocLength == nLength )
      {
         return;
      }

      if( !pOldData->IsLocked() )  // Don't reallocate a locked buffer that's shrinking
      {
         string_data* pNewData = pstringmanager->allocate( nLength );
         if( pNewData == NULL )
         {
            set_length( nLength );
            return;
         }

#if _SECURE_TEMPLATE
         CopyChars( char *( pNewData->data() ), nLength, const char *( pOldData->data() ), nLength );
#else
         CopyChars( (char *)( pNewData->data() ), (const char *)( pOldData->data() ), nLength );
#endif

         pOldData->Release();
         attach( pNewData );
         set_length( nLength );
      }
   }

   strsize GetAllocLength() const NOTHROW
   {
      return( get_data()->nAllocLength );
   }

   inline char get_at(strsize iChar ) const;

   char * GetBuffer()
   {
      string_data* pData = get_data();
      if( pData->IsShared() )
      {
         Fork( pData->nDataLength );
      }

      return( m_pszData );
   }
   char * GetBuffer(strsize nMinBufferLength )
   {
      return( PrepareWrite( nMinBufferLength ) );
   }
   char * GetBufferSetLength(strsize nLength )
   {
      char * pszBuffer = GetBuffer( nLength );
      set_length( nLength );

      return( pszBuffer );
   }
   strsize get_length() const NOTHROW
   {
      return( get_data()->nDataLength );
   }
   strsize size() const NOTHROW
   {
      return( get_data()->nDataLength );
   }
   void get_string(char * psz) const NOTHROW
   {
      ::strcpy(psz, (const char *) *this);
   }
   void set_string(const char * psz) NOTHROW
   {
      SetString(psz);
   }
   string_manager * GetManager() const NOTHROW
   {
      string_manager * pstringmanager = get_data()->pstringmanager;
      return pstringmanager ? pstringmanager->Clone() : NULL;
   }

   const char * GetString() const NOTHROW
   {
      return( m_pszData );
   }

   bool is_empty() const NOTHROW
   {
      return *m_pszData == '\0';
   }

   bool empty() const NOTHROW
   {
      return *m_pszData == '\0';
   }

   bool has_char() const NOTHROW
   {
      return m_pszData != NULL && *m_pszData != '\0';
   }

   char * LockBuffer()
   {
      string_data* pData = get_data();
      if( pData->IsShared() )
      {
         Fork( pData->nDataLength );
         pData = get_data();  // Do it again, because the fork might have changed it
      }
      pData->lock();

      return( m_pszData );
   }
   void UnlockBuffer() NOTHROW
   {
      string_data* pData = get_data();
      pData->unlock();
   }
   void Preallocate(strsize nLength )
   {
      PrepareWrite( nLength );
   }
   void ReleaseBuffer(strsize nNewLength = -1 )
   {
      if( nNewLength == -1 )
      {
         nNewLength = StringLength( m_pszData );
      }
      set_length( nNewLength );
   }
   inline void ReleaseBufferSetLength(strsize nNewLength );
   inline void Truncate(strsize nNewLength );
inline   void set_at(strsize iChar,char ch );
   inline void SetManager(string_manager * pstringmanager );
   void SetString(const char * pszSrc )
   {
      SetString( pszSrc, StringLength( pszSrc ) );
   }
   inline void SetString(const char * pszSrc,strsize nLength );

public:
   friend simple_string operator+(
      const simple_string& str1,
      const simple_string& str2 )
   {
      simple_string s( str1.GetManager() );

      Concatenate( s, str1, str1.get_length(), str2, str2.get_length() );

      return( s );
   }

   friend simple_string operator+(
      const simple_string& str1,
      const char * psz2 )
   {
      simple_string s( str1.GetManager() );

      Concatenate( s, str1, str1.get_length(), psz2, StringLength( psz2 ) );

      return( s );
   }

   friend simple_string operator+(
      const char * psz1,
      const simple_string& str2 )
   {
      simple_string s( str2.GetManager() );

      Concatenate( s, psz1, StringLength( psz1 ), str2, str2.get_length() );

      return( s );
   }

   static void __cdecl CopyChars(char* pchDest,const char* pchSrc,strsize nChars ) NOTHROW
   {
      memcpy(pchDest, pchSrc, nChars * sizeof(char));
   }

#if _SECURE_TEMPLATE
   static void __cdecl CopyChars(char* pchDest,size_t nDestLen,const char* pchSrc,strsize nChars ) NOTHROW
   {
#ifdef WINDOWS
      memcpy_s(pchDest, nDestLen * sizeof(char), pchSrc, nChars * sizeof(char));
#else
      memcpy(pchDest, pchSrc, nChars * sizeof(char));
#endif
   }
#endif

   static void __cdecl CopyCharsOverlapped(char* pchDest,const char* pchSrc,strsize nChars ) NOTHROW
   {
      memmove( pchDest, pchSrc, nChars*sizeof( char ) );
   }

#if _SECURE_TEMPLATE
   static void __cdecl CopyCharsOverlapped(char* pchDest,size_t nDestLen,const char* pchSrc,strsize nChars ) NOTHROW
   {
#ifdef WINDOWS
      memmove_s(pchDest, nDestLen * sizeof(char), pchSrc, nChars * sizeof(char));
#else
      memmove(pchDest, pchSrc, nChars * sizeof(char));
#endif
   }
#endif

   inline static strsize __cdecl StringLength(const char * psz ) NOTHROW
   {
      return psz == NULL ? 0 : strlen(psz);
      //strsize nLength = 0;
      //if( psz != NULL )
      //{
      //   const char* pch = psz;
      //   while( *pch != 0 )
      //   {
      //      nLength++;
      //      pch++;
      //   }
      //}

      //return( nLength );
   }

protected:
   static void __cdecl Concatenate(simple_string& strResult, const char * psz1,strsize nLength1, const char * psz2,strsize nLength2 )
   {
      strsize nNewLength = nLength1+nLength2;
      char * pszBuffer = strResult.GetBuffer( nNewLength );
#if _SECURE_TEMPLATE
      CopyChars( pszBuffer, nLength1, psz1, nLength1 );
      CopyChars( pszBuffer+nLength1, nLength2, psz2, nLength2 );
#else
      CopyChars( pszBuffer, psz1, nLength1 );
      CopyChars( pszBuffer+nLength1, psz2, nLength2 );
#endif
      strResult.ReleaseBufferSetLength( nNewLength );
   }


   // Implementation
protected:
   void attach(string_data * pData ) NOTHROW
   {
      m_pszData = static_cast< char * >( pData->data() );
   }
   void Fork(strsize nLength )
   {
      string_data* pOldData = get_data();
      strsize nOldLength = pOldData->nDataLength;
      string_data* pNewData = pOldData->pstringmanager->Clone()->allocate( nLength );
      //if( pNewData == NULL )
      //{
        // throw_memory_exception();
      //}
      strsize nCharsToCopy = ((nOldLength < nLength) ? nOldLength : nLength)+1;  // copy '\0'
#if _SECURE_TEMPLATE
      CopyChars( char *( pNewData->data() ), nCharsToCopy, const char *( pOldData->data() ), nCharsToCopy );
#else
      CopyChars((char *) ( pNewData->data() ), (const char *) ( pOldData->data() ), nCharsToCopy );
#endif
      pNewData->nDataLength = nOldLength;
      pOldData->Release();
      attach( pNewData );
   }
   string_data * get_data() const NOTHROW
   {
      return( reinterpret_cast< string_data * >( m_pszData )-1 );
   }
   char * PrepareWrite(strsize nLength )
   {
      string_data * pOldData = get_data();
      long nShared = 1-pOldData->nRefs;  // nShared < 0 means true, >= 0 means false
      strsize nTooShort = pOldData->nAllocLength-nLength;  // nTooShort < 0 means true, >= 0 means false
      if( (nShared|nTooShort) < 0 )  // If either sign bit is set (i.e. either is less than zero), we need to copy data
      {
         PrepareWrite2( nLength );
      }

      return( m_pszData );
   }
   void PrepareWrite2(strsize nLength )
   {
      string_data * pOldData = get_data();
      if( pOldData->nDataLength > nLength )
      {
         nLength = pOldData->nDataLength;
      }
      if( pOldData->IsShared() )
      {
         Fork( nLength );
      }
      else if( pOldData->nAllocLength < nLength )
      {
         // Grow exponentially, until we hit 1K.
         strsize nNewLength = pOldData->nAllocLength;
         if( nNewLength > 1024 )
         {
            nNewLength += 1024;
         }
         else
         {
            nNewLength *= 2;
         }
         if( nNewLength < nLength )
         {
            nNewLength = nLength;
         }
         Reallocate( nNewLength );
      }
   }
   inline void Reallocate(strsize nLength );
   inline void set_length(strsize nLength )
   {

      //ASSERT(nLength < 0);
         //throw_error_exception("simple_string::set_length nLength < 0");
      ASSERT(nLength <= get_data()->nAllocLength);
      //throw_error_exception("simple_string::set_length nLength > get_data()->nAllocLength");

      get_data()->nDataLength = nLength;
      m_pszData[nLength] = 0;

   }

   static string_data * __cdecl CloneData(string_data * pData )
   {
      string_data * pNewData = NULL;

      string_manager * pNewStringMgr = pData->pstringmanager->Clone();
      if( !pData->IsLocked() && (pNewStringMgr == pData->pstringmanager) )
      {
         pNewData = pData;
         pNewData->AddRef();
      }
      else
      {
         pNewData = pNewStringMgr->allocate( pData->nDataLength);
         //if( pNewData == NULL )
         //{
         //   throw_memory_exception();
         //}
         pNewData->nDataLength = pData->nDataLength;
#if _SECURE_TEMPLATE
         CopyChars( char *( pNewData->data() ), pData->nDataLength+1, const char *( pData->data() ), pData->nDataLength+1 );  // copy '\0'
#else
         CopyChars((char *)( pNewData->data() ), (const char *)( pData->data() ), pData->nDataLength+1 );  // copy '\0'
#endif
      }

      return( pNewData );
   }

public :
   typedef string_file CStrBuf;
public:
   char * m_pszData;

};

class CLASS_DECL_AURA string_file
{
public:

   enum
   {
      AUTO_LENGTH = 0x01,  // Automatically determine the new length of the string at release.  The string must be NULL-terminated.
      SET_LENGTH = 0x02  // set the length of the string object at GetBuffer time
   };

public:
   explicit string_file(simple_string& str ) THROWS :
   m_str( str ),
      m_pszBuffer( NULL ),
      m_nLength( str.get_length() )
#ifdef DEBUG
      ,m_nBufferLength(str.get_length())
#endif
   {
      m_pszBuffer = m_str.GetBuffer();
   }

   string_file(simple_string & str,strsize nMinLength,uint32_t dwFlags = AUTO_LENGTH ) THROWS :
   m_str( str ),
      m_pszBuffer( NULL ),
      m_nLength( (dwFlags&AUTO_LENGTH) ? -1 : nMinLength )
#ifdef DEBUG
      ,m_nBufferLength(nMinLength)
#endif
   {
      if( dwFlags&SET_LENGTH )
      {
         m_pszBuffer = m_str.GetBufferSetLength( nMinLength );
      }
      else
      {
         m_pszBuffer = m_str.GetBuffer( nMinLength );
      }
   }

   ~string_file()
   {
      m_str.ReleaseBuffer( m_nLength );
   }

   operator char *() NOTHROW
   {
      return( m_pszBuffer );
   }
   operator const char *() const NOTHROW
   {
      return( m_pszBuffer );
   }

   inline void set_length(strsize nLength );

   // Implementation
private:
   simple_string & m_str;
   char * m_pszBuffer;
   strsize m_nLength;
#ifdef DEBUG
   strsize m_nBufferLength;
#endif

   // Private copy constructor and copy assignment operator to prevent accidental use
private:
   string_file( const string_file& ) NOTHROW;
   string_file& operator=( const string_file& ) NOTHROW;
};

///////////////////////////////////////////
// string_data inlining

inline char * string_data::data() NOTHROW
{
   return reinterpret_cast < char *> (this+1);
}





class string;






