#pragma once


CLASS_DECL_AURA string_manager * __get_string_manager();
CLASS_DECL_AURA int64_t strtoi(const char * psz);
CLASS_DECL_AURA int64_t strtoi(const unichar * psz);


#if defined(__APPLE)
#define noexcept 
#endif

template <typename T>
inline T FormatArgument(T value) noexcept
{
   return value;
}


inline UINT _gen_GetConversionACP()
{
   return CP_UTF8;
}

class string;


template < typename T >
inline void to_string(string & str, const T & t)
{

   t.to_string(str);

}

//template < typename STRINGABLE >
//inline string & to_string(string & str, const STRINGABLE * pstringable)
//{
//
//   return pstringable->to_string(str);
//
//}
//
//

//#ifdef __GNUC__
//template < typename STRINGALBLE >
//inline void to_string(string & str, const STRINGALBLE & stringable)
//{
//
//   stringable.to_string(str);
//
//}
//
//template < typename STRINGALBLE >
//inline void to_string(string & str, const STRINGALBLE * po)
//{
//
//   po->to_string(str);
//
//}
//#endif
#ifdef WINDOWS
   #define stricmp _stricmp
#endif


#undef new

/////////////////////////////////////////////////////////////////////////////
// Naming conventions:
//    The term "length" can be confusing when dealing with ANSI, Unicode, and
//    MBCS character sets, so this file will use the following naming
//    conventions to differentiate between the different meanings of
//    "length":
//
//    'byte Length' - Length of a buffer in bytes, regardless of character
//       size
//    'char Length' - Number of distinct characters in string.  For wide-
//       character strings, this is equivalent to half the 'byte Length'.
//       For ANSI strings, this is equivalent to the 'byte Length'.  For MBCS
//       strings, 'char Length' counts a lead-byte/trail-byte combination
//       as one character.
//    'Length' - When neither of the above terms is used, 'Length' refers to
//       length in XCHARs, which is equal to 'byte Length'/sizeof(char).
/////////////////////////////////////////////////////////////////////////////


#define IMPLEMENT_CONST_STRING_PTR(stringype, value, name) const const_fixed_string<stringype, sizeof(value)/sizeof(stringype::char)> _init##name ={    {NULL,      sizeof(value)/sizeof(stringype::char)-1,    sizeof(value)/sizeof(stringype::char)-1,    -1},         value   };   const stringype::char* const _value##name = _init##name.m_achData;   extern const stringype* const name = CONST_STRING_PTR(stringype, name);
#define DECLARE_CONST_STRING_PTR(stringype, name) extern const stringype* const name;
#define CONST_STRING_PTR(stringype, name) reinterpret_cast<const stringype* const>(&_value##name)


class crt_char_traits :
   public char_traits_base
{
public:

   static char*         __cdecl  CharNext(const char* p ) throw();
   static int32_t           __cdecl  IsDigit(const char * pch ) throw();
   static int32_t           __cdecl  IsSpace(const char * pch ) throw();
   static int32_t           __cdecl  StringCompare(const char * pszA,const char * pszB ) throw();
   static int32_t           __cdecl  StringCompareIgnore(const char * pszA,const char * pszB ) throw();
   static int32_t           __cdecl  StringCollate(const char * pszA,const char * pszB ) throw();
   static int32_t           __cdecl  StringCollateIgnore(const char * pszA,const char * pszB ) throw();
   static const char *  __cdecl  StringFindString(const char * pszBlock,const char * pszMatch ) throw();
   static char *        __cdecl  StringFindString(char * pszBlock,const char * pszMatch ) throw();
   static const char *  __cdecl  StringFindChar(const char * pszBlock,char chMatch ) throw();
   static const char *  __cdecl  StringFindCharRev(const char * psz,char ch, strsize iStart ) throw();
   static const char *  __cdecl  StringFindStrRev(const char * psz,const char * pszFind, strsize iStart ) throw();
   static const char *  __cdecl  StringScanSet(const char * pszBlock,const char * pszMatch ) throw();
   static strsize       __cdecl  StringSpanIncluding(const char * pszBlock,const char * pszSet ) throw();
   static strsize       __cdecl  StringSpanExcluding(const char * pszBlock,const char * pszSet ) throw();
   static char *        __cdecl  StringUppercase( char * psz ) throw();
   static char *        __cdecl  StringLowercase( char * psz ) throw();
   static char *        __cdecl  StringUppercase(char * psz,size_t size ) throw();
   static char *        __cdecl  StringLowercase(char * psz,size_t size ) throw();
   static char *        __cdecl  StringReverse( char * psz ) throw();
   static strsize       __cdecl  GetFormattedLength(const char * pszFormat, va_list args ) throw();
   static strsize       __cdecl  Format(char * pszBuffer,const char * pszFormat, va_list args ) throw();
   static strsize       __cdecl  Format(char * pszBuffer,size_t nlength,const char * pszFormat, va_list args ) throw();
   static strsize       __cdecl  GetcharLength(const char * pszSrc ) throw();
   static strsize       __cdecl  GetcharLength(const char * pszSrc, strsize nLength ) throw();
   static strsize       __cdecl  GetcharLength(const unichar * pszSource ) throw();
   static strsize       __cdecl  GetcharLength(const unichar * pszSource, strsize nLength ) throw();
   static strsize       __cdecl  GetcharLength(const unichar32 * pszSource) throw();
   static strsize       __cdecl  GetcharLength(const unichar32 * pszSource,strsize nLength) throw();
   static void          __cdecl  ConvertTochar(char * pszDest,strsize nDestLength, const char * pszSrc, strsize nSrcLength = -1 ) throw();
   static void          __cdecl  ConvertTochar(char * pszDest,strsize nDestLength, const unichar * pszSrc,strsize nSrcLength = -1) throw();
   static void          __cdecl  ConvertTochar(char * pszDest,strsize nDestLength,const unichar32 * pszSrc,strsize nSrcLength = -1) throw();
   static void                   ConvertToOem(char* pstrString) RELEASENOTHROW;
   static void                   ConvertToAnsi(char* pstrString) RELEASENOTHROW;
   static void                   ConvertToOem(char* pstrString,size_t size);
   static void                   ConvertToAnsi(char* pstrString,size_t size);
   static void          __cdecl  FloodCharacters(char ch,strsize nLength, char* pch ) throw();

#ifdef WINDOWS

   static BSTR          __cdecl  AllocSysString( const char* pchData, strsize nDataLength ) throw();
   static bool          __cdecl  ReAllocSysString( const char* pchData,BSTR* pbstr,strsize nDataLength ) throw();

#endif

   static uint32_t         __cdecl  FormatMessage(uint32_t dwFlags, LPCVOID pSource, uint32_t dwMessageID,uint32_t dwLanguageID, char * pszBuffer, uint32_t nSize, va_list* pArguments ) throw();
   static uint32_t         __cdecl  format_message(uint32_t dwFlags, LPCVOID pSource, uint32_t dwMessageID,uint32_t dwLanguageID, char * pszBuffer, uint32_t nSize, va_list* pArguments ) throw();
   static strsize       __cdecl  SafeStringLen( const char * psz ) throw();
   static strsize       __cdecl  SafeStringLen( const unichar * psz ) throw();
   static strsize       __cdecl  GetCharLen(const unichar* pch ) throw();
   static strsize       __cdecl  GetCharLen(const char* pch ) throw();
   static uint32_t         __cdecl  GetEnvironmentVariable(const char * pszVar, char * pszBuffer, uint32_t dwSize);

};





















extern CLASS_DECL_AURA string_manager * s_pstringmanager;


class string_trait :
   public crt_char_traits // iterator
{
public:

   inline static string_manager * GetDefaultManager() throw()
   {
      return s_pstringmanager;
   }

};


class fixed_alloc_array;




class CLASS_DECL_AURA string :
   public stdstring < simple_string >
#if defined(VARIADIC_TEMPLATE_FORMAT2)
   , public string_format_printer
#endif
{
public:

   static const int npos;

   //operator class string_composite ();
   //operator class string_composite const () const;
   operator const char *() const throw();
   const char * c_str() const throw();
   const char * data() const throw();

   inline string() throw();
   string(const string & strSrc);

   string(string && strSrc):
      stdstring< simple_string>(for_moving())
   {
      m_pszData = strSrc.m_pszData;
      strSrc.m_pszData = NULL;
   }

#ifdef METROWIN
   string(String ^ & str)
   {

      operator = (begin(str));

   }
#endif


   string(e_context_switcher_null):
      stdstring< simple_string>(string_trait::GetDefaultManager())
   {
   }

   string(e_context_switcher_empty):
      stdstring< simple_string>(string_trait::GetDefaultManager())
   {
   }

   string(const stdstring< simple_string> & strSrc):
      stdstring< simple_string>(strSrc,string_trait::GetDefaultManager())
   {
   }

   string(const simple_string & strSrc):
      stdstring< simple_string>(strSrc,string_trait::GetDefaultManager())
   {
   }


   string(const stdstring< verisimple_wstring> & strSrc):
      stdstring< simple_string>(strSrc,string_trait::GetDefaultManager())
   {
   }

   string(const verisimple_wstring & strSrc):
      stdstring< simple_string>(strSrc,string_trait::GetDefaultManager())
   {
   }

   void construct() throw();
   explicit string( string_manager * pstringmanager ) throw();
   static void __cdecl Construct( string* pstring );

   string(const char * pszSrc);
   string(const uchar * pszSrc);
   //string(char * pszSrc);
   //string(uchar * pszSrc);
   //string(unichar * pszSrc);
   string(const unichar * pszSrc);
   string(const unichar32 * pszSrc);
   string(const string & strSrc, strsize npos, strsize len = -1);

   string(const hstring & hstr) : stdstring< simple_string>(hstr.operator const char *(),string_trait::GetDefaultManager()) {  }
   string(const hwstring & hwstr) : stdstring< simple_string>(hwstr.operator const unichar *(),string_trait::GetDefaultManager()) {  }

   string(const istring & istr);

   string(char ch,strsize nLength = 1);
   string(unichar ch, strsize nLength = 1 );

   string(strsize nLength, char ch);
   string(const char* pch,strsize nLength);
   string(const unichar* pch,strsize nLength);
   string(const unichar32* pch,strsize nLength);

#if defined(METROWIN) && defined(__cplusplus_winrt)
   string(Object ^ o);
   string(Array < byte > ^ a);
#endif

//#ifdef __GNUC__
   string(const var & var);
#ifdef __GNUC__
   string(const property & prop);
#endif
   string(const id & id);



   string(const char * pszSrc,string_manager * pstringmanager);
   string(const unichar * pszSrc,string_manager * pstringmanager);
   string(const uchar* pszSrc,string_manager * pstringmanager);
   string(const char* pch,strsize nLength,string_manager * pstringmanager);
   string(const unichar* pch,strsize nLength,string_manager * pstringmanager);

#if defined(METROWIN) && defined(__cplusplus_winrt)
   string(Object ^ o, string_manager * pstringmanager);
#endif

   ~string() throw();



   //inline string & operator = (const id & str);
   //inline string & operator = (const var & var);
   //inline string & operator = (const property & prop);

   //string & operator = (const string_interface & str );
   ////string & operator = (string strSrc);
   string & operator = (const simple_string & strSrc);
   string & operator = (const string & strSrc);
   string & operator = (string && strSrc);
   string & operator = (const char * pszSrc);
   string & operator = (const unichar * pszSrc);
   string & operator = (const unichar32 * pszSrc);
   string & operator = (const uchar* pszSrc);
   string & operator = (char * pszSrc);
   string & operator = (unichar * pszSrc);
   string & operator = (unichar32 * pszSrc);
   string & operator = (uchar* pszSrc);
   string & operator = (char ch);

#if defined(METROWIN) && defined(__cplusplus_winrt)
   string & operator = (String ^ & str);
#endif

   string & operator = (unichar ch);

   // Assignment operators
   template < typename T >
   inline string & operator =(const T & o);
   //template < typename T >
   //inline string & operator = (T & o);
   //template < typename T >
   //inline string & operator = (T * o);

   string& operator+=(const simple_string& str );
   string& operator+=(const char * pszSrc );
   template< strsize t_nSize >
   string& operator+=(const static_string<t_nSize >& strSrc );
   string& operator+=(const unichar * pszSrc );
   string& operator+=(char ch );
   string& operator+=(uchar ch );
   string& operator+=(unichar ch );
   string& operator+=(const id & id);

   // Override from aura class
   string_manager * GetManager() const throw();


   void push_back(char ch);

   string & assign(const string & str);
   string & assign(const string & str, strsize pos, strsize n);
   string & assign(const char * s, strsize n);
   string & assign(const char * s);
   string & assign(strsize n, char ca);
   string & assign(int64_t n, int64_t ca);
   string & assign(int32_t n, int32_t ca);
   string & assign(uint32_t n, uint32_t ca);
   string & assign(uint64_t n, uint64_t ca);
   template <class InputIterator>
   string & assign (InputIterator first, InputIterator last);


   int32_t compare(const char * psz ) const;
   int32_t compare_ci(const char * psz ) const throw();
   int32_t collate(const char * psz ) const throw();
   int32_t collate_ci(const char * psz ) const throw();

   int32_t compare(strsize iStart, strsize iCount, const char * psz ) const;
   int32_t compare_ci(strsize iStart, strsize iCount, const char * psz ) const;
   int32_t collate(strsize iStart, strsize iCount, const char * psz ) const;
   int32_t collate_ci(strsize iStart, strsize iCount, const char * psz ) const;

   int32_t compare(strsize iStart, strsize iCount, const char * psz, strsize iStart2, strsize iCount2) const;
   int32_t compare_ci(strsize iStart, strsize iCount, const char * psz, strsize iStart2, strsize iCount2) const;
   int32_t collate(strsize iStart, strsize iCount, const char * psz, strsize iStart2, strsize iCount2) const;
   int32_t collate_ci(strsize iStart, strsize iCount, const char * psz, strsize iStart2, strsize iCount2) const;

   bool contains(char ch, strsize start = 0, strsize count = -1);
   bool contains(unichar wch, strsize start = 0, strsize count = -1);
   bool contains(int32_t i, strsize start = 0, strsize count = -1); // utf8 char index
   bool contains(const char * psz, strsize start = 0, strsize count = -1);
   bool contains(const string & str, strsize start = 0, strsize count = -1);

   bool contains_ci(char ch, strsize start = 0, strsize count = -1);
   bool contains_ci(unichar wch, strsize start = 0, strsize count = -1);
   bool contains_ci(int32_t i, strsize start = 0, strsize count = -1); // utf8 char index
   bool contains_ci(const char * psz, strsize start = 0, strsize count = -1);
   bool contains_ci(const string & str, strsize start = 0, strsize count = -1);

   bool contains_wci(char ch, strsize start = 0, strsize count = -1);
   bool contains_wci(unichar wch, strsize start = 0, strsize count = -1);
   bool contains_wci(int32_t i, strsize start = 0, strsize count = -1); // utf8 char index
   bool contains_wci(const char * psz, strsize start = 0, strsize count = -1);
   bool contains_wci(const string & str, strsize start = 0, strsize count = -1);
   // Advanced manipulation


   void reserve(strsize res_arg = 0);

   // Delete 'nCount' characters, starting at index 'iIndex'
   strsize Delete(strsize iIndex, strsize nCount = 1);
   strsize remove(strsize iIndex, strsize nCount = 1);

   string & erase(strsize start = 0, strsize count = -1);


   // Insert character 'ch' before index 'iIndex'
   strsize Insert(strsize iIndex,char ch );

   // Insert string 'psz' before index 'iIndex'
   strsize Insert(strsize iIndex,const char * psz );

   // replace all occurrences of character 'chOld' with character 'chNew'
   strsize replace(char chOld,char chNew, strsize iStart = 0);

   // replace all occurrences of string 'pszOld' with string 'pszNew'
   strsize replace(const char * pszOld,const char * pszNew, strsize iStart = 0);

   strsize replace_ci(const char * pszOld,const char * pszNew, strsize iStart = 0);

   ::count utf8_replace(const char * pszOld,const char * pszNew, strsize iStart = 0);

   string & replace(strsize iStart, strsize nCount, const char * psz);
   string & replace(strsize iStart,strsize nCount,const char * psz, strsize nLen);

   // remove all occurrences of character 'chRemove'
   strsize remove(char chRemove );

   string Tokenize(const char * pszTokens, strsize& iStart ) const;
   // find routines

   // find the first occurrence of character 'ch', starting at index 'iStart'
   strsize find(char ch, strsize start = 0, strsize count = -1) const RELEASENOTHROW;
   strsize find_ci(char ch, strsize start = 0, strsize count = -1) const RELEASENOTHROW;

   strsize find_skip_or_length(char ch, strsize start = 0) const RELEASENOTHROW
   {

      while (true)
      {

         if (m_pszData[start] == '\0')
         {

            return start;

         }

         if (m_pszData[start] == ch)
         {

            start++;

            return start;

         }

         start++;

      }

   }

   strsize find_whitespace_or_length(strsize start = 0) const RELEASENOTHROW;

   // look for a specific sub-string

   // find the first occurrence of string 'pszSub', starting at index 'iStart'
   strsize find(const char * pszSub,strsize start = 0,strsize count = -1,const char ** pszTail = NULL) const RELEASENOTHROW;
   strsize find_w(const char * pszSub,strsize start = 0,strsize count = -1,const char ** pszTail = NULL) const RELEASENOTHROW;
   strsize find_ci(const char * pszSub,strsize start = 0,strsize count = -1,const char ** pszTail = NULL) const RELEASENOTHROW;
   strsize find_wci(const char * pszSub, strsize start = 0, strsize count = -1, const char ** pszTail = NULL) const RELEASENOTHROW;

   // find the first occurrence of string 'pszSub', starting at index 'iStart', if found returns the index of first character after the end of the found string
   strsize find_tail(const char * pszSub,strsize start = 0,strsize count = -1) const RELEASENOTHROW;
   strsize find_w_tail(const char * pszSub,strsize start = 0,strsize count = -1) const RELEASENOTHROW;
   strsize find_ci_tail(const char * pszSub,strsize start = 0,strsize count = -1) const RELEASENOTHROW;
   strsize find_wci_tail(const char * pszSub,strsize start = 0,strsize count = -1) const RELEASENOTHROW;

   // find the first occurrence of any of the characters in string 'pszCharSet'
   strsize FindOneOf(const char * pszCharSet, strsize iStart = 0, strsize n = -1 ) const RELEASENOTHROW;

   strsize find_first_in(const string & str, strsize pos = 0) const RELEASENOTHROW;
   strsize find_first_in(const char * s, strsize pos, strsize n) const RELEASENOTHROW;
   strsize find_first_in(const char * s, strsize pos = 0) const RELEASENOTHROW;
   strsize find_first_in(char ca, strsize pos = 0) const RELEASENOTHROW;

   strsize find_first_not_in(const string & str, strsize pos = 0) const RELEASENOTHROW;
   strsize find_first_not_in(const char * s, strsize pos, strsize n) const RELEASENOTHROW;
   strsize find_first_not_in(const char * s, strsize pos = 0) const RELEASENOTHROW;
   strsize find_first_not_in(char ca, strsize pos = 0) const RELEASENOTHROW;

   strsize find_last_not_in(const string & str, strsize pos = -1) const RELEASENOTHROW;
   strsize find_last_not_in(const char * s, strsize pos, strsize n) const RELEASENOTHROW;
   strsize find_last_not_in(const char * s, strsize pos = -1) const RELEASENOTHROW;
   strsize find_last_not_in(char ca, strsize pos = -1) const RELEASENOTHROW;

   strsize find_last_in(const string & str, strsize pos = -1) const RELEASENOTHROW;
   strsize find_last_in(const char * s, strsize pos, strsize n) const RELEASENOTHROW;
   strsize find_last_in(const char * s, strsize pos = -1) const RELEASENOTHROW;
   strsize find_last_in(char ca, strsize pos = -1) const RELEASENOTHROW;

   strsize find_first_of(const string & str, strsize pos = 0) const RELEASENOTHROW;
   strsize find_first_of(const char * s, strsize pos, strsize n) const RELEASENOTHROW;
   strsize find_first_of(const char * s, strsize pos = 0) const RELEASENOTHROW;
   strsize find_first_of(char ca, strsize pos = 0) const RELEASENOTHROW;

   strsize find_first_not_of(const string & str, strsize pos = 0) const RELEASENOTHROW;
   strsize find_first_not_of(const char * s, strsize pos, strsize n) const RELEASENOTHROW;
   strsize find_first_not_of(const char * s, strsize pos = 0) const RELEASENOTHROW;
   strsize find_first_not_of(char ca, strsize pos = 0) const RELEASENOTHROW;

   strsize find_last_not_of(const string & str, strsize pos = -1) const RELEASENOTHROW;
   strsize find_last_not_of(const char * s, strsize pos, strsize n) const RELEASENOTHROW;
   strsize find_last_not_of(const char * s, strsize pos = -1) const RELEASENOTHROW;
   strsize find_last_not_of(char ca, strsize pos = -1) const RELEASENOTHROW;

   strsize find_last_of(const string & str, strsize pos = -1) const RELEASENOTHROW;
   strsize find_last_of(const char * s, strsize pos, strsize n) const RELEASENOTHROW;
   strsize find_last_of(const char * s, strsize pos = -1) const RELEASENOTHROW;
   strsize find_last_of(char ca, strsize pos = -1) const RELEASENOTHROW;

   // find the last occurrence of character 'ch'
   strsize reverse_find(char ch, strsize iStart = -1 ) const RELEASENOTHROW;

   // find the last occurrence of string 'sz'
   strsize reverse_find(const char * sz,strsize iStart = -1) const RELEASENOTHROW;

   strsize rfind(char ch,strsize iStart = -1) const RELEASENOTHROW {
      return reverse_find(ch,iStart);
   };
   strsize rfind(const char * sz,strsize iStart = -1) const RELEASENOTHROW {
      return reverse_find(sz,iStart);
   };


   string str() const
   {
      return *this;
   }


   inline bool ends_ci(const string & strSuffixCandidate) const;

   inline bool ensure_ends_ci(const string & strSuffix);

   bool begins_ci(const char * s) const;


   char last_char() const;

   // manipulation

   // Convert the string to uppercase
   string& make_upper();

   // Convert the string to lowercase
   string& make_lower();

   // Reverse the string
   string& MakeReverse();

   string lower() const;

   string upper() const;


   // Convert the string to uppercase
   string uppered() const;

   // Convert the string to lowercase
   string lowered() const;

   // Convert the string to lowercase
   string reversed() const;

   // trimming

   // remove all trailing whitespace
   string& trim_right();

   // remove all leading whitespace
   string& trim_left();

   // remove all leading and trailing whitespace
   string& trim();

   // remove all leading and trailing occurrences of character 'chTarget'
   string& trim(char chTarget );

   // remove all leading and trailing occurrences of any of the characters in the string 'pszTargets'
   string& trim(const char * pszTargets );

   // trimming anything (either side)

   // remove all trailing occurrences of character 'chTarget'
   string& trim_right(char chTarget );

   // remove all trailing occurrences of any of the characters in string 'pszTargets'
   string& trim_right(const char * pszTargets );

   // remove all leading occurrences of character 'chTarget'
   string& trim_left(char chTarget );

   // remove all leading occurrences of any of the characters in string 'pszTargets'
   string& trim_left(const char * pszTargets );


   // remove all trailing whitespace
   string right_trimmed() const;

   // remove all leading whitespace
   string left_trimmed() const;

   // remove all leading and trailing whitespace
   string trimmed() const;

   // remove all leading and trailing occurrences of character 'chTarget'
   string trimmed(char chTarget) const;

   // remove all leading and trailing occurrences of any of the characters in the string 'pszTargets'
   string trimmed(const char * pszTargets) const;

   // trimming anything (either side)

   // remove all trailing occurrences of character 'chTarget'
   string right_trimmed(char chTarget) const;

   // remove all trailing occurrences of any of the characters in string 'pszTargets'
   string right_trimmed(const char * pszTargets) const;

   // remove all leading occurrences of character 'chTarget'
   string left_trimmed(char chTarget) const;

   // remove all leading occurrences of any of the characters in string 'pszTargets'
   string left_trimmed(const char * pszTargets) const;


   // Convert the string to the OEM character set
   void AnsiToOem();

   // Convert the string to the ANSI character set
   void OemToAnsi();

   // Very simple sub-string extraction

   // Return the substring starting at index 'iFirst'
   string Mid(strsize iFirst ) const;

   string substr(strsize iFirst) const;

   // Return the substring starting at index 'iFirst', with length 'nCount'
   string Mid(strsize iFirst,strsize nCount ) const;

   string substr(strsize iFirst, strsize nCount) const;

   string utf8_substr(strsize iFirst) const;

   string utf8_substr(strsize iFirst, strsize nCount) const;

   strsize utf8_get_length() const;

   inline strsize utf8_length() const;

   void clear();

   strsize length() const;

   // Return the substring consisting of the rightmost 'nCount' characters
   string Right(strsize nCount ) const;

   // Return the substring consisting of the leftmost 'nCount' characters
   string Left(strsize nCount ) const;

   // Return the substring consisting of the leftmost characters in the set 'pszCharSet'
   string SpanIncluding(const char * pszCharSet ) const;

   // Return the substring consisting of the leftmost characters not in the set 'pszCharSet'
   string SpanExcluding(const char * pszCharSet ) const;

   // Format data using format string 'pszFormat'

   // these overloads are not needed for the variadic template to compile
   // they keep the function consistent with single argument (no extra variadic arguments)
   // e.g. str.Format("Print this: Hello Format!!");
   inline void Format(const char *s)
   {

      assign(s);

   }

   inline void AppendFormat(const char *s)
   {

      append(s);

   }

#ifdef VARIADIC_TEMPLATE_FORMAT2

   template<typename T, typename... Args>
   void Format(const char *s, const T & value, Args... args)
   {

      ::string_format format;

      format.format(s, value, args...);

      assign(format.m_pszBuffer, format.m_iLength);

   }

   template<typename T, typename... Args>
   void AppendFormat(const char *s, const T & value, Args... args)
   {

      string str;

      ::string_format format;

      format.format(s, value, args...);

      append(format.m_pszBuffer, format.m_iLength);

   }


#elif defined(VARIADIC_TEMPLATE_FORMAT)


   template <typename ... Args>
   void Format(char const * const format, Args && ... args) noexcept
   {
      _Format(format, FormatArgument(args) ...);
   }



   void __cdecl _Format(const char * pszFormat, ...);

   void __cdecl _AppendFormat(const char *pszFormat, ...);



#else


   void __cdecl Format(const char * pszFormat, ... );

   void __cdecl AppendFormat(const char *pszFormat, ...);




#endif

   void AppendFormatV(const char * pszFormat, va_list args );

   void FormatV(const char * pszFormat, va_list args );

   void __cdecl FormatMessage(const char * pszFormat, ... );

   void __cdecl format_message(const char * pszFormat, ... );

   // Format a message using format string 'pszFormat' and va_list
   void FormatMessageV(const char * pszFormat, va_list* pArgList );

#ifdef WINDOWS

   // OLE BSTR support

   // allocate a BSTR containing a copy of the string
   BSTR AllocSysString() const;

   BSTR SetSysString(BSTR* pbstr ) const;

#endif

   // set the string to the value of environment var 'pszVar'
   bool GetEnvironmentVariable(const char * pszVar);

   // set the string to the value of environment var 'pszVar'
   bool getenv(const char * pszVar);

   // Load the string from resource 'nID'
   bool load_string(::aura::application * papp, id id);

   // Load the string from resource 'nID' in module 'hInstance'
   /*    bool load_string(HINSTANCE hInstance,strsize nID )
   {
   const STRINGRESOURCEIMAGE* pImage = gen_GetStringResourceImage( hInstance, nID );
   if( pImage == NULL )
   {
   return( FALSE );
   }

   strsize nLength = str_traits::GetcharLength( pImage->achString, pImage->nLength );
   char * pszBuffer = GetBuffer( nLength );
   str_traits::ConvertTochar( pszBuffer, nLength, pImage->achString, pImage->nLength );
   ReleaseBufferSetLength( nLength );

   return( TRUE );
   }*/

   // Load the string from resource 'nID' in module 'hInstance', using language 'wLanguageID'
   /*bool load_string(HINSTANCE hInstance,strsize nID,WORD wLanguageID )
   {
   const STRINGRESOURCEIMAGE* pImage = gen_GetStringResourceImage( hInstance, nID, wLanguageID );
   if( pImage == NULL )
   {
   return( FALSE );
   }

   strsize nLength = str_traits::GetcharLength( pImage->achString, pImage->nLength );
   char * pszBuffer = GetBuffer( nLength );
   str_traits::ConvertTochar( pszBuffer, nLength, pImage->achString, pImage->nLength );
   ReleaseBufferSetLength( nLength );

   return( TRUE );
   }*/

   void resize (strsize n)
   {
      resize(n, '\0');
   }

   void resize (strsize n, char c)
   {

      strsize nOldSize = length();

      if(n < nOldSize)
      {
         Truncate(n);
      }
      else
      {
         LPSTR lpsz = GetBufferSetLength(n);
         memset(&lpsz[nOldSize], c, n - nOldSize);
         ReleaseBuffer(n);
      }

   }


   friend string CLASS_DECL_AURA operator+(const string & str1,const string & str2 );
   friend string CLASS_DECL_AURA operator+(const string & str1,const char * psz2 );
   friend string CLASS_DECL_AURA operator+(const char * psz1,const string & str2 );
   friend string CLASS_DECL_AURA operator+(const string & str1,unichar ch2 );
   friend string CLASS_DECL_AURA operator+(unichar ch1,const string & str2 );
   friend string CLASS_DECL_AURA operator+(const string & str1,char ch2 );
   friend string CLASS_DECL_AURA operator+(char ch1,const string & str2 );
   friend string CLASS_DECL_AURA operator+(const string & str1,int32_t ch2 );
   friend string CLASS_DECL_AURA operator+(int32_t ch1,const string & str2 );
   friend string CLASS_DECL_AURA operator+(const string & str1,int64_t ch2 );
   friend string CLASS_DECL_AURA operator+(int64_t ch1,const string & str2 );
   friend string CLASS_DECL_AURA operator+(const var & var, const char * psz);
   friend string CLASS_DECL_AURA operator+(const char * psz, const var & var);
   friend string CLASS_DECL_AURA operator+(const var & var, const string & str);
   friend string CLASS_DECL_AURA operator+(const string & str, const var & var);



   //bool operator==(const string_interface & str) const;
   bool operator==(const string & str) const;
   bool operator==(const char * psz2) const;
   bool operator==(const unichar * psz2) const;
   bool operator==(char psz2) const;
   bool operator==(unichar psz2) const;
   bool operator==(int32_t i) const;

   //bool operator>(const string_interface & str) const;
   bool operator>(const string & str) const;
   bool operator>(const char * psz2) const;
   bool operator>(const unichar * psz2) const;
   bool operator>(char psz2) const;
   bool operator>(unichar psz2) const;
   bool operator>(int32_t i) const;

//   bool operator<(const string_interface & str) const;
   bool operator<(const string & str) const;
   bool operator<(const char * psz2) const;
   bool operator<(const unichar * psz2) const;
   bool operator<(char psz2) const;
   bool operator<(unichar psz2) const;
   bool operator<(int32_t i) const;

   //inline bool operator!=(const string_interface & str)   const { return !operator ==(str); }
   inline bool operator!=(const string & str )            const {
      return !operator ==(str);
   }
   inline bool operator!=(const char * psz)                     const {
      return !operator ==(psz);
   }
   inline bool operator!=(const unichar * psz)                     const {
      return !operator ==(psz);
   }
   inline bool operator!=(char ch)                       const {
      return !operator ==(ch);
   }
   inline bool operator!=(unichar ch)                       const {
      return !operator ==(ch);
   }
   inline bool operator!=(int32_t i)                      const {
      return !operator ==(i);
   }

   //inline bool operator>=(const string_interface & str)   const { return !operator <(str); }
   inline bool operator>=(const string & str )            const {
      return !operator <(str);
   }
   inline bool operator>=(const char * psz)                     const {
      return !operator <(psz);
   }
   inline bool operator>=(const unichar * psz)                     const {
      return !operator <(psz);
   }
   inline bool operator>=(char ch)                       const {
      return !operator <(ch);
   }
   inline bool operator>=(unichar ch)                       const {
      return !operator <(ch);
   }
   inline bool operator>=(int32_t i)                      const {
      return !operator <(i);
   }

   //inline bool operator<=(const string_interface & str)   const { return !operator >(str); }
   inline bool operator<=(const string & str )            const {
      return !operator >(str);
   }
   inline bool operator<=(const char * psz)                     const {
      return !operator >(psz);
   }
   inline bool operator<=(const unichar * psz)                     const {
      return !operator >(psz);
   }
   inline bool operator<=(char ch)                       const {
      return !operator >(ch);
   }
   inline bool operator<=(unichar ch)                       const {
      return !operator >(ch);
   }
   inline bool operator<=(int32_t i)                      const {
      return !operator >(i);
   }


   typedef strsize size_type;


   inline static string empty_string()
   {
      return string();
   }



#if defined(METROWIN) && defined(__cplusplus_winrt)
   inline operator String ^() const
   {
      return ref new String(wstring(*this));
   }
   inline operator String ^()
   {
      return ref new String(wstring(*this));
   }
#endif



};





template < typename T >
inline string & string::operator = (const T & o)
{

   to_string(*this, o);

   return *this;

}


//template < typename T >
//inline string & string::operator = (T & o)
//{
//
//   to_string(*this,  o);
//
//   return *this;
//
//}
//
//template < typename T >
//inline string & string::operator = (T * po)
//{
//
//   to_string(*this,  po);
//
//   return *this;
//
//}

template< strsize t_nSize >
string& string::operator+=(const static_string<t_nSize >& strSrc )
{
   simple_string::operator+=( strSrc );

   return *this;
}

inline string & string::operator=(const simple_string& strSrc )
{
   simple_string::operator=( strSrc );

   return *this;
}


template < class InputIterator >
string & string::assign ( InputIterator first, InputIterator last )
{

   Empty();

   for(InputIterator it = first; it <= last; it++)
   {

      *this += *it;

   }

   return *this;

}


inline strsize string::find_first_in(const string & str, strsize pos) const RELEASENOTHROW
{
   return find_first_of(str, pos);
}

inline strsize string::find_first_in(const char * s, strsize pos, strsize n) const RELEASENOTHROW
{
   return find_first_of(s, pos, n);
}

inline strsize string::find_first_in(const char * s, strsize pos) const RELEASENOTHROW
{
   return find_first_of(s, pos);
}

inline strsize string::find_first_in(char ca, strsize pos) const RELEASENOTHROW
{
   return find_first_of(ca, pos);
}

inline strsize string::find_first_not_in(const string & str, strsize pos) const RELEASENOTHROW
{
   return find_first_not_of(str, pos);
}

inline strsize string::find_first_not_in(const char * s, strsize pos, strsize n) const RELEASENOTHROW
{
   return find_first_not_of(s, pos, n);
}

inline strsize string::find_first_not_in(const char * s, strsize pos) const RELEASENOTHROW
{
   return find_first_not_of(s, pos);
}

inline strsize string::find_first_not_in(char ca, strsize pos) const RELEASENOTHROW
{
   return find_first_not_of(ca, pos);
}

inline strsize string::find_last_not_in(const string & str, strsize pos) const RELEASENOTHROW
{
   return find_first_not_of(str, pos);
}

inline strsize string::find_last_not_in(const char * s, strsize pos, strsize n) const RELEASENOTHROW
{
   return find_last_not_of(s, pos, n);
}

inline strsize string::find_last_not_in(const char * s, strsize pos) const RELEASENOTHROW
{
   return find_last_not_of(s, pos);
}

inline strsize string::find_last_not_in(char ca, strsize pos) const RELEASENOTHROW
{
   return find_last_not_of(ca, pos);
}

inline strsize string::find_last_in(const string & str, strsize pos) const RELEASENOTHROW
{
   return find_last_of(str, pos);
}

inline strsize string::find_last_in(const char * s, strsize pos, strsize n) const RELEASENOTHROW
{
   return find_last_of(s, pos, n);
}

inline strsize string::find_last_in(const char * s, strsize pos) const RELEASENOTHROW
{
   return find_last_of(s, pos);
}

inline strsize string::find_last_in(char ca, strsize pos) const RELEASENOTHROW
{
   return find_last_of(ca, pos);
}

inline strsize string::length() const
{
   return get_length();
}

inline strsize string::utf8_length() const
{
   return utf8_get_length();
}














#define BAD_WCHAR ((unichar)(0xFFFF))

/// macro - number of elements in array
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(*(a)))

#include "aura/primitive/str/x/x_charcategory.h"
#include "aura/primitive/primitive_bit.h"
#include "aura/primitive/collection/collection_bit_array.h"



























//inline string::string(const id & id) :
//   stdstring<simple_string>(string_trait::GetDefaultManager())
//{
//
//   operator = (id.to_string());
//
//}
//
//inline string & string::operator = (const id & id)
//{
//
//   return operator = (id.m_psz);
//
//}


inline   string::string() throw() :
   stdstring<simple_string>(string_trait::GetDefaultManager())
{
}





inline int32_t __cdecl crt_char_traits::StringCompare(const char * pszA,const char * pszB ) throw()
{

   return strcmp( reinterpret_cast< const  char* >( pszA ), reinterpret_cast< const  char* >( pszB ) );

}


inline int32_t __cdecl crt_char_traits::StringCompareIgnore(const char * pszA,const char * pszB ) throw()
{

   return stricmp(reinterpret_cast< const  char* >( pszA ), reinterpret_cast< const  char* >( pszB ) );

}


inline int32_t string::compare(const char * psz ) const
{
   
   return( string_trait::StringCompare( GetString(), psz ) );

}


inline int32_t string::compare_ci(const char * psz ) const throw()
{
   
   return( string_trait::StringCompareIgnore( GetString(), psz ) );

}


inline string CLASS_DECL_AURA operator+(const string & str1,const string & str2 )
{
   string strResult( str1.GetManager() );

   string::Concatenate( strResult, str1, str1.get_length(), str2, str2.get_length() );

   return( strResult );
}

inline   string CLASS_DECL_AURA operator+(const string & str1,const char * psz2 )
{
   string strResult( str1.GetManager() );

   string::Concatenate( strResult, str1, str1.get_length(), psz2, string::StringLength( psz2 ) );

   return( strResult );
}

inline   string CLASS_DECL_AURA operator+(const char * psz1,const string & str2 )
{
   string strResult( str2.GetManager() );

   string::Concatenate( strResult, psz1, string::StringLength( psz1 ), str2, str2.get_length() );

   return( strResult );
}

inline   string CLASS_DECL_AURA operator+(const string & str1,unichar ch2 )
{
   string strResult( str1.GetManager() );
   char chTemp = char( ch2 );

   string::Concatenate( strResult, str1, str1.get_length(), &chTemp, 1 );

   return( strResult );
}

inline   string CLASS_DECL_AURA operator+(const string & str1,char ch2 )
{
   string strResult( str1.GetManager() );
   char chTemp = char( ch2 );

   string::Concatenate( strResult, str1, str1.get_length(), &chTemp, 1 );

   return( strResult );
}

inline   string CLASS_DECL_AURA operator+(unichar ch1,const string & str2 )
{
   string strResult( str2.GetManager() );
   char chTemp = char( ch1 );

   string::Concatenate( strResult, &chTemp, 1, str2, str2.get_length() );

   return( strResult );
}

inline   string CLASS_DECL_AURA operator+(char ch1,const string & str2 )
{
   string strResult( str2.GetManager() );
   char chTemp = char( ch1 );

   string::Concatenate( strResult, &chTemp, 1, str2, str2.get_length() );

   return( strResult );
}


inline   string CLASS_DECL_AURA operator+ (const string & str1, int32_t i2)
{

   string strResult( str1.GetManager() );

   strResult = str1 + ::str::from(i2);

   return strResult;
}

inline   string CLASS_DECL_AURA operator+ (int32_t i1, const string & str2)
{

   string strResult( str2.GetManager() );

   strResult = ::str::from(i1) + str2;

   return strResult;
}

inline   string CLASS_DECL_AURA operator+ (const string & str1, int64_t i2)
{

   string strResult( str1.GetManager() );

   strResult = str1 + ::str::from(i2);

   return strResult;
}

inline   string CLASS_DECL_AURA operator+ (int64_t i1, const string & str2)
{

   string strResult( str2.GetManager() );

   strResult = ::str::from(i1) + str2;

   return strResult;
}



inline bool string::operator==(const string & str) const
{

   return get_length() == str.get_length() && compare(str.m_pszData) == 0;

}


inline bool string::operator==(const char * psz) const
{

   return strcmp(m_pszData, psz) == 0;

}


inline bool string::operator==(const unichar * psz) const
{

   return operator == (string(psz));

}


inline bool string::operator==(char ch) const
{

   return get_length() == 1 && operator[](0) == ch;

}


inline bool string::operator==(unichar ch) const
{
#ifdef __GNUC__
   return operator==(string(ch, 1));
#else
   return operator==(string(ch));
#endif

}


inline bool string::operator==(int32_t i) const
{

   return strtoi(*this) == i;

}


inline bool string::operator<(const string & str) const
{

   return compare(str) < 0;

}

inline bool string::operator<(const char * psz) const
{

   return operator < (string(psz));

}


inline bool string::operator<(const unichar * psz) const
{

   return operator < (string(psz));

}


inline bool string::operator<(char ch) const
{

#ifdef __GNUC__
   return operator < (string(ch,1)) ;
#else
   return operator < (string(ch)) ;
#endif

}


inline bool string::operator<(unichar ch) const
{
#ifdef __GNUC__
   return operator < (string(ch,1)) ;
#else
   return operator < (string(ch));
#endif

}


inline bool string::operator<(int32_t i) const
{

   return strtoi(*this) < i;

}


inline bool string::operator>(const string & str) const
{

   return compare(str) > 0 ;

}


inline bool string::operator>(const char * psz) const
{

   return operator > (string(psz));

}


inline bool string::operator>(const unichar * psz) const
{

   return operator > (string(psz));

}


inline bool string::operator>(char ch) const
{
#ifdef __GNUC__
   return operator > (string(ch, 1));
#else
   return operator > (string(ch));
#endif

}


inline bool string::operator>(unichar ch) const
{
#ifdef __GNUC__
   return operator > (string(ch, 1));
#else
   return operator > (string(ch));
#endif
}


inline bool string::operator>(int32_t i) const
{

   return strtoi(*this) > i;

}





inline string::operator const char *() const throw()
{
   return simple_string::operator const char *();
}


inline const char * string::c_str() const throw()
{
   return simple_string::operator const char *();
}

inline const char * string::data() const throw()
{
   return simple_string::operator const char *();
}

inline strsize string::remove(strsize iIndex,strsize nCount)
{
   return Delete(iIndex, nCount);
}


//inline bool CLASS_DECL_AURA operator==(const string_interface & str1   , const string & str2)  { return str2 == str1; }
inline bool CLASS_DECL_AURA operator==(const char *  psz                     , const string & str )  {
   return str  == psz ;
}
inline bool CLASS_DECL_AURA operator==(const unichar *  psz                     , const string & str )  {
   return str  == psz ;
}
inline bool CLASS_DECL_AURA operator==(char   ch                      , const string & str )  {
   return str  == ch  ;
}
inline bool CLASS_DECL_AURA operator==(unichar   ch                      , const string & str )  {
   return str  == ch  ;
}
inline bool CLASS_DECL_AURA operator==(int32_t i                       , const string & str )  {
   return str  == i   ;
}

//inline bool CLASS_DECL_AURA operator>(const string_interface & str1   , const string & str2 )   { return str2 < str1; }
inline bool CLASS_DECL_AURA operator>(const char * psz                      , const string & str  )   {
   return str  < psz ;
}
inline bool CLASS_DECL_AURA operator>(const unichar * psz                      , const string & str  )   {
   return str  < psz ;
}
inline bool CLASS_DECL_AURA operator>(char ch                        , const string & str  )   {
   return str  < ch  ;
}
inline bool CLASS_DECL_AURA operator>(unichar ch                        , const string & str  )   {
   return str  < ch  ;
}
inline bool CLASS_DECL_AURA operator>(int32_t i                       , const string & str  )   {
   return str  < i   ;
}

//inline bool CLASS_DECL_AURA operator<(const string_interface & str1   , const string & str2 )   { return str2 > str1; }
inline bool CLASS_DECL_AURA operator<(const char * psz                      , const string & str  )   {
   return str  > psz ;
}
inline bool CLASS_DECL_AURA operator<(const unichar * psz                      , const string & str  )   {
   return str  > psz ;
}
inline bool CLASS_DECL_AURA operator<(char ch                        , const string & str  )   {
   return str  > ch  ;
}
inline bool CLASS_DECL_AURA operator<(unichar ch                        , const string & str  )   {
   return str  > ch  ;
}
inline bool CLASS_DECL_AURA operator<(int32_t i                       , const string & str  )   {
   return str  > i   ;
}

//inline bool CLASS_DECL_AURA operator!=(const string_interface & str1,const string & str2)  { return !::operator==(str1, str2); }
inline bool CLASS_DECL_AURA operator!=(const char * psz,const string & str)                      {
   return !::operator==(psz, str);
}
inline bool CLASS_DECL_AURA operator!=(const unichar * psz,const string & str)                      {
   return !::operator==(psz, str);
}
inline bool CLASS_DECL_AURA operator!=(char ch,const string & str)                        {
   return !::operator==(ch, str);
}
inline bool CLASS_DECL_AURA operator!=(unichar ch,const string & str)                        {
   return !::operator==(ch, str);
}
inline bool CLASS_DECL_AURA operator!=(int32_t i, const string & str)                      {
   return !::operator==(i, str);
}

//inline bool CLASS_DECL_AURA operator>=(const string_interface & str1,const string & str2)  { return !::operator<(str1, str2); }
inline bool CLASS_DECL_AURA operator>=(const char * psz,const string & str)                      {
   return !::operator<(psz, str);
}
inline bool CLASS_DECL_AURA operator>=(const unichar * psz,const string & str)                      {
   return !::operator<(psz, str);
}
inline bool CLASS_DECL_AURA operator>=(char ch,const string & str)                        {
   return !::operator<(ch, str);
}
inline bool CLASS_DECL_AURA operator>=(unichar ch,const string & str)                        {
   return !::operator<(ch, str);
}
inline bool CLASS_DECL_AURA operator>=(int32_t i, const string & str)                      {
   return !::operator<(i, str);
}

//inline bool CLASS_DECL_AURA operator<=(const string_interface & str1,const string & str2)  { return !::operator>(str1, str2); }
inline bool CLASS_DECL_AURA operator<=(const char * psz,const string & str)                      {
   return !::operator>(psz, str);
}
inline bool CLASS_DECL_AURA operator<=(const unichar * psz,const string & str)                      {
   return !::operator>(psz, str);
}
inline bool CLASS_DECL_AURA operator<=(char ch,const string & str)                        {
   return !::operator>(ch, str);
}
inline bool CLASS_DECL_AURA operator<=(unichar ch,const string & str)                        {
   return !::operator>(ch, str);
}
inline bool CLASS_DECL_AURA operator<=(int32_t i, const string & str)                      {
   return !::operator>(i, str);
}


//template < >
//inline string & to_string(string & str, const string & strSrc)
//{
//
//   str.assign(strSrc);
//
//   return str;
//
//}

template < >
inline void to_string(string & str, const string & strSrc)
{

   str.assign(strSrc);

}



//template < >
//inline string & to_string(string & str,  char const * & psz)
//{
//
//   return str = psz;
//
//}
//
//template < >
//inline string & to_string(string & str, char * psz)
//{
//
//   str.assign(psz);
//
//   return str;
//
//}

//template < >
//inline string & to_string(string & str,const unsigned char * & psz)
//{
//
//   str.assign((const char *) psz);
//
//   return str;
//
//}

//template < >
//inline string & to_string(string & str, const char & ch)
//{
//
//   return str = ch;
//
//}
//
//template < >
//inline string & to_string(string & str, const unichar *& pwsz)
//{
//
//   return str = pwsz;
//
//}
//


//#if defined(METROWIN) && defined(__cplusplus_winrt)
//
//
//template < >
//inline void to_string(string & str, const String ^ & strSrc)
//{
//
//   str= (const unichar *) wstring(begin(strSrc));
//
//   //return str;
//
//}
//
//
//#endif







namespace str
{

   inline CLASS_DECL_AURA bool ends_eat(string & str, const char * lpcszSuffix)
   {

      strsize iLen = strlen(lpcszSuffix);

      if(iLen > str.length())
         return false;

      if(strncmp(&str[str.length() - iLen], lpcszSuffix, iLen) != 0)
         return false;

      str.Truncate(str.length() - iLen);

      return true;

   }

} // namespace str


inline CLASS_DECL_AURA string operator + (const id & id, const char * psz)
{

   return string(id) + psz;

}



inline CLASS_DECL_AURA string operator + (const char * psz, const id & id)
{

   return psz + string(id);

}


inline CLASS_DECL_AURA string operator + (const string & str, const id & id)
{

   return str + string(id);

}





inline string string::lower() const
{

   return string(*this).make_lower();

}

inline string string::upper() const
{

   return string(*this).make_upper();

}







//template < typename STRINGALBLE >
//inline string to_string(STRINGALBLE & stringable)
//{
//
//   string str;
//
//   to_string(str, stringable);
//
//   return str;
//
//}
//
//template < typename STRINGALBLE >
//inline string to_string(STRINGALBLE * pstringable)
//{
//
//   string str;
//
//   to_string(str, pstringable);
//
//   return str;
//
//}
//
//






inline const char * FormatArgument (const string & value) noexcept
{
   return value.c_str();
}





inline string & string::operator = (char * pszSrc)
{

   return operator =((const char *)pszSrc);

}


inline string & string::operator = (unichar * pszSrc)
{

   return operator =((const unichar *)pszSrc);

}


inline string & string::operator = (unichar32 * pszSrc)
{

   return operator =((const unichar32 *)pszSrc);

}


inline string & string::operator = (uchar* pszSrc)
{

   return operator =((const uchar *)pszSrc);

}
