#include "framework.h"


// Windows Desktop Thread Local Storage for Draw2d Direct2d plugin

//#ifdef __cplusplus_winrt
//void my_debug()
//{
//}
//#else

#include <winapifamily.h>
#include <windows.h>
#include <shlwapi.h>
#include <wrl/client.h>
#include <D2d1_1.h>


#include <DXGI1_2.h>
#include <Dwrite.h>
#include <D3D11.h>
#include <D3D11_1.h>
#include <Dxgi1_3.h>     
#include <Initguid.h>
#include <DXGIDebug.h>
///void my_debug()
//{
//   Microsoft::WRL::ComPtr<IDXGIDebug1> d;
//   HRESULT hr =DXGIGetDebugInterface1(IID_IDXGIDebug1,&d);
//   if(SUCCEEDED(hr))
//   {
//      d->ReportLiveObjects(DXGI_DEBUG_ALL,DXGI_DEBUG_RLO_ALL);
//   }
//}
//#endif

//typedef GUID DXGI_DEBUG_ID;
//
//DEFINE_GUID(DXGI_DEBUG_ALL,0xe48ae283,0xda80,0x490b,0x87,0xe6,0x43,0xe9,0xa9,0xcf,0xda,0x8);
//DEFINE_GUID(DXGI_DEBUG_DX,0x35cdd7fc,0x13b2,0x421d,0xa5,0xd7,0x7e,0x44,0x51,0x28,0x7d,0x64);
//DEFINE_GUID(DXGI_DEBUG_DXGI,0x25cddaa4,0xb1c6,0x47e1,0xac,0x3e,0x98,0x87,0x5b,0x5a,0x2e,0x2a);
//DEFINE_GUID(DXGI_DEBUG_APP,0x6cd6e01,0x4219,0x4ebd,0x87,0x9,0x27,0xed,0x23,0x36,0xc,0x62);
//
//DEFINE_GUID(DXGI_DEBUG_D3D11,0x4b99317b,0xac39,0x4aa6,0xbb,0xb,0xba,0xa0,0x47,0x84,0x79,0x8f);
typedef HRESULT WINAPI FN_DXGIGetDebugInterface(
   REFIID riid,
   void **ppDebug
   );
typedef FN_DXGIGetDebugInterface * PFN_DXGIGetDebugInterface;
class dxgidebug
{
public:
   Microsoft::WRL::ComPtr<IDXGIDebug1> d;
   void init()
   {
      //HMODULE hmodule = GetModuleHandle("Dxgidebug.dll");
      //if (hmodule != NULL)
      {
        // PFN_DXGIGetDebugInterface pfn_DXGIGetDebugInterface = (PFN_DXGIGetDebugInterface) ::GetProcAddress(hmodule, "DXGIGetDebugInterface");
        // if (pfn_DXGIGetDebugInterface != NULL)
         {
            //HRESULT hr = pfn_DXGIGetDebugInterface(IID_IDXGIDebug, &d);
            HRESULT hr = DXGIGetDebugInterface1(0, IID_IDXGIDebug1, &d);
         }

      }

   }
   void debug()
   {
      if (d.Get() != NULL)
      {
         ::output_debug_string("MY_DEBUG : IDXGIDebug::ReportLiveObjects");
         d->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
      }
      else
      {
         init();
      }

   }
} *g_pdxgidebug;

void my_debug()
{
   g_pdxgidebug->debug();
}


//#endif



   namespace draw2d_direct2d
   {

      class plugin
      {
      public:

         Microsoft::WRL::ComPtr<IDWriteFactory> g_pwritefactory;
         Microsoft::WRL::ComPtr<ID2D1Factory1> g_pd2factory;
         Microsoft::WRL::ComPtr<ID2D1Device> g_pd2device;
         Microsoft::WRL::ComPtr<ID2D1DeviceContext> g_pd2devicecontext;
         Microsoft::WRL::ComPtr<ID3D11DeviceContext> g_pd3devicecontext;
         Microsoft::WRL::ComPtr<ID3D11DeviceContext1> g_pd3devicecontext1;
         Microsoft::WRL::ComPtr<ID3D11Device> g_pd3device;
         Microsoft::WRL::ComPtr<ID3D11Device1> g_pd3device1;
         Microsoft::WRL::ComPtr<IDXGIDevice> g_pdxgidevice;
         D3D_FEATURE_LEVEL g_featurelevel;
         void initialize();
      } * g_pplugin;

      void finalize()
      {
         delete g_pplugin;
         delete g_pdxgidebug;
      }

      void initialize()
      {
         g_pdxgidebug = new dxgidebug;
         g_pplugin = new plugin;
         g_pplugin->initialize();
      }

void plugin::initialize()
{
      // This flag adds support for surfaces with a different color channel ordering
      // than the API default. It is required for compatibility with Direct2D.
      UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
      // If the project is in a debug build, enable debugging via SDK Layers with this flag.
      creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

      // This array defines the set of DirectX hardware feature levels this app will support.
      // Note the ordering should be preserved.
      // Don't forget to declare your application's minimum required feature level in its
      // description.  All applications are assumed to support 9.1 unless otherwise stated.
      D3D_FEATURE_LEVEL featureLevels[] =
      {
         D3D_FEATURE_LEVEL_11_1,
         D3D_FEATURE_LEVEL_11_0,
         D3D_FEATURE_LEVEL_10_1,
         D3D_FEATURE_LEVEL_10_0,
         D3D_FEATURE_LEVEL_9_3,
         D3D_FEATURE_LEVEL_9_2,
         D3D_FEATURE_LEVEL_9_1
      };

      // Create the Direct3D 11 API device object and a corresponding context.
      Microsoft::WRL::ComPtr<ID3D11Device> device;
      Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
      ::draw2d_direct2d::throw_if_failed(
         D3D11CreateDevice(
         nullptr,                    // Specify nullptr to use the default adapter.
         D3D_DRIVER_TYPE_HARDWARE,
         0,
         creationFlags,              // Set debug and Direct2D compatibility flags.
         featureLevels,              // List of feature levels this app can support.
         ARRAYSIZE(featureLevels),
         D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Metro style apps.
         &device,                    // Returns the Direct3D device created.
         &g_featurelevel,            // Returns feature level of device created.
         &context                    // Returns the device immediate context.
         )
         );

      // Get the Direct3D 11.1 API device and context interfaces.
      ::draw2d_direct2d::throw_if_failed(
         device.As(&g_pd3device)
         );
      ::draw2d_direct2d::throw_if_failed(
         device.As(&g_pd3device1)
         );

      ::draw2d_direct2d::throw_if_failed(
         context.As(&g_pd3devicecontext)
         );
      ::draw2d_direct2d::throw_if_failed(
         context.As(&g_pd3devicecontext1)
         );

      // Get the underlying DXGI device of the Direct3D device.
      ::draw2d_direct2d::throw_if_failed(
         device.As(&g_pdxgidevice)
         );

      // Create the Direct2D device object and a corresponding context.
      ::draw2d_direct2d::throw_if_failed(
         GetD2D1Factory1()->CreateDevice(g_pdxgidevice.Get(), &g_pd2device)
         );

      //::draw2d_direct2d::throw_if_failed(
      //   g_pd2device->CreateDeviceContext(
      //   D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
      //   //D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
      //   &g_pd2devicecontext
      //   )
      //   );

      //ID2D1DeviceContext * pdevicecontext = g_pd2devicecontext.Get();

      //Sys(get_thread_app()).m_pdevicecontext    = pdevicecontext;
      //System.m_pmutexDc          = new mutex(get_app());


}

   } // namespace draw2d_direct2d

#define d2d1_fax_options D2D1_FACTORY_OPTIONS // fax of merde
#define d2d1_thread_model D2D1_FACTORY_TYPE_MULTI_THREADED // ???? muliple performance multi thread hidden option there exists cost uses?

CLASS_DECL_DRAW2D_DIRECT2D IDWriteFactory * TlsGetWriteFactory(bool bCreate)
{

   if(::draw2d_direct2d::g_pplugin->g_pwritefactory != NULL || !bCreate)
      return ::draw2d_direct2d::g_pplugin->g_pwritefactory.Get();

   HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &::draw2d_direct2d::g_pplugin->g_pwritefactory);

   if(FAILED(hr))
      return NULL;

   return ::draw2d_direct2d::g_pplugin->g_pwritefactory.Get();

}


ID2D1Factory1 * GetD2D1Factory1(bool bCreate)
{


   if(::draw2d_direct2d::g_pplugin->g_pd2factory != NULL || !bCreate)
      return ::draw2d_direct2d::g_pplugin->g_pd2factory.Get();

   d2d1_fax_options options;
   memset(&options, 0, sizeof(options));

   HRESULT hr = ::D2D1CreateFactory(d2d1_thread_model, __uuidof(ID2D1Factory1), &options, &::draw2d_direct2d::g_pplugin->g_pd2factory);

   if(FAILED(hr))
       return NULL;

   return ::draw2d_direct2d::g_pplugin->g_pd2factory.Get();

}


ID3D11Device * TlsGetD3D11Device()
{

   return ::draw2d_direct2d::g_pplugin->g_pd3device.Get();

}


ID3D11DeviceContext * TlsGetD3D11DeviceContext()
{

   return ::draw2d_direct2d::g_pplugin->g_pd3devicecontext.Get();

}

ID3D11DeviceContext1 * TlsGetD3D11DeviceContext1()
{

   return ::draw2d_direct2d::g_pplugin->g_pd3devicecontext1.Get();

}

ID3D11Device1 * TlsGetD3D11Device1()
{

   return ::draw2d_direct2d::g_pplugin->g_pd3device1.Get();

}

IDXGIDevice * TlsGetDXGIDevice()
{

   return ::draw2d_direct2d::g_pplugin->g_pdxgidevice.Get();

}
ID2D1DeviceContext * TlsGetD2D1DeviceContext()
{

   return ::draw2d_direct2d::g_pplugin->g_pd2devicecontext.Get();

}

ID2D1Device * TlsGetD2D1Device()
{

   return ::draw2d_direct2d::g_pplugin->g_pd2device.Get();

}

CLASS_DECL_DRAW2D_DIRECT2D float point_dpi(float points)
{

   FLOAT dpiX, dpiY;

   GetD2D1Factory1()->GetDesktopDpi(&dpiX, &dpiY);

	return points * dpiY / 72.f;

}

CLASS_DECL_DRAW2D_DIRECT2D float dpiy(float y)
{
   
   FLOAT dpiX, dpiY;

   GetD2D1Factory1()->GetDesktopDpi(&dpiX, &dpiY);

   return y * dpiY / 96.f;

}

CLASS_DECL_DRAW2D_DIRECT2D float dpix(float x)
{
   
   FLOAT dpiX, dpiY;

   GetD2D1Factory1()->GetDesktopDpi(&dpiX, &dpiY);

   return x * dpiX / 96.f;

}


CLASS_DECL_DRAW2D_DIRECT2D float y_dpi(float y)
{
   
   FLOAT dpiX, dpiY;

   GetD2D1Factory1()->GetDesktopDpi(&dpiX, &dpiY);

   return y / dpiY;

}

CLASS_DECL_DRAW2D_DIRECT2D float x_dpi(float x)
{
   
   FLOAT dpiX, dpiY;

   GetD2D1Factory1()->GetDesktopDpi(&dpiX, &dpiY);

   return x / dpiX;

}



