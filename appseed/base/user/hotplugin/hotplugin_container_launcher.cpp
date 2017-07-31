//#include "framework.h"
//#include "base/user/user.h"


#ifdef HOTPLUGIN_SUBSYSTEM


namespace hotplugin
{


   container_launcher::container_launcher(string strPlatform, const char * pszChannel)
   {
      
      m_strChannel   = pszChannel;

      m_strPlatform  = strPlatform;

   }


   string container_launcher::get_executable_path()
   {

      return ::path::app_app_nest(m_strPlatform);

   }


   string container_launcher::get_params()
   {

      return string(": channel=") + m_strChannel;

   }


} // namespace hotplugin


#endif





