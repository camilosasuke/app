#include "framework.h"
//#include "base/user/user.h"
//#include "metrowin.h"
#include "app/appseed/draw2d_direct2d/draw2d_direct2d.h"

using namespace Windows::UI::Core;
using namespace Windows::Foundation;
using namespace Microsoft::WRL;
using namespace Windows::Graphics::Display;
using namespace D2D1;


extern CLASS_DECL_AURA mutex * g_pmutexGraphicsDraw;

namespace metrowin
{

   // Constructor.
   directx_base::directx_base(::aura::application * papp):
      m_pauraapp(papp),
      m_windowSizeChangeInProgress(false),
      m_dpi(-1.0f),
      m_mutexDc(papp)
   {
      m_dpiIni = 1.0f;
      m_dpi = -999.0f;
      m_bInitialized = false;
      m_bInit = false;

      g_pmutexGraphicsDraw = &draw2d_mutex();

   }

   // Initialize the DirectX resources required to run.
   void directx_base::Initialize(CoreWindow^ window,float dpi)
   {

      m_window = window;

      m_dpiIni = dpi;

      CreateDeviceIndependentResources();
      //CreateDeviceResources();
      //SetDpi(dpi);
      m_bInit = true;
   }


   bool directx_base::defer_init()
   {
      synch_lock sl(&draw2d_mutex());

      if(m_bInitialized)
         return true;

      if(!m_bInit)
         return false;

      CreateDeviceResources();

      SetDpi(m_dpiIni);

      m_bInitialized = true;

      return true;

   }
   // Recreate all device resources and set them back to the current state.
   void directx_base::HandleDeviceLost()
   {
      synch_lock sl(&draw2d_mutex());

      // Reset these member variables to ensure that SetDpi recreates all resources.
      float dpi = m_dpi;
      m_dpi = -1.0f;
      m_windowBounds.Width = 0;
      m_windowBounds.Height = 0;
      m_swapChain = nullptr;

      CreateDeviceResources();
      SetDpi(dpi);
   }

   // These are the resources required independent of the device.
   void directx_base::CreateDeviceIndependentResources()
   {
      synch_lock sl(&draw2d_mutex());

      D2D1_FACTORY_OPTIONS options;
      ZeroMemory(&options,sizeof(D2D1_FACTORY_OPTIONS));

#if defined(_DEBUG)
      // If the project is in a debug build, enable Direct2D debugging via SDK Layers.
      options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif


      ::metrowin::throw_if_failed(
         DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            &m_dwriteFactory
         )
      );

      ::metrowin::throw_if_failed(
         CoCreateInstance(
            CLSID_WICImagingFactory,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&m_wicFactory)
         )
      );






















































      // Create a DirectWrite text format object.
      ::metrowin::throw_if_failed(
         m_dwriteFactory->CreateTextFormat(
            L"Gabriola",
            nullptr,
            DWRITE_FONT_WEIGHT_REGULAR,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            64.0f,
            L"en-US", // locale
            &m_textFormat
         )
      );

      // Center the text horizontally.
      ::metrowin::throw_if_failed(
         m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER)
      );

      // Center the text vertically.
      ::metrowin::throw_if_failed(
         m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER)
      );





   }

   // These are the resources that depend on the device.
   void directx_base::CreateDeviceResources()
   {

      synch_lock sl(&draw2d_mutex());

      //      // This flag adds support for surfaces with a different color channel ordering
      //      // than the API default. It is required for compatibility with Direct2D.
      //      UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
      //      ComPtr<IDXGIDevice> dxgiDevice;
      //
      //#if defined(_DEBUG)
      //      // If the project is in a debug build, enable debugging via SDK Layers with this flag.
      //      creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
      //#endif
      //
      //      // This array defines the set of DirectX hardware feature levels this app will support.
      //      // Note the ordering should be preserved.
      //      // Don't forget to declare your application's minimum required feature level in its
      //      // description.  All applications are assumed to support 9.1 unless otherwise stated.
      //      D3D_FEATURE_LEVEL featureLevels[] =
      //      {
      //         D3D_FEATURE_LEVEL_11_1,
      //         D3D_FEATURE_LEVEL_11_0,
      //         D3D_FEATURE_LEVEL_10_1,
      //         D3D_FEATURE_LEVEL_10_0,
      //         D3D_FEATURE_LEVEL_9_3,
      //         D3D_FEATURE_LEVEL_9_2,
      //         D3D_FEATURE_LEVEL_9_1
      //      };
      //
      //      // Create the Direct3D 11 API device object and a corresponding context.
      //      ComPtr<ID3D11Device> device;
      //      ComPtr<ID3D11DeviceContext> context;
      //      ::metrowin::throw_if_failed(
      //         D3D11CreateDevice(
      //         nullptr,                    // Specify nullptr to use the default adapter.
      //         D3D_DRIVER_TYPE_HARDWARE,
      //         0,
      //         creationFlags,              // Set debug and Direct2D compatibility flags.
      //         featureLevels,              // List of feature levels this app can support.
      //         ARRAYSIZE(featureLevels),
      //         D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Metro style apps.
      //         &device,                    // Returns the Direct3D device created.
      //         &m_featureLevel,            // Returns feature level of device created.
      //         &context                    // Returns the device immediate context.
      //         )
      //         );




      //
      //      // Get the Direct3D 11.1 API device and context interfaces.
      //      ::metrowin::throw_if_failed(
      //         device.As(&m_d3dDevice)
      //         );

      m_d3dDevice = TlsGetD3D11Device1();

      m_d3dContext = TlsGetD3D11DeviceContext1();

      //m_d2dDevice = TlsGetD3D11DeviceCo1();

      //
      //      ::metrowin::throw_if_failed(
      //         context.As(&m_d3dContext)
      //         );
      //
      //      // Get the underlying DXGI device of the Direct3D device.
      //      ::metrowin::throw_if_failed(
      //         m_d3dDevice.As(&dxgiDevice)
      //         );
      //
      // Create the Direct2D device object and a corresponding context.
      //::metrowin::throw_if_failed(
      //   GetD2D1Factory1()->CreateDevice(TlsGetDXGIDevice(),&m_d2dDevice)
      //   );

      m_d2dDevice = TlsGetD2D1Device();
      //
      //::metrowin::throw_if_failed(
      //   m_d2dDevice->CreateDeviceContext(
      //   D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
      //   //D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
      //   &m_d2dContext
      //   )
      //   );

      m_d2dContext = TlsGetD2D1DeviceContext();
      //
      //      ID2D1DeviceContext * pdevicecontext = m_d2dContext.Get();
      //
      //      System.m_pdevicecontext    = pdevicecontext;
      //      System.m_pmutexDc          = &m_mutexDc;







































      ::metrowin::throw_if_failed(
         m_d2dContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Black),
            &m_blackBrush
         )
      );



   }

   // Helps track the DPI in the helper class.
   // This is called in the dpiChanged event handler in the view class.
   void directx_base::SetDpi(float dpi)
   {
      synch_lock sl(&draw2d_mutex());

      if(dpi != m_dpi)
      {
         // Save the DPI of this display in our class.
         m_dpi = dpi;

         System.m_dpi = dpi;

         // Update Direct2D's stored DPI.
         m_d2dContext->SetDpi(m_dpi,m_dpi);

         m_size.cx = m_window->Bounds.Width;

         m_size.cy = m_window->Bounds.Height;

         // Often a DPI change implies a window size change. In some cases Windows will issue
         // both a size changed event and a DPI changed event. In this case, the resulting bounds
         // will not change, and the window resize code will only be executed once.
         UpdateForWindowSizeChange();
      }
   }

   // This routine is called in the event handler for the view SizeChanged event.
   void directx_base::UpdateForWindowSizeChange()
   {

      // Only handle window size changed if there is no pending DPI change.

      m_window->Dispatcher->RunAsync(CoreDispatcherPriority::Normal,ref new Windows::UI::Core::DispatchedHandler([this]()
      {

         ::aura::system::g_p->m_pbasesystem->m_possystemwindow->m_bWindowSizeChange = true;

      }));

   }

   //::Windows::Graphics::Display::DisplayInformation ^ displayinformation = ::Windows::Graphics::Display::DisplayInformation::GetForCurrentView();

   //if(m_dpi != displayinformation->LogicalDpi)
   //{
   //   return;
   //}
   void directx_base::OnWindowSizeChange()
   {

      synch_lock sl(&draw2d_mutex());

      if (m_size.cx != m_windowBounds.Width ||
         m_size.cy != m_windowBounds.Height)
      {
         m_d2dContext->SetTarget(nullptr);
         m_d2dTargetBitmap = nullptr;
         m_d3dRenderTargetView = nullptr;
         m_d3dDepthStencilView = nullptr;
         m_windowSizeChangeInProgress = true;
         CreateWindowSizeDependentResources();
         sl.unlock();
         System.m_possystemwindow->m_pui->SetWindowPos(ZORDER_TOP, 0, 0, m_size.cx, m_size.cy, SWP_SHOWWINDOW);
         if (System.directrix()->m_varTopicQuery.has_property("client_only"))
         {
            for (int i = 0; i < System.m_possystemwindow->m_pui->m_uiptraChild.get_count(); i++)
            {
               if (System.m_possystemwindow->m_pui->m_uiptraChild[i]->IsWindowVisible())
               {
                  System.m_possystemwindow->m_pui->m_uiptraChild[i]->SetWindowPos(ZORDER_TOP, 0, 0, m_size.cx, m_size.cy, SWP_SHOWWINDOW);

                  //System.m_posdata->m_pui->m_uiptraChild[i]->on_layout();
               }
            }
         }
      }

   }


   // Allocate all memory resources that change on a window SizeChanged event.
   void directx_base::CreateWindowSizeDependentResources()
   {
      synch_lock sl(&draw2d_mutex());

      // Store the window bounds so the next time we get a SizeChanged event we can
      // avoid rebuilding everything if the size is identical.
      m_windowBounds.Width = m_size.cx;
      m_windowBounds.Height = m_size.cy;

      if(m_swapChain != nullptr)
      {
         // If the swap chain already exists, resize it.
         HRESULT hr = m_swapChain->ResizeBuffers(
                         2,
                         0, // If you specify zero, DXGI will use the width of the client area of the target window.
                         0, // If you specify zero, DXGI will use the height of the client area of the target window.
                         DXGI_FORMAT_UNKNOWN, // Set this value to DXGI_FORMAT_UNKNOWN to preserve the existing format of the back buffer.
                         0);

         if(hr == DXGI_ERROR_DEVICE_REMOVED)
         {
            // If the device was removed for any reason, a new device and swapchain will need to be created.
            HandleDeviceLost();

            // Everything is set up now. Do not continue execution of this method.
            return;
         }
         else if(hr == DXGI_ERROR_INVALID_CALL)
         {
            // i1;
            //  return;
            TRACE("directx_base::CreateWindowSizeDependentResources(1) DXGI_ERROR_INVALID_CALL");
         }
         else
         {
            ::metrowin::throw_if_failed(hr);
         }
      }
      else
      {
         // Otherwise, create a new one using the same adapter as the existing Direct3D device.
         DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {0};
         swapChainDesc.Width = 0;                                     // Use automatic sizing.
         swapChainDesc.Height = 0;
         swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;           // This is the most common swap chain format.
         swapChainDesc.Stereo = false;
         swapChainDesc.SampleDesc.Count = 1;                          // Don't use multi-sampling.
         swapChainDesc.SampleDesc.Quality = 0;
         swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
         swapChainDesc.BufferCount = 2;                               // Use double-buffering to minimize latency.
         swapChainDesc.Scaling = DXGI_SCALING_NONE;
         swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // All Metro style apps must use this SwapEffect.
         swapChainDesc.Flags = 0;

         ComPtr<IDXGIDevice1> dxgiDevice;
         ::metrowin::throw_if_failed(
            m_d3dDevice.As(&dxgiDevice)
         );

         ComPtr<IDXGIAdapter> dxgiAdapter;
         ::metrowin::throw_if_failed(
            dxgiDevice->GetAdapter(&dxgiAdapter)
         );

         ComPtr<IDXGIFactory2> dxgiFactory;
         ::metrowin::throw_if_failed(
            dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory))
         );

         CoreWindow^ window = m_window.Get();
         ::metrowin::throw_if_failed(
            dxgiFactory->CreateSwapChainForCoreWindow(
               m_d3dDevice.Get(),
               reinterpret_cast<IUnknown*>(window),
               &swapChainDesc,
               nullptr,
               &m_swapChain
            )
         );

         // Ensure that DXGI does not queue more than one frame at a time. This both reduces latency and
         // ensures that the application will only render after each VSync, minimizing power consumption.
         ::metrowin::throw_if_failed(
            dxgiDevice->SetMaximumFrameLatency(1)
         );
      }

      // Create a Direct3D render target view of the swap chain back buffer.
      ComPtr<ID3D11Texture2D> backBuffer;
      ::metrowin::throw_if_failed(
         m_swapChain->GetBuffer(0,IID_PPV_ARGS(&backBuffer))
      );

      ::metrowin::throw_if_failed(
         m_d3dDevice->CreateRenderTargetView(
            backBuffer.Get(),
            nullptr,
            &m_d3dRenderTargetView
         )
      );

      // Cache the rendertarget dimensions in our helper class for convenient use.
      D3D11_TEXTURE2D_DESC backBufferDesc = {0};
      backBuffer->GetDesc(&backBufferDesc);
      m_renderTargetSize.Width  = static_cast<float>(backBufferDesc.Width);
      m_renderTargetSize.Height = static_cast<float>(backBufferDesc.Height);

      // Create a depth stencil view for use with 3D rendering if needed.
      CD3D11_TEXTURE2D_DESC depthStencilDesc(
         DXGI_FORMAT_D24_UNORM_S8_UINT,
         backBufferDesc.Width,
         backBufferDesc.Height,
         1,
         1,
         D3D11_BIND_DEPTH_STENCIL
      );

      ComPtr<ID3D11Texture2D> depthStencil;
      ::metrowin::throw_if_failed(
         m_d3dDevice->CreateTexture2D(
            &depthStencilDesc,
            nullptr,
            &depthStencil
         )
      );

      auto viewDesc = CD3D11_DEPTH_STENCIL_VIEW_DESC(D3D11_DSV_DIMENSION_TEXTURE2D);
      ::metrowin::throw_if_failed(
         m_d3dDevice->CreateDepthStencilView(
            depthStencil.Get(),
            &viewDesc,
            &m_d3dDepthStencilView
         )
      );

      // Set the 3D rendering viewport to target the entire window.
      CD3D11_VIEWPORT viewport(
         0.0f,
         0.0f,
         static_cast<float>(backBufferDesc.Width),
         static_cast<float>(backBufferDesc.Height)
      );

      m_d3dContext->RSSetViewports(1,&viewport);

      // Create a Direct2D target bitmap associated with the
      // swap chain back buffer and set it as the current target.
      D2D1_BITMAP_PROPERTIES1 bitmapProperties =
         BitmapProperties1(
            D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
            PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED),
            m_dpi,
            m_dpi
         );

      ComPtr<IDXGISurface> dxgiBackBuffer;
      ::metrowin::throw_if_failed(
         m_swapChain->GetBuffer(0,IID_PPV_ARGS(&dxgiBackBuffer))
      );

      ::metrowin::throw_if_failed(
         m_d2dContext->CreateBitmapFromDxgiSurface(
            dxgiBackBuffer.Get(),
            &bitmapProperties,
            &m_d2dTargetBitmap
         )
      );

      m_d2dContext->SetTarget(m_d2dTargetBitmap.Get());

      // Grayscale text anti-aliasing is recommended for all Metro style apps.
      m_d2dContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

      String^ text = "Hello World From ... DirectWrite!";

      D2D1_SIZE_F size = m_d2dContext->GetSize();

      // Create a DirectWrite Text Layout object
      ::metrowin::throw_if_failed(
         m_dwriteFactory->CreateTextLayout(
            text->Data(),                       // Text to be displayed
            text->Length(),                     // Length of the text
            m_textFormat.Get(),                 // DirectWrite Text Format object
            size.width,                         // Width of the Text Layout
            size.height,                        // Height of the Text Layout
            &m_textLayout
         )
      );

      // Text range for the "DirectWrite!" at the end of the string
      DWRITE_TEXT_RANGE textRange = {21,12}; // 21 references the "D" in DirectWrite! and 12 is the number of characters in the word

      // Set the font size on that text range to 100
      ::metrowin::throw_if_failed(
         m_textLayout->SetFontSize(100.0f,textRange)
      );

      // Create a DirectWrite Typography object
      ::metrowin::throw_if_failed(
         m_dwriteFactory->CreateTypography(
            &m_textTypography
         )
      );

      // Enumerate a stylistic set 6 font feature for application to our text on_layout
      DWRITE_FONT_FEATURE fontFeature = {DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_6,1};

      // Apply the previously enumerated font feature to our Text Typography object
      ::metrowin::throw_if_failed(
         m_textTypography->AddFontFeature(fontFeature)
      );

      // Move our text range to the entire length of the string
      textRange.length = text->Length();
      textRange.startPosition = 0;

      // Apply our recently defined typography to our entire text range
      ::metrowin::throw_if_failed(
         m_textLayout->SetTypography(
            m_textTypography.Get(),
            textRange
         )
      );

      // Move the text range to the end again
      textRange.length = 12;
      textRange.startPosition = 21;

      // Set the underline on the text range
      ::metrowin::throw_if_failed(
         m_textLayout->SetUnderline(TRUE,textRange)
      );

      // Set the font weight on the text range
      ::metrowin::throw_if_failed(
         m_textLayout->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD,textRange)
      );


   }

   // Method to deliver the final image to the display.
   void directx_base::Present()
   {

      synch_lock sl(&draw2d_mutex());

      if(!defer_init())
         return;

      // The application may optionally specify "dirty" or "scroll" rects to improve efficiency
      // in certain scenarios.  In this sample, however, we do not utilize those features.
      DXGI_PRESENT_PARAMETERS parameters = {0};
      parameters.DirtyRectsCount = 0;
      parameters.pDirtyRects = nullptr;
      parameters.pScrollRect = nullptr;
      parameters.pScrollOffset = nullptr;

      if (m_swapChain == nullptr)
      {

         return;

      }

      // The first argument instructs DXGI to block until VSync, putting the application
      // to sleep until the next VSync. This ensures we don't waste any cycles rendering
      // frames that will never be displayed to the screen.
      HRESULT hr = m_swapChain->Present1(1,0,&parameters);

      // Discard the contents of the render target.
      // This is a valid operation only when the existing contents will be entirely
      // overwritten. If dirty or scroll rects are used, this call should be removed.
      m_d3dContext->DiscardView(m_d3dRenderTargetView.Get());

      // Discard the contents of the depth stencil.
      m_d3dContext->DiscardView(m_d3dDepthStencilView.Get());

      // If the device was removed either by a disconnect or a driver upgrade, we
      // must recreate all device resources.
      if(hr == DXGI_ERROR_DEVICE_REMOVED)
      {
         HandleDeviceLost();
      }
      else
      {
         ::metrowin::throw_if_failed(hr);
      }

      sl.unlock();

      m_window->Dispatcher->RunAsync(CoreDispatcherPriority::Normal,ref new Windows::UI::Core::DispatchedHandler([this]()
      {
         if(m_windowSizeChangeInProgress)
         {
            // A window size change has been initiated and the app has just completed presenting
            // the first frame with the new size. Notify the resize manager so we can short
            // circuit any resize animation and prevent unnecessary delays.
            CoreWindowResizeManager::GetForCurrentView()->NotifyLayoutCompleted();
            m_windowSizeChangeInProgress = false;
         }
      }));

   }

   void directx_base::ValidateDevice()
   {
      synch_lock sl(&draw2d_mutex());
      // The D3D Device is no longer valid if the default adapter changes or if
      // the device has been removed.

      // First, get the information for the adapter related to the current device.

      ComPtr<IDXGIDevice1> dxgiDevice;
      ComPtr<IDXGIAdapter> deviceAdapter;
      DXGI_ADAPTER_DESC deviceDesc;
      ::metrowin::throw_if_failed(m_d3dDevice.As(&dxgiDevice));
      ::metrowin::throw_if_failed(dxgiDevice->GetAdapter(&deviceAdapter));
      ::metrowin::throw_if_failed(deviceAdapter->GetDesc(&deviceDesc));

      // Next, get the information for the default adapter.

      ComPtr<IDXGIFactory2> dxgiFactory;
      ComPtr<IDXGIAdapter1> currentAdapter;
      DXGI_ADAPTER_DESC currentDesc;
      ::metrowin::throw_if_failed(CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)));
      ::metrowin::throw_if_failed(dxgiFactory->EnumAdapters1(0,&currentAdapter));
      ::metrowin::throw_if_failed(currentAdapter->GetDesc(&currentDesc));

      // If the adapter LUIDs don't match, or if the device reports that it has been removed,
      // a new D3D device must be created.

      if((deviceDesc.AdapterLuid.LowPart != currentDesc.AdapterLuid.LowPart) ||
            (deviceDesc.AdapterLuid.HighPart != currentDesc.AdapterLuid.HighPart) ||
            FAILED(m_d3dDevice->GetDeviceRemovedReason()))
      {
         // Release references to resources related to the old device.
         dxgiDevice = nullptr;
         deviceAdapter = nullptr;

         // Create a new device and swap chain.
         HandleDeviceLost();
      }
   }


   HRESULT directx_base::Render()
   {
      synch_lock sl(&draw2d_mutex());

      if(!defer_init())
         return E_FAIL;

      m_d2dContext->BeginDraw();

      D2D1_COLOR_F cr;
      cr.a = 1.0f;
      cr.b = 0xcc / 255.f;
      cr.r = 0xcc / 255.f;
      cr.g = 0xcc / 255.f;
      m_d2dContext->Clear(cr);


      m_d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());

      /*m_d2dContext->DrawTextLayout(
         D2D1::Point2F(0.0f, 0.0f),
         m_textLayout.Get(),
         m_blackBrush.Get()
         );*/

      /*simple_graphics g;

      g.reference_os_data(m_d2dContext.Get());

      RECT rect;

      rect.left     = 200;
      rect.top      = 200;
      rect.right    = 300;
      rect.bottom   = 100;

      g.fill_solid_rect(rect, ARGB(127, 255, 255, 240));

      g.m_pdc = NULL;*/
      ::draw2d::graphics_sp dc(get_app()->allocer());
      dc->attach((ID2D1DeviceContext *)m_d2dContext.Get());

      sl.unlock();

      if (m_dib.is_null())
      {

         m_dib.alloc(m_pauraapp->allocer());

      }

      m_dib->create(1920, 1080);

      m_dib->Fill(0, 0, 0, 0);

      Sys(::aura::system::g_p).m_possystemwindow->m_pui->_000OnDraw(m_dib->get_graphics());

      dc->from(m_dib->get_size(), m_dib->get_graphics(), SRCCOPY);

      sl.lock();

      dc->detach();


      // We ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
      // is lost. It will be handled during the next call to Present.
      HRESULT hr = m_d2dContext->EndDraw();

      if(FAILED(hr))
      {

         if(hr == D2DERR_RECREATE_TARGET)
         {

            trace_hr("directx_base::Render, EndDraw", hr);

         }
         else if(hr == D2DERR_WRONG_STATE)
         {

            trace_hr("directx_base::Render, EndDraw",hr);

         }
         else
         {

            trace_hr("directx_base::Render, EndDraw",hr);

         }

      }

      return hr;

   }


} // namespace metrowin



