#include "framework.h"
//#include "base/user/user.h"
//#include "metrowin.h"


CLASS_DECL_BASE int g_iMouse;

#undef System
#undef Platform

using namespace Platform;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::System;
using namespace Windows::Graphics::Display;
using namespace Windows::System::Threading;

uint_ptr keycode_to_char(uint_ptr n)
{
   return n;
}


uint_ptr virtualkey_to_char(::Windows::System::VirtualKey e)
{
   switch(e)
   {
   case ::Windows::System::VirtualKey::Number0:
      return '0';
   case ::Windows::System::VirtualKey::Number1:
      return '1';
   case ::Windows::System::VirtualKey::Number2:
      return '2';
   case ::Windows::System::VirtualKey::Number3:
      return '3';
   case ::Windows::System::VirtualKey::Number4:
      return '4';
   case ::Windows::System::VirtualKey::Number5:
      return '5';
   case ::Windows::System::VirtualKey::Number6:
      return '6';
   case ::Windows::System::VirtualKey::Number7:
      return '7';
   case ::Windows::System::VirtualKey::Number8:
      return '8';
   case ::Windows::System::VirtualKey::Number9:
      return '9';
   case ::Windows::System::VirtualKey::A:
      return 'a';
   case ::Windows::System::VirtualKey::B:
      return 'b';
   case ::Windows::System::VirtualKey::C:
      return 'c';
   case ::Windows::System::VirtualKey::D:
      return 'd';
   case ::Windows::System::VirtualKey::E:
      return 'e';
   case ::Windows::System::VirtualKey::F:
      return 'f';
   case ::Windows::System::VirtualKey::G:
      return 'g';
   case ::Windows::System::VirtualKey::H:
      return 'h';
   case ::Windows::System::VirtualKey::I:
      return 'i';
   case ::Windows::System::VirtualKey::J:
      return 'j';
   case ::Windows::System::VirtualKey::K:
      return 'k';
   case ::Windows::System::VirtualKey::L:
      return 'l';
   case ::Windows::System::VirtualKey::M:
      return 'm';
   case ::Windows::System::VirtualKey::N:
      return 'n';
   case ::Windows::System::VirtualKey::O:
      return 'o';
   case ::Windows::System::VirtualKey::P:
      return 'p';
   case ::Windows::System::VirtualKey::Q:
      return 'q';
   case ::Windows::System::VirtualKey::R:
      return 'r';
   case ::Windows::System::VirtualKey::S:
      return 's';
   case ::Windows::System::VirtualKey::T:
      return 't';
   case ::Windows::System::VirtualKey::U:
      return 'u';
   case ::Windows::System::VirtualKey::V:
      return 'v';
   case ::Windows::System::VirtualKey::W:
      return 'w';
   case ::Windows::System::VirtualKey::X:
      return 'x';
   case ::Windows::System::VirtualKey::Y:
      return 'y';
   case ::Windows::System::VirtualKey::Z:
      return 'z';
   case ::Windows::System::VirtualKey::Shift:
      return VK_SHIFT;
   case ::Windows::System::VirtualKey::LeftShift:
      return VK_LSHIFT;
   case ::Windows::System::VirtualKey::RightShift:
      return VK_RSHIFT;
   case ::Windows::System::VirtualKey::Control:
      return VK_CONTROL;
   case ::Windows::System::VirtualKey::LeftControl:
      return VK_LCONTROL;
   case ::Windows::System::VirtualKey::RightControl:
      return VK_RCONTROL;
   case ::Windows::System::VirtualKey::Menu:
      return VK_MENU;
   case ::Windows::System::VirtualKey::LeftMenu:
      return VK_LMENU;
   case ::Windows::System::VirtualKey::RightMenu:
      return VK_RMENU;
   case ::Windows::System::VirtualKey::Tab:
      return VK_TAB;
   case ::Windows::System::VirtualKey::Enter:
      return VK_RETURN;
   default:;
   }
   return (int)e;
}


::user::e_key virtualkey_to_userkey(::Windows::System::VirtualKey e)
{
   switch(e)
   {
   case ::Windows::System::VirtualKey::Number0:
      return ::user::key_0;
   case ::Windows::System::VirtualKey::Number1:
      return ::user::key_1;
   case ::Windows::System::VirtualKey::Number2:
      return ::user::key_2;
   case ::Windows::System::VirtualKey::Number3:
      return ::user::key_3;
   case ::Windows::System::VirtualKey::Number4:
      return ::user::key_4;
   case ::Windows::System::VirtualKey::Number5:
      return ::user::key_5;
   case ::Windows::System::VirtualKey::Number6:
      return ::user::key_6;
   case ::Windows::System::VirtualKey::Number7:
      return ::user::key_7;
   case ::Windows::System::VirtualKey::Number8:
      return ::user::key_8;
   case ::Windows::System::VirtualKey::Number9:
      return ::user::key_9;
   case ::Windows::System::VirtualKey::A:
      return ::user::key_a;
   case ::Windows::System::VirtualKey::B:
      return ::user::key_b;
   case ::Windows::System::VirtualKey::C:
      return ::user::key_c;
   case ::Windows::System::VirtualKey::D:
      return ::user::key_d;
   case ::Windows::System::VirtualKey::E:
      return ::user::key_e;
   case ::Windows::System::VirtualKey::F:
      return ::user::key_f;
   case ::Windows::System::VirtualKey::G:
      return ::user::key_g;
   case ::Windows::System::VirtualKey::H:
      return ::user::key_h;
   case ::Windows::System::VirtualKey::I:
      return ::user::key_i;
   case ::Windows::System::VirtualKey::J:
      return ::user::key_j;
   case ::Windows::System::VirtualKey::K:
      return ::user::key_k;
   case ::Windows::System::VirtualKey::L:
      return ::user::key_l;
   case ::Windows::System::VirtualKey::M:
      return ::user::key_m;
   case ::Windows::System::VirtualKey::N:
      return ::user::key_n;
   case ::Windows::System::VirtualKey::O:
      return ::user::key_o;
   case ::Windows::System::VirtualKey::P:
      return ::user::key_p;
   case ::Windows::System::VirtualKey::Q:
      return ::user::key_q;
   case ::Windows::System::VirtualKey::R:
      return ::user::key_r;
   case ::Windows::System::VirtualKey::S:
      return ::user::key_s;
   case ::Windows::System::VirtualKey::T:
      return ::user::key_t;
   case ::Windows::System::VirtualKey::U:
      return ::user::key_u;
   case ::Windows::System::VirtualKey::V:
      return ::user::key_v;
   case ::Windows::System::VirtualKey::W:
      return ::user::key_w;
   case ::Windows::System::VirtualKey::X:
      return ::user::key_x;
   case ::Windows::System::VirtualKey::Y:
      return ::user::key_y;
   case ::Windows::System::VirtualKey::Z:
      return ::user::key_z;
   case ::Windows::System::VirtualKey::Shift:
      return ::user::key_shift;
   case ::Windows::System::VirtualKey::LeftShift:
      return ::user::key_lshift;
   case ::Windows::System::VirtualKey::RightShift:
      return ::user::key_rshift;
   case ::Windows::System::VirtualKey::Control:
      return ::user::key_control;
   case ::Windows::System::VirtualKey::LeftControl:
      return ::user::key_lcontrol;
   case ::Windows::System::VirtualKey::RightControl:
      return ::user::key_rcontrol;
   case ::Windows::System::VirtualKey::Menu:
      return ::user::key_alt;
   case ::Windows::System::VirtualKey::LeftMenu:
      return ::user::key_lalt;
   case ::Windows::System::VirtualKey::RightMenu:
      return ::user::key_ralt;
   case ::Windows::System::VirtualKey::Tab:
      return ::user::key_tab;
   case ::Windows::System::VirtualKey::Enter:
      return ::user::key_return;
   case ::Windows::System::VirtualKey::Delete:
      return ::user::key_delete;
   case ::Windows::System::VirtualKey::Back:
      return ::user::key_back;
   case ::Windows::System::VirtualKey::Left:
      return ::user::key_left;
   case ::Windows::System::VirtualKey::Right:
      return ::user::key_right;
   case ::Windows::System::VirtualKey::Up:
      return ::user::key_up;
   case ::Windows::System::VirtualKey::Down:
      return ::user::key_down;
   case ::Windows::System::VirtualKey::PageUp:
      return ::user::key_up;
   case ::Windows::System::VirtualKey::PageDown:
      return ::user::key_down;
   case ::Windows::System::VirtualKey::Home:
      return ::user::key_home;
   case ::Windows::System::VirtualKey::End:
      return ::user::key_end;
   case ::Windows::System::VirtualKey::Escape:
      return ::user::key_escape;
   case 186:
      return ::user::key_semicolon;
   case 187:
      return ::user::key_equal;
   case 188:
      return ::user::key_comma;
   case 189:
      return ::user::key_hyphen;
   case 190:
      return ::user::key_dot;
   case 191:
      return ::user::key_semicolon2;
   case 193:
      return ::user::key_slash2;
   case ::Windows::System::VirtualKey::Space:
      return ::user::key_space;
   default:;
   {
      int iKey = (int)e;
      debug_break();
   }
   }

   return (::user::e_key) e;

}


uint_ptr virtualkey_to_code(::Windows::System::VirtualKey e)
{
   switch(e)
   {
   case 190:
      return '.';
   case ::Windows::System::VirtualKey::Number0:
      return 1;
   case ::Windows::System::VirtualKey::Number1:
      return 2;
   case ::Windows::System::VirtualKey::Number2:
      return 3;
   case ::Windows::System::VirtualKey::Number3:
      return 4;
   case ::Windows::System::VirtualKey::Number4:
      return 5;
   case ::Windows::System::VirtualKey::Number5:
      return 6;
   case ::Windows::System::VirtualKey::Number6:
      return 7;
   case ::Windows::System::VirtualKey::Number7:
      return 8;
   case ::Windows::System::VirtualKey::Number8:
      return 9;
   case ::Windows::System::VirtualKey::Number9:
      return 10;
   case ::Windows::System::VirtualKey::A:
      return 'a';
   case ::Windows::System::VirtualKey::B:
      return 'b';
   case ::Windows::System::VirtualKey::C:
      return 'ca';
   case ::Windows::System::VirtualKey::D:
      return 'd';
   case ::Windows::System::VirtualKey::E:
      return 'e';
   case ::Windows::System::VirtualKey::F:
      return 'f';
   case ::Windows::System::VirtualKey::G:
      return 'g';
   case ::Windows::System::VirtualKey::H:
      return 'h';
   case ::Windows::System::VirtualKey::I:
      return 'i';
   case ::Windows::System::VirtualKey::J:
      return 'j';
   case ::Windows::System::VirtualKey::K:
      return 'k';
   case ::Windows::System::VirtualKey::L:
      return 'l';
   case ::Windows::System::VirtualKey::M:
      return 'm';
   case ::Windows::System::VirtualKey::N:
      return 'n';
   case ::Windows::System::VirtualKey::O:
      return 'o';
   case ::Windows::System::VirtualKey::P:
      return 'p';
   case ::Windows::System::VirtualKey::Q:
      return 'q';
   case ::Windows::System::VirtualKey::R:
      return 'r';
   case ::Windows::System::VirtualKey::S:
      return 's';
   case ::Windows::System::VirtualKey::T:
      return 't';
   case ::Windows::System::VirtualKey::U:
      return 'u';
   case ::Windows::System::VirtualKey::V:
      return 'v';
   case ::Windows::System::VirtualKey::W:
      return 'w';
   case ::Windows::System::VirtualKey::Y:
      return 'y';
   case ::Windows::System::VirtualKey::Z:
      return 'z';
   case ::Windows::System::VirtualKey::Shift:
      return VK_SHIFT;
   case ::Windows::System::VirtualKey::LeftShift:
      return VK_LSHIFT;
   case ::Windows::System::VirtualKey::RightShift:
      return VK_RSHIFT;
   case ::Windows::System::VirtualKey::Control:
      return VK_CONTROL;
   case ::Windows::System::VirtualKey::LeftControl:
      return VK_LCONTROL;
   case ::Windows::System::VirtualKey::RightControl:
      return VK_RCONTROL;
   case ::Windows::System::VirtualKey::Menu:
      return VK_MENU;
   case ::Windows::System::VirtualKey::LeftMenu:
      return VK_LMENU;
   case ::Windows::System::VirtualKey::RightMenu:
      return VK_RMENU;
   default:;
   }
   return (int)e;
}

UINT system_main(LPVOID lp)
{
   ::base::system * m_psystem = (::base::system *) lp;
   try
   {
      //m_psystem->set_thread(m_psystem);
      m_psystem->m_iReturnCode = 0;
      m_psystem->m_bReady = true;
      m_psystem->m_iReturnCode = m_psystem->on_run();
      if(m_psystem->m_iReturnCode != 0)
      {
         ::OutputDebugStringW(L"application::main on_run termination failure");
      }
      if(m_psystem->is_system())
      {
         m_psystem->post_quit();
      }
      try
      {
         m_psystem->m_iReturnCode = m_psystem->exit_thread();
      }
      catch(...)
      {
         m_psystem->m_iReturnCode = -1;
      }

   }
   catch(::exit_exception &)
   {

      m_psystem->post_quit();

   }

   return 0;
}



namespace metrowin
{


   directx_application::directx_application(::base::system * psystem,::String ^ strId):
      m_mutex(NULL)
   {

      m_ptLastCursor.X = 0;
      m_ptLastCursor.Y = 0;

      m_bLeftButton        = false;
      m_bMiddleButton      = false;
      m_bRightButton       = false;


      m_strId = strId;

      if(!main_initialize())
         throw "";


      //_set_purecall_handler(_ca2_purecall);

      m_psystem = psystem;


      m_psystem->m_pbasesystem->m_possystemwindow->m_pui = new ::user::interaction(m_psystem);

      m_psystem->m_pbasesystem->m_possystemwindow->m_pwindow = this;

      m_papp = m_psystem;

      //   ASSERT(hPrevInstance == NULL);



      ::metrowin::main_init_data * pinitmaindata  = new ::metrowin::main_init_data;



      pinitmaindata->m_hInstance             = NULL;
      pinitmaindata->m_hPrevInstance         = NULL;
      pinitmaindata->m_vssCommandLine        = strId;
      pinitmaindata->m_nCmdShow              = 0;

      m_psystem->init_main_data(pinitmaindata);

//      m_psystem->m_paxisapp   = m_psystem;
  //    m_psystem->m_psystem    = m_paxissystem;



      //MessageBox(NULL, "box1", "box1", MB_ICONINFORMATION);



   }


//   ::user::window_draw * directx_application::create_twf_2ex()
//   {
//
////      ::metrowin::window_draw * pwindowdraw = new ::metrowin::window_draw(m_psystem);
//
//  //    pwindowdraw->m_xapp = this;
//
//      return pwindowdraw;
//
//   }


   void directx_application::init_part_2ex()
   {

      m_psystem->m_possystemwindow->m_pwindow = this;

      int nReturnCode = 0;

      if (!m_psystem->begin_synch())
      {

         throw 0;

      }

      m_directx->defer_init();


      ::user::native_window_initialize initialize;

      initialize.pwindow = this;
      initialize.window = m_window.Get();

      m_psystem->m_possystemwindow->m_pui->initialize_native_window(&initialize);

      m_psystem->m_possystemwindow->m_pui->m_pimpl.cast < ::metrowin::interaction_impl >()->m_xapp = this;

      //m_psystem->m_ptwf = create_twf_2ex();

      //m_psystem->m_ptwf->twf_start();

      stringa straLibrary = m_psystem->command()->m_varTopicQuery["app"].stra();

      for(int i = 0; i < m_psystem->command()->m_varTopicQuery["app"].get_count(); i++)
      {
         string strApp = m_psystem->command()->m_varTopicQuery["app"].stra()[i];
      }

      straLibrary.replace("\\", "_");
      straLibrary.replace("-", "_");

      for(int i = 0; i < straLibrary.get_count(); i++)
      {
         string strLibrary = straLibrary[i];
         int iFind = strLibrary.find("/");
         if(iFind >= 0)
            strLibrary = strLibrary.Left(iFind) + '_' + strLibrary.Mid(iFind + 1);
         iFind = strLibrary.find("/", iFind + 1);
         if(iFind >= 0)
            strLibrary.Truncate(iFind);
         m_psystem->m_mapAppLibrary[m_psystem->command()->m_varTopicQuery["app"][i]] = strLibrary;
      }

      for(int i = 0; i < m_psystem->command()->m_varTopicQuery["app"].get_count(); i++)
      {
         string strApp = m_psystem->command()->m_varTopicQuery["app"][i];
      }

      m_psystem->m_mapAppLibrary["app/ca2/cube"] = "ca2";
      m_psystem->m_mapAppLibrary["app/ca2/bergedge"] = "ca2";

      __begin_thread(m_psystem, &system_main, m_psystem);


   }

   void directx_application::Initialize(CoreApplicationView ^ applicationView)
   {

      applicationView->Activated += ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &directx_application::OnActivated);

      CoreApplication::Suspending += ref new EventHandler<SuspendingEventArgs^>(this, &directx_application::OnSuspending);

      CoreApplication::Resuming += ref new EventHandler<Object^>(this, &directx_application::OnResuming);

   }


   void directx_application::install_message_handling_2ex()
   {

      CoreWindow ^ window = m_window.Get();

      window->PointerCursor = ref new CoreCursor(CoreCursorType::Arrow, 0);

      window->SizeChanged += ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &directx_application::OnWindowSizeChanged);

      window->PointerMoved += ref new TypedEventHandler < CoreWindow^, PointerEventArgs^>(this, &directx_application::OnPointerMoved);

      window->CharacterReceived += ref new TypedEventHandler<CoreWindow^, CharacterReceivedEventArgs^>(this, &directx_application::OnCharacterReceived);

      window->KeyDown += ref new TypedEventHandler < CoreWindow^, KeyEventArgs^>(this, &directx_application::OnKeyDown);

      window->KeyUp += ref new TypedEventHandler < CoreWindow^, KeyEventArgs^>( this, &directx_application::OnKeyUp);

      window->PointerPressed += ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &directx_application::OnPointerPressed);

      window->PointerReleased += ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &directx_application::OnPointerReleased);

      ::Windows::Graphics::Display::DisplayInformation ^ displayinformation = ::Windows::Graphics::Display::DisplayInformation::GetForCurrentView();

      displayinformation->DpiChanged += ref new TypedEventHandler < DisplayInformation ^, Object ^ >(this, &directx_application::DpiChanged);

      displayinformation->DisplayContentsInvalidated += ref new TypedEventHandler < DisplayInformation ^, Object ^ >(this, &directx_application::DisplayContentsInvalidated);

   }

   void directx_application::SetWindow(_In_ CoreWindow^ window)
   {

      m_window = window;

      install_message_handling_2ex();

      m_directx = ref new directx_base(m_psystem);


      ::Windows::Graphics::Display::DisplayInformation ^ displayinformation = ::Windows::Graphics::Display::DisplayInformation::GetForCurrentView();

      m_directx->Initialize(window, displayinformation->LogicalDpi);

      init_part_2ex();

      m_directx->defer_init();

      m_rectLastWindowRect = m_window->Bounds;

      //m_psystem->m_posdata->m_pui->on_layout();

//      m_directx->UpdateForWindowSizeChange();

   }

   void directx_application::Load(String^ entryPoint)
   {
   }

   void directx_application::Run()
   {
      m_window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessUntilQuit);
   }


   void directx_application::Uninitialize()
   {

   }


   void directx_application::OnWindowSizeChanged(CoreWindow ^ sender, WindowSizeChangedEventArgs ^ args)
   {

      m_directx->m_size.set_size(args->Size.Width,args->Size.Height);

      m_rectLastWindowRect.Width = m_directx->m_size.cx;

      m_rectLastWindowRect.Height = m_directx->m_size.cy;

      m_directx->UpdateForWindowSizeChange();

   }


   void directx_application::DpiChanged(::Windows::Graphics::Display::DisplayInformation ^ sender, Object ^ obj)
   {

      m_rectLastWindowRect = m_window->Bounds;

      m_directx->SetDpi(sender->LogicalDpi);


   }


   void directx_application::DisplayContentsInvalidated(::Windows::Graphics::Display::DisplayInformation ^ sender, Object ^ obj)
   {

      // Ensure the D3D Device is available for rendering.

      m_directx->ValidateDevice();

   }


   void directx_application::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
   {

      m_window->Activate();

      if (args != nullptr)
      {

         if (args->Kind == ActivationKind::Protocol)
         {

            ProtocolActivatedEventArgs ^ eventArgs = (ProtocolActivatedEventArgs ^)args;

            string str = eventArgs->Uri->AbsoluteUri;

            ::aura::application * papp = m_psystem->m_possystemwindow->m_pui->m_pimpl->get_app();

            if (papp->m_pipi == NULL)
            {
               
               sp(::user::interaction) pui;

               m_psystem->get_frame(pui);

               if (pui == NULL)
               {

                  return;

               }

               papp = pui->get_app();

               if (papp == NULL)
               {

                  return;

               }

               if (papp->m_pipi == NULL)
               {

                  return;

               }

            }

            if (::str::begins_eat_ci(str, papp->m_pipi->m_rx.m_strBaseChannel))
            {

               if (::str::begins_eat_ci(str, ":///"))
               {

                  if (::str::begins_eat_ci(str, "send?message="))
                  {

                     papp->m_pipi->on_receive(&papp->m_pipi->m_rx, m_psystem->url().url_decode(str));

                  }
                  else if (::str::begins_eat_ci(str, "send?messagebin="))
                  {

                     strsize iFind = str.find(',');

                     if (iFind >= 0)
                     {

                        int message = atoi(str.Left(iFind));

                        memory m;

                        Sys(get_app()).base64().decode(m, m_psystem->url().url_decode(str.Mid(iFind + 1)));

                        papp->m_pipi->on_receive(&papp->m_pipi->m_rx, message, m.get_data(), m.get_size());

                     }

                  }

               }

            }

         }

      }


   }


   void directx_application::OnSuspending(Object ^ sender, SuspendingEventArgs ^ args)
   {

   }


   void directx_application::OnResuming(Object ^ sender, Object ^ args)
   {

   }


   void directx_application::OnCharacterReceived(Windows::UI::Core::CoreWindow ^ , Windows::UI::Core::CharacterReceivedEventArgs ^ args)
   {

      if(m_psystem == NULL)
         return;

      if (m_psystem->m_possystemwindow->m_pui == NULL)
         return;

      if (m_psystem->m_possystemwindow->m_pui->m_pimpl == NULL)
         return;

      smart_pointer < ::message::base > spbase;

      ::message::key * pkey = canew(::message::key(get_app()));

      spbase = pkey;

      pkey->m_uiMessage = WM_CHAR;
      pkey->m_pwnd = m_psystem->m_possystemwindow->m_pui;
      pkey->m_nChar = keycode_to_char(args->KeyCode);
//      pkey->m_charrecv = args;


      m_psystem->m_possystemwindow->m_pui->m_pimpl->queue_message_handler(spbase);

   }


   void directx_application::OnKeyDown(Windows::UI::Core::CoreWindow ^ , Windows::UI::Core::KeyEventArgs ^ args)
   {
      if (args->VirtualKey == ::Windows::System::VirtualKey::Shift)
      {
         m_bFontopusShift = true;
      }

      if(m_psystem == NULL)
         return;

      if (m_psystem->m_possystemwindow->m_pui == NULL)
         return;

      if (m_psystem->m_possystemwindow->m_pui->m_pimpl == NULL)
         return;

      smart_pointer < ::message::base > spbase;

      ::message::key * pkey = canew(::message::key(get_app()));

      spbase = pkey;

      pkey->m_uiMessage          = WM_KEYDOWN;
      pkey->m_pwnd               = m_psystem->m_possystemwindow->m_pui;
      pkey->m_nChar              = virtualkey_to_char(args->VirtualKey);
      pkey->m_ekey               = virtualkey_to_userkey(args->VirtualKey);
      pkey->m_wparam             = pkey->m_nChar;
      pkey->m_nFlags             = virtualkey_to_code(args->VirtualKey);
      pkey->m_lparam             = pkey->m_nFlags << 16;
//      pkey->m_key = args;


      m_psystem->m_possystemwindow->m_pui->m_pimpl->queue_message_handler(spbase);

   }

   void directx_application::OnKeyUp(Windows::UI::Core::CoreWindow ^ , Windows::UI::Core::KeyEventArgs ^ args)
   {

      if(m_psystem == NULL)
         return;

      if (m_psystem->m_possystemwindow->m_pui == NULL)
         return;

      if (m_psystem->m_possystemwindow->m_pui->m_pimpl == NULL)
         return;

      smart_pointer < ::message::base > spbase;

      ::message::key * pkey = new  ::message::key(get_app());

      spbase = pkey;


      if (args->VirtualKey == ::Windows::System::VirtualKey::Shift)
      {
         m_bFontopusShift = false;
      }

      pkey->m_uiMessage       = WM_KEYUP;
      pkey->m_pwnd            = m_psystem->m_possystemwindow->m_pui;
      pkey->m_nChar           = virtualkey_to_char(args->VirtualKey);
      pkey->m_ekey               = virtualkey_to_userkey(args->VirtualKey);
      pkey->m_wparam          = pkey->m_nChar;
//      pkey->m_key = args;

      //if (m_psystem->m_psimpleui != NULL && m_psystem->m_psimpleui->IsWindowVisible())
      //{
      //   string str;
      //   str = (char)pkey->m_nChar;
      //   if (m_bFontopusShift)
      //   {
      //      if (pkey->m_nChar == 0xbe)
      //      {
      //         str = ">";
      //      }
      //      else if (str == "2")
      //      {
      //         str = "@";
      //      }
      //      else
      //      {
      //         str.make_upper();
      //      }
      //   }
      //   else
      //   {
      //      if (pkey->m_nChar == 0xbe)
      //      {
      //         str = ".";
      //      }
      //   }
      //   m_psystem->m_psimpleui->on_char(virtualkey_to_userkey(args->VirtualKey), str);
      //}
      //else
      //{
         m_psystem->m_possystemwindow->m_pui->m_pimpl->queue_message_handler(spbase);
      //}



   }


   void directx_application::OnPointerMoved(Windows::UI::Core::CoreWindow ^ , Windows::UI::Core::PointerEventArgs ^ args)
   {

      if(m_psystem == NULL)
         return;

      if(m_psystem->m_possystemwindow->m_pui == NULL)
         return;

      if(m_psystem->m_possystemwindow->m_pui->m_pimpl == NULL)
         return;

      Windows::UI::Input::PointerPoint^ pointerPoint = args->CurrentPoint;

      ::g_iMouse = pointerPoint->PointerId;

      smart_pointer < ::message::base > spbase;

      ::message::mouse * pmouse = new  ::message::mouse(get_app());

      spbase = pmouse;

      pmouse->m_pt.x       = (LONG) pointerPoint->RawPosition.X;
      pmouse->m_pt.y       = (LONG) pointerPoint->RawPosition.Y;
      pmouse->m_uiMessage  = WM_MOUSEMOVE;
      pmouse->m_pwnd       = m_psystem->m_possystemwindow->m_pui;

      m_ptLastCursor = pointerPoint->RawPosition;

      m_psystem->m_possystemwindow->m_pui->m_pimpl->queue_message_handler(spbase);

   }


   void directx_application::OnPointerPressed(Windows::UI::Core::CoreWindow ^ , Windows::UI::Core::PointerEventArgs ^ args)
   {

      if(m_psystem == NULL)
         return;

      if(m_psystem->m_possystemwindow->m_pui == NULL)
         return;

      if(m_psystem->m_possystemwindow->m_pui->m_pimpl == NULL)
         return;

      Windows::UI::Input::PointerPoint^ pointerPoint = args->CurrentPoint;

      ::g_iMouse = pointerPoint->PointerId;

      smart_pointer < ::message::base > spbase;

      ::message::mouse * pmouse = new  ::message::mouse(get_app());

      spbase = pmouse;

      pmouse->m_pt.x = (LONG) pointerPoint->RawPosition.X;

      pmouse->m_pt.y = (LONG) pointerPoint->RawPosition.Y;

      if(args->CurrentPoint->Properties->IsLeftButtonPressed && !m_bLeftButton)
      {

         pmouse->m_uiMessage     = WM_LBUTTONDOWN;

         m_bLeftButton           = true;
         m_bMiddleButton         = false;
         m_bRightButton          = false;

      }
      else if(args->CurrentPoint->Properties->IsRightButtonPressed && !m_bRightButton)
      {

         pmouse->m_uiMessage     = WM_RBUTTONDOWN;

         m_bLeftButton           = false;
         m_bMiddleButton         = false;
         m_bRightButton          = true;

      }
      else if(args->CurrentPoint->Properties->IsMiddleButtonPressed && !m_bMiddleButton)
      {

         pmouse->m_uiMessage     = WM_MBUTTONDOWN;

         m_bLeftButton           = false;
         m_bMiddleButton         = true;
         m_bRightButton          = false;

      }

      pmouse->m_pwnd = m_psystem->m_possystemwindow->m_pui;

      m_ptLastCursor = pointerPoint->RawPosition;

      m_psystem->m_possystemwindow->m_pui->m_pimpl->queue_message_handler(spbase);

   }

   void directx_application::OnPointerReleased(Windows::UI::Core::CoreWindow ^ , Windows::UI::Core::PointerEventArgs ^ args)
   {

      if(m_psystem == NULL)
         return;


      Windows::UI::Input::PointerPoint^ pointerPoint = args->CurrentPoint;

      ::g_iMouse = pointerPoint->PointerId;

      smart_pointer < ::message::base > spbase;

      ::message::mouse * pmouse = new  ::message::mouse(get_app());

      spbase = pmouse;

      pmouse->m_pt.x = (LONG) pointerPoint->RawPosition.X;

      pmouse->m_pt.y = (LONG) pointerPoint->RawPosition.Y;

      if(m_bLeftButton && !args->CurrentPoint->Properties->IsLeftButtonPressed)
      {

         pmouse->m_uiMessage     = WM_LBUTTONUP;
         m_bLeftButton           = false;

      }
      else if(m_bRightButton && !args->CurrentPoint->Properties->IsRightButtonPressed)
      {

         pmouse->m_uiMessage     = WM_RBUTTONUP;
         m_bRightButton          = false;

      }
      else if(m_bMiddleButton && !args->CurrentPoint->Properties->IsMiddleButtonPressed)
      {

         pmouse->m_uiMessage     = WM_MBUTTONUP;
         m_bMiddleButton         = false;

      }





      if (m_psystem->m_possystemwindow->m_pui == NULL)
         return;

      if (m_psystem->m_possystemwindow->m_pui->m_pimpl == NULL)
         return;


      pmouse->m_pwnd = m_psystem->m_possystemwindow->m_pui;

      m_ptLastCursor = pointerPoint->RawPosition;

      if (::get_thread() == NULL)
      {
       
         ::set_thread(m_psystem->m_possystemwindow->m_pui->get_app());

      }

      m_psystem->m_possystemwindow->m_pui->m_pimpl->queue_message_handler(spbase);

   }

   directx_application_source::directx_application_source(::base::system * paxissystem, const string & strId)
   {

      m_pbasesystem     = paxissystem;

      m_strId           = strId;

   }


   Windows::ApplicationModel::Core::IFrameworkView^ directx_application_source::CreateView()
   {

      return ref new directx_application(m_pbasesystem,m_strId);

   }


   directx_application_source ^ new_directx_application_source(::base::system * paxissystem, const string & strId)
   {

      string str = strId;

      //str += " client_only";

      //str += " full_screen";

      return ref new directx_application_source(paxissystem, str);

   }


   Windows::Foundation::Rect directx_application::get_window_rect()
   {

      Windows::Foundation::Rect rect = m_rectLastWindowRect;

/*      rect.X = 0;
      rect.Y = 0;
      rect.Width = 600;
      rect.Height = 480;

      if(m_window == nullptr)
         return rect;

      ::wait(m_window->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler ([=, &rect]()
      {
         try
         {
            rect = m_window->Bounds;
         }
         catch(...)
         {
         }
      })));*/

      return rect;

   }

   Windows::Foundation::Point directx_application::get_cursor_pos()
   {

      single_lock sl(&m_mutex, true);

      Windows::Foundation::Point p = m_ptLastCursor;

/*      if(m_window == nullptr)
         return p;

      if(g_iMouse < 0)
         return p;

      ::wait(m_window->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler ([=, &p]()
      {
         try
         {

            Windows::Foundation::Collections::IVectorView < Windows::Devices::Input::PointerDevice ^ > ^ deva = ::Windows::Devices::Input::PointerDevice::GetPointerDevices();

            for(unsigned int ui = 0; ui < deva->Size; ui++)
            {

               Windows::Devices::Input::PointerDevice ^ dev = deva->GetAt(ui);

               if(dev->PointerDeviceType == ::Windows::Devices::Input::PointerDeviceType::Mouse)
               {

                  Windows::UI::Input::PointerPoint ^ pointerPoint = Windows::UI::Input::PointerPoint::GetCurrentPoint(g_iMouse);

                  p = pointerPoint->RawPosition;

                  break;

               }

            }

         }
         catch(...)
         {
         }
      })), 5); // wait for up to 5 milliseconds

      m_ptLastCursor = p;*/

      return p;

   }


   ::aura::application * directx_application::get_app() const
   {

      return m_papp;

   }


} // namespace metrowin




