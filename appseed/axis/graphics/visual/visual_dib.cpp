//#include "framework.h"

#ifdef WINDOWS
#include <wincodec.h>
#endif

#include "freeimage/Source/FreeImage.h"

////#include "ft2build.h"


////#include FT_FREETYPE_H

bool windows_write_dib_to_file(::file::file_sp, ::draw2d::dib * pdib, ::visual::save_image * psaveimage, ::aura::application * papp);
bool windows_load_dib_from_file(::draw2d::dib * pdib, ::file::file_sp, ::aura::application * papp);


namespace visual
{

   dib_sp::pointer::pointer()
   {

      m_dwTime = 0;

   }

   dib_sp::pointer::~pointer()
   {


   }


   dib_sp::array::array(::aura::application * papp) :
      object(papp)
   {

      m_uiLoop = 0;
      m_uiLoopCount = 0;
      m_bStart = false;
      m_iLastFrame = -1;

   }

   dib_sp::array::~array()
   {

   }




   dib_sp::dib_sp()
   {
      m_eload = load_none;
      m_dSpeed = 1.0;


   }


   dib_sp::dib_sp(const ::aura::allocatorsp & allocer) :
      ::draw2d::dib_sp(allocer)
   {
      m_eload = load_none;
      m_dSpeed = 1.0;


   }


   dib_sp::~dib_sp()
   {
      if (m_sparray.is_set())
      {

         if (m_sparray->pred_find_first([&](auto & i) { return i->m_dib.m_p == m_p; }) >= 0)
         {
            m_p = NULL;
         }

      }
   }

   bool dib_sp::load_thumbnail(var varFile, int w, int h)
   {

#ifdef WINDOWS

      if (!m_p->create(w, h))
      {
         return false;
      }

      if (!m_p->load_thumbnail(varFile))
      {

         return false;

      }


      return true;

#else

      todo(get_app());
      
      return false;

#endif


   }

   bool dib_sp::load_from_file(var varFile, bool bCache, bool bCreateHelperMaps)
   {

      if (varFile.is_empty())
      {

         m_eload = load_fail;

         return false;

      }

      if (varFile.get_file_path().extension().compare_ci("svg") == 0
         || varFile.get_file_path().find_ci(".svg?") > 0)
      {

         m_p->create_nanosvg(App(m_p->m_pauraapp).file().as_string(varFile));

         m_eload = load_ok;

         if (bCreateHelperMaps)
         {

            m_p->create_helper_map();

         }

         return true;

      }

      if (varFile.get_file_path().extension().compare_ci("gif") == 0
         || varFile.get_file_path().find_ci(".gif?") > 0)
      {

         m_sparray = canew(array(m_p->m_pauraapp));

         if (!Sys(m_p->m_pauraapp).visual().imaging().load_from_file(m_sparray, varFile, bCache))
         {

            m_sparray.release();

            m_eload = load_fail;

            return false;

         }

         if (!m_p->create(m_sparray->m_size))
         {

            m_sparray.release();

            m_eload = load_fail;

            return false;

         }

         if (bCreateHelperMaps)
         {

            for (auto & p : *m_sparray)
            {

               p->m_dib->create_helper_map();

            }

         }


         m_eload = load_ok;

         return true;

      }

      if (!Sys(m_p->m_pauraapp).visual().imaging().load_from_file(m_p, varFile, bCache))
      {

         m_eload = load_fail;

         return false;

      }

      if (bCreateHelperMaps)
      {

         m_p->create_helper_map();

      }

      m_eload = load_ok;

      return true;

   }


   bool dib_sp::load_from_matter(const char * pszMatter, bool bCache, bool bCreateHelperMaps)
   {

      ::file::path path = m_p->m_pauraapp->m_paxisapp->dir().matter(pszMatter);

      if (!Sys(m_p->m_pauraapp).visual().imaging().load_from_file(m_p, path, bCache))
      {

         m_eload = load_fail;

         return false;

      }

      if (bCreateHelperMaps)
      {

         m_p->create_helper_map();

      }

      m_eload = load_ok;

      return true;

   }


   bool dib_sp::read_from_file(::file::file_sp spfile)
   {

      if (!Sys(m_p->m_pauraapp).visual().imaging().LoadImageFromFile(m_p, spfile))
      {

         m_eload = load_fail;

         return false;

      }

      m_eload = load_ok;

      return true;

   }


   bool dib_sp::save_to_file(var varFile, save_image * psaveimage)
   {
      ::file::file_sp spfile;
      spfile = Sess(m_p->m_pauraapp).file().get_file(varFile, ::file::mode_create | ::file::share_deny_write | ::file::mode_write | ::file::type_binary | ::file::defer_create_directory);
      if (spfile.is_null())
         return false;
      return write_to_file(spfile, psaveimage);
   }

   bool dib_sp::write_to_file(::file::file_sp pfile, save_image * psaveimage)
   {
      save_image saveimageDefault;
      if (psaveimage == NULL)
         psaveimage = &saveimageDefault;
//#ifdef WINDOWS

  //    return windows_write_dib_to_file(pfile, m_p, psaveimage, m_p->get_app());

//#else


      bool bOk = false;

      bool b8 = false;
      bool b24 = false;
      int iFreeImageSave = 0;
      FREE_IMAGE_FORMAT eformat = (FREE_IMAGE_FORMAT)0;
      string strFile;
      switch (psaveimage->m_eformat)
      {
      case ::visual::image::format_png:
         eformat = FreeImage_GetFIFFromFormat("PNG");
         strFile = "foo.png";
         break;
      case ::visual::image::format_bmp:
         eformat = FIF_BMP;
         strFile = "foo.bmp";
         break;
      case ::visual::image::format_gif:
         b8 = true;
         eformat = FIF_GIF;
         strFile = "foo.gif";
         break;
      case ::visual::image::format_jpeg:
         b24 = true;
         eformat = FreeImage_GetFIFFromFormat("JPEG");
         strFile = "foo.jpg";
         if (psaveimage->m_iQuality > 80)
         {
            iFreeImageSave |= JPEG_QUALITYSUPERB;
         }
         else if (psaveimage->m_iQuality > 67)
         {
            iFreeImageSave |= JPEG_QUALITYGOOD;
         }
         else if (psaveimage->m_iQuality > 33)
         {
            iFreeImageSave |= JPEG_QUALITYNORMAL;
         }
         else if (psaveimage->m_iQuality > 15)
         {
            iFreeImageSave |= JPEG_QUALITYAVERAGE;
         }
         else
         {
            iFreeImageSave |= JPEG_QUALITYBAD;
         }
         break;
      default:
         return false;
      }

      eformat = FreeImage_GetFIFFromFilename(strFile);


      FIMEMORY * pfm1 = FreeImage_OpenMemory();
      FIBITMAP * pfi7 = Sys(m_p->m_pauraapp).visual().imaging().dib_to_FI(m_p);
      FIBITMAP * pfi8 = NULL;
      bool bConv;
      if (b8)
      {
         pfi8 = FreeImage_ConvertTo8Bits(pfi7);
         bConv = true;
      }
      else if (b24)
      {
         pfi8 = FreeImage_ConvertTo24Bits(pfi7);
         bConv = true;
      }
      else
      {
         //FreeImage_SetTransparent(pfi8,true);
         pfi8 = pfi7;
         bConv = false;
      }

      bOk = FreeImage_SaveToMemory(eformat, pfi8, pfm1, iFreeImageSave) != FALSE;

      BYTE * pbData = NULL;
      DWORD dwSize = 0;
      if (bOk)
         bOk = FreeImage_AcquireMemory(pfm1, &pbData, &dwSize) != FALSE;
      if (bOk)
      {
         try
         {
            pfile->write(pbData, dwSize);
         }
         catch (...)
         {
            bOk = false;
         }
      }

      FreeImage_CloseMemory(pfm1);
      if (bConv)
      {
         FreeImage_Unload(pfi8);
      }
      FreeImage_Unload(pfi7);



      return bOk != FALSE;

//#endif

   }




   save_image::save_image()
   {
      m_eformat = ::visual::image::format_png;
      m_iQuality = 100;
   }


#ifndef  WINDOWS

   bool dib_sp::from(class draw2d::graphics * pgraphics, struct FIBITMAP * pfi, bool bUnload)
   {

      return Sys(m_p->m_pauraapp).visual().imaging().from(m_p, pgraphics, pfi, bUnload);

   }

#endif

   /*::size dib_sp::size()
   {

      if (m_sparray.is_set() && m_sparray->get_count() > 0 && m_sparray->m_dwTotal > 0)
      {

         return m_sparray->m_size;

      }

      return m_p->size();

   }*/

   void dib_sp::defer_update()
   {
      if (m_sparray.is_set() && m_sparray->get_count() > 0 && m_sparray->m_dwTotal > 0)
      {

         //if (!m_p->create(m_sparray->m_size))
         //{

           // return false;

         //}
         if (!m_sparray->m_bStart)
         {
            m_sparray->m_bStart = true;

            m_sparray->m_dwStart = get_tick_count();
         }

         DWORD dwTime = (DWORD) (((double)(get_tick_count()) - m_sparray->m_dwStart)  * m_dSpeed);

         UINT uiLoop = dwTime / m_sparray->m_dwTotal;

         if (uiLoop > m_sparray->m_uiLoop)
         {

            m_sparray->m_uiLoop = uiLoop;

            m_sparray->m_iLastFrame = -1;

         }

         index iCurrentFrame = 0;

         if (m_sparray->m_uiLoopCount == 0 || m_sparray->m_uiLoop < m_sparray->m_uiLoopCount)
         {

            dwTime %= m_sparray->m_dwTotal;

            DWORD dwT = 0;

            for (index i = 0; i < m_sparray->get_count(); i++)
            {

               dwT += m_sparray->element_at(i)->m_dwTime;

               if (dwTime < dwT)
               {

                  iCurrentFrame = i;

                  break;

               }

            }

         }

         /*    for (index iFrame = m_sparray->m_iLastFrame + 1; iFrame <= iCurrentFrame; iFrame++)
             {

                if (m_sparray->element_at(iFrame).m_dwTime <= 0)
                {
                   output_debug_string("0 delay frame");
                }
                else
                {

                   dispose_current_frame();
                   overlay_frame(iFrame);
                }

                m_sparray->m_iLastFrame = iFrame;

             }


    */

         if (m_sparray->m_iLastFrame != iCurrentFrame)
         {

            m_sparray->m_iLastFrame = iCurrentFrame;

            m_p = m_sparray->element_at(iCurrentFrame)->m_dib.m_p;

         }

      }

   }

   bool dib_sp::pointer::to(::draw2d::graphics * pgraphics)
   {


      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

      if (!m_dib->to(pgraphics, m_rect.top_left(), m_rect.size()))
      {

         return false;

      }

      return true;

   }

   bool dib_sp::dispose_current_frame()
   {

      if (m_sparray->m_dibCompose.is_null())
      {

         m_sparray->m_dibCompose.alloc(m_p->allocer());

      }

      m_sparray->m_dibCompose->create(m_sparray->m_size);

      if (m_sparray->m_iLastFrame < 0)
      {

         // Draw background and increase loop count
         m_sparray->m_dibCompose->Fill(m_sparray->m_crBack);

         return false;

      }

      bool bOk = true;


      switch (m_sparray->element_at(m_sparray->m_iLastFrame)->m_edisposal)
      {
      case dib_sp::pointer::disposal_undefined:
      case dib_sp::pointer::disposal_none:
         // We simply draw on the previous frames. Do nothing here.
         break;
      case dib_sp::pointer::disposal_background:
      {
         // Dispose background
         // Clear the area covered by the current raw frame with background color
         m_sparray->m_dibCompose->get_graphics()->set_alpha_mode(::draw2d::alpha_mode_set);
         COLORREF crBack = m_sparray->m_crBack;
         byte bAlpha = argb_get_a_value(crBack);

         m_sparray->m_dibCompose->get_graphics()->FillSolidRect(m_sparray->element_at(m_sparray->m_iLastFrame)->m_rect, crBack);
      }
      break;
      case dib_sp::pointer::disposal_previous:
         // Dispose previous
         // We restore the previous composed frame first
         //hr = RestoreSavedFrame();
         break;
      default:
         // Invalid disposal method
         return false;
      }

      return true;
   }

   bool dib_sp::overlay_frame(int iFrame)
   {

      //// If starting a new animation loop
      //if (iFrame <= 0)
      //{

      //}

      // Produce the next frame
      if (!m_sparray->element_at(iFrame)->to(m_sparray->m_dibCompose->get_graphics()))
      {

         return false;

      }

      m_p->get_graphics()->set_alpha_mode(::draw2d::alpha_mode_set);

      m_p->get_graphics()->BitBlt(0, 0, m_p->m_size.cx, m_p->m_size.cy,
         m_sparray->m_dibCompose->get_graphics());




      return true;

   }


} // namespace visual


#if 0


CLASS_DECL_AXIS void draw_freetype_bitmap(::draw2d::dib * m_p, int32_t dx, int32_t dy, void * pftbitmap, int xParam, int yParam)
{

   FT_Bitmap * bitmap = (FT_Bitmap *)pftbitmap;
   FT_Int x = (FT_Int)xParam;
   FT_Int y = (FT_Int)yParam;

   FT_Int  i, j, p, q;
   FT_Int  x_max = x + bitmap->width;
   FT_Int  y_max = y + bitmap->rows;

   m_p->map();

   for (i = x, p = 0; i < x_max; i++, p++)
   {
      for (j = y, q = 0; j < y_max; j++, q++)
      {
         if (i < 0 || j < 0 || i >= m_p->m_size.cx || j >= m_p->m_size.cy)
            continue;

         int32_t a = bitmap->buffer[q * bitmap->width + p];

         *((COLORREF *)&((byte *)m_p->get_data())[(dy + j) * m_p->m_iScan + (dx + i) * 4]) = ARGB(a, 0, 0, 0);

      }
   }

}


CLASS_DECL_AXIS void draw_freetype_bitmap(::draw2d::dib * m_p, int32_t dx, int32_t dy, void * pftbitmap, int xParam, int yParam, byte aParam, byte r, byte g, byte b)
{

   FT_Bitmap * bitmap = (FT_Bitmap *)pftbitmap;
   FT_Int x = (FT_Int)xParam;
   FT_Int y = (FT_Int)yParam;

   FT_Int  i, j, p, q;
   FT_Int  x_max = x + bitmap->width;
   FT_Int  y_max = y + bitmap->rows;

   m_p->map();

   for (i = x, p = 0; i < x_max; i++, p++)
   {
      for (j = y, q = 0; j < y_max; j++, q++)
      {
         if (i < 0 || j < 0 || i >= m_p->m_size.cx || j >= m_p->m_size.cy)
            continue;

         int32_t a = bitmap->buffer[q * bitmap->width + p];

         if (a > 0)
         {

            *((COLORREF *)&((byte *)m_p->get_data())[(dy + j) * m_p->m_iScan + (dx + i) * 4]) = ARGB(a * aParam / 255, r, g, b);

         }
         else
         {

            *((COLORREF *)&((byte *)m_p->get_data())[(dy + j) * m_p->m_iScan + (dx + i) * 4]) = 0;

         }

      }
   }

}

#endif



#ifdef WINDOWS

/******************************************************************
*                                                                 *
*  DemoApp::GetBackgroundColor()                                  *
*                                                                 *
*  Reads and stores the background color for gif.                 *
*                                                                 *
******************************************************************/


bool windows_load_dib_from_frame(
   comptr< IWICBitmapFrameDecode> & pframe,
   ::draw2d::dib * pdib,
   IWICImagingFactory * piFactory,
   IWICBitmapDecoder * piDecoder,
   int iFrame)
{

   try
   {

      HRESULT hr = piDecoder->GetFrame(iFrame, &pframe.get());

      WICPixelFormatGUID px;
      ZERO(px);
      if (pframe == NULL)
      {
         return false;
      }
      if (FAILED(hr))
      {
         return false;
      }

      hr = pframe->GetPixelFormat(&px);
      if (FAILED(hr))
      {
         return false;
      }



      if (px == GUID_WICPixelFormat32bppBGRA)
      {
         UINT width = 0;
         UINT height = 0;

         pframe->GetSize(&width, &height);
         pdib->create(width, height);
         pdib->map();
         hr = pframe->CopyPixels(NULL, pdib->m_iScan, pdib->m_iScan * height, (BYTE *)pdib->m_pcolorref);
#ifdef METROWIN
         pdib->mult_alpha();
#endif

      }
      else
      {

         comptr<IWICFormatConverter> pbitmap;

         if (SUCCEEDED(hr))
         {

            hr = piFactory->CreateFormatConverter(&pbitmap.get());

         }



         px = GUID_WICPixelFormat32bppBGRA;

         if (SUCCEEDED(hr))
         {

            hr = pbitmap->Initialize(pframe, px, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeCustom);
         }

         //Step 4: Create render target and D2D bitmap from IWICBitmapSource
         UINT width = 0;
         UINT height = 0;
         if (SUCCEEDED(hr))
         {
            hr = pbitmap->GetSize(&width, &height);
         }

         pdib->create(width, height);

         pdib->map();

         hr = pbitmap->CopyPixels(NULL, pdib->m_iScan, pdib->m_iScan * height, (BYTE *)pdib->m_pcolorref);


      }



   }
   catch (...)
   {
      return false;
   }

   return true;

}

HRESULT windows_GetBackgroundColor(::visual::dib_sp::array * pdiba,

   IWICImagingFactory * piFactory, IWICBitmapDecoder * piDecoder, IWICMetadataQueryReader *pMetadataQueryReader)
{
   DWORD dwBGColor;
   WICColor rgColors[256];
   UINT cColorsCopied = 0;
   PROPVARIANT propVariant;
   PropVariantInit(&propVariant);
   comptr <IWICPalette> pWicPalette;

   pdiba->m_backgroundIndex = 0;

   // If we have a global palette, get the palette and background color
   HRESULT hr = pMetadataQueryReader->GetMetadataByName(
      L"/logscrdesc/GlobalColorTableFlag",
      &propVariant);
   if (SUCCEEDED(hr))
   {
      hr = (propVariant.vt != VT_BOOL || !propVariant.boolVal) ? E_FAIL : S_OK;
      PropVariantClear(&propVariant);
   }

   if (SUCCEEDED(hr))
   {
      // Background color index
      hr = pMetadataQueryReader->GetMetadataByName(
         L"/logscrdesc/BackgroundColorIndex",
         &propVariant);
      if (SUCCEEDED(hr))
      {
         hr = (propVariant.vt != VT_UI1) ? E_FAIL : S_OK;
         if (SUCCEEDED(hr))
         {
            pdiba->m_backgroundIndex = propVariant.bVal;
         }
         PropVariantClear(&propVariant);
      }
   }

   // Get the color from the palette
   if (SUCCEEDED(hr))
   {
      hr = piFactory->CreatePalette(&pWicPalette.get());
   }

   if (SUCCEEDED(hr))
   {
      // Get the global palette
      hr = piDecoder->CopyPalette(pWicPalette);
   }


   if (SUCCEEDED(hr))
   {
      hr = pWicPalette->GetColors(
         ARRAYSIZE(rgColors),
         rgColors,
         &cColorsCopied);
   }

   if (SUCCEEDED(hr))
   {
      pdiba->m_cra.set_size(cColorsCopied);
      for (UINT ui = 0; ui < cColorsCopied; ui++)
      {
         pdiba->m_cra[ui] = rgColors[ui];
      }
      // Check whether background color is outside range
      hr = (pdiba->m_backgroundIndex >= cColorsCopied) ? E_FAIL : S_OK;
   }

   if (SUCCEEDED(hr))
   {

      dwBGColor = pdiba->m_cra[pdiba->m_backgroundIndex];

      pdiba->m_crBack = dwBGColor;

   }

   return hr;

}


#define unequal(a, b, n) ((a - n < b && b < a + n) || (b - n < a && a < b + n))


HRESULT windows_GetRawFrame(
   ::draw2d::dib * pdibCompose,
   ::visual::dib_sp::array * pdiba,
   IWICImagingFactory * piFactory,
   IWICBitmapDecoder * piDecoder,
   UINT uFrameIndex)
{

   sp(::visual::dib_sp::pointer) pointer = pdiba->element_at(uFrameIndex);

   comptr < IWICBitmapFrameDecode >    pframe;

   comptr < IWICMetadataQueryReader >  pFrameMetadataQueryReader;

   PROPVARIANT propValue;

   PropVariantInit(&propValue);

   // Retrieve the current frame
   HRESULT hr = piDecoder->GetFrame(uFrameIndex, &pframe.get());

   comptr < IWICFormatConverter > pbitmap;

   if (SUCCEEDED(hr))
   {

      hr = piFactory->CreateFormatConverter(&pbitmap.get());

   }

   if (SUCCEEDED(hr))
   {

      hr = pbitmap->Initialize(
         pframe,
         GUID_WICPixelFormat8bppIndexed,
         WICBitmapDitherTypeNone,
         nullptr,
         0.f,
         WICBitmapPaletteTypeCustom);

   }

   UINT width = 0;

   UINT height = 0;

   if (SUCCEEDED(hr))
   {

      hr = pbitmap->GetSize(&width, &height);

   }

   pointer->m_dib->create(width, height);

   pointer->m_dib->map();

   byte_array ba;

   ba.allocate((memory_size_t) pointer->m_dib->area());

   hr = pbitmap->CopyPixels(NULL, pointer->m_dib->m_size.cx, (UINT) ba.get_size(), (BYTE *)ba.get_data());

   if (FAILED(hr))
   {

      return false;

   }

   hr = pframe->GetMetadataQueryReader(&pFrameMetadataQueryReader.get());

   if (SUCCEEDED(hr))
   {

      hr = pFrameMetadataQueryReader->GetMetadataByName(L"/imgdesc/Left", &propValue);

      if (SUCCEEDED(hr))
      {

         hr = (propValue.vt == VT_UI2 ? S_OK : E_FAIL);

         if (SUCCEEDED(hr))
         {

            pointer->m_rect.left = propValue.uiVal;

         }

         PropVariantClear(&propValue);

      }

   }

   if (SUCCEEDED(hr))
   {

      hr = pFrameMetadataQueryReader->GetMetadataByName(L"/imgdesc/Top", &propValue);

      if (SUCCEEDED(hr))
      {

         hr = (propValue.vt == VT_UI2 ? S_OK : E_FAIL);

         if (SUCCEEDED(hr))
         {

            pointer->m_rect.top = propValue.uiVal;

         }

         PropVariantClear(&propValue);

      }

   }

   if (SUCCEEDED(hr))
   {

      hr = pFrameMetadataQueryReader->GetMetadataByName(L"/imgdesc/Width", &propValue);

      if (SUCCEEDED(hr))
      {

         hr = (propValue.vt == VT_UI2 ? S_OK : E_FAIL);

         if (SUCCEEDED(hr))
         {

            pointer->m_rect.right = propValue.uiVal + pointer->m_rect.left;

         }

         PropVariantClear(&propValue);

      }

   }

   if (SUCCEEDED(hr))
   {

      hr = pFrameMetadataQueryReader->GetMetadataByName(L"/imgdesc/Height", &propValue);

      if (SUCCEEDED(hr))
      {

         hr = (propValue.vt == VT_UI2 ? S_OK : E_FAIL);

         if (SUCCEEDED(hr))
         {

            pointer->m_rect.bottom = propValue.uiVal + pointer->m_rect.top;

         }

         PropVariantClear(&propValue);

      }

   }

   if (SUCCEEDED(hr))
   {

      // Get delay from the optional Graphic Control Extension
      if (SUCCEEDED(pFrameMetadataQueryReader->GetMetadataByName(
         L"/grctlext/Delay",
         &propValue)))
      {

         hr = (propValue.vt == VT_UI2 ? S_OK : E_FAIL);

         if (SUCCEEDED(hr))
         {

            UINT ui;

            // Convert the delay retrieved in 10 ms units to a delay in 1 ms units
            hr = UIntMult(propValue.uiVal, 10, &ui);

            pointer->m_dwTime = ui;

         }

         PropVariantClear(&propValue);

      }
      else
      {

         // Failed to get delay from graphic control extension. Possibly a
         // single frame image (non-animated gif)
         pointer->m_dwTime = 0;

      }

      if (pointer->m_dwTime <= 0)
      {

         output_debug_string("0 delay");

      }

      if (SUCCEEDED(hr))
      {
         // Insert an artificial delay to ensure rendering for gif with very small
         // or 0 delay.  This delay number is picked to match with most browsers'
         // gif display speed.
         //
         // This will defeat the purpose of using zero delay intermediate frames in
         // order to preserve compatibility. If this is removed, the zero delay
         // intermediate frames will not be visible.
         //if (pointer.m_dwTime < 90)
         {
            // pointer.m_dwTime = 90;
         }

      }

   }

   if (SUCCEEDED(hr))
   {

      if (SUCCEEDED(pFrameMetadataQueryReader->GetMetadataByName(
         L"/grctlext/Disposal",
         &propValue)))
      {

         hr = (propValue.vt == VT_UI1) ? S_OK : E_FAIL;

         if (SUCCEEDED(hr))
         {

            pointer->m_edisposal = (::visual::dib_sp::pointer::e_disposal) propValue.bVal;

         }

      }
      else
      {

         // Failed to get the disposal method, use default. Possibly a
         // non-animated gif.
         pointer->m_edisposal = ::visual::dib_sp::pointer::disposal_undefined;

      }

   }

   BYTE transparentIndex = 0;

   hr = S_OK;

   if (SUCCEEDED(hr))
   {

      hr = pFrameMetadataQueryReader->GetMetadataByName(
         L"/grctlext/TransparencyFlag",
         &propValue);

      if (SUCCEEDED(hr))
      {

         hr = (propValue.vt != VT_BOOL) ? E_FAIL : S_OK;

         if (SUCCEEDED(hr))
         {

            pointer->m_bTransparent = propValue.boolVal != FALSE;

         }

         PropVariantClear(&propValue);

      }

   }

   if (pointer->m_bTransparent)
   {

      if (SUCCEEDED(hr))
      {

         hr = pFrameMetadataQueryReader->GetMetadataByName(
            L"/grctlext/TransparentColorIndex",
            &propValue);

         if (SUCCEEDED(hr))
         {

            hr = (propValue.vt != VT_UI1) ? E_FAIL : S_OK;

            if (SUCCEEDED(hr))
            {

               transparentIndex = propValue.bVal;

            }

            PropVariantClear(&propValue);

         }

      }

   }

   if (SUCCEEDED(hr))
   {

      hr = (transparentIndex >= pdiba->m_cra.get_count()) ? E_FAIL : S_OK;

   }

   if (SUCCEEDED(hr))
   {

      pointer->m_crTransparent = pdiba->m_cra[transparentIndex];

   }
   else
   {

      pointer->m_crTransparent = ARGB(255, 255, 255, 255);

   }

   bool bTransparent;

   COLORREF cr;

   COLORREF crBack = 0;

   if (pointer->m_rect.area() == pdiba->m_size.area())
   {

      ::count c = 0;

      int64_t iR = 0;

      int64_t iG = 0;

      int64_t iB = 0;

      int iLight = 0;

      int iDark = 0;

      // Roughly detect colors on transparency borders...

      // ... first, at horizontal orientation...

      for (index y = 0; y < pointer->m_dib->m_size.cy; y++)
      {

         bTransparent = true;

         cr = 0;

         for (index x = 0; x < pointer->m_dib->m_size.cx; x++)
         {

            index iIndex = ba[x + y * pointer->m_dib->m_size.cx];

            index iNextIndex = -1;

            if (x < pointer->m_dib->m_size.cx - 1)
            {

               iNextIndex = ba[x + y * pointer->m_dib->m_size.cx + 1];

            }

            if (iIndex >= pdiba->m_cra.get_count())
            {

               continue;

            }

            if (bTransparent)
            {

               if (iIndex == transparentIndex)
               {

                  pdiba->m_bTransparent = true;

                  continue;

               }
               else
               {

                  cr = pdiba->m_cra[iIndex];

                  bTransparent = false;

               }

            }
            else
            {

               if (iNextIndex == transparentIndex)
               {

                  cr = pdiba->m_cra[iIndex];

                  bTransparent = true;

               }
               else
               {

                  cr = pdiba->m_cra[iIndex];

                  continue;

               }

            }

            iR += argb_get_r_value(cr);

            iG += argb_get_g_value(cr);

            iB += argb_get_b_value(cr);
            c++;

         }

      }

      // ... then, at vertical orientation...

      for (index x = 0; x < pointer->m_dib->m_size.cx; x++)
      {

         bTransparent = true;

         cr = 0;

         for (index y = 0; y < pointer->m_dib->m_size.cy; y++)
         {

            index iIndex = ba[x + y * pointer->m_dib->m_size.cx];

            index iNextIndex = -1;

            if (y < pointer->m_dib->m_size.cy - 1)
            {

               iNextIndex = ba[x + (y + 1) * pointer->m_dib->m_size.cx];

            }

            if (iIndex >= pdiba->m_cra.get_count())
            {

               continue;

            }

            if (bTransparent)
            {

               if (iIndex == transparentIndex)
               {

                  continue;

               }
               else
               {

                  cr = pdiba->m_cra[iIndex];

                  bTransparent = false;

               }

            }
            else
            {

               if (iNextIndex == transparentIndex)
               {

                  cr = pdiba->m_cra[iIndex];

                  bTransparent = true;

               }
               else
               {

                  cr = pdiba->m_cra[iIndex];

                  continue;

               }

            }

            iR += argb_get_r_value(cr);

            iG += argb_get_g_value(cr);

            iB += argb_get_b_value(cr);

            c++;

         }

      }

      // and if detected transparency, roughly calculate if average border color is dark or light.

      if (c <= 0)
      {

         crBack = ARGB(255, 255, 255, 255);

         if (uFrameIndex <= 0)
         {

            pdiba->m_bTransparent = false;

         }

      }
      else
      {

         double dMin = (double) MIN(MIN(iR / c, iG / c), iB / c);

         double dMax = (double)MAX(MAX(iR / c, iG / c), iB / c);

         if ((dMin + dMax) / 2.0 > 127.0) // Light
         {

            crBack = ARGB(255, 255, 255, 255);

         }
         else
         {

            crBack = ARGB(255, 0, 0, 0);

         }

         if (uFrameIndex <= 0)
         {

            pdiba->m_bTransparent = true;

         }

      }

      pdiba->m_crTransparent = crBack;

      //pdiba->m_iTransparentIndex = transparentIndex;

   }



   if (pdiba->m_bTransparent)
   {


      if (pointer->m_bTransparent)
      {

         crBack = pointer->m_crTransparent;

         pdiba->m_iTransparentIndex = transparentIndex;

         pdiba->m_bTransparent = true;

      }
      else
      {

         crBack = pointer->m_crTransparent;

         size s = pointer->m_dib->m_size;

         int cx = s.cx;

         int cy = s.cy;

         if (pointer->m_rect.size() == pdiba->m_size)
         {

            if (ba[0] == ba[cx - 1]) // && pointer->m_rect.left == 0 && pointer->m_rect.top == 0 && pointer->m_rect.right == pdiba->m_size.cx)
            {

               transparentIndex = ba[0];

               pdiba->m_iTransparentIndex = transparentIndex;

            }
            else if (ba[cx * cy - 1] == ba[cx * (cy - 1)])// && pointer->m_rect.left == 0 && pointer->m_rect.bottom == pdiba->m_size.cy && pointer->m_rect.right == pdiba->m_size.cx)
            {

               transparentIndex = ba[cx * cy - 1];

               pdiba->m_iTransparentIndex = transparentIndex;

            }
            else
            {

               transparentIndex = pdiba->m_iTransparentIndex;

            }

         }
         else
         {

            transparentIndex = pdiba->m_iTransparentIndex;

         }

      }

      double dA1 = argb_get_a_value(crBack);

      double dR1 = argb_get_r_value(crBack);

      double dG1 = argb_get_g_value(crBack);

      double dB1 = argb_get_b_value(crBack);

      bool bUnequal = true;

      double dUnequal = 127.0;

      double dUnequalScaleDown = 1.00;

      double dUnequalRate = 1.00;

      int w = pointer->m_dib->m_iScan / sizeof(COLORREF);

      int i = 0;

      index x;

      index y;

      visual::fastblur f(pointer->m_dib->allocer());

      {

         f->create(pdibCompose->m_size);

         f->channel_copy(::visual::rgba::channel_red, ::visual::rgba::channel_alpha, pdibCompose);

      }

      for (y = 0; y < pointer->m_dib->m_size.cy; y++)
      {

         bTransparent = pointer->m_rect.left == 0; // borders transparent?

         for (x = 0; x < pointer->m_dib->m_size.cx; x++)
         {

            index iIndex = ba[y * pointer->m_dib->m_size.cx + x];

            index iPixel = y * w + x;

            if (iIndex >= pdiba->m_cra.get_count())
            {

               pointer->m_dib->m_pcolorref[iPixel] = 0;

               continue;

            }

            cr = pdiba->m_cra[iIndex];

            double dA = argb_get_a_value(cr);

            if (dA == 0.0)
            {
               output_debug_string("zero alpha");
            }
            else if (dA == 255.0)
            {
               output_debug_string("opaque");
            }
            else
            {
               output_debug_string("translucent!!!");
            }

            double dR = argb_get_r_value(cr);

            double dG = argb_get_g_value(cr);

            double dB = argb_get_b_value(cr);

            if (iIndex == transparentIndex)
            {

               bTransparent = true;

               pointer->m_dib->m_pcolorref[iPixel] = 0;

               continue;

            }

            if (bUnequal && bTransparent)
            {

               if (unequal(dR, dR1, dUnequal) && unequal(dG, dG1, dUnequal) && unequal(dB, dB1, dUnequal))
               {

                  double dA2;

                  if (dR1 < 127)
                  {

                     dA2 = MIN(dR, MIN(dG, dB)) * dUnequalRate;

                  }
                  else
                  {

                     dA2 = (255.0 - MAX(dR, MAX(dG, dB))) * dUnequalRate;

                  }

                  f->m_pcolorref[iPixel] = (byte)(dA * dA2 / 255.0);
                  pointer->m_dib->m_pcolorref[iPixel] = ARGB(
                     (byte)255,
                     (byte)dR,
                     (byte)dG,
                     (byte)dB);

                  continue;

               }
               else
               {

                  bTransparent = false;

               }

            }
            else
            {

               bTransparent = false;

            }

            f->m_pcolorref[iPixel] = (byte)(dA);
            pointer->m_dib->m_pcolorref[iPixel] = ARGB((byte)255, (byte)dR, (byte)dG, (byte)dB);

         }

         bTransparent = pointer->m_rect.right == pdiba->m_size.cx; // borders transparent?

         x--;

         for (; x >= 0; x--)
         {

            index iPixel = y * w + x;

            cr = pointer->m_dib->m_pcolorref[iPixel];

            double dA = argb_get_a_value(cr);

            double dR = argb_get_r_value(cr);

            double dG = argb_get_g_value(cr);

            double dB = argb_get_b_value(cr);

            if (dA == 0.0)
            {

               bTransparent = true;

               continue;

            }

            if (bUnequal && bTransparent)
            {

               if (unequal(dR, dR1, dUnequal) && unequal(dG, dG1, dUnequal) && unequal(dB, dB1, dUnequal))
               {

                  double dA2;

                  if (dR1 < 127)
                  {

                     dA2 = MIN(dR, MIN(dG, dB)) * dUnequalRate;

                  }
                  else
                  {

                     dA2 = (255.0 - MAX(dR, MAX(dG, dB))) * dUnequalRate;

                  }

                  f->m_pcolorref[iPixel] = (byte)(dA * dA2 / 255.0);
                  pointer->m_dib->m_pcolorref[iPixel] = ARGB(
                     (byte)255,
                     (byte)dR,
                     (byte)dG,
                     (byte)dB);

                  continue;

               }
               else
               {

                  bTransparent = false;

               }

            }
            else
            {

               bTransparent = false;

            }

            f->m_pcolorref[iPixel] = (byte)(dA);
            pointer->m_dib->m_pcolorref[iPixel] = ARGB((byte)255, (byte)dR, (byte)dG, (byte)dB);

         }

      }


      dUnequal *= dUnequalScaleDown;


      for (x = 0; x < pointer->m_dib->m_size.cx; x++)
      {

         bTransparent = pointer->m_rect.top == 0; // borders transparent?

         for (y = 0; y < pointer->m_dib->m_size.cy; y++)
         {

            index iPixel = y * w + x;

            cr = pointer->m_dib->m_pcolorref[iPixel];

            double dA = argb_get_a_value(cr);

            double dR = argb_get_r_value(cr);

            double dG = argb_get_g_value(cr);

            double dB = argb_get_b_value(cr);

            if (dA == 0.0)
            {

               bTransparent = true;

               continue;

            }

            if (bUnequal && bTransparent)
            {

               if (unequal(dR, dR1, dUnequal) && unequal(dG, dG1, dUnequal) && unequal(dB, dB1, dUnequal))
               {

                  double dA2;

                  if (dR1 < 127)
                  {

                     dA2 = MIN(dR, MIN(dG, dB)) * dUnequalRate;

                  }
                  else
                  {

                     dA2 = (255.0 - MAX(dR, MAX(dG, dB))) * dUnequalRate;

                  }

                  f->m_pcolorref[iPixel] = (byte)(dA * dA2 / 255.0);
                  pointer->m_dib->m_pcolorref[iPixel] = ARGB(
                     (byte)255,
                     (byte)dR,
                     (byte)dG,
                     (byte)dB);

                  continue;

               }
               else
               {

                  bTransparent = false;

               }

            }
            else
            {

               bTransparent = false;

            }

            f->m_pcolorref[iPixel] = (byte)(dA);
            pointer->m_dib->m_pcolorref[iPixel] = ARGB((byte)255, (byte)dR, (byte)dG, (byte)dB);

         }

         bTransparent = pointer->m_rect.bottom == pdiba->m_size.cy; // borders transparent?

         y--;

         for (; y >= 0; y--)
         {

            index iPixel = y * w + x;

            cr = pointer->m_dib->m_pcolorref[iPixel];

            double dA = argb_get_a_value(cr);

            double dR = argb_get_r_value(cr);

            double dG = argb_get_g_value(cr);

            double dB = argb_get_b_value(cr);

            if (dA == 0.0)
            {

               bTransparent = true;

               continue;

            }

            if (bUnequal && bTransparent)
            {

               if (unequal(dR, dR1, dUnequal) && unequal(dG, dG1, dUnequal) && unequal(dB, dB1, dUnequal))
               {

                  double dA2;

                  if (dR1 < 127)
                  {

                     dA2 = MIN(dR, MIN(dG, dB)) * dUnequalRate;

                  }
                  else
                  {

                     dA2 = (255.0 - MAX(dR, MAX(dG, dB))) * dUnequalRate;

                  }

                  f->m_pcolorref[iPixel] = (byte)(dA * dA2 / 255.0);
                  pointer->m_dib->m_pcolorref[iPixel] = ARGB(
                     (byte)255,
                     (byte)dR,
                     (byte)dG,
                     (byte)dB);

                  continue;

               }
               else
               {

                  bTransparent = false;

               }

            }
            else
            {

               bTransparent = false;

            }

            f->m_pcolorref[iPixel] = (byte)(dA);
            pointer->m_dib->m_pcolorref[iPixel] = ARGB((byte)255, (byte)dR, (byte)dG, (byte)dB);

         }

      }



      dUnequal *= dUnequalScaleDown;



      for (y = 0; y < pointer->m_dib->m_size.cy; y++)
      {

         bTransparent = pointer->m_rect.left == 0; // borders transparent?

         for (x = 0; x < pointer->m_dib->m_size.cx && y + x < pointer->m_dib->m_size.cy; x++)
         {

            index iPixel = (y + x) * w + x;

            cr = pointer->m_dib->m_pcolorref[iPixel];

            double dA = argb_get_a_value(cr);

            double dR = argb_get_r_value(cr);

            double dG = argb_get_g_value(cr);

            double dB = argb_get_b_value(cr);

            if (dA == 0.0)
            {

               bTransparent = true;

               continue;

            }

            if (bUnequal && bTransparent)
            {

               if (unequal(dR, dR1, dUnequal) && unequal(dG, dG1, dUnequal) && unequal(dB, dB1, dUnequal))
               {

                  double dA2;

                  if (dR1 < 127)
                  {

                     dA2 = MIN(dR, MIN(dG, dB)) * dUnequalRate;

                  }
                  else
                  {

                     dA2 = (255.0 - MAX(dR, MAX(dG, dB))) * dUnequalRate;

                  }

                  f->m_pcolorref[iPixel] = (byte)(dA * dA2 / 255.0);
                  pointer->m_dib->m_pcolorref[iPixel] = ARGB(
                     (byte)255,
                     (byte)dR,
                     (byte)dG,
                     (byte)dB);

                  continue;

               }
               else
               {

                  bTransparent = false;

               }

            }
            else
            {

               bTransparent = false;

            }

            f->m_pcolorref[iPixel] = (byte)(dA);
            pointer->m_dib->m_pcolorref[iPixel] = ARGB((byte)255, (byte)dR, (byte)dG, (byte)dB);

         }

         bTransparent = pointer->m_rect.right == pdiba->m_size.cx; // borders transparent?

         x--;

         for (; x >= 0; x--)
         {

            index iPixel = (y + x) * w + x;

            cr = pointer->m_dib->m_pcolorref[iPixel];

            double dA = argb_get_a_value(cr);

            double dR = argb_get_r_value(cr);

            double dG = argb_get_g_value(cr);

            double dB = argb_get_b_value(cr);

            if (dA == 0.0)
            {

               bTransparent = true;

               continue;

            }

            if (bUnequal && bTransparent)
            {

               if (unequal(dR, dR1, dUnequal) && unequal(dG, dG1, dUnequal) && unequal(dB, dB1, dUnequal))
               {

                  double dA2;

                  if (dR1 < 127)
                  {

                     dA2 = MIN(dR, MIN(dG, dB)) * dUnequalRate;

                  }
                  else
                  {

                     dA2 = (255.0 - MAX(dR, MAX(dG, dB))) * dUnequalRate;

                  }

                  f->m_pcolorref[iPixel] = (byte)(dA * dA2 / 255.0);
                  pointer->m_dib->m_pcolorref[iPixel] = ARGB(
                     (byte)255,
                     (byte)dR,
                     (byte)dG,
                     (byte)dB);

                  continue;

               }
               else
               {

                  bTransparent = false;

               }

            }
            else
            {

               bTransparent = false;

            }

            f->m_pcolorref[iPixel] = (byte)(dA);
            pointer->m_dib->m_pcolorref[iPixel] = ARGB((byte)255, (byte)dR, (byte)dG, (byte)dB);

         }

      }



      dUnequal *= dUnequalScaleDown;



      for (x = 0; x < pointer->m_dib->m_size.cx; x++)
      {

         bTransparent = pointer->m_rect.top == 0; // borders transparent?

         for (y = 0; y < pointer->m_dib->m_size.cy && x-y >= 0; y++)
         {

            index iPixel = y * w + x - y;

            cr = pointer->m_dib->m_pcolorref[iPixel];

            double dA = argb_get_a_value(cr);

            double dR = argb_get_r_value(cr);

            double dG = argb_get_g_value(cr);

            double dB = argb_get_b_value(cr);

            if (dA == 0.0)
            {

               bTransparent = true;

               continue;

            }

            if (bUnequal && bTransparent)
            {

               if (unequal(dR, dR1, dUnequal) && unequal(dG, dG1, dUnequal) && unequal(dB, dB1, dUnequal))
               {

                  double dA2;

                  if (dR1 < 127)
                  {

                     dA2 = MIN(dR, MIN(dG, dB)) * dUnequalRate;

                  }
                  else
                  {

                     dA2 = (255.0 - MAX(dR, MAX(dG, dB))) * dUnequalRate;

                  }

                  f->m_pcolorref[iPixel] = (byte)(dA * dA2 / 255.0);
                  pointer->m_dib->m_pcolorref[iPixel] = ARGB(
                     (byte)255,
                     (byte)dR,
                     (byte)dG,
                     (byte)dB);

                  continue;

               }
               else
               {

                  bTransparent = false;

               }

            }
            else
            {

               bTransparent = false;

            }

            f->m_pcolorref[iPixel] = (byte)(dA);
            pointer->m_dib->m_pcolorref[iPixel] = ARGB((byte)255, (byte)dR, (byte)dG, (byte)dB);

         }

         bTransparent = pointer->m_rect.bottom == pdiba->m_size.cy; // borders transparent?

         y--;

         for (; y >= 0; y--)
         {

            index iPixel = y * w + x - y;

            cr = pointer->m_dib->m_pcolorref[iPixel];

            double dA = argb_get_a_value(cr);

            double dR = argb_get_r_value(cr);

            double dG = argb_get_g_value(cr);

            double dB = argb_get_b_value(cr);

            if (dA == 0.0)
            {

               bTransparent = true;

               continue;

            }

            if (bUnequal && bTransparent)
            {

               if (unequal(dR, dR1, dUnequal) && unequal(dG, dG1, dUnequal) && unequal(dB, dB1, dUnequal))
               {

                  double dA2;

                  if (dR1 < 127)
                  {

                     dA2 = MIN(dR, MIN(dG, dB)) * dUnequalRate;

                  }
                  else
                  {

                     dA2 = (255.0 - MAX(dR, MAX(dG, dB))) * dUnequalRate;

                  }

                  f->m_pcolorref[iPixel] = (byte)(dA * dA2 / 255.0);
                  pointer->m_dib->m_pcolorref[iPixel] = ARGB(
                     (byte)255,
                     (byte)dR,
                     (byte)dG,
                     (byte)dB);

                  continue;

               }
               else
               {

                  bTransparent = false;

               }

            }
            else
            {

               bTransparent = false;

            }

            f->m_pcolorref[iPixel] = (byte)(dA);
            pointer->m_dib->m_pcolorref[iPixel] = ARGB((byte)255, (byte)dR, (byte)dG, (byte)dB);

         }

      }



      {

         if (uFrameIndex > 0 && pdiba->element_at(uFrameIndex-1)->m_edisposal == ::visual::dib_sp::pointer::disposal_background)
         {

            pdibCompose->get_graphics()->set_alpha_mode(::draw2d::alpha_mode_set);

            COLORREF crBack = pdiba->m_crBack;

            byte bAlpha = argb_get_a_value(crBack);

            if (bAlpha == 0)
            {

               crBack = 0;

            }
            else
            {

               output_debug_string("non zero alpha");

            }

            RECT r = pdiba->element_at(uFrameIndex - 1)->m_rect;

            pdibCompose->get_graphics()->FillSolidRect(r, crBack);

         }

         pdibCompose->get_graphics()->set_alpha_mode(::draw2d::alpha_mode_blend);

         ::point pt = pointer->m_rect.top_left();

         ::size sz = pointer->m_rect.size();

         pdibCompose->get_graphics()->BitBlt(pt, sz, pointer->m_dib->get_graphics());

         pointer->m_dib->from(pdibCompose);

      }



      //f.initialize(pointer->m_dib->m_size, 1);

      //f->channel_invert(::visual::rgba::channel_red);

      //f.blur();
      //f.blur();
      //f.blur();
      //f.blur();
      //f.blur();
      //f.blur();

      //f->channel_invert(::visual::rgba::channel_red);

      pointer->m_dib->div_alpha();

      pointer->m_dib->channel_copy(::visual::rgba::channel_alpha, ::visual::rgba::channel_red, f);

      pointer->m_dib->mult_alpha();


   }
   else
   {

      int w = pointer->m_dib->m_iScan / sizeof(COLORREF);

      for (index y = 0; y < pointer->m_rect.height(); y++)
      {

         for (index x = 0; x < pointer->m_rect.width(); x++)
         {

            index iIndex = ba[x + y * pointer->m_rect.width()];

            if (iIndex >= pdiba->m_cra.get_count())
            {

               pointer->m_dib->m_pcolorref[y*w + x] = 0;

               continue;

            }

            cr = pdiba->m_cra[ba[x + y * pointer->m_rect.width()]];

            pointer->m_dib->m_pcolorref[y*w + x] = cr;

         }

      }

   }

   //pointer->m_dwTime = 1000;

   PropVariantClear(&propValue);

   return hr;

}


bool windows_load_diba_from_file(::visual::dib_sp::array * pdiba, ::file::file_sp pfile, ::aura::application * papp)
{

   if (!defer_co_initialize_ex(false))
      return false;

   file_size_t iSize = pfile->get_length();

   memory mem(papp);

   mem.allocate((memory_size_t) iSize);

   pfile->read(mem.get_data(), (memory_size_t) iSize);

   try
   {

      comptr<IStream> pstream = mem.CreateIStream();

      comptr < IWICImagingFactory > piFactory = NULL;

      comptr< IWICBitmapDecoder > decoder;

      comptr<IWICMetadataQueryReader> pMetadataQueryReader;

      HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory,
         (LPVOID*)&piFactory);

      if (FAILED(hr))
      {

         return false;

      }

      hr = piFactory->CreateDecoderFromStream(pstream, NULL, WICDecodeMetadataCacheOnDemand, &decoder.get());

      if (FAILED(hr))
      {

         return false;

      }

      PROPVARIANT propValue;

      PropVariantInit(&propValue);


      UINT cFrames;

      // Get the frame count
      hr = decoder->GetFrameCount(&cFrames);
      if (FAILED(hr))
      {

         return false;

      }

      // Create a MetadataQueryReader from the decoder
      hr = decoder->GetMetadataQueryReader(&pMetadataQueryReader.get());


      if (FAILED(hr))
      {

         return false;

      }

      // Get background color
      if (FAILED(windows_GetBackgroundColor(pdiba, piFactory, decoder, pMetadataQueryReader)))
      {
         // Default to transparent if failed to get the color
         pdiba->m_crBack = 0;

      }

      // Get global frame size
      // Get width
      hr = pMetadataQueryReader->GetMetadataByName(
         L"/logscrdesc/Width",
         &propValue);
      if (SUCCEEDED(hr))
      {
         hr = (propValue.vt == VT_UI2 ? S_OK : E_FAIL);
         if (SUCCEEDED(hr))
         {
            pdiba->m_sizeLogical.cx = propValue.uiVal;
         }
         PropVariantClear(&propValue);
      }

      // Get height
      hr = pMetadataQueryReader->GetMetadataByName(
         L"/logscrdesc/Height",
         &propValue);
      if (SUCCEEDED(hr))
      {
         hr = (propValue.vt == VT_UI2 ? S_OK : E_FAIL);
         if (SUCCEEDED(hr))
         {
            pdiba->m_sizeLogical.cy = propValue.uiVal;
         }
         PropVariantClear(&propValue);
      }

      // Get pixel aspect ratio
      hr = pMetadataQueryReader->GetMetadataByName(
         L"/logscrdesc/PixelAspectRatio",
         &propValue);
      if (SUCCEEDED(hr))
      {
         hr = (propValue.vt == VT_UI1 ? S_OK : E_FAIL);
         if (SUCCEEDED(hr))
         {
            UINT uPixelAspRatio = propValue.bVal;

            if (uPixelAspRatio != 0)
            {
               // Need to calculate the ratio. The value in uPixelAspRatio
               // allows specifying widest pixel 4:1 to the tallest pixel of
               // 1:4 in increments of 1/64th
               float pixelAspRatio = (uPixelAspRatio + 15.f) / 64.f;

               // Calculate the image width and height in pixel based on the
               // pixel aspect ratio. Only shrink the image.
               if (pixelAspRatio > 1.f)
               {
                  pdiba->m_size.cx = pdiba->m_sizeLogical.cx;
                  pdiba->m_size.cy = static_cast<unsigned int>(pdiba->m_sizeLogical.cy / pixelAspRatio);
               }
               else
               {
                  pdiba->m_size.cx = static_cast<unsigned int>(pdiba->m_sizeLogical.cx * pixelAspRatio);
                  pdiba->m_size.cy = pdiba->m_sizeLogical.cy;
               }
            }
            else
            {
               // The value is 0, so its ratio is 1
               pdiba->m_size = pdiba->m_sizeLogical;
            }
         }
         PropVariantClear(&propValue);

         // Get looping information
         if (SUCCEEDED(hr))
         {
            // First check to see if the application block in the Application Extension
            // contains "NETSCAPE2.0" and "ANIMEXTS1.0", which indicates the gif animation
            // has looping information associated with it.
            //
            // If we fail to get the looping information, loop the animation infinitely.
            if (SUCCEEDED(pMetadataQueryReader->GetMetadataByName(
               L"/appext/application",
               &propValue)) &&
               propValue.vt == (VT_UI1 | VT_VECTOR) &&
               propValue.caub.cElems == 11 &&  // Length of the application block
               (!memcmp(propValue.caub.pElems, "NETSCAPE2.0", propValue.caub.cElems) ||
                  !memcmp(propValue.caub.pElems, "ANIMEXTS1.0", propValue.caub.cElems)))
            {
               PropVariantClear(&propValue);

               hr = pMetadataQueryReader->GetMetadataByName(L"/appext/data", &propValue);
               if (SUCCEEDED(hr))
               {
                  //  The data is in the following format:
                  //  byte 0: extsize (must be > 1)
                  //  byte 1: loopType (1 == animated gif)
                  //  byte 2: loop count (least significant byte)
                  //  byte 3: loop count (most significant byte)
                  //  byte 4: set to zero
                  if (propValue.vt == (VT_UI1 | VT_VECTOR) &&
                     propValue.caub.cElems >= 4 &&
                     propValue.caub.pElems[0] > 0 &&
                     propValue.caub.pElems[1] == 1)
                  {
                     pdiba->m_uiLoopCount = MAKEWORD(propValue.caub.pElems[2],
                        propValue.caub.pElems[3]);

                     // If the total loop count is not zero, we then have a loop count
                     // If it is 0, then we repeat infinitely
                     //if (pdiba->m_uiLoopCount == 0)
                     //{
                     //   pdiba->m_iLoop = -1;
                     //}
                  }
               }
            }
         }

      }



      PropVariantClear(&propValue);



      pdiba->set_size(cFrames);

      pdiba->m_dwTotal = 0;

      ::draw2d::dib_sp dibCompose(papp->allocer());

      dibCompose->create(pdiba->m_size);

      dibCompose->Fill(0);

      for (UINT ui = 0; ui < cFrames; ui++)
      {

         pdiba->element_at(ui) = canew(::visual::dib_sp::pointer());

         pdiba->element_at(ui)->m_dib.alloc(papp->allocer());

         windows_GetRawFrame(dibCompose, pdiba, piFactory, decoder, (UINT) ui);

         pdiba->m_dwTotal += pdiba->element_at(ui)->m_dwTime;

      }

   }
   catch (...)
   {

      return false;

   }

   return true;
}



bool windows_load_dib_from_file(::draw2d::dib * pdib, ::file::file_sp pfile, ::aura::application * papp)
{

   if (!defer_co_initialize_ex(false))
      return false;

   file_size_t iSize = pfile->get_length();

   memory mem(papp);

   mem.allocate((memory_size_t) iSize);

   pfile->read(mem.get_data(), (memory_size_t) iSize);

   try
   {

      comptr<IStream> pstream = mem.CreateIStream();
      comptr < IWICImagingFactory > piFactory = NULL;

      comptr< IWICBitmapDecoder > decoder;
      HRESULT hr = CoCreateInstance(
         CLSID_WICImagingFactory,
         NULL,
         CLSCTX_INPROC_SERVER,
         IID_IWICImagingFactory,
         (LPVOID*)&piFactory);

      if (SUCCEEDED(hr))
      {

         hr = piFactory->CreateDecoderFromStream(pstream, NULL, WICDecodeMetadataCacheOnDemand, &decoder.get());
      }

      //if(SUCCEEDED(hr))
      //{

      //   hr = decoder->Initialize(pstream,WICDecodeMetadataCacheOnDemand);

      //}

      comptr< IWICBitmapFrameDecode> pframe;

      if (!windows_load_dib_from_frame(pframe, pdib, piFactory, decoder, 0))
      {

         return false;

      }

   }
   catch (...)
   {
      return false;
   }
   //DWORD dw2 =::get_tick_count();
   //TRACE("InPath %d ms\n",dw2 - dw1);
   //dwLast = dw2;

   //memfile.Truncate(0);

   //}
   return true;

   }

#ifdef METROWIN

bool windows_load_dib_from_file(::draw2d::dib * pdib, Windows::Storage::Streams::IRandomAccessStream ^stream , ::aura::application * papp)
{

   if (!defer_co_initialize_ex(true))
      return false;

   try
   {

      comptr < IStream > pstream;

      ::CreateStreamOverRandomAccessStream(stream, IID_PPV_ARGS(&pstream.get()));

      comptr < IWICImagingFactory > piFactory = NULL;

      comptr< IWICBitmapDecoder > decoder;
      HRESULT hr = CoCreateInstance(
         CLSID_WICImagingFactory,
         NULL,
         CLSCTX_INPROC_SERVER,
         IID_IWICImagingFactory,
         (LPVOID*)&piFactory);

      if (SUCCEEDED(hr))
      {

         hr = piFactory->CreateDecoderFromStream(pstream, NULL, WICDecodeMetadataCacheOnDemand, &decoder.get());
      }

      //if(SUCCEEDED(hr))
      //{

      //   hr = decoder->Initialize(pstream,WICDecodeMetadataCacheOnDemand);

      //}

      comptr< IWICBitmapFrameDecode> pframe;

      if (!windows_load_dib_from_frame(pframe, pdib, piFactory, decoder, 0))
      {

         return false;

      }

   }
   catch (...)
   {
      return false;
   }
   //DWORD dw2 =::get_tick_count();
   //TRACE("InPath %d ms\n",dw2 - dw1);
   //dwLast = dw2;

   //memfile.Truncate(0);

   //}
   return true;

}

#endif

bool windows_write_dib_to_file(::file::file_sp pfile, ::draw2d::dib * pdib, ::visual::save_image * psaveimage, ::aura::application * papp)
{

#ifdef METROWIN

   Windows::Storage::Streams::InMemoryRandomAccessStream ^ randomAccessStream = ref new Windows::Storage::Streams::InMemoryRandomAccessStream();

   //::wait(randomAccessStream->WriteAsync(get_os_buffer()));

   comptr < IStream > pstream;

   ::CreateStreamOverRandomAccessStream(randomAccessStream, IID_PPV_ARGS(&pstream.get()));

#else

   comptr < IStream > pstream = LIBCALL(shlwapi, SHCreateMemStream)(NULL, NULL);

#endif

   //m_spmemfile->Truncate(0);

   //m_spmemfile->seek_to_begin();

   comptr < IWICImagingFactory > piFactory = NULL;
   comptr < IWICBitmapEncoder > piEncoder = NULL;
   comptr < IWICBitmapFrameEncode > piBitmapFrame = NULL;
   comptr < IPropertyBag2 > pPropertybag = NULL;

   comptr < IWICStream > piStream = NULL;
   UINT uiWidth = pdib->size().cx;
   UINT uiHeight = pdib->size().cy;

   HRESULT hr = CoCreateInstance(
      CLSID_WICImagingFactory,
      NULL,
      CLSCTX_INPROC_SERVER,
      IID_IWICImagingFactory,
      (LPVOID*)&piFactory);

   if (SUCCEEDED(hr))
   {
      hr = piFactory->CreateStream(&piStream.get());
   }

   if (SUCCEEDED(hr))
   {
      hr = piStream->InitializeFromIStream(pstream);
   }

   if (SUCCEEDED(hr))
   {
      switch (psaveimage->m_eformat)
      {
      case ::visual::image::format_bmp:
         hr = piFactory->CreateEncoder(GUID_ContainerFormatBmp, NULL, &piEncoder.get());
         break;
      case ::visual::image::format_gif:
         hr = piFactory->CreateEncoder(GUID_ContainerFormatGif, NULL, &piEncoder.get());
         break;
      case ::visual::image::format_jpeg:
         hr = piFactory->CreateEncoder(GUID_ContainerFormatJpeg, NULL, &piEncoder.get());
         break;
      case ::visual::image::format_png:
         hr = piFactory->CreateEncoder(GUID_ContainerFormatPng, NULL, &piEncoder.get());
         break;
      default:
         break;
      }
   }

   if (SUCCEEDED(hr))
   {
      hr = piEncoder->Initialize(piStream, WICBitmapEncoderNoCache);
   }

   if (SUCCEEDED(hr))
   {
      hr = piEncoder->CreateNewFrame(&piBitmapFrame.get(), &pPropertybag.get());
   }

   if (SUCCEEDED(hr))
   {
      //if(m_bJxr)
      //{
      //   //PROPBAG2 option ={0};
      //   //option.pstrName = L"ImageQuality";
      //   //VARIANT varValue;
      //   //VariantInit(&varValue);
      //   //varValue.vt = VT_R4;
      //   //varValue.fltVal = 0.49f;
      //   PROPBAG2 option ={0};
      //   option.pstrName = L"UseCodecOptions";
      //   VARIANT varValue;
      //   VariantInit(&varValue);
      //   varValue.vt = VT_BOOL;
      //   varValue.boolVal = -1;
      //   if(SUCCEEDED(hr))
      //   {
      //      hr = pPropertybag->Write(1,&option,&varValue);
      //   }
      //   option.pstrName = L"Quality";
      //   VariantInit(&varValue);
      //   varValue.vt = VT_UI1;
      //   varValue.bVal = 184;
      //   if(SUCCEEDED(hr))
      //   {
      //      hr = pPropertybag->Write(1,&option,&varValue);
      //   }
      //   option.pstrName = L"Subsampling";
      //   VariantInit(&varValue);
      //   varValue.vt = VT_UI1;
      //   varValue.bVal = 1;
      //   if(SUCCEEDED(hr))
      //   {
      //      hr = pPropertybag->Write(1,&option,&varValue);
      //   }
      //   option.pstrName = L"Overlap";
      //   VariantInit(&varValue);
      //   varValue.vt = VT_UI1;
      //   varValue.bVal = 2;
      //   if(SUCCEEDED(hr))
      //   {
      //      hr = pPropertybag->Write(1,&option,&varValue);
      //   }
      //   option.pstrName = L"StreamOnly";
      //   VariantInit(&varValue);
      //   varValue.vt = VT_BOOL;
      //   varValue.boolVal = -1;
      //   if(SUCCEEDED(hr))
      //   {
      //      hr = pPropertybag->Write(1,&option,&varValue);
      //   }
      //}
      if (psaveimage->m_eformat == ::visual::image::format_jpeg)
      {
         PROPBAG2 option = { 0 };
         option.pstrName = L"ImageQuality";
         VARIANT varValue;
         VariantInit(&varValue);
         varValue.vt = VT_R4;
         varValue.fltVal = MAX(0.f, MIN(1.f, psaveimage->m_iQuality / 100.0f));
         if (SUCCEEDED(hr))
         {
            hr = pPropertybag->Write(1, &option, &varValue);
         }
      }
      if (SUCCEEDED(hr))
      {
         hr = piBitmapFrame->Initialize(pPropertybag);
      }
   }

   if (SUCCEEDED(hr))
   {
      hr = piBitmapFrame->SetSize(uiWidth, uiHeight);
   }

   WICPixelFormatGUID formatGUID = GUID_WICPixelFormat32bppBGRA;
   if (SUCCEEDED(hr))
   {
      hr = piBitmapFrame->SetPixelFormat(&formatGUID);
   }
   pdib->map();
   if (SUCCEEDED(hr))
   {
      if (IsEqualGUID(formatGUID, GUID_WICPixelFormat32bppBGRA))
      {
         if (SUCCEEDED(hr))
         {
            hr = piBitmapFrame->WritePixels(uiHeight, pdib->m_iScan, uiHeight*pdib->m_iScan, (BYTE *)pdib->m_pcolorref);
         }
      }
      else
      {

         comptr <IWICBitmap> pbitmap;

         if (SUCCEEDED(hr))
         {
            hr = piFactory->CreateBitmapFromMemory(
               pdib->size().cx,
               pdib->size().cy,
               GUID_WICPixelFormat32bppBGRA,
               pdib->m_iScan,
               pdib->m_iScan * pdib->size().cy,
               (BYTE *)pdib->m_pcolorref,
               &pbitmap.get()
            );
         }

         comptr<IWICFormatConverter> pconverter;

         if (SUCCEEDED(hr))
         {

            hr = piFactory->CreateFormatConverter(&pconverter.get());

         }



         if (SUCCEEDED(hr))
         {

            hr = pconverter->Initialize(pbitmap, formatGUID, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeCustom);
         }

         //Step 4: Create render target and D2D bitmap from IWICBitmapSource
         //UINT width=0;
         //UINT height=0;
         //if(SUCCEEDED(hr))
         //{
         //   hr = pbitmap->GetSize(&width,&height);
         //}

         //pdib->create(width,height);

         if (SUCCEEDED(hr))
         {
            hr = piBitmapFrame->WriteSource(pconverter, NULL);
         }


         //for(int k = 0; k < height; k++)
         //{
         //   memcpy(&pb[k * iStride],&mem.get_data()[(height - 1 - k) * iStride],iStride);
         //}

      }
   }



   if (SUCCEEDED(hr))
   {
      hr = piBitmapFrame->Commit();
   }

   if (SUCCEEDED(hr))
   {
      hr = piEncoder->Commit();
   }

   //if(piFactory)
   //   piFactory->Release();

   //if(piBitmapFrame)
   //   piBitmapFrame->Release();

   //if(piEncoder)
   //   piEncoder->Release();

   //if(piStream)
   //   piStream->Release();



   STATSTG stg;
   ZERO(stg);
   pstream->Stat(&stg, STATFLAG_NONAME);
   LARGE_INTEGER l;
   l.QuadPart = 0;
   pstream->Seek(l, STREAM_SEEK_SET, NULL);


   memory mem(papp);

   mem.allocate(1024 * 1024);

   ULONG ulPos = 0;
   ULONG ulRead;
   ULONGLONG ul;
   do
   {

      ulRead = 0;

      ul = stg.cbSize.QuadPart - ulPos;

      pstream->Read(mem.get_data(), (ULONG) MIN(ul, mem.get_size()), &ulRead);

      if (ulRead > 0)
      {

         pfile->write(mem.get_data(), ulRead);

         ulPos += ulRead;

      }

   } while (ulRead > 0 && stg.cbSize.QuadPart - ulPos > 0);

   //pstream->Release();

   return true;

}

#endif
