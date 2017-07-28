#include "framework.h"




#ifdef WINDOWSEX
#include <dde.h>
#endif

#define TEST 0

bool wm_set_icon(oswindow w, ::draw2d::dib * p);
bool wm_set_icon(oswindow w, stringa & straMatter);
extern CLASS_DECL_CORE thread_int_ptr < DWORD_PTR > t_time1;

manual_reset_event * simple_frame_window::helper_task::g_pevent = NULL;

simple_frame_window::helper_task::helper_task(simple_frame_window * pframe) :
   ::thread(pframe->get_app()),
   m_pframe(pframe) //,
//m_ev(pframe->get_app())
{

   if(g_pevent == NULL)
   {

      g_pevent = new manual_reset_event(pframe->get_app());

   }

   begin();

}


int simple_frame_window::helper_task::run()
{

   while(get_run_thread())
   {


      g_pevent->wait(millis(84));

      // the computer maybe blown here, where there is no code (when it is not running)... by falling into a curve in a road from a truck or by the multiverses bramas collapsing into a high energy dot.com... and bubble restarts when the spirtual world decides for restarting the virtual machine - with some pauses - as we does not detect change in time vector, as it is a non-readable, executable/paused/non existent only register in the parent processor... Imagine a overhaul upgrade with much more strings in the chords, why they mantain consitency between virtual machines versions... they like to hinder a lot!! strange, this is a hello 666... // and the time they have to overhaul is infinite, because they can pause our ticker... besides I hope no ones stops their tick counters...
      if(m_pframe->m_bSizeMove) // not here, any error here (or am i wrong, the OpSys may not have started the FULLStack DevOp).... because it is lInUx... its not ADVENTURE_Clean_NoERRORs_may_be_old_tommorrow_just_EX_OS...
      {

         if( // slim and clean people (no drugs) do not like fatty acids double check in the bag( unless they are slim and clean?!?!?!)
            ::get_tick_count() - m_pframe->m_dwLastSizeMove > 284)// the tester (without UPS) can loose a save specially here (where is the error, sixes or 666) // Halloween is coming
            // this a reason for using manual_reset_event for every simple_frame_window, accepts the candy, and the trick? you get both, this the whole trick!!!
         {

            //::MessageBox(NULL, "Going to save window rect", "SaveWindowRect", MB_ICONEXCLAMATION);

            m_pframe->m_bSizeMove = false;// the tester (without UPS) can loose a save here

            try
            {

               TRACE("m_pframe->does_display_match() the tester is about to get close to get sad... (yes, he is mad...)");
               if(m_pframe->does_display_match()) // the tester (without UPS) can loose a save even here
               {

                  TRACE("m_pframe->WindowDataSaveWindowRect() the tester is close to get sad... (yes, he is mad...)");
                  m_pframe->WindowDataSaveWindowRect(); // the tester (without UPS) can loose a save here

               }

            }
            catch(...)
            {

            }

         }

      }
      //Sleep(184); // the tester (without UPS) can loose a save here

   }

   return 0;

}


void simple_frame_window::helper_task::defer_save_window_rect()
{
}


simple_frame_window::simple_frame_window(::aura::application * papp) :
   object(papp),
   m_dibBk(allocer()),
   m_fastblur(allocer())
{

   m_etranslucency = ::user::TranslucencyUndefined;

   m_bShowTask = true;

   m_bDefaultNotifyIcon = false;

   m_bTransparentFrame = false;

   m_bblur_Background = false;
   m_bCustomFrameBefore = true;
   m_bWindowFrame = true;
   m_bLayered = true;
   m_pframeschema = NULL;
   m_pdocumenttemplate = NULL;

   //m_phelpertask = new helper_task(this);

}

simple_frame_window::~simple_frame_window()
{
   //m_phelpertask->m_pframe = NULL;
   //m_phelpertask->m_bRun = false;
}

void simple_frame_window::assert_valid() const
{

   ::user::frame_window::assert_valid();

}

void simple_frame_window::dump(dump_context & dumpcontext) const
{

   ::user::frame_window::dump(dumpcontext);

}

// // BEGIN_MESSAGE_MAP(simple_frame_window, ::user::frame_window)
//{{__MSG_MAP(simple_frame_window)
/* xxx   ON_WM_DESTROY()
   ON_WM_SIZE()
   ON_WM_CLOSE()
   ON_WM_NCCALCSIZE()
   ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS_BAR, OnUpdateControlBarMenu)
   ON_COMMAND_EX(ID_VIEW_STATUS_BAR, OnBarCheck)
   ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, OnUpdateControlBarMenu)
   ON_COMMAND_EX(ID_VIEW_TOOLBAR, OnBarCheck)
   ON_UPDATE_COMMAND_UI(ID_VIEW_REBAR, OnUpdateControlBarMenu)
   ON_COMMAND_EX(ID_VIEW_REBAR, OnBarCheck) */
//}}__MSG_MAP
// // END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// simple_frame_window message handlers

void simple_frame_window::install_message_handling(::message::dispatch * pinterface)
{
   ::user::frame_window::install_message_handling(pinterface);
   IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &simple_frame_window::_001OnCreate);
   IGUI_WIN_MSG_LINK(WM_NCACTIVATE, pinterface, this, &simple_frame_window::_001OnNcActivate);
#ifdef WINDOWSEX
   IGUI_WIN_MSG_LINK(WM_DDE_INITIATE   , pinterface, this, &simple_frame_window::_001OnDdeInitiate);
#endif
   IGUI_WIN_MSG_LINK(WM_DESTROY, pinterface, this, &simple_frame_window::_001OnDestroy);
   IGUI_WIN_MSG_LINK(WM_CLOSE, pinterface, this, &simple_frame_window::_001OnClose);
   IGUI_WIN_MSG_LINK(WM_SIZE, pinterface, this, &simple_frame_window::_001OnSize);
   IGUI_WIN_MSG_LINK(WM_SYSCOMMAND, pinterface, this, &simple_frame_window::_001OnSysCommand);
   IGUI_WIN_MSG_LINK(WM_GETMINMAXINFO, pinterface, this, &simple_frame_window::_001OnGetMinMaxInfo);
   IGUI_WIN_MSG_LINK(WM_USER + 184, pinterface, this, &simple_frame_window::_001OnUser184);
   IGUI_WIN_MSG_LINK(WM_MOUSEMOVE, pinterface, this, &simple_frame_window::_001OnMouseMove);
   IGUI_WIN_MSG_LINK(WM_DISPLAYCHANGE, pinterface, this, &simple_frame_window::_001OnDisplayChange);
   IGUI_WIN_MSG_LINK(WM_SHOWWINDOW, pinterface, this, &simple_frame_window::_001OnShowWindow);

   connect_update_cmd_ui("transparent_frame", &simple_frame_window::_001OnUpdateToggleTransparentFrame);
   connect_command("transparent_frame", &simple_frame_window::_001OnToggleTransparentFrame);

   connect_update_cmd_ui("view_full_screen", &simple_frame_window::_001OnUpdateViewFullScreen);
   connect_command("view_full_screen", &simple_frame_window::_001OnViewFullScreen);

   connect_command("app_exit", &simple_frame_window::_001OnAppExit);

   IGUI_WIN_MSG_LINK(WM_APPEXIT, pinterface, this, &simple_frame_window::_001OnAppExit);
   IGUI_WIN_MSG_LINK(WM_ACTIVATEAPP, pinterface, this, &simple_frame_window::_001OnActivateApp);
   IGUI_WIN_MSG_LINK(WM_ACTIVATE, pinterface, this, &simple_frame_window::_001OnActivate);

#ifdef WINDOWSEX

   IGUI_WIN_MSG_LINK(System.m_uiWindowsTaskbarCreatedMessage, pinterface, this, &simple_frame_window::_001OnTaskbarCreated);

#endif

}




bool simple_frame_window::IsFullScreen()
{

   return WfiIsFullScreen();

}


sp(::user::interaction) simple_frame_window::WindowDataGetWnd()
{

   return this;

}


void simple_frame_window::_001OnDestroy(signal_details * pobj)
{

   pobj->previous();

   try
   {

      if (m_pnotifyicon != NULL)
      {

         m_pnotifyicon->Destroy();

      }

   }
   catch(...)
   {

   }


   try
   {
      if (m_pauraapp != NULL && &Application != NULL)
      {
         Application.remove_frame(this);
      }
   }
   catch (...)
   {
   }

   try
   {
      if (&Session != NULL)
      {
         Session.remove_frame(this);
      }
   }
   catch (...)
   {
   }

   try
   {
      if (m_pauraapp != NULL && m_pauraapp->m_pcoresystem != NULL && &System != NULL)
      {
         System.remove_frame(this);
      }
   }
   catch (...)
   {
   }

}


sp(::user::wndfrm::frame::frame) simple_frame_window::create_frame_schema()
{

   sp(::user::wndfrm::frame::frame) pschema = Application.wndfrm().get_frame_schema(m_varFrame["wndfrm"], m_varFrame["schema"]);

   pschema->set_style(m_varFrame["style"]);




   return pschema;

}


#ifdef WINDOWSEX

HICON load_icon(::aura::application * papp, stringa & straMatter, string strIcon, int cx, int cy)
{

   HICON hicon = NULL;

   ::file::path path;

   for (auto & strMatter : straMatter)
   {

      path = strMatter;

      path = App(papp).dir().matter(path / strIcon);

      hicon = (HICON)LoadImage(NULL, path, IMAGE_ICON, cx, cy, LR_LOADFROMFILE);

      if (hicon != NULL)
      {

         break;

      }

   }

   return hicon;

}

#else

bool load_icon(::visual::dib_sp & d, ::aura::application * papp, stringa & straMatter, string strIcon)
{

   ::file::path path;

   for (auto & strMatter : straMatter)
   {

      path = strMatter;

      path = App(papp).dir().matter(path / strIcon);

      if(d.load_from_file(path))
      {

         return true;

      }

   }

   return false;

}

#endif


void simple_frame_window::_001OnCreate(signal_details * pobj)
{

   SCAST_PTR(::message::create, pcreate, pobj);

   if (m_pdocumenttemplate->m_strMatter.has_char())
   {

      m_varFrame = Application.file().as_json("matter://" + m_pdocumenttemplate->m_strMatter + "/frame.json");

   }

   if (pobj->previous())
   {

      return;

   }

   if (m_varFrame["schema"].is_empty())
   {

      m_varFrame["schema"] = "005";

   }
   
   if (m_varFrame["style"].is_empty())
   {

      m_varFrame["style"] = "DarkWarmBlue";

   }

   if (m_varFrame["control_box"]["transparent_button"]["visible"].is_false())
   {

      m_workset.m_ebuttonaHide.add(::user::wndfrm::frame::button_transparent_frame);

   }
   if (m_varFrame["control_box"]["dock_button"]["visible"].is_false())
   {

      m_workset.m_ebuttonaHide.add(::user::wndfrm::frame::button_dock);

   }

   if (m_workset.m_ebuttonaHide.contains(::user::wndfrm::frame::button_transparent_frame))
   {

      m_bTransparentFrame = false;

   }
   else
   {

      data_get("transparent_frame", m_bTransparentFrame);

   }

   sp(::user::place_holder) pplaceholder = GetParent();

   if (pplaceholder != NULL)
   {
      sp(::user::place_holder_container) pcontainer = pplaceholder->GetParent();
      if (pcontainer != NULL)
      {
         pcontainer->on_hold(this, pplaceholder);
      }
   }

   if (m_bAutoWindowFrame)
   {
      if(Application.command()->m_varTopicQuery.has_property("client_only"))
      {
         m_bWindowFrame = false;
      }
      else
      {
#if defined(METROWIN) || defined(APPLE_IOS)
         m_bWindowFrame = GetParent() == NULL || GetParent()->m_pimpl != NULL;
#else
         m_bWindowFrame = GetParent() == NULL;
#endif
      }
   }

   if (m_bWindowFrame)
   {
      /*WNDCLASS wndclass;

      char szBuf [64];
      ::GetClassName(get_handle(), szBuf, _countof(szBuf));

      GetClassInfo(System.m_hInstance,
      szBuf,
      &wndclass);*/

      // trans      SetIcon(wndclass.hIcon, false);

      // trans      HICON hicon = GetIcon(false);

      sp(::user::wndfrm::frame::frame) pinteractionframe = NULL;

      try
      {


         pinteractionframe = create_frame_schema();

      }
      catch (not_installed & e)
      {

         System.remove_frame(this);

         throw e;

      }
      catch (...)
      {

         pcreate->m_lresult = -1;

         pcreate->m_bRet = true;

         return;

      }
      if (m_puserschemaSchema == NULL)
      {

         string strSchema = m_varFrame["wndfrm"];
         
         m_puserschemaSchema = Session.get_user_schema(strSchema, get_app());

      }

      if (m_puserschemaSchema == NULL)
      {

         m_puserschemaSchema = Application.userschema();




      }

      //frame::FrameSchema * pschema = dynamic_cast < ::frame::FrameSchema * > (pinteractionframe);

      if (pinteractionframe != NULL && (_ca_is_basis() || Application.command()->m_varTopicQuery["version"] == "basis"))
      {

         //pinteractionframe->set_style("BlueRedPurple");

      }

      /*{
         frame::FrameSchemaHardCoded001 * pschemaSpec = dynamic_cast < frame::FrameSchemaHardCoded001 * > (pschema);
         if(pschemaSpec != NULL && (_ca_is_basis() || Application.command()->m_varTopicQuery["version"] == "basis"))
         {
         pschemaSpec->SetStyle(frame::FrameSchemaHardCoded001::StyleBlueRedPurple);
         }
         }
         {
         frame::FrameSchemaHardCoded002 * pschemaSpec = dynamic_cast < frame::FrameSchemaHardCoded002 * > (pschema);
         if(pschemaSpec != NULL && (_ca_is_basis() || Application.command()->m_varTopicQuery["version"] == "basis"))
         {
         pschemaSpec->SetStyle(frame::FrameSchemaHardCoded002::StyleBlueRedPurple);
         }
         }
         {
         frame::FrameSchemaHardCoded005 * pschemaSpec = dynamic_cast < frame::FrameSchemaHardCoded005 * > (pschema);
         if(pschemaSpec != NULL && (_ca_is_basis() || Application.command()->m_varTopicQuery["version"] == "basis"))
         {
         pschemaSpec->SetStyle(frame::FrameSchemaHardCoded005::StyleBlueRedPurple);
         }
         }
         {
         frame::FrameSchemaHardCoded008 * pschemaSpec = dynamic_cast < frame::FrameSchemaHardCoded008 * > (pschema);
         if(pschemaSpec != NULL && (_ca_is_basis() || Application.command()->m_varTopicQuery["version"] == "basis"))
         {
         pschemaSpec->SetStyle(frame::FrameSchemaHardCoded008::StyleBlueRedPurple);
         }
         }*/

      m_pframeschema = pinteractionframe;
      m_workset.AttachFrameSchema(m_pframeschema);
      if (!m_workset.update(
               this,
               this,
               this,
               this))
      {
         pcreate->m_bRet = false;
         return;
      }

#if defined(LINUX) || defined(APPLEOS)

      m_workset.SetActiveFlag(TRUE);

#endif

   }


   if (get_window_text().is_empty())
   {

      set_window_text(Application.get_app_user_friendly_task_bar_name());

   }


   //if (GetParent() == NULL)
   {

      defer_synch_layered();

   }


   create_bars();

   defer_set_icon();

   defer_create_notification_icon();

   m_pimpl->show_task(m_bShowTask);

   //post_message(WM_USER + 184, 2);

   pcreate->m_bRet = false;

}


void simple_frame_window::defer_set_icon()
{

#ifdef WINDOWSEX

   if (GetParent() == NULL)
   {
      ::file::path strMatter = get_window_default_matter();
      //http://www.cplusplus.com/forum/general/28470/
      //blackcoder41 (1426)  Sep 12, 2010 at 2:43pm
      //hIconSm = (HICON)LoadImage(NULL, "menu_two.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);

      stringa straMatter;

      if (strMatter.name(0) == "system")
      {

         straMatter.add("main");

         straMatter.add(strMatter);

      }
      else
      {

         straMatter.add(strMatter);

         straMatter.add("main");

      }

      HICON hicon = load_icon(get_app(), straMatter, "icon.ico", 16, 16);

      if (hicon != NULL)
      {

         SendMessage(get_handle(), (UINT)WM_SETICON, ICON_SMALL, (LPARAM)hicon);

      }

      hicon = load_icon(get_app(), straMatter, "icon.ico", 16, 16);

      if (hicon != NULL)
      {

         SendMessage(get_handle(), (UINT)WM_SETICON, ICON_BIG, (LPARAM)hicon);

      }

      if (m_bWindowFrame)
      {

         hicon = load_icon(get_app(), straMatter, "icon.ico", 24, 24);

         if (hicon != NULL)
         {

            ::visual::icon * picon = canew(::visual::icon(get_app(), hicon));

            if (picon != NULL && m_workset.m_pappearance != NULL)
            {

               m_workset.m_pappearance->m_picon = picon;

            }

         }

      }
   }

#elif defined(LINUX)

   if (GetParent() == NULL)
   {

      ::file::path strMatter = get_window_default_matter();

      stringa straMatter;

      if (strMatter.name(0).is_equal("system"))
      {

         straMatter.add("main");

         straMatter.add(strMatter);

      }
      else
      {

         straMatter.add(strMatter);

         straMatter.add("main");

      }

      wm_set_icon(get_handle(), straMatter);

      ::visual::dib_sp d(allocer());

      if(load_icon(d, get_app(), straMatter, "icon.png"))
      {

         wm_set_icon(get_handle(), d);

      }

   }

#endif

}


void simple_frame_window::_001OnShowWindow(signal_details * pobj)
{

   SCAST_PTR(::message::show_window, pshow, pobj);

   if(!pshow->m_bShow)
   {

      output_debug_string("test07");

   }

   defer_set_icon();

   if(m_bDefaultNotifyIcon)
   {

      if(m_pnotifyicon != NULL)
      {
         m_pnotifyicon->step();

      }
      OnUpdateToolWindow(pshow->m_bShow);

   }

}


void simple_frame_window::_001OnDisplayChange(signal_details * pobj)
{

   SCAST_PTR(::message::base, pbase, pobj);

   post_simple_command(simple_command_load_window_rect, (LPARAM)FALSE);

   pobj->m_bRet = true;

   pbase->set_lresult(0);

   Default();

}



void simple_frame_window::_001OnTaskbarCreated(signal_details * pobj)
{

   defer_create_notification_icon();

}


void simple_frame_window::_001OnSize(signal_details * pobj)
{

   UNREFERENCED_PARAMETER(pobj);

   //if (does_display_match())
   //{

   //   WindowDataSaveWindowRect();

   //}

   //m_phelpertask->m_bSizeMove = true;

   //m_phelpertask->defer_save_window_rect();



//   m_phelpertask->m_ev.set_event();

   //WindowDataSaveWindowRect();

}


bool simple_frame_window::on_create_client(::user::create_struct * lpcs, ::create * pcreate)
{

   return ::user::frame_window::on_create_client(lpcs, pcreate);

}


bool simple_frame_window::pre_create_window(::user::create_struct& cs)
{

   if (!::user::frame_window::pre_create_window(cs))
      return FALSE;


   //cs.style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME;
   cs.style = WS_POPUP;
   cs.style &= ~WS_VISIBLE;
   cs.style |= WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

   return TRUE;
}

void simple_frame_window::on_layout()
{

   //{

   //   DWORD dwTime2 = ::get_tick_count();

   //   //TRACE("message_handler call time0= %d ms",dwTime2 - t_time1.operator DWORD_PTR());
   //   TRACE("smpfrmwnd::lyot call timeF= %d ms",dwTime2 - t_time1.operator DWORD_PTR());

   //}

   if (m_bWindowFrame && m_workset.IsAppearanceEnabled())
   {
      m_workset.on_layout();
   }
   else
   {
      ::user::frame_window::on_layout();
   }
   //{

   //   DWORD dwTime2 = ::get_tick_count();

   //   //TRACE("message_handler call time0= %d ms",dwTime2 - t_time1.operator DWORD_PTR());
   //   TRACE("smpfrmwnd::lyot call timeG= %d ms",dwTime2 - t_time1.operator DWORD_PTR());

   //}


}

void simple_frame_window::ViewOnActivateFrame(sp(::user::impact) pview, UINT user, sp(::user::interaction) pframe)
{
   UNREFERENCED_PARAMETER(pview);
   UNREFERENCED_PARAMETER(user);
   UNREFERENCED_PARAMETER(pframe);
   //   if(pview != NULL)
   //      pview->OnActivateFrame(WA_INACTIVE, (sp(::user::simple_frame_window)) pframe);
}

void simple_frame_window::_001OnGetMinMaxInfo(signal_details * pobj)
{
#ifdef WINDOWSEX
   SCAST_PTR(::message::base, pbase, pobj);
   MINMAXINFO FAR * lpMMI = (MINMAXINFO FAR*) pbase->m_lparam.m_lparam;
   if (IsFullScreen())
   {
      lpMMI->ptMaxSize.y = m_FullScreenWindowRect.height();
      lpMMI->ptMaxTrackSize.y = lpMMI->ptMaxSize.y;
      lpMMI->ptMaxSize.x = m_FullScreenWindowRect.width();
      lpMMI->ptMaxTrackSize.x = lpMMI->ptMaxSize.x;
   }
#else
   throw todo(get_app());
#endif
}


void simple_frame_window::ShowControlBars(bool bShow, bool bLeaveFullScreenBarsOnHide)
{

   UINT nShow;

   if (bShow)
   {

      nShow = SW_SHOW;

   }
   else
   {

      nShow = SW_HIDE;

   }

   POSITION pos = m_toolbarmap.get_start_position();

   id idKey;

   ::user::toolbar * pbar;

   while (pos != NULL)
   {

      m_toolbarmap.get_next_assoc(pos, idKey, pbar);

      try
      {

         if(pbar != NULL && (bShow || (!pbar->m_bFullScreenBar || !bLeaveFullScreenBarsOnHide)))
         {

            pbar->ShowWindow(nShow);

         }

      }
      catch (...)
      {

      }

   }

   on_layout();

}


void simple_frame_window::WfiOnFullScreen()
{

   ShowControlBars(false, true);

   _001WindowFullScreen();

}

bool simple_frame_window::WfiToggleTransparentFrame()
{

   if (m_workset.m_ebuttonaHide.contains(::user::wndfrm::frame::button_transparent_frame))
   {

      m_bTransparentFrame = false;

      return false;

   }

   m_bTransparentFrame = !m_bTransparentFrame;

   data_set("transparent_frame",m_bTransparentFrame);

   //on_layout();

   //_001UpdateWindow();

   set_need_layout();

   return true;

}


bool simple_frame_window::frame_is_transparent()
{

   return m_bTransparentFrame;

}


void simple_frame_window::WfiOnExitFullScreen()
{

   ShowControlBars(true);

   WorkSetClientInterface::WfiOnExitFullScreen();

}


void simple_frame_window::_001OnViewFullScreen(signal_details * pobj)
{

   UNREFERENCED_PARAMETER(pobj);

   ToggleFullScreen();

}


void simple_frame_window::_001OnMouseMove(signal_details * pobj)
{
   UNREFERENCED_PARAMETER(pobj);
   //   SCAST_PTR(::message::mouse, pmouse, pobj);
}

void simple_frame_window::_001OnUpdateViewFullScreen(signal_details * pobj)
{
   SCAST_PTR(::aura::cmd_ui, pcmdui, pobj);
   pcmdui->m_pcmdui->Enable();
   pcmdui->m_pcmdui->_001SetCheck(WfiIsFullScreen());
   pcmdui->m_bRet = true;
}


void simple_frame_window::ToggleFullScreen()
{

   if (WfiIsFullScreen())
   {

      WfiRestore(false);

   }
   else
   {

      WfiFullScreen();

   }

}


bool simple_frame_window::_001CanEnterScreenSaver()
{
   return true;
}

void simple_frame_window::_001OnSysCommand(signal_details * pobj)
{
   SCAST_PTR(::message::base, pbase, pobj);

   if (pbase->m_wparam == SC_SCREENSAVE)
   {
      if (!_001CanEnterScreenSaver())
      {
         pbase->m_bRet = true;
         pbase->set_lresult(0);
         return;
      }
   }

   if (m_bWindowFrame)
   {
      if (pbase->m_wparam == SC_MAXIMIZE)
      {
         WfiMaximize();
         pbase->m_bRet = true;
         pbase->set_lresult(0);
      }
      else if (pbase->m_wparam == SC_RESTORE)
      {
         if (WfiIsFullScreen())
         {
            WfiFullScreen();
         }
         else
         {
            WfiRestore(false);
         }
         pbase->m_bRet = true;
         pbase->set_lresult(0);
      }
      else if (pbase->m_wparam == SC_MINIMIZE)
      {
         WfiMinimize();
         pbase->m_bRet = true;
         pbase->set_lresult(0);
      }
   }

}


void simple_frame_window::_001OnToggleCustomFrame(signal_details * pobj)
{
   UNREFERENCED_PARAMETER(pobj);
   SetCustomFrame(!GetCustomFrame());
}

void simple_frame_window::_001OnUpdateToggleCustomFrame(signal_details * pobj)
{
   SCAST_PTR(::aura::cmd_ui, pcmdui, pobj);
   pcmdui->m_pcmdui->Enable();
   pcmdui->m_pcmdui->_001SetCheck(m_bWindowFrame);
}


void simple_frame_window::_001OnToggleTransparentFrame(signal_details * pobj)
{

   UNREFERENCED_PARAMETER(pobj);

   WfiToggleTransparentFrame();

}


void simple_frame_window::_001OnUpdateToggleTransparentFrame(signal_details * pobj)
{

   SCAST_PTR(::aura::cmd_ui, pcmdui, pobj);

   pcmdui->m_pcmdui->Enable();

   //if (GetTopLevelFrame()->frame_is_transparent())
   //{

   //   MessageBox(NULL, "ft", "", MB_OK);

   //}
   //else
   //{

   //   MessageBox(NULL, "fnt", "", MB_OK);

   //}

   pcmdui->m_pcmdui->_001SetCheck(frame_is_transparent());

}


void simple_frame_window::ActivateFrame(int32_t nCmdShow)
{

   ::user::frame_window::ActivateFrame(nCmdShow);
}

void simple_frame_window::GetBorderRect(LPRECT lprect)
{
   *lprect = m_rectBorder;
}

void simple_frame_window::SetBorderRect(const RECT & rect)
{
   m_rectBorder = rect;
}

void simple_frame_window::SetCustomFrame(bool bCustom)
{
   m_bWindowFrame = bCustom;
   m_workset.Enable(bCustom);
   on_layout();
   RedrawWindow();

}

bool simple_frame_window::GetCustomFrame()
{
   return m_bWindowFrame;
}


void simple_frame_window::_001OnAppExit(signal_details * pobj)
{

   if (get_parent() != NULL)
   {

      pobj->m_bRet = false;

      return;

   }

   if (!m_bDefaultNotifyIcon)
   {

      _001OnClose(pobj);

      return;

   }

   if(pobj != NULL)
   {

      pobj->m_bRet = true;

   }

   System.request_exit();


}


void simple_frame_window::_001OnClose(signal_details * pobj)
{

   if ((bool)m_varFrame["hide_on_close"])
   {

      ShowWindow(SW_HIDE);

      if (pobj != NULL)
      {

         pobj->m_bRet = true;

      }

      return;

   }

   if(m_bDefaultNotifyIcon)
   {

      if(pobj != NULL)
      {

         SCAST_PTR(::message::base, pbase, pobj);

         pbase->m_bRet = true;

      }

      if(IsWindowVisible())
      {

         ShowWindow(SW_HIDE);

      }
      else
      {

         ShowWindow(SW_SHOW);

         InitialFramePosition(true);

      }

      return;

   }



   if (m_iModalCount > 0)
   {
      EndModalLoop(IDOK);
      pobj->m_bRet = true;
      return;
   }
   else if (GetTopLevelFrame() != NULL
            && (GetTopLevelFrame()->m_iModalCount > 0))
   {
      GetTopLevelFrame()->EndModalLoop(IDOK);
      pobj->m_bRet = true;
      return;
   }
   else if (m_iModalCount > 0)
   {
      m_pimpl->EndModalLoop(IDOK);
      pobj->m_bRet = true;
      ShowWindow(SW_HIDE);
      return;
   }
   else if (GetTopLevelFrame() != NULL && GetTopLevelFrame()->m_iModalCount > 0)
   {
      GetTopLevelFrame()->EndModalLoop(IDOK);
      pobj->m_bRet = true;
      return;
   }

   if (pobj)
   {
      pobj->m_bRet = true;
   }
   // Note: only queries the active document
   sp(::user::document) pdocument = GetActiveDocument();
   if (pdocument != NULL && !pdocument->can_close_frame(this))
   {
      // document can't close right now -- don't close it
      return;
   }

   ::aura::application * papp = &Application;

   if (papp->m_pcoreapp->is_system() || papp->m_pcoreapp->is_session())
   {

      // TODO: instead of closing all applications in process System.m_apptra, should close application that make part of
      // cube, bergedge, session or system.

      ::aura::application_ptra appptra = System.get_appptra();

      for (int32_t i = 0; i <  appptra.get_count(); i++)
      {

         sp(::aura::application) pappChild = appptra[i];

         if (!pappChild->m_pcoreapp->_001CloseApplicationByUser(this))
            return;

      }

   }
   else if(Application.GetVisibleTopLevelFrameCountExcept(this) <= 0)
   {

      if (!papp->m_pcoreapp->_001CloseApplicationByUser(this))
         return;

   }
   else
   {
      DestroyWindow();
   }

}

#ifdef WINDOWSEX

void simple_frame_window::OnNcCalcSize(bool bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp)
{
   UNREFERENCED_PARAMETER(bCalcValidRects);
   if(m_workset.IsEnabled() && m_workset.m_pframeschema != NULL)
   {
      m_workset.OnNcCalcSize(&lpncsp->rgrc[0]);
   }
   else
   {
      // trans      ::user::frame_window::OnNcCalcSize(bCalcValidRects, lpncsp);
   }

}

#endif

void simple_frame_window::_001OnActivateApp(signal_details * pobj)
{
   
   SCAST_PTR(::message::base, pbase, pobj);

   pbase->previous();
   bool bActive = pbase->m_wparam != FALSE;
   if (GetParent() == NULL && GetExStyle() & WS_EX_LAYERED)
   {

      //if (bActive)
      //{

      //   if (WfiIsIconic())
      //   {

      //      WfiRestore();

      //   }
      //   ::SetActiveWindow(get_handle());
      //}

      pbase->m_bRet = true;

      pbase->m_lresult = 0;

   }
}

void simple_frame_window::_001OnActivate(signal_details * pobj)
{

   SCAST_PTR(::message::activate, pactivate, pobj);

   pactivate->previous();

   bool bMinimized = HIWORD(pactivate->m_lparam);

   int iActive = LOWORD(pactivate->m_wparam);
   if (iActive)
   {

      if (GetExStyle() & WS_EX_LAYERED)
      {

         if (iActive == WA_CLICKACTIVE)
         {

         //   if (bMinimized || WfiIsIconic())
         //   {

         //      WfiRestore();

         //   }
         //   else
         //   {

         //      WfiMinimize();

         //   }

         //}
         //else if (bMinimized)
         //{



         //   WfiRestore();


         }

         pactivate->m_bRet = true;

         pactivate->m_lresult = 0;

      }

   }
   else
   {

      //if (GetExStyle() & WS_EX_LAYERED)
      //{

      //   if (pactivate->m_lparam == 0)
      //   {

      //      if (!WfiIsIconic())
      //      {

      //         WfiMinimize(true);

      //      }

      //   }

         pactivate->m_bRet = true;

         pactivate->m_lresult = 0;


      //

//         if (!bMinimized && !WfiIsIconic())
//         { 
//         
////            WfiMinimize();
//
//         }


      //}

   }

}

void simple_frame_window::_001OnNcActivate(signal_details * pobj)
{
   SCAST_PTR(::message::nc_activate, pncactivate, pobj);
   // stay active if WF_STAYACTIVE bit is on
   if (m_nFlags & WF_STAYACTIVE)
      pncactivate->m_bActive = TRUE;

   // but do not stay active if the window is disabled
   if (!is_window_enabled())
      pncactivate->m_bActive = FALSE;

   /*if(m_bWindowFrame)
   {
   m_workset.SetActiveFlag(pncactivate->m_bActive);
   pncactivate->set_lresult(TRUE); // bStop
   pncactivate->m_bRet = true;
   return;
   }
   else*/
   {
      // do not call the base class because it will call Default()
      //  and we may have changed bActive.
      pncactivate->set_lresult(DefWindowProc(WM_NCACTIVATE, pncactivate->m_bActive, 0)); // bStop
      pncactivate->m_bRet = pncactivate->get_lresult() != 0;
      return;
   }
}



bool simple_frame_window::LoadFrame(const char * pszMatter, uint32_t dwDefaultStyle, ::user::interaction * pParentWnd, ::create * pcreate)
{

   UNREFERENCED_PARAMETER(pParentWnd);

   // only do this once
   //   ASSERT_VALID_IDR(nIDResource);
   //   ASSERT(m_nIDHelp == 0 || m_nIDHelp == nIDResource);

   m_strMatterHelp = pszMatter;    // ID for help context (+HID_BASE_RESOURCE)

   //   string strFullString;
   //   if (strFullString.load_string(nIDResource))
   //      __extract_sub_string(m_strTitle, strFullString, 0);    // first sub-string

   const char * lpszTitle = m_strTitle;

   if (pParentWnd == NULL)
      pParentWnd = Application.get_request_parent_ui(this, pcreate);

   dwDefaultStyle &= ~WS_VISIBLE;

   ::rect rectFrame;

   sp(::user::place_holder) pholder;

   if(pParentWnd != NULL && (pholder = pParentWnd).is_set())
   {

      pholder->GetClientRect(rectFrame);

   }
   else
   {

      rectFrame = null_rect();

   }

   m_bLayoutEnable = false;

   if (!create_window_ex(0L, NULL, lpszTitle, dwDefaultStyle, rectFrame, pParentWnd, /*nIDResource*/ 0, pcreate))
   {

      return false;   // will self destruct on failure normally

   }


   /* trans   // save the default menu handle
      ASSERT(get_handle() != NULL);
      m_hMenuDefault = ::GetMenu(get_handle()); */

   // load accelerator resource
   //   LoadAccelTable(MAKEINTRESOURCE(nIDResource));

   if (pcreate == NULL)   // send initial update
      SendMessageToDescendants(WM_INITIALUPDATE, 0, (LPARAM)0, TRUE, TRUE);

   return TRUE;

}




void simple_frame_window::_001OnDdeInitiate(signal_details * pobj)
{
   SCAST_PTR(::message::base, pbase, pobj);
   pbase->set_lresult(DefWindowProc((uint32_t)pbase->m_wparam, pbase->m_lparam, pbase->get_lresult()));
}


void simple_frame_window::pre_translate_message(signal_details * pobj)
{

   SCAST_PTR(::message::base, pbase, pobj);

   if(pbase->m_uiMessage == message_display_change)
   {

      Wfi();

   }
   else if (pbase->m_uiMessage == WM_MOUSEMOVE)
   {

   }
   else if(pbase->m_uiMessage == WM_KEYDOWN || pbase->m_uiMessage == WM_SYSKEYDOWN)
   {

      SCAST_PTR(::message::key, pkey, pobj);

      if(pkey->m_ekey == ::user::key_alt || pkey->m_ekey == ::user::key_lalt || pkey->m_ekey == ::user::key_ralt)
      {

         m_bFullScreenAlt = false;

      }
      else if(pkey->m_ekey == ::user::key_control || pkey->m_ekey == ::user::key_lcontrol || pkey->m_ekey == ::user::key_rcontrol)
      {

         m_bFullScreenCtrl = false;

      }
      else if(pkey->m_ekey == ::user::key_return)
      {

         if (Session.is_key_pressed(::user::key_control) && Session.is_key_pressed(::user::key_alt))
         {

            m_bFullScreenAlt = true;

            m_bFullScreenCtrl = true;

            if (!IsFullScreen())
            {

               if (DeferFullScreen(true, false))
               {

                  pbase->m_bRet = true;

                  return;

               }

            }

         }

      }

   }
   else if(pbase->m_uiMessage == WM_KEYUP || pbase->m_uiMessage == WM_SYSKEYUP)
   {

      SCAST_PTR(::message::key, pkey, pobj);

      if(pkey->m_ekey == ::user::key_alt || pkey->m_ekey == ::user::key_lalt || pkey->m_ekey == ::user::key_ralt)
      {

         if (IsFullScreen() && Session.is_key_pressed(::user::key_control) && !m_bFullScreenAlt && !m_bFullScreenCtrl)
         {

            if (WfiRestore(m_eappearanceBefore != ::user::AppearanceFullScreen))
            {

               pbase->m_bRet = true;

               m_bFullScreenAlt = false;

               return;

            }

         }

         m_bFullScreenAlt = false;

      }
      else if(pkey->m_ekey == ::user::key_control || pkey->m_ekey == ::user::key_lcontrol || pkey->m_ekey == ::user::key_rcontrol)
      {

         if(IsFullScreen() && Session.is_key_pressed(::user::key_alt) && !m_bFullScreenAlt && !m_bFullScreenCtrl)
         {

            if(WfiRestore(m_eappearanceBefore != ::user::AppearanceFullScreen))
            {

               pbase->m_bRet = true;

               m_bFullScreenAlt = false;

               return;

            }

         }

         m_bFullScreenAlt = false;

      }

   }

   return ::user::frame_window::pre_translate_message(pobj);

}


void simple_frame_window::InitialFramePosition(bool bForceRestore)
{

   if(m_bFrameMoveEnable)
   {

      if(Application.command()->m_varTopicQuery.has_property("wfi_maximize")
            && (GetParent() == NULL
#if defined(ANDROID) || defined(METROWIN) || defined(APPLE_IOS)
                || GetParent() == System.m_possystemwindow->m_pui
#endif
               ))
      {

         WfiMaximize();

      }
      else if(Application.command()->m_varTopicQuery.has_property("client_only")
              || Application.command()->m_varTopicQuery.has_property("full_screen"))
      {

         if(m_workset.IsAppearanceEnabled())
         {

            WfiFullScreen();

         }
         else
         {

            best_monitor(NULL,::null_rect(),true);

         }

      }
      else
      {


         WindowDataLoadWindowRect(bForceRestore,true);

         WindowDataEnableSaveWindowRect(true);

      }

   }

   ActivateTopParent();

   BringToTop(-1);

   if (m_workset.IsAppearanceEnabled())
   {

      if (m_workset.get_appearance() != NULL && m_workset.GetAppearance() == ::user::AppearanceIconic)
      {

         WfiRestore(false);

      }

   }

   m_bLayoutEnable = true;

   //on_layout();

   if (m_palphasource != NULL)
   {

      m_palphasource->on_alpha_target_initial_frame_position();

   }

   m_bInitialFramePosition = false;


}


void simple_frame_window::_001OnDeferPaintLayeredWindowBackground(::draw2d::graphics * pgraphics)
{

   if(m_pauraapp == NULL
   || m_pauraapp->m_pcoresession == NULL
   || m_pauraapp->m_pcoresession->m_psavings == NULL)
   {

      return;

   }


   if (Session.savings().is_trying_to_save(::aura::resource_processing)
         || Session.savings().is_trying_to_save(::aura::resource_translucent_background))
   {

      rect rectClient;

      GetClientRect(rectClient);

      pgraphics->FillSolidRect(rectClient, RGB(0, 0, 0));

   }
   else
   {

      ::user::frame_window::_001OnDeferPaintLayeredWindowBackground(pgraphics);

   }

}


void simple_frame_window::_000OnDraw(::draw2d::graphics * pgraphicsParam)
{
   defer_check_layout();

   defer_check_zorder();

   if (!IsWindowVisible() || WfiIsIconic())
      return;

   rect rectClient;

   GetWindowRect(rectClient);

   rectClient -= rectClient.top_left();

   bool bDib = false;

   double dAlpha = get_alpha();

   ::draw2d::graphics * pgraphics = pgraphicsParam;

   if(dAlpha == 0.0)
   {

      output_debug_string("Alpha is Zero\n");

   }

   if(rectClient.area() > 0 && dAlpha > 0.0 && dAlpha < 1.0 && GetExStyle() & WS_EX_LAYERED)
   {

      if(m_dibAlpha.is_null())
      {

         m_dibAlpha.alloc(allocer());

      }

      m_dibAlpha->create(rectClient.size());

      m_dibAlpha->Fill(0,0,0,0);

      pgraphics = m_dibAlpha->get_graphics();

      pgraphics->SetViewportOrg(pgraphics->GetViewportOrg());

      bDib = true;

   }

   if(dAlpha > 0.0)
   {

      if(m_puserschemaSchema != NULL && m_puserschemaSchema->_001OnDrawMainFrameBackground(pgraphics,this))
      {

         _001DrawThis(pgraphics);

         _001DrawChildren(pgraphics);

         _008CallOnDraw(pgraphics);

      }
      else if (m_bblur_Background)
      {

         _001DrawThis(pgraphics);

         _001DrawChildren(pgraphics);

         _008CallOnDraw(pgraphics);

      }
      else if(!Session.savings().is_trying_to_save(::aura::resource_processing)
              && !Session.savings().is_trying_to_save(::aura::resource_display_bandwidth)
              && !Session.savings().is_trying_to_save(::aura::resource_memory))
         //&& (GetParent() != NULL || (this->GetExStyle() & WS_EX_LAYERED) != 0))
      {

#if TEST

         pgraphics->FillSolidRect(0, 0, 100, 100, ARGB(128, 255, 0, 0));

#endif

         _010OnDraw(pgraphics);

#if TEST

         pgraphics->FillSolidRect(0, 100, 100, 100, ARGB(128, 0, 0, 255));

#endif

      }
      else
      {

#if TEST

         pgraphics->FillSolidRect(60, 10, 50, 50, ARGB(128, 184, 177, 84));

#endif

         _001DrawThis(pgraphics);

         _001DrawChildren(pgraphics);

         _008CallOnDraw(pgraphics);

#if TEST

         pgraphics->FillSolidRect(10, 60, 50, 50, ARGB(128, 184, 177, 84));

#endif

      }

   }

   if(bDib)
   {

      pgraphicsParam->set_alpha_mode(::draw2d::alpha_mode_blend);

      pgraphicsParam->alpha_blend(null_point(),rectClient.size(),pgraphics,null_point(),dAlpha);

   }

}


void simple_frame_window::_001OnDraw(::draw2d::graphics * pgraphics)
{

   if(m_bblur_Background)
   {

      class imaging & imaging = System.visual().imaging();

      rect rectClient;

      GetClientRect(rectClient);

      //rectClient.offset(rectClient.top_left());

      if(Session.savings().is_trying_to_save(::aura::resource_translucent_background))
      {

         //pgraphics->FillSolidRect(rectClient, RGB(150, 220, 140));

      }
      else if(Session.savings().is_trying_to_save(::aura::resource_processing)
              || Session.savings().is_trying_to_save(::aura::resource_blur_background))
      {

         imaging.color_blend(pgraphics,rectClient,RGB(150,180,140),150);

      }
      else
      {
#ifndef LINUX
         if(rectClient.size() != m_dibBk->size())
         {
            m_dibBk->create(rectClient.size());
            m_dibBk->Fill(0,184,184,170);
            //HMODULE hmodule = ::LoadLibrary("ca2performance.dll");
            //::visual::fastblur *( *pfnNew )(sp(::aura::application)) = (::visual::fastblur *(*)(sp(::aura::application))) ::GetProcAddress(hmodule, "new_fastblur");
            m_fastblur.alloc(allocer());
            m_fastblur.initialize(rectClient.size(),2);
         }
         if(m_fastblur.is_set() && m_fastblur->area() > 0)
         {

            m_fastblur->get_graphics()->BitBlt(0,0,rectClient.width(),rectClient.height(),pgraphics,0,0,SRCCOPY);
            m_fastblur.blur();
            imaging.bitmap_blend(
               m_fastblur->get_graphics(),
               null_point(),
               rectClient.size(),
               m_dibBk->get_graphics(),
               null_point(),
               49);
            pgraphics->from(rectClient.size(),
                                       m_fastblur->get_graphics(),
                                       null_point(),
                                       SRCCOPY);
         }
#endif
      }
   }

   _011OnDraw(pgraphics);


}


bool simple_frame_window::on_before_set_parent(sp(::user::interaction) pinterface)
{

   WindowDataSaveWindowRect();

   if (!::user::box::on_before_set_parent(pinterface))
      return false;

   return true;

}


void simple_frame_window::on_set_parent(::user::interaction * puiParent)
{



   ::user::frame_window::on_set_parent(puiParent);

   if (puiParent == NULL)
   {

      if (m_bLayered)
      {

         ModifyStyleEx(0, WS_EX_LAYERED);

      }

   }
   m_workset.m_pwndEvent = this;

   if (m_pupdowntarget != NULL && wfi_is_up_down())
   {

      // an updowntarget always show the frame for upping/downing

      m_bWindowFrame = true;

      m_workset.Enable(true);

      if (puiParent == NULL)
      {

         m_workset.m_bSizingEnabled = true;

         m_workset.m_bMovingEnabled = true;

      }
      else
      {

         m_workset.m_bSizingEnabled = false;

         m_workset.m_bMovingEnabled = false;

      }

      m_workset.install_message_handling(m_pimpl);

      m_workset.m_pframeschema->get_control_box()->set_need_layout();

   }
   else
   {

      if (m_bAutoWindowFrame)
      {

         if (!m_bWindowFrame)
         {

            m_bWindowFrame = m_bCustomFrameBefore;

         }

         if (!m_workset.IsEnabled())
         {

            m_workset.Enable(true);

         }

      }
      else
      {

         if (puiParent != NULL)
         {

            m_bCustomFrameBefore = m_bWindowFrame;

            m_bWindowFrame = false;

            m_workset.Enable(false);

         }
         else
         {

            m_bWindowFrame = m_bCustomFrameBefore;

            m_workset.Enable(m_bWindowFrame);

         }
      }

   }

   if (puiParent == NULL || !puiParent->is_place_holder())
   {

      WindowDataLoadWindowRect(false);

      WindowDataEnableSaveWindowRect(true);

   }

   set_need_layout();

   RedrawWindow();

}

bool simple_frame_window::GetClientRect(LPRECT lprect)
{

   if (m_bWindowFrame && m_pframeschema != NULL && !WfiIsFullScreen() && !frame_is_transparent())
   {

      return m_pframeschema->get_window_client_rect(lprect);

   }
   else
   {

      return ::user::frame_window::GetClientRect(lprect);

   }


}


bool simple_frame_window::is_application_main_window()
{

   return Application.m_puiMain == this;

}


bool simple_frame_window::LoadToolBar(sp(::type) sptype, id idToolBar, const char * pszToolBar, uint32_t dwCtrlStyle, uint32_t dwStyle)
{

   sp(::user::toolbar) ptoolbar = m_toolbarmap[idToolBar];

   if(ptoolbar.is_null())
   {

      ptoolbar = Application.alloc(sptype);

      if(ptoolbar == NULL)
         return false;

      ptoolbar->create_window_ex(this,dwCtrlStyle,dwStyle);

   }

   if(ptoolbar == NULL)
      return false;

   string strMatter = Application.dir().matter(pszToolBar);

   if (ptoolbar->oprop("matter_annotation") == strMatter)
   {

      return true;

   }

   string strXml = Application.file().as_string(strMatter);

   if(!ptoolbar->LoadXmlToolBar(strXml))
   {
      
      return false;

   }

   m_toolbarmap.set_at(idToolBar,ptoolbar);

   AddControlBar(ptoolbar);

   ptoolbar->set_need_layout();

   ptoolbar->oprop("matter_annotation") = strMatter;

   set_need_layout();

   return true;

}


void simple_frame_window::defer_create_notification_icon()
{

   if (!m_bDefaultNotifyIcon)
   {

      return;

   }

   if (m_pnotifyicon.is_set())
   {

      m_pnotifyicon->Destroy();

   }

   if (m_piconNotify.is_null())
   {

      m_piconNotify = canew(::visual::icon(get_app()));

      const char * pszAppName = Application.m_strAppName;

      m_piconNotify->load_app_tray_icon(pszAppName);

   }

   m_pnotifyicon = canew(::user::notify_icon(get_app()));

   m_pnotifyicon->create(1, this, m_piconNotify);

   //if (m_workset.m_pframeschema != NULL)
   {

      //m_workset.m_pframeschema->m_spcontrolbox->hide_button(::user::wndfrm::frame::button_minimize);
      m_workset.m_bMinimizeToTray = true;

   }

}


void simple_frame_window::_001OnUser184(signal_details * pobj)
{

   SCAST_PTR(::message::base, pbase, pobj);

   if (pbase->m_wparam == 0 && pbase->m_lparam == 0)
   {

      InitialFramePosition(true);

      pbase->m_bRet = true;

   }
   else if(pbase->m_wparam == 2)
   {

      throw simple_exception(get_app());

   }
   else if(pbase->m_wparam == 123)
   {
      if(does_display_match())
      {

         WindowDataSaveWindowRect();

      }


   }
}

// persistent frame implemenation using updowntarget
bool simple_frame_window::WndFrameworkDownUpGetUpEnable()
{

   return m_pupdowntarget != NULL && wfi_is_up_down() && wfi_is_down();

}

bool simple_frame_window::WndFrameworkDownUpGetDownEnable()
{

   return m_pupdowntarget != NULL && wfi_is_up_down() && wfi_is_up();

}


void simple_frame_window::WfiOnDown()
{

   frame_Attach();

}


void simple_frame_window::WfiOnUp()
{

   frame_Detach();

}

bool simple_frame_window::WfiIsMoving()
{

   return m_bWindowFrame && m_workset.GetMovingManager() != NULL && m_workset.GetMovingManager()->IsMoving();

}


bool simple_frame_window::WfiIsSizing()
{

   return m_bWindowFrame && m_workset.GetSizingManager() != NULL && m_workset.GetSizingManager()->IsSizing();

}



bool simple_frame_window::create_window(const char * lpszClassName,const char * lpszWindowName,uint32_t dwStyle,const RECT & rect,sp(::user::interaction) pParentWnd,const char * lpszMenuName,uint32_t dwExStyle, ::create * pcreate)
{

   return ::user::frame_window::create_window(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, lpszMenuName, dwExStyle, pcreate);

}





bool simple_frame_window::_001OnCmdMsg(::aura::cmd_msg * pcmdmsg)
{

   if (m_workset._001OnCmdMsg(pcmdmsg))
      return true;

   if (pcmdmsg->m_pcommandtargetSource == this)
   {

      // then pump through frame
      if (::user::frame_window::_001OnCmdMsg(pcmdmsg))
         return true;

   }

   // pump through current ::user::impact FIRST
   sp(::user::impact) pview = GetActiveView();
   if (pview != NULL && pview->_001OnCmdMsg(pcmdmsg))
      return TRUE;

   pview = get_child_by_id("pane_first");
   if (pview != NULL && pview->_001OnCmdMsg(pcmdmsg))
      return TRUE;

   if (pcmdmsg->m_pcommandtargetSource != this)
   {

      // then pump through frame
      if (::user::frame_window::_001OnCmdMsg(pcmdmsg))
         return true;

   }

   // then pump through parent
   sp(::user::interaction) puiParent = GetParent();
   if (puiParent != NULL && puiParent->_001OnCmdMsg(pcmdmsg))
      return TRUE;

   // last but not least, pump through cast
   ::core::application* pApp = get_app()->m_pcoreapp;
   if (pApp != NULL && pApp->_001OnCmdMsg(pcmdmsg))
      return TRUE;

   sp(command_target_interface) pcommandtargetinterface = Session.get_keyboard_focus();

   if (pcommandtargetinterface != NULL)
   {
      if (pcommandtargetinterface->_001OnCmdMsg(pcmdmsg))
         return TRUE;
   }

   return FALSE;
}


#ifdef WINDOWSEX



void simple_frame_window::OnDropFiles(HDROP hDropInfo)
{

   SetActiveWindow();      // activate us first !
   UINT nFiles = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);

   ::file::patha patha;
   wchar_t szFileName[_MAX_PATH];
   for (UINT iFile = 0; iFile < nFiles; iFile++)
   {
      
      if (::DragQueryFileW(hDropInfo, iFile, szFileName, _MAX_PATH))
      {

         patha.add(szFileName);

      }

   }
   ::DragFinish(hDropInfo);

   Session.on_frame_window_drop_files(this, patha);

}

// query end session for main frame will attempt to close it all down
bool simple_frame_window::OnQueryEndSession()
{
   ::base::application* pApp = &System;
   if(pApp != NULL && pApp->m_puiMain == this)
      return pApp->m_pcoreapp->save_all_modified();

   return TRUE;
}


void simple_frame_window::OnEndSession(bool bEnding)
{

   if(!bEnding)
      return;

   Application.close(::core::end_system);

}


#else

//void simple_frame_window::OnDropFiles(HDROP hDropInfo)
//{
//
//   UNREFERENCED_PARAMETER(hDropInfo);
//
//}
//


#endif






LRESULT simple_frame_window::OnDDEInitiate(WPARAM wParam, LPARAM lParam)
{


#ifdef WINDOWSEX

   ::core::application* pApp = &System;
   if (pApp != NULL &&
         LOWORD(lParam) != 0 && HIWORD(lParam) != 0 &&
         (ATOM)LOWORD(lParam) == pApp->m_atomApp &&
         (ATOM)HIWORD(lParam) == pApp->m_atomSystemTopic)
   {
      // make duplicates of the incoming atoms (really adding a reference)
      char szAtomName[_MAX_PATH];
      VERIFY(GlobalGetAtomName(pApp->m_atomApp,
                               szAtomName, _MAX_PATH - 1) != 0);
      VERIFY(GlobalAddAtom(szAtomName) == pApp->m_atomApp);
      VERIFY(GlobalGetAtomName(pApp->m_atomSystemTopic,
                               szAtomName, _MAX_PATH - 1) != 0);
      VERIFY(GlobalAddAtom(szAtomName) == pApp->m_atomSystemTopic);

      // send the WM_DDE_ACK (caller will delete duplicate atoms)
      ::SendMessage((oswindow)wParam, WM_DDE_ACK, (WPARAM)get_handle(),
                    MAKELPARAM(pApp->m_atomApp, pApp->m_atomSystemTopic));
   }

#else

   ::exception::throw_not_implemented(get_app());

#endif

   return 0L;
}


// always ACK the execute command - even if we do nothing
LRESULT simple_frame_window::OnDDEExecute(WPARAM wParam, LPARAM lParam)
{


#ifdef WINDOWSEX

   // unpack the DDE message
   uint_ptr unused;
   HGLOBAL hData;
   //IA64: Assume DDE LPARAMs are still 32-bit
   VERIFY(UnpackDDElParam(WM_DDE_EXECUTE, lParam, &unused, (uint_ptr*)&hData));

   // get the command string
   const char * lpsz = (const char *)GlobalLock(hData);
   string strCommand;
   try
   {
      strCommand = lpsz;
      GlobalUnlock(hData);
   }
   catch (memory_exception * pe)
   {
      GlobalUnlock(hData);
      pe->Delete();
   }


   // acknowledge now - before attempting to execute
   ::PostMessage((oswindow)wParam, WM_DDE_ACK, (WPARAM)get_handle(),
                 //IA64: Assume DDE LPARAMs are still 32-bit
                 ReuseDDElParam(lParam, WM_DDE_EXECUTE, WM_DDE_ACK,
                                (UINT)0x8000, (uint_ptr)hData));

   // don't execute the command when the window is disabled
   if (!is_window_enabled())
   {
      TRACE(::aura::trace::category_AppMsg, 0, "Warning: DDE command '%s' ignored because window is disabled.\n",
            strCommand.GetString());
      return 0;
   }

   // execute the command
   LPTSTR lpszCommand = strCommand.GetBuffer();
   if (!System.OnDDECommand(lpszCommand))
      TRACE(::aura::trace::category_AppMsg, 0, "Error: failed to execute DDE command '%s'.\n", lpszCommand);
   strCommand.ReleaseBuffer();

#else

   ::exception::throw_not_implemented(get_app());

#endif

   return 0L;

}

LRESULT simple_frame_window::OnDDETerminate(WPARAM wParam, LPARAM lParam)
{

#ifdef WINDOWSEX

   ::PostMessage((oswindow)wParam, WM_DDE_TERMINATE, (WPARAM)get_handle(), lParam);

#else

   ::exception::throw_not_implemented(get_app());

#endif

   return 0L;

}


void simple_frame_window::NotifyFloatingWindows(uint32_t dwFlags)
{
   ASSERT_VALID(this);
   // trans   ASSERT(get_handle() != NULL);

   // get top level parent frame window first unless this is a child window
   sp(::user::frame_window) pParent = (GetStyle() & WS_CHILD) ? this : GetTopLevelFrame();
   ASSERT(pParent != NULL);
   if (dwFlags & (FS_DEACTIVATE | FS_ACTIVATE))
   {
      // update parent window activation state
      bool bActivate = !(dwFlags & FS_DEACTIVATE);
      bool bEnabled = pParent->is_window_enabled();

      if (bActivate && bEnabled && pParent != this)
      {
         // Excel will try to Activate itself when it receives a
         // WM_NCACTIVATE so we need to keep it from doing that here.
         m_nFlags |= WF_KEEPMINIACTIVE;
         pParent->send_message(WM_NCACTIVATE, TRUE);
         m_nFlags &= ~WF_KEEPMINIACTIVE;
      }
      else
      {
         pParent->send_message(WM_NCACTIVATE, FALSE);
      }
   }

   // then update the state of all floating windows owned by the parent
#ifdef WINDOWSEX

   sp(::user::interaction) oswindowDesktop = System.get_desktop_window();

   if (oswindowDesktop.is_null())
      return;

   sp(::user::interaction) oswindow = oswindowDesktop->get_wnd(GW_CHILD);

   while (oswindow != NULL)
   {

      if (::user::is_descendant(pParent, oswindow))
         oswindow->send_message(WM_FLOATSTATUS, dwFlags);

      oswindow = oswindow->get_wnd(GW_HWNDNEXT);

   }

#else
   throw todo(get_app());
#endif
}



// query end session for main frame will attempt to close it all down
void simple_frame_window::_001OnQueryEndSession(signal_details * pobj)
{
   SCAST_PTR(::message::base, pbase, pobj);
   if (&System != NULL && System.m_puiMain == this)
   {
      pbase->set_lresult(System.save_all_modified());
      pbase->m_bRet = true;
      return;
   }
   pbase->set_lresult(TRUE);
   return;
}


bool simple_frame_window::BaseOnControlEvent(::user::control_event * pevent)
{

   if (m_workset.BaseOnControlEvent(pevent))
   {

      return true;

   }

   return ::user::frame_window::BaseOnControlEvent(pevent);

}


string simple_frame_window::get_window_default_matter()
{

   if (m_pdocumenttemplate == NULL)
   {

      return frame_window::get_window_default_matter();

   }

   return m_pdocumenttemplate->m_strMatter;

}


void simple_frame_window::guserbaseOnInitialUpdate(signal_details * pobj)
{
   SCAST_PTR(::message::base, pbase, pobj);
   ::user::FrameInitialUpdate * pfiu = (::user::FrameInitialUpdate *)pbase->m_lparam.m_lparam;
   if (pfiu != NULL)
   {
      sp(::user::frame_window) pframe = (this);
      // if the frame does not have an active ::user::impact, set to first pane
      sp(::user::impact) pview = NULL;
      if (pframe->GetActiveView() == NULL)
      {
         sp(::user::interaction) pwindow = pframe->GetDescendantWindow("pane_first");
         if (pwindow != NULL && base_class < ::user::impact >::bases(pwindow))
         {
            pview = (pwindow.m_p);
            pframe->SetActiveView(pview, FALSE);
         }
      }

      if (pfiu->m_bMakeVisible)
      {
         // send initial update to all views (and other controls) in the frame
         pframe->SendMessageToDescendants(WM_INITIALUPDATE, 0, (LPARAM)0, TRUE, TRUE);

         // give ::user::impact a chance to save the focus (CFormView needs this)
         if (pview != NULL)
            pview->OnActivateFrame(WA_INACTIVE, pframe);

         // finally, activate the frame
         // (send the default show command unless the main desktop window)
         int32_t nCmdShow = -1;      // default
         ::core::application* pApp = &System;
         if (pApp != NULL && pApp->m_puiMain == pframe)
         {
            nCmdShow = System.m_nCmdShow; // use the parameter from WinMain
            System.m_nCmdShow = -1; // set to default after first time
         }
         bool bFullScreen;
         data_get("FullScreen", bFullScreen);
         if (bFullScreen)
         {
            WfiFullScreen();
         }
         else
         {
            pframe->ActivateFrame(nCmdShow);
         }
         if (pview != NULL)
            pview->OnActivateView(TRUE, pview, pview);

      }

      sp(::user::document) pdoc = pfiu->m_pdoc;
      // update frame counts and frame title (may already have been visible)
      if (pdoc != NULL)
         pdoc->update_frame_counts();
      pframe->on_update_frame_title(TRUE);

      RedrawWindow();
   }
   pbase->set_lresult(0);
}


void simple_frame_window::_001OnClip(::draw2d::graphics * pgraphics)
{

}


void simple_frame_window::_010OnDraw(::draw2d::graphics * pgraphics)
{

   if (!is_this_visible())
      return;


   if(GetExStyle() & WS_EX_LAYERED)
   {

      ::user::interaction_spa uia;

      {

         synch_lock sl(m_pmutex);

         uia = m_uiptraChild;

      }

      {

         ::draw2d::keep k(pgraphics);

         try
         {

            for(auto & pui : uia)
            {

               if (!base_class < ::user::wndfrm::frame::control_box > ::bases(pui))
               {

                  pui->_000OnDraw(pgraphics);

               }

            }

         }
         catch (...)
         {

         }

      }



      _001DrawThis(pgraphics);

      {

         ::draw2d::keep k(pgraphics);

         try
         {

            for (auto & pui : uia)
            {

               if (base_class < ::user::wndfrm::frame::control_box > ::bases(pui))
               {

                  string str;

                  pui->get_window_text(str);

                  if (str == "r")
                  {

                     //TRACE0("x button");

                  }

                  if (pui->IsWindowVisible())
                  {

                     if (str == "r")
                     {

                        //TRACE0("x button visible");

                     }

                     pui->_000OnDraw(pgraphics);

                  }

               }

            }

         }
         catch (...)
         {

         }

      }

      _008CallOnDraw(pgraphics);

   }
   else
   {

      _001DrawThis(pgraphics);

      _001DrawChildren(pgraphics);

      _008CallOnDraw(pgraphics);

   }

}


void simple_frame_window::_011OnDraw(::draw2d::graphics * pgraphics)
{

   if (m_bWindowFrame && !Session.savings().is_trying_to_save(::aura::resource_display_bandwidth))
   {

      ::user::wndfrm::frame::WorkSetClientInterface::_001OnDraw(pgraphics);

   }
   else if(!_001IsTranslucent())
   {

      rect rect;

      GetClientRect(rect);

      pgraphics->FillSolidRect(rect, _001GetColor(::user::color_background));

   }

}


bool simple_frame_window::WfiOnMove(bool bTracking)
{

   if (!bTracking)
   {

      ::fork(get_app(), [this]()
      {

         WindowDataSaveWindowRect();

      });

   }

   return true;

}


bool simple_frame_window::WfiOnSize(bool bTracking)
{

   if (!bTracking)
   {

      WindowDataSaveWindowRect();

   }

   return true;

}


void simple_frame_window::WfiOnClose()
{

   post_message(WM_CLOSE);

}


void simple_frame_window::WfiOnMaximize()
{

   _001WindowMaximize();

}


void simple_frame_window::WfiOnMinimize(bool bNoActivate)
{

   _001WindowMinimize(bNoActivate);

}


void simple_frame_window::WfiOnRestore()
{

   _001WindowRestore();

}


void simple_frame_window::WfiOnDock(::user::EAppearance eappearance)
{

   _001WindowDock(eappearance);

}


bool simple_frame_window::DeferFullScreen(bool bFullScreen, bool bRestore)
{
   if (bFullScreen)
   {
      sp(::user::interaction) pwndParentFrame = GetParentFrame();
      if (pwndParentFrame == NULL)
      {
         if (!WfiIsFullScreen())
         {
            WfiFullScreen();
            return true;
         }
         else
         {
            return false;
         }
      }
      simple_frame_window * pframe = dynamic_cast <simple_frame_window *> (pwndParentFrame.m_p);
      if (pframe == NULL)
      {
         if (!WfiIsFullScreen())
         {
            WfiFullScreen();
            return true;
         }
         else
         {
            return false;
         }
      }

      if (pframe->DeferFullScreen(bFullScreen, bRestore))
         return true;

      if (!WfiIsFullScreen())
      {
         WfiFullScreen();
         return true;
      }
      else
      {
         return false;
      }
   }
   else
   {
      if (WfiIsFullScreen())
      {
         WfiRestore(false);
         return true;
      }
      sp(::user::interaction) pwndParentFrame = GetParentFrame();
      if (pwndParentFrame == NULL)
      {
         return false;
      }
      simple_frame_window * pframe = dynamic_cast <simple_frame_window *> (pwndParentFrame.m_p);
      if (pframe == NULL)
      {
         return false;
      }
      return pframe->DeferFullScreen(bFullScreen, bRestore);
   }
}


bool simple_frame_window::calc_layered()
{

   if (m_bLayered && _001GetTranslucency() != ::user::TranslucencyNone)
   {
      return !Session.savings().is_trying_to_save(::aura::resource_processing)
             && !Session.savings().is_trying_to_save(::aura::resource_display_bandwidth);
   }
   else
   {

      return false;

   }

}


bool simple_frame_window::get_translucency(::user::ETranslucency & etranslucency)
{

   if (m_etranslucency != ::user::TranslucencyUndefined)
   {

      etranslucency = m_etranslucency;

      return true;

   }

   return ::user::frame_window::get_translucency(etranslucency);

}


void simple_frame_window::data_on_after_change(signal_details * pobj)
{
   box::data_on_after_change(pobj);
   SCAST_PTR(database::change_event, phint, pobj);
   if (phint->m_key.m_id == "ca2.savings")
   {
      defer_synch_layered();
   }
}




bool simple_frame_window::on_simple_command(e_simple_command ecommand, lparam lparam, LRESULT & lresult)
{

   return ::user::frame_window::on_simple_command(ecommand, lparam, lresult);

}

class ::mini_dock_frame_window* simple_frame_window::CreateFloatingFrame(uint32_t dwStyle)
{
   UNREFERENCED_PARAMETER(dwStyle);
   return NULL;
}


















bool simple_frame_window::set_appearance(::user::EAppearance eappearance)
{

   bool bOk1 = ::user::frame_window::set_appearance(eappearance);

   m_workset.SetAppearance(eappearance);

   return bOk1;

}


bool simple_frame_window::create_bars()
{

   if(!on_create_bars())
      return false;

   if (!m_bInitialFramePosition)
   {

      set_need_layout();

   }

   return true;

}


bool simple_frame_window::on_create_bars()
{

   ::file::path path = Application.dir().matter(m_pdocumenttemplate->m_strMatter / "toolbar.xml");

   if (Application.file().exists(path))
   {

      LoadToolBar(m_pdocumenttemplate->m_strMatter, m_pdocumenttemplate->m_strMatter / "toolbar.xml");

   }

   return true;

}


void simple_frame_window::InitialUpdateFrame(::user::document * pDoc,bool bMakeVisible)
{

   ::user::frame_window::InitialUpdateFrame(pDoc,bMakeVisible);

}


void simple_frame_window::_001OnTimer(::timer * ptimer)
{

   ::user::frame_window::_001OnTimer(ptimer);
   ::user::wndfrm::frame::WorkSetClientInterface::_001OnTimer(ptimer);

}
void simple_frame_window::OnNotifyIconContextMenu(UINT uiNotifyIcon)
{
   point pt;
   ::GetCursorPos(&pt);
   track_popup_xml_matter_menu("popup_notification.xml", 0, pt);
}

void simple_frame_window::OnNotifyIconLButtonDblClk(UINT uiNotifyIcon)
{
   UNREFERENCED_PARAMETER(uiNotifyIcon);
}

void simple_frame_window::OnNotifyIconLButtonDown(UINT uiNotifyIcon)
{
   InitialFramePosition(true);
   UNREFERENCED_PARAMETER(uiNotifyIcon);
}


void simple_frame_window::OnNotifyIconClose(UINT uiNotifyIcon)
{

   if(m_bDefaultNotifyIcon)
   {

      post_message(WM_CLOSE);

   }

}


void simple_frame_window::OnNotifyIconQuit(UINT uiNotifyIcon)
{

   if(m_bDefaultNotifyIcon)
   {

      post_message(WM_APPEXIT);

   }

}


bool simple_frame_window::__close_is_closed()
{

   return !IsWindowVisible();

}


bool simple_frame_window::notify_icon_frame_is_opened()
{

   return !__close_is_closed();

}


void simple_frame_window::OnInitialFrameUpdate(bool bMakeVisible)
{

   if(!m_bDefaultNotifyIcon)
   {

      ::user::frame_window::OnInitialFrameUpdate(bMakeVisible);

      return;

   }

   OnUpdateToolWindow(bMakeVisible);

   if(bMakeVisible)
   {

      InitialFramePosition();

   }

}


void simple_frame_window::OnUpdateToolWindow(bool bVisible)
{

   if (!m_bDefaultNotifyIcon)
   {

      return;

   }

   if(!bVisible)
   {

      defer_dock_application(false);

   }

   if (m_pimpl == NULL)
   {

      return;

   }

   m_pimpl->show_task(bVisible && m_bShowTask);

}

void simple_frame_window::show_task(bool bShow)
{

   if (m_pimpl == NULL)
   {

      return;

   }

   m_pimpl->show_task(bShow);

}


::user::front_end_schema * simple_frame_window::get_user_front_end_schema()
{

   if (m_workset.m_pframeschema == NULL)
   {

      return NULL;

   }

   return m_workset.m_pframeschema->get_user_front_end_schema();

}


bool simple_frame_window::IsNotifyIconEnabled()
{

   return m_bDefaultNotifyIcon;

}


bool simple_frame_window::get_color(COLORREF & cr, ::user::e_color ecolor)
{

   if (m_workset.m_pframeschema != NULL)
   {


      if (m_workset.m_pframeschema->get_color(cr, ecolor))
      {

         return true;

      }

   }

   return ::user::frame_window::get_color(cr, ecolor);

}

