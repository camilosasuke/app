#pragma once


#if !defined(METROWIN)

CLASS_DECL_AURA int32_t call_async(const char * pszPath, const char * pszParam, const char * pszDir, int32_t iShow, bool bPrivileged, unsigned int * puiPid = NULL);

typedef int32_t CALLSYNCONRETRY(int32_t iTry,uint_ptr dwParam);

typedef CALLSYNCONRETRY * PFNCALLSYNCONRETRY;

CLASS_DECL_AURA uint32_t call_sync(const char * pszPath, const char * pszParam, const char * pszDir, int32_t iShow, int32_t iRetry, int32_t iSleep = 1000,PFNCALLSYNCONRETRY pfnOnRetry = NULL, uint_ptr dwParam = 0, unsigned int * puiPid = NULL);

#endif


#if !defined(METROWIN) && !defined(LINUX) && !defined(APPLEOS)

CLASS_DECL_AURA int32_t get_current_processor_index();

CLASS_DECL_AURA int32_t get_current_process_maximum_affinity();

CLASS_DECL_AURA int32_t get_current_process_affinity_order();

#endif


CLASS_DECL_AURA string expand_env(string str);

CLASS_DECL_AURA string consume_param(const char * pszCommandLine, const char ** pszEndPtr);
//CLASS_DECL_AURA string get_command_line_param(const char * pszCommandLine, const char * pszParam, const char * pszIfParamValue, const char * pszReplaceParam);
CLASS_DECL_AURA bool get_command_line_param(string & strValue, const char * pszCommandLine, const char * pszParam);
CLASS_DECL_AURA bool get_command_line_param(string & strValue, const char * pszCommandLine, const char * pszParam, const char * pszDefault);
CLASS_DECL_AURA string get_command_line_param(const char * pszCommandLine,const char * pszParam);


CLASS_DECL_AURA string time_binary_platform(string strPlatform);
CLASS_DECL_AURA string process_platform_dir_name();
CLASS_DECL_AURA string process_platform_dir_name2();
CLASS_DECL_AURA string process_version_dir_name();


#if !defined(METROWIN)


CLASS_DECL_AURA string module_path_from_pid(uint32_t pid);
CLASS_DECL_AURA int_array module_path_get_pid(const char * pszModuleName, bool bModuleNameIsPropertyFormatted = true);


#ifndef WINDOWS

CLASS_DECL_AURA stringa cmdline_from_pid(uint32_t pid);
CLASS_DECL_AURA int_array app_get_pid(const char * pszModuleName);


#endif


#endif

#ifndef METROWIN
CLASS_DECL_AURA bool process_contains_module(string & strImage, DWORD processID, const char * pszLibrary);
CLASS_DECL_AURA void shared_library_process(dword_array & dwa, stringa & straProcesses, const char * pszLibrary);
#endif



CLASS_DECL_AURA bool is_shared_library_busy(uint32_t processid, const stringa & stra);
CLASS_DECL_AURA bool is_shared_library_busy(const stringa & stra);


CLASS_DECL_AURA bool launch_application(::aura::application * papp, const string & strAppId, const string & strParams, int iBitCount);



bool shell_execute_async(const char * pszFile, const char * pszParams);
bool shell_execute_sync(const char * pszFile, const char * pszParams, ::duration durationTimeout = one_minute());

bool root_execute_async(const char * pszFile, const char * pszParams);
bool root_execute_sync(const char * pszFile, const char * pszParams, ::duration durationTimeout = one_minute());

