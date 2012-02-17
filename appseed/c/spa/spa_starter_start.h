#pragma once

CLASS_DECL_c void update_ca2_build();
CLASS_DECL_c const char * ca2_get_build();
CLASS_DECL_c void update_ca2_installed(bool bUnloadIfNotInstalled = true);
CLASS_DECL_c bool is_ca2_installed();
CLASS_DECL_c void update_ca2_updated();
CLASS_DECL_c bool is_ca2_updated();
CLASS_DECL_c void set_ca2_updated(const char * pszBuild);
CLASS_DECL_c void update_updated();
CLASS_DECL_c bool is_updated();
CLASS_DECL_c void set_updated(const char * pszBuild);


CLASS_DECL_c void set_installing_ca2(bool bSet = true);
CLASS_DECL_c bool is_installing_ca2();


CLASS_DECL_c vsstring get_command_line_param(const char * pszCommandLine, const char * pszParam, const char * pszIfParamValue, const char * pszReplaceParam);
CLASS_DECL_c vsstring get_command_line_param(const char * pszCommandLine, const char * pszParam);


class CLASS_DECL_c spa_starter_start
{
public:
      
      
   ::hotplugin::plugin  *              m_pplugin;
   vsstring                            m_strCommandLine;


   UINT start();

};

DWORD WINAPI _ca2_starter_start(LPVOID pvoid);