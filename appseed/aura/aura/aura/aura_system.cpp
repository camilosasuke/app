#include "framework.h" // previously aura/user/user.h
#include "aura/user/colorertake5/colorertake5.h"

#if defined(LINUX) || defined(ANDROID)

   //#include <fcntl.h>
   //#include <unistd.h>


   //#include <link.h>

   //#include <dlfcn.h>
   //#include <ctype.h>
   //#include <sys/stat.h>
#endif

#ifdef LINUX
   UINT __x11_thread(void * pparam);
#endif

void dappy(const char * psz);
#ifdef WINDOWSEX
string get_user_name()
{
   WCHAR wsz[1024];
   DWORD dwSize = sizeof(wsz) / sizeof(WCHAR);
   ::GetUserNameW(wsz,&dwSize);
   return string(wsz);
}
#endif
#ifdef MACOS
void ns_app_terminate();
void ns_create_alias(const char * pszTarget, const char * pszSource);
#endif
namespace aura
{


   class ::id system::idEmpty;

   system * system::g_p = NULL;


   system::system(::aura::application * papp, void * pdata) :
      m_process(this),
      m_base64(this),
      m_httpsystem(this)
   {

      m_bThreadToolsForIncreasedFps = false;

      m_typemap.InitHashTable(2048);

      m_bAcid = false;
      
      m_pdumpcontext = new dump_context();

#ifndef WINDOWS
      
      exception::translator::attach();
      
#endif

#ifdef VSNORD

      m_pandroidinitdata = (android_init_data *) pdata;

#endif

      if (g_p == NULL)
      {

         g_p = this;

      }

      g_pszCooperativeLevel = "aura";

#ifdef MATTER_CACHE_FROM_HTTP_SERVER

      m_bMatterFromHttpCache = true;

#else

      m_bMatterFromHttpCache = false;

#endif

      set_app(this);
      
//#ifdef APPLEOS
  //    translator::attach();
//#endif

      m_nSafetyPoolSize          = 512;        // default size

      m_pmath                    = canew(math::math(this));
      m_pgeometry                = canew(geometry::geometry(this));
//      m_phtml = NULL;


      m_paurasystem = this;

      if(papp == NULL)
      {

         oprop("parent_system") = (sp(object)) NULL;

      }
      else
      {

         oprop("parent_system") = papp->m_paurasystem;

      }


//      m_pfnVerb = &system::install_uninstall_verb;



      m_bDoNotExitIfNoApplications              = true;



//      m_peengine = NULL;


      m_pmachineeventcentral = NULL;

      //::ca::application::m_file.set_app(this);
      //::ca::application::m_dir.set_app(this);

      string strId;
      //strId = m_strAppName;
      //strId += ::str::has_char(m_strAppId, ".");
      //strId += ::str::has_char(m_strBaseSupportId, ".");


      strId = "ca2log";


      xxdebug_box("Going to start Log","Just before initialize log",0);

      // log starts here
      if(!initialize_log(strId))
      {
         xxdebug_box("Could not initialize log","Failed to initialize log",0);
         throw "failed to initialize log";
      }


      /*
      if(psystemParent == NULL)
      {

      m_peengine                                = new ::exception::engine(this);

      }
      else
      {

      m_peengine                                = psystemParent->m_peengine;

      }
      */

      m_ftlibrary = NULL;

      //use_base_ca2_allocator();


      m_pfactory = canew(class base_factory(this));
      m_pfactory->set_app(this);

      m_pfactory->creatable_large < ::file::simple_binary_file >(type_info < ::file::binary_file >());
      m_pfactory->creatable_large < ::file::string_file >();
      m_pfactory->creatable_large < ::memory_file >();
      m_pfactory->creatable_large < ::int64_array >();

      factory().default_cloneable_large < stringa >();
      factory().default_cloneable_large < memory >();
      factory().default_cloneable_large < int_array >();

      factory().creatable_small < ::file::application >();
      factory().creatable_small < ::file::dir::application >();

      m_phtml = NULL;

      __node_aura_factory_exchange(this);

      m_pdatetime = canew(class ::datetime::department(this));


      thread::s_bAllocReady = true;

      m_pxml = canew(::xml::department(this));

      m_pxml->construct(this);

      if(!m_pxml->initialize1())
         throw simple_exception(this,"failed to construct system m_pxml->initialize1()");

      if(!m_pxml->initialize())
         throw simple_exception(this,"failed to construct system m_pxml->initialize()");


//      m_spmutexFactory = canew(mutex(get_app()));

      m_bGudoNetCache = true;

      m_purldepartment = NULL;

      m_pcompress = NULL;

      m_strAppName               = "system";
      m_strInstallToken          = "system";

      m_dwAfterApplicationFirstRequest = 0;


      factory().creatable_small < ::visual::icon >();
      m_mapLibrary["draw2d"] = canew(::aura::library(this));
      m_purldepartment = new url::department(this);

      m_pcompress = new ::axis::compress_department(this);

      m_pcompress->set_app(this);

      ::draw2d::dib::static_initialize();

#if defined(INSTALL_SUBSYSTEM)

      m_spinstall = canew(::install::install(this));

#endif


   }


   system::~system()
   {

      try
      {

         m_pmachineeventcentral.release();

      }
      catch (...)
      {

      }

      try
      {

         if (m_pfactory != NULL)
         {

            m_pfactory->enable_simple_factory_request(false);

            m_pfactory.release();

         }

      }
      catch (...)
      {
         TRACE("system::exit_application: Potentially catastrophical error : error disabling simple factory request");
      }

      if (g_p == this)
      {

         g_p = NULL;

      }

#ifndef WINDOWS
      
      exception::translator::detach();
      
#endif
      
      ::aura::del(m_pdumpcontext);


      

   }



//   bool system::install_uninstall_verb()
//   {
//
////      ::output_debug_string("::aura::system::install_uninstall_verb " + demangle(typeid(*this).name()) + "\n");
//
//      static DWORD dwStart = get_tick_count();
//
//      if(directrix()->m_varTopicQuery.has_property("install") && (get_tick_count() - dwStart) > (1000 * 1000))
//      {
//
//         ::output_debug_string("::aura::system::install_uninstall_verb " + demangle(typeid(*this).name()) +
//         " quitting (1)\n");
//
//         return false;
//
//      }
//
//      if(directrix()->m_varTopicQuery.has_property("uninstall") && (get_tick_count() - dwStart) > (1000 * 1000))
//      {
//
//         ::output_debug_string("::aura::system::install_uninstall_verb " + demangle(typeid(*this).name()) +
//         " quitting (2)\n");
//
//         return false;
//
//      }
//
//      return common_verb();
//
//   }
//

   void system::defer_check_exit()
   {

      if(!m_bDoNotExitIfNoApplications)
      {

         ::aura::application_ptra appptra;

         appptra = get_appptra();

         for(int32_t i = 0; i < appptra.get_size();)
         {

            try
            {

               if(appptra[i] == NULL || appptra[i]->is_session() || appptra[i]->is_system())
               {

                  appptra.remove_at(i);

                  continue;

               }
               else if(appptra[i]->is_serviceable() && appptra[i]->m_strAppId != directrix()->m_varTopicQuery["app"].get_string())
               {

                  appptra.remove_at(i);

                  continue;

               }

            }
            catch(...)
            {

               appptra.remove_at(i);

               continue;

            }

            i++;

         }

         if(appptra.get_size() <= 0)
         {

            post_quit();

         }

         if(appptra.get_size() == 1 && appptra.contains(this))
         {

            post_quit();

         }

      }


   }

//
//
//   bool system::verb()
//   {
//
//
//      return (this->*m_pfnVerb)();
//
//
//   }


   void system::construct(const char * pszAppId)
   {
      

      ::aura::application::construct(pszAppId);

   }




   base_factory & system::factory()
   {

      return *m_pfactory;

   }


   //::exception::engine & system::eengine()
   //{

   //   return *m_peengine;

   //}

   ::datetime::department & system::datetime()
   {

      return *m_pdatetime;

   }

   ::aura::str & system::str()
   {

      return *m_puserstr;

   }



   bool system::process_initialize()
   {
      

      //m_peengine = new ::exception::engine(this);

      m_pfactory->cloneable_large < ::create >();
      m_pfactory->cloneable_large < application_bias >();
      m_pfactory->cloneable_large < command_line >();
      m_pfactory->cloneable_large < manual_reset_event >();
      m_pfactory->cloneable_large < mutex >();
      m_pfactory->cloneable_large < event >();

      if(!::aura::application::process_initialize())
         return false;

      m_spos.alloc(allocer());


      if(!set_main_init_data(m_pinitmaindata))
         return false;


      if(is_installing() || is_uninstalling())
      {

#ifdef MACOS

         /*         if(0 != setuid(0))
                     return false;

                  uid_t uid = getuid();

                  string str("installing or uninstalling as root : getuid() %d", uid);

                  ::dir::mk("/ca2core");

                  file_put_contents_dup("/ca2core/teste.txt", str, str.length());
                  */
#endif

#if 0
         // Create authorization reference
         OSStatus status;
         AuthorizationRef authorizationRef;

         // AuthorizationCreate and pass NULL as the initial
         // AuthorizationRights set so that the AuthorizationRef gets created
         // successfully, and then later call AuthorizationCopyRights to
         // determine or extend the allowable rights.
         // http://developer.apple.com/qa/qa2001/qa1172.html
         status = AuthorizationCreate(NULL, kAuthorizationEmptyEnvironment,
                                      kAuthorizationFlagDefaults, &authorizationRef);
         if (status != errAuthorizationSuccess)
         {
            TRACE("Error Creating Initial Authorization: %d", status);
            return false;
         }

         // kAuthorizationRightExecute == "system.privilege.admin"
         AuthorizationItem right = {kAuthorizationRightExecute, 0, NULL, 0};
         AuthorizationRights rights = {1, &right};
         AuthorizationFlags flags = kAuthorizationFlagDefaults |
                                    kAuthorizationFlagInteractionAllowed |
                                    kAuthorizationFlagPreAuthorize |
                                    kAuthorizationFlagExtendRights;

         // Call AuthorizationCopyRights to determine or extend the allowable rights.
         status = AuthorizationCopyRights(authorizationRef, &rights, NULL, flags, NULL);
         if (status != errAuthorizationSuccess)
         {
            TRACE("Copy Rights Unsuccessful: %d", status);
            return false;
         }
#endif
      }


      if(m_pmachineeventcentral == NULL)
      {
#ifndef APPLE_IOS
         m_pmachineeventcentral = canew(::machine_event_central(this));
         if(!m_pmachineeventcentral->initialize())
            return false;
         if(m_pmachineeventcentral->is_close_application())
            return false;
#endif
      }

      m_spfile.alloc(allocer());

      if(!m_spfile->initialize())
      {
         
         thiserr << "failed to initialize file-system";
         
         return false;
         
      }

      m_spdir.alloc(allocer());

      if(!m_spdir->initialize())
      {
         
         thiserr << "failed to initialize dir-system";
         
         return false;
         
      }


      output_debug_string("CommonAppData (matter) : " + System.dir().commonappdata()  + "\n");
      output_debug_string("commonappdata (matter) : " + System.dir().commonappdata() + "\n");
      output_debug_string("Common App Data (matter) : " + System.dir().commonappdata() + "\n");
      output_debug_string("common app data (matter) : " + System.dir().commonappdata() + "\n");

      dappy(string(typeid(*this).name()) + " : Going to ::axis::session " + ::str::from(m_iReturnCode));


      if(!alloc_session())
      {

         TRACE("Failed to allocate session");

         return false;

      }


      //m_spcrypto.alloc(allocer());

      m_spcrypto = canew(::crypto::crypto(this));

      if (!m_spcrypto.is_set())
         return false;


      bool bOk = true;

      try
      {

         draw2d_factory_exchange();

      }
      catch (...)
      {

         output_debug_string("Unable to find ANY *DRAW2D* plugin. Quitting...");

      }


      return true;

   }

   bool system::initialize1()
   {



      if(!::aura::application::initialize1())
         return false;

      m_puserstr = canew(::aura::str(this));

      if(m_puserstr == NULL)
         return false;

      if(!str().initialize())
         return false;

      return true;

   }

   bool system::initialize2()
   {

      if (!::aura::application::initialize2())
      {

         return false;

      }

      return true;

   }


   bool system::initialize_application()
   {

      if(!m_paurasession->begin_synch(&m_iReturnCode))
      {
         
         return false;
         
      }

      dappy(string(typeid(*this).name()) + " : ::aura::session OK " + ::str::from(m_iReturnCode));


      m_pfactory->enable_simple_factory_request();

      if (!::aura::application::initialize_application())
      {

         return false;

      }

      return true;

   }


   bool system::finalize()
   {

      __wait_threading_count_except(this,::millis((5000) * 77));

      bool bOk = false;

      try
      {

         bOk = ::aura::application::finalize();

      }
      catch(...)
      {

         bOk = false;

      }

      return bOk;

   }


   int32_t system::exit_application()
   {


      try
      {
         
         for(auto & pair : System.m_appmap)
         {
            
            try
            {
               
               if(pair.m_element2->m_paurasystem == this)
               {
                  
                  pair.m_element2->m_paurasystem = NULL;
                  
               }
               
            }
            catch(...)
            {
               
            }
            
         }
         
      }
      catch(...)
      {
   
      }
         

      __wait_threading_count(::millis((5000) * 8));

      try
      {


         /*      try
         {
         if(m_plemonarray != NULL)
         {
         delete m_plemonarray;
         }
         }
         catch(...)
         {
         }
         m_plemonarray = NULL;
         */


         m_pmath.release();

         m_pgeometry.release();

      }
      catch(...)
      {

         m_iReturnCode = -86;

      }


      for(int i = 0; i < m_serviceptra.get_size(); i++)
      {

         try
         {

            m_serviceptra[i]->Stop(0);

         }
         catch(...)
         {

         }

      }


      for(int i = 0; i < m_serviceptra.get_size(); i++)
      {
         try
         {
            m_serviceptra[i]->Stop((5000) * 2);
         }
         catch(...)
         {
         }
      }

      m_serviceptra.remove_all();

      multithreading::post_quit_and_wait(m_paurasession, seconds(60));

//      m_basesessionptra.remove_all();

      m_spfile.release();

      m_spdir.release();

      {

         mutex m(get_app(), false, "Global\\ca2_datetime_departament");

         synch_lock sl(&m);

         m_pdatetime.release();

      }

      m_plog.release();

      m_pmath.release();

      int32_t iRet = 0;

      try
      {

         iRet = ::aura::application::exit_application();

      }
      catch(...)
      {

      }


      try
      {

         m_spos.release();

      }
      catch(...)
      {

      }


      try
      {

         if(m_pmachineeventcentral != NULL)
         {

            m_pmachineeventcentral->post_quit();

         }

      }
      catch(...)
      {

      }



      m_plog.release();



      {

         synch_lock sl(&m_mutexFactory);

         m_typemap.remove_all();

         m_typemap.release();

      }



      ::aura::application::exit_application();

#ifdef METROWIN
//      m_pdevicecontext = nullptr;

//      m_pmutexDc.release();
#endif



      //if(m_peengine != NULL)
      //{

      //   delete m_peengine;

      //   m_peengine = NULL;

      //}
#ifdef MACOS
      ns_app_terminate();
#endif

      return iRet;

   }


   UINT system::os_post_to_all_threads(UINT uiMessage,WPARAM wparam,lparam lparam)
   {

      post_to_all_threads(uiMessage,wparam,lparam);

      return 0;

   }

   object * system::clone()
   {
      // by the time, it is not possible to clone a system
      return NULL;
   }



   void system::discard_to_factory(object * pca)
   {

      if(m_pfactory == NULL)
         return;

      m_pfactory->discard(pca);

   }



   bool system::is_system()
   {

      return true;

   }






   int32_t system::_001OnDebugReport(int32_t i1,const char * psz1,int32_t i2,const char * psz2,const char * psz3,va_list args)
   {

      return _debug_logging_report(i1,psz1,i2,psz2,psz3,args);

   }



   int32_t system::_debug_logging_report(int32_t iReportType, const char * pszFileName, int32_t iLineNumber, const char * pszModuleName, const char * pszFormat,va_list list)
   {

      if(m_plog == NULL || !m_plog->m_bExtendedLog)
      {

         return ::aura::SimpleDebugReport(iReportType,pszFileName,iLineNumber,pszModuleName,pszFormat,list);

      }

      string str;

      if(pszFileName != NULL || pszModuleName != NULL)
      {

         stringa stra;

         if(pszFileName != NULL)
            stra.add(pszFileName);

         if(pszModuleName != NULL)
            stra.add(pszFileName);

         str += stra.implode(", ");

         str += ": ";

      }

      string str2;

      if(pszFormat != NULL)
      {

//         if(list != NULL)
         {

            str2.FormatV(pszFormat,list);

         }
         //     else
         {

            //          str2 = pszFormat;

         }

      }

      str = str + str2;

      string strPrint(str);

      strPrint.replace("%","%%");

      if(m_plog != NULL)
      {

         m_plog->print(strPrint);

      }

      if(iReportType == _CRT_ASSERT)
      {

         return 1;

      }
      else
      {

         return 0;

      }

   }




   bool system::assert_failed_line(const char * lpszFileName,int32_t iLine)
   {
      UNREFERENCED_PARAMETER(lpszFileName);
      UNREFERENCED_PARAMETER(iLine);
      return false;
   }


   bool system::on_assert_failed_line(const char * lpszFileName,int32_t iLine)
   {
      UNREFERENCED_PARAMETER(lpszFileName);
      UNREFERENCED_PARAMETER(iLine);
      return true;
   }

   mutex * system::get_openweather_city_mutex()
   {
      
      synch_lock sl(m_pmutex);

      if (m_spmutexOpenweatherCity.is_null())
      {
         
#ifdef METROWIN

         m_spmutexOpenweatherCity = canew(mutex(this));

#else

         m_spmutexOpenweatherCity = canew(mutex(this, false, "Global\\ca2_weather_city"));

#endif
            
      }

      return m_spmutexOpenweatherCity;

   }



   object * system::on_alloc(::aura::application * papp, ::type * ptype)
   {

      if(m_pfactory.is_null())
         return NULL;

      object * pobj = m_pfactory->create(papp,ptype);

      if(pobj != NULL)
         return pobj;

      on_allocation_error(papp,ptype);

      return NULL;

   }


   object * system::alloc(::aura::application * papp, ::type * ptype)
   {

      object * p = on_alloc(papp,ptype);

      if(p == NULL)
         return NULL;

      return p;

   }


   object * system::alloc(::aura::application * papp,const std_type_info & info)
   {

      return on_alloc(papp,get_type_info(info));

   }


   void system::on_allocation_error(::aura::application * papp, ::type * ptype)
   {

      UNREFERENCED_PARAMETER(papp);
      UNREFERENCED_PARAMETER(ptype);

   }


   object * system::alloc(::aura::application * papp,const class id & idType)
   {

      sp(::type) ptype = get_type_info(idType);

      if(ptype.is_null())
         return NULL;

      object * p = on_alloc(papp,ptype);

      if(p == NULL)
         return NULL;

      return p;

   }




   ::type * system::get_type_info(const ::std_type_info & info)
   {

      synch_lock sl(&m_mutexFactory);

      ::id id;

#ifdef WINDOWS

      id = info.raw_name();

#else

      id = info.name();

#endif
      
      sp(type) & typeinfo = m_typemap[id];

      if (typeinfo.is_null())
      {
       
         typeinfo = canew(type(info));

      }

      return typeinfo.m_p;

   }


   ::xml::department & system::xml()
   {
      return *m_pxml;
   }



   class ::str::base64 & system::base64()
   {

      return m_base64;

   }



   ::aura::log & system::log()
   {
      return *m_plog;
   }





   machine_event_central & system::machine_event_central()
   {
      return *m_pmachineeventcentral;
   }


   ::aura::session * system::query_session(index iEdge)
   {

      return NULL;

   }


   ::aura::os & system::os()
   {

      return *m_spos;

   }


   //spa(::aura::session) & system::basesessionptra()
   //{

   //   return m_basesessionptra;

   //}


   ::process::department & system::process()
   {

      return m_process;

   }


   bool system::initialize_log(const char * pszId)
   {
      if(m_plog != NULL)
         return true;
      m_plog = canew(::aura::log(this));
      m_plog->set_extended_log();
      m_plog->set_app(this);
      if(!m_plog->initialize(pszId))
      {
         m_plog.release();
         return false;
      }
      //      ::core::trace_v = &::core::system_log_trace_v;
      return true;
   }






   void system::appa_load_string_table()
   {

      retry_single_lock rsl(m_pmutex,millis(100),millis(100));

//      for(int32_t i = 0; i < appptra().get_size(); i++)
      //    {
      //     ::aura::application * papp = appptra()(i);
      //   papp->load_string_table();
      //}

   }

   void system::appa_set_locale(const char * pszLocale,::action::context actioncontext)
   {

      retry_single_lock rsl(m_pmutex,millis(100),millis(100));

//      for(int32_t i = 0; i < appptra().get_size(); i++)
//     {
      //       ::aura::application * papp = appptra()(i);
      //       papp->set_locale(pszLocale,actioncontext);
      //    }

   }

   void system::appa_set_schema(const char * pszStyle,::action::context actioncontext)
   {

      retry_single_lock rsl(m_pmutex,millis(100),millis(100));

//      for(int32_t i = 0; i < appptra().get_size(); i++)
      //    {
      //       ::aura::application * papp = appptra()(i);
      //       papp->set_schema(pszStyle,actioncontext);
      //    }

   }



   bool system::assert_running_global(const char * pszAppName,const char * pszId)
   {
      if(string(pszId).has_char())
      {
         //         HANDLE h = ::OpenMutex(SYNCHRONIZE, FALSE, get_global_id_mutex_name(pszAppName, pszId));
         ::mutex * pmutex = mutex::open_mutex(this,get_global_id_mutex_name(pszAppName,pszId));
         if(pmutex == NULL)
         {

            string strApp = pszAppName;
            strApp += "app.exe";

            string strParameters;
            strParameters = ": global_mutex_id=\"" + string(pszId) + "\"";

#if defined(WINDOWSEX) || defined(LINUX) || defined(APPLEOS)

            ::aura::shell_launcher launcher(NULL,NULL,dir().module()/strApp,strParameters,NULL,SW_SHOW);

            launcher.execute();

#else

            throw todo(get_app());

#endif

            return false;
         }
         else
         {
            delete pmutex;
         }
         return true;
      }
      else
      {
         //HANDLE h = ::OpenMutex(SYNCHRONIZE, FALSE, get_global_mutex_name(pszAppName));
         ::mutex * pmutex = mutex::open_mutex(this,get_global_mutex_name(pszAppName));
         if(pmutex == NULL)
         {
            string strApp = pszAppName;
            strApp += "app.exe";

#if defined(METROWIN)

            throw todo(get_app());

#else

            ::aura::shell_launcher launcher(NULL,NULL,dir().module()/strApp,NULL,NULL,SW_SHOW);

            launcher.execute();

#endif

            return false;
         }
         else
         {
            //::CloseHandle(h);
            delete pmutex;
         }
         return true;
      }
   }

   bool system::assert_running_local(const char * pszAppName,const char * pszId)
   {
      string strAppName(pszAppName);
      string strId(pszId);
      if(strId.has_char())
      {
         //HANDLE h = ::OpenMutex(SYNCHRONIZE, FALSE, get_local_id_mutex_name(pszAppName, strId));
         ::mutex * pmutex = mutex::open_mutex(this,get_local_id_mutex_name(pszAppName,strId));
         if(pmutex == NULL)
         {
            string strApp;
            strApp = "app.exe";
            string strParameters;
            strParameters = ": app=" + strAppName + " local_mutex_id=\"" + strId + "\"";

#ifdef METROWIN

            throw todo(get_app());

#else

            ::aura::shell_launcher launcher(NULL,NULL,dir().ca2module() / strApp,strParameters,NULL,SW_SHOW);

            launcher.execute();

#endif

            return false;
         }
         else
         {
            //::CloseHandle(h);
            delete pmutex;
         }
         return true;
      }
      else
      {
         //         HANDLE h = ::OpenMutex(SYNCHRONIZE, FALSE, get_local_mutex_name(pszAppName));
         ::mutex * pmutex = mutex::open_mutex(this,get_local_mutex_name(pszAppName));
         if(pmutex == NULL)
         {
            string strApp;
            strApp = "app.exe";
            string strParameters;
            strParameters = ": app=" + strAppName;

#ifdef METROWIN

            throw todo(get_app());

#else

            ::aura::shell_launcher launcher(NULL,NULL,dir().ca2module() / strApp,strParameters,NULL,SW_SHOW);

            launcher.execute();

#endif

            return false;
         }
         else
         {
            //::CloseHandle(h);
            delete pmutex;
         }
         return true;
      }
   }










   ::count system::get_application_count()
   {

      ::count c = 0;

      //try
      //{

      //   for(index iBaseSession = 0; iBaseSession < m_basesessionptra.get_count(); iBaseSession++)
      //   {

      //      try
      //      {

      //         c += m_basesessionptra[iBaseSession]->appptra().get_count();

      //      }
      //      catch(...)
      //      {

      //      }

      //   }

      //}
      //catch(...)
      //{

      //}

      return c;

   }


   application_ptra system::get_appptra()
   {

      application_ptra appptra;

      //try
      //{

      //   for(index iBaseSession = 0; iBaseSession < m_basesessionptra.get_count(); iBaseSession++)
      //   {

      //      try
      //      {

      //         appptra += m_basesessionptra[iBaseSession]->appptra();

      //      }
      //      catch(...)
      //      {

      //      }

      //   }

      //}
      //catch(...)
      //{


      //}

      return appptra;

   }


//   ::file::path system::get_ca2_module_folder()
//   {
//
//      single_lock sl(m_pmutex,true);
//
//      return m_pathCa2ModuleFolder;
//
//   }
//
//
//   ::file::path system::get_ca2_module_file_path()
//   {
//
//      string strModuleFileName;
//
//#ifdef WINDOWSEX
//
//      unichar lpszModuleFilePath[MAX_PATH + 1];
//
//      if(GetModuleFileNameW(::GetModuleHandleA("core.dll"),lpszModuleFilePath,MAX_PATH + 1))
//      {
//
//         strModuleFileName = lpszModuleFilePath;
//
//      }
//
//#elif defined(METROWIN)
//
//      throw todo(this);
//
//#elif defined(ANDROID)
//
//      throw todo(this);
//
//#else
//
//#if defined(LINUX)
//
//      {
//
//         void * handle = dlopen("core.so",0);
//
//         if(handle == NULL)
//            return "";
//
//         link_map * plm;
//
//         dlinfo(handle,RTLD_DI_LINKMAP,&plm);
//
//         strModuleFileName = plm->l_name;
//
//         dlclose(handle);
//
//         //         m_strCa2ModuleFolder = dir::name(strModuleFileName);
//
//      }
//
//#else
//
//      {
//
//         char * pszCurDir = getcwd(NULL,0);
//
//         string strCurDir = pszCurDir;
//
//         free(pszCurDir);
//
//         strModuleFileName = strCurDir,"libbase.dylib");
//
//         if(Application.file().exists(strModuleFileName))
//         {
////            m_strCa2ModuleFolder = strCurDir;
//            goto finishedCa2Module;
//         }
//
//         strModuleFileName = m_strModuleFolder,"libbase.dylib");
//
//         if(Application.file().exists(strModuleFileName))
//         {
////            m_strCa2ModuleFolder = m_strModuleFolder;
//            goto finishedCa2Module;
//         }
//
//         strModuleFileName = Application.dir_pathfind(getenv("LD_LIBRARY_PATH"),"libbase.dylib","rfs"); // readable - normal file - non zero sized
//
//         if(Application.file().exists(strModuleFileName))
//         {
////            m_strCa2ModuleFolder = System.dir().name(strModuleFileName);
//            goto finishedCa2Module;
//         }
//
//      }
//
//   finishedCa2Module:;
//
//#endif
//
//#endif
//
//      return strModuleFileName;
//
//
//   }


   //::file::path system::get_module_folder()
   //{

   //   return m_pathModuleFolder;

   //}


   //::file::path system::get_module_file_path()
   //{


   //}


   //::file::path system::get_module_title()
   //{

   //   return get_module_file_path().title();

   //}


   //::file::path system::get_module_name()
   //{

   //   return get_module_file_path().name();

   //}

   colorertake5::ParserFactory & system::parser_factory()
   {

      if(m_pparserfactory == NULL)
      {

         m_pparserfactory = new colorertake5::ParserFactory(this);

      }

      return *m_pparserfactory;

   }


   ::file::path system::dir_appmatter_locator(::aura::application * papp)
   {

      return "";

   }


   string system::crypto_md5_text(const string & str)
   {

      ::exception::throw_not_implemented(get_app());

      return "";

   }


   uint32_t _thread_proc_start_system(void * p)
   {

      ::aura::system * psystem = (::aura::system *)p;

      return psystem->main();

   }

   CLASS_DECL_AURA void __start_system(::aura::system * psystem)
   {

      ::create_thread(NULL,0,&_thread_proc_start_system,(LPVOID)psystem,0,0);

   }


   void * & system::ftlibrary()
   {

      return m_ftlibrary;


   }


   void system::install_progress_add_up(int iAddUp)
   {

      UNREFERENCED_PARAMETER(iAddUp);

   }

   bool system::alloc_session()
   {

      if (m_paurasession != NULL)
      {

         return true;

      }

      ::aura::session * paurasession = on_create_session();

      if(paurasession == NULL)
         return false;

      m_paurasession = paurasession;

      m_paxissession = paurasession->m_paxissession;

      m_pbasesession = paurasession->m_pbasesession;

      m_paurasession->construct(this,0);

      return true;

   }

   ::aura::session * system::on_create_session()
   {

      return new ::aura::session(this);

   }





   ::aura::session * system::get_session(index iEdge,application_bias * pbiasCreation)
   {

      ::aura::session * paurasession = NULL;

      //if(m_paurabergedgemap == NULL)
      //   return NULL;

      if(!m_aurabergedgemap.Lookup(iEdge,paurasession))
      {

         // todo (camilo) real multiple session native support

         //paurasession = create_application("application","session",true,pbiasCreation);

         paurasession = m_paurasession; // note (camilo) by the time, assigns always the session "0"

         if(paurasession == NULL)
            return NULL;

         paurasession->m_iEdge = iEdge;

         m_aurabergedgemap.set_at(iEdge,paurasession);

      }

      return paurasession;

   }


   string system::get_system_configuration()
   {

#if CA2_PLATFORM_VERSION == CA2_BASIS

         return "basis";

#else

         return "stage";

#endif

   }


   string system::get_system_platform()
   {

#ifdef X86

         return "x86";

#else

         return "x64";

#endif

   }


   string system::get_latest_build_number(const char * pszConfiguration)
   {

      string strConfiguration(pszConfiguration);

      if (strConfiguration.is_empty())
      {

         strConfiguration = get_system_configuration();

      }

      if (m_mapCachedLatestBuild[strConfiguration].length() > 0)
      {

         return m_mapCachedLatestBuild[strConfiguration];

      }

      string strBuild;

      string strSpaIgnitionBaseUrl;

      if (strConfiguration == "basis")
      {

         strSpaIgnitionBaseUrl = "https://server.ca2.cc/api/spaignition";

      }
      else
      {

         strSpaIgnitionBaseUrl = "https://server.ca2.cc/api/spaignition";

      }

      int iRetry = 0;

   RetryBuildNumber:

      if (iRetry > 10)
      {

         return "";

      }

      iRetry++;

      strBuild = http_get(strSpaIgnitionBaseUrl + "/query?node=build&configuration=" + strConfiguration);

      ::str::_008Trim(strBuild);

      if (strBuild.length() != 19)
      {

         Sleep(100 * iRetry);

         goto RetryBuildNumber;

      }

      m_mapCachedLatestBuild[strConfiguration] = strBuild;

      return strBuild;

   }


#if defined(INSTALL_SUBSYSTEM)

   int32_t system::start_installation(const char * pszCommand)
   {

      return install().start(pszCommand);

   }

#endif


   //int32_t system::install_start(const char * pszCommandLine, const char * pszBuild)
   //{

   //   return install().start(pszCommandLine, pszBuild);

   //}


   //int32_t system::install_progress_app_add_up(int32_t iAddUp)
   //{

   //   UNREFERENCED_PARAMETER(iAddUp);

   //   return (int32_t)(install().m_progressApp()++);

   //}


   //int32_t system::install_progress_app_add_up(int32_t iAddUp)
   //{

   //   return -1;

   //}

   void system::on_start_find_applications_from_cache()
   {

   }

   void system::on_end_find_applications_from_cache(::file::byte_istream & is)
   {

   }

   void system::on_end_find_applications_to_cache(::file::byte_ostream & os)
   {

   }

   void system::on_map_application_library(::aura::library & library)
   {



   }




   bool system::is_application_installed(const char * pszAppId, const char * pszBuild, const char * pszPlatform, const char * pszConfiguration, const char * pszLocale, const char * pszSchema)
   {

      synch_lock sl(m_pmutex);

      ::file::path path;

      path = dir().commonappdata_locale_schema(pszAppId, pszBuild, pszPlatform, pszConfiguration, pszLocale, pszSchema) / "installed.txt";

      if(!Application.file().exists(path))
         return false;

      return true;

   }


   void system::on_request(::create * pcreate)
   {

      ::aura::session * psession = get_session(pcreate->m_spCommandLine->m_iEdge, pcreate->m_spCommandLine->m_pbiasCreate);

      if(psession == NULL)
      {

         ::output_debug_string("::aura::system::on_request session NULL An error that prevents the application from starting has occurred.\
\n\r\nPlease run app-removal.exe and restart the application, or contact the administrator. Startup Error !!");

         ::simple_message_box(get_splash(),"An error that prevents the application from starting has occurred.\r\n\r\nPlease run app-removal.exe and restart the application, or contact the administrator.","Startup Error",MB_ICONEXCLAMATION);

#ifdef WINDOWSEX

         ::ExitProcess(-17);

#endif

         return;

      }

      ::output_debug_string("::aura::system::on_request session = " + demangle(typeid(*psession).name()) + "("+::str::from((int_ptr) psession)+")\n\n");


      psession->request_create(pcreate);


   }


   bool system::find_applications_from_cache()
   {

      on_start_find_applications_from_cache();

      if(directrix()->m_varTopicQuery.has_property("install"))
         return true;

      ::file::file_sp file = Session.m_spfile->get_file(System.dir().appdata() / "applibcache.bin",::file::type_binary | ::file::mode_read);

      if(file.is_null())
         return false;

      ::file::byte_istream is(file);

      is >> m_mapAppLibrary;

      on_end_find_applications_from_cache(is);

      return true;

   }



   bool system::find_applications_to_cache(bool bSave)
   {

      return true;

#if defined(CUBE) || defined(VSNORD)
      return true;
#endif

      /*      m_spfilehandler(new ::core::filehandler::handler(this));*/

      m_mapAppLibrary.remove_all();

      string strLibraryId;

      ::file::listing straTitle(this);

      ::file::path pathCa2Module = System.dir().ca2module();

      ::output_debug_string("\n\n::aura::system::find_applications_to_cache\n\n");

      ::output_debug_string("ca2 module folder : " + pathCa2Module);

      ::output_debug_string("\n\n\n");

      straTitle.ls_pattern(pathCa2Module, { "*.*" });

      for(int32_t i = 0; i < straTitle.get_count(); i++)
      {

         strLibraryId = straTitle[i];


         if(::str::ends_eat_ci(strLibraryId,".dll")
               || ::str::ends_eat_ci(strLibraryId,".so")
               || ::str::ends_eat_ci(strLibraryId,".dylib"))
         {

            if(::str::begins_ci(strLibraryId,"libdraw2d_")
                  || ::str::begins_ci(strLibraryId,"libbase"))
            {
               continue;
            }

            ::output_debug_string("library("+::str::from(i)+") : " + strLibraryId+"\n\n");

            map_application_library(strLibraryId);

         }

      }

      if(!bSave)
         return true;

      ::file::file_sp file;

      try
      {

         file = Session.file().get_file(System.dir().appdata() / "applibcache.bin",::file::defer_create_directory | ::file::type_binary | ::file::mode_create | ::file::mode_write);

      }
      catch(::exception::base &)
      {

         return false;

      }

      ::file::byte_ostream os(file);

      os << m_mapAppLibrary;

      on_end_find_applications_to_cache(os);

      return true;

   }

   bool system::map_application_library(const char * pszLibrary)
   {

      ::aura::library library(this,0,NULL);

      if(!strcmp(pszLibrary,"app_core_rdpclient"))
      {
         TRACE("reach");
      }

      if(!stricmp_dup(pszLibrary,"app_core_hellomultiverse"))
      {
         TRACE("reach app_core_hellomultiverse");
      }

      if(!stricmp_dup(pszLibrary,"wndfrm_core"))
      {
         TRACE("reach wndfrm_core");
      }

      if(!stricmp_dup(pszLibrary,"app_core_hellomultiverse"))
      {
         TRACE("reach app_core_hellomultiverse");
      }

      if(!library.open(pszLibrary,true))
      {
         ::output_debug_string("::system::map_application_library Failed to open library :" + string(pszLibrary) + "\n\n");
         return false;
      }

      if(!library.open_ca2_library())
      {

         ::output_debug_string("::system::map_application_library open_ca2_library(2) Failed :" + string(pszLibrary) + "\n\n");

         return false;

      }

      on_map_application_library(library);

      stringa stra;

      string strRoot = library.get_root();

      library.get_app_list(stra);

      if(stra.get_count() <= 0)
         return false;

      strRoot += "/";

      if(stra.get_count() == 1)
      {

         m_mapAppLibrary.set_at(strRoot + stra[0],pszLibrary);

      }

      string strLibrary = ::file::path(pszLibrary).title();

#if defined(LINUX) || defined(APPLEOS) || defined(ANDROID)

      if(strLibrary == "libbase")
      {

         strLibrary = "base";

      }
      else if(!::str::begins_eat(strLibrary,"libbase"))
      {

         ::str::begins_eat(strLibrary,"lib");

      }

#endif

      string strPrefix = strRoot;

      strPrefix.replace("-","_");

      strPrefix.replace("/","_");

      ::str::begins_eat_ci(strLibrary,strPrefix);

      strRoot += strLibrary;

      strRoot += "/";

      for(int32_t i = 0; i < stra.get_count(); i++)
      {

         m_mapAppLibrary.set_at(strRoot + stra[i],pszLibrary);

      }

      return true;

   }




   bool system::set_main_init_data(::aura::main_init_data * pdata)
   {

      if(pdata != NULL)
      {
         if(!::aura::application::set_main_init_data(pdata))
            return false;
      }

      if(pdata == NULL)
         return true;

      property_set set(this);

      var varFile;
      string strApp;

      set._008ParseCommandFork(pdata->m_vssCommandLine,varFile,strApp);

      string strAppId(strApp);

      if (set.has_property("app"))
      {

         strAppId = set["app"];

      }

      if (set.has_property("appid"))
      {

         strAppId = set["appid"];

      }


      if((varFile.is_empty() && ((strAppId.is_empty() && !set.has_property("show_platform"))
                                 || strAppId == "bergedge")) &&
            !(set.has_property("install") || set.has_property("uninstall")))
      {
         //if(!set.has_property("show_platform") || set["show_platform"] == 1)
         if (set["show_platform"] == 1)
         {
            command()->add_line(" : show_platform=1");
         }
      }
      else
      {
         string strCommandLine = pdata->m_vssCommandLine;
         //strCommandLine.trim();
         //if(strCommandLine[0] == '\"')
         //{
         //   strsize iFind = strCommandLine.find("\"",1);
         //   strCommandLine = strCommandLine.Mid(iFind + 1);
         //}
         //else
         //{
         //   strsize iFind = strCommandLine.find(" ",1);
         //   strCommandLine = strCommandLine.Mid(iFind + 1);
         //}
         command()->add_line(strCommandLine);


#if defined(MACOS)
         
         if(command()->m_spcommandline->m_strExe[0] == '/')
         {
            
            ::file::path p;
            
            p = ::dir::ca2_user();
            
            p /= "mypath" / command()->m_spcommandline->m_varQuery.propset()["app"].get_string() + ".txt";
            
            file_put_contents_dup(p, command()->m_spcommandline->m_strExe);
            
            string strApp = command()->m_spcommandline->m_strExe;
            
            strsize iFind = strApp.find_ci(".app/");
            
            if(iFind > 0)
            {
               
               p = ::dir::ca2_user;
            
               p /= "mypath" / command()->m_spcommandline->m_varQuery.propset()["app"].get_string() + "-app";
               
               ::file::path p2;
               
               p2 = ::dir::ca2_user();
            
               p2 /= "mypath" / ::file::path(command()->m_spcommandline->m_varQuery.propset()["app"].get_string()).folder()/ ::file::path(strApp.Left(iFind + strlen(".app"))).name();

               ns_create_alias(p2, strApp.Left(iFind + strlen(".app")));
               
               
               if(::dir::is(::dir::local()/"localconfig/desk/2desk"))
               {
                  
                  ::file::path p3;
                  
                  p3 = ::dir::local()/"localconfig/desk/2desk"/::file::path(strApp.Left(iFind + strlen(".app"))).name();
                  
                  ns_create_alias(p3, strApp.Left(iFind + strlen(".app")));

               }
            
               file_put_contents_dup(p, "open -a \""+strApp.Left(iFind + strlen(".app")) + "\"");

               chmod(p, 0777);
               
            }
            
         }

#endif
         
      }

      //if(!(set.has_property("install") || set.has_property("uninstall")))
      //{

      //   m_pfnVerb = &system::common_verb;

      //}


      //if(!::core::application::set_main_init_data(pdata))
      // return false;

      return true;

   }

   void system::request_exit()
   {

      auto ptra = Session.m_appptra;

      for(auto papp : ptra)
      {

         papp->m_bAgreeExit            = false;

         papp->m_bAgreeExitOk          = false;

      }

      for(auto papp : ptra)
      {

         papp->command()->command(::primitive::command_on_agree_exit);

      }

      int i = 100;

      MESSAGE msg;
      while(i > 0 && ptra.get_size() > 0)
      {

         if(::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
         {
            ::get_thread()->defer_pump_message();
         }

         for(index j = 0; j < ptra.get_size(); )
         {

            if(ptra[j]->m_bAgreeExitOk)
            {

               if(!ptra[j]->m_bAgreeExit)
               {

                  return;

               }
               else
               {

                  ptra.remove_at(j);

               }

            }
            else
            {

               j++;

            }

         }

         Sleep(100);

         i--;

      }

      if(i == 0)
      {

         return;

      }

      ptra = Session.m_appptra;

      for(auto papp : ptra)
      {
         papp->m_bFranceExit = false;
      }

      for(auto papp : ptra)
      {
         papp->command()->command(::primitive::command_france_exit);
      }

      i = 100;

      while(i > 0 && ptra.get_size() > 0)
      {

         if(::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
         {

            ::get_thread()->defer_pump_message();

         }

         for(index j = 0; j < ptra.get_size(); )
         {

            if(ptra[j]->m_bFranceExit)
            {

               ptra.remove_at(j);

            }
            else
            {

               j++;

            }

         }

         Sleep(100);

         i--;

      }

      post_quit();

   }


   void system::on_command(::primitive::command * pcommand)
   {

      if (pcommand->m_ecommand == ::primitive::command_check_exit)
      {

         defer_check_exit();

      }
      else
      {

         ::aura::application::on_command(pcommand);

      }



   }


   void system::defer_check_openweather_city_list()
   {

      
      synch_lock sl(get_openweather_city_mutex());

      if (m_straCityLo.get_size() == m_straCity.get_size()
         && m_straCity.get_size() == m_iaIds.get_size()
         && m_iaIds.get_size() == m_daLon.get_size()
         && m_daLon.get_size() == m_daLat.get_size()
         && m_straCity.get_size() > 1)
      {
         
         return;

      }

      bool bOk = false;

      try
      {

         file_as_array(m_straCity, dir::system() / "weather-cit.bin");
         file_as_array(m_straCityLo, dir::system() / "weather-cil.bin");
         file_as_array(m_iaIds, dir::system() / "weather-ids.bin");
         file_as_array(m_daLon, dir::system() / "weather-lon.bin");
         file_as_array(m_daLat, dir::system() / "weather-lat.bin");


         bOk = m_straCityLo.get_size() == m_straCity.get_size()
            && m_straCity.get_size() == m_iaIds.get_size()
            && m_iaIds.get_size() == m_daLon.get_size()
            && m_daLon.get_size() == m_daLat.get_size()
            && m_straCity.get_size() > 1;


      }
      catch (...)
      {

      }

      if (!bOk)
      {

         try
         {

            Application.file().del(dir::system() / "weather-cit.bin");
            Application.file().del(dir::system() / "weather-cil.bin");
            Application.file().del(dir::system() / "weather-ids.bin");
            Application.file().del(dir::system() / "weather-lon.bin");
            Application.file().del(dir::system() / "weather-lat.bin");

            m_straCityLo.remove_all();
            m_straCity.remove_all();
            m_iaIds.remove_all();
            m_daLon.remove_all();
            m_daLat.remove_all();

         }
         catch (...)
         {

         }

      }

      if (m_straCityLo.get_size() == m_straCity.get_size()
       && m_straCity.get_size() == m_iaIds.get_size()
       && m_iaIds.get_size() == m_daLon.get_size()
       && m_daLon.get_size() == m_daLat.get_size()
       && m_straCity.get_size() > 1)
      {
      }
      else
      {

         string str;

         str = file().as_string("matter://city.list.json", this);


         if (str.has_char())
         {

            stringa stra;

            stra.add_lines(str);

            for (auto strJson : stra)
            {

               const char * pszJson = strJson;

               //const char * pszJson = "{\"_id\":6322752, \"name\" : \"Curitiba\", \"country\" : \"BR\", \"coord\" : {\"lon\":-49.290821, \"lat\" : -25.50395}}";

               var v;

               v.parse_json(pszJson);

               string strLine = v["name"] + ", " + v["country"];

               m_straCity.add(strLine);

               m_straCityLo.add(strLine.lowered());

               int64_t iId = v["_id"];

               m_iaIds.add(iId);

               double dLon = v["coord"]["lon"];

               m_daLon.add(dLon);

               double dLat = v["coord"]["lat"];

               m_daLat.add(dLat);

            }

            file_put_array(dir::system() / "weather-cit.bin", m_straCity);
            file_put_array(dir::system() / "weather-cil.bin", m_straCityLo);
            file_put_array(dir::system() / "weather-ids.bin", m_iaIds);
            file_put_array(dir::system() / "weather-lon.bin", m_daLon);
            file_put_array(dir::system() / "weather-lat.bin", m_daLat);

         }

      }

   }


   openweather_city * system::openweather_find_city(string strQuery)
   {

      auto & city_auto_pointer = m_mapCity[strQuery];

      if (city_auto_pointer == NULL)
      {

         city_auto_pointer = new openweather_city;

         city_auto_pointer->m_iIndex = openweather_find_city2(
            strQuery,
            city_auto_pointer->m_strCit,
            city_auto_pointer->m_iId,
            city_auto_pointer->m_dLat,
            city_auto_pointer->m_dLon);

      }

      return city_auto_pointer;

   }

   
   index system::openweather_find_city2(string strQuery, string & strCit, int64_t & iId, double & dLat, double & dLon)
   {

      stringa stra;

      stra.explode(",", strQuery);

      stra.trim();

      stra.remove_empty();

      if (stra.get_count() <= 0)
      {

         return -1;

      }

      if(stra.get_count() == 1)
      {

         return openweather_find_city2(strQuery, "", strCit, iId, dLat, dLon, true);

      }
      

      for (index iCount = stra.get_count() - 1; iCount >= 1; iCount--)
      {

         index iIndex = openweather_find_city2(stra.slice(0, iCount).implode(", "), stra.last(), strCit, iId, dLat, dLon, false);

         if (iIndex >= 0)
         {

            return iIndex;

         }

      }

      for (index iCount = stra.get_count() - 1; iCount >= 1; iCount--)
      {

         index iIndex = openweather_find_city2(stra.slice(0, iCount).implode(", "), stra.last(), strCit, iId, dLat, dLon, true);

         if (iIndex >= 0)
         {

            return iIndex;

         }

      }

      return -1;

   }


   
   index system::openweather_find_city2(string strQ1, string strQ2, string & strCit, int64_t & iId, double & dLat, double & dLon, bool bPrefix)
   {

      string strQueryLo;

      string strTry;

      string strTry1;

      string strTry2;

      index iFind;

      defer_check_openweather_city_list();

      if (strQ1.compare_ci("Cologne") == 0 && strQ2.compare_ci("DE") == 0)
      {

         strQ1 = "Koeln";

      }
      else if (strQ1.compare_ci("Washington DC") == 0)
      {

         strQ1 = "Washington, D. C.";

      }

      strQueryLo = strQ1 + ", " + strQ2;

      strQueryLo.make_lower();

      if (bPrefix)
      {

         iFind = m_straCityLo.find_first_begins(strQueryLo);

      }
      else
      {

         iFind = m_straCityLo.find_first(strQueryLo);

      }

      if (iFind >= 0)
      {

         goto found;

      }























      strTry = strQueryLo;

      strTry.replace("'", "");

      if (bPrefix)
      {

         iFind = m_straCityLo.find_first_begins(strTry);

      }
      else
      {

         iFind = m_straCityLo.find_first(strTry);

      }

      if (iFind >= 0)
      {

         goto found;

      }

































      strTry = strQueryLo;

      strTry.replace("st.", "saint");
      strTry.replace("são", "sao");
      strTry.replace("ž", "z");
      strTry.replace("á", "a");
      strTry.replace("à", "a");
      strTry.replace("ä", "a");
      strTry.replace("é", "e");
      strTry.replace("è", "e");
      strTry.replace("ë", "e");
      strTry.replace("í", "i");
      strTry.replace("ì", "i");
      strTry.replace("ï", "i");
      strTry.replace("ó", "o");
      strTry.replace("ò", "o");
      strTry.replace("ö", "o");
      strTry.replace("ú", "u");
      strTry.replace("ù", "u");
      strTry.replace("ü", "u");

      if (bPrefix)
      {

         iFind = m_straCityLo.find_first_begins(strTry);

      }
      else
      {

         iFind = m_straCityLo.find_first(strTry);

      }

      if (iFind >= 0)
      {

         goto found;

      }









      return -1;

   found:

      strCit   = m_straCity[iFind];

      iId      = m_iaIds[iFind];

      dLat     = m_daLat[iFind];

      dLon     = m_daLon[iFind];

      return iFind;

   }


   string system::url_encode(const string & str)
   {

	   //throw interface_only_exception(this);

      return url_encode_dup(str);

   }

#ifdef VSNORD

   bool system::android_set_user_wallpaper(string strUrl)
   {

      m_pandroidinitdata->m_pszUserWallpaper = strdup(strUrl);

      return true;

   }

#endif

   class ::crypto::crypto & system::crypto()
   {

      return *m_spcrypto;

   }


   ::fontopus::user_set & system::userset()
   {

      return m_userset;

   }




} // namespace aura



























