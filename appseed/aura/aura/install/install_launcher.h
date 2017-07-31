#pragma once


#if defined(INSTALL_SUBSYSTEM)


namespace install
{


   class CLASS_DECL_AURA launcher :
      virtual public ::launcher
   {
   public:

      string   m_strPath;
      string   m_strVersion;
      string   m_strBuild;
      string   m_strPlatform;

      launcher(const char * pszPlatform, const char * pszVersion, const char * pszBuild);

      virtual bool ensure_executable() override;

      virtual string get_executable_path() override;

      virtual void start_in_context(bool bAlreadyElevated);

   };


} // namespace install



#endif




