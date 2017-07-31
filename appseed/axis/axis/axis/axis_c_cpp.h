#pragma once


namespace plane
{

   class session;
   class system;
   //class cube;


} // namespace plane


namespace html
{

   class html;
   class data;

} // namespace html



#if defined(INSTALL_SUBSYSTEM)


namespace install
{

   class install;

} // namespace install


#endif


namespace message
{


   class mouse;


} // namespace message


#ifdef HOTPLUGIN_SUBSYSTEM

namespace hotplugin
{

   class host;
   class plugin;

}

#endif

namespace user
{


   class elemental;
   class interaction;
   class interaction_impl;
   class interaction_child;
   class schema;
   class schema_simple_impl;
   class user;
   class copydesk;
   class create_struct;
   class window_draw;
   class control_event;
   class form_window;


} // namespace user




typedef sp(::user::interaction_impl) window_sp;




#undef CaSys
#undef Sys
#define CaSys(pca) (*pca->m_pauraapp->m_paxissystem)
#define Sys(paxisapp) (*paxisapp->m_paxissystem)
#define threadSystem (Sys(get_thread_app()))

#undef Sess
#define Sess(paxisapp) (*paxisapp->m_paxissession)
//#define Session (Sess(m_pauraapp))
//#define Sess(paxisapp) (*paxisapp->m_pcoresession)
//#define Session (Sess(m_pauraapp))

#undef App
#undef Application
#define App(paxisapp) (*paxisapp->m_paxisapp)
#define Application (App(m_pauraapp->m_paxisapp))




namespace axis
{

#if defined METROWIN && defined(__cplusplus_winrt)

   interface class system_window
   {

      virtual Windows::Foundation::Rect get_window_rect() = 0;
      virtual Windows::Foundation::Point get_cursor_pos() = 0;



   };

   CLASS_DECL_AXIS bool get_window_rect(system_window ^ pwindow,RECTD * lprect);
   CLASS_DECL_AXIS bool get_window_rect(system_window ^ pwindow,LPRECT lprect);

#endif


   class session;

} // namespace axis





CLASS_DECL_AXIS bool axis_init();
CLASS_DECL_AXIS bool axis_term();

CLASS_DECL_AXIS bool __node_axis_pre_init();
CLASS_DECL_AXIS bool __node_axis_pos_init();

CLASS_DECL_AXIS bool __node_axis_pre_term();
CLASS_DECL_AXIS bool __node_axis_pos_term();


#include "axis/filesystem/filesystem.h"


#include "axis/axis/geoip/geoip.h"


#if defined(LINUX)

#include "axis/axis/os/ansios/ansios.h"

#elif defined(METROWIN)

#include "axis/os/metrowin/metrowin.h"

#elif defined(MACOS)

#include "axis/os/ansios/ansios.h"
#include "axis/os/macos/macos.h"

#elif defined(ANDROID)

#include "axis/os/ansios/ansios.h"
#include "axis/os/android/android.h"

#elif defined(WINDOWS)

#include "axis/os/windows/windows.h"

#elif defined(APPLE_IOS)

#include "axis/os/ansios/ansios.h"
#include "axis/os/ios/ios.h"
//#include "axis/os/ios/ios_windowing.h"

#elif defined(SOLARIS)

#include "axis/os/ansios/ansios.h"
#include "axis/os/solaris/solaris_user_impl.h"

#else

#error "not implemented!!"

#endif



#if defined(METROWIN)

#include "app/appseed/axis/axis/os/metrowin/metrowin_sockets.h"

#endif


//#include "axis/programming/javascript/javascript.h"

#include "axis/filesystem/fs/fs.h"

//#include "axis/axis/fontopus/fontopus.h"



#include "axis/user/user/user_enum.h"

#include "axis/user/user/user_key_enum.h"

#include "axis/axis/message/message_user.h"

#include "axis/user/user/user_keyboard_layout.h"

#include "axis/user/user/user_keyboard.h"

#include "axis/user/user.h"



//#include "axis/axis/message/message_user.h"


//#include "axis/primitive/datetime/datetime_value.h"


//#include "axis/primitive/datetime/datetime_department.h"

#include "axis/database/database/database.h"

#include "axis/database/sqlitedb/sqlitedb.h"

#include "axis/database/simpledb/simpledb.h"

#include "axis_application.h"


#include "axis_session.h"

#include "axis/filesystem/filesystem/filesystem_axis_application.h"

#include "axis/filesystem/filesystem/filesystem_dir_axis_application.h"

#include "axis/filesystem/filesystem/filesystem_axis_system.h"

#include "axis/filesystem/filesystem/filesystem_dir_axis_system.h"

//#include "axis/axis/compress/compress.h"

//#include "axis/axis/crypto/crypto.h"

#include "axis/graphics/graphics.h"

#include "axis_system.h"








#ifdef METROWIN

#define BYESHYTOULA_STYLE_SOCKS

#else

#ifndef BSD_STYLE_SOCKETS

#define BSD_STYLE_SOCKETS 1

#endif

#endif


#include "axis_os.h"


#include "axis_static_start.h"


#include "axis_simple_app.h"


#include "app/appseed/axis/axis/node/node.h"




#include "primitive/data/data_tree_item.h"

#include "primitive/data/data_tree.h"

#include "primitive/data/data_simple_item.h"


#include "axis/html_lite/html_lite/html_lite.h"



//#include "axis/database/database/database.h"
//
//#include "axis/database/sqlitedb/sqlitedb.h"
//
//#include "axis/database/simpledb/simpledb.h"


#include "axis.inl"


#include "axis/graphics/visual/visual.inl"




