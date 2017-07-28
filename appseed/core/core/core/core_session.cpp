//#include "framework.h"
//#include "core/user/user/user.h"
//#include "core/filesystem/filemanager/filemanager.h"


namespace core
{

   session_docs * create_session_docs();

   void destroy_session_docs(session_docs * pdocs);


   session::session(::aura::application * papp):
      object(papp),
      ::aura::session(papp),
      ::axis::session(papp),
      ::base::session(papp),
      ::thread(papp)
   {

      m_pdocs = create_session_docs();

      m_paurasystem                       = papp->m_paurasystem;

      m_paxissystem                       = papp->m_paxissystem;

      m_pbasesystem                       = papp->m_pbasesystem;

      m_paurasystem->m_pcoresession       = this;

      m_paxissession                      = this;
         
      m_pbasesession                      = this;

      m_pcoresession                      = this;

      m_pauraapp                          = this;

      m_paxisapp                          = this;

      m_pbaseapp                          = this;
      
      m_pcoreapp                          = this;

      m_pcoresession                      = this;

      m_bLicense				               = false;

      m_strAppName                        = "bergedge";

      m_puserex                           = NULL;
      m_pfilemanager = NULL;

   }

   session::~platform_parent
   {

      destroy_session_docs(m_pdocs);


   }


   void session::construct(const char * pszAppId)
   {

      ::core::application::construct("session");

      m_strAppName         = "session";
      m_strBaseSupportId   = "ca2_bergedge";
      m_strInstallToken    = "session";
      m_bLicense           = false;
      m_eexclusiveinstance = ExclusiveInstanceNone;

   }


   void session::install_message_handling(::message::dispatch * pdispatch)
   {
      
      core::application::install_message_handling(pdispatch);
      base::session::install_message_handling(pdispatch);

   }


   bool session::process_initialize()
   {

      if(!::core::application::process_initialize())
         return false;

      if(!::base::session::process_initialize())
         return false;

      if (!process_initialize_userex())
      {

         return false;

      }

      //m_puserex->construct(this);

      return true;

   }


   bool session::initialize1()
   {

      if(!::core::application::initialize1())
         return false;

      if(!::base::session::initialize1())
         return false;

      if (!initialize1_userex())
      {

         return false;

      }

      return true;


   }


   bool session::initialize()
   {

      if(!::core::application::initialize())
         return false;

      if(!::base::session::initialize())
         return false;

      return true;

   }

   
   bool session::initialize2()
   {
      
      if(!::core::application::initialize2())
         return false;
      
      if(!::base::session::initialize2())
         return false;

      return true;

   }



   bool session::initialize_application()
   {

      if (!::core::application::initialize_application())
      {

         return false;

      }

      if (!::base::session::initialize_application())
      {

         return false;

      }

      initialize_bergedge_application_interface();

      SetRegistryKey("ca2core");

      if(!InitializeLocalDataCentral())
      {

         simple_message_box(NULL, "Could not initialize Local data central");

         return false;

      }

      initialize_bergedge_application_interface();

      if(is_remote_session())
      {

         //Session.savings().save(::aura::resource_display_bandwidth);
         //Session.savings().save(::aura::resource_blur_background);
         //Session.savings().save(::aura::resource_blurred_text_embossing);
         //Session.savings().save(::aura::resource_translucent_background);

      }

      return true;

   }


   bool session::finalize()
   {


      bool bOk = true;

      try
      {

         bOk = ::base::session::finalize();

      }
      catch(...)
      {

         bOk = false;
      }

      try
      {

         bOk = ::core::application::finalize();

      }
      catch(...)
      {

         bOk = false;

      }

      return bOk;

   }




   bool session::bergedge_start()
   {

      return true;

   }


   void session::_001OnFileNew()
   {

   }


   bool session::_001OnCmdMsg(::aura::cmd_msg * pcmdmsg)
   {

      return application::_001OnCmdMsg(pcmdmsg);

   }


   void session::load_string_table()
   {

      application::load_string_table();
      application::load_string_table("plane", "");
      application::load_string_table();
      application::load_string_table("session", "");

   }


   void session::launch_app(const char * psz)
   {

      UNREFERENCED_PARAMETER(psz);

   }


   void session::install_app(const char * psz)
   {

      UNREFERENCED_PARAMETER(psz);

   }


   void session::on_request(::create * pcreate)
   {

      ::base::session::on_request(pcreate);

   }


   bool session::open_by_file_extension(const char * pszPathName, application_bias * pbiasCreate)
   {

      sp(::create) cc(allocer());

      cc->m_spCommandLine->m_varFile = pszPathName;

      if(pbiasCreate != NULL)
      {

         cc->m_spApplicationBias->operator=(*pbiasCreate);

      }

      return open_by_file_extension(cc);

   }


   bool session::open_by_file_extension(::create * pcreate)
   {

      string strId;

      string strOriginalPathName(pcreate->m_spCommandLine->m_varFile);

      ::file::path strPathName(strOriginalPathName);

      strsize iFind = strPathName.find("?");

      if(iFind >= 0)
      {

         strPathName = strPathName.Left(iFind);

      }

      if(::str::ends_ci(strPathName, ".ca2"))
      {

      }

      string strProtocol = System.url().get_protocol(strPathName);

      if(strProtocol == "app")
      {

         strId = System.url().get_server(strPathName);

         string str = System.url().get_object(strPathName);

         ::str::begins_eat(str, "/");

         pcreate->m_spCommandLine->m_varFile = str;

      }
      else
      {

         string strExtension = strPathName.extension();

         stringa straApp;

         System.filehandler().get_extension_app(straApp, strExtension);

         if(straApp.get_count() == 1)
         {

            strId = straApp[0];

         }
         else
         {

            strId = "app/sphere/default_file_handler";

         }

      }

      ::aura::application * papp = application_get(strId, true, true, pcreate->m_spApplicationBias);

      if (papp == NULL)
      {

         return false;

      }

      papp->m_pcoreapp->::object::create(pcreate);

      return true;

   }


   session::run_application::run_application()
   {

      m_pauraapp = NULL;

      m_puiParent = NULL;

   }

   //   void session::on_exclusive_instance_conflict(EExclusiveInstance eexclusive)
   //   {
   //
   //      if(eexclusive == ExclusiveInstanceLocalId)
   //      {
   //         /*
   //         ::memory_file file(get_app());
   //         file.from_string(command()->m_varTopicFile);
   //         COPYDATASTRUCT data;
   //         data.dwData = 1984;
   //         data.cbData = (uint32_t) file.get_length();
   //         data.lpData = file.get_data();
   //         oswindow oswindow = ::FindWindowA(NULL, "::draw2d::fontopus::message_wnd::session::");
   //
   //         ::SendMessage(oswindow, WM_COPYDATA, NULL, (LPARAM) &data);*/
   //
   //#if defined(WINDOWSEX) || defined(LINUX) || defined(APPLEOS)
   //
   //         ::aura::ipc::tx channel;
   //
   //         if(channel.open("::draw2d::fontopus::message_wnd::session::"))
   //         {
   //            channel.send(command()->m_varTopicFile, false);
   //            channel.close();
   //         }
   //
   //#else
   //
   //          throw todo(get_app());
   //
   //#endif
   //
   //
   //      }
   //
   //   }
   //

   void session::request_create(::create * pcreate)
   {

      ::base::session::request_create(pcreate);

   }

   void session::request_topic_file(var & varQuery)
   {
      request_file_query(Session.m_varTopicFile, varQuery);
   }

   void session::request_topic_file()
   {
      request_file(Session.m_varTopicFile);
   }

   /*void session::request_application(const char * pszId, var varFile, var varQuery, application_bias * pbiasCreate)
   {

   ::core::application_request request;

   request.m_iEdge         = m_iEdge;
   request.m_strApp        = pszId;
   request.m_varFile       = varFile;
   request.m_varQuery      = varQuery;
   request.m_pbiasCreate   = pbiasCreate;

   request_application(&request);

   }*/
   /*
   sp(::aura::application) session::application_get(const char * pszType, const char * pszId, bool bCreate, bool bSynch, application_bias * pbiasCreate)
   {
   ::aura::application * papp = NULL;

   if(m_mapApplication.Lookup(string(pszType) + ":" + string(pszId), papp))
   return papp;
   else
   {

   if(!bCreate)
   return NULL;

   papp = NULL;

   try
   {

   papp = create_application(pszType, pszId, bSynch, pbiasCreate);

   }
   catch(::exit_exception & e)
   {

   throw e;

   }
   catch(::exception::exception &)
   {

   if(!Application.on_run_exception(e))
   throw exit_exception(get_app());

   }
   catch(...)
   {

   papp = NULL;

   }

   if(papp == NULL)
   return NULL;

   m_mapApplication.set_at(string(pszType) + ":" + string(pszId), papp);
   return papp;
   }
   }
   */


   bool session::is_session()
   {
      return true;
   }

   sp(::aura::application) session::get_current_application()
   {
      return Session.m_pappCurrent;
   }




   bool session::on_install()
   {

      string strFormat;

      string strSentinelPath;

      strSentinelPath = System.dir().element() / "stage/x86/app.sentinel.exe";

      System.os().local_machine_set_run("core app.sentinel", "\"" + strSentinelPath + "\"", true);

      System.os().defer_register_ca2_plugin_for_mozilla();

      return ::core::application::on_install();

   }




   bool session::is_remote_session()
   {


      return System.os().is_remote_session();


   }


   bool session::is_mouse_button_pressed(::user::e_mouse emouse)
   {

      if(emouse == ::user::mouse_left_button)
      {
         return Session.is_key_pressed(::user::key_lbutton);
      }
      else if(emouse == ::user::mouse_right_button)
      {
         return Session.is_key_pressed(::user::key_rbutton);
      }
      else if(emouse == ::user::mouse_middle_button)
      {
         return Session.is_key_pressed(::user::key_mbutton);
      }
      else
      {
         throw "not expected e_mouse value";
      }


   }






   bool session::open_file(::filemanager::data * pdata, ::fs::item_array & itema)
   {

      UNREFERENCED_PARAMETER(pdata);

      if(itema.get_size() > 0)
      {

         return true;

      }

      return false;

   }








   bool session::InitializeLocalDataCentral()
   {

      //m_pdatabase = new nature::database(this);

      //if(m_pdatabase == NULL)
      //{
      //   TRACE("VmpLightApp::initialize_instance failed to instatiate LightDB\n");
      //   return false;
      //}

      //if(!m_pdatabase->Initialize())
      //{
      //   TRACE("VmpLightApp::initialize_instance failed to initialize LightDB\n");
      //   return false;
      //}

      return true;

   }


   bool session::on_exclusive_instance_conflict(EExclusiveInstance eexclusive)
   {
      if(eexclusive == ExclusiveInstanceLocalId)
      {
#ifdef WINDOWSEX
         ::memory_file file(get_app());
         file.from_string(command()->m_varTopicFile);
         COPYDATASTRUCT data;
         data.dwData = 1984;
         data.cbData = (uint32_t) file.get_length();
         data.lpData = file.get_data();
         ::oswindow oswindow = ::FindWindowA(NULL, "::draw2d::fontopus::message_wnd::bergedge::");


         ::SendMessage(oswindow, WM_COPYDATA, 0, (LPARAM) &data);

         return true;

#else

         throw todo(get_app());

#endif

      }

      return false;

   }


   /*void session::request_application(const char * pszId, var varFile, var varQuery, application_bias * pbiasCreate)
   {

   ::core::application_request request;

   request.m_iEdge         = m_iEdge;
   request.m_strApp        = pszId;
   request.m_varFile       = varFile;
   request.m_varQuery      = varQuery;
   request.m_pbiasCreate   = pbiasCreate;

   request_application(&request);

   }*/

   ::aura::application * session::application_get(const char * pszAppId, bool bCreate, bool bSynch, application_bias * pbiasCreate)
   {

      ::aura::application * papp = NULL;

      if(m_pbasesession->m_mapApplication.Lookup(string(pszAppId),papp))
      {
         
         return papp;

      }
      else
      {

         if(!bCreate)
            return NULL;

         papp = NULL;

         try
         {

            papp = create_application(pszAppId, bSynch, pbiasCreate);

         }
         catch(::exit_exception & e)
         {

            throw e;

         }
         catch(::exception::exception & e)
         {

            if (!App(this).on_run_exception(e))
            {

               throw exit_exception(get_app());

            }

         }
         catch(...)
         {

            papp = NULL;

         }

         if (papp == NULL)
         {

            return NULL;

         }

         if(&App(papp) == NULL)
         {

            ::aura::del(papp);

            return NULL;

         }
         
         m_pbasesession->m_mapApplication.set_at(string(pszAppId), papp);

         return papp;

      }

   }



   void session::check_topic_file_change()
   {
      if(Session.m_varCurrentViewFile != Session.m_varTopicFile && !Session.m_varTopicFile.is_empty())
      {
         Session.m_varCurrentViewFile = Session.m_varTopicFile;
         request_topic_file();
      }
   }


   //::user::place_holder_ptra session::get_place_holder(sp(::user::frame_window) pmainframe, ::create * pcreate)
   //{

   //   UNREFERENCED_PARAMETER(pcreate);

   //   ::user::place_holder_ptra holderptra;


   //   ::core::application & app = App(pmainframe->get_app());

   //   string strAppName = app.m_strAppName;

   //   //if(get_document()->get_typed_view < ::bergedge::pane_view >() != NULL)
   //   //{

   //   //   get_document()->get_typed_view < ::bergedge::pane_view >()->set_cur_tab_by_id("app:" + strAppName);

   //   //   holderptra.add(get_document()->get_typed_view < ::bergedge::pane_view >()->get_tab_holder(get_document()->get_typed_view < ::bergedge::pane_view >()->get_tab_by_id("app:" + strAppName)));

   //   //}
   //   //else
   //   //{

   //   //   holderptra.add(get_document()->get_typed_view < ::bergedge::view >());

   //   //}

   //   return holderptra;

   //}

   bool session::place(::user::main_frame * pmainframe, ::create * pcreate)
   {

      //get_place_holder(pmainframe, pcreate).hold(pmainframe);

      return true;

   }





   void session::set_app_title(const char * pszAppId, const char * pszTitle)
   {

      ::aura::application * papp = NULL;

      if(m_pbasesession->m_mapApplication.Lookup(string(pszAppId), papp) && papp != NULL)
      {

         //sp(::bergedge::pane_view) ppaneview = get_document()->get_typed_view < ::bergedge::pane_view >();

         //if(ppaneview != NULL)
         //{

         //   string strAppName(pszAppId);

         //   ::user::tab::pane * ppane = ppaneview->get_pane_by_id("app:" + strAppName);

         //   if(ppane != NULL)
         //   {

         //      ppane->m_istrTitleEx = pszTitle;

         //      ppaneview->on_layout();

         //   }

         //}

      }

   }



   sp(::core::session) session::get_session()
   {

      return this;

   }
   
   

   bool session::is_serviceable()
   {


//      return ::session::application::is_serviceable();
      return false;


   }


   bool session::on_uninstall()
   {


      bool bOk1 = true;
      bool bOk2 = true;

      try
      {
         bOk1 = ::core::application::on_uninstall();
      }
      catch(...)
      {
         bOk1 = false;
      }




      return bOk1 && bOk2;


   }


   bool session::os_native_bergedge_start()
   {

      return ::core::application::os_native_bergedge_start();

   }


   service_base * session::allocate_new_service()
   {


      return ::core::application::allocate_new_service();


   }


   ::visual::cursor * session::get_cursor()
   {
      
      if(m_pbasesession->m_ecursor == ::visual::cursor_none)
         return NULL;
      else if(m_pbasesession->m_ecursor == ::visual::cursor_default)
         return System.visual().get_cursor(m_pbasesession->m_ecursorDefault);
      else
         return System.visual().get_cursor(m_pbasesession->m_ecursor);

   }


   ::visual::cursor * session::get_default_cursor()
   {
      
      return System.visual().get_cursor(m_pbasesession->m_ecursorDefault);

   }


   int32_t session::main()
   {


      return ::core::application::main();


   }

   /*   ::core::filehandler::handler & system::filehandler()
   {
   return *m_spfilehandler;
   }*/



   void session::register_bergedge_application(::aura::application * papp)
   {

      retry_single_lock rsl(m_pmutex,millis(84),millis(84));

      if(papp == NULL || papp->m_pcoreapp == NULL)
         return;

      Session.m_appptra.add_unique(papp);

      if(System.is_installing() || System.is_uninstalling())
         System.m_bDoNotExitIfNoApplications = false;
      else if(!papp->is_session()
         && !papp->is_system()
         && !papp->is_serviceable())
      {

         System.m_bDoNotExitIfNoApplications = false;

      }

   }

   void session::unregister_bergedge_application(::aura::application * papp)
   {

      {

         synch_lock rsl(m_pmutex);

         m_appptra.remove(papp);

      }

      ::get_thread()->kick_thread();

      kick_thread();

      System.kick_thread();

   }




   ::core::session * session::query_bergedge()
   {

      ::core::session * psession = NULL;

      if(System.m_pbergedgemap == NULL)
         return NULL;

      if(!System.m_pbergedgemap->Lookup(0,psession))
      {
         return NULL;
      }

      return psession;

   }

   void session::on_user_login(::fontopus::user * puser)
   {

      ::base::session::on_user_login(puser);


   }

   void application::use_font_sel()
   {

      if (m_pfontlistdata != NULL)
      {

         return;

      }

      if (!is_installing() && !is_uninstalling())
      {

         m_pfontlistdata = new ::visual::font_list_data(this);

         System.visual().fonts().defer_create_font_enumeration();

         fork([&]()
         {
            ::multithreading::set_priority(::multithreading::priority_idle);
            System.visual().fonts().update_font_enumeration();

            m_pfontlistdata->update();

            output_debug_string("fork with idle");

         });

      }



   }


} // namespace plane



