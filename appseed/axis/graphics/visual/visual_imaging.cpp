//#include "framework.h"
#include "freeimage/Source/FreeImage.h"
#include "visual_FreeImageFileProc.h"
#ifdef WINDOWSEX

#undef new
#define min MIN
#define max MAX
//#include <gdiplus.h>
#undef min
#undef max
#define new AURA_NEW


//void fastblur(::draw2d::dib * pimg, int32_t radius);

#elif defined(METROWIN)
//#include <wincodec.h>
//#include <Shcore.h>

#endif

#ifdef ANDROID

#define _stricmp strcasecmp

#endif

bool freeimage_load_diba_from_file(::visual::dib_sp::array * pdiba, ::file::file_sp, ::aura::application * papp);

bool windows_load_diba_from_file(::visual::dib_sp::array * pdiba, ::file::file_sp, ::aura::application * papp);
bool windows_load_dib_from_file(::draw2d::dib * pdib,::file::file_sp,::aura::application * papp);

template <typename T>
inline void SafeRelease(T *&p)
{
   if(nullptr != p)
   {
      p->Release();
      p = nullptr;
   }
}


#define AC_SRC_ALPHA                0x01

#ifdef WINDOWS

const imaging::CSysColorMap imaging::s_psyscolormap[] =
{
   // mapping from color in DIB to system color
   {RGB_TO_RGBQUAD(0x00,0x00,0x00),COLOR_BTNTEXT},       // black
   {RGB_TO_RGBQUAD(0x80,0x80,0x80),COLOR_BTNSHADOW},     // dark gray
   {RGB_TO_RGBQUAD(0xC0,0xC0,0xC0),COLOR_BTNFACE},       // bright gray
   {RGB_TO_RGBQUAD(0xFF,0xFF,0xFF),COLOR_BTNHIGHLIGHT},   // white
   {RGB_TO_RGBQUAD(0xFF,0xFF,0xFF),0x80000000}
};

#else

// throw todo(get_app());

const imaging::CSysColorMap imaging::s_psyscolormap[] =
{
   // mapping from color in DIB to system color
   {RGB_TO_RGBQUAD(0x00,0x00,0x00),0x80000000},       // black
   {RGB_TO_RGBQUAD(0x80,0x80,0x80),0x80000000},     // dark gray
   {RGB_TO_RGBQUAD(0xC0,0xC0,0xC0),0x80000000},       // bright gray
   {RGB_TO_RGBQUAD(0xFF,0xFF,0xFF),0x80000000},   // white
   {RGB_TO_RGBQUAD(0xFF,0xFF,0xFF),0x80000000}
};

#endif

imaging::imaging(::aura::application * papp):
::object(papp),
m_mutex(papp)
{
}

imaging::~imaging()
{
   //    if(m_lpVoid != NULL)
   //        free(m_lpVoid);
}




//FIBITMAP * imaging::HBITMAPtoFI(HBITMAP hbitmap)
//{
//
//   //if (pbitmap == NULL)
//   //   return NULL;
//
//
//
//   //HBITMAP hbitmap = pbitmap->GetHBITMAP();
//
//   if (hbitmap == NULL)
//      return NULL;
//
//   // ...
//   // the following code assumes that you have a valid HBITMAP loaded into the memory
//   BITMAP bm;
//   ::GetObject(hbitmap, sizeof(BITMAP), (char *)&bm);
//   if (bm.bmWidth <= 0 || bm.bmHeight <= 0)
//      return NULL;
//   FIBITMAP * fi;
//   //if(bm.bmBitsPixel == 32)
//   {
//      // fi = FreeImage_AllocateT(bm.bmWidth,bm.bmHeight,bm.bmBitsPixel);
//   }
//   //else
//   {
//      fi = FreeImage_Allocate(bm.bmWidth, bm.bmHeight, bm.bmBitsPixel);
//   }
//   // The GetDIBits function clears the biClrUsed and biClrImportant BITMAPINFO members (dont't know why)
//   // So we save these infos below. This is needed for palettized images only.
//   int32_t nColors = FreeImage_GetColorsUsed(fi);
//   HDC hdc = ::CreateCompatibleDC(NULL);
//
//   GetDIBits(hdc, (HBITMAP)hbitmap, 0, FreeImage_GetHeight(fi), FreeImage_GetBits(fi), FreeImage_GetInfo(fi), DIB_RGB_COLORS);
//
//   ::DeleteDC(hdc);
//
//   ///   pbitmap->ReleaseHBITMAP(hbitmap);
//
//   // restore BITMAPINFO members
//   FreeImage_GetInfoHeader(fi)->biClrUsed = nColors;
//   FreeImage_GetInfoHeader(fi)->biClrImportant = nColors;
//   return fi;
//
//
//}


//#ifndef  METROWIN


FIBITMAP * imaging::LoadImageFile(var varFile,::aura::application * papp)
{
   ::memory_file memfile(get_app());
   System.file().as_memory(varFile,*memfile.get_memory(),papp);
   if(memfile.get_size() <= 0)
      return NULL;
   return LoadImageFile(&memfile);
}

void imaging::free(FIBITMAP * pfibitmap)
{

#ifdef METROWIN

   throw todo(get_app());

#else
   FreeImage_Unload(pfibitmap);
#endif
}

void imaging::SavePng(const char * lpcszFile,FIBITMAP *dib,bool bUnload)
{


#ifdef METROWIN

   throw todo(get_app());

#else

   if(FreeImage_Save(FreeImage_GetFIFFromFormat("PNG"),dib,lpcszFile,0)) {
      //
   }
   if(bUnload)
   {
      FreeImage_Unload(dib);
   }
#endif

}


FIBITMAP * imaging::dib_to_FI(::draw2d::dib * pdib)
{

   if(pdib == NULL)
      return NULL;

   if(pdib->area() <= 0)
      return NULL;

   FIBITMAP * fi;

//   if(bm.bmBitsPixel == 32)
   {
      fi = FreeImage_AllocateT(FIT_BITMAP,pdib->m_size.cx, pdib->m_size.cy,32);
   }
  // else
   {
    //  fi = FreeImage_Allocate(bm.bmWidth,bm.bmHeight,bm.bmBitsPixel);
   }
   // The GetDIBits function clears the biClrUsed and biClrImportant BITMAPINFO members (dont't know why)
   // So we save these infos below. This is needed for palettized images only.
   int32_t nColors = FreeImage_GetColorsUsed(fi);
   //HDC hdc = ::CreateCompatibleDC(NULL);

   int iWidth;
   int iHeight;
   COLORREF * pcolorref;
   int iStrideDst;

   iWidth = FreeImage_GetWidth(fi);

   iHeight = FreeImage_GetHeight(fi);

   pcolorref = (COLORREF *)FreeImage_GetBits(fi);

   if(FreeImage_GetInfo(fi)->bmiHeader.biSizeImage <= 0)
   {

        iStrideDst = iWidth * sizeof(COLORREF);

   }
   else
   {

       iStrideDst = FreeImage_GetInfo(fi)->bmiHeader.biSizeImage / iHeight;

   }

   COLORREF * pdst = pcolorref;

   COLORREF * psrc = pdib->m_pcolorref;

   int iStrideSrc = pdib->m_iScan;

#if  defined(VSNORD)

   for (index y = 0; y < pdib->m_size.cy; y++)
   {

      byte * pbDst = ((byte *)pdst) + ((pdib->m_size.cy - y - 1) * iStrideDst);

      byte * pbSrc = (byte *)psrc + (y * iStrideSrc);

      for (index x = 0; x < pdib->m_size.cx; x++)
      {

         pbDst[0] = pbSrc[2];

         pbDst[1] = pbSrc[1];

         pbDst[2] = pbSrc[0];

         pbDst[3] = pbSrc[3];

         pbDst += 4;

         pbSrc += 4;

      }

   }

#elif defined(APPLEOS)

   byte * pbDst = (byte *)pdst;

   byte * pbSrc = (byte *)psrc;

   ::count c = (count)pdib->area();

   while(c-- > 0)
   {

      pbDst[0] = pbSrc[2];

      pbDst[1] = pbSrc[1];

      pbDst[2] = pbSrc[0];

      pbDst[3] = pbSrc[3];

      pbDst += 4;

      pbSrc += 4;

   }

   /*

    byte * pbDst;

    byte * pbSrc;

    for(int i = 0; i < pdib->m_size.cy; i++)
    {

    pbDst = &((byte *) pdib->m_pcolorref)[pdib->m_iScan * (pdib->m_size.cy - i - 1)];

    pbSrc = &((byte *) pdata)[pbi->bmiHeader.biWidth * sizeof(COLORREF) * i];

    for(int j = 0; j < pdib->m_size.cx; j++)
    {

    pbDst[0] = pbSrc[2];

    pbDst[1] = pbSrc[1];

    pbDst[2] = pbSrc[0];

    pbDst[3] = pbSrc[3];

    pbDst += 4;

    pbSrc += 4;

    }

    }*/

#else
   for(int i = 0; i < pdib->m_size.cy; i++)
   {

      memcpy(
             &((byte *)pdst)[iStrideDst * (pdib->m_size.cy - i - 1)],
             &((byte *)psrc)[iStrideSrc * i],
             iStrideDst);

   }
#endif

   //GetDIBits(hdc,(HBITMAP)hbitmap,0,FreeImage_GetHeight(fi),FreeImage_GetBits(fi),FreeImage_GetInfo(fi),DIB_RGB_COLORS);

   //::DeleteDC(hdc);

   //pbitmap->ReleaseHBITMAP(hbitmap);

   // restore BITMAPINFO members
   FreeImage_GetInfoHeader(fi)->biClrUsed = nColors;
   FreeImage_GetInfoHeader(fi)->biClrImportant = nColors;
   return fi;


}



::draw2d::bitmap_sp imaging::FItoHBITMAP(FIBITMAP * pfibitmap,bool bUnloadFI)
{

   if(pfibitmap == NULL)
      return NULL;

   //   BITMAPINFO * pbi = FreeImage_GetInfo(pfibitmap);
   // void * pData = FreeImage_GetBits(pfibitmap);


   ::draw2d::dib_sp dib(allocer());

   //BITMAPINFO * pi = FreeImage_GetInfo(pFreeImage);

   ::draw2d::graphics_sp spgraphics(allocer());
   spgraphics->CreateCompatibleDC(NULL);

   if(!from(dib,spgraphics,pfibitmap,false))
      return NULL;

   return dib->detach_bitmap();

   /*::draw2d::graphics_sp spgraphics(allocer());
   spgraphics->CreateCompatibleDC(NULL);

   ::draw2d::dib_sp dibSource(get_app());
   dibSource->create(pbi->bmiHeader.biWidth, pbi->bmiHeader.biHeight);

   dibSource->dc_select(false);

   if(pbi->bmiHeader.biHeight != SetDIBits(
   (HDC)spgraphics->get_os_data(),
   (HBITMAP) dibSource->get_bitmap()->get_os_data(),
   0,
   pbi->bmiHeader.biHeight,
   pData,
   pbi,
   DIB_RGB_COLORS))
   {
   if(bUnloadFI)
   {
   FreeImage_Unload(pfibitmap);
   }
   return NULL;
   }

   if(bUnloadFI)
   {
   FreeImage_Unload(pfibitmap);
   }
   */

   //return dibSource->detach_bitmap();
}

::draw2d::bitmap_sp imaging::CreateDIBitmap(::draw2d::graphics * pgraphics,FIBITMAP * pFreeImage)
{


#ifdef WINDOWSEX
   ::draw2d::bitmap_sp bitmap(get_app());

   if(!bitmap->CreateDIBitmap(pgraphics,FreeImage_GetInfoHeader(pFreeImage),CBM_INIT,FreeImage_GetBits(pFreeImage),FreeImage_GetInfo(pFreeImage),DIB_RGB_COLORS))
   {

      TRACELASTERROR();

      return NULL;

   }

   return bitmap;
#else

   throw todo(get_app());

#endif
   return NULL;
}


::draw2d::bitmap_sp imaging::CreateBitmap(::draw2d::graphics * pgraphics,FIBITMAP * pFreeImage)
{
   ::visual::dib_sp dib(allocer());

#ifdef METROWIN

   throw todo(get_app());

#else

   //BITMAPINFO * pi = FreeImage_GetInfo(pFreeImage);



   from(dib, pgraphics,pFreeImage,false);


   return dib->detach_bitmap();

#endif

   /*   ::draw2d::bitmap_sp bitmap(get_app());
   void * pBits = FreeImage_GetBits(pFreeImage);
   if(!bitmap->CreateDIBitmap(pgraphics,
   FreeImage_GetInfoHeader(pFreeImage),
   CBM_INIT,
   pBits,
   FreeImage_GetInfo(pFreeImage),
   DIB_RGB_COLORS))
   {
   TRACELASTERROR();
   return (::draw2d::bitmap *) NULL;
   }

   //   LPVOID lpBits;
   //   BITMAPINFO *pbi = FreeImage_GetInfo(pFreeImage);
   //   HBITMAP hBitmap = ::CreateDIBSection(
   //      NULL,
   //      pbi,
   //      DIB_RGB_COLORS,
   //      &lpBits,
   //      NULL,
   //      0);
   //   memcpy(lpBits, FreeImage_GetBits(pFreeImage), pbi->bmiHeader.biSize);
   LPBITMAPINFO pbi = FreeImage_GetInfo(pFreeImage);
   int32_t iSizeBitsZ = ((pbi->bmiHeader.biWidth * pbi->bmiHeader.biBitCount / 8 + 3) & ~3) * pbi->bmiHeader.biHeight;
   void * pDataZ = malloc(iSizeBitsZ);
   if(pbi->bmiHeader.biHeight != GetDIBits(
   (HDC)pgraphics->get_os_data(),           // handle to device context
   (HBITMAP)bitmap->get_os_data(),      // handle to bitmap
   0,   // first scan line to set in destination bitmap
   FreeImage_GetInfo(pFreeImage)->bmiHeader.biHeight,   // number of scan lines to copy
   pDataZ,    // address of array for bitmap bits
   FreeImage_GetInfo(pFreeImage), // address of structure with bitmap data
   DIB_RGB_COLORS        // RGB or palette index
   ))
   {
   //      int32_t i = 1 +1;
   TRACELASTERROR();
   }
   return bitmap;*/
}



bool imaging::from(::draw2d::dib * pdib,::draw2d::graphics * pgraphics,FIBITMAP *pfibitmap,bool bUnloadFI,::aura::application * papp)
{

   if(pfibitmap == NULL)
      return false;

   if(papp == NULL)
      papp = get_app();

   BITMAPINFO * pbi = NULL;

   void * pdata = NULL;
   FIBITMAP * pimage32 = FreeImage_ConvertTo32Bits(pfibitmap);

   if(pimage32 == NULL)
   {

      return false;

   }

   pbi = FreeImage_GetInfo(pimage32);

   pdata = FreeImage_GetBits(pimage32);


   if(!pdib->create(pbi->bmiHeader.biWidth,pbi->bmiHeader.biHeight))
      return false;


   /*   COLORREF * pcolorref = NULL;

      HBITMAP hbitmap = ::CreateDIBSection(NULL, &pdib->m_info, DIB_RGB_COLORS, (void **)&pcolorref, NULL, 0);

      if (hbitmap == NULL)
      {
      pdib->Destroy();
      return false;
      }

      HDC hdc = ::CreateCompatibleDC(NULL);

      if (pbi->bmiHeader.biHeight != SetDIBits(
      hdc,
      hbitmap,
      0,
      pbi->bmiHeader.biHeight,
      pdata,
      pbi,
      DIB_RGB_COLORS))
      {
      pdib->Destroy();
      if (bUnloadFI)
      {
      FreeImage_Unload(pfibitmap);
      }
      return false;
      }
      */

   pdib->map();

   int stride = pbi->bmiHeader.biWidth * sizeof(COLORREF);

//#if defined(VSNORD) && defined(__arm__)
#if defined(ANDROID)

   // LITTLE_LIT_LIGHT_LITE_LITLE_ENDIANS!!!!!!!!!!

   //::draw2d::copy_colorref(
   //   pdib->m_size.cx,
   //   pdib->m_size.cy,
   //   pdib->m_pcolorref,
   //   pdib->m_iScan,
   //   (COLORREF *)pdata,
   //   stride);

   for (index y = 0; y < pdib->m_size.cy; y++)
   {

      byte * pbDst = ((byte *)pdib->m_pcolorref) + ((pdib->m_size.cy - y - 1) * pdib->m_iScan);

      byte * pbSrc = (byte *)pdata + (y * stride);

      for (index x = 0; x < pdib->m_size.cx; x++)
      {

         pbDst[0] = pbSrc[2];

         pbDst[1] = pbSrc[1];

         pbDst[2] = pbSrc[0];

         pbDst[3] = pbSrc[3];

         pbDst += 4;

         pbSrc += 4;

      }

   }

#elif defined(APPLEOS)

   byte * pbDst = (byte *)pdib->m_pcolorref;

   byte * pbSrc = (byte *)pdata;

   ::count c = (count)pdib->area();

   while(c-- > 0)
   {

      pbDst[0] = pbSrc[2];

      pbDst[1] = pbSrc[1];

      pbDst[2] = pbSrc[0];

      pbDst[3] = pbSrc[3];

      pbDst += 4;

      pbSrc += 4;

   }

   /*

      byte * pbDst;

      byte * pbSrc;

      for(int i = 0; i < pdib->m_size.cy; i++)
      {

      pbDst = &((byte *) pdib->m_pcolorref)[pdib->m_iScan * (pdib->m_size.cy - i - 1)];

      pbSrc = &((byte *) pdata)[pbi->bmiHeader.biWidth * sizeof(COLORREF) * i];

      for(int j = 0; j < pdib->m_size.cx; j++)
      {

      pbDst[0] = pbSrc[2];

      pbDst[1] = pbSrc[1];

      pbDst[2] = pbSrc[0];

      pbDst[3] = pbSrc[3];

      pbDst += 4;

      pbSrc += 4;

      }

      }*/

#else
   for(int i = 0; i < pdib->m_size.cy; i++)
   {

      memcpy(
         &((byte *)pdib->m_pcolorref)[pdib->m_iScan * (pdib->m_size.cy - i - 1)],
         &((byte *)pdata)[pbi->bmiHeader.biWidth * sizeof(COLORREF) * i],
         pdib->m_iScan);

   }
#endif

//#if defined(LINUX) || defined(VSNORD) || defined(METROWIN)
   pdib->mult_alpha_fast();
//#endif



   //   RGBQUAD bkcolor;
   FreeImage_Unload(pimage32);
   if(bUnloadFI)
   {

      FreeImage_Unload(pfibitmap);

   }

   return true;

}
FIBITMAP * imaging::LoadImageFile(::file::file_sp  pfile)
{

   if(pfile == NULL)
      return NULL;

   FreeImageIO io;
   io.read_proc = __ReadProc2;
   io.seek_proc = __SeekProc2;
   io.tell_proc = __TellProc2;
   io.write_proc = __WriteProc2;
   FIBITMAP *lpVoid = NULL;
   try
   {
      pfile->seek_to_begin();
      FREE_IMAGE_FORMAT format;
      format = FreeImage_GetFileTypeFromHandle(&io,(::file::file *)pfile.m_p);
      pfile->seek_to_begin();
      if(true)
      {
         lpVoid = FreeImage_LoadFromHandle(format,&io,(::file::file *)pfile.m_p);
      }
   }
   catch(...)
   {
   }

   return lpVoid;


}


/*FIBITMAP * imaging::LoadImageFile(CArchive & ar)
{
ASSERT(!ar.IsStoring());

return LoadImageFile(ar.GetFile());

}
*/
/*HBITMAP imaging::LoadBitmap(
const char * lpszType,
const char * lpszId)
{
::exception::throw_not_implemented(get_app());
/*
::memory_file file(get_app());

::core::Resource resource;

if(!resource.ReadResource(*file.get_memory(), (UINT) MAKEINTRESOURCE(lpszId), lpszType))
return false;

file.seek_to_begin();

::file::file_sp  pfile = &file;

FreeImageIO io;
io.read_proc   = ___Ex1File__ReadProc;
io.seek_proc   = ___Ex1File__SeekProc;
io.tell_proc   = ___Ex1File__TellProc;
io.write_proc  = ___Ex1File__WriteProc;

FREE_IMAGE_FORMAT format;
format = FreeImage_GetFileTypeFromHandle(&io, pfile ,16);
FIBITMAP *pfi = NULL;
if(true)
{
pfi = FreeImage_LoadFromHandle(format, &io, pfile);
}

if(pfi == NULL)
return NULL;

BITMAPINFO * pbi = FreeImage_GetInfo(pfi);
void * pData = FreeImage_GetBits(pfi);


HDC hdcSource = CreateDC(
"DISPLAY",
NULL,
NULL,
NULL);

HBITMAP hBitmapSource = ::CreateCompatibleBitmap(
hdcSource,
pbi->bmiHeader.biWidth,
pbi->bmiHeader.biHeight);

if(pbi->bmiHeader.biHeight != SetDIBits(
hdcSource,
hBitmapSource,
0,
pbi->bmiHeader.biHeight,
pData,
pbi,
DIB_RGB_COLORS))
{
FreeImage_Unload(pfi);
DeleteDC(hdcSource);
delete_object(hBitmapSource);
return NULL;
}

FreeImage_Unload(pfi);
DeleteDC(hdcSource);

return hBitmapSource;
*/
//}
//*/


//#endif // WINDOWSEX


bool imaging::LoadImage(::visual::dib_sp::array * pdiba, var varFile)
{

   ::file::file_sp file = App(pdiba->get_app()).file().get_file(varFile, ::file::type_binary | ::file::mode_read | ::file::share_deny_write);

   if(file.is_null())
   {
      
      return false;
      
   }

   if(!LoadImageFromFile(pdiba, file))
   {
      
      return false;
      
   }
   
   return true;
   
}


bool imaging::LoadImage(::draw2d::dib * pdib,var varFile)
{

   ::file::file_sp file = App(pdib->get_app()).file().get_file(varFile, ::file::type_binary | ::file::mode_read | ::file::share_deny_write);
   
   if(file.is_null())
   {
      
      return false;
      
   }

   if(!LoadImageFromFile(pdib, file))
   {
      
      return false;
      
   }

   return true;

}


bool imaging::LoadImageFromFile(::visual::dib_sp::array * pdiba, ::file::file * pfile)
{
   
   if(!freeimage_load_diba_from_file(pdiba, pfile, get_app()))
   {
   
      return false;
         
   }
   
   return true;
   
}


bool imaging::LoadImageFromFile(::draw2d::dib * pdib, ::file::file * pfile)
{
   
   FIBITMAP * pfi = LoadImageFile(pfile);
   
   if(pfi == NULL)
   {
      
      return false;
      
   }
   
   ::draw2d::graphics_sp spgraphics(allocer());
   
   spgraphics->CreateCompatibleDC(NULL);

   int iExifOrientation = -1;

   bool bOrientation = false;

   FITAG *tag = NULL;

   FIMETADATA *mdhandle = FreeImage_FindFirstMetadata(FIMD_EXIF_MAIN, pfi, &tag);

   if (mdhandle)
   {
      
      do
      {
      
         auto type = FreeImage_GetTagType(tag);

         auto value = FreeImage_GetTagValue(tag);

         if (!_stricmp(FreeImage_GetTagKey(tag), "orientation"))
         {
            
            bOrientation = true;

            if (type == FIDT_SHORT)
            {

               iExifOrientation = *((unsigned short*)value);

            }
               
         }

         if (bOrientation)
         {

            break;

         }
         
      }
      while (FreeImage_FindNextMetadata(mdhandle, &tag));

      FreeImage_FindCloseMetadata(mdhandle);

   }
   
   if(!from(pdib,spgraphics,(FIBITMAP *)pfi,true))
   {
    
      return false;
      
   }

   if (bOrientation)
   {

      ::draw2d::dib_sp dib(allocer());

      //double dPiQuarter = ::atan(1.0);

      //double dPi = dPiQuarter * 4.0;

      // http://sylvana.net/jpegcrop/exif_orientation.html
      //1) transform = "";;
      //2) transform = "-flip horizontal";;
      //3) transform = "-rotate 180";;
      //4) transform = "-flip vertical";;
      //5) transform = "-transpose";;
      //6) transform = "-rotate 90";;
      //7) transform = "-transverse";;
      //8) transform = "-rotate 270";;
      //*) transform = "";;
      switch (iExifOrientation)
      {
      case 2:
         dib->flip_horizontal(pdib);
         pdib->from(dib);
         break;
      case 3:
         dib->rotate(pdib, 180.0);
         pdib->from(dib);
         break;
      case 4:
         dib->flip_vertical(pdib);
         pdib->from(dib);
         break;
      case 5:
         dib->flip_horizontal(pdib);
         pdib->rotate(dib, -270.0);
         break;
      case 6:
         dib->rotate(pdib, -90.0);
         pdib->from(dib);
         break;
      case 7:
         dib->flip_horizontal(pdib);
         pdib->rotate(dib, -90.0);
         break;
      case 8:
         dib->rotate(pdib, -270.0);
         pdib->from(dib);
         break;
      case 1:
      default:
         break;
      }

      

   }
   
   return true;
   
   
}



   void EmbossedTextOut(
      ::draw2d::graphics *        pgraphics,
      int32_t                 x,
      int32_t                 y,
      const char *            lpcsz,
      size_t                  cb,
      COLORREF                crText,
      COLORREF                crShadow,
      int32_t                 cx,
      int32_t                 cy)
   {


      /* If text length is -1, use lstrlen to get the length
       ** of the text.
       */
      if(cb == -1)
         cb = strlen(lpcsz);

      /* If the shadow or text color is -1, use the
       ** system color for that one.
       */

      //   COLORREF                crOld;
      //   UINT                    uMode;
      SIZED                    sizeText;
      RECT                    rcText;

      if(crShadow == (COLORREF)-1)
         crShadow = Sess(pgraphics->m_pauraapp).get_default_color(COLOR_BTNSHADOW);

      if(crText == (COLORREF)-1)
         crText = Sess(pgraphics->m_pauraapp).get_default_color(COLOR_BTNTEXT);

      /* setup the DC, saving off the old values
      */
      //uMode = pgraphics->SetBkMode(OPAQUE);
      //crOld = pgraphics->set_text_color(crShadow);

      /* draw the text at the desired offset using the
      ** shadow color, then again at the normal position
      ** using the text color.  This will the text an 'Embossed'
      ** or 'drop shadowed' look depending on what shadow color
      ** and offset are used.
      */
      sizeText = pgraphics->GetTextExtent(string(lpcsz,cb));
      rcText.left   = x;    rcText.right  = x + cx + sizeText.cx;
      rcText.top    = y;    rcText.bottom = y + cy + sizeText.cy;
      //ExtTextOut(hDC, x+cx, y+cy, ETO_OPAQUE, &rcText, lpsz, cb, NULL);
      //pgraphics->SetBkMode(TRANSPARENT);
      //ExtTextOut(hDC, x-cx, y+cy, NULL, &rcText, lpsz, cb, NULL);
      //ExtTextOut(hDC, x-cx, y-cy, NULL, &rcText, lpsz, cb, NULL);
      //ExtTextOut(hDC, x+cx, y-cy, NULL, &rcText, lpsz, cb, NULL);
      //ExtTextOut(hDC, x+cx, y+cy, NULL, &rcText, lpsz, cb, NULL);
      pgraphics->ExtTextOut(x + cx,y + cy,0,null_rect(),lpcsz,(int)cb,NULL);
      //pgraphics->SetBkMode(TRANSPARENT);
      //pgraphics->set_text_color(crText);
      if(!pgraphics->ExtTextOut(x,y,0,null_rect(),lpcsz,(int)cb,NULL))
      {
         //      TRACE("Failed to ExtTextOut, GetLastError() -->%d\n", GetLastError());
      }

      /* restore the DC
      */
      //pgraphics->set_text_color(crOld);
      //pgraphics->SetBkMode(uMode);


   }

   void GetMultiLineTextExtent(HDC hDC,stringa * pArray,LPSIZE lpSize)
   {

#ifdef WINDOWSEX

      TEXTMETRICW tm;

      GetTextMetricsW(hDC,&tm);

      index nIndex;
      ::count nSize;

      nSize = pArray->get_size();
      lpSize->cx = 0;
      size size;
      for(nIndex = 0; nIndex < nSize; nIndex++)
      {
         const string &str = pArray->get_at(nIndex);
         GetTextExtentPoint32(hDC,str,(int32_t)str.get_length(),&size);
         if(size.cx > lpSize->cx)
            lpSize->cx = size.cx;
      }
      //   lpSize->cy =
      //      (tm.tmHeight +
      //      tm.tmExternalLeading +
      //      tm.tmInternalLeading) *
      nSize;
      lpSize->cy = (LONG)((tm.tmHeight + tm.tmExternalLeading) * nSize);

#else

      throw todo(get_thread_app());

#endif

   }


   void DrawMultiLineText(HDC hDC,stringa * pArray)
   {

#ifdef WINDOWSEX

      TEXTMETRICW tm;

      GetTextMetricsW(hDC,&tm);

      index nIndex;
      ::count nSize;

      nSize = pArray->get_size();

      int32_t yPos = 0;

      for(nIndex = 0; nIndex < nSize; nIndex++)
      {

         const string &str = pArray->get_at(nIndex);

         TextOut(hDC,0,yPos,str,(int32_t)str.get_length());

         yPos +=
            tm.tmHeight +
            tm.tmExternalLeading +
            tm.tmInternalLeading;

      }

#else

      throw todo(get_thread_app());

#endif

   }

   bool imaging::GrayVRCP(
      ::draw2d::graphics * pgraphics,
      int32_t x,
      int32_t y,
      int32_t cx,
      int32_t cy,
      COLORREF crAlpha)
   {
      UNREFERENCED_PARAMETER(pgraphics);
      UNREFERENCED_PARAMETER(x);
      UNREFERENCED_PARAMETER(y);
      UNREFERENCED_PARAMETER(cx);
      UNREFERENCED_PARAMETER(cy);
      UNREFERENCED_PARAMETER(crAlpha);
      ::exception::throw_not_implemented(get_app());
      ASSERT(FALSE);

      return 0;

   }

   bool imaging::GrayVRCP(
      ::draw2d::graphics * pgraphics,
      ::draw2d::bitmap * pbitmap,
      ::draw2d::bitmap * pbitmapMask,
      const RECT & rect,
      COLORREF crTransparent)
   {

      return GrayVRCP(
         pgraphics,
         pbitmap,
         pbitmapMask,
         rect.left,
         rect.top,
         rect.right - rect.left,
         rect.bottom - rect.top,
         crTransparent);
   }

   /*sp(::image_list) imaging::CreateGrayVRCPImageList(
   ::draw2d::graphics * pgraphics,
   sp(::image_list) pilGray,
   sp(::image_list) pilParam)
   {
   sp(::image_list) pil = pilGray;

   pil->create(pilParam);

   ::draw2d::graphics_sp spgraphics(allocer());

   spgraphics->CreateCompatibleDC(pgraphics);

   ::image_list::info ii;

   for(int32_t i = 0; i < pil->get_image_count(); i++)
   {
   VERIFY(pil->get_image_info(i, &ii));
   rect rect = ii.m_rect;
   GrayVRCP(spgraphics->get_os_data(), ii.hbmImage, ii.hbmMask, rect.left, rect.top, rect.width(), rect.height(), RGB(192, 192, 192));
   }
   return pil;

   }


   bool imaging::CreateHueImageList(
   ::draw2d::graphics * pgraphics,
   sp(::image_list) pilGray,
   sp(::image_list) pilParam,
   COLORREF crHue,
   double dCompress)
   {
   sp(::image_list) pil = pilGray;

   if(!pil->create(pilParam))
   return false;

   ::draw2d::graphics_sp spgraphics(allocer());

   spgraphics->CreateCompatibleDC(pgraphics);

   ::image_list::info ii;

   for(int32_t i = 0; i < pil->get_image_count(); i++)
   {
   VERIFY(pil->get_image_info(i, &ii));
   rect rect = ii.m_rect;
   HueVRCP(
   spgraphics->get_os_data(),
   ii.hbmImage,
   rect.left,
   rect.top,
   rect.width(),
   rect.height(),
   crHue,
   dCompress);
   }
   return true;

   }*/



   bool imaging::CreateHueImageList(::draw2d::graphics * pgraphics, image_list * pilGray, image_list * pilParam,COLORREF crHue,double dCompress)
   {
      //synch_lock ml(&user_mutex());
      image_list * pil = pilGray;

      *pil = *pilParam;

      ::draw2d::graphics_sp spgraphics(allocer());

      spgraphics->CreateCompatibleDC(pgraphics);


      HueVRCP(pil->m_spdib,crHue,dCompress);

      return true;

   }


   bool imaging::Createcolor_blend_ImageList(image_list * pilGray, image_list * pilParam,COLORREF cr,BYTE bAlpha)
   {

      //synch_lock ml(&user_mutex());

      try
      {

         image_list * pil = pilGray;

         *pil = *pilParam;

         if(pil->m_spdib->get_graphics() == NULL)
            return false;

         if(pil->m_spdib->get_graphics()->get_os_data() == NULL)
            return false;

         ::draw2d::graphics_sp spgraphics(allocer());

#if defined(LINUX) || defined(APPLEOS) || defined(VSNORD)

         spgraphics->CreateCompatibleDC(NULL);

#elif defined(WINDOWS)

         spgraphics->CreateCompatibleDC(NULL);
         spgraphics->SetMapMode(MM_TEXT);

#else

         throw todo(get_app());

#endif

         //::draw2d::bitmap * pbitmapOld = spgraphics->get_current_bitmap();

         color_blend(pil->m_spdib->get_graphics(),null_point(),pil->m_spdib->size(),cr,bAlpha);

         //   pil->m_spdib->channel_from(visual::rgba::channel_alpha, pilParam->m_spdib);

         //spgraphics->SelectObject(pbitmapOld);

         return true;

      }
      catch(...)
      {
         return false;
      }

   }

   bool imaging::Createcolor_blend_dib(::draw2d::dib * pdib, COLORREF cr, BYTE bAlpha)
   {

      try
      {

         pdib->unmap();

         if (pdib->get_graphics() == NULL)
            return false;

         if (pdib->get_graphics()->get_os_data() == NULL)
            return false;

         color_blend(pdib->get_graphics(), null_point(), pdib->size(), cr, bAlpha);

         return true;

      }
      catch (...)
      {

         return false;

      }

   }


   /*
   sp(::image_list) imaging::CreateGrayVRCPImageList(
   ::draw2d::graphics * pgraphics,
   sp(::image_list) pilParam)
   {
   sp(::image_list) pil = new ::image_list();

   pil->create(pilParam);

   ::draw2d::graphics_sp spgraphics(allocer());

   spgraphics->CreateCompatibleDC(pgraphics);

   ::image_list::info ii;

   for(int32_t i = 0; i < pil->get_image_count(); i++)
   {
   VERIFY(pil->get_image_info(i, &ii));
   rect rect = ii.m_rect;
   GrayVRCP(spgraphics->get_os_data(), ii.hbmImage, ii.hbmMask, rect.left, rect.top, rect.width(), rect.height(), RGB(192, 192, 192));
   }
   return pil;

   }

   */


   bool imaging::GrayVRCP(
      ::draw2d::graphics * pgraphics,
      ::draw2d::bitmap * pbitmap,
      ::draw2d::bitmap * pbitmapMask,
      int32_t x,
      int32_t y,
      int32_t cx,
      int32_t cy,
      COLORREF crAlpha)
   {
      UNREFERENCED_PARAMETER(crAlpha);
      //COLORREF cr3dface = Session.get_default_color(COLOR_3DFACE);

#ifdef WINDOWSEX

      COLORREF cr3dshadow = Session.get_default_color(COLOR_3DSHADOW);

#else

      COLORREF cr3dshadow = ARGB(255,127,127,127);

#endif

      //BYTE uch3dfaceR = rgba_get_r(cr3dface);
      //BYTE uch3dfaceG = rgba_get_g(cr3dface);
      //BYTE uch3dfaceB = rgba_get_b(cr3dface);
      BYTE uch3dshadowR = rgba_get_r(cr3dshadow);
      BYTE uch3dshadowG = rgba_get_g(cr3dshadow);
      BYTE uch3dshadowB = rgba_get_b(cr3dshadow);


#ifdef WINDOWSEX

      COLORREF cr3dhighlight = Session.get_default_color(COLOR_3DHILIGHT);

#else

      COLORREF cr3dhighlight = ARGB(255,192,192,192);

#endif


      BYTE uch3dhighlightR = rgba_get_r(cr3dhighlight);
      BYTE uch3dhighlightG = rgba_get_g(cr3dhighlight);
      BYTE uch3dhighlightB = rgba_get_b(cr3dhighlight);

      class size size = pbitmap->get_size();

      UINT cbLine = ((size.cx * 3 + 3) & ~3);
      UINT cbImage = size.cy * cbLine;

      UINT cbMask = size.cy * ((size.cx + 3) & ~3);


      BITMAPINFO bmi;

      bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      bmi.bmiHeader.biWidth = size.cx;
      bmi.bmiHeader.biHeight = - size.cy;
      bmi.bmiHeader.biPlanes = 1;
      bmi.bmiHeader.biBitCount = 24;
      bmi.bmiHeader.biCompression = BI_RGB;
      bmi.bmiHeader.biSizeImage = 0;
      bmi.bmiHeader.biXPelsPerMeter = 1;
      bmi.bmiHeader.biYPelsPerMeter = 1;
      bmi.bmiHeader.biClrUsed = 0;
      bmi.bmiHeader.biClrImportant = 0;

      LPBYTE lpbData = (LPBYTE)malloc(cbImage);
      LPBYTE lpbShadow = (LPBYTE)malloc(cbMask);
      LPBYTE lpbMask = (LPBYTE)malloc(cbMask);
      //LPBYTE lpbShadow = lpbData;

#ifdef WINDOWSEX

      UINT uiScanLines = size.cy;


      if(!GetDIBits(
         (HDC)pgraphics->get_os_data(),
         (HBITMAP)pbitmap->get_os_data(),
         0,
         uiScanLines,
         lpbData,
         &bmi,
         DIB_RGB_COLORS))
      {
         return false;
      }

#else

      throw todo(get_app());

#endif

      class size sizeMask = pbitmapMask->get_size();

      BITMAPINFO * pbmiMask = (BITMAPINFO *)malloc(sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD));

      pbmiMask->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      pbmiMask->bmiHeader.biWidth = sizeMask.cx;
      pbmiMask->bmiHeader.biHeight = - sizeMask.cy;
      pbmiMask->bmiHeader.biPlanes = 1;
      pbmiMask->bmiHeader.biBitCount = 8;
      pbmiMask->bmiHeader.biCompression = BI_RGB;
      pbmiMask->bmiHeader.biSizeImage = 0;
      pbmiMask->bmiHeader.biXPelsPerMeter = 1;
      pbmiMask->bmiHeader.biYPelsPerMeter = 1;
      pbmiMask->bmiHeader.biClrUsed = 0;
      pbmiMask->bmiHeader.biClrImportant = 0;

#ifdef WINDOWSEX

      if(!GetDIBits((HDC)pgraphics->get_os_data(),
         (HBITMAP)pbitmapMask->get_os_data(),
         0,uiScanLines,
         lpbMask,pbmiMask,DIB_RGB_COLORS))
      {
         return false;
      }

#else

      throw todo(get_app());

#endif

      //   memcpy(lpbShadow, lpbData, cbImage);

      LPBYTE lpbBase = lpbData + cbLine * y + x * 3;
      LPBYTE lpbBaseShift = lpbData + cbLine * (y + 1) + (x + 1) * 3;
      LPBYTE lpbBaseShadow = lpbShadow + size.cx * y + x * 3;
      LPBYTE lpbBaseMask = lpbMask + size.cx * y + x;
      LPBYTE lpbBaseMaskShift = lpbMask + size.cx * (y + 1) + (x + 1);


      BYTE br,bg,bb;

      //   COLORREF crBtnFace = Session.get_default_color(COLOR_BTNFACE);
      //   COLORREF crBtnShad = Session.get_default_color(COLOR_BTNSHADOW);
      //   COLORREF crWndBack = Session.get_default_color(COLOR_WINDOW);

      //   BYTE bRBtnFace = rgba_get_r(crBtnFace);
      //   BYTE bGBtnFace = rgba_get_g(crBtnFace);
      //   BYTE bBBtnFace = rgba_get_b(crBtnFace);

      //   BYTE bRBtnShad = rgba_get_r(crBtnShad);
      //   BYTE bGBtnShad = rgba_get_g(crBtnShad);
      //   BYTE bBBtnShad = rgba_get_b(crBtnShad);

      //   BYTE bRWndBack = rgba_get_r(crWndBack);
      //   BYTE bGWndBack = rgba_get_g(crWndBack);
      //   BYTE bBWndBack = rgba_get_b(crWndBack);

      int32_t i;
      for(i = 0; i < cy; i ++)
      {
         LPBYTE lpbLine = lpbBase + cbLine * i;
         LPBYTE lpbLineShadow = lpbBaseShadow + size.cx * i;
         LPBYTE lpbLineMask = lpbBaseMask + size.cy * i;
         for(int32_t j = 0; j < cx; j++)
         {
            bb = *lpbLine++;
            bg = *lpbLine++;
            br = *lpbLine++;
            if((((br + bg + bb) / 3) < 192) &&
               (*lpbLineMask == 0))
            {
               *lpbLineShadow++ = 1;
            }
            else
            {
               *lpbLineShadow++ = 0;
            }
            lpbLineMask++;
         }
      }











      for(i = 0; i < cy; i ++)
      {
         LPBYTE lpbLine = lpbBase + cbLine * i;
         for(int32_t j = 0; j < cx; j++)
         {
            *lpbLine++ = 0;
            *lpbLine++ = 0;
            *lpbLine++ = 0;
         }
      }










      for(i = 0; i < cy; i ++)
      {
         LPBYTE lpbLineMask = lpbBaseMask + size.cx * i;
         for(int32_t j = 0; j < cx; j++)
         {
            *lpbLineMask++ = 1;
         }
      }




      int32_t cyminus1 = cy - 1;
      int32_t cxminus1 = cx - 1;
      BYTE b;

      for(i = 0; i < cyminus1; i ++)
      {
         LPBYTE lpbLine = lpbBaseShift + cbLine * i;
         LPBYTE lpbLineShadow = lpbBaseShadow + size.cx * i;
         LPBYTE lpbLineMask = lpbBaseMaskShift + size.cx * i;
         for(int32_t j = 0; j < cxminus1; j++)
         {
            b = *lpbLineShadow;
            lpbLineShadow++;

            if(b == 1)
            {
               *lpbLine++ = uch3dhighlightB;
               *lpbLine++ = uch3dhighlightG;
               *lpbLine++ = uch3dhighlightR;
               *lpbLineMask++ = 0;
            }
            else
            {
               lpbLine += 3;
               lpbLineMask++;
            }
         }
      }







      for(i = 0; i < cy; i ++)
      {
         LPBYTE lpbLine = lpbBase + cbLine * i;
         LPBYTE lpbLineShadow = lpbBaseShadow + size.cx * i;
         LPBYTE lpbLineMask = lpbBaseMask + size.cx * i;
         for(int32_t j = 0; j < cx; j++)
         {
            b = *lpbLineShadow;
            lpbLineShadow++;

            if(b == 1)
            {
               *lpbLine++ = uch3dshadowB;
               *lpbLine++ = uch3dshadowG;
               *lpbLine++ = uch3dshadowR;
               *lpbLineMask++ = 0;
            }
            else
            {
               lpbLine += 3;
               lpbLineMask++;
            }
         }
      }

#ifdef WINDOWSEX

      if(!SetDIBits(
         (HDC)pgraphics->get_os_data(),
         (HBITMAP)pbitmap->get_os_data(),
         0,
         uiScanLines,
         lpbData,
         &bmi,
         DIB_RGB_COLORS))
      {
         return false;
      }

      if(!SetDIBits(
         (HDC)pgraphics->get_os_data(),
         (HBITMAP)pbitmapMask->get_os_data(),
         0,
         uiScanLines,
         lpbMask,
         pbmiMask,
         DIB_RGB_COLORS))
      {
         return false;
      }

#else

      throw todo(get_app());

#endif

      ::free(lpbShadow);
      ::free(lpbData);
      ::free(lpbMask);
      ::free(pbmiMask);

      return true;

   }

   bool imaging::GetDeviceContext24BitsCC(
      ::draw2d::graphics *pgraphics,
      BITMAP & bm,
      BITMAPINFO & bmi,
      memory & memorystorage,
      ::draw2d::bitmap * pbitmap,
      const RECT & rectParam,
      int32_t &iWidthParam,
      UINT & uiStartScanLineParam,
      UINT & uiScanLineCountParam,
      int32_t & iLimitYParam)
   {
      UNREFERENCED_PARAMETER(pbitmap);
      rect rect(rectParam);

      //   int32_t x = rect.left;
      int32_t y = rect.top;
      //   int32_t cx = rect.width();
      int32_t cy = rect.height();

      ::draw2d::bitmap_sp spbmpTemp(get_app());
      if(!spbmpTemp->CreateCompatibleBitmap(pgraphics,1,1))
      {
         return false;
      }

      ::draw2d::bitmap * pbmpOld = pgraphics->SelectObject(spbmpTemp);

      if(pbmpOld != NULL)
      {
         try
         {
#ifdef WINDOWSEX
            ::draw2d::bitmap * pbmp = pbmpOld;
#endif

            ::exception::throw_not_implemented(get_app());
            /*         if(!pbmp->GetObject(sizeof(bm), &bm))
            {
            pgraphics->SelectObject(pbmpOld);
            return false;
            }*/

            bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
            bmi.bmiHeader.biWidth = bm.bmWidth ;
            bmi.bmiHeader.biHeight = bm.bmHeight;
            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biBitCount = 24;
            bmi.bmiHeader.biCompression = BI_RGB;
            bmi.bmiHeader.biSizeImage = 0;
            bmi.bmiHeader.biXPelsPerMeter = 0;
            bmi.bmiHeader.biYPelsPerMeter = 0;
            bmi.bmiHeader.biClrUsed = 0;
            bmi.bmiHeader.biClrImportant = 0;
            int32_t iLineBytes =  ((bm.bmWidth * 3) + 3) & ~3;
            iWidthParam = iLineBytes;
            try
            {
               memorystorage.allocate(iLineBytes * cy);
            }
            catch(memory_exception * pe)
            {
               delete pe;
               throw 4000;
            }
#ifdef WINDOWSEX

            LPVOID lpv = memorystorage.get_data();
#endif
            //       point pointViewport = pgraphics->GetViewportOrg();

            UINT uiStartScanLine = MAX(0,bm.bmHeight - y - cy);
            UINT uiScanLines = cy + MIN(0,bm.bmHeight - y - cy);

            uiStartScanLineParam = uiStartScanLine;
            uiScanLineCountParam = uiScanLines;

#ifdef WINDOWSEX

            if(!(iLimitYParam = GetDIBits
               (
               (HDC)pgraphics->get_os_data(),
               (HBITMAP)pbmp->get_os_data(),
               uiStartScanLine,
               uiScanLines,
               lpv,
               &bmi,
               DIB_RGB_COLORS
               )
               ))
            {
               pgraphics->SelectObject(pbmpOld);
               return false;
            }

#else

            throw todo(get_app());

#endif

         }
         catch(int32_t)
         {
            //         UINT user = GetLastError();
            pgraphics->SelectObject(pbmpOld);
            return false;
         }
         pgraphics->SelectObject(pbmpOld);
         return true;
      }
      else
      {
         pgraphics->SelectObject(pbmpOld);
         return false;
      }
   }

   /*bool imaging::GetDeviceContext24BitsAllCC(
   ::draw2d::graphics *pgraphics,
   BITMAP & bm,
   BITMAPINFO & bmi,
   memory & memorystorage,
   ::draw2d::bitmap   ** ppbitmap,
   LPRECT lprect,
   int32_t &iWidthParam,
   UINT & uiStartScanLineParam,
   UINT & uiScanLineCountParam,
   int32_t & iLimitYParam)
   {

   int32_t x = 0;
   int32_t y = 0;
   int32_t cx = -1;
   int32_t cy = -1;

   ::draw2d::bitmap * pbmpOld = pgraphics->get_current_bitmap();
   if(pbmpOld == NULL)
   {
   return false;
   }

   ::draw2d::bitmap spbmpTemp;
   if(!spbmpTemp->CreateCompatibleBitmap(pgraphics, 1, 1))
   {
   return false;
   }

   if(pgraphics->SelectObject(spbmpTemp))
   {
   // pgraphics is a pointer to a memory device context
   try
   {
   ::draw2d::bitmap * pbmp = pbmpOld;

   *ppbitmap = pbmp;

   // This is a memory device context
   if(!pbmp->GetObject(sizeof(bm), &bm))
   throw 3500;

   cx = bm.bmWidth;
   cy = bm.bmHeight;

   bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
   bmi.bmiHeader.biWidth = bm.bmWidth ;
   bmi.bmiHeader.biHeight = -bm.bmHeight;
   bmi.bmiHeader.biPlanes = 1;
   bmi.bmiHeader.biBitCount = 24;
   bmi.bmiHeader.biCompression = BI_RGB;
   bmi.bmiHeader.biSizeImage = 0;
   bmi.bmiHeader.biXPelsPerMeter = 0;
   bmi.bmiHeader.biYPelsPerMeter = 0;
   bmi.bmiHeader.biClrUsed = 0;
   bmi.bmiHeader.biClrImportant = 0;
   int32_t iLineBytes =  ((bm.bmWidth * 3) + 3) & ~3;
   iWidthParam = iLineBytes;
   try
   {
   memorystorage.allocate(iLineBytes * cy);
   }
   catch(memory_exception * pe)
   {
   delete pe;
   throw 4000;
   }
   LPVOID lpv = memorystorage.get_data();
   point pointViewport = pgraphics->GetViewportOrg();

   UINT uiStartScanLine = 0;
   UINT uiScanLines = cy;

   uiStartScanLineParam = uiStartScanLine;
   uiScanLineCountParam = uiScanLines;

   if(!(iLimitYParam = GetDIBits
   (
   (HDC)pgraphics->get_os_data(),
   (HBITMAP) pbmp->get_os_data(),
   uiStartScanLine,
   uiScanLines,
   lpv,
   &bmi,
   DIB_RGB_COLORS
   )
   ))
   throw 5000;
   }
   catch(int32_t)
   {
   UINT user = GetLastError();
   pgraphics->SelectObject(pbmpOld);
   return false;
   }
   }
   else
   {
   try
   {
   ::draw2d::bitmap * pbitmap = *ppbitmap;
   // this is a display spgraphics

   if(pbitmap->m_hObject == NULL)
   {
   pbitmap->CreateBitmap(cx + 100, cy + 100, 1, 24, NULL);
   }
   else
   {
   if(!pbitmap->GetObject(sizeof(bm), &bm))
   throw 3500;
   if(bm.bmWidth < cx ||
   bm.bmHeight  < cy)
   {
   pbitmap->delete_object();
   pbitmap->CreateBitmap(cx + 100, cy + 100, 1, 24, NULL);
   }
   }

   ::draw2d::graphics_sp graphicsMem(allocer());
   graphicsMem->CreateCompatibleDC(pgraphics);
   ::draw2d::bitmap * pbmpMemOld = graphicsMem->SelectObject(pbitmap);
   graphicsMem->BitBlt(
   0, 0,
   cx, cy,
   pgraphics,
   x, y,
   SRCCOPY);
   if(!pbitmap->GetObject(sizeof(bm), &bm))
   throw 3500;
   cx = bm.bmWidth;
   cy = bm.bmHeight;


   bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
   bmi.bmiHeader.biWidth = bm.bmWidth ;
   bmi.bmiHeader.biHeight = -bm.bmHeight;
   bmi.bmiHeader.biPlanes = 1;
   bmi.bmiHeader.biBitCount = 24;
   bmi.bmiHeader.biCompression = BI_RGB;
   bmi.bmiHeader.biSizeImage = 0;
   bmi.bmiHeader.biXPelsPerMeter = 0;
   bmi.bmiHeader.biYPelsPerMeter = 0;
   bmi.bmiHeader.biClrUsed = 0;
   bmi.bmiHeader.biClrImportant = 0;
   int32_t iLineBytes =  ((bm.bmWidth * 3) + 3) & ~3;
   iWidthParam = iLineBytes;
   try
   {
   memorystorage.allocate(iLineBytes * cy);
   }
   catch(memory_exception * pe)
   {
   delete pe;
   throw 4000;
   }

   LPVOID lpv = memorystorage.get_data();

   point pointViewport;
   pointViewport = pgraphics->GetViewportOrg();
   UINT uiStartScanLine = bm.bmHeight;

   uiStartScanLineParam = uiStartScanLine;

   //UINT uiStartScanLine = bm.bmHeight - y - cy;
   UINT uiScanLines = cy;
   uiScanLineCountParam = uiScanLines;
   int32_t xOffset = (x + pointViewport.x) * 3;
   //if(!GetDIBits(dcAux.get_os_data(), bitmap, bm.bmHeight - cy, cy, lpv, &bmi, DIB_RGB_COLORS))
   //  throw 5000;
   int32_t iLimitY = cy;
   if(!(iLimitYParam =
   GetDIBits(
   graphicsMem->get_os_data(),
   *pbitmap,
   uiStartScanLine,
   uiScanLines,
   lpv,
   &bmi,
   DIB_RGB_COLORS)))
   throw 5000;
   int32_t iLimitX = cx;

   if(bm.bmWidth - x - pointViewport.x < iLimitX)
   {
   iLimitX = bm.bmWidth - x - pointViewport.x;
   }

   graphicsMem->SelectObject(pbmpMemOld);
   graphicsMem->DeleteDC();
   pgraphics->SelectObject(pbmpOld);
   return true;
   }
   catch(int32_t)
   {
   return false;
   }
   }

   pgraphics->SelectObject(pbmpOld);
   return true;
   }*/

   void imaging::BitmapBlend24CC(
      LPBYTE lpbAParam,
      int32_t x1,
      int32_t y1,
      int32_t w1,
      int32_t cx,
      int32_t cy,
      LPBYTE lpbBParam,
      int32_t x2,
      int32_t y2,
      int32_t w2,
      LPBYTE lpbCParam,
      int32_t x3,
      int32_t y3,
      int32_t w3)
   {
      int32_t i,j;
      x1 *= 3;
      x2 *= 3;
      x3 *= 3;
      y1 *= w1;
      y2 *= w2;
      y3 *= w3;

      lpbAParam = (LPBYTE)lpbAParam + y1 + x1;
      lpbBParam = (LPBYTE)lpbBParam + y2 + x2;
      lpbCParam = (LPBYTE)lpbCParam + y3 + x3;

      //   BYTE dwA;
      for(i = 0; i < cy; i++)
      {
         LPBYTE lpbA = (LPBYTE)lpbAParam + (w1 * i);
         LPBYTE lpbB = (LPBYTE)lpbBParam + (w2 * i);
         LPBYTE lpbC = (LPBYTE)lpbCParam + (w3 * i);
         for(j = 0; j < cx; j++)
         {
            lpbA[0] = (BYTE)(((((uint32_t)lpbA[0]) * ((uint32_t)(255 - lpbC[0])) + (((uint32_t)lpbB[0]) * ((uint32_t)lpbC[0])))) / 255);
            lpbA[1] = (BYTE)(((((uint32_t)lpbA[1]) * ((uint32_t)(255 - lpbC[1])) + (((uint32_t)lpbB[1]) * ((uint32_t)lpbC[1])))) / 255);
            lpbA[2] = (BYTE)(((((uint32_t)lpbA[2]) * ((uint32_t)(255 - lpbC[2])) + (((uint32_t)lpbB[2]) * ((uint32_t)lpbC[2])))) / 255);
            lpbA+=3;
            lpbB+=3;
            lpbC+=3;

         }
      }

   }


   void imaging::BitmapBlend24CC(
      LPBYTE lpbDestParam,
      int32_t xDest,
      int32_t yDest,
      int32_t wDest,
      int32_t cx,
      int32_t cy,
      LPBYTE lpbSrcParam,
      int32_t xSrc,
      int32_t ySrc,
      int32_t wSrc,
      BYTE bAlpha)
   {
      int32_t i,j;
      xDest *= 3;
      xSrc *= 3;
      yDest *= wDest;
      ySrc *= wSrc;

      //   int32_t maxw = cx * 3;
      //   int32_t cw = (maxw) & ~3;



      int32_t iDestPadding = wDest - cx * 3;
      int32_t iSrcPadding = wSrc - cx * 3;


      lpbDestParam = (LPBYTE)lpbDestParam + yDest + xDest;
      lpbSrcParam = (LPBYTE)lpbSrcParam + ySrc + xSrc;

      //   BYTE bAlphaComplement = 255 - bAlpha;
      //   BYTE dwA;
      if(bAlpha == 0)
      {
      }
      if(bAlpha == 255)
      {
         for(i = 0; i < cy; i++)
         {
            LPBYTE lpbDst = (LPBYTE)lpbDestParam + (wDest * i);
            LPBYTE lpbSrc = (LPBYTE)lpbSrcParam + (wSrc * i);
            for(j = 0; j < cx; j++)
            {
               *lpbDst++ = *lpbSrc++;
               *lpbDst++ = *lpbSrc++;
               *lpbDst++ = *lpbSrc++;
            }
         }
      }
      else
      {
         LPBYTE lpbDst = (LPBYTE)lpbDestParam;
         LPBYTE lpbSrc = (LPBYTE)lpbSrcParam;
         for(i = 0; i < cy; i++)
         {

            for(j = 0; j < cx; j++)
            {
               lpbDst[0] = (BYTE)(((((int32_t)lpbSrc[0] - lpbDst[0]) * ((int32_t)(bAlpha))) / 256) + (int32_t)lpbDst[0]);
               lpbDst[1] = (BYTE)(((((int32_t)lpbSrc[1] - lpbDst[1]) * ((int32_t)(bAlpha))) / 256) + (int32_t)lpbDst[1]);
               lpbDst[2] = (BYTE)(((((int32_t)lpbSrc[2] - lpbDst[2]) * ((int32_t)(bAlpha))) / 256) + (int32_t)lpbDst[2]);
               lpbDst+=3;
               lpbSrc+=3;
            }
            lpbDst = (LPBYTE)lpbDst + iDestPadding;
            lpbSrc = (LPBYTE)lpbSrc + iSrcPadding;
         }
         /*      lpushDest = (uint16_t *) lpbDestParam;
         lpushSrc = (uint16_t *) lpbSrcParam;
         for(i = 0; i < cy; i++)
         {
         for(j = 0; j < cw; j+=2)
         {
         dwSrc = (*lpushSrc & 0xff) | ((*lpushSrc & 0xff00) << 16);
         dwDest = (*lpushDest & 0xff) | ((*lpushDest & 0xff00) << 16);
         dwDest = (((dwSrc - dwDest) * bAlpha) / 256) + dwDest;
         *lpushDest = (dwDest & 0xff) | ((dwDest & 0x00ff0000) >> 16);
         lpushSrc++;
         lpushDest++;
         }
         LPBYTE lpbDst = (LPBYTE) lpushDest;
         LPBYTE lpbSrc = (LPBYTE) lpushSrc;
         for(;j < maxw; j++)
         {
         *lpbDst++ = (BYTE) (((uint32_t) (*lpbSrc++ - *lpbDst) * ((uint32_t) (bAlpha))) / 256) + *lpbDst;
         }
         lpbDst = (LPBYTE) lpbDestParam + iDestPadding;
         lpbSrc = (LPBYTE) lpbSrcParam + iSrcPadding;
         }*/
      }

   }


   /*bool imaging::bitmap_blend(
   ::draw2d::graphics * pdcDst, // destination device
   point pt,
   size size,
   ::draw2d::graphics * pdcSrc, // source device
   point ptSrc,
   ::draw2d::graphics * pdcAlpha, // alpha information device (in alpha channel)
   point ptAlpha,
   ::draw2d::graphics * pdcAlphaComplement, // alpha information device (in alpha channel)
   point ptAlphaComplement)
   {

   UNREFERENCED_PARAMETER(ptAlphaComplement);
   ::draw2d::dib_sp dibA(get_app());

   if(!dibA->create(size))
   return false;

   ::draw2d::dib_sp dibB(get_app());

   if(!dibB->create(size))
   return false;

   //::draw2d::bitmap * pbmpOldA = (::draw2d::bitmap *) dibA->get_graphics()->SelectObject(dibA->get_graphics());
   //::draw2d::bitmap * pbmpOldB = (::draw2d::bitmap *) dibB->get_graphics()->SelectObject(dibB->get_graphics());

   dibB->get_graphics()->from(null_point(), size, pdcSrc, ptSrc, SRCCOPY);


   dibA->get_graphics()->from(null_point(), size, pdcAlphaComplement, ptAlpha, SRCCOPY);

   GdiFlush();


   //      int64_t a = size.area();

   BLENDFUNCTION bf;
   bf.BlendOp     = AC_SRC_OVER;
   bf.BlendFlags  = 0;
   bf.SourceConstantAlpha = 0xFF;
   bf.AlphaFormat = AC_SRC_ALPHA;

   bool bOk;

   try
   {
   dibB->get_graphics()->BitBlt(0, 0, size.cx, size.cy, dibA->get_graphics(), 0, 0, SRCCOPY);
   }
   catch(...)
   {
   bOk = false;
   }


   GdiFlush();

   dibA->get_graphics()->FillSolidRect(0, 0, size.cx, size.cy, RGB(255, 255, 255));
   dibB->get_graphics()->from(null_point(), size, dibA->get_graphics(), null_point(), SRCAND);

   //dibA->get_graphics()->BitBlt(0, 0, cx, cy, pdcAlpha, x3, y3, SRCCOPY);
   dibB->get_graphics()->from(null_point(), size, pdcAlpha, ptAlpha, SRCPAINT);

   try
   {
   pdcDst->alpha_blend(pt, size, dibB->get_graphics(), bf);
   }
   catch(...)
   {
   bOk = false;
   }

   GdiFlush();


   return true;
   }*/

   /*bool imaging::bitmap_blend(
   ::draw2d::graphics * pdcA, // destination device
   point pt,
   size size,
   ::draw2d::graphics * pdcB, // source device
   point ptSrc,
   ::draw2d::graphics * pdcC, // alpha information device
   point ptAlpha,
   byte uchAlphaConstant,
   ::draw2d::dib * pdibA,
   ::draw2d::dib * pdibB)
   {
   ::draw2d::dib_sp dibA;
   ::draw2d::dib_sp dibB;

   if(pdibA == NULL)
   {
   dibA.alloc(allocer());
   pdibA = dibA;
   }
   if(pdibB == NULL)
   {
   dibB.alloc(allocer());
   pdibB = dibB;
   }


   if(!pdibA->create(size))
   return false;
   if(!pdibB->create(size))
   return false;


   pdibB->get_graphics()->from(null_point(), size, pdcB, ptSrc, SRCCOPY);


   pdibA->get_graphics()->from(null_point(), size, pdcC, ptAlpha, SRCCOPY);

   GdiFlush();


   UINT32 * lpuiA = (UINT32 *) pdibA->get_data();

   int64_t a = size.area();
   int64_t i;

   int64_t b = a - a % 4;
   for(i = 0; i < b; i +=4)
   {
   lpuiA[0] = 0xff000000 - ((lpuiA[0]) & 0xff000000);
   lpuiA[1] = 0xff000000 - ((lpuiA[1]) & 0xff000000);
   lpuiA[2] = 0xff000000 - ((lpuiA[2]) & 0xff000000);
   lpuiA[3] = 0xff000000 - ((lpuiA[3]) & 0xff000000);
   lpuiA += 4;
   }
   for (; i < a; i++)
   {
   *lpuiA++ = 0xff000000 - ((*lpuiA) & 0xff000000);
   }

   BLENDFUNCTION bf;
   bf.BlendOp     = AC_SRC_OVER;
   bf.BlendFlags  = 0;
   bf.SourceConstantAlpha = uchAlphaConstant;
   bf.AlphaFormat = AC_SRC_ALPHA;

   bool bOk;
   try
   {
   pdibB->get_graphics()->alpha_blend(size, pdibA->get_graphics(), bf);
   }
   catch(...)
   {
   bOk = false;
   }


   GdiFlush();

   lpuiA = (UINT32 *) pdibA->get_data();
   UINT32 * lpuiB = (UINT32 *) pdibB->get_data();

   for (i = 0; i < b; i +=4)
   {
   lpuiA[0] = (0xff000000 - (lpuiA[0] & 0xff000000)) | (lpuiB[0]  & 0x00ffffff);
   lpuiA[1] = (0xff000000 - (lpuiA[1] & 0xff000000)) | (lpuiB[1]  & 0x00ffffff);
   lpuiA[2] = (0xff000000 - (lpuiA[2] & 0xff000000)) | (lpuiB[2]  & 0x00ffffff);
   lpuiA[3] = (0xff000000 - (lpuiA[3] & 0xff000000)) | (lpuiB[3]  & 0x00ffffff);
   lpuiA += 4;
   lpuiB += 4;
   }
   for (; i < a; i++)
   {
   *lpuiA++ = (0xff000000 - (*lpuiA & 0xff000000)) | (*lpuiB++  & 0x00ffffff);
   }

   try
   {
   pdcA->alpha_blend(pt, size, pdibA->get_graphics(), bf);
   }
   catch(...)
   {
   bOk = false;
   }


   GdiFlush();


   return true;


   }
   */

   /*bool imaging::bitmap_blend(
   ::draw2d::graphics * pdcA, // destination device
   point pt,
   size size,
   ::draw2d::dib * pdibB, // source device
   point ptSrc,
   ::draw2d::dib * pdibC, // alpha information device
   point ptAlpha,
   byte uchAlphaConstant)
   {

   UNREFERENCED_PARAMETER(ptSrc);
   UNREFERENCED_PARAMETER(ptAlpha);

   GdiFlush();


   UINT32 * lpuiA = (UINT32 *) pdibC->get_data();

   int64_t a = size.area();
   int64_t i;

   int64_t b = a - a % 4;
   for(i = 0; i < b; i +=4)
   {
   lpuiA[0] = 0xff000000 - ((lpuiA[0]) & 0xff000000);
   lpuiA[1] = 0xff000000 - ((lpuiA[1]) & 0xff000000);
   lpuiA[2] = 0xff000000 - ((lpuiA[2]) & 0xff000000);
   lpuiA[3] = 0xff000000 - ((lpuiA[3]) & 0xff000000);
   lpuiA += 4;
   }
   for (; i < a; i++)
   {
   *lpuiA++ = 0xff000000 - ((*lpuiA) & 0xff000000);
   }

   BLENDFUNCTION bf;
   bf.BlendOp     = AC_SRC_OVER;
   bf.BlendFlags  = 0;
   bf.SourceConstantAlpha = uchAlphaConstant;
   bf.AlphaFormat = AC_SRC_ALPHA;

   bool bOk;
   try
   {
   pdibB->get_graphics()->alpha_blend(size, pdibC->get_graphics(), bf);
   }
   catch(...)
   {
   bOk = false;
   }


   GdiFlush();

   lpuiA = (UINT32 *) pdibC->get_data();
   UINT32 * lpuiB = (UINT32 *) pdibB->get_data();

   for (i = 0; i < b; i +=4)
   {
   lpuiA[0] = (0xff000000 - (lpuiA[0] & 0xff000000)) | (lpuiB[0]  & 0x00ffffff);
   lpuiA[1] = (0xff000000 - (lpuiA[1] & 0xff000000)) | (lpuiB[1]  & 0x00ffffff);
   lpuiA[2] = (0xff000000 - (lpuiA[2] & 0xff000000)) | (lpuiB[2]  & 0x00ffffff);
   lpuiA[3] = (0xff000000 - (lpuiA[3] & 0xff000000)) | (lpuiB[3]  & 0x00ffffff);
   lpuiA += 4;
   lpuiB += 4;
   }
   for (; i < a; i++)
   {
   *lpuiA++ = (0xff000000 - (*lpuiA & 0xff000000)) | (*lpuiB++  & 0x00ffffff);
   }

   try
   {
   pdcA->alpha_blend(pt, size, pdibC->get_graphics(), bf);
   }
   catch(...)
   {
   bOk = false;
   }
   GdiFlush();


   return true;


   }

   bool imaging::BitmapBlend2(
   ::draw2d::graphics * pdcA, // destination device
   point pt,
   size size,
   ::draw2d::graphics * pdcB, // source device
   point ptSrc,
   ::draw2d::graphics * pdcC, // alpha information device
   point ptAlpha)
   {
   BITMAPINFO bmi;

   // zero the memory for the bitmap info
   ZeroMemory(&bmi, sizeof(BITMAPINFO));

   ::draw2d::dib_sp dibA(get_app());

   if(!dibA->create(size))
   return false;

   ::draw2d::dib_sp dibB(get_app());

   if(!dibB->create(size))
   return false;

   dibB->get_graphics()->from(size, pdcB, ptSrc, SRCCOPY);

   dibA->get_graphics()->from(size, pdcC, ptAlpha, SRCCOPY);

   GdiFlush();


   UINT32 * lpuiA = (UINT32 *) dibA->get_data();

   //   int64_t a = size.area();
   //      int32_t i;

   //   int64_t b = a - 4;
   /*      for (i = 0; i < b; i +=4)
   {
   lpuiA[0] = ((lpuiA[0] << 8) & 0xff000000);
   lpuiA[1] = ((lpuiA[1] << 8) & 0xff000000);
   lpuiA[2] = ((lpuiA[2] << 8) & 0xff000000);
   lpuiA[3] = ((lpuiA[3] << 8) & 0xff000000);
   lpuiA += 4;
   }
   for (; i < a; i++)
   {
   *lpuiA++ = ((*lpuiA << 8) & 0xff000000);
   }

   rect rect(0, 0, cx, cy);
   dibA->get_graphics()->InvertRect(rect);*/

   /* BLENDFUNCTION bf;
   bf.BlendOp     = AC_SRC_OVER;
   bf.BlendFlags  = 0;
   bf.SourceConstantAlpha = 0xFF;
   bf.AlphaFormat = AC_SRC_ALPHA;

   bool bOk;
   try
   {
   dibB->get_graphics()->alpha_blend(size, dibA->get_graphics(), bf);
   }
   catch(...)
   {
   bOk = false;
   }


   GdiFlush();

   lpuiA = (UINT32 *) dibA->get_data();
   //   UINT32 * lpuiB = (UINT32 *) dibB->get_data();

   rect rect(null_point(), size);
   //      dibA->get_graphics()->InvertRect(rect);

   ::draw2d::dib_sp dibPat(get_app());
   dibPat->create(64, 64);
   dibPat->Fill(255, 0, 0, 0);

   ::draw2d::brush_sp brush(allocer());
   brush->CreatePatternBrush(dibPat->get_bitmap());

   ::draw2d::dib_sp spdib(allocer());
   spdib->create(size);

   spdib->get_graphics()->FillRect(rect, brush);

   try
   {
   pdcA->alpha_blend(pt, size, dibA->get_graphics(), bf);
   }
   catch(...)
   {
   bOk = false;
   }

   GdiFlush();

   return true;



   }

   */
   /*bool imaging::bitmap_blend(
      ::draw2d::graphics * pdcDst, // destination device
      point pt,
      size size,
      ::draw2d::graphics * pdcSrcWithAlpha, // source device
      point ptSrc)
      {
      return color_blend(pdcDst, pt, size, pdcSrcWithAlpha, ptSrc);
      }*/

   bool imaging::BitmapDivBlend(
      ::draw2d::graphics * pdcDst, // destination device
      point ptDst,
      size size,
      ::draw2d::graphics * pdcSrc, // source device
      point ptSrc,
      BYTE bAlpha)
   {

      visual::dib_sp spdib(allocer());

      spdib->create(size);

      spdib->to(pdcSrc,size);

      spdib->DivideRGB(bAlpha);

      return pdcDst->BitBlt(ptDst.x,ptDst.y,size.cx,size.cy,spdib->get_graphics(),ptSrc.x,ptSrc.y,SRCCOPY);

   }


   bool imaging::bitmap_blend(
      ::draw2d::graphics * pdcDst, // destination device
      point ptDst,
      size size,
      ::draw2d::graphics * pdcSrc, // source device
      point ptSrc,
      BYTE bAlpha)
   {

      return pdcDst->alpha_blend(ptDst,size,pdcSrc,ptSrc,bAlpha / 255.0);

   }



   bool imaging::ColorInvert(::draw2d::graphics * pgraphics,int32_t x,int32_t y,int32_t cx,int32_t cy)
   {


      if(cx <= 0 || cy <= 0)
         return true;

      return false;

      //    single_lock sl(&m_csMem, TRUE);

      /*int32_t iOriginalMapMode ;

      ::draw2d::bitmap_sp spbmpTemp(get_app());

      ::draw2d::bitmap_sp bitmapA(get_app());

      iOriginalMapMode = pgraphics->GetMapMode();
      pgraphics->SetMapMode(MM_TEXT);
      if(!spbmpTemp->CreateCompatibleBitmap(pgraphics, 1, 1))
      {
      pgraphics->SetMapMode(iOriginalMapMode);
      return false;
      }

      if(!pgraphics->SelectObject(spbmpTemp))
      {
      try
      {
      // this is a display spgraphics

      if(bitmapA->get_os_data() == NULL)
      {
      bitmapA->CreateBitmap(cx + 100, cy + 100, 1, 24, NULL);
      }
      else
      {
      BITMAP bm;
      if(!bitmapA->GetObject(sizeof(bm), &bm))
      throw 3500;
      if(bm.bmWidth < cx ||
      bm.bmHeight  < cy)
      {
      bitmapA->CreateBitmap(cx + 100, cy + 100, 1, 24, NULL);
      }
      }

      ::draw2d::graphics_sp graphicsMem(allocer());
      graphicsMem->CreateCompatibleDC(pgraphics);
      ::draw2d::bitmap * pbmpMemOld = graphicsMem->SelectObject(bitmapA);
      graphicsMem->BitBlt(
      0, 0,
      cx, cy,
      pgraphics,
      x, y,
      SRCCOPY);
      BITMAP bm;
      if(!bitmapA->GetObject(sizeof(bm), &bm))
      throw 3500;

      memory memstorageA;


      BITMAPINFO bmi;
      bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
      bmi.bmiHeader.biWidth = bm.bmWidth ;
      bmi.bmiHeader.biHeight = -bm.bmHeight;
      bmi.bmiHeader.biPlanes = 1;
      bmi.bmiHeader.biBitCount = 24;
      bmi.bmiHeader.biCompression = BI_RGB;
      bmi.bmiHeader.biSizeImage = 0;
      bmi.bmiHeader.biXPelsPerMeter = 0;
      bmi.bmiHeader.biYPelsPerMeter = 0;
      bmi.bmiHeader.biClrUsed = 0;
      bmi.bmiHeader.biClrImportant = 0;
      int32_t iLineBytes =  (bm.bmWidthBytes + 3) & ~3;
      try
      {
      memstorageA.allocate(iLineBytes * cy);
      }
      catch(memory_exception * pe)
      {
      delete pe;
      throw 4000;
      }

      LPVOID lpv = memstorageA.get_data();

      point pointViewport;
      pointViewport = pgraphics->GetViewportOrg();
      UINT uiStartScanLine = bm.bmHeight - y - cy - pointViewport.y;
      //UINT uiStartScanLine = bm.bmHeight - y - cy;
      UINT uiScanLines = cy;
      int32_t xOffset = (x + pointViewport.x) * 3;
      //if(!GetDIBits(dcAux.get_os_data(), bitmapA, bm.bmHeight - cy, cy, lpv, &bmi, DIB_RGB_COLORS))
      //  throw 5000;
      int32_t iLimitY = cy;
      if(!(iLimitY =
      GetDIBits(
      (HDC)graphicsMem->get_os_data(),
      (HBITMAP)bitmapA->get_os_data(),
      uiStartScanLine,
      uiScanLines,
      lpv,
      &bmi,
      DIB_RGB_COLORS)))
      return false;
      int32_t iLimitX = cx;

      if(bm.bmWidth - x - pointViewport.x < iLimitX)
      {
      iLimitX = bm.bmWidth - x - pointViewport.x;
      }

      LPBYTE lpb;
      int32_t i, j;
      for(i = 0; i < iLimitY; i++)
      {
      lpb = (LPBYTE) lpv + (iLineBytes * i) + xOffset;
      for(j = 0; j < iLimitX; j++)
      {
      *lpb++ = (BYTE) 255 - *lpb;
      *lpb++ = (BYTE) 255 - *lpb;
      *lpb++ = (BYTE) 255 - *lpb;
      }
      }
      if(!SetDIBits(
      (HDC)graphicsMem->get_os_data(),
      (HBITMAP)bitmapA->get_os_data(),
      uiStartScanLine,
      uiScanLines,
      lpv,
      &bmi,
      DIB_RGB_COLORS))
      throw 6000;
      pgraphics->BitBlt(x, y, cx, cy, graphicsMem, 0, 0, SRCCOPY);
      graphicsMem->SelectObject(pbmpMemOld);
      graphicsMem->DeleteDC();
      pgraphics->SetMapMode(iOriginalMapMode);
      return true;
      }
      catch(int32_t)
      {
      pgraphics->SetMapMode(iOriginalMapMode);
      return false;
      }
      }
      else
      {
      try
      {

      // This is a memory device context
      BITMAP bm;
      if(!pbmp->GetObject(sizeof(bm), &bm))
      throw 3500;

      BITMAPINFO bmi;
      bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
      bmi.bmiHeader.biWidth = bm.bmWidth ;
      bmi.bmiHeader.biHeight = -bm.bmHeight;
      bmi.bmiHeader.biPlanes = 1;
      bmi.bmiHeader.biBitCount = 24;
      bmi.bmiHeader.biCompression = BI_RGB;
      bmi.bmiHeader.biSizeImage = 0;
      bmi.bmiHeader.biXPelsPerMeter = 0;
      bmi.bmiHeader.biYPelsPerMeter = 0;
      bmi.bmiHeader.biClrUsed = 0;
      bmi.bmiHeader.biClrImportant = 0;
      int32_t iLineBytes =  ((bm.bmWidth * 3) + 3) & ~3;

      memory memstorageA;
      try
      {
      memstorageA.allocate(iLineBytes * cy);
      }
      catch(memory_exception * pe)
      {
      delete pe;
      throw 4000;
      }
      LPVOID lpv = memstorageA.get_data();
      point pointViewport = pgraphics->GetViewportOrg();
      UINT uiStartScanLine = bm.bmHeight - y - cy - pointViewport.y;
      //UINT uiStartScanLine = bm.bmHeight - y - cy;
      UINT uiScanLines = cy;
      int32_t xOffset = (x + pointViewport.x) * 3;


      //int32_t xOffset = (x) * 3;
      int32_t iLimitY = cy;
      if(!(iLimitY =
      GetDIBits(
      (HDC)pgraphics->get_os_data(),
      (HBITMAP) pbmp->get_os_data(),
      uiStartScanLine,
      uiScanLines,
      lpv,
      &bmi,
      DIB_RGB_COLORS)))
      throw 5000;
      int32_t iLimitX = cx;


      if(bm.bmWidth - x - pointViewport.x < iLimitX)
      {
      iLimitX = bm.bmWidth - x - pointViewport.x;
      }

      LPBYTE lpb;
      int32_t i, j;
      for(i = 0; i < iLimitY; i++)
      {
      lpb = (LPBYTE) lpv + (iLineBytes * i) + xOffset;
      if(lpb < lpv)
      continue;
      for(j = 0; j < iLimitX; j++)
      {
      *lpb++ = (BYTE) 255 - *lpb ;
      *lpb++ = (BYTE) 255 - *lpb;
      *lpb++ = (BYTE) 255 - *lpb;
      }
      }

      if(!SetDIBits((HDC)pgraphics->get_os_data(),
      (HBITMAP) pbmp->get_os_data(),
      uiStartScanLine, uiScanLines,
      lpv, &bmi, DIB_RGB_COLORS))
      throw 6000;
      }
      catch(int32_t)
      {
      pgraphics->SetMapMode(iOriginalMapMode);
      return false;
      }
      }
      pgraphics->SetMapMode(iOriginalMapMode);
      return true;*/
   }

   /*bool imaging::allocate(int32_t iSize)
   {
   if(iSize > m_iSize)
   {
   m_lpVoid = malloc(iSize);
   if(m_lpVoid == NULL)
   {
   return false;
   }
   else
   {
   m_iSize = iSize;
   }
   }
   return true;

   }*/

//   void imaging::SaveJpeg(const char * lpcszFile,::draw2d::bitmap_sp pbitmap)
//   {
//
//#ifdef METROWIN
//
//      throw todo(get_app());
//
//#else
//      FIBITMAP * fi= HBITMAPtoFI(pbitmap);
//      fi = FreeImage_ConvertTo24Bits(fi);
//      if(FreeImage_Save(FIF_JPEG,fi,lpcszFile,JPEG_QUALITYBAD))
//      {
//         //
//      }
//      FreeImage_Unload(fi);
//#endif
//#endif
//   }
//
//   void imaging::SavePng(const char * lpcszFile,::draw2d::bitmap_sp pbitmap)
//   {
//
//#ifdef METROWIN
//
//      throw todo(get_app());
//
//#else
//      FIBITMAP * fi= HBITMAPtoFI(pbitmap);
//      if(FreeImage_Save(FIF_PNG,fi,lpcszFile,0))
//      {
//         //
//      }
//      FreeImage_Unload(fi);
//#endif
//#endif
//
//   }



//   ::draw2d::bitmap_sp imaging::LoadImageSync(const char * lpcszImageFilePath,::aura::application * papp)
//   {
//#ifdef AXIS_FREEIMAGE
//      FIBITMAP * pfi = imaging::LoadImageFile(lpcszImageFilePath,papp);
//
//      if(pfi == NULL)
//         return NULL;
//
//      return FItoHBITMAP(pfi,true);
//#endif
//      return NULL;
//   }




   bool imaging::color_blend_3dRect(::draw2d::graphics *pgraphics,const RECT & rectParam,COLORREF crTopLeft,BYTE bAlphaTopLeft,COLORREF crBottomRight,BYTE bAlphaBottomRight)
   {
      ::rect rect(rectParam);
      int32_t x = rect.left;
      int32_t y = rect.top;
      int32_t cx = rect.width();
      int32_t cy = rect.height();
      color_blend(pgraphics,point(x,y),size(cx - 1,1),crTopLeft,bAlphaTopLeft);
      color_blend(pgraphics,point(x,y),size(1,cy - 1),crTopLeft,bAlphaTopLeft);
      color_blend(pgraphics,point(x + cx - 1,y + 1),size(1,cy - 1),crBottomRight,bAlphaBottomRight);
      color_blend(pgraphics,point(x + 1,y + cy - 1),size(cx - 1,1),crBottomRight,bAlphaBottomRight);
      return true;
   }

   bool imaging::clip_color_blend(
      ::draw2d::graphics * pgraphics,
      const RECT & rectParam,
      COLORREF cr,
      BYTE alpha)
   {
      class ::rect rect(rectParam);
      return clip_color_blend(
         pgraphics,
         rect.top_left(),
         rect.size(),
         cr,
         alpha);
   }


   bool imaging::clip_color_blend(::draw2d::graphics * pgraphics,point pt,size size,COLORREF cr,BYTE bA)
   {

      pgraphics->FillSolidRect(0,0,size.cx,size.cy,ARGB(bA,argb_get_r_value(cr),argb_get_g_value(cr),argb_get_b_value(cr)));

      return true;

   }

   /*

   bool imaging::clip_color_blend(::draw2d::graphics * pgraphics, const RECT & rect, COLORREF cr, BYTE alpha, ::draw2d::region * prgnClip)
   {

   class rect rect(lpcrect);

   return clip_color_blend(pgraphics, rect.top_left(), rect.size(), cr, alpha, prgnClip);

   }


   bool imaging::clip_color_blend(::draw2d::graphics * pgraphics, point pt, size size, COLORREF cr, BYTE bA, ::draw2d::region * prgnClip)
   {
   ::draw2d::bitmap_sp bitmapA(get_app());

   BITMAP   bm;

   rect rect(pt, size);

   point ptViewport = pgraphics->GetViewportOrg();

   rect.offset(ptViewport);

   ClipSave(
   pgraphics,
   bitmapA,
   NULL,
   &bm,
   rect,
   prgnClip);

   bool bOk = color_blend(pgraphics, pt, size, cr, bA);

   ClipRestore(
   pgraphics,
   bitmapA,
   NULL,
   &bm,
   rect,
   prgnClip);

   pgraphics->SetViewportOrg(ptViewport);

   return bOk;
   }

   */

   bool imaging::color_blend(
      ::draw2d::graphics * pgraphics,
      const RECT64 * lpcrect,
      COLORREF cr,
      BYTE alpha)
   {
      rect rect32;
      ::copy(rect32,lpcrect);
      return color_blend(pgraphics,rect32,cr,alpha);
   }


   bool imaging::color_blend(
      ::draw2d::graphics * pgraphics,
      const RECT & rectParam,
      COLORREF cr,
      BYTE alpha)
   {
      class rect rect(rectParam);
      return color_blend(
         pgraphics,
         rect.top_left(),
         rect.size(),
         cr,
         alpha);
   }


   bool imaging::color_blend(::draw2d::graphics * pgraphics,int32_t x,int32_t y,int32_t cx,int32_t cy,COLORREF cr,BYTE bA)
   {

      return color_blend(pgraphics,point(x,y),size(cx,cy),cr,bA);

   }

   bool imaging::color_blend(::draw2d::graphics * pgraphics,point pt,size size,COLORREF cr,BYTE bA)
   {

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

      pgraphics->FillSolidRect(pt.x,pt.y,size.cx,size.cy,(cr & 0x00ffffff) | (bA << 24));

      return true;

   }




   /*

   bool imaging::ClipSave(
   ::draw2d::graphics * pgraphics,
   ::draw2d::bitmap * pbitmap,
   ::draw2d::bitmap * pbitmapOld,
   BITMAP * pbmp,
   const RECT & rect,
   ::draw2d::region * prgnClip)
   {

   if(pgraphics == NULL)
   return false;

   if(pbitmap == NULL)
   return false;

   if(prgnClip == NULL)
   return false;


   rect rectClipBox;

   prgnClip->get_bounding_box(rectClipBox);

   rect rectUpdate;

   rectUpdate.intersect(rectClipBox, lpcrect);

   //   if(!rectUpdate.is_empty())
   //   {
   ::draw2d::graphics_sp spgraphics(allocer());
   spgraphics->CreateCompatibleDC(pgraphics);
   spgraphics->SetViewportOrg(pgraphics->GetViewportOrg());
   if(!CreateBitmap(
   spgraphics,
   pgraphics,
   pbitmap,
   pbitmapOld,
   pbmp,
   rectUpdate.right,
   rectUpdate.bottom))
   {
   return false;
   }

   ::draw2d::region rgnUpdate;

   rgnUpdate.create_rect(lpcrect);

   rgnUpdate.combine(&rgnUpdate, prgnClip, ::draw2d::region::combine_exclude);

   rgnUpdate.get_bounding_box(rectUpdate);

   spgraphics->SelectObject(pbitmap);

   spgraphics->SelectClipRgn(&rgnUpdate);

   if(!spgraphics->BitBlt(
   rectUpdate.left,
   rectUpdate.top,
   rectUpdate.width(),
   rectUpdate.height(),
   pgraphics,
   rectUpdate.left,
   rectUpdate.top,
   SRCCOPY))
   return false;
   //   }
   return true;

   /*   if(pgraphics == NULL)
   return false;

   if(pbitmap == NULL)
   return false;

   if(prgnClip == NULL)
   return false;


   rect rectClipBox;

   prgnClip->GetRgnBox(rectClipBox);

   //   ::draw2d::region rgnUpdate;
   //   rgnUpdate.create_rect(lpcrect);
   //   rgnUpdate.CombineRgn(&rgnUpdate, prgnClip, ::draw2d::region::combine_exclude);
   ::draw2d::graphics_sp spgraphics(allocer());
   spgraphics->CreateCompatibleDC(pgraphics);
   spgraphics->SetViewportOrg(pgraphics->GetViewportOrg());

   if(!CreateBitmap(
   &spgraphics,
   pgraphics,
   pbitmapOld,
   pbitmap,
   pbmp,
   rect.right,
   rect.bottom))
   {
   return false;
   }
   //      spgraphics->SelectClipRgn(&rgnUpdate);
   if(!spgraphics->BitBlt(
   rect.left,
   rect.top,
   rect.right - rect.left,
   rect.bottom - rect.top,
   pgraphics,
   rect.left,
   rect.top,
   SRCCOPY))
   {
   return false;
   }
   else
   {
   return true;
   }*/
   //}


   /*
   bool imaging::ClipRestore(
   ::draw2d::graphics * pgraphics,
   ::draw2d::bitmap * pbitmap,
   ::draw2d::bitmap * pbitmapOld,
   BITMAP * pbmp,
   const RECT & rect,
   ::draw2d::region * prgnClip)
   {
   UNREFERENCED_PARAMETER(pbitmapOld);
   UNREFERENCED_PARAMETER(pbmp);
   if(pgraphics == NULL)
   return false;

   if(pbitmap == NULL)
   return false;

   rect rectClipBox;

   prgnClip->get_bounding_box(rectClipBox);

   rect rectUpdate;

   rectUpdate.intersect(rectClipBox, lpcrect);

   if(!rectUpdate.is_empty())
   {
   ::draw2d::graphics_sp spgraphics(allocer());
   spgraphics->CreateCompatibleDC(pgraphics);
   spgraphics->SetViewportOrg(pgraphics->GetViewportOrg());
   spgraphics->SelectObject(pbitmap);
   ::draw2d::region_sp rgnClip(allocer());
   //rgnClip->create_rect(0, 0, 0, 0);

   int32_t iClip = 0;

   ::draw2d::region_sp rgnUpdate(get_app());

   #ifdef WINDOWS

   ::GetClipRgn((HDC)pgraphics->get_os_data(), (HRGN) rgnClip->get_os_data());

   rgnUpdate->create_rect(lpcrect);

   rgnUpdate->combine(rgnUpdate, prgnClip, ::draw2d::region::combine_exclude);

   #else

   throw todo(get_app());

   #endif

   pgraphics->SelectClipRgn(rgnUpdate);
   rgnUpdate->GetRgnBox(rectUpdate);
   bool bOk;
   bOk = pgraphics->BitBlt(
   rectUpdate.left,
   rectUpdate.top,
   rectUpdate.width(),
   rectUpdate.height(),
   spgraphics,
   rectUpdate.left,
   rectUpdate.top,
   SRCCOPY) != 0;



   if(iClip == 1)
   {
   pgraphics->SelectClipRgn(rgnClip);
   }
   else
   pgraphics->SelectClipRgn(NULL);
   return bOk;
   }
   return true;

   /*   if(pgraphics == NULL)
   return false;

   if(pbitmap == NULL)
   return false;

   rect rectClipBox;

   prgnClip->GetRgnBox(rectClipBox);

   rect rectUpdate(lpcrect);

   //rectUpdate.intersect(rectClipBox, lpcrect);

   ::draw2d::graphics_sp spgraphics(allocer());
   spgraphics->CreateCompatibleDC(pgraphics);
   spgraphics->SetViewportOrg(pgraphics->GetViewportOrg());
   spgraphics->SelectObject(pbitmap);
   ::draw2d::region rgnClip;
   rgnClip.create_rect(0, 0, 0, 0);
   int32_t iClip = ::GetClipRgn((HDC)pgraphics->get_os_data(), (HRGN) rgnClip);
   rect rC;
   prgnClip->GetRgnBox(rC);
   ::draw2d::region rgnUpdate;
   rgnUpdate.create_rect(rectUpdate);
   rgnUpdate.CombineRgn(&rgnUpdate, prgnClip, ::draw2d::region::combine_exclude);
   pgraphics->SelectClipRgn(&rgnUpdate);
   rect r1;
   rgnUpdate.get_bounding_box(r1);
   bool bOk;
   bOk = pgraphics->BitBlt(
   rectUpdate.left,
   rectUpdate.top,
   rectUpdate.width(),
   rectUpdate.height(),
   &spgraphics,
   rectUpdate.left,
   rectUpdate.top,
   SRCCOPY) != 0;



   if(iClip == 1)
   {
   pgraphics->SelectClipRgn(&rgnClip);
   }
   else
   pgraphics->SelectClipRgn(NULL);
   return bOk;*/
   //}

   /*
   bool imaging::ClipSave(
   ::draw2d::graphics * pgraphics,
   ::draw2d::bitmap * pbitmap,
   ::draw2d::bitmap * pbitmapOld,
   BITMAP * pbmp,
   const RECT & rect)
   {

   if(pgraphics == NULL)
   return false;

   if(pbitmap == NULL)
   return false;

   ::draw2d::region_sp rgnClip(allocer());

   rgnClip->create_rect(0, 0, 0, 0);

   #ifdef WINDOWS

   if(::GetClipRgn((HDC)pgraphics->get_os_data(), (HRGN) rgnClip->get_os_data()) == 1)
   {
   return ClipSave(
   pgraphics,
   pbitmap,
   pbitmapOld,
   pbmp,
   lpcrect,
   rgnClip);
   }

   #else

   throw todo(get_app());

   #endif

   return true;

   }

   bool imaging::ClipRestore(
   ::draw2d::graphics * pgraphics,
   ::draw2d::bitmap * pbitmap,
   ::draw2d::bitmap * pbitmapOld,
   BITMAP * pbmp,
   const RECT & rect)
   {
   if(pgraphics == NULL)
   return false;

   if(pbitmap == NULL)
   return false;

   ::draw2d::region_sp rgnClip(allocer());

   rgnClip->create_rect(0, 0, 0, 0);

   #ifdef WINDOWS

   if(::GetClipRgn((HDC)pgraphics->get_os_data(), (HRGN) rgnClip->get_os_data()) == 1)
   {
   return ClipRestore(
   pgraphics,
   pbitmap,
   pbitmapOld,
   pbmp,
   lpcrect,
   rgnClip);
   }

   return true;

   #else

   throw todo(get_app());

   #endif

   }
   */

   bool imaging::CreateBitmap(
      ::draw2d::graphics * pgraphics,
      ::draw2d::graphics * pdcScreen,
      ::draw2d::bitmap * pbitmap,
      ::draw2d::bitmap * pbitmapOld,
      BITMAP * pbmp,
      int32_t cx,
      int32_t cy)
   {
      int32_t cxout = cx;
      int32_t cyout = cy;

      pgraphics->SelectObject(pbitmapOld);
      bool bCreate = true;
      if(pbitmap->get_os_data() != NULL)
      {
         class size size = pbitmap->get_size();
         if(size.cx >= cx && size.cy >= cy)
         {
            bCreate = false;
         }

      }
      if(bCreate)
      {
         if(!pbitmap->CreateCompatibleBitmap(pdcScreen,cxout,cyout))
         {
            return false;
         }
      }
      if(!pgraphics->SelectObject(pbitmap))
      {
         if(!pbitmap->CreateCompatibleBitmap(pdcScreen,cxout,cyout))
         {
            return false;
         }
         if(!pgraphics->SelectObject(pbitmap))
         {
            return false;
         }
      }
      if(bCreate)
      {
         pgraphics->FillSolidRect(rect(0,0,cxout,cyout),RGB(0,0,0));
      }
      return true;
   }


   bool imaging::CreateBitmap(::draw2d::graphics *pgraphics,::draw2d::bitmap * pbitmapOld,::draw2d::bitmap *pbitmap,BITMAP *pbmp,int32_t cx,int32_t cy)
   {
      int32_t cxout = cx;
      int32_t cyout = cy;

      pgraphics->SelectObject(pbitmapOld);
      bool bCreate = true;
      if(pbitmap->get_os_data() != NULL)
      {
         class size size = pbitmap->get_size();
         if(size.cx >= cx && size.cy >= cy)
         {
            bCreate = false;
         }

      }
      if(bCreate)
      {
         if(!pbitmap->CreateCompatibleBitmap(pgraphics,cxout,cyout))
         {
            return false;
         }
      }
      if(!pgraphics->SelectObject(pbitmap))
      {
         if(!pbitmap->CreateCompatibleBitmap(pgraphics,cxout,cyout))
         {
            return false;
         }
         if(!pgraphics->SelectObject(pbitmap))
         {
            return false;
         }
      }
      if(bCreate)
      {
         pgraphics->FillSolidRect(rect(0,0,cxout,cyout),RGB(255,196,255));
      }
      return true;
   }





   bool imaging::blur(::draw2d::graphics *pdcDst,point ptDst,size size,::draw2d::graphics * pdcSrc,point ptSrc,int32_t iRadius)
   {

      if(size.cx <= 0 || size.cy <= 0)
         return true;

      ::visual::fastblur f(allocer());

      if(!f.initialize(size,abs(iRadius)))
         return false;

      f->get_graphics()->set_alpha_mode(::draw2d::alpha_mode_set);

      f->from(null_point(),pdcSrc,ptSrc,size);

      f.blur();

      f->to(pdcDst,ptDst,size);

      return true;

   }

   void imaging::blur_32CC(::draw2d::dib * pdibDst,::draw2d::dib * pdibSrc,int32_t iRadius)
   {
      int32_t iFilterWidth = iRadius * 2 + 1;
      int32_t iFilterHeight = iRadius * 2 + 1;
      int32_t divisor = iFilterWidth * iFilterHeight;
      BYTE *lpbSource;
      BYTE *lpwDestination;
      BYTE *lpbSource_1;
      BYTE *lpbSource_2;
      BYTE *lpFilter;
      BYTE * pFilter = new BYTE[iFilterHeight *iFilterWidth];
      memset(pFilter,1,iFilterHeight * iFilterWidth);

      LPBYTE lpbSrc = (LPBYTE)pdibSrc->get_data();
      LPBYTE lpbDst = (LPBYTE)pdibDst->get_data();
      int32_t cx = pdibSrc->m_size.cx;
      int32_t cy = pdibSrc->m_size.cy;
      //int32_t wSrc = cx * 4;
      //int32_t wDest = cx * 4;
      int32_t wSrc = pdibSrc->m_iScan;
      int32_t wDest = pdibDst->m_iScan;
      int32_t maxx1 = cx;
      int32_t maxy1 = cy;
      //   int32_t maxy2 = cy - iFilterWidth;
      //   int32_t maxy3 = cy - iFilterWidth / 2;
      int32_t max3x1 = maxx1 * 4;
      //   int32_t max3x2 = (maxx1 - iFilterHeight) * 4;
      //   int32_t max3x3 = (maxx1 - iFilterHeight / 2) * 4;
      //int32_t bm3Width = (cy * 3 + 3) & ~0x3L;
      //   int32_t w = cx * 3;
      //   memcpy(m_lpwDestination, m_lpbSource, bmWidth * bmHeight * 3);
      //   memcpy(m_lpwDestination, m_lpbSource, bm3Width * bmHeight);
      /*for(int32_t y = 0; y < cy; y++)
      {
      lpbSource = lpbSrc + wSrc * y;
      lpwDestination = lpbDst + wDest * y;
      for(int32_t x = 0; x < w; x++)
      {
      *lpwDestination = *lpbSource;
      lpwDestination++;
      lpbSource++;
      }
      }*/
      uint32_t dwR;
      uint32_t dwG;
      uint32_t dwB;
      uint32_t dwA;

      int32_t iFilterXBegin;
      int32_t iFilterXEnd;
      int32_t iFilterYBegin;
      int32_t iFilterYEnd;

      int32_t yLowerBound[4];
      int32_t yUpperBound[4];
      int32_t xLowerBound[4];
      int32_t xUpperBound[4];

      // top
      yLowerBound[0] = 0;
      yUpperBound[0] = iFilterWidth / 2;
      xLowerBound[0] = 0;
      xUpperBound[0] = max3x1;

      // left
      yLowerBound[1] = iFilterWidth / 2;
      yUpperBound[1] = maxy1 - iFilterWidth / 2;
      xLowerBound[1] = 0;
      xUpperBound[1] = iFilterWidth / 2;

      // right
      yLowerBound[2] = iFilterWidth / 2;
      yUpperBound[2] = maxy1 - iFilterWidth / 2;
      xLowerBound[2] = max3x1 - iFilterWidth / 2;
      xUpperBound[2] = max3x1;

      // bottom
      yLowerBound[3] = maxy1 - iFilterWidth / 2;
      yUpperBound[3] = maxy1;
      xLowerBound[3] = 0;
      xUpperBound[3] = max3x1;

      /*   for(int32_t i = 0; i < 4; i++)
      {
      int32_t yL = yLowerBound[i];
      int32_t yU = yUpperBound[i];
      int32_t xL = xLowerBound[i];
      int32_t xU = xUpperBound[i];
      for(int32_t y1 = yL; y1 < yU; y1++)
      {
      if(y1 < iFilterWidth / 2)
      {
      iFilterYBegin = iFilterWidth / 2 - y1;
      }
      else
      {
      iFilterYBegin = 0;
      }
      if(y1 > maxy3)
      {
      iFilterYEnd = iFilterWidth - y1 - maxy3;
      }
      else
      {
      iFilterYEnd = iFilterWidth;
      }
      lpbSource = lpbSrc + (wSrc * MAX((y1 - iFilterWidth / 2), 0));
      lpwDestination = lpbDst + (wDest  * y1);
      for(int32_t x1 = xL; x1 < xU; x1+=3)
      {
      if(x1 < iFilterHeight /2)
      {
      iFilterXBegin = iFilterHeight / 2 - x1;
      }
      else
      {
      iFilterXBegin = 0;
      }
      if(x1 > max3x3)
      {
      iFilterXEnd = iFilterHeight  - x1 + max3x3;
      }
      else
      {
      iFilterXEnd = iFilterHeight;
      }
      lpbSource_1 = lpbSource + ((MAX(x1 - iFilterWidth / 2 * 3, 0)));
      lpFilter = pFilter;
      //            lpConv = pConv;

      dwR = 0;
      dwG = 0;
      dwB = 0;
      for(int32_t y2 = iFilterYBegin; y2 < iFilterYEnd; y2++)
      {
      lpbSource_2 = lpbSource_1 + (wSrc * y2);
      for(int32_t x2 = iFilterXBegin; x2 < iFilterXEnd; x2++)
      {
      dwR += *lpbSource_2++ * *lpFilter;
      dwG += *lpbSource_2++ * *lpFilter;
      dwB += *lpbSource_2++ * *lpFilter++;

      }
      }
      lpwDestination_1 = lpwDestination + x1;
      divisor = (iFilterYEnd - iFilterYBegin) * (iFilterXEnd - iFilterXBegin );
      if(divisor == 0)
      {
      lpwDestination_1[0] = MAX(0, MIN(dwR, 255));
      lpwDestination_1[1] = MAX(0, MIN(dwG, 255));
      lpwDestination_1[2] = MIN(0, MIN(dwB, 255));
      }
      else
      {
      lpwDestination_1[0] = dwR / divisor;
      lpwDestination_1[1] = dwG / divisor;
      lpwDestination_1[2] = dwB / divisor;
      }
      }
      }
      }*/

      iFilterYBegin  = 0;
      iFilterYEnd    = iFilterWidth;
      iFilterXBegin = 0;
      iFilterXEnd = iFilterHeight;

      int32_t iFilterHalfWidth = iFilterWidth / 2;
      int32_t iFilterHalfWidthBytes = iFilterHalfWidth * 3;

      int32_t yL = iFilterHalfWidth;
      int32_t yU = maxy1 - iFilterHalfWidth;
      int32_t xL = iFilterHalfWidthBytes;
      int32_t xU = max3x1 - iFilterHalfWidthBytes;


      int32_t y1 = yL;
      int32_t y2 = yL - iFilterHalfWidth;
      int32_t x1;
      int32_t x2;

      divisor = (iFilterYEnd - iFilterYBegin) * (iFilterXEnd - iFilterXBegin);

      for(; y1 < yU;)
      {
         lpbSource = lpbSrc + (wSrc * y2);

         x1 = xL;
         x2 = xL - iFilterHalfWidthBytes;
         lpwDestination = lpbDst + (wDest  * y1) + x1;
         for(; x1 < xU;)
         {
            lpbSource_1 = lpbSource + x2;
            lpFilter = pFilter;

            dwR = 0;
            dwG = 0;
            dwB = 0;
            dwA = 0;
            for(int32_t yFilter = iFilterYBegin; yFilter < iFilterYEnd; yFilter++)
            {
               lpbSource_2 = lpbSource_1 + (wSrc * yFilter);
               for(int32_t xFilter = iFilterXBegin; xFilter < iFilterXEnd; xFilter++)
               {
                  dwR += *lpbSource_2++ * *lpFilter;
                  dwG += *lpbSource_2++ * *lpFilter;
                  dwB += *lpbSource_2++ * *lpFilter;
                  dwA += *lpbSource_2++ * *lpFilter;

               }
            }


            *lpwDestination++ = (uchar)(dwR / divisor);
            *lpwDestination++ = (uchar)(dwG / divisor);
            *lpwDestination++ = (uchar)(dwB / divisor);
            *lpwDestination++ = (uchar)(dwA / divisor);
            x1 += 3;
            x2 += 3;
         }
         y1++;
         y2++;
      }

      delete pFilter;
   }


   void imaging::blur_32CC_r2(::draw2d::dib * pdibDst,::draw2d::dib * pdibSrc)
   {
      LPBYTE lpbSrc = (LPBYTE)pdibSrc->get_data();
      LPBYTE lpbDst = (LPBYTE)pdibDst->get_data();
      int32_t cx = pdibSrc->m_size.cx;
      int32_t cy = pdibSrc->m_size.cy;
      //int32_t wSrc = cx * 4;
      //int32_t wDest = cx * 4;
      int32_t wSrc = pdibSrc->m_iScan;
      int32_t wDest = pdibDst->m_iScan;

      BYTE *lpbSource;
      BYTE *lpwDestination;
      BYTE *lpbSource_1;
      BYTE *lpbSource_2[5];


      if(cx <= 0)
         return;
      if(cy <= 0)
         return;

      int32_t maxx1 = cx;
      int32_t maxy1 = cy;
      //   int32_t maxy2 = cy - 5;
      //   int32_t maxy3 = cy - 2;
      int32_t max3x1 = maxx1 * 4;
      //   int32_t max3x2 = (maxx1 - 5) * 4;
      ///   int32_t max3x3 = (maxx1 - 2) * 4;
      //   int32_t w = cx * 3;
      uint32_t dwR;
      uint32_t dwG;
      uint32_t dwB;
      uint32_t dwA;

      int32_t yL = 2;
      int32_t yU = maxy1 - 2;
      int32_t xL = 8;
      int32_t xU = max3x1 - 8;


      int32_t y1 = 2;
      int32_t y2 = yL - 2;
      int32_t x1;
      int32_t x2;

      for(; y1 < yU;)
      {
         lpbSource = lpbSrc + (wSrc * y2);

         x1 = xL;
         x2 = xL - 8;
         lpwDestination = lpbDst + (wDest  * y1) + x1;
         for(; x1 < xU;)
         {
            lpbSource_1 = lpbSource + x2;
            dwR = 0;
            dwG = 0;
            dwB = 0;
            dwA = 0;
            lpbSource_2[0] = lpbSource_1;
            lpbSource_2[1] = lpbSource_1 + wSrc;
            lpbSource_2[2] = lpbSource_1 + wSrc * 2;
            lpbSource_2[3] = lpbSource_1 + wSrc * 3;
            lpbSource_2[4] = lpbSource_1 + wSrc * 4;

            // red
            lpwDestination[0] =
               (lpbSource_2[0][0] + lpbSource_2[0][4] + lpbSource_2[0][8] + lpbSource_2[0][12] + lpbSource_2[0][16]
               + lpbSource_2[1][0] + lpbSource_2[1][4] + lpbSource_2[1][8] + lpbSource_2[1][12] + lpbSource_2[1][16]
               + lpbSource_2[2][0] + lpbSource_2[2][4] + lpbSource_2[2][8] + lpbSource_2[2][12] + lpbSource_2[2][16]
               + lpbSource_2[3][0] + lpbSource_2[3][4] + lpbSource_2[3][8] + lpbSource_2[3][12] + lpbSource_2[3][16]
               + lpbSource_2[4][0] + lpbSource_2[4][4] + lpbSource_2[4][8] + lpbSource_2[4][12] + lpbSource_2[4][16]
               ) / 25;

            // green
            lpwDestination[1] =
               (lpbSource_2[0][1] + lpbSource_2[0][5] + lpbSource_2[0][9] + lpbSource_2[0][13] + lpbSource_2[0][17]
               + lpbSource_2[1][1] + lpbSource_2[1][5] + lpbSource_2[1][9] + lpbSource_2[1][13] + lpbSource_2[1][17]
               + lpbSource_2[2][1] + lpbSource_2[2][5] + lpbSource_2[2][9] + lpbSource_2[2][13] + lpbSource_2[2][17]
               + lpbSource_2[3][1] + lpbSource_2[3][5] + lpbSource_2[3][9] + lpbSource_2[3][13] + lpbSource_2[3][17]
               + lpbSource_2[4][1] + lpbSource_2[4][5] + lpbSource_2[4][9] + lpbSource_2[4][13] + lpbSource_2[4][17]
               ) / 25;

            // blue
            lpwDestination[2] =
               (lpbSource_2[0][2] + lpbSource_2[0][6] + lpbSource_2[0][10] + lpbSource_2[0][14] + lpbSource_2[0][18]
               + lpbSource_2[1][2] + lpbSource_2[1][6] + lpbSource_2[1][10] + lpbSource_2[1][14] + lpbSource_2[1][18]
               + lpbSource_2[2][2] + lpbSource_2[2][6] + lpbSource_2[2][10] + lpbSource_2[2][14] + lpbSource_2[2][18]
               + lpbSource_2[3][2] + lpbSource_2[3][6] + lpbSource_2[3][10] + lpbSource_2[3][14] + lpbSource_2[3][18]
               + lpbSource_2[4][2] + lpbSource_2[4][6] + lpbSource_2[4][10] + lpbSource_2[4][14] + lpbSource_2[4][18]
               ) / 25;

            // alpha
            lpwDestination[3] =
               (lpbSource_2[0][3] + lpbSource_2[0][7] + lpbSource_2[0][11] + lpbSource_2[0][15] + lpbSource_2[0][19]
               + lpbSource_2[1][3] + lpbSource_2[1][7] + lpbSource_2[1][11] + lpbSource_2[1][15] + lpbSource_2[1][19]
               + lpbSource_2[2][3] + lpbSource_2[2][7] + lpbSource_2[2][11] + lpbSource_2[2][15] + lpbSource_2[2][19]
               + lpbSource_2[3][3] + lpbSource_2[3][7] + lpbSource_2[3][11] + lpbSource_2[3][15] + lpbSource_2[3][19]
               + lpbSource_2[4][3] + lpbSource_2[4][7] + lpbSource_2[4][11] + lpbSource_2[4][15] + lpbSource_2[4][19]
               ) / 25;

            lpwDestination += 4;
            x1 += 4;
            x2 += 4;
         }
         y1++;
         y2++;
      }

   }

   bool imaging::channel_gray_blur(::draw2d::graphics *pdcDst,point ptDst,size size,::draw2d::graphics * pdcSrc,point ptSrc,int32_t iChannel,int32_t iRadius)
   {
      if(size.cx <= 0 || size.cy <= 0)
         return true;

      ::draw2d::dib_sp dibDst(allocer());

      if(!dibDst.is_set())
         return false;

      if(!dibDst->create(size))
         return false;

      ::draw2d::dib_sp dibSrc(allocer());

      if(!dibSrc.is_set())
         return false;

      if(!dibSrc->create(size))
         return false;

      dibSrc->get_graphics()->set_alpha_mode(::draw2d::alpha_mode_set);

      if(!dibSrc->from(null_point(),pdcSrc,ptSrc,size))
         return false;

      if(!channel_gray_blur_32CC(
         dibDst,
         dibSrc,
         iChannel,
         iRadius))
         return false;

      if(!dibDst->to(pdcDst,ptDst,size))
         return false;

      return true;
   }


   bool imaging::channel_alpha_gray_blur(::draw2d::graphics *pdcDst,point ptDst,size size,::draw2d::graphics * pdcSrc,point ptSrc,int32_t iChannel,int32_t iRadius)
   {
      if(size.cx <= 0 || size.cy <= 0)
         return true;

      ::draw2d::dib_sp dibDst(allocer());

      if(!dibDst.is_set())
         return false;

      if(!dibDst->create(size))
         return false;

      ::draw2d::dib_sp dibSrc(allocer());

      if(!dibSrc.is_set())
         return false;

      if(!dibSrc->create(size))
         return false;

      dibSrc->get_graphics()->set_alpha_mode(::draw2d::alpha_mode_set);

      if(!dibSrc->from(null_point(),pdcSrc,ptSrc,size))
         return false;

      if(!channel_alpha_gray_blur_32CC(
         dibDst,
         dibSrc,
         iChannel,
         iRadius))
         return false;

      if(!dibDst->to(pdcDst,ptDst,size))
         return false;

      return true;
   }

   bool imaging::channel_gray_blur_32CC(::draw2d::dib * pdibDst,::draw2d::dib * pdibSrc,
      int32_t iChannel,int32_t iRadius)
   {

      if(iRadius <= 0)
         return true;

      int32_t cx = pdibDst->m_size.cx;
      int32_t cy = pdibDst->m_size.cy;

      if(cx != pdibSrc->m_size.cx
         || cy != pdibSrc->m_size.cy)
         return false;

      LPBYTE lpbDst = (LPBYTE)pdibDst->get_data();
      LPBYTE lpbSrc = (LPBYTE)pdibSrc->get_data();

      //int32_t wSrc = cx * 4;
      //int32_t wDst = cx * 4;
      int32_t wSrc = pdibSrc->m_iScan;
      int32_t wDst = pdibDst->m_iScan;

      int32_t iFilterW = iRadius * 2 + 1;
      int32_t iFilterH = iRadius * 2 + 1;
      int32_t iFilterHalfW = iRadius;
      int32_t iFilterHalfH = iRadius;
      //   int32_t iFilterArea = iFilterW * iFilterH;
      int32_t iDivisor;

      BYTE *lpwDestination;
      BYTE *lpwDestination_1;


      BYTE *lpbSource;
      BYTE *lpbSource_1;
      BYTE *lpbSource_2;
      BYTE *lpbSource_3;

      uint32_t dwI;

      int32_t x,y;
      int32_t x2;
      int32_t x3;
      int32_t iFilterXLowerBound;
      int32_t iFilterXUpperBound;
      int32_t iFilterYLowerBound;
      int32_t iFilterYUpperBound;
      int32_t xFilter;
      int32_t yFilter;

      int32_t xLowerBound[4];
      int32_t xUpperBound[4];
      int32_t yLowerBound[4];
      int32_t yUpperBound[4];

      int32_t xmax = cx - 1;
      int32_t ymax = cy - 1;

      // top
      xLowerBound[0] = 0;
      xUpperBound[0] = cx - 1;
      yLowerBound[0] = 0;
      yUpperBound[0] = iFilterHalfH - 1;

      // left
      xLowerBound[1] = 0;
      xUpperBound[1] = iFilterHalfW - 1;
      yLowerBound[1] = iFilterHalfH;
      yUpperBound[1] = cy - iFilterHalfH - 1;

      // right
      xLowerBound[2] = cx - iFilterHalfW;
      xUpperBound[2] = cx - 1;
      yLowerBound[2] = iFilterHalfH;
      yUpperBound[2] = cy - iFilterHalfH - 1;

      // bottom
      xLowerBound[3] = 0;
      xUpperBound[3] = cx - 1;
      yLowerBound[3] = cy - iFilterHalfW;
      yUpperBound[3] = cy - 1;


      int32_t iFilterLine;

      for(int32_t i = 0; i < 4; i++)
      {
         int32_t xL = xLowerBound[i];
         int32_t xU = xUpperBound[i];
         int32_t yL = yLowerBound[i];
         int32_t yU = yUpperBound[i];
         for(y = yL; y <= yU; y++)
         {
            iFilterYLowerBound = y - iFilterHalfH;
            if(iFilterYLowerBound < 0)
               iFilterYLowerBound = 0;
            iFilterYUpperBound = y + iFilterHalfH;
            if(iFilterYUpperBound > ymax)
               iFilterYUpperBound = ymax;

            lpbSource = lpbSrc + (wSrc * MAX(iFilterYLowerBound,0));
            lpwDestination = lpbDst + (wDst  * y);
            x = xL;
            x2 = x - iFilterHalfW;
            x3 = x * 4;
            for(; x <= xU; x++,x3+=4)
            {
               iFilterXLowerBound = x - iFilterHalfW;
               if(iFilterXLowerBound < 0)
                  iFilterXLowerBound = 0;
               iFilterXUpperBound = x + iFilterHalfW;
               if(iFilterXUpperBound > xmax)
                  iFilterXUpperBound = xmax;

               lpbSource_1 = lpbSource + MAX(iFilterXLowerBound,0) * 4 + iChannel;

               dwI = 0;
               iFilterLine = 0;
               for(yFilter = iFilterYLowerBound; yFilter <= iFilterYUpperBound; yFilter++)
               {
                  lpbSource_2 = lpbSource_1 + (wSrc * iFilterLine);
                  for(xFilter = iFilterXLowerBound; xFilter <= iFilterXUpperBound; xFilter++)
                  {
                     dwI += *lpbSource_2;
                     lpbSource_2 += 4;
                  }
                  iFilterLine++;
               }
               lpwDestination_1 = lpwDestination + x3;
               iDivisor = (iFilterYUpperBound - iFilterYLowerBound + 1) * (iFilterXUpperBound - iFilterXLowerBound + 1);
               if(iDivisor == 0)
               {
                  lpwDestination_1[0] = (BYTE)MAX(0,MIN(dwI,255));
                  lpwDestination_1[1] = (BYTE)MAX(0,MIN(dwI,255));
                  lpwDestination_1[2] = (BYTE)MIN(0,MIN(dwI,255));
               }
               else
               {
                  lpwDestination_1[0] = (BYTE)(dwI / iDivisor);
                  lpwDestination_1[1] = lpwDestination_1[0];
                  lpwDestination_1[2] = lpwDestination_1[0];
               }
            }
         }
      }

      iDivisor = iFilterW * iFilterH;

      iFilterYLowerBound     = 0;
      iFilterYUpperBound     = iFilterW - 1;
      iFilterXLowerBound     = 0;
      iFilterXUpperBound     = iFilterH - 1;

      int32_t yL = iFilterHalfH;
      int32_t yU = cy - iFilterHalfH + (iFilterHalfH % 2 == 1 ? 0 : 1);
      int32_t xL = iFilterHalfW;
      int32_t xU = cx - iFilterHalfW + (iFilterHalfW % 2 == 1 ? 0 : 1);

      int32_t y1 = yL;
      int32_t x1;

      iDivisor = (iFilterYUpperBound - iFilterYLowerBound + 1) * (iFilterXUpperBound - iFilterXLowerBound + 1);

      int32_t wDiff = wDst - (xU - xL) * 4;
      lpbSource = lpbSrc + iChannel;
      lpwDestination = lpbDst + (wDst * yL) + xL * 4;
      for(; y1 < yU;)
      {
         x1 = xL;
         lpbSource_1 = lpbSource;
         for(; x1 < xU;)
         {
            dwI = 0;
            lpbSource_2 = lpbSource_1;
            for(int32_t yFilter = iFilterYLowerBound; yFilter <= iFilterYUpperBound; yFilter++)
            {
               lpbSource_3 = lpbSource_2;
               for(int32_t xFilter = iFilterXLowerBound; xFilter <= iFilterXUpperBound; xFilter++)
               {
                  dwI += *lpbSource_3;
                  lpbSource_3 += 4;
               }

               lpbSource_2 += wSrc;
            }

            dwI /= iDivisor;
            *lpwDestination++ = (uchar)dwI;
            *lpwDestination++ = (uchar)dwI;
            *lpwDestination++ = (uchar)dwI;
            *lpwDestination++ = (uchar)lpbSrc[(wDst * yL) + xL * 4];

            x1++;
            lpbSource_1 += 4;
         }
         lpwDestination += wDiff;
         y1++;
         lpbSource += wSrc;
      }

      return true;

   }

   bool imaging::channel_alpha_gray_blur_32CC(::draw2d::dib * pdibDst,::draw2d::dib * pdibSrc,
      int32_t iChannel,int32_t iRadius)
   {

      if(iRadius <= 0)
         return true;

      int32_t cx = pdibDst->m_size.cx;
      int32_t cy = pdibDst->m_size.cy;

      if(cx != pdibSrc->m_size.cx
         || cy != pdibSrc->m_size.cy)
         return false;

      LPBYTE lpbDst = (LPBYTE)pdibDst->get_data();
      LPBYTE lpbSrc = (LPBYTE)pdibSrc->get_data();

      //int32_t wSrc = cx * 4;
      //int32_t wDst = cx * 4;
      int32_t wSrc = pdibSrc->m_iScan;
      int32_t wDst = pdibDst->m_iScan;

      int32_t iFilterW = iRadius * 2 + 1;
      int32_t iFilterH = iRadius * 2 + 1;
      int32_t iFilterHalfW = iRadius;
      int32_t iFilterHalfH = iRadius;
      //   int32_t iFilterArea = iFilterW * iFilterH;
      int32_t iDivisor;

      BYTE *lpwDestination;
      BYTE *lpwDestination_1;


      BYTE *lpbSource;
      BYTE *lpbSource_1;
      BYTE *lpbSource_2;
      BYTE *lpbSource_3;

      uint32_t dwI;

      int32_t x,y;
      int32_t x2;
      int32_t x3;
      int32_t iFilterXLowerBound;
      int32_t iFilterXUpperBound;
      int32_t iFilterYLowerBound;
      int32_t iFilterYUpperBound;
      int32_t xFilter;
      int32_t yFilter;

      int32_t xLowerBound[4];
      int32_t xUpperBound[4];
      int32_t yLowerBound[4];
      int32_t yUpperBound[4];

      int32_t xmax = cx - 1;
      int32_t ymax = cy - 1;

      // top
      xLowerBound[0] = 0;
      xUpperBound[0] = cx - 1;
      yLowerBound[0] = 0;
      yUpperBound[0] = iFilterHalfH - 1;

      // left
      xLowerBound[1] = 0;
      xUpperBound[1] = iFilterHalfW - 1;
      yLowerBound[1] = iFilterHalfH;
      yUpperBound[1] = cy - iFilterHalfH - 1;

      // right
      xLowerBound[2] = cx - iFilterHalfW;
      xUpperBound[2] = cx - 1;
      yLowerBound[2] = iFilterHalfH;
      yUpperBound[2] = cy - iFilterHalfH - 1;

      // bottom
      xLowerBound[3] = 0;
      xUpperBound[3] = cx - 1;
      yLowerBound[3] = cy - iFilterHalfW;
      yUpperBound[3] = cy - 1;


      int32_t iFilterLine;

      for(int32_t i = 0; i < 4; i++)
      {
         int32_t xL = xLowerBound[i];
         int32_t xU = xUpperBound[i];
         int32_t yL = yLowerBound[i];
         int32_t yU = yUpperBound[i];
         for(y = yL; y <= yU; y++)
         {
            iFilterYLowerBound = y - iFilterHalfH;
            if(iFilterYLowerBound < 0)
               iFilterYLowerBound = 0;
            iFilterYUpperBound = y + iFilterHalfH;
            if(iFilterYUpperBound > ymax)
               iFilterYUpperBound = ymax;

            lpbSource = lpbSrc + (wSrc * MAX(iFilterYLowerBound,0));
            lpwDestination = lpbDst + (wDst  * y);
            x = xL;
            x2 = x - iFilterHalfW;
            x3 = x * 4;
            for(; x <= xU; x++,x3+=4)
            {
               iFilterXLowerBound = x - iFilterHalfW;
               if(iFilterXLowerBound < 0)
                  iFilterXLowerBound = 0;
               iFilterXUpperBound = x + iFilterHalfW;
               if(iFilterXUpperBound > xmax)
                  iFilterXUpperBound = xmax;

               lpbSource_1 = lpbSource + MAX(iFilterXLowerBound,0) * 4 + iChannel;

               dwI = 0;
               iFilterLine = 0;
               for(yFilter = iFilterYLowerBound; yFilter <= iFilterYUpperBound; yFilter++)
               {
                  lpbSource_2 = lpbSource_1 + (wSrc * iFilterLine);
                  for(xFilter = iFilterXLowerBound; xFilter <= iFilterXUpperBound; xFilter++)
                  {
                     dwI += *lpbSource_2;
                     lpbSource_2 += 4;
                  }
                  iFilterLine++;
               }
               lpwDestination_1 = lpwDestination + x3;
               iDivisor = (iFilterYUpperBound - iFilterYLowerBound + 1) * (iFilterXUpperBound - iFilterXLowerBound + 1);
               if(iDivisor == 0)
               {
                  lpwDestination_1[0] = (BYTE)MAX(0,MIN(dwI,255));
                  lpwDestination_1[1] = (BYTE)MAX(0,MIN(dwI,255));
                  lpwDestination_1[2] = (BYTE)MIN(0,MIN(dwI,255));
                  lpwDestination_1[3] = (BYTE)MIN(0,MIN(dwI,255));
               }
               else
               {
                  lpwDestination_1[0] = (BYTE)(dwI / iDivisor);
                  lpwDestination_1[1] = lpwDestination_1[0];
                  lpwDestination_1[2] = lpwDestination_1[0];
                  lpwDestination_1[3] = lpwDestination_1[0];
               }
            }
         }
      }

      iDivisor = iFilterW * iFilterH;

      iFilterYLowerBound     = 0;
      iFilterYUpperBound     = iFilterW - 1;
      iFilterXLowerBound     = 0;
      iFilterXUpperBound     = iFilterH - 1;

      int32_t yL = iFilterHalfH;
      int32_t yU = cy - iFilterHalfH;
      int32_t xL = iFilterHalfW;
      int32_t xU = cx - iFilterHalfW;

      int32_t y1 = yL;
      int32_t x1;

      iDivisor = (iFilterYUpperBound - iFilterYLowerBound + 1) * (iFilterXUpperBound - iFilterXLowerBound + 1);

      int32_t wDiff = wDst - (xU - xL) * 4;
      lpbSource = lpbSrc + iChannel;
      lpwDestination = lpbDst + (wDst * yL) + xL * 4;
      for(; y1 < yU;)
      {
         x1 = xL;
         lpbSource_1 = lpbSource;
         for(; x1 < xU;)
         {
            dwI = 0;
            lpbSource_2 = lpbSource_1;
            for(int32_t yFilter = iFilterYLowerBound; yFilter <= iFilterYUpperBound; yFilter++)
            {
               lpbSource_3 = lpbSource_2;
               for(int32_t xFilter = iFilterXLowerBound; xFilter <= iFilterXUpperBound; xFilter++)
               {
                  dwI += *lpbSource_3;
                  lpbSource_3 += 4;
               }

               lpbSource_2 += wSrc;
            }

            dwI /= iDivisor;
            *lpwDestination++ = (uchar)dwI;
            *lpwDestination++ = (uchar)dwI;
            *lpwDestination++ = (uchar)dwI;
            *lpwDestination++ = (uchar)dwI;

            x1++;
            lpbSource_1 += 4;
         }
         lpwDestination += wDiff;
         y1++;
         lpbSource += wSrc;
      }

      return true;

   }

   bool imaging::channel_gray_blur(
      ::draw2d::graphics * pdcDst,
      point ptDst,
      size size,
      ::draw2d::graphics * pdcSrc,
      point ptSrc,
      int32_t iChannel,
   class size sizeFilter,
      LPBYTE lpbFilter)
   {
      if(size.cx <= 0 || size.cy <= 0)
         return true;

      ::draw2d::dib_sp dibDst(allocer());

      if(!dibDst.is_set())
         return false;

      if(!dibDst->create(size))
         return false;

      ::draw2d::dib_sp dibSrc(allocer());

      if(!dibSrc.is_set())
         return false;

      if(!dibSrc->create(size))
         return false;

      if(!dibSrc->from(null_point(),pdcSrc,ptSrc,size))
         return false;

      if(!channel_gray_blur_32CC(
         dibDst,
         dibSrc,
         iChannel,
         sizeFilter.cx,
         sizeFilter.cy,
         lpbFilter))
         return false;

      if(!dibDst->to(pdcDst,ptDst,size))
         return false;

      return true;

   }


   bool imaging::channel_gray_blur_32CC(::draw2d::dib * pdibDst,::draw2d::dib * pdibSrc,
      int32_t iChannel,
      int32_t iFilterWidth,
      int32_t iFilterHeight,
      LPBYTE lpbFilter)
   {

      int32_t cx = pdibDst->m_size.cx;
      int32_t cy = pdibDst->m_size.cy;

      if(cx != pdibSrc->m_size.cx
         || cy != pdibSrc->m_size.cy)
         return false;

      LPBYTE lpbDst = (LPBYTE)pdibDst->get_data();
      LPBYTE lpbSrc = (LPBYTE)pdibSrc->get_data();

      //int32_t wSrc = cx * 4;
      //int32_t wDst = cx * 4;
      int32_t wSrc = pdibSrc->m_iScan;
      int32_t wDst = pdibDst->m_iScan;

      int32_t iFilterArea = iFilterWidth * iFilterHeight;
      int32_t divisor;

      BYTE *lpwDestination;


      BYTE *lpbSource;
      BYTE *lpbSource_1;
      BYTE *lpbSource_2;
      BYTE *lpbSource_3;

      BYTE *lpFilter;
      BYTE * pFilter = new BYTE[iFilterHeight *iFilterWidth];
      memset(pFilter,1,iFilterHeight * iFilterWidth);

      int32_t maxx1 = cx;
      int32_t maxy1 = cy;
      //   int32_t maxy2 = cy - iFilterWidth;
      //   int32_t maxy3 = cy - iFilterWidth / 2;
      int32_t max3x1 = maxx1 * 4;
      //   int32_t max3x2 = (maxx1 - iFilterHeight) * 4;
      //   int32_t max3x3 = (maxx1 - iFilterHeight / 2) * 4;
      //int32_t bm3Width = (cy * 3 + 3) & ~0x3L;
      //   int32_t w = cx * 4;
      //   memcpy(m_lpwDestination, m_lpbSource, bmWidth * bmHeight * 3);
      //   memcpy(m_lpwDestination, m_lpbSource, bm3Width * bmHeight);
      /*for(int32_t y = 0; y < cy; y++)
      {
      lpbSource = lpbSrc + wSrc * y;
      lpwDestination = lpbDst + wDest * y;
      for(int32_t x = 0; x < w; x++)
      {
      *lpwDestination = *lpbSource;
      lpwDestination++;
      lpbSource++;
      }
      }*/
      uint32_t dwI;

      int32_t iFilterXBegin;
      int32_t iFilterXEnd;
      int32_t iFilterYBegin;
      int32_t iFilterYEnd;

      int32_t yLowerBound[4];
      int32_t yUpperBound[4];
      int32_t xLowerBound[4];
      int32_t xUpperBound[4];

      // top
      yLowerBound[0] = 0;
      yUpperBound[0] = iFilterWidth / 2;
      xLowerBound[0] = 0;
      xUpperBound[0] = max3x1;

      // left
      yLowerBound[1] = iFilterWidth / 2;
      yUpperBound[1] = maxy1 - iFilterWidth / 2;
      xLowerBound[1] = 0;
      xUpperBound[1] = iFilterWidth / 2;

      // right
      yLowerBound[2] = iFilterWidth / 2;
      yUpperBound[2] = maxy1 - iFilterWidth / 2;
      xLowerBound[2] = max3x1 - iFilterWidth / 2;
      xUpperBound[2] = max3x1;

      // bottom
      yLowerBound[3] = maxy1 - iFilterWidth / 2;
      yUpperBound[3] = maxy1;
      xLowerBound[3] = 0;
      xUpperBound[3] = max3x1;

      /*   for(int32_t i = 0; i < 4; i++)
      {
      int32_t yL = yLowerBound[i];
      int32_t yU = yUpperBound[i];
      int32_t xL = xLowerBound[i];
      int32_t xU = xUpperBound[i];
      for(int32_t y1 = yL; y1 < yU; y1++)
      {
      if(y1 < iFilterWidth / 2)
      {
      iFilterYBegin = iFilterWidth / 2 - y1;
      }
      else
      {
      iFilterYBegin = 0;
      }
      if(y1 > maxy3)
      {
      iFilterYEnd = iFilterWidth - y1 - maxy3;
      }
      else
      {
      iFilterYEnd = iFilterWidth;
      }
      lpbSource = lpbSrc + (wSrc * MAX((y1 - iFilterWidth / 2), 0));
      lpwDestination = lpbDst + (wDest  * y1);
      for(int32_t x1 = xL; x1 < xU; x1+=3)
      {
      if(x1 < iFilterHeight /2)
      {
      iFilterXBegin = iFilterHeight / 2 - x1;
      }
      else
      {
      iFilterXBegin = 0;
      }
      if(x1 > max3x3)
      {
      iFilterXEnd = iFilterHeight  - x1 + max3x3;
      }
      else
      {
      iFilterXEnd = iFilterHeight;
      }
      lpbSource_1 = lpbSource + ((MAX(x1 - iFilterWidth / 2 * 3, 0)));
      lpFilter = pFilter;
      //            lpConv = pConv;

      dwR = 0;
      dwG = 0;
      dwB = 0;
      for(int32_t y2 = iFilterYBegin; y2 < iFilterYEnd; y2++)
      {
      lpbSource_2 = lpbSource_1 + (wSrc * y2);
      for(int32_t x2 = iFilterXBegin; x2 < iFilterXEnd; x2++)
      {
      dwR += *lpbSource_2++ * *lpFilter;
      dwG += *lpbSource_2++ * *lpFilter;
      dwB += *lpbSource_2++ * *lpFilter++;

      }
      }
      lpwDestination_1 = lpwDestination + x1;
      divisor = (iFilterYEnd - iFilterYBegin) * (iFilterXEnd - iFilterXBegin );
      if(divisor == 0)
      {
      lpwDestination_1[0] = MAX(0, MIN(dwR, 255));
      lpwDestination_1[1] = MAX(0, MIN(dwG, 255));
      lpwDestination_1[2] = MIN(0, MIN(dwB, 255));
      }
      else
      {
      lpwDestination_1[0] = dwR / divisor;
      lpwDestination_1[1] = dwG / divisor;
      lpwDestination_1[2] = dwB / divisor;
      }
      }
      }
      }*/

      iFilterYBegin  = 0;
      iFilterYEnd    = iFilterWidth;
      iFilterXBegin = 0;
      iFilterXEnd = iFilterHeight;

      divisor = 0;
      lpFilter = lpbFilter;
      for(int32_t i = 0; i < iFilterArea; i++)
      {
         divisor += *lpFilter++;
      }

      int32_t iFilterHalfWidth = iFilterWidth / 2;
      //   int32_t iFilterHalfWidthBytes = iFilterHalfWidth * 3;

      int32_t yL = iFilterHalfWidth;
      int32_t yU = cy - iFilterHalfWidth;
      int32_t xL = iFilterHalfWidth;
      int32_t xU = cx - iFilterHalfWidth;


      int32_t y1 = yL;
      //int32_t y2 = yL - iFilterHalfWidth;
      int32_t x1;
      //int32_t x2;

      divisor = (iFilterYEnd - iFilterYBegin) * (iFilterXEnd - iFilterXBegin);

      int32_t wDiff = wDst - (xU - xL) * 4;
      lpbSource = lpbSrc + iChannel;
      lpwDestination = lpbDst + (wDst  * yL) + xL * 4;
      for(; y1 < yU;)
      {


         x1 = xL;
         //x2 = xL - iFilterHalfWidth;

         lpbSource_1 = lpbSource;
         for(; x1 < xU;)
         {

            lpFilter = pFilter;

            dwI = 0;
            lpbSource_2 = lpbSource_1;
            for(int32_t yFilter = iFilterYBegin; yFilter < iFilterYEnd; yFilter++)
            {
               lpbSource_3 = lpbSource_2;
               for(int32_t xFilter = iFilterXBegin; xFilter < iFilterXEnd; xFilter++)
               {
                  dwI += *lpbSource_3 * *lpFilter++;
                  lpbSource_3 += 4;
               }

               lpbSource_2 += wSrc;
            }

            dwI /= divisor;
            *lpwDestination++ = (uchar)dwI;
            *lpwDestination++ = (uchar)dwI;
            *lpwDestination++ = (uchar)dwI;
            *lpwDestination++ = (uchar)lpbSource_1[3];
            //      x1 += 3;
            x1++;
            lpbSource_1 += 4;
            //x2 += 3;
         }
         lpwDestination += wDiff;
         y1++;
         //y2++;
         lpbSource += wSrc;
      }

      delete pFilter;
      return true;
   }



   bool imaging::color_blend(::draw2d::graphics * pgraphics,const RECT & rect,::draw2d::graphics * pdcColorAlpha,point ptAlpha,::draw2d::dib * pdibWork)
   {

      return pgraphics->BitBlt(rect.left,rect.top,width(rect),height(rect),pdcColorAlpha,ptAlpha.x,ptAlpha.y,SRCCOPY);

   }


   bool imaging::true_blend(::draw2d::graphics * pgraphics,const RECT & rect,::draw2d::graphics * pdcColorAlpha,point ptAlpha,::draw2d::dib * pdibWork,::draw2d::dib * pdibWork2,::draw2d::dib * pdibWork3)
   {

      return pgraphics->BitBlt(rect.left,rect.top,width(rect),height(rect),pdcColorAlpha,ptAlpha.x,ptAlpha.y,SRCCOPY);

   }

   // COLOR_DEST = SRC_ALPHA * COLOR_SRC  + (1 - SRC_ALPHA) * COLOR_DST

   /*
   bool imaging::color_blend(::draw2d::graphics * pgraphics, point pt, size size, ::draw2d::graphics * pdcColorAlpha, point ptAlpha, ::draw2d::dib * pdibWork, ::draw2d::dib * pdibWork2)
   {






   }
   */

   bool imaging::color_blend(::draw2d::graphics * pgraphics,point pt,size size,::draw2d::graphics * pdcColorAlpha,point ptAlpha,double dBlend)
   {

      if(pt.x < 0)
      {
         ptAlpha.x += -pt.x;
         pt.x = 0;
      }

      if(pt.y < 0)
      {
         ptAlpha.y += -pt.y;
         pt.y = 0;
      }


      if(dBlend >= 1.0)
      {

         return pgraphics->BitBlt(pt.x,pt.y,size.cx,size.cy,pdcColorAlpha,0,0,SRCCOPY) != FALSE;

      }
      else
      {

         ::visual::dib_sp dib(allocer());

         if(!dib->create(size))
            return false;

         dib->get_graphics()->set_alpha_mode(::draw2d::alpha_mode_set);
         dib->from(point(0,0),pdcColorAlpha,ptAlpha,size);
         dib->channel_multiply(visual::rgba::channel_alpha,dBlend);

         return pgraphics->BitBlt(pt.x,pt.y,size.cx,size.cy,dib->get_graphics(),0,0,SRCCOPY) != FALSE;

      }

   }


   bool imaging::color_blend(::draw2d::graphics * pgraphics,const RECT & rectParam,::draw2d::graphics * pdcColorAlpha,point ptAlpha,double dBlend)
   {
      class rect rect(rectParam);
      return color_blend(pgraphics,rect.top_left(),rect.size(),pdcColorAlpha,ptAlpha,dBlend);
   }



   void imaging::color_blend_24CC(
      LPBYTE lpbAParam,
      int32_t x1,
      int32_t y1,
      int32_t w1,
      int32_t cx,
      int32_t cy,
      COLORREF cr,
      LPBYTE lpbCParam,
      int32_t x3,
      int32_t y3,
      int32_t w3)
   {
      int32_t i,j;
      x1 *= 3;
      x3 *= 3;
      y1 *= w1;
      y3 *= w3;

      BYTE bR = rgba_get_r(cr);
      BYTE bG = rgba_get_g(cr);
      BYTE bB = rgba_get_b(cr);

      lpbAParam = (LPBYTE)lpbAParam + y1 + x1;
      lpbCParam = (LPBYTE)lpbCParam + y3 + x3;

      //   BYTE dwA;
      for(i = 0; i < cy; i++)
      {
         LPBYTE lpbA = (LPBYTE)lpbAParam + (w1 * i);
         LPBYTE lpbC = (LPBYTE)lpbCParam + (w3 * i);
         for(j = 0; j < cx; j++)
         {
            lpbA[0] = (BYTE)(((((uint32_t)lpbA[0]) * ((uint32_t)(255 - lpbC[0])) + (((uint32_t)bB) * ((uint32_t)lpbC[0])))) / 255);
            lpbA[1] = (BYTE)(((((uint32_t)lpbA[1]) * ((uint32_t)(255 - lpbC[1])) + (((uint32_t)bG) * ((uint32_t)lpbC[1])))) / 255);
            lpbA[2] = (BYTE)(((((uint32_t)lpbA[2]) * ((uint32_t)(255 - lpbC[2])) + (((uint32_t)bR) * ((uint32_t)lpbC[2])))) / 255);
            lpbA += 3;
            lpbC += 3;
         }
      }

   }





   bool imaging::alpha_spread_R2(::draw2d::graphics *pdcDst,point ptDst,size size,::draw2d::graphics * pdcSrc,point ptSrc,BYTE bMin)
   {
      if(size.cx <= 0 || size.cy <= 0)
         return true;

      UINT user;
      UINT uiStartScanLine;
      UINT uiScanLineCount;


      rect rectDst(ptDst,size);
      rect rectSrc(ptSrc,size);

      int32_t iwDest;

      BITMAPINFO bmiDst;
      BITMAP   bmDst;
      int32_t iLimitYDst;

      ::draw2d::bitmap_sp bitmapDst(get_app());

      memory memstorageA;
      memory memstorageB;

      if(!GetDeviceContext24BitsCC(
         pdcDst,
         bmDst,
         bmiDst,
         memstorageA,
         bitmapDst,
         rectDst,
         iwDest,
         uiStartScanLine,
         uiScanLineCount,
         iLimitYDst))
      {
         return false;
      }

      LPBYTE lpbDst = memstorageA.get_data();


      int32_t iwSrc;

      BITMAPINFO bmiSrc;
      BITMAP   bmSrc;
      int32_t iLimitYSrc;

      ::draw2d::bitmap_sp bitmapSrc(get_app());

      if(!GetDeviceContext24BitsCC(
         pdcSrc,bmSrc,bmiSrc,
         memstorageB,
         bitmapSrc,
         rectSrc,
         iwSrc,
         user,
         user,
         iLimitYSrc))
      {
         return false;
      }

      LPBYTE lpbSrc = memstorageB.get_data();


      point ptViewportDst = pdcDst->GetViewportOrg();
      point ptViewportSrc = pdcSrc->GetViewportOrg();

      int32_t xvpDst = ptDst.x + ptViewportDst.x;
      int32_t xvpSrc = ptSrc.x + ptViewportSrc.x;

      int32_t iLimitX = size.cx;

      if(bmDst.bmWidth - xvpDst < iLimitX)
      {
         iLimitX = bmDst.bmWidth - xvpDst;
      }
      if(bmSrc.bmWidth - xvpSrc < iLimitX)
      {
         iLimitX = bmSrc.bmWidth - xvpSrc;
      }


      if(xvpDst < 0)
         return false;
      if(xvpSrc < 0)
         return false;

      int32_t iLimitY = MIN(iLimitYDst,iLimitYSrc);

      alpha_spread_R2_24CC(
         lpbDst,
         xvpDst,
         0,
         iwDest,
         iLimitX,
         iLimitY,
         lpbSrc,
         xvpSrc,
         0,
         iwSrc,
         bMin);


#ifdef WINDOWSEX

      ::draw2d::bitmap * pbmpOld = pdcDst->get_current_bitmap();

      if(pbmpOld == NULL)
      {
         ::draw2d::graphics_sp graphicsMem(allocer());
         graphicsMem->CreateCompatibleDC(pdcDst);
         if(!SetDIBits(
            (HDC)graphicsMem->get_os_data(),
            (HBITMAP)bitmapDst->get_os_data(),
            uiStartScanLine,
            uiScanLineCount,
            lpbDst,
            &bmiDst,
            DIB_RGB_COLORS))
            throw 6000;
         ::draw2d::bitmap * pbmpMemOld = graphicsMem->SelectObject(bitmapDst);
         if(!pdcDst->BitBlt(ptDst.x,ptDst.y,size.cx,size.cy,graphicsMem,ptSrc.x,ptSrc.y,SRCCOPY))
         {
            ASSERT(FALSE);
         }
         graphicsMem->SelectObject(pbmpMemOld);
      }
      else
      {
         ::draw2d::bitmap_sp bitmap(get_app());
         bitmap->CreateCompatibleBitmap(pdcDst,1,1);
         //      ::draw2d::bitmap * pbitmap = pdcDst->SelectObject(bitmap);
         if(!SetDIBits(
            (HDC)pdcDst->get_os_data(),
            (HBITMAP)bitmapDst->get_os_data(),
            uiStartScanLine,
            uiScanLineCount,
            lpbDst,
            &bmiDst,
            DIB_RGB_COLORS))
            throw 6000;
         pdcDst->SelectObject(bitmapDst);
      }

#else

      throw todo(get_app());

#endif

      return true;

   }


   bool imaging::alpha_spread(::draw2d::graphics *pdcDst,point ptDst,size size,::draw2d::graphics * pdcSrc,point ptSrc,BYTE bMin,int32_t iRadius)
   {

      if(size.cx <= 0 || size.cy <= 0)
         return true;

      //   single_lock sl(&m_csMem, TRUE);


      UINT user;
      UINT uiStartScanLine;
      UINT uiScanLineCount;


      rect rectDest(ptDst,size);
      rect rectSrc(ptSrc,size);

      int32_t iwDest;

      BITMAPINFO bmiDest;
      BITMAP   bmDest;
      int32_t iLimitYDest;



      ::draw2d::bitmap_sp bitmapDest(get_app());
      memory memstorageA;
      memory memstorageB;

      if(!GetDeviceContext24BitsCC(
         pdcDst,
         bmDest,
         bmiDest,
         memstorageA,
         bitmapDest,
         rectDest,
         iwDest,
         uiStartScanLine,
         uiScanLineCount,
         iLimitYDest))
      {
         return false;
      }

      LPBYTE lpbDst = memstorageA.get_data();


      int32_t iwSrc;

      BITMAPINFO bmiSrc;
      BITMAP   bmSrc;
      int32_t iLimitYSrc;

      ::draw2d::bitmap_sp bitmapSrc(get_app());

      if(!GetDeviceContext24BitsCC(
         pdcSrc,bmSrc,bmiSrc,
         memstorageB,
         bitmapSrc,
         rectSrc,
         iwSrc,
         user,
         user,
         iLimitYSrc))
      {
         return false;
      }

      LPBYTE lpbSrc = memstorageB.get_data();


      point ptViewportDest = pdcDst->GetViewportOrg();
      point ptViewportSrc = pdcSrc->GetViewportOrg();

      int32_t xvpDest = ptDst.x + ptViewportDest.x;
      int32_t xvpSrc = ptDst.y + ptViewportSrc.x;

      int32_t iLimitX = size.cx;

      if(bmDest.bmWidth - xvpDest < iLimitX)
      {
         iLimitX = bmDest.bmWidth - xvpDest;
      }
      if(bmSrc.bmWidth - xvpSrc < iLimitX)
      {
         iLimitX = bmSrc.bmWidth - xvpSrc;
      }


      if(xvpDest < 0)
         return false;
      if(xvpSrc < 0)
         return false;

      int32_t iLimitY = MIN(iLimitYDest,iLimitYSrc);

      alpha_spread__24CC(
         lpbDst,
         xvpDest,
         0,
         iwDest,
         iLimitX,
         iLimitY,
         lpbSrc,
         xvpSrc,
         0,
         iwSrc,
         bMin,iRadius);


#ifdef WINDOWSEX

      ::draw2d::bitmap * pbmpOld = pdcDst->get_current_bitmap();

      if(pbmpOld == NULL)
      {
         ::draw2d::graphics_sp graphicsMem(allocer());
         graphicsMem->CreateCompatibleDC(pdcDst);
         if(!SetDIBits(
            (HDC)graphicsMem->get_os_data(),
            (HBITMAP)bitmapDest->get_os_data(),
            uiStartScanLine,
            uiScanLineCount,
            lpbDst,
            &bmiDest,
            DIB_RGB_COLORS))
            throw 6000;
         ::draw2d::bitmap * pbmpMemOld = graphicsMem->SelectObject(bitmapDest);
         if(!pdcDst->BitBlt(ptDst.x,ptDst.y,size.cx,size.cy,graphicsMem,ptSrc.x,ptSrc.y,SRCCOPY))
         {
            ASSERT(FALSE);
         }
         graphicsMem->SelectObject(pbmpMemOld);
      }
      else
      {
         ::draw2d::bitmap_sp bitmap(get_app());
         bitmap->CreateCompatibleBitmap(pdcDst,1,1);
         //      ::draw2d::bitmap * pbitmap = pdcDst->SelectObject(bitmap);
         if(!SetDIBits(
            (HDC)pdcDst->get_os_data(),
            (HBITMAP)bitmapDest->get_os_data(),
            uiStartScanLine,
            uiScanLineCount,
            lpbDst,
            &bmiDest,
            DIB_RGB_COLORS))
            throw 6000;
         pdcDst->SelectObject(bitmapDest);
      }

      return true;

#else

      throw todo(get_app());

#endif

   }


   void imaging::alpha_spread_R2_24CC(LPBYTE lpbDst,int32_t xDest,int32_t yDest,int32_t wDest,int32_t cx,int32_t cy,LPBYTE lpbSrc,int32_t xSrc,int32_t ySrc,int32_t wSrc,BYTE bMin)
   {

      UNREFERENCED_PARAMETER(xDest);
      UNREFERENCED_PARAMETER(yDest);
      UNREFERENCED_PARAMETER(xSrc);
      UNREFERENCED_PARAMETER(ySrc);
      int32_t iFilterWidth = 2 * 2 + 1;
      int32_t iFilterHeight = 2 * 2 + 1;
      int32_t divisor = iFilterWidth * iFilterHeight;
      BYTE *lpbSource;
      BYTE *lpbSource_1;
      BYTE *lpbSource_2;
      BYTE *lpwDestination;
      BYTE *lpFilter;
      BYTE * pFilter = new BYTE[iFilterHeight *iFilterWidth];
      memset(pFilter,1,iFilterHeight * iFilterWidth);

      int32_t maxx1 = cx;
      int32_t maxy1 = cy;
      //   int32_t maxy2 = cy - iFilterWidth;
      //   int32_t maxy3 = cy - iFilterWidth / 2;
      int32_t max3x1 = maxx1 * 3;
      //   int32_t max3x2 = (maxx1 - iFilterHeight) * 3;
      //   int32_t max3x3 = (maxx1 - iFilterHeight / 2) * 3;
      //   int32_t w = cx * 3;
      uint32_t dwR;
      uint32_t dwG;
      uint32_t dwB;

      int32_t iFilterXBegin;
      int32_t iFilterXEnd;
      int32_t iFilterYBegin;
      int32_t iFilterYEnd;

      int32_t yLowerBound[4];
      int32_t yUpperBound[4];
      int32_t xLowerBound[4];
      int32_t xUpperBound[4];

      // top
      yLowerBound[0] = 0;
      yUpperBound[0] = iFilterWidth / 2;
      xLowerBound[0] = 0;
      xUpperBound[0] = max3x1;

      // left
      yLowerBound[1] = iFilterWidth / 2;
      yUpperBound[1] = maxy1 - iFilterWidth / 2;
      xLowerBound[1] = 0;
      xUpperBound[1] = iFilterWidth / 2;

      // right
      yLowerBound[2] = iFilterWidth / 2;
      yUpperBound[2] = maxy1 - iFilterWidth / 2;
      xLowerBound[2] = max3x1 - iFilterWidth / 2;
      xUpperBound[2] = max3x1;

      // bottom
      yLowerBound[3] = maxy1 - iFilterWidth / 2;
      yUpperBound[3] = maxy1;
      xLowerBound[3] = 0;
      xUpperBound[3] = max3x1;

      iFilterYBegin  = 0;
      iFilterYEnd    = iFilterWidth;
      iFilterXBegin = 0;
      iFilterXEnd = iFilterHeight;

      int32_t iFilterHalfWidth = iFilterWidth / 2;
      int32_t iFilterHalfWidthBytes = iFilterHalfWidth * 3;

      int32_t yL = iFilterHalfWidth;
      int32_t yU = maxy1 - iFilterHalfWidth;
      int32_t xL = iFilterHalfWidthBytes;
      int32_t xU = max3x1 - iFilterHalfWidthBytes;


      int32_t y1 = yL;
      int32_t y2 = yL - iFilterHalfWidth;
      int32_t x1;
      int32_t x2;

      uint32_t bMin3 = bMin * 3;

      divisor = (iFilterYEnd - iFilterYBegin) * (iFilterXEnd - iFilterXBegin);

      bool bSpread;

      for(; y1 < yU;)
      {
         lpbSource = lpbSrc + (wSrc * y2);

         x1 = xL;
         x2 = xL - iFilterHalfWidthBytes;
         lpwDestination = lpbDst + (wDest  * y1) + x1;
         for(; x1 < xU;)
         {
            lpbSource_1 = lpbSource + x2;
            lpFilter = pFilter;

            dwR = 0;
            dwG = 0;
            dwB = 0;
            bSpread = false;
            for(int32_t yFilter = iFilterYBegin; yFilter < iFilterYEnd; yFilter++)
            {
               lpbSource_2 = lpbSource_1 + (wSrc * yFilter);
               for(int32_t xFilter = iFilterXBegin; xFilter < iFilterXEnd; xFilter++)
               {
                  dwB = lpbSource_2[0];
                  dwG = lpbSource_2[1];
                  dwR = lpbSource_2[2];

                  if(dwR + dwG + dwB > bMin3)
                  {
                     bSpread = true;
                     break;
                  }
                  lpbSource_2 += 3;
               }
            }

            if(bSpread)
            {
               *((uint32_t *)lpwDestination) |= 0x00ffffff;
               lpwDestination += 3;
            }
            else
            {
               lpwDestination += 3;
            }
            x1 += 3;
            x2 += 3;
         }
         y1++;
         y2++;
      }

      delete pFilter;
   }


   void imaging::alpha_spread__24CC(
      LPBYTE lpbDst,int32_t xDest,int32_t yDest,int32_t wDest,int32_t cx,int32_t cy,
      LPBYTE lpbSrc,int32_t xSrc,int32_t ySrc,int32_t wSrc,
      BYTE bMin,int32_t iRadius)
   {
      UNREFERENCED_PARAMETER(xDest);
      UNREFERENCED_PARAMETER(yDest);
      UNREFERENCED_PARAMETER(xSrc);
      UNREFERENCED_PARAMETER(ySrc);
      int32_t iFilterW      = iRadius * 2 + 1;
      int32_t iFilterH      = iRadius * 2 + 1;
      int32_t iFilterHalfW  = iFilterW / 2;
      int32_t iFilterHalfH  = iFilterH / 2;
      int32_t iFilterArea   = iFilterW * iFilterH;
      int32_t divisor       = iFilterW * iFilterH;
      BYTE *lpbSource;
      BYTE *lpbSource_1;
      BYTE *lpbSource_2;
      BYTE *lpwDestination;
      BYTE *lpFilter;
      BYTE *pFilter;


      int32_t i;
      int32_t x;
      int32_t y;
      int32_t x1;
      int32_t y1;
      int32_t x2;
      int32_t y2;

      int32_t iRadius2 = iRadius * iRadius;
      int32_t r2;

      auto & filter = m_alpha_spread__24CC_filterMap[iRadius];

      if(filter.is_set())
      {

         pFilter = filter->get_data();

      }
      else
      {
         filter = canew(memory());
         filter->allocate(iFilterArea);
         pFilter = filter->get_data();
         for(y = 0; y <= iFilterHalfH; y++)
         {
            for(x = 0; x <= iFilterHalfW; x++)
            {
               x1 = iFilterHalfW - x;
               y1 = iFilterHalfH - y;
               r2 = x1 * x1 + y1 * y1;
               if(r2 <= iRadius2)
                  i = 1;
               else
                  i = 0;
               pFilter[x + y * iFilterW]                                   = (byte)i;
               pFilter[iFilterW - 1 - x + y * iFilterW]                    = (byte)i;
               pFilter[iFilterW - 1 - x + (iFilterH - 1 - y) * iFilterW]   = (byte)i;
               pFilter[x + (iFilterH - 1 - y) * iFilterW]                  = (byte)i;
            }
         }
      }

      int32_t maxx1 = cx;
      int32_t maxy1 = cy;
      //   int32_t maxy2 = cy - iFilterW;
      //   int32_t maxy3 = cy - iFilterW / 2;
      int32_t max3x1 = maxx1 * 3;
      //   int32_t max3x2 = (maxx1 - iFilterH) * 3;
      //   int32_t max3x3 = (maxx1 - iFilterH / 2) * 3;
      //   int32_t w = cx * 3;
      uint32_t dwR;
      uint32_t dwG;
      uint32_t dwB;


      int32_t iFilterXLowerBound;
      int32_t iFilterXUpperBound;
      int32_t iFilterYLowerBound;
      int32_t iFilterYUpperBound;

      int32_t yLowerBound[4];
      int32_t yUpperBound[4];
      int32_t xLowerBound[4];
      int32_t xUpperBound[4];

      // top
      xLowerBound[0] = 0;
      xUpperBound[0] = cx - 1;
      yLowerBound[0] = 0;
      yUpperBound[0] = iFilterHalfH - 1;

      // left
      xLowerBound[1] = 0;
      xUpperBound[1] = iFilterHalfW - 1;
      yLowerBound[1] = iFilterHalfH;
      yUpperBound[1] = cy - iFilterHalfH - 1;

      // right
      xLowerBound[2] = cx - iFilterHalfW;
      xUpperBound[2] = cx - 1;
      yLowerBound[2] = iFilterHalfH;
      yUpperBound[2] = cy - iFilterHalfH - 1;

      // bottom
      xLowerBound[3] = 0;
      xUpperBound[3] = cx - 1;
      yLowerBound[3] = cy - iFilterHalfW;
      yUpperBound[3] = cy - 1;

      int32_t xL;
      int32_t xU;
      int32_t yL;
      int32_t yU;

      bool bSpread;
      uint32_t bMin3 = bMin * 3;


      for(i = 0; i < 4; i++)
      {
         xL = xLowerBound[i];
         xU = xUpperBound[i];
         yL = yLowerBound[i];
         yU = yUpperBound[i];

         y1 = yL;
         y2 = y1 - iFilterHalfH;
         for(; y1 <= yU;)
         {
            if(y1 < iFilterHalfH)
            {
               iFilterYLowerBound = iFilterHalfH - y1;
            }
            else
            {
               iFilterYLowerBound = 0;
            }
            if(y1 > (cy - iFilterHalfH))
            {
               iFilterYUpperBound = iFilterH - (y1 - (cy - iFilterHalfH)) - 1;
            }
            else
            {
               iFilterYUpperBound = iFilterH - 1;
            }

            lpbSource = lpbSrc + (wSrc * MAX(y2,0));

            x1 = xL;
            x2 = (x1 - iFilterHalfW) * 3;
            lpwDestination = lpbDst + (wDest  * y1) + x1 * 3;
            for(; x1 <= xU;)
            {
               if(x1 < iFilterHalfH)
               {
                  iFilterXLowerBound = iFilterHalfH - x1;
               }
               else
               {
                  iFilterXLowerBound = 0;
               }
               if(x1 > (cx - iFilterHalfH + 1))
               {
                  iFilterXUpperBound = iFilterH - (x1 - (cx - iFilterHalfH + 1));
               }
               else
               {
                  iFilterXUpperBound = iFilterH - 1;
               }

               lpbSource_1 = lpbSource + MAX(x2,0);


               dwR = 0;
               dwG = 0;
               dwB = 0;
               bSpread = false;
               for(int32_t yFilter = iFilterYLowerBound; yFilter < iFilterYUpperBound; yFilter++)
               {
                  lpbSource_2 = lpbSource_1 + (wSrc * yFilter);
                  lpFilter = pFilter + yFilter * iFilterW + iFilterXLowerBound;
                  for(int32_t xFilter = iFilterXLowerBound; xFilter < iFilterXUpperBound; xFilter++)
                  {
                     if(*lpFilter >= 1)
                     {
                        dwB = lpbSource_2[0];
                        dwG = lpbSource_2[1];
                        dwR = lpbSource_2[2];

                        if(dwR + dwG + dwB > bMin3)
                        {
                           *((uint32_t *)lpwDestination) |= 0x00ffffff;
                           goto breakFilter;
                        }
                     }
                     lpFilter++;
                     lpbSource_2 += 3;
                  }
               }
            breakFilter:
               lpwDestination += 3;
               x1++;
               x2 += 3;
            }
            y1++;
            y2++;
         }
      }

      iFilterYLowerBound = 0;
      iFilterYUpperBound = iFilterW - 1;
      iFilterXLowerBound = 0;
      iFilterXUpperBound = iFilterH - 1;

      int32_t iFilterHalfWidth = iFilterW / 2;
      int32_t iFilterHalfWidthBytes = iFilterHalfWidth * 3;

      yL = iFilterHalfWidth;
      yU = maxy1 - iFilterHalfWidth;
      xL = iFilterHalfWidthBytes;
      xU = max3x1 - iFilterHalfWidthBytes;

      y1 = yL;
      y2 = yL - iFilterHalfWidth;



      divisor = (iFilterYUpperBound - iFilterYLowerBound + 1) * (iFilterXUpperBound - iFilterXLowerBound + 1);


      for(; y1 < yU;)
      {
         lpbSource = lpbSrc + (wSrc * y2);

         x1 = xL;
         x2 = xL - iFilterHalfWidthBytes;
         lpwDestination = lpbDst + (wDest  * y1) + x1;
         for(; x1 < xU;)
         {
            lpbSource_1 = lpbSource + x2;
            lpFilter = pFilter;

            dwR = 0;
            dwG = 0;
            dwB = 0;
            bSpread = false;
            for(int32_t yFilter = iFilterYLowerBound; yFilter <= iFilterYUpperBound; yFilter++)
            {
               lpbSource_2 = lpbSource_1 + (wSrc * yFilter);
               lpFilter = pFilter + yFilter * iFilterW + iFilterXLowerBound;
               for(int32_t xFilter = iFilterXLowerBound; xFilter <= iFilterXUpperBound; xFilter++)
               {
                  if(*lpFilter >= 1)
                  {
                     dwB = lpbSource_2[0];
                     dwG = lpbSource_2[1];
                     dwR = lpbSource_2[2];

                     if(dwR + dwG + dwB > bMin3)
                     {
                        bSpread = true;
                        break;
                     }
                  }
                  lpFilter++;
                  lpbSource_2 += 3;
               }
            }

            if(bSpread)
            {
               *((uint32_t *)lpwDestination) |= 0x00ffffff;
               lpwDestination += 3;
            }
            else
            {
               lpwDestination += 3;
            }
            x1 += 3;
            x2 += 3;
         }
         y1++;
         y2++;
      }

   }

   bool imaging::channel_spread(
      ::draw2d::graphics *pdcDst,
      point ptDst,
      size size,
      ::draw2d::graphics * pdcSrc,
      point ptSrc,
      int32_t iChannel,int32_t iRadius)
   {

      return channel_spread_set_color(
         pdcDst,
         ptDst,
         size,
         pdcSrc,
         ptSrc,
         iChannel,
         iRadius,
         0xffffffff);

   }


   bool imaging::channel_spread_set_color(
      ::draw2d::graphics *pdcDst,
      point ptDst,
      size size,
      ::draw2d::graphics * pdcSrc,
      point ptSrc,
      int32_t iChannel,int32_t iRadius,
      COLORREF cr)
   {
      if(size.is_empty())
         return true;

      ::draw2d::dib_sp dibDst(allocer());

      if(!dibDst.is_set())
         return false;

      if(!dibDst->create(size))
         return false;

      ::draw2d::dib_sp dibSrc(allocer());

      if(!dibSrc.is_set())
         return false;

      if(!dibSrc->create(size))
         return false;


      dibSrc->get_graphics()->set_alpha_mode(::draw2d::alpha_mode_set);

      if(!dibSrc->from(null_point(),pdcSrc,ptSrc,size))
         return false;

      if(!channel_spread__32CC(
         dibDst,
         dibSrc,
         iChannel,
         iRadius,cr))
         return false;


      if(!dibDst->to(pdcDst,ptDst,size))
         return false;

      return true;

   }


   bool imaging::spread(
      ::draw2d::graphics *pdcDst,
      point ptDst,
      size size,
      ::draw2d::graphics * pdcSrc,
      point ptSrc,
      int32_t iRadius)
   {

      return spread_set_color(
         pdcDst,
         ptDst,
         size,
         pdcSrc,
         ptSrc,
         iRadius,
         0xffffffff);

   }


   bool imaging::spread_set_color(
      ::draw2d::graphics *pdcDst,
      point ptDst,
      size size,
      ::draw2d::graphics * pdcSrc,
      point ptSrc,
      int32_t iRadius,
      COLORREF cr)
   {
      if(size.is_empty())
         return true;

      ::draw2d::dib_sp dibDst(allocer());

      if(!dibDst.is_set())
         return false;

      if(!dibDst->create(size))
         return false;

      ::draw2d::dib_sp dibSrc(allocer());

      if(!dibSrc.is_set())
         return false;

      if(!dibSrc->create(size))
         return false;

      dibSrc->get_graphics()->set_alpha_mode(::draw2d::alpha_mode_set);

      if(!dibSrc->from(null_point(),pdcSrc,ptSrc,size))
         return false;

      if(!spread__32CC(
         dibDst,
         dibSrc,
         iRadius,cr))
         return false;

      if(!dibDst->to(pdcDst,ptDst,size))
         return false;

      return true;

   }


   bool imaging::channel_spread__32CC(::draw2d::dib * pdibDst,::draw2d::dib * pdibSrc,int32_t iChannel,int32_t iRadius,COLORREF crSpreadSetColor)
   {
      synch_lock sl(&m_mutex);
      int32_t iFilterW      = iRadius * 2 + 1;
      int32_t iFilterH      = iRadius * 2 + 1;
      int32_t iFilterHalfW  = iRadius;
      int32_t iFilterHalfH  = iRadius;
      int32_t iFilterArea   = iFilterW * iFilterH;
      int32_t divisor       = iFilterW * iFilterH;
      BYTE *lpbSource;
      BYTE *lpbSource_1;
      BYTE *lpbSource_2;
      BYTE *lpwDestination;
      BYTE *lpFilter;
      BYTE * pFilter;

      int32_t i;
      int32_t x;
      int32_t y;
      int32_t x1;
      int32_t y1;
      int32_t x2;
      int32_t y2;

      int32_t iRadiusSquare = iRadius * iRadius;
      int32_t rSquare;

      auto & filter = m_alpha_spread__32CC_filterMap[iRadius];

      if (filter.is_set())
      {
         pFilter = filter->get_data();
      }
      else
      {
         filter = canew(memory());
         filter->allocate(iFilterArea);
         pFilter = filter->get_data();
         for(y = 0; y < iFilterH; y++)
         {
            for(x = 0; x < iFilterW; x++)
            {
               x1 = iFilterHalfW - x;
               y1 = iFilterHalfH - y;
               rSquare = x1 * x1 + y1 * y1;
               if(rSquare <= iRadiusSquare)
                  i = 1;
               else
                  i = 0;
               pFilter[x + y * iFilterW] = (byte)i;
            }
         }
      }

      int32_t cx = pdibDst->m_size.cx;
      int32_t cy = pdibDst->m_size.cy;

      if(cx != pdibSrc->m_size.cx
         || cy != pdibSrc->m_size.cy)
         return false;

      LPBYTE lpbDst = (LPBYTE)pdibDst->get_data();
      LPBYTE lpbSrc = (LPBYTE)pdibSrc->get_data();

      //int32_t wSrc = cx * 4;
      //int32_t wDst = cx * 4;
      int32_t wSrc = pdibSrc->m_iScan;
      int32_t wDst = pdibDst->m_iScan;

      int32_t maxx1 = cx;
      int32_t maxy1 = cy;
      //   int32_t maxy2 = cy - iFilterW;
      //   int32_t maxy3 = cy - iFilterW / 2;
      int32_t max3x1 = maxx1 * 4;
      //   int32_t max3x2 = (maxx1 - iFilterH) * 4;
      //   int32_t max3x3 = (maxx1 - iFilterH / 2) * 4;
      //int32_t w = cx * 4;

      ::draw2d::copy_colorref(cx,cy,(COLORREF *) lpbDst,pdibDst->m_iScan,(COLORREF *)lpbSrc,pdibSrc->m_iScan);
      //memcpy(lpbDst,lpbSrc,cx * cy * 4);


      int32_t iFilterXLowerBound;
      int32_t iFilterXUpperBound;
      int32_t iFilterYLowerBound;
      int32_t iFilterYUpperBound;

      int32_t yLowerBound[4];
      int32_t yUpperBound[4];
      int32_t xLowerBound[4];
      int32_t xUpperBound[4];

      // top
      xLowerBound[0] = 0;
      xUpperBound[0] = cx - 1;
      yLowerBound[0] = 0;
      yUpperBound[0] = iFilterHalfH - 1;

      // left
      xLowerBound[1] = 0;
      xUpperBound[1] = iFilterHalfW - 1;
      yLowerBound[1] = iFilterHalfH;
      yUpperBound[1] = cy - iFilterHalfH - 1;

      // right
      xLowerBound[2] = cx - iFilterHalfW;
      xUpperBound[2] = cx - 1;
      yLowerBound[2] = iFilterHalfH;
      yUpperBound[2] = cy - iFilterHalfH - 1;

      // bottom
      xLowerBound[3] = 0;
      xUpperBound[3] = cx - 1;
      yLowerBound[3] = cy - iFilterHalfH;
      yUpperBound[3] = cy - 1;

      int32_t xL;
      int32_t xU;
      int32_t yL;
      int32_t yU;


      int32_t xMax = cx - 1;
      int32_t yMax = cy - 1;

      // limits due the filter
      int32_t xMaxFilterBound = xMax - iFilterHalfW;
      int32_t yMaxFilterBound = yMax - iFilterHalfH;

      int32_t xFilterMax = iFilterW - 1;
      int32_t yFilterMax = iFilterH - 1;

      for(i = 0; i < 4; i++)
      {
         xL = xLowerBound[i];
         xU = xUpperBound[i];
         yL = yLowerBound[i];
         yU = yUpperBound[i];

         y1 = yL;
         y2 = y1 - iFilterHalfH;
         for(; y1 <= yU;)
         {
            if(y1 < iFilterHalfH)
            {
               iFilterYLowerBound = iFilterHalfH - y1;
            }
            else
            {
               iFilterYLowerBound = 0;
            }
            if(y1 > yMaxFilterBound)
            {
               iFilterYUpperBound = yFilterMax - (y1 - yMaxFilterBound);
            }
            else
            {
               iFilterYUpperBound = yFilterMax;
            }

            lpbSource = lpbSrc + wSrc * y2;

            x1 = xL;
            x2 = (x1 - iFilterHalfW) * 4;
            lpwDestination = lpbDst + (wDst  * y1) + x1 * 4;
            if(*((uint32_t *)lpwDestination) != 0xffffffff)
            {
               for(; x1 <= xU; x1++)
               {
                  if(x1 < iFilterHalfH)
                  {
                     iFilterXLowerBound = iFilterHalfH - x1;
                  }
                  else
                  {
                     iFilterXLowerBound = 0;
                  }
                  if(x1 > xMaxFilterBound)
                  {
                     iFilterXUpperBound = xFilterMax - (x1 - xMaxFilterBound);
                  }
                  else
                  {
                     iFilterXUpperBound = xFilterMax;
                  }

                  lpbSource_1 = lpbSource + MAX(x2,0) + iChannel;


                  for(int32_t yFilter = iFilterYLowerBound; yFilter < iFilterYUpperBound; yFilter++)
                  {
                     lpbSource_2 = lpbSource_1 + (wSrc * yFilter);
                     lpFilter = pFilter + yFilter * iFilterW + iFilterXLowerBound;
                     for(int32_t xFilter = iFilterXLowerBound; xFilter < iFilterXUpperBound; xFilter++)
                     {
                        if(*lpFilter >= 1)
                        {
                           if(lpbSource_2[0] > 0)
                           {
                              *((uint32_t *)lpwDestination) = crSpreadSetColor;
                              goto breakFilter;
                           }
                        }
                        lpFilter++;
                        lpbSource_2 += 4;
                     }
                  }
               breakFilter:
                  lpwDestination += 4;
                  x2 += 4;
               }
            }
            y1++;
            y2++;
         }
      }

      iFilterYLowerBound = 0;
      iFilterYUpperBound = iFilterW - 1;
      iFilterXLowerBound = 0;
      iFilterXUpperBound = iFilterH - 1;

      int32_t iFilterHalfWidth = iFilterW / 2;
      int32_t iFilterHalfWidthBytes = iFilterHalfWidth * 4;

      yL = iFilterHalfWidth;
      yU = maxy1 - iFilterHalfWidth;
      xL = iFilterHalfWidthBytes;
      xU = max3x1 - iFilterHalfWidthBytes;

      y1 = yL;
      y2 = yL - iFilterHalfWidth;



      divisor = (iFilterYUpperBound - iFilterYLowerBound + 1) * (iFilterXUpperBound - iFilterXLowerBound + 1);


      for(; y1 < yU;)
      {
         lpbSource = lpbSrc + (wSrc * y2) + iChannel;

         x1 = xL;
         x2 = xL - iFilterHalfWidthBytes;
         lpwDestination = lpbDst + (wDst  * y1) + x1;
         for(; x1 < xU;)
         {
            lpbSource_1 = lpbSource + x2;
            lpFilter = pFilter;

            if(*((uint32_t *)lpwDestination) != 0xffffffff)
            {
               for(int32_t yFilter = iFilterYLowerBound; yFilter <= iFilterYUpperBound; yFilter++)
               {
                  lpbSource_2 = lpbSource_1 + (wSrc * yFilter);
                  lpFilter = pFilter + yFilter * iFilterW + iFilterXLowerBound;
                  for(int32_t xFilter = iFilterXLowerBound; xFilter <= iFilterXUpperBound; xFilter++)
                  {
                     if(*lpFilter >= 1)
                     {
                        if(lpbSource_2[0] > 0)
                        {
                           *((uint32_t *)lpwDestination) = crSpreadSetColor;
                           goto breakFilter2;
                        }
                     }
                     lpFilter++;
                     lpbSource_2 += 4;
                  }
               }
            }
         breakFilter2:
            lpwDestination += 4;
            x1 += 4;
            x2 += 4;
         }
         y1++;
         y2++;
      }

      return true;
   }
























   bool imaging::spread__32CC(::draw2d::dib * pdibDst,::draw2d::dib * pdibSrc,int32_t iRadius,COLORREF crSpreadSetColor)
   {
      int32_t iFilterW      = iRadius * 2 + 1;
      int32_t iFilterH      = iRadius * 2 + 1;
      int32_t iFilterHalfW  = iRadius;
      int32_t iFilterHalfH  = iRadius;
      int32_t iFilterArea   = iFilterW * iFilterH;
      int32_t divisor       = iFilterW * iFilterH;
      BYTE *lpbSource;
      BYTE *lpbSource_1;
      BYTE *lpbSource_2;
      BYTE *lpwDestination;
      BYTE *lpFilter;
      BYTE * pFilter;

      int32_t i;
      int32_t x;
      int32_t y;
      int32_t x1;
      int32_t y1;
      int32_t x2;
      int32_t y2;

      int32_t iRadius2 = iRadius * iRadius;
      int32_t r2;

      auto pmemory = get_thread()->oprop("m_alpha_spread__32CC_filterMap(" + ::str::from(iRadius) + ")").cast < memory >();

      if (pmemory != NULL)
      {
         pFilter = pmemory->get_data();
      }
      else
      {
         pmemory = canew(memory());
         pmemory->allocate(iFilterArea);
         pFilter = pmemory->get_data();
         for(y = 0; y < iFilterHalfH; y++)
         {
            for(x = 0; x < iFilterHalfW; x++)
            {
               x1 = iFilterHalfW - x;
               y1 = iFilterHalfH - y;
               r2 = x1 * x1 + y1 * y1;
               if(r2 <= iRadius2)
                  i = 1;
               else
                  i = 0;
               pFilter[x + y * iFilterW]                                   = (byte)i;
            }
         }
         get_thread()->oprop("m_alpha_spread__32CC_filterMap(" + ::str::from(iRadius) + ")") = pmemory;
      }

      int32_t cx = pdibDst->m_size.cx;
      int32_t cy = pdibDst->m_size.cy;

      if(cx != pdibSrc->m_size.cx
         || cy != pdibSrc->m_size.cy)
         return false;

      LPBYTE lpbDst = (LPBYTE)pdibDst->get_data();
      LPBYTE lpbSrc = (LPBYTE)pdibSrc->get_data();

      //int32_t wSrc = cx * 4;
      //int32_t wDst = cx * 4;
      int32_t wSrc = pdibSrc->m_iScan;
      int32_t wDst = pdibDst->m_iScan;

      int32_t maxx1 = cx;
      int32_t maxy1 = cy;
      //   int32_t maxy2 = cy - iFilterW;
      //   int32_t maxy3 = cy - iFilterW / 2;
      int32_t max3x1 = maxx1 * 4;
      //   int32_t max3x2 = (maxx1 - iFilterH) * 4;
      //   int32_t max3x3 = (maxx1 - iFilterH / 2) * 4;
      //int32_t w = cx * 4;

      ::draw2d::copy_colorref(cx,cy,(COLORREF *)lpbDst,pdibDst->m_iScan,(COLORREF *)lpbSrc,pdibSrc->m_iScan);
      //memcpy(lpbDst,lpbSrc,cx * cy * 4);


      int32_t iFilterXLowerBound;
      int32_t iFilterXUpperBound;
      int32_t iFilterYLowerBound;
      int32_t iFilterYUpperBound;

      int32_t yLowerBound[4];
      int32_t yUpperBound[4];
      int32_t xLowerBound[4];
      int32_t xUpperBound[4];

      // top
      xLowerBound[0] = 0;
      xUpperBound[0] = cx - 1;
      yLowerBound[0] = 0;
      yUpperBound[0] = iFilterHalfH - 1;

      // left
      xLowerBound[1] = 0;
      xUpperBound[1] = iFilterHalfW - 1;
      yLowerBound[1] = iFilterHalfH;
      yUpperBound[1] = cy - iFilterHalfH - 1;

      // right
      xLowerBound[2] = cx - iFilterHalfW;
      xUpperBound[2] = cx - 1;
      yLowerBound[2] = iFilterHalfH;
      yUpperBound[2] = cy - iFilterHalfH - 1;

      // bottom
      xLowerBound[3] = 0;
      xUpperBound[3] = cx - 1;
      yLowerBound[3] = cy - iFilterHalfH;
      yUpperBound[3] = cy - 1;

      int32_t xL;
      int32_t xU;
      int32_t yL;
      int32_t yU;


      int32_t xMax = cx - 1;
      int32_t yMax = cy - 1;

      // limits due the filter
      int32_t xMaxFilterBound = xMax - iFilterHalfW;
      int32_t yMaxFilterBound = yMax - iFilterHalfH;

      int32_t xFilterMax = iFilterW - 1;
      int32_t yFilterMax = iFilterH - 1;

      for(i = 0; i < 4; i++)
      {
         xL = xLowerBound[i];
         xU = xUpperBound[i];
         yL = yLowerBound[i];
         yU = yUpperBound[i];

         y1 = yL;
         y2 = y1 - iFilterHalfH;
         for(; y1 <= yU;)
         {
            if(y1 < iFilterHalfH)
            {
               iFilterYLowerBound = iFilterHalfH - y1;
            }
            else
            {
               iFilterYLowerBound = 0;
            }
            if(y1 > yMaxFilterBound)
            {
               iFilterYUpperBound = yFilterMax - (y1 - yMaxFilterBound);
            }
            else
            {
               iFilterYUpperBound = yFilterMax;
            }

            lpbSource = lpbSrc + wSrc * y2;

            x1 = xL;
            x2 = (x1 - iFilterHalfW) * 4;
            lpwDestination = lpbDst + (wDst  * y1) + x1 * 4;
            if(*((uint32_t *)lpwDestination) != crSpreadSetColor)
            {
               for(; x1 <= xU; x1++)
               {
                  if(x1 < iFilterHalfH)
                  {
                     iFilterXLowerBound = iFilterHalfH - x1;
                  }
                  else
                  {
                     iFilterXLowerBound = 0;
                  }
                  if(x1 > xMaxFilterBound)
                  {
                     iFilterXUpperBound = xFilterMax - (x1 - xMaxFilterBound);
                  }
                  else
                  {
                     iFilterXUpperBound = xFilterMax;
                  }

                  lpbSource_1 = lpbSource ;


                  for(int32_t yFilter = iFilterYLowerBound; yFilter < iFilterYUpperBound; yFilter++)
                  {
                     lpbSource_2 = lpbSource_1 + (wSrc * yFilter);
                     lpFilter = pFilter + yFilter * iFilterW + iFilterXLowerBound;
                     for(int32_t xFilter = iFilterXLowerBound; xFilter < iFilterXUpperBound; xFilter++)
                     {
                        if(*lpFilter >= 1)
                        {
                           if(*((uint32_t *)lpbSource_2))
                           {
                              *((uint32_t *)lpwDestination) = crSpreadSetColor;
                              goto breakFilter;
                           }
                        }
                        lpFilter++;
                        lpbSource_2 += 4;
                     }
                  }
               breakFilter:
                  lpwDestination += 4;
                  x2 += 4;
               }
            }
            y1++;
            y2++;
         }
      }

      iFilterYLowerBound = 0;
      iFilterYUpperBound = iFilterW - 1;
      iFilterXLowerBound = 0;
      iFilterXUpperBound = iFilterH - 1;

      int32_t iFilterHalfWidth = iFilterW / 2;
      int32_t iFilterHalfWidthBytes = iFilterHalfWidth * 4;

      yL = iFilterHalfWidth;
      yU = maxy1 - iFilterHalfWidth;
      xL = iFilterHalfWidthBytes;
      xU = max3x1 - iFilterHalfWidthBytes;

      y1 = yL;
      y2 = yL - iFilterHalfWidth;



      divisor = (iFilterYUpperBound - iFilterYLowerBound + 1) * (iFilterXUpperBound - iFilterXLowerBound + 1);


      for(; y1 < yU;)
      {
         lpbSource = lpbSrc + (wSrc * y2);

         x1 = xL;
         x2 = xL - iFilterHalfWidthBytes;
         lpwDestination = lpbDst + (wDst  * y1) + x1;
         for(; x1 < xU;)
         {
            lpbSource_1 = lpbSource + x2;
            lpFilter = pFilter;

            if(*((uint32_t *)lpwDestination) != 0xffffffff)
            {
               for(int32_t yFilter = iFilterYLowerBound; yFilter <= iFilterYUpperBound; yFilter++)
               {
                  lpbSource_2 = lpbSource_1 + (wSrc * yFilter);
                  lpFilter = pFilter + yFilter * iFilterW + iFilterXLowerBound;
                  for(int32_t xFilter = iFilterXLowerBound; xFilter <= iFilterXUpperBound; xFilter++)
                  {
                     if(*lpFilter >= 1)
                     {
                        if(lpbSource_2[0] > 0)
                        {
                           *((uint32_t *)lpwDestination) = crSpreadSetColor;
                           goto breakFilter2;
                        }
                     }
                     lpFilter++;
                     lpbSource_2 += 4;
                  }
               }
            }
         breakFilter2:
            lpwDestination += 4;
            x1 += 4;
            x2 += 4;
         }
         y1++;
         y2++;
      }

      return true;
   }

































































   bool imaging::pixelate(::draw2d::graphics *pdcDst,int32_t xDest,int32_t yDest,int32_t cx,int32_t cy,::draw2d::graphics * pdcSrc,int32_t xSrc,int32_t ySrc,int32_t iSize)
   {

      /*::draw2d::graphics_sp & dibA->get_graphics()  = pthreadsafedata->m_dcA;
      ::draw2d::graphics_sp & dibB->get_graphics()  = pthreadsafedata->m_dcA;

      ::draw2d::bitmap & bitmapA  = pthreadsafedata->bitmapA;
      ::draw2d::bitmap & bitmapB  = pthreadsafedata->bitmapB;

      int32_t cxDest = cx / iSize;
      int32_t cyDest = cy / iSize;

      if(dibA->get_graphics()->get_os_data() == NULL)
      {
      ::draw2d::graphics_sp dcScreen;
      spgraphicsScreen->CreateDC("DISPLAY", NULL, NULL, NULL);
      dibA->get_graphics()->CreateCompatibleDC(&dcScreen);
      }
      if(dibB->get_graphics()->get_os_data() == NULL)
      {
      ::draw2d::graphics_sp dcScreen;
      spgraphicsScreen->CreateDC("DISPLAY", NULL, NULL, NULL);
      dibB->get_graphics()->CreateCompatibleDC(&dcScreen);
      }
      if(spbitmapB->m_hObject == NULL)
      {
      ::draw2d::graphics_sp dcScreen;
      spgraphicsScreen->CreateDC("DISPLAY", NULL, NULL, NULL);
      spbitmapB->CreateBitmap(cxDest + 100, cyDest + 100, 1, 32, NULL);

      }
      else
      {
      BITMAP bm;
      if(!spbitmapB->GetObject(sizeof(bm), &bm))
      throw 3500;
      if(bm.bmWidth < cx ||
      bm.bmHeight  < cy)
      {
      spbitmapB->delete_object();
      spbitmapB->CreateBitmap(cxDest + 100, cyDest + 100, 1, 32, NULL);
      }
      }
      dibB->get_graphics()->SelectObject(bitmapB);

      dibB->get_graphics()->SetStretchBltMode(HALFTONE);
      dibB->get_graphics()->StretchBlt(0, 0, cxDest, cyDest, pdcSrc, xSrc, ySrc, cx, cy, SRCCOPY);

      int32_t x1 = xDest;
      int32_t x2 = xDest + iSize;
      int32_t y1;
      int32_t y2;
      for(int32_t x = 0; x < cxDest; x++)
      {
      y1 = yDest;
      y2 = yDest + iSize;
      for(int32_t y = 0; y < cyDest; y++)
      {
      pdcDst->FillSolidRect(x1, y1, x2, y2, dibB->get_graphics()->GetPixel(x, y));
      y1 += iSize;
      y2 += iSize;
      }
      x1 += iSize;
      x2 += iSize;
      }

      return true;*/




      if(cx <= 0 || cy <= 0)
         return true;

      UINT user;
      UINT uiStartScanLine;
      UINT uiScanLineCount;


      memory memstorageA;
      memory memstorageB;
      memory memstorageC;

      rect rectDest;
      rectDest.left = xDest;
      rectDest.right = xDest + cx;
      rectDest.top = yDest;
      rectDest.bottom = yDest + cy;

      rect rectSrc;
      rectSrc.left = xSrc;
      rectSrc.right = xSrc + cx;
      rectSrc.top = ySrc;
      rectSrc.bottom = ySrc + cy;

      int32_t iwDest;

      BITMAPINFO bmiDest;
      BITMAP   bmDest;
      int32_t iLimitYDest;

      ::draw2d::bitmap_sp bitmapDest(get_app());

      if(!GetDeviceContext24BitsCC(
         pdcDst,
         bmDest,
         bmiDest,
         memstorageA,
         bitmapDest,
         rectDest,
         iwDest,
         uiStartScanLine,
         uiScanLineCount,
         iLimitYDest))
      {
         return false;
      }

      LPBYTE lpbDst = memstorageA.get_data();


      int32_t iwSrc;

      BITMAPINFO bmiSrc;
      BITMAP   bmSrc;
      int32_t iLimitYSrc;

      ::draw2d::bitmap_sp bitmapSrc(get_app());

      if(!GetDeviceContext24BitsCC(
         pdcSrc,bmSrc,bmiSrc,
         memstorageB,
         bitmapSrc,
         rectSrc,
         iwSrc,
         user,
         user,
         iLimitYSrc))
      {
         return false;
      }

      LPBYTE lpbSrc = memstorageB.get_data();

      memstorageC.allocate((cx + iSize) / iSize * (cy + iSize) / iSize);
      //   LPBYTE lpc = memstorageC.get_data();


      point ptViewportDest = pdcDst->GetViewportOrg();
      point ptViewportSrc = pdcSrc->GetViewportOrg();

      int32_t xvpDest = xDest + ptViewportDest.x;
      int32_t xvpSrc = xSrc + ptViewportSrc.x;

      int32_t iLimitX = cx;

      if(bmDest.bmWidth - xvpDest < iLimitX)
      {
         iLimitX = bmDest.bmWidth - xvpDest;
      }
      if(bmSrc.bmWidth - xvpSrc < iLimitX)
      {
         iLimitX = bmSrc.bmWidth - xvpSrc;
      }


      if(xvpDest < 0)
         return false;
      if(xvpSrc < 0)
         return false;

      int32_t iLimitY = MIN(iLimitYDest,iLimitYSrc);

      pixelate_24CC(
         lpbDst + xvpDest * 3,
         0,
         0,
         iwDest,
         iLimitX,
         iLimitY,
         lpbSrc + xvpSrc * 3,
         0,
         0,
         iwSrc,
         iSize);


#ifdef WINDOWSEX

      ::draw2d::bitmap * pbmpOld = pdcDst->get_current_bitmap();

      if(pbmpOld == NULL)
      {
         ::draw2d::graphics_sp graphicsMem(allocer());
         graphicsMem->CreateCompatibleDC(pdcDst);
         if(!SetDIBits(
            (HDC)graphicsMem->get_os_data(),
            (HBITMAP)bitmapDest->get_os_data(),
            uiStartScanLine,
            uiScanLineCount,
            lpbDst,
            &bmiDest,
            DIB_RGB_COLORS))
            throw 6000;
         ::draw2d::bitmap * pbmpMemOld = graphicsMem->SelectObject(bitmapDest);
         if(!pdcDst->BitBlt(xDest,yDest,cx,cy,graphicsMem,xSrc,ySrc,SRCCOPY))
         {
            ASSERT(FALSE);
         }
         graphicsMem->SelectObject(pbmpMemOld);
      }
      else
      {
         ::draw2d::bitmap_sp bitmap(get_app());
         bitmap->CreateCompatibleBitmap(pdcDst,1,1);
         //      ::draw2d::bitmap * pbitmap = pdcDst->SelectObject(bitmap);
         if(!SetDIBits(
            (HDC)pdcDst->get_os_data(),
            (HBITMAP)bitmapDest->get_os_data(),
            uiStartScanLine,
            uiScanLineCount,
            lpbDst,
            &bmiDest,
            DIB_RGB_COLORS))
         {
            pdcDst->SelectObject(bitmapDest);
            return false;
         }
         pdcDst->SelectObject(bitmapDest);
      }

      return true;

#else

      throw todo(get_app());

#endif

   }

   void imaging::pixelate_24CC(
      LPBYTE   lpbDst,
      int32_t      xDest,
      int32_t      yDest,
      int32_t      wDest,
      int32_t      cx,
      int32_t      cy,
      LPBYTE   lpbSrc,
      int32_t      xSrc,
      int32_t      ySrc,
      int32_t      wSrc,
      int32_t      iSize)
   {
      UNREFERENCED_PARAMETER(xDest);
      UNREFERENCED_PARAMETER(yDest);
      UNREFERENCED_PARAMETER(xSrc);
      UNREFERENCED_PARAMETER(ySrc);
      int32_t      divisor;
      BYTE *   lpbSource;
      BYTE *   lpwDestination;
      BYTE *   lpwDestination_1;
      BYTE *   lpwDestination_2;
      BYTE *   lpbSource_1;
      BYTE *   lpbSource_2;

      int32_t iFilterWidth = iSize;
      int32_t iFilterHeight = iSize;
      int32_t maxx1 = cx;
      int32_t maxy1 = cy;
      //   int32_t maxy2 = cy - iFilterWidth;
      //   int32_t maxy3 = cy - iFilterWidth / 2;
      int32_t max3x1 = maxx1 * 3;
      //   int32_t max3x2 = (maxx1 - iFilterHeight) * 3;
      //   int32_t max3x3 = (maxx1 - iFilterHeight / 2) * 3;
      //int32_t w = cx * 3;

      uint32_t dwR;
      uint32_t dwG;
      uint32_t dwB;

      int32_t iFilterXBegin;
      int32_t iFilterXEnd;
      int32_t iFilterYBegin;
      int32_t iFilterYEnd;

      int32_t yLowerBound[4];
      int32_t yUpperBound[4];
      int32_t xLowerBound[4];
      int32_t xUpperBound[4];

      // top
      yLowerBound[0] = 0;
      yUpperBound[0] = iFilterWidth / 2;
      xLowerBound[0] = 0;
      xUpperBound[0] = max3x1;

      // left
      yLowerBound[1] = iFilterWidth / 2;
      yUpperBound[1] = maxy1 - iFilterWidth / 2;
      xLowerBound[1] = 0;
      xUpperBound[1] = iFilterWidth / 2;

      // right
      yLowerBound[2] = iFilterWidth / 2;
      yUpperBound[2] = maxy1 - iFilterWidth / 2;
      xLowerBound[2] = max3x1 - iFilterWidth / 2;
      xUpperBound[2] = max3x1;

      // bottom
      yLowerBound[3] = maxy1 - iFilterWidth / 2;
      yUpperBound[3] = maxy1;
      xLowerBound[3] = 0;
      xUpperBound[3] = max3x1;

      /*   for(int32_t i = 0; i < 4; i++)
      {
      int32_t yL = yLowerBound[i];
      int32_t yU = yUpperBound[i];
      int32_t xL = xLowerBound[i];
      int32_t xU = xUpperBound[i];
      for(int32_t y1 = yL; y1 < yU; y1++)
      {
      if(y1 < iFilterWidth / 2)
      {
      iFilterYBegin = iFilterWidth / 2 - y1;
      }
      else
      {
      iFilterYBegin = 0;
      }
      if(y1 > maxy3)
      {
      iFilterYEnd = iFilterWidth - y1 - maxy3;
      }
      else
      {
      iFilterYEnd = iFilterWidth;
      }
      lpbSource = lpbSrc + (wSrc * MAX((y1 - iFilterWidth / 2), 0));
      lpwDestination = lpbDst + (wDest  * y1);
      for(int32_t x1 = xL; x1 < xU; x1+=3)
      {
      if(x1 < iFilterHeight /2)
      {
      iFilterXBegin = iFilterHeight / 2 - x1;
      }
      else
      {
      iFilterXBegin = 0;
      }
      if(x1 > max3x3)
      {
      iFilterXEnd = iFilterHeight  - x1 + max3x3;
      }
      else
      {
      iFilterXEnd = iFilterHeight;
      }
      lpbSource_1 = lpbSource + ((MAX(x1 - iFilterWidth / 2 * 3, 0)));
      lpFilter = pFilter;
      //            lpConv = pConv;

      dwR = 0;
      dwG = 0;
      dwB = 0;
      for(int32_t y2 = iFilterYBegin; y2 < iFilterYEnd; y2++)
      {
      lpbSource_2 = lpbSource_1 + (wSrc * y2);
      for(int32_t x2 = iFilterXBegin; x2 < iFilterXEnd; x2++)
      {
      dwR += *lpbSource_2++ * *lpFilter;
      dwG += *lpbSource_2++ * *lpFilter;
      dwB += *lpbSource_2++ * *lpFilter++;

      }
      }
      lpwDestination_1 = lpwDestination + x1;
      divisor = (iFilterYEnd - iFilterYBegin) * (iFilterXEnd - iFilterXBegin );
      if(divisor == 0)
      {
      lpwDestination_1[0] = MAX(0, MIN(dwR, 255));
      lpwDestination_1[1] = MAX(0, MIN(dwG, 255));
      lpwDestination_1[2] = MIN(0, MIN(dwB, 255));
      }
      else
      {
      lpwDestination_1[0] = dwR / divisor;
      lpwDestination_1[1] = dwG / divisor;
      lpwDestination_1[2] = dwB / divisor;
      }
      }
      }
      }*/

      iFilterYBegin  = 0;
      iFilterYEnd    = iFilterWidth;
      iFilterXBegin = 0;
      iFilterXEnd = iFilterHeight;

      int32_t iFilterHalfWidth = iFilterWidth / 2;
      int32_t iFilterHalfWidthBytes = iFilterHalfWidth * 3;

      int32_t yL = iFilterHalfWidth;
      int32_t yU = maxy1 - iFilterHalfWidth;
      int32_t xL = iFilterHalfWidthBytes;
      int32_t xU = max3x1 - iFilterHalfWidthBytes;


      int32_t y1 = yL;
      int32_t y2 = yL - iFilterHalfWidth;
      int32_t x1;
      int32_t x2;

      divisor = (iFilterYEnd - iFilterYBegin)
         * (iFilterXEnd - iFilterXBegin);


      int32_t cDest   = wDest - (iFilterWidth * 3);
      int32_t cSrc    = wSrc - (iFilterWidth * 3);
      int32_t iSize3  = iSize * 3;

      int32_t cyDest  = wDest * iSize;
      int32_t cySrc   = wSrc  * iSize;



      lpbSource         = lpbSrc;
      lpwDestination    = lpbDst;
      for(; y1 < yU;)
      {
         x1 = xL;
         x2 = xL - iFilterHalfWidthBytes;
         for(; x1 < xU;)
         {
            lpbSource_1 = lpbSource + x2;

            dwR = 0;
            dwG = 0;
            dwB = 0;
            lpbSource_2 = lpbSource_1;
            int32_t yFilter;
            for(yFilter = iFilterYBegin; yFilter < iFilterYEnd; yFilter++)
            {
               for(int32_t xFilter = iFilterXBegin; xFilter < iFilterXEnd; xFilter++)
               {
                  dwR += *lpbSource_2++;
                  dwG += *lpbSource_2++;
                  dwB += *lpbSource_2++;
               }
               lpbSource_2 += cSrc;
            }

            dwR /= divisor;
            dwG /= divisor;
            dwB /= divisor;

            lpwDestination_1 = lpwDestination + x2;
            lpwDestination_2 = lpwDestination_1;
            for(yFilter = 0; yFilter < iFilterYEnd; yFilter++)
            {
               for(int32_t xFilter = 0; xFilter < iFilterXEnd; xFilter++)
               {
                  lpwDestination_2[0] = (BYTE)dwR;
                  lpwDestination_2[1] = (BYTE)dwG;
                  lpwDestination_2[2] = (BYTE)dwB;
                  lpwDestination_2 += 3;
               }
               lpwDestination_2 += cDest;
            }


            x1 += iSize3;
            x2 += iSize3;
         }
         y1 += iSize;
         y2 += iSize;
         lpbSource         += cySrc;
         lpwDestination    += cyDest;
      }

   }

   bool imaging::alpha_pixelate(
      ::draw2d::graphics *pdcDst,
      int32_t xDest,
      int32_t yDest,
      int32_t cx,
      int32_t cy,
      ::draw2d::graphics * pdcSrc1,
      int32_t xSrc1,
      int32_t ySrc1,
      ::draw2d::graphics * pdcSrc2,
      int32_t xSrc2,
      int32_t ySrc2,
      int32_t iSize,
      int32_t iAlpha)
   {
      if(cx <= 0 || cy <= 0)
         return true;

      memory memstorageA;
      memory memstorageB;
      memory memstorageC;


      UINT user;
      UINT uiStartScanLine;
      UINT uiScanLineCount;


      rect rectDest;
      rectDest.left = xDest;
      rectDest.right = xDest + cx;
      rectDest.top = yDest;
      rectDest.bottom = yDest + cy;

      rect rectSrc1;
      rectSrc1.left = xSrc1;
      rectSrc1.right = xSrc1 + cx;
      rectSrc1.top = ySrc1;
      rectSrc1.bottom = ySrc1 + cy;

      rect rectSrc2;
      rectSrc2.left = xSrc2;
      rectSrc2.right = xSrc2 + cx;
      rectSrc2.top = ySrc2;
      rectSrc2.bottom = ySrc2 + cy;

      int32_t iwDest;

      BITMAPINFO bmiDest;
      BITMAP   bmDest;
      int32_t iLimitYDest;

      ::draw2d::bitmap_sp bitmapDest(get_app());

      if(!GetDeviceContext24BitsCC(
         pdcDst,
         bmDest,
         bmiDest,
         memstorageA,
         bitmapDest,
         rectDest,
         iwDest,
         uiStartScanLine,
         uiScanLineCount,
         iLimitYDest))
      {
         return false;
      }

      LPBYTE lpbDst = memstorageA.get_data();


      int32_t iwSrc1;
      int32_t iwSrc2;

      BITMAPINFO bmiSrc1;
      BITMAP   bmSrc1;
      int32_t iLimitYSrc1;

      ::draw2d::bitmap_sp bitmapSrc1(get_app());

      if(!GetDeviceContext24BitsCC(
         pdcSrc1,bmSrc1,bmiSrc1,
         memstorageB,
         bitmapSrc1,
         rectSrc1,
         iwSrc1,
         user,
         user,
         iLimitYSrc1))
      {
         return false;
      }

      LPBYTE lpbSrc1 = memstorageB.get_data();

      BITMAPINFO bmiSrc2;
      BITMAP   bmSrc2;
      int32_t iLimitYSrc2;

      ::draw2d::bitmap_sp bitmapSrc2(get_app());

      if(!GetDeviceContext24BitsCC(
         pdcSrc2,bmSrc2,bmiSrc2,
         memstorageC,
         bitmapSrc2,
         rectSrc2,
         iwSrc2,
         user,
         user,
         iLimitYSrc2))
      {
         return false;
      }

      LPBYTE lpbSrc2 = memstorageC.get_data();

      point ptViewportDest = pdcDst->GetViewportOrg();
      point ptViewportSrc1 = pdcSrc1->GetViewportOrg();
      point ptViewportSrc2 = pdcSrc2->GetViewportOrg();

      int32_t xvpDest = xDest + ptViewportDest.x;
      int32_t xvpSrc1 = xSrc1 + ptViewportSrc1.x;
      int32_t xvpSrc2 = xSrc2 + ptViewportSrc2.x;

      int32_t iLimitX = cx;

      if(bmDest.bmWidth - xvpDest < iLimitX)
      {
         iLimitX = bmDest.bmWidth - xvpDest;
      }
      if(bmSrc1.bmWidth - xvpSrc1 < iLimitX)
      {
         iLimitX = bmSrc1.bmWidth - xvpSrc1;
      }
      if(bmSrc2.bmWidth - xvpSrc2 < iLimitX)
      {
         iLimitX = bmSrc2.bmWidth - xvpSrc2;
      }


      if(xvpDest < 0)
         return false;
      if(xvpSrc1 < 0)
         return false;
      if(xvpSrc2 < 0)
         return false;

      int32_t iLimitY = MIN(MIN(iLimitYDest,iLimitYSrc1),iLimitYSrc2);

      alpha_pixelate_24CC(
         lpbDst + xvpDest * 3,
         0,
         0,
         iwDest,
         iLimitX,
         iLimitY,
         lpbSrc1 + xvpSrc1 * 3,
         0,
         0,
         iwSrc1,
         lpbSrc2 + xvpSrc2 * 3,
         0,
         0,
         iwSrc2,
         iSize,
         iAlpha);

#ifdef WINDOWSEX

      ::draw2d::bitmap * pbmpOld = pdcDst->get_current_bitmap();

      if(pbmpOld == NULL)
      {
         ::draw2d::graphics_sp graphicsMem(allocer());
         graphicsMem->CreateCompatibleDC(pdcDst);
         if(!SetDIBits(
            (HDC)graphicsMem->get_os_data(),
            (HBITMAP)bitmapDest->get_os_data(),
            uiStartScanLine,
            uiScanLineCount,
            lpbDst,
            &bmiDest,
            DIB_RGB_COLORS))
            throw 6000;
         ::draw2d::bitmap * pbmpMemOld = graphicsMem->SelectObject(bitmapDest);
         if(!pdcDst->BitBlt(xDest,yDest,cx,cy,graphicsMem,xDest,yDest,SRCCOPY))
         {
            ASSERT(FALSE);
         }
         graphicsMem->SelectObject(pbmpMemOld);
      }
      else
      {
         ::draw2d::bitmap_sp bitmap(get_app());
         bitmap->CreateCompatibleBitmap(pdcDst,1,1);
         //      ::draw2d::bitmap * pbitmap = pdcDst->SelectObject(bitmap);
         if(!SetDIBits(
            (HDC)pdcDst->get_os_data(),
            (HBITMAP)bitmapDest->get_os_data(),
            uiStartScanLine,
            uiScanLineCount,
            lpbDst,
            &bmiDest,
            DIB_RGB_COLORS))
         {
            pdcDst->SelectObject(bitmapDest);
            return false;
         }
         pdcDst->SelectObject(bitmapDest);
      }

      return true;

#else

      throw todo(get_app());

#endif

   }

   void imaging::alpha_pixelate_24CC(
      LPBYTE lpbDst,
      int32_t xDest,
      int32_t yDest,
      int32_t wDest,
      int32_t cx,
      int32_t cy,
      LPBYTE lpbSrc1,
      int32_t xSrc1,
      int32_t ySrc1,
      int32_t wSrc1,
      LPBYTE lpbSrc2,
      int32_t xSrc2,
      int32_t ySrc2,
      int32_t wSrc2,
      int32_t iSize,
      int32_t iAlpha)
   {
      UNREFERENCED_PARAMETER(xDest);
      UNREFERENCED_PARAMETER(yDest);
      UNREFERENCED_PARAMETER(xSrc1);
      UNREFERENCED_PARAMETER(ySrc1);
      UNREFERENCED_PARAMETER(xSrc2);
      UNREFERENCED_PARAMETER(ySrc2);
      //   const int32_t constFilterWidth = 3;
      //   const int32_t constFilterHeight = 3;
      int32_t divisor = iSize * iSize;
      BYTE *lpbSource1;
      BYTE *lpbSource2;
      BYTE *lpwDestination;
      BYTE *lpwDestination_1;
      BYTE *lpwDestination_2;
      BYTE *lpbSource1_1;
      BYTE *lpbSource1_2;
      BYTE *lpbSource2_1;
      BYTE *lpbSource2_2;
      //   WORD *lpwDestination_2;
      //   BYTE *lpFilter;
      //uint32_t *lpConv;
      //   BYTE pFilter[constFilterHeight *constFilterWidth];
      //uint32_t pConv[constFilterHeight * constFilterWidth * 3];
      //   memset(pFilter, 1, constFilterHeight * constFilterWidth);

      int32_t iFilterWidth = iSize;
      int32_t iFilterHeight = iSize;
      int32_t maxx1 = cx;
      int32_t maxy1 = cy;
      //   int32_t maxy2 = cy - iFilterWidth;
      //   int32_t maxy3 = cy - iFilterWidth / 2;
      int32_t max3x1 = maxx1 * 3;
      //   int32_t max3x2 = (maxx1 - iFilterHeight) * 3;
      //   int32_t max3x3 = (maxx1 - iFilterHeight / 2) * 3;
      //int32_t bm3Width = (cy * 3 + 3) & ~0x3L;
      //   int32_t w = cx * 3;
      //   memcpy(m_lpwDestination, m_lpbSource, bmWidth * bmHeight * 3);
      //   memcpy(m_lpwDestination, m_lpbSource, bm3Width * bmHeight);
      /*for(int32_t y = 0; y < cy; y++)
      {
      lpbSource = lpbSrc + wSrc * y;
      lpwDestination = lpbDst + wDest * y;
      for(int32_t x = 0; x < w; x++)
      {
      *lpwDestination = *lpbSource;
      lpwDestination++;
      lpbSource++;
      }
      }*/
      uint32_t dwR1;
      uint32_t dwG1;
      uint32_t dwB1;
      uint32_t dwR2;
      uint32_t dwG2;
      uint32_t dwB2;

      int32_t iFilterXBegin;
      int32_t iFilterXEnd;
      int32_t iFilterYBegin;
      int32_t iFilterYEnd;

      int32_t yLowerBound[4];
      int32_t yUpperBound[4];
      int32_t xLowerBound[4];
      int32_t xUpperBound[4];

      // top
      yLowerBound[0] = 0;
      yUpperBound[0] = iFilterWidth / 2;
      xLowerBound[0] = 0;
      xUpperBound[0] = max3x1;

      // left
      yLowerBound[1] = iFilterWidth / 2;
      yUpperBound[1] = maxy1 - iFilterWidth / 2;
      xLowerBound[1] = 0;
      xUpperBound[1] = iFilterWidth / 2;

      // right
      yLowerBound[2] = iFilterWidth / 2;
      yUpperBound[2] = maxy1 - iFilterWidth / 2;
      xLowerBound[2] = max3x1 - iFilterWidth / 2;
      xUpperBound[2] = max3x1;

      // bottom
      yLowerBound[3] = maxy1 - iFilterWidth / 2;
      yUpperBound[3] = maxy1;
      xLowerBound[3] = 0;
      xUpperBound[3] = max3x1;

      /*   for(int32_t i = 0; i < 4; i++)
      {
      int32_t yL = yLowerBound[i];
      int32_t yU = yUpperBound[i];
      int32_t xL = xLowerBound[i];
      int32_t xU = xUpperBound[i];
      for(int32_t y1 = yL; y1 < yU; y1++)
      {
      if(y1 < iFilterWidth / 2)
      {
      iFilterYBegin = iFilterWidth / 2 - y1;
      }
      else
      {
      iFilterYBegin = 0;
      }
      if(y1 > maxy3)
      {
      iFilterYEnd = iFilterWidth - y1 - maxy3;
      }
      else
      {
      iFilterYEnd = iFilterWidth;
      }
      lpbSource = lpbSrc + (wSrc * MAX((y1 - iFilterWidth / 2), 0));
      lpwDestination = lpbDst + (wDest  * y1);
      for(int32_t x1 = xL; x1 < xU; x1+=3)
      {
      if(x1 < iFilterHeight /2)
      {
      iFilterXBegin = iFilterHeight / 2 - x1;
      }
      else
      {
      iFilterXBegin = 0;
      }
      if(x1 > max3x3)
      {
      iFilterXEnd = iFilterHeight  - x1 + max3x3;
      }
      else
      {
      iFilterXEnd = iFilterHeight;
      }
      lpbSource_1 = lpbSource + ((MAX(x1 - iFilterWidth / 2 * 3, 0)));
      lpFilter = pFilter;
      //            lpConv = pConv;

      dwR = 0;
      dwG = 0;
      dwB = 0;
      for(int32_t y2 = iFilterYBegin; y2 < iFilterYEnd; y2++)
      {
      lpbSource_2 = lpbSource_1 + (wSrc * y2);
      for(int32_t x2 = iFilterXBegin; x2 < iFilterXEnd; x2++)
      {
      dwR += *lpbSource_2++ * *lpFilter;
      dwG += *lpbSource_2++ * *lpFilter;
      dwB += *lpbSource_2++ * *lpFilter++;

      }
      }
      lpwDestination_1 = lpwDestination + x1;
      divisor = (iFilterYEnd - iFilterYBegin) * (iFilterXEnd - iFilterXBegin );
      if(divisor == 0)
      {
      lpwDestination_1[0] = MAX(0, MIN(dwR, 255));
      lpwDestination_1[1] = MAX(0, MIN(dwG, 255));
      lpwDestination_1[2] = MIN(0, MIN(dwB, 255));
      }
      else
      {
      lpwDestination_1[0] = dwR / divisor;
      lpwDestination_1[1] = dwG / divisor;
      lpwDestination_1[2] = dwB / divisor;
      }
      }
      }
      }*/

      iFilterYBegin  = 0;
      iFilterYEnd    = iFilterWidth;
      iFilterXBegin = 0;
      iFilterXEnd = iFilterHeight;

      int32_t iFilterHalfWidth = iFilterWidth / 2;
      int32_t iFilterHalfWidthBytes = iFilterHalfWidth * 3;

      int32_t yL = iFilterHalfWidth;
      int32_t yU = maxy1 - iFilterHalfWidth;
      int32_t xL = iFilterHalfWidthBytes;
      int32_t xU = max3x1 - iFilterHalfWidthBytes;


      int32_t y1 = yL;
      int32_t y2 = yL - iFilterHalfWidth;
      int32_t x1;
      int32_t x2;

      divisor = 255 * (iFilterYEnd - iFilterYBegin) * (iFilterXEnd - iFilterXBegin);

      int32_t iRate1 = iAlpha;
      int32_t iRate2 = 255 - iAlpha;

      uint32_t dwR;
      uint32_t dwG;
      uint32_t dwB;

      int32_t cFilter  = wDest - iFilterWidth * 3;
      int32_t cFilter1 = wSrc1 - iFilterWidth * 3;
      int32_t cFilter2 = wSrc2 - iFilterWidth * 3;

      int32_t iSize3 = iSize * 3;

      for(; y1 < yU;)
      {
         lpbSource1     = lpbSrc1 + (wSrc1 * y2);
         lpbSource2     = lpbSrc2 + (wSrc2 * y2);
         lpwDestination = lpbDst + (wDest * y2);

         x1 = xL;
         x2 = xL - iFilterHalfWidthBytes;

         for(; x1 < xU;)
         {
            lpbSource1_1 = lpbSource1 + x2;
            lpbSource2_1 = lpbSource2 + x2;


            dwR1 = 0;
            dwG1 = 0;
            dwB1 = 0;
            dwR2 = 0;
            dwG2 = 0;
            dwB2 = 0;
            lpbSource1_2 = lpbSource1_1;
            lpbSource2_2 = lpbSource2_1;
            int32_t yFilter;
            for(yFilter = 0; yFilter < iFilterYEnd; yFilter++)
            {
               for(int32_t xFilter = 0; xFilter < iFilterXEnd; xFilter++)
               {
                  dwR1 += *lpbSource1_2++;
                  dwG1 += *lpbSource1_2++;
                  dwB1 += *lpbSource1_2++;
                  dwR2 += *lpbSource2_2++;
                  dwG2 += *lpbSource2_2++;
                  dwB2 += *lpbSource2_2++;
               }
               lpbSource1_2 += cFilter1;
               lpbSource2_2 += cFilter2;
            }

            dwR = (dwR1 * iRate1 + dwR2 * iRate2) / divisor;
            dwG = (dwG1 * iRate1 + dwG2 * iRate2) / divisor;
            dwB = (dwB1 * iRate1 + dwB2 * iRate2) / divisor;

            lpwDestination_1 = lpwDestination + x2;
            lpwDestination_2 = lpwDestination_1;
            for(yFilter = 0; yFilter < iFilterYEnd; yFilter++)
            {
               for(int32_t xFilter = 0; xFilter < iFilterXEnd; xFilter++)
               {
                  lpwDestination_2[0] = (BYTE)dwR;
                  lpwDestination_2[1] = (BYTE)dwG;
                  lpwDestination_2[2] = (BYTE)dwB;
                  lpwDestination_2 += 3;
               }
               lpwDestination_2 += cFilter;
            }


            x1 += iSize3;
            x2 += iSize3;
         }
         y1 += iSize;
         y2 += iSize;
      }

   }



   // dCompress de 0 a 1
   bool imaging::HueVRCP(::draw2d::dib * pdib,COLORREF crHue,double dCompress)
   {


      pdib->map();

      if(pdib->get_data() == NULL)
      {
         pdib->unmap();
         return false;
      }

      COLORREF cra[256];

      color color;
      for(int32_t i = 0; i < 256; i++)
      {
         color.set_rgb(crHue);
         color.hls_mult(1.0,(i / 255.0) * (1.0 - dCompress) + dCompress,1.0);
         cra[i] = color.get_bgr();
      }


      LPBYTE lpb = (LPBYTE)pdib->get_data();


      int64_t area = pdib->area();

      for(int64_t i = 0; i < area; i++)
      {
         *((uint32_t *)lpb) = (cra[(lpb[0] + lpb[1] + lpb[2]) / 3]) | lpb[3] << 24;
         lpb += 4;
      }

      pdib->unmap();

      return true;

   }


   void imaging::AlphaTextOut(::draw2d::graphics *pgraphics,int32_t left,int32_t top,const char * lpcsz,int32_t len,COLORREF cr,double dBlend)
   {
      string str(lpcsz,len);
      if(dBlend <= 0.0)
         return;
      ::draw2d::brush_sp brushText(allocer());
      if(dBlend >= 1.0)
      {
         brushText->create_solid(cr);
         pgraphics->SelectObject(brushText);
         pgraphics->text_out(left,top,str);
         return;
      }
      brushText->create_solid(ARGB((BYTE)(255 * dBlend),argb_get_r_value(cr),argb_get_g_value(cr),argb_get_b_value(cr)));
      pgraphics->SelectObject(brushText);
      pgraphics->text_out(left,top,str);
   }

   bool imaging::load_from_file(::visual::dib_sp::array * pdiba, var varFile, bool bCache)
   {

      single_lock sl(&m_mutex);

      //// image cache load
      //// cache of decompression time
      //string strFile;
      //if (bCache && varFile.get_type() == var::type_string)
      //   //if(false)
      //{
      //   strFile = varFile;
      //   strFile.replace(":/", "\\_");
      //   strFile.replace(":\\", "\\_\\");
      //   strFile.replace("/", "\\");
      //   strFile = System.dir().time() / "cache" / strFile;
      //   strFile += ".dib";
      //   if (Sess(papp).file().exists(strFile))
      //   {
      //      try
      //      {
      //         ::file::file_sp file = Sess(papp).file().get_file(strFile, ::file::mode_read | ::file::share_deny_write | ::file::type_binary);
      //         if (file.is_null())
      //         {
      //            return true;
      //         }
      //         ::file::byte_istream istream(file);
      //         istream >> *pdib;
      //         if (!istream.fail())
      //         {
      //            return true;
      //         }
      //      }
      //      catch (...)
      //      {
      //      }
      //   }
      //}


      try
      {

         if (!LoadImage(pdiba, varFile))
         {
            
            return false;
            
         }

      }
      catch (...)
      {

         return false;

      }


      //// image cache write
      //if (bCache && strFile.has_char())
      //{
      //   try
      //   {

      //      ::file::file_sp file = App(papp).file().get_file(strFile, ::file::mode_create | ::file::mode_write | ::file::type_binary | ::file::defer_create_directory);

      //      if (file.is_set())
      //      {

      //         ::file::byte_ostream ostream(file);

      //         ostream << *pdib;

      //      }

      //   }
      //   catch (...)
      //   {
      //   }
      //}

      return true;

   }


   bool imaging::load_from_file(::draw2d::dib * pdib,var varFile,bool bCache)
   {

      single_lock sl(&m_mutex);

      // image cache load
      // cache of decompression time
      string strFile;
      if(bCache && varFile.get_type() == var::type_string)
         //if(false)
      {
         strFile = varFile;
         strFile.replace(":/","\\_");
         strFile.replace(":\\","\\_\\");
         strFile.replace("/","\\");
         strFile = System.dir().time()/"cache"/strFile;
         strFile += ".dib";
         if(Session.file().exists(strFile))
         {
            try
            {
               ::file::file_sp file = Session.file().get_file(strFile,::file::mode_read | ::file::share_deny_write | ::file::type_binary);
               if(file.is_null())
               {
                  return true;
               }
               ::file::byte_istream istream(file);
               istream >> *pdib;
               if(!istream.fail())
               {
                  return true;
               }
            }
            catch(...)
            {
            }
         }
      }


      try
      {
         
         
         if(!LoadImage(pdib,varFile))
         {
            
            return false;
            
         }

      }
      catch(...)
      {

         return false;

      }


      // image cache write
      if(bCache && strFile.has_char())
      {
         try
         {

            ::file::file_sp file =Application.file().get_file(strFile,::file::mode_create | ::file::mode_write | ::file::type_binary | ::file::defer_create_directory);

            if(file.is_set())
            {

               ::file::byte_ostream ostream(file);

               ostream << *pdib;

            }

         }
         catch(...)
         {
         }
      }

      return true;

   }








   bool imaging::load_from_file(::visual::cursor * pcursor,var varFile, bool bFromCache)
   {
      
      string str(varFile);
      
      if(!::str::ends_eat_ci(str,".png"))
      {
         
         return false;
         
      }
      
      if(!load_from_file(pcursor->m_dib,varFile,bFromCache))
      {
         
         return false;
         
      }

      str += ".xml";
      
      string strNode = Session.file().as_string(str);
      
      ::xml::document doc(get_app());
      
      if(doc.load(strNode))
      {
         
         pcursor->m_szHotspotOffset.cx = doc.get_root()->attr("x");
         
         pcursor->m_szHotspotOffset.cy = doc.get_root()->attr("y");
         
      }
      
      return true;
      
   }


