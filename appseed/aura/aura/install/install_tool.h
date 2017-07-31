#pragma once


#if defined(INSTALL_SUBSYSTEM)


namespace install
{


   class CLASS_DECL_AURA tool :
      virtual public ::netserver::socket_handler
   {
   public:


      class md5_item
      {
      public:

         DWORD       m_dwStart;
         string      m_strMd5;

      };


      string_map < string_map < string_map < md5_item > > >       m_mapMd5;
      string                                                      m_strTraceLabel;


      tool(::aura::application * papp);
      virtual ~tool();


      virtual bool is_file_ok(const char * path1, const char * pszTemplate, const char * pszFormatBuild, string pszPlatform);
      virtual bool is_file_ok(const stringa & straPath, const stringa & straTemplate, stringa & straMd5, const string & strFormatBuild, int iMd5Retry, string strPlatform);

      void http_get_md5(stringa straTemplate, const char * pszFormatBuild, string strPlatform);
      stringa get_reference_md5(stringa straTemplate, const char * pszFormatBuild, string strPlatform);

      virtual void app_install_call_sync(const char * pszPlatform, const char * szParameters, const char * pszBuild);
      virtual bool app_install_send_short_message(const char * psz, const char * pszParams, bool bLaunch, const char * pszBuild);
      virtual void start_app_install_in_context(string strPlatform, bool bAlreadyElevated);

      virtual void trace(const string & str);
      virtual void trace(double dRate);

      virtual bool get_admin();

#ifdef WINDOWSEX
      
      virtual void run_vcredist(string strPlatform);
      
#endif
      

   };


} // namespace install


#endif




