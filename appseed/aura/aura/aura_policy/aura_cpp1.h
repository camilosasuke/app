/////////////////////////////////////////////////////////////////////////////////////////
//
//   All Pure and Aural ca2 and God and Gods and Goddssesses and devils's blessings
//
//   C++
//
//   C => in ca2 scope
//
//   Thank you God for C++!! Cheap and valuable!!
//

#pragma once

#ifdef WINDOWS
#pragma warning(disable: 4251)
#endif

#include "nodeapp/operational_system/operational_system.h"

// very short name ([{c}])ontext (switchers, as it as context) enums
enum e_context_switcher_null
{
   cnull
};


namespace aura
{


   class application;
   class system;
   class application_signal_details;

   class command;

   class str;
   class str_context;


} // namespace aura


namespace aura
{

   class allocatorsp;

   template < typename T >
   inline void del(T * & p)
   {

      //Thank you Fiora a Eterna!!

      //Fiora Aeterna☄ ‏@FioraAeterna 20m20 minutes ago

      //   maybe it's cynical but I'm starting to think the real reason so many newer games have constant autosaves is because they crash all the time
      //   Details

      // BRT 2015-02-18 19:08
      // catch all (...) here in aura::del ... but should remove try catch from all underlying calls (frees, memory_frees, memory_dbg_frees).

      try
      {

         if (p != NULL)
         {

            T * pdel = p;

            p = NULL;

            delete pdel;

         }

      }
      catch (...)
      {

      }

   }

   template < typename T >
   inline void adel(T * & p)
   {

      //Thank you Fiora a Eterna!!

      //Fiora Aeterna☄ ‏@FioraAeterna 20m20 minutes ago

      //   maybe it's cynical but I'm starting to think the real reason so many newer games have constant autosaves is because they crash all the time
      //   Details

      // BRT 2015-02-18 19:08
      // catch all (...) here in aura::del ... but should remove try catch from all underlying calls (frees, memory_frees, memory_dbg_frees).

      try
      {

         if (p != NULL)
         {

            delete[] p;

            p = NULL;

         }

      }
      catch (...)
      {

      }

   }


   template < typename T >
   inline void free(T * & p)
   {

      //Thank you Mummi!!

      //Nice late afternoon coffee of ever!!

      //Thank you Fiora a Eterna!!

      //Fiora Aeterna☄ ‏@FioraAeterna some minutes ago

      //   maybe it's cynical but I'm starting to think the real reason so many newer games have constant autosaves is because they crash all the time
      //   Details

      // BRT 2016-11-23 19:08
      // catch all (...) here in aura::del ... but should remove try catch from all underlying calls (frees, memory_frees, memory_dbg_frees).

      try
      {

         if (p != NULL)
         {

            void * pdel = (void *)p;

            p = NULL;

            ::free(pdel);

         }

      }
      catch (...)
      {

      }

   }

} // namespace aura



  // very short name ([{c}])ontext (switchers, as it as context) enums
enum e_context_switcher_empty
{
   cempty
};

// very short name ([{c}])ontext (switchers, as it as context) enums
enum e_context_switcher_no_exception
{
   no_exception
};

// very short name ([{c}])ontext (switchers, as it as context) enums
enum e_context_switcher_failed
{
   failure,
   failure_no_log
};


namespace aura
{

   enum e_setting
   {

      setting_none,
      setting_wallpaper,
      setting_app_activation

   };

} // namespace system



template<class T>
struct remove_reference
{
   typedef T TYPE;
};

template<class T>
struct remove_reference<T&>
{
   typedef T TYPE;
};

template<class T>
struct remove_reference<T&&>
{
   typedef T TYPE;
};

template<class T> inline
typename remove_reference<T>::TYPE&& move(T && t)
{

   return (static_cast<typename remove_reference<T>::TYPE&&>(t));

}



template < class T >
class smart_pointer;


#define sp(TYPE) ::smart_pointer < TYPE >




class string;
class istring;
class var;
class property;
class property_set;
class object;
class type;
class base_edit;

class timer_callback;


namespace aura
{

   class Timer;

};

class timer;

typedef bool FN_TIMER(timer * ptimer);
typedef FN_TIMER * PFN_TIMER;


//template < typename Type,typename RawType = typename Type >
//class string_array;
//typedef string_array < string > stringa;




namespace user
{

   class primitive;
   class frame;
   class menu;
   class document;

   class impact_system;
   class printer;
   class impact;
   class elemental;


} // namespace aura

#if defined(INSTALL_SUBSYSTEM)

namespace install
{

   class install;


} // namespace install

#endif


namespace data
{


   class listener;
   class data;
   class simple_data;
   class data_container_base;


} // namespace data

namespace sockets // only usable from base.dll and dependants
{

   class sockets; // only usable from base.dll and dependants


} // namespace sockets // only usable from base.dll and dependants


namespace url
{

   class department; // only usable from base.dll and dependants


} // namespace url


namespace axis // only usable from axis.dll and dependants
{

   class compress_department; // only usable from axis.dll and dependants


} // namespace axis // only usable from axis.dll and dependants

#ifdef DEBUG
  // Debug ASSERTs then throws. Retail throws if condition not met
#define ENSURE_THROW(cond, exception)   \
   do { int32_t _gen__condVal=!!(cond); ASSERT(_gen__condVal); if (!(_gen__condVal)){exception;} } while (false)
#define ENSURE(cond)      ENSURE_THROW(cond, throw invalid_argument_exception(get_thread_app()) )
#define ENSURE_ARG(cond)   ENSURE_THROW(cond, throw invalid_argument_exception(get_thread_app()) )
#endif


  // extern ::core::CTrace TRACE;
#ifdef DEBUG
#ifndef TRACE
#define TRACE ::aura::trace_add_file_and_line(m_pauraapp, __FILE__, __LINE__)
#define APPTRACE ::aura::trace_add_file_and_line(papp, __FILE__, __LINE__)
  //#define TRACE2 TRACE
#endif
  //#define VERIFY(f)          ASSERT(f)
  //#define DEBUG_ONLY(f)      (f)

  // The following trace macros are provided for backward compatiblity
  //  (they also take a fixed number of parameters which provides
  //   some amount of extra error checking)
#define TRACE0(sz)              TRACE("%s", (const char *) (sz))
#define TRACE1(sz, p1)          TRACE(sz, p1)
#define TRACE2(sz, p1, p2)      TRACE(sz, p1, p2)
#define TRACE3(sz, p1, p2, p3)  TRACE(sz, p1, p2, p3)

  // These __dump macros also provided for backward compatibility
#define __dump0(spgraphics, sz)   dumpcontext << _T(sz)
#define __dump1(spgraphics, sz, p1) dumpcontext << _T(sz) << p1


#define DEBUG_ONLY(f)      f

#else // DEBUG

#define DEBUG_ONLY(f)      

  //#define VERIFY(f)          ((void)(f))
  //#define DEBUG_ONLY(f)      ((void)0)
#pragma warning(push)
#pragma warning(disable : 4793)
inline void c_cdecl __trace(...) { }
#pragma warning(pop)
#if defined(APPLEOS) || defined(ANDROID)
#define TRACE              
#define APPTRACE           
#else
#define TRACE              __noop
#define APPTRACE           __noop
#endif
#define TRACE0(sz)
#define TRACE1(sz, p1)
#define TRACE2(sz, p1, p2)
#define TRACE3(sz, p1, p2, p3)

#endif // !DEBUG*/






#define ASSERT_POINTER(p, type) \
   ASSERT(((p) != NULL) && __is_valid_address((p), sizeof(type), FALSE))

#define ASSERT_NULL_OR_POINTER(p, type) \
   ASSERT(((p) == NULL) || __is_valid_address((p), sizeof(type), FALSE))


#if defined(__arm__)
#  define UNUSED_ALWAYS(x)
#elif defined(__GNUC__)
#  define UNUSED_ALWAYS(x) __attribute__((__unused__))
#elif defined(_MSC_VER)
#  define UNUSED_ALWAYS(x)
#else
#	define UNUSED_ALWAYS(x) x
#endif




#ifdef DEBUG
#define REPORT_EXCEPTION(pException, szMsg) \
   do { \
      string str; \
      if (pException->get_error_message(str, 0)) \
         TRACE(::aura::trace::category_AppMsg, 0, "%s (%s:%d)\n%s\n", szMsg, __FILE__, __LINE__, str); \
      else \
         TRACE(::aura::trace::category_AppMsg, 0, "%s (%s:%d)\n", szMsg, __FILE__, __LINE__); \
      ASSERT(FALSE); \
   } while (0)
#else
#define REPORT_EXCEPTION(pException, szMsg) \
   do { \
      string strMsg; \
      char  szErrorMessage[512]; \
      if (pException->get_error_message(szErrorMessage, sizeof(szErrorMessage)/sizeof(*szErrorMessage), 0)) \
         strMsg.Format("%s (%s:%d)\n%s", szMsg, __FILE__, __LINE__, szErrorMessage); \
      else \
         strMsg.Format("%s (%s:%d)", szMsg, __FILE__, __LINE__); \
      System.simple_message_box(strMsg); \
   } while (0)
#endif



#define EXCEPTION_IN_DTOR(pException) \
   do { \
      REPORT_EXCEPTION((pException), "Exception thrown in destructor"); \
      delete pException; \
   } while (0)

#define __BEGIN_DESTRUCTOR try {
#define __END_DESTRUCTOR   } catch (::exception::base *pException) { EXCEPTION_IN_DTOR(pException); }




#ifndef C_RUNTIME_ERRORCHECK_SPRINTF
#define C_RUNTIME_ERRORCHECK_SPRINTF(expr) \
do { \
   errno_t _saveErrno = errno; \
   errno = 0; \
   (expr); \
   if(0 != errno) \
   { \
      ::c_runtime_error_check(errno); \
   } \
   else \
   { \
      errno = _saveErrno; \
   } \
} while (0)
#endif // C_RUNTIME_ERRORCHECK_SPRINTF


CLASS_DECL_AURA errno_t c_runtime_error_check(errno_t error);
CLASS_DECL_AURA void __cdecl __clearerr_s(FILE *stream);


class dump_context;
//class string_interface;
class id_space;
class type;
class ptra;
class factory_item_base;
class fixed_alloc_no_sync;
class critical_section;
class var_array;
//class pair_set_interface;
//class str_str_interface;
class command_thread;
class thread;
class command_target;
class cmd_ui;
class critical_section;
class mutex;
class string;
class id;

namespace colorertake5
{

   class ParserFactory;


} // namespace colorertake5

namespace visual
{


   class icon;
   class cursor;


} // namespace visual


class memory;


//namespace plane
//{
//
//   class session;
//   class system;
//   //class cube;
//
//
//} // namespace plane

class cmd_ui;

namespace datetime
{


   class department;


} // namespace datetime


namespace draw2d
{


   class graphics;


} // namespace draw2d


namespace file
{

   class listing;
   class path;
   typedef CLASS_DECL_AURA::string_array < ::file::path, string > patha;
   class file;
   class istream;
   class ostream;
   class serializable;


} // namespace file


class machine_event_data;


namespace html
{

   class html;

} // namespace html


#include "aura/aura/aura/aura_auto.h"






class image_list;




struct SIZEPARENTPARAMS
{

   RECT rect;       // parent client rectangle (trim as appropriate)
   SIZE sizeTotal;  // total size on each side as on_layout proceeds
   bool bStretch;   // should stretch to fill all space

};


typedef struct RECTD RECTD;


namespace aura
{


   //#if defined METROWIN && defined(__cplusplus_winrt)
   //
   //   interface class system_window
   //   {
   //
   //      virtual Windows::Foundation::Rect get_window_rect() = 0;
   //      virtual Windows::Foundation::Point get_cursor_pos() = 0;
   //
   //
   //
   //   };
   //
   //   CLASS_DECL_AURA bool get_window_rect(system_window ^ pwindow,RECTD * lprect);
   //   CLASS_DECL_AURA bool get_window_rect(system_window ^ pwindow,LPRECT lprect);
   //
   //#endif


   class file;


   namespace trace
   {

      class trace;

   }

   class session;

} // namespace aura


  // only usable from axis and axis dependants
namespace axis
{

   class application;
   class session;
   class system;

}


// only usable from base and base dependants
namespace base
{

   class application;
   class session;
   class system;

}

// only usable from core and core dependants
namespace core
{

   class application;
   class session;
   class system;

}

typedef ::aura::system * CREATE_SYSTEM();
typedef CREATE_SYSTEM * LPFN_CREATE_SYSTEM;

CLASS_DECL_AURA extern LPFN_CREATE_SYSTEM g_pfn_create_system;
CLASS_DECL_AURA extern const char * g_pszCooperativeLevel;

namespace data
{


   class item;


} // namespace data


namespace xml
{

   class node;

} // namespace xml




enum e_extract
{

   extract_none,
   extract_first,
   extract_all

};


namespace str
{


   namespace international
   {


      class locale_schema;


   } // namespace international


} // namespace str




namespace draw2d
{


   class graphics;


} // namespace draw2d


class random_access_iterator { public: };


#ifdef WINDOWS

typedef HANDLE HTHREAD;

#else

#ifndef LINUX
typedef pthread_t HTHREAD;
#endif

#endif


class event;


class thread;




typedef smart_pointer < thread > thread_sp;


//class thread_impl;


//typedef smart_pointer < thread_impl > thread_impl_sp;


namespace html
{

   class html; // defined and set by html library/component

}



#define SCAST_PTR(TYPE, ptarget, psource) TYPE * ptarget = dynamic_cast < TYPE * > (psource);
#define SCAST_REF(TYPE, rtarget, psource) TYPE & rtarget = *(dynamic_cast < TYPE * > (psource))


#define CaSys(pca) (*pca->m_pauraapp->m_paurasystem)
#define Sys(pauraapp) (*pauraapp->m_paurasystem)
#define System (Sys(this->m_pauraapp))
#define threadSystem (Sys(get_thread_app()))


#define Sess(pauraapp) (*pauraapp->m_paurasession)
#define Session (Sess(m_pauraapp))


#undef App
#define App(pauraapp) (*pauraapp)
#define Application (App(m_pauraapp))


// return - result - if not ok
#ifndef RINOK
#define RINOK(x) { int32_t __result__ = (x); if (__result__ != 0) return __result__; }
#endif

// throw - exception - result exception - if not ok
#ifndef TINOK
#define TINOK(e, x) { int32_t __result__ = (x); if (__result__ != 0) throw new e(get_app(), __result__); }
#endif


#ifdef WINDOWS
#pragma warning(disable: 4251)  // using non-exported as public in exported
#endif


#ifndef WINDOWSEX

typedef void * HDWP;

#endif


typedef  void(*PFN_ca2_factory_exchange)(::aura::application * papp);

#ifdef WINDOWS
CLASS_DECL_AURA bool defer_co_initialize_ex(bool bMultiThread);
#endif

CLASS_DECL_AURA bool aura_init();
CLASS_DECL_AURA bool aura_term();

CLASS_DECL_AURA bool __node_aura_pre_init();
CLASS_DECL_AURA bool __node_aura_pos_init();

CLASS_DECL_AURA bool __node_aura_pre_term();
CLASS_DECL_AURA bool __node_aura_pos_term();


CLASS_DECL_AURA::aura::application * get_thread_app();

#include "aura/primitive/primitive_cflag.h"

#include "aura/aura/aura/aura_definition.h"

#include "aura/aura/aura/aura_action_source.h"

#include "aura/aura/aura/aura_new.h"

#include "aura/aura/aura/aura_lparam.h"
#include "aura/aura/aura/aura_muldiv64.h"



template <class TYPE>
inline bool is_null(const TYPE & ref)
{
   return &ref == NULL;
}

#define NULL_REF(class) (*((class *) NULL))




#include "aura/aura/aura/aura_pointer.h"

#include "aura/aura/aura/aura_smart_pointer1.h"
#include "aura/aura/aura/aura_smart_pointer2.h"


#include "aura/aura/aura/aura_keep_true.h"


namespace file
{


   using file_sp = sp(file);


} // namespace file


#include "aura/primitive/primitive_logic.h"



#include "aura/primitive/math/math_static_numeric_info.h"
#include "aura/primitive/math/math_numeric_info.h"
#include "aura/primitive/math/math_c_number.h"
  //#include "aura/primitive/math/math_complex.h"


#include "aura/primitive/datetime/datetime_duration.h"


#include "aura/primitive/str/str.h"



#include "aura/primitive/datetime/datetime_date_span.h"
#include "aura/primitive/datetime/datetime_time_span.h"
#include "aura/primitive/datetime/datetime_time.h"
#include "aura/primitive/datetime/datetime_zonetime.h"
#include "aura/primitive/datetime/datetime_file_time_span.h"
#include "aura/primitive/datetime/datetime_file_time.h"

#include "aura/primitive/enum.h"


#include "aura/aura/aura/aura_element.h"

#include "aura/aura/aura/aura_debug.h"


#include "aura/aura/aura_app_core.h"



#include "aura/aura/aura/aura_class.h"

#include "aura/primitive/comparison/comparison_md5.h"

#include "aura/aura/aura/aura_conv.h"

#include "aura/aura/aura/aura_core.h"

#include "aura/primitive/primitive_request_interface.h"

#include "aura/multithreading/multithreading_wait_result.h"

#include "aura/multithreading/multithreading_waitable.h"

#include "aura/primitive/primitive_object.h"


#include "aura/primitive/comparison/comparison.h"

#include "aura/net/net.h"


class object;
// Note: file names are still ANSI strings (filenames rarely need UNICODE)
CLASS_DECL_AURA void assert_valid_object(const object* pOb, const char * lpszFileName, int32_t nLine);
CLASS_DECL_AURA void __dump(const object* pOb); // dump an object from CodeView

#ifdef DEBUG
CLASS_DECL_AURA errno_t c_runtime_error_check(errno_t error);
#define C_RUNTIME_ERROR_CHECK(expr) ::c_runtime_error_check(expr)
// Debug ASSERT_VALIDs then throws. Retail throws if pOb is NULL
#define ENSURE_VALID_THROW(pOb, exception)   \
   do { ASSERT_VALID(pOb); if (!(pOb)){exception;} } while (false)
#define ENSURE_VALID(pOb)   ENSURE_VALID_THROW(pOb, throw invalid_argument_exception(get_thread_app()) )

#else
#define C_RUNTIME_ERROR_CHECK(expr) expr
#endif

#ifdef DEBUG
#	define UNUSED(x) x
#else
#  define UNUSED(x) UNUSED_ALWAYS(x)
#endif


#include "aura/aura/aura/aura_common.h"

#include "aura/primitive/collection/collection_common.h"

#include "aura/primitive/collection/collection_raw_array_decl.h"


#include "aura/primitive/collection/collection_lemon_heap.h"



#include "aura/filesystem/file/file_c.h"





#include "aura/aura/aura/aura_action_context.h"



#include "aura/aura/aura/aura_fixed_alloc.h"



#include "aura/aura/aura/aura_allocate.h"
#include "aura/aura/aura/aura_plex.h"


#include "aura/primitive/primitive_id.h"

#include "aura/primitive/str/str_interface.h"
#include "aura/primitive/str/str_composite.h"
#include "aura/primitive/str/str_base64.h"



#include "aura/primitive/primitive_type.h"

//#include "aura/aura/aura/aura_smart_pointer2.h"

CLASS_DECL_AURA int trace_hr(const char * psz, HRESULT hr);





CLASS_DECL_AURA string get_last_error_string();

CLASS_DECL_AURA string get_error_string(uint64_t uiError);

CLASS_DECL_AURA void set_aura(void * p, ::aura::application * papp);
CLASS_DECL_AURA::aura::application * get_aura(void * p);

CLASS_DECL_AURA string get_system_error_message(uint32_t dwError);

#ifdef DEBUG
#define DEBUG_NOTE __FILE__
#ifdef __MCRTDBG
#define AURA_NEW new
#else
#define AURA_NEW new(DEBUG_NOTE, __LINE__)
#endif
#define THREAD_NOTE __get_thread_note()
#define SET_THREAD_NOTE(x) __set_thread_note(x);
#else
#define SET_THREAD_NOTE(x)
#define AURA_NEW new
#endif



class CLASS_DECL_AURA c_class
{
public:


   static c_class s_cclass;


   c_class();
   c_class(const c_class &);
   virtual ~c_class();


};



#define return_(y, x) {y = x; return;}


extern "C"
{


   int32_t _c_lock_is_active(const char * pszName);
   int32_t _c_lock(const char * pszName, void ** pdata);
   int32_t _c_unlock(void ** pdata);


}


CLASS_DECL_AURA string _ca_get_file_name(const char * psz, bool bCreate = false, int32_t * pfd = NULL);

CLASS_DECL_AURA string get_system_error_message(uint32_t dwError);

#include "aura/filesystem/file/file_serializable.h"
#include "aura/aura/aura/aura_system_trace.h"

#include "aura/multithreading/multithreading_common.h"
#include "aura/multithreading/multithreading_thread_os.h"

#include "aura/aura/aura_policy/aura_cpp1a.h"


//#include "aura/primitive/collection/collection1.inl"
//#include "aura/primitive/collection/collection_array_impl.inl"
//#include "aura/primitive/collection/collection_raw_array_impl.inl"
//#include "aura/primitive/collection/collection_array_base.inl"
//#include "aura/primitive/str/str1.inl"
//#include "aura/aura/aura1.inl"


#ifdef VARIADIC_TEMPLATE_FORMAT2

template<typename T, typename... Args>
inline void string_format::format(const char * & s, const T & value, Args... args)
{

   while (*s)
   {

      if (*s == '%' && *(++s) != '%')
      {

         defer_get_additional_argument(s, value, args...);

         return;

      }

      append(*s++);

   }

   throw simple_exception(get_thread_app(), "extra arguments provided to format");


}




#endif



inline lparam::lparam(::object * p)
{
   p->add_ref();
   m_lparam = (LPARAM)p;
}


template<class TYPE>
void dump_elements(dump_context & dumpcontext, const TYPE* pElements, ::count nCount)
{
   ENSURE((nCount == 0) || (pElements != NULL));
   ASSERT((nCount == 0) ||
      __is_valid_address(pElements, (size_t)nCount * sizeof(TYPE), FALSE));
#ifdef WINDOWS
   &dumpcontext; // not used
   pElements;  // not used
   nCount; // not used
#endif
           // default does nothing
}


template<class TYPE>
inline void CopyElements(TYPE* pDest, const TYPE* pSrc, ::count nCount)
{
   ENSURE((nCount == 0) || ((pDest != 0) && (pSrc != 0)));
   ASSERT((nCount == 0) || __is_valid_address(pDest, (size_t)nCount * sizeof(TYPE)));
   ASSERT((nCount == 0) || __is_valid_address(pSrc, (size_t)nCount * sizeof(TYPE)));

   // default is element-copy using assignment
   while (nCount--)
      *pDest++ = *pSrc++;
}

#undef new

#if defined(LINUX) || defined(METROWIN) || defined(ANDROID)


inline void * __cdecl operator new(size_t nSize, void * p)
{

   UNREFERENCED_PARAMETER(nSize);

   return p;

}



inline void __cdecl operator delete(void * p, void * palloc) del_throw_spec
{

   UNREFERENCED_PARAMETER(p);
   UNREFERENCED_PARAMETER(palloc);

}


#endif






inline CLASS_DECL_AURA void * __cdecl operator new (size_t size, const c_class &)
{

   return memory_alloc(size);

}


inline CLASS_DECL_AURA void * __cdecl operator new[](size_t size, const c_class &)
{

   return memory_alloc(size);

}


#define C_NEW new(c_class::s_cclass)


inline void * __cdecl operator new(size_t nSize, const char * lpszFileName, int32_t nLine) new_throw_spec
{

   return ::operator new(nSize, _NORMAL_BLOCK, lpszFileName, nLine);

}


inline void * __cdecl operator new[](size_t nSize, const char * lpszFileName, int32_t nLine) new_throw_spec
{

   return ::operator new[](nSize, _NORMAL_BLOCK, lpszFileName, nLine);

}


inline void __cdecl operator delete(void * pData, const char * /* lpszFileName */, int32_t /* nLine */) del_throw_spec
{

   ::operator delete(pData, _NORMAL_BLOCK, NULL, -1);

}


inline void __cdecl operator delete[](void * pData, const char * /* lpszFileName */, int32_t /* nLine */) del_throw_spec
{

   ::operator delete(pData, _NORMAL_BLOCK, NULL, -1);

}


inline void * __cdecl operator new(size_t nSize, int32_t nType, const char * lpszFileName, int32_t nLine)
{

   return memory_alloc_dbg(nSize, nType, lpszFileName, nLine);

}


inline void __cdecl operator delete(void * p, int32_t nType, const char * /* lpszFileName */, int32_t /* nLine */)
{

   memory_free_dbg(p, nType);

}


inline void * __cdecl operator new[](size_t nSize, int32_t nType, const char * lpszFileName, int32_t nLine)
{

   return ::operator new(nSize, nType, lpszFileName, nLine);

}


inline void __cdecl operator delete[](void * p, int32_t nType, const char * lpszFileName, int32_t nLine)
{

   ::operator delete(p, nType, lpszFileName, nLine);

}
















namespace aura
{

   // impl
   template < class APP >
   sp(::aura::application) single_application_library < APP > ::get_new_application(const char * pszAppId)
   {

      if (!contains_app(pszAppId))
         return NULL;

      ::aura::application * papp = new APP();

      if (papp == NULL)
         return NULL;

      try
      {

         papp->construct(pszAppId);

      }
      catch (...)
      {

         ::aura::del(papp);

         return NULL;

      }

      return papp;

   }


} // namespace aura





  //void object::keep_alive()
  //{
  //   try
  //   {
  //      on_keep_alive();
  //   }
  //   catch(...)
  //   {
  //   }
  //}
  //
  //void on_keep_alive();
  //virtual bool is_alive();




inline bool IsDirSep(WCHAR ch)
{

   return (ch == '\\' || ch == '/');

}


namespace math
{

   template < typename T >
   class complex;


} // namespace math

namespace _std
{


   template <class T> void swap(T& a, T& b)
   {
      T c(a); a = b; b = c;
   }


}

namespace std
{

   using string = ::string;
   using wstring = ::wstring;


   using ostream = ::file::ostream;

   using istream = ::file::istream;



   template < typename T >
   using numeric_limits = ::numeric_info < T >;

   //      template <class T> void sort(T & t1,T & t2)
   //    {
   //     ::sort::sort < T >(t1,t2);
   //}

   template < class iterator, class COMPARE = ::comparison::less < typename iterator::BASE_TYPE, typename iterator::BASE_ARG_TYPE > >
   void sort(const iterator & a, const iterator & b)
   {
      ::sort::quick_sort < iterator, COMPARE >(a, b);
   }

   template < typename T >
   using auto_ptr = ::pointer < T >;


   using stdio_file = ::file::file;


   template < typename T >
   T abs(T t)
   {
      if (t < ::numeric_info< T >::null())
         return -t;
      else
         return t;
   }


   using streamsize = file_size_t;
   using streampos = file_position_t;
   using streamoff = file_offset_t;

   using setw = ::file::set_width;

#if !defined(WINDOWSEX) && !defined(APPLEOS) && !defined(METROWIN)

   using bad_alloc = ::memory_exception;

#endif

   template < typename T >
   using complex = ::math::complex < T >;


   template <class T> const T& min(const T& a, const T& b) { return !(a > b) ? a : b; }
   template <class T> const T& max(const T& a, const T& b) { return !(a < b) ? a : b; }

   template <class RandomAccessIterator>
   void make_heap(RandomAccessIterator first, RandomAccessIterator last)
   {
      ::lemon::make_heap(first, last);
   }

   template <class RandomAccessIterator, class Compare>
   void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
   {
      ::lemon::make_heap(first, last, comp);
   }

   template <class RandomAccessIterator, class Compare>
   void pop_heap(RandomAccessIterator first, RandomAccessIterator last)
   {
      ::lemon::pop_heap(first, last);
   }

   template <class RandomAccessIterator, class Compare>
   void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
   {
      ::lemon::pop_heap(first, last, comp);
   }

   template <class RandomAccessIterator, class Compare>
   void push_heap(RandomAccessIterator first, RandomAccessIterator last)
   {
      ::lemon::push_heap(first, last);
   }

   template <class RandomAccessIterator, class Compare>
   void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
   {
      ::lemon::push_heap(first, last, comp);
   }

   template <class RandomAccessIterator>
   void sort_heap(RandomAccessIterator first, RandomAccessIterator last)
   {
      ::lemon::sort_heap(first, last);
   }

   template <class RandomAccessIterator, class Compare>
   void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
   {
      ::lemon::sort_heap(first, last, comp);
   }



#if !defined(__APPLE__)

   template <class T> void swap(T& a, T& b)
   {
      _std::swap(a, b);
   }

#endif


}


template < typename T >
inline string & to_json(string & str, const T & value, bool bNewLine)
{

   return str = ::str::from(value);

}


CLASS_DECL_AURA void dappy(const char * psz);



#define new AURA_NEW






