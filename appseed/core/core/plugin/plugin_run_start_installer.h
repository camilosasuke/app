#pragma once


#ifdef HOTPLUGIN_SUBSYSTEM


#include "core/core/core/core_run_start_installer.h"


namespace plugin
{



   class run_start_installer :
      virtual public ::core::run_start_installer
   {
   public:

      
      plugin * m_pplugin;


      run_start_installer(::aura::application * papp, plugin * pplugin);
         
   
      virtual void run_start_install(const char * pszStart);

   };


} // namespace plugin


#endif


