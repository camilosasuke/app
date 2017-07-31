#pragma once


#if defined(INSTALL_SUBSYSTEM)


namespace install
{


   class CLASS_DECL_BASE starter_start :
      virtual public object
   {
   public:


      ::hotplugin::host *              m_phost;
      ::hotplugin::plugin  *           m_pplugin;
      string                           m_strCommandLine;
      bool                             m_bStarting;

      starter_start(::aura::application * papp);

      UINT start();

   };


   uint32_t _ca2_starter_start(void * pvoid);


} // namespace install



#endif

