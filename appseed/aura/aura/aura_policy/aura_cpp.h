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

   class interactive;

} // namespace user


namespace aura
{


   class application;
   class system;
   class application_signal_details;

   class command;

   class str;
   class str_context;


} // namespace aura


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
   class canvas;


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
   typedef CLASS_DECL_AURA ::string_array < ::file::path,string > patha;
   class file;
   class istream;
   class ostream;
   class serializable;


} // namespace file


class machine_event_data;

#ifdef HOTPLUGIN_SUBSYSTEM

namespace hotplugin
{
   class host;
   class plugin;
}

#endif



namespace html
{

   class html;

} // namespace html


#include "aura/aura/aura/aura_auto.h"



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

         if(p != NULL)
         {

            T * pdel = p;

            p = NULL;

            delete pdel;

         }

      }
      catch(...)
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

         if(p != NULL)
         {

            delete[] p;

            p = NULL;

         }

      }
      catch(...)
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

            void * pdel = (void *) p;

            p = NULL;

            ::free(pdel);

         }

      }
      catch (...)
      {

      }

   }

} // namespace aura



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
CLASS_DECL_AURA extern int g_iDerivedApplication; // one-based-index of derived application, zero if main application


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

#define AppUser(pauraapp) (*pauraapp->m_paurasession->fontopus()->get_user())
#define ApplicationUser (AppUser(m_pauraapp))


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


CLASS_DECL_AURA ::aura::application * get_thread_app();

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


#include "aura/primitive/primitive_object.h"

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


#include "aura/aura/xml/xml_exportable.h"
#include "aura/aura/xml/xml_importable.h"


#include "aura/primitive/comparison/comparison.h"


#include "aura/primitive/collection/collection_decl.h"
#include "aura/primitive/collection/collection_array_decl.h"
#include "aura/primitive/collection/collection_raw_array_decl.h"
#include "aura/primitive/collection/collection_lemon_array_decl.h"
#include "aura/primitive/collection/collection_smart_pointer_array.h"
#include "aura/primitive/collection/collection_comparable_eq_array.h"
#include "aura/primitive/collection/collection_comparable_array.h"
#include "aura/primitive/collection/collection_comparable_raw_array.h"


#include "aura/primitive/collection/collection_ptr_array.h"


#include "aura/primitive/collection/collection_lemon_heap.h"


#include "aura/aura/aura/aura_action_context.h"



#include "aura/aura/aura/aura_fixed_alloc.h"



#include "aura/aura/aura/aura_allocate.h"
#include "aura/aura/aura/aura_plex.h"


#include "aura/primitive/primitive_id.h"

#include "aura/primitive/str/str_interface.h"
#include "aura/primitive/str/str_composite.h"
#include "aura/primitive/str/str_base64.h"



#include "aura/aura/exception/exception.h"
#include "aura/aura/aura/aura_common.h"
#include "aura/filesystem/filesystem/filesystem_path.h"





#include "aura/aura/aura/aura_library.h"






#include "aura/filesystem/filesystem.h"





#include "aura/primitive/geometry/geometry.h"


#include "aura/primitive/collection/collection.h"


#include "aura/primitive/geometry/geometry_box.h"


#include "aura/aura/aura/aura_cmp.h"


#include "aura/filesystem/file/file_string_file.h"


#include "aura/primitive/str/str_format.h"


#include "aura/primitive/primitive_var2.h"


#include "aura/primitive/primitive_id_space.h"


#include "aura/primitive/math/math_scalar_enum.h"


#include "aura/primitive/math/math_scalar.h"


#include "aura/aura/aura/aura_progress.h"


#include "aura/aura/aura/aura_enum.h"


#include "aura/multithreading/multithreading.h"


#include "aura/aura/aura/aura_draw2d.h"


CLASS_DECL_AURA mutex & draw2d_mutex();


#include "aura/aura/aura/aura_gudo.h"




//#include "aura/user/user_create_context.h"


#if defined(LINUX)

#include "aura/os/ansios/ansios.h"

#elif defined(METROWIN)

#include "aura/os/metrowin/metrowin.h"
#include "aura/os/metrowin/metrowin_user_impl.h"

#elif defined(MACOS)

#include "aura/os/ansios/ansios.h"
#include "aura/os/macos/macos.h"

#elif defined(ANDROID)

#include "aura/os/ansios/ansios.h"
#include "aura/os/android/android.h"

#elif defined(WINDOWS)

#include "aura/aura/os/windows/windows.h"

#elif defined(APPLE_IOS)

#include "aura/os/ansios/ansios.h"
#include "aura/os/ios/ios.h"
//#include "aura/os/ios/ios_windowing.h"

#elif defined(SOLARIS)

#include "aura/os/ansios/ansios.h"
#include "aura/os/solaris/solaris_user_impl.h"

#else


#error "not implemented!!"

#endif





class CLASS_DECL_AURA openweather_city
{
public:

   index    m_iIndex;
   string   m_strCnt;
   string   m_strCit;
   int64_t  m_iId;
   double   m_dLat;
   double   m_dLon;

};




CLASS_DECL_AURA string get_system_error_message(uint32_t dwError);


#include "aura/aura/aura/aura_plex_heap.h"

#include "aura/primitive/primitive_type.h"


//#include "user/user/user_enum.h"
//#include "user/user/user_keyboard_focus.h"
//#include "user/user/user_mouse_focus.h"
//#include "user/user/user_elemental.h"
#include "aura/aura/aura/aura_flags.h"
#include "aura/primitive/primitive_check.h"
//#include "user/user/user_check_interface.h"
#include "aura/aura/aura/aura_command.h"
#include "aura/aura/aura/aura_command_msg.h"
#include "aura/aura/aura/aura_command_ui.h"
#include "aura/aura/aura/aura_command_ui_test.h"
//#include "graphics/visual/visual_const.h"
//#include "user/user/user_key_enum.h"
#include "aura/aura/message/message.h"
//#include "aura/user/user/user_check_interface.h"
#include "aura/aura/aura/aura_command_target.h"
//#include "user/user/user_schema.h"
//#include "user/user/user_schema_simple_impl.h"
//#include "user/user/user_schema_layered_frame.h"
//#include "user/user/user_text_interface.h"
//#include "user/user/user_draw_interface.h"
//#include "user/user/user_window_util.h"
//#include "user/user/user_interaction_base.h"
//#include "user/user/user_buffer.h"
#include "aura/aura/aura/aura_keep.h"
#include "aura/aura/aura/aura_restore.h"
#include "aura/aura/aura/aura_md5.h"
#include "aura/filesystem/file/file_stream2.h"
//#include "user/user/user_interaction.h"
//#include "user/user/user_interaction_impl_base.h"
//#include "user/user/user_interaction_child.h"
//#include "user/user/user_interaction_impl.h"
//#include "user/user/user_control_bar.h"
//#include "user/user/user_wait_cursor.h"
//#include "user/user/user_frame_window.h"
//#include "user/user/user_server.h"
//#include "user/user/user_impact_system.h"





#include "aura/primitive/primitive_interlocked_long.h"
#include "aura/primitive/primitive_interlocked_long_pulse.h"

#include "aura/primitive/data/data_data.h"
#include "aura/primitive/data/data_data_listener.h"
#include "aura/primitive/data/data_data_container.h"
#include "aura/primitive/data/data_item.h"
//#include "aura/primitive/data/data_tree_item.h"
//#include "aura/primitive/data/data_tree.h"
//#include "aura/primitive/data/data_simple_item.h"







//----------




#include "aura/primitive/math/math_objects.h"




//----------





#include "aura/aura/aura/aura_simple_log.h"


#include "aura/primitive/collection/collection_file_path_map.h"

#include "aura/primitive/primitive_edit.h"
#include "aura/aura/aura/aura_department.h"
#include "aura/aura/aura/aura_department_container.h"
#include "aura/aura/xml/xml.h"

#include "aura/aura/aura/aura_live_signal.h"
#include "aura/multithreading/multithreading_thread.h"
#include "aura/multithreading/multithreading_forking_thread.h"
#include "aura/multithreading/multithreading_delay_thread.h"
#include "aura/multithreading/multithreading_tools.h"
#include "aura/aura/aura_timer_callback.h"
#include "aura/aura/aura/aura_timer_item.h"
#include "aura/aura/aura/aura_timer_array.h"
#include "aura/aura/aura_timer.h"
#include "aura/aura/aura_timer_event.h"

#include "aura/multithreading/multithreading_thread_impl.h"
#include "aura/multithreading/multithreading_simple_thread.h"
#include "aura/multithreading/multithreading_go_thread.h"
#include "aura/multithreading/multithreading_signal_thread.h"


#include "aura/primitive/primitive_job.h"

#include "aura/user/simple/simple_message_box.h"

#include "aura/aura/aura/aura_user.h"

#include "aura/aura/aura/aura_log.h"
#include "aura/aura/aura/aura_trace.h"



#include "aura/primitive/primitive_memory_base.h"
#include "aura/primitive/primitive_memory.h"
#include "aura/primitive/primitive_shared_memory.h"
#include "aura/primitive/primitive_virtual_memory.h"
#include "aura/primitive/primitive_memory_container.h"



#include "aura/filesystem/file/file_memory_file.h"
#include "aura/filesystem/file/file_circular_file.h"


//#include "aura/filesystem/file/file_timeout_buffer.h"
//#include "aura/filesystem/file/file_transfer_buffer.h"

#include "aura/aura/aura/aura_local_memory_map.h"

#include "aura/primitive/str/str_international2.h"



//#if defined(METROWIN)
//
//#include "app/appseed/aura/aura/os/metrowin/metrowin_sockets.h"
//
//#endif


#include "aura/primitive/primitive_factory.h"


#include "aura/primitive/math/math_rng.h"
#include "aura/primitive/math/math_department.h"
#include "aura/primitive/geometry/geometry_geometry.h"


CLASS_DECL_AURA string get_exe_path();


//#include "user/user/user_window_map.h"
//
//#include "user/user/user_keyboard_layout.h"
//#include "user/user/user_keyboard.h"
//#include "user/user/user_user.h"

#include "aura/aura/aura/aura_main_init_data.h"

#include "aura/aura/aura/aura_savings.h"

#include "aura/net/net.h"


#include "aura/aura/aura/aura_launcher.h"


#include "aura/aura/aura/aura_ipc.h"


#include "aura/aura/aura/aura_app_launcher.h"


#include "aura/aura/aura/aura_ipi.h"




#include "aura/filesystem/filesystem/filesystem_application.h"

#include "aura/filesystem/filesystem/filesystem_dir_application.h"

#include "aura/filesystem/filesystem/filesystem_listing.h"

#include "aura/filesystem/file/file_set.h"

#include "aura/primitive/primitive_application_bias.h"


#include "aura/user/user/user_interactive.h"


//#include "aura/aura/aura/aura_application_interface.h"
#include "aura/aura/aura/aura_application.h"
#include "aura/aura/aura/aura_application_ptra.h"


#include "aura/aura/aura/aura_gudo_application.inl"


#include "aura/aura/aura/aura_application_signal_details.h"

#include "aura/aura/fontopus/fontopus.h"

#include "aura/aura/aura/aura_session.h"



#include "aura/aura/exception/exception_engine.h"

#include "aura/aura/aura/aura_fixed_alloc_impl.h"
#include "aura/aura/aura/aura_plex_heap_impl.h"
#include "aura/primitive/primitive_command_line.h"
#include "aura/primitive/primitive_command.h"
#include "aura/aura/aura/aura_create_context.h"
#include "aura/primitive/primitive_create.h"
#include "aura/primitive/primitive_request_signal.h"




#include "aura/aura/aura/aura_net.h"



#include "aura/primitive/primitive_command_thread.h"




#include "aura/filesystem/file/file_buffered_file.h"


#include "aura/aura/aura/aura_machine_event_data.h"
#include "aura/aura/aura/aura_machine_event.h"
#include "aura/aura/aura/aura_machine_event_central.h"

#include "aura/primitive/datetime/datetime.h"

#include "aura/primitive/str/str_international_locale_schema.h"

#include "aura/aura/aura/aura_id_pool.h"



#include "aura/aura/aura/aura_cregexp.h"
#include "aura/aura/aura/aura_cregexp_util.h"

#ifdef VSNORD

#include "aura/aura/os/android/android_init_data.h"

#endif


#include "aura/aura/process/process.h"

#include "aura/filesystem/filesystem/filesystem_system.h"

#include "aura/aura/aura/aura_file_watcher.h"
#include "aura/aura/aura/aura_file_watcher_listener_thread.h"

#include "aura/filesystem/filesystem/filesystem_dir_system.h"

#include "aura/aura/aura/aura_os.h"

#include "aura/aura/crypto/crypto.h"

#include "aura/aura/aura/aura_system.h"

#include "aura/aura/install/install.h"

#include "aura/aura/aura/aura_system_str.h"

#include "aura/os/os_process.h"









//#include "aura/install/install_trace.h"


#define new AURA_NEW


#include "aura/aura/aura/aura_system_trace.h"




#include "aura/multithreading/multithreading.inl"


#include "aura/primitive/str/str.inl"

#include "aura/aura/aura.inl"

#include "primitive/collection/collection.inl"


/*namespace numeric_info
{


template < typename T >
inline T get_maximum_value()
{
throw not_implemented(get_thread_app());
}

template < typename T >
inline T get_minimum_value()
{
throw not_implemented(get_thread_app());
}
template < typename T >
inline T get_null_value()
{
return 0;
}
template < typename T >
inline T get_unitary_value()
{
return 1;
}

template < typename T >
inline T get_allset_value()
{
T t;
memset(&t,0xff,sizeof(T));
return t;
}

template < typename T >
inline bool is_signed()
{
return ((T)-1) < 0;
}

template < typename T >
inline bool is_integer()
{
// guess, as float and double is implemented
return true;
}


template < typename T >
inline string json(const T & value)
{
return ::str::from(value);
}

} // namespace numeric_info

*/


#ifdef VARIADIC_TEMPLATE_FORMAT2

template<typename T,typename... Args>
inline void string_format::format(const char * & s,const T & value,Args... args)
{

   while(*s)
   {

      if(*s == '%' && *(++s) != '%')
      {

         defer_get_additional_argument(s,value,args...);

         return;

      }

      append(*s++);

   }

   throw simple_exception(get_thread_app(),"extra arguments provided to format");


}




#endif





#ifndef WINDOWSEX

#include "aura/os/cross/windows/windows_thread_impl.h"

#endif




#include "aura/primitive/collection/collection_array_base.inl"
#include "aura/primitive/collection/collection_array_impl.inl"
#include "aura/primitive/collection/collection_raw_array_impl.inl"
#include "aura/primitive/collection/collection_sort_array_impl.inl"
#include "aura/primitive/collection/collection_lemon_array_impl.inl"

#include "aura/primitive/collection/collection_list_impl.inl"
#include "aura/primitive/collection/collection_map_impl.inl"

#include "aura/primitive/collection/collection_sort_map_impl.inl"

#include "aura/primitive/collection/collection_fifo_map_impl.inl"



#include "aura/primitive/primitive_factory_impl.inl"





#include "aura/aura/aura/aura_system_smart_pointer.inl"
#include "aura/aura/aura/aura_system_object.inl"
#include "aura/aura/aura/aura_system_id.inl"
#include "aura/aura/aura/aura_system_factory.inl"
#include "aura/aura/aura/aura_system_primitive_var.inl"
#include "aura/aura/aura/aura_system_xml.inl"



//#include "aura/aura/aura/aura_core_os.h"


#include "aura/aura/aura/aura_chronometer.h"







#include "aura/aura/aura/aura_assert_running_task.h"










#include "aura/aura/aura/aura_number.h"


#include "aura/aura/aura/aura_debug.h"


#include "aura/aura/aura/aura_printf.h"
#include "aura/aura/aura/aura_sprintf.h"



#include "aura/aura/aura/aura_math.h"









#include "aura/aura/aura/aura_file_watcher.h"
#include "aura/aura/aura/aura_file_watcher_impl.h"


#include "aura/aura/aura_console_window.h"


#include "aura/aura/aura/os.h"


#include "aura/multithreading/multithreading_data.h"


#include "aura/aura/aura/aura_shell_launcher.h"


#include "aura/aura/aura/aura_async.h"


#include "aura/aura/aura/aura_message_loop.h"

#include "aura/aura/aura/aura_international.h"

#include "aura/aura/aura/aura_static_start.h"



#define return_(y, x) {y = x; return;}


extern "C"
{


   int32_t _c_lock_is_active(const char * pszName);
   int32_t _c_lock(const char * pszName,void ** pdata);
   int32_t _c_unlock(void ** pdata);


}


CLASS_DECL_AURA string _ca_get_file_name(const char * psz,bool bCreate = false,int32_t * pfd = NULL);

CLASS_DECL_AURA string get_system_error_message(uint32_t dwError);


#include "aura/aura/aura/aura_simple_app.h"


#include "aura/aura/aura/aura_profiler.h"


#include "aura/os/os2.h"


#include "app/appseed/aura/aura/node/node.h"


#include "aura/aura/install/install.h"


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

   template < class KEY,class VALUE >
   using map = ::map < KEY,const KEY &,VALUE,const VALUE & >;

   template < class TYPE >
   using list = ::list< TYPE >;

   template < class TYPE >
   using vector = ::array< TYPE >;

   template < class TYPE >
   using set = ::set< TYPE >;

   using ostream = ::file::ostream;
   using ofstream = ::file::file_ostream;

   using istream = ::file::istream;
   using ifstream = ::file::file_istream;

   using stream = ::file::stream;
   using fstream = ::file::file_stream;

   using iostream = ::file::stream;
   using iofstream = ::file::file_stream;

   using stringstream = ::file::plain_text_stream_string_file;
   using ostringstream = ::file::plain_text_stream_string_file;

   template < typename T >
   using numeric_limits = ::numeric_info < T >;

   //      template <class T> void sort(T & t1,T & t2)
   //    {
   //     ::sort::sort < T >(t1,t2);
   //}

   template < class iterator >
   void sort(iterator & a, iterator & b)
   {
      ::sort::quick_sort_iter(a, b);
   }

   template < typename T >
   using auto_ptr = ::pointer < T >;

   template < typename T >
   using stack = ::stack < T >;


   using filebuf = ::file::stdio_file;

   //using ios_base = ::file::stream_base;

   // replace ::::file:: => ::file::


   using runtime_error = ::runtime_error;

   using stdio_file = ::file::file;


   template < typename T >
   T abs(T t)
   {
      if(t < ::numeric_info< T >::null())
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


   template <class T> const T& min(const T& a,const T& b) { return !(a > b) ? a : b; }
   template <class T> const T& max(const T& a,const T& b) { return !(a < b) ? a : b; }

   template <class RandomAccessIterator>
   void make_heap(RandomAccessIterator first,RandomAccessIterator last)
   {
      ::lemon::make_heap(first,last);
   }

   template <class RandomAccessIterator,class Compare>
   void make_heap(RandomAccessIterator first,RandomAccessIterator last,Compare comp)
   {
      ::lemon::make_heap(first,last,comp);
   }

   template <class RandomAccessIterator,class Compare>
   void pop_heap(RandomAccessIterator first,RandomAccessIterator last)
   {
      ::lemon::pop_heap(first,last);
   }

   template <class RandomAccessIterator,class Compare>
   void pop_heap(RandomAccessIterator first,RandomAccessIterator last,Compare comp)
   {
      ::lemon::pop_heap(first,last,comp);
   }

   template <class RandomAccessIterator,class Compare>
   void push_heap(RandomAccessIterator first,RandomAccessIterator last)
   {
      ::lemon::push_heap(first,last);
   }

   template <class RandomAccessIterator,class Compare>
   void push_heap(RandomAccessIterator first,RandomAccessIterator last,Compare comp)
   {
      ::lemon::push_heap(first,last,comp);
   }

   template <class RandomAccessIterator>
   void sort_heap(RandomAccessIterator first,RandomAccessIterator last)
   {
      ::lemon::sort_heap(first,last);
   }

   template <class RandomAccessIterator,class Compare>
   void sort_heap(RandomAccessIterator first,RandomAccessIterator last,Compare comp)
   {
      ::lemon::sort_heap(first,last,comp);
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




#include "aura/primitive/primitive_color.h"


#include "aura/aura/aura/aura_str_context.h"


#include "aura/filesystem/file/file_html_file.h"


#include "aura/primitive/data/data_simple_item.h"


#include "aura/primitive/data/data_tree_item.h"


#include "aura/primitive/data/data_tree.h"


#include "aura/filesystem/file/file_edit_file.h"


CLASS_DECL_AURA int trace_hr(const char * psz,HRESULT hr);





CLASS_DECL_AURA string get_last_error_string();

CLASS_DECL_AURA string get_error_string(uint64_t uiError);

CLASS_DECL_AURA void set_aura(void * p,::aura::application * papp);
CLASS_DECL_AURA ::aura::application * get_aura(void * p);

#include "aura/aura/aura_error.h"
#include "aura/primitive/primitive.h"


#include "aura/user/user.h"


#include "aura/aura/aura_speaker.h"


#include "aura/pcre/pcre.h"


#include "aura/charguess/charguess.h"


#include "aura/net/net.h"


#include "aura/net/net_email_department.h"


#include "aura/aura/scripting/javascript/javascript.h"


#include "aura/aura/compress/compress.h"


#include "aura/primitive/str/str_str3.h"


#include "aura/graphics/graphics.h"

#include "aura/install/install2.h"


namespace user
{

   inline oswindow primitive::get_safe_handle() const
   {
      if(((byte *)this) < (byte *)(((byte *)NULL) + (16 * 1024))) // consider invalid
      {
         return NULL;
      }
      return get_handle();
   }






} // namespace user

#include "aura/primitive/collection/collection_string_array_impl.inl"

#include "aura/primitive/primitive.inl"

#include "aura/filesystem/filesystem.inl"

#include "aura/net/net.inl"






