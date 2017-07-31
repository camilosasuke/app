#pragma once


#if defined(INSTALL_SUBSYSTEM)


namespace install
{


   class application;


   enum e_item_type
   {

      item_type_none,
      item_type_spa,

   };


   class bootstrap;

   class installer;


} // namespace install


#include "install_window.h"
#include "install_install_item.h"
#include "install_socket.h"
#include "install_socket_handler.h"
#include "install_socket_thread.h"
#include "install_trace.h"


#include "install_tool.h"


#include "install_bootstrap.h"


#include "install_installer.h"


#include "install_application.h"


#endif
