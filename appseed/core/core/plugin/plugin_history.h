#pragma once


#ifdef HOTPLUGIN_SUBSYSTEM


namespace plugin
{

   class CLASS_DECL_CORE history :
      virtual public ::core::history
   {
   public:

      
      plugin * m_pplugin;


      history(::aura::application * papp, plugin * pplugin);
         
   
      virtual bool hist(const char *pszUrl);

   };


} // namespace plugin


#endif



