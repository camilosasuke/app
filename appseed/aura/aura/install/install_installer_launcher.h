#pragma once


#if defined(INSTALL_SUBSYSTEM)


namespace install
{


   class CLASS_DECL_AURA installer_launcher :
      virtual public ::launcher,
      virtual public ::object
   {
   public:


      string   m_strPath;
      string   m_strConfiguration;


      installer_launcher(::aura::application * papp, const char * pszConfiguration);


      virtual bool ensure_executable();

      virtual string get_executable_path();


   };


} // namespace install


#endif



