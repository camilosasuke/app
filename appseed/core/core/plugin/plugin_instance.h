#pragma once


#ifdef HOTPLUGIN_SUBSYSTEM


namespace plugin
{


   class CLASS_DECL_CORE instance :
      public ::plugin::plugin
   {
   public:



      oswindow                          m_oswindow;
      bool                          m_bStream;



      instance(::aura::application * papp);
      virtual ~instance();


      uint16_t HandleEvent(void * pvoid);


      virtual host_interaction * create_host_interaction();

      void shut();

      virtual bool initialize();


   };


} // namespace plugin


#endif


