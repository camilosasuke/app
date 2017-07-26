//#include "framework.h"
//#include "core/filesystem/filemanager/filemanager.h"
#include "base/database/simpledb/simpledb.h"


#ifdef WINDOWS
   #include <Wtsapi32.h>
   #include <Userenv.h>
#endif
#ifdef WINDOWSEX
   #include "base/os/windows/windows_system_interaction_impl.h"
#endif

#ifdef LINUX

   Display * x11_get_display();

   //#include <dlfcn.h>
   //#include <link.h>
   //#include <ctype.h>
   //#include <unistd.h>
#elif defined(APPLEOS)
   //#include <dlfcn.h>
   //#include <mach-o/dyld.h>
#endif

#ifdef WINDOWS
   //#include <cderr.h>      // Commdlg Error definitions
   //#include <winspool.h>
   #ifdef WINDOWSEX
      //#include "app/appseed/aura/aura/node/windows/windows.h"
   #endif
#endif


namespace core
{


   const char application::gen_FileSection[] = "Recent File List";
   const char application::gen_FileEntry[] = "File%d";
   const char application::gen_PreviewSection[] = "Settings";
   const char application::gen_PreviewEntry[] = "PreviewPages";


   application::application() :
      ::object(this), // start m_pauraapp as this for constructor referencing this app
      thread(NULL)
   {

      m_pmainpane = NULL;

      if (m_pauraapp == NULL)
      {
         set_app(this);
      }


      // almost always forgotten, assumed, as exception, responsability of application to add first ref on constructor.
      //::add_ref(this);

      srand(::get_tick_count());

      m_bService = false;

      m_iResourceId = 8001;

      ::core::profiler::initialize();

      m_pcoreapp = this;

      m_pwndMain = NULL;

      m_pdocmanager = NULL;

      m_psignal->connect(this, &application::on_application_signal);

      m_eexclusiveinstance = ExclusiveInstanceNone;
      m_peventReady = NULL;
      m_strLocale = "_std";
      m_strSchema = "_std";

      m_iGcomBackgroundUpdateMillis = -1;

      //      m_pcalculator = NULL;

   }


   application::~application()
   {

   }



   void application::construct(const char * pszId)
   {

      if (pszId == NULL)
      {

         m_strId = "";

      }
      else
      {

         m_strId = pszId;

      }

      ::base::application::construct(m_strId);

      if (m_strAppName.is_empty())
      {

         if (m_strAppId.has_char())
         {

            m_strAppName = m_strAppId;

         }
         else if (m_strInstallToken.has_char())
         {

            m_strAppName = m_strInstallToken;
         }

      }

   }

   application * application::get_app() const
   {
      return (application *) this;
   }




   bool application::is_system()
   {

      return false;

   }


   bool application::is_session()
   {

      return false;

   }


   bool application::is_installing()
   {

      return ::base::application::is_installing();

   }


   bool application::is_uninstalling()
   {

      return ::base::application::is_uninstalling();

   }


   bool application::is_serviceable()
   {

      return ::base::application::is_serviceable();

   }



   int32_t application::main()
   {

      return ::base::application::main();

   }


   bool application::os_native_bergedge_start()
   {

      return true;

   }


   bool application::process_initialize()
   {

      thisinfo << "start";

      if (!::base::application::process_initialize())
      {

         thiserr << "end failure (1)";

         return false;

      }


      if(!process_initialize_userfs())
      {

         thiserr << "end failure (2)";

         return false;

      }

      thisinfo << "end";

      return true;

   }




   bool application::initialize1()
   {

      thisstart;

      if (!::base::application::initialize1())
      {

         thiserr << "end failure (1)";

         return false;

      }


      if (!initialize1_wndfrm())
      {

         thiserr << "end failure (2)";

         return false;

      }


      m_dwAlive = ::get_tick_count();

      if (!is_system() && !is_session())
      {

         ::aura::session * paurasession = m_paurasession;

         Session.register_bergedge_application(this);

      }


      if (!initialize1_userfs())
      {

         thiserr << "end failure (3)";

         return false;

      }


      if (!is_system() && !is_session() && !is_installing() && !is_uninstalling())
      {

         string str;
         // if system locale has changed (compared to last recorded one by core)
         // use the system locale
         if (data_get(".local://system_locale", str))
         {
            if (str.has_char())
            {
               if (str != Session.get_locale())
               {
                  try
                  {
                     data_set(".local://system_locale", Session.get_locale());
                     data_set("locale", Session.get_locale());
                  }
                  catch (...)
                  {
                  }
               }
            }
         }
         else
         {
            data_set(".local://system_locale", Session.get_locale());
         }

         if (command()->m_varTopicQuery["locale"].get_count() > 0)
         {
            str = command()->m_varTopicQuery["locale"].stra()[0];
            data_set(".local://system_locale", str);
            data_set("locale", str);
            Session.set_locale(str, ::action::source::database());
         }
         else if (command()->m_varTopicQuery["lang"].get_count() > 0)
         {
            str = command()->m_varTopicQuery["lang"].stra()[0];
            data_set(".local://system_locale", str);
            data_set(".local://locale", str);
            Session.set_locale(str, ::action::source::database());
         }
         else if (data_get(".local://locale", str))
         {
            if (str.has_char())
            {
               Session.set_locale(str, ::action::source::database());
            }
         }
         // if system schema has changed (compared to last recorded one by core)
         // use the system schema
         if (data_get(".local://system_schema", str))
         {
            if (str.has_char())
            {
               if (str != Session.get_schema())
               {
                  try
                  {
                     data_set(".local://system_schema", Session.get_schema());
                     data_set("schema", Session.get_schema());
                  }
                  catch (...)
                  {
                  }
               }
            }
         }
         else
         {
            data_set(".local://system_schema", Session.get_schema());
         }

         if (command()->m_varTopicQuery["schema"].get_count() > 0)
         {
            str = command()->m_varTopicQuery["schema"].stra()[0];
            data_set(".local://system_schema", str);
            data_set(".local://schema", str);
            Session.set_schema(str, ::action::source::database());
         }
         else if (data_get(".local://schema", str))
         {
            if (str.has_char())
            {
               Session.set_schema(str, ::action::source::database());
            }
         }


         data_pulse_change("ca2.local://savings", NULL);


         Sess(this).fill_locale_schema(*Session.str_context()->m_plocaleschema);


         Sys(this).appa_load_string_table();

      }


      /*if(!m_spuser->initialize1())
      return false;
      if(!m_spuser->initialize2())
      return false;*/




      return true;

   }


   bool application::initialize2()
   {

      if (!::base::application::initialize2())
         return false;

      return true;

   }


   bool application::initialize3()
   {

      if (!::base::application::initialize3())
         return false;

      return true;

   }











   void application::on_request(::create * pcreate)
   {

      ::base::application::on_request(pcreate);

   }







   /*   ::lemon::array & application::array()
   {

   return *m_plemonarray;

   }
   */

   bool application::initialize_application()
   {

#ifdef DEBUG

      debug_ca2_variadic_template_base_formatting_integer_zero_padding();

#endif

      thisstart;

      //try
      //{

      //   throw_numeric_parser_exception("little test of numeric parser exception at sys (main?) function stack?");

      //}
      //catch (numeric_parser_exception & exp)
      //{

      //   output_debug_string("caught!!!! : " + exp.m_strMessage);

      //}

      if (!::base::application::initialize_application())
      {

         thisfail << 1;

         return false;

      }

      thisok << 1;

      if (!is_session() && !is_system())
      {

         if (directrix()->m_varTopicQuery.has_property("install"))
         {

            if (is_user_service())
            {

               if (Session.fontopus()->m_puser != NULL && Session.fontopus()->m_puser->m_strLogin == "system")
               {

                  Session.fontopus()->m_puser = NULL;

               }

               if (!m_strAppId.begins_ci("app-core/netnode") && m_strAppId != "app-core/mydns")
               {

                  ApplicationUser;

               }

            }

         }
         else
         {

            //Session.keyboard();

         }

         thisok << 1.1;

      }

      thisend;

      return true;

   }


   int32_t application::exit_application()
   {

      try
      {

         for(auto & pair : System.m_appmap)
         {

            try
            {

               if(pair.m_element2->m_pcoreapp == this)
               {

                  pair.m_element2->m_pcoreapp = NULL;

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

      try
      {

         close(end_app);

      }
      catch (...)
      {

      }



      try
      {
         if (!is_system())
         {
            Session.unregister_bergedge_application(this);
         }
      }
      catch (...)
      {

      }

      try
      {

         m_spobjectUserFs.release();

      }
      catch (...)
      {


      }

      m_puserfs = NULL;





      try
      {
         ::base::application::exit_application();
      }
      catch (...)
      {
      }

      return m_iReturnCode;


   }


   LRESULT application::GetPaintMsgProc(int32_t nCode, WPARAM wParam, LPARAM lParam)
   {
      UNREFERENCED_PARAMETER(nCode);
      UNREFERENCED_PARAMETER(wParam);
      UNREFERENCED_PARAMETER(lParam);
      return 0;
   }





   bool application::CreateFileFromRawResource(UINT nID, const char * lpcszType, const char * lpcszFilePath)
   {
      UNREFERENCED_PARAMETER(nID);
      UNREFERENCED_PARAMETER(lpcszType);
      UNREFERENCED_PARAMETER(lpcszFilePath);
      /*      HINSTANCE hinst = ::core::FindResourceHandle(MAKEINTRESOURCE(nID), lpcszType);
      if(hinst == NULL)
      return false;
      HRSRC hrsrc = ::FindResource(
      hinst,
      MAKEINTRESOURCE(nID),
      lpcszType);
      if(hrsrc == NULL)
      return false;
      HGLOBAL hres = ::LoadResource(hinst, hrsrc);
      if(hres == NULL)
      return false;
      uint32_t dwResSize = ::SizeofResource(hinst, hrsrc);

      if(hres != NULL)
      {
      UINT FAR* lpnRes = (UINT FAR*)::LockResource(hres);
      try
      {
      // create the .mdb file
      ::file::file_sp f(get_app());

      if(f->open(lpcszFilePath, ::file::mode_create | ::file::mode_write ))
      {
      // write the ::fontopus::user-defined resource to the .mdb file
      f->write(lpnRes, dwResSize);
      f->flush();
      }
      else
      {
      #ifdef DEBUG
      *::aura::system::g_p->m_pdumpcontext << "File could not be opened \n";
      #endif
      }
      }
      catch(::core::file_exception_sp * pe)
      {
      #ifdef DEBUG
      //         *::aura::system::g_p->m_pdumpcontext << "File could not be opened " << pe->m_cause << "\n";
      #endif
      }


      #ifndef WIN32 //unlock Resource is obsolete in the Win32 API
      ::UnlockResource(hres);
      #endif
      ::FreeResource(hres);
      }
      return true;*/

      return false;
   }

#ifdef WINDOWS

   bool application::OnMessageWindowMessage(LPMESSAGE lpmsg)
   {
      UNREFERENCED_PARAMETER(lpmsg);
      return false;
   }

#elif defined(LINUX)

   bool application::OnMessageWindowMessage(XEvent * pevent)
   {
      UNREFERENCED_PARAMETER(pevent);
      return false;
   }

#endif

   void application::OnAppLanguage(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      m_signalAppLanguageChange.emit();
   }



   void application::OnUpdateRecentFileMenu(cmd_ui * pcmdui)
   {
      UNREFERENCED_PARAMETER(pcmdui);
      /*TRACE("\nCVmsGenApp::OnUpdateRecentFileMenu");
      if(m_pRecentFileList == NULL)
      {
      pcmdui->Enable(FALSE);
      //string str;
      //str.load_string(IDS_RECENT_FILE);
      //pcmdui->SetText(str);
      for (int32_t iMRU = 1; iMRU < 10; iMRU++)
      pcmdui->m_pMenu->DeleteMenu(pcmdui->m_nID + iMRU, MF_BYCOMMAND);
      return;
      }

      ASSERT(m_pRecentFileList->m_arrNames != NULL);

      ::user::menu* pMenu = pcmdui->m_pMenu;
      if (m_pRecentFileList->m_strOriginal.is_empty() && pMenu != NULL)
      pMenu->GetMenuString(pcmdui->m_nID, m_pRecentFileList->m_strOriginal, MF_BYCOMMAND);

      if (m_pRecentFileList->m_arrNames[0].is_empty())
      {
      // no MRU files
      if (!m_pRecentFileList->m_strOriginal.is_empty())
      pcmdui->SetText(m_pRecentFileList->m_strOriginal);
      pcmdui->Enable(FALSE);
      return;
      }

      if (pcmdui->m_pMenu == NULL)
      return;

      ::user::menu * pmenu = CMenuUtil::FindPopupMenuFromID(pcmdui->m_pMenu, pcmdui->m_nID);

      //if(pmenu == NULL)
      //{
      // pmenu = pcmdui->m_pMenu;
      //}

      bool bCmdUIMenu = pmenu == pcmdui->m_pMenu;

      if(!bCmdUIMenu)
      return;

      int32_t nID = pcmdui->m_nID;
      int32_t nIndex = CMenuUtil::GetMenuPosition(pmenu, nID);

      for (int32_t iMRU = 0; iMRU < m_pRecentFileList->m_nSize; iMRU++)
      pcmdui->m_pMenu->DeleteMenu(pcmdui->m_nID + iMRU, MF_BYCOMMAND);



      char szCurDir[_MAX_PATH];
      GetCurrentDirectory(_MAX_PATH, szCurDir);
      int32_t nCurDir = lstrlen(szCurDir);
      ASSERT(nCurDir >= 0);
      szCurDir[nCurDir] = '\\';
      szCurDir[++nCurDir] = '\0';

      string strName;
      string strTemp;
      for (iMRU = 0; iMRU < m_pRecentFileList->m_nSize; iMRU++)
      {
      if (!m_pRecentFileList->GetDisplayName(strName, iMRU, szCurDir, nCurDir))
      break;

      // double up any '&' characters so they are not underlined
      const char * lpszSrc = strName;
      LPTSTR lpszDest = strTemp.GetBuffer(strName.get_length()*2);
      while (*lpszSrc != 0)
      {
      if (*lpszSrc == '&')
      *lpszDest++ = '&';
      if (_istlead(*lpszSrc))
      *lpszDest++ = *lpszSrc++;
      *lpszDest++ = *lpszSrc++;
      }
      *lpszDest = 0;
      strTemp.ReleaseBuffer();

      // insert mnemonic + the file name
      char buf[10];
      wsprintf(buf, "&%d ", (iMRU+1+m_pRecentFileList->m_nStart) % 10);

      //      pcmdui->m_pMenu->InsertMenu(pcmdui->m_nIndex++,
      //         MF_STRING | MF_BYPOSITION, pcmdui->m_nID++,
      //         string(buf) + strTemp);
      pmenu->InsertMenu(nIndex,
      MF_STRING | MF_BYPOSITION, nID,
      string(buf) + strTemp);
      nIndex++;
      nID++;
      if(bCmdUIMenu)
      {
      pcmdui->m_nIndex = nIndex;
      pcmdui->m_nID = nID;
      }
      }

      // update end menu count
      if(bCmdUIMenu)
      {
      pcmdui->m_nIndex--; // point to last menu added
      pcmdui->m_nIndexMax = pcmdui->m_pMenu->GetMenuItemCount();
      }

      pcmdui->m_bEnableChanged = TRUE;    // all the added items are enabled*/

   }

   bool application::GetResourceData(UINT nID, const char * lpcszType, memory &storage)
   {
      UNREFERENCED_PARAMETER(nID);
      UNREFERENCED_PARAMETER(lpcszType);
      UNREFERENCED_PARAMETER(storage);
      /*      HINSTANCE hinst = ::core::FindResourceHandle(MAKEINTRESOURCE(nID), lpcszType);

      if(hinst == NULL)
      return false;

      HRSRC hrsrc = ::FindResource(
      hinst,
      MAKEINTRESOURCE(nID),
      lpcszType);

      if(hrsrc == NULL)
      return false;

      HGLOBAL hres = ::LoadResource(hinst, hrsrc);
      if(hres == NULL)
      return false;

      uint32_t dwResSize = ::SizeofResource(hinst, hrsrc);

      if(hres != NULL)
      {
      UINT FAR* lpnRes = (UINT FAR*)::LockResource(hres);
      try
      {
      storage.set_data(lpnRes, dwResSize);
      }
      catch(::core::file_exception_sp * pe)
      {
      #ifdef DEBUG
      //            *::aura::system::g_p->m_pdumpcontext << "File could not be opened " << pe->m_cause << "\n";
      #endif
      }


      #ifndef WIN32 //unlock Resource is obsolete in the Win32 API
      ::UnlockResource(hres);
      #endif
      ::FreeResource(hres);
      }
      return true;*/
      return false;

   }

#ifdef WINDOWSEX

   HENHMETAFILE application::LoadEnhMetaFile(UINT uiResource)
   {
      memory storage;
      if (!GetResourceData(uiResource, "EnhMetaFile", storage))
      {
         return NULL;
      }
      return SetEnhMetaFileBits((UINT)storage.get_size(), storage.get_data());
   }

#endif

   /////////////////////////////////////////////////////////////////////////////
   // WinApp UI related functions

   void application::EnableModelessEx(bool bEnable)
   {
      UNREFERENCED_PARAMETER(bEnable);
#ifdef ___NO_OLE_SUPPORT
      UNUSED(bEnable);
#endif

      // no-op if main window is NULL or not a frame_window
      /*      sp(::user::interaction) pMainWnd = System.m_puiMain;
      if (pMainWnd == NULL || !pMainWnd->is_frame_window())
      return;*/

#ifndef ___NO_OLE_SUPPORT
      // check if notify hook installed
      /*   ::core::frame_window* pFrameWnd =
      dynamic_cast < ::core::frame_window * > (pMainWnd);
      ASSERT(pFrameWnd != NULL);
      if (pFrameWnd->GetNotifyHook() != NULL)
      pFrameWnd->GetNotifyHook()->OnEnableModeless(bEnable);*/
#endif
   }


   /*id_space * application::GetGenIdSpace()
   {
   return m_pidspace;
   }*/

   /*
   string application::load_string(const id_space * pspace, int32_t iKey)
   {
   string str;
   int32_t iId = GetResourceId(pspace, iKey);
   if(iId == -1)
   return str;
   str.load_string(iId);
   return str;
   }
   */





   // Main running routine until application exits
   int32_t application::run()
   {
      /*   if (m_puiMain == NULL) // may be a service or console application window
      {
      // Not launched /Embedding or /Automation, but has no main window!
      TRACE(::aura::trace::category_AppMsg, 0, "Warning: m_puiMain is NULL in application::run - quitting application.\n");
      __post_quit_message(0);
      }*/
      //      return application::run();
      return ::base::application::run();

   }


   /////////////////////////////////////////////////////////////////////////////
   // application idle processing
   bool application::on_idle(LONG lCount)
   {
      /*      if (lCount <= 0)
      {
      thread::on_idle(lCount);

      // call doc-template idle hook
      ::count count = 0;
      if (m_pdocmanager != NULL)
      count = document_manager().get_template_count();

      for(index index = 0; index < count; index++)
      {
      sp(impact_system) ptemplate = document_manager().get_template(index);
      ASSERT_KINDOF(impact_system, ptemplate);
      ptemplate->on_idle();
      }
      }
      else if (lCount == 1)
      {
      VERIFY(!thread::on_idle(lCount));
      }
      return lCount < 1;  // more to do if lCount < 1*/
      return 0;
   }

   /////////////////////////////////////////////////////////////////////////////
   // Special exception handling

   void application::process_window_procedure_exception(::exception::base* e, signal_details * pobj)
   {
      ENSURE_ARG(e != NULL);
      ENSURE_ARG(pobj != NULL);
      SCAST_PTR(::message::base, pbase, pobj);
      // handle certain messages in thread
      switch (pbase->m_uiMessage)
      {
      case WM_CREATE:
      case WM_PAINT:
         return thread::process_window_procedure_exception(e, pobj);
      }

      // handle all the rest
      //linux UINT nIDP = __IDP_INTERNAL_FAILURE;   // generic message string
      const char * nIDP = "Internal Failure";
      pbase->set_lresult(0);        // sensible default
      if (pbase->m_uiMessage == WM_COMMAND)
      {
         if (pbase->m_lparam == 0)
            //linux nIDP = __IDP_COMMAND_FAILURE; // command (not from a control)
            nIDP = "Command Failure";
         pbase->set_lresult((LRESULT)TRUE);        // pretend the command was handled
      }
      if (base_class < memory_exception >::bases(e))
      {
         e->ReportError(MB_ICONEXCLAMATION | MB_SYSTEMMODAL, nIDP);
      }
      else if (base_class < user_exception >::bases(e))
      {
         // ::fontopus::user has not been alerted yet of this catastrophic problem
         e->ReportError(MB_ICONSTOP, nIDP);
      }
   }

   bool application::_001OnCmdMsg(::aura::cmd_msg * pcmdmsg)

   {
      if (command_target_interface::_001OnCmdMsg(pcmdmsg))
         return TRUE;
      return 0;
   }



   /*
   bool application::hex_to_memory(memory & memory, const char * pszHex)
   {
   ::count len = strlen(pszHex);
   ::count count = (len + 1) / 2;
   memory.allocate(count);
   index i = 0;
   byte b;
   while(*pszHex != '\0')
   {
   char ch = (char) tolower(*pszHex);
   if(ch >= '0' && ch <= '9')
   {
   b = ch - '0';
   }
   else if(ch >= 'a' && ch <= 'f')
   {
   b = ch - 'a' + 10;
   }
   else
   {
   return false;
   }
   pszHex++;
   if(*pszHex == '\0')
   {
   memory.get_data()[i] = b;
   return true;
   }
   b = b << 4;
   ch = (char) tolower(*pszHex);
   if(ch >= '0' && ch <= '9')
   {
   b |= (ch - '0');
   }
   else if(ch >= 'a' && ch <= 'f')
   {
   b |= (ch - 'a' + 10);
   }
   else
   {
   return false;
   }
   pszHex++;
   memory.get_data()[i] = b;
   i++;
   }
   return true;
   }


   void application::memory_to_hex(string & strHex, memory & memory)
   {
   ::count count = memory.get_size();
   LPSTR lpsz = strHex.GetBufferSetLength(count * 2);
   for(index i = 0; i < count; i++)
   {
   *lpsz++ = ::hex::lower_from((byte) ((memory.get_data()[i] >> 4) & 0xf));
   *lpsz++ = ::hex::lower_from((byte) (memory.get_data()[i] & 0xf));
   }
   strHex.ReleaseBuffer(count * 2);
   }
   */











   // // BEGIN_MESSAGE_MAP(application, command_target)
   //{{__MSG_MAP(application)
   // Global File commands
   //   ON_COMMAND(ID_APP_EXIT, &application::OnAppExit)
   // MRU - most recently used file menu
   //   ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE1, &application::OnUpdateRecentFileMenu)
   //   ON_COMMAND_EX_RANGE(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE16, &application::OnOpenRecentFile)
   //}}__MSG_MAP
   // // END_MESSAGE_MAP()

   /*#ifdef WINDOWS


   typedef LANGID (WINAPI*PFNGETUSERDEFAULTUILANGUAGE)();
   typedef LANGID (WINAPI*PFNGETSYSTEMDEFAULTUILANGUAGE)();


   class CActivationContext
   {
   protected :
   HANDLE m_hCtxt;
   uint_ptr m_uCookie;

   // If pointers are NULL then we are on a platform that does not support WinSXS.
   typedef HANDLE (WINAPI * PFNCreateActCtx)(PCACTCTX);
   static PFNCreateActCtx s_pfnCreateActCtx;

   typedef void (WINAPI * PFNReleaseActCtx)(HANDLE);
   static PFNReleaseActCtx s_pfnReleaseActCtx;

   typedef bool (WINAPI * PFNActivateActCtx)(HANDLE, uint_ptr*);
   static PFNActivateActCtx s_pfnActivateActCtx;

   typedef bool (WINAPI * PFNDeactivateActCtx)(uint32_t, uint_ptr);
   static PFNDeactivateActCtx s_pfnDeactivateActCtx;

   static bool s_bPFNInitialized;

   public:
   CActivationContext(HANDLE hCtxt = INVALID_HANDLE_VALUE) : m_hCtxt( hCtxt ), m_uCookie( 0 )
   {
   // Multiple threads initializing is fine since they will initialize the pointers to the
   // same value.
   if (!s_bPFNInitialized)
   {
   HMODULE hKernel = GetModuleHandle("KERNEL32");
   ENSURE (hKernel != NULL);
   #ifdef _UNICODE
   s_pfnCreateActCtx = (PFNCreateActCtx) GetProcAddress(hKernel, "CreateActCtxW");
   #else
   s_pfnCreateActCtx = (PFNCreateActCtx) GetProcAddress(hKernel, "CreateActCtxA");
   #endif
   s_pfnReleaseActCtx = (PFNReleaseActCtx) GetProcAddress(hKernel, "ReleaseActCtx");
   s_pfnActivateActCtx = (PFNActivateActCtx) GetProcAddress(hKernel, "ActivateActCtx");
   s_pfnDeactivateActCtx = (PFNDeactivateActCtx) GetProcAddress(hKernel, "DeactivateActCtx");
   if (s_pfnCreateActCtx != NULL)
   {
   // If one of the functions is present then all the functions have to be present.
   ENSURE( s_pfnReleaseActCtx != NULL &&
   s_pfnActivateActCtx != NULL &&
   s_pfnDeactivateActCtx != NULL);
   }
   else
   {
   // If one of the functions is not present then all the functions should not be present.
   ENSURE( s_pfnReleaseActCtx == NULL &&
   s_pfnActivateActCtx == NULL &&
   s_pfnDeactivateActCtx == NULL);
   }
   s_bPFNInitialized = true;
   }
   };

   ~CActivationContext()
   {
   Release();
   }

   bool create( PCACTCTX pactctx )
   {
   // NULL on a platform that do not support WinSXS
   if (s_pfnCreateActCtx == NULL)
   {
   return true;
   }

   ASSERT( pactctx != NULL );
   if ( pactctx == NULL )
   {
   return false;
   }

   ASSERT( m_hCtxt == INVALID_HANDLE_VALUE );
   if ( m_hCtxt != INVALID_HANDLE_VALUE )
   {
   return false;
   }

   return ( ( m_hCtxt = s_pfnCreateActCtx( pactctx ) ) != INVALID_HANDLE_VALUE );
   }

   void Release()
   {
   // NULL on a platform that do not support WinSXS
   if (s_pfnReleaseActCtx == NULL)
   {
   return;
   }

   if ( m_hCtxt != INVALID_HANDLE_VALUE )
   {
   Deactivate();
   s_pfnReleaseActCtx( m_hCtxt );
   }
   }

   bool Activate()
   {
   // NULL on a platform that do not support WinSXS
   if (s_pfnActivateActCtx == NULL)
   {
   return true;
   }

   ASSERT( m_hCtxt != INVALID_HANDLE_VALUE );
   if ( m_hCtxt == INVALID_HANDLE_VALUE )
   {
   return false;
   }

   ASSERT( m_uCookie == 0 );
   if ( m_uCookie != 0 )
   {
   return false;
   }

   return ( s_pfnActivateActCtx( m_hCtxt, &m_uCookie) == TRUE );
   }

   bool Deactivate()
   {
   // NULL on a platform that do not support WinSXS
   if (s_pfnDeactivateActCtx == NULL)
   {
   return true;
   }

   if ( m_uCookie != 0 )
   {
   uint_ptr uCookie = m_uCookie;
   m_uCookie = 0;
   return ( s_pfnDeactivateActCtx(0, uCookie) == TRUE );
   }
   return true;
   }
   };


   CActivationContext::PFNCreateActCtx CActivationContext::s_pfnCreateActCtx = NULL;
   CActivationContext::PFNReleaseActCtx CActivationContext::s_pfnReleaseActCtx = NULL;
   CActivationContext::PFNActivateActCtx CActivationContext::s_pfnActivateActCtx = NULL;
   CActivationContext::PFNDeactivateActCtx CActivationContext::s_pfnDeactivateActCtx = NULL;
   bool CActivationContext::s_bPFNInitialized = false;


   // HINSTANCE of the module
   extern "C" IMAGE_DOS_HEADER __ImageBase;

   */


   bool application::LoadSysPolicies()
   {
      return _LoadSysPolicies();
   }

   // This function is not exception safe - will leak a registry key if exceptions are thrown from some places
   // To reduce risk of leaks, I've declared the whole function throw(). This despite the fact that its callers have
   // no dependency on non-throwing.
   bool application::_LoadSysPolicies() throw()
   {

#ifdef WINDOWSEX

      HKEY hkPolicy = NULL;
      DWORD dwValue = 0;
      DWORD dwDataLen = sizeof(dwValue);
      DWORD dwType = 0;

      // clear current policy settings.
      m_dwPolicies = ___SYSPOLICY_NOTINITIALIZED;

      static __system_policy_data rgExplorerData[] =
      {
         {"NoRun",___SYSPOLICY_NORUN},
         {"NoDrives",___SYSPOLICY_NODRIVES},
         {"RestrictRun",___SYSPOLICY_RESTRICTRUN},
         {"NoNetConnectDisconnect",___SYSPOLICY_NONETCONNECTDISCONNECTD},
         {"NoRecentDocsHistory",___SYSPOLICY_NORECENTDOCHISTORY},
         {"NoClose",___SYSPOLICY_NOCLOSE},
         {NULL,0}
      };

      static __system_policy_data rgNetworkData[] =
      {
         {"NoEntireNetwork",___SYSPOLICY_NOENTIRENETWORK},
         {NULL,0}
      };

      static __system_policy_data rgComDlgData[] =
      {
         {"NoPlacesBar",___SYSPOLICY_NOPLACESBAR},
         {"NoBackButton",___SYSPOLICY_NOBACKBUTTON},
         {"NoFileMru",___SYSPOLICY_NOFILEMRU},
         {NULL,0}
      };

      static __system_policies rgPolicies[] =
      {
         {  "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
            rgExplorerData
         },
         {  "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Network",
            rgNetworkData
         },
         {  "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Comdlg32",
            rgComDlgData
         },
         {NULL,0}
      };

      __system_policies *pPolicies = rgPolicies;
      __system_policy_data *pData = NULL;

      while (pPolicies->szPolicyKey != NULL)
      {

         if (ERROR_SUCCESS == ::RegOpenKeyEx(
                  HKEY_CURRENT_USER,
                  pPolicies->szPolicyKey,
                  0,
                  KEY_QUERY_VALUE,
                  &hkPolicy
               ))
         {
            pData = pPolicies->pData;
            while (pData->szPolicyName)
            {
               if (ERROR_SUCCESS == ::RegQueryValueEx(
                        hkPolicy,
                        pData->szPolicyName,
                        NULL,
                        &dwType,
                        (BYTE*)&dwValue,
                        &dwDataLen))
               {
                  if (dwType == REG_DWORD)
                  {
                     if (dwValue != 0)
                        m_dwPolicies |= pData->dwID;
                     else
                        m_dwPolicies &= ~pData->dwID;
                  }
               }
               dwValue = 0;
               dwDataLen = sizeof(dwValue);
               dwType = 0;
               pData++;
            }
            ::RegCloseKey(hkPolicy);
            hkPolicy = NULL;
         }
         pPolicies++;
      };

#endif

      return TRUE;


   }

   bool application::GetSysPolicyValue(uint32_t dwPolicyID, bool *pbValue)
   {
      if (!pbValue)
         return FALSE; // bad pointer
      *pbValue = (m_dwPolicies & dwPolicyID) != 0;
      return TRUE;
   }

   bool application::InitApplication()
   {


      LoadSysPolicies();

      return TRUE;


   }



   /*   void application::LoadStdProfileSettings(UINT nMaxMRU)
   {
   UNREFERENCED_PARAMETER(nMaxMRU);
   ASSERT_VALID(this);

   // 0 by default means not set
   m_nNumPreviewPages = GetProfileInt(gen_PreviewSection, gen_PreviewEntry, 0);
   }*/

   /*void application::ParseCommandLine(CCommandLineInfo& rCmdInfo)
   {
   for (int32_t i = 1; i < __argc; i++)
   {
   const char * pszParam = __targv[i];
   bool bFlag = FALSE;
   bool bLast = ((i + 1) == __argc);
   if (pszParam[0] == '-' || pszParam[0] == '/')
   {
   // remove flag specifier
   bFlag = TRUE;
   ++pszParam;
   }
   rCmdInfo.ParseParam(pszParam, bFlag, bLast);
   }
   }*/

   /////////////////////////////////////////////////////////////////////////////
   // CCommandLineInfo implementation

   /*CCommandLineInfo::CCommandLineInfo()
   {
   m_bShowSplash = TRUE;
   m_bRunEmbedded = FALSE;
   m_bRunAutomated = FALSE;
   m_nShellCommand = FileNew;
   }

   CCommandLineInfo::~CCommandLineInfo()
   {
   }

   void CCommandLineInfo::ParseParam(const char* pszParam,bool bFlag,bool bLast)
   {
   if (bFlag)
   {
   const astring strParam(pszParam);
   ParseParamFlag(strParam.GetString());
   }
   else
   ParseParamNotFlag(pszParam);

   ParseLast(bLast);
   }*/

   /*
   #ifdef UNICODE
   void CCommandLineInfo::ParseParam(const char* pszParam, bool bFlag, bool bLast)
   {
   if (bFlag)
   ParseParamFlag(pszParam);
   else
   ParseParamNotFlag(pszParam);

   ParseLast(bLast);
   }
   #endif // UNICODE
   */

   /*
   void CCommandLineInfo::ParseParamFlag(const char* pszParam)
   {
   // OLE command switches are case insensitive, while
   // shell command switches are case sensitive

   if (lstrcmpA(pszParam, "pt") == 0)
   m_nShellCommand = FilePrintTo;
   else if (lstrcmpA(pszParam, "p") == 0)
   m_nShellCommand = FilePrint;
   else if (::__invariant_stricmp(pszParam, "Register") == 0 ||
   ::__invariant_stricmp(pszParam, "Regserver") == 0)
   m_nShellCommand = AppRegister;
   else if (::__invariant_stricmp(pszParam, "Unregister") == 0 ||
   ::__invariant_stricmp(pszParam, "Unregserver") == 0)
   m_nShellCommand = AppUnregister;
   else if (lstrcmpA(pszParam, "dde") == 0)
   {
   m_nShellCommand = FileDDE;
   }
   else if (::__invariant_stricmp(pszParam, "Embedding") == 0)
   {
   m_bRunEmbedded = TRUE;
   m_bShowSplash = FALSE;
   }
   else if (::__invariant_stricmp(pszParam, "Automation") == 0)
   {
   m_bRunAutomated = TRUE;
   m_bShowSplash = FALSE;
   }
   }

   void CCommandLineInfo::ParseParamNotFlag(const char* pszParam)
   {
   if (m_strFileName.is_empty())
   m_strFileName = pszParam;
   else if (m_nShellCommand == FilePrintTo && m_strPrinterName.is_empty())
   m_strPrinterName = pszParam;
   else if (m_nShellCommand == FilePrintTo && m_strDriverName.is_empty())
   m_strDriverName = pszParam;
   else if (m_nShellCommand == FilePrintTo && m_strPortName.is_empty())
   m_strPortName = pszParam;
   }

   #ifdef UNICODE
   void CCommandLineInfo::ParseParamNotFlag(const char* pszParam)
   {
   if (m_strFileName.is_empty())
   m_strFileName = pszParam;
   else if (m_nShellCommand == FilePrintTo && m_strPrinterName.is_empty())
   m_strPrinterName = pszParam;
   else if (m_nShellCommand == FilePrintTo && m_strDriverName.is_empty())
   m_strDriverName = pszParam;
   else if (m_nShellCommand == FilePrintTo && m_strPortName.is_empty())
   m_strPortName = pszParam;
   }
   #endif

   void CCommandLineInfo::ParseLast(bool bLast)
   {
   if (bLast)
   {
   if (m_nShellCommand == FileNew && !m_strFileName.is_empty())
   m_nShellCommand = FileOpen;
   m_bShowSplash = !m_bRunEmbedded && !m_bRunAutomated;
   }
   }
   */

   /////////////////////////////////////////////////////////////////////////////
   // App termination

   void application::SaveStdProfileSettings()
   {
      ASSERT_VALID(this);


      //      if (m_nNumPreviewPages != 0)
      //       WriteProfileInt(gen_PreviewSection, gen_PreviewEntry, m_nNumPreviewPages);
   }






#ifdef WINDOWSEX


   /////////////////////////////////////////////////////////////////////////////
   // WinHelp Helper


   void application::WinHelp(uint_ptr dwData, UINT nCmd)
   {
      UNREFERENCED_PARAMETER(dwData);
      UNREFERENCED_PARAMETER(nCmd);

      // return global cast help mode state to FALSE (backward compatibility)
      m_bHelpMode = FALSE;
      // trans pMainWnd->PostMessage(WM_KICKIDLE); // trigger idle update

      //trans pMainWnd->WinHelp(dwData, nCmd);
   }

   /////////////////////////////////////////////////////////////////////////////
   // HtmlHelp Helper

   void application::HtmlHelp(uint_ptr dwData, UINT nCmd)
   {

      UNREFERENCED_PARAMETER(dwData);

      UNREFERENCED_PARAMETER(nCmd);

      // return global cast help mode state to FALSE (backward compatibility)
      m_bHelpMode = FALSE;
      // trans pMainWnd->PostMessage(WM_KICKIDLE); // trigger idle update

      // trans pMainWnd->HtmlHelp(dwData, nCmd);
   }


   void application::WinHelpInternal(uint_ptr dwData, UINT nCmd)
   {
      UNREFERENCED_PARAMETER(dwData);
      UNREFERENCED_PARAMETER(nCmd);
      //   sp(::user::interaction) pMainWnd = System.m_puiMain;
      //   ENSURE_VALID(pMainWnd);

      // return global cast help mode state to FALSE (backward compatibility)
      m_bHelpMode = FALSE;
      // trans pMainWnd->PostMessage(WM_KICKIDLE); // trigger idle update
      // trans pMainWnd->WinHelpInternal(dwData, nCmd);
   }


#endif





   /////////////////////////////////////////////////////////////////////////////
   // application idle processing

   void application::DevModeChange(LPTSTR lpDeviceName)
   {
      UNREFERENCED_PARAMETER(lpDeviceName);

#ifdef WINDOWS
      if (m_hDevNames == NULL)
         return;

#endif

   }



   bool application::on_run_exception(::exception::exception & e)
   {

      ::output_debug_string("core::application::on_run_exception An unexpected error has occurred and no special exception handling is available.\n");

      if (e.m_bHandled)
      {

         return e.m_bContinue;

      }

      if (typeid(e) == typeid(not_installed))
      {

         not_installed & notinstalled = dynamic_cast <not_installed &> (e);

         return handle_not_installed(notinstalled);

      }

      //simple_message_box_timeout("An unexpected error has occurred and no special exception handling is available.<br>Timeout: $simple_message_box_timeout", 5000);

      return true; // continue or exit application? by default: continue by returning true

   }







   bool application::final_handle_exception(::exception::exception & e)
   {
      UNREFERENCED_PARAMETER(e);
      //linux      exit(-1);

      if (!is_system())
      {

         // get_app() may be it self, it is ok...
         if (Sys(get_app()).final_handle_exception((::exception::exception &) e))
            return true;


      }

      return false;
   }


   /*   bool application::open_link(const char * pszLink, const char * pszTarget)
   {
   UNREFERENCED_PARAMETER(pszLink);
   UNREFERENCED_PARAMETER(pszTarget);
   return false;
   }
   */










   void application::_001OnFileNew()
   {
      string strId = m_strId;
      char chFirst = '\0';
      if (strId.get_length() > 0)
      {
         chFirst = strId[0];
      }
      /*      if (m_pdocmanager != NULL)
      document_manager()._001OnFileNew();*/
   }

   void application::on_file_open()
   {
      ENSURE(m_pdocmanager != NULL);
      //document_manager().on_file_open();
   }


   // This is core API library.
   //
   //
   //
   //
   //
   //
   //
   //




   /*void ::core::FormatString1(string & rString, UINT nIDS, const char * lpsz1)
   {
   __format_strings(rString, nIDS, &lpsz1, 1);
   }

   void ::core::FormatString2(string & rString, UINT nIDS, const char * lpsz1,
   const char * lpsz2)
   {
   const char * rglpsz[2];
   rglpsz[0] = lpsz1;
   rglpsz[1] = lpsz2;
   __format_strings(rString, nIDS, rglpsz, 2);
   }*/

   /////////////////////////////////////////////////////////////////////////////


   /////////////////////////////////////////////////////////////////////////////
   // Basic Help support (for backward compatibility to core API 2.0)

   void application::OnHelp()  // use context to derive help context
   {
      if (m_dwPromptContext != 0)
      {
         // do not call WinHelp when the error is failing to lauch help
         //         if (m_dwPromptContext != HID_BASE_PROMPT+__IDP_FAILED_TO_LAUNCH_HELP)
         //          WinHelpInternal(m_dwPromptContext);
         return;
      }

      // otherwise, use window::OnHelp implementation
      /* trans ::window_sp pwindow = System.m_puiMain;
      ENSURE_VALID(pwindow);
      if (!pwindow->is_frame_window())
      pwindow->OnHelp();
      else
      ((pwindow))->OnHelp();*/
   }


   void application::OnHelpIndex()
   {

#ifdef WINDOWSEX

      WinHelpInternal(0L, HELP_INDEX);

#endif

   }


   void application::OnHelpFinder()
   {

#ifdef WINDOWSEX

      WinHelpInternal(0L, HELP_FINDER);

#endif

   }


   void application::OnHelpUsing()
   {

#ifdef WINDOWSEX

      WinHelpInternal(0L, HELP_HELPONHELP);

#endif

   }


   /////////////////////////////////////////////////////////////////////////////
   // Context Help Mode support (backward compatibility to core API 2.0)

   void application::OnContextHelp()
   {
      // just use frame_window::OnContextHelp implementation
      /* trans   m_bHelpMode = HELP_ACTIVE;
      sp(::user::frame_window) pMainWnd = (System.m_puiMain);
      ENSURE_VALID(pMainWnd);
      ENSURE(pMainWnd->is_frame_window());
      pMainWnd->OnContextHelp();
      m_bHelpMode = pMainWnd->m_bHelpMode;
      pMainWnd->PostMessage(WM_KICKIDLE); // trigger idle update */
   }

   /////////////////////////////////////////////////////////////////////////////


   // This is core API library.
   //
   //
   //
   //
   //
   //
   //
   //




   /////////////////////////////////////////////////////////////////////////////
   // WinApp support for printing

   /*bool application::GetPrinterDeviceDefaults(PRINTDLG* pPrintDlg)
   {
   UpdatePrinterSelection(m_hDevNames == NULL); //force default if no current
   if (m_hDevNames == NULL)
   return FALSE;               // no printer defaults

   ENSURE_ARG(pPrintDlg != NULL);
   pPrintDlg->hDevNames = m_hDevNames;
   pPrintDlg->hDevMode = m_hDevMode;

   ::GlobalUnlock(m_hDevNames);
   ::GlobalUnlock(m_hDevMode);
   return TRUE;
   }*/

   void application::UpdatePrinterSelection(bool bForceDefaults)
   {
      UNREFERENCED_PARAMETER(bForceDefaults);
   }


   void application::OnFilePrintSetup()
   {
   }

   void application::SelectPrinter(HANDLE hDevNames, HANDLE hDevMode, bool bFreeOld)
   {
      UNREFERENCED_PARAMETER(hDevNames);
      UNREFERENCED_PARAMETER(hDevMode);
      UNREFERENCED_PARAMETER(bFreeOld);
      ::exception::throw_not_implemented(get_app());
   }

   ::draw2d::graphics * application::CreatePrinterDC()
   {
      ::exception::throw_not_implemented(get_app());
      return NULL;
   }

   /////////////////////////////////////////////////////////////////////////////


   // This is core API library.
   //
   //
   //
   //
   //
   //
   //
   //



   /////////////////////////////////////////////////////////////////////////////
   // application User Interface Extensions

   void application::OnAppExit()
   {

      // same as double-clicking on main window close box

      ASSERT(m_puiMain != NULL);

      ((::user::interaction *) m_puiMain->m_pvoidUserInteraction)->send_message(WM_CLOSE);

   }


   void application::HideApplication()
   {
      try
      {
         if (m_puiMain == NULL)
            return;

         // hide the application's windows before closing all the documents
         ((::user::interaction *) m_puiMain->m_pvoidUserInteraction)->ShowWindow(SW_HIDE);
         // trans    m_puiMain->ShowOwnedPopups(FALSE);

         // put the window at the bottom of zorder, so it isn't activated
         ((::user::interaction *) m_puiMain->m_pvoidUserInteraction)->SetWindowPos(ZORDER_BOTTOM, 0, 0, 0, 0,
               SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
      }
      catch (...)
      {
      }

   }





   bool application::save_all_modified()
   {
      /*      if (m_pdocmanager != NULL)
      return document_manager().save_all_modified();*/
      return TRUE;
   }



   bool application::OnDDECommand(LPTSTR lpszCommand)
   {
      /*      if (m_pdocmanager != NULL)
      return document_manager().OnDDECommand(lpszCommand);
      else*/
      return FALSE;
   }



   void application::EnableModeless(bool bEnable)
   {
      DoEnableModeless(bEnable);
   }

   void application::DoEnableModeless(bool bEnable)
   {
      UNREFERENCED_PARAMETER(bEnable);
#ifdef ___NO_OLE_SUPPORT
      UNUSED(bEnable);
#endif

      // no-op if main window is NULL or not a frame_window
      /*   sp(::user::interaction) pMainWnd = System.m_puiMain;
      if (pMainWnd == NULL || !pMainWnd->is_frame_window())
      return;*/

#ifndef ___NO_OLE_SUPPORT
      // check if notify hook installed
      /*xxx
      ASSERT_KINDOF(frame_window, pMainWnd);
      sp(::user::frame_window) pFrameWnd = (sp(::user::frame_window))pMainWnd;
      if (pFrameWnd->m_pNotifyHook != NULL)
      pFrameWnd->m_pNotifyHook->OnEnableModeless(bEnable);
      */
#endif
   }

   int32_t application::DoMessageBox(const char * lpszPrompt, UINT nType, UINT nIDPrompt)
   {
      return ShowAppMessageBox(this, lpszPrompt, nType, nIDPrompt);
   }



//#ifndef VARIADIC_TEMPLATE
//
//   int32_t application::simple_message_box(::user::primitive * pwndOwner, UINT fuStyle, const char * pszFormat, ...)
//   {
//      va_list va;
//      va_start(va, pszFormat);
//      string str;
//      str.FormatV(pszFormat, va);
//      va_end(va);
//      return simple_message_box(pwndOwner, str, fuStyle);
//   }
//
//#endif
//
//
   // Helper for message boxes; can work when no application can be found
   int32_t application::ShowAppMessageBox(sp(application)pApp, const char * lpszPrompt, UINT nType, UINT nIDPrompt)
   {

      throw not_implemented(pApp);

   }


   /* int32_t ::core::simple_message_box(const char * lpszText, UINT nType, UINT nIDHelp)
   {
   application* papp = &System;
   if (papp != NULL)
   {
   return papp->m_pcoreapp->DoMessageBox(lpszText, nType, nIDHelp);
   }
   else
   {
   return application::ShowAppMessageBox(NULL, lpszText, nType, nIDHelp);
   }
   }
   */

   /*int32_t System.simple_message_box(UINT nIDPrompt, UINT nType, UINT nIDHelp)
   {
   string string;
   if (!string.load_string(nIDPrompt))
   {
   TRACE(::aura::trace::category_AppMsg, 0, "Error: failed to load message box prompt string 0x%04x.\n",
   nIDPrompt);
   ASSERT(FALSE);
   }
   if (nIDHelp == (UINT)-1)
   nIDHelp = nIDPrompt;
   return System.simple_message_box(string, nType, nIDHelp);
   }*/








   void application::RegisterShellFileTypes(bool bCompat)
   {
      ENSURE(m_pdocmanager != NULL);
      //      document_manager().RegisterShellFileTypes(bCompat);
   }

   void application::UnregisterShellFileTypes()
   {
      ENSURE(m_pdocmanager != NULL);
      //    document_manager().UnregisterShellFileTypes();
   }


   int32_t application::get_open_document_count()
   {
      ENSURE(m_pdocmanager != NULL);
      //  return document_manager().get_open_document_count();
      return 0;
   }


   // This is core API library.
   //
   //
   //
   //
   //
   //
   //
   //

   /////////////////////////////////////////////////////////////////////////////
   // application Settings Helpers


   void application::SetRegistryKey(const char * lpszRegistryKey)
   {
      //ASSERT(m_pszRegistryKey == NULL);
      //ASSERT(lpszRegistryKey != NULL);
      //ASSERT(m_strAppName.has_char());

      ////bool bEnable = __enable_memory_tracking(FALSE);
      //free((void *)m_pszRegistryKey);
      //m_pszRegistryKey = strdup(lpszRegistryKey);
      //free((void *)m_pszProfileName);
      //m_pszProfileName = strdup(m_strAppName);
      ////__enable_memory_tracking(bEnable);
   }

   void application::SetRegistryKey(UINT nIDRegistryKey)
   {
      //UNREFERENCED_PARAMETER(nIDRegistryKey);
      //ASSERT(m_pszRegistryKey == NULL);
      //::exception::throw_not_implemented(get_app());
      ///*char szRegistryKey[256];
      //VERIFY(::core::LoadString(nIDRegistryKey, szRegistryKey));
      //SetRegistryKey(szRegistryKey);*/
   }


#ifdef WINDOWSEX

   // returns key for HKEY_CURRENT_USER\"Software"\RegistryKey\ProfileName
   // creating it if it doesn't exist
   // responsibility of the caller to call RegCloseKey() on the returned HKEY
   HKEY application::GetAppRegistryKey()
   {
      //ASSERT(m_pszRegistryKey != NULL);
      //ASSERT(m_pszProfileName != NULL);

      //HKEY hAppKey = NULL;
      //HKEY hSoftKey = NULL;
      //HKEY hCompanyKey = NULL;
      //if(RegOpenKeyEx(HKEY_CURRENT_USER,"software",0,KEY_WRITE | KEY_READ,
      //   &hSoftKey) == ERROR_SUCCESS)
      //{
      //   DWORD dw;
      //   if(RegCreateKeyEx(hSoftKey,m_pszRegistryKey,0,REG_NONE,
      //      REG_OPTION_NON_VOLATILE,KEY_WRITE | KEY_READ,NULL,
      //      &hCompanyKey,&dw) == ERROR_SUCCESS)
      //   {
      //      RegCreateKeyEx(hCompanyKey,m_pszProfileName,0,REG_NONE,
      //         REG_OPTION_NON_VOLATILE,KEY_WRITE | KEY_READ,NULL,
      //         &hAppKey,&dw);
      //   }
      //}
      //if(hSoftKey != NULL)
      //   RegCloseKey(hSoftKey);
      //if(hCompanyKey != NULL)
      //   RegCloseKey(hCompanyKey);

//      return hAppKey;
      return NULL;
   }

   // returns key for:
   //      HKEY_CURRENT_USER\"Software"\RegistryKey\AppName\lpszSection
   // creating it if it doesn't exist.
   // responsibility of the caller to call RegCloseKey() on the returned HKEY
   HKEY application::GetSectionKey(const char * lpszSection)
   {
      ASSERT(lpszSection != NULL);

      HKEY hSectionKey = NULL;
      HKEY hAppKey = GetAppRegistryKey();
      if (hAppKey == NULL)
         return NULL;

      DWORD dw;
      RegCreateKeyEx(hAppKey, lpszSection, 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_READ, NULL, &hSectionKey, &dw);
      RegCloseKey(hAppKey);
      return hSectionKey;
   }

#endif

   /*   UINT application::GetProfileInt(const char * lpszSection, const char * lpszEntry,
   int32_t nDefault)
   {
   ASSERT(lpszSection != NULL);
   ASSERT(lpszEntry != NULL);
   if (m_pszRegistryKey != NULL) // use registry
   {
   HKEY hSecKey = GetSectionKey(lpszSection);
   if (hSecKey == NULL)
   return nDefault;
   uint32_t dwValue;
   uint32_t dwType;
   uint32_t dwCount = sizeof(uint32_t);
   LONG lResult = RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
   (LPBYTE)&dwValue, &dwCount);
   RegCloseKey(hSecKey);
   if (lResult == ERROR_SUCCESS)
   {
   ASSERT(dwType == REG_DWORD);
   ASSERT(dwCount == sizeof(dwValue));
   return (UINT)dwValue;
   }
   return nDefault;
   }
   else
   {
   ASSERT(m_pszProfileName != NULL);
   return ::GetPrivateProfileInt(lpszSection, lpszEntry, nDefault,
   m_pszProfileName);
   }
   }

   string application::GetProfileString(const char * lpszSection, const char * lpszEntry,
   const char * lpszDefault)
   {
   ASSERT(lpszSection != NULL);
   ASSERT(lpszEntry != NULL);
   if (m_pszRegistryKey != NULL)
   {
   HKEY hSecKey = GetSectionKey(lpszSection);
   if (hSecKey == NULL)
   return lpszDefault;
   string strValue;
   uint32_t dwType=REG_NONE;
   uint32_t dwCount=0;
   LONG lResult = RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
   NULL, &dwCount);
   if (lResult == ERROR_SUCCESS)
   {
   ASSERT(dwType == REG_SZ);
   lResult = RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
   (LPBYTE)strValue.GetBuffer(dwCount/sizeof(char)), &dwCount);
   strValue.ReleaseBuffer();
   }
   RegCloseKey(hSecKey);
   if (lResult == ERROR_SUCCESS)
   {
   ASSERT(dwType == REG_SZ);
   return strValue;
   }
   return lpszDefault;
   }
   else
   {
   ASSERT(m_pszProfileName != NULL);

   if (lpszDefault == NULL)
   lpszDefault = "";   // don't pass in NULL
   char szT[4096];
   uint32_t dw = ::GetPrivateProfileString(lpszSection, lpszEntry,
   lpszDefault, szT, _countof(szT), m_pszProfileName);
   ASSERT(dw < 4095);
   return szT;
   }
   }

   bool application::GetProfileBinary(const char * lpszSection, const char * lpszEntry,
   BYTE** ppData, UINT* pBytes)
   {
   ASSERT(lpszSection != NULL);
   ASSERT(lpszEntry != NULL);
   ASSERT(ppData != NULL);
   ASSERT(pBytes != NULL);
   *ppData = NULL;
   *pBytes = 0;
   if (m_pszRegistryKey != NULL)
   {
   HKEY hSecKey = GetSectionKey(lpszSection);
   if (hSecKey == NULL)
   {
   return FALSE;
   }

   // ensure destruction

   // linux ::core::CRegKey rkSecKey(hSecKey);

   uint32_t dwType=0;
   uint32_t dwCount=0;
   LONG lResult = RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType, NULL, &dwCount);
   *pBytes = dwCount;
   if (lResult == ERROR_SUCCESS)
   {
   ASSERT(dwType == REG_BINARY);
   *ppData = new BYTE[*pBytes];
   lResult = RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
   *ppData, &dwCount);
   }
   if (lResult == ERROR_SUCCESS)
   {
   ASSERT(dwType == REG_BINARY);
   return TRUE;
   }
   else
   {
   delete [] *ppData;
   *ppData = NULL;
   }
   return FALSE;
   }
   else
   {
   ASSERT(m_pszProfileName != NULL);

   string str = GetProfileString(lpszSection, lpszEntry, NULL);
   if (str.is_empty())
   return FALSE;
   ASSERT(str.get_length()%2 == 0);
   int_ptr nLen = str.get_length();
   *pBytes = UINT(nLen)/2;
   *ppData = new BYTE[*pBytes];
   for (int32_t i=0;i<nLen;i+=2)
   {
   (*ppData)[i/2] = (BYTE)
   (((str[i+1] - 'A') << 4) + (str[i] - 'A'));
   }
   return TRUE;
   }
   }


   bool application::WriteProfileInt(const char * lpszSection, const char * lpszEntry,
   int32_t nValue)
   {
   ASSERT(lpszSection != NULL);
   ASSERT(lpszEntry != NULL);
   if (m_pszRegistryKey != NULL)
   {
   HKEY hSecKey = GetSectionKey(lpszSection);
   if (hSecKey == NULL)
   return FALSE;
   LONG lResult = RegSetValueEx(hSecKey, lpszEntry, NULL, REG_DWORD,
   (LPBYTE)&nValue, sizeof(nValue));
   RegCloseKey(hSecKey);
   return lResult == ERROR_SUCCESS;
   }
   else
   {
   ASSERT(m_pszProfileName != NULL);

   char szT[16];
   _stprintf_s(szT, _countof(szT), "%d", nValue);
   return ::WritePrivateProfileString(lpszSection, lpszEntry, szT,
   m_pszProfileName);
   }
   }

   bool application::WriteProfileString(const char * lpszSection, const char * lpszEntry,
   const char * lpszValue)
   {
   ASSERT(lpszSection != NULL);
   if (m_pszRegistryKey != NULL)
   {
   LONG lResult;
   if (lpszEntry == NULL) //delete whole department
   {
   HKEY hAppKey = GetAppRegistryKey();
   if (hAppKey == NULL)
   return FALSE;
   lResult = ::RegDeleteKey(hAppKey, lpszSection);
   RegCloseKey(hAppKey);
   }
   else if (lpszValue == NULL)
   {
   HKEY hSecKey = GetSectionKey(lpszSection);
   if (hSecKey == NULL)
   return FALSE;
   // necessary to cast away const below
   lResult = ::RegDeleteValue(hSecKey, (LPTSTR)lpszEntry);
   RegCloseKey(hSecKey);
   }
   else
   {
   HKEY hSecKey = GetSectionKey(lpszSection);
   if (hSecKey == NULL)
   return FALSE;
   lResult = RegSetValueEx(hSecKey, lpszEntry, NULL, REG_SZ,
   (LPBYTE)lpszValue, (lstrlen(lpszValue)+1)*sizeof(char));
   RegCloseKey(hSecKey);
   }
   return lResult == ERROR_SUCCESS;
   }
   else
   {
   ASSERT(m_pszProfileName != NULL);
   ASSERT(lstrlen(m_pszProfileName) < 4095); // can't read in bigger
   return ::WritePrivateProfileString(lpszSection, lpszEntry, lpszValue,
   m_pszProfileName);
   }
   }

   bool application::WriteProfileBinary(const char * lpszSection, const char * lpszEntry,
   LPBYTE pData, UINT nBytes)
   {
   ASSERT(lpszSection != NULL);
   if (m_pszRegistryKey != NULL)
   {
   LONG lResult;
   HKEY hSecKey = GetSectionKey(lpszSection);
   if (hSecKey == NULL)
   return FALSE;
   lResult = RegSetValueEx(hSecKey, lpszEntry, NULL, REG_BINARY,
   pData, nBytes);
   RegCloseKey(hSecKey);
   return lResult == ERROR_SUCCESS;
   }

   // convert to string and write out
   LPTSTR lpsz = new char[nBytes*2+1];
   UINT i;
   for (i = 0; i < nBytes; i++)
   {
   lpsz[i*2] = (char)((pData[i] & 0x0F) + 'A'); //low nibble
   lpsz[i*2+1] = (char)(((pData[i] >> 4) & 0x0F) + 'A'); //high nibble
   }
   lpsz[i*2] = 0;

   ASSERT(m_pszProfileName != NULL);

   bool bResult = WriteProfileString(lpszSection, lpszEntry, lpsz);
   delete[] lpsz;
   return bResult;
   }

   //#include "framework.h"*/




   /*   property_set & application::propset(object * pobject)
   {
   single_lock sl(&m_mapObjectSet, TRUE);
   return m_mapObjectSet[pobject];
   }

   property_set * application::existing_propset(object * pobject)
   {
   single_lock sl(&m_mapObjectSet, TRUE);
   auto p = m_mapObjectSet.PLookup(pobject);
   if(p == NULL)
   return NULL;
   return &p->m_value;
   }*/



   bool application::does_launch_window_on_startup()
   {
      return true;
   }

   bool application::activate_app()
   {
      if (m_puiMain != NULL)
      {
         ((::user::interaction *) m_puiMain->m_pvoidUserInteraction)->ShowWindow(SW_SHOWNORMAL);
      }
      return true;
   }



















   bool application::finalize()
   {

      bool bFinalize = true;

      try
      {
         ::base::application::finalize();
      }
      catch (...)
      {
         bFinalize = false;
      }

      return bFinalize;

   }




   sp(::user::interaction) application::get_request_parent_ui(sp(::user::interaction) pinteraction, ::create * pcreate)
   {

      sp(::user::interaction) puiParent = NULL;

      if (puiParent == NULL)
      {
         puiParent = pcreate->m_puiParent;
      }

      if (puiParent == NULL && pcreate->m_spApplicationBias.is_set())
      {
         puiParent = pcreate->m_spApplicationBias->m_puiParent;
      }

      //if(puiParent == NULL && m_pbasesession != NULL && m_pbasesession->m_pcoresession != NULL && !pcreate->m_bClientOnly
      if (puiParent == NULL && m_pbasesession != NULL && m_pbasesession->m_pcoresession != NULL
            && !pcreate->m_bOuterPopupAlertLike && m_pbasesession->m_pcoresession != this)
      {
         puiParent = Session.get_request_parent_ui(pinteraction, pcreate);
      }

      return puiParent;

   }







   void application::_001OnFileNew(signal_details * pobj)
   {

      var varFile;

      var varQuery;

      varQuery["command"] = "new_file";

      request_file_query(varFile, varQuery);

      pobj->m_bRet = true;

   }


   ::user::document * application::_001OpenDocumentFile(var varFile)
   {
      string strId = m_strId;
      char chFirst = '\0';
      if (strId.get_length() > 0)
      {
         chFirst = strId[0];
      }

      return NULL;

      //      return m_pimpl->_001OpenDocumentFile(varFile);

   }


   bool application::_001OnDDECommand(const char * lpcsz)
   {
      ::exception::throw_not_implemented(get_app());
      //return m_pimpl->_001OnDDECommand(lpcsz);

      return false;
   }

   //   ::core::file_system & application::file_system()
   // {
   //  return m_spfilesystem;
   //}







   ::window_sp application::get_desktop_window()
   {
#if defined(METROWIN) || defined(APPLEOS)
      throw todo(this);
      /*#elif defined(LINUX)

      //      synch_lock sl(&user_mutex());

      xdisplay pdisplay.
      pdisplay.open(NULL) = x11_get_display();

      oswindow window(pdisplay, DefaultRootWindow(pdisplay));

      XCloseDisplay(pdisplay);

      return window_from_os_data(window);
      */
#else
      return window_from_os_data(::GetDesktopWindow());
#endif
   }





   void application::assert_valid() const
   {
      thread::assert_valid();


      if (::get_thread() != (thread*)this)
         return;     // only do subset if called from different thread

      ASSERT(::get_thread() == this);
      //ASSERT(afxCurrentInstanceHandle == m_hInstance);

      /*      if (m_pdocmanager != NULL)
      ASSERT_VALID(m_pdocmanager);*/
   }

   void application::dump(dump_context & dumpcontext) const
   {

      ::base::application::dump(dumpcontext);

      //dumpcontext << "\nm_lpCmdLine = " << m_strCmdLine;
      //dumpcontext << "\nm_nCmdShow = " << m_nCmdShow;
      //dumpcontext << "\nm_pszAppName = " << m_strAppName;
      dumpcontext << "\nm_bHelpMode = " << m_bHelpMode;
      dumpcontext << "\nm_pszHelpFilePath = " << m_strHelpFilePath;
      dumpcontext << "\nm_pszProfileName = " << m_strProfileName;

#ifdef WINDOWS
      dumpcontext << "\nm_hDevMode = " << (void *)m_hDevMode;
      dumpcontext << "\nm_hDevNames = " << (void *)m_hDevNames;
#endif

      dumpcontext << "\nm_dwPromptContext = " << (UINT)m_dwPromptContext;
      //      dumpcontext << "\nm_eHelpType = " << m_eHelpType;


      /*      if (m_pdocmanager != NULL)
      document_manager().dump(dumpcontext);*/

      dumpcontext << "\nm_nWaitCursorCount = " << m_iWaitCursorCount;
      dumpcontext << "\nm_nNumPreviewPages = " << m_nNumPreviewPages;

      dumpcontext << "\n";
   }








   void application::install_message_handling(::message::dispatch * pdispatch)
   {
      base::application::install_message_handling(pdispatch);
   }




   bool application::base_support()
   {

      //if(!application::base_support())
      // return false;

      if (m_strBaseSupportId.is_empty())
      {

         property_set propertyset;

         message_box("err\\developer\\base_support\\support_id_not_specified.xml", propertyset);

         return false;

      }

      return true;
   }

   string application::message_box(const string & pszMatter, property_set & propertyset)
   {
      UNREFERENCED_PARAMETER(propertyset);
      UNREFERENCED_PARAMETER(pszMatter);
      return "";
   }

   bool application::bergedge_start()
   {
      string strId = m_strId;
      char chFirst = '\0';
      if (strId.get_length() > 0)
      {
         chFirst = strId[0];
      }

      return true;
   }








#ifdef WINDOWSEX
   bool Is_Vista_or_Later()
   {
      OSVERSIONINFOEX osvi;
      DWORDLONG dwlConditionMask = 0;
      byte op = VER_GREATER_EQUAL;

      // Initialize the OSVERSIONINFOEX structure.

      ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
      osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
      osvi.dwMajorVersion = 6;
      //   osvi.dwMinorVersion = 1;
      //   osvi.wServicePackMajor = 0;
      //   osvi.wServicePackMinor = 0;

      // Initialize the condition mask.

      VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, op);
      //VER_SET_CONDITION( dwlConditionMask, VER_MINORVERSION, op );
      //VER_SET_CONDITION( dwlConditionMask, VER_SERVICEPACKMAJOR, op );
      //VER_SET_CONDITION( dwlConditionMask, VER_SERVICEPACKMINOR, op );

      // Perform the test.

      return VerifyVersionInfo(
                &osvi,
                VER_MAJORVERSION | VER_MINORVERSION |
                VER_SERVICEPACKMAJOR | VER_SERVICEPACKMINOR,
                dwlConditionMask) != FALSE;
   }
#endif

   bool application::initialize()
   {

      if (!::base::application::initialize())
         return false;

      xxdebug_box("::base::application::initialize ok", "::base::application::initialize ok", MB_ICONINFORMATION);

      //m_pcalculator = new ::calculator::calculator(this);

      //m_pcalculator->construct(this);

      //if(!m_pcalculator->initialize())
      //   return false;

      xxdebug_box("m_pcalculator::initialize ok", "m_pcalculator::initialize ok", MB_ICONINFORMATION);

      xxdebug_box("m_pcolorertake5::initialize ok", "m_pcolorertake5::initialize ok", MB_ICONINFORMATION);

      m_dwAlive = ::get_tick_count();


      //m_dwAlive = ::get_tick_count();

      //if(is_system())
      //{

      //   System.m_spcopydesk.alloc(allocer());

      //   if(!System.m_spcopydesk->initialize())
      //      return false;

      //}

      //if(is_system()
      //   && command_thread()->m_varTopicQuery["app"] != "app-core/netnodelite"
      //   && command_thread()->m_varTopicQuery["app"] != "app-core/netnode_dynamic_web_server"
      //   && command_thread()->m_varTopicQuery["app"] != "app-gtech/alarm"
      //   && command_thread()->m_varTopicQuery["app"] != "app-gtech/sensible_service")
      //{
      //   System.http().defer_auto_initialize_proxy_configuration();
      //}



      //<<<<<<< .mine
      //      if(!System.m_spcopydesk->initialize())
      //         return false;
      //
      //   }
      //
      //   if(is_system()
      //      && command_thread()->m_varTopicQuery["app"] != "app-core/netnodelite"
      //      && command_thread()->m_varTopicQuery["app"] != "app-core/netnode_dynamic_web_server"
      //      && command_thread()->m_varTopicQuery["app"] != "app-gtech/sensible_netnode"
      //      && command_thread()->m_varTopicQuery["app"] != "app-gtech/sensible_service")
      //   {
      //      System.http().defer_auto_initialize_proxy_configuration();
      //   }
      //
      //
      //
      //=======
      //>>>>>>> .r7309
      m_dwAlive = ::get_tick_count();

      if (!initialize_userex())
      {

         return false;

      }

      //      if(!::cubebase::application::initialize())
      //       return false;


      //      m_puserbase = new ::user::user();

      //    m_puserbase->construct(this);

      //  if(!m_puserbase->initialize())
      //return false;


      xxdebug_box("m_pfilemanager::initialize ok", "m_pfilemanager::initialize ok", MB_ICONINFORMATION);


      xxdebug_box("m_pusermail::initialize ok", "m_pusermail::initialize ok", MB_ICONINFORMATION);

      m_dwAlive = ::get_tick_count();





      m_dwAlive = ::get_tick_count();

      //m_splicense(new class ::fontopus::license(this));


//      if(!is_system() && !is_session())
//      {
//
//         Session.register_bergedge_application(this);
//
//      }

      xxdebug_box("register_bergedge_application ok", "register_bergedge_application ok", MB_ICONINFORMATION);

      m_dwAlive = ::get_tick_count();

      ensure_app_interest();


      xxdebug_box("ensure_app_interest ok", "ensure_app_interest ok", MB_ICONINFORMATION);
      return true;

   }

   void application::pre_translate_message(signal_details * pobj)
   {
      SCAST_PTR(::message::base, pbase, pobj);
      if (pbase->m_uiMessage == WM_USER + 124 && pbase->m_pwnd == NULL)
      {
         /*      OnMachineEvent((flags < machine_event::e_flag> *) pmsg->lParam);
         delete (flags < machine_event::e_flag> *) pmsg->lParam;*/
         pbase->m_bRet = true;
         return;
      }
      return thread::pre_translate_message(pobj);
   }



   void application::_001CloseApplication()
   {

      post_quit();

   }


   void application::on_create_split_view(::user::split_view * psplit)
   {
   }



   void application::EnableShellOpen()
   {
      ASSERT(m_atomApp == 0 && m_atomSystemTopic == 0); // do once
      if (m_atomApp != 0 || m_atomSystemTopic != 0)
      {
         return;
      }

      // Win95 & Win98 sends a WM_DDE_INITIATE with an atom that points to the
      // int16_t file name so we need to use the int16_t file name.
      string strShortName;
      strShortName = System.file().module();

      // strip out path
      //string strFileName = ::PathFindFileName(strShortName);
      // strip out extension
      //LPTSTR pszFileName = strFileName.GetBuffer();
      //::PathRemoveExtension(pszFileName);
      //strFileName.ReleaseBuffer();

      //      m_atomApp = ::GlobalAddAtom(strFileName);
      //    m_atomSystemTopic = ::GlobalAddAtom("system");
   }



   sp(::user::interaction) application::uie_from_point(point pt)
   {

      user::interaction_spa wnda = *m_puiptraFrame;

      user::oswindow_array oswindowa;

      oswindowa = wnda.get_hwnda();

      user::window_util::SortByZOrder(oswindowa);

      for (int32_t i = 0; i < oswindowa.get_count(); i++)
      {

         sp(::user::interaction) puieWindow = wnda.find_first(oswindowa[i]);

         sp(::user::interaction) puie = puieWindow->_001FromPoint(pt);

         if (puie != NULL)
            return puie;

      }

      return NULL;

   }


   bool application::on_install()
   {

      if (!::base::application::on_install())
         return false;

      string strId = m_strId;

      char chFirst = '\0';

      if (strId.get_length() > 0)
      {

         chFirst = strId[0];

      }

      return true;

   }


   bool application::on_run_install()
   {
      if (m_strId == "session" || m_strAppName == "session")
      {
         if (!directrix()->m_varTopicQuery.has_property("session_start"))
         {
            ::multithreading::post_quit(&System);
         }
      }
      else
      {
         ::multithreading::post_quit(&System);
      }


      return true;
   }

   bool application::on_uninstall()
   {

      bool bOk = ::base::application::on_uninstall();

      string strId = m_strId;
      char chFirst = '\0';
      if (strId.get_length() > 0)
      {
         chFirst = strId[0];
      }

      return bOk;
   }

   bool application::on_run_uninstall()
   {

      if (m_strId == "session")
      {
         if (!directrix()->m_varTopicQuery.has_property("session_start"))
         {
            ::multithreading::post_quit(&System);
         }
      }
      else
      {
         ::multithreading::post_quit(&System);
      }

      return true;
   }





   void application::on_application_signal(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      //      SCAST_PTR(signal_details, psignal, pobj);
      /*if(psignal->m_esignal == signal_exit_instance)
      {
      if(m_copydesk.is_set()
      && m_copydesk->IsWindow())
      {
      m_copydesk->DestroyWindow();
      }
      }*/
   }


   sp(::aura::printer) application::get_printer(const char * pszDeviceName)
   {

      return NULL;

   }




   bool application::set_keyboard_layout(const char * pszPath, ::action::context actioncontext)
   {

      return Session.keyboard().load_layout(pszPath, actioncontext);

   }






   int32_t application::track_popup_menu(const char * pszMatter, point pt, sp(::user::interaction) puie)
   {
      UNREFERENCED_PARAMETER(pszMatter);
      UNREFERENCED_PARAMETER(pt);
      UNREFERENCED_PARAMETER(puie);
      return 1;
   }



   bool application::get_fs_size(string & strSize, const char * pszPath, bool & bPending)
   {
      int64_t i64Size;
      if (!get_fs_size(i64Size, pszPath, bPending))
      {
         strSize.Empty();
         return false;
      }
      if (i64Size > 1024 * 1024 * 1024)
      {
         double d = (double)i64Size / (1024.0 * 1024.0 * 1024.0);
         strSize.Format("%0.2f GB", d);
      }
      else if (i64Size > 1024 * 1024)
      {
         double d = (double)i64Size / (1024.0 * 1024.0);
         strSize.Format("%0.1f MB", d);
      }
      else if (i64Size > 1024)
      {
         double d = (double)i64Size / (1024.0);
         strSize.Format("%0.0f KB", d);
      }
      else if (i64Size > 0)
      {
         strSize.Format("1 KB");
      }
      else
      {
         strSize.Format("0 KB");
      }
      if (bPending)
      {
         strSize = "~" + strSize;
      }
      return true;
   }

   bool application::get_fs_size(int64_t & i64Size, const char * pszPath, bool & bPending)
   {
      db_server * pcentral = dynamic_cast <db_server *> (&System.m_simpledb.db());
      if (pcentral == NULL)
         return false;
      return pcentral->m_pfilesystemsizeset->get_cache_fs_size(i64Size, pszPath, bPending);
   }


   void application::set_title(const char * pszTitle)
   {

      Session.set_app_title(m_strAppName, pszTitle);

   }


   bool application::_001CloseApplicationByUser(sp(::user::interaction) pwndExcept)
   {

      // attempt to save all documents
      if (!save_all_modified())
         return false;     // don't close it

      // hide the application's windows before closing all the documents
      HideApplication();

      close(end_app);

      return true;

   }


   oswindow application::get_ca2_app_wnd(const char * psz)
   {
      UNREFERENCED_PARAMETER(psz);
      return NULL;
   }


   int32_t application::send_simple_command(const char * psz, void * osdataSender)
   {
      string strApp;
      stringa stra;
      stra.add_tokens(psz, "::", true);
      if (stra.get_size() > 0)
      {
         strApp = stra[0];
         oswindow oswindow = get_ca2_app_wnd(strApp);
         if (oswindow != NULL)
         {
            return send_simple_command((void *)oswindow, psz, osdataSender);
         }
      }
      return -1;
   }

   int32_t application::send_simple_command(void * osdata, const char * psz, void * osdataSender)
   {
#ifdef WINDOWSEX
      ::oswindow oswindow = (::oswindow) osdata;
      if (!::IsWindow(oswindow))
         return -1;
      COPYDATASTRUCT cds;
      memset(&cds, 0, sizeof(cds));
      cds.dwData = 888888;
      cds.cbData = (uint32_t)strlen(psz);
      cds.lpData = (PVOID)psz;
      return (int32_t)SendMessage(oswindow, WM_COPYDATA, (WPARAM)osdataSender, (LPARAM)&cds);
#else
      throw todo(get_app());
#endif
   }


   void application::ensure_app_interest()
   {

#ifndef METROWIN

      for (int32_t i = 0; i < m_straAppInterest.get_count(); i++)
      {
         if (m_straAppInterest[i] != m_strAppName && !::IsWindow(m_mapAppInterest[m_straAppInterest[i]]))
         {
            System.assert_running_local(m_straAppInterest[i]);
         }
      }

#else

      //throw todo(get_app());

#endif

   }




   sp(::aura::application) application::get_system()
   {
      return new application();
   }


















   /*
   sp(::core::application) application::assert_running(const char * pszAppId)
   {


   sp(::core::application) papp = NULL;


   try
   {

   bool bFound = false;

   for(int32_t i  = 0; i < System.m_appptra.get_count(); i++)
   {
   try
   {

   papp = System.m_appptra(i);

   if(papp->m_pcoreapp->m_strAppName == pszAppId)
   {
   bFound = true;
   break;
   }

   }
   catch(...)
   {
   }

   }

   bool bCreate = !bFound;

   if(bFound)
   {

   bool bRunning = false;

   try
   {
   if(papp->m_pcoreapp->is_running())
   {
   bRunning = true;
   }
   }
   catch(...)
   {
   }


   if(bCreate)
   {

   sp(::create) spcreate(allocer());

   papp = Session.start_application("application", pszAppId, spcreate);

   }

   }
   catch(const ::exit_exception & e)
   {

   throw e;

   }
   catch(const ::exception::exception & e)
   {

   if(!Application.on_run_exception((::exception::exception &) e))
   throw exit_exception(get_app());

   }
   catch(...)
   {

   papp = NULL;

   }


   return papp;

   }
   */





   //typedef  void (* PFN_ca2_factory_exchange)(::aura::application * papp);




   ::file::file_sp application::friendly_get_file(var varFile, UINT nOpenFlags)
   {

      try
      {

         return Session.m_spfile->get_file(varFile, nOpenFlags);

      }
      catch (::file::exception & e)
      {

         string strMessage = e.get_message();

         App(this).simple_message_box(NULL, strMessage, MB_OK);

         return NULL;

      }

   }











   //sp(::aura::application) application::create_application(const char * pszType,const char * pszId,bool bSynch,application_bias * pbias)
   //{

   //   sp(::aura::application) pbaseapp = instantiate_application(pszType,pszId,pbias);

   //   if(pbaseapp == NULL)
   //      return NULL;

   //   ::aura::application * papp = (pbaseapp);

   //   if(!papp->m_pcoreapp->start_application(bSynch,pbias))
   //   {
   //      try
   //      {
   //         pbaseapp.release();
   //      }
   //      catch(...)
   //      {
   //      }
   //      return NULL;
   //   }


   //   return pbaseapp;

   //}


   ::user::document * application::place_hold(::user::interaction * pui)
   {

      return NULL;

   }



   bool application::add_library(::aura::library * plibrary)
   {

      plibrary->set_app(this);

      System.add_library(plibrary);

      return true;

   }


   void application::data_on_after_change(signal_details * pobj)
   {
      SCAST_PTR(::database::change_event, pchange, pobj);
      if (pchange->m_key.m_id == "ca2.savings")
      {
         pchange->data_get(Session.savings().m_eresourceflagsShouldSave);
      }
   }





   int32_t application::GetVisibleTopLevelFrameCountExcept(sp(::user::interaction) pwndExcept)
   {

      ::user::interaction_spa wnda = *m_puiptraFrame;

      int32_t iCount = 0;
      for (int32_t i = 0; i < wnda.get_size(); i++)
      {
         sp(::user::interaction) pwnd = wnda.element_at(i);
         if (pwnd != NULL &&
               pwnd != pwndExcept &&
               pwnd->IsWindow() &&
               pwnd->IsWindowVisible() &&
               pwnd->get_window_type() == ::user::interaction::type_frame &&
               !(pwnd->GetStyle() & WS_CHILD))
         {
            iCount++;
         }
      }
      return iCount;
   }


   int32_t application::GetVisibleFrameCount()
   {

      ::user::interaction_spa wnda = *m_puiptraFrame;

      int32_t iCount = 0;
      for (int32_t i = 0; i < wnda.get_size(); i++)
      {
         sp(::user::interaction) pwnd = wnda.element_at(i);
         if (pwnd != NULL
               && pwnd->IsWindow()
               && pwnd->IsWindowVisible())
         {
            iCount++;
         }
      }
      return iCount;
   }


   void application::on_create_keyboard()
   {

      string str;

      // keyboard on_layout
      //if(data_get("keyboard_layout",str) && str.has_char())
      {
         // Session.set_keyboard_layout(str,::action::source::database());
      }
      //else
      {
         Session.set_keyboard_layout(NULL, ::action::source::database());
      }

   }


   //sp(type) application::user_default_controltype_to_typeinfo(enum user::e_control_type econtroltype)
   //{

   //   return Sess(this).userex()->controltype_to_typeinfo(econtroltype);

   //}


   bool application::platform_open_by_file_extension(int iEdge, const char * pszPathName, application_bias * pbiasCreate)
   {

      return System.get_platform(iEdge)->open_by_file_extension(pszPathName, pbiasCreate);
   }

   bool application::platform_open_by_file_extension(int iEdge, ::create * pcc)
   {

      return System.get_platform(iEdge)->open_by_file_extension(pcc);
   }


   sp(::aura::application) application::create_platform(::aura::session * psession)
   {

      return canew(::core::session(psession));

   }


   void application::on_show_view(::user::tab * ptab)
   {



   }



   string application::get_cred(string & strUsername, string & strPassword, string strToken)
   {

      return ::base::application::fontopus_get_cred(strUsername, strPassword, strToken);

   }

   void application::set_cred(string strToken, const char * pszUsername, const char * pszPassword)
   {

      ::base::application::set_cred(strToken, pszUsername, pszPassword);

   }

   void application::set_cred_ok(string strToken, bool bOk)
   {

      ::base::application::set_cred_ok(strToken, bOk);

   }


   bool application::_001OnAgreeExit()
   {

      document_manager().close_all_documents(true);

      return true;

   }



} // namespace core















#ifdef WINDOWSEX

BOOL LaunchAppIntoDifferentSession(const char * pszProcess, const char * pszCommand, const char * pszDir, STARTUPINFO * psi, PROCESS_INFORMATION * ppi, int iSession)
{
   //PROCESS_INFORMATION pi;
   //STARTUPINFO si;
   BOOL bResult = FALSE;
   DWORD dwSessionId, winlogonPid;
   HANDLE hUserToken, hUserTokenDup, hPToken, hProcess;
   DWORD dwCreationFlags;

   // Log the client on to the local computer.

   if (iSession < 0)
   {
      dwSessionId = WTSGetActiveConsoleSessionId();
   }
   else
   {
      dwSessionId = iSession;
   }

   //////////////////////////////////////////
   // Find the winlogon process
   ////////////////////////////////////////

   PROCESSENTRY32 procEntry;

   HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
   if (hSnap == INVALID_HANDLE_VALUE)
   {
      return 1;
   }

   procEntry.dwSize = sizeof(PROCESSENTRY32);

   if (!Process32First(hSnap, &procEntry))
   {
      return 1;
   }

   do
   {
      if (_stricmp(procEntry.szExeFile, "winlogon.exe") == 0)
      {
         // We found a winlogon process...
         // make sure it's running in the console session
         DWORD winlogonSessId = 0;
         HANDLE h = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, procEntry.th32ProcessID);
         if (ProcessIdToSessionId(procEntry.th32ProcessID, &winlogonSessId))
         {
            if (winlogonSessId == dwSessionId)
            {
               winlogonPid = procEntry.th32ProcessID;
               break;

            }
         }
         else
         {
            DWORD dwLastError = GetLastError();

            //            APPTRACE(::get_thread_app())("%d", dwLastError);
         }
      }

   } while (Process32Next(hSnap, &procEntry));

   ////////////////////////////////////////////////////////////////////////

   LIBCALL(wtsapi32, WTSQueryUserToken)(dwSessionId, &hUserToken);
   dwCreationFlags = NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE;
   //ZeroMemory(&si,sizeof(STARTUPINFO));
   psi->cb = sizeof(STARTUPINFO);
   psi->lpDesktop = "winsta0\\default";
   //ZeroMemory(&pi,sizeof(pi));
   TOKEN_PRIVILEGES tp;
   LUID luid;
   hProcess = OpenProcess(MAXIMUM_ALLOWED, FALSE, winlogonPid);

   if (!::OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY
                           | TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY | TOKEN_ADJUST_SESSIONID
                           | TOKEN_READ | TOKEN_WRITE, &hPToken))
   {
      int abcd = GetLastError();
      debug_print("Process token open Error: %u\n", GetLastError());
   }

   if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
   {
      debug_print("Lookup Privilege value Error: %u\n", GetLastError());
   }
   tp.PrivilegeCount = 1;
   tp.Privileges[0].Luid = luid;
   tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

   DuplicateTokenEx(hPToken, MAXIMUM_ALLOWED, NULL,
                    SecurityIdentification, TokenPrimary, &hUserTokenDup);
   int dup = GetLastError();

   //Adjust Token privilege
   SetTokenInformation(hUserTokenDup,
                       TokenSessionId, (void*)(DWORD_PTR)dwSessionId, sizeof(DWORD));

   if (!AdjustTokenPrivileges(hUserTokenDup, FALSE, &tp, sizeof(TOKEN_PRIVILEGES),
                              (PTOKEN_PRIVILEGES)NULL, NULL))
   {
      int abc = GetLastError();
      debug_print("Adjust Privilege value Error: %u\n", GetLastError());
   }

   if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
   {
      debug_print("Token does not have the provilege\n");
   }

   LPVOID pEnv = NULL;

   if (LIBCALL(userenv, CreateEnvironmentBlock)(&pEnv, hUserTokenDup, TRUE))
   {
      dwCreationFlags |= CREATE_UNICODE_ENVIRONMENT;
   }
   else
      pEnv = NULL;

   // Launch the process in the client's logon session.

   bResult = CreateProcessAsUser(
                hUserTokenDup,                     // client's access token
                pszProcess,    // file to execute
                (char *)pszCommand,                 // command line
                NULL,            // pointer to process SECURITY_ATTRIBUTES
                NULL,               // pointer to thread SECURITY_ATTRIBUTES
                FALSE,              // handles are not inheritable
                dwCreationFlags,     // creation flags
                pEnv,               // pointer to _new environment block
                pszDir,               // name of current directory
                psi,               // pointer to STARTUPINFO structure
                ppi                // receives information about _new process
             );
   // End impersonation of client.

   //GetLastError Shud be 0

   int iResultOfCreateProcessAsUser = GetLastError();

   //Perform All the Close Handles tasks

   CloseHandle(hProcess);
   CloseHandle(hUserToken);
   CloseHandle(hUserTokenDup);
   CloseHandle(hPToken);

   return 0;
}

bool enable_windows_token_privilege(HANDLE h, LPCSTR lpcszName)
{

   TOKEN_PRIVILEGES tp;

   if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid))
   {

      int iError = GetLastError();

      debug_print("Lookup Privilege value Error: %u\n", iError);

      return false;

   }

   tp.PrivilegeCount = 1;

   tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

   if (!AdjustTokenPrivileges(h, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, NULL))
   {

      int iError = GetLastError();

      debug_print("Adjust Privilege value Error: %u\n", iError);

      return false;

   }

   return true;

}

BOOL LaunchAppIntoSystemAcc(const char * pszProcess, const char * pszCommand, const char * pszDir, STARTUPINFO * psi, PROCESS_INFORMATION * ppi)
{
   //PROCESS_INFORMATION pi;
   //STARTUPINFO si;
   BOOL bResult = FALSE;
   //   DWORD dwSessionId,winlogonPid;
   HANDLE hUserTokenDup, hProcess, hPToken;
   DWORD dwCreationFlags;
   HANDLE hUserToken = NULL;


   // Log the client on to the local computer.

   dwCreationFlags = NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE;
   //ZeroMemory(&si,sizeof(STARTUPINFO));
   psi->cb = sizeof(STARTUPINFO);
   psi->lpDesktop = "winsta0\\default";
   //ZeroMemory(&pi,sizeof(pi));

//   LUID luid;
   //hProcess = OpenProcess(MAXIMUM_ALLOWED,FALSE,winlogonPid);
   hProcess = ::GetCurrentProcess();

   //hPToken = hUserToken;

   if (!::OpenProcessToken(hProcess, TOKEN_ALL_ACCESS, &hPToken))
   {
      int abcd = GetLastError();
      debug_print("Process token open Error: %u\n", GetLastError());
   }

   if (!enable_windows_token_privilege(hPToken, SE_DEBUG_NAME))
   {

      return FALSE;

   }

   if (!enable_windows_token_privilege(hPToken, SE_CREATE_TOKEN_NAME))
   {
      return FALSE;
   }

   if (!enable_windows_token_privilege(hPToken, SE_TCB_NAME))
   {

      return FALSE;

   }

   if (!enable_windows_token_privilege(hPToken, SE_ASSIGNPRIMARYTOKEN_NAME))
   {

      return FALSE;

   }

   if (!enable_windows_token_privilege(hPToken, SE_INCREASE_QUOTA_NAME))
   {

      return FALSE;

   }

   //if(GetLastError() == ERROR_NOT_ALL_ASSIGNED)
   //{
   //   debug_print("Token does not have the provilege\n");
   //}
   // "LOCAL SERVICE" or "LocalService" ?
   // "NETWORK SERVICE" or "NetworkService" ?
   if (!LogonUserW(L"LocalService", L"NT AUTHORITY", NULL, LOGON32_LOGON_SERVICE, LOGON32_PROVIDER_DEFAULT, &hUserToken))
   {
      DWORD dwError = ::GetLastError();
      string str;
      str.Format("Lookup Privilege value Error: %u\n", dwError);
      ::MessageBox(NULL, str, "Help Me", MB_OK);
      return FALSE;
   }
   if (!DuplicateTokenEx(hUserToken, TOKEN_ALL_ACCESS, NULL, SecurityDelegation, TokenPrimary, &hUserTokenDup))
   {
      int dup = GetLastError();
      debug_print("DuplicateTokenEx Error: %u\n", GetLastError());
   }

   //Adjust Token privilege
   //SetTokenInformation(hUserTokenDup,
   //   TokenSessionId,(void*)dwSessionId,sizeof(DWORD));



   LPVOID pEnv = NULL;

   if (LIBCALL(userenv, CreateEnvironmentBlock)(&pEnv, hUserTokenDup, TRUE))
   {
      dwCreationFlags |= CREATE_UNICODE_ENVIRONMENT;
   }
   else
      pEnv = NULL;

   // Launch the process in the client's logon session.

   bResult = CreateProcessAsUser(
                hUserTokenDup,                     // client's access token
                pszProcess,    // file to execute
                (char *)pszCommand,                 // command line
                NULL,            // pointer to process SECURITY_ATTRIBUTES
                NULL,               // pointer to thread SECURITY_ATTRIBUTES
                FALSE,              // handles are not inheritable
                dwCreationFlags,     // creation flags
                pEnv,               // pointer to _new environment block
                pszDir,               // name of current directory
                psi,               // pointer to STARTUPINFO structure
                ppi                // receives information about _new process
             );
   // End impersonation of client.

   //GetLastError Shud be 0

   //int iResultOfCreateProcessAsUser = GetLastError();

   //Perform All the Close Handles tasks

   //CloseHandle(hProcess);
   //CloseHandle(hUserToken);
   //CloseHandle(hUserTokenDup);
   //CloseHandle(hPToken);

   return TRUE;
}


#endif // WINDOWSEX





